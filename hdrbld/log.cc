/*
-------------------------------------------------------------------------
OBJECT NAME:	hdrbld.c

FULL NAME:

ENTRY POINTS:	AddToChangeLog()
		DiffSDI()
		DumpToChangeLog()

STATIC FNS:	none

DESCRIPTION:

INPUT:

OUTPUT:

REFERENCES:	Xwin.c, init.c then enters XtAppMainLoop()

REFERENCED BY:	user

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "hdrbld.h"

#include <time.h>

static int	logCnt = 0;
static char	*changeLog[512];

/* -------------------------------------------------------------------- */
void AddToChangeLog(char newMsg[])
{
  changeLog[logCnt] = (char *)GetMemory(strlen(newMsg)+1);
  strcpy(changeLog[logCnt], newMsg);
  ++logCnt;

}

/* -------------------------------------------------------------------- */
void DiffSDI(Sh *old, Sh *newS)
{
  int	i;
  char	temp[32];
  bool	CalsChanged = FALSE;


  if (strcmp(old->dsm_locn, newS->dsm_locn))
    {
    sprintf(buffer, "\tDSM: from %s to %s\n", old->dsm_locn, newS->dsm_locn);
    AddToChangeLog(buffer);
    }

  if (strcmp(old->type, newS->type))
    {
    sprintf(buffer, "\tType: from %s to %s\n", old->type, newS->type);
    AddToChangeLog(buffer);
    }

  if (old->rate != newS->rate)
    {
    sprintf(buffer, "\tRate: from %ld to %ld\n", old->rate, newS->rate);
    AddToChangeLog(buffer);
    }

  if (old->adsaddr != newS->adsaddr)
    {
    sprintf(buffer, "\tAddress: from %ld to %ld\n", old->adsaddr, newS->adsaddr);
    AddToChangeLog(buffer);
    }

  if (old->ch_gain != newS->ch_gain)
    {
    sprintf(buffer, "\tGain: from %ld to %ld\n", old->ch_gain, newS->ch_gain);
    AddToChangeLog(buffer);
    }

  if (old->ch_offset != newS->ch_offset)
    {
    sprintf(buffer, "\tOffset: from %ld to %ld\n", old->ch_offset, newS->ch_offset);
    AddToChangeLog(buffer);
    }

  if (old->order != newS->order)
    {
    sprintf(buffer, "\tCals: from %ld to %ld\n", old->order, newS->order);
    AddToChangeLog(buffer);
    CalsChanged = TRUE;
    }
  else
    for (i = 0; i < old->order; ++i)
      if (old->cof[i] != newS->cof[i])
        CalsChanged = TRUE;

  if (CalsChanged)
    {
    strcpy(buffer, "\tOld Cals: ");
    for (i = 0; i < old->order; ++i)
      {
      sprintf(temp, "%f ", old->cof[i]);
      strcat(buffer, temp);
      }

    AddToChangeLog(buffer);

    strcpy(buffer, "\tNew Cals: ");
    for (i = 0; i < newS->order; ++i)
      {
      sprintf(temp, "%f ", newS->cof[i]);
      strcat(buffer, temp);
      }

    AddToChangeLog(buffer);
    }

}	/* END DIFFSDI */

/* -------------------------------------------------------------------- */
void DumpToChangeLog(char FileName[])
{
  FILE		*fp;
  int		i;
  char		*slash;
  time_t	t;

  strcpy(buffer, FileName);
  slash = strrchr(buffer, '/');

  if (slash == NULL)
    buffer[0] = '\0';
  else
    slash[1] = '\0';

  strcat(buffer, "HeaderChangeLog");


  if ((fp = fopen(buffer, "a")) == NULL)
    {
    fprintf(stderr, "Can't open %s.\n", FileName);
    return;
    }

  t = time(NULL);
  fprintf(fp, "\n%s\n", ctime(&t));

  for (i = 0; i < logCnt; ++i)
    fprintf(fp, changeLog[i]);

  fclose(fp);

}	/* END DUMPTOCHANGELOG */

/* END LOG.C */
