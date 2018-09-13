/*
 * drv_crc.c
 *
 *  Created on: Sep 10, 2018
 *      Author: abel
 */

#include <drv_crc.h>

void init_CRC_module(void)
{
	//CRC->CR = 0x00000001;
	LL_CRC_ResetCRCCalculationUnit(CRC);

	__HAL_RCC_CRC_CLK_ENABLE()
	;
	LL_CRC_SetInitialData(CRC, 0xFFFF);

	LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_16B);
	LL_CRC_SetPolynomialCoef(CRC, (uint32_t) 0x1021); //0x1EDC6F41
	LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_BYTE);
	LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_BIT);
}


uint32_t get_CRC32_result(uint8_t *ptr_data_buff, uint8_t size)
{
	uint32_t temp = 0;
	uint8_t i;

	for (i = 0; i < size; i++)
	{
		LL_CRC_FeedData8(CRC, *(ptr_data_buff + i));
		temp = LL_CRC_ReadData32(CRC);
	}

	temp ^= 0xFFFFFFFF;

	return temp;

}
