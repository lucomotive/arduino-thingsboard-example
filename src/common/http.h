

#ifndef HTTP_H
#define HTTP_H

#include "ArduinoHttpClient.h"
#include "ArduinoJson.h"
#include "HttpClient.h"
#include "_env"

namespace http {

template <typename V>
int send_telemetry(HttpClient &http, const char *key, const V &value) {
  JsonDocument doc;
  doc[key] = value;

  char payload[128];
  serializeJson(doc, payload, sizeof(payload));

  char post[128];
  sprintf(post, "/api/v1/%s/telemetry", USER_NAME);

  // shttp.beginRequest();
  http.post(post, "application/json", payload);
  // http.sendHeader("Content-Type", "application/json");
  // http.sendHeader("Content-Length", strlen(payload));
  // http.beginBody();
  // http.print(payload);
  // http.endRequest();

  Serial.print("Telemetry sent: ");
  Serial.println(payload);

  int status = http.responseStatusCode();
  auto reply = http.responseBody();

  Serial.print("Telemetry reply: ");
  Serial.println(reply);
  Serial.print(key);
  Serial.print(" -> HTTP ");
  Serial.println(status);

  return status >= 200 && status < 300;
}

}; // namespace http

#endif
