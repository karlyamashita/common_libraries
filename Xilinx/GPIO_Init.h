/*
 * GPIO_Init.h
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_GPIO_INIT_H_
#define SRC_GPIO_INIT_H_

#define CHANNEL_1 1
#define CHANNEL_2 2

// pins config for RDRVR
enum {
	Pin_C6_CFG1_S1,
	Pin_B6_CFG1_S2,
	Pin_C3_CFG2_S1,
	Pin_A4_CFG2_S2,
	Pin_C5_RX_EQ1_S1,
	Pin_A5_RX_EQ1_S2,
	Pin_B2_RX_EQ2_S1,
	Pin_B1_RX_EQ2_S2,
	Pin_F11_TX_EQ1_S1,
	Pin_D12_TX_EQ1_S2,
	Pin_F13_TX_EQ2_S1,
	Pin_E12_TX_EQ2_S2,

	Pin_D2_CFG1_S1,
	Pin_E1_CFG1_S2,
	Pin_C1_CFG2_S1,
	Pin_D4_CFG2_S2,
	Pin_H5_RX_EQ1_S1,
	Pin_H4_RX_EQ1_S2,
	Pin_J6_RX_EQ2_S1,
	Pin_G4_RX_EQ2_S2,
	Pin_T8_TX_EQ1_S1,
	Pin_W7_TX_EQ1_S2,
	Pin_H6_TX_EQ2_S1,
	Pin_K4_TX_EQ2_S2,
};

enum{
	Pin_D21_SSMUX1_OEn,
	Pin_D20_SSMUX1_PDn,
	Pin_G20_SSMUX1_SEL1,
	Pin_F22_SSMUX1_SEL2,
	Pin_E22_SSMUX2_OEn,
	Pin_F12_SSMUX2_PDn,
	Pin_F4_SSMUX2_SEL1,
	Pin_A17_SSMUX2_SEL2,
};



int Init_GPIO(void);
void GPIO_Init_Direction(void);


#endif /* SRC_GPIO_INIT_H_ */
