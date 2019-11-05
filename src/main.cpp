#include <iostream>
#include "ADXL357.hpp"

int main()
{
	ADXL357 adxl357();
	auto rate = SET_ODR_4000;
	int mtime = 1;
	adxl357.start();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(0b000, SET_ODR_4000);

	adxl357.dumpInfo();
}
