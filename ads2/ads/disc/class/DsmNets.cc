/* DsmNets.cc
   Creates and manages the DsmNet network communication classes.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmNets.h>

/* -------------------------------------------------------------------- */
DsmNets::DsmNets (void (*netMsg)(int action, char* name, char* msg_str)) :
         DsmConfig ()
{
  int stat;

  // Determine if operating in the NATS mode.
  selectByLocn(CTL_LOCN_STR);
  nats_mode = nats();

  // Create a DsmNet for each config file entry except for ctl.  Don't create
  // DsmNets for dsm's if operting the the NATS mode.
  for (stat = (int)firstDsm(); stat; stat = (int)nextDsm())
    {
    if ((isDsmLocation() && !nats_mode) || isWindsLocation())
      {
      dsm_net[index()] = new DsmNet(TCP_ACCEPT, MX_PHYS, port(), hostName(), 
                                     location(), netMsg);
      printf("DsmNet for host %s created.\n", hostName());
      }
    }

  // Set the winds_net pointer.
  winds_net = selectNet(WINDS_LOCN_STR);

#ifdef SOCK_WINDS
  winds_net->setSendBufSize(32767);
#endif

  firstNet();
}

/* -------------------------------------------------------------------- */
DsmNet* DsmNets::firstNet()
 
// Selects the first pickoff net.
{
  int stat;
 
  // No net for the ctl location.
  for (stat = (int)firstDsm(); stat; stat = nextDsm())
    if ((isDsmLocation() && !nats_mode) || isWindsLocation())
      return(cur_net = dsm_net[index()]);

  return((DsmNet*)0);

}

/* -------------------------------------------------------------------- */
DsmNet* DsmNets::nextNet()
 
// Selects the next pickoff net.
{
  int stat;
 
  // No net for the ctl location.
  for (stat = (int)nextDsm(); stat; stat = nextDsm())
    if ((isDsmLocation() && !nats_mode) || isWindsLocation())
      return(cur_net = dsm_net[index()]);

  return((DsmNet*)0);

}

/* -------------------------------------------------------------------- */
DsmNet* DsmNets::selectNet(char *locn)
 
// Selects a pickoff net by dsm location.
{
  int stat;
 
  // No header for the ctl location.
  if (!strcmp(locn, CTL_LOCN_STR) || !selectByLocn(locn))
    {
    fprintf(stderr, "DsmNets: No net exists for dsm location %s.\n", locn);
    exit(ERROR);
    }

  return(cur_net = dsm_net[index()]);

}

/* END DSMNETS.CC */
