/*
-------------------------------------------------------------------------
OBJECT NAME:	ACdata.h

FULL NAME:	Manage NCAR-RAF Aircraft netCDF file.

TYPE:		

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#ifndef ACDATA_H
#define ACDATA_H

#include "mcr.h"
#include "netcdf.hh"
#include "Time.h"

#include <cfloat>
#include <cmath>
#include <iostream>
#include <algorithm>

#define MAX_SEGS	128

/* -------------------------------------------------------------------- */
class AircraftData {

public:
	AircraftData(const char fileName[]);

  void	SearchStraightAndLevel();
  bool	NextTimeSegment(FlightClock& start, FlightClock& end);

  void	LoadCorrectionData(FlightClock& currTime);

  float	Roll()		{ return(roll); }
  float	Pitch()		{ return(pitch); }
  float	GSpeed()	{ return(gspd); }
  float	Drift()		{ return(drft); }
  float	RadarAlt()	{ return(radAlt); }
  float	Year()		{ return(year); }
  float	Month()		{ return(month); }
  float	Day()		{ return(day); }
  float	Hour()		{ return(hour); }
  float	Minute()	{ return(minute); }
  float	Second()	{ return(second); }
  float	Lat()		{ return(lat); }
  float	Lon()		{ return(lon); }
  float	Track()		{ return(trk); }
  float	TAS()		{ return(tas); }
  float	Heading()	{ return(hdg); }
  float	Alt()		{ return(alt); }
  float	WS()		{ return(ws); }
  float	WD()		{ return(wd); }
  float	Zenith()	{ return(zen); }
  float	Azimuth()	{ return(azim); }

  float	MeanTrack()		{ return(meanTrack[currSeg-1]); }
  float	MeanSpeed()		{ return(meanGSpeed[currSeg-1]); }
  float	MeanAlt()		{ return(meanAlt[currSeg-1]); }
  float	MeanRoll()		{ return(meanRoll[currSeg-1]); }

private:
  NcFile	*file;
  NcVar		*hourVP, *minVP, *secVP, *yearVP, *monthVP, *dayVP;
  NcVar		*latVP, *lonVP, *trkVP, *hdgVP, *altVP, *wsVP, *wdVP, *tasVP,
		*gspdVP, *zenVP, *azimVP, *rad_altVP, *rollVP, *pitchVP,
		*driftVP;

  FlightClock	startTime, endTime, startSeg[MAX_SEGS], endSeg[MAX_SEGS];
  int		nSegs, currSeg;
  float		meanTrack[MAX_SEGS], meanGSpeed[MAX_SEGS], meanRoll[MAX_SEGS],
		meanAlt[MAX_SEGS];

  float	year, month, day, hour, minute, second, roll, pitch, radAlt, drft, gspd,
	lat, lon, trk, tas, hdg, alt, ws, wd, zen, azim;

  static const float	startGSF = 50.0, startALT = 2500.0;

};	/* END NETCDF.H */

#endif
