#include "msg.h"
#include <Arduino.h>

namespace msg {


OpCode getOpCode(Message m) {
  return static_cast<OpCode>(m >> (32 - OP_CODE_LEN));
}

bool isStop(Message m) {
  return getOpCode(m) == STOP;
}

bool isReady(Message m) {
  return getOpCode(m) == READY;
}

bool isNoop(Message m) {
  return getOpCode(m) == NOOP;
}

bool isDone(Message m) {
  return getOpCode(m) == DONE;
}


bool stopMsgContainsBuf(Message m, uint8_t bufId) {
  return ((m >> bufId) % 2) == 1;
}

uint8_t readyMsgGetBuf(Message m) {
  return (m << OP_CODE_LEN) >> (32 - READY_BUF_ID_LEN);
}

uint32_t readyMsgGetSector(Message m) {
  return (m << (OP_CODE_LEN+READY_BUF_ID_LEN)) >> (32 - READY_BUF_SECTOR_LEN);
}

Message sectorReadyMsg(uint8_t buffer, uint32_t sectorId) {

  return (READY << (32 - OP_CODE_LEN))
         | (firstNBits(buffer, READY_BUF_ID_LEN) << (32 - (OP_CODE_LEN + READY_BUF_ID_LEN)))
         | firstNBits(sectorId, READY_BUF_SECTOR_LEN);
}


Message stopMsgEmpty() {
  return STOP << (32 - OP_CODE_LEN); // I guess this isn't technically needed
}

Message stopMsgWithBuf(Message prev, uint8_t buffer) {
  return prev | (1 << buffer);
}


uint32_t firstNBits(uint32_t val, uint8_t n) {
  return val & setFirstNBits(n);
}

uint32_t setFirstNBits(uint8_t n) {
  if (n == 32) {
    return -1;
  }

  return (1 << n) - 1;
}

Message doneMsg(uint8_t bufId) {
  return (DONE << (32 - OP_CODE_LEN)) | ((uint32_t) bufId);
}

uint8_t doneMsgGetBuf(Message m) {
  return (m << OP_CODE_LEN) >> OP_CODE_LEN;
}


} // namespace msg