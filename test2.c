#include <fujicom.h>
#include <stdio.h>

cmdFrame_t cmd;

void main(void)
{
	int tries=1024;

	while(tries--)
	{
		cmd.ddev = 0x70;
		cmd.dcomnd = 0x00;
		fujicom_init(2);
		printf("%c",fujicom_command(&cmd));
		fujicom_done();
	}
}
