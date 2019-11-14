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

	void log(vector<Sample> &samples, int m_time = 1, bool convert = true);
	void logContinuousTCP(tcpSocket *tcpsocket);
	void logContinuousUDP(udpSocket *udpsocket, const struct sockaddr *clientAddress);

	private:
	ADXL357 *m_adxl357;
};



#endif
