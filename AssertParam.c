/*
 * AssertParam.c
 *
 *  Created on: Jul 15, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Description: A better approach to using assert where user can do some debugging in assert_failed() function.
 *              TI's assert is a macro so can't override.
 *
 */


// User should override this function.
__weak void assert_failed(uint8_t *file, uint32_t line)
{
    // do not modify this function. Override it instead.
    UNUSED(file);
    UNUSED(line);
}



/*
 * Example how to use.
 *
 * #define IS_UART_BASE(__UARTBASE__) ( ((__UARTBASE__) == UART0_BASE) || ((__UARTBASE__) == UART1_BASE) )
 *
 * assert_param(IS_UART_BASE(UART0_BASE)); // if not zero, will call assert_failed() with parameters.
 *
 */
