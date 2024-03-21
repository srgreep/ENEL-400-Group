import serial
import time


def write_to_rc(hand):
    start_time = time.time()
    while (time.time() - start_time) < 2:
        ser.write(hand)
        c = ser.read()
        print(c)
        # while(c != b'~'):
        #     c = ser.read()

 
if __name__ == "__main__":
    port = 'COM13'
    while True:
        try:
            ser = serial.Serial(port, 9600, timeout=0.5)
            break
        except serial.SerialException:
            port = input("Invalid port try again: ")
 
 
    ##############  stop the car  #################
    print("stop")
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############    Forward      #################
    print("forward")
    hand = bytearray([126,31,0,0,0,50]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  stop the car  #################
    print("stop")
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############    Backward      #################
    print("backward")
    hand = bytearray([126,40,0,0,80,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  stop the car  #################
    print("stop")
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  Forward Left  #################
    print("forward left")
    hand = bytearray([126,40,50,0,0,80]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  stop the car  #################
    print("stop")
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  Forward Right  #################
    print("forward right")
    hand = bytearray([126,40,0,50,0,80]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
    ##############  stop the car  #################
    print("stop")
    hand = bytearray([126,0,0,0,0,0]) #thumb, pointer, middle, ring, pinky
    write_to_rc(hand)
 

 
    ser.close()