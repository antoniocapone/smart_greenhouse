
/*
  * TX from nodeMCU info about temperature and humidity then turn on or off the fan. 
*/

#include <SoftwareSerial.h>

#include "sensor.h"

#define terrainSensor1  A0
#define terrainSensor2  A1
#define terrainSensor3  A2

#define pump1           D8
#define pump2           D9
#define pump3           D10

#define software_tx     D2
#define software_rx     D3

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
