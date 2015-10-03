// reading an entire binary file
#include "rcf.h"

using namespace std;

int main () {

  My_Rcf_Hdr_Un thisRcfHdr;
  My_RC_FL_Un thisRcFl;
  My_RC_FL_Un thisRcFl2;

  streampos size;

  ifstream file ("RCF/NRCKA189.RCF", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    //size = file.tellg();
    size = sizeof(RCF_HDR);
    file.seekg (0, ios::beg);
    file.read (thisRcfHdr.Array, size);
    file.seekg ((-2*sizeof(float)),ios::cur);
    file.read (thisRcFl.Array, sizeof(RC_FL_Read));
    file.read (thisRcFl2.Array, sizeof(RC_FL_Read));

    if (file)
      cout<< "all: "<<size<<" bytes were read.\n";
    else
      cout << "error: only " <<file.gcount()<<" bytes were read.\n";

    file.close();

    char out;
    int j = 0;
    for (int i=0; i<size; i++) {
      if (j < 9) {
        cout << i << ":" << (char) thisRcfHdr.Array[i];
      } else {
        cout << i << ":" << (char) thisRcfHdr.Array[i];
        cout << '\n';
        j = 0;
      }
    }
      
    cout << "int size: "<<sizeof(int)<<"  short: "<<sizeof(short)<<"  long: "<<sizeof(long)<<"  float:"<< sizeof(float)<<"\n";
    cout<< "size:"<<size<<"\n";
  
    cout << "\n";
    cout << "Format    :"<<thisRcfHdr.Rcf_Hdr.RCformat << '\n';
    thisRcfHdr.Rcf_Hdr.RAOBfilename[79] = '\n';
    cout << thisRcfHdr.Rcf_Hdr.RAOBfilename << '\n';
    thisRcfHdr.Rcf_Hdr.RCfilename[79] = '\n';
    cout << thisRcfHdr.Rcf_Hdr.RCfilename << '\n';
    cout << "Raobcount :" << thisRcfHdr.Rcf_Hdr.RAOBcount << '\n';
    cout << "LR1       :"<<thisRcfHdr.Rcf_Hdr.LR1<<'\n';
    cout << "zLRb      :"<<thisRcfHdr.Rcf_Hdr.zLRb<<'\n';
    cout << "LR2       :"<<thisRcfHdr.Rcf_Hdr.LR2<<'\n';
    cout << "RecordStep:"<<thisRcfHdr.Rcf_Hdr.LR2<<'\n';
    cout << "RAOBmin   :"<<thisRcfHdr.Rcf_Hdr.RAOBmin<<'\n';
    cout << "ExcessTamplitude:"<<thisRcfHdr.Rcf_Hdr.ExcessTamplitude<<'\n';
    cout << "Nobs:"<<thisRcfHdr.Rcf_Hdr.Nobs<<'\n';
    cout << "Nret:"<<thisRcfHdr.Rcf_Hdr.Nret<<'\n';
    cout << "dZ:\n";
    for (int i=0; i<32; i++) {cout << thisRcfHdr.Rcf_Hdr.dZ[i] <<", ";}
    cout << thisRcfHdr.Rcf_Hdr.dZ[32]<<'\n';
    cout << "NFL:"<<thisRcfHdr.Rcf_Hdr.NFL<<'\n';
    cout << "Zr:\n";
    for (int i=0; i<19; i++) {cout << thisRcfHdr.Rcf_Hdr.Zr[i] <<", ";}
    cout << thisRcfHdr.Rcf_Hdr.Zr[19]<<'\n';
    for (int i=0; i<20; i++) { 
      swap_endian(reinterpret_cast<byte*>(&thisRcfHdr.Rcf_Hdr.Zr[i]), sizeof(float), ENDIAN_BIG, ENDIAN_LITTLE); 
      cout << thisRcfHdr.Rcf_Hdr.Zr[i] << ", ";
    }
    cout << "\n";
    cout << "Nlo:"<<thisRcfHdr.Rcf_Hdr.Nlo<<'\n';
    cout << "SURC:"<<thisRcfHdr.Rcf_Hdr.SURC<<'\n';

    My_EH thisEH;
    int i = 0;
    for (int j=1376; j<sizeof(END_HDR); j++,i++) thisEH.Array[i]=thisRcfHdr.Array[j];
    cout << "SURC:" << thisEH.EH.SURC << '\n';
    cout << "CHnLSBloss:\n";
    for (int i=0; i<3;i++) {cout<< thisEH.EH.CHnLSBloss[i]<<',';}
    cout << '\n';
    j = 0;
/*
    for (int i=0; i<900;i++) {
      if (j<10) { cout<<i<<":"<<thisEH.EH.Gmatrix[i]<<" ";j++;}
      else { cout<<i<<":"<<thisEH.EH.Gmatrix[i]<<"\n";j=0;}
    }
*/

    //Convert from row major storage to column major storage for SmatrixN1
    float SmatrixN1[15][3][10];
    int x=0,y=0,z=0;
    for (int i=0; i<450;i++) {
      SmatrixN1[x][y][z] = thisEH.EH.Gmatrix[i];
      //cout << x << " " << y << " " << z << '\n';
      x++;
      if (x > 14) { 
        x=0;
        y++; 
        if (y>2) {
          y=0;
          z++;
        }
      }
    }
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN1:"<<SmatrixN1[14][2][i]<<'\n';
//      cout << "    SMATrIXN2:"<<thisEH.EH.SmatrixN2[14][2][i]<<'\n';
    }
    cout << "SmatrixN1[5][1][5]:"<<SmatrixN1[5][1][5]<<'\n';
    for (int i = 450; i<460;i++) {cout<<" GM["<<i<<"]:"<<thisEH.EH.Gmatrix[i];}
    cout << "\n";
    //Convert from row major storage to column major storage for SmatrixN2
cout<<"***********************************\n";

    float SmatrixN2[15][3][10];
    x=0,y=0,z=0,j=0;
    for (int i=450; i<900;i++) {
      SmatrixN2[x][y][z] = thisEH.EH.Gmatrix[i];
      if (j<10) cout<<i<<':'<<thisEH.EH.Gmatrix[i]<<" ";
      else {j=0; cout<<i<<':'<<thisEH.EH.Gmatrix[i]<<" \n";}
      x++;
      if (x > 14) { 
        x=0;
        y++; 
        if (y>2) {
          y=0;
          z++;
        }
      }
    }
cout << "************************************\n";
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN2:"<<SmatrixN2[0][0][i]<<'\n';
      cout << "    SMATrIXN2:"<<SmatrixN2[14][2][i]<<'\n';
    }
    cout << "SmatrixN2[5][1][5]:"<<SmatrixN2[5][1][5]<<'\n';
    //char SURC[4]; 
    //for (int j=1376; j<1380; j++,i++) SURC[i]=thisRcfHdr.Array[j];
    //cout << "NewSURC:"<<SURC<<"\n";
