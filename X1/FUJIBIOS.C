/**
 * @brief Experimental FujiNet BIOS 1
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <dos.h>
#include <fujicom.h>
#include <stdio.h>

cmdFrame_t cmd;

void get_adapter_config(unsigned es, unsigned bx)
{
	cmd.ddev = 0x70;
	cmd.dcomnd=0xE8;

	_AX = fujicom_command_read(&cmd,(unsigned char *)MK_FP(es,bx),140);
}

unsigned char test[4]={'A','A','A','A'};

unsigned char adapter_ready(unsigned es, unsigned bx,unsigned char dl)
{
	unsigned char al=0;

	cmd.ddev   = 0x70;
	cmd.dcomnd = 0x00;
	cmd.daux1  = dl;

	al = fujicom_command_read(&cmd,(unsigned char *)MK_FP(es,bx),512);
	return al;
}

void interrupt intf5(unsigned bp, unsigned di, unsigned si,
			unsigned ds, unsigned es, unsigned dx,
			unsigned cx, unsigned bx, unsigned ax)
{
	enable();
	switch(ax >> 8)
	{
		case 0x00: /* READY */
			ax=adapter_ready(es,bx,dx&0xFF);
			break;
		case 0xE8: /* GET ADAPTER CONFIG */
			get_adapter_config(es,bx);
			break;
	}

}

void main(void)
{
	fujicom_init(2);
	disable();
	setvect(0xF5,intf5);
	enable();
	keep(0,1024);
}
 
