/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_blanks.c

FULL NAME:	Read BlankOuts file.

ENTRY POINTS:	ReadBlankOuts()

DESCRIPTION:	Read proj/###/Production/BlanksOuts file, which contains
		start and end times of of variables that are considered
		bad and should be written in with nan.

INPUT:		${PROJ_DIR}/###/Production/BlankOuts

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void ReadBlankOuts()
{
  int	index;
  int	sTime[4], eTime[4];
  char	*bo[512], target[NAMELEN];

  sprintf(buffer, "%s.%s", BLANKVARS.c_str(), cfg.FlightNumber().c_str());
  if (AccessProjectFile(buffer, "r") == FALSE)
    return;

  ReadTextFile(buffer, bo);

  for (int i = 0; bo[i]; ++i)
  {
    sscanf(bo[i], "%s %d:%d:%d %d:%d:%d", target,
        &sTime[0], &sTime[1], &sTime[2], &eTime[0], &eTime[1], &eTime[2]);

    /* Turn time period into record numbers. */
    sTime[3] = SecondsSinceMidnite(sTime);
    eTime[3] = SecondsSinceMidnite(eTime);

    if (eTime[3] == sTime[3])
      eTime[3] = sTime[3] + 1;

    if (eTime[3] < sTime[3])
      eTime[3] += 86400;

    std::pair<int, int> tm(sTime[3], eTime[3]);

    if ((index = SearchTable(raw, target)) != ERR)
      raw[index]->blank_out.push_back(tm);

    if ((index = SearchTable(derived, target)) != ERR)
      derived[index]->blank_out.push_back(tm);
  }

  FreeTextFile(bo);

}	/* END READBLANKOUTS */

/* END RD_BLANKS.C */
