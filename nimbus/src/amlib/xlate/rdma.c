/*
-------------------------------------------------------------------------
OBJECT NAME:	rdma.c

FULL NAME:	Translate RDMA houskeeping variables to Nimbus record
		format

ENTRY POINTS:	xlrdma*()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2001-07
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

static int	timeCounter = 0;
static NR_TYPE	temp, press;
static RAWTBL	*ardma_varp;

extern char	*ADSrecord;
extern NR_TYPE	*SampledData;

NR_TYPE	crdma_cnc[128];

/* -------------------------------------------------------------------- */
void xlardma(RAWTBL *varp, void *input, NR_TYPE *np)
{
  ardma_varp = varp;
  memset((char *)np, 0, varp->Length * sizeof(NR_TYPE));

}

/* -------------------------------------------------------------------- */
void xlrdmahvps(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Rdma_blk	*rdma = (Rdma_blk *)input;

  static int firstTime = TRUE;
  static FILE *outFP = NULL, *rdmapFP = NULL;

  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->hvps);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;

  for (size_t i = 0; i < 64; ++i)
    crdma_cnc[i] = floatNAN;

  float *ardma = &SampledData[ardma_varp->SRstart];

  /* Two ASCII files will be written.  .rdma will be the raw data, cnts and
   * concentrations, for archival to the MASS Store.  .cnts.rdma is counts
   * only to be fed into the RDMA processor rdmap.
   */
  if (firstTime)
    {
    extern char		OutputFileName[];	// imported from cb_main.c

    if (cfg.ProductionRun())
      sprintf(buffer, "%s/%s%s.rdma", getenv("PROD_DATA"),
		cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());
    else
      sprintf(buffer, "%s/%s%s.rdma", getenv("DATA_DIR"),
		cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());

    if (cfg.ProcessingMode() != Config::RealTime)
      {
      if ((outFP = fopen(buffer, "w+")) == NULL)
        fprintf(stderr, "xlate/rdma.c: can't open %s\n", buffer);


      sprintf(buffer, "%s/%s%s.cnts.rdma", getenv("DATA_DIR"),
		cfg.ProjectNumber().c_str(), cfg.FlightNumber().c_str());

      if ((rdmapFP = fopen(buffer, "w+")) == NULL)
        fprintf(stderr, "xlate/rdma.c: can't open %s\n", buffer);
      }
    else
      rdmapFP = outFP = NULL;

    if (rdmapFP)
      fprintf(rdmapFP, "%s\n", OutputFileName);	/* Stamp netCDF file name */

    press = temp = 0.0;

    firstTime = FALSE;
    }

  if (rdma->item_type[0] == 'c')
    {
    if (outFP)
      {
      fprintf(outFP, "%s @ %02d:%02d:%02d\n", rdma->item_type,
	ntohs(((short *)ADSrecord)[1]), ntohs(((short *)ADSrecord)[2]),
	ntohs(((short *)ADSrecord)[3]));

      /* This code to put the labview concentrations array into the netCDF
       * The cell diameters will not match.  This is for "debugging" purposes
       * only.
       */
/*
      crdma_cnc[0] = (NR_TYPE)ntohf(rdma->scan[0]);

      for (size_t i = 0; ntohf(rdma->scan[i]) != -99.0; ++i)
        {
        fprintf(outFP, "%.2f ", (NR_TYPE)ntohf(rdma->scan[i]));

        if (i > 1 && strcmp(rdma->item_type, "cnup") == 0)
          crdma_cnc[i] = (NR_TYPE)ntohf(rdma->scan[i]);

        if (i > 1 && strcmp(rdma->item_type, "cndn") == 0)
          crdma_cnc[i] = (NR_TYPE)ntohf(rdma->scan[64-i]);
        }
*/
      fprintf(outFP, "\n"); fflush(outFP);
      }


    /* Drop concentration arrays, we are recomputing anew.
     */
    if (strcmp(rdma->item_type, "cnts"))
      return;

    press /= timeCounter;
    temp /= timeCounter;

    if (press > 2.0 || press < 0.0)
      press = 0.83;

    if (temp > 100.0 || temp < 0.0)
      temp = 20.0;
 
    if (rdmapFP)
      {
      fprintf(rdmapFP, "%02d/%02d/%02d %02d:%02d:%02d  ",
        ntohs(((short *)ADSrecord)[4]), ntohs(((short *)ADSrecord)[5]),
        ntohs(((short *)ADSrecord)[6]), ntohs(((short *)ADSrecord)[1]),
	ntohs(((short *)ADSrecord)[2]), ntohs(((short *)ADSrecord)[3]));

      fprintf(rdmapFP, "%f %f  ", temp+Kelvin, press);

      for (size_t i = 0; ntohf(rdma->scan[i]) != -99.0; ++i)
        {
        fprintf(rdmapFP, "%.2f ", (NR_TYPE)ntohf(rdma->scan[i]));
        ardma[i+1] = (NR_TYPE)ntohf(rdma->scan[i]);
        }

      fprintf(rdmapFP, "-99.0\n"); fflush(rdmapFP);
      }

    timeCounter = 0;
    press = temp = 0.0;
    }
  else
    {
    ++timeCounter;
    }
}	/* END XLRDMAHVPS */

/* -------------------------------------------------------------------- */
void xlrdmavsh(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->vsh);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmaqa(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qa);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmaqsh(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qsh);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmaqs(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qs);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmaqex(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qex);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmavd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->vd);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmarh(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->rh);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
}

/* -------------------------------------------------------------------- */
void xlrdmatemp(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->temp);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
  else
    temp += *np;
}

/* -------------------------------------------------------------------- */
void xlrdmapabs(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->pabs);
  if (*np < -32000.0 || *np > 32000.0)
    *np = floatNAN;
  else
    press += *np;
}

/* END RDMA.C */
