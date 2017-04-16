/*
 * DMX_slave - driver for an Arduino Pro Mini to receive DMX data via I2C and spit it out using the DmxSimple Library
 * 
 * Teddy Lowe and Slaton Spangler, April 2017
 */

#include <Wire.h>
#include <DmxSimple.h>

#define DEBUG 1

void setup() {
  // initialize the DMX output on pin 3
  DmxSimple.usePin(3);

  // We only use 4 lights, ignore other channels
  DmxSimple.maxChannel(4);

  // Start as I2C slave on address 8
  Wire.begin(8);  
  // Receive the keyframes
  Wire.onReceive(receiveData);

  if (DEBUG)
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveData(int a){
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
}

