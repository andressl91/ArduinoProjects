import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 10000
#server_address = ('localhost', port)
server_address = ('192.168.20.6', port)
sock.bind(server_address)
print(f"Starting server, listning on port {port}")
sock.listen(1)

data_delivery = []
while True:
    # Wait for a connect

    connection, client_address = sock.accept()
    if connection:
        print(f"{client_address} says hello")
        try:
            print(f"Connection from {connection}")

            while True:
                data = connection.recv(2**4)
                # data_delivery.append(data)
                
                if data.decode('utf-8') == "S1":
                    
                    while data.decode('utf-8') != "End":
                        print(f"Recieved {data}")
                        data = connection.recv(2**4)
                        data_delivery.append(data.decode('utf-8'))
                        print(data_delivery)
                    
                    print("All sensor data recieved")
                    data_delivery = []

                #    print("Sending data back to client")
                #    connection.send(b"Got your stuff")
                    # connection.sendall(data)

                #    print(data_delivery)

                else:
                    print("No more data from client")
                    break
        finally:
            # Clean up the connection
            # connection.close()
            print("Finally")

