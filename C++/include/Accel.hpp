#include "PiSPI.hpp"


class Accel
{
public:
	Accel(uint8_t channel, int speed, int mode, uint8_t bitsperword);
	~Accel();


	bool read(uint8_t reg, uint8_t *buf, size_t length = 1);
	bool write(uint8_t reg, uint8_t val);

private:
	PiSPI *m_piSPI = nullptr;
};
