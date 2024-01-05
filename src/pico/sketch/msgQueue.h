/*
The heart of inter-process communications between core0 and core1 of the rp2040.
This is ultimately based on the rp2040's two 8-element FIFO cores. These queues are pretty small so extra caution
for the volume of events passed, as well as how to react when the queue is full, should be taken.
*/


#define OP_CODE_LEN 2

#define STOP_VAL_LEN 4 // representing 4 sound buffers. lowest bits

#define READY_BUF_ID_LEN 4
#define READY_BUF_SECTOR_LEN 26

enum OpCode {
  STOP,
  READY,
  CHANGE, // TODO
  BLAH,   // TODO
}

typedef uint32_t Message;

inline OpCode getOpCode(Message m) {
  return m >> (32 - OP_CODE_LEN);
}

inline bool isStop(Message m) {
  return getOpCode(m) == STOP;
}

inline bool isReady(Message m) {
  return getOpCode(m) == READY;
}

inline bool stopMsgContainsBuf(Message m, uint8_t bufId) {
  return ((m >> bufId) % 2) == 1;
}

inline uint8_t readyMsgGetBuf(Message m) {
  return (m << OP_CODE_LEN) >> (32 - READY_BUF_ID_LEN);
}

inline uint32_t readyMsgGetSector(Message m) {
  return (m << (OP_CODE_LEN+READY_BUF_ID_LEN)) >> (32 - READY_BUF_SECTOR_LEN);
}