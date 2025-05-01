/*
 * CommandList_Drv.c
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


//*********** USER UPDATABLE SECTION *************

/*
 * Description: Instances of Command functions with information and pointer to a function to be called.
 * 				Each args string should have \r\n except for the last one string.
 * 				Each additional args string after the first string should have a single tab.
 * 				Try to keep args and notes as short as possible.
 *
 */

const Command_t help =
{
	.command = "help",
	.args = "help\r\n"
			"\thelp 1",
	.notes = NULL,
	.func = Command_List_Print
};

const Command_t version =
{
	.command = "version",
	.args = NULL,
	.notes = NULL,
	.func = Version,
};

const Command_t pm_vout_command =
{
	.command = "pm vout command",
	.args = "pm vout command ? <page>\r\n"
			"\tpm vout command 0,<mV value>",
	.notes = "When using ?, Enable Rail Address. Else, Disable Rail Address",
	.func = Vout_Command
};

const Command_t pm_read_vout =
{
	.command = "pm read vout",
	.args = "pm read vout ? <page>",
	.notes = NULL,
	.func = NULL
};

const Command_t i2c_address =
{
	.command = "i2c address",
	.args = "i2c address ?\r\n"
			"\ti2c address 1,<rail addr>,<slave addr #1>,<slave addr #2>\r\n"
			"\ti2c address 2,<ra>,<sa #1>,<sa #2><sa #3>,<sa #4>\r\n"
			"\ti2c address 3,<ra>,<sa #1>,<sa #2><sa #3>,<sa #4>,<sa #5>,<sa #6>",
	.notes = NULL,
	.func = I2C_Address
};

const Command_t pm_status =
{
	.command = "pm status",
	.args = "pm status word\r\n"
			"\tpm status byte",
	.notes = NULL,
	.func = NULL
};

const Command_t serial =
{
	.command = "serial",
	.args = "serial ?\r\n"
			"\tserial <string>",
	.notes = NULL,
	.func = NULL
};

const Command_t rail_address =
{
	.command = "rail address",
	.args = "rail address ?\r\n"
			"\trail address en <1 or 0>",
	.notes = NULL,
	.func = NULL
};

const Command_t pm_store_user_all =
{
	.command = "pm store user all",
	.args = NULL,
	.notes = NULL,
	.func = NULL
};

const Command_t telemetry =
{
	.command = "telemetry",
	.args = "telemtry ?\r\n"
			"\ttelemtry en 1 <time in ms>\r\n"
			"\ttelemtry en 0",
	.notes = NULL,
	.func = TelemetryEn
};

const Command_t bootloader =
{
	.command = "bootloader",
	.args = NULL,
	.notes = NULL,
	.func = Bootloader
};

const Command_t adc =
{
	.command = "adc",
	.args = "adc op sns i\r\n"
			"\tadc ltm4681 sns v\r\n"
			"\tadc op sns register voltage",
	.notes = NULL,
	.func = NULL
};

const Command_t freq =
{
	.command = "freq",
	.args = "freq ?\r\n"
			"\tfreq 1, <time in ms>\r\n"
			"\tfreq 0",
	.notes = NULL,
	.func = Frequency,
};


/*
 * Description: User shall add each instances of commands to array.
 */
const Command_t Command_List[] =
{
	help,
	adc,
	bootloader,
	freq,
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

//****** END USER UPDATABLE SECTION *******


//****** DO NOT MODIFY BELOW **************

int cmd_list_poll_mode = 0;
int puttyIsColor = false;

/*
 * Description: User shall call this function in a timed fashion. Every 10ms should suffice.
 * 				Using timed trigger will reduce the uC from being overloaded during printing of all available commands
 * 				and reduce blocking.
 */
void Command_List_Poll(void)
{
	int array_size = sizeof(Command_List);
	int element_size = sizeof(Command_List[0]);
	int num_elements = array_size/element_size;
	char arguments[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};// so fits in str variable
	char notes[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};
	static int cmdPtr = 0;
	char str2[UART_DMA_QUEUE_DATA_SIZE / 2] = {0};
	CommandData_t cData = {0};

	if(cmd_list_poll_mode)
	{
		if(Command_List[cmdPtr].args != NULL)
		{
			sprintf(arguments, "%s", Command_List[cmdPtr].args);
		}
		if(Command_List[cmdPtr].notes != NULL)
		{
			sprintf(notes, "[x] %s", Command_List[cmdPtr].notes);
		}
		if(puttyIsColor)
		{
			snprintf((char*)cData.data, UART_DMA_QUEUE_DATA_SIZE / 2, "\033[0;36m > %s \033[0m\r\n", Command_List[cmdPtr].command);
		}
		else
		{
			snprintf((char*)cData.data, UART_DMA_QUEUE_DATA_SIZE / 2, "> %s\r\n", Command_List[cmdPtr].command);
		}

		if(arguments[0] == '\0' && notes[0] == '\0')
		{
			sprintf(str2, " ");
		}
		else if(notes[0] == '\0')
		{
			snprintf(str2, UART_DMA_QUEUE_DATA_SIZE / 2, "\t%s", arguments[0] == '\0' ? "":arguments);
		}
		else
		{
			snprintf(str2, UART_DMA_QUEUE_DATA_SIZE / 2, "\t%s\r\n \t%s", arguments[0] == '\0' ? "":arguments , notes[0] == '\0' ? "":notes);
		}

		strcat((char*)cData.data, str2);

		Command_ListNotify(&cData); // see notes at bottom of this file

		if(++cmdPtr == num_elements)
		{
			cmd_list_poll_mode = 0; // done
			cmdPtr = 0; // reset
			sprintf((char*)cData.data, "***** END *****");
			Command_ListNotify(&cData);
		}
	}
}


/*
 * Description: This will start to return all the functions created with the command name and arguments.
 * Input: msg arg can be NULL or 1. retStr not used, leave alone so it'll return OK.
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

	return NO_ERROR;
}


/*
 * User will need to create a function (Command_ListNotify) to receive CommandData_t data structure.
 * This is done so it's a generic function call outside of this file. But inside the function,
 * the actual uC UART transmit routine is used. So that way Command_List_Poll above does not need to be modified.
 */
/*
int Command_ListNotify(CommandData_t *msg)
{
	int status = NO_ERROR;

	// User will need to send msg using the uC UART transmit driver.

	return status;
}
*/

