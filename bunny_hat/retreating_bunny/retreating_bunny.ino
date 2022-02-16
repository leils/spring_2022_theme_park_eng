// #include <ESP32Servo.h>
#include "ServoEasing.hpp"
#include "pinDefs.h"
#include "Adafruit_VL53L0X.h"

// make an instance of the sensor library:
Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
const int maxDistance  = 300;

bool bunnyUp = true;
bool lastBunnyUp = bunnyUp;
bool bunnyTwitch = false;

ServoEasing Servo1; //pin 5 on ESP32
const int upPos = 15; //startPos is the default for this servo
const int downPos = 130;
const int servoSpeed = 150;
const int buttonPin = 36;

float chanceToTwitch = .05;

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
  //handle twitch

  if (sensor.isRangeComplete()) {
    int result = sensor.readRangeResult();
    if (result < maxDistance) {
      bunnyUp = false;
    } else {
      bunnyUp = true;
      if (bunnyTwitch) {
        handleTwitch();
      }
    }
  }

  //Only run servo changes if we're changing direction
  if (bunnyUp != lastBunnyUp) {
    // Serial.println("State change detected.");
    (bunnyUp) ? handleBunnyUp() : handleBunnyDown();
  }

  lastBunnyUp = bunnyUp;

  delay(10);
}

void handleTwitch() {
  float diceRoll = random(0, 99)/100.00;
  int twitchPosition = random(upPos, upPos+20);

  if (diceRoll < chanceToTwitch) {
    Servo1.easeTo(twitchPosition);
    delay(15);
  }
}

void handleBunnyDown() {
  Servo1.easeTo(downPos);
}

void handleBunnyUp() {
  Servo1.easeTo(upPos);
}
