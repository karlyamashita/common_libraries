/*
 * PeripheralsInit.c
 *
 *  Created on: Mar 12, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "PeripheralsInit.h"


//#pragma DATA_ALIGN(pui8ControlTable, 1024)
//uint8_t pui8ControlTable[1024];

//static uint16_t pui16ADCBuffer1[ADC_SAMPLE_BUF_SIZE];
//static uint16_t pui16ADCBuffer2[ADC_SAMPLE_BUF_SIZE];



void SystemTickInit(void){
    // set tick period ms
    SysTickPeriodSet(SysCtlClockGet() / 1000);

    // Enable SysTick.
    SysTickEnable();

    // Enable the SysTick Interrupt.
    SysTickIntEnable();

    //register a handler
    SysTickIntRegister(SysTick_Handler);

    // enable interrupt to processor
    IntMasterEnable();
}


void SystemClock_Config(void){
    // setup clock
    //
    // Configure the device to run at 25 MHz
    //
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN);
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void MX_GPIO_Init(void){

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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    //  set as outputs
    // PORTF
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= Debug_Grn_Pin;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0; // lock
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Debug_Grn_Pin | Debug_Red_Pin | Led_Red_Pin | Led_Grn_Pin | Load4En_Pin);

    // PORTD
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= Load3En_Pin;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0; // lock
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, Load3En_Pin);

    // PORTE
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, Load1En_Pin | Load2En_Pin);

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    // PORTD
    MAP_GPIODirModeSet(GPIO_PORTD_BASE, Load3_48V_FLTn_Pin | Alert1_Pin | Alert2_Pin, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, Load3_48V_FLTn_Pin | Alert1_Pin | Alert2_Pin, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // PORTA
    MAP_GPIODirModeSet(GPIO_PORTA_BASE, I2C1_SCL_Pin | I2C1_SDA_Pin, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, I2C1_SCL_Pin | I2C1_SDA_Pin, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);


    //set interrupt pins
    GPIOIntTypeSet(GPIO_PORTD_BASE, Load3_48V_FLTn_Pin | Alert1_Pin | Alert2_Pin, GPIO_BOTH_EDGES);
    // PORTA
   // GPIOIntTypeSet(GPIO_PORTA_BASE, I2C1_SCL_Pin | I2C1_SDA_Pin, GPIO_BOTH_EDGES);

    //register a handler
    GPIOIntRegister(GPIO_PORTD_BASE, PortDIntHandler);
    // PORTA
    //GPIOIntRegister(GPIO_PORTA_BASE, PortAIntHandler);

    // enable interrupts for pins
    GPIOIntEnable(GPIO_PORTD_BASE, Load3_48V_FLTn_Pin | Alert1_Pin | Alert2_Pin);
    // PORTA
    //GPIOIntEnable(GPIO_PORTA_BASE, I2C1_SCL_Pin | I2C1_SDA_Pin);
}

/*
 * Description: also PWM
 *
 *
 */
/*
void MX_TIM1_Init(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //
    // Enable the GPIO pin for the LED (PF3) as an output.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Enable the GPIO port that is used for the PWM output.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the PWM function for this pin.
    //
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);

    //
    // Configure PWM0 to count down without synchronization.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (SysCtlClockGet() / 250));

    //
    // For this example the PWM0 duty cycle will be variable.  The duty cycle
    // will start at 0.1% and will increase to 75%. After a duty cycle of 75%
    // is reached, it is reset back to 0.1%.  This dynamic adjustment of the
    // pulse width is done in the PWM0 load interrupt, which increases the
    // duty cycle by 0.1% every time the reload interrupt is received.
    //

    //
    // Set the PWM increment variable based on the System Clock. Since this
    // is a 250 Hz PWM, continue to use the equation N = (1 / f) * SysClk.
    // Then to set the initial period to 0.1% by dividing (N / 1000).
    // This variable will be used to increment PWM0 by 0.1% on each
    // interrupt.
    //
    g_ui32PWMIncrement = ((SysCtlClockGet() / 250) / 1000);

    //
    // Set the initial PWM0 Pulse Width with the calculated increment variable
    // to start at 0.1% duty cycle.
    //
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, g_ui32PWMIncrement);

    //
    // Enable processor interrupts.
    //
    MAP_IntMasterEnable();

    //
    // Allow PWM0 generated interrupts.  This configuration is done to
    // differentiate fault interrupts from other PWM0 related interrupts.
    //
    MAP_PWMIntEnable(PWM0_BASE, PWM_INT_GEN_0);

    //
    // Enable the PWM0 LOAD interrupt on PWM Gen 0.
    //
    MAP_PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);

    //
    // Enable the interrupt for PWM Gen 0 on the processor (NVIC).
    //
    MAP_IntEnable(INT_PWM0_0);

    //
    // Enable the PWM Out Bit 0 (PB6) output signal.
    //
    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);

    //
    // Enable the PWM generator block.
    //
    MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_0);

}
*/


/*
 * Description: Master for TMP101 sensors
 *
 *
 */
void MX_I2C0_Init(void){

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
}

