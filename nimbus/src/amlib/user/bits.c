/*
-------------------------------------------------------------------------
OBJECT NAME:	bits.c

FULL NAME:	Extract individual bits.

ENTRY POINTS:	sbit0()
		sbit1()
		sbit2()
		sbit3()
		sbit4()
		sbit5()
		sbit6()
		sbit7()

STATIC FNS:	none

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sbit0(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0001 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT0 */

/* -------------------------------------------------------------------- */
void sbit1(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0002 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT1 */

/* -------------------------------------------------------------------- */
void sbit2(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0004 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT2 */

/* -------------------------------------------------------------------- */
void sbit3(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0008 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT3 */

/* -------------------------------------------------------------------- */
void sbit4(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0010 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT4 */

/* -------------------------------------------------------------------- */
void sbit5(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0020 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT5 */

/* -------------------------------------------------------------------- */
void sbit6(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0040 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT6 */

/* -------------------------------------------------------------------- */
void sbit7(DERTBL *varp)
{
	short	flag;

	flag = (short)GetSample(varp, 0) & 0x0080 ? TRUE : FALSE;
	PutSample(varp, (NR_TYPE)flag);

}	/* END SBIT7 */

/* END BITS.C */
