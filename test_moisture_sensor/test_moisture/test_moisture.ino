// Moisture Sensor Arduino Code


//By Circuitdigest 

#define sensorPin A0


void setup() {

  Serial.begin(9600);
}

void loop() {


  Serial.print("Analog output: ");


  Serial.println(readSensor());


  delay(500);


}

//  This function returns the analog data to calling function

int readSensor() {


  int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor


  int outputValue = map(sensorValue, 0, 1023, 100, 0); // map the 10-bit data to 8-bit data


  return outputValue;             // Return analog moisture value

}