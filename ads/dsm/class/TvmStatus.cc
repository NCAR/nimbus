// MODULE:  TvmStatus.cc
// PURPOSE: The default display that shows time,date,flight number,
//	    project number,machine name, and system status messages.
// DATE:    3/28/95
// AUTHOR:  Reif Heck

#include <TvmStatus.h>

TvmStatus::TvmStatus(TvmDisplay &td):tvm_display(td)
{
  display_active = FALSE;
  
  sprintf (system_time,"IS\"%02d:%02d:%02d\"", 0, 0, 0);
  sprintf (system_date, "IS\"%02d/%02d/%02d\"", 0, 0, 0);
  sprintf (flight_number, "IS\"%s\"", "R00");
  sprintf (project_number, "IS\"%s\"", "000");
  machine_name[0] = '\0';
  status_string[0] = '\0';
}


void TvmStatus::clearDisplay()
{
  display_active = FALSE;
  tvm_display.clearDisplay();
}

void TvmStatus::initDisplay() 
{
  display_active=TRUE;
  tvm_display.writeString("BL\"DSM STATUS\"");
  tvm_display.writeString("BC150,0000003");
  tvm_display.writeString("CS004,026");
  tvm_display.writeString("IS\"FLIGHT NUMBER:\"");
  tvm_display.writeString("CS004,046");
  tvm_display.writeString("IS\"PROJECT NUMBER:\"");
  tvm_display.writeString("CS004,066");
  tvm_display.writeString("IS\"MACHINE NAME:\"");
  tvm_display.writeString("GX0021,000,083,239,127");
  updateStatusDisplay();
}

int TvmStatus::processKeys()
{

  switch(tvm_display.getKeycode())
    {
      case SYSTEM_STATUS:
	return(DISPLAY_ACTION_TOGGLE);
      default:
        return(DISPLAY_ACTION_NONE);
    }        
}


void TvmStatus::displayTime (int hour, int minute, int sec)
{
  sprintf (system_time,"IS\"%02d:%02d:%02d\"", hour, minute, sec);
  if(display_active)
    {
      tvm_display.writeString("CS004,006");
      tvm_display.writeString(system_time);
    }
}


void TvmStatus::displayDate(int year, int month, int day)
{
  sprintf (system_date, "IS\"%02d/%02d/%02d\"", month, day, year);
  if(display_active)
    {
      tvm_display.writeString("CS176,006");
      tvm_display.writeString(system_date);
    }
}


void TvmStatus::displayFlightNumber(char *flt_num)
{
  sprintf (flight_number, "IS\"%s\"", flt_num);
  if(display_active)
    {
      tvm_display.writeString("CS150,026");
      tvm_display.writeString(flight_number);
    }
}


void TvmStatus::displayProjectNumber(char *proj_num)
{
  sprintf (project_number, "IS\"%s\"", proj_num);
  if(display_active)
    {
      tvm_display.writeString("CS150,046");
      tvm_display.writeString(project_number);
    }
}


void TvmStatus::displayMachineName(char *mach_name)
{
  sprintf (machine_name, "IS\"%s\"", mach_name);
  if(display_active)
    {
      tvm_display.writeString("CS150,066");
      tvm_display.writeString(machine_name);
    }
}

void TvmStatus::statusWindow(char *status)
{
  sprintf (status_string, "IS\"%s\"", status);
  if(display_active)
    {
      tvm_display.writeString("FS001");
      tvm_display.writeString("CS006,96");
      tvm_display.writeString(status_string);
      tvm_display.writeString("FS000");
    }
}

void TvmStatus::updateStatusDisplay()
{
  tvm_display.writeString("CS004,006");
  tvm_display.writeString(system_time);
  tvm_display.writeString("CS176,006");
  tvm_display.writeString(system_date);
  tvm_display.writeString("CS150,026");
  tvm_display.writeString(flight_number);
  tvm_display.writeString("CS150,046");
  tvm_display.writeString(project_number);
  tvm_display.writeString("CS150,066");
  tvm_display.writeString(machine_name);
}
