// MODULE:  TvmFixedFmt.h
// PURPOSE: Class Definition and needed includes and defines
// DATE:    3/28/95
// AUTHOR:  Reif Heck 

#ifndef TVMFIXEDFMT_H
#define TVMFIXEDFMT_H

#include "mv162.h" /* mvme162 includes */
#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdioLib.h>
 
#include <tvmDefs.h>

#include <TvmDisplay.h>
 
// DEFINE KEYCODES FOR FIXED FORMAT DISPLAY

#define FIXED_FORMAT          150

// DEFINE SCREEN LOCATIONS AND NUMBER OF LOCATION

#define NAME_LOCATION_0     ("CS004,030")
#define NAME_LOCATION_1     ("CS004,046")
#define NAME_LOCATION_2     ("CS004,062")
#define NAME_LOCATION_3     ("CS004,078")
#define NAME_LOCATION_4     ("CS004,094")
#define NAME_LOCATION_5     ("CS004,110")

#define VALUE_LOCATION_0    ("CS112,030")
#define VALUE_LOCATION_1    ("CS112,046")
#define VALUE_LOCATION_2    ("CS112,062")
#define VALUE_LOCATION_3    ("CS112,078")
#define VALUE_LOCATION_4    ("CS112,094")
#define VALUE_LOCATION_5    ("CS112,110")

class TvmFixedFmt
{
  public:
    TvmFixedFmt(TvmDisplay& td,int display_nbr );
    void initDisplay();
    void clearDisplay();
    int processKeys();
    void displayTime(int hour, int minute, int sec);
    void initVariableNames(char *var_name, int entry);
    void updateVariableEntry(float value, int entry);

  private:
    void initScreenLocation(int entry, char *name_loc, char *value_loc);
    TvmDisplay &tvm_display;
    char system_time[20];
    char button_label[20];
    char name_location[TVM_FIXED_FMT_MAX_ENTRIES][20];
    char value_location[TVM_FIXED_FMT_MAX_ENTRIES][20];
    char variable_name[TVM_FIXED_FMT_MAX_ENTRIES][20];
    char variable_value[TVM_FIXED_FMT_MAX_ENTRIES][20];
    int display_active;
    int display_number;
};

#endif // TVMFIXEDFMT_H
