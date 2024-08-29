/*
 * PeripheralsInit.c
 *
 *  Created on: Mar 12, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "PeripheralsInit.h"



uint8_t i2c_dataRx[8] = {0};
uint8_t i2c_dataTx[8] = {0};

/*
 * Description: Enable system tick with 1ms interrupt
 *
 *
 */
void SystemTickInit(void){
    // set tick period ms
    MAP_SysTickPeriodSet(SysCtlClockGet() / 1000);

    // Enable SysTick.
    MAP_SysTickEnable();

    // Enable the SysTick Interrupt.
    MAP_SysTickIntEnable();

    //register a handler
    SysTickIntRegister(SysTick_Handler);

    // enable interrupt to processor
    MAP_IntMasterEnable();
}

/*
 * Description: Configure the clock
 *
 *
 */
void SystemClock_Config(void){
    // setup clock
    //
    // Configure the device to run at 25 MHz
    //
    //
#ifdef LAUNCH_PAD
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Launchpad
#else
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN); // SIB
#endif
}

// Avenger

/*
 * Description:
 *
 * Input:
 * Output:
 */
void MX_GPIO_Init(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }


    //  set as outputs
    // PORTA
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3 );
    //MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTA_BASE, GPIO_PIN_3 ); // open drain

    // PORTB
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, USB_VBUS_Enable_Pin | RETIMER_DIS_N_Pin_PB7);
    ///MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTB_BASE, USB_VBUS_Enable_Pin); // open drain

    // PORTC
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, TC_RST_N_OUT_PC5 | RETIMER_P_POL_Pin_PC6 | USBHUB_Reset_Pin);
    MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTC_BASE, USBHUB_Reset_Pin);

    // PORTD
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= FT4232_Reset_3_Pin;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0; // lock
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, FT4232_Reset_1_Pin | FT4232_Reset_2_Pin | FT4232_Reset_3_Pin);
    MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTD_BASE, FT4232_Reset_1_Pin | FT4232_Reset_2_Pin | FT4232_Reset_3_Pin); // make open drain

    // PORTE
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, PMIC_S_Pin | PMIC_M_Pin | SSD_Alart_Pin_PE5);

    // PORTF
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= TIB_TYPE_Pin_PF0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0; // lock
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, UART_ENABLE_Pin_PF4 | TIB_TYPE_Pin_PF0 | Led_Red_Pin | Led_Grn_Pin | VBAT_EN_Pin);
    //MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTC_BASE, VBAT_EN_Pin);

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    // PORTA
    //MAP_GPIODirModeSet(GPIO_PORTA_BASE, LTM4675_Alert_Pin, GPIO_DIR_MODE_IN);
    //MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, LTM4675_Alert_Pin, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // PORTB
    MAP_GPIODirModeSet(GPIO_PORTB_BASE, USB_VBUS_FLAG_Pin | TMP101_Alert_Pin, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, USB_VBUS_FLAG_Pin | TMP101_Alert_Pin, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // PORTC
    MAP_GPIODirModeSet(GPIO_PORTC_BASE, SPI_DAV_V_Pin_PC4, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, SPI_DAV_V_Pin_PC4, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // PORTE
    MAP_GPIODirModeSet(GPIO_PORTE_BASE, SSD_Alart_Pin_PE5, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, SSD_Alart_Pin_PE5 , GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // Init interrupts
    //set interrupt pins
    // PORTA
    //GPIOIntTypeSet(GPIO_PORTA_BASE, LTM4675_Alert_Pin, GPIO_BOTH_EDGES);
    // PORTB
    GPIOIntTypeSet(GPIO_PORTB_BASE, TMP101_Alert_Pin | USB_VBUS_FLAG_Pin, GPIO_BOTH_EDGES);
    // PORTE
    GPIOIntTypeSet(GPIO_PORTE_BASE, SSD_Alart_Pin_PE5, GPIO_BOTH_EDGES);

    //register a handler
    // PORTA
    //GPIOIntRegister(GPIO_PORTA_BASE, PortAIntHandler);
    // PORTB
    GPIOIntRegister(GPIO_PORTB_BASE, GPIO_PortB_Handler);
    // PORTE
    GPIOIntRegister(GPIO_PORTE_BASE, GPIO_PortE_Handler);

    // enable interrupts for pins
    // PORTA
    //GPIOIntEnable(GPIO_PORTA_BASE, LTM4675_Alert_Pin);
    // PORTB
    GPIOIntEnable(GPIO_PORTB_BASE, TMP101_Alert_Pin | USB_VBUS_FLAG_Pin);
    // PORTE
    GPIOIntEnable(GPIO_PORTE_BASE, SSD_Alart_Pin_PE5);


}

/*
 * Description: Master for MCDP6200, INA3221 and TMP108 sensors
 *
 *
 *
 */
void MX_I2C1_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
    {
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    // init the master module clock and 100kbps
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

    MAP_IntEnable(INT_I2C1);

    // Register Handler
    I2CIntRegister(I2C1_BASE, I2C1_EV_IRQHandler);

}

//tI2CMInstance i2cm = {0};

void pvCallbackData(void)
{

}

void MX_I2CMInit(void)
{
   // I2CMInit(&i2cm, I2C1_BASE, INT_I2C1, UDMA_CHANNEL_UART0TX, UDMA_CHANNEL_UART0RX,SysCtlClockGet());



   // I2CMRead(&i2cm, 0x40, i2c_dataTx, 1, i2c_dataRx, 2, &I2C1M_Rx_Callback, pvCallbackData);

}

void I2C1M_Rx_Callback(void)
{

}

void I2C1_Reset_Bus(void)
{
    // peripheral = SYSCTL_PERIPH_I2C1
    SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

    MX_I2C1_Init();
}

/*
 * Description: Port for protocol messages
 *
 * Input:
 * Output:
 */
void MX_USART0_UART_Init(void){

    // enable peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, UART0_PIN_RX | UART0_PIN_TX);

    MAP_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);

    MAP_UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_EOT); // 6-12-2024, finally found how to get Tx to interrupt

    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);
    UARTFIFOEnable(UART0_BASE);

    //register a handler
    UARTIntRegister(UART0_BASE, USART0_IRQHandler);
}

