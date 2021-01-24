#ifndef ROOM
#define ROOM

void init_room(void);

void turn_light_on(void);
void turn_light_off(void);

void turn_heater_on(void);
void turn_heater_off(void);

void turn_water_on(void);
void turn_water_off(void);

float get_temperature(void);
float get_humidity(void);

#endif
