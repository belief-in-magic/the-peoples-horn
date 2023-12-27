
#include "doublebuf.h"
#include "util.h"

DoubleBuf::DoubleBuf(const char* filename) {
  newSource(filename);
}

DoubleBuf::~DoubleBuf() {
  closeSource();
}

// block first thread while initializing
bool DoubleBuf::newSource(const char* filename) {

  closeSource();

  currentSource = new Wav(filename);

  curReadBuf = 0;
  curWriteBuf = 1;

  bytesReadUpTo = 0;
  bytesWriteUpTo = 0;

  swapReady = true;
  hasFinishedWritingCurBuf = false;

  return swapAndPopulateWriteBuf();
}

uint32_t DoubleBuf::readNextSample() {

  uint16_t bytesPerSample = file.bytesPerSample();
  uint32_t offset = bytesReadUpTo % SINGLE_BUFFER_SIZE;
  uint32_t val = 0;

  if (isFinished()) {
    Serial.println("ERROR MODE: trying to read bytes out of range");
    return 0;
  }

  while (bytesReadUpTo >= bytesWriteUpTo) {
    Serial.println("KINDA ERROR MODE: reading bytes too early. need to busy wait. returning nothing");
    return 0;
  }

  for (int i = 0; i < bytesPerSample; i++) {
    val |= (buf[curReadBuf][offset+i] << (i*8)); // TODO might be other order?
  }

  bytesReadUpTo += bytesPerSample;

  if (isFinished()) {
    closeSource();
  }

  else if (bytesReadUpTo % SINGLE_BUFFER_SIZE == 0) {
    swapBufs();
  }

  return val;
}

bool DoubleBuf::isFinished() {
  return bytesReadUpTo == file.dataSize();
}


// called by first core (one that gets the samples)
bool DoubleBuf::swapBufs() {

  curReadBuf = (curReadBuf + 1) % 2;
  curWriteBuf = (curWriteBuf + 1) % 2;

  // no need to write if this swap will be the last
  if (bytesWriteUpTo + SINGLE_BUFFER_SIZE <= file.dataSize()) {
    writeReady = true;
  }
}

// called by second core
bool DoubleBuf::populateWriteBuf() {

  if (! writeReady) {
    Serial.println("ERROR: write not ready")
  }

  int32_t numBytesToWrite = min(SINGLE_BUFFER_SIZE, currentSource.dataSize() - bytesWriteUpTo);

  if (numBytesToWrite <= 0) {
    Serial.println("ERROR - why are we trying to write zero bytes?");
    return false;
  }

  // read from SD and write to the current writeBuf
  file.read(buf[curWriteBuf], numBytesToWrite);

  writeReady = false;

  bytesWriteUpTo += numBytesToWrite;

  return true;
}


DoubleBuf::closeSource() {
  if (currentSource != nullptr) {
    delete currentSource;
  }

  currentSource = nullptr;
}


