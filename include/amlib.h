/*
-------------------------------------------------------------------------
OBJECT NAME:	amlib.h

DESCRIPTION:	Header File declaring amlib functions.
-------------------------------------------------------------------------
*/

#ifndef AMLIB_H
#define AMLIB_H

#include "header.h"
#include <math.h>
#include <netinet/in.h>

const int MAX_FSSP = 4;
const int MAX_260X = 2;
const int MAX_ASAS = 2;
const int MAX_F300 = 2;
const int MAX_PMS2D = (MAX_PMS2 * 2);

const NR_TYPE FTMTR  = 0.3048;
const NR_TYPE MPS2   = 9.7959;
const NR_TYPE KTS2MS = 0.514791;
const NR_TYPE FTMIN  = 0.00508;

const double RAD_DEG = 180.0 / M_PI;
const double DEG_RAD = M_PI / 180.0;

/* Resolver/synchro conversion equation
 */
const NR_TYPE RESOLV14BIT = 180.0 / 8192.0;
const NR_TYPE RESOLV16BIT = 360.0 / 65536.0;


/* Values for 'FeedBack' variable	*/
#define	nFeedBackTypes		2

#define LOW_RATE_FEEDBACK	0
#define HIGH_RATE_FEEDBACK	1

#define AMBIENT(t, rf, xm2)	((t + 273.16) / (1.0 + 0.2 * rf * xm2) - 273.16)

#define GetSample(dp, di)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? AveragedData[dp->depend_LRindex[di]] \
		: HighRateData[dp->depend_HRindex[di] + SampleOffset])

#define GetVector(dp, di, vlen)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? &AveragedData[dp->depend_LRindex[di]] \
		: &HighRateData[dp->depend_HRindex[di] + (SampleOffset * vlen)])

#define GetSampleFor1D(dp, di)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? AveragedData[dp->depend_LRindex[di]] \
		: HighRateData[dp->depend_HRindex[di] + (SampleOffset << 1)])


#define PutSample(dp, y)	\
	if (FeedBack == LOW_RATE_FEEDBACK) \
		AveragedData[dp->LRstart] = y; \
	else \
		HighRateData[dp->HRstart + SampleOffset] = y

#define PutStaticSample(dp, y)	\
	if (FeedBack == LOW_RATE_FEEDBACK) \
		AveragedData[dp->LRstart] = y[HIGH_RATE]; \
	else \
		HighRateData[dp->HRstart + SampleOffset] = y[SampleOffset]

struct _dnfn
	{
	char		*name;
	void		(*constructor)(void *);
	void		(*xlate)(RAWTBL *, void *, float *);
	void		(*compute)(DERTBL *);
	} ;

NR_TYPE	FirstPoly(NR_TYPE x, NR_TYPE c[]);
NR_TYPE	SecondPoly(NR_TYPE x, NR_TYPE c[]);

int	SearchDERIVFTNS(char target[]);
void	RunAMLIBinitializers();

extern NR_TYPE		*AveragedData, *HighRateData;
extern int		FeedBack, SampleOffset;
extern struct _dnfn	deriveftns[];

extern float	HDRversion;

NR_TYPE *GetDefaultsValue(char target[], char var[]);

#if defined(__LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN) || defined(LITTLE_ENDIAN)
float	ntohf(float);
#else
#define ntohf(x)        (x)
#endif

#include "amlibProto.h"

#endif

/* END AMLIB.H */
