#ifndef _ADXL357_HPP_
#define _ADXL357_HPP_

#include <vector>
#include "Sample.hpp"
#include "Accel.hpp"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <string>
#include <iostream>

#define START 							0b0
#define STOP 								0b1

#define REG_DEVID_AD 				0x00
#define REG_DEVID_MST 			0x01
#define REG_PARTID 					0x02
#define REG_REVID 					0x03
#define REG_STATUS 					0x04
#define REG_FIFO_ENTRIES 		0x05
#define REG_TEMP2 					0x06
#define REG_TEMP1 					0x07
#define REG_XDATA3 					0x08
#define REG_XDATA2 					0x09
#define REG_XDATA1 					0x0A
#define REG_YDATA3 					0x0B
#define REG_YDATA2 					0x0C
#define REG_YDATA1 					0x0D
#define REG_ZDATA3 					0x0E
#define REG_ZDATA2 					0x0F
#define REG_ZDATA1 					0x10
#define REG_FIFO_DATA 			0x11
#define REG_OFFSET_X_H 			0x1E
#define REG_OFFSET_X_L 			0x1F
#define REG_OFFSET_Y_H 			0x20
#define REG_OFFSET_Y_L 			0x21
#define REG_OFFSET_Z_H 			0x22
#define REG_OFFSET_Z_L 			0x23
#define REG_ACT_EN 					0x24
#define REG_ACT_THRESH_H 		0x25
#define REG_ACT_THRESH_L 		0x26
#define REG_ACT_COUNT 			0x27
#define REG_FILTER 					0x28
#define REG_FIFO_SAMPLES 		0x29
#define REG_INT_MAP				 	0x2A
#define REG_SYNC 						0x2B
#define REG_RANGE 					0x2C
#define REG_POWER_CTL 			0x2D
#define REG_SELF_TEST 			0x2E
#define REG_RESET 					0x2F

//Settings
#define SET_RANGE_10G 			0b01
#define SET_RANGE_20G 			0b10
#define SET_RANGE_40G 			0b11

#define SET_HPF_OFF 				0b000
#define SET_HPF_247 				0b001
#define SET_HPF_62_084 			0b010
#define SET_HPF_15_545 			0b011
#define SET_HPF_3_862 			0b100
#define SET_HPF_0_954 			0b101
#define SET_HPF_0_238 			0b110

#define SET_ODR_4000 				0b0000
#define SET_ODR_2000 				0b0001
#define SET_ODR_1000 				0b0010
#define SET_ODR_500 				0b0011
#define SET_ODR_250 				0b0100
#define SET_ODR_125 				0b0101
#define SET_ODR_62_5 				0b0110
#define SET_ODR_31_25 			0b0111
#define SET_ODR_15_625 			0b1000
#define SET_ODR_7_813 			0b1001
#define SET_ODR_3_906 			0b1010

class ADXL357 : public Accel
{
public:
	ADXL357(uint8_t channel = 0, int speed = 1000000, int mode = 0, uint8_t bitsperword = 8);
	~ADXL357();

	bool fifoFull();
	bool fifoOverRange();

	void start();
	void stop();

	void dumpInfo();
	uint8_t whoAmI();

	void setRange(uint8_t range);
	void setFilter(uint8_t hpf = 0b000, uint8_t lpf = 0b0000);
	double getRate();
	double getSensitivityFactor();
	int get_range();

	int16_t tempRaw();

	int32_t getXraw();
	int32_t getX();

	int32_t getYraw();
	int32_t getY();

	int32_t getZraw();
	int32_t getZ();

	Sample getXYZ();

	void getFifo(vector<Sample> *samples);
	void getFifoSample(Sample *sample);
	void emptyFifo();
	bool hasNewData();

	void getSamplesFast(vector<Sample> *samples, size_t nSampels = 1000); //Get specified numbers of samples from FIFO.
	void getsamplesRaw(vector<Sample> *samples, int nSampels = 1000);
	void getsamples(vector<Sample> *samples, int nSampels = 1000);

private:
	double m_factor = 1;
};

#endif
