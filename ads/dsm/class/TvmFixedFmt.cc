// MODULE:  TvmFixedFmt.cc
// PURPOSE: This display module writes the various fixed format data
//	    ,acquired by the data sampling modules.
// DATE:    4/05/95
// AUTHOR:  Reif Heck

#include <TvmFixedFmt.h>

TvmFixedFmt::TvmFixedFmt(TvmDisplay &td,int display_nbr) : tvm_display(td)
{
  display_active = FALSE;
  display_number = display_nbr; 

  initScreenLocation (0, NAME_LOCATION_0, VALUE_LOCATION_0); 
  initScreenLocation (1, NAME_LOCATION_1, VALUE_LOCATION_1); 
  initScreenLocation (2, NAME_LOCATION_2, VALUE_LOCATION_2); 
  initScreenLocation (3, NAME_LOCATION_3, VALUE_LOCATION_3); 
  initScreenLocation (4, NAME_LOCATION_4, VALUE_LOCATION_4); 
  initScreenLocation (5, NAME_LOCATION_5, VALUE_LOCATION_5); 

  memset (variable_name, 0, sizeof (variable_name));
  memset (variable_value, 0, sizeof (variable_value));
}

void TvmFixedFmt::clearDisplay()
{
  display_active = FALSE;
  tvm_display.clearDisplay();
}

void TvmFixedFmt::initDisplay() 
{
  int i;

  display_active=TRUE;
  sprintf (button_label,"BL\"FIXED FMT%02d\"", display_number);

  tvm_display.writeString(button_label);
  tvm_display.writeString("BC150,0000003");

  for(i=0; i<TVM_FIXED_FMT_MAX_ENTRIES; i++)
    {
      if(strlen(variable_name[i]))
        {
          tvm_display.writeString(name_location[i]);
          tvm_display.writeString(variable_name[i]);
          tvm_display.writeString(value_location[i]);
          tvm_display.writeString(variable_value[i]);
          printf ("i = %d, name = %s, value = %s\n", i, variable_name[i], 
                  variable_value[i]);
	}
    }
}

int TvmFixedFmt::processKeys()
{

  switch(tvm_display.getKeycode())
    {
      case FIXED_FORMAT:
	return(DISPLAY_ACTION_TOGGLE);
      default:
        return(DISPLAY_ACTION_NONE);
    }        
}


void TvmFixedFmt::displayTime (int hour, int minute, int sec)
{
  sprintf (system_time,"IS\"%02d:%02d:%02d\"", hour, minute, sec);
  if(display_active)
    {
      tvm_display.writeString("CS004,006");
      tvm_display.writeString(system_time);
    }
}


void TvmFixedFmt::initVariableNames(char *var_name,int entry)
{
  if (entry<TVM_FIXED_FMT_MAX_ENTRIES) {
    sprintf (variable_name[entry],"IS\"%-12s\"", var_name);
    updateVariableEntry (0.0, entry);
  }
}

void TvmFixedFmt::updateVariableEntry(float value,int entry)
{
  if (entry < TVM_FIXED_FMT_MAX_ENTRIES) {
    sprintf (variable_value[entry],"IS\"%+08.2f\"", value);

    if (display_active && strlen(variable_name[entry])) {
      tvm_display.writeString(value_location[entry]);
      tvm_display.writeString(variable_value[entry]);
    }
  }
}

/* Private Functions. */

void TvmFixedFmt::initScreenLocation(int entry, char *name_loc, char *value_loc)
{
  if (entry < TVM_FIXED_FMT_MAX_ENTRIES) {
    strcpy (name_location[entry], name_loc);
    strcpy (value_location[entry], value_loc);
  }
}

