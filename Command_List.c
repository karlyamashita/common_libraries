/*
 * Command_List.c
 *
 *  Created on: Apr 10, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Description: Command functions with information and pointer to a function to be called.
 * 				extern each of these to main.h
 * 				Example; extern const Command_t help
 */

const Command_t help =
{
	.command = "help",
	.args = "help | help 1 = colored commands",
	.func = Print_Functions
};

const Command_t version =
{
	.command = "get fw version",
	.args = "none",
	.func = Version
};

const Command_t bootloader =
{
	.command = "set bootloader",
	.args = "none",
	.func = Bootloader
};

const Command_t serial_get =
{
	.command = "get serial",
	.args = "none",
	.func = FLASH_DATA_Get_String
};

const Command_t part_get =
{
	.command = "get part",
	.args = "none",
	.func = FLASH_DATA_Get_String
};

const Command_t rail_address_set =
{
	.command = "set enable rail address",
	.args = "set enable rail address:1 | set enable rail address:0",
	.func = LTM46xx_SetMFR_RailAddressFlag
};

const Command_t pm_vout_command_set =
{
	.command = "set pm vout command",
	.args = "set pm vout command:0,<mV>",
	.notes = "Be sure to <set enable rail address:0>",
	.func = LTM46xx_SetPwrMod
};

/*
 * Description: List of commands
 */
Command_t Command_List[] =
{
	help,
	version,
	bootloader,
	serial_get,
	part_get,
	rail_address_set,
	pm_vout_command_set
};

/*
 * Description: This will return all the functions created with the command name and arguments.
 * Input: msg arg can be NULL or 1. retStr not used, pass NULL.
 * Output: None
 * Return: NO_ACK
 * Note: Need snprintf instead
 */
int Print_Functions(char* msg, char *retStr)
{
	int array_size = sizeof(Command_List);
	int element_size = sizeof(Command_List[0]);
	int num_elements = array_size/element_size;
	char str[UART_DMA_QUEUE_DATA_SIZE] = {0};
	int puttyIsColor = false;
	char description[] = "<command> [args] [notes]";

	if(atoi(msg) == 1)
	{
		puttyIsColor = 1;
	}

	UART_DMA_NotifyUser(&uart2_msg, description, strlen(description), true);

	for (int i = 0; i < num_elements; i++)
	{
		if(puttyIsColor)
		{
			snprintf(str, UART_DMA_QUEUE_DATA_SIZE, "\033[0;36m %s \033[0m [%s] [%s]", Command_List[i].command, Command_List[i].args, Command_List[i].notes);
		}
		else
		{
			
			snprintf(str, UART_DMA_QUEUE_DATA_SIZE, "%s [%s] [%s]", Command_List[i].command, Command_List[i].args, Command_List[i].notes);
		}
		UART_DMA_NotifyUser(&uart2_msg, str, strlen(str), true);
	}

	return NO_ACK;
}



