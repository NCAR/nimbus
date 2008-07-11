// MODULE:  TvmUtilities.cc
// PURPOSE: Class for changing TVM24128 display characteristics
// DATE:    3/15/95
// AUTHOR:  Reif Heck

#include <TvmUtilities.h>

TvmUtilities::TvmUtilities(TvmDisplay &td):tvm_display(td)
{
  backlight_on = TRUE;
  reverse_on = FALSE;
  beep_on = FALSE;
  display_active = FALSE;
}


void TvmUtilities::initDisplay()
{
  tvm_display.writeString("BL\"UTILITIES\"");
  tvm_display.writeString("BC150,0000003");
  tvm_display.writeString("BL\"CONTRAST +\"");
  tvm_display.writeString("BC180,0010006");
  tvm_display.writeString("BL\"CONTRAST -\"");
  tvm_display.writeString("BC185,0030006");

  if (backlight_on) {
    tvm_display.writeString("BL\"BACKLIGHT ON \"");	// Load label
    tvm_display.writeString("BC160,0010000"); 		// Place button
  }
  else {
    tvm_display.writeString("BL\"BACKLIGHT OFF\"");
    tvm_display.writeString("BC165,0010000");
  }

  if (reverse_on) {
    tvm_display.writeString("BL\"REV VIDEO ON \"");
    tvm_display.writeString("BC170,0030000");
  }
  else {
    tvm_display.writeString("BL\"REV VIDEO OFF\"");
    tvm_display.writeString("BC175,0030000"); 
  }

  if (beep_on) {
    tvm_display.writeString("BL\"BEEP ON      \"");
    tvm_display.writeString("BC190,0050000");
  }
  else {
    tvm_display.writeString("BL\"BEEP OFF     \"");
    tvm_display.writeString("BC195,0050000");
  }
  display_active = TRUE;
}

void TvmUtilities::clearDisplay()
{
  display_active = FALSE;
  tvm_display.clearDisplay();
}


int TvmUtilities::processKeys()
{
  switch(tvm_display.getKeycode())
    {
      case UTILITIES:
        return(DISPLAY_ACTION_TOGGLE);
      case BL_ON:			// toggle from on to off
        tvm_display.writeString("BD160");         // Delete old button
        tvm_display.writeString("BL\"BACKLIGHT OFF\"");  // Load label
        tvm_display.writeString("BC165,0010000"); // Place button
	tvm_display.backLightOff();
        backlight_on = FALSE;
	return(DISPLAY_ACTION_KEY);

      case BL_OFF:			// toggle from off to on
        tvm_display.writeString("BD165");          // Delete old button
        tvm_display.writeString("BL\"BACKLIGHT ON \"");   // Load label
        tvm_display.writeString("BC160,0010000");  // Place button 
	tvm_display.backLightOn();
        backlight_on = TRUE;
	return(DISPLAY_ACTION_KEY);

      case REV_VIDEO_ON:		// toggle from on to off
        tvm_display.writeString("BD170");          // Delete old button
        tvm_display.writeString("BL\"REV VIDEO OFF\""); // Load label
        tvm_display.writeString("BC175,0030000");      // Place button
	tvm_display.revVideoOff();
        reverse_on = FALSE;
	return(DISPLAY_ACTION_KEY);

      case REV_VIDEO_OFF:		// toggle from off to on
        tvm_display.writeString("BD175");      	// Delete old button
        tvm_display.writeString("BL\"REV VIDEO ON \""); // Load label
        tvm_display.writeString("BC170,0030000");       // Place button
	tvm_display.revVideoOn();
        reverse_on = TRUE;
	return(DISPLAY_ACTION_KEY);

      case BEEP_ON:			// toggle from on to off
        tvm_display.writeString("BD190");          // Delete old button
	tvm_display.beepOff();
        beep_on = FALSE;
        tvm_display.clearDisplay();
        initDisplay();
	return(DISPLAY_ACTION_KEY);

      case BEEP_OFF:			// toggle from off to on
        tvm_display.writeString("BD195");          // Delete old button
	tvm_display.beepOn();
        beep_on = TRUE;
        tvm_display.clearDisplay();
        initDisplay();
	return(DISPLAY_ACTION_KEY);

      case CONTRAST_UP:
        tvm_display.contrastUp();
	return(DISPLAY_ACTION_KEY);

      case CONTRAST_DN:
        tvm_display.contrastDown();
	return(DISPLAY_ACTION_KEY);

      default:
        return(DISPLAY_ACTION_NONE);
  }
}
