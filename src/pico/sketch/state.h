#pragma once

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

// could probably be split into two different classes, one for the player core, and the sd core
class State {
  /*
  uint32_t activeSounds[MAX_CONCURRENT_SOUNDS]; // only modified by the first core, the second core reads this to determine which dbuf to write to
  DoubleBuf* buffers[MAX_CONCURRENT_SOUNDS];

  // used only by the first core to keep track of the sounds that have completed preparation
  bool readySounds[MAX_CONCURRENT_SOUNDS];

  private:
    // used by the first core
    bool sendSoundPrepareRequest(uint32_t soundId);
    // check if any sound is ready to play
    bool soundReady();
    uint32_t getPreparedSound(); // blocks

    // used by second core
    uint32_t numSoundPrepRequests();
    uint32_t getSoundRequest(); // blocks
    void notifySoundPrepared(uint32_t soundId);
    bool prepareSound(uint32_t soundId);

  public:
    State();
    ~State();
    void core1StateSetup();
    void core2StateSetup();

    void core1StateLoop();
    void core2StateLoop();

    */
};