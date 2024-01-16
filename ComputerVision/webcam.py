import numpy as np
import cv2
import sys

vid = cv2.VideoCapture(0) 
while(True): 
    ret, frame = vid.read() 
    grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    lower = np.array([0, 100, 20])
    upper = np.array([10, 255, 255])
    frame_HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    frame = cv2.inRange(frame_HSV, lower, upper)
    contours, _ = cv2.findContours(frame, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 

    


    light_level = np.average(grey)
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