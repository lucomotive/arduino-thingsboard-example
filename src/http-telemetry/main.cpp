// run with
// pio run -e http -t upload && pio device monitor

#include "_env"

#include "HttpClient.h"
#include "WiFi.h"
#include "WiFiSSLClient.h"
#include "common/http.h"
#include "common/timer.h"
#include "common/wifi.h"

// setup a wifi client and mqtt client
WiFiSSLClient wifiClient;
HttpClient httpClient(wifiClient, SERVER_URL, HTTPS_PORT);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(LED_BUILTIN, OUTPUT);

  connectWiFi(WIFI_SSID, WIFI_PASSWD);

  httpClient.connectionKeepAlive();
}

// setup timers for async programming

// timer and variables for telemetry
poll_timer_t rssi_telemetry_timer{.start_ = 0, .duration_ = 2000};
int rssi = 0;

// timer and variables for telemetry
poll_timer_t temp_telemetry_timer{.start_ = 0, .duration_ = 3300};
int temp = 0;

// timer and variables for led blink
poll_timer_t blink_timer{.start_ = 0, .duration_ = 1000};
bool led_state = false;

void loop() {
  // read sensor values
  rssi = WiFi.RSSI();
  temp = random() % 16 + 70; // in a real application, read the sensor

  // get the current time
  unsigned long now = millis();

  bool rssi_rdy = poll_timer(&rssi_telemetry_timer, now);
  if (rssi_rdy) {
    rssi_telemetry_timer.start_ = now; // reset
    http::send_telemetry(httpClient, "rssi", rssi);
  }

  bool temp_rdy = poll_timer(&temp_telemetry_timer, now);
  if (temp_rdy) {
    temp_telemetry_timer.start_ = now; // reset
    http::send_telemetry(httpClient, "temp", temp);

    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }

  bool blink_rdy = poll_timer(&blink_timer, now);
  if (blink_rdy) {
    blink_timer.start_ = now; // reset
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }
}
