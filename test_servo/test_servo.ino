#include <Servo.h>

#define SERVO1_PIN 9     // A PWM PIN!!
#define SERVO2_PIN 6

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards


int pos = 0;    // variable to store the servo position


void setup() {

  myservo1.attach(SERVO1_PIN);  // attaches the SERVO_PIN  to the servo object
  myservo2.attach(SERVO2_PIN);
}


void loop() {

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees

    // in steps of 1 degree

    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position

  }

  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees

    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position

  }

}
