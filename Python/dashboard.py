import dash
from dash.dependencies import Input, Output
import dash_core_components as dcc
import dash_html_components as html
from pandas_datareader.data import DataReader
import pandas
import time
from collections import deque
import plotly.graph_objs as go
import numpy as np
from scipy import signal
from scipy.signal import blackman
import warnings
import os


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

app = dash.Dash('Accel-data', external_stylesheets=["https://cdnjs.cloudflare.com/ajax/libs/materialize/0.100.2/css/materialize.min.css"])

df = pandas.read_csv("data/data_235Hz.csv", usecols=['x', 'y', 'z'])

X = df['x']
Y = df['y']
Z = df['z']

freq = 4000
#print(X.values[1:].ravel())
Xy_fft, Xx_fft = fftPlot(X.values[1:].ravel(), 1/freq, title='X_FFT')
Yy_fft, Yx_fft = fftPlot(Y.values[1:].ravel(), 1/freq, title='Y_FFT')
Zy_fft, Zx_fft = fftPlot(Z.values[1:].ravel(), 1/freq, title='Z_FFT') 

files = os.listdir("data/")

data_dict = {
"X" : X,
"Y" : Y,
"Z" : Z,
"X_FFT": Xy_fft,
"Y_FFT": Yy_fft,
"Z_FFT": Zy_fft
}


app.layout = html.Div([
    html.Div([
        html.H2('Data', id='out',
                style={'float': 'left',
                    }),
        ]),
    dcc.Dropdown(id='Data-files',
                options=[{'label': s, 'value': s} for s in files],
                #value=[],
                multi=False
                ),
    dcc.Dropdown(id='vehicle-data-name',
                options=[{'label': s, 'value': s} for s in data_dict.keys()],
                value=['X', 'X_FFT'],
                multi=True
                ),
    html.Div(children=html.Div(id='graphs'), className='row')
    ], className="container",style={'width':'98%','margin-left':10,'margin-right':10,'max-width':50000})


@app.callback(Output('graphs', 'children'), 
[Input('vehicle-data-name', 'value')])
def display_graphs(selected_values):
    graphs = []
    
    if len(selected_values)>2:
        class_choice = 'col s12 m6 l4'
    elif len(selected_values) == 2:
        class_choice = 'col s12 m6 l6'
    else:
        class_choice = 'col s12'


    for values in selected_values:

        if 'fft' not in values.lower():
            data = go.Scatter(
                y=df[values.lower()],
                name='Scatter',
                fill="tozeroy",
                fillcolor="#6897bb"
                )
        elif 'x_fft' in values.lower():
            data = go.Scatter(
                x=Xx_fft,
                y=np.abs(Xy_fft),
                name='Scatter',
                fill="tozeroy",
                fillcolor="#6897bb"
                )
        elif 'y_fft' in values.lower():
            data = go.Scatter(
                x=Yx_fft,
                y=np.abs(Yy_fft),
                name='Scatter',
                fill="tozeroy",
                fillcolor="#6897bb"
                )
        elif 'z_fft' in values.lower():
            data = go.Scatter(
                x=Zx_fft,
                y=np.abs(Zy_fft),
                name='Scatter',
                fill="tozeroy",
                fillcolor="#6897bb"
                )


        graphs.append(html.Div(dcc.Graph(
            id=values,
            animate=True,
            figure={'data': [data],'layout' : go.Layout(
                                                        margin={'l':50,'r':1,'t':45,'b':1},
                                                        title='{}'.format(values))}
            ), className=class_choice))
    
    return graphs

''''@app.callback(Output('out', 'children'),[Input('get-samples', 'n_clicks')])
def get_samples(n_clicks):
    df = pandas.read_csv("data/data_235Hz.csv")
    X = df['x']
    Y = df['y']
    Z = df['z']

    freq = 4000
    #print(X.values[1:].ravel())
    Xy_fft, Xx_fft = fftPlot(X.values[1:].ravel(), 1/freq, title='X_FFT')
    Yy_fft, Yx_fft = fftPlot(Y.values[1:].ravel(), 1/freq, title='X_FFT')
    Zy_fft, Zx_fft = fftPlot(Z.values[1:].ravel(), 1/freq, title='X_FFT') 
    return 'Got it' '''

if __name__ == '__main__':
    app.run_server(debug=True)