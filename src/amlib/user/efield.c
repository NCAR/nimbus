/****************************************** 09OCT1987 ******  EFM  */
/** Electric field mills calculation  */

/** Input variables

     Variables used:
       fmt,fmb,fmrt,fmrb,fmrr,fmrl - insensitive channels
       sfmt,sfmb,sfmrt,sfmrb,sfmrr,sfmrl - sensitive channels
*/

#include "nimbus.h"
#include "amlib.h"

/* edata - data statements for electric field calculations */
static float eb1 = 1.63;
static float eb2 = 1.53;
static float ed1 = 1.;
static float ed2 = 1.;
static float eg = 1.5217;
static float eh = 2.25;
static float dconv = 0.0218;
static float ecz = 0.33;
static float erry = 0.98;
static float errz = 0.94;
static float erz = 1.30;
static float eqz = 1.;
static float eqrz = 1.;
static float eqry = 1.;
static float esb1 = 1.63;
static float esb2 = 1.53;
static float esd1 = 1.;
static float esd2 = 1.;
static float esg = 1.;
static float esh = 1.;
static float escz = 0.33;
static float esrry = 0.98;
static float esrrz = 0.91;
static float esrz = 1.30;
static float esqz = 1.;
static float esqrz = 1.;
static float esqry = 1.;
 
static NR_TYPE	efqyr,efqzr,efx1,efx2,efyr,efzr,efz,efqyrs,efqzrs,efqzs,
		efx1s,efx2s,efyrs,efzrs,efzs;
NR_TYPE	efqz;
NR_TYPE	fmrl,fmrr,fmrt,fmrb,fmt,fmb,sfmrl,sfmrr,sfmrt,sfmrb,sfmt,sfmb;

/** EFM calculation: Cooper memo 9/4/87--- */
/* ..... Calculate fields from field mill measurements */
 
/* -------------------------------------------------------------------- */
void sefqz(DERTBL *varp)
{

	fmrl	= GetSample(varp, 0);
	fmrr	= GetSample(varp, 1);
	fmrt	= GetSample(varp, 2);
	fmrb	= GetSample(varp, 3);
	fmt	= GetSample(varp, 4);
	fmb	= GetSample(varp, 5);
	sfmrl	= GetSample(varp, 6);
	sfmrr	= GetSample(varp, 7);
	sfmrt	= GetSample(varp, 8);
	sfmrb	= GetSample(varp, 9);
	sfmt	= GetSample(varp, 10);
	sfmb	= GetSample(varp, 11);

	efqyr	= 0.5 * (fmrl+eqry*fmrr);
	efqzr	= 0.5 * (fmrt+eqrz*fmrb);
	efqz	= 0.5 * (fmt+eqz*fmb);
	efx1	= ed1 * ecz * (efqzr-eb1*efqz);
	efx2	= ed2 * ecz * (efqyr-eb2*efqz);
	efyr	= eg * eh * ecz * 0.5 * (fmrl-erry*fmrr);
	efzr	= eh * ecz * 0.5 * (fmrt-errz*fmrb);
	efz	= ecz * 0.5 * (fmt-erz*fmb);
	efqyrs	= 0.5 * (sfmrl+esqry*sfmrr);
	efqzrs	= 0.5 * (sfmrt+esqrz*sfmrb);
	efqzs	= 0.5 * (sfmt+esqz*sfmb);
	efx1s	= esd1 * escz * (efqzrs-esb1*efqzs);
	efx2s	= esd2 * escz * (efqyrs-esb2*efqzs);
	efyrs	= esg * esh * escz * 0.5 * (sfmrl-esrry*sfmrr);
	efzrs	= esh * escz * 0.5 * (sfmrt-esrrz*sfmrb);
	efzs	= escz * 0.5 * (sfmt-esrz*sfmb);

	PutSample(varp, efqz);
}

/* -------------------------------------------------------------------- */
void sefqyr(DERTBL *varp)
{
	PutSample(varp, efqyr);
}

/* -------------------------------------------------------------------- */
void sefqzr(DERTBL *varp)
{
	PutSample(varp, efqzr);
}

/* -------------------------------------------------------------------- */
void sefx1(DERTBL *varp)
{
	PutSample(varp, efx1);
}

/* -------------------------------------------------------------------- */
void sefx2(DERTBL *varp)
{
	PutSample(varp, efx2);
}

/* -------------------------------------------------------------------- */
void sefyr(DERTBL *varp)
{
	PutSample(varp, efyr);
}

/* -------------------------------------------------------------------- */
void sefzr(DERTBL *varp)
{
	PutSample(varp, efzr);
}

/* -------------------------------------------------------------------- */
void sefz(DERTBL *varp)
{
	PutSample(varp, efz);
}

/* -------------------------------------------------------------------- */
void sefqyrs(DERTBL *varp)
{ 
	PutSample(varp, efqyrs);
} 

/* -------------------------------------------------------------------- */
void sefqzrs(DERTBL *varp)
{
	PutSample(varp, efqzrs);
}

/* -------------------------------------------------------------------- */
void sefqzs(DERTBL *varp)
{
	PutSample(varp, efqzs);
}

/* -------------------------------------------------------------------- */
void sefx1s(DERTBL *varp)
{
	PutSample(varp, efx1s);
}

/* -------------------------------------------------------------------- */
void sefx2s(DERTBL *varp)
{
	PutSample(varp, efx2s);
}

/* -------------------------------------------------------------------- */
void sefyrs(DERTBL *varp)
{
	PutSample(varp, efyrs);
}

/* -------------------------------------------------------------------- */
void sefzrs(DERTBL *varp)
{
	PutSample(varp, efzrs);
}

/* -------------------------------------------------------------------- */
void sefzs(DERTBL *varp)
{
	PutSample(varp, efzs);
}
