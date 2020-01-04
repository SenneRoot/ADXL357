import dash
from dash.dependencies import Input, Output
import dash_core_components as dcc
import dash_html_components as html
from pandas_datareader.data import DataReader
import pandas as pd
import pandas
import time
from collections import deque
import plotly.graph_objs as go
import numpy as np
from scipy import signal
from scipy.signal import blackman
import scipy
import warnings
import os


def fft_plot(samples, freq, num_samples):
    fig_fft = go.Figure()
    fig = go.Figure()

    for column in samples:
        sam = samples[column]
        N = num_samples
        # sample spacing
        T = 1.0 / freq

        x = np.linspace(0.0, N*T, N)
        y = signal.detrend(sam)
        yf = scipy.fftpack.fft(y)
        xf = np.linspace(0.0, 1.0/(2.0*T), N/2)

        fig.add_trace(go.Scatter(y=sam, mode='lines', name=column))
        fig_fft.add_trace(go.Scatter(x=xf[0:], y=2.0/N * np.abs(yf[:N//2])[0:], mode='lines', name=column + '_fft'))

    return fig, fig_fft

app = dash.Dash('Accel-data', external_stylesheets=["https://cdnjs.cloudflare.com/ajax/libs/materialize/0.100.2/css/materialize.min.css"])
files_all = os.listdir("data/")
files = [i for i in files_all if i.endswith('.json')]
data_file = "data.json"

app.layout = html.Div([
    html.Div([
        html.H2('ADXL357 vibration analysis', id='out',
                style={'float': 'left',}),
        ]),
    html.Div([
    dcc.Dropdown(id='Data-files',
                options=[{'label': s, 'value': s} for s in files],
                value=data_file,
                multi=False
                ),
        ]),
    html.Div(children=html.Div(id='graphs'), className='row')
    ], className="container",style={'width':'98%','margin-left':10,'margin-right':10,'max-width':50000})


@app.callback(
    dash.dependencies.Output('Data-files', 'options'),
    [dash.dependencies.Input('Data-files', 'value')]
)
def update_date_dropdown(name):
    files_all = os.listdir("data/")
    files = [i for i in files_all if i.endswith('.json')]
    return [{'label': s, 'value': s} for s in files]

@app.callback(Output('graphs', 'children'), 
[Input('Data-files', 'value')])
def update_file_name(value):
    graphs = []
    print(value)
    data_file = value

    class_choice = 'col s12 m12 l12'

    d = pd.read_json("data/" + data_file)
    df = pd.DataFrame(d[['xSamples', 'ySamples', 'zSamples']])

    fig, fig_fft = fft_plot(df, d['Frequency'].values[0], d['NumberSamples'].values[0])

    graphs.append(html.Div(dcc.Graph(
            id="fig",
            animate=False,
            figure=fig
            ), className=class_choice))
    
    graphs.append(html.Div(dcc.Graph(
            id="fig_fft",
            animate=False,
            figure=fig_fft
            ), className=class_choice))
    
    return graphs

if __name__ == '__main__':
    app.run_server(debug=True)