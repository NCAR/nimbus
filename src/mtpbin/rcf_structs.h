/*
 ** Structures and Unions for holding Retrieval Coefficient File (RCF)
 ** data for the Microwave Temperature Profiler (MTP)
 ** See rcf.h for more information
 */

#include "mtp.h"

// Structure and Union to Read the header record.  Note: For some reason the
//  Header structure itself is not matching up with this (despite the VB6 
//  code indicating that it "should").  For this reason another Union to get
//  second half of the RCF header record is defined below.
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
  float dZ[NUM_RETR_LVLS];      // Retrieval offset levels wrt flight level
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
  
// Structure and Union to obtain the 2nd half of the header structure.  Note 
//  That this also only obtains part of the rest of the header with the 
//  floating point values going to 0.00 too early into the reading of the 
//  Sentitivey matrix linear and Quadratic terms.  A puzzle for
//  another day.
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
  float sBP; 			// Flight level pressure altitude (hPa)
  float sOBrms[NUM_OBSVBLS];    // 1-sigma apriori observable errors
  float sOBav[NUM_OBSVBLS];     // Archive Average observables
  float sBPrl[NUM_RETR_LVLS];   // Pressure at retrieval levels
  float sRTav[NUM_RETR_LVLS];   // Average T at retrieval levels
  float sRMSa[NUM_RETR_LVLS];   // Variance in T at retrieval levels
  float sRMSe[NUM_RETR_LVLS];   // Formal error in T at retrieval levels
  float Src[990];     	        // 33 retrieval levels, 30 observables
  float Spare[67];
} RC_FL_Read;

typedef union{RC_FL_Read RC_read ; char Array[sizeof(RC_FL_Read)];} My_RC_FL_Un;
   
// Structure to hold each flight level retrieval coefficient information.
typedef struct {
  float Palt; 		         // Flight level pressure altitude (hPa)
  float OBRms[NUM_OBSVBLS];      // 1-sigma apriori observable errors
  float OBAvg[NUM_OBSVBLS];      // Archive Average observables
  float PAltRl[NUM_RETR_LVLS];   // Pressure at retrieval levels
  float TAvgRl[NUM_RETR_LVLS];   // Average T at retrieval levels
  float TVarRl[NUM_RETR_LVLS];   // Variance in T at retrieval levels
  float TRmsRl[NUM_RETR_LVLS];   // Formal error in T at retrieval levels
  float RC[NUM_RETR_LVLS][NUM_OBSVBLS];  // Retrieval Coefficients 
} RC_Set_1FL;
