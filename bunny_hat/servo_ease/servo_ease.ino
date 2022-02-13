#include "ServoEasing.hpp"
#include "pinDefs.h"
/*
   Platform     Servo1      Servo2      Servo3
   -------------------------------------------
   ESP32        5           18          19
*/
ServoEasing Servo1; // pin 5 ont he ESP32
int startPos = 90;
int servoSpeed = 40;
void setup() {
// put your setup code here, to run once:
  Serial.begin(115200);
if (Servo1.attach(SERVO1_PIN, startPos) == INVALID_SERVO) {
    Serial.println("Error attaching servo");
  }
delay(1000);// wait a sec for the servo to move to the start
  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
}
void loop() {
// put your main code here, to run repeatedly:
// Linear
  Servo1.setEasingType(EASE_LINEAR);
  Servo1.easeTo(105);
  Servo1.easeTo(65);
delay(1000);
// Quad Bouncing
  Servo1.setEasingType(EASE_QUADRATIC_BOUNCING);
  Servo1.easeTo(105);
  Servo1.easeTo(65);
delay(1000);
//
  Servo1.setEasingType(EASE_QUADRATIC_IN_OUT);
  Servo1.easeTo(105);
  Servo1.easeTo(65);
delay(1000);
  Servo1.setEasingType(EASE_ELASTIC_BOUNCING);
  Servo1.easeTo(105, 10);
  Servo1.easeTo(65, 10);
delay(1000);
}
