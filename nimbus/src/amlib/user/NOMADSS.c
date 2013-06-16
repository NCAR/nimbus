/*
-------------------------------------------------------------------------
OBJECT NAME:	NOMADSS.c

FULL NAME:	

DESCRIPTION:	For the Univ of Wash folks who need a cabin relative humidity

COPYRIGHT:	University Corporation for Atmospheric Research, 2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE denominator = 3.8235391; // exp(17.67*(20/263.5) 

//static NR_TYPE	cal[] = { 58.915, 33.57, 1.0 };

static NR_TYPE	esw(NR_TYPE T);


/* -------------------------------------------------------------------- */
// A calculation of cabin RH at 20 degrees celcius given to me by 
// Jesse Ambrose of UofW during NOMADSS campaign - Tom Baltzer

void srhcab(DERTBL *varp)
{
  NR_TYPE	atx, rhum, rhum20cab, numerator;

  rhum = GetSample(varp, 0);
  atx = GetSample(varp, 1);

  numerator = exp(17.67*(atx/(atx+243.5)));

  rhum20cab = rhum * (numerator/denominator);

  //printf("rhcab calculation. %f*(%f/%f) = %f\n", rhum, numerator, denominator, rhum20cab);

  PutSample(varp, rhum20cab);

}

/* -------------------------------------------------------------------- */
//  A calculation based on information from Stuart Beaton in an e-mail to
//  Jorgen on 6/11/2013


void sewuvh(DERTBL *varp)
{
  NR_TYPE	xsigv, xcelltemp, psx, xcellpres, n, ewcell, ewuvh;

  xsigv = GetSample(varp, 0);
  xcelltemp = GetSample(varp, 1);
  psx = GetSample(varp, 1);
  xcellpres = GetSample(varp, 1);

  n = -2e17*log((xsigv - 0.34)/1.055);
  ewcell = n * 1.38065e-23 * (xcelltemp+273.14)*10000;
  ewuvh = ewcell*(psx/xcellpres);

  //printf("rhcab calculation. %f*(%f/%f) = %f\n", rhum, numerator, denominator, rhum20cab);

  PutSample(varp, ewuvh);

}

/* -------------------------------------------------------------------- */

