/* DsmHeaders.cc
   Creates and manages the dsm sampling headers. 

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmHeaders.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DsmHeaders::DsmHeaders (int create) : DsmConfig ()

// Constructor.
{
  int stat;

// Create the main header class, and read the main header file.
  main_header = new TapeHeader;
  if (main_header->readFile (mainHeaderName()) == ERROR) 
    exit (ERROR);

//Create a header for each entry in the config file except winds.
  for (stat = (int)firstHeader(); stat; stat = (int)nextHeader()) 
    dsm_header[index()] = new TapeHeader;

// If create is true, create the dsm headers from the main headers, 
// otherwise read the existing the existing header files.
  if (create) 
    createHeaders();
  else 
    readHeaders();

  firstHeader();			// select the first header
}
/*****************************************************************************/

int DsmHeaders::firstHeader ()
 
// Selects the first dsm header.
{
  int stat;
  
// No header for the winds location.
  for (stat = firstDsm(); stat && !strcmp (location(), WINDS_LOCN_STR);
       stat = nextDsm());

  if (stat) {
    cur_header = dsm_header[index()];
    return TRUE;
  }
  else 
    return FALSE;
}
/*****************************************************************************/
 
int DsmHeaders::nextHeader ()
 
//  Selects the next dsm header.
{
  int stat;
  
// No header for the winds location.
  for (stat = nextDsm(); stat && !strcmp (location(), WINDS_LOCN_STR);
       stat = nextDsm());
 
  if (stat) {
    cur_header = dsm_header[index()];
    return TRUE;
  }
  else 
    return FALSE;
}
/*****************************************************************************/
 
void DsmHeaders::selectHeader (char *locn)
 
// Selects a header by dsm location.
{
  int stat;
  
// No header for the winds location.
  if (!strcmp (locn, WINDS_LOCN_STR) || !selectByLocn (locn)) {
    fprintf (stderr, "DsmHeaders: No header exists for dsm location %s.\n",
             locn);
    exit (ERROR);
  }
  cur_header = dsm_header[index()];
}
/*****************************************************************************/
 
void DsmHeaders::readHeaders()
 
// Reads existing disk file headers.
{
  int stat;
 
// Read a header for each entry in the config file except for winds.
  for (stat = (int)firstHeader(); stat; stat = (int)nextHeader()) 
      cur_header->readFile (dsmHeaderName());
}
/******************************************************************************
** Private Functions
******************************************************************************/

void DsmHeaders::createHeaders()

// Parses the main header, and creates the individual dsm headers.

{
  int stat;
  Blk hdr_des;			// local copy of the HDR block descriptor
  Fl fl;

// Get a copy of the main header flight descriptor.
  mainHeader()->firstDesc();
  fl = *(Fl*)(mainHeader()->getDesc());

// Get a copy of the main header HDR descriptor.
  mainHeader()->nextDesc();
  hdr_des = *(Blk*)(mainHeader()->getDesc());	

// Enter the Flight and HDR descriptors into each header.
  for (stat = (int)firstHeader(); stat; stat = (int)nextHeader())
    {
    curHeader()->enterDesc((char*)&fl, ntohl(fl.item_len));
    strcpy(hdr_des.dsm_locn, location());  	// set dsm_locn
    curHeader()->enterDesc((char*)&hdr_des, ntohl(hdr_des.item_len));
    }

// Parse the main header, and build the individual dsm headers.
  while (mainHeader()->nextDesc())
    {
    if (mainHeader()->shType() && strcmp(mainHeader()->name(), "DUMMY") == 0)
      continue;

    selectHeader(mainHeader()->dsm_locn());
    curHeader()->enterDesc(mainHeader()->getDesc(), 
                          mainHeader()->item_len());
    }

// Calculate the start and offset fields, and write the headers to disk.
  for (stat = (int)firstHeader(); stat; stat = (int)nextHeader())
    {
    curHeader()->calcDescFields();
    curHeader()->writeFile(dsmHeaderName()); 
    fprintf(stderr, "Header %s created.\n", dsmHeaderName());
    } 

}
