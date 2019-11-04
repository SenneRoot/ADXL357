#include "sample.hpp"

Sample::Sample()
{
	m_rawX = 0;
	m_rawY = 0;
	m_rawZ = 0;

	m_converted = false;
}

Sample::~Sample()
{

}

void Sample::setRawX(uint32_t val)
{
	m_rawX = val;
}

void Sample::setRawY(uint32_t val)
{
	m_rawY = val;
}

void Sample::setRawZ(uint32_t val)
{
	m_rawZ = val;
}

uint32_t Sample::getRawX()
{
	return m_rawX;
}

uint32_t Sample::getRawY()
{
	return m_rawY;
}

uint32_t Sample::getRawZ()
{
	return m_rawZ;
}

double Sample::getX()
{
	if(m_converted)
		return m_X;
	else
	{
		return 0;
	}
}

double Sample::getY()
{
	if(m_converted)
		return m_X;
	else
	{
		return 0;
	}
}

double Sample::getZ()
{
	if(m_converted)
		return m_X;
	else
	{
		return 0;
	}
}


void Sample::getRawSampleData(uint32_t *x, uint32_t *y, uint32_t *z)
{
	*x = m_rawX;
	*y = m_rawY;
	*z = m_rawZ;
}

bool Sample::getSampleData(double *x, double *y, double *z)
{
	if(!m_converted)
		return false;

	*x = m_X;
	*y = m_Y;
	*z = m_Z;

	return true;
}

void Sample::convertSample(double factor)
{
	m_X = convertTwoComp(m_rawX) * factor;
	m_Y = convertTwoComp(m_rawY) * factor;
	m_Z = convertTwoComp(m_rawZ) * factor;

	m_converted = true;
}
