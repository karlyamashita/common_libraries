/*
 * TM4C12x.h
 *
 *  Created on: Jun 14, 2024
 *      Author: karl.yamashita
 */

#ifndef TM4C12X_H_
#define TM4C12X_H_


typedef enum
{
    TM4C_OK       = 0x00U,
    TM4C_ERROR    = 0x01U,
    TM4C_BUSY     = 0x02U,
    TM4C_TIMEOUT  = 0x03U
} TM4C_StatusTypeDef;

typedef enum
{
    TM4C_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
    TM4C_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
    TM4C_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
    TM4C_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
    TM4C_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
    TM4C_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
    TM4C_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                 process is ongoing                         */
    TM4C_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                 process is ongoing                         */
    TM4C_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */

} TM4C_I2C_StateTypeDef;

typedef enum
{
  TM4C_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
  TM4C_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  TM4C_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
  TM4C_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */

} TM4C_I2C_ModeTypeDef;

typedef enum
{
  TM4C_UNLOCKED = 0x00U,
  TM4C_LOCKED   = 0x01U
} TM4C_LockTypeDef;

typedef struct
{
    // I2C Master
    volatile uint32_t I2CMSA;         /*!< I2C Master Slave Address, Address offset: 0x00 */
    volatile uint32_t I2CMCS;         /*!< I2C Master Control/Status, Address offset: 0x04 */
    volatile uint32_t I2CMDR;         /*!< I2C Master Data, Address offset: 0x08 */
    volatile uint32_t I2CMTPR;        /*!< I2C Master Timer Period, Address offset: 0x0C */
    volatile uint32_t I2CMIMR;        /*!< I2C Master Interrupt Mask, Address offset: 0x10 */
    volatile uint32_t I2CMRIS;        /*!< I2C Master Raw Interrupt Status, Address offset: 0x14 */
    volatile uint32_t I2CMMIS;        /*!< I2C Master Masked Interrupt Status, Address offset: 0x18 */
    volatile uint32_t I2CMICR;        /*!< I2C Master Interrupt Clear, Address offset: 0x1C */
    volatile uint32_t I2CMCR;         /*!< I2C Master Configuration, Address offset: 0x20 */
    volatile uint32_t I2CMCLKOCNT;    /*!< I2C Master Clock Low Timeout Count, Address offset: 0x24 */
    volatile uint32_t I2CMBMON;       /*!< I2C Master Bus Monitor, Address offset: 0x2C */
    volatile uint32_t I2CMCR2;        /*!< I2C Master Configuration 2, Address offset: 0x38 */
    // I2C Slave
    volatile uint32_t I2CSOAR;        /*!< I2C Slave Own Address, Address offset: 0x800 */
    volatile uint32_t I2CSCSR;         /*!< I2C Slave Control/Status, Address offset: 0x804 */
    volatile uint32_t I2CSDR;    /*!< I2C Slave Data, Address offset: 0x808 */
    volatile uint32_t I2CSIMR;       /*!< I2C Slave Interrupt Mask, Address offset: 0x80C */
    volatile uint32_t I2CSRIS;        /*!< I2C Slave Raw Interrupt Status, Address offset: 0x810 */
    volatile uint32_t I2CSMIS;         /*!< I2C Slave Masked Interrupt Status, Address offset: 0x814 */
    volatile uint32_t I2CSICR;    /*!< I2C Slave Interrupt Clear, Address offset: 0x818 */
    volatile uint32_t I2CSOAR2;       /*!< I2C Slave Own Address 2, Address offset: 0x81C */
    volatile uint32_t I2CSACKCTL;        /*!< I2C Slave ACK Control, Address offset: 0x820 */
    // I2C Status and Control
    volatile uint32_t I2CPP;       /*!< I2C Peripheral Properties, Address offset: 0xFC0 */
    volatile uint32_t I2CPC;        /*!< I2C Peripheral Configuration, Address offset: 0xFC4 */

} I2C_TypeDef;

#define TM4C_I2C_ERROR_NONE      (0x00000000U)    /*!< No error              */
#define TM4C_I2C_ERROR_BERR      (0x00000001U)    /*!< BERR error            */
#define TM4C_I2C_ERROR_ARLO      (0x00000002U)    /*!< ARLO error            */
#define TM4C_I2C_ERROR_AF        (0x00000004U)    /*!< ACKF error            */
#define TM4C_I2C_ERROR_OVR       (0x00000008U)    /*!< OVR error             */
#define TM4C_I2C_ERROR_DMA       (0x00000010U)    /*!< DMA transfer error    */
#define TM4C_I2C_ERROR_TIMEOUT   (0x00000020U)    /*!< Timeout error         */
#define TM4C_I2C_ERROR_SIZE      (0x00000040U)    /*!< Size Management error */
#define TM4C_I2C_ERROR_DMA_PARAM (0x00000080U)    /*!< DMA Parameter Error   */
#if (USE_TM4C_I2C_REGISTER_CALLBACKS == 1)
#define TM4C_I2C_ERROR_INVALID_CALLBACK  (0x00000100U)    /*!< Invalid Callback error */
#endif /* USE_TM4C_I2C_REGISTER_CALLBACKS */
#define TM4C_I2C_ERROR_INVALID_PARAM     (0x00000200U)    /*!< Invalid Parameters error  */


/** @defgroup I2C_MEMORY_ADDRESS_SIZE I2C Memory Address Size
  * @{
  */
#define I2C_MEMADD_SIZE_8BIT            (0x00000001U)
#define I2C_MEMADD_SIZE_16BIT           (0x00000002U)



#define I2C_MEM_ADD_MSB(__ADDRESS__)              ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & \
                                                                         (uint16_t)(0xFF00U))) >> 8U)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)              ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)(0x00FFU))))


#define READ_REG(REG)         ((REG))


#define __TM4C_LOCK(__HANDLE__)             \
  do{                                      \
    if((__HANDLE__)->Lock == TM4C_LOCKED)   \
    {                                      \
      return TM4C_BUSY;                     \
    }                                      \
    else                                   \
    {                                      \
      (__HANDLE__)->Lock = TM4C_LOCKED;     \
    }                                      \
  }while (0U)

#define __TM4C_UNLOCK(__HANDLE__)           \
  do{                                      \
    (__HANDLE__)->Lock = TM4C_UNLOCKED;     \
  }while (0U)


#endif /* TM4C12X_H_ */
