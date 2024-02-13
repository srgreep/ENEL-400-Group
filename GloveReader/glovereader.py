import serial
import matplotlib.pyplot as plt


port = 'COM9'
while True:
    try:
        ser = serial.Serial(port, 9600)
        break
    except serial.SerialException:
        port = input("Invalid port try again: ")

pinky, ring, middle, pointer, thumb = 78, 79, 80, 81, 82
# fig, ax = plt.subplots()
# fingers = ['pinky', 'ring', 'middle', 'pointer', 'thumb']
flexPercent =[0,0,0,0,0]
# bar_colors = ['tab:red', 'tab:blue', 'tab:green', 'tab:orange', 'tab:purple']

# bars = ax.bar(fingers, flexPercent, color=bar_colors)
# ax.set_title("Finger Flex")
# ax.set_ylim(0, 4050)  # Assuming sensor values are in the range 0-4050
# # Function to update the plot
# def update_plot():
#     for bar, value in zip(bars, flexPercent):
#         bar.set_height(value)
#     fig.canvas.draw()
#     plt.pause(0.001)


ser.write(pinky)
flexPercent[0] = ser.read(2)
ser.write(ring)
flexPercent[1] = ser.read(2)
ser.write(middle)
flexPercent[2] = ser.read(2)
ser.write(pointer)
flexPercent[3] = ser.read(2)
ser.write(thumb)
flexPercent[3] = ser.read(2)

print(flexPercent)
ser.close()


# update_plot()
