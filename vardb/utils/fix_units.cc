/*
-------------------------------------------------------------------------
OBJECT NAME:	fix_units.c

FULL NAME:	VarDB dump

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	Mass replacement of units with units that conform to Unidata
		UD Units project.  This doesn't do lookups in UDunits, you
		need to hardcode the changes into this program.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005

COMPILE:	g++ -I/jnet/local/include fix_units.cc -L/jnet/local/lib -lVarDB -lraf -o fix_units

-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  if (argc < 2)
    {
    fprintf(stderr, "Usage: vdbdump [-a] [proj_num | VarDB_filename]\n");
    return(1);
    }

  if (InitializeVarDB(argv[1]) == ERR)
    {
    fprintf(stderr, "vdbdump: Initialize failure.\n");
    return(1);
    }


  printf("Version %d, with %d records.\n", ntohl(VarDB_Hdr.Version),
						VarDB_nRecords);

  VarDB_SetUnits("CLAT", "degree_N");
  VarDB_SetMinLimit("CLAT", -90.0);
  VarDB_SetMaxLimit("CLAT", 90.0);

  VarDB_SetUnits("GLAT", "degree_N");
  VarDB_SetMinLimit("GLAT", -90.0);
  VarDB_SetMaxLimit("GLAT", 90.0);

  VarDB_SetUnits("GGLAT", "degree_N");
  VarDB_SetMinLimit("GGLAT", -90.0);
  VarDB_SetMaxLimit("GGLAT", 90.0);

  VarDB_SetUnits("LAT", "degree_N");
  VarDB_SetMinLimit("LAT", -90.0);
  VarDB_SetMaxLimit("LAT", 90.0);

  VarDB_SetUnits("DSLAT", "degree_N");
  VarDB_SetMinLimit("DSLAT", -90.0);
  VarDB_SetMaxLimit("DSLAT", 90.0);

  VarDB_SetUnits("LATC", "degree_N");
  VarDB_SetMinLimit("LATC", -90.0);
  VarDB_SetMaxLimit("LATC", 90.0);

  VarDB_SetUnits("XLATC", "degree_N");
  VarDB_SetMinLimit("XLATC", -90.0);
  VarDB_SetMaxLimit("XLATC", 90.0);


  VarDB_SetUnits("CLON", "degree_E");
  VarDB_SetMinLimit("CLON", -180.0);
  VarDB_SetMaxLimit("CLON", 180.0);

  VarDB_SetUnits("GLON", "degree_E");
  VarDB_SetMinLimit("GLON", -180.0);
  VarDB_SetMaxLimit("GLON", 180.0);

  VarDB_SetUnits("GGLON", "degree_E");
  VarDB_SetMinLimit("GGLON", -180.0);
  VarDB_SetMaxLimit("GGLON", 180.0);

  VarDB_SetUnits("LON", "degree_E");
  VarDB_SetMinLimit("LON", -180.0);
  VarDB_SetMaxLimit("LON", 180.0);

  VarDB_SetUnits("DSLON", "degree_E");
  VarDB_SetMinLimit("DSLON", -180.0);
  VarDB_SetMaxLimit("DSLON", 180.0);

  VarDB_SetUnits("LONC", "degree_E");
  VarDB_SetMinLimit("LONC", -180.0);
  VarDB_SetMaxLimit("LONC", 180.0);

  VarDB_SetUnits("XLONC", "degree_E");
  VarDB_SetMinLimit("XLONC", -180.0);
  VarDB_SetMaxLimit("XLONC", 180.0);

  VarDB_SetUnits("WD", "degree_T");
  VarDB_SetMinLimit("WD", 0.0);
  VarDB_SetMaxLimit("WD", 360.0);
  VarDB_SetUnits("WDC", "degree_T");
  VarDB_SetMinLimit("WDC", 0.0);
  VarDB_SetMaxLimit("WDC", 360.0);
  VarDB_SetUnits("CTHDG", "degree_T");
  VarDB_SetMinLimit("CTHDG", 0.0);
  VarDB_SetMaxLimit("CTHDG", 360.0);
  VarDB_SetUnits("TKAT", "degree_T");
  VarDB_SetMinLimit("TKAT", 0.0);
  VarDB_SetMaxLimit("TKAT", 360.0);
  VarDB_SetUnits("THDG", "degree_T");
  VarDB_SetMinLimit("THDG", 0.0);
  VarDB_SetMaxLimit("THDG", 360.0);
  VarDB_SetUnits("GGTRK", "degree_T");
  VarDB_SetMinLimit("GGTRK", 0.0);
  VarDB_SetMaxLimit("GGTRK", 360.0);
  VarDB_SetUnits("DSWD", "degree_T");
  VarDB_SetMinLimit("DSWD", 0.0);
  VarDB_SetMaxLimit("DSWD", 360.0);

  VarDB_SetMinLimit("DRFTA", -90.0);
  VarDB_SetMaxLimit("DRFTA", 90.0);
  VarDB_SetMinLimit("CROLL", -180.0);
  VarDB_SetMaxLimit("CROLL", 180.0);
  VarDB_SetMinLimit("ROLL", -180.0);
  VarDB_SetMaxLimit("ROLL", 180.0);
  VarDB_SetMinLimit("CPITCH", -180.0);
  VarDB_SetMaxLimit("CPITCH", 180.0);
  VarDB_SetMinLimit("PITCH", -180.0);
  VarDB_SetMaxLimit("PITCH", 180.0);

  VarDB_SetUnits("EV1", "none");

  for (int i = 0; i < VarDB_nRecords; ++i)
  {
    char	*vu, *p;

    vu = ((struct var_v2 *)VarDB)[i].Units;

    /* Clean up metres first.
     */
    if (strcmp(vu, "M") == 0)
      strcpy(vu, "m");

    if ((p = strstr(vu, "uM")) || (p = strstr(vu, "kM")))
      *(p+1) = 'm';

    if ((p = strstr(vu, "M/")) )
      *p = 'm';

    if ((p = strstr(vu, "M2")) )
      *p = 'm';

    if ((p = strstr(vu, "M3")) )
      *p = 'm';

    if (strcmp(vu, "kV/M") == 0)
      strcpy(vu, "kV/m");

    if (strcmp(vu, "kVdc") == 0)
      strcpy(vu, "kV");


    if (strcmp(vu, "Vdc") == 0 || strcmp(vu, "Volts") == 0)
      strcpy(vu, "V");

    if (strcmp(vu, "C") == 0)
      strcpy(vu, "deg_C");

    if (strcmp(vu, "G") == 0)
      strcpy(vu, "g");

    if (strcmp(vu, "hours") == 0)
      strcpy(vu, "hour");

    if (strcmp(vu, "radians") == 0)
      strcpy(vu, "radian");

    if (strcmp(vu, "minutes") == 0)
      strcpy(vu, "minute");

    if (strcmp(vu, "knots") == 0)
      strcpy(vu, "knot");

    if (strcmp(vu, "deg") == 0)
      strcpy(vu, "degree");

    if (strncmp(vu, "micron", 6) == 0)
      strcpy(vu, "um");

    if (strcmp(vu, "deg/S") == 0)
      strcpy(vu, "deg/s");

    if (strcmp(vu, "g/m3") == 0)
      strcpy(vu, "gram/m3");

    if (strcmp(vu, "g/kg") == 0)
      strcpy(vu, "gram/kg");


    if (strcmp(vu, "Lpm") == 0)
      strcpy(vu, "L/min");

    if (strcmp(vu, "cc/m3") == 0)
      strcpy(vu, "cm3/m3");

    if (strcmp(vu, "cnts") == 0 ||
	strcmp(vu, "cnt") == 0 ||
	strcmp(vu, "counts") == 0)
      strcpy(vu, "count");

    if ( (p = strstr(vu, "N/")) )
      *p = '#';


    printf("%-16s %-16s %-60s\n", ((struct var_v2 *)VarDB)[i].Name,
	((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);
/*
    ulong     p0, p1;
    float     *rc0 = (float *)&p0, *rc1 = (float *)&p1;

    p0 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].MinLimit)));
    p1 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].MaxLimit)));
    printf(" %6.1f %6.1f\n", *rc0, *rc1);
*/
  }


  SaveVarDB(argv[1]);

  ReleaseVarDB();

  return(0);

}	/* END MAIN */

/* END FIX_UNITS.CC */
