/* PmsVme2dTable.h
   Class for managing the vme interface pms 2d sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMSVME2DTABLE_H
#define PMSVME2DTABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class PmsVme2dTable {
public:
  PmsVme2dTable ();			// constructor
  void enter (TapeHeader&);     // make a table entry
  int nextEntry();      // select the next probe entry
  void displayTable();          // display the table contents
 
  inline int firstEntry()   {return ((index = 0) < count);}
  inline int interface()    {return intfc[index];}
  inline int channel()      {return chan[index];}
  inline char* name()   {return p_name[index];}
  inline int type()     {return p_type[index];}
  inline int resolution()   {return res[index];}

private:
  str12 p_name [PMS2D_NUM_PROBES];		// probe names
  int intfc[PMS2D_NUM_PROBES];           // interface numbers
  int chan[PMS2D_NUM_PROBES];            // channel numbers
  int p_type[PMS2D_NUM_PROBES];          // probe types
  int res[PMS2D_NUM_PROBES];             // probe resolutions
  int index;				// current entry index
  int count;				// number of entries
};

#endif
