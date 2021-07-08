/**
 * @file lptimer.c
 * @author 
 * @brief
 * @version 0.1
 * @date 2020-05-12
 *
 * @copyright Fanhai Data Tech. (c) 2020
 *
 */

#include "lptimer.h"
#include "pmu.h"
#include "sysc.h"
/**
 * @brief LPtimer init
 *
 * @param iClkSrc: LP_CLKSEL_LRC , LP_CLKSEL_XTL
 * @param iDel:  delay ms (lrc 262s,xtl max65s)
 * @param iMode :LPT_SIG_TIME_CNT , LPT_PIT_CNT
 * note:clk source = lrc  at least 4ms err,
 */
void LPT_Init(int iClkSrc, int iDel, int iMode) {
    PARAM_CHECK((iClkSrc != LP_CLKSEL_LRC) &&
                (iClkSrc != LP_CLKSEL_XTL));
    PARAM_CHECK((iMode != LPT_SIG_TIME_CNT) && (iMode != LPT_PIT_CNT));

    SYSC->CLKENCFG |= SYSC_CLKENCFG_LPTIM;

    LPTIM->CFG = (iDel >> 2) - 1;
    if (iMode == LPT_SIG_TIME_CNT) {
        LPTIM->CR &= ~LPTIM_CR_PITE;
    } else {
        LPTIM->CR |= LPTIM_CR_PITE;
    }
    PMU->WPT = PMU_WPT_V0;
    PMU->WPT = PMU_WPT_V1;
    PMU->CR |= PMU_CR_LPTCLKEN;
//    LPTIM->CR |= LPTIM_CR_EN;
}

/**
 * @brief LPT 使能控制
 *
 * @param iCtrl:ENABLE or DISABLE
 */
void LPT_EnableControl(int iCtrl) {
    if (iCtrl == ENABLE)
        LPTIM->CR |= LPTIM_CR_EN;
    else
        LPTIM->CR &= ~LPTIM_CR_EN;
}
/**
 * @brief Lptime deinit
 *
 */
void LPT_DeInit(void) {
    PMU->WPT = PMU_WPT_V0;
    PMU->WPT = PMU_WPT_V1;
    PMU->CR &= ~PMU_CR_LPTCLKEN;
    SYSC->CLKENCFG &= ~SYSC_CLKENCFG_LPTIM;
}

/**
 * @brief enable interrupt
 *
 */
void LPT_EnableIRQ(void) { LPTIM->CR |= LPTIM_CR_IE; }
/**
 * @brief disable interrupt
 *
 */
void LPT_DisableIRQ(void) { LPTIM->CR &= ~LPTIM_CR_IE; }

/**
 * @brief get current count value
 *
 * @return u16 :16bit count
 */
u16 LPT_GetCount(void) {
    u16 tmp0, tmp1;
    while (1) {
        tmp0 = LPTIM->CNT & LPTIM_CNT;
        tmp1 = LPTIM->CNT & LPTIM_CNT;
        if (tmp0 == tmp1)
            break;
    }
    return tmp0;
}
