#include "input.h"


InputState::InputState() {

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    isActive[i] = false;
    lastChange[i] = 0;

  }

  lastChordStart = 0;
  chordStarted = false;
  currentChord = 0;

  setUpInputIO();
}

void InputState::setUpInputIO() {

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    pinMode(SB0+i, INPUT);
  }

}

uint32_t InputState::pollSoundButtonsWithInactiveCooldown() {

  uint32_t v = 0;
  bool pinIsActive;
  uint64_t currentTime = millis();

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    pinIsActive = digitalRead(SB0+i) == SOUND_BUTTON_ACTIVE;

    if ((pinIsActive != isActive[i]) && (currentTime - lastChange[i]) > DEBOUNCE_MS) {

       lastChange[i] = currentTime;

       if (pinIsActive) {
         v |= (1 << i);
         isActive[i] = true;
       } else {
         isActive[i] = false;
       }

    }
  }

  return v;
}

std::optional<uint32_t> InputState::tick() {

  uint64_t currentTime = millis();

  uint32_t buttonCapture = pollSoundButtonsWithInactiveCooldown();

  if (chordStarted) {

    currentChord |= buttonCapture;

    // check if the chord wait time has elapsed, if so then we can commit to a sound and buffer
    if ((currentTime - lastChordStart) > CHORD_WAIT_TIME_MS) {

      chordStarted = false;
      lastChordStart = currentTime;

      return currentChord;
    }
  } else { // otherwise check for any new chords

    if (buttonCapture > 0) {
      chordStarted = true;
      lastChordStart = currentTime;
      currentChord = buttonCapture;

    }
  }

  return {};

}