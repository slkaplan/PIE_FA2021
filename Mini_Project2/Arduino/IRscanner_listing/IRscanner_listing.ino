#include <Servo.h>

#define DEBUG 0 // set this to true to enable serial.println of debugging code segments.

Servo pan_servo;
Servo tilt_servo;

// pan and tilt angles to scan through
const int pan_range[2] = {50, 130};
const int tilt_range[2] = {0, 135};

// rotation in degrees between sensor readings
const int pan_step = 1;
const int tilt_step = 1;

#define IR_PIN A0
#define PACKET_SIZE 3

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    scanAndTransmit();
}

int readIR()
{
    return analogRead(IR_PIN);
}

int filterMin()
{
    // reads three samples and returns the minimum.
    // this is necessary to avoid spikes in the analog signal.
    int N_SAMPLES = 3;
    int output = 1025;
    for (int i = 0; i < N_SAMPLES; i++)
    {
        output = min(output, readIR());
    }
    return output;
}

void scanAndTransmit()
{
    int packet[PACKET_SIZE];
    bool response = false;

    // iterate over the entire scanning range
    for (int pan = pan_range[0]; pan < pan_range[1]; pan += pan_step)
    {
        for (int tilt = tilt_range[0]; tilt < tilt_range[1]; tilt += tilt_step)
        {
            packet[1] = pan;
            packet[2] = tilt;
            pan_servo.write(pan);
            tilt_servo.write(tilt);

            packet[0] = filterMin();

            response = sendPacket(packet); // returns true if the handshake is successful
            digitalWrite(LED_BUILTIN, response);

            // if the response was bad, re-send the packet until true
            while (response == false)
            {
                delay(100);
                response = sendPacket(packet);
            }

            delay(100); // helps ensure that the serial buffer doesn't overflow too quickly
        }
    }
}

void packetHeader() // send at the beginning of a packet
{
    Serial.print('\\');
}

void packetFooter() // send at the end of a packet
{
    Serial.print('\n');
}

bool sendPacket(int packet[PACKET_SIZE])
{
    // prints each value in packet separated by a comma
    // This format is used for parsing on the Python side.
    packetHeader();
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        Serial.print(packet[i]);

        if (i < PACKET_SIZE - 1)
        {
            Serial.print(',');
        }
    }
    packetFooter();
    return checkReceived(packet); // Returns true if the handshake is completed and the values match.
}

bool checkReceived(int sentPacket[PACKET_SIZE])
{
    // waits for a handshake from the Python code and checks if the received packet
    // matches the sent packet
    int receivedPacket[PACKET_SIZE];

    //delay until any packet is received
    while (Serial.available() <= 0)
    {
        if (DEBUG)
        {
            Serial.print("."); // wait for a packet response
            delay(50);
        }
    }
    // check if the received packet matches the sent one
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        if (Serial.available() <= 0)
        {
            // if the Serial buffer ends before we reach the
            // packet size, then the handshake has failed.
            if (DEBUG)
            {
                Serial.println("Message too short");
            }
            return false;
        }
        receivedPacket[i] = Serial.read() - '0'; // the - '0' converts from an ascii character to an equivalent integer
    }

    // debugging statements
    if (DEBUG)
    {
        if (!equalArrays(receivedPacket, sentPacket))
        {
            Serial.println("Message didn't match");
        }
        else
        {
            Serial.println("Message matches!");
        }

        Serial.print("receivedPacket = ");
        printArray(receivedPacket);
        Serial.print(", sentPacket = ");
        printArray(sentPacket);
        Serial.println("");
    }

    // after reading the first characters in PACKET_SIZE, ignore the rest
    flushBuffer();
    return equalArrays(receivedPacket, sentPacket);
}

void printArray(int array[PACKET_SIZE]) // prints each element an array; used for debugging
{
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        Serial.print(array[i]);
    }
}

bool equalArrays(int array1[PACKET_SIZE], int array2[PACKET_SIZE]) // checks if each element in two arrays are equal
{
    for (int i = 0; i < PACKET_SIZE; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}

void flushBuffer() // flush the serial buffer
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}