import pandas as pd
import matplotlib.pyplot as plt
import scipy.fftpack
import numpy as np
from scipy import signal
import plotly.graph_objects as go

def fft_plot(samples):
    fig, ax = plt.subplots(len(samples.columns))
    fig_fft = go.Figure()
    fig = go.Figure()
    i = 0
    for column in samples:
        sam = samples[column]
        N = sam.shape[0]
        # sample spacing
        T = 1.0 / 4000.0

        x = np.linspace(0.0, N*T, N)
        y = signal.detrend(sam)
        #y = sam
        yf = scipy.fftpack.fft(y)
        xf = np.linspace(0.0, 1.0/(2.0*T), N/2)

        
        #ax.plot(xf, 2.0/N * np.abs(yf[:N//2]))
        ax[i].plot(xf[1:], 2.0/N * np.abs(yf[:N//2])[1:])

        fig.add_trace(go.Scatter(y=sam, mode='lines', name=column))
        fig_fft.add_trace(go.Scatter(x=xf[1:], y=2.0/N * np.abs(yf[:N//2])[1:], mode='lines', name=column + '_fft'))
        i += 1
        #ax.semilogy(xf, 2.0/N * np.abs(yf[0:N//2]))
    fig_fft.show()
    fig.show()
    #plt.show()


if __name__ == '__main__':
    d = pd.read_json("data/data.json")

    df = pd.DataFrame(d[['xSamples', 'ySamples', 'zSamples']])
    x = df['xSamples'].to_numpy()

    #df.plot()
    #plt.show(block=False)

    fft_plot(df)