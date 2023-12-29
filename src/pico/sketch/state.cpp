#include "state.h"

State::State() {

 for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
   readySounds[i] = false;
 }


}

State::core1StateSetup() {



   // Pre-populate the sounds buffers
   for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
     activeSounds[i] = i;
     prepareSound(i);
   }

}

State::core2StateSetup() {

  uint32_t soundId;
  // Wait for the signals to prepare MAX_CONCURRENT_SOUNDS sounds
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    soundId = getSoundRequest();
    prepareSound(soundId);
  }


}