#include "sensor.h"

#include <Arduino.h>

void create_sensor(sensor_t* sensor, const uint8_t _sensor_pin) {
  sensor->sensor_pin = _sensor_pin;
  sensor->sensor_value = 0;
}

int read_sensor(sensor_t* sensor, bool raw) {
  int value = analogRead(sensor->sensor_pin);
  if (!raw)
    value = map(sensorValue, 0, 1023, 100, 0);

  return value;
}