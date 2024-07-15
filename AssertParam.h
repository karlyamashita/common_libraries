/*
 * AssertParam.h
 *
 *  Created on: Jul 15, 2024
 *      Author: karl.yamashita
 */

#ifndef ASSERTPARAM_H_
#define ASSERTPARAM_H_


#if !defined(UNUSED)
#define UNUSED(X) (void)X         /* To avoid gcc/g++ warnings */
#endif /* UNUSED */

// User should use this call which will call assert_failed if not zero.
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))

__weak void assert_failed(uint8_t *file, uint32_t line);


#endif /* ASSERTPARAM_H_ */
