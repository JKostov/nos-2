#include "paho_mqtt.h"
#include "MQTTClient.h"

#ifndef BROKER_WRAPPER
#define BROKER_WRAPPER

int connect_broker(char *broker_address);

int publish_message(char* topic, char *payload);

int subscribe(char *topic, messageHandler message_handler);

#endif