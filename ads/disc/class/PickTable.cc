/* PickTable.cc
   Class definition for the data pickoff table class.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <PickTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/

PickTable::PickTable (char *name)
// Initializes the table.
{
  strcpy (table_name , name);
  table_size = 0;
  table_idx = 0; 
  table_cnt = 0;
 
// acquire memory for the table.
  grow();
}
/*****************************************************************************/

int PickTable::putEntry (char *name, int source, int dest, int len, 
                         int src_off, int dest_off, int rate)
                         
// Enters information into the table.
{
  PickEntry *entry;                            	// temp pointer

  if (table_cnt >= table_size) 
    grow();

  entry = table + table_cnt;
  strcpy (entry->name, name);                     // enter information
  entry->source = source;
  entry->dest = dest;
  entry->len = len;
  entry->src_off = src_off;
  entry->dest_off = dest_off;
  entry->rate = rate;
  table_cnt++;
  return OK;                     // increment table index
}   
/*****************************************************************************/
 
PickEntry* PickTable::firstEntry()

// Sets the table index to 0
{
  if (!table_cnt)
    return (PickEntry*)NULL;			// no table entries

  table_idx = 0;
  return table;
}
/*****************************************************************************/
 
PickEntry* PickTable::nextEntry()

// Returns the next table entry
{
  if (table_idx < (table_cnt - 1)) {
    table_idx++;
    return table + table_idx;
  }
  return (PickEntry*)NULL;			// end of table reached
}
/*****************************************************************************/
 
void PickTable::displayTable()

// Display the contents of the table.
{
  PickEntry *entry;
  
  printf ("\n%s Pickoff Table\n", table_name);
  printf ("name     source  dest    length  src_off dest_off rate\n");
  printf ("-------- ------  ----    ------  ------- -------- ----\n");

  for (entry = firstEntry(); (int)entry; entry = nextEntry()) {
    printf ("%8s %4d    %4d    %4d    %4d    %4d     %4d\n", 
            entry->name, entry->source, entry->dest, entry->len, 
            entry->src_off, entry->dest_off, entry->rate);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/

void PickTable::grow ()

// Increase the table size by PICKTABLE_SIZE entries.
{
  PickEntry *new_table; 
  int new_size;

// Allocate memory for the new table size.
  new_size = table_size + PICKTABLE_SIZE;
  if (!(int)(new_table = (PickEntry*)malloc (new_size * sizeof(PickEntry)))) {
    fprintf (stderr, "PickTable::grow: malloc failed.\n");
    exit (ERROR);
  }
            
// Copy the existing table into the new table.
  if (table_size) {
    memcpy (new_table, table, table_size * sizeof(PickEntry)); 
    free ((char*)table);
  }
  table = new_table;
  table_size = new_size;
  return;
}
/*****************************************************************************/

