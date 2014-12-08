/*
-------------------------------------------------------------------------
OBJECT NAME:	vardb.h

FULL NAME:	Variable DataBase header file

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2011
-------------------------------------------------------------------------
*/

#ifndef VARDB_H
#define	VARDB_H

#include <stdint.h>

#define	FIXED		0
#define FLOATING	1

#define VARDB_NL	16	/* Name Length	*/
#define VARDB_UL	16	/* Units Length	*/
#define VARDB_TL	64	/* Title Length	*/

#ifndef ERR
#define ERR	(-1)
#define OK	(0)
#endif

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

struct vardb_hdr	/* This is the first struct in the file	*/
	{
	int32_t	MagicCookie;
	int32_t	Version;
	int32_t	nRecords;
	int32_t	RecordLen;
	char	unused[8];
	};

struct var_v1
	{
	char	Name[VARDB_NL];
	char	Units[VARDB_UL];
	char	AlternateUnits[VARDB_UL];
	char	Title[VARDB_TL];

	int32_t	type;			/* FIXED or FLOATING	*/
	float	FixedRange[2];
	float	FloatRange;
	float	MinLimit;
	float	MaxLimit;

	float	CalRange[2];	/* Analog SDI's only	*/
	};

struct var_v2
	{
	char	Name[VARDB_NL];
	char	Units[VARDB_UL];
	char	AlternateUnits[VARDB_UL];
	char	Title[VARDB_TL];

	int32_t	is_analog;	/* bool true/false if analog channel */
	int32_t	voltageRange[2];
	int32_t	defaultSampleRate;
	float	MinLimit;
	float	MaxLimit;

	float	CalRange[2];	/* Analog SDI's only	*/

	int32_t	Category;
	int32_t	standard_name;	/* V3, was "unused" space in V2	*/
	int32_t	reference;	/* V3 - bool, was "unused" space in V2	*/
	};


extern "C" {

typedef struct vardb_hdr VarDB_Hdr;

extern VarDB_Hdr	master_VarDB_Hdr;
extern void		*master_VarDB;

extern VarDB_Hdr	varDB_Hdr;
extern void		*VarDB;

extern int	VarDB_NcML;


bool	VarDB_isNcML();

const char
	*VarDB_GetUnits(const char vn[]),
	*VarDB_GetAlternateUnits(const char vn[]),
	*VarDB_GetCategoryName(const char vn[]),
	*VarDB_GetStandardNameName(const char vn[]),
	*VarDB_GetTitle(const char vn[]),
	*VarDB_GetDependencies(const char vn[]);
char	**VarDB_GetVariablesInCategory(const int catNum);

int	InitializeVarDB(const char fileName[]),
	VarDB_lookup(const char name[]),
	SaveVarDB(const char fileName[]),
	ReadCategories(), ReadStandardNames(),
	VarDB_GetCategory(const char vn[]),
	VarDB_GetCategoryNumber(const char catagoryName[]),
	VarDB_GetCategoryList(char *list[]),
	VarDB_GetStandardName(const char vn[]),
	VarDB_GetStandardNameNumber(const char catagoryName[]),
	VarDB_GetStandardNameList(char *list[]);

void	SortVarDB(), ReleaseVarDB(), SetCategoryFileName(const char fn[]),
	SetStandardNameFileName(const char fn[]);

float	VarDB_GetFixedRangeLower(const char vn[]),
	VarDB_GetFixedRangeUpper(const char vn[]),
	VarDB_GetFloatRange(const char vn[]),
	VarDB_GetFillValue(const char vn[]),
	VarDB_GetMinLimit(const char vn[]),
	VarDB_GetMaxLimit(const char vn[]),
	VarDB_GetCalRangeLower(const char vn[]),
	VarDB_GetCalRangeUpper(const char vn[]),
	VarDB_GetSpikeSlope(const char vn[]);

bool	VarDB_isAnalog(const char vn[]);
//	VarDB_isRangeFixed(const char vn[]),
//	VarDB_isRangeFloating(const char vn[]);

int	VarDB_SetUnits(const char vn[], char units[]),
	VarDB_SetAlternateUnits(const char vn[], char units[]),
	VarDB_SetTitle(const char vn[], char title[]),
	VarDB_SetIsAnalog(const char vn[], bool value),
	VarDB_SetVoltageRangeLower(const char vn[], int32_t value),
	VarDB_SetVoltageRangeUpper(const char vn[], int32_t value),
	VarDB_SetDefaultSampleRate(const char vn[], int32_t value),
	VarDB_SetMinLimit(const char vn[], float value),
	VarDB_SetMaxLimit(const char vn[], float value),
	VarDB_SetCalRangeLower(const char vn[], float value),
	VarDB_SetCalRangeUpper(const char vn[], float value),
	VarDB_SetCategory(const char vn[], uint32_t value),
	VarDB_SetStandardName(const char vn[], uint32_t value),
	VarDB_SetReference(const char vn[], uint32_t value),
	VarDB_SetSpikeSlope(const char vn[], float value);
int32_t VarDB_GetVoltageRangeUpper(const char vn[]);
}


#endif
