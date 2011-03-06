/*
-------------------------------------------------------------------------
OBJECT NAME:	irsL51.c

FULL NAME:	Convert Litton 51 Intertial block

ENTRY POINTS:	xlinssec(), xlalon(), xlalat(), xlgsi(), xltrki(), xlthi(),
		xlalpha(), xlxvi(), xlyvi(), xlpitch51(), xlcroll51(),
		xlroll51(), xlphdg51(), xlvzi51()

STATIC FNS:	angle()
		velocity()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/ctape.h>

#define	INS_MASK	0x0003ffff
#define MASK		0x3ffff000
#define MSIGN		0x40000000

static double	angle(long), velocity(long);
NR_TYPE	angcr(float,float,float,float);


/* -------------------------------------------------------------------- */
void xlinssec(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Ins_blk *)input)->second + ((Ins_blk *)input)->t250 / 250.0;
}

/* -------------------------------------------------------------------- */
void xlalat(RAWTBL *varp, void *input, NR_TYPE *output)
{
	NR_TYPE	alat;

	if ((alat = (NR_TYPE)angle(((Ins_blk *)input)->lat)) > 180.0)
		alat -= 360.0;

	*output = (NR_TYPE)alat;

}	/* END XLALAT */

/* -------------------------------------------------------------------- */
void xlalon(RAWTBL *varp, void *input, NR_TYPE *output)
{
	NR_TYPE	alon;

	if ((alon = (NR_TYPE)angle(((Ins_blk *)input)->lon)) > 180.0)
		alon -= 360.0;

	*output = (NR_TYPE)alon;

}	/* END XLALON */

/* -------------------------------------------------------------------- */
void xlgsi(RAWTBL *varp, void *input, NR_TYPE *output)
{
	*output = (NR_TYPE)((((Ins_blk *)input)->gndspd >> 12) & INS_MASK) * KNOTS_TO_MPS;
	
}	/* END XLGSI */

/* -------------------------------------------------------------------- */
void xltrki(RAWTBL *varp, void *input, NR_TYPE *output)
{
	*output = (NR_TYPE)fmod(angle(((Ins_blk *)input)->track)+360.0, (double)360.0);

}	/* END XLTRKI */

/* -------------------------------------------------------------------- */
void xlthi(RAWTBL *varp, void *input, NR_TYPE *output)
{
	output[0] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->truehd1)+360.0, (double)360.0);
	output[1] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->truehd2)+360.0, (double)360.0);
	output[2] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->truehd3)+360.0, (double)360.0);
	output[3] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->truehd4)+360.0, (double)360.0);
	output[4] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->truehd5)+360.0, (double)360.0);

}	/* END XLTHI */

/* -------------------------------------------------------------------- */
void xlalpha(RAWTBL *varp, void *input, NR_TYPE *output)
{
	output[0] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->alpha1)+360.0, (double)360.0);
	output[1] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->alpha2)+360.0, (double)360.0);
	output[2] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->alpha3)+360.0, (double)360.0);
	output[3] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->alpha4)+360.0, (double)360.0);
	output[4] = (NR_TYPE)fmod(angle(((Ins_blk *)input)->alpha5)+360.0, (double)360.0);

}	/* END XLALPHA */

/* -------------------------------------------------------------------- */
void xlxvi(RAWTBL *varp, void *input, NR_TYPE *output)
{
	output[0] = (NR_TYPE)velocity(((Ins_blk *)input)->vx1) * 0.3048;
	output[1] = (NR_TYPE)velocity(((Ins_blk *)input)->vx2) * 0.3048;
	output[2] = (NR_TYPE)velocity(((Ins_blk *)input)->vx3) * 0.3048;
	output[3] = (NR_TYPE)velocity(((Ins_blk *)input)->vx4) * 0.3048;
	output[4] = (NR_TYPE)velocity(((Ins_blk *)input)->vx5) * 0.3048;
	output[5] = (NR_TYPE)velocity(((Ins_blk *)input)->vx6) * 0.3048;
	output[6] = (NR_TYPE)velocity(((Ins_blk *)input)->vx7) * 0.3048;
	output[7] = (NR_TYPE)velocity(((Ins_blk *)input)->vx8) * 0.3048;
	output[8] = (NR_TYPE)velocity(((Ins_blk *)input)->vx9) * 0.3048;
	output[9] = (NR_TYPE)velocity(((Ins_blk *)input)->vx10) * 0.3048;

}	/* END XLXVI */

