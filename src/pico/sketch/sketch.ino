
#include <I2S.h>
#include <SPI.h>
#include <SD.h>


#include "msg.h"


using namespace msg;


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
  //Serial.println("Starting setup for first core");

  //rp2040.fifo.push(0);
  //rp2040.fifo.pop(); // wait for the second core's setup to complete first

  //Serial.println("Starting setup for first core, waiting done");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }


  // msg test

  Message m = sectorReadyMsg(3, 127);
  Serial.print("msg: ");
  Serial.println(m, BIN);
  Serial.println(isStop(m));
  Serial.println(isReady(m));
  Serial.println(isNoop(m));

  Serial.println(readyMsgGetBuf(m));
  Serial.println(readyMsgGetSector(m));

  m = stopMsgEmpty();
  m = stopMsgWithBuf(m,0);
  m = stopMsgWithBuf(m,3);

  Serial.print("msg: ");
  Serial.println(m, BIN);
  Serial.println(isStop(m));
  Serial.println(isReady(m));
  Serial.println(isNoop(m));

  Serial.println(stopMsgContainsBuf(m, 0));
  Serial.println(stopMsgContainsBuf(m, 1));
  Serial.println(stopMsgContainsBuf(m, 2));
  Serial.println(stopMsgContainsBuf(m, 3));


  //state.core0_stateSetup(&i2s);

  /*
  // TEST

    Serial.print("Initializing SD card...");
    SPI.setRX(pSD_MISO);
    SPI.setTX(pSD_MOSI);
    SPI.setSCK(pSD_SCK);

    if (!SD.begin(pSD_CS)) {
      Serial.println("initialization failed!");
      return;
    }

    Serial.println("done!");

  Serial.println("Starting test for sd card read");

  File f[4];

  f[0] = SD.open("0001.wav", FILE_READ);
  f[1] = SD.open("0010.wav", FILE_READ);
  f[2] = SD.open("0011.wav", FILE_READ);
  f[3] = SD.open("0100.wav", FILE_READ);

  uint8_t buf[4][2][20000];

  int c = 0;
  while (true) {
    bool stillRead = false;
    for (int i = 0; i < 4; i++) {
      Serial.print(i);
      Serial.print(": Position: ");
      Serial.print(f[i].position());
      Serial.print(", out of ");
      Serial.println(f[i].size());

      uint32_t t1 = millis();
      f[i].read(buf[i][c], 20000);
      uint32_t t2 = millis();

      Serial.print("Millis time to read: ");
      Serial.println(t2-t1);

      if (f[i].position() != f[i].size()) {
        stillRead = true;
      } else {
        f[i].close();
      }
    }

    if (stillRead == false) {
      Serial.println("done all");
      break;
    }

    c = (c+1)%2;


  }
  */

}


void setup1() {
/*
  rp2040.fifo.pop();
  Serial.println("Starting setup for second core");

  state.core1_stateSetup();

  rp2040.fifo.push(0);
*/
}


void loop() {
  //state.core0_stateLoop();
  //Serial.println(rp2040.fifo.available());
  /*
  int i;
  uint32_t t1 = millis();

  uint32_t k;
  for (i = 0; i < 44000; i++) {

    if (rp2040.fifo.push_nb((uint32_t)i) == false) {
      break;
    }

  }

  uint32_t t2 = millis();


  Serial.print("Total time in millis: ");
  Serial.println(t2-t1);

  Serial.print("i: ");
  Serial.println(i);

  Serial.println(rp2040.fifo.available());

  while(true) {}
  */
}


void loop1() {

  //delay(4000);
  //Serial.println(rp2040.fifo.available());
}

