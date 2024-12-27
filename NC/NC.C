/**
 * @brief   netcat - Terminal prototype
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "fujicom.h"
#include "com.h"

char url[256];
char buf[8192];
char txbuf[256];
char username[256];
char password[256];

cmdFrame_t c;

struct _status
{
	unsigned short bw;
	unsigned char connected;
	unsigned char err;
} status;

int nc(char *s)
{
	int err = 0;

	/* Open fujicom */
	fujicom_init(2);

		strcpy(username,"thomc");
		strcpy(password,"e1xb64XC46");

		c.ddev = 0x71;
		c.dcomnd = 0xFD;
		c.daux1 = c.daux2 = 0;
		fujicom_command_write(&c,username,sizeof(username));

		c.ddev = 0x71;
		c.dcomnd = 0xFE;
		c.daux1 = c.daux2 = 0;
		fujicom_command_write(&c,password,sizeof(password));

	/* Open URL */
	c.ddev = 0x71;
	c.dcomnd = 'O';
	c.daux1 = 0x0C; /* READ/WRITE */
	c.daux2 = 0x00; /* NO TRANSLATION */
	strcpy(url,s);

	if (fujicom_command_write(&c,url,sizeof(url)) != 'C')
	{
		/* Get error from status */
		c.ddev = 0x71;
		c.dcomnd = 'S';
		c.daux1 = c.daux2 = 0;
		fujicom_command_read(&c,
					(unsigned char *)&status,
					 sizeof(status));

		printf("\nCould not open URL, error: %u\n",status.err);
		goto bye;
	}

	/* connected, get initial status */

	c.ddev = 0x71;
	c.dcomnd = 'S';
	c.daux1 = c.daux2 = 0;
	fujicom_command_read(&c,(unsigned char *)&status,sizeof(status));

	while(status.connected)
	{
		int i=0;
		int bw=0;

		delay(1);

		while (kbhit())
		{
			txbuf[i++]=getch();
		}

		if (i)
		{
			c.ddev = 0x71;
			c.dcomnd = 'W';
			c.daux1 = i;
			c.daux2 = 0;
			fujicom_command_write(&c,(unsigned char *)&txbuf,i);
			i=0;
		}

		if (!fujicom_net_available())
			continue; 

		c.ddev = 0x71;
		c.dcomnd = 'S';
		c.daux1 = c.daux2 = 0;
		fujicom_command_read(&c,
					(unsigned char *)&status, 
					 sizeof(status));

		if (!status.bw)
			continue;

		bw = (status.bw > sizeof(buf) ? sizeof(buf) : status.bw);

		c.ddev = 0x71;
		c.dcomnd = 'R';
		c.daux1 = bw & 0xFF;
		c.daux2 = bw >> 8;
		fujicom_command_read(&c,&buf[0],bw);

		for (i=0;i<bw;i++)
			putchar(buf[i]);
	}

bye:
	/* Send close */
	c.ddev = 0x71;
	c.dcomnd = 'C';
	c.daux1 = c.daux2 = 0;
	fujicom_command(&c);

	fujicom_done();

	return err;
}

int usage(void)
{
	printf("\nnc <N:url>\n");
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return usage();

	nc(argv[1]);
}
