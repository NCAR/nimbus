/* NatsData.cc
   Generates NATS pickoff records, and expands pickoff records.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research

   Revisions:


*/

#include <NatsData.h>

/******************************************************************************
** Public Functions
******************************************************************************/

NatsData::NatsData (TapeHeader &dsm_hdr) : PickTable("NATS")
                        
// Constructor.
{
  int rate = 0;                         // current sdi sample rate
  int stat;

// Create the record buffer.
  lbuf = new RandomBuf (dsm_hdr.lrlen());	// record buffer

  dsm_hdr.firstDesc();			// skip the flight descriptor
  pick_len = 0;
  smp_intv = 0;

// Parse the header, and build the pickoff table.
  for (stat = dsm_hdr.nextDesc(); stat; stat = dsm_hdr.nextDesc ()) {
 
// Sdi variables are grouped into blocks by sample rate. A single table entry 
// is made for each sdi block.
    if (dsm_hdr.shType()) {
 
// Check for a new sample rate.  This marks the start of a new block.
      if (dsm_hdr.rate() != rate) {
        putEntry (dsm_hdr.item_type(), 
                  dsm_hdr.start(),	// full record start 
                  pick_len, 		// pickoff record start
                  dsm_hdr.offset(), 	// entry length
                  dsm_hdr.offset(), 	// full record offset
                  0,			// pickoff record offset
                  dsm_hdr.rate());	// sample rate

// The offset between sdi samples is the same as the length of one set of 
// sdi samples.
        pick_len += dsm_hdr.offset();	// increment destination index
        rate = dsm_hdr.rate();
      }
    }
 
// Make an entry for all non-Sh and non asynchronous descriptors.
    else if (!dsm_hdr.pms2Type() && !dsm_hdr.asyType()) {
      putEntry (dsm_hdr.item_type(),
                dsm_hdr.start(),	// full record start
                pick_len,		// pickoff record start
                dsm_hdr.sampleLength(),	// entry length
                dsm_hdr.sampleLength(), // full record offset
                0, 			// pickoff record offset
                dsm_hdr.rate());	// sample rate

      pick_len += dsm_hdr.sampleLength();	// increment destination index
    }
  }
  displayTable();
}
/*****************************************************************************/

void NatsData::pickoff (char *buf)

// Builds the pickoff record from the passed in 1 second sync record.
{
  PickEntry *entry;

  for (entry = firstEntry(); (int)entry; entry = nextEntry())
       lbuf->putBuf (buf + entry->source, entry->dest, entry->len);
}
/*****************************************************************************/

void NatsData::expand (char *buf)

// Builds the expanded 1 sec sync record from the passed in pickoff record.
{
  PickEntry *entry;
  int sidx;				// expanded record index
  int rate_cnt;

// Build the expanded record by duplicating the single samples in the pickoff
// record sample rate times.
  for (entry = firstEntry(); (int)entry; entry = nextEntry()) {
    for (rate_cnt = 0, sidx = entry->source; rate_cnt < entry->rate; 
         rate_cnt++, sidx += entry->len)
      lbuf->putBuf (buf + entry->dest, sidx, entry->len);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
