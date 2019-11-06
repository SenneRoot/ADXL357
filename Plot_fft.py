import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import warnings

def fftPlot(sig, dt=None, block=False, plot=True):
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

    sigFFT = np.fft.fft(sig) / t.shape[0]  # divided by size t for coherent magnitude

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
        plt.title('Analytic FFT plot')
        plt.show(block=block)

    return sigFFTPos, freqAxisPos


if __name__ == "__main__":
    # Import csv file
    df = pd.read_csv('data.csv', usecols=['z'], index_col=False)
	
    dt = 1 / 4000
    f0 = 1 / dt / 4
	
    #conv_arr= df.values
    #split matrix into 3 columns each into 1d array
	
    #sig = np.delete(conv_arr,[1,2],axis=1) 
    sig = df.values.ravel()
    
    #sig = df.to_numpy()
	
    #print(sig)
    #t = np.arange(0, 1 + dt, dt)
    #sig = np.sin(2 * np.pi * f0 * t)
    print(sig)
    fftPlot(sig, dt=dt)
    fftPlot(sig, block = True)
    #t = np.arange(0, 1 + dt, dt)
    #sig = np.sin(2 * np.pi * f0 * t) + 10 * np.sin(2 * np.pi * f0 / 2 * t)
    #fftPlot(sig, dt=dt, block=True)




