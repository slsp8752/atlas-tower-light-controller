/*
   DMX_slave - driver for an Arduino Pro Mini to receive DMX data via I2C and spit it out using the DmxSimple Library

   Teddy Lowe and Slaton Spangler, April 2017
*/

// TODO
// Limit duration
// Normalize millis axis during fade calculation


#include <DmxSimple.h>
#include <Wire.h>
#include <avr/wdt.h>

#define DEBUG 0 // Set to 1 to get serial output for debugging
#define INPUT_SIZE 32 // This is the size of a keyframe, in bytes
#define NUM_KEYS 10   // This is the total number of keyframes, for future use
#define REDCHANNEL 1
#define GREENCHANNEL 2
#define BLUECHANNEL 3


// globals
char input[INPUT_SIZE];
bool newKeySet;
unsigned long previousTime = 0;

// Keyframe structure

struct keyframe{
  int knum;       // keyframe number, 1-10
//  int lnum;       // light number, 1-4, future plan
  int red;        // red value, 0-255
  int green;      // green value, 0-255
  int blue;       // blue value, 0-255
  int mode;       // keyframe mode, currently unused = 0, snap = 1, fade = 2
  unsigned long duration;   // keyframe duration, 0-10000 ms
};

// initial setup, if you want more keyframes add them here and include them in the keyframes[] array
keyframe key0 = {.knum = 0, .red = 255, .green = 0, .blue = 0, .mode = 2, .duration = 3000}; //Red
keyframe key1 = {.knum = 1, .red = 255, .green = 110, .blue = 255, .mode = 2, .duration = 3000}; //Orange
keyframe key2 = {.knum = 2, .red = 255, .green = 255, .blue = 0, .mode = 2, .duration = 3000}; //Yellow
keyframe key3 = {.knum = 3, .red = 0, .green = 255, .blue = 0, .mode = 2, .duration = 3000}; //Green
keyframe key4 = {.knum = 4, .red = 0, .green = 255, .blue = 255, .mode = 2, .duration = 3000}; //Cyan 
keyframe key5 = {.knum = 5, .red = 0, .green = 0, .blue = 255, .mode = 2, .duration = 3000}; //Blue
keyframe key6 = {.knum = 6, .red = 110, .green = 0, .blue = 255, .mode = 2, .duration = 3000}; //Purple
keyframe key7 = {.knum = 7, .red = 170, .green = 0, .blue = 255, .mode = 2, .duration = 3000}; //Burple 
keyframe key8 = {.knum = 8, .red = 255, .green = 0, .blue = 255, .mode = 2, .duration = 3000}; //Pink
keyframe key9 = {.knum = 9, .red = 255, .green = 255, .blue = 255, .mode = 2, .duration = 3000}; //White
keyframe currentKey = key0;
keyframe nextKey = key1;

struct keyframe keyframes[NUM_KEYS] = {key0, key1, key2, key3, key4, key5, key6, key7, key8, key9};
//struct keyframe keyframes[NUM_KEYS] = {key0, key1, key2, key3, key4, key5, key6, key7, key8};

void setup() {

  wdt_enable(WDTO_2S); // Watchdog timer - Resets the system if it stalls
  //Temp LED pins - Used to test system without a DMX light
  //pinMode(9, OUTPUT);
  //pinMode(10, OUTPUT);
  //pinMode(11, OUTPUT);

  if (DEBUG)
    Serial.begin(115200);

  newKeySet = true;
  
  // initialize the DMX output on pin 3
  DmxSimple.usePin(3);

  // Start as I2C slave on address 8
  Wire.begin(8);
  // Receive the keyframes
  Wire.onReceive(receiveData);
  DmxSimple.maxChannel(3);

}

