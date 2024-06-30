
#pragma once

#include <cstdint>
#include <optional>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

#include "interface.h"
#include "sharedState.h"

Adafruit_MCP23X17 mcp;

#define CHORD_WAIT_TIME_MS 50

#define INACTIVE_COOLDOWN_MS 50

#define DEBOUNCE_MS 40

#define NUM_SOUND_BUTTONS 4

#define SOUND_ACTIVE 0

// Pin IDs for the MCP23017. Note that these are not the actual pins, but their ids.
// See https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library

// Input pin IDs, corresponds with the GPB
#define IO_B0 8
#define IO_B1 9
#define IO_B2 10
#define IO_B3 11
#define IO_ROT_CLK 12
#define IO_ROT_CNCLK 13
#define IO_ROT_DOWN 14


class InputState {

  private:

    uint64_t lastChange[NUM_SOUND_BUTTONS];

    bool isActive[NUM_SOUND_BUTTONS];

    uint64_t lastChordStart;
    uint64_t lastMuted;
    bool muteState;

    bool disable;

    bool chordStarted;
    uint32_t currentChord;
    
    void setUpInputIO();

    uint32_t pollSoundButtonsWithInactiveCooldown();

    bool pollSoundButton(int button);    

    bool pollMuteButton();


  public:
    InputState();
    std::optional<uint32_t> getNextSound();
    bool isMuted();

    void disableInput();
};
