#include <I2S.h>
#include <SPI.h>
#include <SD.h>

#include "input.h"

#include "core0.h"
#include "core1.h"

// GPIO for speakers (I2S)
#define pBCLK 18
#define pWS (pBCLK+1)
#define pDOUT 20

const int sampleRate = 44100;

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

SharedState sharedState;
Core0State core0State(&sharedState, &i2s);
Core1State core1State(&sharedState);

void setup() {

  Serial.begin(115200);

  delay(4000);


  Serial.println("BEGIN ****************************************************");

  Serial.println("core0 - starting setup");

  rp2040.fifo.push(123);
  rp2040.fifo.pop(); // wait for the second core's setup to complete first

  core0State.setup();

  Serial.println("core0 - waiting on other core done");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

    Serial.println("core0 - setup done");

}


void setup1() {
  rp2040.fifo.pop();
  Serial.println("core1 - starting setup");
  core1State.setup();
  Serial.println("core1 - setup done");

  rp2040.fifo.push(123);

}


void loop() {
  core0State.loop();
}


void loop1() {  
  core1State.loop();
}

