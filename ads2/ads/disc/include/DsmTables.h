/* DsmTables.h
   Creates and manages the dsm data pickoff tables.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMTABLES_H
#define DSMTABLES_H

#include <dsmctl.h>

#include <DsmHeaders.h>				// dsm tape headers class
#include <PickTable.h>				// pickoff table class
#include <TapeHeader.h>				// tape header class

class DsmTables : private DsmConfig {
public:
  DsmTables(DsmHeaders &headers);		// constructor
  PickTable *firstTable();			// select first table
  PickTable *nextTable();			// select next table
  PickTable *selectTable(char *locn);		// select table by dsm location

private:
  PickTable* dsm_table[MAX_DSM];		// pickoff tables
};

#endif
