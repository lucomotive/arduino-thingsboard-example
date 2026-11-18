// run with
// pio run -e telemetry -t upload && pio device monitor

#include "_env"

#include "PubSubClient.h"
#include "WiFi.h"
#include "WiFiSSLClient.h"
#include "common/mqtt.h"
#include "common/timer.h"
#include "common/wifi.h"

// setup a wifi client and mqtt client
WiFiSSLClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(LED_BUILTIN, OUTPUT);

  connectWiFi(WIFI_SSID, WIFI_PASSWD);
  mqttClient.setServer(SERVER_URL, MQTT_PORT);
}

// setup timers for async programming

// timer and variables for telemetry
// timer and variables for telemetry
poll_timer_t telemetry_timer{.start_ = 0, .duration_ = 3300};
int temp = 0;

void loop() {
  mqtt::connect(mqttClient, CLIENT_ID, USER_NAME, PASSWD);
  mqttClient.loop();

  // read sensor values
  temp = random() % 16 + 70; // in a real application, read the sensor

  // get the current time
  unsigned long now = millis();

  // check the timer and run the telemetry
  bool rssi_rdy = poll_timer(&telemetry_timer, now);
  if (rssi_rdy) {
    telemetry_timer.start_ = now; // reset
    mqtt::send_telemetry(mqttClient, "temp", temp);
  }
}
