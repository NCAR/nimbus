//
// Filename:  val.h
// Author:    Marc Anderson
// Compiler:  GNU C++ 2.6.0
// Copyright 1994, All Rights Reserved
//
// Description:
//    definitions for Valkyrie validation library data structures
//
//

#ifndef VAL_H
#define VAL_H

#include <time.h>
#include <fstream>
#include <stdarg.h>
#include <cstring>
#include "stddefs.h"
#include "valqueue.h"

// WINDS/WINPUT include files:
#include "nimbus.h"
#include "header.h"
#include "decode.h"
#include "amlib.h"

// plus a little kludge to a couple more WINPUT routines
#include "cint.h"

// and some WINPUT externs that aren't defined anywhere in include files
// (although they _should_ be :-)
extern NR_TYPE	*SampledData;   
extern char	*proj_number;
extern int	GetShmemElapsedTime();


// (socket library stuff):
#include "message.h"
#include "sleep.h"        
#include "report.h"




///////////////////////////////////////////////////////////////////////////////
//
// general validation library constants and type definitions
//


//
// general typedefs
//
typedef u_int16 ValFreq;                 // frequency representation
                                         //    (# events/min)

typedef real32 ValVariance;              // statistical variance for
                                         // flatline / level shift /

typedef real32 ValSlope;                 // slope for level shift tests

typedef real64 WinputDatavalue;          // datavalue of a WINPUT variable

typedef u_int16 ValVarrefIndex;          // indexes a ValVarref array

typedef u_int16 WinputSRindex;           // index to specify SR sample

typedef u_int16 WinputHRindex;           // index to specify HR sample

typedef u_int16 WinputVarIndex;          // indexes raw[], sdi[], or derived[]

typedef int16 WinputSRfreq;              // freq in SR db (i.e.1000 == 1000 hz)



//
// enum ValTestRtn
//
// validation tests return a type of this
//
enum ValTestRtn
{
    VAL_PASSED,        // test was passed (no further information)
    VAL_FAILED_LOW,    // test was failed -- low severity failure
    VAL_FAILED_HIGH    // test was failed -- high severity failure
};



//
// struct ValErrorMsg
//
// structure containing error messages for the purpose of the internal
// validation library
//
struct ValErrorMsg
{
    ValSeverity sev;
    ValType type;
    char message[SOCK_MSGLEN];

    ValErrorMsg(ValSeverity tmp_sev, ValType tmp_type, const char *tmp_msg)
    { 
	sev = tmp_sev; type = tmp_type;
	strncpy(message, tmp_msg, SOCK_MSGLEN);
    }
};




//
// the types of WINPUT variables we deal with (3 major databases)
//
enum WinputVarType
{
    WINPUT_SDI,
    WINPUT_RAW,
    WINPUT_DERIVED
};


// (high rate database fixed @25 hz)
const WinputHRindex WINPUT_HR_FREQ = 25;   


// generic error return code for ValTest procedures when an invalid WINPUT
// database is specified (not necessarily a single type)
const WinputDatavalue VALTEST_INVALID_TYPE = -1;


//
// class ValVarref
//
// this structure completely specifies a winput variable so that it can
// be referenced transparently by the validation library without
// screwing with WINPUT at all.  it provides all of the necessary
// interface functionality.
//
class ValVarref
{
    WinputVarType type;
    WinputVarIndex index;
    
  public:
    ValVarref( WinputVarType temp_type, WinputVarIndex temp_index)
       { type = temp_type; index = temp_index; }

    WinputVarType get_type(void) { return type; }

    WinputVarIndex get_index(void) { return index; }

    char *get_name(void);

    // look up value in WINPUT 'average' database
    WinputDatavalue get_winput_avg(void);

    // look up value(s) or frequency in WINPUT 'sample rate' database 
    WinputDatavalue get_winput_SR(WinputSRindex sample_index);

    boolean get_winput_SR_range(WinputDatavalue *dest,
				WinputSRindex start_index,
				WinputSRindex end_index);

    WinputSRfreq get_winput_SR_freq(void);   // return frequency

    // look up value(s) in WINPUT 'high rate' database (fixed at 25 hertz)
    WinputDatavalue get_winput_HR(WinputHRindex sample_index = 0);

    boolean get_winput_HR_range(WinputDatavalue *dest,
				WinputHRindex start_index,
				WinputHRindex end_index);
};


//
// class ValTimeQueueNode
//
// node in queue of error times.  (used to maintain 'last 2 minutes' 
// counter)  
//
// (probably could be done better with templates, but i'm too lazy to
// convert my generic queue class to do this :-)
//
class ValTimeQueueNode : private ValQueueNode
{
  public:
    int error_time;

    ValTimeQueueNode(int init_time) { error_time = init_time; }
};

const int NULL_TIME = -1;   // used in error returns that return time


//
// class ValTimeQueue
//
// queue of error times.. (used to maintain 'last 2 minutes' counter)
//
// (probably could be done better with templates, but i'm too lazy to
// convert my generic queue class to do this :-)
//
class ValTimeQueue : private ValQueue
{
  public:
    ValTimeQueue(void) { }
    ~ValTimeQueue(void) { while(get_num_items()) (void) dequeue(); }

    int dequeue(void);
    boolean enqueue(int new_time);
    int at_head(void);

