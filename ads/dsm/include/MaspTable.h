/* MaspTable.h
   Class for managing the Masp sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef MASPTABLE_H
#define MASPTABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class MaspTable {
public:
  MaspTable ();			// constructor
  void enter (TapeHeader&);     // make a table entry
  int nextEntry();      // select the next probe entry
  void displayTable();          // display the table contents
 
  inline int firstEntry()   {return ((index = 0) < count);}
  int start()           {return masp_start;}
  int length()          {return masp_length;}

private:
  int index;				// current entry index
  int count;				// number of entries
  int masp_start;
  int masp_length;
};

#endif
