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
    
}

void InputState::setMcp(Adafruit_MCP23X17 *mcpIf) {
    mcp = mcpIf;
    setUpInputIO();
}

void InputState::setUpInputIO() {

    // pinMode(SOUND_S0, OUTPUT);
    // pinMode(SOUND_S1, OUTPUT);
    // pinMode(SOUND_S2, OUTPUT);
    // pinMode(SOUND_A, INPUT);

    Serial.println("Starting MCP23017 init");

    IO_I2C_IF.setSDA(IO_I2C_SDA);
    IO_I2C_IF.setSCL(IO_I2C_SCL);

    IO_I2C_IF.begin();

    if (!mcp->begin_I2C(MCP23XXX_ADDR, &IO_I2C_IF)) {
      Serial.println("Error Init MCP23017");
      while(1);
    }

    mcp->pinMode(8, INPUT_PULLUP);
    mcp->pinMode(9, INPUT_PULLUP);
    mcp->pinMode(10, INPUT_PULLUP);
    mcp->pinMode(11, INPUT_PULLUP);

    mcp->pinMode(12, INPUT_PULLUP);
    mcp->pinMode(13, INPUT_PULLUP);
    mcp->pinMode(14, INPUT_PULLUP);

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


    int pinId = IO_B0;
    
    switch (button) {

    case 0:
        pinId = IO_B0;
        break;
    case 1:
        pinId = IO_B1;
        break;
    case 2:
        pinId = IO_B2;
        break;
    case 3:
        pinId = IO_B3;
        break;
     
    }

    return mcp->digitalRead(pinId) == SOUND_ACTIVE;
    //return false;
}


bool InputState::pollMuteButton() {
   
    return mcp->digitalRead(IO_ROT_DOWN) == 0;
    //return false;
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
