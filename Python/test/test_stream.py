import socket
import threading
import time
import matplotlib.pyplot as plt 
import numpy as np
import struct

CHUNK = 1024 * 4
RATE = 4000

target_host = "10.219.2.89"
target_port = 8080
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.sendto("log\n".encode('utf-8'),(target_host,target_port))

datalist = []

def log_data():
    while True:
        data, addr = client.recvfrom(4096)
        if data.decode('utf-8') is not '\n':
            datalist.append(data.decode('utf-8'))

if __name__ == "__main__":
    thread = threading.Thread(target=log_data)
    thread.start()
    
    time.sleep(2)
    fig, ax = plt.subplots()

    x = np.arange(0, 2 * CHUNK, 2)
    line, = ax.plot(x, np.random.rand(CHUNK))

    while True:
        show_data = datalist[:CHUNK]
        datalist = datalist[CHUNK:]

        line.set_ydata(show_data)
        fig.canvas.draw()
        fig.canvas.flush_events()


 

    # create a socket object
    

    # send some data
    

    
    # receive some data
    data, addr = client.recvfrom(4096)
    while (data.decode('utf-8') != 'done'):
        if data.decode('utf-8') is not '\n':
            datalist.append(data.decode('utf-8'))
        data, addr = client.recvfrom(4096)