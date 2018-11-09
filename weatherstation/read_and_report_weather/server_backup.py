import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 10000
#server_address = ('localhost', port)
server_address = ('192.168.20.20', port)
sock.bind(server_address)
print(f"Starting server, listning on port {port}")
sock.listen(1)

while True:
    # Wait for a connect

    connection, client_address = sock.accept()
    
    try:
        print(f"Connection from {connection}")

        while True:
            data = connection.recv(2**4)
            print(f"Recieved {data}")

            if data:
                print("Sending data back to client")
                connection.sendall(data)
            else:
                print("No more data from client")
                break
    finally:
        # Clean up the connection
        connection.close()

