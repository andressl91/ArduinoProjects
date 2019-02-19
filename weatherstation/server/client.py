import socket
import time
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
port = 1234
#server_address = ('localhost', port)
server_address = ('127.0.0.1', port)

# server_address = ('192.168.20.6', port)
print(f"connecting to {port}")
sock.connect(server_address)

try:

    # Send data
    message = b'S1'
    sock.sendall(message)
    time.sleep(1)
    #mock_sensor_data = b"Temperature: 10, Humidity: 20, Status: 0"
    #sock.sendall(mock_sensor_data)
    
    # Recieve data
    data = sock.recv(2**6)
    data = str(data, encoding='utf-8')
    print(f"received data: {data}")



finally:
    print("Closing socket")
    sock.close()
