/* MODULE: TvmDisplay.cc
*  PURPOSE: TVM24128 LCD Status Display Class
*  DATE: 3/15/95
*  AUTHOR: REIF HECK
*
*/

#include <TvmDisplay.h>

TvmDisplay::TvmDisplay():SccClass(SCC_PORT_B)
{
  contrast_level=16;           // Set beginning contrast level
  // Initialize display
//writeString("@R");    // Turn off handshaking 
  writeString("@D");    // Disable keypress interrupts 
  writeString("SE1");   // Turn on EL backlight 
//writeString("DR1");   // Turn on reverse video
  writeString("CA000"); // Turn off cursor 
  writeString("GP0");   // Set graphics plane to plane 0
  writeString("FP1");   // Set font plane to plane 1
  writeString("BA100"); // Set button attributes (key latch on) 
  writeString("FS000"); // Select Large font 
  writeString("SC16");  // Set starting contrast level to 16
  writeString("@K");    // Seems to need this to start
  clearDisplay();

  blStatus = TRUE;
  rvStatus = TRUE;
  bpStatus = FALSE;
}

/************************ PUBLIC FUNCTIONS ***********************/    

void TvmDisplay::clearDisplay()
{
  writeString("DC"); /* Clear old display */
  writeString("BE"); /* Clear old buttons */
}


int TvmDisplay::chkKeypress()
{
  writeString("@K");           // Check for keypress 
  readString(read_buffer);     // Done second time due to
  if(!(strncmp(read_buffer,"@K1",3))) // unknown TVM bug
    {
      return(TRUE);
    }
  return(FALSE);
}


int TvmDisplay::getKeycode()
{
  int keycode;

  writeString("BK");       // Get button code  
  readString(read_buffer); // Must be done twice

// Sometimes an @K1 is returned in response to the BK instead of the correct
// key code.  In this case the BK command is issued again.
  while (!(strncmp(read_buffer,"@K1",3))) 
    {
      writeString("BK");	
      readString(read_buffer);
    }
  keycode=atoi(read_buffer+1);    // Convert keycode to integer
  printf("KEYCODE=%d\n",keycode);
  return(keycode);
}

void TvmDisplay :: backLightOn()
{
  writeString("SE1");	       // Turn backlight on
  blStatus=TRUE;		       // Set Status
}


void TvmDisplay :: backLightOff()
{
  writeString("SE0");		// Turn backlight off
  blStatus=FALSE;			// Set Status
}


void TvmDisplay :: revVideoOn()
{
  writeString("DR1");		    // Turn on reverse video
  rvStatus=TRUE;		    	    // Set status
}


void TvmDisplay :: revVideoOff()
{
  writeString("DR0");		     // Turn off reverse video
  rvStatus=FALSE;
}


void TvmDisplay :: beepOn()
{
  writeString("BA101");          // key latch & click on
  bpStatus=TRUE;
}


void TvmDisplay :: beepOff()
{
  writeString("BA100");          // key latch on & click off
  bpStatus=FALSE;
  return;
}


void TvmDisplay :: contrastUp()
{
  if(contrast_level<31) {
     contrast_level++;
     setContrast();
  }
}


void TvmDisplay :: contrastDown()
{
  if(contrast_level>0) {
     contrast_level--;
     setContrast();
  }

}

/* Private Functions. */

void TvmDisplay :: setContrast()

{
  char buffer[20];

  sprintf(buffer,"SC%d",contrast_level);	// place in command string
  writeString(buffer);	// Send command to display
/**
  writeString("FS001");		// Turn on small font
  writeString("CA0");		// Turn off cursor
  writeString("CS100,88");	// Place cursor
  sprintf(buffer,"IS\"CONTRAST= %d\"", contrast_level);	// Build quoted string
  writeString(buffer);		// Send quoted string
  writeString("CS100,98");	// Place cursor
  writeString("IS\"(MIN=0 MAX=31)\""); // Send quoted string
  writeString("FS000");		// Turn on large font
**/
  return;
}
