import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, ifft
import pandas as pd

# Import csv file
df = pd.read_csv('data.csv', usecols=['x'], index_col=False)
#print(df.head(0))

#plot data
plt.figure(figsize=(12,4))
df.plot(linestyle = '', marker = '*', color='r')
plt.savefig('test.png')
plt.show()

#FFT
#number of sample points
N = 32000
#frequency of signal (in Hz)
T = 1 / 4000
#create x-axis for time length of signal
x = np.linspace(0, N*T, N)
#create array that corresponds to values in signal
y = df
#perform FFT on signal
yf = fft(y)
#create new x-axis: frequency from signal
xf = np.linspace(0.0, 1.0/(2.0*T), N//2)
#plot results
plt.plot(xf, yf[0:N//2], label = 'signal')
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel(r'Spectral Amplitude')
plt.legend(loc=1)
plt.savefig('test_res.png')
plt.show()