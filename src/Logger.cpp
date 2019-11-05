#include "Logger.hpp"

Logger::Logger(ADXL357 *adxl357)
{
	m_adxl357 = adxl357;
}

Logger::~Logger()

{
	delete ADXL357;
}


vector<Sample> Logger::log(int m_time, bool convert)
{
	vector<Sample> samples;

	if(ADXL357 == nullptr)
		return samples;

	m_adxl357.stop();
	m_adxl357.emptyFifo();

	int nSamples = m_time * ADXL357.getRate();
	double period = 1 / ADXL357.getRate();

	m_adxl357.start();
	while(samples.size() < nSamples)
	{
		if (ADXL357.fifoOverRange())
		{
			cout << "The FIFO overrange bit was set. That means some data was lost." << endl;
		}

		if(adxl357.hasNewData())
		{
			vector<Sample> temp = adxl357.getFifo();
			samples.insert(samples.end(), temp.begin(), temp.end());
		}
	}
	m_adxl357.stop();

	if(convert)
	{
		for(auto &sample : samples)
		{
			samples.convertSample();
		}
	}

	return samples;
}
