#pragma once
#include <iostream>
#include <fstream>
#include <arpa/inet.h>

typedef unsigned char   byte;

typedef struct {
  short RCformat;
  char  CreationDateTime[8];
  char  RAOBfilename[80];
  char  RCfilename[80];
  short RAOBcount;
  float LR1;     		// LR above top of RAOB
  float zLRb;			// LR break altitude
  float LR2;			// LR above break altitude
  float RecordStep;		// Record Step through available RAOBs
  float RAOBmin;		// Minimum acceptable RAOB altitude
  float ExcessTamplitude;	// Random Excess Noise Level on Ground
  short Nobs;			// Number of observables
  short Nret;			// Number of retrieval levels
  float dZ[33];                 // Retrieval offset levels wrt flight level
  short NFL;			// Number of flight levels
  float Zr[20];                 // Flight levels (km)
  short Nlo;			// Number of LO channels
  float LO[3];                  // LO frequencies (GHz)
  short Nel;			// Number of elevation angles
  float El[10];                 // Scan mirror elevation angles
  short Nif;			// Number of IF frequencies
  float IFoff[3][16];    	// IF frequency offsets (GHz)
  float IFwt[3][16];     	// Weights assigned to each IF frequency
  float Spare[130] ;
  char SURC[4];          	// SU IFB used to calculate RCs (added 20050128)
  float CHnLSBloss[3];          // CHn LSB RF loss
  float RAOBbias;         	// Bias added to RAOB before calculating RCs
  float CH1LSBloss;		// CH1 LSB linear RF loss gradient
  //   Sensitivity matrix: iRC, NFL, Nlo, Nel
  float SmatrixN1[15][3][10];   // Linear term
  float SmatrixN2[15][3][10];   // Quadratic term
} RCF_HDR;

typedef union{RCF_HDR Rcf_Hdr ; char Array[sizeof(RCF_HDR)];} My_Rcf_Hdr_Un;

typedef struct {
  char SURC[4];                 // SU IFB used to calculate RCs (added 20050128)
  float CHnLSBloss[3];          // CHn LSB RF loss
  float RAOBbias;               // Bias added to RAOB before calculating RCs
  float CH1LSBloss;             // CH1 LSB linear RF loss gradient
  //   Sensitivity matrix: iRC, NFL, Nlo, Nel
  //float SmatrixN1[15][3][10];   // Linear term
  //float SmatrixN2[15][3][10];   // Quadratic term
  float Gmatrix[900];
} END_HDR;

typedef union{END_HDR EH ; char Array[sizeof(END_HDR)];} My_EH;
  
// Structure and union to read each flight level co-efficient info
typedef struct {
  float sBP; 			//Flight level pressure altitude (hPa)
  float sOBrms[30];             //1-sigma apriori observable errors
  float sOBav[30];              //Archive Average observables
  float sBPrl[33];              //Pressure at retrieval levels
  float sRTav[33];              //Average T at retrieval levels
  float sRMSa[33];              //Variance in T at retrieval levels
  float sRMSe[33];              //Formal error in T at retrieval levels
  float Src[990];     	        //33 reztrieval levels, 30 observables
  float Spare[67];
} RC_FL_Read;

typedef union{RC_FL_Read RC_read ; char Array[sizeof(RC_FL_Read)];} My_RC_FL_Un;
 
// Structure to hold each flight level retrieval coefficient information.
typedef struct {
  float sBP; 			//Flight level pressure altitude (hPa)
  float sOBrms[30];             //1-sigma apriori observable errors
  float sOBav[30];              //Archive Average observables
  float sBPrl[33];              //Pressure at retrieval levels
  float sRTav[33];              //Average T at retrieval levels
  float sRMSa[33];              //Variance in T at retrieval levels
  float sRMSe[33];              //Formal error in T at retrieval levels
  float Src[33][30]; 	        //33 reztrieval levels, 30 observables
  float Spare[67];
} RC_Set_1FL;

enum Endianness
{
    ENDIAN_BIG,
    ENDIAN_LITTLE
};

inline void swap_endian(byte* bytes, unsigned int nBytes, 
                        Endianness source, Endianness dest )
{
  if (source == dest)
    return;

  byte* head = bytes;
  byte* last = bytes + nBytes - 1;

  while (head < last)
  {
    byte temp = *head;
    *head = *last;
    *last = temp;

    head++;
    last--;
  }
}

