
import serial
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np
import time
import sys


    


def update():
    global count
    global pos1
    # while True:
    #     ser_data = serialPort.read(1)
    #     if (ser_data.decode('utf8') == '\\'):
    #         print("header")
    #         break

    # for i in range(PACKET_SIZE):
    #     ser_data = serialPort.read(1)
    #     serialPacket[i] = (ser_data.decode('utf8'))
    
    # in_data = float(serialPacket)
    new = np.array([[count,count,count]])
    pos1 = np.append(pos1, new, 0)
    scttrPlt.setData(pos=pos1, color=color)
    count = count + 1
    print(count)
    # serialPacket = [0] * PACKET_SIZE
    #serialPackets.append(serialPacket)

    # print("".join(serialPacket))
    # serialPort.write("".join(serialPacket).encode('utf8'))
    




if __name__ == '__main__':
    app = QtGui.QApplication([])
    w = gl.GLViewWidget()
    w.opts['distance'] = 20
    w.show()
    w.setWindowTitle('pyqtgraph example: GLScatterPlotItem')

    g = gl.GLGridItem()
    w.addItem(g)

    arduinoComPort = "COM6"
    baudRate = 9600
    global serialPort
    # serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

    
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
