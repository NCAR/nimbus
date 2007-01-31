/*
-------------------------------------------------------------------------
OBJECT NAME:	save.c

FULL NAME:	Save DB

ENTRY POINTS:	SaveVarDB()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"
#include "portable.h"

extern long VarDB_RecLength, VarDB_nRecords;

/* -------------------------------------------------------------------- */
int SaveVarDB(const char fileName[])
{
  FILE	*fp;
  int	rc;

  if ((fp = fopen(fileName, "w+")) == NULL)
    {
    fprintf(stderr, "VarDB: can't open %s\n", fileName);
    return(ERR);
    }

  VarDB_Hdr.nRecords = htonl(VarDB_nRecords);
  VarDB_Hdr.RecordLen = htonl(VarDB_RecLength);

  fwrite((char *)&VarDB_Hdr, sizeof(struct vardb_hdr), 1, fp);

  if ((rc = fwrite((char *)VarDB, VarDB_RecLength, VarDB_nRecords,fp)) != VarDB_nRecords)
    {
    fprintf(stderr, "VarDB: wrote %d out of %d entries\n", rc, VarDB_nRecords);
    fclose(fp);
    return(ERR);
    }

  fclose(fp);
  return(OK);

}	/* END SAVEVARDB */

/* END SAVE.C */
