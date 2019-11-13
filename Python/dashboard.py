import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
import plotly.graph_objs as go
import plotly
import numpy as np
from scipy import signal
from scipy.signal import blackman
import warnings
import json
import socket


freq = 4000
df = pd.read_csv('data_235Hz.csv')

figx = go.Figure(go.Scatter(y = df['x'],
                    name='ADXL357 x '))

figx.update_layout(title='ADXL357 x',
                    plot_bgcolor='rgb(230, 230,230)',
                    showlegend=True)

figy = go.Figure(go.Scatter(y = df['y'],
                    name='ADXL357 y'))

figy.update_layout(title='ADXL357 y',
                    plot_bgcolor='rgb(230, 230,230)',
                    showlegend=True)

figz = go.Figure(go.Scatter(y = df['z'],
                    name='ADXL357 z '))

figz.update_layout(title='ADXL357 z',
                    plot_bgcolor='rgb(230, 230,230)',
                    showlegend=True)

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
    

    return sigFFTPos, freqAxisPos

def read_and_plot(column = 'x', block = False):
    path = 'data_235Hz.csv'
    df = pd.read_csv(path, usecols=[column], index_col=False)
    dt = 1 / freq
	
    sig = df.values.ravel()

    #fftPlot(sig, dt=None, block=block, title = column)
    return fftPlot(sig, dt=dt, block=block, title = column, plot=False)

y, x = read_and_plot()

figFFT = go.Figure(go.Scatter(x = x, y = np.abs(y),
                    name='ADXL357 fft '))

figFFT.update_layout(title='ADXL357 fft',
                    plot_bgcolor='rgb(230, 230,230)',
                    showlegend=True)

app = dash.Dash()

app.layout = html.Div([
    html.Div([
        html.Button('Get new data', id='button'),
        html.Div(id='container-button-basic',
                    children='go on')
    ]),

    html.Div([
        html.Div([
            html.H3('X'),
            dcc.Graph(id='g1', figure=figx)
        ], className="six columns"),

        html.Div([
            html.H3('Y'),
            dcc.Graph(id='g2', figure=figy)
        ], className="six columns"),

        html.Div([
            html.H3('Z'),
            dcc.Graph(id='g3', figure=figz)
        ], className="six columns"),

        html.Div([
            html.H3('FFT'),
            dcc.Graph(id='g4', figure=figFFT)
        ], className="six columns"),
    ], className="row")
])

def list_slice(S, step):
    return [S[i::step] for i in range(step)]

@app.callback(
    dash.dependencies.Output('container-button-basic', 'children'),
    [dash.dependencies.Input('button', 'n_clicks')])
def update_output(n_clicks):
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
        data, addr = client.recvfrom(4096)

    dt = 1 / freq
    datalist.pop(0)

    x = list_slice(datalist, 3)

    sigX = np.asarray(x[0])
    sigY = np.asarray(x[1])
    sigZ = np.asarray(x[2])

    y, x = fftPlot(sigX, dt=dt, block=False, title = 'x')

    figFFT = go.Figure(go.Scatter(x = x, y = np.abs(y),
                    name='ADXL357 fft '))

    #fftPlot(sigY, dt=dt, block=False, title = 'y')
    #fftPlot(sigZ, dt=dt, block=True, title = 'z')


    return 'the button has been clicked {} times'.format(n_clicks)


if __name__ == '__main__':
    app.run_server()