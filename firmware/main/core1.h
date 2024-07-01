/*

Main class for the second core

Tasks include:
  - Reading from the SD card, into the buffers
  - Sending command messages to the first core, as well as receiving Acks
  - Handling user IO, which includes managing the SharedState

The bottleneck for the main loop is reading sectors from the SD card. This is actually quite an expensive
operation given that we read from the SD card using SPI. For 20kb this takes around 50ms.
*/

#pragma once

#include <cstdint>
#include <SD.h>
#include <SPI.h>
#include "interface.h"
#include "sharedState.h"
#include "soundPolicy.h"
#include "input.h"
#include "msg.h"

class Core1State {

  SharedState* sharedState;

  InputState inputState;
  SoundPolicy soundPolicy;
  bool isMuted;

  uint32_t highestAckedSector[MAX_CONCURRENT_SOUNDS]; // what is this for again?
  uint32_t prepareNext[MAX_CONCURRENT_SOUNDS];

  private:
    void setUpSD();
    void handleInboundMsgs();

  public:
    void setup(Adafruit_MCP23X17 *mcp);
    void triggerSound(uint8_t buf, uint32_t sound); // TODO temp add here for testing

    Core1State(SharedState* ss);
    void loop();
};
