/*
 ******************************************************************
     
    Fast 2DC processing package, NCAR/RAF

    COMPILER COMMAND:
    c++ procf2dc.cpp -o procf2dc -I/usr/local/netcdf-3.6.0-p1/include 
        -L/usr/local/netcdf-3.6.0-p1/lib -lnetcdf_c++ -lnetcdf

    Last changed: ARB, 6/2010

 ******************************************************************
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <unistd.h>
#include <arpa/inet.h>

#include <raf/PMSspex.h>

#include "config.h"
#include "probe.h"
#include "ProbeData.h"
#include "netcdf.h"

using namespace std;
   
const int nDiodes = 64;
const int numbins = 128;
const int binoffset = 1;  // Offset for RAF conventions, number of empty bins before counting begins 
const int slicesPerRecord = 512;
const string markerline = "</OAP>";  // Marks end of XML header


struct struct_particle {
   long time1hz; 
   double inttime; 
   float size, csize, xsize, ysize, area, holearea, circlearea; 
   bool allin, wreject, ireject;
};

// Standard RAF record format for 2D records.
typedef struct type_buffer {
    char probetype;
    char probenumber;
    short hour;
    short minute;
    short second;
    short year;
    short month;
    short day;
    short tas;                  // True airspeed.
    unsigned short msec;        // millisecond of data timestamp.
    short overload;
    unsigned char image[4096];
} P2d_rec;



//--------Find index for maximum element of an array---------
int maxindex(float x[],int n)
{
  int ixmax=0;
  double xmax=0;
  for (int i=0; i<n; i++){
    if (x[i]>xmax){
      xmax=x[i];
      ixmax=i;
    }
  }
  return ixmax;
}


// ----------------MATRIX INVERSION ROUTINE----------------
double invert3(double m[3][3], double n[3][3]) {
   //Invert a 3x3 matrix.  m is original matrix,
   //n is inverted matrix, det is determinant.  Inverse
   //is undefined when det is zero.
   double det;
   det=m[0][0]*m[1][1]*m[2][2] +
       m[1][0]*m[2][1]*m[0][2] +
       m[2][0]*m[0][1]*m[1][2] -
       m[0][0]*m[2][1]*m[1][2] -
       m[1][0]*m[0][1]*m[2][2] -
       m[2][0]*m[1][1]*m[0][2];
   if (det==0) return det;  //Can't invert
   n[0][0]=(m[1][1]*m[2][2] - m[1][2]*m[2][1])/det;
   n[0][1]=(m[0][2]*m[2][1] - m[2][2]*m[0][1])/det;
   n[0][2]=(m[0][1]*m[1][2] - m[1][1]*m[0][2])/det;
   n[1][0]=(m[1][2]*m[2][0] - m[2][2]*m[1][0])/det;
   n[1][1]=(m[0][0]*m[2][2] - m[2][0]*m[0][2])/det;
   n[1][2]=(m[0][2]*m[1][0] - m[1][2]*m[0][0])/det;
   n[2][0]=(m[1][0]*m[2][1] - m[2][0]*m[1][1])/det;
   n[2][1]=(m[0][1]*m[2][0] - m[2][1]*m[0][0])/det;
   n[2][2]=(m[0][0]*m[1][1] - m[1][0]*m[0][1])/det;
   return det;
}


// ----------------DOUBLE POISSON FIT ROUTINE----------------
double dpoisson_fit(float x[], float y[], double a[3], int n){
   //update the "a" fit matrix for a double-poisson fit.
   //Sum of squares of residual values is returned.
   //Uses the Gauss-Newton nonlinear least squares regression method.
   //See Numerical Methods for Engineers, Chapra & Canale 1998 p.468
   
   double kc, f[n], diff[n], J[3][n];  //The function, difference, and Jacobian  
   kc=log10(exp(1));                   //for normalization to 1
   double ysum=0, olda1;
   float damp=0.2;                     //Damping factor to prevent runaways
   int iteration=0, miniterations=5, maxiterations=50;
   float percentchange=100.0;

   //for (int i=0; i<n; i++) cout<< y[i]<<", ";
   //cout<<endl;

   //Normalize to 1
   for (int i=0; i<n; i++) ysum += y[i];
   for (int i=0; i<n; i++) y[i] /= ysum;
   
   //Don't try with low counts
   if (ysum < 10) return -1;
   
   //Find first guess
   int imax=maxindex(y,n);
   a[0]=0.8;
   a[1]=1.0/x[imax];
   a[2]=1.0e6;//1.0/(x[imax]/1.0e3);

   //Iterate function from min to max iteration count, stopping if change is under 1%
   while (((iteration < maxiterations) && (percentchange > 1.0)) || (iteration < miniterations)){
      iteration++;
      //Compute f, difference, and Jacobian for the given "a"
      for (int i=0; i<n; i++){
         f[i]=a[0]*a[1]*x[i]*exp(-a[1]*x[i])*kc+(1.0-a[0])*a[2]*x[i]*exp(-a[2]*x[i])*kc;
         diff[i]=y[i]-f[i];
         J[0][i]=a[1]*x[i]*exp(-a[1]*x[i])*kc-a[2]*x[i]*exp(-a[2]*x[i])*kc;
         J[1][i]=a[0]*x[i]*exp(-a[1]*x[i])*kc-a[0]*a[1]*x[i]*x[i]*exp(-a[1]*x[i])*kc;
         J[2][i]=(1.0-a[0])*x[i]*exp(-a[2]*x[i])*kc-(1.0-a[0])*a[2]*x[i]*x[i]*exp(-a[2]*x[i])*kc; 
      }
      //Multiply Jacobian by its transpose and invert
      double det, JJt[3][3]={{0}}, JJti[3][3]={{0}};
      for (int i=0; i<3; i++){
         for (int j=0; j<3; j++){
            for (int k=0; k<n; k++){
               JJt[i][j]=JJt[i][j]+J[i][k]*J[j][k];
            }
         }
      }
   
      det=invert3(JJt, JJti);
   
      //Multiply transposed Jacobian by Diff
      double JtDiff[3]={0};
      for (int i=0; i<3; i++){
         for (int j=0; j<n; j++){
            JtDiff[i]=JtDiff[i]+diff[j]*J[i][j];
         }
      }
    
      //Compute deltaA and update
      double deltaA[3]={0};
      for (int i=0; i<3; i++){
         for (int j=0; j<3; j++){
            deltaA[i]=deltaA[i] + JtDiff[j] * JJti[i][j];
         }
      } 
     
      olda1=a[1];
      a[0]=a[0]+damp*deltaA[0];
      a[1]=a[1]+damp*deltaA[1];
      a[2]=a[2]+damp*deltaA[2];
      percentchange=100*(abs(a[1])-olda1)/olda1; 
   }
   
   //Return square of difference
   double diff2=0;
   for (int i=0; i<n; i++){
      f[i]=a[0]*a[1]*x[i]*exp(-a[1]*x[i])*kc+(1.0-a[0])*a[2]*x[i]*exp(-a[2]*x[i])*kc;
      diff2=diff2+pow(y[i]-f[i],2);
   }
   //cout << "A= " << a[0] <<" "<<a[1]<<" "<<a[2]<<" X2= "<<diff2<<endl;
   //cout << "Iterations= " <<iteration<<endl;
   return diff2;
} 


// ----------------CIRCLE SIZE ROUTINE----------------
struct_particle findsize(short img[][nDiodes], short nslices, float res){
   // Based on http://tog.acm.org/resources/GraphicsGems/gems/BoundSphere.c 
   // Graphics Gems I, Article by Ritter

   double dx,dy,rad,rad_sq,xspan,yspan,maxspan;
   double old_to_p,old_to_p_sq,old_to_new;
   struct Point2Struct {double x, y;} xmin,xmax,ymin,ymax,dia1,dia2,cen;
   short foreval=0;  //values that indicate background and foreground 
   vector<short> x,y;
   struct_particle particle;
   bool allin=1;
   float area=0, theta, phi;
   int mindiode=nDiodes, maxdiode=0, minslice=nslices, maxslice=0;
   
   //Stuff x and y vectors, find x and y size, area, check for edge touching
   //May want to erode image to outline to increase performance.
   for (int i=0; i<nslices; i++) for(int j=0; j<nDiodes; j++) {
      if(img[i][j]==foreval) {
         y.push_back(i); 
         x.push_back(j);
         if((j==0) || (j==nDiodes-1)) allin=0;
         area++;
         if(j<mindiode) mindiode=j;
         if(j>maxdiode) maxdiode=j;
         if(i<minslice) minslice=i;
         if(i>maxslice) maxslice=i;         
      }
   }

   particle.allin=allin;
   particle.xsize=(maxdiode-mindiode+1)*res;
   particle.ysize=(maxslice-minslice+1)*res;
   particle.area=area;
   
   //Check for empty roi
   if (x.size()==0) {
      particle.csize=0.0; particle.xsize=0.0; particle.ysize=0; 
      return particle;
   }

   //FIRST PASS: find 6 minima/maxima points 
   xmin.x=ymin.y= 1000; // initialize for min/max compare 
   xmax.x=ymax.y= -1000;
   for (size_t i=0;i<x.size();i++){
      if (x[i]<xmin.x) {xmin.x=x[i]; xmin.y=y[i];} // New xminimum point 
      if (x[i]>xmax.x) {xmax.x=x[i]; xmax.y=y[i];}
      if (y[i]<ymin.y) {ymin.x=x[i]; ymin.y=y[i];}
      if (y[i]>ymax.y) {ymax.x=x[i]; ymax.y=y[i];}
   }

   // Set xspan = distance between the 2 points xmin & xmax (squared) 
   dx = xmax.x - xmin.x;
   dy = xmax.y - xmin.y;
   xspan = dx*dx + dy*dy ;

   // Same for y span 
   dx = ymax.x - ymin.x;
   dy = ymax.y - ymin.y;
   yspan = dx*dx + dy*dy ;

   // Set points dia1 & dia2 to the maximally separated pair 
   dia1 = xmin; dia2 = xmax; // assume xspan biggest 
   maxspan = xspan;
   if (yspan>maxspan){
      maxspan = yspan;
      dia1 = ymin; dia2 = ymax;
   }

   // dia1,dia2 is a diameter of initial sphere 
   // calc initial center 
   cen.x = (dia1.x+dia2.x)/2.0;
   cen.y = (dia1.y+dia2.y)/2.0;
   // calculate initial radius**2 and radius 
   dx = dia2.x-cen.x; // x component of radius vector 
   dy = dia2.y-cen.y; // y component of radius vector 
   rad_sq = dx*dx + dy*dy ;
   rad = sqrt(rad_sq);

   // SECOND PASS: increment current sphere 
   for (size_t i=0;i<x.size();i++){
      dx = x[i]-cen.x;
      dy = y[i]-cen.y;
      old_to_p_sq = dx*dx + dy*dy;
      if (old_to_p_sq > rad_sq) 	// do r**2 test first 
         { 	// this point is outside of current sphere 
         old_to_p = sqrt(old_to_p_sq);
         // calc radius of new sphere 
         rad = (rad + old_to_p) / 2.0;
         rad_sq = rad*rad; 	// for next r**2 compare 
         old_to_new = old_to_p - rad;
         // calc center of new sphere 
         cen.x = (rad*cen.x + old_to_new*x[i]) / old_to_p;
         cen.y = (rad*cen.y + old_to_new*y[i]) / old_to_p;
      }
   }
   rad=rad+0.5;            //adjust to true size for 50% shadow 
   
   //Find area of enclosing circle (in the array)
   theta=acos(min((cen.x/rad),1.0));            //angle
   phi=acos(min((nDiodes-1-cen.x)/rad,1.0));
   // find area= triangles(left) + triangles(right) + (remaining wedges)
   particle.circlearea=(cen.x*rad*sin(theta) + (nDiodes-1-cen.x)*rad*sin(phi) + 
                       3.14*rad*rad*((3.14-phi-theta)/3.14));
   particle.csize=(rad*2.0)*res; 

   return particle;
}



// ----------------HOLE FILL ROUTINE----------------
short fillholes2(short img_original[][nDiodes], short nslices){
  short img[nslices][nDiodes]; //create a new image for processing 
  short backval=1, foreval=0;  //values that indicate background and foreground 
  short label=1, area_added=0;
  stack<int> sx, sy;
  int itest[4], jtest[4];
  bool edgetouch;
  
  //Make blank image
  memset(img, 0, sizeof(img));
  
  //Check pixels for background values (to be filled)
  for (int i = 0; i < nslices; i++) {
     for (int j = 0; j < nDiodes; j++) {
        if ((img_original[i][j]==backval) && (img[i][j]==0)) {
           sx.push(i);
           sy.push(j);
           img[i][j]=label;
           while (!sx.empty()) {
              //Define neighborhood to test
              itest[0]=max(sx.top()-1,0);         jtest[0]=sy.top();
              itest[1]=min(sx.top()+1,nslices-1); jtest[1]=sy.top();
              itest[2]=sx.top();                  jtest[2]=max(sy.top()-1,0);
              itest[3]=sx.top();                  jtest[3]=min(sy.top()+1,nDiodes-1);
              sx.pop();  //Element has been used, erase
              sy.pop();
              for (int k=0; k<4; k++){
                 //Check neighborhood for unmarked pixels, label and add them to stack
                 if ((img_original[itest[k]][jtest[k]]==backval) && (img[itest[k]][jtest[k]]==0)){
                    sx.push(itest[k]);
                    sy.push(jtest[k]);
                    img[itest[k]][jtest[k]]=label;
                 }
              }
           }
           label++;
        }
     }
  }
 
  for (short ic=1; ic<label; ic++){      //Run through all possible values 1:c
     //Check edges for this value
     edgetouch=0;
     for (int i=0; i<nslices; i++) if (img[i][0]==ic) edgetouch=1;
     if (edgetouch==0) for (int i=0; i<nslices; i++) if (img[i][nDiodes-1]==ic) edgetouch=1;
     if (edgetouch==0) for (int j=0; j<nDiodes; j++) if (img[0][j]==ic) edgetouch=1;
     if (edgetouch==0) for (int j=0; j<nDiodes; j++) if (img[nslices-1][j]==ic) edgetouch=1;
     if (edgetouch==0){
        for (int i=0; i<nslices; i++) for (int j=0; j<nDiodes; j++) {
           if (img[i][j]==ic) {img_original[i][j]=foreval; area_added++;}
        }
     }
  }  
  return area_added;

}  

// ----------------POISSON SPOT CORRECTION FOR WATER----------------
float poisson_spot_correction(float area_img, float area_hole, bool allin){
   //Based on Korolev JTECH #24 2007 p. 376
   float Dspot_Dedge[]={0.003,0.008,0.017,0.024,0.033,0.04,0.047,0.054,0.062,0.072,0.076,0.088,0.093,0.096,
      0.101,0.119,0.123,0.127,0.13,0.134,0.139,0.148,0.175,0.18,0.184,0.188,0.192,0.195,0.199,0.202,0.206,0.209, 
      0.213,0.217,0.221,0.225,0.23,0.235,0.243,0.327,0.334,0.34,0.345,0.351,0.355,0.36,0.365,0.369,
      0.373,0.377,0.381,0.385,0.389,0.393,0.397,0.4,0.404,0.408,0.411,0.415,0.419,0.422,0.426,0.429, 
      0.433,0.436,0.439,0.443,0.446,0.45,0.453,0.457,0.46,0.463,0.467,0.47,0.473,0.477,0.48,0.484,0.487,
      0.49,0.494,0.497,0.501,0.504,0.507,0.511,0.514,0.518,0.521,0.525,0.528,0.532,0.535,0.539, 
      0.543,0.547,0.55,0.554,0.558,0.562,0.566,0.569,0.572,0.575,0.578,0.581,0.584,0.587,0.59,0.593,
      0.596,0.598,0.601,0.605,0.61,0.614,0.618,0.623,0.627,0.631,0.635,0.64,0.644,0.648,0.653,0.657, 
      0.662,0.666,0.671,0.676,0.68,0.685,0.69,0.695,0.7,0.705,0.711,0.716,0.721,0.727,0.733,0.738,
      0.744,0.751,0.757,0.763,0.77,0.777,0.784,0.792,0.8,0.808,0.817,0.826,0.836,0.846,0.858,0.87, 
      0.884,0.901,0.921,0.95};
  
   float Dedge_D0[]={1.0,1.054,1.083,1.101,1.095,1.11,1.148,1.162,1.155,1.123,1.182,1.121,1.162,1.21,1.242,
      1.134,1.166,1.202,1.238,1.27,1.294,1.278,1.13,1.148,1.17,1.194,1.218,1.242,1.265,1.288,1.31,1.331,1.351, 
      1.369,1.386,1.4,1.411,1.416,1.407,1.074,1.08,1.087,1.096,1.106,1.117,1.127,1.139,1.15,1.162,1.173,
      1.185,1.197,1.208,1.22,1.232,1.243,1.255,1.266,1.277,1.289,1.3,1.311,1.322,1.333,1.344,1.355, 
      1.366,1.376,1.387,1.397,1.407,1.418,1.428,1.438,1.448,1.458,1.467,1.477,1.486,1.496,1.505,1.515,
      1.524,1.533,1.542,1.551,1.559,1.568,1.577,1.585,1.594,1.602,1.61,1.618,1.626,1.634,1.642,1.65, 
      1.657,1.665,1.673,1.68,1.687,1.694,1.702,1.709,1.716,1.722,1.729,1.736,1.742,1.749,1.755,1.761,
      1.768,1.774,1.78,1.786,1.791,1.797,1.803,1.808,1.813,1.819,1.824,1.829,1.834,1.839,1.843,1.848, 
      1.852,1.857,1.861,1.865,1.869,1.872,1.876,1.88,1.883,1.886,1.889,1.892,1.895,1.897,1.899,1.901,
      1.903,1.905,1.906,1.907,1.908,1.908,1.908,1.908,1.907,1.905,1.903,1.9,1.897,1.892,1.885,1.877,1.865,1.845};
/*
   float Zd[]={0.0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,
      1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2.0,2.05,2.1,2.15,2.2, 
      2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3.0,3.05,3.1,3.15,3.2,3.25,3.3,
      3.35,3.4,3.45,3.5,3.55,3.6,3.65,3.7,3.75,3.8,3.85,3.9,3.95,4.0,4.05,4.1,4.15,4.2,4.25,4.3,4.35,4.4, 
      4.45,4.5,4.55,4.6,4.65,4.7,4.75,4.8,4.85,4.9,4.95,5.0,5.05,5.1,5.15,5.2,5.25,5.3,5.35,5.4,5.45,5.5,
      5.55,5.6,5.65,5.7,5.75,5.8,5.85,5.9,5.95,6.0,6.05,6.1,6.15,6.2,6.25,6.3,6.35,6.4,6.45,6.5,6.55,6.6, 
      6.65,6.7,6.75,6.8,6.85,6.9,6.95,7.0,7.05,7.1,7.15,7.2,7.25,7.3,7.35,7.4,7.45,7.5,7.55,7.6,7.65,7.7,
      7.75,7.8,7.85,7.9,7.95,8.0,8.05,8.1,8.15};
*/
   float ratio, correction=1;
   
   if((area_img>0) && (area_hole>0) && (allin==1)){
     //Use the area option in paper: sqrt(Sspot/Sedge)
     ratio = sqrt(area_hole/(area_img+area_hole));
     int ip=1;
     while(ratio > Dspot_Dedge[ip]) ip++;
	  //cout<<Zd[ip-1]<<endl;
     correction = Dedge_D0[ip-1];
   }
   return correction;
}

