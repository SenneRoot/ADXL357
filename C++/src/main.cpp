#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"
#include <wiringPi.h>
#include <unistd.h>
#include "Sender.hpp"


#define btn_pin 8

void setupGPIO()
{
		wiringPiSetup();

		pinMode(btn_pin, INPUT);
		pullUpDnControl(btn_pin, PUD_UP);
}

bool read_btn(int btnPin)
{
	//debounce pin
	if(!digitalRead(btnPin) || digitalRead(btnPin))
	{
		usleep(1);
		return digitalRead(btnPin);
	}
	//should never reach this part
	return true;
}

using namespace std;
using namespace std::chrono;

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	setupGPIO();
	vector<Sample> samples;
	ADXL357 adxl357;
	bool logged = false;
	double time = 0.005;

	Sender sender("tcp://localhost:1883", "", 0);

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

		if(!read_btn(btn_pin))
		{
			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));
			//be sure to start the sensor before logging Continuous to avoid starting and stopping the sensor
			adxl357.start();

			//log Continuous, the time parameter determines the polling interval
			while(!digitalRead(btn_pin))
			{
				logger.logContinuous(samples, rate, time, true);
				printf("\rLogging ---> %6d", samples.size());
        fflush(stdout);
			}
			//its now safe to put the sensor back in measurement mode
			adxl357.stop();
			logged = true;
		}

	 	//send the logged samples over MQTT protocol
		if(logged)
		{
			std::string date = std::string("\"") + tmbuf + std::string("\"");
			std::string sensor = "\"ADXL357\"";
			std::string nSamples = "\"" + to_string(samples.size()) + "\"";
			std::string sampleArr = "[";


			for (auto& sample : samples)
			{
				sampleArr += "\"" + to_string(sample.getX()) + "\"" + "," + "\"" + to_string(sample.getY()) + "\""  + "," + "\"" + to_string(sample.getZ()) + "\"" + ",";
			}

			sampleArr += "]";
			std::string payload =  "{" +  "\"Sensor\" : " + sensor + ", \"Time\" : " + date + ", \"NumberSamples\" : " + nSamples + ", \"Samples\" : " + sampleArr + "}";
			//Publish to the topic
			//top.publish(std::move(payload));
			sender.send(payload, "testTopic");
			printf("\nDone!\n");
			samples.clear();
			logged = false;
		}
	}

		// Disconnect
		return 0;
}
