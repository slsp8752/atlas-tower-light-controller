/*
   DMX_slave - driver for an Arduino Pro Mini to receive DMX data via I2C and spit it out using the DmxSimple Library

   Teddy Lowe and Slaton Spangler, April 2017
*/

// TODO
// Parse keyframe data
// "k1lNr255g000b128mFt10000"
//


#include <Wire.h>
#include <DmxSimple.h>
#include <Wire.h>

#define DEBUG 1
#define INPUT_SIZE 32 //This needs to be a max-size, but currently functions as an absolute length.


// globals
char input[INPUT_SIZE];

// add details for keyframe data types

#define DEBUG 1

void setup() {

  if (DEBUG)
    Serial.begin(9600);

  // initialize the DMX output on pin 3
  DmxSimple.usePin(3);

  // We only use 4 lights, ignore other channels
  DmxSimple.maxChannel(4);

  // Start as I2C slave on address 8
  Wire.begin(8);
  // Receive the keyframes
  Wire.onReceive(receiveData);

}

void loop() {
  // Send dmx data!

}

void receiveData(int a) {
  
  int i = 0;
  
  while(Wire.available()){
    
    if (i >= INPUT_SIZE-1)
      break;

    input[i] = Wire.read();

    i++;
  }

  // check keyframe number - if 1, then clear other keyframes first
  // refill keyframe buffers
  
  
  Serial.println(input);
  
}

