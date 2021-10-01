
import serial
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np
import time
import sys
import math


def correction(data):
    # this function applies the calibration curve for the IR sensor

    # print for debugging
    print(data)

    if data <= 0:
        # if < 0, make very small so things w/exponents don't break
        data = 0.0000001
    if data >= 150:
        #if > 150, make 150 as that is the max range of sensor
        data = 150

<<<<<<< HEAD
    # this is our calibration equation
    return 2906.5 * pow(data, -.899)
=======
    # foo = 2906.5 * pow(data, -.899)
    foo = 15954 * pow(data, -1.087)
>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d


def xyx_transfrom(ir, pan, tilt):
<<<<<<< HEAD
    # this function takes polar coords (dist, pan+tilt angle) and converts to cartesian coords
    x = ir * math.cos(math.radians(pan))
    y = ir * math.sin(math.radians(pan))
    z = ir * math.sin(math.radians(tilt))
=======
    pan = math.radians(pan)
    tilt = math.radians(tilt)

    x = ir * math.cos(pan)
    y = ir * math.sin(pan)# / math.cos(pan)
    z = ir * math.cos(tilt)
>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d

    return [x,y,z]

def update():
    # this function is called by pyqtgraph and updates the plot. All comms w/Arduino happen here

    # this need to be global to be used in the plot (which lives outside this function)
    global count
    global pos1
    global serialPort
<<<<<<< HEAD
    
    # list for Arduino serial data
=======

>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d
    serialLine = []

    # this is the big Arduino comm loop
    while True:
        ser_data = serialPort.read(1) # read 1 byte from Arduino
        if (ser_data.decode('utf8') == '\\'): # check for header (sign to begin message)
            break
        print("WAITING")

    while True:
        ser_data = serialPort.read(1) # read 1 byte from Arduino
        ser_char = ser_data.decode('utf8') # convert bytes -> utf8
        if (ser_char == '\n'): # check for message end char
            break
        serialLine.append(ser_char) 

    # create string from list in order to use split operator. Returns a list of strings
    serialStrings = "".join(serialLine).split(',')
    serialLine = [] # reset serialLine for the next message

    # must convert strings to nums to actual nums!
    # also applying servo offsets
    ir_raw = int(serialStrings[0])

    ir_corrected = correction(ir_raw)
<<<<<<< HEAD
    pan_angle = int(serialStrings[1]) - 84 
    tilt_angle = 90 - int(serialStrings[2]) + 13
=======
    pan_angle = int(serialStrings[1]) - 80
    tilt_angle = int(serialStrings[2])
>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d

    # convert from polar to cartesian
    x, y, z = xyx_transfrom(ir_corrected, pan_angle, tilt_angle)
<<<<<<< HEAD
    
    # write data back to Arduino for error checking
=======


>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d
    for string in serialStrings:
        serialPort.write(string.encode('utf8'))



    # graphing stuff!
<<<<<<< HEAD
    new = np.array([[x,y,z]]) # creates new 1x3 array
    pos1 = np.append(pos1, new, 0) # append to current scatter array (holds all data)
    scttrPlt.setData(pos=pos1, color=color) # set the updated data to the scatter plot object
    
=======
    new = np.array([[x,y,z]]) # new 1x3
    pos1 = np.append(pos1, new, 0) # append to current scatter array
    scttrPlt.setData(pos=pos1, color=color) # reset data
    count = count + 1

>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d


if __name__ == '__main__':
    # creates QT gui app window
    app = QtGui.QApplication([])
    w = gl.GLViewWidget()
    w.opts['distance'] = 20
    w.show()
    w.setWindowTitle('3D IR Scanner')

    # setting grid options
    g = gl.GLGridItem()
    g.translate(0, -10, 0)
    g.setSize(200,200,200)
    w.addItem(g)

    # setting Arduino stuff
    arduinoComPort = "COM7"
    baudRate = 9600
    serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

<<<<<<< HEAD
    # creates original scatter data array
=======

    count = 0

    serialPackets = []
    PACKET_SIZE = 3
    serialPacket = [0] * PACKET_SIZE

>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d
    pos1 = np.array([[0,0,0]])

    # sets color to white
    color = (1,1,1,1)
    size = 0.5

<<<<<<< HEAD
    # needs to be global for update function to work
    global scttrPlt 
=======
    global scttrPlt
>>>>>>> 1a4641863c65a991b21af562fed10480fa5d533d
    scttrPlt = gl.GLScatterPlotItem(pos=pos1, size=size, color=color, pxMode=False)
    w.addItem(scttrPlt)

    # connects the window to the update function (will crash if something in the serial comm hangs)
    t = QtCore.QTimer()
    t.timeout.connect(update)

    t.start(50)

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
