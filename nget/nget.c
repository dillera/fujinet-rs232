/**
 * @brief   Retrieve file from network endpoint
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

/* large byte buffer used for copy. */
unsigned char buf[8192];

/* Status structure */
struct _status
{
	unsigned short bw;	 /* # of bytes waiting 0-65535 	*/
	unsigned char connected; /* Are we connected? 		*/
	unsigned char error;	 /* error code	1-255		*/
} status;

/* Register packs for int86x() */
union REGS r;
struct SREGS sr;

/**
 * @brief main nget function
 * @param src string ptr to the N: source URL
 * @param dst string ptr to the destination filename
 * @return The exit error code 0 = success
 */
int nget(char *src, char *dst)
{
	FILE *fp = fopen(dst,"wb");
	int err  = 0;
	unsigned long total=0;
	char *username=NULL, *password=NULL;

	if ((src[0] != 'N') && (src[1] != ':'))
	{
		printf("\nInvalid N: URL. Terminating.\n");
		return 1;
	}

	if (!fp)
	{
		printf("\nCould not open destination file. Terminating.\n");
		return 2;
	}

	username=getenv("FUJI_USER");
	password=getenv("FUJI_PASS");

	if (username)
	{
		memset(url,0,sizeof(url));
		strcpy(url,username);

		/* Perform username command */
		r.h.ah = 0x80;
		r.h.al = 0x71;
		r.h.cl = 0xFD;
		r.x.dx = 0x0000;

		sr.es  = FP_SEG(&url);
		r.x.bx = FP_OFF(&url);
		r.x.di = sizeof(url);

		int86x(0xF5,&r,&r,&sr);
	}

	if (password)
	{
		memset(url,0,sizeof(url));
		strcpy(url,password);

		/* Perform password command */
		r.h.ah = 0x80;
		r.h.al = 0x71;
		r.h.cl = 0xFE;
		r.x.dx = 0x0000;

		sr.es  = FP_SEG(&url);
		r.x.bx = FP_OFF(&url);
		r.x.di = sizeof(url);

		int86x(0xF5,&r,&r,&sr);
	}

	memset(url,0,sizeof(url));
	strcpy(url,src);

	/* Perform OPEN command */
        r.h.ah   = 0x80;
	r.h.al   = 0x71;
	r.h.cl   = 'O';
	r.h.dl   = 0x04; /* READ ONLY */
	r.h.dh   = 0x00; /* NO TRANSLATION */
	
	sr.es    = FP_SEG(&url);
	r.x.bx   = FP_OFF(&url);
	r.x.di   = sizeof(url);

	int86x(0xF5,&r,&r,&sr);

 	delay(10);

	/* Perform initial status command */
	r.h.ah   = 0x40;
	r.h.al   = 0x71;
	r.h.cl   = 'S';
	r.h.dh   = 0x00;
	r.h.dl   = 0x00;

	sr.es    = FP_SEG(&status);
	r.x.bx   = FP_OFF(&status);
	r.x.di   = sizeof(status);

	int86x(0xF5,&r,&r,&sr);

	if (status.error > 1 && !status.bw)
	{
		printf("\nOPEN ERROR: %u\n",status.error);
		return status.error;
	}

	/* if (!status.connected)
	{
		printf("\nOPEN ERROR: Host immediately disconnected.\n");
		return 0xFF;
	} */

	while (1)
	{
		int bw = (status.bw > sizeof(buf) ? sizeof(buf) : status.bw);
		char reply = 0;

		if (!bw && status.error == 136)
			break;
		else if (!bw)
			continue;

		delay(1);

		/* Do read */
		r.h.ah   = 0x40;
		r.h.al   = 0x71;
		r.h.cl   = 'R';
		r.x.dx   = bw;

		sr.es    = FP_SEG(&buf);
		r.x.bx   = FP_OFF(&buf);
		r.x.di   = bw;

		int86x(0xF5,&r,&r,&sr);
		reply = r.h.al;

		if (reply != 'C')
		{
		 	printf("\nREAD ERROR AT %lu bytes. Reply was %c\n",total,reply);
		 	return 144;
		}

		/* Do write */
		fwrite(buf,bw,1,fp);

		total += bw;

		printf("%10lu bytes transferred.\r",total);

		delay(1);

		/* Do next status */
		r.h.ah = 0x40;
		r.h.al = 0x71;
		r.h.cl = 'S';
		r.h.dl = 0x00;
		r.h.dh = 0x00;

		sr.es  = FP_SEG(&status);
		r.x.bx = FP_OFF(&status);
		r.x.di = sizeof(status);

		int86x(0xF5,&r,&r,&sr);
	}

bye:
	/* Perform CLOSE command */
	r.h.ah   = 0x00;
	r.h.al   = 0x71;
	r.h.cl   = 'C';
	r.x.dx   = 0x0000;

	int86(0xF5,&r,&r);

	fclose(fp);
	
	return err;
}

/**
 * @brief show usage if incorrect # of parameters.
 */
void usage()
{
	printf("\nNGET <N:url> <fn>\n");
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

	return nget(argv[1],argv[2]);
}
