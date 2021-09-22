from PyQt5 import QtWidgets, QtCore, QtGui
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import sys  # We need sys so that we can pass argv to QApplication
import os
from random import randint
import serial



class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):

        super(MainWindow, self).__init__(*args, **kwargs)

        arduinoComPort = "COM7"
        baudRate = 9600
        global serialPort
        serialPort = serial.Serial(arduinoComPort, baudRate, timeout=1)

        self.graphWidget = pg.PlotWidget()

        self.setCentralWidget(self.graphWidget)



        self.x = list(range(100))  # 100 time points
        self.y = [randint(0,100) for _ in range(100)]  # 100 data points

        self.graphWidget.setBackground('w')
        pen = pg.mkPen(color=(255, 0, 0))

        self.data_line =  self.graphWidget.plot(self.x, self.y, pen=pen)
        self.timer = QtCore.QTimer()
        self.timer.setInterval(50)
        self.timer.timeout.connect(self.update_plot_data)
        self.timer.start()



    def update_plot_data(self):


        ser_data = serialPort.readline().decode()
        while len(ser_data) == 0:
            ser_data = serialPort.readline().decode()


        self.x = self.x[1:]  # Remove the first y element.

        self.x.append(self.x[-1] + 1)  # Add a new value 1 higher than the last.



        self.y = self.y[1:]  # Remove the first

        self.y.append(float(ser_data))  # Add a new random value.



        self.data_line.setData(self.x, self.y)  # Update the data.





app = QtWidgets.QApplication(sys.argv)

w = MainWindow()



w.show()

sys.exit(app.exec_())