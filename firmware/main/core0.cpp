#include "core0.h"

Core0State::Core0State(SharedState* s, I2S* i) {
  sharedState = s;
  i2s = i;
}

void Core0State::setup() {
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    resetBuffer(i);
  }
}

void Core0State::loop() {

  // check for any new messages
  handleInboundMsgs();

  // poll available buffers and send to I2S driver
  int16_t sample = readBuffers();
  sendI2S(sample);
}

void Core0State::handleInboundMsgs() {
  using namespace msg;

  uint32_t availableMsgs = sharedState->availableMessagesCore0();


  for (int i = 0; i < availableMsgs; i++) {

    Message m = sharedState->popMsgCore0();

    if (isStop(m)) {
      Serial.print("core0 - receiving stop message: ");
      Serial.println(m, BIN);

      // stop all relevant buffers, and then ack this msg
      for (int b = 0; b <  MAX_CONCURRENT_SOUNDS; b++) {
        if (stopMsgContainsBuf(m, b)) {

          Serial.print("core0 - stopping core: ");
          Serial.println(b);

          resetBuffer(b);
        }
      }

      Serial.println("core0 - sending ack to stop message");
      bool r = sharedState->sendMsgToCore1(m);

      if (r == false) {
        Serial.println("core0 - ERROR: Cannot push stop msg ack.");
      }

    } else if (isReady(m)) {
      uint8_t buffer = readyMsgGetBuf(m);
      uint32_t sectorId = readyMsgGetSector(m);

      if (sectorId == 0) {
        activeBuffers[buffer] = true; // set to active buffer if not so yet
        currPointers[buffer] = 0;
      }

      Serial.print("core0 - receiving ready for buffer/sector: ");
      Serial.print(buffer);
      Serial.print("/");
      Serial.println(sectorId);

      if (!((mostRecentReadySector[buffer] == sectorId) || (mostRecentReadySector[buffer]+1 == sectorId))) {
        Serial.print("core0 - ERROR: Unexpected sector id, buffer/sector: ");
        Serial.print(buffer);
        Serial.print("/");
        Serial.println(sectorId);
        Serial.print("most recent ready: ");
        Serial.println(mostRecentReadySector[buffer]);

        //while (true) {;}
        // TODO
        continue;
      }

      mostRecentReadySector[buffer] = sectorId;
      // we send an ack in proceedRead, when we start actually reading from this sector

    } else {
      Serial.println("core0 - Unrecognized msg");
    }

  }
}

int16_t Core0State::readBuffers() {

  int32_t sum = 0;
  // play sounds from readySounds
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (proceedToRead(i)) {
      uint32_t offset = currPointers[i] % DOUBLE_BUFFER_SIZE;
      uint32_t rawSampleValue = ((sharedState->buffers)[i]).readSample(offset);


      sum += (int32_t) ((int16_t) rawSampleValue);

      // increment pointers
      currPointers[i] += 2; // increment by 2 bytes ie a 16 bit sample
    }
  }

  int16_t sampleAdjusted = ((int16_t) max(-32766, min(32766, sum))) / 2 ;

  return sampleAdjusted;
}


void Core0State::sendI2S(int16_t sample) {
  //Serial.print("core0 - Sending i2s. Sample: ");
  //Serial.println(sample);
  i2s->write(sample);
  i2s->write(sample);
}

// TODO remove stateful operations/messaging from this function
bool Core0State::proceedToRead(uint8_t buffer) {
  using namespace msg;

  uint32_t currPointer = currPointers[buffer];
  uint32_t currentSector = currPointer / SINGLE_BUFFER_SIZE;
  uint32_t readySector = mostRecentReadySector[buffer];
  uint32_t maxSectors = ((sharedState->buffers)[buffer]).getNumSectors();

  // Buffer is not active
  if (!activeBuffers[buffer]) {
    return false;
  }

  // Sound has completed, but we have not set this buffer to inactive yet
  if (currentSector >= maxSectors) {
    activeBuffers[buffer] = false;
    // send message done message to

    Message m = doneMsg(buffer);
    bool r = sharedState->sendMsgToCore1(m);


    //Serial.print("core0 - Sending done message for buffer/currentSector: ");
    //Serial.print(buffer);
    //Serial.print("/");
    //Serial.println(currentSector);

    if (r == false) {
      Serial.println("ERROR: Cannot push ready msg ack.");
    }

    return false;
  }

  // First core has not completed writing the next sector
  if (currentSector > readySector) {
    return false;
  }


  if (readySector == currentSector + 1) {
      return true;
  }

  if (readySector == currentSector) {

    if (currPointer % SINGLE_BUFFER_SIZE == 0) { // we have started to read from a new buffer

      Message m = sectorReadyMsg(buffer, readySector);

      Serial.print("core0 - Sending message to signal starting to read from a buffer/sector: ");
      Serial.print(buffer);
      Serial.print("/");
      Serial.println(readySector);

      bool r = sharedState->sendMsgToCore1(m);

      if (r == false) {
        Serial.println("ERROR: Cannot push ready msg ack.");
      }
    }

    return true;

  } else {
    Serial.println("Problem with proceed to read. Dumping...");

    Serial.print("currPointer: ");
    Serial.println(currPointer);

    Serial.print("currentSector: ");
    Serial.println(currentSector);

    Serial.print("readySector: ");
    Serial.println(readySector);

    while (true) {;}

  }

  return false;
}


void Core0State::resetBuffer(uint8_t buffer) {
  activeBuffers[buffer] = false;
  currPointers[buffer] = 0;
  mostRecentReadySector[buffer] = 0;
}
