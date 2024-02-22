import socket
from time import sleep
from math import cos
from math import pi
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverAddressPort = ("127.0.0.1", 5052)

#Palm
Sphere_0 = "[526, 32, 0,"
Sphere_1 = "480, 39, -21,"
#Thumb knuckle
Sphere_2 = "443, 57, -33,"
#Thumb
Sphere_3 = "417, 69, -44,"
Sphere_4 = "392, 73, -54,"

#pointer kunckle
Sphere_5 = "464, 149, -22,"
#pointer finger
Sphere_6 = "464, 195, -22,"
Sphere_7 = "495, 200, -40,"
Sphere_8 = "495, 200, -40,"
#middle knuckle
Sphere_9 = "496, 159, -14,"
#middle finger
Sphere_10 = "492, 209, -20,"
Sphere_11 = "490, 200, -29,"
Sphere_12 = "488, 250, -38,"
#ring knuckle
Sphere_13 = "525, 156, -18,"
#ring finger
Sphere_14 = "527, 205, -28,"
Sphere_15 = "527, 232, -37,"
Sphere_16 = "490, 257, -44,"
#pinky knuckle
Sphere_17 = "553, 145, -24,"
#pinky finger
Sphere_18 = "550, 184, -35,"
Sphere_19 = "555, 190, -40,"
Sphere_20 = "535, 180, -44]"

hand = {"pointer":200, "middle": 250, "ring": 257, "pinky":180}
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
   