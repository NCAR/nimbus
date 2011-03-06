/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsHw.c

FULL NAME:	Convert Honeywell GPS block data

ENTRY POINTS:	xlghmstat(), xlghutc(), xlghalt(), xlghhdop(), xlghvdop(),
		xlghlat(), xlghlon(), xlghlatf(), xlghlonf(), xlghgspd(),
		xlghvns(), xlghvew(), xlghvzi(), xlghstat(), xlghvfom(),
		xlghhfom()

DESCRIPTION:	Honeywell GPS data is stored in Arinc 429 format.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const int	transportDelay = 0;


/* -------------------------------------------------------------------- */
void xlghmstat(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((HwGps_blk *)p)->meas_status;

}	/* END XLGHMSTAT */

/* -------------------------------------------------------------------- */
void xlghutc(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (Seconds) 20 bits, +- 10.0 Seconds, label 074
	 */
	x = (((HwGps_blk *)p)->utc_time & 0x7fffffff) >> 11;
	*np = x * 9.536743;

}	/* END XLGHUTC */

/* -------------------------------------------------------------------- */
void xlghalt(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (M) 20 bits, +- 131072.0 Ft, label 076
	 */
	x = (((HwGps_blk *)p)->altitude & 0x7fffffff) >> 11;
	*np = x * 0.125;

	varp->DynamicLag = transportDelay;

}	/* END XLGHALT */

/* -------------------------------------------------------------------- */
void xlghhdop(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (N/A) 15 bits, +- 1024.0, label 101
	 */
	x = (((HwGps_blk *)p)->hdop & 0x7fffffff) >> 16;
	*np = x * 0.031;

}	/* END XLGHHDOP */

/* -------------------------------------------------------------------- */
void xlghvdop(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (N/A) 15 bits, +- 1024.0, label 102
	 */
	x = (((HwGps_blk *)p)->vdop & 0x7fffffff) >> 16;
	*np = x * 0.031;

}	/* END XLGHVDOP */

/* -------------------------------------------------------------------- */
void xlghlat(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (degs) 21 bits, +- 1.0 pirad, label 110
	 */
	x = (((HwGps_blk *)p)->latitude & 0x7fffffff) >> 11;
	*np = x * 0.000172;

	varp->DynamicLag = transportDelay;

}	/* END XLGHLAT */

/* -------------------------------------------------------------------- */
void xlghlon(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (degs) 21 bits, +- 1.0 pirad, label 111
	 */
	x = (((HwGps_blk *)p)->longitude & 0x7fffffff) >> 11;
	*np = x * 0.000172;

	varp->DynamicLag = transportDelay;

}	/* END XLGHLON */

/* -------------------------------------------------------------------- */
void xlghlatf(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (degs) 11 bits, +- 1.0 pirad, label 120
	 */
	x = (((HwGps_blk *)p)->lat_fine & 0x7fffffff) >> 19;
	*np = x * 8.38e-8;

	varp->DynamicLag = transportDelay;

}	/* END XLGHLATF */

/* -------------------------------------------------------------------- */
void xlghlonf(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (degs) 11 bits, +- 1.0 pirad, label 121
	 */
	x = (((HwGps_blk *)p)->long_fine & 0x7fffffff) >> 19;
	*np = x * 8.38e-8;

	varp->DynamicLag = transportDelay;

}	/* END XLGHLONF */

/* -------------------------------------------------------------------- */
void xlghgspd(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (M/s) 15 bits, 0 to 4095 kts, label 112
	 */
	x = (((HwGps_blk *)p)->ground_speed & 0x7fffffff) >> 16;
	*np = x * 0.125 * KNOTS_TO_MPS;

	varp->DynamicLag = transportDelay;

}	/* END XLGHGSPD */

/* -------------------------------------------------------------------- */
void xlghvzi(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (M/s) 15 bits, +-32768 kts, label 165
	 */
	x = (((HwGps_blk *)p)->vertical_velocity & 0x7fffffff) >> 16;
	*np = x * FTMIN;

	varp->DynamicLag = transportDelay;

}	/* END XLGHVZI */

/* -------------------------------------------------------------------- */
void xlghvew(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (M/s) 15 bits, +-4096 kts, label 167
	 */
	x = (((HwGps_blk *)p)->velocity_ew & 0x7fffffff) >> 16;
	*np = x * 0.125 * KNOTS_TO_MPS;

	varp->DynamicLag = transportDelay;

}	/* END XLGHVEW */

/* -------------------------------------------------------------------- */
void xlghvns(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (M/s) 15 bits, +-4096 kts, label 166
	 */
	x = (((HwGps_blk *)p)->velocity_ns & 0x7fffffff) >> 16;
	*np = x * 0.125 * KNOTS_TO_MPS;

	varp->DynamicLag = transportDelay;

}	/* END XLGHVNS */

/* -------------------------------------------------------------------- */
void xlghvfom(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (m) 15 bits, +1024 kts, label 136
	 */
	x = (((HwGps_blk *)p)->vertical_fom & 0x7fffffff) >> 16;
	*np = x * 0.031;

	varp->DynamicLag = transportDelay;

}	/* END XLGHVFOM */

/* -------------------------------------------------------------------- */
void xlghhfom(RAWTBL *varp, void *p, NR_TYPE *np)
{
	int	x;

	/* (m) 15 bits, +1024 kts, label 247
	 */
	x = (((HwGps_blk *)p)->horizontal_fom & 0x7fffffff) >> 16;
	*np = x * 0.031;

	varp->DynamicLag = transportDelay;

}	/* END XLGHHFOM */

/* -------------------------------------------------------------------- */
void xlghstat(RAWTBL *varp, void *p, NR_TYPE *np)
{
	/* (N/A) N/A bits, +-N/A, label 273
	 */
	*np = ((HwGps_blk *)p)->sensor_status;

}	/* END XLGHSTAT */

/* END GPSHW.C */
