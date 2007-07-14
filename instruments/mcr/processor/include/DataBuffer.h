/*
-------------------------------------------------------------------------
OBJECT NAME:	DataBuffer.h

FULL NAME:	Create Main Canvas Window

TYPE:		

DESCRIPTION:	Maintain toggle buffers, apply cal coes, process data.

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#ifndef DATABUFFER_H
#define DATABUFFER_H

#include "mcr.h"
#include "Netcdf.h"
#include <raf/hdrAPI.h>

const int	SampleRate	= 5000;		// 5Khz
const int	SamplesPerRev	= (int)(SampleRate / 3.47);
const int	nSamplesPerScan	= SamplesPerRev / 4;	// 90 degree opening

const double	DegToRad	= M_PI / 180.0;

struct bit
	{
	float	pixel;
	float	x, y;	// Offset from airplane in meters.
	};

#define N_SCANS		5
#define MAX_SAMPLES	800

/* -------------------------------------------------------------------- */
class DataBuffer {

public:
  friend	class OutFile;

	DataBuffer(Header *hdr, int channel);

  void	ProcessRecord(Mcr_rec *rec);
  void	ComputePositionForData(AircraftData& netCDF);
  void	RemoveFullScans(), ClearAllScans();
  bool	WriteRecord(FILE *fp, int scan);

  int	nScans()		{ return(scanIdx); }

  float	Voltage(int idx)	{ return(volts[idx]); }
  float	Housekeeping(int idx)	{ return(hskp[idx]); }

  int	MilliSecond(int scanIdx)	{ return(milliSec[scanIdx]); }


private:
  int	SearchStartNextScan(float v[], int currentIdx);
  int	SearchEndScan(float v[], int currentIdx);
  void	SearchHousekeeping(int currentIdx);

  Sh	*theVP;
  int	channel, projNum;

  float	volts[5000], hskp[32], Vref, Bref;
  int	outSampleIdx, inSampleIdx, scanIdx, milliSec[N_SCANS],
	nSamplesPerScan[N_SCANS];
  struct bit	scan[N_SCANS][MAX_SAMPLES];

  Header	*adsHdr;

  int		polyOrder;
  float		cals[5];

};	/* END DATABUFFER.H */

#endif
