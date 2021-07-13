/**
 * @file anac.c
 * @author 
 * @brief
 * @version 0.1
 * @date 2021-06-30
 *
 * @copyright Fanhai Data Tech. (c) 2021
 *
 */

#include "anac.h"
#include "sysc.h"

/**
 * @brief ANAC init
 *
 * @param freq :anac module freq (kHz)
 */
void ANAC_Init(int freq) 
{
    SYSC->CLKENCFG |= SYSC_CLKENCFG_ANAC;
    SystemCoreClockUpdate();
    int tmp = SystemCoreClock / 1000 / freq;
    PARAM_CHECK(tmp < 2);
    int tmp1 = SystemCoreClock / 1000 / (((tmp / 2 - 1) + 1) * 2) / 500;
    PARAM_CHECK(tmp1 < 2);
    SDC->ME_CTL = ANAC_ME_CTL_ANAC_EN;
    SYSC_SetANAC_CLKDiv(tmp / 2 - 1, tmp1 - 1);
}

/**
 * @brief anac deinit
 *
 */
void ANAC_DeInit(void) 
{
    int i;
    SYSC_WPT_UNLOCK();
    SYSC->MSFTRSTCFG |= SYSC_MSFTRSTCFG_ANAC;
    for (i = 10; i > 0; --i)
        ;
    SYSC->CLKENCFG &= ~SYSC_CLKENCFG_ANAC;
}

/**
 * @brief 模拟电源使能
 *
 */
void ANAC_AnalogPowerEn(void) 
{
    ANAC_WPT_UNLOCK();
    SDC->ANAC_CFG |= ANAC_ANAC_CFG_BGR_EN;
    {
        volatile int iTmp = (long long)SystemCoreClock * 100 / 7000000;
        while (--iTmp)
            asm("nop");
    }
    ANAC_WPT_UNLOCK();
    SDC->ANAC_CFG |= ANAC_ANAC_CFG_ALDO4A_EN;
    {
        volatile int iTmp = (long long)SystemCoreClock * 70 / 7000000;
        while (--iTmp)
            asm("nop");
    }
}


/**
 * @brief adc init
 *
 * @param chn ADC_CHN_xxx
 * @param buffEn ENABLE , DISABLE
 * @param verfSel ADC_VREF_SEL_xxx
 * @param verfVol ADC_VREF_VOLT_xxx
 * @param smpTimes ADC_SMP_TIMS_x
 * @param smpCycle ADC_SMP_CYCLE_xx
 * @note ：调用此函数后要进行AD通道的IO口设置为模拟口与选择模拟功能为AD输入
 */
void ADC_Init(int chn, int buffEn, int verfSel, int verfVol, int smpTimes,
              int smpCycle) 
{
    ADC->CFG = (chn << 7) | (verfSel << 4) | (verfVol << 1);
    if (buffEn == ENABLE) {
    	ADC->CFG |= (1 << 6);
    } else {
    	ADC->CFG &= ~(1 << 6);
    }
    if (verfSel == ADC_VREF_SEL_INT) {
    	ADC->CFG |= 0x01;
    } else {
    	ADC->CFG &= ~0x01;
    }
    ADC->CFG = (smpTimes << 13) | (smpCycle << 11);
}
/**
 * @brief ADC enalble
 *
 */
void ADC_Enable(void) { ADC->CTL |= 0x01; }
/**
 * @brief ADC disable
 *
 */
void ADC_Disable(void) { ADC->CTL &= ~0x01; }

/**
 * @brief ADC enable interrupt
 *
 */
void ADC_EnableIRQ(void) { ADC->CTL |= (1 << 1); }
/**
 * @brief start AD convert
 *
 */

/**
 * @brief ADC disable interrupt
 *
 */
void ADC_DisableIRQ(void) { ADC->CTL &= ~(1 << 1); }
/**
 * @brief ADC start convert
 *
 */
void ADC_StartConvert(void) { ADC->CTL |= (1 << 2); }

/**
 * @brief ADC get single convert result value
 *
 * @param valNo :ADC_VAL_NOx
 * @return u32 :val
 */
u32 ADC_GetSingleResultValue(int valNo) {
    PARAM_CHECK(valNo > 7);
    return ADC->VAL[valNo];
}

/**
 * @brief get convert average value
 *
 * @return u32
 */
u32 ADC_GetAverageValue(void) { return ADC->AVG_VAL; }
/**
 * @brief get interrupt flag
 *
 * @return BOOL
 */
BOOL ADC_GetIntFlag(void) { return (ANAC->ANAC_FLAG & 0x01) ? TRUE : FALSE; }
/**
 * @brief clear interrupt flag
 *
 */
void ADC_ClrIntFlag(void) { ANAC->ANAC_FLAG |= 0x01; }


