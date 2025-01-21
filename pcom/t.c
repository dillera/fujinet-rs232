#include <dos.h>
#include <stdio.h>
#include "pcom.h"

void main(void)
{
	pcom_base=0x02f8;
	pcom_set(9600,'N',8,1);

	while (1)
	{
		printf("0x%02x\n",inportb(pcom_base+LSR) & 0x01);
		printf("%c\n",pcom_getc());
	}
}
