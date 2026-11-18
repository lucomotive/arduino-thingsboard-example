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
  // begin serial connection
  Serial.begin(115200);
  // this waits until the serial connection is ready
  while (!Serial)
    ;

  // this function connect your device to a wifi network
  // check the file wifi.h in the "common" folder or click on it with the
  // Ctrl-Key pressed to see what is does
  wifi::connect(WIFI_SSID, WIFI_PASSWD);

  // connect the mqtt cient to the thingsboard server
  mqttClient.setServer(SERVER_URL, MQTT_PORT);
}

// setup timer for async programming
poll_timer_t telemetry_timer{.start_ = 0, .duration_ = 3300};

void loop() {
  // the mqtt::connect function runs only if the client disconnects
  // (check out ./common/mqtt.h)
  mqtt::connect(mqttClient, CLIENT_ID, USER_NAME, PASSWD);
  // this needs to be run each iteration
  mqttClient.loop();

  // read pseudo sensor values
  // in a real application, actually read the sensor
  const int temp = random() % 16 + 70;

  // get the current time
  unsigned long now = millis();
  // check the timer and run the telemetry
  bool rssi_rdy = poll_timer(&telemetry_timer, now);
  if (rssi_rdy) {
    telemetry_timer.start_ = now; // reset
    // this sends the sensor reading with the key "temp" as telemetry to the
    // server.
    // you should be able to observe the telemetry on the platform
    mqtt::send_telemetry(mqttClient, "temp", temp);
  }
}
