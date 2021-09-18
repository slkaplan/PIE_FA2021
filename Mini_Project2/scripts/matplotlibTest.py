import matplotlib.pyplot as plt
import numpy as np

yaw_num = 10
pitch_num = 6

yaw_range = [0, 180]
pitch_range = [0, 180]

def initialize_dimensions():
    yaw_vals =   np.linspace(yaw_range[0],   yaw_range[1],   yaw_num)
    pitch_vals = np.linspace(pitch_range[0], pitch_range[1], pitch_num)

    yaw, pitch = np.meshgrid(yaw_vals, pitch_vals)
    depth = np.zeros(yaw.shape)

    return yaw, pitch, depth

def set_reading(new_yaw, new_pitch, new_depth):
    # find the raster reading closest to the actual reading and set it to new_depth
    yaw_loc = round(new_yaw / (yaw_range[1] - yaw_range[0]) * yaw_num)
    pitch_loc = round(new_pitch / (pitch_range[1] - pitch_range[0]) *s pitch_num)

    depth[yaw_loc, pitch_loc] = new_depth


yaw, pitch, depth = initialize_dimensions()
set_reading(100, 20, 2)

fig = plt.figure()
ax = plt.axes(projection='3d')

print(yaw)
print(depth)


ax.scatter3D(yaw, depth, pitch, cmap='Greens')
ax.set_xlabel('yaw')
ax.set_ylabel('depth')
ax.set_zlabel('pitch')

plt.show()


print(7)