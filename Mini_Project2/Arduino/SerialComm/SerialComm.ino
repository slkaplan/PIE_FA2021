#define PACKET_SIZE 3
#define DEBUG 0

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect
    }
}

int myPacket[PACKET_SIZE];
bool response;

void loop()
{
    transmitScan();
}

void transmitScan() {
    sweepHeader();
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < PACKET_SIZE; j++) {
            myPacket[j] = i;
        }
        response = sendPacket(myPacket);
        digitalWrite(LED_BUILTIN, response);
    }
    sweepFooter();
}

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

    while (Serial.available() <= 0)
    {
        if (DEBUG) {
            Serial.print("."); // wait for a packet response
            delay(200);
        }
    }
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

        flushBuffer();
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