/*
 * drv_dma1.c
 *
 *  Created on: Sep 12, 2018
 *      Author: abel
 */

#include <drv_dma1.h>

#define ARR_LEN 8
uint8_t int8_buffer[ARR_LEN] =
		{ 0xFF, 0x01, 0xF2, 0x03, 0x04, 0x05, 0x06, 0x07 };


void init_DMA1(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE()
	;
	LL_DMA_InitTypeDef DMA_InitStruct;

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress  = (uint32_t) &CRC->DR;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.Mode                   = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.NbData                 = ARR_LEN;
	DMA_InitStruct.PeriphOrM2MSrcAddress  = &(int8_buffer[0]);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_INCREMENT;
	DMA_InitStruct.PeriphRequest          = LL_DMA_REQUEST_1; /*Stream1*/
	DMA_InitStruct.Priority               = LL_DMA_PRIORITY_HIGH;

	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitStruct);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_ClearFlag_GI1(DMA1);
	LL_DMA_ClearFlag_HT1(DMA1);
	LL_DMA_ClearFlag_TC1(DMA1);

}

void init_DMA1_NVIC(void)
{
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
	NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);

}

