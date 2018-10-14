/*
-------------------------------------------------------------------------
OBJECT NAME:	mtp.c

FULL NAME:	MTP

ENTRY POINTS:	scal()		Convert counts to brightness temperature
                sretrieve()     Obtain the physical temperature profile from 
		                a scan. 

DESCRIPTION:	Derived calculations for the Microwave Temperature Profiler 
                (MTP) instrument. The MTP build by JPL for NCAR as part of 
		the set of instruments known as HAIS (build specifically in 
		support of the Haiper GV aircraft) came with processing 
		software written in Visual Basic 6.0.

COPYRIGHT:	University Corporation for Atmospheric Research, 2017
 - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include "mtp.h"
#include "mtp_rcf.h"
#include "mtp_rcf_set.h"
#include "mtp_retriever.h"
#include <sys/stat.h>
#include <algorithm> // std::fill()

// Set constants for platinum wire gain equation for temperature of target. 
// These should remain constant as long as physical target doesn't change.
// If target is replaced, these may change.
static float _AA = -244.3364635;
static float _Bb = 0.462418;
static float _cC = 0.0000588;
static float _DD = -0.000000013;
static float _Wtg = 0.1;

// The scan angle associated with the horizontal scan
static int _LocHor = 5;

// per-flight constants from the Defaults.<flight> files
// @param CND0 float[] are the 'Cnd0' values from the REF file.  
// I believe that these are the "temperatures" of the target in each channel
// in degrees C when the noise diode is on.  Comments in the VB code call
// it the Noise Diode Temperature Fit Offset value.
// @param GOF float[] are the 'GOF' values from the REF file.
// I believe that these are an offset for the temperature of the mixer, one
// for each channel.  Comments in the VB code say they are Gain Equation Offsets
// but their use belies that.
// @param GEC1 float[] is the 'GEC(chan,1)' vector from the REF file
// called Gain Equation Coefficients, they do seem to be used in that way
// There is one value per channel, this one is the offset coefficient.
// @param gec2 float[] is the 'GEC(chan,2)' vector from the REF file
// called Gain Equation Coefficients, this one is the linear coefficient
// applied to the difference of the Mixer temperature minus the GOF above.
static const int nCoeffs = 3;
static float CND0[nCoeffs] = {1.0,1.0,1.0}, GOF[nCoeffs] = {1.0,1.0,1.0}, 
             GEC1[nCoeffs] = {1.0,1.0,1.0}, GEC2[nCoeffs] = {1.0,1.0,1.0};

char RCFdir[256];
const std::string RCFDIR = "%s/%s/MTP/RCF";
char CALdir[256];
const std::string CALDIR = "%s/%s/MTP/raw/%s";

int numFlightLevels = 13;  // never changes - every project will have 13 levels
// If no flight levels given in Defaults file, default to these levels.
float defaultLevels[] = {13.0,12.0,9.5,8.0,6.0,5.0,3.5,2.5,2.0,1.5,1.0,0.5,0.0};
std::vector<float> FLIGHTLEVELSKM; // flight levels from Defaults file

static Retriever *Rtr;

/* -------------------------------------------------------------------- */
/* Read in flight levels from the defaults file.                            */
static void readLevels(const char name[],std::vector<float> *var)
{

    float *tmp;

    for (int i = 0; i < numFlightLevels; ++i) {
        var->push_back(defaultLevels[i]);
    }

    if ((tmp = GetDefaultsValue(name, name)) == NULL)
    {
	sprintf(buffer, "%s value defaulting to ", name);
	for (int i = 0; i < numFlightLevels; ++i) {
	  sprintf(buffer+strlen(buffer),"%f ", var[0][i]);
	}
	sprintf(buffer+strlen(buffer),"in AMLIB function mtpInit.\n");
        LogMessage(buffer);
    }
    else
    {
	numFlightLevels = var->size();
	sprintf(buffer, "mtp.c: %s set to ", name);
        for (int i = 0; i < numFlightLevels; ++i) {
          var->push_back(tmp[i]);
	  sprintf(buffer+strlen(buffer),"%f ", var[0][i]);
	}
        sprintf(buffer+strlen(buffer),"from Defaults file.\n");
	LogMessage(buffer);
    }    
}
/* -------------------------------------------------------------------- */
/* Read in constants from the defaults file.                            */
static void readDefs(const char name[],float var[nCoeffs])
{

    float *tmp;

    if ((tmp = GetDefaultsValue(name, name)) == NULL)
    {
	sprintf(buffer, "%s value defaulting to %f %f %f in AMLIB function mtpInit.\n",
                name, var[0], var[1], var[2]);
        LogMessage(buffer);
    }
    else
    {
        for (int i = 0; i < nCoeffs; ++i) var[i] = tmp[i];
        sprintf(buffer,"mtp.c: %s set to %f %f %f from Defaults file.\n", name,
	      var[0], var[1], var[2]);
	LogMessage(buffer);
    }    
}
/* -------------------------------------------------------------------- */
void mtpInit(var_base *varp)
{
    char name[256];
    char *raw_data_dir;
    int indx;

    /* Get the dir where the RCF and CAL files are located */
    raw_data_dir = (char *)getenv("RAW_DATA_DIR");
    (void)sprintf(RCFdir, RCFDIR.c_str(), raw_data_dir, cfg.ProjectNumber().c_str());
    (void)sprintf(CALdir, CALDIR.c_str(), raw_data_dir, cfg.ProjectNumber().c_str(), cfg.FlightDate().c_str());

    /* Check if RCFdir exists. If not, turn off MTP processing but let nimbus continue. */
    struct stat stDirInfo;
    if (stat(RCFdir, &stDirInfo) !=0) {
	sprintf(buffer,"MTP RCF dir %s does not exist - turning off MTP processing\n", RCFdir);
	LogMessage(buffer);
	varp->Output = false;
	((DERTBL*)varp)->compute = 0;

	// SCANBT is used to calculate TEMPC, so turn that off as well.
	if ((indx = SearchTable(derived, "TEMPC_MTP")) != ERR) {
          derived[indx]->Output = false;
          ((DERTBL*)derived[indx])->compute = 0;
	}
	return;
    } else {

      strcpy(name,"CND0");
      readDefs(name,CND0);
      strcpy(name,"GOF");
      readDefs(name,GOF);
      strcpy(name,"GEC1");
      readDefs(name,GEC1);
      strcpy(name,"GEC2");
      readDefs(name,GEC2);

      strcpy(name,"FLIGHTLEVELSKM");
      readLevels(name,&FLIGHTLEVELSKM);


      /* Put together a functioning retrieval_coefficient_fileset */
      RetrievalCoefficientFileSet RCF_Set(RCFdir);
      RCF_Set.setFlightLevelsKm((float *)&FLIGHTLEVELSKM[0], numFlightLevels);
      Rtr = new Retriever(RCF_Set);
    }


}
/* -------------------------------------------------------------------- */
/* Calibrate the MTP scans using constants that are written to the .CAL */
/* file by the VB code for the flight in question.                      */
/* The equations in this routine come from MTPGainEquation.docx (in svn)*/
/* When the MTP instrument completes a scan of the atmosphere the scan  */
/* counts are converted to Brightness Temperatures using this routine.  */
void scal(DERTBL *varp)
{  
  float Gnd[NUM_CHANNELS];
  // For storing the rolling gain value - one value per channel
  float _Gain[NUM_CHANNELS];
  NR_TYPE scanbt[NUM_CHANNELS*NUM_SCAN_ANGLES];
  NR_TYPE scnt_inv[NUM_CHANNELS*NUM_SCAN_ANGLES];

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  NR_TYPE *scnt = GetVector(varp, 0); // The vector of counts produced by the 
                  // MTP instrument for this scan. This vector should be of 
		  // length 30 - three points for each angle (one per channel)
  NR_TYPE *tcnt = GetVector(varp, 1); // The vector of counts produced by the
                  // MTP target. This vector should be of length 6 - three per
		  // channel for the noise diode turned on and three for it 
		  // turned off.
  NR_TYPE saat = GetSample(varp, 2);  //Scan Avg Ambient Air Temp (OAT)
  NR_TYPE tr350cntp=GetSample(varp,3);//Platinum Multiplxr R350 Counts
  NR_TYPE tmixcntp=GetSample(varp,4); //Platinum Multiplxr Mixer Temperature Cnts
  NR_TYPE tr600cntp=GetSample(varp,5);//Platinum Multiplxr R600 Counts

  // Create a gain vector with all elements set to zero.
  for (size_t i=0; i<NUM_CHANNELS; i++) _Gain[i]=0.0;

  /* The scan counts are stored in the ads file as cnts[angle,channel], i.e.
   * {a1c1,a1c2,a1c3,a2c1,...}. Processing requires, and the final data are 
   * output as {c1a1,c1a2,c1a3,c1a4,...}. Invert the array here. (Note that this
   * will eventually be moved to a process function in 
   * nidas/src/nidas/dynld/raf. As of now, SCNT in the processed .nc file
   * is in the original matrix, but scanbt generated by this code is in the
   * inverted matrix, so they can't easily be compared.*/
  for (size_t j=0; j<NUM_SCAN_ANGLES; j++) {
      for (size_t i=0; i<NUM_CHANNELS; i++) {
         scnt_inv[i*10+j]=scnt[j*3+i];
      }
  }

  /* Create a Gnd vector */
  int TargIndx = 0;     // Index into tcnt for the target w/ ND on
  int TargNDIndx = 0;   // Index into tcnt for the target w/ ND off
  for (size_t i=0; i<NUM_CHANNELS; i++)
  {
      Gnd[i] = (tcnt[TargIndx]-tcnt[TargNDIndx])/CND0[i];
      TargIndx = TargIndx+1;
      TargNDIndx = TargNDIndx+1;
  }

  // Counts for TMix
  float r = 350.0+(250.0*(tmixcntp-tr350cntp)/(tr600cntp-tr350cntp));

  // Temperature of the mixer (TMix, also known as Tifa)
  float TMix = _AA +_Bb*r + _cC*r*r + _DD*r*r*r;

  // g() is initialized to a value at the beginning of a flight and then is 
  // adjusted on a scan by scan basis in an iterative fashion. 
  for (size_t i=0; i<NUM_CHANNELS; i++)
  {
      if (_Gain[i] == 0.0) {
         // Initialize the gain
         _Gain[i] = (TMix-GOF[0])*GEC2[i] + GEC1[i];
      } else {
         _Gain[i] = (_Gain[i] + Gnd[i] * _Wtg)/ (1+_Wtg);
      }
  }

  // Calculate Brightness Temperatures
  for (size_t i=0; i<NUM_CHANNELS;i++)
  {
      for (size_t j=0;j<NUM_SCAN_ANGLES;j++)
      {
         scanbt[i*10+j]=saat+(scnt_inv[i*10+j]-scnt_inv[i*10+_LocHor])/_Gain[i];
      }
  }


  // @returns a vector of Brightness Temperatures for the scan. 
  // There are 30 elements to the vector, 3 values at each of the 10 angles
  // of the MTP's scan.
  PutVector(varp, &scanbt);
}      /* END scal */

