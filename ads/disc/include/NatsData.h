/* NatsData.h
   Generates NATS pickoff records, and expands pickoff records.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef NATSDATA_H
#define NATSDATA_H

#include <header.h>
#include <natsDefs.h>

#include <PickTable.h>
#include <RandomBuf.h>
#include <TapeHeader.h>

class NatsData : private PickTable {
public:
  NatsData (TapeHeader &dsm_hdr);
              
  void pickoff (char *buf);
  void expand (char *buf);

  const char *const buffer()	{return lbuf->getBuf();}
  int pickLength()		{return pick_len;}
  void releaseBuf()      	{lbuf->releaseBuf();}
  int checkSmpIntv() {return !(smp_intv = ++smp_intv % NATS_SMP_INTVL);} 

private:
  RandomBuf *lbuf;		// local buffer
  int pick_len;			// length of the pickoff block
  int smp_intv;			// sampled data interval counter
};

#endif
