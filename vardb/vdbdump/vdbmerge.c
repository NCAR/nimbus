/*
-------------------------------------------------------------------------
OBJECT NAME:	vdbmerge.c

FULL NAME:	VarDB merge

DESCRIPTION:	This merges /home/local/proj/defaults/VarDB into another
		VarDB.  This allows indivdual copies to be brought upto
		date quickly and easily.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include <raf/vardb.h>

struct vardb_hdr	VarDB_Hdr1, VarDB_Hdr2;
struct var_v2		VarDB1[1000], VarDB2;

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  FILE	*fp1, *fp2;
  int	rc, newCount;
  int	i, j;
  int	Replaced;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: vdbmerge VarDB_filename\n");
    exit(1);
  }


  if ((fp1 = fopen(argv[1], "r+b")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open %s\n", argv[1]);
    return(ERR);
  }

  if ((fp2 = fopen("/home/local/proj/defaults/VarDB", "rb")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open $LOCAL/proj/defaults/VarDB\n");
    fclose(fp1);
    return(ERR);
  }

  fread((char *)&VarDB_Hdr1, sizeof(struct vardb_hdr), 1, fp1);
  fread((char *)&VarDB_Hdr2, sizeof(struct vardb_hdr), 1, fp2);

  if (VarDB_Hdr1.Version != VarDB_Hdr2.Version)
  {
    fprintf(stderr, "VarDB: versions don't match.\n");
    fclose(fp1);
    fclose(fp2);
    return(ERR);
  }

  newCount = ntohl(VarDB_Hdr1.nRecords);
  fread((char *)VarDB1, sizeof(struct var_v2), newCount, fp1);

  for (i = 0; i < ntohl(VarDB_Hdr2.nRecords); ++i)
  {
    fread((char *)&VarDB2, sizeof(struct var_v2), 1, fp2);

    Replaced = FALSE;

    for (j = 0; j < newCount; ++j)
      if (strcmp(VarDB1[j].Name, VarDB2.Name) == 0)
      {
        VarDB1[j] = VarDB2;
        Replaced = TRUE;
        break;
      }

    if (!Replaced)	/* then ADD	*/
      VarDB1[newCount++] = VarDB2;
  }

  rewind(fp1);
  fwrite((char *)&VarDB_Hdr1, sizeof(struct vardb_hdr), 1, fp1);
  fwrite((char *)VarDB1, sizeof(struct var_v2), newCount, fp1);

  fclose(fp1);
  fclose(fp2);

  return(0);

}	/* END MAIN */

/* END VDBMERGE.C */
