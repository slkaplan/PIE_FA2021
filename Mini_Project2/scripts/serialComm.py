
import serial

arduinoComPort = "COM6"
baudRate = 9600
global serialPort
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

serialPackets = []
PACKET_SIZE = 3
serialPacket = [0] * PACKET_SIZE

while True:
    while True:
        ser_data = serialPort.read(1)
        if (ser_data.decode('utf8') == '\\'):
            print("header")
            break

    for i in range(PACKET_SIZE):
        ser_data = serialPort.read(1)
        serialPacket[i] = (ser_data.decode('utf8'))
    serialPackets.append(serialPacket)

    print("".join(serialPacket))
    serialPort.write("".join(serialPacket).encode('utf8'))
