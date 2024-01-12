/*
#pragma once

#include "doublebuf.h"
#include <cstdint>
#include <I2S.h>
#include <SPI.h>
#include <SD.h>

// GPIO pin numbers for main buttons
#define SB0 12
#define SB1 13
#define SB2 14
#define SB3 15

// GPIO for sd card in spi mode (pico w tested only, you may need to change this depending on hardware)
#define pSD_CS 5
#define pSD_SCK 6
#define pSD_MOSI 7
#define pSD_MISO 4


#define DEBOUNCE_TIME_MS 100
#define CHORD_WAIT_TIME_MS 70

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

// could probably be split into two different classes, one for the player core, and the sd core
class State {

  DoubleBuf* buffers[MAX_CONCURRENT_SOUNDS];

  uint32_t activeSounds[MAX_CONCURRENT_SOUNDS]; // only modified by the first core, the second core reads this to determine which dbuf to write to


  // used only by the first core to keep track of the sounds that have completed preparation
  bool core0_readySounds[MAX_CONCURRENT_SOUNDS];
  I2S* core0_i2s;

  // used only by the second core to keep track of which buffers and which sounds have requested to be overwritten.
  // value of 0 means that the buffer does not need to be overwritten
  uint32_t nextBufferWrite[MAX_CONCURRENT_SOUNDS];

  uint32_t lastRising[MAX_CONCURRENT_SOUNDS];
  uint32_t chordStartTime;

  uint32_t core1_stateCounter;

  int timesRan = 0;
  uint32_t lastRan = 0;

  private:

    // used by the first core
    //void core0_setUpI2S();

    void core0_handleRequests();

    // used by second core
    void core1_setUpSD();
    void core1_setUpInput();

    void core1_handleInput();
    void core1_handleRequests();

  public:
    State();
    ~State();
    void core0_stateSetup(I2S* i2s);
    void core1_stateSetup();

    void core0_stateLoop();
    void core1_stateLoop();

};
*/