# ADXL357

C++ ADXL357 datalogger used to analyze machine vibrations

## Dependecies

1. [Paho mqtt c++](https://github.com/eclipse/paho.mqtt.cpp)
2. wiring pi

## Dashboard

The dashboard can be started by running dashboard.py (You may have to install [Dash](https://plot.ly/dash/)). This can be found in the [Python](Python) directory.

## Log MQTT messages

To log the messages published by the C++ program you can use the mqtt logger that find in [Python/mqtt_logger](Python/mqtt_logger) directory. See [Python/mqtt_logger/README](Python/mqtt_logger/README.md) for usage.
