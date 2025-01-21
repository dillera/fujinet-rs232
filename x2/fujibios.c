/**
 * @brief   FujiBIOS Experiment #2
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <stdio.h>
#include <dos.h>
#include "com.h"
#include "fujicom.h"

/**
 * @brief the COM port to use (1 or 2)
 */
int _port = 0;

/**
 * @brief A FUJICOM Command frame
 */
cmdFrame_t c;

/**
 * @brief AH = 00, Do command with no payload
 * @param ax (Contains AL which is the command to execute)
 * @param cx (Contains DAUX1 and DAUX2 parameters)
 * @param dx (DL contains the device ID)
 * @return The command result ('C'omplete or 'E'rror)
 */
unsigned char ah00(unsigned int ax, unsigned int cx, unsigned dx)
{
	int r = 0;

	enable();
	fujicom_init(_port);

	c.ddev   = dx & 0xFF;
	c.dcomnd = ax & 0xFF;
	c.daux1  = cx & 0xFF;
	c.daux2  = cx >> 8;
	r = fujicom_command(&c);
	fujicom_done();

	return r;
}

/**
 * @brief AH = 40, Do command with payload to computer (READ)
 * @param ax (Contains AL which is the command to execute)
 * @param cx (Contains DAUX1 and DAUX2 parameters, doubles as length)
 * @param es (Contains segment for destination buffer)
 * @param bx (Contains offset for destination buffer)
 * @param dx (DL contains the device ID)
 * @return The command result ('C'omplete or 'E'rror)
 */
unsigned char ah40(unsigned int ax,
		   unsigned int cx,
		   unsigned int es,
		   unsigned int bx,
		   unsigned int dx)
{
	int r = 0;

	enable();
	fujicom_init(_port);

	c.ddev   = dx & 0xFF;
	c.dcomnd = ax & 0xFF;
	c.daux1  = cx & 0xFF;
	c.daux2  = cx >> 8;
	r = fujicom_command_read(&c,(unsigned char *)MK_FP(es,bx),cx);
	fujicom_done();

	return r;
}

/**
 * @brief AH = 80, Do command with payload to FujiNet (WRITE)
 * @param ax (Contains AL which is the command to execute)
 * @param cx (Contains DAUX1 and DAUX2 parameters, doubles as length)
 * @param es (Contains segment for source buffer)
 * @param bx (Contains offset for source buffer)
 * @param dx (DL contains the device ID)
 * @return The command result ('C'omplete or 'E'rror)
 */
unsigned char ah80(unsigned int ax,
		   unsigned int cx,
		   unsigned int es,
		   unsigned int bx,
		   unsigned int dx)
{
	int r = 0;

	enable();
	fujicom_init(_port);

	c.ddev   = dx & 0xFF;
	c.dcomnd = ax & 0xFF;
	c.daux1  = cx & 0xFF;
	c.daux2  = cx >> 8;
	r = fujicom_command_write(&c,(unsigned char *)MK_FP(es,bx),cx);
	fujicom_done();

	return r;
}

/**
 * @brief The primary dispatch for INT 0xF5
 * @param bp Preserved BP register
 * @param di Preserved DI register
 * @param si Preserved SI register
 * @param ds Preserved DS register
 * @param es Preserved ES register
 * @param dx Preserved DX register
 * @param cx Preserved CX register
 * @param bx Preserved BX register
 * @param ax Preserved AX register
 */
void interrupt intf5(unsigned bp, unsigned di, unsigned si,
			unsigned ds, unsigned es, unsigned dx,
			unsigned cx, unsigned bx, unsigned ax)
{
	enable();
	switch(ax >> 8)
	{
		case 0x00: /* NO PAYLOAD */
			_AL = ah00(ax,cx,dx);
			break;
		case 0x40: /* FujiNet->Computer */
			_AL = ah40(ax,cx,es,bx,dx);
			break;
		case 0x80: /* Computer->FujiNet */
			_AL = ah80(ax,cx,es,bx,dx);
			break;
	}
}

void main(void)
{
	_port = atoi(getenv("FUJICOM_PORT"));

	if (!_port)
		_port = 1;

	disable();
	setvect(0xF5,intf5);
	enable();

	puts("\nFUJINET BIOS Installed.\n");

	keep(0,2048);
}
