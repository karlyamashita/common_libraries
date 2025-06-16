/*
 * CommandList_Handler.c
 *
 *  Created on: May 1, 2025
 *      Author: karl.yamashita
 */

#include "main.h"

#include "ADC_Handler.h"


// update this for total instances created
#define COMMAND_TOTAL 13


const Command_t help =
{
	.command = "help",
	.args = "help\r\n"
			"\thelp <color> k,r,g,y,b,m,c,w",
	.func = Command_List_Print
};


/*
 * User shall add new instances to array
 */
//Command_t Command_List[COMMAND_TOTAL] = {0};

const Command_t Command_List[] =
{
	CMD_HELP,
	CMD_ADC,
	CMD_DCOK
};


/*
 * Description: Add new instances to array to be init
 */
void CommandList_Init(void)
{
	/*
	Command_List[0] = help;
	Command_List[1] = adc;
	Command_List[2] = dcok;
	Command_List[3] = debug;
	Command_List[4] = gpio;
	Command_List[5] = i2caddress;
	Command_List[6] = interpol;
	Command_List[7] = ltm4683;
	Command_List[8] = railaddren;
	Command_List[9] = serial;
	Command_List[10] = stm32;
	Command_List[11] = telemetry;
	Command_List[12] = version;
	*/
}




// need to place here for CommandList_Drv.c while extern placed in header file. "extern int CommandList_array_size;"
int CommandList_array_size = sizeof(Command_List);

