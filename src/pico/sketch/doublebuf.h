

#define SINGLE_BUFFER_SIZE 20000

// Maybe abstract to stream
class DoubleBuf {

  uint8_t buf[2][SINGLE_BUFFER_SIZE];

  Wav* currentSource = nullptr;

  uint8_t curReadBuf;
  uint8_t curWriteBuf;

  uint32_t curWriteBufOffset;
  uint32_t curWriteBufSize;
  uint32_t curReadPtr; // can probably be generalized
  uint32_t curReadBufSize;

  bool hasFinishedWritingCurBuf;

  public:
    DoubleBuf(const char* filename);
    ~DoubleBuf();

    // Used by the first core to read samples
    bool newSource(const char* filename);
    bool closeSource();
    bool isFinished();
    uint32_t readNextSample(); // will block if not ready

    // Used by second core to write from the WAV file
    bool writeBufReady();
    bool readIntoWriteBuf();

}