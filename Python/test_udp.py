import socket
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import warnings
from scipy import signal
from scipy.signal import blackman
import sys

freq = 4000

def read_and_plot(column = 'x', block = False):
    if len(sys.argv) > 1:
	    path = '../data/' + sys.argv[1] + '.csv'
    else:
        print("File not found! Exiting...")
        quit()
    df = pd.read_csv(path, usecols=[column], index_col=False)
	
    dt = 1 / freq
    f0 = 1 / dt / 4
	
    sig = df.values.ravel()

    #fftPlot(sig, dt=None, block=block, title = column)
    fftPlot(sig, dt=dt, block=block, title = column)

	
	

def fftPlot(sig, dt=None, block=False, plot=True, title = 'Analytic FFT plot'):
    # here it's assumes analytic signal (real signal...)- so only half of the axis is required

    if dt is None:
        dt = 1
        t = np.arange(0, sig.shape[-1])
        xLabel = 'samples'
    else:
        t = np.arange(0, sig.shape[-1]) * dt
        xLabel = 'freq [Hz]'

    if sig.shape[0] % 2 != 0:
        warnings.warn("signal prefered to be even in size, autoFixing it...")
        t = t[0:-1]
        sig = sig[0:-1]
		
    w = blackman(t.shape[0])
	
    sig = signal.detrend(sig, axis=0)
    #newsig = sig - np.mean(sig)
    sigFFT = np.fft.fft(sig*w) / t.shape[0]  # divided by size t for coherent magnitude

    freq = np.fft.fftfreq(t.shape[0], d=dt)

    # plot analytic signal - right half of freq axis needed only...
    firstNegInd = np.argmax(freq < 0)
    freqAxisPos = freq[0:firstNegInd]
    sigFFTPos = 2 * sigFFT[0:firstNegInd]  # *2 because of magnitude of analytic signal

    if plot:
        plt.figure()
        plt.plot(freqAxisPos, np.abs(sigFFTPos))
        plt.xlabel(xLabel)
        plt.ylabel('mag')
        plt.title(title)
        plt.show(block=block)

    return sigFFTPos, freqAxisPos



if __name__ == "__main__":
    target_host = "10.219.2.89"
    target_port = 8080

    # create a socket object
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # send some data
    client.sendto("log\n".encode('utf-8'),(target_host,target_port))

    datalist = []
    # receive some data
    data, addr = client.recvfrom(4096)
    while (data.decode('utf-8') != 'done'):
        if data.decode('utf-8') is not '\n':
            datalist.append(data.decode('utf-8'))
            print(data.decode('utf-8'))
        data, addr = client.recvfrom(4096)

    dt = 1 / freq
    datalist.pop(0)
    sig = np.asarray(datalist)
    fftPlot(sig, dt=dt, block=True, title = 'x')


