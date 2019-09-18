/*******************************************************************************
 Simple SPI Transfer function

  File Name:
    drv_spi.c

  Summary:
    Initializes SPI 1. Transfers data over SPI.
    Uses SPI FIFO to speed up transfer.

  Description:
    .

  Remarks:
    
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2016 Microchip Technology Inc. and its subsidiaries.  
You may use this software and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, 
COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER 
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED 
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, 
MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE 
WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
 *******************************************************************************/
// DOM-IGNORE-END

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "drv_spi.h"
//#include "peripheral/spi/plib_spi.h"		// SPI PLIB Header
//#include "system_config.h"
//#include "system_definitions.h"


int8_t DRV_SPI_ChipSelectAssert(uint8_t spiSlaveDeviceIndex, bool assert)
{
    int8_t error = 0;

    // Select Chip Select
    switch (spiSlaveDeviceIndex) {
//        case DRV_CANFDSPI_INDEX_0:
//            if (assert) SYS_PORTS_PinClear(PORTS_ID_0, SPI_CS0_PORT_ID, SPI_CS0_PORT_PIN);
//            else SYS_PORTS_PinSet(PORTS_ID_0, SPI_CS0_PORT_ID, SPI_CS0_PORT_PIN);
//            break;
//        case DRV_CANFDSPI_INDEX_1:
//            if (assert) SYS_PORTS_PinClear(PORTS_ID_0, SPI_CS1_PORT_ID, SPI_CS1_PORT_PIN);
//            else SYS_PORTS_PinSet(PORTS_ID_0, SPI_CS1_PORT_ID, SPI_CS1_PORT_PIN);
//            break;
        default:
            error = -1;
            break;
    }

    return error;
}

void DRV_SPI_Initialize(void)
{
//    /* Disable the SPI module to configure it*/
//    PLIB_SPI_Disable(SPI_ID_1);

//    /* Set up Master mode */
//    PLIB_SPI_MasterEnable(SPI_ID_1);
//    PLIB_SPI_PinDisable(SPI_ID_1, SPI_PIN_SLAVE_SELECT);

//    /* Set up if the SPI is allowed to run while the rest of the CPU is in idle mode*/
//    PLIB_SPI_StopInIdleDisable(SPI_ID_1);

//    /* Set up clock Polarity and output data phase*/
//#ifdef SPI_MODE_00
//    PLIB_SPI_ClockPolaritySelect(SPI_ID_1, SPI_CLOCK_POLARITY_IDLE_LOW);
//    PLIB_SPI_OutputDataPhaseSelect(SPI_ID_1, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK);
//#else
//    PLIB_SPI_ClockPolaritySelect(SPI_ID_1, SPI_CLOCK_POLARITY_IDLE_HIGH);
//    PLIB_SPI_OutputDataPhaseSelect(SPI_ID_1, SPI_OUTPUT_DATA_PHASE_ON_IDLE_TO_ACTIVE_CLOCK);
//#endif
//    
//    /* Set up the Input Sample Phase*/
//    PLIB_SPI_InputSamplePhaseSelect(SPI_ID_1, SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE);

//    /* Communication Width Selection */
//    PLIB_SPI_CommunicationWidthSelect(SPI_ID_1, SPI_COMMUNICATION_WIDTH_8BITS);

//    /* Baud rate selection */
//    PLIB_SPI_BaudRateSet(SPI_ID_1, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_1), SPI_BAUDRATE);

//    /* Protocol selection */
//    PLIB_SPI_FramedCommunicationDisable(SPI_ID_1);
//    if (PLIB_SPI_ExistsAudioProtocolControl(SPI_ID_1)) {
//        PLIB_SPI_AudioProtocolDisable(SPI_ID_1);
//    }

//    /* Buffer type selection */
//    if (PLIB_SPI_ExistsFIFOControl(SPI_ID_1)) {
//        PLIB_SPI_FIFOEnable(SPI_ID_1);
//        PLIB_SPI_FIFOInterruptModeSelect(SPI_ID_1, SPI_FIFO_INTERRUPT_WHEN_TRANSMIT_BUFFER_IS_COMPLETELY_EMPTY);
//        PLIB_SPI_FIFOInterruptModeSelect(SPI_ID_1, SPI_FIFO_INTERRUPT_WHEN_RECEIVE_BUFFER_IS_NOT_EMPTY);
//    }

//    PLIB_SPI_BufferClear(SPI_ID_1);
//    PLIB_SPI_ReceiverOverflowClear(SPI_ID_1);

//    /* Enable the Module */
//    PLIB_SPI_Enable(SPI_ID_1);

    return;
}

int8_t DRV_SPI_TransferData(uint8_t spiSlaveDeviceIndex, uint8_t *SpiTxData, uint8_t *SpiRxData, uint16_t spiTransferSize)
{
    int8_t error = 0;
    bool continueLoop;
    uint16_t txcounter = 0;
    uint16_t rxcounter = 0;
    uint8_t unitsTxed = 0;
    const uint8_t maxUnits = 16;

    // Assert CS
    error = DRV_SPI_ChipSelectAssert(spiSlaveDeviceIndex, true);
    if (error != 0) return error;

    // Loop until spiTransferSize
    do {
        continueLoop = false;
        unitsTxed = 0;
/*
        // Fill transmit FIFO
        if (PLIB_SPI_TransmitBufferIsEmpty(SPI_ID_1)) {
            while (!PLIB_SPI_TransmitBufferIsFull(SPI_ID_1) && (txcounter < spiTransferSize) && unitsTxed != maxUnits) {
                PLIB_SPI_BufferWrite(SPI_ID_1, SpiTxData[txcounter]);
                txcounter++;
                continueLoop = true;
                unitsTxed++;
            }
        }

        // Read as many bytes as were queued for transmission
        while (txcounter != rxcounter) {
            while (PLIB_SPI_ReceiverFIFOIsEmpty(SPI_ID_1));
            SpiRxData[rxcounter] = PLIB_SPI_BufferRead(SPI_ID_1);
            rxcounter++;
            continueLoop = true;
        }
*/
        // Make sure data gets transmitted even if buffer wasn't empty when we started out with
        if ((txcounter > rxcounter) || (txcounter < spiTransferSize)) {
            continueLoop = true;
        }

    } while (continueLoop);

    // De-assert CS
    error = DRV_SPI_ChipSelectAssert(spiSlaveDeviceIndex, false);

    return error;
}

