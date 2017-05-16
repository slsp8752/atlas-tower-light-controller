#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  for (int i = 0; i<20; i++){
    Wire.beginTransmission(8); // transmit to device #8
    //for (int j = 0; j < i+1; j++){
      Wire.write("k1r255g000b128mFt10000k2r255g000b128mFt10000k3r255g000b128mFt10000");
    //}
    Wire.endTransmission();    // stop transmitting
    delay(10000);
  }
  delay(1000000);
}