/*
 * Description: Port for protocol messages
 *
 * Input:
 * Output:
 */
void MX_USART6_UART_Init(void)
{

    // enable peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART6))
    {
    }

    //
    // Set GPIO PD4 and PD5 as UART pins.
    //
    GPIOPinConfigure(GPIO_PD4_U6RX);
    GPIOPinConfigure(GPIO_PD5_U6TX);
    MAP_GPIOPinTypeUART(GPIO_PORTD_BASE, Uart6_PIN_RX_PD4 | Uart6_PIN_TX_PD5);

    MAP_UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(INT_UART6);
    MAP_UARTIntEnable(UART6_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);

    MAP_UARTTxIntModeSet(UART6_BASE, UART_TXINT_MODE_EOT); // 6-12-2024, finally found how to get Tx to interrupt

    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);
    UARTFIFOEnable(UART6_BASE);

    //register a handler
    UARTIntRegister(UART6_BASE, USART6_IRQHandler);
}

/*
 * Description: SIB REV0/1, PMIC S, PMIC M, SIB ID
 *
 * Input:
 * Output:
 */
void MX_ADC0_Init(void){

    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // enable PORTB the ADC is on
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // enable PORTD the ADC is on
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // enable PORTE the ADC is on
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // enable timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // set ADC pin PORTB
    GPIOPinTypeADC(GPIO_PORTB_BASE, VBUS_5V_CON_PRES_Pin);

    // set ADC pin PORTE
    GPIOPinTypeADC(GPIO_PORTE_BASE, SIB_Rev1_Pin | SIB_Rev2_Pin | SIB_ID_Pin | PMIC_S_Pin | PMIC_M_Pin);

    // Use ADC0 to sample once for each timer period.
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_HALF, 1);

    SysCtlDelay(10);

    // ******************************************************************************************************************************
    //                Setup Channels for sequencer 0
    // ******************************************************************************************************************************

    //Disable the ADC0 sequencer 0 interrupt on the processor (NVIC)
    IntDisable(INT_ADC0SS0);

    // Disable interrupts for ADC0 sample sequencer 0 to configure it.
    ADCIntDisable(ADC0_BASE, SEQ_0);

    // disable ADC0 sample sequencer 0
    ADCSequenceDisable(ADC0_BASE, SEQ_0);

    // sample sequencer 0 with a processor signal trigger
    //ADCSequenceConfigure(ADC0_BASE, SEQ_0, ADC_TRIGGER_TIMER, 0);
    ADCSequenceConfigure(ADC0_BASE, SEQ_0, ADC_TRIGGER_PROCESSOR, 0); // no longer using interrupts to get ADC values. Sequence init by MCU.

    // configure sequencer 0 , step,  sample channels, interrupt flag, last conversion on sequencer 0.
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 0, ADC_VBUS);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 1, ADC_SIB_ID);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 2, ADC_SIB_REV0);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 3, ADC_SIB_REV1);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 4, ADC_PMIC_S);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 5, ADC_PMIC_M);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 6, ADC_CTL_TS); // channel FIFO 6 and 7 are the same reading
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_0, 7, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); // internal temperature sensor ADC_CTL_TS

    // enable sequencer 0
    ADCSequenceEnable(ADC0_BASE, SEQ_0);

    // clear interrupt prior to sampling
    ADCIntClear(ADC0_BASE, SEQ_0);


    // ******************** No longer using timer interrupts to get ADC values so below is not used. Sequence init by MCU. ***********************************

    //register a handler
    //ADCIntRegister(ADC0_BASE, SEQ_0, ADC0_IRQHandler);

    // enable interrupt
    //ADCIntEnable(ADC0_BASE, SEQ_0);

    // Enable the interrupt for ADC0 sequence 0 on the processor (NVIC).
    //IntEnable(INT_ADC0SS0);
    /*
    // Configure timer for A/D
    // Configure a 16-bit periodic timer.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);

    // Set ADC sampling frequency to be 25KHz i.e. every 40uS.
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet()/25000) - 1);

    // Enable the ADC trigger output for Timer A.
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

    // Enable processor interrupts.
    IntMasterEnable();

    // Enable Timer 0 which will start the whole application process.
    TimerEnable(TIMER0_BASE, TIMER_A);
*/
}

