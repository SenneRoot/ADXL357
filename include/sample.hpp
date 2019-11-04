#ifndef _SAMPLE_HPP_
#define _SAMPLE_HPP_

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

class Sample
{
	public:
	Sample();
	~Sample();

	void setRawX(int32_t val);
	void setRawY(int32_t val);
	void setRawZ(int32_t val);

	void getRawSampleData(uint32_t *x, uint32_t *y, uint32_t *z);
	void getSampleData(double *x, double *y, double *z);

	void convertSample(double factor);


	private:
	int32_t convertTwoComp(uint32_t source);

	int32_t m_rawX;
	int32_t m_rawY;
	int32_t m_rawZ;

	double m_X;
	double m_Y;
	double m_Z;



};


#endif
