/**
 * @brief   Set SSID/Password
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#include <dos.h>
#include <stdio.h>
#include <fujicom.h>

struct SETSSID
{
	char ssid[33];
	char password[64];
} ss;

union REGS r;
struct SREGS sr;

int main(int argc, char *argv[])
{
	if (argc<3)
		{
			printf("setssid <ssid> <password>\r\n");
			return 1;
		}

	strcpy(ss.ssid,argv[1]);
	strcpy(ss.password,argv[2]);

	r.h.ah = 0x80; /* WRITE */
	r.h.al = 0x70; /* to FUJI device */
	r.h.cl = 0xFB; /* Set SSID command */

	/* set payload */
	sr.es  = FP_SEG(&ss);
	r.x.bx = FP_OFF(&ss);
	r.x.di = sizeof(ss); /* payload size */
	

	int86x(0xF5,&r,&r,&sr);

	printf("Set SSID Returned '%c'\r\n",r.h.al);

	return 0;
}