/*
 * Description: DUT SITE ID0/1/2,
 *
 * Input:
 * Output:
 */
void MX_ADC1_Init(void){

    // Enable ADC1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC1));

    // enable PORTB the ADC is on
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // enable PORTD the ADC is on
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // enable PORTE the ADC is on
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // enable timer
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // set ADC pin PORTD
    GPIOPinTypeADC(GPIO_PORTD_BASE, DUT_SITE_ID0_Pin | DUT_SITE_ID1_Pin | DUT_SITE_ID2_Pin);

    // Use ADC0 to sample once for each timer period.
    ADCClockConfigSet(ADC1_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_HALF, 1);

    SysCtlDelay(10);

    // ******************************************************************************************************************************
    //                Setup Channels for sequencer 1
    // ******************************************************************************************************************************

    //Disable the ADC1 sequencer 1 interrupt on the processor (NVIC)
    IntDisable(INT_ADC1SS0);

    // Disable interrupts for ADC1 sample sequencer 1 to configure it.
    ADCIntDisable(ADC1_BASE, SEQ_0);

    // disable ADC1 sample sequencer 1
    ADCSequenceDisable(ADC1_BASE, SEQ_0);

    // sample sequencer 1 with a processor signal trigger
    //ADCSequenceConfigure(ADC1_BASE, SEQ_1, ADC_TRIGGER_TIMER, 0);
    ADCSequenceConfigure(ADC1_BASE, SEQ_0, ADC_TRIGGER_PROCESSOR, 0);

    // configure sequencer 1 , step,  sample channels, interrupt flag, last conversion on sequencer 1.
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 0, ADC_DUT_ID0);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 1, ADC_DUT_ID1);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 2, ADC_DUT_ID2);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 3, ADC_DUT_ID0);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 4, ADC_DUT_ID1);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 5, ADC_DUT_ID2);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 6, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC1_BASE, SEQ_0, 7, ADC_CTL_TS  | ADC_CTL_IE | ADC_CTL_END); // for SEQ1, number of samples is 4,
                                                                                            // so read channel again else i believe values will get out of sync?
    // enable sequencer 1
    ADCSequenceEnable(ADC1_BASE, SEQ_0);

    // clear interrupt prior to sampling
    ADCIntClear(ADC1_BASE, SEQ_0);


    //register a handler
    //ADCIntRegister(ADC1_BASE, SEQ_1, ADC1_IRQHandler);

    // enable interrupt
    //ADCIntEnable(ADC1_BASE, SEQ_1);

    // Enable the interrupt for ADC1 sequence 1 on the processor (NVIC).
    //IntEnable(INT_ADC1SS1);
    /*
    // Configure timer for A/D
    // Configure a 16-bit periodic timer.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);

    // Set ADC sampling frequency to be 25KHz i.e. every 40uS.
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet()/25000) - 1);

    // Enable the ADC trigger output for Timer A.
    TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

    // Enable processor interrupts.
    IntMasterEnable();

    // Enable Timer 0 which will start the whole application process.
    TimerEnable(TIMER0_BASE, TIMER_A);

    */
}

