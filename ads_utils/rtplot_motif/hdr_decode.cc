/*
-------------------------------------------------------------------------
OBJECT NAME:	hdr_decode.c

FULL NAME:	Decode Header

ENTRY POINTS:	DecodeHeader()

STATIC FNS:	in_hdr(), in_sdi(), in_irs(), in_ophir3(), in_pms1d(),
		in_pms1dv2(), locatePMS(), add_file_to_???TBL(),
		add_name_to_???TBL(), add_derived_names(), in_ins(),
		check_cal_coes()

DESCRIPTION:	Read header & add variables to appropriate table.  There
		are 3 major tables here:
			- SDI table
			- Raw table (From block probes)
			- Derived table

		This module is the brunt end interface between an ADS tape
		and the rest of nimbus.

INPUT:		Header filename.

OUTPUT:		sdi, nsdi, raw, nraw, derived, nderive
		(These globals are initialized in this file)

REFERENCES:	Header API (libhdr_api.a)
		OpenProjectFile(), AccessProjectFile()
		ReadTextFile(), FreeTextFile()
		SearchList(), SearchDERIVEFTNS()
		ReadStaticLags(), SetUpDependencies(), ReadDefaultsFile()
		SortTable()

REFERENCED BY:	rtplot.c

NOTE:		This file is from hell.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-9
-------------------------------------------------------------------------
*/

#include <cstdlib>
#include <unistd.h>

#include "define.h"
#include "decode.h"
#include "amlib.h"

#include "raf/adsIOrt.h"

ADS_rtFile	*file;

char	*ProjectDirectory, *ProjectNumber;

/* Global to ease parameter passing in this file */
static int	InertialSystemCount;
static int	start, rate, probe_cnt = 0;
static char	*item_type, location[NAMELEN],
		*rawlist[MAX_RAW*2];


static void	add_raw_names(char name[]);
static RAWTBL	*add_name_to_RAWTBL(char vn[]);
static int	check_cal_coes(int order, float *coef);
static void	add_file_to_RAWTBL(char file[]), in_hdr(),
		in_sdi(Sh *vn), in_irs(Irs *vn), in_ophir3(Blk *vn);

static Header	*hdr;


extern int	Aircraft;

/* -------------------------------------------------------------------- */
int DecodeHeader(char dummy[])
{
  void	*vn;
  FILE	*fp;
  char	*loc;

  nsdi = nraw = InertialSystemCount = 0;

  probe_cnt = 0;

  file = new ADS_rtFile();
  hdr = file->hdr;

  Aircraft = atoi(&(hdr->Aircraft())[1]);
  strcpy(FlightNumber, hdr->FlightNumber());

  ProjectDirectory = getenv("PROJ_DIR");
  ProjectNumber = hdr->ProjectNumber();

  sprintf(buffer, "%s/%s", ProjectDirectory, ProjectNumber);
  if (access(buffer, R_OK) == ERR)
    {
    if ((fp = OpenProjectFile("%s/%s/ProjectName", "r", RETURN)) != NULL)
      {
      fgets(buffer, 512, fp);
      buffer[strlen(buffer)-1] = '\0';	/* Remove '\n'	*/

      ProjectName = new char[strlen(buffer)+1];
      strcpy(ProjectName, buffer);
      title = ProjectName;
      title += " - ";
      title += hdr->ProjectNumber();
      fclose(fp);
      }
    }
  else
    ProjectName = "";


  ReadTextFile(RAWNAMES, rawlist);

  SetLookupSuffix((char *)NULL);


  /* This is the main loop, loop through all variables in header
   */
  for (vn = hdr->GetFirst(); vn; vn = hdr->GetNext())
    {
    start = hdr->Start(vn);
    location[0] = '\0';

    /* Initialize variable/probe into appropriate table(s).
     */
    if (!strcmp(hdr->ItemType(vn), HSKP_STR) ||
        !strcmp(hdr->ItemType(vn), DIGOUT_STR) ||
        !strcmp(hdr->ItemType(vn), SDI_STR))
      {
      in_sdi((Sh *)vn); 
      continue;
      }


    if (!strcmp(hdr->ItemType(vn), HDR_STR))
      in_hdr();
    else
    if (!strcmp(hdr->ItemType(vn), IRS_STR))
      {
      if (hdr->AircraftLocation((Irs *)vn))
        {
        location[0] = '_';
        strcpy(&location[1], hdr->AircraftLocation((Irs *)vn));
        }

      strcpy(buffer, hdr->ItemType(vn));
      strcat(buffer, location);
      in_irs((Irs *)vn);
      }
    else
    if (!strcmp(hdr->ItemType(vn), UVHYG_STR))
      {
      rate = sizeof(struct Uv_blk) / sizeof(struct Uv_smp);
      add_raw_names(item_type);
      }
    else
    if (!strcmp(hdr->ItemType(vn), OPHIR3_STR))
      {
      rate = 1;
      in_ophir3((Blk *)vn);
      }
    else
    if (!strcmp(hdr->ItemType(vn), GPS_TRIM_STR) ||
        !strcmp(hdr->ItemType(vn), GPS_TANS2_STR) ||
        !strcmp(hdr->ItemType(vn), GPS_TANS3_STR))
      {
      rate = 1;
      add_raw_names(item_type);
      }
    else
    if (!strcmp(hdr->ItemType(vn), PPS_GPS_STR))
      {
      rate = 1;
      add_raw_names(item_type);
      }
    else
    if (!strcmp(hdr->ItemType(vn), HW_GPS_STR))
      {
      if (hdr->AircraftLocation((Blk *)vn))
        {
        location[0] = '_';
        strcpy(&location[1], hdr->AircraftLocation((Blk *)vn));
        }

      add_file_to_RAWTBL(GPSHWNAMES);
      }
    else
    if (!strcmp(hdr->ItemType(vn), DME_STR))
      {
      rate = 1;
      add_raw_names(item_type);
      }
    else
    if (!strcmp(hdr->ItemType(vn), DPRES_STR))
      {
      rate = 5;
      add_raw_names(item_type);
      }
    else
    if (!strcmp(hdr->ItemType(vn), SER_STR))
      add_name_to_RAWTBL("SERIAL");
    }



{
/*
int	i;
for (i = 0; i < nsdi; ++i)
  printf("%-12s%5d\n", sdi[i]->name, sdi[i]->convertOffset);

for (i = 0; i < nraw; ++i)
  printf("%-12s%3d%5d\n", raw[i]->name, raw[i]->SampleRate, raw[i]->NRstart);

*/
}


  sdi[nsdi] = NULL;
  raw[nraw] = NULL;

  SortTable((char **)sdi, 0, nsdi - 1);
  SortTable((char **)raw, 0, nraw - 1);

  return(0);

}	/* END DECODEHEADER */

