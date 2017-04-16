#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("k1lNr255g000b128mFt10000");
  Wire.endTransmission();    // stop transmitting
  delay(500);
}
