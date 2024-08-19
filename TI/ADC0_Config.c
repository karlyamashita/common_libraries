/*
 * ADC0_Config.c
 *
 *  Created on: Jul 12, 2024
 *      Author: karl.yamashita
 */


#include "main.h"
#include "ADC0_Config.h"


/*
 * Description: SIB REV0/1, PMIC S, PMIC M, SIB ID
 *
 * Input:
 * Output:
 */
void ADC0_Config(void)
{
    // enable ADC Clock
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // enable PORTD
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    // enable PORTE
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // set ADC pin PORTD
    MAP_GPIOPinTypeADC(GPIO_PORTD_BASE, PSC48V_IMON_Pin | PSC48V_SNS_Pin);

    // set ADC pin PORTE
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, BRD_REV1_Pin | BRD_REV0_Pin | FAN_MON1_Pin | FAN_MON2_Pin | BRD_ID_Pin);

    // Use ADC0 to sample once for each timer period.
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_HALF, 1);

    // ******************************************************************************************************************************
    //                Setup Channels for sequencer 0
    // ******************************************************************************************************************************

    //Disable the ADC0 sequencer 0 interrupt on the processor (NVIC)
    MAP_IntDisable(INT_ADC0SS0);

    // Disable interrupts for ADC0 sample sequencer 0 to configure it.
    MAP_ADCIntDisable(ADC0_BASE, ADC0_SEQ_0);

    // disable ADC0 sample sequencer 0
    MAP_ADCSequenceDisable(ADC0_BASE, ADC0_SEQ_0);

    // sample sequencer 0 with a processor signal trigger
    MAP_ADCSequenceConfigure(ADC0_BASE, ADC0_SEQ_0, ADC_TRIGGER_PROCESSOR, 0);

    // configure sequencer 0 , step,  sample channels, interrupt flag, last conversion on sequencer 0.

    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 0, ADC_BRD_REV1);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 1, ADC_BRD_REV0);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 2, ADC_FAN_MON1);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 3, ADC_FAN_MON2);

    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 4, ADC_BRD_ID);

    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 5, ADC_PSC48V_SNS);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 6, ADC_PSC48V_IMON);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_0, 7, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); // internal temperature sensor ADC_CTL_TS

    // enable sequencer 0
    MAP_ADCSequenceEnable(ADC0_BASE, ADC0_SEQ_0);

    // clear interrupt prior to sampling
    MAP_ADCIntClear(ADC0_BASE, ADC0_SEQ_0);


    // ******************** No longer using timer interrupts to get ADC values so below is not used. Sequence init by MCU. ***********************************

    //register a handler
    ADCIntRegister(ADC0_BASE, ADC0_SEQ_0, ADCSeq0Handler);

    // enable interrupt
    MAP_ADCIntEnable(ADC0_BASE, ADC0_SEQ_0);

    // Enable the interrupt for ADC0 sequence 0 on the processor (NVIC).
    MAP_IntEnable(INT_ADC0SS0);
}




