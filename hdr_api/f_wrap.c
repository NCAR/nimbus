/*
-------------------------------------------------------------------------
OBJECT NAME:	f_wrap.c

FULL NAME:	Fortran Wrappers

ENTRY POINTS:	All

DESCRIPTION:	This module contains all the C wrapper procedures for
		Fortran 77 support.

REFERENCES:	All C modules of same name.

REFERENCED BY:	User application Fortran programs

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <memory.h>
#include "hdr_api.h"

#define SPACES(n)	&spaces[12-(n)]

char	*spaces = "            ";


/* -------------------------------------------------------------------- */
initflightheader_(filename, action)
char	*filename;
int	action;
{
	return(InitFlightHeader(filename, action));

}

/* -------------------------------------------------------------------- */
releaseflightheader_()
{
	ReleaseFlightHeader();

}

/* --------------------------------------------------------------------
 * START Group 1
 */

/* -------------------------------------------------------------------- */
/* Not currently supported for FORTRAN, make the user construct their
 * won list with getfirst & getnext.  --Chris
 *
getvariablelist_(p, l, indx)
char	p[][LEN_8];
int	l;
{
	int	i, j;
	char	**vl, *s, *t;

	vl = GetVariableList();

	for (i = 0; vl[i]; ++i)
		{
		s = vl[i];
		t = p[i];

		for (j = 0; *t++ = *s++; ++j)
			;

		for (; j < LEN_8; ++j)
			*t++ = SPACE;
		}

}
*/
/* -------------------------------------------------------------------- */
getfirst_(p, l)
char	*p;
int	l;
{
	int	i;

	strcpy(p, GetFirst());

	for (i = strlen(p); i < l; ++i)
		p[i] = SPACE;

}

/* -------------------------------------------------------------------- */
getnext_(p, l)
char	*p;
int	l;
{
	int	i;
	char	*rc;

	if (rc = GetNext())
		strcpy(p, rc);
	else
		p[0] = '\0';

	for (i = strlen(p); i < l; ++i)
		p[i] = SPACE;
}

/* --------------------------------------------------------------------
 * START Group 2
 */

/* -------------------------------------------------------------------- */
getversion_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetVersion(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getprojectnumber_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetProjectNumber(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getflightnumber_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetFlightNumber(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
gettapenumber_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetTapeNumber(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getheaderdate_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetHeaderDate(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_12; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getheadertime_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetHeaderTime(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_12; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
gettimezone_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetTimeZone(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_4; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getaircraft_(result)
char	*result;
{
	int	i, rc;
	char	*p;

	rc = GetAircraft(&p);
	strcpy(result, p);

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getnumberitems_(result)
long	*result;
{
	return(GetNumberItems(result));
}

/* -------------------------------------------------------------------- */
get_lrlen_(result)
long	*result;
{
	return(get_lrlen(result));
}

/* -------------------------------------------------------------------- */
get_lrppr_(result)
long	*result;
{
	return(get_lrppr(result));
}

/* -------------------------------------------------------------------- */
get_thdrlen_(result)
long	*result;
{
	return(get_thdrlen(result));
}

/* --------------------------------------------------------------------
 * START Group 3
 */

/* -------------------------------------------------------------------- */
getprimaryaddr_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetPrimaryAddr(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getsecondaryaddr_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetSecondaryAddr(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getsampleoffset_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetSampleOffset(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getitemlen_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetItemLength(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getstart_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetStart(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getlength_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetLength(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getrate_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetRate(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getorder_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetOrder(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getreadaddr1_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetReadAddr1(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getreadaddr2_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetReadAddr2(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getwriteaddr_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetWriteAddr(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getresolution_(vn, result)
char	*vn;
short	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetResolution(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getlrlength_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetLRLength(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getlrppr_(vn, result)
char	*vn;
long	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetLRPPR(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getconversionfactor_(vn, result)
char	*vn;
float	*result;
{
	int	rc;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';
	rc = GetConversionFactor(vn, result);
	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getcalcoeff_(vn, result)
char	*vn;
float	*result;
{
	int	rc;
	long	order;
	float	*f;
	char	*p;

	*(p = strchr(vn, SPACE)) = '\0';

	if ((rc = GetCalCoeff(vn, &f)) == OK)
		{
		if (GetOrder(vn, &order) == ERR)
			order = 0;

		memcpy((char *)result, (char *)f, (int)(sizeof(float) * order));
		}

	*p = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getlocation_(vn, result)
char	*vn, *result;
{
	int	i, rc;
	char	*p, *val;

	*(p = strchr(vn, SPACE)) = '\0';

	if ((rc = GetLocation(vn, &val)) == OK)
		strcpy(result, val);

	*p = SPACE;

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getname_(vn, result)
char	*vn, *result;
{
	int	i, rc;
	char	*p, *val;

	*(p = strchr(vn, SPACE)) = '\0';

	if ((rc = GetName(vn, &val)) == OK)
		strcpy(result, val);

	*p = SPACE;

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
getitemtype_(vn, result)
char	*vn, *result;
{
	int	i, rc;
	char	*p, *val;

	*(p = strchr(vn, SPACE)) = '\0';

	if ((rc = GetItemType(vn, &val)) == OK)
		strcpy(result, val);

	*p = SPACE;

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* -------------------------------------------------------------------- */
gettype_(vn, result)
char	*vn, *result;
{
	int	i, rc;
	char	*p, *val;

	*(p = strchr(vn, SPACE)) = '\0';

	if ((rc = GetType(vn, &val)) == OK)
		strcpy(result, val);

	*p = SPACE;

	for (i = strlen(result); i < LEN_8; ++i)
		result[i] = SPACE;

	return(rc);
}

/* END F_WRAP.C */
