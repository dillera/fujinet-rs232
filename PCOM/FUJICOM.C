/**
 * #FUJINET Low Level Routines
 */

#include "pcom.h"
#include "fujicom.h"

void fujicom_init(void)
{
	int p=1;
	int baud=9600;

	if (getenv("FUJI_PORT"))
		p=atoi(getenv("FUJI_PORT"));

	if (getenv("FUJI_BAUD"))
		baud=atoi(getenv("FUJI_BAUD"));

	switch(p)
	{
		default:
		case 1:
			pcom_base = 0x3f8;
			pcom_int = 12;
			break;
		case 2:
			pcom_base = 0x2f8;
			pcom_int = 11;
			break;
	}

	pcom_set(baud,'N',8,1);
}

unsigned char fujicom_cksum(unsigned char *buf, unsigned short len)
{
	unsigned int chk = 0;
	int i=0;

	for (i=0;i<len;i++)
		chk = ((chk+buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);

	return (unsigned char)chk;
}

/**
 * @brief Internal function, send command, get response.
 *
 * @param c ptr to command frame to send
 * @return 'A'ck, or 'N'ak.
 */
char _fujicom_send_command(cmdFrame_t *c)
{
	unsigned char *cc = (unsigned char *)c;

	/* Calculate checksum and place in frame */
	c->dcksum = fujicom_cksum(cc,4);

	/* Assert DTR to indicate start of command frame */
	pcom_set_dtr(1);

	/* Write command frame */
	pcom_put(cc,sizeof(cmdFrame_t));

	/* Desert DTR to indicate end of command frame */
	pcom_set_dtr(0);

	return (char)pcom_getc();
}

char fujicom_command(cmdFrame_t *c)
{
	_fujicom_send_command(c);

	return pcom_getc();
}

char fujicom_command_read(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	int r; /* response */
 	int i;

	r = _fujicom_send_command(c);

	if (r == 'N')
		return r; /* Return NAK */

	/* Get COMPLETE/ERROR */

	r = pcom_getc();

	if (r == 'C')
	{
		/* Complete, get payload */

		for (i=0;i<len;i++)
		{
			buf[i]=pcom_getc();
		}

		/* Get Checksum byte, we don't use it. */
		pcom_getc();
	}

	/* Otherwise, we got an error, return it. */
	return (unsigned char)r;
}

char fujicom_command_write(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	unsigned char r; /* response */
	int i;
	unsigned char ck;

	r = _fujicom_send_command(c);

	if (r == 'N')
		return r;

	/* Write the payload */
	pcom_put(buf,len);

	/* Write the checksum */
	ck=fujicom_cksum(buf,len);
	pcom_put(&ck,1);

	/* Wait for ACK/NACK */
	r = pcom_getc();
	
	if (r == 'N')
		return r;

	/* Wait for COMPLETE/ERROR */
	return pcom_getc(); 
}

void fujicom_done(void)
{

}
