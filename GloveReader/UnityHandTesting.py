import socket
from time import sleep
from math import cos
from math import pi
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverAddressPort = ("127.0.0.1", 5052)


pointer = 200
middle = 250
ring = 257
pinky = 180
t = 0
while(True):
    t = 0 if t >= 2*pi else t+0.1
    pointer = 50*cos(t)+50
    middle = 50*cos(t+1)+50
    ring = 50*cos(t+2)+50
    pinky = 50*cos(t+3)+50
    thumb = 50*cos(t+3.5)+50
    send = "%s, %s, %s, %s, %s" %(str(pointer), str(middle), str(ring), str(pinky), str(thumb))
    sock.sendto(str.encode(str(send)), serverAddressPort)
    sleep(.1)
   