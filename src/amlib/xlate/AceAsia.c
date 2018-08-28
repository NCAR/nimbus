/*
-------------------------------------------------------------------------
OBJECT NAME:	AceAsia.c

FULL NAME:	Translate RDMA houskeeping variables to Nimbus record
		format

ENTRY POINTS:	xlrdma*()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlNephP10(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Neph_UW_blk *)input)->tsG_1);

}

/* -------------------------------------------------------------------- */
void xlNephT10(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Neph_UW_blk *)input)->tsG_10);

}

/* -------------------------------------------------------------------- */
void xlNephRH10(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Neph_UW_blk *)input)->tsG_180);

}

/* -------------------------------------------------------------------- */
void xlNephTSG10(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Neph_UW_blk *)input)->tsG_180);

}

