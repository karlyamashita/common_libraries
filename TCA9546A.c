#include "main.h"
#include "TCA9546A.h"


/*
 * Description: Write to TCA9546A the channel to select 
 * Input: channel number to select on TCA9546A
 * Output: none
 */
i2c_error_t TCA9546A_SetChannel(uint8_t channel)
{
    return I2C_0_writeNBytes(TCA9546A_ADDRESS, &channel, 1);
}

/*
 * Description: Query TCA9546A for which channel is selected
 * Input: none
 * Output: Channel number
*/
i2c_error_t TCA9546A_GetSelectedChannel(uint8_t *data)
{
	return I2C_0_readNBytes(TCA9546A_ADDRESS,  data, 1);
}
