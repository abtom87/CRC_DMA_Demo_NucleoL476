/*
 * drv_trng.h
 *
 *  Created on: Sep 9, 2018
 *      Author: abel
 */

#ifndef DRV_TRNG_H_
#define DRV_TRNG_H_

#include <stm32l4xx_ll_rng.h>

#include <stdint.h>


#define USE_POLLING
//#define USE_INTERRUPT

//#define U32 __UINT32_TYPE__
void init_TRNG_module(void);
void init_TRNG_NVIC(void);
uint32_t Get_TRNG(void);


#endif /* DRV_TRNG_H_ */
