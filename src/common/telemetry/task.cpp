

#include "task.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "WiFiNINA.h"

// this is the mqtt topic the telemetry posts to.
// it is defined by thingsboard, to be accepted as telemetry by the server.
//
// other servers and mqtt broker might use other topics
const char TOPIC[] = "v1/devices/me/telemetry";

// this initializes the telemetry task.
// it only takes in a frequency, how often the device sends telemetry per second
template <typename V>
TelemetryTask<V>::TelemetryTask(float freq, const char *key, const V &value)
    : timer_(1000.0f / freq), key_(key), ref_(value) {}

// like the blink task, this functions polls the TelemetryTask.
// it waits for the timer, checks if the mqtt client is connected and sends a
// simple rssi value to the server
template <typename V> bool TelemetryTask<V>::poll(PubSubClient &client) {
  if (!timer_.elapsed())
    return false;
  timer_.restart();

  // only works if mqtt is connected
  if (!client.connected()) {
    Serial.println("Client not connected. Can not send telemetry.");
    return true;
  }

  // thingsBoard expects JSON on this topic
  JsonDocument doc;
  doc[key_] = ref_;

  char payload[128];
  serializeJson(doc, payload, sizeof(payload));

  client.publish(TOPIC, payload);

  Serial.print("Telemetry sent: ");
  Serial.println(payload);
}

// needed for linker
template class TelemetryTask<float>;
template class TelemetryTask<int>;
