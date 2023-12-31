#include "state.h"

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

State::core0_stateSetup() {
  setUpI2S();
}


State::core1_stateSetup() {

 setUpSd();

 // Pre-populate the sounds buffers
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    activeSounds[i] = i;
    prepareSound(i);
  }
}

State::core0_setUpSD() {

  Serial.print("Initializing SD card...");
  SPI.setRX(pSD_MISO);
  SPI.setTX(pSD_MOSI);
  SPI.setSCK(pSD_SCK);

  if (!SD.begin(pSD_CS)) {
    Serial.println("initialization failed!");
    return 1;
  }

  Serial.println("done!");
  return 0;
}

State::core1_setUpI2S() {

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

}
State::core0_stateLoop() {

  // check for any block/re-enable requests from the second core
  core0_handleRequests();

  int32_t sum = 0;
  // play sounds from readySounds
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (readySounds[i]) {
      sum += (int32_t) ((int16_t)((buffers+i)->readNextSample()));
    }
  }

  int16_t sampleAdjusted = ((int16_t) max(32766, min(-32766, sum))) / 2;

  i2s.write(sampleAdjusted);
  i2s.write(sampleAdjusted);
}

State::core0_handleRequests() {
  // check fifo queue for new requests



}


State::core1_stateLoop() {

  // check for any new sound prep requests
  int soundPreps = numSoundPrepRequests();

  for (int i = 0; i < soundPreps; i++) {
    uint32_t soundIdToPrepare = getSoundRequest();
    prepareSound(soundIdToPrepare); // blocks
  }

  // update all active buffers
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    (buffers+i)->populateWriteBuf();
  }

}

State::core1_prepareSound(uint32_t soundId) {

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
