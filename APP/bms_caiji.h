/*
 * @Author: your name
 * @Date: 2021-11-25 10:32:09
 * @LastEditTime: 2021-12-10 11:29:08
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \6811RW_STM3210x_Hal\APP\bms.h
 */
/*
 * bms.h
 *
 *  Created on: 2021年11月25日
 *      Author: lxl
 */

#ifndef BMS_CAIJI_H_
#define BMS_CAIJI_H_

#include "drv_LTC6811_hw.h"

#ifndef RT_THREAD
#define RT_THREAD 0
#endif

#define CHARGING 1




#if RT_THREAD
#define BMS_Delay(x) rt_thread_mdelay(x)

#else
    #ifdef STM32_HAL_LIBRARY
#define BMS_Delay(x) HAL_Delay(x)
    #else
#define BMS_Delay(x) delay_ms(x)
    #endif 
#endif



#define Batcellnum  90

typedef struct{
    struct
    {
        uint8_t  selfcheckflag :1;//电池自检标志位，标志位为0,表示读取电池电压失败
        uint8_t  selfErrorNum :7;//自检出错次数
        uint32_t TotalVol;//实际总压值/=500 (V)
        uint16_t CellVoltage[Batcellnum];//实际单体电压值/10000 (V)
        uint16_t CellMaxVol;//电池包中最大电压值
        uint16_t CellMinVol;//电池包中最小电压值
        struct
        {
            uint8_t  Calibration :1;//充电或者维护完成一次就校正一次
            float cur;//电池当前容量
            float init;//上电初始校正容量
        }cap;
    }Bat;
    struct
    {
        uint8_t LowCellVolFlag :1;//放电单体过放标志位
        uint8_t LowTotalVolFlag:1;//放电总压过放标志位
        uint8_t OverCurrentFlag:1;//放电过流标志位
        uint8_t ShortCircuitFlag:1;//放电短路标志
        uint8_t protectFlag:1;//保护标志位
        uint16_t count;//放电次数
        uint16_t Time;//放电时间，单位秒
        float Current;//放电电流
        float Cap;//已放电容量，单位毫安时
        struct 
        {
            uint16_t CutOffTime;//系统预置放电关断时间
            uint16_t CutOffVol;//放电关断电压
            uint16_t ResD_vol;//放电关断恢复放电电压
            uint16_t DisWarnCellVol;//放电单体过放告警电压
            uint16_t DisWarnTolVol;//放电总压过放报警电压
        }conf;
    }DisCharge;
    struct
    {   
        uint8_t overCellVolFlag :1;//充电单体过压标志位
        uint8_t overTotalVolFlag :1;//充电总压过压标志位
        uint8_t overCurrentFlag :1;//充电过流标志位
        uint8_t overTime;//充电完成后计时
        enum
        {
            CHARGEINIT,
            CHARGECONCUR,
            CHARGECONVOL,
            CHARGEFINISH,
            CHARGEPRO,
        }state;//充电时状态
        uint16_t count;//充电次数 
        uint16_t remainingtime;//充电剩余时间，单位为秒,最长剩余18小时
        uint32_t time;//已充电时间，单位为秒
        struct 
        {
            uint16_t CellProtVol;//单体充电保护电压（任一单体达到该电压值，停止充电）可设置
            uint16_t TotalProtVol;//总体保护电压
        }conf;
        float Current;//充电电流，0-10A
        float Caped;//电池已充电容量
        struct
        {
            uint8_t Flag :1;//需均衡标志位
            uint8_t OpenFlag :1;//均衡打开标志位
            uint8_t Index;//需均衡的电池位置 
            uint16_t Time;//均衡时间
        }Balance;
        struct
        {
            uint8_t Flag :1;//恒压充
            uint8_t TurnDownFlag :1;//调小电流标志 
            uint8_t TurnDownCnt :5;//阶梯电流调小次数
            uint8_t Time;//降电流时每梯度时间,单位为s
            struct
            {
                uint16_t Vol;//恒压充开启电压
                float TurnOffCurrent;//恒压充结束电流
            }conf;
        }JiTi;
        // ConstantVOL_Charge_Str ConstantVol;//恒压充管理
    }Charge;

}bms_t;
extern bms_t bms;

uint8_t BMS_GetBatteryTotalVol(uint32_t *vol);
uint8_t BMS_GetBatteryTotalVol_Filter(uint32_t *vol);
uint8_t BMS_GetBatteryCellVol(uint16_t *cellvol);
uint8_t BMS_GetBatteryCellVol_Filter(uint16_t *cellvol);
uint8_t BMS_CheckCellStackStatus(uint16_t *CellStackVoltage,uint8_t num,
		uint8_t ischarge,uint16_t *max,uint16_t *min,uint8_t *bFlag,uint8_t *BalanceCellIndex);

#endif /* BMS_H_ */
