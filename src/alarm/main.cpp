// run with
// pio run -e telemetry -t upload && pio device monitor

#include "_env"

#include "PubSubClient.h"
#include "WiFiNINA.h"
#include "WiFiSSLClient.h"
#include "api/Common.h"
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
int temp = 0;

// definition of poll based tasks
// look at loop function
BlinkTask blinkTask(LED_BUILTIN, 0.7);
TelemetryTask<int> tempTelemetryTask(1., "temp", temp);
TelemetryTask<int> rssiTelemetryTask(0.6, "rssi", rssi);

void loop() {
  connectMQTT(client, CLIENT_ID, USER_NAME, PASSWD);
  client.loop();

  // read values
  rssi = WiFi.RSSI();
  temp = random() % 16 + 70;

  // three tasks run in "parallel"
  auto rtt = rssiTelemetryTask.poll(client);
  auto ttt = tempTelemetryTask.poll(client);
  auto bt = blinkTask.poll();

  // dont busy loop if no task was ready
  if (!rtt && !ttt && !bt)
    delay(10);
}
