
import serial
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np
import time
import sys
import math


def correction(data):
    print(data)
    if data <= 0:
        data = 0.0000001

    # foo = 2906.5 * pow(data, -.899)
    foo = 15954 * pow(data, -1.087)

    # if foo <= 20:
    #     foo = 20
    # if foo >= 150:
    #     foo = 150

    return foo

def xyx_transfrom(ir, pan, tilt):
    pan = math.radians(pan)
    tilt = math.radians(tilt)

    x = ir * math.cos(pan)
    y = ir * math.sin(pan)# / math.cos(pan)
    z = ir * math.cos(tilt)

    # print(f"DIST: {x} Y: {y} Z: {z}")

    return [x,y,z]


def update():
    global count
    global pos1
    global serialPort

    serialLine = []

    while True:
        ser_data = serialPort.read(1)
        if (ser_data.decode('utf8') == '\\'):
            break
        print("WAITING")

    while True:
        ser_data = serialPort.read(1)
        ser_char = ser_data.decode('utf8')
        if (ser_char == '\n'):
            break
        serialLine.append(ser_char)

    # create string from list in order to use split operator. Returns a list of strings
    serialStrings = "".join(serialLine).split(',')
    serialLine = []

    ir_raw = int(serialStrings[0])

    ir_corrected = correction(ir_raw)
    pan_angle = int(serialStrings[1]) - 80
    tilt_angle = int(serialStrings[2])

    x, y, z = xyx_transfrom(ir_corrected, pan_angle, tilt_angle)


    for string in serialStrings:
        serialPort.write(string.encode('utf8'))



    # graphing stuff!
    new = np.array([[x,y,z]]) # new 1x3
    pos1 = np.append(pos1, new, 0) # append to current scatter array
    scttrPlt.setData(pos=pos1, color=color) # reset data
    count = count + 1



if __name__ == '__main__':
    app = QtGui.QApplication([])
    w = gl.GLViewWidget()
    w.opts['distance'] = 20
    w.show()
    w.setWindowTitle('pyqtgraph example: GLScatterPlotItem')

    g = gl.GLGridItem()
    g.translate(0, -10, 0)
    g.setSize(200,200,200)
    w.addItem(g)

    arduinoComPort = "COM7"
    baudRate = 9600
    serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)


    count = 0

    serialPackets = []
    PACKET_SIZE = 3
    serialPacket = [0] * PACKET_SIZE

    pos1 = np.array([[0,0,0]])

    color = (1,1,1,1)
    size = 0.5

    global scttrPlt
    scttrPlt = gl.GLScatterPlotItem(pos=pos1, size=size, color=color, pxMode=False)
    w.addItem(scttrPlt)

    t = QtCore.QTimer()
    t.timeout.connect(update)

    t.start(50)



    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
