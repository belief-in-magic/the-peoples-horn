/*

#include "input.h"
#include <Arduino.h>


bool InputController::soundButtonState[NUM_SOUND_BUTTONS];
uint64_t InputController::soundButtonLastPressed[NUM_SOUND_BUTTONS];
InputStream InputController::inputStream;

void InputController::setup() {


  for(int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    soundButtonState[i] = false;
    soundButtonLastPressed[i] = millis();
  }

  inputStream = InputStream();

  // TODO might be a better way to do this with macros/first class-like functions
  attachInterrupt(SB0, soundFallingEventP0, CHANGE);
  attachInterrupt(SB1, soundFallingEventP1, CHANGE);
  attachInterrupt(SB2, soundFallingEventP2, CHANGE);
  attachInterrupt(SB3, soundFallingEventP3, CHANGE);
}

void InputController::soundFallingEventP0() {
  uint64_t currMillis = millis();

  if (isBounce(currMillis, soundButtonLastPressed[0])) {
    return; // debounce
  }

  Serial.print("Pushing event: ");
  Serial.print(SB0_ACTIVE);
  Serial.print(",");
  Serial.println(currMillis);
  InputEvent event;
  event.timeMillis = currMillis;
  event.type = SB0_ACTIVE;
  event.val = 1;

  inputStream.push(event);
}

void InputController::soundFallingEventP1() {
  soundButtonState[1] = digitalRead(SB0);
}

void InputController::soundFallingEventP2() {
  uint64_t currMillis = millis();
  if (isBounce(currMillis, soundButtonLastPressed[2])) {
    return; // debounce
  }

  soundButtonState[2] = true;
  soundButtonLastPressed[2] = currMillis;
}

void InputController::soundFallingEventP3() {
  uint64_t currMillis = millis();
  if (isBounce(currMillis, soundButtonLastPressed[3])) {
    return; // debounce
  }

  soundButtonState[3] = true;
  soundButtonLastPressed[3] = currMillis;
}


bool InputController::isBounce(uint64_t currTime, uint64_t previous) {
  return (currTime - previous) <= DEBOUNCE_TIME_MS;
}

*/