/*
 * drv_trng.c
 *
 *  Created on: Sep 9, 2018
 *      Author: abel
 */


#include <drv_trng.h>


void init_TRNG_module(void)
{
	__HAL_RCC_RNG_CLK_ENABLE();
	RNG->CR =0x00000000;

	LL_RNG_Enable(RNG);
#ifdef USE_INTERRUPT
	LL_RNG_EnableIT(RNG);
#endif
	LL_RNG_ClearFlag_CEIS(RNG);
	LL_RNG_ClearFlag_SEIS(RNG);

}


void init_TRNG_NVIC(void)
{
	NVIC_EnableIRQ(RNG_IRQn);
	NVIC_SetPriority(RNG_IRQn,0);
	NVIC_ClearPendingIRQ(RNG_IRQn);
}




uint32_t Get_TRNG(void)
{
	uint32_t trng_val=0;
		while(SET ==LL_RNG_IsActiveFlag_CECS(RNG) ){
		}


		while( RESET==LL_RNG_IsActiveFlag_DRDY(RNG)){
		}
		trng_val=LL_RNG_ReadRandData32(RNG);

		return trng_val;


}


