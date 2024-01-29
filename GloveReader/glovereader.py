import serial
import matplotlib.pyplot as plt
ser = serial.Serial('COM14');

fig, ax = plt.subplots()
fingers = ['pinky', 'ring', 'middle', 'pointer', 'thumb']
flexPercent =[0,0,0,0,0]
bar_colors = ['tab:red', 'tab:blue', 'tab:green', 'tab:orange', 'tab:purple']

bars = ax.bar(fingers, flexPercent, color=bar_colors)
ax.set_title("Finger Flex")
ax.set_ylim(0, 1023)  # Assuming sensor values are in the range 0-1023
# Function to update the plot
def update_plot():
    for bar, value in zip(bars, flexPercent):
        bar.set_height(value)
    fig.canvas.draw()
    plt.pause(0.001)


while True:
    input = ser.read(2)
    finger = input[0]>>5
    value = (input[0] & 0b011100000) + input[1]
    match finger:
        case 0b000:
            flexPercent[0] = value
        case 0b001:
            flexPercent[1] = value
        case 0b010:
            flexPercent[2] = value
        case 0b011:
            flexPercent[3] = value
        case 0b100:
            flexPercent[4] = value
        case _:
            print("DANGER")
    update_plot()
