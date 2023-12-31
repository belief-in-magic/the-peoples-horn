//

#pragma once

#include <cstdint>

#define INPUT_BUFFER_SIZE 128

enum InputEventType {
  SB0_ACTIVE,
  SB1_ACTIVE,
  SB2_ACTIVE,
  SB3_ACTIVE
};

struct InputEvent {
  uint64_t timeMillis;
  InputEventType type;
  uint32_t val;
};

// Interrupt-safe (not tested) ring buffer implementation.
// Not thread-safe, so this should only be used within one core
class InputStream {

  InputEvent buf[INPUT_BUFFER_SIZE];

  uint32_t writePtr = 0;
  uint32_t readPtr = 0;


  public:
    InputStream();

    // Non-blocking operations
    uint32_t numQueuedEvents();
    void push(InputEvent event); // will drop the event if the buffer is full

    InputEvent next(); // will return garbage if full TODO don't do this
};