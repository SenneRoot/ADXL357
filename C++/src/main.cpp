
#include <iostream>
#include <string>
#include "../headers/exception.hpp"
#include "../headers/inetclientstream.hpp"

#include <stdlib.h>
#include <unistd.h>

/*
 * This program connects to host:port (usually localhost),
 * closes the socket, reopens it with the same parameters
 * (to show what you may do with sockets... ;), receives
 * a message from the server and sends a message back.
 */

int main(void) {
    using std::string;

    using libsocket::inet_stream;

    string host = "::1";
    string port = "1235";
    string answer;

    answer.resize(32);

    try {
        libsocket::inet_stream sock(host, port, LIBSOCKET_IPv6);

        sock >> answer;

        std::cout << answer;

        sock << "Hello back!\n";

        // sock is closed here automatically!
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    return 0;
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
