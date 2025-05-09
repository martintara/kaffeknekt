import os
import socket

path = '/tmp/socket'

if os.path.exists(path):
    os.unlink(path)
        

server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

server.bind(path)
server.listen(2)

print("Waiting for client connection")
connection, cli_addr = server.accept()
print("Connected:", cli_addr)

try:
    while True:
        data = connection.recv(1024)
        if not data:
            print("no data available")
            break
    
finally:
    connection.close()
    os.unlink(path)
