#include "core1.h"
#include "sharedState.h"

Core1State::Core1State(SharedState* ss) {

  sharedState = ss;

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    highestAckedSector[i] = 0;
    prepareNext[i] = false;
  }

  setUpInput();
  setUpSD();

}


void Core1State::setUpInput() {
  pinMode(SB0, INPUT);
  pinMode(SB1, INPUT);
  pinMode(SB2, INPUT);
  pinMode(SB3, INPUT);
}

void Core1State::setUpSD() {
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


void Core1State::prepareNextSector(uint8_t buf) {

  ((sharedState->buffers)[buf]).prepareNextSector();

}