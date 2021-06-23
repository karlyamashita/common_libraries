/*
 * HAL_Status.h
 *
 *  Created on: Mar 8, 2021
 *      Author: karl.yamashita
 */

#ifndef HAL_STATUS_H_
#define HAL_STATUS_H_


#define HAL_STATUS_DEFINED

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U,
} HAL_StatusTypeDef;



#endif /* HAL_STATUS_H_ */
