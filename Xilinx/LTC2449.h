/*
 * LTC2449.h
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_LTC2449_H_
#define INC_LTC2449_H_

#define BUFFER_SIZE 4


#define LTC2449_RESOLUTION 0.000000244

enum{
	HalfThanNegVRef,
	GTHalfNegVRefLTZero,
	GTZeroLTHalfVRef,
	GTHalfVRef
};


/*
 * Description: This is the data result from the LTC2449
 *
 */
typedef union{
	struct{
		uint32_t data;
	}DataStream;
	struct{
		uint8_t data[4];
	}Bytes;
	struct{
		unsigned :5;
		unsigned :23;
		unsigned range:2;
		unsigned :2;
	}InputRange;
	struct{
		unsigned sub_LSB :5; // sub LSB beyond the 24 bit level result
		unsigned result :24; // the result
		unsigned SIG : 1; // (third output bit) is the conversion result sign indicator (SIG). If VIN is >0, this bit is HIGH. If VIN is <0, this bit is LOW.
		unsigned DMY :1; // dummy
		unsigned EOC :1; // End of Conversion. This bit is HIGH during the conversion and goes LOW when the conversion is complete.
	}Status;
}LTC2449_SDO_Data;

/*
 * Description: Sets the Channel and speed
 *
 */
typedef union{
	struct {
		uint32_t data;
	}DataStream;
	struct{
		uint8_t byte1;
		uint8_t byte0;
		uint8_t byte3;
		uint8_t byte2;
	}Bytes;
	/*
	struct{
		unsigned :16;
		unsigned :3;
		unsigned TWOX:1; // speed multiplier
		unsigned OSR:4; // speed
		unsigned A0:1;
		unsigned A1:1;
		unsigned A2:1;
		unsigned ODD:1;
		unsigned SGL:1; // single ended. Set to 1 on init,
		unsigned Conversion:3; // this should either be b101, b100, b000
	}ChannelSpdRes;
	*/
	struct{
		unsigned :3;
		unsigned TWOX:1; // speed multiplier
		unsigned OSR:4; // speed
		unsigned A0:1;
		unsigned A1:1;
		unsigned A2:1;
		unsigned ODD:1;
		unsigned SGL:1; // single ended. Set to 1 on init,
		unsigned Conversion:3; // this should either be b101, b100, b000
		unsigned :16;
	}ChannelSpdRes;

	/*
	struct{
		unsigned :16;
		unsigned :3;
		unsigned TWOX:1;
		unsigned OSR0:1;
		unsigned OSR1:1;
		unsigned OSR2:1;
		unsigned OSR3:1;
		unsigned A0:1;
		unsigned A1:1;
		unsigned A2:1;
		unsigned ODD:1;
		unsigned SGL:1; // single ended. Set to 1 on init,
		unsigned EN:1;
		unsigned :1;
		unsigned :1;
	}Status;
	*/
}LTC2449_SDI_Data;


bool LTC2449_GetBusyState(void);
int LTC2449_SetChannel(uint8_t channel);
int LTC2449_GetChannelValue(uint8_t channel, LTC2449_SDO_Data *data_OUT) ;

#endif /* INC_LTC2449_H_ */
