/*

Main class for the second core

Tasks include:
  - Reading from the SD card, into the buffers
  - Sending command messages to the first core, as well as receiving acks
  - Handling user IO, which includes managing the SharedState

The bottleneck for the main loop is reading sectors from the SD card. This is actually quite an expensive
operation given that we read from the SD card using SPI. For 20kb this takes around 50ms.
*/

#pragma once

#include <cstdint>
#include <SD.h>
#include <SPI.h>
#include "doubleBuf.h"
#include "shareState.h"
#include "msg.h"

// GPIO pin numbers for main buttons TODO move this into its own InputState class file
#define SB0 12
#define SB1 13
#define SB2 14
#define SB3 15

// GPIO for sd card in spi mode (pico w tested only, you may need to change this depending on hardware)
#define pSD_CS 5
#define pSD_SCK 6
#define pSD_MOSI 7
#define pSD_MISO 4


class Core1State {

  SharedState* sharedState;

  uint32_t highestAckedSector[MAX_CONCURRENT_SOUNDS];
  bool prepareNext[MAX_CONCURRENT_SOUNDS];

  private:
    void setUpSD();

    void prepareNextSector(uint8_t buf);
    void triggerSound(uint8_t buf, uint32_t sound);

  public:
    Core1State(SharedState* ss);
    void loop();
};