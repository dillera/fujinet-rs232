/**
 * OSD
 */

#include <stdio.h>
#include <time.h> 
#include "grlib.h"

void osd(char *lat, char *lon, long ts)
{
	char t[41];

	gr_text(10,21,"CURRENT ISS POSITION");
	sprintf(t,"        %s",ctime(ts));
	gr_text(0,22,t);
	sprintf(t,"           LATITUDE: %10s",lat);
	gr_text(0,23,t);
	sprintf(t,"          LONGITUDE: %10s",lon);
	gr_text(0,24,t);
}
