#include "secrets.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

bool useWifi = false;
int ledState = LOW;

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
    ledState = HIGH;
    digitalWrite(LED_BUILTIN, ledState);

    useWifi = true;
  } else {
    Serial.println("WiFi not connected");
  }
}

void ping(OSCMessage &msg) {
  Serial.println("ping received");
}

void led(OSCMessage &msg) {
  Serial.println("led message received");
  ledState = abs(ledState - 1);
  digitalWrite(LED_BUILTIN, ledState);
}

void loop() {
  if (useWifi) {
    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0) {
      while (size--) {
        msg.fill(Udp.read());
      }
      if (!msg.hasError()) {
        msg.dispatch("/ping", ping);
        msg.dispatch("/led", led);
      } else {
        error = msg.getError();
        Serial.print("error: ");
        Serial.println(error);
      }
    }
  }
}
