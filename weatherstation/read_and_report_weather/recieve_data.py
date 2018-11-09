import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = socket.gethostname()
port = 12345

sock.bind((host, port))

sock.listen(5)


while True:
    c, addr = sock.accept()
    data = c.recv(1024)
    print(f"Connected from {str(addr)}")
    if not data:
        break

    print(f"From connected user {str(data)}")
    
   
