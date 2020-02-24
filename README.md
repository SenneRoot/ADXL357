
# ADXL357 Logger and Analyzer
C++ ADXL357 datalogger used to analyze machine vibrations

## ADXL357 Logger (Raspberry Pi)

### Usage

Compile using make in [C++](C++) directory.

First run make in [C++ directory](C++):

    make

Then run with:

    ./bin/ADXL357 MQTT_BROKER_IP (optional, defaults to localhost)

### Dependecies

1. [Paho mqtt c++](https://github.com/eclipse/paho.mqtt.cpp)
2. wiring pi

(tested on a Raspberry Pi 3b+)

## Analyzing the data

### Dashboard

The dashboard can be started by running [dashboard.py](Python/dashboard/dashboard.py) (You may have to install [Dash](https://plot.ly/dash/)). This file can be found in the [Python](Python) directory.

![image](https://user-images.githubusercontent.com/19331519/75137020-b9eceb80-56e6-11ea-85c1-231d8fd2fbd6.png)

### Log MQTT messages

To log the messages published by the C++ program, you can use the mqtt logger which is found in [Python/mqtt_logger](Python/mqtt_logger) directory. See [Python/mqtt_logger/README](Python/mqtt_logger/README.md) for usage.

Example usage:

    python .\mqtt_data_logger.py -b localhost -t ADXL357 -q 1 -r 1 -s -m -d
