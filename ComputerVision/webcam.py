import numpy as np
import cv2
import sys

vid = cv2.VideoCapture(0) 
while(True): 
    ret, frame = vid.read() 
    grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    lower_red = np.array([0, 100, 20])
    upper_red = np.array([10, 255, 255])
    lower_blue = np.array([101,50,38])
    upper_blue = np.array([110,255,255])
    frame_HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    frame = cv2.inRange(frame_HSV, lower_red, upper_red)
    contours, _ = cv2.findContours(frame, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
    for c in contours:
        tempCircle = cv2.minAreaRect(c)
        width = int(tempCircle[1][1])
        height = int(tempCircle[1][0])
        approx = cv2.approxPolyDP( c, 0.01 * cv2.arcLength(c, True), True)
        x = int(tempCircle[0][0])
        y = int(tempCircle[0][1])
        if(width+height/2 >500 & len(approx) ==0):
            cv2.putText(frame, 'circle', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
    
    cv2.imshow('frame', frame) 
    # the 'q' button is set as the 
    # quitting button you may use any 
    # desired button of your choice 
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
  
# After the loop release the cap object 
vid.release() 
# Destroy all the windows 
cv2.destroyAllWindows() 