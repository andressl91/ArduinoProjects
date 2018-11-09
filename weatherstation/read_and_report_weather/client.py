import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
port = 10000
server_address = ('localhost', port)
print(f"connecting to {port}")
sock.connect(server_address)

try:

    # Send data
    message = b'This is the message.  It will be repeated.'
    print(f"sending message {message}")
    sock.sendall(message)

    # Look for the response
    amount_received = 0
    amount_expected = len(message)

    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print(f"received data: {data}")


finally:
    print("Closing socket")
    sock.close()
