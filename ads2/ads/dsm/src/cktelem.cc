#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <taskLib.h>
 
// Program include files.
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <header.h>

#include <IsioPort.h>
#include <natsDefs.h>

void statusMsg (char *msg);

IsioPort *isio;

int cktelem()
{
  int stat;

  isio = new IsioPort((char*)(A24D16_BASE + ISIO1_BASE),NATS_PORT,
                NATS_BAUD,NATS_DATA_BITS,NATS_PARITY,
                NATS_STOP_BITS,NATS_HW_HANDSHAKE, statusMsg);

  for(;;)
    {
      stat=isio->putcnt("SENDING NATS BLOCK\n",strlen("SENDING NATS BLOCK"));
      taskDelay(sysClkRateGet()/2);
    }
}
/*****************************************************************************/

void statusMsg (const char *msg)
{
  fprintf (stderr, msg);
}
