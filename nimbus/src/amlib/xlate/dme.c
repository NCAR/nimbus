/*
-------------------------------------------------------------------------
OBJECT NAME:	dme.c

FULL NAME:	Translate DME block

ENTRY POINTS:	xldmedi(), xldmefl(), xldmech()

DESCRIPTION:	Computes dme distance, channel and flag

INPUT:		DME block pointer

OUTPUT:		desired value

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* Look-up table for bi-quinary to decimal */
static int	look[] = {9, 24, 20, 12, 10, 6, 5, 3, 18, 17};

static NR_TYPE	dmech, dmefl, dmedi;


/* -------------------------------------------------------------------- */
void xldmedi(RAWTBL *varp, void *pdme, NR_TYPE *output)
{
  int	i;
  int	dme1, dme2, dme3;  /* temp storage for raw data */
  int	itemp;
  NR_TYPE	ftemp;

  dme1 = ntohl(((Dme_blk *)pdme)->freq);
  dme3 = ntohl(((Dme_blk *)pdme)->dist);
  dme2 = dme3 >> 16;

  /* Convert the channel number from bi-quinary to megahertz */ 
  ftemp = (NR_TYPE)(dme1 & 1) / 20.0;
  itemp = (dme1 & 0x3e) >> 1;   

  for (i = 0; i < 10; i++)
    if (look[i] == itemp)
      break;

  ftemp += (NR_TYPE)i / 10.0;
  itemp = (dme1 & 0x7c0) >> 6; 

  for (i = 0; i < 10; i++)
    if (look[i] == itemp)
      break;

  ftemp += i;
  itemp = (dme1 & 0x1800) >> 11;
  ftemp += (itemp - 1) * 10 + 100;
  dmech = ftemp;


  /* Convert the data over into usable units */
  ftemp = (NR_TYPE)((dme3 & 0xf0) >> 4) / 100.0 +
          (NR_TYPE)(dme2 & 0x0f) / 10.0 + ((dme2 & 0xf0) >> 4);

  dmedi = ftemp + ((dme2 & 0xf00) >> 8) * 10 + ((dme2 & 0x3000) >> 12) * 100;

  dmefl = (dme2 & 0xc000) >> 14;

  *output = dmedi;
}

/* -------------------------------------------------------------------- */
void xldmech(RAWTBL *varp, void *p, NR_TYPE *output)
{
  *output = dmech;
}

/* -------------------------------------------------------------------- */
void xldmefl(RAWTBL *varp, void *p, NR_TYPE *output)
{
  *output = dmefl;
}

/* END DME.C */
