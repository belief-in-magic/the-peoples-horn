

#include <I2S.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

#include "interface.h"
#include "input.h"
#include "core0.h"
#include "core1.h"


const int sampleRate = 44100;

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

Adafruit_MCP23X17 mcp;

SharedState sharedState;
Core0State core0State(&sharedState, &i2s);
Core1State core1State(&sharedState);

void setup() {

  Serial.begin(115200);


  Serial.println("BEGIN ****************************************************");

  Serial.println("core0 - starting setup");

  rp2040.fifo.push(123);
  rp2040.fifo.pop(); // wait for the second core's setup to complete first

  core0State.setup();

  Serial.println("core0 - waiting on other core done");

  i2s.setBCLK(SPEAKER_I2S_BCLK);
  i2s.setDATA(SPEAKER_I2S_DOUT);
  i2s.setBitsPerSample(I2S_BITS_PER_SAMPLE);

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
  core1State.setup(&mcp);
  Serial.println("core1 - setup done");

  rp2040.fifo.push(123);

}


void loop() {
  core0State.loop();
}


void loop1() {  
  core1State.loop();

}

