#include "core0.h"

Core0State::Core0State(SharedState* s, I2S* i) {

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    activeBuffers[i] = 0;
    currPointers[i] = 0;
    mostRecentReadySector[i] = 0;
  }

  sharedState = s;
  i2s = i;

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

  uint32_t availableMsgs = numQueuedMsgs();

  for (int i = 0; i < availableMsgs; i++) {

    Message m = popMsg();

    if (isStop(m)) {

      // stop all relevant buffers, and then ack this msg
      for (int b = 0; b <  MAX_CONCURRENT_SOUNDS; b++) {
        if (stopMsgContainsBuf(m, b)) {
          activeBuffers[b] = false;
        }
      }

      int r = pushMsg(m);

      if (r == false) {
        Serial.println("ERROR: Cannot push stop msg ack.");
      }
    } else if (isReady(msg)) {
      uint8_t buffer = readyMsgGetBuf(m);
      uint32_t sectorId = readyMsgGetSector(m);

      if (!((mostRecentReadySector[buffer] == sectorId) || (mostRecentReadySector[buffer]+1 == sectorId))) {
        Serial.println("ERROR: Unexpected sector id");
      }

      mostRecentReadySector[buffer] = sectorId;
      // we send an ack in proceedRead, when we start actually reading from this sector

    } else {

      Serial.println("Unrecognized msg");
    }

  }
}

int16_t Core0State::readBuffers() {

  int32_t sum = 0;
  // play sounds from readySounds
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (proceedToRead(i)) {
      //uint32_t offset = currPointers[i] % DOUBLE_BUFFER_SIZE;
      uint32_t rawSampleValue = ((sharedState->buffers)[i]).readSample(currPointers[i]);

      sum += (int32_t) ((int16_t) rawSampleValue);

      // increment pointers
      currPointers[i] += 2 // increment by 2 bytes ie a 16 bit sample
    }
  }

  int16_t sampleAdjusted = ((int16_t) max(-32766, min(32766, sum)))/2

  return sampleAdjusted;
}


void sendI2S(int16_t sample) {
  i2s->write(sample);
  i2s->write(sample);
}


bool Core0State::proceedToRead(uint8_t buffer) {
  using namespace msg;

  uint32_t currPointer = currPointers[buffer]
  uint32_t currentSector = currPointer / SINGLE_BUFFER_SIZE;
  uint32_t readySector = mostRecentReadySector[buffer];

  if (!activeBuffers[buffer]) {
    return false;
  } else if (readySector == currentSector+1) {

    if (currPointer % SINGLE_BUFFER_SIZE) { // we have started to read from a new buffer

      Message m = sectorReadyMsg(buffer, readySector);

      int r = pushMsg(m);

      if (r == false) {
        Serial.println("ERROR: Cannot push ready msg ack.");
      }
    }

    return true;
  } else if (readySector == currentSector) {
    return currPointer < (SINGLE_BUFFER_SIZE*(currentSector+1))
  } else {
    Serial.println("Problem with proceed to read");
  }

  return false;
}