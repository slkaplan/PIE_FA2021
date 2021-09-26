
import serial

arduinoComPort = "COM7"
baudRate = 9600
ser = serial.Serial(arduinoComPort, baudRate, timeout=1)



#
# main loop to read data from the Arduino, then display it
#
while True:
  #
  # ask for a line of data from the serial port, the ".decode()" converts the
  # data from an "array of bytes", to a string
  #
  lineOfData = ser.readline().decode()

  #
  # check if data was received
  #
  if len(lineOfData) > 0:
      data = ser.readline().decode()

      print(data)

    # #
    # # data was received, convert it into 4 integers
    # #
    # a, b, c, d = (int(x) for x in lineOfData.split(','))

    # #
    # # print the results
    # #
    # print("a = " + str(a), end="")
    # print(", b = " + str(b), end="")
    # print(", c = " + str(c), end="")
    # print(", d = " + str(d))
