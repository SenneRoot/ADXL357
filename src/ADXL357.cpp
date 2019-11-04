#include "ADXL357.hpp"
#include "PiSPI.hpp"


ADXL357::ADXL357()
{
	piSPI = new PiSPI(0, 1000000, 0);
}

ADXL357::~ADXL357()
{

}

bool ADXL357::read(uint8_t reg, uint8_t *buf, size_t length)
{
	if (piSPI == nullptr)
		return false;

	uint8_t address = (reg << 1) | 0b1;
	piSPI->Read(address, buf, length);
}

bool ADXL357::write(uint8_t reg, uint8_t val)
{
	if (piSPI == nullptr)
		return false;

	uint8_t address = (reg << 1) & 0b11111110;
	piSPI->Write(address, &val, 1);
}

bool ADXL357::fifoFull()
{
	uint8_t buf[1];
	read(REG_STATUS, buf, 1);

	return buf[0] & 0b10;
}

bool ADXL357::fifoOverRange()
{
	uint8_t buf[1];
	read(REG_STATUS, buf, 1);

	return buf[0] & 0b100;
}

void ADXL357::start()
{
	uint8_t buf[1];
	read(REG_POWER_CTL, buf, 1);
	write(REG_POWER_CTL, buf[0] & START);
}

void ADXL357::stop()
{
	uint8_t buf[1];
	read(REG_POWER_CTL, buf, 1);
	write(REG_POWER_CTL, buf[0] & STOP);
}

void ADXL357::dumpInfo()
{
	printf("ADXL355 SPI Info Dump\n");
  printf("========================================\n");
	uint8_t buf[64];
	//uint8_t adid, memsid, devid;
	read(REG_DEVID_AD, buf, 3);
	printf("Analog Devices ID: %d\n", buf[0]);
	printf("Analog Devices MEMS ID: %d\n", buf[1]);
	printf("Device ID: %d\n", buf[2]);

	read(REG_POWER_CTL, buf);
	printf("Power Control Status: %d %s\n", buf[0], buf[0] & STOP ? "--> Standby" : "--> Measurement Mode");
}

uint8_t ADXL357::whoAmI()
{
	uint8_t buf[1];
	read(REG_PARTID, buf, 1);

	return buf[0];
}

void ADXL357::setRange(uint8_t range)
{
	stop();
	switch (range)
	{
	case SET_RANGE_10G:
		write(REG_RANGE, SET_RANGE_10G);
		m_factor = 1 / 51200;
		break;
	case SET_RANGE_20G:
		write(REG_RANGE, SET_RANGE_20G);
		m_factor = 1 / 25600;
		break;
	case SET_RANGE_40G:
		write(REG_RANGE, SET_RANGE_40G);
		m_factor = 1 / 12800;
		break;
	default:
		break;
	}
	start();
}

void ADXL357::setFilter(uint8_t hpf, uint8_t lpf)
{
	stop();
	write(REG_FILTER, (hpf << 4) | lpf);
	start();
}

int16_t ADXL357::tempRaw()
{
	uint8_t buf[2];
	read(REG_TEMP2, buf, 2);

	return ((((int16_t)buf[1]) << 8) | (int16_t)buf[0]);
}

int32_t ADXL357::getXraw()
{
	uint8_t buf[3];
	read(REG_XDATA3, buf, 3);

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getX()
{
	uint8_t buf[3];
	read(REG_XDATA3, buf, 3);

	return twoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

int32_t ADXL357::getYraw()
{
	uint8_t buf[3];
	read(REG_YDATA3, buf, 3);

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getY()
{
	uint8_t buf[3];
	read(REG_YDATA3, buf, 3);

	return twoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

int32_t ADXL357::getZraw()
{
	uint8_t buf[3];
	read(REG_ZDATA3, buf, 3);

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getZ()
{
	uint8_t buf[3];
	read(REG_ZDATA3, buf, 3);

	return twoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

Sample ADXL357::getXYZ()
{
	uint8_t buf[9];
	Sample sample;
	read(REG_XDATA3, buf, 9);

	sample.setRawX((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
  sample.setRawY((((int32_t)buf[3]) << 16) | ((int32_t)buf[4] << 8) | ((int32_t)buf[5]));
	sample.setRawZ((((int32_t)buf[6]) << 16) | ((int32_t)buf[7] << 8) | ((int32_t)buf[8]));
}

vector<Sample> ADXL357::getFifo()
{
	vector<Sample> samples;
	Sample sample;
	uint8_t bufx[3], bufy[3], bufz[3];

	read(REG_FIFO_DATA, bufx, 3);
	while(bufx[2] & 0b10 == 0)
	{
		read(REG_FIFO_DATA, bufy, 3);
		read(REG_FIFO_DATA, bufz, 3);

		sample.setRawX((((int32_t)bufx[0]) << 16) | ((int32_t)bufx[1] << 8) | ((int32_t)bufx[2]));
		sample.setRawY((((int32_t)bufy[0]) << 16) | ((int32_t)bufy[1] << 8) | ((int32_t)bufy[2]));
		sample.setRawZ((((int32_t)bufz[0]) << 16) | ((int32_t)bufz[1] << 8) | ((int32_t)bufz[2]));

		samples.push_back(sample);

		read(REG_FIFO_DATA, bufx, 3);
	}
	
	return samples;
}

void ADXL357::emptyFifo()
{
	uint8_t buf[3];
	read(REG_FIFO_DATA, buf, 3);
	while (buf[2] & 0b10 == 0)
	{
		read(REG_FIFO_DATA, buf, 3);
	}
}

bool ADXL357::hasNewData()
{
	uint8_t buf[1];
	read(REG_STATUS, buf, 1);

	return buf[0] & 0b1;
}

vector<Sample> ADXL357::getSamplesFast(int nSamples)
{
	vector<Sample> samples;
	uint8_t buf[3];

	while(samples.size() < nSamples)
	{
		vector<Sample> temp = getFifo();
		samples.insert(samples.end(), temp.begin(), temp.end());
	}

	return samples;
}
