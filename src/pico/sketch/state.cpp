#include "state.h"

// 0 means disable, 1 means enable
#define ENABLE_MSG_MASK = 128

#define VAL_MSG_MASK = 127


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

 core1_stateCounter = 0;
 chordStartTime = 0;

 // Pre-populate the sounds buffers
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    lastRising[i] = 0;
    nextBufferWrite[i] = 0;

    activeSounds[i] = i;
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

  uint32_t msg, val;
  int messages = rp2040.fifo.available();

  for (int i = 0; i < messages; i++) {
    msg = rp2040.fifo.pop();

    val = VAL_MSG_MASK & msg;

    // disable requested buffer
    if (val >= 0 && val <= MAX_CONCURRENT_SOUNDS) {
      if ((msg & ENABLE_MSG_MASK) > 0) {
        readySounds[val] = true;
      } else {
        readySounds[val] = false;
        // send ack
        rp2040.fifo.push(val);
      }
    }
  }
}

State::core1_setUpInput() {
  pinMode(SB0, INPUT);
  pinMode(SB1, INPUT);
  pinMode(SB2, INPUT);
  pinMode(SB3, INPUT);
}

State::core1_stateLoop() {

  core1_handleInput();
  core1_handleRequests();

  // update single buffer if needed:
  currentBufferToUpdate = core1_stateCounter++ % MAX_CONCURRENT_SOUNDS;
  if (nextBufferWrite[currentBufferToUpdate] == 0) {
    (buffers+currentBufferToUpdate)->populateWriteBuf();
  }
}

State::core1_handleRequests() {
  uint32_t msg;
  uint32_t bufferToWrite;
  uint32_t reenableMsg;

  int messages = rp2040.fifo.available();

  for (int i = 0; i < messages; i++) {

    // should only be an ack message
    msg = rp2040.fifo.pop();

    if (msg >= 0 && msg <= MAX_CONCURRENT_SOUNDS) {
      // immediately prepare sound for specified buffer
      bufferToWrite = msg;
      (buffers+bufferToWrite)->newSource(nextBufferWrite[bufferToWrite]);

      reenableMsg = msg | ENABLE_MSG_MASK;
      rp2040.fifo.push(reenableMsg);
    }
  }
}

State::core1_handleInput() {

  int gpioPin;
  uint32_t currMillis = millis();

  if (digitalRead(PB0) == 0) { // test
    nextBufferWrite[0] = 1; // set first buffer to the s01 sound.

    // send stop message to the first core
    rp2040.fifo.push(0) // TODO change to NB?
  }

  /*
  // check inputs for any changes
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    gpioPin = SB0+i;

    if (digitalRead(gpioPin) == 0) { // low-triggered right now

      if (lastRising[i] != 0 && (currMillis - lastRising[i]) >= CHORD_WAIT_TIME_MS) {
        // send event

        lastRising[i] = 0;
      }
    }
  }
  */

}
