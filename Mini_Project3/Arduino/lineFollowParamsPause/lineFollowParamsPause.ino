#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *MotorL = AFMS.getMotor(1);
Adafruit_DCMotor *MotorR = AFMS.getMotor(2);

int sensorLPin = A1;    // select the input pin for the potentiometer
int sensorRPin = A2;    // select the input pin for the potentiometer
int sensorXLPin = A0;
int sensorXRPin = A3;

int sensorLValue = 0;  // variable to store the value coming from the sensor
int sensorRValue = 0;
int sensorXLValue = 0;
int sensorXRValue = 0;

byte ledLPin = 7;
byte ledRPin = 8;

bool mosfetLOn = true;    
bool mosfetROn = true;    
bool pause = false;          // Uses char 'p'
float baseSpeed = 55;       // Uses char 'b'
float errorPercent = 0.7;   // Uses char 'e' base speed * error percent is the correction applied to each wheel
float errorPercent2 = 0.8;  // char 'r'
float reduction = 0;
float leftRightMatch = 1.0; // If one wheel drives faster
int sensorCutoff = 600;     // Uses char 'c'  IR sensor cutoff, same for both sensors
int mode = 0;               // Uses char 'm' to change
    
// Variables for Serial Editing
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;

void setup() {
  AFMS.begin();
  MotorR->setSpeed(0);
  MotorL->setSpeed(0);
  HC12.begin(9600);               // Serial port to HC12
  Serial.begin(9600);
  pinMode(ledLPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  digitalWrite(ledLPin, mosfetLOn);
  digitalWrite(ledRPin, mosfetROn);
}

void loop() {
 
  // Grab Serial Data and update values
  recvWithStartEndMarkers();
  updateParams();

  // Read the Sensors/update data
  sensorLValue = analogRead(sensorLPin);
  sensorRValue = analogRead(sensorRPin);
  sensorXLValue = analogRead(sensorXLPin);
  sensorXRValue = analogRead(sensorXRPin);

  Serial.print(sensorXLValue);
  Serial.print(" ");
  Serial.print(sensorLValue);
  Serial.print(" ");
  Serial.print(sensorRValue);
  Serial.print(" ");
  Serial.println(sensorXRValue);
  


  // Act
  if(!pause){
    
    // Line Follow
    if(mode == 0){
      linefollow2(baseSpeed, sensorCutoff);
    }
    // Calibration
    else if(mode == 1){
    //TODO 
    }
    // Debug
    else if(mode == 2){
     //TODO
    }
    
  }
  else{
    MotorR->setSpeed(0);
    MotorL->setSpeed(0);
  }
}

void linefollow1(int speed, int range) {
  if (analogRead(A0) > range && analogRead(A3) < range) {
    MotorR->setSpeed(speed * 0);
    MotorL->setSpeed(speed * 1.5);
  }
  else if (analogRead(A0) < range && analogRead(A3) > range) {
    MotorR->setSpeed(speed * 1.5);
    MotorL->setSpeed(speed * 0);
  }
  else {
    MotorR->setSpeed(speed);
    MotorL->setSpeed(speed);
  }
}

// This operates on the same premise as above but rather than on or off, it applies a negative and 
// positive correction to each wheel, which may make things smoother
void linefollow2(int speed, int range) {
  int motorRSpeed = 0;
  int motorLSpeed = 0;
//  Serial.print(sensorXLValue);
//  Serial.print(" ");
//  Serial.print(sensorLValue);
//  Serial.print(" ");
//  Serial.print(sensorRValue);
//  Serial.print(" ");
//  Serial.println(sensorXRValue);

  // If both right sensors hit, sharp right corner. Turn right until a left sensor hits
  if (sensorXRValue > range && sensorRValue > range) {
//    MotorR->setSpeed(0);// + speed*errorPercent2);
//    MotorL->setSpeed(speed);// - speed*errorPercent2);
    motorRSpeed = 0;
    motorLSpeed = speed;
    //Serial.println("Turning Left");
    MotorR->run(BACKWARD);
    MotorL->run(BACKWARD);
    while(sensorLValue < range && sensorXLValue < range){
      delay(1);
      sensorLValue = analogRead(sensorLPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
    }
  }
  // If both left sensors hit, sharp left corner. Turn right until a right sensor hits
  else if (sensorXLValue > range && sensorLValue > range) {
//    MotorR->setSpeed(speed);// - speed*errorPercent2);//speed - speed*errorPercent2);
//    MotorL->setSpeed(0);// + speed*errorPercent2);// + speed*errorPercent2);
    motorRSpeed = speed;
    motorLSpeed = 0;
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(FORWARD);
    while(sensorRValue < range && sensorXRValue < range){
      delay(1);
      sensorRValue = analogRead(sensorRPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
    }
  }
  // If outer right sensor hits, sharp left
  else if (sensorXRValue > range) {
//    MotorR->setSpeed(speed - speed*(errorPercent2));//speed - speed*errorPercent2);
//    MotorL->setSpeed(speed + speed*(errorPercent2));// + speed*errorPercent2);
    motorRSpeed = speed - speed*(errorPercent2/2);
    motorLSpeed = speed + speed*(errorPercent2);
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    while(sensorRValue < range && sensorLValue < range){
      delay(1);
      sensorRValue = analogRead(sensorRPin);
      sensorLValue = analogRead(sensorLPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
    }
  }
  // If outer left sensor hits, sharp right
  else if (sensorXLValue > range) {
//    MotorR->setSpeed(speed + speed*(errorPercent2));// + speed*errorPercent2);
//    MotorL->setSpeed(speed - speed*(errorPercent2));//speed - speed*errorPercent2);
    motorRSpeed = speed + speed*(errorPercent2);
    motorLSpeed = speed - speed*(errorPercent2/2);
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    while(sensorLValue < range && sensorRValue < range){
      delay(1);
      sensorLValue = analogRead(sensorLPin);
      sensorRValue = analogRead(sensorRPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
    }
  }
  
  else if (sensorRValue > range && sensorLValue < range) {
//    MotorR->setSpeed(speed - speed*(errorPercent/1.5));
//    MotorL->setSpeed(speed + speed*errorPercent);
    motorRSpeed = speed - speed*(errorPercent/1.5);
    motorLSpeed = speed + speed*errorPercent;
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
  }
  else if (sensorRValue < range && sensorLValue > range) {
//    MotorR->setSpeed(speed + (speed*errorPercent));
//    MotorL->setSpeed(speed - (speed*(errorPercent/1.5)));
    motorLSpeed = speed - speed*errorPercent;
    motorRSpeed = speed + speed*(errorPercent/1.5);
    //Serial.println("Turning Right");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
  }
  else {
//    MotorR->setSpeed(speed*1.2);
//    MotorL->setSpeed(speed*1.2);
    motorRSpeed = speed*1.2;
    motorLSpeed = speed*1.2;
    //Serial.println("straight");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
  }
  MotorR->setSpeed(motorRSpeed);
  MotorL->setSpeed(motorLSpeed);

  String radio_msg = ""; 
  radio_msg = radio_msg + sensorXLValue + ',' + sensorLValue + ',' + sensorRValue +',' + sensorXRValue +','+ motorRSpeed+','+motorLSpeed+";\n";
  char copy[50];
  radio_msg.toCharArray(copy, 50);
  //Serial.println(copy);
  HC12.write(copy);
}

// This acts similar to linefollow2, except with 4 sensors. The idea is that the outer sensors can catch
// any cases where the robot cannot turn quickly enough to follow the line. Any black on the outer sensors
// results in a much bigger turning response from the robot, designated at errorPercent * expo. This should
// allow the robot to handle tighter corners at faster baseline speeds.

void linefollow3(int speed, int range, float expo) {
  if (sensorXRValue > range) {
    MotorR->setSpeed(speed - speed*(expo*errorPercent));
    MotorL->setSpeed(speed + speed*(expo*errorPercent));
  }
  else if (sensorXLValue > range) {
    MotorR->setSpeed(speed - speed*(expo*errorPercent));
    MotorL->setSpeed(speed + speed*(expo*errorPercent));
  }
  else if (sensorRValue > range && sensorLValue < range) {
    MotorR->setSpeed(speed - speed*errorPercent);
    MotorL->setSpeed(speed + speed*errorPercent);
  }
  else if ((sensorRValue < range) && (sensorLValue > range)) {
    MotorR->setSpeed(speed + speed*errorPercent);
    MotorL->setSpeed(speed - speed*errorPercent);
  }
  /*else {
    MotorR->setSpeed(speed);
    MotorL->setSpeed(speed);
  }*/
}
// IN PROGRESS (An idea I had that I didnt yet implement -Tigey)
void linefollow4(int speed, int range) {
  Serial.print(sensorXLValue);
  Serial.print(" ");
  Serial.print(sensorLValue);
  Serial.print(" ");
  Serial.print(sensorRValue);
  Serial.print(" ");
  Serial.print(sensorXRValue);
  Serial.print(" ");
  Serial.print(sensorLValue + 2*sensorXLValue);
  Serial.print(" ");
  Serial.println(sensorRValue + 2*sensorXRValue);
  
  int baseline = sensorRValue + 2*sensorXRValue + sensorLValue + 2*sensorXLValue;
  int binXR = 0;
  int binR = 0;
  int binL = 0;
  int binXL = 0;
  /*if(sensorXRValue > range{  int binXR = 1;  }
  if sensorRValue > range{  int binR = 1;  }
  if sensorLValue > range{  int binL = 1;  }
  if sensorXLValue > range{  int binXL = 1;  }
  
  MotorR->setSpeed(speed * binXR);
  MotorL->setSpeed(speed - speed*((sensorLValue + 2*sensorXLValue)/(baseline)));*/
}

//void recvWithStartEndMarkers() {
//    static boolean recvInProgress = false;
//    static byte ndx = 0;
//    char startMarker = '<';
//    char endMarker = '>';
//    char rc;
//    bool gotData = false;
//    while (Serial.available() > 0) {
//      newData = true;
//      rc = Serial.read();
//      receivedChars[ndx] = rc;
//      ndx++;
//    }
//    if(newData){
//      receivedChars[ndx] = '\0'; // terminate the string
//      ndx = 0;
//    }
//
//}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}


void showParsedData() {
  // UNcoment to show what is sent
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Float ");
    Serial.println(floatFromPC);
}

void updateParams(){
  if (newData == true) {
    strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
    char firstChar = messageFromPC[0];
    switch (firstChar) {
      case 'p':
        // pause
        pause = !pause;
        break;
      case 'b':
        // pause
        baseSpeed = floatFromPC;
        break;
      case 'e':
        // pause
        errorPercent = floatFromPC;
        break;
      case 'r':
        // pause
        errorPercent2 = floatFromPC;
        break;
      case 'c':
        // pause
        sensorCutoff = floatFromPC;
        break;
      case 'm':
        // pause
        mode = floatFromPC;
        break;
      default:
        // statements
        Serial.print("Could not match '");
        Serial.print(messageFromPC);
        Serial.print("' to a variable");
        break;
    }
  }
}
