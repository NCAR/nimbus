/*
-------------------------------------------------------------------------
OBJECT NAME:	extern.h

FULL NAME:	Global Variable External Declarations

DESCRIPTION:	

AUTHOR:		websterc@ncar
-------------------------------------------------------------------------
*/

#ifndef EXTERN_H
#define EXTERN_H

extern bool	Grid, Frozen, FlashMode;

extern char	buffer[], DataPath[], *ProjectName, FlightNumber[];

extern SDITBL  *sdi[];          /* SDI (aka raw) variable list  */
extern RAWTBL  *raw[];          /* Alphabeticly sorted pointers */
extern DEFAULT *Defaults[];

extern int	nsdi, nraw, nVariables;
extern VARTBL	Variable[];


extern int	NumberDataSets, NumberSeconds, NumberElements[], CurrentDataSet;
extern int	Aircraft;

extern char	*ADSrecord;
extern NR_TYPE	*bits, *volts, *SampledData;

extern double	*plotData[], *xData[];

extern std::string	title;

void	AddDataToBuffer(NR_TYPE *newData), DecodeHeader(),
	ApplyCalCoes(NR_TYPE *record), DecodeRecord();

#endif

/* END EXTERN.H */
