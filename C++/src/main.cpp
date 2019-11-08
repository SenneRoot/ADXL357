#include "udpSocket.hpp"
#include <iostream>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

int main()
{
	udpSocket udpsocket(8080);

	vector<Sample> samples;
	ADXL357 adxl357;
	bool writeData = false;
	bool log = false;
	int time = 4;
	string dataPath = "../../data/data_motor.csv";

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();

	while (1)
	{
		char buf[MAXLINE];
		struct sockaddr clientAddress;
		uint len;

		udpsocket.receive(buf, &clientAddress, &len);
		std::string message(buf);
		//printf("%s", buf);
		message.pop_back();
		cout << message << endl;

		if(!message.compare("log"))
		{
			log = true;
			udpsocket.send("logging data", &clientAddress, &len);
		}
		else
			log = false;




		if (log)
		{
			Logger logger(&adxl357);
			clock_t begin = clock();
			cout << "Starting Logging dat for " << time << " seconds to gather " << 4000 * time << " samples" << endl;
			logger.log(&samples, time, true);
			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			cout << "Resulting log: " << endl;
			cout << "Number of samples: " << samples.size() << " Time: " << elapsed_secs << endl;
		}

		if (writeData)
		{
			ofstream out(dataPath);
			if (!out)
			{
				cout << "Couldn't open file." << endl;
				return 1;
			}

			out << "x"
					<< ","
					<< "y"
					<< ","
					<< "z" << endl;

			for (auto &sample : samples)
			{
				out << sample.getX() << "," << sample.getY() << "," << sample.getZ() << endl;
			}
		}
	}
}
