/*    XMAC2 = mach number squared calculation function              XMAC2 */

#include "nimbus.h"
#include "amlib.h"

NR_TYPE xmac2(NR_TYPE a)
{
  return(5.0 * (pow((double)(a+1.0), (double)0.28571) - 1.0));
}