// ----------------PARTICLE REJECTION ---------------------------
void reject_particle(struct_particle& x, float cutoff, float nextinttime, float pixel_res, 
                    float smallbin, float largebin, float wc, bool recon){
   //Decides on the rejection of a particle.
   //Ice rejects will return value of 2 or higher
   //Water rejects will return value of 1 or higher                 
   float ar;
   ar=(x.area+x.holearea)/x.circlearea;
   x.wreject=0; x.ireject=0;

   //Any conditions
   if ((x.inttime < cutoff) || (nextinttime < cutoff)) {x.wreject=1; x.ireject=1;}
   if (ar < 0.1) {x.wreject=1; x.ireject=1;}
   if ((recon == 0) && (x.allin == 0)) {x.wreject=1; x.ireject=1;}
  
   //Water conditions
   if ((ar < 0.4) || ((ar < 0.5) && (x.size > pixel_res*10.0))) x.wreject=1;
   if (x.size > 6000) x.wreject=1;
   if ((x.size/wc < smallbin) or (x.size/wc > largebin)) x.wreject=1;

   //Ice conditions
   if ((x.size < smallbin) or (x.size > largebin)) x.ireject=1;
}

// ----------------TIME CONVERSION ---------------------------
int hms2sfm(int hms){
   //Convert hhmmss to seconds from midnight
   int hour = hms / 10000;
   int minute = (hms-hour*10000) / 100;
   int second = hms - hour*10000 - minute*100;
   int sfm = hour*3600 + minute*60 + second;      
   return sfm;
}

