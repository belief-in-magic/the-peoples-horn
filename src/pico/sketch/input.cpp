#include "input.h"


InputState::InputState() {

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    isActive[i] = false;
    lastChange[i] = 0;
  }

  lastChordStart = 0;
  chordStarted = false;
  currentChord = 0;
  lastMuted = 0;

  setUpInputIO();
}

void InputState::setUpInputIO() {

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    pinMode(SOUND_B0+i, INPUT);
  }

}

uint32_t InputState::pollSoundButtonsWithInactiveCooldown() {

  uint32_t v = 0;
  bool pinIsActive;
  uint64_t currentTime = millis();

  for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    pinIsActive = digitalRead(SOUND_B0+i) == SOUND_BUTTON_ACTIVE;

    if ((currentTime - lastChange[i]) > DEBOUNCE_MS) {

       if (isActive[i] != pinIsActive) {
         lastChange[i] = currentTime;
         isActive[i] = pinIsActive;

         // start chord if not already started
         if (pinIsActive && !chordStarted) {
           chordStarted = true;
           lastChordStart = currentTime;
           currentChord = 0;
         }

       }

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


bool InputState::pollMuteButton() {

  return digitalRead(MUTE_B);
}

bool InputState::isMuted() {

  uint64_t currentTime = millis();

  bool muteCapture = pollMuteButton();

  if (muteCapture && (currentTime-lastMuted) > DEBOUNCE_MS) {
    lastMuted = currentTime;
    return true;
  }

  return false;
}

// TODO tidy up this
std::optional<uint32_t> InputState::getNextSound() {

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
  }

  return {};

}