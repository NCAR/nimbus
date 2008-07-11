// MODULE:  TvmTape.cc
// PURPOSE: Class for controlling two EXABYTE DRIVES from the TVM24128 display
// DATE:    3/24/95
// AUTHOR:  Reif Heck

#include <TvmTape.h>

TvmTape::TvmTape(TvmDisplay &td):tvm_display(td)
{
  drv_num=DRV_0;
  drv_action=TAPE_UNLOAD;
  drv_status[DRV_0] = NOT_PRES;
  display_active = FALSE;
}


void TvmTape::initDisplay()
{
  tvm_display.writeString("BL\"TAPE CONTROL\"");
  tvm_display.writeString("BC150,0000003");
  tvm_display.writeString("BL\"RECORD DRV0\"");
  tvm_display.writeString("BC160,0010000");
  tvm_display.writeString("BL\"RECORD DRV1\"");
  tvm_display.writeString("BC165,0010006");
  tvm_display.writeString("BL\"STOP RECORD\"");
  tvm_display.writeString("BC180,0040003");
  tvm_display.writeString("BL\"UNLOAD DRV0\"");
  tvm_display.writeString("BC170,0050000");
  tvm_display.writeString("BL\"UNLOAD DRV1\"");
  tvm_display.writeString("BC175,0050006");
  tvm_display.writeString("CS000,48");
  tvm_display.writeString("IS\"DRV0 STATUS:\"");
  tvm_display.writeString("CS144,48");
  tvm_display.writeString("IS\"DRV1 STATUS:\"");
  display_active = TRUE;
  displayDriveStatus (DRV_0, drv_status[DRV_0]);
}

void TvmTape::clearDisplay()
{
  display_active = FALSE;
  tvm_display.clearDisplay();
}


int TvmTape::processKeys()
{
  switch(tvm_display.getKeycode())
    {
      case TAPE_CONTROL:
        return(DISPLAY_ACTION_TOGGLE);
      case RECORD_DRV0:
        drv_num=DRV_0;
	drv_action=TAPE_RECORD;
	return(DISPLAY_ACTION_KEY);
      case STOP_RECORD:
	drv_action=TAPE_STOP;
	return(DISPLAY_ACTION_KEY);
      case UNLOAD_DRV0:
        drv_num=DRV_0;
	drv_action=TAPE_UNLOAD;
	return(DISPLAY_ACTION_KEY);
      default:
   	return(DISPLAY_ACTION_NONE);
    }
}
 

void TvmTape::displayDriveStatus(int drv_number,int drive_status)
{
  drv_status[drv_number] = drive_status;

  if (!display_active)
    return;
  
  switch(drv_number)  // Place cursor at correct location on display
    {
      case DRV_0:
        tvm_display.writeString("CS004,64");
        break;
     } 

  switch(drive_status)  // Send status string to display
    {
      case UNLOADED:
  	tvm_display.writeString("IS\"UNLOADED    \"");
	break;

      case LOADING:
  	tvm_display.writeString("IS\"LOADING     \"");
	break;

      case LOADED:
  	tvm_display.writeString("IS\"LOADED      \"");
	break;

      case RECORDING:
  	tvm_display.writeString("IS\"RECORDING   \"");
	break;

      case UNLOADING:
  	tvm_display.writeString("IS\"UNLOADING   \"");
	break;

      case NOT_PRES:
        tvm_display.writeString("IS\"NOT PRESENT \"");
	break;

      case FAILED:
  	tvm_display.writeString("IS\"FAILED      \"");
	break;

      case BUSY:
  	tvm_display.writeString("IS\"BUSY        \"");
	break;
    } 
}
