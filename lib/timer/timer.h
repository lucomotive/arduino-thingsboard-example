#ifndef POLL_TIMER_H
#define POLL_TIMER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  unsigned long start_;
  unsigned long duration_;
} poll_timer_t;

// returns true when timer is elapsed
bool poll_timer(poll_timer_t *timer, unsigned long now);

#ifdef __cplusplus
}
#endif

#endif
