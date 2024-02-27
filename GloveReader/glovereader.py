import serial
import matplotlib.pyplot as plt
import socket
from time import sleep

#function to update plot with new values
def update_plot():
    for bar, value in zip(bars, flexValues):
        bar.set_height(value)
    fig.canvas.draw()
    plt.pause(0.001)

#transmit and recieve finger values 100000 times
def calibration():
    for i in range(200):
        #pinky value
        ser.write(pinky)
        flexValues[0] = int(ser.readline().strip())
        maxValues[0] = max(flexValues[0], maxValues[0])
        minValues[0] = min(flexValues[0], minValues[0])
        #ring value
        ser.write(ring)
        flexValues[1] = int(ser.readline().strip())
        maxValues[1] = max(flexValues[1], maxValues[1])
        minValues[1] = min(flexValues[1], minValues[1])
        #middle value
        ser.write(middle)
        flexValues[2] = int(ser.readline().strip())
        maxValues[2] = max(flexValues[2], maxValues[2])
        minValues[2] = min(flexValues[2], minValues[2])
        #pointer value
        ser.write(pointer)
        flexValues[3] = int(ser.readline().strip())
        maxValues[3] = max(flexValues[3], maxValues[3])
        minValues[3] = min(flexValues[3], minValues[3])
        #thumb value
        ser.write(thumb)
        flexValues[4] = int(ser.readline().strip())
        maxValues[4] = max(flexValues[4], maxValues[4])
        minValues[4] = min(flexValues[4], minValues[4])

        # print(flexValues) # array of all 5 finger values in loop
        # print(maxValues)  # max values from the 5 fingers overall
        # print(minValues)  # min values from the 5 fingers overall
        # print()

def plot_fingers():
    for i in range(10000):
        #pinky value
        ser.write(pinky)
        flexValues[0] = map_range(int(ser.readline().strip()), minValues[0], maxValues[0])

        #ring value
        ser.write(ring)
        flexValues[1] = map_range(int(ser.readline().strip()), minValues[1], maxValues[1])

        #middle value
        ser.write(middle)
        flexValues[2] = map_range(int(ser.readline().strip()), minValues[2], maxValues[2])

        #pointer value
        ser.write(pointer)
        flexValues[3] = map_range(int(ser.readline().strip()), minValues[3], maxValues[3])

        #thumb value
        ser.write(thumb)
        flexValues[4] = map_range(int(ser.readline().strip()), minValues[4], maxValues[4])
        update_plot()

def unityHand(pointer, middle, ring, pinky, thumb):
    send = "%s, %s, %s, %s, %s" %(str(pointer), str(middle), str(ring), str(pinky), str(thumb))
    sock.sendto(str.encode(str(send)), serverAddressPort)
    sleep(.1)

def map_range(x, in_min, in_max):
  if(x ==0 or in_min==in_max):
      return 0
  return (x - in_min) * (100) // (in_max - in_min)

if __name__ == "__main__":
    port = 'COM9'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    serverAddressPort = ("127.0.0.1", 5052)
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
    minValues = [4000,4000,4000,4000,40000]
    bar_colors = ['tab:red', 'tab:blue', 'tab:green', 'tab:orange', 'tab:purple']
    bars = ax.bar(fingers, flexValues, color=bar_colors)
    ax.set_title("Finger Flex")
    ax.set_ylim(0, 100)
    calibration()
    plot_fingers()
    ser.close()