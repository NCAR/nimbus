/* DsmTables.cc
   Creates and manages the dsm data pickoff tables.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmTables.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DsmTables::DsmTables (DsmHeaders &headers) : DsmConfig ()

// Constructor.
{
  PickTable *table;			// current table pointer
  str8 dsm_locn;			// current dsm location
  str8 type;				// current sdi sample type
  int source = 0;			// current sdi sample source index
  int dest = 0;				// current sdi sample destination index
  int length = 0;			// current sdi sample length
  int src_off = 0;			// current sdi source sample offset
  int dest_off = 0;			// current sdi destination sample offset
  int rate = 0;				// current sdi sample rate
  int last_sh = FALSE;			// last descriptor was an Sh type flag
  int stat;

  strcpy (dsm_locn, "");		// null current location

// Reset each of the dsm headers to the first descriptor.
// Create a pickoff table class for each dsm header.
  for (stat=(int)headers.firstHeader(); stat; stat=(int)headers.nextHeader()) { 
    headers.curHeader()->firstDesc();
    headers.curHeader()->nextDesc();		// skip the HDR block
    selectTable (headers.curHeader()->dsm_locn());
    dsm_table[index()] = new PickTable (location());
  }

// Make an entry for the HDR descriptor from the main header.
  headers.mainHeader()->firstDesc();
  headers.mainHeader()->nextDesc();
  table = selectTable (headers.mainHeader()->dsm_locn());
  table->putEntry (headers.mainHeader()->item_type(), 
                   headers.mainHeader()->start(), 
                   headers.mainHeader()->start(), 
                   headers.mainHeader()->length(), 
                   0, 0, 1);

// Parse the remainder of the headers, and build the individual pickoff tables.
  while (headers.mainHeader()->nextDesc ()) {

    if (headers.mainHeader()->shType() &&
        strcmp(headers.mainHeader()->name(), "DUMMY") == 0)
      continue;

// Select the dsm header for the current descriptor.
    headers.selectHeader (headers.mainHeader()->dsm_locn());
    headers.curHeader()->nextDesc();		// select next descriptor

// Compare descriptor types.
    if (strcmp (headers.mainHeader()->item_type(),
                headers.curHeader()->item_type()) ||
        strcmp (headers.mainHeader()->dsm_locn(),
                headers.curHeader()->dsm_locn())) {
      fprintf (stderr, 
        "DsmTables: Descriptor type mismatch.\n"); 
      fprintf (stderr, 
        "mainHeader->itemType = %s, curHeader->itemType = %s.\n",
        headers.mainHeader()->item_type(), headers.curHeader()->item_type()); 
      fprintf (stderr, 
        "mainHeader->dsm_locn = %s, curHeader->dsm_locn = %s.\n",
         headers.mainHeader()->dsm_locn(), headers.curHeader()->dsm_locn());
      exit (ERROR);
    }

// Make table entries.  Sdi variables are grouped into blocks by sample rate
// and dsm location.  A single table entry is made for each sdi block.
    if (headers.mainHeader()->shType()) {

// Check for a new sample rate or a new dsm location.  This marks the start
// of a new block.
      if ((headers.mainHeader()->rate() != rate) || 
          strcmp (headers.mainHeader()->dsm_locn(), dsm_locn)) {

// If there is a previous block, then make an entry for it. 
        if (last_sh)  {
          table = selectTable (dsm_locn);
          table->putEntry (type, source, dest, length, src_off,
                                        dest_off, rate);
        }
        strcpy (dsm_locn, headers.mainHeader()->dsm_locn());
        strcpy (type, headers.mainHeader()->item_type());
        source = headers.curHeader()->start();
        dest = headers.mainHeader()->start();
        src_off = headers.curHeader()->offset();
        dest_off = headers.mainHeader()->offset();
        rate = headers.mainHeader()->rate();
        length = headers.mainHeader()->length();
      }
      else
        length += headers.mainHeader()->length();
      last_sh = TRUE;
    }

// Else not an Sh type.
    else {

// If the last descriptor was an Sh type, make an entry for the last Sh block.
      if (last_sh) {
        table = selectTable (dsm_locn);
        table->putEntry (type, source, dest, length, src_off,
                                        dest_off, rate);
        last_sh = FALSE;
      }

// Need to modify item_type for events.  CJW 11/23/95
      if (headers.mainHeader()->evtType()) {
        char  tmpName[16];

        strcpy(tmpName, "EVT");
        strcat(tmpName, (const char *)headers.mainHeader()->locn());
        table = selectTable (headers.mainHeader()->dsm_locn());
        table->putEntry (tmpName,
                         headers.curHeader()->start(),
                         headers.mainHeader()->start(), 
                         headers.mainHeader()->length(), 
                         0, 0, 1);
        }


// Make an entry for all non-Sh and non asynchronous descriptors.
      if (!headers.mainHeader()->pms2Type() && !headers.mainHeader()->evtType()
          && !headers.mainHeader()->asyType()) {
        table = selectTable (headers.mainHeader()->dsm_locn());
        table->putEntry (headers.mainHeader()->item_type(), 
                         headers.curHeader()->start(),
                         headers.mainHeader()->start(), 
                         headers.mainHeader()->length(), 
                         0, 0, 1);

// If this is a Pms1v2 descriptor, and particle spacing is enabled, make
// an entry for the particle spacing block.
        if (headers.mainHeader()->pms1v2Type() && 
            headers.mainHeader()->ps_length()) {
          table->putEntry (headers.mainHeader()->item_type(), 
                         headers.curHeader()->ps_start(),
                         headers.mainHeader()->ps_start(), 
                         headers.mainHeader()->ps_length(), 
                         0, 0, 1);
        }
      }
    }
  }

  for (table = firstTable(); (int)table; table = nextTable())
    table->displayTable();
}
/*****************************************************************************/

PickTable* DsmTables::firstTable ()
 
// Selects the first pickoff table.
{
  int stat;

// No table for the winds location.
  for (stat = firstDsm(); stat && !strcmp (location(), WINDS_LOCN_STR);
       stat = nextDsm());
 
  if (stat)
    return dsm_table[index()];
  else
    return (PickTable*)0;
}
/*****************************************************************************/
 
PickTable* DsmTables::nextTable ()
 
// Selects the next pickoff table.
{
  int stat;
 
// No table for the winds location.
  for (stat = nextDsm(); stat && !strcmp (location(), WINDS_LOCN_STR);
       stat = nextDsm());
 
  if (stat)
    return dsm_table[index()];
  else
    return (PickTable*)0;
}
/*****************************************************************************/
 
PickTable* DsmTables::selectTable (char *locn)
 
// Selects a pickoff table by dsm location.
{
  int stat;
 
// No header for the winds location.
  if (!strcmp (locn, WINDS_LOCN_STR) || !selectByLocn (locn)) {
    fprintf (stderr, "DsmTables: No table exists for dsm location %s.\n",
             locn);
    exit (ERROR);
  }
  return dsm_table[index()];
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/
