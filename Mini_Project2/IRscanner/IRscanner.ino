#include <Servo.h>
<<<<<<< HEAD
#include <RunningMedian.h>
=======
#include <math.h> 
>>>>>>> 82e6f9af2ef5cda183b54bd0eb9a09d84f455121

Servo pan_servo;
Servo tilt_servo;

#define pan_pot A1   // analog pin used to connect the potentiometer
#define tilt_pot A2
int pan_pot_val;    // variable to read the value from the analog pin
int tilt_pot_val;

int IR_MAX = 150;
int IR_MIN = 20;

int DISTANCE;

#define IR_PIN A0

// MOVING AVERAGE
#define WINDOW_SIZE 5
int INDEX = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;

RunningMedian pan_pot_median = RunningMedian(9);
RunningMedian tilt_pot_median = RunningMedian(9);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pan_servo.attach(9);
  tilt_servo.attach(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  pan_pot_val = analogRead(pan_pot);            // reads the value of the potentiometer (value between 0 and 1023)
  tilt_pot_val = analogRead(tilt_pot);            // reads the value of the potentiometer (value between 0 and 1023)

  pan_pot_median.add(pan_pot_val);
  pan_pot_val = pan_pot_median.getMedian();
  tilt_pot_median.add(tilt_pot_val);
  tilt_pot_val = tilt_pot_median.getMedian();

  pan_pot_val = map(pan_pot_val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  tilt_pot_val = map(tilt_pot_val, 0, 1023, 0, 180);

  Serial.print("pan_pot_val = ");
  Serial.print(pan_pot_val);
  Serial.print(",    tilt_pot_val = ");
  Serial.println(tilt_pot_val);

  // tilt_servo.write(90); // test case

  pan_servo.write(pan_pot_val);                  // sets the servo position according to the scaled value
  tilt_servo.write(tilt_pot_val);

  int IR_RAW = analogRead(IR_PIN);
  DISTANCE = 2906 * (pow(IR_RAW, -.899));

  // new eq:
  //DISTANCE = 660
  DISTANCE = moving_avg(DISTANCE);
//  if (DISTANCE >= IR_MAX){
//    DISTANCE = IR_MAX;
//  }
//  if (DISTANCE <= IR_MIN){
//    DISTANCE = IR_MIN;
//  }

<<<<<<< HEAD
  // Serial.println(AVERAGED);
=======
  //Serial.println(AVERAGED);
  Serial.println(M_E);
>>>>>>> 82e6f9af2ef5cda183b54bd0eb9a09d84f455121
  delay(50);
  //Serial.println(DISTANCE);


  //Serial.println(pan_pot_val);

}

int moving_avg(int data){
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  READINGS[INDEX] = DISTANCE;           // Add the newest reading to the window
  SUM = SUM + DISTANCE;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result

//  Serial.print(DISTANCE);
//  Serial.print(",");

}


// int pot_index = 0;
// int pot_readings[WINDOW_SIZE] = {0};
// int moving_median(int data) {
//   pot_readings[pot_index] = data;

// }

