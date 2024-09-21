#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t sensor_pin;
  int     sensor_value;
} sensor_t;

typedef struct {
  bool air1;
  bool air2;
  bool air3;
} app_handle_t;

void create_sensor(sensor_t* sensor, const uint8_t _sensor_pin);
int read_sensor(sensor_t* sensor, bool raw);



#endif