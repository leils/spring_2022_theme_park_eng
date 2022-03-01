// Super basic lab with LED fading

int ledPin = 13;    // onboard LED connected to pin 13
void setup() {
  Serial.begin(9600);
}

void loop() {
  // fade in from min to max
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 16 milliseconds to see the dimming effect
    delay(16);
    // send vals to the serial monitor
    Serial.println(fadeValue);
  }

  // fade out from max to min
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 1) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 16 milliseconds to see the dimming effect
    delay(16);
    // send vals to the serial monitor
    Serial.println(fadeValue);
  }
}
