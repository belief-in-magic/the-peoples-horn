
#pragma once

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

#include <cstdint>
#include "buf.h"

class SharedState {
  public:
    SharedState();

    Buf buffers[MAX_CONCURRENT_SOUNDS];
};