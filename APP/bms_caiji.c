/*
 * @Author: your name
 * @Date: 2021-11-25 10:32:26
 * @LastEditTime: 2021-12-10 11:29:21
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \6811RW_STM3210x_Hal\APP\bms.c
 */
/*
 * bms.c
 *
 *  Created on: 2021年11月25日
 *      Author: lxl
 */



#include "drv_LTC6811.h"
#include "drv_LTC6811_proto.h"
#include "stdint.h"
#include "bms_caiji.h"


//50000(5V)/100 = 500(50mv) ,均衡充的开启条件
#define  EqualizingVoltageDifference  500
//定义LTC6811芯片的数量
#define CHIP_COUNT 8
//定义电池节数
#ifndef Batcellnum
#define Batcellnum  90
#endif
//定义6811的硬件地址
const uint8_t ltc6811addr[CHIP_COUNT]={0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0};

//一个6811有多少个单体，从最底层数起走
const uint8_t _6811_Have_BatCell[CHIP_COUNT] = {12,12,12,12,12,12,12,6};
//电池单节在6811的位置关系，从最底层开始数，到下一片6811就加12，比如电池15节，在第二片6811中的12处，则计数为24;
const uint8_t BatCell_in6811_Index[Batcellnum] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
												21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
												41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,
												61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
												81,82,83,84,85,86,87,88,89,90};

//用于检查const uint8_t BatCell_in6811_Index[Batcellnum] 定义是否有错误;
#define Bat_lastcell_Index 19
#if (Bat_lastcell_Index == 0)
#error "Batcell_in6811_Index[Batcellnum] define index error "
#endif






bms_t bms;

uint8_t BMS_GetBatteryTotalVol(uint32_t *vol)
{
	uint8_t read6811successflag = 0;
	uint8_t i;
	uint16_t _totalvol[CHIP_COUNT] = {0};
	uint32_t volsum = 0;
	for(i = 0;i < CHIP_COUNT;++i)
	{
		LTC6811_ADD = ltc6811addr[i];
		if(LTC6811_get_total_voltage(&_totalvol[i]) == 1)
		{
			read6811successflag ++;
		}
		volsum += _totalvol[i]; 
	}
	*vol = volsum;
	return read6811successflag;
}

uint8_t BMS_GetBatteryTotalVol_Filter(uint32_t *vol)
{
	static uint8_t cnt = 0;
	static uint32_t totalvol[5] = {0};
	uint32_t totalvolsum;
	uint8_t i;
	uint8_t flag;
	for(i = 4;i > 0;--i)
	{
		totalvol[i] = totalvol[i-1];
	}
	flag = BMS_GetBatteryTotalVol(&totalvol[0]);
	cnt ++;
	totalvolsum = 0;
	for(i = 0;i < 5;++i)
		totalvolsum += totalvol[i];
	if(cnt > 5)
	{
		cnt = 5;

	}
	*vol = totalvolsum / cnt;
	return flag;
}

uint8_t BMS_GetBatteryCellVol(uint16_t *cellvol)
{
	uint16_t vol[96];//最多支持8片6811读取
	uint8_t i;

	for(i = 0;i < CHIP_COUNT;++i)
	{
		LTC6811_ADD = ltc6811addr[i];
		LTC6811_Get_PerCell_Voltage(&vol[i*12]);
		BMS_Delay(10);
	}
	for(i = 0;i < Batcellnum;++i)
	{
		*cellvol = vol[BatCell_in6811_Index[i] - 1];
		cellvol++;
	}
	return 0;
}

