#include "DataMng.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>


/* -------------------------------------------------------------------- */
DataMng::DataMng(FILE * fp, short b) 
{
  _fp=fp;
  _bit_n=b;
  _fsize=0;
  Init();
}

DataMng::DataMng()
{
  _fsize=0;
  _bit_n=0;
  _fp=NULL;
  _pts=NULL;
}


DataMng::~DataMng() {
  _bit_n=0 ;	
  
  if (_hdr!=NULL) { delete _hdr;}
  if (_twod_rec!=NULL) { delete _twod_rec;}
 
  if (_fp) { delete _fp;}
  if (_pts) { delete _pts;} 
  if (_ptstx) { delete _ptstx;} 
  if (_ptsln) { delete _ptsln;} 
}

/* -------------------------------------------------------------------- */
bool DataMng::IsSameFSize() {
  if (!_chkInit()) {return false;}
  
  //get filesize
  fseek (_fp, 0, SEEK_END);
  int size=ftell(_fp);
 
  //check if the same file size
  if (_fsize!=0 && size==_fsize) {
    QTime t =QTime::currentTime();
    std::cout<<"\n Same file size! "<<t.hour()<<":"<<t.minute()<<":"<<t.second()<<std::endl;
    return false;
  } 
  _fsize = size;
    
  return true;
}


/* -------------------------------------------------------------------- */
void DataMng::Init()
{
  if (!_chkInit()){return;}
  if (_bit_n==64) {
    _byte_n= _bit_n/8; 
    _row_n= 7;
    _rcdpr_n= 2; 
    _byte_usb2d_rcd= 8 +TBYTE; //4104
  }

  if (_bit_n==32) {
    _byte_n= _bit_n/8; 
    _slide_n= TBYTE/_byte_n;
    _row_n= 16;
    _rcdpr_n= 1; 
    _byte_usb2d_rcd= 8 +TBYTE; //????32
  }

  _slide_n= TBYTE/_byte_n;
  _pts= new QPointArray(TBYTE*8*_row_n*_rcdpr_n); 


  // get file pointer at end of nidas header.
  rewind(_fp);
  char buff[TBYTE];
  fread(buff, TBYTE, 1, _fp);
  char * p = strstr(buff, "end header\n");
  if (p == 0) {
    printf("Can't locate 'end header'\n");
    exit(-1);
  }
  int offs =(p - buff) + 11;
  fseek(_fp, offs, 0);
  if (fgetpos(_fp, &_pp)!=0) {  
    std::cerr<<"\ngetpos error\n";
    _fp=NULL;
  }
}


/* -------------------------------------------------------------------- */
bool DataMng::_chkInit() {
  if (_bit_n<=0 ||_fp==NULL ) {std::cout << "Initialize class DataMng is required! \n"<< std::endl; return false;}
  return true;
}


