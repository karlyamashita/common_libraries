/*
 * Command_List.c
 *
 *  Created on: Apr 10, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Description: Command functions with information and pointer to a function to be called.
 * 				Each args string should have \r\n except for the last one string.
 * 				Each additional args string after the first string should have a single tab.
 *
 */

const Command_t help =
{
	.command = "help",
	.args = "help\r\n"
			"\thelp 1",
	.notes = NULL,
	.func_g = Command_List_Print,
	.func_s = NULL
};

const Command_t version =
{
	.command = "version",
	.args = NULL,
	.notes = NULL,
	.func_g = Version,
	.func_s = NULL
};

const Command_t pm_vout_command =
{
	.command = "pm vout command",
	.args = "pm vout command set:0,<mV value>\r\n"
			"\tpm vout command get:<page>",
	.notes = "When using \'set\', Enable Rail Address. When using \'get\', Disable Rail Address",
	.func_s = Vout_Command_Set,
	.func_g = Vout_Command_Get
};

const Command_t pm_read_vout =
{
	.command = "pm read vout",
	.args = "pm read vout:0",
	.notes = NULL,
	.func_g = NULL
};

const Command_t i2c_address =
{
	.command = "i2c address",
	.args = "i2c address set:<ltm count>,<rail addr>,<slave addr #1>,<slave addr #2><#3><#4><#5><#6>\r\n"
			"\ti2c address get",
	.notes = NULL,
	.func_g = I2C_Address_Get,
	.func_s = I2C_Address_Set
};

const Command_t pm_status =
{
	.command = "pm status",
	.args = "pm status word\r\n"
			"\tpm status byte",
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t serial =
{
	.command = "serial",
	.args = "serial set:<string>\r\n"
			"\tserial get",
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t rail_address =
{
	.command = "rail address",
	.args = "rail address en set:<1 or 0>\r\n"
			"\trail address en get",
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t pm_store_user_all =
{
	.command = "pm store user all",
	.args = NULL,
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t telemetry =
{
	.command = "telemetry",
	.args = "telemtry en:<1 or 0>\r\n"
			"\ttelemtry send rate:<value in ms>",
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t bootloader =
{
	.command = "bootloader",
	.args = NULL,
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

const Command_t adc =
{
	.command = "adc",
	.args = "adc op sns i\r\n"
			"\tadc ltm4681 sns v\r\n"
			"\tadc op sns register voltage",
	.notes = NULL,
	.func_g = NULL,
	.func_s = NULL
};

/*
 * Description: List of instances of commands.
 */
const Command_t Command_List[] =
{
	help,
	adc,
	bootloader,
	i2c_address,
	pm_status,
	pm_store_user_all,
	pm_read_vout,
	pm_vout_command,
	rail_address,
	serial,
	telemetry,

	version
};

int cmd_list_poll_mode = 0;
int puttyIsColor = false;

/*
 * Description: Poll to print the commands in non blocking mode.
 */
void Command_List_Poll(void)
{
	int array_size = sizeof(Command_List);
	int element_size = sizeof(Command_List[0]);
	int num_elements = array_size/element_size;
	char str[UART_DMA_QUEUE_DATA_SIZE] = {0};
	char arguments[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};// so fits in str variable
	char notes[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};
	static int cmdPtr = 0;

	if(cmd_list_poll_mode)
	{
		if(Command_List[cmdPtr].args != NULL)
		{
			sprintf(arguments, "%s", Command_List[cmdPtr].args);
		}
		else
		{
			sprintf(arguments, " ");
		}
		if(Command_List[cmdPtr].notes != NULL)
		{
			sprintf(notes, "[x] %s", Command_List[cmdPtr].notes);
		}
		else
		{
			sprintf(notes, " ");
		}
		if(puttyIsColor)
		{
			snprintf(str, UART_DMA_QUEUE_DATA_SIZE, "\033[0;36m > %s \033[0m %s %s", Command_List[cmdPtr].command, Command_List[cmdPtr].args, Command_List[cmdPtr].notes);
		}
		else
		{
			snprintf(str, UART_DMA_QUEUE_DATA_SIZE, "> %s\r\n \t%s\r\n \t%s", Command_List[cmdPtr].command, arguments, notes);
		}

		UART_DMA_NotifyUser(&uart2_msg, str, strlen(str), true);

		if(++cmdPtr == num_elements)
		{
			cmd_list_poll_mode = 0; // done
			cmdPtr = 0; // reset
		}
	}
}


/*
 * Description: This will start to return all the functions created with the command name and arguments.
 * Input: msg arg can be NULL or 1. retStr not used, pass NULL.
 * Output: None
 *
 */
int Command_List_Print(char* msg, char *retStr)
{
	if(atoi(msg) == 1)
	{
		puttyIsColor = 1;
	}
	else
	{
		puttyIsColor = 0;
	}

	cmd_list_poll_mode = 1; // start

	UART_DMA_NotifyUser(&uart2_msg, "", strlen(""), true);

	return NO_ERROR;
}



