#ifndef MQTT_H
#define MQTT_H

#include "ArduinoJson.h"
#include "PubSubClient.h"

namespace mqtt {
// this function connects the mqtt client to the server
void connect(PubSubClient &client, const char *const id,
             const char *const username, const char *const passwd) {
  while (!client.connected()) {
    Serial.print("Connecting to ThingsBoard...");
    // clientId, username (token), password (none)
    if (client.connect(id, username, passwd))
      Serial.println("connected");
    else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// this is the telemetry topic the arduino posts to.
// it is defined by thingsboard.
// it is necessary for the message to be accepted as telemetry by the server.
//
// other servers and mqtt broker might use other topics
const char TELEMETRY_TOPIC[] = "v1/devices/me/telemetry";

template <typename V>
void send_telemetry(PubSubClient &mqtt, const char *key, const V &value) {
  // thingsBoard expects JSON on this topic
  JsonDocument doc;
  doc[key] = value;

  char payload[128];
  serializeJson(doc, payload, sizeof(payload));

  mqtt.publish(TELEMETRY_TOPIC, payload);

  Serial.print("Telemetry sent: ");
  Serial.println(payload);
}

}; // namespace mqtt

#endif
