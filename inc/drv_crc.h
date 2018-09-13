/*
 * drv_crc.h
 *
 *  Created on: Sep 10, 2018
 *      Author: abel
 */

#ifndef DRV_CRC_H_
#define DRV_CRC_H_

#include <stm32l4xx_ll_crc.h>
#include <stm32l4xx_hal_crc_ex.h>
#include <stm32l4xx_hal_crc.h>
#include <stdint.h>

void init_CRC_module(void);
uint32_t get_CRC32_result(uint8_t*, uint8_t);

#endif /* DRV_CRC_H_ */
