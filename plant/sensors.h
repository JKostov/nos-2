#ifndef PLANT_SENSORS
#define PLANT_SENSORS

void register_sensor(char* deviceId, float (*getValue)(void), char* dataName);

char *read_sensor_data_json(void);

#endif