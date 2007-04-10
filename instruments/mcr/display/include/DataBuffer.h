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

#include "mcrtd.h"

#include "hdrAPI.h"
#include "CanvasWindow.h"

const int SampleRate = 5000;	// 5Khz
const int nSamplesPerScan = 340;

/* -------------------------------------------------------------------- */
class DataBuffer {

public:
	DataBuffer(const char proj_num[]);

  void	SetChannel(int newChannel);

  void	SetStartVoltage(float v)	{ startVolt[currentChannel] = v; }
  void	SetEndVoltage(float v)		{ endVolt[currentChannel] = v; }
  float	GetStartVoltage()		{ return(startVolt[currentChannel]); }
  float	GetEndVoltage()			{ return(endVolt[currentChannel]); }

  int	CurrentChannel()	{ return(currentChannel); }
  void	CountsToVolts(Mcr_rec *rec);
  bool	MakeImages();

  int	nImages()		{ return(imageIdx); }
  char	*ImageData(int idx)	{ return(imageData[idx]); }
  float	Voltage(int idx)	{ return(volts[idx]); }
  float	Housekeeping(int idx)	{ return(hskp[idx]); }
  float	PeakVoltage()		{ return(peak); }

  void	RemoveFullScans();

  void	StartRecording(char *dataFile);
  void	StopRecording();
  void	RecordData(Mcr_rec *rec);

private:
  int	SearchNextScan(float v[], int currentIdx);
  void	SearchHousekeeping(int currentIdx);


  Header	*hdr;

  int	currentChannel, projNum;
  Sh	*vp[7], *theVP;

  float	volts[5000], hskp[32], peak;
  float	startVolt[7], endVolt[7];
  float	Vref, Bref;		// Used to fix channels 6 & 7.
  char	imageData[20][360];
  int	imageIdx, outSampleIdx, inSampleIdx, recordCounter;

  static const int	RecordsToRecord = 10;	// Actually 

  FILE	*fp;

  void	PrintThermalVolts(int idx);

};	/* END DATABUFFER.H */

#endif
