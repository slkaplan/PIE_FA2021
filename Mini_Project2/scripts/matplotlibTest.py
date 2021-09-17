import matplotlib.pyplot as plt
import numpy as np

def initialize_dimensions()
    yaw_range = [0, 180]
    pitch_range = [0, 180]

    yaw_num = 10
    pitch_num = 6

    yaw_vals =   np.linspace(yaw_range[0],   yaw_range[1],   yaw_num)
    pitch_vals = np.linspace(pitch_range[0], pitch_range[1], pitch_num)

    yaw, pitch = np.meshgrid(yaw_vals, pitch_vals)
    depth = np.zeros(yaw.size)

    fig = plt.figure()
    ax = plt.axes(projection='3d')

    print(depth)

    ax.scatter3D(yaw, depth, pitch, cmap='Greens')
    ax.set_xlabel('yaw')
    ax.set_ylabel('depth')
    ax.set_zlabel('pitch')

    plt.show()


print(7)