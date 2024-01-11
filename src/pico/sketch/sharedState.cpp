#include "sharedState.h"


SharedState::SharedState() {
  for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++) {
    buffers[i] = Buf(); // TODO is this needed?
  }

}
