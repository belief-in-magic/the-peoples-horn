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

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

// GPIO pin numbers
#define pBCLK 26
#define pWS (pBCLK+1)
#define pDOUT 28

// GPIO pin numbers for main buttons
#define pB1 12
#define pB2 13
#define pB3 14
#define pB4 15

// GPIO for sd card in spi mode (pico w tested only, you may need to change this depending on hardware)
#define pSD_CS 5
#define pSD_SCK 6
#define pSD_MOSI 7
#define pSD_MISO 4

const int sampleRate = 44100;

int count = 0;
int led = 1;

DoubleBuf* dbuf;

bool setUpSD() {

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


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  Serial.println("Starting");

  setUpSD();

  Serial.println("Starting double buffer...");
  dbuf = new DoubleBuf("s11.wav");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  digitalWrite(LED_BUILTIN, led);

  rp2040.fifo.push(1);
}

void setup1() {
  // wait on first core to finish setup
  rp2040.fifo.pop();

}

void loop() {

  if (!dbuf->isFinished()) {
    int32_t sample = dbuf->readNextSample();
    int16_t sampleAdjusted = ((int16_t) sample) / 2;

    i2s.write(sampleAdjusted);
    i2s.write(sampleAdjusted);
  }
}

void loop1() {
   if (!dbuf->isFinished()) {
     dbuf->populateWriteBuf();
   }
}