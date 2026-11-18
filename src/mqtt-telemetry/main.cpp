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

  // setup led for blinking task
  pinMode(LED_BUILTIN, OUTPUT);

  wifi::connect(WIFI_SSID, WIFI_PASSWD);
  mqttClient.setServer(SERVER_URL, MQTT_PORT);
}

// setup timers for async programming
poll_timer_t rssi_telemetry_timer{.start_ = 0, .duration_ = 2000};
poll_timer_t temp_telemetry_timer{.start_ = 0, .duration_ = 3300};
poll_timer_t blink_timer{.start_ = 0, .duration_ = 1000};
bool led_state = false;

void loop() {
  mqtt::connect(mqttClient, CLIENT_ID, USER_NAME, PASSWD);
  mqttClient.loop();

  // read sensor values
  const int rssi = WiFi.RSSI();
  const int temp = random() % 16 + 70; // in a real application, read the sensor

  // get the current time
  const unsigned long now = millis();

  const bool rssi_rdy = poll_timer(&rssi_telemetry_timer, now);
  if (rssi_rdy) {
    rssi_telemetry_timer.start_ = now; // reset
    mqtt::send_telemetry(mqttClient, "rssi", rssi);
  }

  const bool temp_rdy = poll_timer(&temp_telemetry_timer, now);
  if (temp_rdy) {
    temp_telemetry_timer.start_ = now; // reset
    mqtt::send_telemetry(mqttClient, "temp", temp);

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  const bool blink_rdy = poll_timer(&blink_timer, now);
  if (blink_rdy) {
    blink_timer.start_ = now; // reset
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }
}
