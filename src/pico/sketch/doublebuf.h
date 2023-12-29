
#pragma once

#include "wav.h"

#define SINGLE_BUFFER_SIZE 20000

// Maybe abstract to stream
class DoubleBuf {

  uint8_t buf[2][SINGLE_BUFFER_SIZE];

  uint8_t curReadBuf;  // which buf to read samples from
  uint8_t curWriteBuf; // which buf to write data to (back buffer)

  uint32_t bytesReadUpTo;
  uint32_t bytesWriteUpTo;

  uint32_t dataSize;

  bool writeReady;

  Wav* currentSource = nullptr;


  private:

    // Used by first core
    bool swapBufs();

  public:
    DoubleBuf(const char* filename);
    ~DoubleBuf();

    // Used by the first core to read samples
    bool closeSource();
    bool isFinished();
    int32_t readNextSample(); // will block if not ready

    // Used by second core to write from the WAV file
    bool newSource(const char* filename);
    bool populateWriteBuf();

    // temp
    void dumpState();
};