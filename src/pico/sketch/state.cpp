#include "state.h"
#include <Arduino.h>

// 0 means disable, 1 means enable
#define ENABLE_MSG_MASK 128
#define VAL_MSG_MASK 127
#define NO_OP_MSG 256

State::State() { }

// Should never really be called
State::~State() {
 for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
   delete buffers[i];
 }
}

void State::core0_stateSetup(I2S* i2s) {
  core0_i2s = i2s;

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    activeSounds[i] = i;
  }
}


void State::core1_stateSetup() {

 Serial.println("core1 setup start");

 core1_setUpSD();
 core1_setUpInput();

 core1_stateCounter = 0;

 chordStartTime = 0;

 // Pre-populate the sounds buffers
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    readySounds[i] = false; // set to "don't play"

    lastRising[i] = 0;
    nextBufferWrite[i] = 0;

    buffers[i] = new DoubleBuf(i+1); // set to the first 4 sounds
  }

  Serial.println("core1 setup end");
}

void State::core1_setUpSD() {

  Serial.print("Initializing SD card...");
  SPI.setRX(pSD_MISO);
  SPI.setTX(pSD_MOSI);
  SPI.setSCK(pSD_SCK);

  if (!SD.begin(pSD_CS)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("done!");
}


void State::core0_stateLoop() {

  // check for any block/re-enable requests from the second core
  core0_handleRequests();

  int32_t sum = 0;
  // play sounds from readySounds
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (readySounds[i]) {
      sum += (int32_t) ((int16_t)((buffers[i])->readNextSample()));
    }
  }

  int16_t sampleAdjusted = ((int16_t) max(-32766, min(32766, sum)))/2;

  core0_i2s->write(sampleAdjusted);
  core0_i2s->write(sampleAdjusted);
}

void State::core0_handleRequests() {
  // check fifo queue for new requests

  uint32_t msg, bufferId;
  int numMessages = rp2040.fifo.available();

  uint32_t msgForBuffer[MAX_CONCURRENT_SOUNDS]; // TODO generalize this

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    msgForBuffer[i] = NO_OP_MSG;
  }

  for (int i = 0; i < numMessages; i++) {
    msg = rp2040.fifo.pop();
    bufferId = VAL_MSG_MASK & msg;
    msgForBuffer[bufferId] = msg;
  }

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (msgForBuffer[i] == NO_OP_MSG) {
      continue;
    }

    if ((msgForBuffer[i] & ENABLE_MSG_MASK) > 0) {
      Serial.print("core0 - enabling due to msg: ");
      Serial.println(msgForBuffer[i]);
      readySounds[i] = true;
      //Serial.println("core0 - Sending reenable ack message");
      rp2040.fifo.push(msgForBuffer[i]);
    } else {
      //Serial.print("core0 - disabling due to msg: ");
      //Serial.println(msgForBuffer[i]);
      readySounds[i] = false;
      rp2040.fifo.push(msgForBuffer[i]); // ack that the buffer has been disabled
    }
  }

}

void State::core1_setUpInput() {
  pinMode(SB0, INPUT);
  pinMode(SB1, INPUT);
  pinMode(SB2, INPUT);
  pinMode(SB3, INPUT);
}

void State::core1_stateLoop() {

  core1_handleInput();
  core1_handleRequests();

  // update single buffer if needed:
  int currentBufferToUpdate = core1_stateCounter++ % MAX_CONCURRENT_SOUNDS;
  if (nextBufferWrite[currentBufferToUpdate] == 0) {
    (buffers[currentBufferToUpdate])->populateWriteBuf();
  }
}

void State::core1_handleRequests() {

  uint32_t msg;
  uint32_t bufferToWrite, bufferId;
  uint32_t reenableMsg;

  int numMessages = rp2040.fifo.available();

  uint32_t msgForBuffer[MAX_CONCURRENT_SOUNDS]; // TODO generalize this

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    msgForBuffer[i] = NO_OP_MSG;
  }

  for (int i = 0; i < numMessages; i++) {
    msg = rp2040.fifo.pop();
    bufferId = VAL_MSG_MASK & msg;
    msgForBuffer[bufferId] = msg; // messages should always just be the buffer id in this case, though
  }

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {

    if (msgForBuffer[i] == NO_OP_MSG) {
      continue;
    }

    bufferToWrite = msgForBuffer[i] & VAL_MSG_MASK;

    if ((msgForBuffer[i] & ENABLE_MSG_MASK) > 0) {
      Serial.print("core1 - receiving ack msg for reenabling: ");
      Serial.print(msgForBuffer[i]);
      // set next buffer to write 0, meaning that we don't need to reset this buffer
      nextBufferWrite[bufferToWrite] = 0;

    } else {
      Serial.print("core1 - receiving ack msg for disabling: ");
      Serial.println(msgForBuffer[i]);

      (buffers[bufferToWrite])->newSource(nextBufferWrite[bufferToWrite]);

      reenableMsg = msg | ENABLE_MSG_MASK;
      Serial.println("core1 - sending reenable");
      rp2040.fifo.push(reenableMsg);
    }
  }

}

void State::core1_handleInput() {

  int gpioPin;
  uint32_t currMillis = millis();

  if ((digitalRead(SB0) == 0) && (timesRan < 2)) { // test
    if ((nextBufferWrite[0] == 0) && (nextBufferWrite[1] == 0)) {

      lastRan = currMillis;
      timesRan++;

      nextBufferWrite[0] = 5;
      nextBufferWrite[1] = 6;

      // send stop message to the first core
      Serial.println("Sending initial message to disable 0 and 1");
      rp2040.fifo.push(0); // TODO change to NB?
      rp2040.fifo.push(1); // TODO change to NB?
    }
  }
//
//  if (digitalRead(SB1) == 0) { // test
//    if (nextBufferWrite[1] == 0) {
//      lastRan = currMillis;
//      timesRan++;
//
//
//      // send stop message to the first core
//
//    }
//  }

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
