import matplotlib.pyplot as plt
import numpy as np

d = [5,10,15,20,25,30,35,40,45,50,55,60,65,70,75]
analog_val = [545,414,287,214,180,144,109,105,89,77,85,81,61,62,57]

fit = np.polyfit(d, np.log(analog_val), 1)
print(fit)

# plt.plot(d, analog_val)
# plt.show()