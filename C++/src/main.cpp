#include <string>
#include <chrono>
#include <unistd.h>
#include <stdio.h>
#include <ctime>
#include <wiringPi.h>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "Sender.hpp"

void setupGPIO(vector<int> inputs, vector<int> outputs);
bool read_btn(int btnPin);

using namespace std;

int main(int argc, char *argv[])
{
	//create vector to save the samples into
	vector<Sample> samples;
	ADXL357 adxl357;
	const double polling_time = 0.005;
	const int btn_pin = 8;

	string file_name = "results";
	if (argc > 1)
	{
		file_name = argv[1];
	}

	// Setup the GPIO wiring pi lib, pass btn_pin in as a input
	setupGPIO({btn_pin}, {});
	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();
	double rate = adxl357.getRate();

	while (1)
	{
		Logger logger(&adxl357);
		string timeStamp;

		if (!read_btn(btn_pin))
		{
			timeStamp = getTimeStamp();
			//be sure to start the sensor before logging Continuous to avoid starting and stopping the sensor
			adxl357.start();

			//log Continuous, the polling_time parameter determines the polling interval
			while (!digitalRead(btn_pin))
			{
				logger.logContinuous(samples, rate, polling_time, false);
				printf("\rLogging ---> %6d", samples.size());
				fflush(stdout);
			}
			//its now safe to put the sensor back in standby mode
			adxl357.stop();
			logger.setLogged(true);
		}

		//send the logged samples over MQTT protocol (JSON Format)
		if (logger.logged())
		{
			cout << "\nSaving data..." << flush;
			std::string payload = buildPayload(samples, "ADXL357", rate, adxl357.get_range(), timeStamp, adxl357.getSensitivityFactor(), logger.numFifoOveranged());
			std::ofstream out(to_string(file_name) + ".json");
    		out << payload;
    		out.close();
			cout << "OK" << endl;
			samples.clear();
			logger.setLogged(false);
		}
		else
		{
			samples.clear();
		}
	}
	return 0;
}

string buildPayload(vector<Sample> &samples, string sensorName, double rate, int range, string timeStamp, double sensitivityFactor, int nfifoOverranged)
{
	string date = "\"" + timeStamp + "\""; //as a json string type
	string sensor = "\"" + sensorName + "\"";
	string sfreq = to_string(rate); //as a json number type
	string srange = to_string(range);
	string nSamples = to_string(samples.size());
	string sfifoOverranged = to_string(nfifoOverranged);

	string xSamples = "[";
	string ySamples = "[";
	string zSamples = "[";
	for (auto &sample : samples)
	{
		sample.convertSample(sensitivityFactor);
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

	return "{ \"Sensor\" : " + sensor + ", \"Frequency\" : " + sfreq + ", \"Range\" : " + srange + ", \"Time_stamp\" : " + date + ", \"NumberSamples\" : " + nSamples + ", \"NUM_FIFO_OVER\" : " + sfifoOverranged + ", \"xSamples\" : " + xSamples + ", \"ySamples\" : " + ySamples + ", \"zSamples\" : " + zSamples + "}";
}

string getTimeStamp()
{
	char tmbuf[32];
	time_t t = std::time(NULL);
	strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));
	//this will return time in for example, 2020-01-23 10:11:12
	return std::string(tmbuf);
	//this will return time in for example, thu 23 jan 10:11:12 2020
	//return std::string(ctime(&now));
}

void setupGPIO(vector<int> inputs, vector<int> outputs)
{
	wiringPiSetup();

	for (const auto &input : inputs)
	{
		pinMode(input, INPUT);
		pullUpDnControl(input, PUD_UP);
	}

	for (const auto &output : outputs)
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
