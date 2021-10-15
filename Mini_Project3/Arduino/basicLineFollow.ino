#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *MotorL = AFMS.getMotor(1);
Adafruit_DCMotor *MotorR = AFMS.getMotor(2);



void setup() {
    AFMS.begin();
    MotorR->setSpeed(0);
    MotorL->setSpeed(0);
    Serial.begin(9600);
    
}

void loop() {
    MotorR->run(FORWARD);
    MotorL->run(BACKWARD);
    linefollow1(40, 750);
    Serial.print(analogRead(A0));
    Serial.println(analogRead(A3));
    
}

void linefollow1(int speed, int range) {
  if(analogRead(A0) > range && analogRead(A3) < range){
    MotorR->setSpeed(speed*0);
    MotorL->setSpeed(speed*1.5);
  }
  else if(analogRead(A0) < range && analogRead(A3) > range){
    MotorR->setSpeed(speed*1.5);
    MotorL->setSpeed(speed*0);
  }
  else{
    MotorR->setSpeed(speed);
    MotorL->setSpeed(speed);
  }
}
