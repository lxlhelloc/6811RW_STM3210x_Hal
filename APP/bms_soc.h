
/*
 * bms_soc.h
 *
 *  Created on: 2021年12月10日
 *      Author: lxl
 */

#ifndef BMS_SOC_H_
#define BMS_SOC_H_

#include "stdint.h"

//电池总容量  单位为AH
#define BatTotalCap 30

float Get_Init_Soc(float cellvol);
float Get_AHcal_Soc(uint8_t chargeover,float* charged,float* discharged);
float Get_VolCal_Soc(float cellvol);

#endif /* BMS_SOC_H_ */
