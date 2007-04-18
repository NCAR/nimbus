#ifndef _DataUsb2d64_h_
#define _DataUsb2d64_h_

#include <iostream>
#include <qwidget.h>

#include "DataMng.h"

/* -------------------------------------------------------------------- */
/**
 * Class to mng PMS-2D 64 bit data.
 */
class DataUsb2d64: public DataMng
{

public:
  DataUsb2d64 ( FILE * fp);
  virtual ~DataUsb2d64() {}  

 
  
protected:


private:
  FILE * _fp;
};

#endif
