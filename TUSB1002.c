/*
 * TUSB1002.c
 *
 *  Created on: Aug 19, 2021
 *      Author: karl.yamashita
 *
 *
 *		This is used in combination with a TS5A3357QDCURQ1 so you'll see reference to both devices in this file.
 *			The pins that are set/cleared in the functions below is the TS5A3357QDCURQ1 which in turn sets the TUSB1002 pins accordingly.
 *
 *      Requires TimerCallback.c/h
 */

#include "main.h"
#include "TUSB1002.h"


extern XGpio Gpio_1;

static bool Redriver1_Pulse_EN_flag = false;
static bool Redriver2_Pulse_EN_flag = false;

/*
 * Description: There are 16 functions to choose. For each functions there are two index.
 * 				Each index corresponds to what that COM is connected to.
 * 				According to the 405787 schematic,
 * 				0 = ground (IN2 = 1, IN1 = 1)
 * 				1 = 3v3 (IN2 = 0, IN1 = 1)
 * 				2 = resistor (IN2 = 1, IN1 = 0)
 * 				3 = Hi-Z (IN2 = 0, IN1 = 0)
 *
 */
const uint8_t TS5A3357QDCURQ1_portPins[17][2] = {
		{0, 0}, // index 0, not used

		{0, 0}, // 1
		{0, 2},
		{0, 3},
		{0, 1},
		{2, 0},
		{2, 2},
		{2, 3}, // 7
		{2, 1},
		{3, 0},
		{3, 2}, // 10
		{3, 3},
		{3, 1},
		{1, 0},
		{1, 2},
		{1, 3},
		{1, 1} // 16
};


/*
 * Description: Register callbacks to set the Redriver(x) EN pins high.
 *
 *
 */
void TUSB1002_Init(void){
	TimerCallbackRegister(TUSB1002_SetRedriver1_En, 100, NO_REPEAT_TIMER);
	TimerCallbackEnable(TUSB1002_SetRedriver1_En, T_DISABLED);

	TimerCallbackRegister(TUSB1002_SetRedriver2_En, 100, NO_REPEAT_TIMER);
	TimerCallbackEnable(TUSB1002_SetRedriver2_En, T_DISABLED);
}

/*
 * Description: Set Chan 1 RX EQ
 *
 *
 */
void TUSB1002_SetRedriver1_Rx_EQ(uint8_t idxPtr){
	uint8_t ptr;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << RX_EQ1_S1;
		pinSet |= 1 << RX_EQ1_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << RX_EQ1_S1;
		pinClear |= 1 << RX_EQ1_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << RX_EQ1_S1;
		pinSet |= 1 << RX_EQ1_S2;
		break;
	case 3: // float
		pinClear |= 1 << RX_EQ1_S1;
		pinClear |= 1 << RX_EQ1_S2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << RX_EQ2_S1;
		pinSet |= 1 << RX_EQ2_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << RX_EQ2_S1;
		pinClear |= 1 << RX_EQ2_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << RX_EQ2_S1;
		pinSet |= 1 << RX_EQ2_S2;
		break;
	case 3: // float
		pinClear |= 1 << RX_EQ2_S1;
		pinClear |= 1 << RX_EQ2_S2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver1_Pulse_EN_flag){
		TUSB1002_PulseRedriver1_En();
	}
}

/*
 * Description: Set TX EQ 1
 *
 *
 */
void TUSB1002_SetRedriver1_Tx_EQ(uint8_t idxPtr){
	uint8_t ptr;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];
	switch(ptr){
	case 0: // ground
		pinSet |= 1 << TX_EQ1_S1;
		pinSet |= 1 << TX_EQ1_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << TX_EQ1_S1;
		pinClear |= 1 << TX_EQ1_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << TX_EQ1_S1;
		pinSet |= 1 << TX_EQ1_S2;
		break;
	case 3: // float
		pinClear |= 1 << TX_EQ1_S1;
		pinClear |= 1 << TX_EQ1_S2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << TX_EQ2_S1;
		pinSet |= 1 << TX_EQ2_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << TX_EQ2_S1;
		pinClear |= 1 << TX_EQ2_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << TX_EQ2_S1;
		pinSet |= 1 << TX_EQ2_S2;
		break;
	case 3: // float
		pinClear |= 1 << TX_EQ2_S1;
		pinClear |= 1 << TX_EQ2_S2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver1_Pulse_EN_flag){
		TUSB1002_PulseRedriver1_En();
	}
}


