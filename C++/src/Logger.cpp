#include "Logger.hpp"

Logger::Logger(ADXL357 *adxl357)
{
	m_adxl357 = adxl357;
}

Logger::~Logger()

{
	//if(m_adxl357 != nullptr)
		//delete m_adxl357;
}

void Logger::log(vector<Sample> &samples, int m_time, bool convert)
{
	if(m_adxl357 == nullptr)
		return;

	if(!samples.empty())
		{
			samples.clear();
		}

	m_adxl357->stop();
	m_adxl357->emptyFifo();

	size_t nSamples = m_time * m_adxl357->getRate();
	//double period = 1 / m_adxl357->getRate();

	m_adxl357->start();
	while(samples.size() < nSamples)
	{
		if (m_adxl357->fifoOverRange())
		{
			cout << "The FIFO overrange bit was set. That means some data was lost." << endl;
		}

		if(m_adxl357->hasNewData())
		{
			vector<Sample> temp;
			m_adxl357->getFifo(&temp);
			samples.insert(samples.end(), temp.begin(), temp.end());
		}
	}
	m_adxl357->stop();

	if(convert)
	{
		for(auto &sample : samples)
		{
			sample.convertSample(m_adxl357->getSensitivityFactor());
		}
	}
}

void Logger::logContinuousTCP(tcpSocket *tcpsocket)
{
	if(m_adxl357 == nullptr)
		return;

	m_adxl357->stop();
	m_adxl357->emptyFifo();

	m_adxl357->start();

	while(true)
	{
		if (m_adxl357->fifoOverRange())
		{
			cout << "The FIFO overrange bit was set. That means some data was lost." << endl;
		}

		Sample sample;
		m_adxl357->getFifoSample(&sample);
		sample.convertSample(m_adxl357->getSensitivityFactor());
		double x = sample.getX();
		tcpsocket->send(&x);
	}
}
