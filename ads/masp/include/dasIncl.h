/* dasIncl.h -- This file holds all of the required #inlcude statements 
                for the DAS

   Original Author:  Keith S. Barr
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      07/20/1994 ksb Created
*/

#ifndef DASINCL_H
#define DASINCL_H

/* VxWorks and Motoroal MVME 162-13 headers */
#include <mv162.h>
#include <vxWorks.h>
#include <sockLib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <hostLib.h>
#include <msgQLib.h>
#include <wdLib.h>
#include <dosFsLib.h>
#include <iv.h>
#include <types.h>
#include <socket.h>
#include <in.h>
#include <string.h>
#include <stdioLib.h>
#include <taskLib.h>
#include <intLib.h>
#include <tyLib.h>
#include <ioLib.h>
#include <ctype.h>
#include <rebootLib.h>
#include <semLib.h>
#include <logLib.h>


/* home-grown header files */
#include <vmema.h>	/* VMEbus register and interrupt setup	*/
#include <packet.h>	/* Data structures			*/
#include <maspDas.h>	/* internal DAS data structures		*/
#include <masp.h>	/* top level setup for MASP		*/
#include <telSer.h>	/* MVIP-301 header file			*/

#endif
