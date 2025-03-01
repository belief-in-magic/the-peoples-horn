#include "core1.h"
#include "sharedState.h"


Core1State::Core1State(SharedState* ss) {
  sharedState = ss;

  isMuted = false;
}

void Core1State::setup(Adafruit_MCP23X17* mcp) {

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    highestAckedSector[i] = 0;
    prepareNext[i] = 0;
  }

  inputState.setMcp(mcp);

  setUpSD();
}

void Core1State::loop() {
  using namespace msg;

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    handleInboundMsgs();


    if (inputState.isMuted() && !isMuted) {
      Serial.println("Muting");
      isMuted = true;

      // send message to stop all active buffers
      Message stopMsg = stopMsgEmpty();

      for (int b = 0; b < MAX_CONCURRENT_SOUNDS; b++) {
        prepareNext[b] = 0; // set to null sound
        stopMsg = stopMsgWithBuf(stopMsg, b);
      }

      if (!(sharedState->sendMsgToCore0(stopMsg))) {
        Serial.println("core1 - ERROR cannot push trigger MUTE stop message");
      }

    }

    if (!inputState.isMuted() && isMuted) {
      Serial.println("Unmuting");
      isMuted = false;

      // flush any messages
    }

    std::optional<uint32_t> soundToPlay = inputState.getNextSound();

    if (soundToPlay && !isMuted) {

      uint8_t bufToPlay = soundPolicy.evictBuffer(*soundToPlay);

      Serial.print("core1 - Evicting to play: ");
      Serial.println(bufToPlay);

      triggerSound(bufToPlay, *soundToPlay);
    }

    if (isMuted) continue;

    Buf* currBufPtr = ((sharedState->buffers) + i);

    // if the next sector is ready, AND we are not currently waiting for the ack to stop a buffer
    if (prepareNext[i] == 0 && currBufPtr->isNextSectorReady()) {

      uint32_t newSector = currBufPtr->prepareNextSector();

      if (newSector == -1) {
        Serial.print("core1 - Could not prepare buffer: ");
        Serial.println(i);

        while(true) {;}
      }

      Serial.print("core1 - Sending sector ready for buffer/sector: ");
      Serial.print(i);
      Serial.print("/");
      Serial.println(newSector);

      // send ready message to tell the first core that this sector, for this core, is ready
      Message readyMsg = sectorReadyMsg(i, newSector);

      if (!(sharedState->sendMsgToCore0(readyMsg))) {
        Serial.println("core1 - ERROR: Cannot push ready msg");
      }

    }

  }
}

void Core1State::handleInboundMsgs() {
  using namespace msg;

  uint32_t availableMsgs = sharedState->availableMessagesCore1();

  for (int i = 0; i < availableMsgs; i++) {
    Message m = sharedState->popMsgCore1();

    if (isStop(m)) {
      // received ack for stopping some buffers, this means that we can start updating them
      for (int b = 0; b < MAX_CONCURRENT_SOUNDS; b++) {
        if (stopMsgContainsBuf(m, b) && (prepareNext[b] != 0)) {

          Serial.print("core1 - receiving stop command (ack) for buffer: ");
          Serial.println(b);

          
          // update buffer b with a new sound
          bool fileStatus = ((sharedState->buffers)[b]).newSource(prepareNext[b]);             
          if (!fileStatus) {
              // silently ignore?
              continue;
          }

          prepareNext[b] = 0; // set to null sound
        }
      }
    } else if (isReady(m)) {

      // received ready msg from the first core, for a certain sector. This means that that core has begun
      // reading from that sector. We can now start updating the next sector for that buffer.
      uint8_t readyBuffer = readyMsgGetBuf(m);
      uint32_t sector = readyMsgGetSector(m);

      Serial.print("core1 - receiving ready (ack) for buffer/sector: ");
      Serial.print(readyBuffer);
      Serial.print("/");
      Serial.println(sector);

      if (readyBuffer >= MAX_CONCURRENT_SOUNDS) {
        Serial.println("core1 - Error: received buffer for ready msg is greater than MAX_CONCURRENT_SOUNDS");
        continue;
      }

      Buf* bufPtr = ((sharedState->buffers) + readyBuffer);
      bufPtr->markNextSectorReady(); // mark the sector as ready to be written to
    } else if (isDone(m)) {

      //Serial.print("core1 - Receiving is done message: ");
      //Serial.println(m, BIN);

      uint8_t finishedBuffer = doneMsgGetBuf(m);

      soundPolicy.setComplete(finishedBuffer);

    } else {
      Serial.println("core1 - Unrecognized msg");
    }

  }
}

void Core1State::setUpSD() {
    Serial.println("Initializing SD card...");

    SD_SPI_IF.setSCK(SD_SPI_SCK);
    SD_SPI_IF.setRX(SD_SPI_MISO);
    SD_SPI_IF.setTX(SD_SPI_MOSI);

    if (!SD.begin(SD_SPI_CS, SD_SPI_IF)) {
        Serial.println("initialization failed!");
        return;
    }

    Serial.println("done!");

}

void Core1State::triggerSound(uint8_t buf, uint32_t sound) {

  Serial.println("core1 - Triggering sound");

  using namespace msg;

  if (buf >= MAX_CONCURRENT_SOUNDS) {
    Serial.println("Cannot trigger sound, the provided buffer is too high");
    return;
  }

  prepareNext[buf] = sound;

  // send a stop msg
  Message stopMsg = stopMsgEmpty();
  stopMsg = stopMsgWithBuf(stopMsg, buf);

  Serial.print("core1 - Sending stop message for buffer: ");
  Serial.println(buf);

  if (!(sharedState->sendMsgToCore0(stopMsg))) {
    Serial.println("core1 - ERROR cannot push trigger stop message");
  }
}
