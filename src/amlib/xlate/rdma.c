/*
-------------------------------------------------------------------------
OBJECT NAME:	rdma.c

FULL NAME:	Translate RDMA houskeeping variables to Nimbus record
		format

ENTRY POINTS:	xlrdma*()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2001-02
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static int	timeCounter = 0;
static float	temp, press;

extern char	*ADSrecord;


/* -------------------------------------------------------------------- */
void xlardma(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	i;

  memset((char *)np, 0, varp->Length * sizeof(NR_TYPE));

}

/* -------------------------------------------------------------------- */
void xlrdmahvps(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	i;
  Rdma_blk	*rdma = (Rdma_blk *)input;

  static int firstTime = TRUE;
  static FILE *outFP = NULL, *rdmapFP = NULL;

  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->hvps);


  /* Two ASCII files will be written.  .rdma will be the raw data, cnts and
   * concentrations, for archival to the MASS Store.  .cnts.rdma is counts
   * only to be fed into the RDMA processor rdmap.
   */
  if (firstTime)
    {
    extern char		OutputFileName[];	// imported from cb_main.c

    if (ProductionRun)
      sprintf(buffer, "%s/%s%s.rdma", getenv("PROD_DATA"),
			ProjectNumber, FlightNumber);
    else
      sprintf(buffer, "%s/%s%s.rdma", getenv("DATA_DIR"),
			ProjectNumber, FlightNumber);

    if (Mode != REALTIME)
      {
      if ((outFP = fopen(buffer, "w+")) == NULL)
        fprintf(stderr, "xlate/rdma.c: can't open %s\n", buffer);


      sprintf(buffer, "%s/%s%s.cnts.rdma", getenv("DATA_DIR"),
			ProjectNumber, FlightNumber);

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


      for (i = 0; ntohf(rdma->scan[i]) != -99.0; ++i)
        fprintf(outFP, "%.2f ", (NR_TYPE)ntohf(rdma->scan[i]));

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

      fprintf(rdmapFP, "%f %f  ", temp+273.16, press);

      for (i = 0; ntohf(rdma->scan[i]) != -99.0; ++i)
        fprintf(rdmapFP, "%.2f ", (NR_TYPE)ntohf(rdma->scan[i]));

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

}

/* -------------------------------------------------------------------- */
void xlrdmaqa(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qa);

}

/* -------------------------------------------------------------------- */
void xlrdmaqsh(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qsh);

}

/* -------------------------------------------------------------------- */
void xlrdmaqs(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qs);

}

/* -------------------------------------------------------------------- */
void xlrdmaqex(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->qex);

}

/* -------------------------------------------------------------------- */
void xlrdmavd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->vd);

}

/* -------------------------------------------------------------------- */
void xlrdmarh(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->rh);

}

/* -------------------------------------------------------------------- */
void xlrdmatemp(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->temp);

  temp += *np;

}

/* -------------------------------------------------------------------- */
void xlrdmapabs(RAWTBL *varp, void *input, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohf(((Rdma_blk *)input)->pabs);

  press += *np;

}

/* END RDMA.C */
