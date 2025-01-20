#include <dos.h>

int Active_page(void)
{
	unsigned char page;

	_AH = 0x0F;
	geninterrupt(0x10);
	page = _BH;
	return page;
}

void Goto_XY(int col, int row)
{
	_BH = Active_page();
	_DH = --row;
	_DL = --col;
	_AH = 0x02;
	geninterrupt(0x10);
}

char Get_char(void)
{
	unsigned char chr;

	_BH = Active_page();
	_AH = 0x08;
	geninterrupt(0x10);
	chr = _AL;
	return chr;
}

unsigned int Get_key(unsigned char mode)
{
	unsigned int key;

	_AH = mode;
	geninterrupt(0x16);
	key = _AX;
	return key;
}

char Get_attr(void)
{
	unsigned char attr;

	_BH = Active_page();
	_AH = 0x08;
	geninterrupt(0x10);
	attr = _AH;
	return attr;
}

int Get_X(void)
{
	unsigned char col;

	_BH = Active_page();
	_AH = 0x03;
	
	geninterrupt(0x10);
	
	col = _DL;

	return ++col;
}

int Get_Y(void)
{
	unsigned char row;

	_BH = Active_page();
	_AH = 0x03;

	geninterrupt(0x10);

	row = _DH;

	return ++row;
}

unsigned char Get_mode(void)
{
	unsigned char mode;

	_AH = 0x0F;

	geninterrupt(0x10);

	mode = _AL;

	return mode;
}

void Set_mode(unsigned char mode)
{
	_AL = mode;
	_AH = 0x00;
	geninterrupt(0x10);
}

void Clear_screen(void)
{
	unsigned char mode;

	mode = Get_mode();

	_BH = 0x00;
	_CX = 0x0000;
	_DX = 0x184F; /* 24 rows, 79 columns */
	_AX = 0x0600; /* Clear all 25 rows */

	geninterrupt(0x10);

	Set_mode(mode);

	Goto_XY(1,1);
}

void Write_chr(unsigned char chr)
{
	unsigned char attr;

	attr = Get_attr();
	_BL = attr;
	_AL = chr;
	_AH = 0x0E;
	geninterrupt(0x10);
}

static unsigned int es_static;
static unsigned int bp_static;

void Write_tty(unsigned char *str)
{
	unsigned char x;
	unsigned char y;
	unsigned int len;
	unsigned char page;
	unsigned char attr;

	x = Get_X();
	y = Get_Y();
	len = strlen(str);
	page = Active_page();
	attr = Get_attr();

	es_static = _ES;
	bp_static = _BP;

	_CX = len;
	_DH = --y;
	_DL = --x;
	_BH = page;
	_BL = attr;
	_ES = _DS;
	_BP = str;
	_AX = 0x1301;

	geninterrupt(0x10);

	_BP = bp_static;
	_ES = es_static;
}

