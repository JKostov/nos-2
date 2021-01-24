#include "room.h"
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"

#define SLEEP_IN_SECONDS 1

#define COLLING_PER_SECOND 0.015
#define HEATING_PER_SECOND 0.03

#define HUMIDITY_DRYING_PER_SECOND 0.08 
#define HUMIDITY_WATERING_PER_SECOND 0.1
#define HUMIDITY_LIGHT_DRYING_PER_SECOND 0.035

static char stack[THREAD_STACKSIZE_MAIN];

static float roomTemperature = 20.0;
static float plantHumidity = 70.0;
static int lightOn = 0;
static int waterOn = 0;
static int heaterOn = 0;

static void *room_thread_handler(void *arg)
{
  (void)arg;

  while (1)
  {
    if (lightOn)
    {
      plantHumidity -= HUMIDITY_LIGHT_DRYING_PER_SECOND * SLEEP_IN_SECONDS;
    }

    if (heaterOn)
    {
      roomTemperature += HEATING_PER_SECOND * SLEEP_IN_SECONDS;
    }
    else
    {
      roomTemperature -= COLLING_PER_SECOND * SLEEP_IN_SECONDS;
    }

    if (waterOn)
    {
      plantHumidity += HUMIDITY_WATERING_PER_SECOND * SLEEP_IN_SECONDS;
    }
    else
    {      
      plantHumidity -= HUMIDITY_DRYING_PER_SECOND * SLEEP_IN_SECONDS;
    }

    xtimer_sleep(SLEEP_IN_SECONDS);
  }

  return NULL;
}

void init_room(void)
{
  thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, room_thread_handler, NULL, "room_thread");
}

void turn_light_on(void)
{
  lightOn = 1;
}

void turn_light_off(void)
{
  lightOn = 0;
}

void turn_heater_on(void)
{
  heaterOn = 1;
}

void turn_heater_off(void)
{
  heaterOn = 0;
}

void turn_water_on(void)
{
  waterOn = 1;
}

void turn_water_off(void)
{
  waterOn = 0;
}

float get_temperature(void)
{
  return roomTemperature;
}

float get_humidity(void)
{
  return plantHumidity;
}