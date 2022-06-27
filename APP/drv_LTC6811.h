/*
 * @Descripttion: 
 * @version: 
 * @Author: lxl
 * @Date: 2021-11-18 11:27:10
 * @LastEditTime: 2021-12-01 14:36:23
 */
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-18     lxl       the first version
 */



#ifndef _DRV_LTC6811_H
#define _DRV_LTC6811_H

#include "stdint.h"





#define BMS_NORMAL		0X00
#define BMS_ERROR		0X01




typedef struct {
	uint16_t cell1;
	uint16_t cell2;
	uint16_t cell3;
}CELLV;





uint8_t LTC6811_get_total_voltage(uint16_t *ptv);
uint8_t LTC6811_Get_PerCell_Voltage(uint16_t* p);
uint8_t LTC6811_ActiveBallancing(uint8_t ltc6811_addr,uint8_t CellNumber);
void LTC6811_Get_GPIO1_GPIO3(uint16_t *vol);
void LTC6811_Get_GPIO4_VREF2(uint16_t *vol);





#endif
