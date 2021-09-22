
import serial
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import numpy as np

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
serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

serialPackets = []
PACKET_SIZE = 3
serialPacket = [0] * PACKET_SIZE

pos = np.array([[0,0,0]])

color = (1,1,1,1)
size = 0.5

global scttrPlt 
scttrPlt = gl.GLScatterPlotItem(pos=pos, size=size, color=color, pxMode=False)
w.addItem(scttrPlt)


##
##  First example is a set of points with pxMode=False
##  These demonstrate the ability to have points with real size down to a very small scale 
## 
# pos = np.empty((53, 3))
# size = 10
# color = (1.0, 0.0, 0.0, 0.5)
# pos[0] = (0,0,0) 
# pos[1] = (0,0,0) 
# pos[2] = (0,1,0) 

# z = 0.5
# d = 6.0
# # for i in range(3,53):
# #     pos[i] = (0,0,z)
# #     size[i] = 2./d
# #     color[i] = (0.0, 1.0, 0.0, 0.5)
# #     z *= 0.5
# #     d *= 2.0
    
# sp1 = gl.GLScatterPlotItem(pos=pos, size=size, color=color, pxMode=False)
# # sp1.translate(5,5,0)
# w.addItem(sp1)

# n=5
# numX, startX, endX = n, -1, 1+n
# numY, startY, endY = n, -1, 1+n
# numZ, startZ, endZ = n, -1, 1+n

# X = np.linspace(startX, endX, numX)
# Y = np.linspace(startY, endY, numY)
# Z = np.linspace(startZ, endZ, numZ)

#position of scatter in 3D
# pos = np.array([[0,0,0] for i in X for j in Y for k in Z])



def update():
    while True:
        ser_data = serialPort.read(1)
        if (ser_data.decode('utf8') == '\\'):
            print("header")
            break

    for i in range(PACKET_SIZE):
        ser_data = serialPort.read(1)
        serialPacket[i] = (ser_data.decode('utf8'))
    
    in_data = float(serialPacket)
    scttrPlt.setData(pos=pos, color=color)
    serialPacket = [0] * PACKET_SIZE
    #serialPackets.append(serialPacket)

    print("".join(serialPacket))
    serialPort.write("".join(serialPacket).encode('utf8'))
    ## update volume colors
    
    
    # ## update surface positions and colors
    # global sp3, d3, pos3
    # z = -np.cos(d3*2+phase)
    # pos3[:,2] = z
    # color = np.empty((len(d3),4), dtype=np.float32)
    # color[:,3] = 0.3
    # color[:,0] = np.clip(z * 3.0, 0, 1)
    # color[:,1] = np.clip(z * 1.0, 0, 1)
    # color[:,2] = np.clip(z ** 3, 0, 1)
    # sp3.setData(pos=pos3, color=color)
    
t = QtCore.QTimer()
t.timeout.connect(update)
t.start(50)


## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
