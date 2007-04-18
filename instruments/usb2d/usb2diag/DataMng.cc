#include "DataMng.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

const unsigned long long DataMng::_syncWord = 0xAAAAAAA000000000LL;
const unsigned long long DataMng::_syncMask = 0xFFFFFFF000000000LL;


/* -------------------------------------------------------------------- */
DataMng::DataMng(FILE * fp, short b) : _fp(fp)
{
  _fsize=0;
  _init(b);
}

/* -------------------------------------------------------------------- */
bool DataMng::Posfp() {
  if (!_chkInit()) {return false;}
  //get filesize
  fseek (_fp, 0, SEEK_END);
  int size=ftell(_fp);
 
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

  //check if the same file size
  if (_fsize!=0 && size==_fsize) {
    printf("\n Same file size!");
    return false;
  }
  _fsize = size;
 
  return true;
}

/* -------------------------------------------------------------------- */
QPointArray* DataMng::GetPoints()
{ 
  if (!_chkInit()) {return NULL;}
  _get2drec(); 
  size_t	x, y = 10;
  _cnt = 0;
  for (int i = 0; i < _row_n; ++i)
  {
    x = 5;
    for (int j = 0; j< _rcdpr_n; ++j)
    {
       _getRecord(x, y, (unsigned char*)_twod_rec[i*_rcdpr_n+j].data);
       x += _slide_n+1;
    }
    y += _bit_n+10;
  }
  return _pts;
}

/* -------------------------------------------------------------------- */
void DataMng::_getRecord(int start_x, int start_y, unsigned char * data_p)
{
  for (int i = 0; i < _slide_n; ++i)
  {
    //get  and flip slide
    unsigned char slide[_byte_n];
    for (int n=0; n<_byte_n; ++n) {
      slide[_byte_n-1-n]=*data_p; data_p++;
    }
//    if ((slice & _syncMask) == _syncWord)
//      printf("%llu\n", slice & ~_syncMask);	// Print timing word.
  
    //check every bit
    for (int b=0; b<_byte_n; ++b) {
      for (int j = 0; j < 8; ++j)
      {
        if ( !((slide[b] >> j) & 0x00000001) )
        {
          _pts->setPoint(_cnt, start_x + i, start_y + _bit_n-(8*b+j));
          ++_cnt;
        }
      }//j
    }//b
  }//i
}


/* -------------------------------------------------------------------- */
void DataMng::_get2drec() {
   int trcd = _row_n*_rcdpr_n;
   _hdr = new nidas_hdr[trcd];
   _twod_rec = new usb2d_rec[trcd];
   int c = 0;

   //get the last few records
   while (!feof(_fp)){
     nidas_hdr	h;
     usb2d_rec d;

     fread(&h, sizeof(nidas_hdr), 1, _fp);
     if (h.length==_byte_usb2d_rcd)
     {
       fread(&d, h.length, 1, _fp);
       _hdr[c]=h;
       _twod_rec[c]=d;
     //  printf("-----%llu %lu %lu %lx %lx\n", h.timetag, h.length,
	//	h.sample_id, d.tas, d.id);
       c++;
       if (c>=trcd) {c=0;}

     } else {
       unsigned char dd[h.length];
       fread(&dd, h.length, 1, _fp);
     }
   } 
  
   //put cycled buff back to the order
   nidas_hdr	hdr[trcd];
   usb2d_rec	twod_rec[trcd];
  
   if ((c> 0)&& (_hdr[c].length>0)) {
     for (int i =0; i<trcd; i++) {
       hdr[i]= _hdr[c];
       twod_rec[i]= _twod_rec[c];
       c++;
       if (c>=trcd) {c=0;}
     }
     //back to _hdr and _twod_rec
     for (int i =0; i<trcd; i++) {
       _hdr[i]= hdr[i];
       _twod_rec[i]= twod_rec[i];
     }
   } 

   //print out data
   for (int i=0; i<trcd; i++) {
     printf("%llu %lu %lu %lx %lx\n", _hdr[i].timetag, _hdr[i].length,
		_hdr[i].sample_id, _twod_rec[i].tas, _twod_rec[i].id);
   }
}


/* -------------------------------------------------------------------- */
void DataMng::_init( short b)
{
  _bit_n 	= b;
  if (_bit_n==64) {
    _byte_n= _bit_n/8; 
    _row_n= 8;
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
 
}

/* -------------------------------------------------------------------- */
bool DataMng::_chkInit() {
  if (_bit_n<=0) {std::cout << "Initialize class DataMng is required! \n"<< std::endl; return false;}
  return true;
}