uint8_t BMS_GetBatteryCellVol_Filter(uint16_t *cellvol)
{
	static uint8_t cnt = 0,filter_Group = 1;
	static uint16_t CellVol[Batcellnum][5] ={0};
	uint8_t i,j;
	uint16_t Cell_Vol[Batcellnum] = {0};
	uint32_t Cell_Volsum[Batcellnum]={0};
	BMS_GetBatteryCellVol(Cell_Vol);
	for(i = 0;i < Batcellnum;++i)
	{
		CellVol[i][cnt] = Cell_Vol[i];
	}
	cnt++;
	if(filter_Group != 5)
		filter_Group = cnt;
	cnt %= 5;

	for(i = 0;i < Batcellnum; ++i)
	{
		for(j = 0;j < filter_Group;++j)
		{
			Cell_Volsum[i] += CellVol[i][j]; 
		}
		cellvol[i] = Cell_Volsum[i]/filter_Group;

	}
	return 0;
}

uint8_t BMS_CheckCellStackStatus(uint16_t *CellStackVoltage,uint8_t num,uint8_t ischarge,uint16_t *max,uint16_t *min,uint8_t *bFlag,uint8_t *BalanceCellIndex)
{
	unsigned char i;
	unsigned int MaxVoltageCellIndex = 0,MinVoltageCellIndex = 0;
	for(i = 0;i < num-1;i++)
	{
		if(*(CellStackVoltage + MaxVoltageCellIndex) < *(CellStackVoltage + i+1))
		{
			MaxVoltageCellIndex = i+1;
		}
		if(*(CellStackVoltage + MinVoltageCellIndex) > *(CellStackVoltage + i+1))
		{
			MinVoltageCellIndex = i+1;
		}
	}
    *max=CellStackVoltage[MaxVoltageCellIndex];
    *min=CellStackVoltage[MinVoltageCellIndex];

    if(ischarge == CHARGING)
    {
	    if(*(CellStackVoltage + MaxVoltageCellIndex) - *(CellStackVoltage + MinVoltageCellIndex) >= EqualizingVoltageDifference)
	    {
		    *BalanceCellIndex = MaxVoltageCellIndex + 1;
            *bFlag = 1;
		    return 1;
	    }
	    else
	    {
		    *BalanceCellIndex = 0xFF;
            *bFlag= 0;
		    return 0;
	    }
    }
    else
    {
        *BalanceCellIndex = 0xFF;
        *bFlag=0;
        return 0;
    }
    return 0;
}



int BMS_OpenBalanceCharge(uint8_t bat_index)
{
	uint8_t ic_index;
	uint8_t cell_in6811_index;
	if(bat_index > Batcellnum)
	return -1;
	ic_index = BatCell_in6811_Index[bat_index]/12;
	cell_in6811_index = BatCell_in6811_Index[bat_index]%12;
	LTC6811_ActiveBallancing(ltc6811addr[ic_index],cell_in6811_index);
	return 0;
}


int BMS_CloseBalanceCharge(uint8_t bat_index)
{
	uint8_t ic_index;
	if(bat_index > Batcellnum)
	return -1;
	ic_index = BatCell_in6811_Index[bat_index]/12;
	LTC6811_ActiveBallancing(ltc6811addr[ic_index],0);
	return 0;
}


int BMS_BalanceCharge(uint16_t *batcellvol)
{
	uint8_t NeedBalanChargeFlag=0,BalanIndex=0;
	uint16_t max,min;
	BMS_CheckCellStackStatus(batcellvol,Batcellnum,CHARGING,&max,&min,&NeedBalanChargeFlag,&BalanIndex);
	return 0;
}

int BMS_CollectBattery(void)
{
	uint8_t readflag,temp;
	readflag = BMS_GetBatteryTotalVol_Filter(&bms.Bat.TotalVol);
	if(readflag == 0)//读取失败
	{
		bms.Bat.selfErrorNum++;
		if(bms.Bat.selfErrorNum > 50)
			bms.Bat.selfcheckflag = 0;
	}
	else
		bms.Bat.selfcheckflag = 1;
		
	BMS_GetBatteryCellVol_Filter(bms.Bat.CellVoltage);
	BMS_CheckCellStackStatus(bms.Bat.CellVoltage,Batcellnum,0,&bms.Bat.CellMaxVol,&bms.Bat.CellMinVol,&temp,&temp);
	return 0;
}
