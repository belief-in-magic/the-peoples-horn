
#pragma once

#define SINGLE_BUFFER_SIZE 20000
#define DOUBLE_BUFFER_SIZE (2*SINGLE_BUFFER_SIZE)

class Buf {

  uint8_t buf[DOUBLE_BUFFER_SIZE];
  uint32_t dataSize;
  uint32_t numSectors;

  uint32_t sectorsWritten;

  Wav* currentSource;

  bool nextSectorReady = false;

  private:
    void closeSource();

  public:
    Buf();

    // only used by the first core
    int32_t readSample(uint32_t ptr);

    // following are used by the second core
    bool newSource(uint32_t sound);
    bool markNextSectorReady();
    bool isNextSectorReady();
    uint32_t prepareNextSector(); // expensive operation, returns the prepared sector num

};