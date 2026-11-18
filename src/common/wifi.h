#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include "Arduino.h"
#include "WiFiNINA.h"

namespace wifi {

// this function connects the device to a wifi network
void connect(const char *const ssid, const char *const passwd) {
  if (passwd)
    WiFi.begin(ssid, passwd);
  else
    WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.println("WiFi connected");
}

}; // namespace wifi

#endif
