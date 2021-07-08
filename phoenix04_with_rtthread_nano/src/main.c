/*
 ============================================================================
 Name        : main.c
 Author      :    
 Version     :
 Copyright   : Your copyright notice
 Description : Hello RISC-V World in C
 ============================================================================
 */


#include "lib_include.h"
#include <rtthread.h>

int main(void)
{
	while(1)
	{
	    printf("Hello world\n");
	    rt_thread_mdelay(50000);

	    printf("Hello riscv\n");
	    rt_thread_mdelay(50000);
	}
	return 0;
}

