#include <I2S.h>
#include <SPI.h>
#include <SD.h>

#include "input.h"

#include "core0.h"
#include "core1.h"

// GPIO pin numbers
#define pBCLK 26
#define pWS (pBCLK+1)
#define pDOUT 28

const int sampleRate = 44100;

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

SharedState sharedState;
Core0State core0State(&sharedState, &i2s);
Core1State core1State(&sharedState);

InputState tInputState;

void setup() {

  Serial.begin(115200);


  delay(3000);
  Serial.println("BEGIN ****************************************************");

  Serial.println("core0 - starting setup");

  rp2040.fifo.push(123);
  rp2040.fifo.pop(); // wait for the second core's setup to complete first

  core0State.setup();

  Serial.println("core0 - setup for first core, waiting done");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

}


void setup1() {
  rp2040.fifo.pop();
  core1State.setup();
  rp2040.fifo.push(123);

}


void loop() {
  core0State.loop();
}


int timesRun = 0;
void loop1() {
  
  core1State.loop();
  /*

  if (millis() > 5000 && timesRun == 0) {
    Serial.println("Triggering sound");
    core1State.triggerSound(2, 2);
    //core1State.triggerSound(3, 5);
    timesRun++;
  }

  */
  
}

