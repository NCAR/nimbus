// MODULE:  TvmUtilites.h
// PURPOSE: Required includes and class definition
// DATE:    3/23/95
// AUTHOR:  Reif Heck

#ifndef TVMUTILITIES_H
#define TVMUTILITIES_H
 
#include <vxWorks.h>

#include <tvmDefs.h>

#include <TvmDisplay.h>
 
#define UTILITIES 150
#define BL_ON 160
#define BL_OFF 165
#define REV_VIDEO_ON 170
#define REV_VIDEO_OFF 175
#define CONTRAST_UP 180
#define CONTRAST_DN 185
#define BEEP_ON 190
#define BEEP_OFF 195

class TvmUtilities
{
  public:
    TvmUtilities(TvmDisplay& td);
    void initDisplay();
    void clearDisplay();
    int processKeys();

  private:
    TvmDisplay &tvm_display;
    int backlight_on;			// backlight state
    int beep_on;			// beep state
    int reverse_on;			// reverse video state
    int display_active;
};

#endif // TvmUtilities.h
