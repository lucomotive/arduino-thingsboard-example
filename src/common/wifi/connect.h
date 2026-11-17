#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include "Arduino.h"
#include "WiFiNINA.h"

// this function connects the device to a wifi network
void connectWiFi(const char *const ssid, const char *const passwd = NULL);

#endif
