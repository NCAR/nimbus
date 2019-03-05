/*
-------------------------------------------------------------------------
OBJECT NAME:    mtp.c

FULL NAME: MTP related stuff

ENTRY POINTS:	Len2Name()
		initMTPTimeDim()

DESCRIPTION:	Len2Name() converts NetCDF attributes of type Vector## where
		## is a length, to a descriptive name.
		initMTPTimeDim() created the MTP Time dimension with a record
		approx. once every 17 seconds.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "NetCDF.h"
#include <netcdf.h>

#define BUFF_SIZE  256

/* -------------------------------------------------------------------- */
/* Convert netCDF dimension name from an integer length to a descriptive
 * name. Changes dims from Vector## to names that describe physical
 * phenomenon.
 * Input:
 *     length - interger length of array
 * Output:
 *     name   - string to be used in place of integer
 */
char *Len2Name(size_t length)
{
    static char name[32];
    switch(length) {
        case 2: sprintf(name,"MTPTarget");
                break;
        case 3: sprintf(name,"MTPChannel");
                break;
        case 6: sprintf(name,"CAL_MTP");
                break;
        case 10:sprintf(name,"BTSCANANGLE_MTP");
                break;
        case 30:sprintf(name,"TPL_MTP");
                break;
        case 33:sprintf(name,"ALT_MTP");
                break;
        default:sprintf(name,"Vector%zu",length);
    }
    return (name);
}	/* END Len2Name */

/* -------------------------------------------------------------------- */
/* Create the MTP time dimension. Since the data report once every 17 seconds
 * rather than at 1hz.
 * Input:
 * 	TimeLength - length of the MTP time dimension
 * Input/Output:
 * 	MTPTimeDim - index of the time dimension
 * 	dims	   - array of dimensions used to define MTPTime variable
 */
void initMTPTimeDim(int _ncid, size_t TimeLength, int dims[], int *MTPTimeDim, int *MTPtimeVarID)
{
  int status;

  // Define the MTPTime dimension
  status = nc_def_dim(_ncid,"MTPTime", TimeLength, MTPTimeDim);
  if (status != NC_NOERR)
  {
    fprintf(stderr,"CreateNetCDF: error defining MTPTime dimension status = %d\n", status);
    fprintf(stderr, "%s\n", nc_strerror(status));
  }

  // Define the MTPTime variable MTPTime(MTPTime)
  dims[0]=*MTPTimeDim;
  status = nc_def_var(_ncid, "MTPTime", NC_LONG, 1, dims, MTPtimeVarID);
  if (status != NC_NOERR)
  {
    fprintf(stderr,"CreateNetCDF: error, raw variable MTPTime status = %d\n", status);
    fprintf(stderr, "%s\n", nc_strerror(status));
  }

  // Add attributes to the MTPTime variable
  strcpy(buffer, "time of MTP measurement");
  nc_put_att_text(_ncid, *MTPtimeVarID, "long_name", strlen(buffer)+1, buffer);
  strcpy(buffer, "time");
  nc_put_att_text(_ncid, *MTPtimeVarID, "standard_name", strlen(buffer)+1, buffer);
  //writeTimeUnits(*MTPtimeVarID,-1)
}	/* END initMTPTimeDim */
/* -------------------------------------------------------------------- */
/* Read in the limited set list of RCF files to process with.
 * Output:
 * 	filelist - limited set list of RCF files to process with
 */
void readRCFlist(std::vector<std::string> &filelist)
{

    std::string file = "%s/%s/%s/RCFlist."+cfg.FlightNumber();

    FILE *fp = OpenProjectFile(file, "r", RETURN);

    if (fp == 0)
    {
	return;
    }

    while (fgets(buffer, BUFF_SIZE, fp) != NULL)
    {
	buffer[strlen(buffer)-1]=0; //remove the carriage return
	filelist.push_back(buffer);
    }

    fclose(fp);

}	/* END readRCFlist */

/* MTP.C */
