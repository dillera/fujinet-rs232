#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujicom.h"
#include "com.h"

cmdFrame_t cmd;

unsigned char buf[512];

int main(void)
{
    unsigned int sector = atoi(argv[1]);
    unsigned int i=0;
    
    if (argc<2)
    {
        printf("DSKREAD <0-sector number>\n");
        exit(1);
    }

    cmd.ddev = 0x31;  /* Drive 1 */
    cmd.dcomnd = 'R'; /* Read */
    cmd.daux1 = sector & 0xFF;
    cmd.daux2 = sector >> 8;

    printf("fujicom_command_read(%c) for sector %u",fujicom_command_read(&cmd, buf, sizeof(buf)),sector);

    for (i=0;i<sizeof(buf);i++)
    {
        printf("%02x ",buf[i]);
    }

    return 0;
}
