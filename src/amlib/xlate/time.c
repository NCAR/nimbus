/*
-------------------------------------------------------------------------
OBJECT NAME:	time.c

FULL NAME:	Translate time to new record format

ENTRY POINTS:	xlhour(), xlmin(), xlsec(), xlyear(), xlmonth(), xlday()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-7
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern int FlightDate[]; /* 0 = month, 1 = day, 2 = year */

/* -------------------------------------------------------------------- */
void xlhour(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->hour);

}	/* END XLHOUR */

/* -------------------------------------------------------------------- */
void xlmin(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->minute);

}	/* END XLMIN */

/* -------------------------------------------------------------------- */
void xlsec(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->second);

}	/* END XLSEC */

/* -------------------------------------------------------------------- */
void xlyear(RAWTBL *varp, void *input, NR_TYPE *output)
{
  if (HDRversion < 2.99999)
    *output = FlightDate[2];
  else
    *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->year);

  if (*output < 1000)	// We now have year 2070 problem.
    if (*output < 70)
      *output += 2000;
    else
      *output += 1900;

}	/* END XLYEAR */

/* -------------------------------------------------------------------- */
void xlmonth(RAWTBL *varp, void *input, NR_TYPE *output)
{
  if (HDRversion < 2.99999)
    {
    *output = FlightDate[0];	/* Month from ADS header */
    
/* This needs to be fixed.  Add check for End of Month
    Hdr_blk *h = (Hdr_blk *)input;

    if (h->hour == 0 && h->minute == 0 && h->second == 0)
      *output += 1.0;
*/
    }
  else
    *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->month);

}	/* END XLMONTH */

/* -------------------------------------------------------------------- */
void xlday(RAWTBL *varp, void *input, NR_TYPE *output)
{
  if (HDRversion < 2.99999)
    {
    Hdr_blk *h = (Hdr_blk *)input;

    *output = FlightDate[1];	/* Day from ADS header	*/
    
    /* Midnight rollover */
    if (h->hour == 0 && h->minute == 0 && h->second == 0)
      *output += 1.0;
    }
  else
    *output = (NR_TYPE)ntohs(((Hdr_blk *)input)->day);

}	/* END XLDAY */

/* END TIME.C */
