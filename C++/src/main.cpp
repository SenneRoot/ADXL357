//#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"


int main()
{
	vector<Sample> samples;
	ADXL357 adxl357;
	bool log = false;
	int time = 4;

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	//adxl357.dumpInfo();

	while (1)
	{
		char buf[MAXLINE];
		fread(&buf, 1, 1, stdin);

		if(!strcmp(buf, "log"))
		{
			log = true;
			fwrite(&buf, 1, 1, stdout);
			fflush(stdout);
		}

		if (log)
		{
			Logger logger(&adxl357);
			logger.log(samples, time, true);
		}

	}
}
