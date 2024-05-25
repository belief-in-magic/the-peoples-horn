
#include "buf.h"
#include "wav.h"

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

  if (!currentSource->isSuccessOpen()) {
      return false;
  }

  sectorsWritten = 0;
  dataSize = currentSource->dataSize();
  numSectors = dataSize % SINGLE_BUFFER_SIZE == 0
    ? dataSize/SINGLE_BUFFER_SIZE
    : dataSize/SINGLE_BUFFER_SIZE + 1;

  //Serial.print("core1: creating new sound in buffer. Datasize:");
  //Serial.print(dataSize);
  //Serial.print(", sectors: ");
  //Serial.println(numSectors);


  markNextSectorReady();

  return true;
}

// TODO maybe we can remove the need to pass a pointer?
int32_t Buf::readSample(uint32_t ptr) {

  uint32_t val;

  for (int i = 0; i < 2; i++) {
    val |= (buf[ptr+i] << (i*8));
  }

  return val;
}

bool Buf::markNextSectorReady() {
  nextSectorReady = true;
  return nextSectorReady;
}

bool Buf::isNextSectorReady() {
  return nextSectorReady && !(sectorsWritten >= numSectors);
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

    Serial.print("sector offset: ");
    Serial.println(so);

    Serial.print("bytes to write: ");
    Serial.println(bytesToWrite);

    return -1;
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


uint32_t Buf::getNumSectors() {
  return numSectors;
}
