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
	//adxl357.start();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(0b000, SET_ODR_4000);

	adxl357.dumpInfo();

	Logger logger(&adxl357);

	samples = logger.log();

	cout << samples.size();
	//for(auto& sample : samples)
	//{
	//	cout << "X: " << sample.getX() << "Y: " << sample.getY() << "Z: " << sample.getZ() << endl;
	//}

}
