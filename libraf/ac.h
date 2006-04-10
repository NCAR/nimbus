/*
-------------------------------------------------------------------------
OBJECT NAME:	ac.h

FULL NAME:	Aircraft header file.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#ifndef AC_H
#define	AC_H

#ifdef __cplusplus
extern "C" {
#endif
void	InitAircraftSpecs(const char aircraft[]), ReleaseAircraftSpecs(void);
char	*GetAircraftParameter(const char tailNumber[], char parameter[]);
int	GetAircraftList(char *list[]);
#ifdef __cplusplus
}
#endif

#endif
