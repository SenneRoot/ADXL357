#include "ADXL357.hpp"
#include "PiSPI.hpp"


ADXL357::ADXL357(uint8_t channel, int speed, int mode, uint8_t bitsperword)
{
	piSPI = new PiSPI(channel, speed, mode, bitsperword);
}

ADXL357::~ADXL357()
{
	//delete piSPI;
}

bool ADXL357::read(uint8_t reg, uint8_t *buf, size_t length)
{
	if (piSPI == nullptr)
		return false;

	uint8_t address = (reg << 1) | 0b1;
	return piSPI->Read(address, buf, length);
}

bool ADXL357::write(uint8_t reg, uint8_t val)
{
	if (piSPI == nullptr)
		return false;

	uint8_t address = (reg << 1) & 0b11111110;
	return piSPI->Write(address, &val, 1);
}

bool ADXL357::fifoFull()
{
	uint8_t buf[1];
	if(!read(REG_STATUS, buf, 1))
	{
		cout << "Reading FIFO Full bit Failed!" << endl;
		return false;
	}

	return buf[0] & 0b10;
}

bool ADXL357::fifoOverRange()
{
	uint8_t buf[1];
	if(!read(REG_STATUS, buf, 1))
	{
		cout << "Reading FIFO Overrange bit Failed!" << endl;
		return false;
	}


	return buf[0] & 0b100;
}

void ADXL357::start()
{
	uint8_t buf[1];
	if(!read(REG_POWER_CTL, buf, 1))
		cout << "Reading Power CTL Failed while starting!" << endl;
	if(!write(REG_POWER_CTL, buf[0] & START))
		cout << "Writing Power CTL Failed while starting!" << endl;
}

void ADXL357::stop()
{
	uint8_t buf[1];
	if(!read(REG_POWER_CTL, buf, 1))
		cout << "Reading Power CTL Failed while stopping!" << endl;
	if(!write(REG_POWER_CTL,  buf[0] | STOP))
		cout << "Writing Power CTL Failed while stopping!" << endl;
}

void ADXL357::dumpInfo()
{
	printf("ADXL355 SPI Info Dump\n");
  printf("========================================\n");
	uint8_t buf[64];
	//uint8_t adid, memsid, devid;
	if(read(REG_DEVID_AD, buf, 3))
	{
		printf("Analog Devices ID: %#02x\n", buf[0]);
		printf("Analog Devices MEMS ID: %#02x\n", buf[1]);
		printf("Device ID: %#02x\n", buf[2]);
	}
	else
		cout << "Reading ID Registers Failed!" << endl;

	if(read(REG_POWER_CTL, buf))
		printf("Power Control Status: %d %s\n", buf[0], buf[0] & STOP ? "--> Standby" : "--> Measurement Mode");
	else
		cout << "Reading Power CTL Failed!" << endl;

}

uint8_t ADXL357::whoAmI()
{
	uint8_t buf[1];
	if(!read(REG_PARTID, buf, 1))
		cout << "Reading Register Part ID Failed!" << endl;

	return buf[0];
}

void ADXL357::setRange(uint8_t range)
{
	//stop();
	bool res = false;

	switch (range)
	{
	case SET_RANGE_10G:
		res = write(REG_RANGE, SET_RANGE_10G);
		m_factor = 1 / 51200;
		break;
	case SET_RANGE_20G:
		res = write(REG_RANGE, SET_RANGE_20G);
		m_factor = 1 / 25600;
		break;
	case SET_RANGE_40G:
		res = write(REG_RANGE, SET_RANGE_40G);
		m_factor = 1 / 12800;
		break;
	default:
		break;
	}
	if(!res)
	{
		cout << "Writing Range register Failed!" << endl;
	}
	//start();
}

void ADXL357::setFilter(uint8_t hpf, uint8_t lpf)
{
	//stop();
	if(!write(REG_FILTER, (hpf << 4) | lpf))
		cout << "Writing Filter register Failed!" << endl;
	//start();
}

double ADXL357::getRate()
{
	stop();
	uint8_t buf[1];

	if(!read(REG_FILTER, buf, 1))
	{
		cout << "Reading Filter Register Failed!" << endl;
	}
	else
	{
		switch ((buf[0] & 0xF))
		{
		case SET_ODR_4000:
			return 4000;
			break;
		case SET_ODR_2000:
			return 2000;
			break;
		case SET_ODR_1000:
			return 1000;
			break;
		case SET_ODR_500:
			return 500;
			break;
		case SET_ODR_250:
			return 250;
			break;
		case SET_ODR_125:
			return 125;
			break;
		case SET_ODR_62_5:
			return 62.5;
			break;
		case SET_ODR_31_25:
			return 31.25;
			break;
		case SET_ODR_15_625:
			return 15.625;
			break;
		case SET_ODR_7_813:
			return 7.813;
			break;
		case SET_ODR_3_906:
			return 3.906;
			break;
		default:
			break;
		}
	}

	return -1;
}

