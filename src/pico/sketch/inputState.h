
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