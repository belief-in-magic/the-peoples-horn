
#pragma once

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

class SharedState {
  public:
    SharedState();

    Buf buffers[MAX_CONCURRENT_SOUNDS];
};