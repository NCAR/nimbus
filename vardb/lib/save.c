/*
-------------------------------------------------------------------------
OBJECT NAME:	save.c

FULL NAME:	Save DB

ENTRY POINTS:	SaveVarDB()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		OK/ERR

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-7
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"
#include "portable.h"

extern long VarDB_RecLength, VarDB_nRecords;

/* -------------------------------------------------------------------- */
SaveVarDB(const char fileName[])
{
  FILE	*fp;
  int	rc;
//strcat(fileName, ".xml");
  if ((fp = fopen(fileName, "w+")) == NULL)
    {
    fprintf(stderr, "VarDB: can't open %s\n", fileName);
    return(ERR);
    }

//SaveXML(fp);
//goto vexit;

  VarDB_Hdr.nRecords = htonl(VarDB_nRecords);
  VarDB_Hdr.RecordLen = htonl(VarDB_RecLength);

  fwrite((char *)&VarDB_Hdr, sizeof(struct vardb_hdr), 1, fp);

  if ((rc = fwrite((char *)VarDB, VarDB_RecLength, VarDB_nRecords,fp)) != VarDB_nRecords)
    {
    fprintf(stderr, "VarDB: wrote %d out of %d entries\n", rc, VarDB_nRecords);
    fclose(fp);
    return(ERR);
    }

vexit:
  fclose(fp);
  return(OK);

}	/* END SAVEVARDB */

/* -------------------------------------------------------------------- */
SaveXML(FILE *out)
{
  int	i;

  fprintf(out, "<?xml version='1.0'?>\n");
  fprintf(out, "<vardb version='3.0'>\n");

  for (i = 0; i < VarDB_nRecords; ++i)
  {
    fprintf(out, "  <variable name='%s'>\n", ((struct var_v2 *)VarDB)[i].Name);
    fprintf(out, "    <long_name>%s</long_name>\n",
	((struct var_v2 *)VarDB)[i].Title);
    fprintf(out, "    <units>%s</units>\n",
	((struct var_v2 *)VarDB)[i].Units);
    fprintf(out, "    <alt_units>%s</alt_units>\n",
	((struct var_v2 *)VarDB)[i].AlternateUnits);

    fprintf(out, "    <category>%s</category>\n",
	VarDB_GetCategoryName(((struct var_v2 *)VarDB)[i].Name));

    fprintf(out, "    <valid_range>%f %f</valid_range>\n",
	ntohf(((struct var_v2 *)VarDB)[i].MinLimit),
	ntohf(((struct var_v2 *)VarDB)[i].MaxLimit));

    if (((struct var_v2 *)VarDB)[i].type == FIXED)
      fprintf(out, "    <display_range type='Fixed'>%f %f</display_range>\n",
	  ntohf(((struct var_v2 *)VarDB)[i].FixedRange[0]),
	  ntohf(((struct var_v2 *)VarDB)[i].FixedRange[1]));
    else
      fprintf(out, "    <display_range type='Floating'>%f</display_range>\n",
	  ntohf(((struct var_v2 *)VarDB)[i].FloatRange));

    fprintf(out, "  </variable>\n");
  }

  fprintf(out, "</vardb>\n");

}

/* END SAVE.C */
