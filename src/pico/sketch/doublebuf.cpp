
#include "doublebuf.h"
#include "util.h"

DoubleBuf::DoubleBuf(uint32_t soundId) {
  newSource(soundId);
}

DoubleBuf::~DoubleBuf() {
  closeSource();
}

// block first thread while initializing
bool DoubleBuf::newSource(uint32_t soundId) {

  Serial.println("Closing any old files");
  closeSource();

  Serial.println("Creating new wave file");
  currentSource = new Wav(soundId);

  curReadBuf = 0;
  curWriteBuf = 1;

  bytesReadUpTo = 0;
  bytesWriteUpTo = 0;

  dataSize = currentSource->dataSize();

  writeReady = true;

  //Serial.println("Populating write buf");
  populateWriteBuf();

  //Serial.println("Swapping bufs");
  swapBufs();

  Serial.println("Completed!");

  return true;
}

int32_t DoubleBuf::readNextSample() {

  if (isFinished()) {
    //Serial.println("ERROR MODE: trying to read bytes out of range");
    return 0;
  }

  uint16_t bytesPerSample = currentSource->bytesPerSample();
  uint32_t offset = bytesReadUpTo % SINGLE_BUFFER_SIZE;
  int32_t val = 0;

  while (bytesReadUpTo >= bytesWriteUpTo) {
    Serial.println("KINDA ERROR MODE: reading bytes too early. need to busy wait. returning nothing");
    dumpState();

    while(true) {}
  }

  for (int i = 0; i < bytesPerSample; i++) {
    val |= (buf[curReadBuf][offset+i] << (i*8));
  }

  bytesReadUpTo += bytesPerSample;

  if (isFinished()) {
    Serial.println("Closing source");
    closeSource();
  } else if (bytesReadUpTo % SINGLE_BUFFER_SIZE == 0) {
    swapBufs();
  }

  return val;
}

bool DoubleBuf::isFinished() {
  return (bytesReadUpTo == dataSize);
}


// called by first core (one that gets the samples), and once by the second core during initialization
bool DoubleBuf::swapBufs() {

  curReadBuf = (curReadBuf + 1) % 2;
  curWriteBuf = (curWriteBuf + 1) % 2;

  writeReady = true;

  return true;
}

// called by second core
bool DoubleBuf::populateWriteBuf() {

  if ((!writeReady) || isFinished()) {
    return false;
  }

  if (bytesWriteUpTo >= dataSize) {
    return false;
  }

  int32_t numBytesToWrite = min(SINGLE_BUFFER_SIZE, dataSize - bytesWriteUpTo);

  //Serial.print("New numBytesToWrite: ");
  //Serial.println(numBytesToWrite);

  if (numBytesToWrite <= 0) {
    Serial.println("ERROR - why are we trying to write zero bytes?");
    return false;
  }

  //Serial.print("Reading data from sd card. curWriteBuf: ");
  //Serial.println(curWriteBuf);

  //Serial.print("numBytesToWrite: ");
  //Serial.println(numBytesToWrite);

  // read from SD and write to the current writeBuf
  currentSource->readData(buf[curWriteBuf], numBytesToWrite);

  writeReady = false;

  bytesWriteUpTo += numBytesToWrite;
  Serial.println("Populating completed");
  return true;
}


bool DoubleBuf::closeSource() {
  if (currentSource != nullptr) {
    delete currentSource;
  }

  currentSource = nullptr;

  return true;
}

void DoubleBuf::dumpState() {

    Serial.println("Dump start -----------");
    Serial.print("curReadBuf: ");
    Serial.println(curReadBuf);
    Serial.print("curWriteBuf: ");
    Serial.println(curWriteBuf);

    Serial.print("bytesReadUpTo: ");
    Serial.println(bytesReadUpTo);
    Serial.print("bytesWriteUpTo: ");
    Serial.println(bytesWriteUpTo);

    Serial.print("writeReady: ");
    Serial.println(writeReady);

    Serial.print("dataSize: ");
    Serial.println(dataSize);

    Serial.println("Dump end -----------");

}