/*
 * Description: TC communication
 *
 *
 */
void MX_SPI_Init(void)
{
    //
    // The SSI0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0))
    {
    }

    //
    // For this example SSI0 is used with PortA[5:2].  The actual port and pins
    // used may be different on your part, consult the data sheet for more
    // information.  GPIO port A needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    //GPIOPinConfigure(GPIO_PA3_SSI0FSS); // controlled by spi_utilites.c

    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);


    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PA5 - SSI0Tx (TM4C123x) / SSI0XDAT1 (TM4C129x)
    //      PA4 - SSI0Rx (TM4C123x) / SSI0XDAT0 (TM4C129x)
    //      PA3 - SSI0Fss
    //      PA2 - SSI0CLK
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
                       SSI_MODE_MASTER, 2000000, 8);

    // init uDMA
   // uDMAEnable();

    // set up DMA
   // SSIDMAEnable(SSI0_BASE, SSI_DMA_RX | SSI_DMA_TX);

    // enable interrupt
    SSIIntRegister(SSI0_BASE, SPI_Handler);
    SSIIntEnable(SSI0_BASE, SSI_RXFF);

    //
    // Enable the SSI0 module.
    //
    SSIEnable(SSI0_BASE);
}

/*
 * Description: Initialize Watch Dog
 *
 *
 */
void MX_WDOG_Init(void){

    //
    // Enable the peripherals used by this example.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    //
    // Enable the watchdog interrupt.
    //
    MAP_IntEnable(INT_WATCHDOG);

    //
    // Set the period of the watchdog timer.
    //
    MAP_WatchdogReloadSet(WATCHDOG0_BASE, MAP_SysCtlClockGet());

    //
    // Enable reset generation from the watchdog timer.
    //
    MAP_WatchdogResetEnable(WATCHDOG0_BASE);

    //
    // Enable the watchdog timer.
    //
    MAP_WatchdogEnable(WATCHDOG0_BASE);
}

void MX_EEPROM_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
    {
    }

    EEPROMInit();
}


