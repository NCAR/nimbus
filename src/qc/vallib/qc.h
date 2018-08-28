/*
* Filename:  qc.h
* Author:    Marc Anderson
* Compiler:  GNU C++ 2.6.0
* Copyright 1995, All Rights Reserved
*
* Description:
*   definitions and prototypes for qc.cc
*
*/

#ifndef QC_H
#define QC_H


/* these C++ definitions are only necessary for QC.C and PSEUDOWINPUT_C */
#ifdef QC_C

#include "val.h"


const int QC_MAX_RT_TESTS = 5;
const int QC_MAX_PP_VARS  = 5;
const int QC_MAX_PP_TESTS = 5;


// 
// struct ValRealtimeTestbatch
//
// realtime tests all consist of only one variable with any number of tests
//
struct ValRealtimeTestbatch
{
    ValVarref *varref;

    ValTest *tests[QC_MAX_RT_TESTS];
    u_int16 num_tests;

    boolean enabled;    // enabled explicitly on initialization

    ValRealtimeTestbatch(void) { num_tests = 0; enabled = FALSE; }
};


//
// struct ValPostTestbatch
//
// postprocessing test batches consist of any number of variables mapped
// to any number of tests
//
// (not used for anything, but this is definitely how this structure
// will look if postprocessing is ever implemented) -- i.e. multiple
// variables mapped to multiple tests..
//
struct ValPostTestbatch
{
    ValVarref *varref[QC_MAX_PP_VARS];
    u_int16 num_vars;

    ValTest *tests[QC_MAX_PP_TESTS];
    u_int16 num_tests;

    ValPostTestbatch(void) { num_vars = num_tests = 0; }
};

/* has to be done this wierd way in order for C-->C++ linking to be correct */
int qc_rt_init(void);   /* (not using 'boolean' to avoid conflict)*/
void qc_rt_dumpstate(void);  /* (with chris) */
int qc_rt_exec(void);
void qc_rt_cleanup(void);

int qc_pp_init(void);
int qc_pp_exec(void);
void qc_pp_cleanup(void);

#else  /* end c++ stuff */

/* chris's "C" extern definitions */
extern int qc_rt_init(void);   /* (not using 'boolean' to avoid conflict)*/
extern void qc_rt_dumpstate(void);  /* (with chris) */
extern int qc_rt_exec(void);
extern void qc_rt_cleanup(void);

extern int qc_pp_init(void);
extern int qc_pp_exec(void);
extern void qc_pp_cleanup(void);

#endif  /* end PSEUDOWINPUT_C and QC_C */

#endif
