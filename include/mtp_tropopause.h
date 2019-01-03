/*! \file tropopause.h
 * \brief This class encapsulates calculation of tropopause heights from
 * physical temperature profiles.
 **
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** Copyright 2015-2016 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
 **
*/

#ifndef _TROPOPAUSE_h
#define _TROPOPAUSE_h

//NR_TYPE altc[NUM_RETR_LVLS];
//NR_TYPE tempc[NUM_RETR_LVLS];

float findTropopause(NR_TYPE*,NR_TYPE*);

#endif

