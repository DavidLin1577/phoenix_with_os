/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "lib_include.h"
#include "risc_v_csr.h"
#include "system_phnx04.h"

#define _SCB_BASE       (0xE0000000UL)
#define _SYSTICK_CMPLO   (*(rt_uint32_t *)(_SCB_BASE + 0x04))
#define _SYSTICK_CMOHI   (*(rt_uint32_t *)(_SCB_BASE + 0x08))
#define _SYSTICK_LO      (*(rt_uint32_t *)(_SCB_BASE + 0x0C))
#define _SYSTICK_HI      (*(rt_uint32_t *)(_SCB_BASE + 0x10))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
	if ((ticks - 1) > 0xFFFFFF)
	{
	    return 1;
	}

	_SYSTICK_CMPLO = ticks & 0xFFFF;
	_SYSTICK_CMPLO = 65000;
	_SYSTICK_CMOHI = 0x0;
	_SYSTICK_LO    = 0x0;
	_SYSTICK_HI    = 0x0;

	EnableMtimeIRQ();

	return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Clock Update */
    SystemCoreClockUpdate();
    
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void MTIM_IntHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

	_SYSTICK_CMPLO = 800;
	_SYSTICK_CMOHI = 0x0;
	_SYSTICK_LO    = 0x0;
	_SYSTICK_HI    = 0x0;

	printf("MTIM_IntHandler\n");

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}