/* -------------------------------------------------------------------- */
static void in_hdr()
{
  /* These items must be added to the tables first (i.e. don't
   * add any variables above the main loop in the fn above).
   */
  start = 0; rate = 1;
  add_name_to_RAWTBL("HOUR");
  add_name_to_RAWTBL("MINUTE");
  add_name_to_RAWTBL("SECOND");

}	/* END IN_HDR */

/* -------------------------------------------------------------------- */
static void in_sdi(Sh *vn)
{
  int		i;
  SDITBL	*cp;

  if (hdr->SampleRate(vn) == 5000)
    return;

  if (strcmp(hdr->VariableName(vn), "DUMMY") == 0)
    return;

  /* By placing an SDI variable into the 'deriveftns[]' list
   * (ninc/amlib.fns) the variable will automatically be placed
   * into the 'raw' list instead of 'sdi' (i.e. if you have an SDI
   * variable that requires special processing).
   */
  if (SearchDERIVEFTNS(hdr->VariableName(vn)) != ERR)
    {
    RAWTBL	*rp;

    rp = add_name_to_RAWTBL(hdr->VariableName(vn));

    rp->convertFactor = hdr->AtoDconversionFactor(vn);
    rp->convertOffset = hdr->AtoDconversionOffset(vn);

    rp->order = hdr->CalibrationOrder(vn);

    for (i = 0; i < rp->order; ++i)
      rp->cof[i] = hdr->CalibrationCoefficient(vn, i);

    rp->order = check_cal_coes(rp->order, rp->cof);

    rp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;

    if (strcmp(rp->name, "HGM232") == 0) {  /* Status bit for APN-232   */
      rp = add_name_to_RAWTBL("HGM232S");
      rp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;
      }
    }


  /* Ok, it's strictly nth order polynomial, put it in the SDI table.
   */
  if (nsdi == MAX_SDI)
    {
    cerr << "MAX_SDI reached, modify in nimbus.h and recompile.\n";
    exit(1);
    }

  cp = sdi[nsdi++] = new SDITBL;

  cp->convertFactor = hdr->AtoDconversionFactor(vn);
  cp->convertOffset = hdr->AtoDconversionOffset(vn);
  cp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;

  strcpy(cp->name, hdr->VariableName(vn));
  strcpy(cp->type, hdr->SDItype(vn));
  cp->SampleRate	= hdr->SampleRate(vn);
  cp->OutputRate	= LOW_RATE;
  cp->ADSstart		= hdr->Start(vn) >> 1;
  cp->StaticLag		= 0;
  cp->SpikeSlope	= 0;
  cp->Average		= NULL;
  cp->Dirty		= false;
  cp->Modulo		= NULL;
  cp->Output		= true;
  cp->DependedUpon	= false;

  cp->order = hdr->CalibrationOrder(vn);

  for (i = 0; i < cp->order; ++i)
    cp->cof[i] = hdr->CalibrationCoefficient(vn, i);

  cp->order = check_cal_coes(cp->order, cp->cof);
  cp->order = check_cal_coes(cp->order, cp->cof);

}	/* END IN_SDI */

