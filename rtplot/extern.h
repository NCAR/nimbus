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

extern std::vector<SDITBL *> sdi;          /* SDI (aka raw) variable list  */
extern std::vector<RAWTBL *> raw;          /* Alphabeticly sorted pointers */

extern DEFAULT *Defaults[];

extern size_t	nVariables;
extern VARTBL	Variable[];


extern size_t	NumberDataSets, NumberSeconds, NumberElements[], CurrentDataSet;
extern int	Aircraft;

extern char	*ADSrecord;
extern NR_TYPE	*bits, *volts, *SampledData;

extern double	*plotData[], *xData[];

extern std::string	title;

void	AddDataToBuffer(NR_TYPE *newData), DecodeHeader(),
	ApplyCalCoes(NR_TYPE *record), DecodeRecord();

#endif

/* END EXTERN.H */
