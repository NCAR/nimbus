/*
-------------------------------------------------------------------------
OBJECT NAME:	float.c

FULL NAME:	Generic float array decoding

ENTRY POINTS:	xlfloat*()

STATIC FNS:	none

DESCRIPTION:	JPLTDL & LHTDL (Bruce Gandrud)

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlfloat0(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[0]);

}	/* END XLFLOAT0 */

/* -------------------------------------------------------------------- */
void xlfloat1(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[1]);

}	/* END XLFLOAT1 */

/* -------------------------------------------------------------------- */
void xlfloat2(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[2]);

}	/* END XLFLOAT2 */

/* -------------------------------------------------------------------- */
void xlfloat3(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[3]);

}	/* END XLFLOAT3 */

/* -------------------------------------------------------------------- */
void xlfloat4(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[4]);

}	/* END XLFLOAT4 */

/* -------------------------------------------------------------------- */
void xlfloat5(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[5]);

}	/* END XLFLOAT5 */

/* -------------------------------------------------------------------- */
void xlfloat6(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[6]);

}	/* END XLFLOAT6 */

/* -------------------------------------------------------------------- */
void xlfloat7(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[7]);

}	/* END XLFLOAT7 */

/* -------------------------------------------------------------------- */
void xlfloat8(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[8]);

}	/* END XLFLOAT8 */

/* -------------------------------------------------------------------- */
void xlfloat9(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ntohf(((float *)input)[9]);

}	/* END XLFLOAT9 */

/* END FLOAT.C */
