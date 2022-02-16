// #include <ESP32Servo.h>
#include "ServoEasing.hpp"
#include "pinDefs.h"
#include "Adafruit_VL53L0X.h"

// make an instance of the sensor library:
Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
const int maxDistance  = 400;

bool bunnyUp = true;
bool lastBunnyUp = bunnyUp;

ServoEasing Servo1; //pin 5 on ESP32
int upPos = 15; //startPos is the default for this servo
int downPos = 130;
int servoSpeed = 150;
int buttonPin = 36;

void setup() {
  Serial.begin(115200);
  if (!Serial) delay(3000);

  pinMode(buttonPin, INPUT);

  // Set up sensor
  if (!sensor.begin()) {
    Serial.println("Sensor not responding. Check wiring.");
    while(true);
  }
  sensor.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
  sensor.startRangeContinuous();

  // Set up servo
  if (Servo1.attach(SERVO1_PIN, upPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start
  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
  Servo1.setEasingType(EASE_CUBIC_OUT);
}


void loop(){
  if (sensor.isRangeComplete()) {
    int result = sensor.readRangeResult();
    if (result < 300) {
      bunnyUp = false;
    } else {
      bunnyUp = true;
    }
  }

  //Only run servo changes if we're changing direction
  if (bunnyUp != lastBunnyUp) {
    Serial.println("State change detected.");
    (bunnyUp) ? handleBunnyUp() : handleBunnyDown();
  }

  lastBunnyUp = bunnyUp;

  delay(5);
}

void handleBunnyDown() {
  Servo1.easeTo(downPos);
}

void handleBunnyUp() {
  Servo1.easeTo(upPos);
}
