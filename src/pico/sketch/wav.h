#pragma once

#include <SD.h>
#include <cstdint>

#define RIFF_CHUNK_SIZE 12
#define FMT_CHUNK_SIZE 24
#define CHUNK_ID_SIZE 4
#define CHUNK_SIZE_SIZE 4

#define MAX_FILENAME_LEN 128

class Wav {

    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint16_t bitsPerSample;

    uint32_t dataChunkOffset;
    uint32_t dataChunkBlobSize;

    char filename[MAX_FILENAME_LEN];

    File file;

  public:
    Wav(uint32_t soundId);
    ~Wav();
    bool readData(uint8_t* buf, uint32_t size);
    uint32_t dataSize();
    uint16_t bytesPerSample();

  private:
    bool seekDataChunk();
    void writeFileName(uint32_t soundId);
};
