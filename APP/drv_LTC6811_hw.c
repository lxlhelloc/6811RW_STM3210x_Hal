/*
 * @Descripttion: 
 * @version: 
 * @Author: lxl
 * @Date: 2021-11-18 11:00:43
 * @LastEditTime: 2021-11-24 14:26:10
 */

#include "drv_LTC6811_hw.h"
#include "stdint.h"

#ifdef STM32_HAL_LIBRARY
#include "stm32f1xx_hal_spi.h"
//HAl库
void spi_write_array(uint8_t len,uint8_t data[])
{

	HAL_SPI_Transmit(&hspi1,data,len,50);

}

void spi_write_read(uint8_t tx_Data[],uint8_t tx_len,uint8_t* re_data,uint8_t rx_len)
{
	HAL_SPI_Transmit(&hspi1,tx_Data,tx_len,50);
	HAL_SPI_Receive(&hspi1,re_data,rx_len,50);
}

#else
// 标准库
void spi_write_array(uint8_t len, 
                     uint8_t data[])
{
    uint8_t i; 
	//DASILY_RESET();
    for ( i = 0; i < len; i++ )
    {
        SPI1_SendByte(data[i]); 
    }
	//DASILY_SET();
}

void spi_write_read(uint8_t  tx_Data[],
                    uint8_t  tx_len, 
                    uint8_t* rx_data, 
                    uint8_t  rx_len)
{
    uint8_t i;
 //   uint8_t data;
    for ( i = 0; i < tx_len; i++ )
    {
        // Transmit byte.
				SPI1_SendByte(tx_Data[i]);
    }
    for ( i = 0; i < rx_len; i++ )
    {
        // Receive byte.
        rx_data[i]=SPI1_SendByte(0xff);
    } 
}

#endif