time_t TwoDtime(const P2d_rec *rec)
{
  struct tm tm;

  tm.tm_mday = ntohs(rec->day);
  tm.tm_mon = ntohs(rec->month) - 1;
  tm.tm_year = ntohs(rec->year) - 1900;
  tm.tm_hour = ntohs(rec->hour);
  tm.tm_min = ntohs(rec->minute);
  tm.tm_sec = ntohs(rec->second);

  return mktime(&tm);
}


//----------------Display particle properties to screen--------
void showparticle(struct_particle& x){
   float ar;
   ar=(x.holearea+x.area)/x.circlearea;
   cout<<setprecision(2);
   cout<<x.time1hz<<" "<<scientific<<x.inttime;
   cout<<fixed;
   cout<<" C="<<setw(8)<<x.csize<<
         " X="<<setw(8)<<x.xsize<<
         " Y="<<setw(8)<<x.ysize<<
         " AR="<<setw(8)<<ar<<
    //     " A="<<setw(10)<<x.area<<
    //     " HA="<<setw(10)<<x.holearea<<
    //     " CA="<<setw(10)<<x.circlearea<<
    //     " Int="<<x.inttime<<
         " AI="<<x.allin<<
         " IR="<<x.ireject<<
         " WR="<<x.wreject<<
         endl;
} 

