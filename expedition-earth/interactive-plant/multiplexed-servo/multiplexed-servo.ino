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

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_SPEED 6 // Speed that the servo moves

// our servo # counter
uint8_t servonum = 0;
int maxServos = 5;
uint16_t servoGoal[] = {SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN};
uint16_t servoState[] = {SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN, SERVOMIN};

void setup() {
  Serial.begin(9600);
  if (!Serial) delay(3000);

  // ----------------------------------- Connect to WiFi network
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
    Serial.println("WiFi not connected");
  }

  // ----------------------------------- PWM servo setup
  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency.
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

void loop() {
  // Let's start with 1 servos
  //
  for (int i = 0; i < maxServos; i++) {
    if (abs(servoState[i] - servoGoal[i]) > SERVO_SPEED) {
      if (servoState[i] > servoGoal[i]) {
        servoState[i] = servoState[i] - SERVO_SPEED;
      } else {
        servoState[i] = servoState[i] + SERVO_SPEED;
      }
      pwm.setPWM(i, 0, servoState[i]);
    }
  }

  if (useWifi) {
    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0) {
      while (size--) {
        msg.fill(Udp.read());
      }
      if (!msg.hasError()) {
        msg.dispatch("/servo0", servo0);
        // TODO
        // msg.dispatch("/servo1", servo1);
        // msg.dispatch("/servo2", servo2);
        // msg.dispatch("/servo3", servo3);
        // msg.dispatch("/servo4", servo4);
        msg.dispatch("/ping", ping);
      } else {
        error = msg.getError();
        Serial.print("error: ");
        Serial.println(error);
      }
    }
  }

  delay(50);
}

void ping(OSCMessage &msg) {
  Serial.println("Ping!");
}

void servo0(OSCMessage &msg) {
  int pos = msg.getInt(0);
  servoGoal[0] = pos;
  // pwm.setPWM(0, 0, pos);
  Serial.print("/servo1 goal: ");
  Serial.println(pos);
}

void servo1(OSCMessage &msg) {
  int pos = msg.getInt(0);
  servoGoal[1] = pos;
  Serial.print("/servo1 goal: ");
  Serial.println(pos);
}

void servo2(OSCMessage &msg) {
  int pos = msg.getInt(0);
  servoGoal[2] = pos;
  Serial.print("/servo1 goal: ");
  Serial.println(pos);
}

void servo3(OSCMessage &msg) {
  int pos = msg.getInt(0);
  servoGoal[3] = pos;
  Serial.print("/servo1 goal: ");
  Serial.println(pos);
}

void servo4(OSCMessage &msg) {
  int pos = msg.getInt(0);
  servoGoal[4] = pos;
  Serial.print("/servo1 goal: ");
  Serial.println(pos);
}
