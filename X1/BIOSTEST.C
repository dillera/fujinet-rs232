#include <dos.h>
#include <stdio.h>

struct _adapterConfig
{
	char ssid[33];
	char hostname[64];
	unsigned char localIP[4];
	unsigned char gateway[4];
	unsigned char netmask[4];
	unsigned char dnsIP[4];
	unsigned char macAddress[6];
	unsigned char bssid[6];
	char fn_version[15];
} ac;

char test[5] = {'B','L','A','H',0};

void main(void)
{
	union REGS r;
	struct SREGS s;

	r.h.ah = 0x00;
	r.x.bx = FP_OFF(&test);
	s.es = FP_SEG(&test);

	int86x(0xF5,&r,&r,&s);

	puts(test);

	printf("AH %c",r.h.ah);
}
