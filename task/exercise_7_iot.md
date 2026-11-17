# Exercise 7: Internet of Things (IoT)

In this exercise students will connect a microcontroller (Arduino with Ethernet Shield) to the internet.
They will use an IoT service provider (Thingsboard) to display and visualize local sensor data collected by the arduino in the www. Furthermore they will remotely control their microcontroller device from the platform.

## Necessary preparations

- Activate your account with the activation link we provide and set a **secure** password
- Many different services and devices can connect to thingsboard. For a µC to connect to thingsboard, a internet connection is required. Make sure your board has the necessary periphery (like a WiFi-Chip)
- Further documentation for the Platform and examples can be found here: https://thingsboard.io/docs/user-guide/

- Your Arduino needs a few libraries to connect to thingsboard: WiFi.h, PubSubClient.h (for mqtt) and HttpClient.h
- you can install the either in the Arduino IDE, or in VSCode with PlatformIO, by editing the platformio.ini file in your project root. This is an example for the "arduino nano 33 iot":

```ini
[env]
platform = atmelsam
board = nano_33_iot
framework = arduino
lib_deps =
    arduino-libraries/WiFiNINA
    knolleary/PubSubClient
    bblanchon/ArduinoJson@^7.4.3
    arduino-libraries/ArduinoHttpClient
```

- note, that some boards need an alterrnative WiFi library. In this case, the WiFiNINA library is used instead. Commonly the alternative libraries are designed to be interchangable, so no further changes in the code are necessary
- for most boards, the "arduino-libraries/WiFi" library is used

## Task

Create your own project by hooking up sensor(s) to the internet.
Use bi-directional communication to control your project and add event-triggered actions such as automatic notifications to the Web-UI or Smartphone App on sensor events.
Create a dashboard (Human-Machine-Interface, HMI) to visualize and control your data and devices.
Make your usual documentation.

## Examples

Look at the example in this repo to get a better understanding on how to interact with the thingsboard platform
