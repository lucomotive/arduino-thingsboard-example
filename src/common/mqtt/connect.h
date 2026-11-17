#ifndef CONNECT_MQTT_H
#define CONNECT_MQTT_H

#include "PubSubClient.h"

// this function connects the mqtt client to the server
void connectMQTT(PubSubClient &client, const char *const id,
                 const char *const username, const char *const passwd);

#endif
