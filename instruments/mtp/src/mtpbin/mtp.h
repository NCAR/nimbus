/*! \file mtp.h
 ** \brief constant declarations
 **
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** Copyright 2015 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
 **
*/
#ifndef _mtp_h
#define _mtp_han 

#define NUM_CHANNELS    3   ///< number of frequencies used by the MTP
#define NUM_SCAN_ANGLES 10  ///< number of scan angles
#define NUM_RETR_LVLS   33  ///< number of retrieval levels per flight level in RCFs
#define NUM_BRT_TEMPS   30  /*!< number of brightness temperatures = 
                               number of scan angles * number of channels */

#endif
