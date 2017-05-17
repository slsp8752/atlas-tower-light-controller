#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  for (int i = 0; i<20; i++){
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write("k1r255g000b128mFt10000");
      Wire.endTransmission();

      Wire.beginTransmission(8);
      Wire.write("k2r255g000b128mFt10000");
      Wire.endTransmission();

      Wire.beginTransmission(8);
      Wire.write("k3r255g000b128mFt10000");
      Wire.endTransmission();
    //}
    delay(10000);
  }
  delay(1000000);
}
