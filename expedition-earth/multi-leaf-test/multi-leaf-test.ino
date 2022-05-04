#include "ServoEasing.hpp"
#include "pinDefs.h"
#include "secrets.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "Adafruit_VL53L0X.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

bool useWifi = false;

ServoEasing Servo1; // pin 5 ont he ESP32
ServoEasing Servo2; // pin 5 ont he ESP32
ServoEasing Servo3; // pin 5 ont he ESP32
int startPos = 91;
int servoSpeed = 200;
void setup() {
// put your setup code here, to run once:
  Serial.begin(115200);
  if (!Serial) delay(3000);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  pinMode(LED_BUILTIN, OUTPUT);

  unsigned long stopWaiting = millis() + 5000;
  while (WiFi.status() != WL_CONNECTED && (millis() < stopWaiting)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");

    Serial.println(localPort);
    digitalWrite(LED_BUILTIN, HIGH);

    useWifi = true;
  } else {
    Serial.println("WiFi not connected, proceeding with button only");
  }

  // Setup button, servo
  pinMode(BUTTON_PIN, INPUT);

  if (Servo1.attach(SERVO1_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }

  if (Servo2.attach(SERVO2_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }

  if (Servo3.attach(SERVO3_PIN, startPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start

  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
  Servo1.setEasingType(EASE_LINEAR);
  Servo1.easeTo(0);

  Servo2.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
  Servo2.setEasingType(EASE_LINEAR);
  Servo2.easeTo(0);

  Servo3.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
  Servo3.setEasingType(EASE_LINEAR);
  Servo3.easeTo(0);
}

void ping(OSCMessage &msg) {
  Serial.println("ping received");
}

void servo1(OSCMessage &msg) {
  int pos = msg.getInt(0);
  Servo1.easeTo(pos);
  Serial.print("/servo1: ");
  Serial.println(pos);
  delay(15);
}

void servo2(OSCMessage &msg) {
  int pos = msg.getInt(0);
  Servo2.easeTo(pos);
  Serial.print("/servo2: ");
  Serial.println(pos);
  delay(15);
}

void servo3(OSCMessage &msg) {
  int pos = msg.getInt(0);
  Servo3.easeTo(pos);
  Serial.print("/servo3: ");
  Serial.println(pos);
  delay(15);
}

void loop() {
  // Using Buttons
  int buttonPressed = digitalRead(BUTTON_PIN);

  if (buttonPressed) {
    Serial.println("button detected, running servo");
    // Linear
    Servo1.easeTo(180);
    Servo1.easeTo(10);
    delay(100);

    Servo2.easeTo(180);
    Servo2.easeTo(10);
    delay(100);

    Servo3.easeTo(180);
    Servo3.easeTo(10);
    delay(100);
  }

  if (useWifi) {
    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0) {
      while (size--) {
        msg.fill(Udp.read());
      }
      if (!msg.hasError()) {
        msg.dispatch("/servo1", servo1);
        msg.dispatch("/servo2", servo1);
        msg.dispatch("/servo3", servo1);
        msg.dispatch("/ping", ping);
      } else {
        error = msg.getError();
        Serial.print("error: ");
        Serial.println(error);
      }
    }
  }
}
