/*
-------------------------------------------------------------------------
OBJECT NAME:	INDOEX.c

FULL NAME:  

ENTRY POINTS:	sfunc()

STATIC FNS:  

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:    

OUTPUT:    

REFERENCES:  

REFERENCED BY:  compute.c

COPYRIGHT:  	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void ogren1(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Aerosol_blk *)input)->BCUT);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void ogren2(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Aerosol_blk *)input)->BABS);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void ogren3(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Aerosol_blk *)input)->BSPD);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void ogren4(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Aerosol_blk *)input)->BSPW);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void ogren5(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Aerosol_blk *)input)->BSPWRH);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void mass1(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Mass_blk *)input)->mass1);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void mass2(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)((Mass_blk *)input)->mass2;

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void mass3(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)((Mass_blk *)input)->mass3;

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void mass4(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)((Mass_blk *)input)->mass4;

}  /* END SFUNC */

