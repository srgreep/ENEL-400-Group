import socket
import cv2
import numpy as np

# Server IP address
SERVER_IP = "10.9.68.163"  # Replace with the IP address of your Raspberry Pi

# Server port
SERVER_PORT = 10001

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to the server
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print("Connected to server.")

    # Receive and display video stream
    while True:
        # Receive video frame
        frame_data = client_socket.recv(65536)  # Receive larger chunks
        if not frame_data:
            print("boo")
            break
        
        # Convert received data to numpy array
        frame_array = np.frombuffer(frame_data, dtype=np.uint8)
        
        # Decode the video frame
        frame = cv2.imdecode(frame_array, flags=cv2.IMREAD_COLOR)

        # Display the frame
        if frame is not None:
            cv2.imshow("Video Stream", frame)
        
        # Check for 'q' key to quit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Close connection
    client_socket.close()
    cv2.destroyAllWindows()

except Exception as e:
    print("Error:", e)
    client_socket.close()