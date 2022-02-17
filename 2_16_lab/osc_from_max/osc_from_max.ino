#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = "facetracking";          // your network SSID (name)
char pass[] = "";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

void setup() {

  Serial.begin(115200);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localPort); // start listening for messages on the defined port
}

 // function to process the data from Max
 // can be ints, floats, or text
void fromMax(OSCMessage &msg) {
  int data = msg.getInt(0);
  Serial.print("/rand: ");
  Serial.println(data);
}


void loop() {
  OSCMessage msg; // instance of OSCMessage that will hold the data
  int size = Udp.parsePacket(); // if there's a valid packet, parse it and store the
  // data size in the variable

  // if we have a packet, read the data into our message object
  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }

    // if no error, process data
    if (!msg.hasError()) {
      msg.dispatch("/ping", fromMax); // if message path matches the msg, run funct
    } else { // if there's an error, report it
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
