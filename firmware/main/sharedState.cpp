#include "sharedState.h"


SharedState::SharedState() {

  queue_init(&core0Fifo, sizeof(msg::Message), FIFO_LENGTH);
  queue_init(&core1Fifo, sizeof(msg::Message), FIFO_LENGTH);

  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    buffers[i] = Buf(); // TODO is this needed?
  }

}

bool SharedState::sendMsgToCore0(msg::Message m) {
  return queue_try_add(&core0Fifo, &m);
}

bool SharedState::sendMsgToCore1(msg::Message m) {
  return queue_try_add(&core1Fifo, &m);
}

uint32_t SharedState::availableMessagesCore0() {
  return (uint32_t) queue_get_level(&core0Fifo);
}

uint32_t SharedState::availableMessagesCore1() {
  return (uint32_t) queue_get_level(&core1Fifo);
}

msg::Message SharedState::popMsgCore0() {
  msg::Message m;

  queue_remove_blocking(&core0Fifo, &m);
  return m;
}

msg::Message SharedState::popMsgCore1() {
  msg::Message m;

  queue_remove_blocking(&core1Fifo, &m);
  return m;
}
