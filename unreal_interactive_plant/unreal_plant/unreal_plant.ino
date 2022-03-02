// Connects the Arduino to Unreal via OSC.
// Set up for my desktop computer & home network.
//
// This version is set up for a dual real and virtual plant
// Sends messages to Unreal when a light is shone upon the photoresistor
// Receives messages to "water" the plant from Unreal

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// photo setup ------------------------------------------------/
#define photoPin 34
const int photoThreshold = 500;
int lastPhotoRead = 0;

// WIFI setup ------------------------------------------------/
char ssid[] = "";          // your network SSID (name)
char pass[] = "";                    // your network password

// //the Arduino's IP
// IPAddress ip(128, 32, 122, 252);
//destination IP
IPAddress outIp(192, 168, 1, 11);
const unsigned int outPort = 8080;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int inPort = 8000;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

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
  Udp.begin(inPort);
  Serial.print("Local port: ");

  Serial.println(inPort);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(photoPin, INPUT);
}

//TODO: run the motor
void unrealTriggerPressed(OSCMessage &msg) {
  Serial.println("Got a trigger press!");
}

// Status should be 0(turned off) or 1(turned on)
void sendLight(int status) {
  Serial.println("Button pressed");
  //the message wants an OSC address as first argument
   OSCMessage outMsg("/buttonPress");
   // msg.add((int32_t)analogRead(0));
   outMsg.add(status);

   Udp.beginPacket(outIp, outPort);
     outMsg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   outMsg.empty(); // free space occupied by message
}
void loop() {
  //*-----------------------------------------------------------*//
  //Send out photoResistor readings
  int photoRead = analogRead(buttonPin);

  if ((photoRead >= photoThreshold) && (lastPhotoRead < photoThreshold)) {
    sendLight(1);
  }
  else if ((photoRead <= photoThreshold) && (lastPhotoRead > photoThreshold)) {
    sendLight(0);
  }
  lastPhotoRead = photoRead;

  //*-----------------------------------------------------------*//
  //Take in OSC messages
  OSCMessage inMsg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      inMsg.fill(Udp.read());
    }
    if (!inMsg.hasError()) {
      //TODO turn this into something for the watering
      inMsg.dispatch("/triggerPress", unrealTriggerPressed);
    } else {
      error = inMsg.getError();
      Serial.print("Error: ");
      Serial.println(error);
    }

  }

   delay(20);
 }
