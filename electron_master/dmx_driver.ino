#include <Wire.h>

int led = D7;
bool lastFrameStored = false;
String keyframeArray[5] = {"","","","",""};
int lastFrameIndex = 0;
bool waitingForFrames = false;
unsigned long prevTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(led, OUTPUT);
  Particle.function("recFrames", recFrames);
  digitalWrite(led, LOW);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
}

int recFrames(String command) {

    Serial.print("Recieved: ");
    Serial.println(command);

    waitingForFrames = true;
    prevTime = millis();

    //TODO: rename this function

    Particle.publish(command);

    //TODO
    // POST status per pair after submitting frames, error message if non-zero return value
    // Stretchgoal: Live control w/ 'checkout key'

    // 23 -> Last frame, ends with F.
    // 44 -> Middle pair
    // 45 -> Last frame, ends with F.
    // Else, invalid

    // -1 denotes invalid data
    if(command.length() == 23){
      if(command[0] != 'k' || command[2] != 'r' || command[6] != 'g' || command[10] != 'b' || command[14] != 'm' || command[16] != 't' || command[22] != 'F') return -1;
    }
    else if(command.length() == 44){
      if(command[0] != 'k' || command[2] != 'r' || command[6] != 'g' || command[10] != 'b' || command[14] != 'm' || command[16] != 't') return -1;
      if(command[22] != 'k' || command[24] != 'r' || command[28] != 'g' || command[32] != 'b' || command[36] != 'm' || command[38] != 't') return -1;
    }
    else if(command.length() == 45){
      if(command[0] != 'k' || command[2] != 'r' || command[6] != 'g' || command[10] != 'b' || command[14] != 'm' || command[16] != 't') return -1;
      if(command[22] != 'k' || command[24] != 'r' || command[28] != 'g' || command[32] != 'b' || command[36] != 'm' || command[38] != 't' || command[44] != 'F') return -1;
    }
    else{
      return -1;
    }

    if(command[command.length()-1] == 'F'){
      //String toStore = command; //Copy the keyframe for modification
      lastFrameIndex = (command[1]-48)/2;
      Serial.print("last frame number: ");
      Serial.println(lastFrameIndex);
      //toStore.substring(0, command.length()-1); //remove the "last frame" indentifier
      if(keyframeArray[lastFrameIndex] != "") return 1;
      keyframeArray[lastFrameIndex] = command.substring(0, command.length()-1); // store the last keyframe in the correct position in the array
      lastFrameStored = true; //Set the "last keyframe received" flag
      Serial.println("Received last frame");
    }

    else{
      if(keyframeArray[(command[1]-48)/2] != "") return 1; // return 1 to denote frame data collision
      keyframeArray[(command[1]-48)/2] = command;
      Serial.print("frame number: ");
      Serial.println(command[1]-48);
      Serial.println("Not last keyframe");
    }

    int stringsRec = 0;
    if(lastFrameStored){
      Serial.println("Last frame has been stored, check for preceding frames");
      for(int i = 0; i < lastFrameIndex+1; i++){
        if(keyframeArray[i] != ""){
          stringsRec++;
        }
      }
      Serial.print("strings received: ");
      Serial.println(stringsRec);
      if(stringsRec-1 == lastFrameIndex){
        Serial.println("All frames are present, begin transmission");
        for(int i = 0; i < stringsRec; i++){
          if(keyframeArray[i].length() == 44){
            String firstHalf = keyframeArray[i].substring(0,22);
            String secondHalf = keyframeArray[i].substring(22);
            Wire.beginTransmission(8);
            delay(30);
            Wire.write(firstHalf);
            Serial.print("frame: ");
            Serial.println(firstHalf);
            Serial.print("i = ");
            Serial.println(i);
            Wire.endTransmission();
            delay(500);

            Wire.beginTransmission(8);
            delay(30);
            Wire.write(secondHalf);
            Serial.print("frame: ");
            Serial.println(secondHalf);
            Serial.print("i = ");
            Serial.println(i);
            Wire.endTransmission();
            delay(500);

          }
        else{
            Wire.beginTransmission(8);
            delay(30);
            Wire.write(keyframeArray[i]);
            Serial.print("frame: ");
            Serial.println(keyframeArray[i]);
            Serial.print("i = ");
            Serial.println(i);
            Wire.endTransmission();
          }

        }

        for(int i = 0; i < 5; i++){
          keyframeArray[i] = "";
        }
        Serial.println("Reset everything");
        lastFrameIndex = 0;
        lastFrameStored = false;
        waitingForFrames = false;
        prevTime = millis();

      }
    }

      return 0; //Return 0, indicating success

    }


void loop() {
  if(waitingForFrames){
    if(millis()-prevTime > 10000){
      for(int i = 0; i < 5; i++){
        keyframeArray[i] = "";
      }
      Serial.println("Reset everything");
      lastFrameIndex = 0;
      lastFrameStored = false;
      waitingForFrames = false;
      prevTime = millis();
    }
  }
}
