/*
-------------------------------------------------------------------------
OBJECT NAME:	plain.c

FULL NAME:	Plain ASCII files.

ENTRY POINTS:	handle_error()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-07
-------------------------------------------------------------------------
*/

#include "define.h"

void handle_error(int status)
{
    fprintf(stderr, "\n*****\n");
    fprintf(stderr, "Caught error %d\n",status);
    fprintf(stderr, "See http://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/NetCDF_002d3-Error-Codes.html for explanation\n");
    fprintf(stderr, "*****\n");
    exit(1);
}
