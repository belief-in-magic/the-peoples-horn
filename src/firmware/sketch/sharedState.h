
#pragma once

// This is limited by both memory and compute constraints
#define MAX_CONCURRENT_SOUNDS 4

#define FIFO_LENGTH 64

#include <cstdint>
#include "buf.h"
#include "msg.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

class SharedState {

  queue_t core0Fifo; // queue that the first core reads from
  queue_t core1Fifo; // queue that the second core reads from

  public:
    SharedState();

    Buf buffers[MAX_CONCURRENT_SOUNDS];

    // will return false if failed to add (queue is full)
    bool sendMsgToCore0(msg::Message m);
    bool sendMsgToCore1(msg::Message m);

    uint32_t availableMessagesCore0();
    uint32_t availableMessagesCore1();

    // BLOCKING, do not call unless you know there are messages
    msg::Message popMsgCore0();
    msg::Message popMsgCore1();
};