#include "humidity.h"
#include "../room/room.h"

float hum_read_humidity(void)
{
  return get_humidity();
}

char* hum_device_id(void)
{
  return "hum";
}