/*
 * Description: Master for FPGA communication
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
}

/*
 * Description: Port for Telemetry data
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
   /*
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }
*/
    MAP_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, Uart0_PIN_RX | Uart0_PIN_TX);

    MAP_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //register a handler
    UARTIntRegister(UART0_BASE, USART0_IRQHandler);
}

/*
 * Description: Debug port
 *
 * Input:
 * Output:
 */
void MX_USART3_UART_Init(void){

    // enable peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    MAP_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, Uart3_PIN_RX | Uart3_PIN_TX);

    MAP_UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(INT_UART3);
    MAP_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);

    //register a handler
    UARTIntRegister(UART3_BASE, USART3_IRQHandler);

}

/*
 * Description: Set AIN1 and AIN4
 *
 * Input:
 * Output:
 */
void MX_ADC0_Init(void){

    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // enable port the ADC CH0 is on
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // enable port the ADC CH1 is on
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // enable timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // set ADC pin CH4
    GPIOPinTypeADC(GPIO_PORTE_BASE, _48V_VMon_Pin);

    // set ADC pin CH0
    GPIOPinTypeADC(GPIO_PORTD_BASE, load3_IMON_Pin);

    // Use ADC0 to sample once for each timer period.
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_HALF, 1);

    SysCtlDelay(10);

    // ******************************************************************************************************************************
    //                Setup Channel 4 for sequence 0, iMON
    // ******************************************************************************************************************************

    //Disable the ADC0 sequence 3 interrupt on the processor (NVIC)
    IntDisable(INT_ADC0SS3);

    // Disable interrupts for ADC0 sample sequence 3 to configure it.
    ADCIntDisable(ADC0_BASE, SEQ_3);

    // disable ADC0 sample sequence 3
    ADCSequenceDisable(ADC0_BASE, SEQ_3);

    // sample sequence 3 with a processor signal trigger
    ADCSequenceConfigure(ADC0_BASE, SEQ_3, ADC_TRIGGER_TIMER, 0);

    // configure sequence 3, step,  sample channel 4, interrupt flag, last conversion on sequence 0. PD3 = CH4, PE2 = CH1
    ADCSequenceStepConfigure(ADC0_BASE, SEQ_3, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

    // enable sequence 3
    ADCSequenceEnable(ADC0_BASE, SEQ_3);

    // clear interrupt prior to sampling
    ADCIntClear(ADC0_BASE, SEQ_3);

    //register a handler
    ADCIntRegister(ADC0_BASE, SEQ_3, ADC_iMon_IRQHandler);

    // enable interrupt
    ADCIntEnable(ADC0_BASE, SEQ_3);

    // Enable the interrupt for ADC0 sequence 3 on the processor (NVIC).
    IntEnable(INT_ADC0SS3);

    // *********************************** END Setup for Channel 0 ******************************************

    // ******************************************************************************************************************************
    //                Setup Channel 1 for sequence 1, 48v monitor
    // ******************************************************************************************************************************

    //Disable the ADC0 sequence 1 interrupt on the processor (NVIC)
    IntDisable(INT_ADC0SS1);

    // Disable interrupts for ADC0 sample sequence 1 to configure it.
    ADCIntDisable(ADC0_BASE, 1);

    // disable ADC0 sample sequence 1
    ADCSequenceDisable(ADC0_BASE, 1);

    // sample sequence 1 with a processor signal trigger
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);

    // configure sequence 1, step,  sample channel 1, interrupt flag, last conversion on sequence 1. PD3 = CH4, PE2 = CH1
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    // enable sequence 1
    ADCSequenceEnable(ADC0_BASE, 1);

    // clear interrupt prior to sampling
    ADCIntClear(ADC0_BASE, 1);

    //register a handler
    ADCIntRegister(ADC0_BASE, 1, ADC_48V_MON_IRQHandler);

    // enable interrupt
    ADCIntEnable(ADC0_BASE, 1);

    // Enable the interrupt for ADC0 sequence 1 on the processor (NVIC).
    IntEnable(INT_ADC0SS1);
    // *********************************** END Setup for Channel 1 ******************************************

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
}

void MX_EEPROM_Init(void){

    uint32_t errorCode;
    /*
    uint32_t eepromValue = 0;
    uint32_t wData = 0;
    uint32_t rData = 0;
*/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0));

    errorCode = EEPROMInit();
    if(errorCode != EEPROM_INIT_OK){
        return;
    }
    /*
    wData = TEMP_HIGH_LIMIT;
    errorCode = EEPROMProgram(&wData, 0, 4);
    EEPROMRead(&rData, 0, 4);
    errorCode = memcmp(&wData, &rData, 4);

    wData = TEMP_LOW_LIMIT;
    errorCode = EEPROMProgram(&wData, 1, 4);

    //EEPROMRead(&rData, 1, 4);
    EEPROMRead(&rData, 0, 4);
    eepromValue = rData;
    EEPROMRead(&rData, 1, 4);
    eepromValue |= rData << 16;

    errorCode = memcmp(&wData, &rData, 4);

    if(errorCode != 0){
        asm(" nop");
    }
    */
}

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
