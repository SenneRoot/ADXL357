import dash
from dash.dependencies import Input, Output
import dash_core_components as dcc
import dash_html_components as html
from pandas_datareader.data import DataReader
import time
from collections import deque
import plotly.graph_objs as go
import random

app = dash.Dash('Accel-data')

max_length = 50
times = deque(maxlen=max_length)
X = deque(maxlen=max_length)
X_FFT = deque(maxlen=max_length)
Y = deque(maxlen=max_length)
Y_FFT = deque(maxlen=max_length)
Z = deque(maxlen=max_length)
Z_FFT = deque(maxlen=max_length)

data_dict = {
"X": X,
"X_FFT": X_FFT,
"Y": Y,
"Y_FFT": Y_FFT,
"Z": Z,
"Z_FFT": Z_FFT
}


app.layout = html.Div([
    html.Button('Get Samples', id='get-samples'),
    dcc.Dropdown(id='vehicle-data-name',
                options=[{'label': s, 'value': s} for s in data_dict.keys()],
                value=['X','X_FFT'],
                multi=True
                ),
    html.Div(id='output')
])


@app.callback(Output('output', 'children'), 
[Input('vehicle-data-name', 'value')])
def display_graphs(selected_values):
    graphs = []
    for values in selected_values:
        graphs.append(dcc.Graph(id=values, figure={'layout': {'title' : values}}))
    return graphs


if __name__ == '__main__':
    app.run_server(debug=True)