/* -------------------------------------------------------------------- */
/* NOTE: this proc is assuming the primary IRS probe is stored in the
 * header first.  Currently this is LEFT PIT.  xbuild.c is responsible
 * for insuring this.
 */
static void in_irs(Irs *vn)
{
  int		i;
  RAWTBL	*rp;
  char		*names[50];
  char		name[NAMELEN];

  if (InertialSystemCount == 0)
    location[0] = '\0';

  /* This is how I resolv conflicting names between IRS & INS
   */
  SetLookupSuffix("_IRS");

  ReadTextFile(IRSNAMES, names);

  for (i = 0; names[i]; ++i)
    {
    sscanf(names[i], "%s %d", name, &rate);

    if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
      continue;
    }


  FreeTextFile(names);

  SetLookupSuffix((char *)NULL);

}	/* END IN_IRS */

/* -------------------------------------------------------------------- */
static void in_ophir3(Blk *vn)
{
  RAWTBL	*rp;
  int		i, j;
  char		*list[32];

  ReadTextFile(OPHIR3NAMES, list);

  for (i = 0; list[i]; ++i)
    {
    if ((rp = add_name_to_RAWTBL(strtok(list[i], " \t"))) == (RAWTBL *)ERR)
      continue;

    rp->convertOffset	= 0;
    rp->convertFactor	= 1.0;
    rp->order		= atoi(strtok((char *)NULL, " \t"));

    for (j = 0; j < rp->order; ++j)
      rp->cof[j] = (float)atof(strtok((char *)NULL, " \t"));
    }

  FreeTextFile(list);

}	/* END IN_OPHIR3 */


/* -------------------------------------------------------------------- */
/* Header Decode Atomic functions					*/
/* -------------------------------------------------------------------- */
static void add_raw_names(char name[])
{
  char  *p;
  char  buff[512];

  /* Find variable in raw list and add ALL associated names to raw table.
   */
  if ( (p = SearchList(rawlist, name)) )
    {
    strcpy(buff, p);
    p = strtok(buff, " \t");

    while ( (p = strtok((char *)NULL, " \t")) )
      add_name_to_RAWTBL(p);
    }

}       /* END ADD_RAW_NAMES */

/* -------------------------------------------------------------------- */
static void add_file_to_RAWTBL(char filename[])
{
  FILE	*fp;

  fp = OpenProjectFile(filename, "r", EXIT);

  while (fscanf(fp, "%s", buffer) != EOF)
    if (buffer[0] != '#')
      add_name_to_RAWTBL(buffer);

  fclose(fp);

}	/* END ADD_FILE_TO_RAWTBL */

/* -------------------------------------------------------------------- */
static RAWTBL *add_name_to_RAWTBL(char name[])
{
  int		indx;
  RAWTBL	*rp;

  if ((indx = SearchDERIVEFTNS(name)) == ERR)
    {
    cerr << "Throwing away " << name << ", has no decode function.\n";
    return((RAWTBL *)ERR);
    }

  if (nraw == MAX_RAW)
    {
    cerr << "MAX_RAW reached, modify in nimbus.h and recompile.\n";
    exit(1);
    }

  rp = raw[nraw++] = new RAWTBL;

  strcpy(rp->name, name);
  if (*location)
    strcat(rp->name, location);

  rp->xlate		= (void (*) (void *, void *, float *))deriveftns[indx].xlate;

  rp->ADSstart		= start >> 1;
  rp->ADSoffset		= 1;
  rp->SampleRate	= rate;
  rp->OutputRate	= LOW_RATE;
  rp->Length		= 1;
  rp->convertOffset	= 0;
  rp->convertFactor	= 1.0;
  rp->order		= 0;
  rp->StaticLag		= 0;
  rp->SpikeSlope	= 0;
  rp->DynamicLag	= 0;

  rp->Dirty		= false;
  rp->Average		= NULL;
  rp->Modulo		= NULL;
  rp->Output		= true;
  rp->DependedUpon	= false;
  rp->ProbeType		= 0;

  return(rp);

}	/* END ADD_NAME_TO_RAWTBL */

/* -------------------------------------------------------------------- */
/* Strip out trailing 0 cal coe's
 */
static int check_cal_coes(int order, float *coef)
{
  do
    {
    --order;
    }
  while (order >= 0 && coef[order] == 0.0);

  return(order + 1);

}	/* END CHECK_CAL_COES */

/* END HDR_DECODE.C */
