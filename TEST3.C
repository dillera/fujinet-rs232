/** 
 * @brief Read test
 */

#include <fujicom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char buf1[512], buf2[512];
cmdFrame_t c;

void main(void)
{
	int i=0;

	fujicom_init(2);

	for (i=0;i<256;i++)
	{
		int j=0;

		memset(buf1,i,sizeof(buf1));
		memset(buf2,0,sizeof(buf2));

		c.ddev = 0x70;
		c.dcomnd = 0x00;
		c.daux1 = i;

		printf("%c",fujicom_command_read(&c,buf2,sizeof(buf2)));

		for (j=0;j<sizeof(buf2);j++)
			if (buf1[j] != buf2[j])
			{
				printf("X");
				break;
			}

		printf(".");
	}

	fujicom_done();
}
