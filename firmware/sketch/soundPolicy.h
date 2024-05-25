/*
  There might be a more apt name for this class, but this essentially defines the eviction policy for deciding which
  buffer to start a new sound in
*/
#pragma once

#include <cstdint>
#include "sharedState.h"

class SoundPolicy {

  uint8_t bufferOrder[MAX_CONCURRENT_SOUNDS];
  uint32_t currentPlaying[MAX_CONCURRENT_SOUNDS];

  public:
    SoundPolicy();

    void setComplete(uint8_t bufferId);
    uint8_t evictBuffer(uint32_t soundToPlay);
};