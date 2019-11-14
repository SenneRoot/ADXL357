import socket
import threading
import time

data = []
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
ip=socket.gethostbyname("10.219.2.89")
port=8081
address=(ip,port)
client.connect(address)  ## <--Add this line.

def log_data():
    while True:
        din = client.recv(1024)
        data.append(din.decode())

if __name__ == "__main__":
    thread = threading.Thread(target=log_data)
    thread.start()

    while True:
        if(len(data) > 0):
            data.pop(0)
        print(len(data))