#include "broker.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define BUF_SIZE 1024
#define MQTT_VERSION_v311 4 /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS 4000

#define DEFAULT_MQTT_CLIENT_ID "plant"
#define DEFAULT_MQTT_PORT 1883
#define DEFAULT_KEEPALIVE_SEC 10
#define MAX_LEN_TOPIC 100

static MQTTClient client;
static Network network;
static char topic_to_subscribe[MAX_LEN_TOPIC];
static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

static int disconnect_broker(void)
{
  int res = MQTTDisconnect(&client);
  if (res < 0)
  {
    printf("mqtt_example: Unable to disconnect\n");
  }
  else
  {
    printf("mqtt_example: Disconnect successful\n");
  }

  NetworkDisconnect(&network);
  return res;
}

int connect_broker(char *broker_address)
{
  NetworkInit(&network);
  MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE, readbuf, BUF_SIZE);
  MQTTStartTask(&client);

  int ret = -1;
  if (client.isconnected)
  {
    printf("Client already connected, disconnecting\n");
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
  }

  int port = DEFAULT_MQTT_PORT;
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  data.MQTTVersion = MQTT_VERSION_v311;
  data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
  data.cleansession = true;
  data.willFlag = false;

  ret = NetworkConnect(&network, broker_address, port);
  if (ret < 0)
  {
    printf("Unable to connect: %d\n", ret);
    return ret;
  }

  ret = MQTTConnect(&client, &data);
  if (ret < 0)
  {
    printf("Unable to connect client %d\n", ret);
    disconnect_broker();
    return ret;
  }

  printf("Connection successfully\n");
  return 0;
}

int publish_message(char* topic, char *payload)
{
  enum QoS qos = QOS0;
  MQTTMessage message;
  message.qos = qos;
  message.retained = false;
  message.payload = payload;
  message.payloadlen = strlen(message.payload);

  int rc;
  if ((rc = MQTTPublish(&client, topic, &message)) < 0)
  {
    printf("Unable to publish (%d)\n", rc);
  }

  return rc;
}

int subscribe(char *topic, messageHandler message_handler)
{
  enum QoS qos = QOS0;

  if (strlen(topic) > MAX_LEN_TOPIC)
  {
    printf("Topic name too long %s\n", topic);
    return -1;
  }

  strncpy(topic_to_subscribe, topic, strlen(topic));

  int ret = MQTTSubscribe(&client, topic_to_subscribe, qos, message_handler);
  if (ret < 0)
  {
    printf("Unable to subscribe to topic: %s \n", topic);
    return ret;
  }

  printf("Subscribed to topic: %s\n", topic);
  return 0;
}