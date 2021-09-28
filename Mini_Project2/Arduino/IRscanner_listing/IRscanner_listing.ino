#include <Servo.h>
#include <math.h>

#define DEBUG 0

int IR_MAX = 150;
int IR_MIN = 20;

int DISTANCE;

#define IR_PIN A0
#define PACKET_SIZE 3

// MOVING AVERAGE
#define WINDOW_SIZE 5
int INDEX = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  scanAndTransmit();
}

int readIR() {
  return analogRead(IR_PIN);
}

int bradStuff() {
    int N_SAMPLES = 3;
    int output = 1025;
    for (int i = 0; i < N_SAMPLES; i++) {
        output = min(output, readIR());
    }
    return output;
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

void scanAndTransmit() {
  int pan_range[2] = {50, 130};
  int tilt_range[2] = {0, 135};

  int pan_step = 3;
  int tilt_step = 2;

  int packet[PACKET_SIZE];
  bool response = false;

  for (int pan = pan_range[0]; pan < pan_range[1]; pan += pan_step) {
    for (int tilt = tilt_range[0]; tilt < tilt_range[1]; tilt += tilt_step) {
      packet[1] = pan;
      packet[2] = tilt;
      pan_servo.write(pan);
      tilt_servo.write(tilt);

      packet[0] = bradStuff();

      response = sendPacket(packet);
      digitalWrite(LED_BUILTIN, response);

      // if response was false, re-send the packet until true
      while (response == false) {
        delay(100);
        response = sendPacket(packet);
      }

      delay(100);
    }
    // packetFooter();
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


bool checkReceived(int sentPacket[PACKET_SIZE])
{
    int receivedPacket[PACKET_SIZE];

    //delay until any packet is received
    while (Serial.available() <= 0)
    {
        if (DEBUG) {
            Serial.print("."); // wait for a packet response
            delay(50);
        }
    }

    // check if the received packet matches the sent one
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        if (Serial.available() <= 0) {
            // if the Serial buffer ends before we reach the
            // packet size, then the handshake has failed.
            if (DEBUG) {
                Serial.println("Message too short");
            }
            return false;
        }
        receivedPacket[i] = Serial.read() - '0'; // the - '0' converts from an ascii character to an equivalent integer
    }

    // debugging code
    if (DEBUG) {
        if (!equalArrays(receivedPacket, sentPacket)) {
            Serial.println("Message didn't match");
        }
        else {
            Serial.println("Message matches!");
        }

        Serial.print("receivedPacket = ");
        printArray(receivedPacket);
        Serial.print(", sentPacket = ");
        printArray(sentPacket);
        Serial.println("");
    }

    flushBuffer(); // after reading the first characters in PACKET_SIZE, ignore the rest
    return equalArrays(receivedPacket, sentPacket);
}

void printArray(int array[PACKET_SIZE]) {
    for (int i = 0; i < PACKET_SIZE; i ++) {
        Serial.print(array[i]);
    }
}

bool equalArrays(int array1[PACKET_SIZE], int array2[PACKET_SIZE]) {
    for (int i = 0; i < PACKET_SIZE; i ++) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

void flushBuffer() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}