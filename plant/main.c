#include <stdlib.h>
#include <stdio.h>
#include <shell.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "jsmn.h"

#include "broker/broker.h"
#include "room/room.h"
#include "sensors/temperature.h"
#include "sensors/humidity.h"
#include "sensors.h"

#define SLEEP_IN_SECONDS 1
#define PUBLISH_TOPIC "plant/sensors"
#define SUBSCRIBE_TOPIC "plant/actuators"
#define MAX_NUMBER_OF_TOKENS 10

static char read_and_publish_thread_stack[THREAD_STACKSIZE_MAIN];

static void *read_and_publish_thread_handler(void *arg)
{
    (void)arg;

    while (1)
    {
        char *data = read_sensor_data_json();

        publish_message(PUBLISH_TOPIC, data);

        xtimer_sleep(SLEEP_IN_SECONDS);
    }

    return NULL;
}

static void _on_msg_received(MessageData *data)
{
    jsmn_parser p;
    jsmntok_t t[MAX_NUMBER_OF_TOKENS];

    char *payload = (char *)data->message->payload;

    printf("%.*s\n", (int)data->message->payloadlen, (char *)data->message->payload);

    jsmn_init(&p);
    int r = jsmn_parse(&p, (char *)data->message->payload, data->message->payloadlen, t, MAX_NUMBER_OF_TOKENS);

    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        printf("Object expected %d\n", r);
        return;
    }

    char name[50];
    sprintf(name, "%.*s", t[1].end - t[1].start, payload + t[1].start);

    if (t[2].type != JSMN_PRIMITIVE)
    {
        printf("Number expected\n");
        return;
    }

    char res[10];
    sprintf(res, "%.*s", t[2].end - t[2].start, payload + t[2].start);

    long ret = strtol(res, NULL, 10);

    if (strcmp(name, "heater") == 0)
    {
        if (ret)
        {
            turn_heater_on();
        }
        else
        {
            turn_heater_off();
        }
    }
    else if (strcmp(name, "light") == 0)
    {
        if (ret)
        {
            turn_light_on();
        }
        else
        {
            turn_light_off();
        }
    }
    else if (strcmp(name, "water") == 0)
    {
        if (ret)
        {
            turn_water_on();
        }
        else
        {
            turn_water_off();
        }
    }
}

static int _start_application(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Broker IP missing\n");
        return 1;
    }

    char *remote_ip = argv[1];

    int res = connect_broker(remote_ip);
    if (res != 0)
    {
        printf("Error\n");
        return res;
    }

    init_room();

    register_sensor(tem_device_id(), tem_read_temperature, "temperature");
    register_sensor(hum_device_id(), hum_read_humidity, "humidity");

    thread_create(read_and_publish_thread_stack, sizeof(read_and_publish_thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_and_publish_thread_handler, NULL, "read_and_publish_thread");

    subscribe(SUBSCRIBE_TOPIC, _on_msg_received);

    return 0;
}

static const shell_command_t shell_commands[] = {
    {"start", "Start app", _start_application},
    {NULL, NULL, NULL}};

int main(void)
{
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
