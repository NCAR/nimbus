// MODULE:  TvmTape.h
// PURPOSE: Required includes and class definition
// DATE:    3/23/95
// AUTHOR:  Reif Heck

#ifndef TVMTAPE_H
#define TVMTAPE_H
 
#include <vxWorks.h>
#include <stdioLib.h>

#include <dsmctl.h>
#include <messageDefs.h>
#include <tvmDefs.h>

#include <TvmDisplay.h>
 
#define TAPE_CONTROL 150
#define RECORD_DRV0  160
#define UNLOAD_DRV0  170
#define STOP_RECORD  180

class TvmTape 
{
  public:
    TvmTape(TvmDisplay& td);
    void initDisplay();
    void clearDisplay();
    int processKeys();
    void displayDriveStatus(int drv_number,int drv_status);
    inline int drive()	{return drv_num;}
    inline int action()	{return drv_action;}

  private:
    TvmDisplay &tvm_display;
    int drv_num;
    int drv_action;
    int drv_status[NDRV];
    int display_active;
};

#endif // TvmTape.h
