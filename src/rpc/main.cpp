// run with
// pio run -e rpc -t upload && pio device monitor

#include "_env"

#include "PubSubClient.h"
// the WiFiNINA library is specifically designed for the arduino nano 33 iot.
// make sure to use the correct one for your board
#include "WiFiNINA.h"
#include "WiFiSSLClient.h"
#include "api/Common.h"
#include "common/mqtt.h"
#include "common/wifi.h"
#include "timer.h"
#include "variant.h"

// setup a wifi client and mqtt client
WiFiSSLClient wifiClient;
PubSubClient mqttClient(wifiClient);

// setup timers for async programming

// timer and variables for telemetry
poll_timer_t telemetry_timer{.start_ = 0, .duration_ = 2000};
int rssi = 0;

// timer and variables for led blink
poll_timer_t blink_timer{.start_ = 0, .duration_ = 1000};
bool led_state = false;
bool enable_blink = true;

// this function is called every time the device receives a message from the
// server
void callback(char *topic, byte *payload, unsigned int length) {
  // copy payload into string
  String message;
  for (unsigned int i = 0; i < length; i++)
    message += (char)payload[i];

  // print whatever we got
  // (good for debugging)
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(message);

  // turn the message into a json
  JsonDocument doc;
  if (deserializeJson(doc, message)) {
    Serial.println("JSON parse failed");
    return;
  }

  // turn the topic (char *) into a string
  String topicStr(topic);

  // here we untangle what we received.
  // did we get a request (RPC) or did an attribute change?
  //
  // this checks if the message was a request
  if (topicStr.startsWith("v1/devices/me/rpc/request/")) {
    // everything after "request/..." is the request id
    String requestId =
        topicStr.substring(String("v1/devices/me/rpc/request/").length());

    // we extract the method from the message
    const char *method = doc["method"];

    // what is the method?
    //
    // setFreq sets the frequency
    if (String(method) == "setFreq") {
      Serial.println("Received RPC setFreq.");
      // get the parameters. is it a float?
      if (doc["params"].is<float>()) {
        auto freq = doc["params"].as<float>();
        blink_timer.duration_ = 1000. / freq;
        Serial.println(String("Setting blink frequency to ") + freq);
      } else
        Serial.println("Received faulty parameter");
    }
    // getFreq returns the current frequency
    else if (String(method) == "getFreq") {
      String responseTopic = "v1/devices/me/rpc/response/" + requestId;

      float freq = 1000. / (float)(blink_timer.duration_);
      Serial.println("Received RPC getFreq.");
      Serial.println("Responding with " + String(freq));

      // send back the response
      mqttClient.publish(responseTopic.c_str(), String(freq).c_str());
    }

  }

  // this is executed if as attribute changed
  else if (topicStr == "v1/devices/me/attributes") {
    // check if a field "blink" that holds a boolean is in the json
    if (doc["blink"].is<bool>())
      enable_blink = doc["blink"].as<bool>();
  }
}

void setup() {
  // setup serial connection
  Serial.begin(115200);
  while (!Serial)
    ;

  // set the led to output
  pinMode(LED_BUILTIN, OUTPUT);

  // connect to wifi
  connectWiFi(WIFI_SSID, WIFI_PASSWD);

  // set server and register callback function
  mqttClient.setServer(SERVER_URL, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  // each loop the connection is checked
  mqtt::connect(mqttClient, CLIENT_ID, USER_NAME, PASSWD);
  // this ticks the client. it checks for incoming messages and calls the
  // callback function
  mqttClient.loop();

  // read ssensor values
  rssi = WiFi.RSSI();

  // get the current time
  unsigned long now = millis();

  bool telemetry_rdy = poll_timer(&telemetry_timer, now);
  if (telemetry_rdy) {
    telemetry_timer.start_ = now; // reset
    mqtt::send_telemetry(mqttClient, "rssi", rssi);
  }

  bool blink_rdy = poll_timer(&blink_timer, now);
  if (blink_rdy) {
    blink_timer.start_ = now; // reset
    if (enable_blink) {
      digitalWrite(LED_BUILTIN, led_state);
      led_state = !led_state;
    }
  }

  // if neither tasks fired, yield
  // this *can* give the µC time for other background tasks
  if (!blink_rdy && !telemetry_rdy)
    yield();
}
