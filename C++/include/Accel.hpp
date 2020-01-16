#include "PiSPI.hpp"


class Accel
{
	Accel();
	~Accel();

public:
	bool read(uint8_t reg, uint8_t *buf, size_t length = 1);
	bool write(uint8_t reg, uint8_t val);

private:
	PiSPI *m_piSPI = nullptr;
};
