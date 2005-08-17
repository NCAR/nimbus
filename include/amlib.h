/*
-------------------------------------------------------------------------
OBJECT NAME:	amlib.h

DESCRIPTION:	Header File declaring amlib functions.
-------------------------------------------------------------------------
*/

#ifndef AMLIB_H
#define AMLIB_H

#include "header.h"
#include <netinet/in.h>

const size_t MAX_FSSP = 4;
const size_t MAX_260X = 2;
const size_t MAX_ASAS = 2;
const size_t MAX_F300 = 2;
const size_t MAX_PMS2D = (MAX_PMS2 * 2);

/* See amlib/xlate/const.c
 */
extern const double FTMTR, MPS2, KTS2MS, FTMIN, Kelvin, WH_GRAVITY;
extern const double EARTH_RADIUS, OMEGAE, CKTMS, GRAVITY;
extern const double RAD_DEG, DEG_RAD;

/* Resolver/synchro conversion equation.  See amlib/xlate/const.c
 */
extern const double RESOLV14BIT, RESOLV16BIT;


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
