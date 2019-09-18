

#ifndef SWC_H
#define SWC_H

void ServiceSWC(uint8_t *data);
uint8_t GetSWC_ButtonStatus(void);


/*
function: sends command to radio.
input: the SWC command
output: none
*/
void SendSWC_Command_CAN1(uint8_t command);


#endif // SWC_H
