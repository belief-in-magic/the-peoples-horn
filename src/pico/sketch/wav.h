#pragma once

#include <SD.h>

#define RIFF_CHUNK_SIZE 12
#define FMT_CHUNK_SIZE 24
#define CHUNK_ID_SIZE 4
#define CHUNK_SIZE_SIZE 4

class Wav {

    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint16_t bitsPerSample;

    uint32_t dataChunkOffset;
    uint32_t dataChunkBlobSize;

    File file;

  public:
    Wav(const char* path);
    ~Wav();
    bool readData(uint8_t* buf, uint32_t size);
    uint32_t dataSize();
    uint16_t bytesPerSample();

  private:
    bool seekDataChunk();
};
