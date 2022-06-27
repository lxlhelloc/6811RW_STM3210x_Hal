
/*
 * bms_soc.c
 *
 *  Created on: 2021年12月10日
 *      Author: lxl
 */


#include "bms_soc.h"


float BatInitSoc = 0.5;



//上电初始化容量
float Get_Init_Soc(float cellvol)
{
    float Temp = 0,Cell_Sum_SOC = 50;
	Temp = cellvol;
	if(Temp>=4.185)
    {
        Cell_Sum_SOC=100.0F;
    }
    else if(Temp>=4.15)
    {
    	Cell_Sum_SOC=99.0F;
    }
   	else if(Temp>=4.0)//y = -569.09x2 + 4715.7x - 9669
   	{
      	Cell_Sum_SOC=(-569.09*Temp*Temp)+(4715.7*Temp)-9670;
   	}
   	else if(Temp>=3.6)//y = -36.37x2 + 377.33x - 839.18
   	{
      	Cell_Sum_SOC=(-36.37*Temp*Temp)+(377.33*Temp)- 839.18;
  	}
   	else if(Temp>=3.4)//y = 342.83x2 - 2213x + 3572.1
   	{
      	Cell_Sum_SOC=(342.83*Temp*Temp)-( 2213*Temp)+3572.1;
   	}
  	else if(Temp>=3.3)//y = 232.64x2 - 1479.2x + 2351.6
   	{
     	Cell_Sum_SOC=(232.64*Temp*Temp)-(1479.2*Temp)+2351.6;
   	}
   	else if(Temp>=3.2)//y = 93.908x2 - 583.39x + 906.16
   	{
        Cell_Sum_SOC=(93.908*Temp*Temp)-(583.39*Temp)+ 906.16;
    }
    else
    	Cell_Sum_SOC=0;
    Cell_Sum_SOC/=100;    
    return Cell_Sum_SOC;
}

//通过安时积分计算得到的容量
float Get_AHcal_Soc(uint8_t chargeover,float* charged,float* discharged)
{
    static uint8_t revise_flag = 0;//初始时电池未校正
	float soc;
    if(chargeover == 1)//在充电完成情况下
    {
        soc = 1;//满电
        revise_flag = 1;//系统进行充电容量校正标记下
        *charged = 0;//已充电和已放电全部归零
        *discharged = 0;
    }
	else//不是在充满电情况下
    {
        if(revise_flag == 1)//电池充满电校正过
        {
            soc = (BatTotalCap - *discharged + *charged)/BatTotalCap;
        }
        else//没有充满电校正过,没有校正前，SOC最大只能估算到0.99
        {
            soc = (BatInitSoc*BatTotalCap + *charged - *discharged)/BatTotalCap;
            if(soc > 0.99)
                soc = 0.99;
        }
        if(soc > 0.999)
            soc = 1;
        else if(soc < 0.001)
            soc = 0;
    }
    return soc;
}

//通过电压曲线计算SOC
float Get_VolCal_Soc(float cellvol)
{
	return Get_Init_Soc(cellvol);
}
