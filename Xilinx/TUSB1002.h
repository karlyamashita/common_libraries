/*
 * TUSB1002.h
 *
 *  Created on: Aug 19, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_TUSB1002_H_
#define SRC_TUSB1002_H_



enum{
	TUSB1002_EQ = 1,
	TUSB1002_VOD
}EQ_VOD;


enum{
	CFG1_S1,
	CFG1_S2,
	CFG2_S1,
	CFG2_S2,
	RX_EQ1_S1,
	RX_EQ1_S2,
	RX_EQ2_S1,
	RX_EQ2_S2,
	TX_EQ1_S1,
	TX_EQ1_S2,
	TX_EQ2_S1,
	TX_EQ2_S2,

	CFG1_S1_REDRV2, // 12
	CFG1_S2_REDRV2, // 13
	CFG2_S1_REDRV2, // 14
	CFG2_S2_REDRV2, // 15
	RX_EQ1_S1_REDRV2,
	RX_EQ1_S2_REDRV2,
	RX_EQ2_S1_REDRV2,
	RX_EQ2_S2_REDRV2,
	TX_EQ1_S1_REDRV2,
	TX_EQ1_S2_REDRV2,
	TX_EQ2_S1_REDRV2,
	TX_EQ2_S2_REDRV2,

	REDRIVER1_EN,
	REDRIVER2_EN
};

void TUSB1002_Init(void);

void TUSB1002_SetRedriver1_Rx_EQ(uint8_t idxPtr);
void TUSB1002_SetRedriver1_Tx_EQ(uint8_t idxPtr);
void TUSB1002_SetRedriver1_Cfg_VOD(uint8_t idxPtr);

void TUSB1002_SetRedriver2_Rx_EQ(uint8_t idxPtr);
void TUSB1002_SetRedriver2_Tx_EQ(uint8_t idxPtr);
void TUSB1002_SetRedriver2_Cfg_VOD(uint8_t idxPtr);

void TUSB1002_PulseRedriver1_En(void);
void TUSB1002_PulseRedriver2_En(void);

void TUSB1002_SetRedriver1_En(void);
void TUSB1002_SetRedriver2_En(void);

void TUSB1002_ClearRedriver1_En(void);
void TUSB1002_ClearRedriver2_En(void);

void TUSB1002_Pulse1En(bool status);
void TUSB1002_Pulse2En(bool status);

void TUSB1002_SetRedriver1_En(void);
void TUSB1002_SetRedriver2_En(void);



#endif /* SRC_TUSB1002_H_ */
