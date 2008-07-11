/* SyncVar.h
 
   Sync data variable class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef SYNCVAR_H
#define SYNCVAR_H

#include <stdio.h>

#include <header.h>
#include <TapeHeader.h>

class SyncVar {
public:
  SyncVar (TapeHeader *hdr, const char *name);

  inline float value() 	{ return sv_value; }
  inline float volts() 	{ return sv_volts; }

  inline void computeFromLR (const char *log_rec) 
		{ if (strcmp(sv_name, DPRES_STR))
		    computeFromSample (*(short*)(log_rec + start));
		  else
		    sv_value = ntohf(*(float *)(log_rec + start)) * 10.0;
		}

  void computeFromSample (short raw);	// compute value from raw sample
 
private:
  float sv_volts;			// current voltage value
  float sv_value;			// current computed value

  str12 sv_name;			// variable name
  float cof[MX_COF];			// cal coefficients
  float convert;			// bits to volts conversion factor
  int convert_offset;			// conversion offset counts
  int order;				// polynomial order
  int offset;				// offset between samples
  int start;				// lr offset to first sample
};

#endif
