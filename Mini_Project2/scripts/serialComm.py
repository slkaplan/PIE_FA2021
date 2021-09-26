
import serial

arduinoComPort = "COM7"
baudRate = 9600
global serialPort
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

serialPackets = []
PACKET_SIZE = 3
serialPacket = [0] * PACKET_SIZE

serialLine = []

while True:
    while True:
        ser_data = serialPort.read(1)
        if (ser_data.decode('utf8') == '\\'):
            print("header")
            break

    while True:
        ser_data = serialPort.read(1)
        ser_char = ser_data.decode('utf8')
        if (ser_char == '\n'):
            break
        serialLine.append(ser_char)

 # create string from list in order to use split operator. Returns a list of strings
    serialStrings = "".join(serialLine).split(',')
    serialLine = []
    print(serialStrings)
    for string in serialStrings:
        serialPort.write(string.encode('utf8'))
