/*
-------------------------------------------------------------------------
OBJECT NAME:	getserial.c

FULL NAME:	Get values for Serial block

ENTRY POINTS:	GetBaudRate()
		GetParity()
		GetNumberDataBits()
		GetNumberStopBits()
		GetFlowControl()
		GetPortNumber()
		GetDataType()
		GetTerminationString()
		GetAlignmentString()

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	HAPI_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	SERIAL


/* -------------------------------------------------------------------- */
int GetBaudRate(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->baud_rate);

  return(OK);

}	/* END GETBAUDRATE */

/* -------------------------------------------------------------------- */
int GetPortNumber(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->port_num);

  return(OK);

}	/* END GETPORTNUMBER */

/* -------------------------------------------------------------------- */
int GetParity(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->parity);

  return(OK);

}	/* END GETPARITY */

/* -------------------------------------------------------------------- */
int GetFlowControl(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->flow_ctl);

  return(OK);

}	/* END GETFLOWCONTROL */

/* -------------------------------------------------------------------- */
int GetNumberDataBits(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->data_bits);

  return(OK);

}	/* END GETDATABITS */

/* -------------------------------------------------------------------- */
int GetNumberStopBits(char vn[], float *result)
{
  int	indx;
  long  value;
  float *p = (float *)&value;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  value = ntohl( *(int *)&(((Serial *)HAPI_var[indx].ptr)->stop_bits) );
  *result = *p;

  return(OK);

}	/* END GETNUMBERSTOPBITS */

/* -------------------------------------------------------------------- */
int GetDataType(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Serial *)HAPI_var[indx].ptr)->data_type);

  return(OK);

}	/* END GETDATATYPE */

/* -------------------------------------------------------------------- */
int GetTerminationString(char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Serial *)HAPI_var[indx].ptr)->term_str;

  return(OK);

}	/* END GETTERMINATIONSTRING */

/* -------------------------------------------------------------------- */
int GetAlignmentString(char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Serial *)HAPI_var[indx].ptr)->align_str;

  return(OK);

}	/* END GETALIGNMENTSTRING */

/* END GETSERIAL.C */
