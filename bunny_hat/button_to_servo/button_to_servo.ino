// #include <ESP32Servo.h>
#include "ServoEasing.hpp"
#include "pinDefs.h"

ServoEasing Servo1; //pin 5 on ESP32
int startPos = 15; //startPos is the default for this servo
int servoSpeed = 150;
int buttonPin = 36;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT);

  if (Servo1.attach(SERVO1_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start
  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
}


void loop(){
  int buttonPressed = digitalRead(buttonPin);

  if (buttonPressed) {
    Serial.println("Button pressed.");
    runServoBounce();
  }
  delay(50);
}

void runServoBounce() {
  Servo1.setEasingType(EASE_QUADRATIC_BOUNCING);
  Servo1.easeTo(160);
  delay(1000);
}
