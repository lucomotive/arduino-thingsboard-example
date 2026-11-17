# Arduino MQTT + Thingsboard example

This repo contains a few code examples for arduino to interact with a thingsboard server.
Currently 3 code examples are provided:

- telemetry (mqtt)
- telemetry (https)
- rpc (remote procedure call)

Also a tutorial how to setup alarms in thingsboard is provided in ./examples

## The _env file

You may have noticed, that all examples include a _env file:

```cpp
#include "_env"
```

The _env file is gitignored and not visible here on github, to keep passwords and credentials secret.
You must create it and initialize your secret variables.

```cpp
#pragma once

const char SERVER_URL[] = "example.net";

const int MQTT_PORT = 10000;
const int HTTPS_PORT = 443;


const char CLIENT_ID[] = "a client id for mqtt";
const char USER_NAME[] = "a user name for mqtt/http";
const char PASSWD[] = "a password for mqtt";

const char WIFI_SSID[] = "your wifi";
const char WIFI_PASSWD[] = "your wifi password";

```
