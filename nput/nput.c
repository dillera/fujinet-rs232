/**
 * @brief   Send file to network endpoint
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

/* buffers for commands like OPEN are expected to be 256 bytes */
unsigned char url[256];

/* 512 byte buffer used for copy. */
unsigned char buf[8192];

/* Register packs for communicating with INT F5 */
union REGS r;
struct SREGS sr;

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
	unsigned long total=0;

	strcpy(url,dst);

	/* Perform OPEN command */
        r.h.ah   = 0x80;
	r.h.al   = 0x71;
	r.h.cl   = 'O';
	r.h.dl   = 0x08; /* WRITE */
	r.h.dh   = 0x00; /* NO TRANSLATION */

	sr.es    = FP_SEG(url);
	r.x.bx   = FP_OFF(url);
	r.x.di   = sizeof(url);
	int86x(0xF5,&r,&r,&sr);

	if (r.h.al != 'C')
	{
		printf("\nCould not open destination file. Terminating.\n");

		r.h.ah = 0x00;
		r.h.al = 0x71;
		r.h.cl = 'C';
		int86(0xF5,&r,&r);

		return 2;
	}

	while (!feof(fp))
	{
		int l = fread(buf,1,sizeof(buf),fp);

		if (!l)
			break;

		r.h.ah = 0x80;		
		r.h.al = 0x71;
		r.h.cl = 'W';
		r.x.dx = l;
		
		sr.es  = FP_SEG(buf);
		r.x.bx = FP_OFF(buf);
		r.x.di = l;

		int86x(0xF5,&r,&r,&sr);

		if (r.h.al != 'C')
		{
			printf("\nWrite error. Terminating\n");
			
			r.h.ah = 0x00;
			r.h.al = 0x71;
			r.h.cl = 'C';
			int86(0xF5,&r,&r);

			fclose(fp);
			err = 0xff;
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