/*
    cout << "IFoff:\n";
    for (int i=0; i<3; i++) {
      for (int j=0; j<16; j++) {
        cout << thisRcfHdr.Rcf_Hdr.IFoff[i][j] <<', ';
      }
      cout << '\n';
    }
    cout << "Nif:"<<thisRcfHdr.Rcf_Hdr.Nif<<'\n';
    for (int i=0; i<10; i++) { 
      for (int j=0; j<3; j++) { 
        cout << thisRcfHdr.Rcf_Hdr.SmatrixN2[1][j][i] << ', ';
      } 
      cout <<'\n';
    }
*/

    //cout << "the entire file content is in memory";
    //
    cout << "sBP:" << thisRcFl.RC_read.sBP<<'\n';
    for (j=0; j<30; j++) cout << "SOBrms["<<j<<"]:"<<thisRcFl.RC_read.sOBrms[j];
    cout << '\n';
    for (j=0; j<30; j++) cout << "SOBav["<<j<<"]:"<<thisRcFl.RC_read.sOBav[j];
    cout << '\n';
    for (j=0; j<990; j++) cout << "Src["<<j<<"]:"<<thisRcFl.RC_read.Src[j];
    cout << '\n';

    cout << "__((___(((_)(()________\n\n";
    cout << "sBP:" << thisRcFl2.RC_read.sBP<<'\n';
    for (j=0; j<30; j++) cout << "SOBrms["<<j<<"]:"<<thisRcFl2.RC_read.sOBrms[j];
    cout << '\n';
    for (j=0; j<30; j++) cout << "SOBav["<<j<<"]:"<<thisRcFl2.RC_read.sOBav[j];
    cout << '\n';
    for (j=0; j<990; j++) cout << "Src["<<j<<"]:"<<thisRcFl2.RC_read.Src[j];
    cout << '\n';
    
  }
  else cout << "Unable to open file";
  return 0;
}