//----------------Display particle image to screen--------
void showroi(short img[][nDiodes], short nslices)
{
  for (int i = 0; i < nslices;i++){
    for (int j = 0; j < nDiodes; j++) cout<<img[i][j]; 
    cout<<endl;
  }
  cout<<endl;
}

// ----------------A FEW BYTE SWAPPING ROUTINES----------------
unsigned long long endianswap_ull(unsigned long long x)
{
   x = (x>>56) | 
        ((x<<40) & 0x00FF000000000000ULL) |
        ((x<<24) & 0x0000FF0000000000ULL) |
        ((x<<8)  & 0x000000FF00000000ULL) |
        ((x>>8)  & 0x00000000FF000000ULL) |
        ((x>>24) & 0x0000000000FF0000ULL) |
        ((x>>40) & 0x000000000000FF00ULL) |
        (x<<56);
        return x;
}


//================================================================================================
// ------------PROCESS 2D-----------------------
//================================================================================================
int process2d(Config & cfg, netCDF & ncfile, ProbeInfo & probe)
{
  /*-----Processing options-------------------------------------------------------
      start/stop time: In UTC seconds
      probe2process:   Indicates which probenumber to process C4, C6, etc.
      pixel_res:       Probe resolution in microns
      armwidth:        Arm separation in cm
      recon:           Turn on/off particle reconstruction
  */

  //-----------Various declarations-----------------------------------------------
  type_buffer buffer;  
  ifstream input_file;
  string line;
  short roi[slicesPerRecord][nDiodes];
  short slice_count=0, firstday;
  unsigned long long slice, firsttimeline, lasttimeline=0, timeline, difftimeline, particle_count=0;
  double lastbuffertime, buffertime = 0, nextit = 0;
  bool firsttimeflag = true;
  float wc;
  long last_time1hz=0, itime, isize, wsize, iit;
  struct_particle particle;
  vector<struct_particle> particle_stack;
  char probetype = probe.id[0];  
  char probenumber = probe.id[1];

  //Set up array of powers of 2, starting with 1
  unsigned long long powerof2[64];
  for (int i=0; i<64; i++) {powerof2[i]=1ULL; for (int j=0; j<i;j++) powerof2[i]=powerof2[i]*2ULL;} 

  probe.ComputeSamplearea(cfg.recon);

  //Time setup
  int numtimes = cfg.stoptime - cfg.starttime + 1;

  assert(numtimes >= 0);

  //Count and concentration arrays setup
  float *count_all[numtimes], *conc_all[numtimes];
  float *count_round[numtimes], *conc_round[numtimes];

  // Allocate contiguos data block.
  count_all[0] = new float[numtimes*(probe.numBins+binoffset)];
  count_round[0] = new float[numtimes*(probe.numBins+binoffset)];
  conc_all[0] = new float[numtimes*(probe.numBins+binoffset)];
  conc_round[0] = new float[numtimes*(probe.numBins+binoffset)];

  //Initialize all these to zero
  for (int i = 0; i < numtimes; i++) {
    if (i > 0) {	// Set up pointers into contiguous data block.
      int offset = i * (probe.numBins+binoffset);

      count_all[i] = count_all[0] + offset;
      count_round[i] = count_round[0] + offset;
      conc_all[i] = conc_all[0] + offset;
      conc_round[i] = conc_round[0] + offset;
    }

    for (int j = 0; j < (probe.numBins+binoffset); j++) {
      count_all[i][j]=0; conc_all[i][j]=0; 
      count_round[i][j]=0; conc_round[i][j]=0;
    }
  }

  ProbeData data(numtimes);

  //Shattering correction and interarrival setup
  const int nitq=400; //number of interarrival times to keep for fitting
  int iitq=0;   //current index of itq
  double bestfit[3]={0};
  double itq[nitq]={1};
  float it_endpoints[cfg.nInterarrivalBins+1], it_midpoints[cfg.nInterarrivalBins], fitspec[cfg.nInterarrivalBins];
  int count_it[numtimes][cfg.nInterarrivalBins+binoffset];
  for (int i=0; i <= cfg.nInterarrivalBins; i++) it_endpoints[i] = pow(10, ((float)i-35)/5.0);
  for (int i=0; i < cfg.nInterarrivalBins; i++) it_midpoints[i] = pow(10, ((float)i-34.5)/5.0);

  if (ncfile.hasTASX())
    ncfile.readTrueAirspeed(&data.tas[0], numtimes);


  //=============Process Particles============================================================
  
  input_file.open(cfg.inputFile.c_str(), ios::binary);
 
  //Skip the XML header
  do getline(input_file, line); while (line.compare(markerline)!=0);

  int buffcount = 0;

  while (!input_file.eof())
  {
     //Read next buffer, compute buffer times
     do
     {
       input_file.read((char*)(&buffer), sizeof(buffer));
     }
     while (((buffer.probetype!=probetype)||(buffer.probenumber!=probenumber)) && (!input_file.eof()));

     if (input_file.gcount() < (int)sizeof(buffer))
       break;

     //Record first buffer day for midnight crossings
     if(buffcount == 0) firstday=ntohs(buffer.day); 

     lastbuffertime = buffertime;
     buffertime = TwoDtime(&buffer) + ((double)ntohs(buffer.msec) / 1000);

     if (buffertime >= cfg.stoptime)
       break;

     firsttimeflag = true;
     //Scroll through each slice, look for sync/time slices
     for (int islice=0; islice<slicesPerRecord; islice++){
        slice=endianswap_ull(((unsigned long long *)buffer.image)[islice]);

        if ((slice & 0xffffff0000000000ULL) == 0xaaaaaa0000000000ULL) {           
           //Found a sync line
           timeline = slice & 0x000000ffffffffffULL;
           if (firsttimeflag) {
              firsttimeline = timeline; 
              firsttimeflag = false;
           }

           //Look for negative interarrival time, set to zero instead
           if (timeline < firsttimeline) difftimeline=0;
           else difftimeline=timeline-firsttimeline;

           //Process the roi
           long time1hz=(long) (lastbuffertime+difftimeline/(12.0e6));
           if (time1hz >= cfg.starttime){
              particle=findsize(roi,slice_count,probe.resolution);
              particle.holearea=fillholes2(roi,slice_count);           
              particle.inttime=(timeline-lasttimeline)/12.0e6;
              particle.time1hz=time1hz;

              //Decide which size to use
              particle.size=particle.csize;  //Default
              if (cfg.smethod=='x') particle.size=particle.xsize;
              if (cfg.smethod=='y') particle.size=particle.ysize;           

              //Update interarrival queue
              itq[iitq]=particle.inttime;
              iitq++;
              if (iitq > (nitq-1)) iitq=0;
           }

           //Debugging output
           if ((buffcount == -100)) {
              cout<<islice<<endl;
              showparticle(particle);
              showroi(roi,slice_count);
           }

           //Check the particle time to see if a new 1-s period has been crossed.
           //If so, place all particles in count matrix
           if (time1hz != last_time1hz){
              itime = last_time1hz - cfg.starttime;  //time index

              //Make sure particles are in correct time range
              if (itime>=0){
                 if (ncfile.hasTASX() == false)
                   data.tas[itime]=((float)ntohs(buffer.tas));

                 //Fill interarrival time array with all particles
//                 for (int i=0; i<particle_stack.size(); i++){                    
//                    iit=0;
//                    while((particle_stack[i].inttime)>it_endpoints[iit+1]) iit++;
//                    count_it[itime][iit+1]++;   //Add 1 to iit for RAF convention
//                 }   

                 //Interarrival time array, queue version
                 for (int i=0; i<nitq; i++){                    
                    iit=0;
                    if (itq[i] < it_endpoints[cfg.nInterarrivalBins]) {  // This should be the largest time allowable 
                       while((itq[i])>it_endpoints[iit+1]) iit++;
                       count_it[itime][iit+binoffset]++;   //Add offset to iit for RAF convention
                    }
                 }

                 for (int i = 0; i < cfg.nInterarrivalBins; i++) fitspec[i] = count_it[itime][i+binoffset];
                 dpoisson_fit(it_midpoints, fitspec, bestfit, cfg.nInterarrivalBins);
                 data.cpoisson1[itime]=(float)bestfit[0];  //Save factors
                 data.cpoisson2[itime]=(float)bestfit[1];
                 data.cpoisson3[itime]=(float)bestfit[2];

                 // Compute shattering corrections if flagged
                 if (cfg.shattercorrect) {
                   data.pcutoff[itime]=(float)(1.0/bestfit[1]*0.05);  // Compute cutoff time
                   data.corrfac[itime]=(float)(1.0/(2*exp(-data.pcutoff[itime]*bestfit[1])-1));  //Compute correction factor
                 } else {
                   data.pcutoff[itime]=0;   // No rejection or corrections
                   data.corrfac[itime]=1.0;
                 }

                 if (cfg.verbose) cout<<itime+cfg.starttime<<" "<<time1hz<<" "<<particle_stack.size()<<" "<<bestfit[0]<<" "<<bestfit[1]<<" "<<bestfit[2]<<endl;

                 // Sort through all particles in this stack
                 for (size_t i = 0; i < particle_stack.size(); i++) {
                    //Find water size correction
                    wc=poisson_spot_correction(particle_stack[i].area,particle_stack[i].holearea,particle_stack[i].allin);

                    // Rejection
                    if(i==particle_stack.size()-1) nextit=particle.inttime;  //This particle is for next time period, but use its inttime
                    else nextit=particle_stack[i+1].inttime;
                    reject_particle(particle_stack[i], data.pcutoff[itime], nextit, probe.resolution,
                                    probe.bin_endpoints[0], probe.bin_endpoints[probe.numBins],wc, cfg.recon);
                    if (cfg.debug) showparticle(particle_stack[i]);                    

                    // Fill count arrays with accepted particles
                    if (!particle_stack[i].ireject){
                       isize=0; 
                       while((particle_stack[i].size)>probe.bin_endpoints[isize+1]) isize++;
                       count_all[itime][isize+binoffset]++;   //Add offset to isize for RAF convention
                       data.all.accepted[itime]++;
                    } else data.all.rejected[itime]++;
                    if (!particle_stack[i].wreject){
                       wsize=0; 
                       while((particle_stack[i].size/wc)>probe.bin_endpoints[wsize+1]) wsize++;
                       count_round[itime][wsize+binoffset]++;   //Add offset to wsize for RAF convention
                       data.round.accepted[itime]++;
                    } else data.round.rejected[itime]++;
                 } // End sorting through particle stack
              } // End of time check

              // Restart particle stack
              particle_stack.clear();
              last_time1hz=time1hz;
           } // End crossed into new time period

           // Add this particle to vector
           particle_stack.push_back(particle);

           // Start a new particle
           particle_count++;
           lasttimeline=timeline;
           slice_count=0;
        } // end of image processing after detection of sync line 
        else {
           // Found an image slice, make the next slice part of binary image
           for (int idiode = 0; idiode < nDiodes; idiode++)
               roi[slice_count][idiode]=((slice & powerof2[63-idiode])/powerof2[63-idiode]);
           slice_count=min(slice_count+1,511);  //Increment slice_count, limit to 511
        }

     } //end slice loop
     buffcount++;  //Update buffer counter
     if (!cfg.verbose && (buffcount % 100==0))
       cout	<< ntohs(buffer.hour) << ':' << ntohs(buffer.minute)
		<< ':' << ntohs(buffer.second) << " - " << buffcount
		<< " records  \r" << flush;
  } //end buffer loop
 
  // Close raw data file
  input_file.close();


  
  //=========Compute sample volume, concentration, total number, and LWC======
  cout << "\nComputing derived parameters...";

  float zFac[probe.numBins], dia2[probe.numBins], dia3[probe.numBins];
  for (int i = 0; i < probe.numBins; i++) {
    zFac[i] = pow((double)probe.bin_midpoints[i] / 1000.0, 6.0);
    dia2[i] = probe.bin_midpoints[i] * probe.bin_midpoints[i];
    dia3[i] = pow((double)probe.bin_midpoints[i], 3.0);
  }

  // Compute
  for (int i = 0; i < numtimes; i++)
  {
    float dbar2_all = 0.0, dbar2_round = 0.0;
    float z_all = 0.0, z_round = 0.0;

    for (int bin = 0; bin < probe.numBins; bin++)
    {
      if (data.tas[i] > 0.0) {
        float sv = probe.samplearea[bin] * data.tas[i];  // Sample volume (m3)

        // Correct counts for the poisson fitting
        if (std::isnan(data.corrfac[i])) data.corrfac[i]=1.0;  //Filter out bad correction factors
        count_all[i][bin+binoffset] *= data.corrfac[i];
        count_round[i][bin+binoffset] *= data.corrfac[i];
        conc_all[i][bin+binoffset] = count_all[i][bin+binoffset] / sv / 1000.0;	// #/L
        conc_round[i][bin+binoffset] = count_round[i][bin+binoffset] / sv / 1000.0;	// #/L

        if (bin >= probe.firstBin) {
          data.all.total_conc[i] += conc_all[i][bin+binoffset];
          data.all.dbar[i]	+= conc_all[i][bin+binoffset] * probe.bin_midpoints[bin];
          dbar2_all		+= conc_all[i][bin+binoffset] * dia2[bin];
          data.all.lwc[i]	+= conc_all[i][bin+binoffset] * dia3[bin];
          z_all			+= conc_all[i][bin+binoffset] * zFac[bin];

          data.round.total_conc[i] += conc_round[i][bin+binoffset];
          data.round.dbar[i]	+= conc_round[i][bin+binoffset] * probe.bin_midpoints[bin];
          dbar2_round		+= conc_round[i][bin+binoffset] * dia2[bin];
          data.round.lwc[i]	+= conc_round[i][bin+binoffset] * dia3[bin];
          z_round		+= conc_round[i][bin+binoffset] * zFac[bin];
        }
      }
    }

    if (z_all > 0.0)
      data.all.dbz[i] = 10.0 * log10((double)(z_all * 1.0e3));

    if (z_round > 0.0)
      data.round.dbz[i] = 10.0 * log10((double)(z_round * 1.0e3));

    if (data.all.total_conc[i] > 0.0001) {
      data.all.dbar[i] /= data.all.total_conc[i];

      data.all.disp[i] = (float)sqrt(fabs((double)(dbar2_all /
                        data.all.total_conc[i] - data.all.dbar[i] *
                        data.all.dbar[i]))) / data.all.dbar[i];
    }

    if (data.round.total_conc[i] > 0.0001) {
      data.round.dbar[i] /= data.round.total_conc[i];

      data.round.disp[i] = (float)sqrt(fabs((double)(dbar2_round /
                        data.round.total_conc[i] - data.round.dbar[i] *
                        data.round.dbar[i]))) / data.round.dbar[i];
    }

    if (dbar2_all > 0.0)
      data.all.eff_rad[i] = 0.5 * (data.all.lwc[i] / dbar2_all);

    if (dbar2_round > 0.0)
      data.round.eff_rad[i] = 0.5 * (data.round.lwc[i] / dbar2_round);

    data.all.lwc[i] *= M_PI / 6.0 * 1.0e-9;
    data.round.lwc[i] *= M_PI / 6.0 * 1.0e-9;
  }



  //=============Write to netCDF==============================================
  if (buffcount <= 1) return 1;  //Don't write empty files

  cout << "\nWriting to netCDF file...";
  ncfile.CreateNetCDFfile(cfg);	// Output file; Create as necessary.
  NcFile *dataFile = ncfile.ncid();

  ncfile.CreateDimensions(numtimes, probe, cfg);

  // Define the variables. 
  NcVar *timevar, *var;
  string varname;

  if ((timevar = ncfile.addTimeVariable(cfg, numtimes)) == 0)
    return netCDF::NC_ERR;

  varname = "interarrival_endpoints";
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncFloat, ncfile.intbindim()))) return netCDF::NC_ERR;
    if (!var->put(it_endpoints, cfg.nInterarrivalBins+1)) return netCDF::NC_ERR; 
  }

  // Counts.  These are not in the ProbeData class yet, hence they are written here. @todo
  varname="A2DCA"+probe.suffix;
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncFloat, ncfile.timedim(), ncfile.spsdim(), ncfile.bindim_plusone()))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Corrected Accumulation per Channel, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
    if (!var->add_att("Resolution", (int)probe.resolution)) return netCDF::NC_ERR;
    if (!var->add_att("nDiodes", probe.nDiodes)) return netCDF::NC_ERR;
    if (!var->add_att("ResponseTime", (float)0.4)) return netCDF::NC_ERR;
    if (!var->add_att("ArmDistance", probe.armWidth * 10)) return netCDF::NC_ERR;
    if (!var->add_att("Rejected", "Roundness below 0.1, interarrival time below 1/20th of distribution peak")) return netCDF::NC_ERR;
    if (!var->add_att("ParticleAcceptMethod", "Reconstruction")) return netCDF::NC_ERR;
  }
  if (!var->put(&count_all[0][0], numtimes, 1, probe.numBins+binoffset)) return netCDF::NC_ERR; 

  varname="A2DCR"+probe.suffix;
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncFloat, ncfile.timedim(), ncfile.spsdim(), ncfile.bindim_plusone()))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Corrected Accumulation per Channel, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
    if (!var->add_att("Resolution", (int)probe.resolution)) return netCDF::NC_ERR;
    if (!var->add_att("nDiodes", probe.nDiodes)) return netCDF::NC_ERR;
    if (!var->add_att("ResponseTime", (float)0.4)) return netCDF::NC_ERR;
    if (!var->add_att("ArmDistance", probe.armWidth * 10)) return netCDF::NC_ERR;
    if (!var->add_att("Rejected", "Roundness below 0.5, interarrival time below 1/20th of distribution peak")) return netCDF::NC_ERR;
    if (!var->add_att("ParticleAcceptMethod", "Reconstruction")) return netCDF::NC_ERR;
  }
  if (!var->put(&count_round[0][0], numtimes, 1, probe.numBins+binoffset)) return netCDF::NC_ERR; 

  varname="I2DCA"+probe.suffix;
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncInt, ncfile.timedim(), ncfile.spsdim(), ncfile.intbindim()))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", -32767)) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Interarrival Time Accumulation, All Particles Including Rejections")) return netCDF::NC_ERR;
    if (!var->add_att("CellSizes", cfg.nInterarrivalBins, it_endpoints)) return netCDF::NC_ERR;
  }
  if (!var->put(&count_it[0][0], numtimes, 1, cfg.nInterarrivalBins+1)) return netCDF::NC_ERR;

  //Concentration
  varname="C2DCA"+probe.suffix;
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncFloat, ncfile.timedim(), ncfile.spsdim(), ncfile.bindim_plusone()))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "#/L")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Concentration per Channel, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
    if (!var->add_att("FirstBin", probe.firstBin)) return netCDF::NC_ERR;
    if (!var->add_att("LastBin", probe.numBins+binoffset-1)) return netCDF::NC_ERR;
    if (!var->add_att("DepthOfField", probe.numBins, &probe.dof[0])) return netCDF::NC_ERR;
    if (!var->add_att("EffectiveAreaWidth", probe.numBins, &probe.eaw[0])) return netCDF::NC_ERR;
    if (!var->add_att("CellSizes", probe.numBins+1, &probe.bin_endpoints[0])) return netCDF::NC_ERR;
    if (!var->add_att("CellSizeUnits", "micrometers")) return netCDF::NC_ERR;
    if (!var->add_att("Density", (float)1.0)) return netCDF::NC_ERR;
  }
  if (!var->put(&conc_all[0][0], numtimes, 1, probe.numBins+binoffset)) return netCDF::NC_ERR; 

  varname="C2DCR"+probe.suffix;
  if ((var = dataFile->get_var(varname.c_str())) == 0) {
    if (!(var = dataFile->add_var(varname.c_str(), ncFloat, ncfile.timedim(), ncfile.spsdim(), ncfile.bindim_plusone()))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "#/L")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Concentration per Channel, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
    if (!var->add_att("FirstBin", probe.firstBin)) return netCDF::NC_ERR;
    if (!var->add_att("LastBin", probe.numBins+binoffset-1)) return netCDF::NC_ERR;
    if (!var->add_att("DepthOfField", probe.numBins, &probe.dof[0])) return netCDF::NC_ERR;
    if (!var->add_att("EffectiveAreaWidth", probe.numBins, &probe.eaw[0])) return netCDF::NC_ERR;
    if (!var->add_att("CellSizes", probe.numBins+1, &probe.bin_endpoints[0])) return netCDF::NC_ERR;
    if (!var->add_att("CellSizeUnits", "micrometers")) return netCDF::NC_ERR;
    if (!var->add_att("Density", (float)1.0)) return netCDF::NC_ERR;
  }
  if (!var->put(&conc_round[0][0], numtimes, 1, probe.numBins+binoffset)) return netCDF::NC_ERR; 

  cout << endl;

  delete [] count_all[0];
  delete [] count_round[0];
  delete [] conc_all[0];
  delete [] conc_round[0];

  return ncfile.WriteData(probe, data);
}

