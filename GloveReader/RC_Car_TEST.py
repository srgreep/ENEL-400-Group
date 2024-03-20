import serial
import time

if __name__ == "__main__":
    port = 'COM14'
    while True:
        try:
            ser = serial.Serial(port, 9600, timeout=0.5)
            break
        except serial.SerialException:
            port = input("Invalid port try again: ")


    ##############  stop the car  #################
    hand = bytearray([0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############    Forward      #################
    hand = bytearray([80,0,0,0,80]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############  stop the car  #################
    hand = bytearray([0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############    Backward      #################
    hand = bytearray([80,0,0,80,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############  stop the car  #################
    hand = bytearray([0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############    Left      #################
    hand = bytearray([80,80,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############  stop the car  #################
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############    Right      #################
    hand = bytearray([80,0,80,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
    ##############  stop the car  #################
    hand = bytearray([0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)



    ser.close()