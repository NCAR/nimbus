/*
-------------------------------------------------------------------------
OBJECT NAME:	hdr_api.h

DESCRIPTION:	Header file tape API package

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#ifndef HDR_API_H
#define HDR_API_H

#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char	bool;

#define MAX_VARS	512
#define TAPE_BLK_SIZE	32767


struct var_list
	{
	void	*ptr;		/* Pointer to Structure		*/
	int	type;		/* Structure Type		*/
	};

#include "ctape.h"

extern struct var_list	HAPI_var[];
extern char		*HAPI_var_name[];
extern bool		HAPI_HeaderInitialized;
extern int		HAPI_name_ptr;
extern float		HAPI_version;

int	HAPI_lookupvar(char vn[], long valid_vars);

#endif

/* END HDR_API.H */
