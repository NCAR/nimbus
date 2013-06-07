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
void srhcab(DERTBL *varp)
{
  NR_TYPE	atx, rhum, rhum20cab, numerator;

  rhum = GetSample(varp, 0);
  atx = GetSample(varp, 1);

  numerator = exp(17.67*(atx/(atx*243.5)));

  rhum20cab = rhum * (numerator/denominator);

  //printf("rhcab calculation. %f*(%f/%f) = %f\n", rhum, numerator, denominator, rhum20cab);

  PutSample(varp, rhum20cab);

}

/* -------------------------------------------------------------------- */
