// MODULE:  TvmStatus.h
// PURPOSE: Class Definition and needed includes and defines
// DATE:    3/28/95
// AUTHOR:  Reif Heck 

#ifndef TVMSTATUS_H
#define TVMSTATUS_H

#include "mv162.h" /* mvme162 includes */
#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdioLib.h>
 
#include <tvmDefs.h>

#include <TvmDisplay.h>
 
// DEFINE KEYCODES FOR STATUS DISPLAY

#define SYSTEM_STATUS 150
 
class TvmStatus
{
  public:
    TvmStatus(TvmDisplay& td);
    void initDisplay();
    void clearDisplay();
    int processKeys();
    void displayTime(int hour, int minute, int sec);
    void displayDate(int year, int month, int day);
    void displayFlightNumber(char *flt_num);
    void displayProjectNumber(char *proj_num);
    void displayMachineName(char *mach_name);
    void statusWindow(char *status);
    void updateStatusDisplay();

  private:
    TvmDisplay &tvm_display;
    char system_time[20];
    char system_date[20];
    char flight_number[20];
    char project_number[20];
    char machine_name[20];
    char status_string[81]; 
    int display_active;
};

#endif // TVMSTATUS_H
