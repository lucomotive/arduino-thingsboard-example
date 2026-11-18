
#include "timer.h"

bool poll_timer(poll_timer_t *timer, unsigned long now) {
  return (now - timer->start_) >= timer->duration_;
}
