/*
-------------------------------------------------------------------------
OBJECT NAME:	avaps.c

FULL NAME:	AVAPS DropSonde

ENTRY POINTS:	xldstime(), xldsps(), xldsat(), xldsmr(), xldswd(), xldsws(),
		xldsvspd(), xldslon(), xldslat(), xldsgalt()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE	dstime[MAX_AVAPS], dsps[MAX_AVAPS], dsat[MAX_AVAPS],
		dswd[MAX_AVAPS], dsws[MAX_AVAPS], dsmr[MAX_AVAPS],
		dslat[MAX_AVAPS], dslon[MAX_AVAPS], dsalt[MAX_AVAPS],
		dsvspd[MAX_AVAPS];

extern char	*AVAPSrecord[];


/* -------------------------------------------------------------------- */
void xldsnum(RAWTBL *varp, void *input, NR_TYPE *output)
{
  int   avapsIndex;

  avapsIndex = atoi(strchr(varp->name, '_') + 1);
  *output = avapsIndex;

}       /* END XLDSNUM */

/* -------------------------------------------------------------------- */
void xldsat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  int	avapsIndex;
  char	localBuff[256], *p;

  avapsIndex = atoi(strchr(varp->name, '_')+1);

  if (strncmp(AVAPSrecord[avapsIndex], "AVAPS", 5))
    return;

  strncpy(localBuff, AVAPSrecord[avapsIndex], 256);

  p = strtok(localBuff, " ");
  p = strtok(NULL, " ");
  p = strtok(NULL, " ");
  p = strtok(NULL, " ");
  p = strtok(NULL, " ");
  dstime[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsps[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsat[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsmr[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dswd[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsws[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsvspd[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dslon[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dslat[avapsIndex] = atof(p);
  p = strtok(NULL, " ");
  dsalt[avapsIndex] = atof(p);

  *output = dsat[avapsIndex];

}	/* END XLDSID */

/* -------------------------------------------------------------------- */
void xldsps(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dsps[avapsIndex];

}	/* END XLDSPS */

/* -------------------------------------------------------------------- */
void xldstime(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dstime[avapsIndex];

}	/* END XLDSAT */

/* -------------------------------------------------------------------- */
void xldsmr(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dsmr[avapsIndex];

}	/* END XLDSMR */

/* -------------------------------------------------------------------- */
void xldswd(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dswd[avapsIndex];

}	/* END XLDSWD */

/* -------------------------------------------------------------------- */
void xldsws(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dsws[avapsIndex];

}	/* END XLDSWS */

/* -------------------------------------------------------------------- */
void xldsvspd(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dsvspd[avapsIndex];

}	/* END XLDSVSPD */

/* -------------------------------------------------------------------- */
void xldslon(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dslon[avapsIndex];

}	/* END XLDSLON */

/* -------------------------------------------------------------------- */
void xldslat(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dslat[avapsIndex];

}	/* END XLDSLAT */

/* -------------------------------------------------------------------- */
void xldsgalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
	int	avapsIndex = atoi(strchr(varp->name, '_')+1);

	*output = dsalt[avapsIndex];

}	/* END XLDSGALT */

/* END AVAPS.C */
