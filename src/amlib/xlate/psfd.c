/*
-------------------------------------------------------------------------
OBJECT NAME:	psfd.c

FULL NAME:	Translate PSFD into new record format

ENTRY POINTS:	xlpsfd()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <unistd.h>

/* -------------------------------------------------------------------- */
void xlpsfd(RAWTBL *varp, void *p, NR_TYPE *output)
{
  ushort	*input = (ushort *)p;
  unsigned long	psfd;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    psfd = (~input[varp->ADSoffset * i - 1] & 0x0001) << 16;
    psfd |= ~input[varp->ADSoffset * i] & 0x0000ffff;

    output[i] = 0.033864 * psfd;
    }

}	/* END XLPSFD */

/* -------------------------------------------------------------------- */
void xlpsfd_ADS2(RAWTBL *varp, void *input, NR_TYPE *output)
{
  unsigned long	psfd;
  ushort	*p1_input, *p2_input;

  static bool	firstTime = TRUE;
  static SDITBL	*psfd1, *psfd2;

  if (firstTime)
    {
    int	indx1, indx2;

    indx1 = SearchTable(sdi, "PSFD1");
    indx2 = SearchTable(sdi, "PSFD2");

    if ((int)psfd1 == ERR || (int)psfd2 == ERR)
      {
      LogMessage("PSFD: can't find PSFD1 or PSFD2, fatal.\n");
      sleep(2);
      exit(1);
      }

    psfd1 = sdi[indx1];
    psfd2 = sdi[indx2];

    firstTime = FALSE;
    }

  p1_input = (ushort *)input;
  p2_input = &((ushort *)input)[-(psfd1->ADSstart) + (psfd2->ADSstart)];

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    psfd = (ntohs(p2_input[psfd2->ADSoffset * i]) & 0x0001) << 16;
    psfd |= ntohs(p1_input[psfd1->ADSoffset * i]) & 0x0000ffff;

    output[i] = 0.033864 * psfd;
    }

}	/* END XLPSFD */

/* -------------------------------------------------------------------- */
void xlxpsfd(RAWTBL *varp, void *input, NR_TYPE *output)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(((Dpres_blk *)input)->press[i]) * 10.0;
 
}	/* END XLETORQ */

/* END PSFD.C */
