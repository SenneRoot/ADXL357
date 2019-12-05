//#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"
//#include <mqtt/client.h>
#include <wiringPi.h>
#include <unistd.h>

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


int main()
{
	setupGPIO();
	vector<vector<Sample>> samples;
	ADXL357 adxl357;
	bool logged = false;
	double time = 0.005;

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();

	while (1)
	{
		Logger logger(&adxl357);

		if(!read_btn(btn_pin))
		{
			//samples.clear();
			vector<Sample> temp;
			while(!digitalRead(btn_pin))
			{
				logger.log(temp, time, true, true);
				printf("\rLogging ---> %6d", temp.size());
        fflush(stdout);
			}
			samples.push_back(temp);
			logged = true;
		}

		if(logged)
		{
			printf("\nDone! logged %2d times\n", samples.size());
			logged = false;
		}
	}
}
