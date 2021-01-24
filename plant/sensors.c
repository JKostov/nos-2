#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBEROF_SENSOR 5

struct sensor_data
{
  char *deviceId;
  float (*getValue)(void);
  char *dataName;
};

static struct sensor_data registered_sensors[MAX_NUMBEROF_SENSOR];
static int sensors_count = 0;

void register_sensor(char *deviceId, float (*getValue)(void), char *dataName)
{
  if (sensors_count == MAX_NUMBEROF_SENSOR)
  {
    puts("Cannot register sensor, max number of sensors reached.\n");
    return;
  }

  int sensor_registed = 0;
  for (int i = 0; i < sensors_count; i++)
  {
    if (strcmp(registered_sensors[i].deviceId, deviceId) == 0)
    {
      sensor_registed = 1;
    }
  }

  if (sensor_registed)
  {
    printf("Sensor with id %s already registered\n", deviceId);
    return;
  }

  struct sensor_data newSensor;

  newSensor.deviceId = strdup(deviceId);
  newSensor.getValue = getValue;
  newSensor.dataName = strdup(dataName);

  registered_sensors[sensors_count++] = newSensor;
}

static char *appendString(char *str1, char *str2)
{
  int len1 = strlen(str1);
  int len2 = strlen(str2);

  char *newString = malloc(sizeof(char) * (len1 + len2 + 1));

  strcpy(newString, str1);
  strcat(newString, str2);

  return newString;
}

char *read_sensor_data_json(void)
{
  char *res = "{ ";

  for (int i = 0; i < sensors_count; i++)
  {
    char valueToString[100];
    sprintf(valueToString, "\"%s\": %.1f", registered_sensors[i].dataName, registered_sensors[i].getValue());
    res = appendString(res, valueToString);

    if (i < sensors_count - 1)
    {
      res = appendString(res, ", ");
    }
  }

  res = appendString(res, " }");
  return res;
}