void loop() 
{
  
  int dmxRed = 255;
  int dmxGreen = 255;
  int dmxBlue = 255;
  // Send dmx data!
  if(newKeySet)
  {  
    currentKey = keyframes[0];
    if(DEBUG){
      Serial.print("Key: ");
      Serial.print(currentKey.knum);
      Serial.print(" Red: ");
      Serial.print(currentKey.red);
      Serial.print(" Green: ");
      Serial.print(currentKey.green);
      Serial.print(" Blue: ");
      Serial.print(currentKey.blue);
      Serial.print(" Mode: ");
      Serial.print(currentKey.mode);
      Serial.print(" Duration: ");
      Serial.println(currentKey.duration);
    }
    
    incrementNextKey();
    
    newKeySet = false; // Reset new keyframe flag
    
  }
  else if(millis()-previousTime > currentKey.duration)
  {
    currentKey = nextKey;
    if(DEBUG){
      Serial.print("Key: ");
      Serial.print(currentKey.knum);
      Serial.print(" Red: ");
      Serial.print(currentKey.red);
      Serial.print(" Green: ");
      Serial.print(currentKey.green);
      Serial.print(" Blue: ");
      Serial.print(currentKey.blue);
      Serial.print(" Mode: ");
      Serial.print(currentKey.mode);
      Serial.print(" Duration: ");
      Serial.println(currentKey.duration);
    }

    incrementNextKey();
    
    previousTime = millis();
  }
  
  //Check fade mode
  int fadeTimeNow = millis() - previousTime;
  int fadeTimeEnd = currentKey.duration;
  
  switch(currentKey.mode){
    
    case 1: // Snap
      dmxRed = currentKey.red;
      dmxGreen = currentKey.green;
      dmxBlue = currentKey.blue;
      break;
    
    case 2: // Fade 
      dmxRed = calculateFade(nextKey.red, currentKey.red, fadeTimeEnd, fadeTimeNow); 
      dmxGreen = calculateFade(nextKey.green, currentKey.green, fadeTimeEnd, fadeTimeNow); 
      dmxBlue = calculateFade(nextKey.blue, currentKey.blue, fadeTimeEnd, fadeTimeNow); 

      break;
    
    default:
    
      break;
  }

  dmxRed = constrain(dmxRed, 0, 255);
  dmxGreen = constrain(dmxGreen, 0, 255);
  dmxBlue = constrain(dmxBlue, 0, 255);

  if(DEBUG){
    Serial.print("Red: ");
    Serial.print(dmxRed);
    Serial.print(" Green: ");
    Serial.print(dmxGreen);
    Serial.print(" Blue: ");
    Serial.println(dmxBlue);
  }

  DmxSimple.write(REDCHANNEL, dmxRed);
  DmxSimple.write(GREENCHANNEL, dmxGreen);
  DmxSimple.write(BLUECHANNEL, dmxBlue);

  // LED Output; Ued to test the system without a DMX light

  //analogWrite(10, dmxRed);
  //analogWrite(11, dmxGreen);
  //analogWrite(9, dmxBlue);

  if(DEBUG){
    Serial.print("RED: ");  
    Serial.println(dmxRed);
    Serial.print("GREEN: ");  
    Serial.println(dmxGreen);
    Serial.print("BLUE: ");  
  }

  //TODO: wait for a little
  delay(2);
  wdt_reset(); //Reset watchdog timer


}

int calculateFade(int valEnd, int valStart, int timeEnd, int timeNow){
  //return(((valEnd-valStart)/(timeEnd)) * (timeNow) + valStart);

  // Calculates the linear interpolation between two values. This is performed 3 times per color, per loop
  
  float p = (float)timeNow/(float)timeEnd;

  return((valStart * p) + (valEnd * (1-p)));
    
}

void incrementNextKey(){
  if(nextKey.knum == (NUM_KEYS - 1)) // This triggers if all frames are used and we've reached the last one
  {
    nextKey = keyframes[0];
  }
  else
  {
    if(keyframes[currentKey.knum + 1].mode){ // Check if next keyframe is used
      if(DEBUG){
        Serial.println(keyframes[currentKey.knum + 1].mode);
        Serial.println("frame exists, load next frame");
      }
    
      nextKey = keyframes[currentKey.knum + 1]; // If so, use it as the next keyframe
    }
    else // This triggers if the frames used is less than NUM_KEYS and we've reached the last frame
    {
      if(DEBUG) { Serial.println("frame doesn't exist, wrap around"); }
      nextKey = keyframes[0]; // else use the first keyframe
    }
  }
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

  int incomingFrame = input[1] - '0';
  if(DEBUG){ Serial.println(input); } //Print keyframe
  // check for malformed data
  if (input[0] != 'k' || input[2] != 'r' || input[6] != 'g' || input[10] != 'b' || input[14] != 'm' || input[16] != 't'){
    if(DEBUG){ Serial.println("Malformed data, exiting parser..."); }
    return;
  }
  
  // check keyframe number - if 1, then clear other keyframes first
  // refill keyframe buffers
  
  if (incomingFrame == 0){
    newKeySet = true;
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
  keyframes[incomingFrame].red = atoi(buffer);

  //green
  buffer[0] = input[7];
  buffer[1] = input[8];
  buffer[2] = input[9];
  buffer[3] = '\0';
  keyframes[incomingFrame].green = atoi(buffer);

  //blue
  buffer[0] = input[11];
  buffer[1] = input[12];
  buffer[2] = input[13];
  buffer[3] = '\0';
  keyframes[incomingFrame].blue = atoi(buffer);

  //mode
  buffer[0] = input[15];
  buffer[1] = '\0';
  keyframes[incomingFrame].mode = atoi(buffer);

  //duration
  buffer[0] = input[17];
  buffer[1] = input[18];
  buffer[2] = input[19];
  buffer[3] = input[20];
  buffer[4] = input[21];
  buffer[5] = '\0';
  keyframes[incomingFrame].duration = atoi(buffer);
  
}

