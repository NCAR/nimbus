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


extern struct vardb_hdr	master_VarDB_Hdr;
extern void		*master_VarDB;

extern struct vardb_hdr	VarDB_Hdr;
extern void		*VarDB;

#ifdef __cplusplus
extern "C" {
#endif
char	*VarDB_GetUnits(const char vn[]),
	*VarDB_GetAlternateUnits(const char vn[]),
	*VarDB_GetCategoryName(const char vn[]),
	*VarDB_GetTitle(const char vn[]),
	**VarDB_GetVariablesInCategory(const int catNum);

int	InitializeVarDB(const char fileName[]),
	VarDB_lookup(const char name[]),
	SaveVarDB(const char fileName[]),
	ReadCategories(),
	VarDB_GetCategory(const char vn[]),
	VarDB_GetCategoryNumber(const char catagoryName[]),
	VarDB_GetCategoryList(char *list[]),
	VarDB_isRangeFixed(const char vn[]),
	VarDB_isRangeFloating(const char vn[]);

void	SortVarDB(), ReleaseVarDB(), SetCategoryFileName(const char fn[]);

float	VarDB_GetFixedRangeLower(const char vn[]),
	VarDB_GetFixedRangeUpper(const char vn[]),
	VarDB_GetFloatRange(const char vn[]),
	VarDB_GetMinLimit(const char vn[]),
	VarDB_GetMaxLimit(const char vn[]),
	VarDB_GetCalRangeLower(const char vn[]),
	VarDB_GetCalRangeUpper(const char vn[]),
	VarDB_GetSpikeSlope(const char vn[]);

int	VarDB_SetUnits(const char vn[], char units[]),
	VarDB_SetAlternateUnits(const char vn[], char units[]),
	VarDB_SetTitle(const char vn[], char title[]),
	VarDB_SetRangeFixed(const char vn[]),
	VarDB_SetRangeFloating(const char vn[]),
	VarDB_SetFixedRangeLower(const char vn[], float value),
	VarDB_SetFixedRangeUpper(const char vn[], float value),
	VarDB_SetFloatRange(const char vn[], float value),
	VarDB_SetMinLimit(const char vn[], float value),
	VarDB_SetMaxLimit(const char vn[], float value),
	VarDB_SetCalRangeLower(const char vn[], float value),
	VarDB_SetCalRangeUpper(const char vn[], float value),
	VarDB_SetCategory(const char vn[], int value),
	VarDB_SetSpikeSlope(const char vn[], float value);
#ifdef __cplusplus
	}
#endif

#endif
