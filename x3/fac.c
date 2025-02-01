/**
 * @brief   Display FujiNet Adapter Config
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <dos.h>

typedef struct _adapterConfig
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
} AdapterConfig;

unsigned char get_adapterconfig(AdapterConfig *ac)
{
	union  REGS r;
	struct SREGS sr;

	r.h.ah = 0x40; /* READ               */
	r.h.al = 0x70; /* FUJI Device        */
	r.h.cl = 0xe8; /* Get Adapter Config */
	r.x.di = sizeof(AdapterConfig);  /* Expecting 140 bytes */
	
	sr.es = FP_SEG(ac);
	r.x.bx = FP_OFF(ac);

	int86x(0xF5,&r,&r,&sr);

	return r.h.al;	
}

void display_adapterconfig(AdapterConfig *ac)
{
	printf("-------------------------------------\r\n");
	printf("Current FujiNet Network Configuration\r\n");
	printf("-------------------------------------\r\n\r\n");
	
	printf("%20s %s\n","SSID:",ac->ssid);
	printf("%20s %s\n","Host Name:",ac->hostname);

	printf("%20s %u.%u.%u.%u\n","IPV4:",ac->localIP[0],
					 ac->localIP[1],
					 ac->localIP[2],
					 ac->localIP[3]);

	printf("%20s %u.%u.%u.%u\n","Gateway:",ac->gateway[0],
					    ac->gateway[1],
					    ac->gateway[2],
					    ac->gateway[3]);

	printf("%20s %u.%u.%u.%u\n","Netmask:",ac->netmask[0],
					 ac->netmask[1],
					 ac->netmask[2],
					 ac->netmask[3]);

	printf("%20s %u.%u.%u.%u\n","DNS:",ac->dnsIP[0],
					 ac->dnsIP[1],
					 ac->dnsIP[2],
					 ac->dnsIP[3]);

	printf("%20s %02X:%02X:%02X:%02X:%02X:%02X\n","MAC:",ac->macAddress[0],
							   ac->macAddress[1],
							   ac->macAddress[2],
							   ac->macAddress[3],
							   ac->macAddress[4],
							   ac->macAddress[5]);

	printf("%20s %02X:%02X:%02X:%02X:%02X:%02X\n","BSSID:",ac->bssid[0],
							   ac->bssid[1],
							   ac->bssid[2],
							   ac->bssid[3],
							   ac->bssid[4],
							   ac->bssid[5]);

	printf("%20s %s\n","Version:",ac->fn_version);
}

int main(void)
{
	AdapterConfig ac;
	unsigned char r; 

	memset(&ac,0,sizeof(AdapterConfig));

	r = get_adapterconfig(&ac);

	if (r != 'C')
	{
		printf("get_adapterConfig returned: %02x",r);
		printf("Could not fetch adapter config from FujiNet. Exiting.");
		return 1;
	}

	display_adapterconfig(&ac);

	return 0;
}