/* -------------------------------------------------------------------- */
void xlyvi(RAWTBL *varp, void *input, NR_TYPE *output)
{
	output[0] = (NR_TYPE)velocity(((Ins_blk *)input)->vy1) * 0.3048;
	output[1] = (NR_TYPE)velocity(((Ins_blk *)input)->vy2) * 0.3048;
	output[2] = (NR_TYPE)velocity(((Ins_blk *)input)->vy3) * 0.3048;
	output[3] = (NR_TYPE)velocity(((Ins_blk *)input)->vy4) * 0.3048;
	output[4] = (NR_TYPE)velocity(((Ins_blk *)input)->vy5) * 0.3048;
	output[5] = (NR_TYPE)velocity(((Ins_blk *)input)->vy6) * 0.3048;
	output[6] = (NR_TYPE)velocity(((Ins_blk *)input)->vy7) * 0.3048;
	output[7] = (NR_TYPE)velocity(((Ins_blk *)input)->vy8) * 0.3048;
	output[8] = (NR_TYPE)velocity(((Ins_blk *)input)->vy9) * 0.3048;
	output[9] = (NR_TYPE)velocity(((Ins_blk *)input)->vy10) * 0.3048;

}	/* END XLYVI */

/* -------------------------------------------------------------------- */
static double angle(long value)
{
	NR_TYPE	angle;
	long	issigned = value & MSIGN;

	value = (value & MASK) >> 12;

	if (issigned)
		value = ~value & INS_MASK;

	angle = (NR_TYPE)(value * 360) / 262144.0;

	if (issigned)
		angle = -angle;

	return(angle);

}	/* END ANGLE */

/* -------------------------------------------------------------------- */
static double velocity(long value)
{
	NR_TYPE	velocity;
	long	issigned = value & MSIGN;

	value = (value & MASK) >> 12;

	if (issigned)
		value = ~value & INS_MASK;

	velocity = (NR_TYPE)value / 8.0;

	if (issigned)
		velocity = -velocity;

	return(velocity);

}	/* END VELOCITY */

/* -------------------------------------------------------------------- */
void xlpitch51(RAWTBL *varp, void *input, NR_TYPE *output)
{
	short	*p = (short *)input;
	NR_TYPE	pitch;

	for (size_t i = 0; i < varp->SampleRate; ++i)
		{
		pitch = ~p[i * varp->ADSoffset] * RESOLV14BIT * 0.125;
		pitch = angcr(pitch, (NR_TYPE)0.0, (float)45.0, (float)-180.0);

		output[i] = (NR_TYPE)pitch;
		}

}	/* END XLPITCH51 */

/* -------------------------------------------------------------------- */
static NR_TYPE	croll[50];

void xlcroll51(RAWTBL *varp, void *input, NR_TYPE *output)
{
	short	*p = (short *)input;

	for (size_t i = 0; i < varp->SampleRate; ++i)
		output[i] = 
		croll[i] = (NR_TYPE)~p[i * varp->ADSoffset] * RESOLV14BIT;

}	/* END XLCROLL51 */

/* -------------------------------------------------------------------- */
void xlroll51(RAWTBL *varp, void *input, NR_TYPE *output)
{
	short	*p = (short *)input;
	NR_TYPE	roll;

	for (size_t i = 0; i < varp->SampleRate; ++i)
		{
		roll = ~p[i * varp->ADSoffset] * RESOLV14BIT * 0.125;
		roll = angcr(roll, croll[i], (float)45.0, (float)-180.0);

		output[i] = (NR_TYPE)roll;
		}

}	/* END XLROLL51 */

/* -------------------------------------------------------------------- */
void xlphdg51(RAWTBL *varp, void *input, NR_TYPE *output)
{
	short	*p = (short *)input;

	for (size_t i = 0; i < varp->SampleRate; ++i)
		output[i] = (NR_TYPE)~p[i*varp->ADSoffset] * RESOLV14BIT * 0.125;

}	/* END XLPHDG51 */

/* -------------------------------------------------------------------- */
void xlvzi51(RAWTBL *varp, void *input, NR_TYPE *output)
{
	NR_TYPE		vzi, vzidif;
	static bool	firstTime = TRUE;
	static int	ncvzi;
	static NR_TYPE	vzi0;

	if (firstTime)
		{
		vzi0 = ((short *)input)[0] * 0.012192;
		ncvzi = 0;
		firstTime = FALSE;
		}

	for (size_t i = 0; i < varp->SampleRate; ++i)
		{
		vzi	= ((short *)input)[i] * 0.012192;
		vzidif	= vzi - vzi0;
		vzi0	= vzi;

		if (fabs((double)vzidif) > 600.0)
			{
			if (vzidif < 0.0)
				ncvzi += 800;
			else
				ncvzi -= 800;
			}

		vzi += ncvzi;

		output[i] = (NR_TYPE)vzi;
		}

}	/* END XLVZI51 */

/* END IRSL51.C */
