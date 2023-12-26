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

const int frequency = 440; // frequency of square wave in Hz
const int amplitude = 700; // amplitude of square wave
const int sampleRate = 16000;

const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

int16_t sample = amplitude; // current sample value
int count = 0;
int led = 1;

bool setUpSD() {

  Serial.print("Initializing SD card...");
  SPI.setRX(pSD_MISO);
  SPI.setTX(pSD_MOSI);
  SPI.setSCK(pSD_SCK);

  if (!SD.begin(pSD_CS)) {
    Serial.println("initialization failed!");
    return 1;
  }

  if (SD.exists("s01.wav")) {
    Serial.println("s01.wav exists.");
  } else {
    Serial.println("s01.wav doesn't exist.");
  }

  Serial.println("s01.wav opening.");
  File wav = SD.open("s01.wav");

  if (!wav) {
    Serial.println("Cannot open wav file");
    return 1;
  }

  if (wav.seek(0)) {
    Serial.println("Failure seeking");
  }

  Serial.println("Reading header from file");
  char hdrBuf[100];
  wav.readBytes(hdrBuf, 100);

  for (int i=0; i < 50; i++) {
    Serial.print(hdrBuf[i], HEX);
    Serial.print(" ");
  }

  Serial.println("");

  for (int i=50; i < 100; i++) {
    Serial.print(hdrBuf[i], HEX);
    Serial.print(" ");
  }

  Serial.println("");


  wav.close();

  Serial.println("done!");
  return 0;
}




void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  delay(2000);

  Serial.println("Starting");

  setUpSD();

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  digitalWrite(LED_BUILTIN, led);
}



void loop() {
  if (count % halfWavelength == 0) {
    // invert the sample every half wavelength count multiple to generate square wave
    sample = -1 * sample;
  }

  // TODO: advance tick

  // TODO: calculate sample based on current state

  // write the same sample twice, once for left and once for the right channel
  i2s.write(sample);
  i2s.write(sample);

  // increment the counter for the next sample
  count++;

  if (count % sampleRate == 0) {
    led = (led+1) % 2;
    digitalWrite(LED_BUILTIN, led);
    Serial.println("I2S simple tone heartbeat");
  }
}