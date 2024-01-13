/*
The heart of inter-process communications between core0 and core1 of the rp2040.
This is ultimately based on the rp2040's two 8-element FIFO cores. These queues are pretty small so extra caution
for the volume of events passed, as well as how to react when the queue is full, should be taken.
*/
#pragma once

#include <cstdint>

#define OP_CODE_LEN 2
#define STOP_VAL_LEN 4 // representing 4 sound buffers. lowest bits
#define READY_BUF_ID_LEN 4
#define READY_BUF_SECTOR_LEN 26

namespace msg {

enum OpCode {
  STOP,
  READY,
  CHANGE, // TODO
  NOOP   // TODO
};

typedef uint32_t Message;


OpCode getOpCode(Message m);
bool isStop(Message m);
bool isReady(Message m);
bool isNoop(Message m);

bool stopMsgContainsBuf(Message m, uint8_t bufId);
Message stopMsgEmpty();
Message stopMsgWithBuf(Message prev, uint8_t buffer);

uint8_t readyMsgGetBuf(Message m);
uint32_t readyMsgGetSector(Message m);

Message sectorReadyMsg(uint8_t buffer, uint32_t sectorId);

uint32_t firstNBits(uint32_t val, uint8_t n);
uint32_t setFirstNBits(uint8_t n);


} // namespace msg