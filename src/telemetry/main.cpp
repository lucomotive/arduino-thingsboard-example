// run with
// pio run -e telemetry -t upload && pio device monitor

// expects a file like:
// const char MQTT_ENDPOINT[] = "...";
// const int MQTT_PORT = 1883;
// const char CLIENT_ID[] = "...";
// const char USER_NAME[] = "...";
// const char PASSWD[] = "...";
#include "../../env/mqtt.h"
// expects a file like:
// const char WIFI_SSID[] = "...";
// const char WIFI_PASSWD[] = "...";
#include "../../env/wifi.h"

#include "PubSubClient.h"
#include "WiFiNINA.h"
#include "WiFiSSLClient.h"
#include "common/blink/task.h"
#include "common/mqtt/connect.h"
#include "common/telemetry/task.h"
#include "common/wifi/connect.h"

// setup a wifi client and mqtt client
WiFiSSLClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(LED_BUILTIN, OUTPUT);

  connectWiFi(WIFI_SSID, WIFI_PASSWD);
  client.setServer(MQTT_ENDPOINT, MQTT_PORT);
}

int rssi = 0;

// definition of poll based tasks
// look at loop function
TelemetryTask<int> telemetryTask(1., "rssi", rssi);
BlinkTask blinkTask(LED_BUILTIN, 0.7);

void loop() {
  connectMQTT(client, CLIENT_ID, USER_NAME, PASSWD);
  client.loop();

  // read values
  rssi = WiFi.RSSI();

  // two tasks run in "parallel"
  auto tel = telemetryTask.poll(client);
  auto bli = blinkTask.poll();

  // dont busy loop if no task was ready
  if (!tel && !bli)
    delay(10);
}