/* -------------------------------------------------------------------------- */
string extractElement(const string & line, string name)
{
  size_t tagpos = line.find(name);
  size_t q1=line.find(">", tagpos);
  size_t q2=line.find("<", q1+1);
  return line.substr(q1+1, q2-q1-1);
}

string extractAttribute(const string & line, string name)
{
  size_t tagpos = line.find(name);
  size_t q1=line.find("\"", tagpos);
  size_t q2=line.find("\"", q1+1);
  return line.substr(q1+1, q2-q1-1);
}

/* -------------------------------------------------------------------------- */
void Read2dStartEndTime(Config & config, ifstream & input_file)
{
  P2d_rec buffer;

  // Read first buffer, get start time
  input_file.read((char*)(&buffer), sizeof(buffer));
  config.starttime = TwoDtime(&buffer);

  // Read last buffer, get stop time
  do input_file.read((char*)(&buffer), sizeof(buffer)); while (!input_file.eof());
  config.stoptime = TwoDtime(&buffer);

  cout << "2D file start time: " << ctime(&config.starttime);
  cout << "          end time: " << ctime(&config.stoptime);

  if (config.starttime > config.stoptime)
  {
    cerr << "Date/time of first record is greater than date/time of last record!\n";
    config.stoptime += 240000;  // Midnight crossing
  }
}

