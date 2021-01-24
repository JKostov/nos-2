#include "temperature.h"
#include "../room/room.h"

float tem_read_temperature(void)
{
  return get_temperature();
}

char* tem_device_id(void)
{
  return "tem";
}