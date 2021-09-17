/*
 * NotifyUser.c
 *
 *  Created on: Sep 15, 2021
 *      Author: karl.yamashita
 *
 *
 */

#include "main.h"
#include "NotifyUser.h"


/*
 * Description: Notify user that the received command is valid.
 *
 * Input:
 * Output:
 */
void NotifyAck(void){
    UartCharBufferTxStruct uartTx;
    char str[64] = {0};

    strcpy(str, "ACK\r\n");

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    UartAddTxMessageBuffer(&uartTx);
}

/*
 * Description: Notify user the received command not valid
 *
 * Input:
 * Output:
 */
void NotifyNack(void){
    UartCharBufferTxStruct uartTx;
    char str[128] = {0};

    strcpy(str, "NACK\r\n");

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    UartAddTxMessageBuffer(&uartTx);
}


/*
 * Description:
 *
 *
 */
void NotifyErrorStatus(uint8_t error){
    UartCharBufferTxStruct uartTx;
    char str[16] = { 0 }; // main string, increase if needed
    char str2[3] = {0};

    strcpy(str, "CMDE=");

    itoa(error, str2, BASE_10);
    strcat(str, str2);
    strcat(str, "\r\n");

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    UartAddTxMessageBuffer(&uartTx);
}

/*
 * Description: Notify user of the FPGA config value
 *
 * Input: The Status value
 * Output:
 */
void NotifyFPGA_Addr_0x22_Value(uint8_t value){
    UartCharBufferTxStruct uartTx;
    char str[64] = { 0 }; // main string, increase if needed
    char str2[2] = {0};

    strcpy(str, "CMDU=0x");

    itoa(value, str2, BASE_16);
    strcat(str, str2);
    strcat(str, "\r\n");

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    UartAddTxMessageBuffer(&uartTx);
}

/*
 * Description: Checks for reset cause, if any. Notifies user of what caused reset
 *
 * Input:
 * Output:
 */
void NotifyResetCause(void){
    UartCharBufferTxStruct uartTx;
    uint32_t status;
    char str[32] = {0};

    status = SysCtlResetCauseGet();
    if ((status & SYSCTL_CAUSE_SW) == SYSCTL_CAUSE_SW)
    {
        SysCtlResetCauseClear(SYSCTL_CAUSE_SW);
        strcpy(str, "CMDR:1\r\n");
    }

    if ((status & SYSCTL_CAUSE_POR) == SYSCTL_CAUSE_POR)
    {
        SysCtlResetCauseClear(SYSCTL_CAUSE_POR);
        strcpy(str, "CMDR:2\r\n");
    }

    if ((status & SYSCTL_CAUSE_WDOG0) == SYSCTL_CAUSE_WDOG0)
    {
        SysCtlResetCauseClear(SYSCTL_CAUSE_WDOG0);
        strcpy(str, "CMDR:3\r\n");
    }

    if ((status & SYSCTL_CAUSE_WDOG1) == SYSCTL_CAUSE_WDOG1)
    {
        SysCtlResetCauseClear(SYSCTL_CAUSE_WDOG1);
        strcpy(str, "CMDR:4\r\n");
    }

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    //UartAddTxMessageBuffer(&uartTx);
    UartTxMessage(&uartTx);
}




