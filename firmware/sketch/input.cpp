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
    muteState = false;
    disable = false;

    setUpInputIO();
}

void InputState::setUpInputIO() {

    pinMode(SOUND_S0, OUTPUT);
    pinMode(SOUND_S1, OUTPUT);
    pinMode(SOUND_S2, OUTPUT);
    pinMode(SOUND_A, INPUT);

}

void InputState::disableInput() {

    disable = true;
}

uint32_t InputState::pollSoundButtonsWithInactiveCooldown() {

    if (disable) {
        return 0;
    }

    uint32_t v = 0;
    bool pinIsActive;
    uint64_t currentTime = millis();

    for (int i = 0; i < NUM_SOUND_BUTTONS; i++) {
        pinIsActive = pollSoundButton(i);
      
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

bool InputState::pollSoundButton(int button) {
    
    if (button == 0) {
        digitalWrite(SOUND_S0, HIGH);
        digitalWrite(SOUND_S1, LOW);
        digitalWrite(SOUND_S2, LOW);
    } else if (button == 1) {
        digitalWrite(SOUND_S0, HIGH); // button 1 and 2 are swapped in the hardware, this is a bug
        digitalWrite(SOUND_S1, HIGH);
        digitalWrite(SOUND_S2, LOW);
    } else if (button == 2) {
        digitalWrite(SOUND_S0, LOW);
        digitalWrite(SOUND_S1, HIGH);
        digitalWrite(SOUND_S2, LOW);
    } else if (button == 3) {
        digitalWrite(SOUND_S0, LOW);
        digitalWrite(SOUND_S1, LOW);
        digitalWrite(SOUND_S2, HIGH);
    }

    delay(2);

    return digitalRead(SOUND_A) == SOUND_ACTIVE;
}


bool InputState::pollMuteButton() {


    digitalWrite(SOUND_S0, HIGH);
    digitalWrite(SOUND_S1, HIGH);
    digitalWrite(SOUND_S2, HIGH);
    delay(2);

    
    return digitalRead(SOUND_A) == SOUND_ACTIVE;
}

bool InputState::isMuted() {

    if (disable) {

        return false;
    }

    uint64_t currentTime = millis();

    bool muteCapture = pollMuteButton();

    if ((currentTime-lastMuted) > DEBOUNCE_MS) {
        lastMuted = currentTime;
        muteState = muteCapture;
        return muteCapture;
    }

    return muteState;
}

// TODO tidy up this
std::optional<uint32_t> InputState::getNextSound() {

    if (disable) {
        return {};
    }
      

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
