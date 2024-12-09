/* 
 * File:   LTM46xx_Handler.h
 * Author: karl.yamashita
 *
 * Created on January 31, 2023, 5:00 PM
 */

#ifndef LTM46XX_HANDLER_H
#define	LTM46XX_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define LTM46xx_I2C_SLAVE_ADDRESS_FLASH_ADDRESS 0x14100 
#define LTM46xx_I2C_SLAVE_ADDRESS_A 0x14100
#define LTM46xx_I2C_SLAVE_ADDRESS_B 0x14104
#define LTM46xx_I2C_RAIL_ADDRESS 0x14108    
    
void LTM46xx_Init(void);
//int LTM46xx_AddrInit(LTM_SlaveRail_t *addr);
int LTM46xx_GetPwrMod(char *msg, char *retStr);
int LTM46xx_SetPwrMod(char *msg);

#ifdef	__cplusplus
}
#endif

#endif	/* LTM46XX_HANDLER_H */

