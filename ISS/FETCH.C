/**
 * Fetch routine
 */

#include <fujicom.h>
#include <string.h>

const char url[256]="N:HTTP://api.open-notify.org/iss-now.json";
const char query_lon[256]="N:/iss_position/longitude";
const char query_lat[256]="N:/iss_position/latitude";
const char query_ts[256]="N:/timestamp";
cmdFrame_t c;

struct _s
{
	unsigned short bw;
	unsigned char connected;
	unsigned char error;
} s;

void fetch(char *lat, char *lon, long *ts)
{
	char ts_s[16];

	memset(ts_s,0,sizeof(ts_s));

	fujicom_init(2);

	/* open */
	c.ddev    = 0x71;
	c.dcomnd = 'O';
	c.daux1  = 0x0C;
	c.daux2  = 0x00;
	fujicom_command_write(&c,(unsigned char *)url,sizeof(url));

	/* Set channel mode to JSON */
	c.dcomnd = 0xFC;
	c.daux1  = 0x00;
	c.daux2  = 0x01;
	fujicom_command(&c);

	/* Parse incoming JSON */
	c.dcomnd = 'P';
	fujicom_command(&c);

	/* Set query to timestamp */
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_ts,sizeof(query_ts));

	/* Get # of bytes waiting for timestamp */
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));

	/* Read Timestamp */
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,ts_s,s.bw);
	*ts=atol(ts_s);

	/* Set query for longitude */
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_lon,sizeof(query_lon));

	/* Get # of bytes waiting for timestamp */
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));

	/* Read Timestamp */
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,lon,s.bw);

	/* Set query for latitude */
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_lat,sizeof(query_lat));

	/* Get # of bytes waiting for timestamp */
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));

	/* Read Timestamp */
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,lat,s.bw);

	fujicom_done();
}
