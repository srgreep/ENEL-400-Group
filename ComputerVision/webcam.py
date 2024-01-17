import numpy as np
import cv2
import sys


def test_camera():
    vid = cv2.VideoCapture(0)
    while(True): 
        ret, frame = vid.read()
        lower_red = np.array([0, 150, 100])
        upper_red = np.array([10, 255, 255])
        lower_blue = np.array([101,50,38])
        upper_blue = np.array([110,255,255])
        grey = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
        frame_HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        frame_BW = cv2.inRange(frame_HSV, lower_red, upper_red)
        ret, thresh = cv2.threshold(frame_BW, 127, 255, 1)
        contours, h = cv2.findContours(thresh, 1, 2)
        cv2.imshow('frame_BW', frame_BW)
        for c in contours:
            tempCircle = cv2.minAreaRect(c)
            led_width = int(tempCircle[1][1])
            led_height = int(tempCircle[1][0])
            approx = cv2.approxPolyDP(c, 0.01 * cv2.arcLength(c, True), True)
            M = cv2.moments(c) 
            if M['m00'] != 0.0: 
                x = int(M['m10']/M['m00']) 
                y = int(M['m01']/M['m00'])
            if len(approx)>10 and (led_width/2)>100:
                cv2.circle(frame,(x, y), int(led_width/2), (0, 255, 0), 2)      
        cv2.putText(frame, 'press ''q'' to quit', (0, 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
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

def track_center(vid):
    ret, frame = vid.read()
    lower_red = np.array([0, 150, 100])
    upper_red = np.array([10, 255, 255])
    lower_blue = np.array([101,50,38])
    upper_blue = np.array([110,255,255])
    frame_HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    frame_BW = cv2.inRange(frame_HSV, lower_red, upper_red)
    ret, thresh = cv2.threshold(frame_BW, 127, 255, 1)
    contours, h = cv2.findContours(thresh, 1, 2)
    for c in contours:
        tempCircle = cv2.minAreaRect(c)
        raduis = int(tempCircle[1][1]/2)
        approx = cv2.approxPolyDP(c, 0.01 * cv2.arcLength(c, True), True)
        M = cv2.moments(c) 
        if M['m00'] != 0.0: 
            x = int(M['m10']/M['m00']) 
            y = int(M['m01']/M['m00'])
        if len(approx)>10 and (raduis/2)>100:
            return x, y, raduis
    return -1, -1, -1

if __name__ == '__main__':
    vid = cv2.VideoCapture(0)
    screen_width = vid.get(cv2.CAP_PROP_FRAME_WIDTH)
    if(screen_width<1):
        print("camera failed")
        exit()
    while(True):
        x, y, r = track_center(vid)
        if x < screen_width/2 + 50:
            print("move right")
        elif x > screen_width/2 + 50:
            print("move left")
        elif x!=-1:
            print("you good")
        else:
            print("circle not seen")
    # After the loop release the cap object 
    vid.release() 
    # Destroy all the windows 
    cv2.destroyAllWindows() 