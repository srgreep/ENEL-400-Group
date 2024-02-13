import serial
import matplotlib.pyplot as plt

port = 'COM9'
#check to see if port is there
while True:
    try:
        ser = serial.Serial(port, 9600, timeout=0.5)
        break
    except serial.SerialException:
        port = input("Invalid port try again: ")

#values to send via bluetooth to ESP32
pinky, ring, middle, pointer, thumb = b'N', b'O', b'P', b'Q', b'R'

#Plot setup
fig, ax = plt.subplots()
fingers = ['pinky', 'ring', 'middle', 'pointer', 'thumb']
flexValues =[0,0,0,0,0]
maxValues = [0,0,0,0,0]
minValues = [4000,4000,4000,4000]
bar_colors = ['tab:red', 'tab:blue', 'tab:green', 'tab:orange', 'tab:purple']
bars = ax.bar(fingers, flexValues, color=bar_colors)
ax.set_title("Finger Flex")
ax.set_ylim(0, 1000)
#function to update plot with new values
def update_plot():
    for bar, value in zip(bars, flexValues):
        bar.set_height(value)
    fig.canvas.draw()
    plt.pause(0.001)

#transmit and recieve finger values 100000 times
for i in range(10000):
    #pinky value
    ser.write(pinky)
    flexValues[0] = int(ser.readline().strip())
    maxValues[0] = max(flexValues[0], maxValues[0])
    #ring value
    ser.write(ring)
    flexValues[1] = int(ser.readline().strip())
    maxValues[1] = max(flexValues[1], maxValues[1])
    #middle value
    ser.write(middle)
    flexValues[2] = int(ser.readline().strip())
    maxValues[2] = max(flexValues[2], maxValues[2])
    #pointer value
    ser.write(pointer)
    flexValues[3] = int(ser.readline().strip())
    maxValues[3] = max(flexValues[3], maxValues[3])
    #thumb value
    ser.write(thumb)
    flexValues[4] = int(ser.readline().strip())
    maxValues[4] = max(flexValues[4], maxValues[4])

    print(flexValues) # array of all 5 finger values in loop
    print(maxValues)  # max values from the 5 fingers overall
    print(minValues)  # min values from the 5 fingers overall
    print()

    update_plot()
ser.close()
