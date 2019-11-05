#ifndef _SAMPLE_HPP_
#define _SAMPLE_HPP_

#include "Sample.hpp"
#include "ADXL357.hpp"

class Logger
{
	public:
	Logger(ADXL357 *adxl357);
	~Logger();

	vector<Sample> log(int m_time = 1, bool convert = true);



	private:
	ADXL357 *m_adxl357;




};



#endif
