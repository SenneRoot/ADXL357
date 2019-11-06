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
	//auto rate = SET_ODR_4000;
	int mtime = 1;
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(0b000, SET_ODR_4000);

	adxl357.dumpInfo();

	Logger logger(&adxl357);

	clock_t begin = clock();

	logger.log(&samples, 2, true);

	clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << "Number of samples: " << samples.size() << "Time: " << elapsed_secs << endl;


	ofstream out("data.csv");
	if( !out )
  {
    cout << "Couldn't open file."  << endl;
    return 1;
  }

	for (auto& sample : samples)
	{
		out << sample.getX() << "," << sample.getY() << "," << sample.getZ() << endl;
	}

}
