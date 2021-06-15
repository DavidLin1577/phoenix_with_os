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
#include "shell_port.h"


int main(void)
{
	printf("Hello world\r\n");

	userShellInit();

	while(1)
    {
		shellTask(&shell);
    }
	return 0;
}

