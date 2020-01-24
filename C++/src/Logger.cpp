#include "Logger.hpp"

Logger::Logger(ADXL357 *adxl357)
{
	m_adxl357 = adxl357;
	m_logged = false;
	m_nfifoOverranged = 0;
}

Logger::~Logger()

{
	//if(m_adxl357 != nullptr)
	//delete m_adxl357;
}

void Logger::log(vector<Sample> &samples, double time, bool convert, bool appendSamples)
{
	if (m_adxl357 == nullptr)
		return;

	if (!samples.empty() && !appendSamples)
	{
		samples.clear();
		m_adxl357->emptyFifo();
	}

	if (samples.empty())
		m_adxl357->emptyFifo();

	m_adxl357->stop();

	size_t nSamples = time * m_adxl357->getRate();
	size_t retrievedSamples = 0;
	//double period = 1 / m_adxl357->getRate();

	m_adxl357->start();
	while (retrievedSamples < nSamples)
	{
		if (m_adxl357->fifoOverRange())
		{
			cout << "\nThe FIFO overrange bit was set. Data could be inaccurate" << endl;
			m_nfifoOverranged++;
		}

		if (m_adxl357->hasNewData())
		{
			vector<Sample> temp;
			m_adxl357->getFifo(&temp);
			retrievedSamples += temp.size();
			samples.insert(samples.end(), temp.begin(), temp.end());
		}
	}
	m_adxl357->stop();

	//m_logged = true;
	if (convert)
	{
		for (auto &sample : samples)
		{
			sample.convertSample(m_adxl357->getSensitivityFactor());
		}
	}
}

void Logger::logContinuous(vector<Sample> &samples, double rate, double time, bool convert)
{
	if (m_adxl357 == nullptr)
		return;

	if (samples.empty())
	{
		//read status reg to clear it from previous statuses
		m_adxl357->fifoFull();
		m_adxl357->stop();
		m_adxl357->emptyFifo();
		m_adxl357->start();
	}

	size_t nSamples = time * rate;
	size_t retrievedSamples = 0;
	//double period = 1 / m_adxl357->getRate();

	while (retrievedSamples < nSamples)
	{
		if (m_adxl357->fifoOverRange())
		{
			cout << "\nThe FIFO overrange bit was set. Data could be inaccurate" << endl;
			m_nfifoOverranged++;
		}

		if (m_adxl357->hasNewData())
		{
			vector<Sample> temp;
			m_adxl357->getFifo(&temp);
			retrievedSamples += temp.size();
			samples.insert(samples.end(), temp.begin(), temp.end());
		}
	}

	//m_logged = true;
	if (convert)
	{
		for (auto &sample : samples)
		{
			sample.convertSample(m_adxl357->getSensitivityFactor());
		}
	}
}

//Returns if fifo overrange is triggerd during measurments, resets after read
int Logger::numFifoOveranged()
{
	int ret = m_nfifoOverranged;
	m_nfifoOverranged = 0;
	return ret;
}

bool Logger::logged()
{
	return m_logged;
}

void Logger::setLogged(bool logged)
{
	m_logged = logged;
}
