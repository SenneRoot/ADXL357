#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"

#include <iostream>
#include <fstream>

//#include <fftw3.h>



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

	logger.log(&samples, 8, true);

	clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << "Number of samples: " << samples.size() << "Time: " << elapsed_secs << endl;


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

	/*const int N = 16384;
	fftw_complex in[N], out[N];
	fftw_plan p, q;

	for (int i = 0; i < N; i++)
	{
		in[i][0] = samples.at(i).getX();
		in[i][1] = 0;
	}

	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(p);
  	for (int i = 0; i < N; i++)
    	printf("freq: %3d %+9.5f %+9.5f I\n", i, out[i][0], out[i][1]);

	fftw_destroy_plan(p);*/
}
