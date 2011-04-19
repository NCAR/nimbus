/*
-------------------------------------------------------------------------
OBJECT NAME:	amlib.h

DESCRIPTION:	Header File declaring amlib functions.
-------------------------------------------------------------------------
*/

#ifndef AMLIB_H
#define AMLIB_H

#include <raf/header.h>
#include <netinet/in.h>

#include <vector>

/* See amlib/xlate/const.c
 */
extern const double FeetToMeters, MPS2, KNOTS_TO_MPS, MPS_TO_KNOTS, FTMIN,
	Kelvin, WH_GRAVITY, StdPress, EARTH_RADIUS, OMEGAE, CKTMS, GRAVITY,
	MolecularWeightWater, RAD_DEG, DEG_RAD, MolecularWeightDryAir,
	Rd_DIV_Cpd, StdSpeedOfSound, Tr, Cp, Rd, Cw, Lv;

/* Resolver/synchro conversion equation.  See amlib/xlate/const.c
 */
extern const double RESOLV14BIT, RESOLV16BIT;


/* Values for 'FeedBack' variable	*/
enum RateFeedBack { LOW_RATE_FEEDBACK, HIGH_RATE_FEEDBACK, nFeedBackTypes };

#define AMBIENT(t, rf, xm2) \
		((t + Kelvin) / (1.0 + 0.2 * rf * xm2) - Kelvin)

#define XMAC2(a) \
		(5.0 * (pow((double)((a)+1.0), Rd_DIV_Cpd) - 1.0))


NR_TYPE GetSample(DERTBL *dp, int di);

#define GetSample(dp, di)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? AveragedData[dp->depends[di]->LRstart] \
		: HighRateData[dp->depends[di]->HRstart + SampleOffset])

#define GetVector(dp, di)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? &AveragedData[dp->depends[di]->LRstart] \
		: &HighRateData[dp->depends[di]->HRstart + (SampleOffset * dp->depends[di]->Length)])

/* PMS1D sample rate is 10Hz.  We want to grab the closest true airspeed,
 * or any 25Hz input value, so multiply SampleOffset by 2.5.
 */
#define GetSampleFor1D(dp, di)	\
	(FeedBack == LOW_RATE_FEEDBACK \
		? AveragedData[dp->depends[di]->LRstart] \
		: HighRateData[dp->depends[di]->HRstart + (int)(2.5 * SampleOffset)])


#define PutSample(dp, y)	\
	if (FeedBack == LOW_RATE_FEEDBACK) \
		AveragedData[dp->LRstart] = (NR_TYPE)y; \
	else \
		HighRateData[dp->HRstart + SampleOffset] = (NR_TYPE)y

#define PutStaticSample(dp, y)	\
	if (FeedBack == LOW_RATE_FEEDBACK) \
		AveragedData[dp->LRstart] = (NR_TYPE)y[HIGH_RATE]; \
	else \
		HighRateData[dp->HRstart + SampleOffset] = (NR_TYPE)y[SampleOffset]

struct _dnfn
	{
	const char	*name;
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
void AddToDefaults(const char varName[], const char attrName[],
	const std::vector<NR_TYPE>& values);
void AddToAttributes(const char varName[], const char attrName[],
	const std::string & text);

#if defined(__LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN) || defined(LITTLE_ENDIAN)
float	ntohf(float);
#else
#define ntohf(x)        (x)
#endif

void decodeADS2analog(RAWTBL *varp, void *input, NR_TYPE *output);

#include "amlibProto.h"

#endif

/* END AMLIB.H */
