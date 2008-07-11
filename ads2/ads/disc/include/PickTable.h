/* PickTable.h
   Class definition for the data pickoff table class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PICKTABLE_H
#define PICKTABLE_H

#define PICKTABLE_SIZE 16			// default max number of entries

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <dsmctl.h>

typedef struct {
  str8 name;                                  	// block name
  int source;                                 	// source block index
  int dest;                                   	// destination block index
  int len;                                    	// number of bytes in the block
  int src_off;                                	// sample source offset
  int dest_off;                               	// sample destination offset
  int rate;                                   	// sample rate
} PickEntry;

class PickTable {
public:
  PickTable (char *name);			// constructor
  int putEntry (char* name, int source, int dest, int len, int src_off, 
                int dest_off, int rate);	// write to table
  PickEntry *firstEntry();                  	// get first entry
  PickEntry *nextEntry();                	// get next entry
  void displayTable();

private:
  void grow();				// increase the table size

  char table_name[20];
  int table_size;			// current table size
  int table_idx;			// current index
  int table_cnt;			// current number of entries
  PickEntry *table;
};

#endif
