// #include <ESP32Servo.h>
#include "ServoEasing.hpp"
#include "pinDefs.h"
#include "Adafruit_VL53L0X.h"

// make an instance of the sensor library:
Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
const int maxDistance  = 400;

bool bunnyIsUp = true;

ServoEasing Servo1; //pin 5 on ESP32
int startPos = 15; //startPos is the default for this servo
int retreatPos = 160;
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
  if (Servo1.attach(SERVO1_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start
  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
}


void loop(){
  if (sensor.isRangeComplete()) {
    int result = sensor.readRangeResult();
    if (result < 300) {
      bunnyIsUp = false;
    } else {
      bunnyIsUp = true;
    }
  }
  delay(5);
}

void runServoBounce() {
  Servo1.setEasingType(EASE_QUADRATIC_BOUNCING);
  Servo1.easeTo(160);
  delay(1000);
}
