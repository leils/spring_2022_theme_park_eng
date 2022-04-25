#include "ServoEasing.hpp"
#include "pinDefs.h"
/*
   Platform     Servo1      Servo2      Servo3
   -------------------------------------------
   ESP32        5           18          19
*/
ServoEasing Servo1; // pin 5 ont he ESP32
int startPos = 91;
int servoSpeed = 200;
void setup() {
// put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);

  if (Servo1.attach(SERVO1_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start

  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
}
void loop() {
  int buttonPressed = digitalRead(BUTTON_PIN);

  if (buttonPressed) {
    Serial.println("button detected, running servo");
    // Linear
    Servo1.setEasingType(EASE_LINEAR);
    Servo1.easeTo(180);
    Servo1.easeTo(10);
  }

  delay(50);
}
