#include <string>
#include <chrono>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"
#include <wiringPi.h>
#include <unistd.h>
#include "Sender.hpp"

void setupGPIO(vector<int> inputs, vector<int> outputs)
{
	wiringPiSetup();

	for(const auto& input : inputs)
	{
		pinMode(input, INPUT);
		pullUpDnControl(input, PUD_UP);
	}

	for(const auto& output : outputs)
	{
		pinMode(output, OUTPUT);
		pullUpDnControl(output, PUD_UP);
	}
}

bool read_btn(int btnPin)
{
	//debounce pin
	if (!digitalRead(btnPin) || digitalRead(btnPin))
	{
		usleep(1);
		return digitalRead(btnPin);
	}
	//should never reach this part
	return true;
}

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[])
{
	setupGPIO({8}, {});
	vector<Sample> samples;
	ADXL357 adxl357;
	bool logged = false;
	double time = 0.005;
	int btn_pin = 8;

	Sender sender("tcp://localhost:1883", "", 0, MQTTVERSION_3_1_1);

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();
	double rate = adxl357.getRate();

	while (1)
	{
		Logger logger(&adxl357);
		char tmbuf[32];

		if (!read_btn(btn_pin))
		{
			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));
			//be sure to start the sensor before logging Continuous to avoid starting and stopping the sensor
			adxl357.start();

			//log Continuous, the time parameter determines the polling interval
			while (!digitalRead(btn_pin))
			{
				logger.logContinuous(samples, rate, time, false);
				printf("\rLogging ---> %6d", samples.size());
				fflush(stdout);
			}
			//its now safe to put the sensor back in measurement mode
			adxl357.stop();
			logged = true;
		}

		//send the logged samples over MQTT protocol (JSON Format)
		if (logged)
		{
			cout << "\nsending data..." << flush;
			std::string sensor = "\"ADXL357\"";
			std::string freq = to_string(rate);
			std::string range = to_string(adxl357.get_range());
			std::string nSamples = to_string(samples.size());
			std::string date = std::string("\"") + tmbuf + std::string("\"");

			std::string xSamples = "[";
			std::string ySamples = "[";
			std::string zSamples = "[";
			for (auto &sample : samples)
			{
				sample.convertSample(adxl357.getSensitivityFactor());
				xSamples += to_string(sample.getX()) + ",";
				ySamples += to_string(sample.getY()) + ",";
				zSamples += to_string(sample.getZ()) + ",";
			}
			xSamples.pop_back();
			ySamples.pop_back();
			zSamples.pop_back();
			xSamples += "]";
			ySamples += "]";
			zSamples += "]";

			std::string payload = "{ \"Sensor\" : " + sensor + ", \"Frequency\" : " + freq + ", \"Range\" : " + range + ", \"Time_stamp\" : " + date + ", \"NumberSamples\" : " + nSamples + ", \"xSamples\" : " + xSamples + ", \"ySamples\" : " + ySamples + ", \"zSamples\" : " + zSamples + "}";
			//Publish to the topic
			sender.send(payload, "ADXL357");
			cout << "OK" << endl;
			samples.clear();
			logged = false;
		}
	}
	return 0;
}
