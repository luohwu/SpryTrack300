import atracsys.stk as tracking_sdk

import platform
if platform.system() == 'Darwin':
    import matplotlib
    matplotlib.use("TkAgg")

import matplotlib.animation as animation
import matplotlib.pyplot as plt
import os
from collections import deque

# Replace atracsys.stk with atracsys.ftk for the fusionTrack.
def exit_with_error(error, tracking_system):
    print(error)
    answer = tracking_system.get_last_error()
    if answer[0] == tracking_sdk.Status.Ok:
        errors_dict = answer[1]
        for level in ['errors', 'warnings', 'messages']:
            if level in errors_dict:
                print(errors_dict[level])
    exit(1)


tracker = tracking_sdk.TrackingSystem()

if tracker.initialise() != tracking_sdk.Status.Ok:
    exit_with_error(
        "Error, can't initialise the atracsys SDK api.", tracker)

if tracker.enumerate_devices() != tracking_sdk.Status.Ok:
    exit_with_error("Error, can't enumerate devices.", tracker)

frame = tracking_sdk.FrameData()

if tracker.create_frame(False, 10, 20, 20, 10) != tracking_sdk.Status.Ok:
    exit_with_error("Error, can't create frame object.", tracker)

answer = tracker.get_enumerated_devices()
if answer[0] != tracking_sdk.Status.Ok:
    exit_with_error("Error, can't get list of enumerated devices", tracker)

print("Tracker with serial ID {0} detected".format(
    hex(tracker.get_enumerated_devices()[1][0].serial_number)))

answer = tracker.get_data_option("Data Directory")
if answer[0] != tracking_sdk.Status.Ok:
    exit_with_error("Error, can't read 'Data Directory' option", tracker)

geometry_path = answer[1]

for geometry in ['geometry110.ini']:
    if tracker.set_geometry(os.path.join(geometry_path, geometry)) != tracking_sdk.Status.Ok:
        exit_with_error("Error, can't create frame object.", tracker)

# Important!!!!!!!!!, the following option values might change in new environment, just check if the device is working or not using the demo app first
tracker.set_int_option(10,2173) # option 10, is Image Compression Threshold
tracker.set_int_option(11,110) # option 11, is Image Integration Time, proper values can be found from DEMO

options=tracker.get_enumerated_options()

def animate(i):
    global x
    global markers_indexes
    global current_marker_index
    x += 1
    tracker.get_last_frame(frame)

    marker_data = [(0, 0, 0), (0, 0, 0)]

    for marker in frame.markers:
        if not marker.geometry_id in markers_indexes:
            markers_indexes[marker.geometry_id] = current_marker_index
            current_marker_index += 1

        if markers_indexes[marker.geometry_id] < max_number_of_tracked_markers:
            marker_data[markers_indexes[marker.geometry_id]] = (
                marker.position[0], marker.position[1], marker.position[2])

    current_frame_data = (
        x, marker_data[0], marker_data[1])

    data.append(current_frame_data)
    ax[0, 0].relim()
    ax[0, 1].relim()
    ax[1, 0].relim()
    ax[0, 0].autoscale_view()
    ax[0, 1].autoscale_view()
    ax[1, 0].autoscale_view()
    for i in range(0, max_number_of_tracked_markers):
        axis_x_data = [data[x][0] for x in range(0, len(data))], [
            data[x][i+1][0] for x in range(0, len(data))]
        axis_x_lines[i].set_data(axis_x_data)
        axis_y_data = [data[x][0] for x in range(0, len(data))], [
            data[x][i+1][1] for x in range(0, len(data))]
        axis_y_lines[i].set_data(axis_y_data)
        axis_z_data = [data[x][0] for x in range(0, len(data))], [
            data[x][i+1][2] for x in range(0, len(data))]
        axis_z_lines[i].set_data(axis_z_data)

    # print real-time values
    # marker_data[0][0]:x, marker_data[0][1]:y, marker_data[0][2]:z
    ax[0, 0].title.set_text(f"Axis x {marker_data[0][0]:.2f}")
    ax[0, 1].title.set_text(f"Axis y {marker_data[0][1]:.2f}")
    ax[1, 0].title.set_text(f"Axis z {marker_data[0][2]:.2f}")



x = 0
fig, ax = plt.subplots(2, 2)
ax[-1, -1].axis('off')
data = deque([(x, (0, 0, 0), (0, 0, 0))], maxlen=50)
max_number_of_tracked_markers = 2
axis_x_lines = []
axis_y_lines = []
axis_z_lines = []
markers_indexes = {}
current_marker_index = 0

for i in range(0, max_number_of_tracked_markers):
    axis_x_data = [data[x][0] for x in range(0, len(data))], [
        data[x][i+1][0] for x in range(0, len(data))]
    axis_x_lines.append(ax[0, 0].plot(axis_x_data)[0])
    axis_y_data = [data[x][0] for x in range(0, len(data))], [
        data[x][i+1][1] for x in range(0, len(data))]
    axis_y_lines.append(ax[0, 1].plot(axis_y_data)[0])
    axis_z_data = [data[x][0] for x in range(0, len(data))], [
        data[x][i+1][2] for x in range(0, len(data))]
    axis_z_lines.append(ax[1, 0].plot(axis_z_data)[0])

ax[0, 0].title.set_text("Axis x")
ax[0, 1].title.set_text("Axis y")
ax[1, 0].title.set_text("Axis z")

ax[0, 0].set_ylabel("Position [mm]")
ax[0, 1].set_ylabel("Position [mm]")
ax[1, 0].set_ylabel("Position [mm]")

ani = animation.FuncAnimation(fig, animate, interval=10)

plt.subplots_adjust(top=0.87, bottom=0.11, left=0.18,
                    right=0.90, hspace=0.34, wspace=0.60)
plt.suptitle("x/y/z positions of markers")
plt.show()