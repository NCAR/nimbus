/*
-------------------------------------------------------------------------
OBJECT NAME:	vardb.h

FULL NAME:		Variable DataBase header file

DESCRIPTION:	

COPYRIGHT:		University Corporation for Atmospheric Research, 1993-95
-------------------------------------------------------------------------
*/

#ifndef VARDB_H
#define	VARDB_H

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
	long	MagicCookie;
	long	Version;
	long	nRecords;
	long	RecordLen;
	char	unused[8];
	};

struct var_v1
	{
	char	Name[VARDB_NL];
	char	Units[VARDB_UL];
	char	AlternateUnits[VARDB_UL];
	char	Title[VARDB_TL];

	long	type;			/* FIXED or FLOATING	*/
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

	long	type;			/* FIXED or FLOATING	*/
	float	FixedRange[2];
	float	FloatRange;
	float	MinLimit;
	float	MaxLimit;

	float	CalRange[2];	/* Analog SDI's only	*/

	long	Category;
	char	unused[8];
	};


extern struct vardb_hdr	VarDB_Hdr;
extern void		*VarDB;

#ifdef __cplusplus
extern "C" {
#endif
char	*VarDB_GetUnits(char vn[]),
	*VarDB_GetAlternateUnits(char vn[]),
	*VarDB_GetCategoryName(char vn[]),
	*VarDB_GetTitle(char vn[]),
	**VarDB_GetVariablesInCategory(int catNum);

int	InitializeVarDB(char fileName[]),
	VarDB_lookup(char name[]),
	SaveVarDB(char fileName[]),
	ReadCategories(),
	VarDB_isRangeFixed(char vn[]),
	VarDB_isRangeFloating(char vn[]),
	VarDB_GetCategory(char vn[]),
	VarDB_GetCategoryNumber(char catagoryName[]),
	VarDB_GetCategoryList(char *list[]);

void	SortVarDB(), ReleaseVarDB(), SetCategoryFileName(char fn[]);

float	VarDB_GetFixedRangeLower(char vn[]),
	VarDB_GetFixedRangeUpper(char vn[]),
	VarDB_GetFloatRange(char vn[]),
	VarDB_GetMinLimit(char vn[]),
	VarDB_GetMaxLimit(char vn[]),
	VarDB_GetCalRangeLower(char vn[]),
	VarDB_GetCalRangeUpper(char vn[]),
	VarDB_GetSpikeSlope(char vn[]);

int	VarDB_SetUnits(char vn[], char units[]),
	VarDB_SetAlternateUnits(char vn[], char units[]),
	VarDB_SetTitle(char vn[], char title[]),
	VarDB_SetRangeFixed(char vn[]),
	VarDB_SetRangeFloating(char vn[]),
	VarDB_SetFixedRangeLower(char vn[], float value),
	VarDB_SetFixedRangeUpper(char vn[], float value),
	VarDB_SetFloatRange(char vn[], float value),
	VarDB_SetMinLimit(char vn[], float value),
	VarDB_SetMaxLimit(char vn[], float value),
	VarDB_SetCalRangeLower(char vn[], float value),
	VarDB_SetCalRangeUpper(char vn[], float value),
	VarDB_SetCategory(char vn[], int value),
	VarDB_SetSpikeSlope(char vn[], float value);
#ifdef __cplusplus
	}
#endif

#endif
