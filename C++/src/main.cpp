#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mqtt/async_client.h>
#include <mqtt/message.h>
#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"
#include <wiringPi.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

const std::string DFLT_ADDRESS { "tcp://localhost:1883" };

const string TOPIC { "testTopic" };
const int	 QOS = 0;

const auto PERIOD = seconds(5);

const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering

const string PERSIST_DIR { "data-persist" };

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	vector<vector<Sample>> samples;
	ADXL357 adxl357;
	bool logged = false;
	double time = 0.005;

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();

	string address = (argc > 1) ? string(argv[1]) : DFLT_ADDRESS;

	mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, PERSIST_DIR);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);

	// Create a topic object. This is a conventience since we will
	// repeatedly publish messages with the same parameters.
	mqtt::topic top(cli, TOPIC, QOS, true);

	// Random number generator [0 - 100]
	random_device rnd;
  mt19937 gen(rnd());
  uniform_int_distribution<> dis(0, 100);

	try {
		// Connect to the MQTT broker
		cout << "Connecting to server '" << address << "'..." << flush;
		cli.connect(connOpts)->wait();
		cout << "OK\n" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		// The time at which to reads the next sample, starting now
		auto tm = steady_clock::now();

		while (true) {
			// Pace the samples to the desired rate
			this_thread::sleep_until(tm);

			// Get a timestamp and format as a string
			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));

			// Simulate reading some data
			adxl357.start();
			double payload[2];
			payload[0] = 15;
			payload[1] = 17;
			adxl357.stop();

			mqtt::message m(TOPIC, &payload, 2, 0, true);

			//samp.convertSample(adxl357.getSensitivityFactor());
			//double payload[3] = {samp.getX(), samp.getY(), samp.getZ()};
			//cout << payload[0] << " " << payload[1] << " " << payload[2] << endl;
			cout << m.to_string() << endl;

			// Publish to the topic
			top.publish(std::move(m.to_string()));

			tm += PERIOD;
		}

		// Disconnect
		cout << "\nDisconnecting..." << flush;
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}





//#include <iostream>
/*#include "ADXL357.hpp"
#include "Logger.hpp"
#include "Sample.hpp"
#include "stdio.h"
//#include <mqtt/client.h>
#include <wiringPi.h>
#include <unistd.h>

#define btn_pin 8

void setupGPIO()
{
		wiringPiSetup();

		pinMode(btn_pin, INPUT);
		pullUpDnControl(btn_pin, PUD_UP);
}

bool read_btn(int btnPin)
{
	//debounce pin
	if(!digitalRead(btnPin) || digitalRead(btnPin))
	{
		usleep(1);
		return digitalRead(btnPin);
	}
	//should never reach this part
	return true;
}


int main()
{
	setupGPIO();
	vector<vector<Sample>> samples;
	ADXL357 adxl357;
	bool logged = false;
	double time = 0.005;

	//setup ADXL357 sensor
	adxl357.stop();
	adxl357.setRange(SET_RANGE_10G);
	adxl357.setFilter(SET_HPF_OFF, SET_ODR_4000);
	adxl357.dumpInfo();

	while (1)
	{
		Logger logger(&adxl357);

		if(!read_btn(btn_pin))
		{
			//samples.clear();
			vector<Sample> temp;
			while(!digitalRead(btn_pin))
			{
				logger.log(temp, time, true, true);
				printf("\rLogging ---> %6d", temp.size());
        fflush(stdout);
			}
			samples.push_back(temp);
			logged = true;
		}

		if(logged)
		{
			printf("\nDone! logged %2d times\n", samples.size());
			logged = false;
		}
	}
}
*/
