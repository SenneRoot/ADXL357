#include "sample.hpp"

Sample::Sample()
{
	m_rawX = 0;
	m_rawY = 0;
	m_rawZ = 0;
}

void Sample::setRawX(int32_t val)
{
	m_rawX = val;
}

void Sample::setRawY(int32_t val)
{
	m_rawY = val;
}

void Sample::setRawZ(int32_t val)
{
	m_rawZ = val;
}

void Sample::getRawSampleData(uint32_t *x, uint32_t *y, uint32_t *z)
{
	*x = m_rawX;
	*y = m_rawY;
	*z = m_rawZ;
}

void Sample::getSampleData(double *x, double *y, double *z)
{
	*x = m_X;
	*y = m_Y;
	*z = m_Z;
}

void Sample::convertSample(double factor)
{

	m_X = convertTwoComp(m_rawX) * factor;
	m_Y = convertTwoComp(m_rawY) * factor;
	m_Z = convertTwoComp(m_rawZ) * factor;
}

int32_t Sample::convertTwoComp(uint32_t source)
{
		int target;
	source = (source >> 4);
  source = (source & 0x000FFFFF);

  if((source & 0x00080000)  == 0x00080000)
    target = (source | 0xFFF00000);
  else
    target = source;

  return target;
}
