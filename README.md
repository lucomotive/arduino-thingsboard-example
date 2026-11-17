# Arduino MQTT + Thingsboard example

## the _env file

You may have noticed, that all examples include a _env file:

```cpp
#include "_env"
```

The _env file is gitignored and not visible here on github, to keep passwords and credentials secret.
You must create it and initialize your secret variables.

```cpp
const char MQTT_ENDPOINT[] = "iot.some-domain.net";
const int MQTT_PORT = 10000;
const char CLIENT_ID[] = "some client id";
const char USER_NAME[] = "some client id";
const char PASSWD[] = "some client id";

const char WIFI_SSID[] = "your wifi";
const char WIFI_PASSWD[] = "your wifi password";
```
