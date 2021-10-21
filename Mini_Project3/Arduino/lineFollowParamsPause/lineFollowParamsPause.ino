#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SoftwareSerial.h>

#include <MedianFilterLib.h>

String Data = "";
bool newData = true;
char *strings[5]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
String radio_msg = "";

int A = 0;
int B = 0;
int C = 0;
int D = 0;
int E = 0;

MedianFilter<int> Af(20);
MedianFilter<int> Bf(20);
MedianFilter<int> Cf(20);
MedianFilter<int> Df(20);
MedianFilter<int> Ef(20);

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

int XRCutoff = 750;
int RCutoff = 700;
int LCutoff = 700;
int XLCutoff = 750;

int laston = 4;

byte ledLPin = 7;
byte ledRPin = 8;

bool mosfetLOn = true;    
bool mosfetROn = true;    
bool pause = false;          // Uses char 'p'
float baseSpeed = 40;       // Uses char 'b'
float errorPercent = 0.4;   // Uses char 'e' base speed * error percent is the correction applied to each wheel
float errorPercent2 = 0.9;  // char 'r'
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

unsigned long pmillis = 0;

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
  for(int i = 0; i < 20; i++) {
    getNewData();
  }
}

void loop() {

 
  // Grab Serial Data and update values
  if(millis() - pmillis > 1000){
    getNewData();
    pmillis = millis();
  }
  
  

  // Read the Sensors/update data
  sensorLValue = analogRead(sensorLPin);
  delay(1);
  sensorRValue = analogRead(sensorRPin);
  delay(1);
  sensorXLValue = analogRead(sensorXLPin);
  delay(1);

  sensorXRValue = analogRead(sensorXRPin);
  delay(1);


  
  


  // Act
  if(!pause){
    
    // Line Follow
    if(mode == 0){
      linefollow1(baseSpeed, sensorCutoff);
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


// This operates on the same premise as above but rather than on or off, it applies a negative and 
// positive correction to each wheel, which may make things smoother
void linefollow1(int speed, int range) {
  int motorRSpeed = 0;
  int motorLSpeed = 0;
  
  if (sensorXRValue > XRCutoff && sensorRValue > RCutoff) {
    laston = 3;
    motorRSpeed = 0;
    motorLSpeed = speed;
    //Serial.println("Turning Left");
    MotorR->run(BACKWARD);
    MotorL->run(BACKWARD);
    while(sensorLValue < LCutoff && sensorXLValue < XLCutoff){
      delay(1);
      sensorLValue = analogRead(sensorLPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
      //print_stat();
    }
    laston = 1;
  }
  // If both left sensors hit, sharp left corner. Turn right until a right sensor hits
  else if (sensorXLValue > XLCutoff && sensorLValue > LCutoff) {
    laston = 1;
    motorRSpeed = speed;
    motorLSpeed = 0;
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(FORWARD);
    while(sensorRValue < RCutoff && sensorXRValue < XRCutoff){
      delay(1);
      sensorRValue = analogRead(sensorRPin);
      MotorR->setSpeed(motorRSpeed);
      MotorL->setSpeed(motorLSpeed);
      //print_stat();
    }
    laston = 2;
  }
  else if (sensorXRValue > XRCutoff) {
    laston = 3;
    motorRSpeed = speed - speed*(errorPercent2);
    motorLSpeed = speed + speed*(errorPercent2);
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    MotorR->setSpeed(motorRSpeed);
    MotorL->setSpeed(motorLSpeed);
  }
  
  else if (sensorXLValue > XLCutoff) {
    laston = 0;
    motorRSpeed = speed + speed*(errorPercent2);
    motorLSpeed = speed - speed*(errorPercent2);
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    MotorR->setSpeed(motorRSpeed);
    MotorL->setSpeed(motorLSpeed);
  }
  
  else if (sensorRValue > RCutoff && sensorLValue < LCutoff) {
    laston = 2;
    motorRSpeed = speed - speed*(errorPercent);
    motorLSpeed = speed + speed*(errorPercent);
    //Serial.println("Turning Left");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    MotorR->setSpeed(motorRSpeed);
    MotorL->setSpeed(motorLSpeed);
  }
  else if (sensorRValue < RCutoff && sensorLValue > LCutoff) {
    laston = 1;
    motorRSpeed = speed + speed*(errorPercent);
    motorLSpeed = speed - speed*(errorPercent);
    //Serial.println("Turning Right");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    MotorR->setSpeed(motorRSpeed);
    MotorL->setSpeed(motorLSpeed);
  }
  else {
    if(laston == 0){
      motorLSpeed = 0;
      motorRSpeed = speed;
    }
    else if(laston == 3){
      motorRSpeed = 0;
      motorLSpeed = speed;
    }
    else{
      motorRSpeed = speed;
      motorLSpeed = speed;
    }
    //Serial.println("straight");
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    MotorR->setSpeed(motorRSpeed);
    MotorL->setSpeed(motorLSpeed);
  }
  
  //print_stat();
}

/*void print_stat(){
  Serial.print(sensorXLValue);
  Serial.print(",");
  Serial.print(sensorLValue);
  Serial.print(",");
  Serial.print(sensorRValue);
  Serial.print(",");
  Serial.print(sensorXRValue);
  Serial.print(",");
  Serial.print(motorRSpeed);
  Serial.print(",");
  Serial.print(motorLSpeed);
  Serial.println(";");
}*/

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











void getNewData(){

  newData = true;
      
      while (HC12.available() && newData == true) {
          char character = HC12.read(); // Receive a single character from the software serial port
          Data.concat(character); // Add the received character to the receive buffer
          
          //Serial.println("available");
          if (character == '\n') {
              int str_len = Data.length() + 1; 

              // Prepare the character array (the buffer) 
              char char_array[str_len];
              
              // Copy it over 
              Data.toCharArray(char_array, str_len);
              
              byte index = 0;
              ptr = strtok(char_array, ",");  // delimiter
              while (ptr != NULL) {
                strings[index] = ptr;
                index++;
                ptr = strtok(NULL, ",");
              }

              
//              A = strings[0];
//              B = strings[1];
//              C = strings[2];
//              D = strings[3];
                A = Af.AddValue(filter(atoi(strings[0])));
                B = Bf.AddValue(filter(atoi(strings[1])));
                C = Cf.AddValue(filter(atoi(strings[2])));
                D = Df.AddValue(filter(atoi(strings[3])));

                bool pause = false;
                baseSpeed = map(A, 0, 1000, 0, 100);         
                errorPercent = map(B, 0, 1000, 0, 100) / 100.0;
                errorPercent2 = map(C, 0, 1000, 0, 100) / 100.0;
                

              Serial.println("baseSpeed: " + String(baseSpeed));
              Serial.println("EP: " + String(errorPercent));
              Serial.println("EP2: " + String(errorPercent2));
              
              //Serial.println();
              //delay(100);
              /*for (int n = 0; n < index; n++)
               {  
                  if(n==0){A[i]=atoi(strings[n]);}
                  if(n==1){B=filter(strings[n]);}
                  if(n==2){C=filter(atoi(strings[n]));}
                  if(n==3){D=filter(atoi(strings[n]));}
                  Serial.print(n);
                  Serial.print("  ");
                  Serial.println(filter(atoi(strings[n])));
               }*/
  
              // Clear receive buffer so we're ready to receive the next line
              Data = "";
              newData = false;
   
          }
      
    
    
    }
}

int filter(int val) {
  if(val > 1019) {
    val = val / 10;
  }
  else if(val > 999) {
    val = 999;
  }
  return val;
}