/* -------------------------------------------------------------------------- */
void ParseHeader(ifstream & input_file, Config & cfg, vector<ProbeInfo> & probe_list)
{
  string line;

  // Parse XML header
  do
  {    
    getline(input_file, line);

    if (line.find("Project") != string::npos)
      cfg.project = extractElement(line, "Project");

    if (line.find("Platform") != string::npos)
      cfg.platform = extractElement(line, "Platform");

    if (line.find("FlightNumber") != string::npos)
      cfg.flightNumber = extractElement(line, "FlightNumber");

    if (line.find("FlightDate") != string::npos)
      cfg.flightDate = extractElement(line, "FlightDate");

    if (line.find("Fast2DC") != string::npos)	//found a line describing a FAST2D probe
    {
      ProbeInfo thisProbe(extractAttribute(line, "probe id"), nDiodes, numbins, binoffset);

      thisProbe.resolution = atof(extractAttribute(line, "resolution").c_str());
      thisProbe.suffix = extractAttribute(line, "suffix");
      thisProbe.serialNumber = extractAttribute(line, "serialnumber");

      // Attempt to read RAF PMSspecs file.
      char *proj_dir = getenv("PROJ_DIR");
      if (proj_dir) {
        string s, file(proj_dir);
        file += "/" + cfg.project + "/" + cfg.platform + "/PMSspecs";
        PMSspex pms_specs(file);

        s = pms_specs.GetParameter(thisProbe.serialNumber.c_str(), "FIRST_BIN");
        thisProbe.firstBin = atoi(s.c_str());

        s = pms_specs.GetParameter(thisProbe.serialNumber.c_str(), "LAST_BIN");
        thisProbe.lastBin = atoi(s.c_str());
      }

      probe_list.push_back(thisProbe);
    }
  } while ((line.compare(markerline)!=0) && (!input_file.eof()));  
}

