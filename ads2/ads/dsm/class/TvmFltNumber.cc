// MODULE:  TvmFltNumber.cc
// PURPOSE: To allow the operator to input a
//	    flight number from the display.
// DATE:    3/27/95
// AUTHOR:  Reif Heck

#include <TvmFltNumber.h>

TvmFltNumber::TvmFltNumber(TvmDisplay &td):tvm_display(td)
{
  display_active = FALSE;
  flt_num[1]=0;
  flt_num[2]=0;
  count=1;
  strcpy (flt_num, "R00");
}

void TvmFltNumber::clearDisplay()
{
  display_active = FALSE;
  tvm_display.clearDisplay();
}

void TvmFltNumber::initDisplay() 
{
  tvm_display.writeString("BL\"FLIGHT NUMBER\"");
  tvm_display.writeString("BC150,0000003");
  tvm_display.writeString("BL\"  1  \"");
  tvm_display.writeString("BC1,0020000");
  tvm_display.writeString("BL\"  2  \"");
  tvm_display.writeString("BC2,0020002");
  tvm_display.writeString("BL\"  3  \"");
  tvm_display.writeString("BC3,0020004");
  tvm_display.writeString("BL\"  4  \"");
  tvm_display.writeString("BC4,0030000");
  tvm_display.writeString("BL\"  5  \"");
  tvm_display.writeString("BC5,0030002");
  tvm_display.writeString("BL\"  6  \"");
  tvm_display.writeString("BC6,0030004");
  tvm_display.writeString("BL\"  7  \"");
  tvm_display.writeString("BC7,0040000");
  tvm_display.writeString("BL\"  8  \"");
  tvm_display.writeString("BC8,0040002");
  tvm_display.writeString("BL\"  9  \"");
  tvm_display.writeString("BC9,0040004");
  tvm_display.writeString("BL\"  R  \"");
  tvm_display.writeString("BC52,0050000");
  tvm_display.writeString("BL\"  0  \"");
  tvm_display.writeString("BC0,0050002");
  tvm_display.writeString("BL\"  T  \"");
  tvm_display.writeString("BC54,0050004");
  tvm_display.writeString("BL\" ENTER \"");
  tvm_display.writeString("BC180,0020007");
  tvm_display.writeString("BL\" CLEAR \"");
  tvm_display.writeString("BC185,0050007");
  displayFlightNumber();
  display_active=TRUE;
}

int TvmFltNumber::processKeys()
{
  switch(tvm_display.getKeycode())
    {
      case FLIGHT_NUMBER:
        count=1;
	return(DISPLAY_ACTION_TOGGLE);
      case ZERO:
	flt_num[count]='0';
	break;
      case ONE:
	flt_num[count]='1';
	break;
      case TWO:
	flt_num[count]='2';
	break;
      case THREE:
	flt_num[count]='3';
	break;
      case FOUR:
	flt_num[count]='4';
	break;
      case FIVE:
	flt_num[count]='5';
	break;
      case SIX:
	flt_num[count]='6';
	break;
      case SEVEN:
	flt_num[count]='7';
	break;
      case EIGHT:
	flt_num[count]='8';
	break;
      case NINE:
	flt_num[count]='9';
	break;
      case R:
	flt_num[0]='R';
  	displayFlightNumber();
  	return(DISPLAY_ACTION_NONE);
	break;
      case T:
	flt_num[0]='T';
  	displayFlightNumber();
  	return(DISPLAY_ACTION_NONE);
	break;
      case ENTER:
        count=1;
        return(DISPLAY_ACTION_KEY);
      case CLEAR:
       	flt_num[1]='0'; 
       	flt_num[2]='0'; 
        count=2;
	break;
      default:
        return(DISPLAY_ACTION_NONE);
    }        
  displayFlightNumber();
  count = (count % 2) + 1;
  return(DISPLAY_ACTION_NONE);
}

void TvmFltNumber::displayFlightNumber()
{
  char buffer[20];

  sprintf (buffer, "IS\"%s\"", flt_num);
  tvm_display.writeString("CS026,032");
  tvm_display.writeString(buffer);
}
