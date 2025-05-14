/*
 * CommandList_Drv.c
 *
 *  Created on: Apr 30, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


//****** DO NOT MODIFY BELOW **************

int cmd_list_poll_run = 0;
int puttyIsColor = false; // if working with putty or MobaXterm, we can print text in color
int colorIs = 37; // default to white

/*
 * Description: User shall call this function in a timed fashion. Every 10ms should suffice.
 * 				Using timed trigger will reduce the uC from being overloaded during printing of all available commands
 * 				and reduce blocking.
 */
void Command_List_Poll(void)
{
	//int array_size = sizeof(Command_List); // defined in CommandList_Handler as extern
	int element_size = sizeof(Command_List[0]);
	int num_elements = CommandList_array_size/element_size;
	char arguments[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};// so fits in str variable
	char notes[(UART_DMA_QUEUE_DATA_SIZE/2) - 1] = {0};
	static int cmdPtr = 0;
	char strColor[16] = {0};
	char command[UART_DMA_QUEUE_DATA_SIZE] = {0};
	char str2[UART_DMA_QUEUE_DATA_SIZE] = {0};

	if(cmd_list_poll_run)
	{
		sprintf(strColor, "\033[0;%dm", colorIs);

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
			sprintf(command, "%s> %s %s\r\n", strColor, Command_List[cmdPtr].command, END_COLOR);
		}
		else
		{
			sprintf(command, "> %s\r\n", Command_List[cmdPtr].command);
		}

		if(arguments[0] == '\0' && notes[0] == '\0')
		{
			sprintf(str2, " ");
		}
		else if(notes[0] == '\0')
		{
			sprintf(str2, "\t%s", arguments[0] == '\0' ? "":arguments);
		}
		else
		{
			snprintf(str2, strlen(arguments) + strlen(notes), "\t%s\r\n \t%s", arguments[0] == '\0' ? "":arguments , notes[0] == '\0' ? "":notes);
		}

		strcat(command, str2);

		Command_ListNotify(command); // see notes at bottom of this file

		if(++cmdPtr == num_elements)
		{
			cmd_list_poll_run = 0; // done
			cmdPtr = 0; // reset
			sprintf(str2, "***** END *****"); // print END
			Command_ListNotify(str2);
		}
	}
}


/*
 * Description: This should be called when "help" is received. This will start to return all the command functions created.
 * 				The 2nd argument is the character representing the color to use for the command name. Good for MobaXterm.
 *
 * Input: msg arg can be NULL or 1. retStr not used, leave alone so it'll return OK.
 * Output: None
 *
 */
int Command_List_Print(char* msg, char *retStr)
{
	char *token;
	char *rest = msg;
	char delim[] = ": ,|\r";

	token = strtok_r(rest, delim, &rest);

	if(token != NULL)
	{
		switch(token[0])
		{
		case 'k': // black
			colorIs = 30;
			break;
		case 'r': // red
			colorIs = 31;
			break;
		case 'g': // green
			colorIs = 32;
			break;
		case 'y': // yellow
			colorIs = 33;
			break;
		case 'b': // blue
			colorIs = 34;
			break;
		case 'm': // magenta
			colorIs = 35;
			break;
		case 'c': // cyan
			colorIs = 36;
			break;
		case 'w': // white
			colorIs = 37;
			break;
		default : // white
			colorIs = 37;
			break;
		}
		if(colorIs != 37)
		{
			puttyIsColor = 1;
		}
		else
		{
			puttyIsColor = 0;
		}
	}

	cmd_list_poll_run = 1; // start

	return NO_ERROR;
}


/*
 * User will need to create a function (Command_ListNotify) to receive CommandData_t data structure.
 * This is done so it's a generic function call outside of this file. But inside the function,
 * the actual uC UART transmit routine is used. So that way inside of Command_List_Poll,
 * Command_ListNotify function call does not need to be modified for other types of microcontroller's UART driver.
 */
/*
int Command_ListNotify(char *msg)
{
	int status = NO_ERROR;

	// User will need to send msg using the specific uC UART transmit driver.

	return status;
}
*/

