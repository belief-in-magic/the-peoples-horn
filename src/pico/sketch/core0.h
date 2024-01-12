/*

Main state class for the first core

Tasks include:
  - Reading from all active buffers and writing to the I2S driver.
  - Handling inbound command messages from the second core. eg "buffer X is ready to read", "disable buffer Y"
  - Ack'ing commands from the second core.

The main loop of this core is (most likely) limited by the blocking nature of the I2S driver, which should currently be
set to around 44khz.
*/

#pragma once

#include <cstdint>
#include <I2S.h>
#include "sharedState.h"
#include "msg.h"

class Core0State {

  bool activeBuffers[MAX_CONCURRENT_SOUNDS];

  // Holds the current byte offset for each buffer. Note this might be greater than the max buffer size, so always
  // modulo this pointer value before using it to reference data from a buffer.
  uint32_t currPointers[MAX_CONCURRENT_SOUNDS];

  uint32_t mostRecentReadySector[MAX_CONCURRENT_SOUNDS];

  // Core 0 should only have read access to this state. TODO figure out smart pointers
  SharedState* sharedState;

  I2S* i2s;

  private:
    void handleInboundMsgs();
    void sendI2S(int16_t sample);
    int16_t readBuffers();
    bool proceedToRead(uint8_t buffer);

  public:
    Core0State(SharedState* ss, I2S *i2s);
    void loop();
};