
#pragma once

#include <cstdint>
#include <optional>
#include <Arduino.h>

#include "sharedState.h"


#define CHORD_WAIT_TIME_MS 50

#define INACTIVE_COOLDOWN_MS 50

#define DEBOUNCE_MS 40

#define SOUND_BUTTON_ACTIVE 0

// GPIO pin numbers for main buttons
#define NUM_SOUND_BUTTONS 4
#define SOUND_B0 12
//#define SOUND_B1 13
//#define SOUND_B2 14
//#define SOUND_B3 15

#define MUTE_B 11



class InputState {

  private:

    uint64_t lastChange[NUM_SOUND_BUTTONS];

    bool isActive[NUM_SOUND_BUTTONS];

    uint64_t lastChordStart;
    uint64_t lastMuted;
    bool muteState;

    bool chordStarted;
    uint32_t currentChord;

    void setUpInputIO();

    uint32_t pollSoundButtonsWithInactiveCooldown();

    bool pollMuteButton();


  public:
    InputState();
    std::optional<uint32_t> getNextSound();
    bool isMuted();
};