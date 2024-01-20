#include "soundPolicy.h"



SoundPolicy::SoundPolicy() {

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    bufferOrder[i] = i;
  }

}

void SoundPolicy::setComplete(uint8_t bufferId) {

  // this really just bumps up the buffer to the next-to-evict spot

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (bufferOrder[i] > bufferOrder[bufferId]) {
      bufferOrder[i]--;
    }
  }

  bufferOrder[bufferId] = MAX_CONCURRENT_SOUNDS-1; // mark this as the next to evict

}


uint8_t SoundPolicy::evictBuffer() {

  uint8_t bufferToEvict = 0;

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (bufferOrder[i] == MAX_CONCURRENT_SOUNDS-1) {
      bufferToEvict = i;
    }

    bufferOrder[i]  = (bufferOrder[i]+1) % MAX_CONCURRENT_SOUNDS;
  }

  return bufferToEvict;
}