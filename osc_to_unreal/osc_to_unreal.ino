
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// test network
char ssid[] = "";          // your network SSID (name)
char pass[] = "";                    // your network password

// //the Arduino's IP
// IPAddress ip(128, 32, 122, 252);
//destination IP
IPAddress outIp(192, 168, 1, 11);
const unsigned int outPort = 8080;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)

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
  Udp.begin(localPort);
  Serial.print("Local port: ");

  Serial.println(localPort);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //the message wants an OSC address as first argument
   OSCMessage msg("/arduino");
   // msg.add((int32_t)analogRead(0));
   msg.add("ping!");

   Udp.beginPacket(outIp, outPort);
     msg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   msg.empty(); // free space occupied by message

   delay(20);}
