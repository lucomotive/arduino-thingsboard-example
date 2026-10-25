#ifndef TELEMETRY_TASK_H
#define TELEMETRY_TASK_H

#include "PubSubClient.h"
#include "timer.h"

template <typename V> class TelemetryTask {
public:
  TelemetryTask(float freq, const char *key, const V &value);
  bool poll(PubSubClient &);

private:
  Timer timer_;
  const char *key_;
  const V &ref_;
};

#endif
