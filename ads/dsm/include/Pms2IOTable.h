/* Pms2IOTable.h
   Class for managing the vme interface pms 1d sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS2IOTABLE_H
#define PMS2IOTABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class Pms2IOTable {
public:
  Pms2IOTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextEntry();			// select the next probe entry
  void displayTable();			// display the table contents
  int firstEntry() 	{return ((index = 0) < count);}
  char* name() 		{return dev_name[index];}
 
private:
  str12 dev_name [MAX_PMS2];		// probe names
  int index;				// current entry index
  int count;				// number of entries
};

#endif
