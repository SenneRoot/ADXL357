#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "Sample.hpp"
#include "ADXL357.hpp"
#include "tcpSocket.hpp"
#include "udpSocket.hpp"

class Logger
{
	public:
	Logger(ADXL357 *adxl357);
	~Logger();

	void log(vector<Sample> &samples, double m_time = 1, bool convert = true, bool appendSamples = false);
	void logContinuous(vector<Sample> &samples, double rate, double m_time = 0.5, bool convert = true);

	void startADXL();
	void stopADXL();

	private:
	ADXL357 *m_adxl357;
};



#endif
