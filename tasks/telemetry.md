# RPC (Remote Procedure Call)

RPC is used to send procedure calls between devices. Thingsboard sends RPCs in JSON format, which for instance could look like this:

```json
{
  “method”: “…”,
  “params”: { … }
}
```

Functions are defined beforehand and programmed to the microcontroller. Once the chip receives a RPC with a valid method, it extracts the parameters and executes the assigned function.

## Task

1. Create a dashboard on the thingsboard server. Create a slider and a button. The slider should set the blinking frequency of the arduinos onboard led. The button is supposed do enable or disable the blinking
2. Simultaniously the arduino should send the RSSI as telemetry every second

## Setup IoT Server

1. Open the Webpage and login with your account
2. Connect your arduino as new device:
   1. Click on the device and navigate to “attributes”
   2. Create a “shared attribute”. It is later use to toggle blinking on the arduino
3. On the left side find “dashboards” and create a new one
4. Create the two required widgets on the dashboard
5. Make the button update the “shared attribute”
6. The slider should send out a “getFreq” and “setFreq” RPC:
   - “getFreq” expects the arduino to return the current frequency
   - “setFreq” sends the sliders value as RPC to the arduino

## Setup Arduino

1. **Setup a internet connection to a wifi network**:

- use the Arduino WiFi library for that task (https://docs.arduino.cc/libraries/wifi/)
  - CAREFUL! Some Arduino-Models use a different WiFi Library. Check your board first!
- if using git, make sure the WiFi credentials are in a separate file and gitignored
  - import the file with credentials with #include to your code
- use the WiFiSSLClient instead of the WiFiClient (otherwise a connection will fail)

2. **Setup the MQTT Protocol and connect to the server**:

- subscribe to the topics:
  - v1/devices/me/rpc/request/+
  - v1/devices/me/attributes
- Send the RSSI every second as telemetry
  - send the data with topic: v1/devices/me/telemetry

3. **Make the Arduino blink (any frequency you want)**
4. **Implement a callback function for the PubSubClient**:

- the callback function is executed every time the arduino receives a message from one of its subscriptions. the function takes 3 parameters:
  - char *topic (a string which topic was received)
  - byte *payload (normally a string in json format)
  - unsigned int length (how many bytes the payload is long)
- turn the payload into a string. print the topic and payload. observe what the arduino receives
- to extract values from the payload, the ArduinoJson library comes in handy
- the callback function runs everytime the arduino receives anything. Make sure to find out which topic was received and what payload was sent
- the “v1/devices/me/rpc/request/+” topic ships with a request id and might look something like this: “v1/devices/me/rpc/request/12345678”. The “getFreq” method expects a reply from the arduino. Publish a value to the topic “v1/devices/me/rpc/response/12345678” or whatever your request id is
