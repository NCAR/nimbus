/*
-------------------------------------------------------------------------
OBJECT NAME:	map_std_name.c

FULL NAME:	VarDB merge

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	Take Standard Name information from master file and map
		into an existing file.
		VarDB.  This allows indivdual copies to be brought upto
		date quickly and easily.

INPUT:		FileName

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
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
  int	rc, masterCount, slaveCount;
  int	i, j;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: map_std_name VarDB_filename\n");
    exit(1);
  }


  if ((fp1 = fopen(argv[1], "r+b")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open %s\n", argv[1]);
    return(ERR);
  }

  if ((fp2 = fopen("/jnet/tmp/chris/raf/proj/defaults/VarDB", "rb")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open $JLOCAL/proj/defaults/VarDB\n");
    fclose(fp1);
    return(ERR);
  }

  fread((char *)&VarDB_Hdr1, sizeof(struct vardb_hdr), 1, fp1);
  fread((char *)&VarDB_Hdr2, sizeof(struct vardb_hdr), 1, fp2);

  VarDB_Hdr1.Version = VarDB_Hdr2.Version;

  slaveCount = ntohl(VarDB_Hdr1.nRecords);
  masterCount = ntohl(VarDB_Hdr2.nRecords);
  fread((char *)VarDB1, sizeof(struct var_v2), slaveCount, fp1);

  for (i = 0; i < masterCount; ++i)
  {
    fread((char *)&VarDB2, sizeof(struct var_v2), 1, fp2);

    for (j = 0; j < slaveCount; ++j)
      if (strcmp(VarDB1[j].Name, VarDB2.Name) == 0)
      {
        VarDB1[j].standard_name = VarDB2.standard_name;
        VarDB1[j].reference = VarDB2.reference;
      }
  }

  rewind(fp1);
  fwrite((char *)&VarDB_Hdr1, sizeof(struct vardb_hdr), 1, fp1);
  fwrite((char *)VarDB1, sizeof(struct var_v2), slaveCount, fp1);

  fclose(fp1);
  fclose(fp2);

  return(0);

}	/* END MAIN */

/* END VDBMERGE.C */
