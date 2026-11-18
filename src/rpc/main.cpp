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

// setup timers for async programming
poll_timer_t telemetry_timer{.start_ = 0, .duration_ = 2000};

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

  const char RESPONSE_PATH[] = "v1/devices/me/rpc/response/";
  const char REQUEST_PATH[] = "v1/devices/me/rpc/request/";
  const char ATTRIBUTES_PATH[] = "v1/devices/me/attributes";

  // here we untangle what we received.
  // did we get a request (RPC) or did an attribute change?
  //
  // this checks if the message was a request
  if (topicStr.startsWith(REQUEST_PATH)) {
    // everything after "request/..." is the request id
    // take a substring from the index after the path
    String requestId = topicStr.substring(strlen(REQUEST_PATH));

    // we extract the method from the message
    // the OR sign | returns a default value (in this case an empty string: "")
    // if the key "method" does not exist.
    const char *method = doc["method"] | "";

    // what is the method?
    // method doesnt exist
    if (strcmp(method, "") == 0) {
      Serial.println("Received empty method");
      return;
    }
    // setFreq sets the frequency
    else if (strcmp(method, "setFreq") == 0) {
      Serial.println("Received RPC setFreq.");
      // get the parameters. is it a float? is it not 0?
      const float freq = doc["params"] | 0.0f;
      if (freq > 0.) {
        blink_timer.duration_ = 1000.0f / freq; // frequency (Hz) to period (ms)
        Serial.println("Setting blink frequency to " + String(freq));
      } else
        Serial.println("Received faulty parameter");
    }
    // getFreq returns the current frequency
    else if (strcmp(method, "getFreq")) {
      String responseTopic = RESPONSE_PATH + requestId;

      float freq = 1000. / (float)(blink_timer.duration_);
      Serial.println("Received RPC getFreq.");
      Serial.println("Responding with " + String(freq));

      // send back the response
      mqttClient.publish(responseTopic.c_str(), String(freq).c_str());
    }

  }
  // this is executed if as attribute changed
  else if (topicStr == ATTRIBUTES_PATH) {
    // this checks if doc["blink"] exists and is a bool
    // if it is, it applies the new value
    // if it is not, it applies the value behind the |
    //  (in this case, if it doesnt exist or isnt a bool, the variable does not
    //  change)
    enable_blink = doc["blink"] | enable_blink;
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
  wifi::connect(WIFI_SSID, WIFI_PASSWD);

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
  const int rssi = WiFi.RSSI();

  // get the current time
  const unsigned long now = millis();

  const bool telemetry_rdy = poll_timer(&telemetry_timer, now);
  if (telemetry_rdy) {
    telemetry_timer.start_ = now; // reset
    mqtt::send_telemetry(mqttClient, "rssi", rssi);
  }

  const bool blink_rdy = poll_timer(&blink_timer, now);
  if (blink_rdy) {
    blink_timer.start_ = now; // reset
    // run only if blink is enabled OR the led_state is on
    // this is done, so the led turns back off after disabling it
    if (enable_blink || led_state) {
      digitalWrite(LED_BUILTIN, led_state);
      led_state = !led_state;
    }
  }
}