    u_int16 get_num_items(void) { return ValQueue::get_num_items(); }
};



// 
// class ValTest
//
// abstract base class for all simple tests
//
class ValTest
{
  public:
    ValVarref *vars;            // maybe this should be physically copied (?)
    ValVarrefIndex num_vars;    // (not currently being used anywhere)
    ValTimeQueue time_queue;    // history log of times that error msgs sent


    // 2 vars below are used to maintain 'consec' counter (w/this algorithm)
    //
    //   if(current_elapsed == (lasterror_elapsed + 1))
    //      ++consec_counter
    //   else
    //      consec_counter = 1    // only one happened in a row
    //
    //   lasterror_elapsed = current_elapsed
    int lasterror_elapsed;
    u_int32 consec_counter;
    
    ValTest(ValVarref temp_vars[], ValVarrefIndex temp_num_vars=1)
       { vars = temp_vars; num_vars = temp_num_vars; consec_counter = 1; 
	 lasterror_elapsed = 0; }


    // perform actual test
    virtual ValTestRtn test(char *current_timestr, 
			    int current_elapsed) = 0;

    // return name of test (for diagnostics)
    virtual char *get_name(void) = 0;    

    // write out a string containing the validation test params (for diags)
    virtual void out_params(std::ostream &str) = 0;

    void error_handle(ValErrorMsg error_msg, char *current_timestr,
		      int current_elapsed); 
    // maybe virtual later (?)
};



//
// class ValTestRange
//
// class for simple range test
//
class ValTestRange : public ValTest
{
    WinputDatavalue low_thresh, high_thresh, highprio_delta;

  public:
    ValTestRange(ValVarref &ref, WinputDatavalue init_low,
		 WinputDatavalue init_high, WinputDatavalue init_delta):
       ValTest(&ref)
    {
	low_thresh = init_low;
	high_thresh = init_high;
	highprio_delta = init_delta;
    }

    ValTestRtn test(char *current_timestr, int current_elapsed);

    char *get_name(void) { return "range"; }

    void out_params(std::ostream &str) 
       { str << low_thresh << " " << high_thresh << " " << highprio_delta; }
};




#define VAL_FLATLINE_NUMHIST   10             // 10  seconds of history

//
// class ValTestFlatline 
//
// class for simple flatline test
//
class ValTestFlatline : public ValTest
{
    ValVariance min_var_low, min_var_high;    // minimum variance to justify
                                              // low/high severity messages

    WinputDatavalue history[VAL_FLATLINE_NUMHIST];  // most recent value in 0

  public:
    ValTestFlatline(ValVarref &ref, ValVariance temp_low,
		      ValVariance temp_high);

    ValTestRtn test(char *current_timestr, int current_elapsed);

    char *get_name(void) { return "flatline"; }

    void out_params(std::ostream &str)
       { str << min_var_low << " " << min_var_high; }
};



//
// class ValTestLevel
//
// class for level shift test
// 
class ValTestLevel : public ValTest
{
    ValSlope min_slope;
    WinputDatavalue min_shift;
    WinputSRindex spike_peak_maxintvls;
    WinputDatavalue spike_rtn_slack;

  public:
    ValTestLevel(ValVarref &ref, ValSlope temp_min_slope,
		 WinputDatavalue temp_min_shift,
		 WinputSRindex maxintvls_temp,
		 WinputDatavalue slack_temp) :
         ValTest(&ref)
    {
	min_slope = temp_min_slope;
	min_shift = temp_min_shift;
	spike_peak_maxintvls = maxintvls_temp;
	spike_rtn_slack = slack_temp;
    }

    ValTestRtn test(char *current_timestr, int current_elapsed);

    char *get_name(void) { return "level"; }
 
    void out_params(std::ostream &str)
       { str << min_slope << " " << min_shift << " " << spike_peak_maxintvls 
	 << " " << spike_rtn_slack; }
};


#ifdef VAL_C // -------------------------------------------------------------

   // string constants (note that these cannot exceed SOCK_TYPELEN (message.h))
   const char *VAL_RANGESTR = "range";
   const char *VAL_LEVELSTR = "level";
   const char *VAL_SPIKESTR = "spike";
   const char *VAL_FLATLINESTR = "flatline";
   const char *VAL_QUITSTR = "quit";   // msg sent to GUI to order it to quit


#ifdef VAL_USEGUI
   // perhaps bad as a global variable, but, unless declared 'static' 
   // within the ValTest class, this is necessary because a single
   // instance of this interfacing object is needed for all validation
   // tests.  
   //
   // this will probably be cleaned up when we implement postprocessing tests,
   // because the Report class and GUI will probably not even be used..
   Report *val_socket;
#endif

#ifdef VAL_DISKLOG
   std::ofstream *val_disklog;
#endif


#else  // not QC_C -----------------------------------------------------------

   extern const char *VAL_RANGESTR;
   extern const char *VAL_LEVELSTR;
   extern const char *VAL_SPIKESTR;
   extern const char *VAL_FLATLINESTR;
   extern const char *VAL_QUITSTR;

#ifdef VAL_USEGUI
   extern Report *val_socket;
#endif

#ifdef VAL_DISKLOG
   extern std::ofstream *val_disklog;
#endif

#endif  // outer ifdef VAL_C -------------------------------------------------


#endif

