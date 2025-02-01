/**
 * Test harness
 */

#include <stdio.h>
#include "fujicom.h"

cmdFrame_t c;

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

void main(void)
{
	char r;

	c.ddev=0x70;
	c.dcomnd=0xE8;

	fujicom_init(2);
	r = fujicom_command_read(&c,(unsigned char *)&ac,sizeof(ac));

	printf("R: %c\n",r);

	printf("%20s %s\n","SSID:",ac.ssid);
	printf("%20s %s\n","Host Name:",ac.hostname);

	printf("%20s %u.%u.%u.%u\n","IPV4:",ac.localIP[0],
					 ac.localIP[1],
					 ac.localIP[2],
					 ac.localIP[3]);

	printf("%20s %u.%u.%u.%u\n","Gateway:",ac.gateway[0],
					    ac.gateway[1],
					    ac.gateway[2],
					    ac.gateway[3]);

	printf("%20s %u.%u.%u.%u\n","Netmask:",ac.netmask[0],
					 ac.netmask[1],
					 ac.netmask[2],
					 ac.netmask[3]);

	printf("%20s %u.%u.%u.%u\n","DNS:",ac.dnsIP[0],
					 ac.dnsIP[1],
					 ac.dnsIP[2],
					 ac.dnsIP[3]);

	printf("%20s %02X:%02X:%02X:%02X:%02X:%02X\n","MAC:",ac.macAddress[0],
							   ac.macAddress[1],
							   ac.macAddress[2],
							   ac.macAddress[3],
							   ac.macAddress[4],
							   ac.macAddress[5]);

	printf("%20s %02X:%02X:%02X:%02X:%02X:%02X\n","BSSID:",ac.bssid[0],
							   ac.bssid[1],
							   ac.bssid[2],
							   ac.bssid[3],
							   ac.bssid[4],
							   ac.bssid[5]);

	printf("%20s %s\n","Version:",ac.fn_version);

	fujicom_done();
} 
 