/* -------------------------------------------------------------------------- */
void processArgs(int argc, char *argv[], Config & config)
{
  // Parse command line arguments
  for (int i = 1; i < argc; i++)
  {
     string arg = argv[i];
     if ((arg.find("-sta")!=string::npos) && (i<(argc-1))) config.starttime=atoi(argv[++i]); else
     if ((arg.find("-sto")!=string::npos) && (i<(argc-1))) config.stoptime=atoi(argv[++i]); else
     if (arg.find("-n")!=string::npos) config.shattercorrect=0; else
     if (arg.find("-a")!=string::npos) config.recon=0; else
     if (arg.find("-x")!=string::npos) config.smethod='x'; else
     if (arg.find("-y")!=string::npos) config.smethod='y'; else
     if (arg.find("-v")!=string::npos) config.verbose=1; else
     if (arg.find("-d")!=string::npos) config.debug=1; else
     if (arg.find("-o")!=string::npos) config.outputFile=argv[++i]; else
     config.inputFile = arg;
  }
}

/* -------------------------------------------------------------------------- */
int usage(const char* argv0)
{
  cerr << endl << "USAGE:  procf2dc [filename.2d] <options>" << endl << endl;
  cerr << "OPTIONS:" << endl;
  cerr << "   -starttime [hhmmss]"<<endl;
  cerr << "         Specify start time in format hhmmss, default is first available time"<<endl;
  cerr << "   -stoptime [hhmmss]"<<endl;
  cerr << "         Specify stop time in format hhmmss, default is last available time"<<endl;
  cerr << "   -xsize"<<endl;
  cerr << "         Use x-sizing (across the array), use of -allin highly recommended"<<endl;
  cerr << "   -ysize"<<endl;
  cerr << "         Use y-sizing (with the airflow)"<<endl;
  cerr << "   -allin"<<endl;
  cerr << "         Require particles to be fully imaged"<<endl;
  cerr << "   -noshattercorrect"<<endl;
  cerr << "         Turn off shattering rejection and corrections"<<endl;
  cerr << "   -verbose"<<endl;
  cerr << "         Send extra output to console"<<endl;;
  cerr << "   -o file_name"<<endl;
  cerr << "         Specify output file, instead of default output name"<<endl<<endl;;
  cerr << "Example:  procf2dc myfile.2d -start 123000 -stop 140000 -xsize -allin"<<endl<<endl;

  return 1;
} 
  

