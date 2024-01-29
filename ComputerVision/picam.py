from picamera2 import Picamera2, Preview
import time

picam = Picamera2()
camera_config = picam.create_preview_configuration()
picam.configure(camera_config)
picam.start_preview(Preview.QT, x=0, y= 0) #QT with ssh / QTGL on pi
picam.start()
time.sleep(10)
picam.capture_file("test.jpg")


