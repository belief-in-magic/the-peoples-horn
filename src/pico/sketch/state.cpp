#include "state.h"

/*
State::State() {

 for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
   readySounds[i] = false;
 }

 // allocate doublebufs
 for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
   buffers[i] = new DoubleBuf(i);
 }

}

// Should never really be called
State::~State() {
 for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
   delete buffers[i];
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
  return;
}

State::core1StateLoop() {
  // check for any new input
  // -> determine which currently active sound to evict
  // -> send the preparation request to the second core


  // check if any sound request have completed preparation, enable them if so

  // play sounds from readySounds

}

State::core2StateLoop() {

  // check for any new sound prep requests
  int soundPreps = numSoundPrepRequests();
  while (int i = 0; i < soundPreps; i++) {
    uint32_t soundIdToPrepare = getSoundRequest();
    prepareSound(soundIdToPrepare); // blocks
  }

  // update all active buffers
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    (buffers+i)->populateWriteBuf();
  }

}

State::prepareSound(uint32_t soundId) {

  int bufferToWrite = -1;
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    if (activeSounds[i] == soundId) {
      bufferToWrite = i;
      break;
    }
  }
  // if sound does not exist in activeSounds, ignore this request as it's stale
  if (bufferToWrite == -1) {
    return;
  }

  (buffers+bufferToWrite)->newSource(soundId);

  notifySoundPrepared(soundId); // blocks
}


int State::numSoundPrepRequests() {
  // run from second core
  return rp2040.fifo.available();
}

void State::notifySoundPrepared(uint32_t soundId) {
  // run from second core
  rp2040.fifo.push(soundId);
}

*/