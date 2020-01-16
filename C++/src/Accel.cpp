#include "Accel.hpp"

Accel::Accel(uint8_t channel, int speed, int mode, uint8_t bitsperword)
{
	m_piSPI = new PiSPI(channel, speed, mode, bitsperword);
}

Accel::~Accel()
{
	if (m_piSPI != nullptr)
		delete m_piSPI;
}

bool Accel::read(uint8_t reg, uint8_t *buf, size_t length)
{
	if (m_piSPI == nullptr)
		return false;

	uint8_t address = (reg << 1) | READ;
	return m_piSPI->Read(address, buf, length);
}

bool Accel::write(uint8_t reg, uint8_t val)
{
	if (m_piSPI == nullptr)
		return false;

	uint8_t address = ~(reg << 1) & WRITE;
	return m_piSPI->Write(address, &val, 1);
}
