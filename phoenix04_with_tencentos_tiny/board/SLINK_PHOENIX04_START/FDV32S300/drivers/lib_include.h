/**
 * @file lib_include.h
 * @author David.lin
 * @brief
 * @version 0.1
 * @date 2021-06-04
 *
 * @copyright Fanhai Data Tech. (c) 2021
 *
 */

// firmware include file
#include "anac.h"
#include "crc.h"
#include "efc.h"
#include "i2c.h"
#include "iom.h"
#include "lptimer.h"
#include "pmu.h"
#include "rtc.h"
#include "spi.h"
#include "sysc.h"
#include "timer.h"
#include "twc.h"
#include "uart.h"
#include "wdt.h"

#ifdef _DEBUG
extern int __wrap_printf(const char *fmt, ...);
#define printf(...) __wrap_printf(__VA_ARGS__)
#else
#define printf(...)
#endif
