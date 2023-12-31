

#include "inputStream.h"

#include <Arduino.h>

#define HALT_INTERRUPTS noInterrupts();
#define RESUME_INTERRUPTS interrupts();

InputStream::InputStream() {
  HALT_INTERRUPTS

  writePtr = 0;
  readPtr = 0;

  RESUME_INTERRUPTS
}

uint32_t InputStream::numQueuedEvents() {
  uint32_t val;

  HALT_INTERRUPTS

  if (writePtr >= readPtr) {
    val = writePtr-readPtr;
  } else {
    val = INPUT_BUFFER_SIZE - (readPtr - writePtr);
  }

  RESUME_INTERRUPTS

  return val;
}

void InputStream::push(InputEvent event) {

  uint32_t nextWritePtr;

  HALT_INTERRUPTS

  nextWritePtr = (writePtr+1) % INPUT_BUFFER_SIZE;

  if (nextWritePtr != readPtr) {
    // only push if queue is not full
    buf[writePtr] = event;
    writePtr = nextWritePtr;
  }

  RESUME_INTERRUPTS
}


InputEvent InputStream::next() {
  InputEvent event;

  HALT_INTERRUPTS

  if (readPtr == writePtr) {
    Serial.println("Trying to pop from empty queue! Returning garbage :( ");
  } else {
    event = buf[readPtr];
    readPtr = (readPtr+1) % INPUT_BUFFER_SIZE;
  }

  RESUME_INTERRUPTS

  return event;
}