/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include <stm32l4xx_ll_rcc.h>
#include <drv_gpio_led.h>
#include "stm32l4xx.h"

#include <drv_trng.h>
#include <drv_usart3.h>
#include <string.h>
#include <drv_crc.h>
#include <drv_dma1.h>

//#define CRC_WITHOUT_DMA

volatile unsigned long msTicks;
void setCrystalClock(uint8_t U8_clk_inMHz);

void print_out(char *, uint32_t);

void Delay_ms(unsigned long del_count)
{
	del_count *= 4;
	msTicks = del_count;
	while (msTicks != 0)
		;
}

void SysTick_Init(void)
{
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	/* Tick every 1 ms*/
	while (SysTick_Config(SystemCoreClock / (1000U)) != 0)
	{
	}

}
void setCrystalClock(uint8_t U8_clk_inMHz)
{
	LL_RCC_MSI_SetCalibTrimming(255);

	uint8_t clk_freq = U8_clk_inMHz;
	switch (clk_freq)
	{

	case 8:

		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_7);
		break;

	case 16:

		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_8);
		break;

	case 24:

		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(RCC_CR_MSIRANGE_9);
		break;

	case 32:

		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(RCC_CR_MSIRANGE_10);
		break;

	case 48:

		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(RCC_CR_MSIRANGE_11);
		break;

	default:
		LL_RCC_MSI_EnableRangeSelection();
		LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
	}

}

#define DATALEN 6
void print_out(char *buffer, uint32_t arg)
{
	char buff[100] = { 0 };
	uint32_t crc_result = 0;

	USART_Tx_string(buffer);
	sprintf(buff, ": %u\n\r", arg);
	USART_Tx_string(buff);

}
volatile uint32_t TRNG_VAl;
volatile uint16_t DMA_CRC_Val;
int8_t DMA_done = 0;

uint16_t ABC;
int main(void)
{
#ifdef CRC_WITHOUT_DMA
	uint32_t crc_result=0;
#endif

	SystemInit();

	SysTick_Init();
	setCrystalClock(16); /*Set System Clock to 16MHz */

	init_led_gpio();

	/* Init USART for debugging */
	init_usart_gpio();
	init_usart_peripheral();

	/* Set clock for Entropy for RNG to generate Random number */
	LL_RCC_SetRNGClockSource(LL_RCC_RNG_CLKSOURCE_MSI);
	init_TRNG_module();

#ifdef USE_POLLING
	TRNG_VAl = Get_TRNG();
#endif
	print_out("TRNG Val", TRNG_VAl);

	/* Initialize CRC Module*/
	init_CRC_module();
#ifdef CRC_WITHOUT_DMA
	crc_result = get_CRC32_result(arr_data,DATALEN);
	print_out("CRC_Result", crc_result);
#else
	init_DMA1();
	init_DMA1_NVIC();
#endif

	while (1)
	{

		if (TRNG_VAl != 0)
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		if (DMA_done == 1)
		{

			print_out("DMA CRC", ABC);
			DMA_done = 0;

		}

		Delay_ms(500);
	}

}

void SysTick_Handler(void)
{ /* SysTick interrupt Handler. */

	if (msTicks != 0)
		msTicks--; /* See startup file startup_LPC17xx.s for SysTick vector */
}

void DMA1_CH1_IRQHandler(void)
{

	if (SET == LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		DMA_CRC_Val = (uint32_t) CRC->DR;
		memcpy(&ABC, &DMA_CRC_Val, 2);
		ABC ^= 0xFFFF;
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_DMA_ClearFlag_GI1(DMA1);
		LL_DMA_ClearFlag_HT1(DMA1);
		LL_DMA_ClearFlag_TC1(DMA1);
		LL_DMA_ClearFlag_TE1(DMA1);
		DMA_done = 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);

		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

	}

}
