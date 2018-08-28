/*
* Filename:  cint.h
* Author:    Marc Anderson
* Compiler:  GNU C 2.6.0
* Copyright 1995, All Rights Reserved
*
* Description:
*   c interface to chris's code
*
*/

#ifndef CINT_H
#define CINT_H

#ifdef CINT_C
  int search_winput_table(char *table[], int ntable, char *targ);
#else
  extern int search_winput_table(char *table[], int ntable, char *targ);
#endif

#endif
