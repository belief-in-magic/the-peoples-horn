
#include "doublebuf.h"

DoubleBuf::DoubleBuf(const char* filename) {
  newSource(filename);
}

DoubleBuf::~DoubleBuf() {
  closeSource();
}

DoubleBuf::newSource(const char* filename) {

  closeSource();

  currentSource = new Wav(filename);

  curReadBuf = 0;
  curWriteBuf = 1;

  curReadPtr = 0;
  curWritBufOffset = 0;

  readIntoWriteBuf();

  //hasFinishedWritingCurBuf =
}

DoubleBuf::readIntoWriteBuf() {


}


DoubleBuf::closeSource() {
  if (currentSource != nullptr) {
    delete currentSource;
  }

  currentSource = nullptr;
}


