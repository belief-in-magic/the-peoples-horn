#include "buf.h"


Buf::Buf() {
  sectorsWritten = 0;
  numSectors = 0;
  dataSize = 0;
  currentSource = nullptr;
  nextSectorReady = false;
}

bool Buf::newSource(uint32_t sound) {
  // close the current source, if needed
  closeSource();

  currentSource = new Wav(sound);

  dataSize = currentSource->dataSize();
  numSectors = dataSize % SINGLE_BUFFER_SIZE == 0
    ? dataSize/SINGLE_BUFFER_SIZE
    : dataSize/SINGLE_BUFFER_SIZE + 1;

  markNextSectorReady();

  return true;
}

// TODO maybe we can remove the need to pass a pointer?
int32_t Buf::readSample(uint32_t ptr) {

  uint32_t val;

  // sector offset: which of the two sectors we should currently be reading from
  uint32_t so = ((sectorsWritten+1) % 2) * SINGLE_BUFFER_SIZE;


  for (int i = 0; i < 2; i++) {
    val |= (buf[so+i] << (i*8));
  }


  return val;
}

bool Buf::markNextSectorReady() {
  nextSectorReady = true;
  return nextSectorReady;
}

bool Buf::isNextSectorReady() {
  return nextSectorReady;
}

uint32_t Buf::prepareNextSector() {

  if (!nextSectorReady) {
    Serial.println("Trying to prepare sector that's not ready");
    return 0;
  }

  if (sectorsWritten >= numSectors) {
    Serial.println("No more sectors to prepare");
    return 0;
  }

  // sector offset: which of the two sectors we should currently be writing to
  uint32_t so = (sectorsWritten % 2) * SINGLE_BUFFER_SIZE;

  uint32_t bytesToWrite = SINGLE_BUFFER_SIZE;


  if (sectorsWritten+1 == numSectors) {

    bytesToWrite = dataSize % SINGLE_BUFFER_SIZE;

    // preemptively zero entire sector if it's the last sector
    for (int i = bytesToWrite; i < SINGLE_BUFFER_SIZE; i++) {
      buf[i+so] = 0;
    }
  }

  bool res = currentSource->readData(buf + so, bytesToWrite);

  if (!res) {
    Serial.println("Error trying to read from SD card");
  }

  sectorsWritten++;
  nextSectorReady = false;

  return sectorsWritten-1;
}


void Buf::closeSource() {
  if (currentSource != nullptr) {
    delete currentSource;
    currentSource = nullptr;
  }
}


