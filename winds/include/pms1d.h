/* pms1d.h -- parameters associated with the pms1d module
*/

#ifndef PMS1D_H
#define PMS1D_H

#include "constants.h"
#include <stdlib.h>

/* For pms1d_probe.type field.
*/
#ifndef FSSP
#define ASAS	0
#define FSSP	1
#define X200	2
#define Y200	3
#define X260	4
#define F300	5
#define MASP	6
#endif

#ifndef BINS_64
#define BINS_64	64
#endif

#define MAX1DPROBES	8
#define MAX1D_PER_QUAD	6
#define MAX_DISPLAYED_1D_PROBES 6

#define NUMINTR		7	/* number of zoom intervals	*/

#define COUNTS		0
#define CONCENTRATION	1

#define CCs		0
#define LITERS		1

#define LOG		0
#define LINEAR		1


struct pms1d_probe	/* Nimbus	*/
	{
	char	name[NAMLEN];
	char	SerialNumber[NAMLEN];	/* So we can use libpms		*/
	int	type;		/* Used for switch cases		*/

	char	rangeName[NAMLEN];	/* FSSP only			*/
	int	range;

	/* Data related	*/
	int	nbins;			/* Number of bins		*/
	float		width[BINS_64];	/* Bin width			*/
	volatile float	actual[BINS_64];/* Counts data			*/
	volatile float	conc[BINS_64];	/* Concentration data		*/
	} ;

struct quad_info
	{
	int	nprobes;	/* # probes in this quadrant	*/
	int	probenum[MAX1D_PER_QUAD]; /* PMS1D probe nums in this quad */
/*	int	range;*/		/* FSSP only			*/
	int	sum_period;
	int	xposn[MAX1D_PER_QUAD * BINS_64];
	int	type;		/* COUNTS or CONCENTRATION	*/
	int	volume;		/* LITERS or CCs	*/
	double	ymin;
	double	ymax;
	int	xtype;		/* LOG or LINEAR	*/
	int	ytype;		/* LOG or LINEAR	*/
	} ;

struct accum_info
	{
	float	data[MAX1D_PER_QUAD][BINS_64];
	} ;

extern int			*nPMS1Dprobes;	/* Shared mem	*/
extern struct pms1d_probe	*PMS1Dprobe;	/* Shared mem	*/
extern struct quad_info		PMS1Dsetup[];
extern struct accum_info	PMS1Daccum[];


#define NumberPMS1Dprobes()		(*nPMS1Dprobes)
#define GetPMS1DprobeName(probe)	(PMS1Dprobe[probe].name)

#define isQuadrantYliters(q)	(PMS1Dsetup[q].volume == LITERS ? TRUE : FALSE)
#define isQuadrantXlinear(q)	(PMS1Dsetup[q].xtype == LINEAR ? TRUE : FALSE)
#define isQuadrantYlinear(q)	(PMS1Dsetup[q].ytype == LINEAR ? TRUE : FALSE)
#define isQuadrantXlog(q)	(PMS1Dsetup[q].xtype == LOG ? TRUE : FALSE)
#define isQuadrantYlog(q)	(PMS1Dsetup[q].ytype == LOG ? TRUE : FALSE)
#define isQuadrantCounts(q)	(PMS1Dsetup[q].type == COUNTS ? TRUE : FALSE)


bool	isPMS1DprobeSelected();

void	TogglePMS1Ddatatype(), TogglePMS1Dvolume(), TogglePMS1DxaxisType(),
	TogglePMS1DyaxisType(), TogglePMS1Dprobe(), set_period(), pmszoom();

/* winput.c only */
void	WritePMS1DdataToSHMEM(float []), SetUpPMS1D_SHMEM(),
	TransferDataToSHMEM(float record[], int index),
	AddToPMS1DprobeList(char [], char [], char [], int);

#endif