//================================================================================================
// ------------MAIN-----------------------
//================================================================================================

int main(int argc, char *argv[])
{
  ifstream input_file;
  vector<ProbeInfo> probes;
  Config config;
  
  // Check for correct number of arguments
  if (argc < 2)
    return usage(argv[0]);

  // Make sure all time calculations are in UTC.
  putenv((char *)"TZ=UTC");

  new NcError(NcError::silent_nonfatal);

  processArgs(argc, argv, config);

  // Open raw file, test for existence
  input_file.open(config.inputFile.c_str(), ios::binary);
  if (!input_file.good()) {
    cerr << "Unable to open " << config.inputFile << endl;
    return 1;
  }  
  
  // Parse the XML header in the 2d file and get list of probes.
  ParseHeader(input_file, config, probes);

  // Return if unreadable file
  if (input_file.eof()) {
     cerr << "Unable to find XML header.  Valid 2D file?" << endl;
     return 1;
  }

  Read2dStartEndTime(config, input_file);

  input_file.close();

  netCDF ncFile(config);

  // Process all probes found in the file
  for (size_t i = 0; i < probes.size(); i++)
  {
    cout	<< "Processing: " << probes[i].serialNumber << probes[i].suffix << endl
		<< "  nDiodes : " << probes[i].nDiodes << endl
		<< "  numBins : " << probes[i].numBins << endl
		<< "      res : " << probes[i].resolution << endl
		<< " armwidth : " << probes[i].armWidth << endl
		<< " FirstBin : " << probes[i].firstBin << endl;

    int errorcode = process2d(config, ncFile, probes[i]); 

    if (!errorcode)
      cout << endl << "Sucessfully processed probe " << i << endl;
    else
      cout << endl << "Error on probe " << i << endl;
  }

  return 0;
}
