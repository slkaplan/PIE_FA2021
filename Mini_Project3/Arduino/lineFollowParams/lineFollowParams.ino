#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *MotorL = AFMS.getMotor(1);
Adafruit_DCMotor *MotorR = AFMS.getMotor(2);

int sensorLPin = A1;    // select the input pin for the potentiometer
int sensorRPin = A2;    // select the input pin for the potentiometer
int sensorXLPin = A0;
int sensorXRPin = A1;

int sensorLValue = 0;  // variable to store the value coming from the sensor
int sensorRValue = 0;
int sensorXLValue = 0;
int sensorXRValue = 0;

byte ledLPin = 7;
byte ledRPin = 8;

bool mosfetLOn = true;    
bool mosfetROn = true;    
bool pause = false;          // Uses char 'p'
float baseSpeed = 40;       // Uses char 'b'
float errorPercent = 0.3;   // Uses char 'e' base speed * error percent is the correction applied to each wheel
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
  Serial.begin(9600);
  pinMode(ledLPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  digitalWrite(ledLPin, mosfetLOn);
  digitalWrite(ledRPin, mosfetROn);
}

void loop() {
 
  MotorR->run(FORWARD);
  MotorL->run(BACKWARD);
  // Grab Serial Data and update values
  recvWithStartEndMarkers();
  updateParams();

  // Read the Sensors/update data
  sensorLValue = analogRead(sensorLPin);
  sensorRValue = analogRead(sensorRPin);
  sensorXLValue = analogRead(sensorXRPin);
  sensorXRValue = analogRead(sensorXLPin);

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
  if (sensorRValue > range && sensorLValue < range) {
    MotorR->setSpeed(speed - speed*errorPercent);
    MotorL->setSpeed(speed + speed*errorPercent);
    Serial.println("Turning Left");
  }
  else if (sensorRValue < range && sensorLValue > range) {
    MotorR->setSpeed(speed + (speed*errorPercent));
    MotorL->setSpeed(speed - (speed*errorPercent));
    Serial.println("Turning Right");
  }
  else {
    MotorR->setSpeed(speed);
    MotorL->setSpeed(speed);
    Serial.println("straight");
  }
}

// This acts similar to linefollow2, except with 4 sensors. The idea is that the outer sensors can catch
// any cases where the robot cannot turn quickly enough to follow the line. Any black on the outer sensors
// results in a much bigger turning response from the robot, designated at errorPercent * expo. This should
// allow the robot to handle tighter corners at faster baseline speeds.

void linefollow3(int speed, int range, int expo) {
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
  else {
    MotorR->setSpeed(speed);
    MotorL->setSpeed(speed);
  }
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    bool gotData = false;
    while (Serial.available() > 0) {
      newData = true;
      rc = Serial.read();
      receivedChars[ndx] = rc;
      ndx++;
    }
    if(newData){
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
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
//    Serial.print("Message ");
//    Serial.println(messageFromPC);
//    Serial.print("Float ");
//    Serial.println(floatFromPC);
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
