/**
 * @brief   Quick tool to set MS-DOS date/time from FujiNet ApeTime
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE for details
 */

#include "fujicom.h"
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

struct _tm
{
	char tm_mday;
	char tm_month;
	char tm_year;
	char tm_hour;
	char tm_min;
	char tm_sec;
} t;

int year=0;

union REGS r;
cmdFrame_t c;
char reply=0;

void main(void)
{
	c.ddev = 0x45;
	c.dcomnd = 0x9A;

	fujicom_init();
	reply = fujicom_command_read(&c,(unsigned char *)&t,sizeof(t));

	if (reply != 'C')
	{
		printf("Could not read time from FujiNet.\nAborted.\n");
		fujicom_done();
		return;
	}

	r.h.ah = 0x2B;
	r.x.cx = t.tm_year + 2000;
	r.h.dh = t.tm_month;
	r.h.dl = t.tm_mday;

	intdos(&r,NULL);

	r.h.ah = 0x2D;
	r.h.ch = t.tm_hour;
	r.h.cl = t.tm_min;
	r.h.dh = t.tm_sec;
	r.h.dl = 0;

	intdos(&r,NULL);

	printf("MS-DOS Time now set from FujiNet\n");
	printf("DATE: %02u/%02u/%02u\n",t.tm_month,t.tm_mday,t.tm_year);
	printf("TIME: %02u:%02u:%02u\n",t.tm_hour,t.tm_min,t.tm_sec);

	return;
}
