/* TvmConfig.h
   Reads tvm display configuration information from the tvmconfig file.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
 
#ifndef TVMCONFIG_H
#define TVMCONFIG_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vxWorks.h>
#include <bootLib.h>

#include <header.h>
#include <tvmDefs.h>

class TvmConfig {
public:
  TvmConfig (char *host, char *locn);           	// constructor
  int nextVar();					// select next entry
  int selectByName (char*);       			// select by var name

  inline int firstVar() 	{return ((tvm_idx = 0) < tvm_cnt);}
  inline char *displayType() 	{return display_type[tvm_idx];}
  inline char *varName() 	{return var_name[tvm_idx];}
  inline int displayNumber() 	{return display_number[tvm_idx];}
  inline int displayEntry() 	{return display_entry[tvm_idx];}
  inline int index()           	{return tvm_idx;} 
 
private:
  int openConfig (char *host);		// open the config file
  void readLabelLines();		// read past the label lines
  void readConfigLines (char *locn);	// read a file entry line

  void closeConfig() 	{fclose (fp);}

  FILE *fp;
  str12 display_type[TVM_FIXED_FMT_TOTAL_ENTRIES];	// display types
  str12 var_name[TVM_FIXED_FMT_TOTAL_ENTRIES];          // variable names
  int display_number[TVM_FIXED_FMT_TOTAL_ENTRIES];      // display numbers
  int display_entry[TVM_FIXED_FMT_TOTAL_ENTRIES];      	// display entry idxs
  int tvm_idx;
  int tvm_cnt;
};
 
#endif
