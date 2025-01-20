#include <dos.h>
#include "dos_dd.h"

extern	unsigned Init_cmd();		/* INIT Command */
extern	unsigned Media_check_cmd();	/* Media Check Command */
extern  unsigned Build_bpb_cmd();       /* Build BPB Command */
extern	unsigned Ioctl_input_cmd();	/* IOCTL Input Command */
extern	unsigned Input_cmd();		/* Input Command */
extern	unsigned Input_no_wait_cmd();	/* Input No Wait Command */
extern	unsigned Input_status_cmd();	/* Input Status Command */
extern	unsigned Input_flush_cmd();	/* Input Flush Command */
extern	unsigned Output_cmd();		/* Output Command */
extern	unsigned Output_verify_cmd();	/* Output Verify Command */
extern	unsigned Output_status_cmd();	/* Output Status Command */
extern	unsigned Output_flush_cmd();	/* Output Flush Command */
extern	unsigned Ioctl_output_cmd();	/* IOCTL Output Command */
extern	unsigned Dev_open_cmd();	/* Device Open Command */
extern  unsigned Dev_close_cmd();	/* Device Close Command */
extern	unsigned Remove_media_cmd();	/* Remove media command */
extern	unsigned Ioctl_cmd();		/* GENERIC IOCTL Command */
extern	unsigned Get_l_d_map_cmd();	/* GET Logical Device Map */
extern	unsigned Set_l_d_map_cmd();	/* SET Logical Device Map */
extern  unsigned Unknown_cmd();		/* UNKNOWN Command Default */

extern	unsigned rc;			/* Function Return Code */
extern  unsigned driver;		/* Global Driver Variable */
extern  unsigned SS_reg;		/* SS Register Variable */
extern  unsigned SP_reg;		/* SP Register Variable */
extern  unsigned ES_reg;		/* ES Register Variable */
extern	unsigned AX_reg;		/* AX Register Variable */
extern	unsigned BX_reg;		/* BX Register Variable */
extern	unsigned CX_reg;		/* CX Register Variable */
extern	unsigned DX_reg;		/* DX Register Variable */
extern	unsigned DS_reg;		/* DS Register Variable */
extern  unsigned SI_reg;		/* SI Register Variable */

extern	unsigned local_stk[STK_SIZE];

extern	struct REQ_struct far *r_ptr;

unsigned (*dos_cmd[DOS_CMDS]) (struct REQ_struct far *r_ptr) =
{
	Init_cmd,
	Media_check_cmd,
	Build_bpb_cmd,
	Ioctl_input_cmd,
	Input_cmd,
	Input_no_wait_cmd,
	Input_status_cmd,
	Input_flush_cmd,
	Output_cmd,
	Output_verify_cmd,
	Output_status_cmd,
	Output_flush_cmd,
	Ioctl_output_cmd,
	Dev_open_cmd,
	Dev_close_cmd,
	Remove_media_cmd,
	Unknown_cmd,
	Unknown_cmd,
	Unknown_cmd,
	Ioctl_cmd,
	Unknown_cmd,
	Unknown_cmd,
	Unknown_cmd,
	Get_l_d_map_cmd,
	Set_l_d_map_cmd
};

void DOS_Setup (unsigned int which,
		unsigned int ES_tmp,
		unsigned int DS_tmp,
		unsigned int AX_tmp)
{
    _AX = _CS;			/* Obtain Code Segment */
    _DS = _AX;			/* Setup Data Segment */

    BX_reg = _BX;		/* Save BX */
    CX_reg = _CX;		/* Save CX */
    DX_reg = _DX;		/* Save DX */

    AX_reg = AX_tmp;		/* Save AX */
    ES_reg = ES_tmp;		/* Save Request Pointer */

    driver = which;		/* Move value from stack */

    SS_reg = _SS;		/* Save Stack Segment */
    SP_reg = _SP;		/* Save Stack Pointer */

    disable();			/* Disable Interrupts */

    _AX = _DS;			/* Obtain Data Segment */
    _SS = _AX;			/* Setup new stack */

    _SP = (unsigned int) &local_stk[STK_SIZE];

    enable();			/* Enable Interrupts */

    if (driver)
    {
	rc = 0x0000;		/* Clear return code */

	r_ptr = MK_FP(ES_reg, BX_reg);

	if (r_ptr->command >= DOS_CMDS)
	{
	    rc = ERROR_BIT | UNKNOWN_CMD; /* return error */
	}
	else
	{
	    rc |= (*dos_cmd[r_ptr->command])(r_ptr);
	}

	r_ptr->status = rc | DONE_BIT; /* return done */
    }
    else
    {
	/* Why is this empty ? */
    }

    disable();
    _SS = SS_reg;
    _SP = SP_reg;
    enable();

    _DX = DX_reg;
    _CX = CX_reg;
    _BX = BX_reg;
    _AX = AX_reg;

    _ES = ES_tmp;
    _DS = DS_tmp;
}

void far Strategy(void)
{
#ifdef DEBUG
   geninterrupt(0x03); /* Break to debugger */
#endif /* DEBUG */

   DOS_Setup(0x00,_ES,_DS,_AX);
}

void far Interrupt(void)
{
#ifdef DEBUG
   geninterrupt(0x03);
#endif /* DEBUG */

   DOS_Setup(0x01,_ES,_DS,_AX);
}
