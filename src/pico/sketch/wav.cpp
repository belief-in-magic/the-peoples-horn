
#include <string.h>
#include "wav.h"
#include "util.h"

// TODO figure out cpp exception handling
Wav::Wav(uint32_t soundId) {

  char hdrBuf[RIFF_CHUNK_SIZE + FMT_CHUNK_SIZE];

  writeFileName(soundId);

  file = SD.open(filename, FILE_READ); // TODO check ownership?
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


  if (numChannels != 1) {
    Serial.println("Only single channel wav files are supported.");
    return;
  }

  if (sampleRate > 50000) {
    Serial.println("Sample rates above 50k are not supported.");
    return;
  }

  if (bitsPerSample > 32) {
    Serial.println("Bit per sample cannot be above 32.");
    return;
  }

  if (!seekDataChunk()) {
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
  Serial.println(dataChunkBlobSize);
  Serial.println("End Info");
  */

}

// Seeks the start of the data blob (datachunk start + 8). Populates the dataChunkOffset and dataChunkBlobSize class variables.
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
      dataChunkBlobSize = curChunkSize;
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

  if (file.position() + size > dataChunkOffset + dataChunkBlobSize + 8) {
    Serial.print("Attempted to read out of bounds data");
    return false;
  }

  //Serial.print("core1 - Reading SD card. From: ");
  //Serial.println(file.position());

  //Serial.print("core1 - size: ");
  //Serial.println(size);

  int v = file.read(buf, size);

  if (v == -1) {
    Serial.print("WAV Could not read: ");
    Serial.println(filename);

    Serial.print("File size: ");
    Serial.println(file.size());

    Serial.print("Size to read: ");
    Serial.println(size);

    Serial.print("Current position: ");
    Serial.println(file.position());

    Serial.print("Bytes available: ");
    Serial.println(file.available());

    Serial.print("Testing if can peek: ");
    Serial.println(file.peek());

    return false;
  }

  //Serial.println("core1 - Done reading from sd card");

  return true;
}

uint32_t Wav::dataSize() {
  return dataChunkBlobSize;
}

uint16_t Wav::bytesPerSample() {
  return bitsPerSample/8;
}

Wav::~Wav() {
  file.close();
}


void Wav::writeFileName(uint32_t soundId) {

  int i;
  for (i = 0; i < 4; i++) {
    if ((soundId >> (3-i)) & 1 == 1) {
      filename[i] = '1';
    } else {
      filename[i] = '0';
    }
  }
  sprintf(filename+i, ".wav");

  //Serial.print("FILENAME: ");
  //Serial.println(filename);
}