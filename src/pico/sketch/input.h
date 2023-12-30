
#pragma once

#include <cstdint>

// GPIO pin numbers for the sound buttons
#define NUM_SOUND_BUTTONS 4

// GPIO pin numbers for main buttons
#define SB0 12
#define SB1 13
#define SB2 14
#define SB3 15

#define DEBOUNCE_TIME_MS 100
#define CHORD_WAIT_TIME_MS 70

// input controller for the second core
class InputController {

  static bool soundButtonState[NUM_SOUND_BUTTONS];
  static uint64_t soundButtonLastPressed[NUM_SOUND_BUTTONS];

  //bool chordStarted;
  //uint64_t chordStartTime;

  public:

    static void setup();

    static void soundFallingEventP0();
    static void soundFallingEventP1();
    static void soundFallingEventP2();
    static void soundFallingEventP3();

    static void soundRisingEventP0();
    //soundRisingEventP1();
    //soundRisingEventP2();
    //soundRisingEventP3();
    static void updateChord(int buttonNum);
    static bool isBounce(uint64_t currTimeMillis, uint64_t previousMillis);
};