/* GreyVmeTable.h
   Class for managing the grey scale interface sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef GREYVMETABLE_H
#define GREYVMETABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class GreyVmeTable {
public:
  GreyVmeTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextEntry();			// select the next probe entry
  int selectByName (char *pname);	// select entry by probe name
  void displayTable();			// display the table contents

  inline int firstEntry() 	{return ((index = 0) < count);}
  inline int interface()	{return intfc[index];}
  inline int channel()		{return chan[index];}
  inline char* name() 		{return p_name[index];}
  inline int type() 		{return p_type[index];}
  inline int resolution() 	{return res[index];}
  inline int chargeSpacing() 	{return charge_spc[index];}
  inline int chargeLocation() 	{return charge_locn[index];}
 
private:
  int intfc[GREY_NUM_PROBES];		// interface numbers
  int chan[GREY_NUM_PROBES];		// channel numbers
  str12 p_name[GREY_NUM_PROBES];	// probe names
  int p_type[GREY_NUM_PROBES];		// probe types
  int res[GREY_NUM_PROBES];		// probe resolutions
  int charge_spc[GREY_NUM_PROBES];	// charge sample spacing
  int charge_locn[GREY_NUM_PROBES];	// charge ring location 
  int index;				// current entry index
  int count;				// number of entries
};

#endif
