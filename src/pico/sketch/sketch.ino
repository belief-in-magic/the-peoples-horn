
#include <I2S.h>
#include <SPI.h>
#include <SD.h>

#include "state.h"

State state;

// GPIO pin numbers
#define pBCLK 26
#define pWS (pBCLK+1)
#define pDOUT 28

const int sampleRate = 44100;

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

void setup() {

  Serial.begin(115200);
  delay(2000);
  Serial.println("BEGIN ****************************************************");
  Serial.println("Starting setup for first core");

  rp2040.fifo.push(0);
  rp2040.fifo.pop(); // wait for the second core's setup to complete first

  Serial.println("Starting setup for first core, waiting done");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  state.core0_stateSetup(&i2s);
}

void setup1() {

  rp2040.fifo.pop();
  Serial.println("Starting setup for second core");

  state.core1_stateSetup();

  rp2040.fifo.push(0);

}

void loop() {
  state.core0_stateLoop();
}

void loop1() {
  state.core1_stateLoop();
}