double ADXL357::getSensitivityFactor()
{
	return m_factor;
}

int16_t ADXL357::tempRaw()
{
	uint8_t buf[2];
	if(!read(REG_TEMP2, buf, 2))
		cout << "Reading Temperature Failed!" << endl;

	return ((((int16_t)buf[1]) << 8) | (int16_t)buf[0]);
}

int32_t ADXL357::getXraw()
{
	uint8_t buf[3];
	if(!read(REG_XDATA3, buf, 3))
		cout << "Reading X Failed!" << endl;

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getX()
{
	uint8_t buf[3];
	if(!read(REG_XDATA3, buf, 3))
		cout << "Reading X Failed!" << endl;

	return Sample::convertTwoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

int32_t ADXL357::getYraw()
{
	uint8_t buf[3];
	if(!read(REG_YDATA3, buf, 3))
		cout << "Reading Y Failed!" << endl;

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getY()
{
	uint8_t buf[3];
	if(!read(REG_YDATA3, buf, 3))
		cout << "Reading Y Failed!" << endl;

	return Sample::convertTwoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

int32_t ADXL357::getZraw()
{
	uint8_t buf[3];
	if(!read(REG_ZDATA3, buf, 3))
		cout << "Reading Z Failed!" << endl;

	return (((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]);
}

int32_t ADXL357::getZ()
{
	uint8_t buf[3];
	if(!read(REG_ZDATA3, buf, 3))
		cout << "Reading Z Failed!" << endl;

	return Sample::convertTwoComp((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
}

Sample ADXL357::getXYZ()
{
	uint8_t buf[9];
	Sample sample;
	if(!read(REG_XDATA3, buf, 9))
		cout << "Reading Acceleration Failed!" << endl;

	sample.setRawX((((int32_t)buf[0]) << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2]));
  sample.setRawY((((int32_t)buf[3]) << 16) | ((int32_t)buf[4] << 8) | ((int32_t)buf[5]));
	sample.setRawZ((((int32_t)buf[6]) << 16) | ((int32_t)buf[7] << 8) | ((int32_t)buf[8]));

	return sample;
}

void ADXL357::getFifo(vector<Sample> *samples)
{
	Sample sample;
	uint8_t bufx[3], bufy[3], bufz[3];

	if(!read(REG_FIFO_DATA, bufx, 3))
	{
		cout << "Reading FIFO Failed!" << endl;
		return samples;
	}

	while((bufx[2] & 0b10) == 0)
	{
		read(REG_FIFO_DATA, bufy, 3);
		read(REG_FIFO_DATA, bufz, 3);

		sample.setRawX((((uint32_t)bufx[0]) << 16) | ((uint32_t)bufx[1] << 8) | ((uint32_t)bufx[2]));
		sample.setRawY((((uint32_t)bufy[0]) << 16) | ((uint32_t)bufy[1] << 8) | ((uint32_t)bufy[2]));
		sample.setRawZ((((uint32_t)bufz[0]) << 16) | ((uint32_t)bufz[1] << 8) | ((uint32_t)bufz[2]));

		samples.push_back(sample);

		read(REG_FIFO_DATA, bufx, 3);
	}
}

void ADXL357::emptyFifo()
{
	uint8_t buf[3];
	if(!read(REG_FIFO_DATA, buf, 3))
		cout << "Emptying FIFO Failed!" << endl;
	while ((buf[2] & 0b10) == 0)
	{
		read(REG_FIFO_DATA, buf, 3);
	}
}

bool ADXL357::hasNewData()
{
	uint8_t buf[1];
	if(!read(REG_STATUS, buf, 1))
		cout << "Reading Has New Data bit Failed!" << endl;

	return (buf[0] & 0b00000001);
}

void ADXL357::getSamplesFast(vector<Sample> *samples, size_t nSamples)
{
	while(samples.size() < nSamples)
	{
		vector<Sample> temp = getFifo();
		samples.insert(samples.end(), temp.begin(), temp.end());
	}
}