/*
 * Description:
 *
 *
 */
void TUSB1002_SetRedriver1_Cfg_VOD(uint8_t idxPtr){
	uint8_t ptr;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << CFG1_S1;
		pinSet |= 1 << CFG1_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << CFG1_S1;
		pinClear |= 1 << CFG1_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << CFG1_S1;
		pinSet |= 1 << CFG1_S2;
		break;
	case 3: // float
		pinClear |= 1 << CFG1_S1;
		pinClear |= 1 << CFG1_S2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << CFG2_S1;
		pinSet |= 1 << CFG2_S2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << CFG2_S1;
		pinClear |= 1 << CFG2_S2;
		break;
	case 2: // Resistor
		pinClear |= 1 << CFG2_S1;
		pinSet |= 1 << CFG2_S2;
		break;
	case 3: // float
		pinClear |= 1 << CFG2_S1;
		pinClear |= 1 << CFG2_S2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver1_Pulse_EN_flag){
		TUSB1002_PulseRedriver1_En();
	}
}

/*****************************************************
					Redriver 2
 *****************************************************/


/*
 * Description:
 *
 *
 */
void TUSB1002_SetRedriver2_Rx_EQ(uint8_t idxPtr){
	uint8_t ptr;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << RX_EQ1_S1_REDRV2;
		pinSet |= 1 << RX_EQ1_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << RX_EQ1_S1_REDRV2;
		pinClear |= 1 << RX_EQ1_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << RX_EQ1_S1_REDRV2;
		pinSet |= 1 << RX_EQ1_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << RX_EQ1_S1_REDRV2;
		pinClear |= 1 << RX_EQ1_S2_REDRV2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << TX_EQ2_S1_REDRV2;
		pinSet |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << TX_EQ2_S1_REDRV2;
		pinClear |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << TX_EQ2_S1_REDRV2;
		pinSet |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << TX_EQ2_S1_REDRV2;
		pinClear |= 1 << TX_EQ2_S2_REDRV2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver2_Pulse_EN_flag){
		TUSB1002_PulseRedriver2_En();
	}
}

/*
 *
 *
 *
 */
void TUSB1002_SetRedriver2_Tx_EQ(uint8_t idxPtr){
	uint8_t ptr;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << TX_EQ1_S1_REDRV2;
		pinSet |= 1 << TX_EQ1_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << TX_EQ1_S1_REDRV2;
		pinClear |= 1 << TX_EQ1_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << TX_EQ1_S1_REDRV2;
		pinSet |= 1 << TX_EQ1_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << TX_EQ1_S1_REDRV2;
		pinClear |= 1 << TX_EQ1_S2_REDRV2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << TX_EQ2_S1_REDRV2;
		pinSet |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << TX_EQ2_S1_REDRV2;
		pinClear |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << TX_EQ2_S1_REDRV2;
		pinSet |= 1 << TX_EQ2_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << TX_EQ2_S1_REDRV2;
		pinClear |= 1 << TX_EQ2_S2_REDRV2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver2_Pulse_EN_flag){
		TUSB1002_PulseRedriver2_En();
	}
}

/*
 * Description: Set CFG2
 *
 *
 *
 */
