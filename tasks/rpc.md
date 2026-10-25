# MQTT Telemetry

MQTT is a popular network protocol in IoT communication. In this task we will use the protocol to communicate with a IoT Server reached at https://iot.mastodon-buri.ts.net.

## Task

1. Connect your arduino to the thingsboard server
2. Send the RSSI value (wifi connection strength) as telemetry every second to the server
3. Make the arduinos builtin led blink at 0.7 Hz

## Setup IoT Server

1. Try opening the servers Webpage and login with your account
2. On the left side under “Entities” click “Devices”
3. Click on “Add device” → “Add new device”
4. Set a name of your choice
5. Setup credentials
   1. Click on MQTT Basic
   2. Generate a client ID, username and password (password is optional)

## Setup Arduino

1. **Setup a internet connection to a wifi network**:

- use the Arduino WiFi library for that task (https://docs.arduino.cc/libraries/wifi/)
  - CAREFUL! Some Arduino-Models use a different WiFi Library. Check your board first!
- if using git, make sure the WiFi credentials are in a separate file and gitignored
  - import this file with #include to your code
- use the WiFiSSLClient instead of the WiFiClient (otherwise a connection will fail)

2. **Setup the MQTT Protocol and connect to the server**:

- use the PubSubClient
- make sure to apply the correct credentials from the IoT Platform
- make sure to connect to the server on port 10000
- Send the RSSI every second as telemetry
  - send the data with topic: v1/devices/me/telemetry
  - you can get the RSSI with: WiFi.RSSI();

3. **Make the Arduino blink**:

- you should avoid the delay function for this, as it blocks execution. While blocked, you can not send telemetry data
- find out how you can run two tasks (blinking and telemetry sending) simultaniously
