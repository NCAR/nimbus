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
void LogXlateMsg(const char msg[])
{
  Hdr_blk *p = (Hdr_blk *)ADSrecord;

  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d: %s", ntohs(p->hour),
	ntohs(p->minute), ntohs(p->second), messg.c_str());
}

/* -------------------------------------------------------------------- */
void Log2dXlateMsg(P2d_rec *p, const char msg[])
{
  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d: %s",
	p->hour, p->minute, p->second, messg.c_str());
}

/* -------------------------------------------------------------------- */
void LogThisRecordMsg(NR_TYPE *record, const char msg[])
{
  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  fprintf(stderr, "%02d:%02d:%02d: %s",
	(int)record[timeIndex[0]],
	(int)record[timeIndex[1]],
	(int)record[timeIndex[2]],
	messg.c_str());
}

/* -------------------------------------------------------------------- */
void LogStdMsg(const char msg[])
{
  LogThisRecordMsg(SampledData, msg);
}

/* END LOG.C */
