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

OUTPUT:		sdi, raw
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
#include <iostream>
#include <unistd.h>

#include "define.h"
#include "decode.h"
#include "amlib.h"

#include <raf/adsIOrt.h>

ADS_rtFile	*file;

char	*ProjectDirectory, *ProjectNumber;

/* Global to ease parameter passing in this file */
static int	InertialSystemCount;
static int	start, rate, probe_cnt = 0;
static char	*item_type, location[NAMELEN],
		*rawlist[MAX_DEFAULTS*4];


static void	add_raw_names(char name[]);
static RAWTBL	*add_name_to_RAWTBL(char vn[]);
static int	check_cal_coes(int order, float *coef);
static void	add_file_to_RAWTBL(char file[]), in_hdr(),
		in_sdi(Sh *vn), in_irs(Irs *vn), in_ophir3(Blk *vn);

static Header	*hdr;


extern int	Aircraft;

/* -------------------------------------------------------------------- */
bool VarCompareLT(const var_base *x, const var_base *y)
{
    return(strcmp(x->name, y->name) < 0);
}

/* -------------------------------------------------------------------- */
static std::vector<float> getCalsForADS2(Sh* vn)
{
  long order;
  float f[12];

  order = hdr->CalibrationOrder(vn);

  for (long i = 0; i < order; ++i)
    f[i] = hdr->CalibrationCoefficient(vn, (size_t)i);

  // Remove trailing zeroes.
  order = check_cal_coes(order, f);

  std::vector<float> cals;

  for (long i = 0; i < order; ++i)
    cals.push_back(f[i]);

  return cals;
}

/* -------------------------------------------------------------------- */
void DecodeHeader()
{
  void	*vn;
  FILE	*fp;

  InertialSystemCount = 0;

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
for (size_t i = 0; i < raw.size(); ++i)
  printf("%-12s%3d%5d\n", raw[i]->name, raw[i]->SampleRate, raw[i]->NRstart);
*/
}

  std::sort(raw.begin(), raw.end(), VarCompareLT);

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

    rp->cof = getCalsForADS2(vn);

    rp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;

    if (strcmp(rp->name, "HGM232") == 0) {  /* Status bit for APN-232   */
      rp = add_name_to_RAWTBL("HGM232S");
      rp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;
      }
    }


  /* Ok, it's strictly nth order polynomial, put it in the SDI table.
   */
  RAWTBL *cp = new RAWTBL(hdr->VariableName(vn));
  raw.push_back(cp);

  cp->convertFactor = hdr->AtoDconversionFactor(vn);
  cp->convertOffset = hdr->AtoDconversionOffset(vn);
  cp->ADSoffset = hdr->InterleaveOffset(vn) >> 1;

  strcpy(cp->type, hdr->SDItype(vn));
  cp->SampleRate	= hdr->SampleRate(vn);
  cp->ADSstart		= hdr->Start(vn) >> 1;
  cp->xlate             = decodeADS2analog;
  cp->Average		= NULL;
  cp->Modulo		= NULL;

  cp->cof = getCalsForADS2(vn);

}	/* END IN_SDI */

/* -------------------------------------------------------------------- */
/* NOTE: this proc is assuming the primary IRS probe is stored in the
 * header first.  Currently this is LEFT PIT.  xbuild.c is responsible
 * for insuring this.
 */
static void in_irs(Irs *)
{
  RAWTBL	*rp;
  char		*names[50];
  char		name[NAMELEN];

  if (InertialSystemCount == 0)
    location[0] = '\0';

  /* This is how I resolv conflicting names between IRS & INS
   */
  SetLookupSuffix("_IRS");

  ReadTextFile(IRSNAMES, names);

  for (size_t i = 0; names[i]; ++i)
    {
    sscanf(names[i], "%s %d", name, &rate);

    if ((rp = add_name_to_RAWTBL(name)) == (RAWTBL *)ERR)
      continue;
    }


  FreeTextFile(names);

  SetLookupSuffix((char *)NULL);

}	/* END IN_IRS */

/* -------------------------------------------------------------------- */
static void in_ophir3(Blk *)
{
  RAWTBL	*rp;
  char		*list[32];

  ReadTextFile(OPHIR3NAMES, list);

  for (size_t i = 0; list[i]; ++i)
    {
    if ((rp = add_name_to_RAWTBL(strtok(list[i], " \t"))) == (RAWTBL *)ERR)
      continue;

    rp->convertOffset	= 0;
    rp->convertFactor	= 1.0;
    long order		= atoi(strtok((char *)NULL, " \t"));

    for (long j = 0; j < order; ++j)
      rp->cof.push_back((float)atof(strtok((char *)NULL, " \t")));
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

  if ((indx = SearchDERIVEFTNS(name)) == ERR)
    {
    std::cerr << "Throwing away " << name << ", has no decode function.\n";
    return((RAWTBL *)ERR);
    }

  RAWTBL *rp = new RAWTBL(name);
  raw.push_back(rp);

  if (*location)
    strcat(rp->name, location);

  rp->xlate		= deriveftns[indx].xlate;

  rp->ADSstart		= start >> 1;
  rp->ADSoffset		= 1;
  rp->SampleRate	= rate;
  rp->Length		= 1;
  rp->convertOffset	= 0;
  rp->convertFactor	= 1.0;

  rp->Average		= NULL;
  rp->Modulo		= NULL;
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

/* -------------------------------------------------------------------- */
var_base::var_base(const char s[])
{
  strcpy(name, s);
  varid = -1;
  LRstart = SRstart = HRstart = 0;

  SampleRate = 0;
  Length = 1;
  ProbeCount = 0;
  ProbeType = 0;

  OutputRate = Config::LowRate;

  Dirty = false;
  Output = true;
  DependedUpon = false;

  Modulo = 0;
  DataQuality   = 0;
}

RAWTBL::RAWTBL(const char s[]) : var_base(s)
{
  type[0] = '\0';
  Initializer = 0;
  xlate = 0;
  Average = 0;

  convertOffset = 0;
  convertFactor = 1.0;

  StaticLag = 0;
  DynamicLag = 0;
  SpikeSlope = 0.0;
}

DERTBL::DERTBL(const char s[]) : var_base(s)
{
  Initializer = 0;
  compute = 0;
  ndep = 0;
}

/* END HDR_DECODE.C */
