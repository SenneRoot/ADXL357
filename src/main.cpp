#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"

int main()
{
	vector<Sample> samples;
	ADXL357 adxl357;
	//auto rate = SET_ODR_4000;
	int mtime = 1;
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(0b000, SET_ODR_4000);

	adxl357.dumpInfo();

	Logger logger(&adxl357);
	logger.log(&samples, 2, true);

	//for(auto& sample : samples)
	//{
	//	cout << "X: " << sample.getX() << " Y: " << sample.getY() << " Z: " << sample.getZ() << endl;
	//}

	cout << "Number of samples: " << samples.size() << endl;
}
