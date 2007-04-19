#include "DataUsb2d64.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <qgarray.h>


//const unsigned long long DataMng::_syncWord = 0xAAAAAAA000000000LL;
//const unsigned long long DataMng::_syncMask = 0xFFFFFFF000000000LL;


/* -------------------------------------------------------------------- */
DataUsb2d64::DataUsb2d64 (FILE * fp) :DataMng(fp, 64)
{
  SetBt(64);
  Setfp(fp);
  Init();
}

/* -------------------------------------------------------------------- */
QPointArray* DataUsb2d64::GetPoints()
{ 
  if (!_chkInit()) {return NULL;}
  _pts= new QPointArray(TBYTE*8*_row_n*_rcdpr_n); 

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
   
  _pts->resize(_cnt );
  return _pts;
}

/* -------------------------------------------------------------------- */
void DataUsb2d64::_getRecord(int start_x, int start_y, unsigned char * data_p)
{
  for (int i = 0; i < _slide_n; ++i)
  {
    //get  and flip slide
    unsigned char s1[_byte_n];//_byte_n=8
    for (int n=0; n<_byte_n; ++n) {
      s1[7-n]=*data_p; data_p++;
    }

    unsigned long long* s2 = (unsigned long long*)s1;
    unsigned long long slide = *s2;
    //std::cout<<"\nlonglongslide: "<<slide<<std::endl<<std::endl;
    slide = ~slide;
//    if ((slice & _syncMask) == _syncWord)
//      printf("%llu\n", slice & ~_syncMask);	// Print timing word.
  
    //check every bit
    unsigned long long  mask =1;
    for (int b=0; b<_bit_n; ++b) { //_bit_n=64
      if (mask & slide) {
        _pts->setPoint(_cnt, start_x + i, start_y + _bit_n-b);
        ++_cnt;
      }
      mask*=2;
    }//b
  }//i
}


/* -------------------------------------------------------------------- */
void DataUsb2d64::_get2drec() {
   int trcd = _row_n*_rcdpr_n;
   _hdr = new nidas_hdr[trcd];
   _twod_rec = new usb2d_rec[trcd];
 
   fsetpos(_fp, &_pp);
   fpos_t tmp=_pp;
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
       //	h.sample_id, d.tas, d.id);p
       c++;
       if (c>=trcd) { 
         c=0;
         _pp=tmp;
         fgetpos(_fp, &tmp);
       }//if

     } else {
       //fseek (_fp, h.length, SEEK_CUR );
       unsigned char dd[h.length];
       fread(&dd, h.length, 1, _fp);
     }
   } //while
  
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

