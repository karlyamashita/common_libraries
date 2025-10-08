/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

/*
 * STM32_UID.h
 *
 *  Created on: Sep 3, 2025
 *      Author: codek
 */

#ifndef INC_STM32_UID_H_
#define INC_STM32_UID_H_

#define STM32_UID_DATA_SIZE 12

typedef union
{
	struct
	{
		uint64_t low;
		uint32_t high;
	};
	struct
	{
		uint32_t uid_0;
		uint32_t uid_1;
		uint32_t uid_2;
	}UID_t;
}STM32_UID_t;

int STM32_UID_Get(char *retStr);
void STM32_UID(STM32_UID_t *msg);

#endif /* INC_STM32_UID_H_ */
