#include <Wire.h>

int led = D7;
int lastFrameStored = false;
String keyframeArray[5] = {"","","","",""};
int lastFrameIndex = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(led, OUTPUT);
  Particle.function("functionName", ledFunction);
  digitalWrite(led, LOW);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);


}

int ledFunction(String command) {

    Serial.print("Recieved: ");
    Serial.println(command);

    //TODO: rename this function

    Particle.publish(command);

    //Split keyframe string into multiple keyframes

    //if(command.length() % 22 != 0) return -1; //If the command string doesn't contain an exact number of frames, return an error code
    //if(command[0] != 'k' || command[2] != 'r' || command[6] != 'g' || command[10] != 'b' || command[14] != 'm' || command[16] != 't') return -1;
    //if(command[22] != 'k' || command[24] != 'r' || command[28] != 'g' || command[32] != 'b' || command[36] != 'm' || command[38] != 't') return -1; //If the string is malformed, return an error code

    if(command[command.length()-1] == 'F'){
      String toStore = command; //Copy the keyframe for modification
      lastFrameIndex = (command[1]-48)/2;
      Serial.print("last frame number: ");
      Serial.println(lastFrameIndex);
      toStore.substring(0, command.length()-1); //remove the "last frame" indentifier
      keyframeArray[lastFrameIndex] = toStore; // store the last keyframe in the correct position in the array
      lastFrameStored = true; //Set the "last keyframe received" flag
      Serial.println("Received last frame");
    }

    else{
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
          Wire.beginTransmission(8);
          delay(30);
          Wire.write(keyframeArray[i]);
          Serial.print("frame: ");
          Serial.println(keyframeArray[i]);
          Serial.print("i =");
          Serial.println(i);
          Wire.endTransmission();
          delay(500);
        }

        for(int i = 0; i < 5; i++){
          keyframeArray[i] = "";
        }
        Serial.println("Reset everything");
        lastFrameIndex = 0;
        lastFrameStored = false;

      }
    }

    //String toSend = "";
    //for(unsigned int i = 0; i < command.length()+2; i++){

      /*if(toSend.length() != 22){ //Individual keyframes are 22 characters long
        toSend += command[i];
        Serial.println(toSend);
        digitalWrite(led,HIGH);
      }

    else{

        if(toSend[0] != 'k' || toSend[2] != 'r' || toSend[6] != 'g' || toSend[10] != 'b' || toSend[14] != 'm' || toSend[16] != 't') return -1; //If the string is malformed, return an error code

        Wire.beginTransmission(8); // transmit to device #8
        delay(30);
        Wire.write(toSend);
        Serial.print("frame: ");
        Serial.println(toSend);
        Serial.print("i =");
        Serial.println(i);
        Wire.endTransmission();
        toSend = "";
        digitalWrite(led,LOW);
        delay(500);
      }*/
      return command.length(); //Return 0, indicating success

    }


void loop() {
}
