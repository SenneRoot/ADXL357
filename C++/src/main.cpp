#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
	cout << "Sample TCP echo client for 'sockpp' "
		<< sockpp::SOCKPP_VERSION << '\n' << endl;

	string host = (argc > 1) ? argv[1] : "localhost";
	in_port_t port = (argc > 2) ? atoi(argv[2]) : 12345;

	sockpp::socket_initializer sockInit;

	// Implicitly creates an inet_address from {host,port}
	// and then tries the connection.

	sockpp::tcp_connector conn({host, port});
	if (!conn) {
		cerr << "Error connecting to server at "
			<< sockpp::inet_address(host, port)
			<< "\n\t" << conn.last_error_str() << endl;
		return 1;
	}

	cout << "Created a connection from " << conn.address() << endl;

    // Set a timeout for the responses
    if (!conn.read_timeout(seconds(5))) {
        cerr << "Error setting timeout on TCP stream: "
                << conn.last_error_str() << endl;
    }

	string s, sret;
	while (getline(cin, s) && !s.empty()) {
		if (conn.write(s) != ssize_t(s.length())) {
			cerr << "Error writing to the TCP stream: "
				<< conn.last_error_str() << endl;
			break;
		}

		sret.resize(s.length());
		ssize_t n = conn.read_n(&sret[0], s.length());

		if (n != ssize_t(s.length())) {
			cerr << "Error reading from TCP stream: "
				<< conn.last_error_str() << endl;
			break;
		}

		cout << sret << endl;
	}

	return (!conn) ? 1 : 0;
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
