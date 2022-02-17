
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <ESP32Servo.h>

// test network
char ssid[] = "facetracking";          // your network SSID (name)
char pass[] = "";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

Servo myservo;

void setup() {
  Serial.begin(115200);

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

  myservo.attach(18, 1000, 2000); // attaches the servo on pin 18 to the servo object
  pinMode(LED_BUILTIN, OUTPUT);
}


void led(OSCMessage &msg) {
  int tog = msg.getInt(0);
  digitalWrite(LED_BUILTIN, tog);
  Serial.print("/led: ");
  Serial.println(tog);
}

void serv(OSCMessage &msg) {
  int pos = msg.getInt(0);
  myservo.write(pos);
  Serial.print("/servo: ");
  Serial.println(pos);
  delay(15);
}

void loop() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/servo", serv);
      msg.dispatch("/led", led);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