/* -------------------------------------------------------------------- */
/* Convert brightness temperatures to atmospheric temperature profiles 
 * by performing an inverse calculation of the radiative transfer model.
 * Requires as input Retrieval Coefficient Files (RCFs) written by the MTP 
 * Visual Basic program. RCF files are templates that describe what a 
 * given RAOB profile would look like to the MTP instrument if the instrument
 * was used to measer the atmosphere described by the profile. The
 * weighted averaged retrieval coefficients from the RCF that best matches our
 * scan at our flight altitude are calculated.
 *
 * When the MTP instrument completes a scan of the atmosphere the scan counts
 * are converted to Brightness Temperatures (in scal above), using these the
 * "best match" RCF is found and it's corresponding Retrieval Coefficients are 
 * used to determine the atmospheric temperature profile.
 */
void sretriever(DERTBL *varp)
{

  NR_TYPE *scanbt = GetVector(varp, 0); // The vector of brightness temperatures
                  // calculated in scal (above) for this scan. This vector should
		  // be of length 30 - three points for each angle (one per 
		  // channel)
  NR_TYPE ggalt = GetSample(varp, 1);  //Aircraft altitude (MSL) meters
  NR_TYPE tempc[NUM_RETR_LVLS]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};

  /* If GGALT is missing, return missing for tempc */
  if (std::isnan(ggalt))
  {
    std::fill(tempc, tempc+NUM_RETR_LVLS, floatNAN);
  }
  else {

    AtmosphericTemperatureProfile ATP;
    ATP = Rtr->Retrieve(scanbt, ggalt/1000.0); // convert m to km

    for (size_t i=0; i<NUM_RETR_LVLS;i++)
    {
      tempc[i]=ATP.Temperatures[i];
    }

  }

  PutVector(varp, &tempc);

}      /* End sretriever */
/* END MTP.C */
