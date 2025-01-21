/**
 * @brief   Send file to network endpoint
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujicom.h"

/* buffers for commands like OPEN are expected to be 256 bytes */
unsigned char url[256];

/* 512 byte buffer used for copy. */
unsigned char buf[512];

/**
 * @brief main nput function
 * @param src string ptr to the N: source URL
 * @param dst string ptr to the destination filename
 * @return The exit error code 0 = success
 */
int nput(char *src, char *dst)
{
	FILE *fp = fopen(src,"rb");
	int err  = 0;
	cmdFrame_t c;
	unsigned long total=0;

	fujicom_init();

	/* Perform OPEN command */
	c.ddev   = 0x71;
	c.dcomnd = 'O';
	c.daux1  = 0x08; /* WRITE */
	c.daux2  = 0x00; /* NO TRANSLATION */
	strcpy(url,dst);
	fujicom_command_write(&c,url,sizeof(url));

	if (fujicom_command_write(&c,url,sizeof(url)) != 'C')
	{
		printf("\nCould not open destination file. Terminating.\n");

		c.ddev = 0x71;
		c.dcomnd = 'C';
		fujicom_command(&c);

		fujicom_done();
		return 2;
	}

	while (!feof(fp))
	{
		int l = fread(buf,1,512,fp);

		if (!l)
			break;
		
		c.ddev = 0x71;
		c.dcomnd = 'W';
		c.daux1 = l & 0xFF;
		c.daux2 = l >> 8;
		if (fujicom_command_write(&c,buf,l) != 'C')
		{
			printf("\nWrite error. Terminating\n");
			
			c.ddev = 0x71;
			c.dcomnd = 'C';
			fujicom_command(&c);

			fclose(fp);
			err = 0xff;
			fujicom_done();
			break;
		}

		total += l;

		printf("%10lu bytes transferred.\r",total);
	}

	return err;
}

/**
 * @brief show usage if incorrect # of parameters.
 */
void usage()
{
	printf("\nnput <fn> <N:url>\n");
}

/**
 * @brief Main program entry
 */
int main(int argc, char *argv[])
{
	if (argc<3)
	{
		usage();
		return 1;
	}

	return nput(argv[1],argv[2]);
}
