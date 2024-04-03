#include "soundPolicy.h"

SoundPolicy::SoundPolicy() {

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    bufferOrder[i] = i;

    currentPlaying[i] = 0;
  }

}

void SoundPolicy::setComplete(uint8_t bufferId) {

  // this really just bumps up the buffer to the next-to-evict spot

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (bufferOrder[i] > bufferOrder[bufferId]) {
      bufferOrder[i]--;
    }
  }

  currentPlaying[bufferId] = 0;

  bufferOrder[bufferId] = MAX_CONCURRENT_SOUNDS-1; // mark this as the next to evict

}


uint8_t SoundPolicy::evictBuffer(uint32_t soundToPlay) {

  // first check if the sound is already playing in a buffer. If so then evict use that buffer.
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (currentPlaying[i] == soundToPlay) {
      return i;
    }
  }

  // otherwise, choose the highest order buffer
  uint8_t bufferToEvict = 0;

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (bufferOrder[i] == MAX_CONCURRENT_SOUNDS-1) {
      bufferToEvict = i;
      currentPlaying[i] = soundToPlay;
    }

    bufferOrder[i]  = (bufferOrder[i]+1) % MAX_CONCURRENT_SOUNDS;
  }

  return bufferToEvict;
}
