#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "ServoEasing.hpp"
#include "pinDefs.h"
#include "Adafruit_VL53L0X.h"

// test network
char ssid[] = "";          // your network SSID (name)
char pass[] = "";                    // your network password

WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

ServoEasing Servo1; //pin 5 on ESP32
const int upPos = 15; //startPos is the default for this servo
const int downPos = 130;
const int servoSpeed = 150;

const int buttonPin = 36;
int lastButtonPress = false;

bool bunnyTwitch = false;
float chanceToTwitch = .05;

void setup() {
  Serial.begin(115200);
  if (!Serial) delay(3000);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");

  Serial.println(localPort);


  // Set up servo
  if (Servo1.attach(SERVO1_PIN, upPos) == INVALID_SERVO) {
      Serial.println("Error attaching servo");
    }
  delay(1000);// wait a sec for the servo to move to the start
  Servo1.setSpeed(servoSpeed); // set the speed of the servo in degrees per second
  Servo1.setEasingType(EASE_CUBIC_OUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void twitch(OSCMessage &msg) {
  int tog = msg.getInt(0);
  bunnyTwitch = (tog) ? true : false;
  Serial.print("/twitch: ");
  Serial.println(bunnyTwitch);
}

void led(OSCMessage &msg) {
  int tog = msg.getInt(0);
  digitalWrite(LED_BUILTIN, tog);
  Serial.print("/led: ");
  Serial.println(tog);
}

void serv(OSCMessage &msg) {
  int pos = msg.getInt(0);
  Servo1.easeTo(pos);
  // myservo.write(pos);
  Serial.print("/servo: ");
  Serial.println(pos);
  delay(15);
}

// void handleTwitch() {
//   float diceRoll = random(0, 99)/100.00;
//   int twitchPosition = random(upPos, upPos+40);
//
//   if (diceRoll < chanceToTwitch) {
//     Servo1.easeTo(twitchPosition);
//     delay(5);
//   }
// }


void loop(){
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/servo", serv);
      msg.dispatch("/led", led);
      msg.dispatch("/twitch", twitch);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  //
  // //this doesn't seem to be working
  // if(bunnyTwitch) {
  //   handleTwitch();
  // }
}
