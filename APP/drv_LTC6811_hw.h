/*
 * @Descripttion: 
 * @version: 
 * @Author: lxl
 * @Date: 2021-11-18 11:01:36
 * @LastEditTime: 2021-11-25 15:23:57
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
#ifndef APPLICATIONS_DRIVERS_DRV_LTC6811_HW_H_
#define APPLICATIONS_DRIVERS_DRV_LTC6811_HW_H_


#ifndef RT_THREAD
#define RT_THREAD 0
#endif

#ifndef STM32_HAL_LIBRARY
#define STM32_HAL_LIBRARY
#endif

#ifdef STM32_HAL_LIBRARY
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

extern SPI_HandleTypeDef hspi1;

#define DASILY_RESET()  HAL_GPIO_WritePin(LTC68_SPI_PORT,LTC68_SPI_CS_PIN,GPIO_PIN_RESET)
#define DASILY_SET()    HAL_GPIO_WritePin(LTC68_SPI_PORT,LTC68_SPI_CS_PIN,GPIO_PIN_SET)

#define LTC68_SPI_SendByte(x) HAL_SPI_Transmit(&hspi1,(uint8_t*)&x,1,50)
void spi_write_array(uint8_t len,uint8_t data[]);
void spi_write_read(uint8_t tx_Data[],uint8_t tx_len,uint8_t* re_data,uint8_t rx_len);

#else
//标准库

#endif

#if RT_THREAD
#include <rtthread.h>

#define LTC68_Delay(x)  rt_thread_mdelay(x)

#else
    #ifdef STM32_HAL_LIBRARY
#define LTC68_Delay(x)  HAL_Delay(x)
    #else
#define LTC68_Delay(x) delay_ms(x) 
    #endif  
#endif

#define LTC68_SPI_PORT GPIOA
#define LTC68_SPI_CS_PIN GPIO_PIN_4


#endif /* APPLICATIONS_DRIVERS_DRV_LTC6811_HW_H_ */
