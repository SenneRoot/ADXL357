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

	void setRawX(uint32_t val);
	void setRawY(uint32_t val);
	void setRawZ(uint32_t val);

	uint32_t getRawX();
	uint32_t getRawY();
	uint32_t getRawZ();

	double getX();
	double getY();
	double getZ();

	void getRawSampleData(uint32_t *x, uint32_t *y, uint32_t *z);
	bool getSampleData(double *x, double *y, double *z);

	void convertSample(double factor);
	//implemented in this way to use it as a static function
	static int32_t convertTwoComp(uint32_t source)
	{
		int target;
		source = (source >> 4);
		source = (source & 0x000FFFFF);

		if ((source & 0x00080000) == 0x00080000)
			target = (source | 0xFFF00000);
		else
			target = source;

		return target;
	};

private:
	uint32_t m_rawX;
	uint32_t m_rawY;
	uint32_t m_rawZ;

	double m_X;
	double m_Y;
	double m_Z;

	bool m_converted;
};

#endif
