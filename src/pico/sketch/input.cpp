
#include "input.h"

#include <Arduino.h>


bool InputController::soundButtonState[NUM_SOUND_BUTTONS];
uint64_t InputController::soundButtonLastPressed[NUM_SOUND_BUTTONS];

void InputController::setup() {

  for(int i = 0; i < NUM_SOUND_BUTTONS; i++) {
    soundButtonState[i] = false;
    soundButtonLastPressed[i] = millis();
  }

  // TODO might be a better way to do this with macros/first class-like functions
  attachInterrupt(SB0, soundFallingEventP0, FALLING);
  attachInterrupt(SB1, soundFallingEventP1, FALLING);
  attachInterrupt(SB2, soundFallingEventP2, FALLING);
  attachInterrupt(SB3, soundFallingEventP3, FALLING);
}

void InputController::soundFallingEventP0() {
  uint64_t currMillis = millis();

  if (isBounce(currMillis, soundButtonLastPressed[0])) {
    return; // debounce
  }
  Serial.println("Falling interrupt start p0");
  delay(5000);
  Serial.println("Falling interrupt p0");
  soundButtonState[0] = true;
  soundButtonLastPressed[0] = currMillis;
}

void InputController::soundFallingEventP1() {
  uint64_t currMillis = millis();
  if (isBounce(currMillis, soundButtonLastPressed[1])) {
    return; // debounce
  }

  Serial.println("Falling interrupt start p1");
    delay(5000);
    Serial.println("Falling interrupt p1");
  soundButtonState[1] = true;
  soundButtonLastPressed[1] = currMillis;

}

void InputController::soundFallingEventP2() {
  uint64_t currMillis = millis();
  if (isBounce(currMillis, soundButtonLastPressed[2])) {
    return; // debounce
  }

  Serial.println("Falling interrupt start p2");
  delay(5000);
  Serial.println("Falling interrupt p2");

  soundButtonState[2] = true;
  soundButtonLastPressed[2] = currMillis;
}

void InputController::soundFallingEventP3() {
  uint64_t currMillis = millis();
  if (isBounce(currMillis, soundButtonLastPressed[3])) {
    return; // debounce
  }

    Serial.println("Falling interrupt start p3");
    delay(5000);
    Serial.println("Falling interrupt p3");

  soundButtonState[3] = true;
  soundButtonLastPressed[3] = currMillis;
}

void updateChord(int buttonNum) {

}

bool InputController::isBounce(uint64_t currTime, uint64_t previous) {
  return (currTime - previous) <= DEBOUNCE_TIME_MS;
}

