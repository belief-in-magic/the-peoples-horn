/*
    bool setBCLK(pin_size_t pin);
    - This assigns two adjacent pins - the pin after this one (one greater)
      is the WS (word select) signal, which toggles before the sample for
      each channel is sent

    bool setDATA(pin_size_t pin);
    - Sets the DOUT pin, can be any valid GPIO pin
*/

#include <I2S.h>
#include <SPI.h>
#include <SD.h>

#include "wav.h"
#include "doublebuf.h"
#include "input.h"

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

// GPIO pin numbers
#define pBCLK 26
#define pWS (pBCLK+1)
#define pDOUT 28

// GPIO for sd card in spi mode (pico w tested only, you may need to change this depending on hardware)
#define pSD_CS 5
#define pSD_SCK 6
#define pSD_MOSI 7
#define pSD_MISO 4

const int sampleRate = 44100;

void setup() {
  rp2040.fifo.pop(); // wait for the second core's setup to complete first

  Serial.println("Starting setup for first core");

  core0StateSetup();
}

void setup1() {
  Serial.begin(115200);
  Serial.println("Starting setup for second core");

  core1StateSetup();

  rp2040.fifo.push(1);

}

void loop() {
  /*
  if (!dbuf0->isFinished() || !dbuf1->isFinished()) {
    int16_t sample0 = (int16_t)(dbuf0->readNextSample());
    //int16_t sample0 = 0;
    int16_t sample1 = (int16_t)(dbuf1->readNextSample());

    int16_t sampleAdjusted = ((int16_t) max(-32766, min(32766, ((int32_t)sample0 + (int32_t)sample1)))) / 2;

    i2s.write(sampleAdjusted);
    i2s.write(sampleAdjusted);
  }
  */

}

void loop1() {
  /*
  if (!dbuf0->isFinished() || !dbuf1->isFinished()) {
    dbuf0->populateWriteBuf();
    dbuf1->populateWriteBuf();
  }
  */

}
