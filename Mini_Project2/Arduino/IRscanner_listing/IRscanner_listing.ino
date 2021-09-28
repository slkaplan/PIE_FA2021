#include <Servo.h>

#define DEBUG 0

Servo pan_servo;
Servo tilt_servo;

const int pan_range[2] = {50, 130};
const int tilt_range[2] = {0, 135};

const int pan_step = 3;
const int tilt_step = 2;

#define IR_PIN A0
#define PACKET_SIZE 3

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

int filterMin() {
    int N_SAMPLES = 3;
    int output = 1025;
    for (int i = 0; i < N_SAMPLES; i++) {
        output = min(output, readIR());
    }
    return output;
}

void scanAndTransmit() {
  int packet[PACKET_SIZE];
  bool response = false;

  for (int pan = pan_range[0]; pan < pan_range[1]; pan += pan_step) {
    for (int tilt = tilt_range[0]; tilt < tilt_range[1]; tilt += tilt_step) {
      packet[1] = pan;
      packet[2] = tilt;
      pan_servo.write(pan);
      tilt_servo.write(tilt);

      packet[0] = filterMin();

      response = sendPacket(packet); // returns true if the handshake is successful
      digitalWrite(LED_BUILTIN, response);

      // if the response was bad, re-send the packet until true
      while (response == false) {
        delay(100);
        response = sendPacket(packet);
      }

      delay(100); // helps ensure that the serial buffer doesn't overflow too quickly
    }
  }
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