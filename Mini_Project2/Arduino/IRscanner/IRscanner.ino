#include <Servo.h>
#include <RunningMedian.h>
#include <math.h>

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
  // manualSet();
  scan();
  delay(50);
}

int readIR() {
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
  return DISTANCE;
}

int moving_avg(int data){
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  READINGS[INDEX] = DISTANCE;           // Add the newest reading to the window
  SUM = SUM + DISTANCE;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  return AVERAGED;
//  Serial.print(DISTANCE);
//  Serial.print(",");

}

void scan() {
  int pan_step = 2;
  int tilt_step = 1;
  int n_steps = int(180 / tilt_step);
  int tilt_sweep[n_steps];
  int packet[3];

  for (int pan_ind = 0; pan_ind < n_steps; pan_ind ++) {
    int pan = pan_ind * tilt_step;
    for (int tilt = 0; tilt < 180; tilt += tilt_step) {
      packet[2] = tilt;
      Serial.print("pan: ");
      Serial.print(pan);
      pan_servo.write(pan);

      packet[1] = pan;
      Serial.print(", tilt: ");
      Serial.print(tilt);
      tilt_servo.write(tilt);

      delay(10);

      int IR = readIR();
      packet[0] = IR;
      Serial.print(", IR: ");
      Serial.print(IR);
      Serial.println("");

      // tilt_sweep[pan_ind] = packet;
    }
    // Serial.println(sizeof(tilt_sweep));
    // Serial.println(String(packet));
  }
}

void scanAndTransmit() {
  int pan_step = 2;
  int tilt_step = 1;
  int n_steps = int(180 / tilt_step);
  int tilt_sweep[n_steps];
  int packet[3];

  for (int pan_ind = 0; pan_ind < n_steps; pan_ind ++) {
    int pan = pan_ind * tilt_step;
    send_header();
    for (int tilt = 0; tilt < 180; tilt += tilt_step) {
      packet[1] = pan;
      packet[2] = tilt;
      pan_servo.write(pan);
      tilt_servo.write(tilt);

      packet[0] = readIR();
      response = sendPacket(packet);
    }
    send_footer();
  }
}

void manualSet() {
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
}

// function definitions from SerialComm.ino

void sweepHeader()
{
    Serial.print('H');
}

void sweepFooter()
{
    Serial.print('F');
}

void packetHeader()
{
    Serial.print('\\');
}

void packetFooter()
{
    Serial.print('\n');
}

bool sendPacket(int packet[PACKET_SIZE])
{
    packetHeader();
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        Serial.print(packet[i]);

        if (i < PACKET_SIZE - 1) {
            Serial.print(',');
        }
    }
    packetFooter();
    return checkReceived(packet);
}
