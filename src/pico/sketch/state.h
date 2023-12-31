#pragma once

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

// could probably be split into two different classes, one for the player core, and the sd core
class State {

  uint32_t activeSounds[MAX_CONCURRENT_SOUNDS]; // only modified by the first core, the second core reads this to determine which dbuf to write to
  DoubleBuf* buffers[MAX_CONCURRENT_SOUNDS];

  // used only by the first core to keep track of the sounds that have completed preparation
  bool readySounds[MAX_CONCURRENT_SOUNDS];

  private:

    // used by the first core
    void core0_setUpI2S();

    void core0_handleRequests();
    void core0_ack(uint32_t reqId);

    // used by second core
    void core1_setUpSD();
    bool core1_prepareSound(uint32_t soundId);

    void core1_handleRequests();
    void core1_sendRequest(uint32_t reqId);

  public:
    State();
    ~State();
    void core0_stateSetup();
    void core1_stateSetup();

    void core0_stateLoop();
    void core1_stateLoop();

};