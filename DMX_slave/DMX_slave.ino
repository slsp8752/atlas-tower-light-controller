/*
   DMX_slave - driver for an Arduino Pro Mini to receive DMX data via I2C and spit it out using the DmxSimple Library

   Teddy Lowe and Slaton Spangler, April 2017
*/

// TODO
// Parse keyframe data
// "k1r255g000b128m1t10000"
//


#include <Wire.h>
#include <DmxSimple.h>
#include <Wire.h>

#define DEBUG 1
#define INPUT_SIZE 22 // This is the size of the I2C buffer, in bytes
#define NUM_KEYS 10   // This is the total number of keyframes, for future use


// globals
char input[INPUT_SIZE];

// Keyframe structure

struct keyframe{
  int knum;       // keyframe number, 1-10
//  int lnum;       // light number, 1-4, future plan
  int red;        // red value, 0-255
  int green;      // green value, 0-255
  int blue;       // blue value, 0-255
  int mode;       // keyframe mode, currently unused = 0, snap = 1, fade = 2
  int duration;   // keyframe duration, 0-10000 ms
};

// initial setup, if you want more keyframes add them here and include them in the keyframes[] array
keyframe key0 = {.knum = 0, .red = 0, .green = 0, .blue = 0, .mode = 1, .duration = 500};
keyframe key1 = {.knum = 1, .red = 255, .green = 255, .blue = 255, .mode = 1, .duration = 500};
keyframe key2 = {.knum = 2, .red = 0, .green = 0, .blue = 0, .mode = 1, .duration = 500};
keyframe key3 = {.knum = 3, .red = 255, .green = 255, .blue = 255, .mode = 1, .duration = 500};
keyframe key4 = {.knum = 4, .red = 0, .green = 0, .blue = 0, .mode = 1, .duration = 500};
keyframe key5 = {.knum = 5, .red = 255, .green = 255, .blue = 255, .mode = 1, .duration = 500};
keyframe key6 = {.knum = 6, .red = 0, .green = 0, .blue = 0, .mode = 1, .duration = 500};
keyframe key7 = {.knum = 7, .red = 255, .green = 255, .blue = 255, .mode = 1, .duration = 500};
keyframe key8 = {.knum = 8, .red = 0, .green = 0, .blue = 0, .mode = 1, .duration = 500};
keyframe key9 = {.knum = 9, .red = 255, .green = 255, .blue = 255, .mode = 1, .duration = 500};

struct keyframe keyframes[NUM_KEYS] = {key0, key1, key2, key3, key4, key5, key6, key7, key8, key9};

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

  // data must be formatted exactly as follows - k1r255g000b128m1t10000
  // currently, if the first keyframe is malformed, the rest will be loaded over past data without regard to the current buffer
  
  int i = 0;
  char buffer[6];
  
  // Grab i2c data, it maxes out at 32 bytes
  while(Wire.available()){
    
    if (i >= INPUT_SIZE-1)
      break;

    input[i] = Wire.read();

    i++;
  }

  Serial.println(input);

  // check for malformed data
  if (input[0] != 'k' || input[2] != 'r' || input[6] != 'g' || input[10] != 'b' || input[14] != 'm' || input[16] != 't'){
    Serial.println(input[0]);
    Serial.println(input[2]);
    Serial.println(input[6]);
    Serial.println(input[10]);
    Serial.println(input[14]);
    Serial.println(input[16]);
    Serial.println("Malformed data, exiting parser...");
    return;
  }
  
  // check keyframe number - if 1, then clear other keyframes first
  // refill keyframe buffers
  
  if (input[1] == 1){
    for(int j = 0; j < NUM_KEYS; j++){
      keyframes[j].mode = 0;            // clear all keyframe modes until proven to be used
    }
  }
  
  // parse data from input, the brute force way

  //red
  buffer[0] = input[3];
  buffer[1] = input[4];
  buffer[2] = input[5];
  buffer[3] = '\0';
  keyframes[input[1]].red = atoi(buffer);

  //green
  buffer[0] = input[7];
  buffer[1] = input[8];
  buffer[2] = input[9];
  buffer[3] = '\0';
  keyframes[input[1]].green = atoi(buffer);

  //blue
  buffer[0] = input[11];
  buffer[1] = input[12];
  buffer[2] = input[13];
  buffer[3] = '\0';
  keyframes[input[1]].blue = atoi(buffer);

  //mode
  buffer[0] = input[15];
  buffer[1] = '\0';
  keyframes[input[1]].mode = atoi(buffer);

  //duration
  buffer[0] = input[17];
  buffer[1] = input[18];
  buffer[2] = input[19];
  buffer[3] = input[20];
  buffer[4] = input[21];
  buffer[5] = '\0';
  keyframes[input[1]].duration = atoi(buffer);

  if(DEBUG) Serial.println(keyframes[input[1]].blue);
  
}

