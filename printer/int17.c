#include <dos.h>
#include <i86.h>
#include <fujicom.h>
#include <stdint.h>
#include "print.h"
#include "commands.h"

static cmdFrame_t _cmdFrame;

#pragma data_seg("_CODE")

/*
 * DL		== direction
 * AL		== device
 * AH		== command
 * CL		== aux1
 * CH		== aux2
 * SI           == aux34
 * ES:BX	== far buffer pointer
 * DI		== buffer length
 */
int int17(uint16_t direction, uint16_t cmdchar, uint16_t aux12, uint16_t aux34,
	  void far *ptr, uint16_t length)
#pragma aux int17 parm [dx] [ax] [cx] [si] [es bx] [di] value [ax]
{
    int reply;
    unsigned char ah=cmdchar >> 8;
    unsigned char al=cmdchar & 0xFF;

    _enable();

    _cmdFrame.device = 0x40;
    _cmdFrame.comnd = 'W';

    switch (ah)
    {
    case 0:
        fujicom_command_write(&_cmdFrame,&al,1);
        return 0;
    case 1:
        return 0;
    case 2:
        return 0;
    }

    return 0;
}

void set17(void)
{
    extern void int17_vect();

    _dos_setvect(0x17, MK_FP(getCS(), int17_vect));
}
