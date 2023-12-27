
#include <string.h>
#include "wav.h"
#include "util.h"


Wav::Wav(const char* filename) {

  char hdrBuf[RIFF_CHUNK_SIZE + FMT_CHUNK_SIZE];

  file = SD.open(filename);
  if (!file) {
    // TODO find thread safe way to print serial
    Serial.print("Cannot open wav file:");
    Serial.println(filename);
    return;
  }

  if (file.size() < RIFF_CHUNK_SIZE + FMT_CHUNK_SIZE) {
    Serial.println("File is too small");
    return;
  }

  file.readBytes(hdrBuf, RIFF_CHUNK_SIZE + FMT_CHUNK_SIZE);

  if (strncmp(hdrBuf, "RIFF", 4) != 0) {
    Serial.println("File is not RIFF format");
    return;
  }

  if (strncmp(hdrBuf + 8, "WAVE", 4) != 0) {
    Serial.println("File is not WAV format");
    return;
  }

  // TODO: add check for endianness
  audioFormat =   tos(hdrBuf+20);
  numChannels =   tos(hdrBuf+22);
  sampleRate =    tol(hdrBuf+24);
  bitsPerSample = tos(hdrBuf+34);

  if (! seekDataChunk()) {
    Serial.println("Could not find data chunk");
    return;
  }

 /*
  Serial.println("Info: ");
  Serial.println(audioFormat);
  Serial.println(numChannels);
  Serial.println(sampleRate);
  Serial.println(bitsPerSample);
  Serial.println(dataChunkOffset);
  Serial.println(dataChunkSize);
  Serial.println("End Info");
  */

}

// Seeks the start of the data blob (datachunk start + 8). Populates the dataChunkOffset and dataChunkSize class variables.
bool Wav::seekDataChunk() {

  char hdrBuf[8];
  uint32_t fileSize = file.size();
  uint32_t curChunkSize;
  uint32_t curPos = RIFF_CHUNK_SIZE + FMT_CHUNK_SIZE;

  file.seek(curPos);

  while (curPos <= fileSize - 8) {
    file.readBytes(hdrBuf, 8);
    curChunkSize = tol(hdrBuf+4);

    if (strncmp(hdrBuf, "data", 4) == 0) {
      dataChunkOffset = curPos;
      dataChunkSize = curChunkSize;
      return true;
    } else {
      curPos += 8 + curChunkSize;
      file.seek(curPos);
    }
  }

  return false;
}

bool Wav::readData(uint8_t* buf, uint32_t size) {

  if (size % (bitsPerSample/8) != 0) {
    Serial.print("Read data size not multiple of bitsPerSample");
    return false;
  }

  if (file.position() + size > dataChunkOffset + dataChunkSize + 8) {
    Serial.print("Attempted to read out of bounds data");
    return false;
  }

  file.read(buf, size);
  return true;
}

Wav::~Wav() {
  file.close();
}
