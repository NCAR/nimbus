/*
-------------------------------------------------------------------------
OBJECT NAME:	prio_rt.c

FULL NAME:	

ENTRY POINTS:	main

STATIC FNS:	none

DESCRIPTION:	Puts parent process into the Real-Time scheduling class.

NOTES:		This program must be set-uid root.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <stdio.h>
#include <sys/priocntl.h>
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>

/* -------------------------------------------------------------------- */
main()
{
  pcinfo_t	pcinfo;
  pcparms_t	parms;
  rtparms_t	*rtparms;

  /* Figure out what the class ID is for the real time class.  This is Sun's
   * way of turning a normal integer constant into a character constant
   * instead...
   */
  strcpy (pcinfo.pc_clname, "RT");
  if (priocntl(0, 0, PC_GETCID, (caddr_t)&pcinfo) == -1)
    {
    fprintf(stderr, "GETCID failed on RT class, en %d\n", errno);
    return;
    }

  /* Put parent process into RT mode.
   * process.
   */
  parms.pc_cid = pcinfo.pc_cid;
  rtparms = (rtparms_t *)&parms.pc_clparms;
  rtparms->rt_pri = 0;
  rtparms->rt_tqnsecs = RT_TQDEF;

  if (priocntl(P_PID, getppid(), PC_SETPARMS, (caddr_t)&parms) == -1)
    fprintf(stderr, "SETPARMS failed, errno %d\n", errno);

}  /* END MAIN */

/* END PRIO_RT.C */
