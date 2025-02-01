/** 
 * @brief Read test
 */

#include <fujicom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

unsigned char buf1[512], buf2[512];
cmdFrame_t c;

void main(void)
{
	int i=0;

	for (i=1;i<256;i++)
	{
		int j=0;
		union REGS r;
		struct SREGS s;

		r.x.ax = 0x0000;
		r.h.dl = i;

		s.es   = FP_SEG(buf2);
		r.x.bx = FP_OFF(buf2);

		memset(buf1,i,sizeof(buf1));
		memset(buf2,0,sizeof(buf2));

		int86x(0xF5,&r,&r,&s);
		
		printf("%c",r.h.al);

		for (j=0;j<sizeof(buf2);j++)
			if (buf1[j] != buf2[j])
			{
				printf("X");
				break;
			}

		printf(".");
	}
}