void TUSB1002_SetRedriver2_Cfg_VOD(uint8_t idxPtr){
	uint8_t ptr = 0;
	uint32_t pinSet = 0; // pins to set
	uint32_t pinClear = 0; // pin to clear

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][0];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << CFG1_S1_REDRV2;
		pinSet |= 1 << CFG1_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << CFG1_S1_REDRV2;
		pinClear |= 1 << CFG1_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << CFG1_S1_REDRV2;
		pinSet |= 1 << CFG1_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << CFG1_S1_REDRV2;
		pinClear |= 1 << CFG1_S2_REDRV2;
		break;
	default:

		break;
	}

	ptr = TS5A3357QDCURQ1_portPins[idxPtr][1];

	switch(ptr){
	case 0: // ground
		pinSet |= 1 << CFG2_S1_REDRV2;
		pinSet |= 1 << CFG2_S2_REDRV2;
		break;
	case 1: // 3.3v
		pinSet |= 1 << CFG2_S1_REDRV2;
		pinClear |= 1 << CFG2_S2_REDRV2;
		break;
	case 2: // Resistor
		pinClear |= 1 << CFG2_S1_REDRV2;
		pinSet |= 1 << CFG2_S2_REDRV2;
		break;
	case 3: // float
		pinClear |= 1 << CFG2_S1_REDRV2;
		pinClear |= 1 << CFG2_S2_REDRV2;
		break;
	default:

		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	if(Redriver2_Pulse_EN_flag){
		TUSB1002_PulseRedriver2_En();
	}
}

/*
 * Description: Pulse EN pin. Sets pin to low then starts a timer callback to call TUSB1002_ToggleRedriver1_En
 *
 *
 */
void TUSB1002_PulseRedriver1_En(void){
	uint32_t pinClear = 0; // pins to clear

	pinClear |= 1 << REDRIVER1_EN;

	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	TimerCallbackEnable(TUSB1002_SetRedriver1_En, T_ENABLED);
}


/*
 * Description: Sets the pin high
 *
 *
 */
void TUSB1002_SetRedriver1_En(void){
	uint32_t pinSet = 0; // pins to set

	TimerCallbackEnable(TUSB1002_SetRedriver1_En, T_DISABLED);

	pinSet |= 1 << REDRIVER1_EN;

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
}


/*
 * Description: Sets the pin low
 *
 *
 */
void TUSB1002_ClearRedriver1_En(void){
	uint32_t pinClear = 0; // pins to set

	pinClear |= 1 << REDRIVER1_EN;

	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);
}


/*
 * Description: Auto pulse when EQ or VOD pins are changed
 *
 *
 */
void TUSB1002_Pulse1En(bool status){
	Redriver1_Pulse_EN_flag = status;
}


/*
 * Description: Pulse EN pin. Sets pin to low then starts a timer callback to call TUSB1002_ToggleRedriver2_En
 *
 *
 */
void TUSB1002_PulseRedriver2_En(void){
	uint32_t pinClear = 0; // pins to clear

	pinClear |= 1 << REDRIVER2_EN;

	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);

	TimerCallbackEnable(TUSB1002_SetRedriver2_En, T_ENABLED);
}


/*
 * Description: Sets the pin high
 *
 *
 */
void TUSB1002_SetRedriver2_En(void){
	uint32_t pinSet = 0; // pins to set

	TimerCallbackEnable(TUSB1002_SetRedriver2_En, T_DISABLED);

	pinSet |= 1 << REDRIVER2_EN;

	XGpio_DiscreteSet(&Gpio_1, CHANNEL_2, pinSet);
}


/*
 * Description: Sets the pin low
 *
 *
 */
void TUSB1002_ClearRedriver2_En(void){
	uint32_t pinClear = 0; // pins to set

	pinClear |= 1 << REDRIVER2_EN;

	XGpio_DiscreteClear(&Gpio_1, CHANNEL_2, pinClear);
}


/*
 * Description: Auto pulse when EQ or VOD pins are changed
 *
 *
 */
void TUSB1002_Pulse2En(bool status){
	Redriver2_Pulse_EN_flag = status;
}

