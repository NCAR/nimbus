/*
* Filename:  cint.c
* Author:    Marc Anderson
* Compiler:  GNU C 2.6.0
* Copyright 1995, All Rights Reserved
*
* Description:
*   only in a couple places, this stupid explicit c++->c interface 
*   is reqd.
*/

#define CINT_C

#include "cint.h"

extern int SearchTable(char *table[], int ntable, char *targ);

/*
* search_winput_table()
*
* call chris's function
*/
int search_winput_table(char *table[], int ntable, char *targ)
{
    return SearchTable(table, ntable, targ); 
}
