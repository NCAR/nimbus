/*
-------------------------------------------------------------------------
OBJECT NAME:	OutFile.h

FULL NAME:	MCR processed output file.

TYPE:		

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#ifndef OUTFILE_H
#define OUTFILE_H

#include "mcr.h"
#include "DataBuffer.h"
#include "Netcdf.h"
#include <raf/hdrAPI.h>
#include <raf/Time.h>

#include <cerrno>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>

#define MAX_WIDTH	400

struct _recP
  {
  MCR_REC_HDR           recHdr;
 
  struct _chP
    {
    MCR_CHANNEL_HDR     chHdr;
    float               dat[MAX_WIDTH];
    } chan[N_CHANNELS];
  } ;

/* -------------------------------------------------------------------- */
class OutFile {

public:
	OutFile(const char fileName[], FlightClock& start, FlightClock& end);
	~OutFile();

  void	WriteHeader(Header *ADSheader, AircraftData& acData);
  void	ApplyScanToFile(DataBuffer *scans[], AircraftData& acData);
  void	InterpMissingData(int pc[]);		// interp between miss_vals

  void	IncrementCurrentScan(float nMeters)	{ planeOffset += nMeters; }


private:
  MCR_HDR	hdr;
  struct _recP   nextRec;
  MCR_CHANNEL_HDR chanHdr[N_CHANNELS];

  FILE	*outFP;
  float	pixWidth, pixLength, planeOffset;
  long	nOutRecords;
  long	startPos, scanNumber;
  long	firstRecord, nRecords, lastRecUsed;

  char	*data;

  void	LoadRecords(int, int);
  void	MoreRecords(int, int);
  void	FlushRecords();
  void	fillACdata(MCR_REC_HDR *rec, AircraftData& acData);

};	/* END OUTFILE.H */

#endif
