#include "DataUsb2d64.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

//const unsigned long long DataMng::_syncWord = 0xAAAAAAA000000000LL;
//const unsigned long long DataMng::_syncMask = 0xFFFFFFF000000000LL;


/* -------------------------------------------------------------------- */
DataUsb2d64::DataUsb2d64 (FILE * fp) :DataMng(fp, 64), _fp(fp)
{
// get file pointer at end of nidas header.
  rewind(_fp);
  char buff[TBYTE];
  fread(buff, TBYTE, 1, _fp);
  char * p = strstr(buff, "end header\n");
  if (p == 0) {
    printf("Can't locate 'end header'\n");
    exit(-1);
  }
  //_init(64);
}


