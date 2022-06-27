/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-18     lxl       the first version
 */

#include "drv_LTC6811.h"
#include "drv_LTC6811_proto.h"
#include "drv_LTC6811_hw.h"
#include "stdint.h"
#include "stdio.h"

uint8_t LTC6811_get_total_voltage(uint16_t *ptv)
{
     uint8_t crc;
     uint8_t dasiy_rxdata[8];//����2Byte PEC���8Byte
     ltc6811_wakeup_standy();
     LTC681x_adstat(MD_27KHZ_14KHZ,STAT_CH_SOC);
     ltc6811_delay_ms(1);
     LTC681x_rdstat_reg(1,1,dasiy_rxdata);
     if(check_crc(dasiy_rxdata,1*8,&crc)==0)
     {
        *ptv=*(uint16_t*)(&dasiy_rxdata[0]);
        #ifdef DEBUG
            printf("get total voltage successful!\n");

        #endif
        return 1;
     }
     return 0;
}

uint8_t LTC6811_Get_PerCell_Voltage(uint16_t* p)
{
    uint8_t i = 0,j = 0;
    uint8_t crc = 0;
    uint8_t dasiy_rxdata[8];
    uint8_t ReturnValue = 1;
    ltc6811_wakeup_standy();
    LTC681x_clrcell();
    ltc6811_delay_ms(5);
    LTC681x_adcvax(MD_27KHZ_14KHZ/*MD_7KHZ_3KHZ*/,DCP_DISABLED);
    ltc6811_delay_ms(10);
    for(i=1;i<5;i++)
    {
        LTC681x_rdcv_reg(i,1,dasiy_rxdata);
        if(check_crc(dasiy_rxdata,8,&crc)==0)
        {
            for(j=0;j<3;j++)
                *(p+j+3*(i-1)) = dasiy_rxdata[j*2] + (dasiy_rxdata[j*2+1] << 8);
            ReturnValue = 1;
            continue;
        }
        else
        {
        	for(j=0;j<12;j++)
        		*(p+j) = 0;
            ReturnValue = 0;
            break;
        }
    }
    return ReturnValue;
}

void LTC6811_Get_GPIO1_GPIO3(uint16_t *vol)//除以10000为电压值
{
    uint8_t crc;
    uint8_t dasiy_rxdata[8];
    CELLV *Auxv;
    ltc6811_wakeup_standy();
    LTC681x_clraux();
    ltc6811_delay_ms(6);
    wakeup_idle(2);     //ֻ��AUX
    ltc6811_wakeup_standy();

    LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_ALL);
//  LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_GPIO1);
//  delay_us(250);

//  LTC681x_adcvax(MD_26HZ_2KHZ,DCP_DISABLED);
//  LTC681x_adcvax(MD_7KHZ_3KHZ,DCP_ENABLED);
    ltc6811_delay_ms(6);
    LTC681x_rdaux_reg(1,1,dasiy_rxdata);
    if(check_crc(dasiy_rxdata,1*8,&crc)==0)
    {
        Auxv=(CELLV*)(dasiy_rxdata);
        vol[0]=Auxv->cell1;
        vol[1]=Auxv->cell2;
        vol[2]=Auxv->cell3;
    }
}

void LTC6811_Get_GPIO4_VREF2(uint16_t *vol)
{
    uint8_t crc;
    uint8_t dasiy_rxdata[8];
    CELLV *Auxv;
    ltc6811_wakeup_standy();
    LTC681x_clraux();
    ltc6811_delay_ms(6);
    wakeup_idle(2);     //ֻ��AUX
    ltc6811_wakeup_standy();

    LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_ALL);
//  LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_GPIO1);
//  delay_us(250);

//  LTC681x_adcvax(MD_26HZ_2KHZ,DCP_DISABLED);
//  LTC681x_adcvax(MD_7KHZ_3KHZ,DCP_ENABLED);
    ltc6811_delay_ms(6);
    LTC681x_rdaux_reg(2,1,dasiy_rxdata);
    if(check_crc(dasiy_rxdata,1*8,&crc)==0)
    {
        Auxv=(CELLV*)(dasiy_rxdata);
        vol[0]=Auxv->cell1;
        vol[1]=Auxv->cell2;
        vol[2]=Auxv->cell3;
    }
}


uint8_t LTC6811_ActiveBallancing(uint8_t ltc6811_addr,uint8_t CellNumber)                                  //
{
    uint8_t i = 0;
    uint16_t PEC = 0;
    uint8_t RxData[8],TxData[8],WriteBuffer[12];//����2Byte PEC���8Byte
    if((ltc6811_addr < 0x80)||(ltc6811_addr > 0xB8))
		return -1;
    if(CellNumber > 12)
		return -1;    
    ltc6811_wakeup_standy();
    ltc6811_delay_ms(5);
//  if((LTC6804_rdcfg(RxData) == 1) && (CellNumber >= 1))//12�ڵ�ض����ŵ�Ļ�CellNumber = 0
    if((LTC6804_rdcfg(RxData) == 1))//12�ڵ�ض����ŵ�Ļ�CellNumber = 0
    {
        for(i=0;i<4;i++)
            *(TxData + i) = *(RxData + i);//�Ѷ�������ǰ4Byte���Ƹ����ͻ���
        if((CellNumber >=1 ) && (CellNumber <= 8))
        {
            TxData[4] = 0x01 << (CellNumber - 1);
            TxData[5] = 0x00;
        }
        else if((CellNumber >8 ) && (CellNumber <= 12))
        {
            TxData[4] = 0x00;
            TxData[5] = 0x01 << (CellNumber - 9);
        }
        else if(CellNumber==0||CellNumber==0xff)        //�رվ���
        {
            TxData[4] = 0x00;
            TxData[5] = 0x00;
        }

//        WriteBuffer[0] = 0x80;
        WriteBuffer[0] = ltc6811_addr;
        WriteBuffer[1] = 0x01;
        PEC = pec15_calc(2, WriteBuffer);
        WriteBuffer[2] = (uint8_t)(PEC >> 8);
        WriteBuffer[3] = (uint8_t)(PEC);
        PEC = pec15_calc(6, TxData);
        for(i=0;i<6;i++)
            WriteBuffer[i+4] = TxData[i];
        WriteBuffer[10] = (uint8_t)(PEC >> 8);
        WriteBuffer[11] = (uint8_t)(PEC);
        DASILY_RESET();
//      delay_us(10);
        spi_write_array(12,WriteBuffer);
//          delay_us(10);
        DASILY_SET();
        return 1;
    }
    else
        return 0;
}




