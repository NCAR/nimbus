/*
-------------------------------------------------------------------------
OBJECT NAME:	amlib.h

DESCRIPTION:	Header File declaring amlib functions.
-------------------------------------------------------------------------
*/

#ifndef AMLIB_H
#define AMLIB_H

#include "header.h"
#include <cmath>
#include <netinet/in.h>

const size_t MAX_FSSP = 4;
const size_t MAX_260X = 2;
const size_t MAX_ASAS = 2;
const size_t MAX_F300 = 2;
const size_t MAX_PMS2D = (MAX_PMS2 * 2);

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
enum RateFeedBack { LOW_RATE_FEEDBACK, HIGH_RATE_FEEDBACK, nFeedBackTypes };

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
		: HighRateData[dp->depend_HRindex[di] + (int)(2.5 * SampleOffset)])


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

NR_TYPE	FirstPoly(NR_TYPE x, std::vector<float>& cof);
NR_TYPE	SecondPoly(NR_TYPE x, std::vector<float>& cof);

int	SearchDERIVFTNS(const char target[]);
void	RunAMLIBinitializers(), LogXlateMsg(const char msg[]),
	Log2dXlateMsg(P2d_rec *p, const char msg[]), LogStdMsg(const char msg[]);

extern NR_TYPE		*AveragedData, *HighRateData;
extern RateFeedBack	FeedBack;
extern size_t		SampleOffset;
extern struct _dnfn	deriveftns[];

extern float	HDRversion;

NR_TYPE *GetDefaultsValue(const char target[], const char var[]);

#if defined(__LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN) || defined(LITTLE_ENDIAN)
float	ntohf(float);
#else
#define ntohf(x)        (x)
#endif

#include "amlibProto.h"

#endif

/* END AMLIB.H */
