/* DsmHeaders.h
   Creates and manages the dsm sampling headers. 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMHEADER_H
#define DSMHEADER_H

#include <dsmctl.h>

#include <DsmConfig.h>				// tape header class
#include <TapeHeader.h>				// tape header class

#include <sys/types.h>
#include <netinet/in.h>

class DsmHeaders : private DsmConfig {
public:
  DsmHeaders (int create);			// constructor
  TapeHeader *mainHeader()	{return main_header;}
  TapeHeader *curHeader() 	{return cur_header;}
  int firstHeader();				// select first header
  int nextHeader();				// select next header
  void selectHeader(char *locn);		// select header by dsm locn
  void readHeaders();				// read existing dsm headers

private:
  void createHeaders();				// create the dsm headers

  TapeHeader *main_header;			// main tape header class
  TapeHeader *cur_header;			// selected tape header class
  TapeHeader *dsm_header[MAX_DSM];		// dsm header classes
};

#endif
