#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/stat.h>

int main(int argc, char *argv[]) {
    const char *fifo_name = "fifo";
    mknod(fifo_name, S_IFIFO | 0666, 0);
    std::ifstream f(fifo_name);
    std::string line;
    getline(f, line);
    auto data_size = std::stoi(line);
    std::cout << "Size: " << data_size << std::endl;
    std::string data;
    {
        std::vector<char> buf(data_size);
        f.read(buf.data(), data_size);
        // write to vector data is valid since C++11
        data.assign(buf.data(), buf.size());
    }
    if (!f.good()) {
        std::cerr << "Read failed" << std::endl;
    }
    std::cout << "Data size: " << data.size() << " content: " << data << std::endl;
}


/*#include <iostream>
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
	string dataPath = "../../data/data_motor.csv";

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();

	adxl357.start();

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
		ofstream out(dataPath);
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
}*/
