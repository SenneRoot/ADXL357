#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

int main()
{
	vector<Sample> samples;
	ADXL357 adxl357;
	bool writeData = true;
	bool log = true;
	int time = 4;

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(0b000, SET_ODR_4000);
	adxl357.dumpInfo();

	if(log)
	{
		Logger logger(&adxl357);
		clock_t begin = clock();
		cout << "Starting Logging dat for " << time << " seconds to gather " << 4000*time  << " samples" << endl; 
		logger.log(&samples, time, true);
		clock_t end = clock();
    		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Resulting log: " << endl;
		cout << "Number of samples: " << samples.size() << " Time: " << elapsed_secs << endl;
	}

	if (writeData)
	{
		ofstream out("data.csv");
		if(!out)
    		{
     			 cout << "Couldn't open file."  << endl;
			return 1;
		}

		out << "x" << "," << "y" << "," << "z" << endl;
		for (auto& sample : samples)
		{
			out << sample.getX() << "," << sample.getY() << "," << sample.getZ() << endl;
		}
	}
}
