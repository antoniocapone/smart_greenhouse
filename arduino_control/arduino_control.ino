
/*
  * TX from nodeMCU info about temperature and humidity then turn on or off the fan. 
*/

#include <SoftwareSerial.h>

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

void create_sensor(sensor_t* sensor, const uint8_t _sensor_pin) {
  sensor->sensor_pin = _sensor_pin;
  sensor->sensor_value = 0;
}

int read_sensor(sensor_t* sensor, bool raw) {
  int value = analogRead(sensor->sensor_pin);
  if (!raw)
    value = map(value, 0, 1023, 100, 0);

  return value;
}

#define terrainSensor1  A0
#define terrainSensor2  A1
#define terrainSensor3  A2

#define pump1           8
#define pump2           9
#define pump3           10

#define software_tx     2
#define software_rx     3

#define max_terrain_hm 65

sensor_t sensor1;
sensor_t sensor2;
sensor_t sensor3;


SoftwareSerial custom_serial(software_rx, software_tx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  custom_serial.begin(115200);
  create_sensor(&sensor1, A0);
  create_sensor(&sensor2, A1);
  create_sensor(&sensor3, A2);
}

void loop() {

  int value_sensor1 = read_sensor(&sensor1, false);
  int value_sensor2 = read_sensor(&sensor2, false);
  int value_sensor3 = read_sensor(&sensor3, false);

  if (value_sensor1 > 65) {
    // Power on pump1 and send TX to NodeMCU
  }

  if (value_sensor2 > 65) {
    // Power on pump2 and send TX to NodeMCU
  }

  if (value_sensor3 > 65) {
    // Power on pump3 and send TX to NodeMCU
  }

  int incomingByte = 0; // for incoming serial data
  // send data only when you receive data:
  if (custom_serial.available() > 0) {
    // read the incoming byte:
    incomingByte = custom_serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}
