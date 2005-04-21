/*
-------------------------------------------------------------------------
OBJECT NAME:	log.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern char *ADSrecord;
extern NR_TYPE *SampledData;

/* -------------------------------------------------------------------- */
void LogXlateMsg(char msg[])
{
  Hdr_blk *p = (Hdr_blk *)ADSrecord;

  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d %s", ntohs(p->hour),
	ntohs(p->minute), ntohs(p->second), messg.c_str());
}

/* -------------------------------------------------------------------- */
void Log2dXlateMsg(P2d_rec *p, char msg[])
{
  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d %s",
	p->hour, p->minute, p->second, messg.c_str());
}

/* -------------------------------------------------------------------- */
void LogStdMsg(char msg[])
{
  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d %s",
	(int)SampledData[timeindex[0]],
	(int)SampledData[timeindex[1]],
	(int)SampledData[timeindex[2]],
	messg.c_str());
}

/* END LOG.C */
