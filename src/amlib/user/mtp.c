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
#include <algorithm> // std::fill()
#include <boost/filesystem.hpp>

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

char RCFfiles[8192]; // comma-separated list of RCF file names found in RCFdir
char RCFdir[256];
const std::string RCFDIR = "%s/%s/MTP/RCF";
//char CALdir[256];
//const std::string CALDIR = "%s/%s/MTP/raw/%s";

static const int numFlightLevels = 13;  // never changes - every project will have 13 levels
// If no flight levels given in Defaults file, default to these levels.
float defaultLevels[numFlightLevels] = {13.0,12.0,9.5,8.0,6.0,5.0,3.5,2.5,2.0,1.5,1.0,0.5,0.0};
float FLIGHTLEVELSKM[numFlightLevels]; // flight levels from Defaults file

NR_TYPE altc[NUM_RETR_LVLS];
NR_TYPE tempc[NUM_RETR_LVLS];
// Indices encoding details of RCF file usage.
NR_TYPE rcfidx, rcfalt1idx, rcfalt2idx, rcfmridx, trop1idx; 
NR_TYPE altctrop1; // Altitude of first tropopause
NR_TYPE tempctrop1; // Temparature of first tropopause

static Retriever *Rtr;

// Constants used in tropopause calculations
int referenceLapseRate = -2; // 2K/km decrease 
float referenceLayerThickness = 2; //2km
float minHt = 5.6; // Lowest altitude to look for tropopause (500mb = 5.6km)
/* -------------------------------------------------------------------- */
/* Read in flight levels from the defaults file.                            */
static void readLevels(const char name[],float var[numFlightLevels])
{

    float *tmp;

    if ((tmp = GetDefaultsValue(name, name)) == NULL)
    {
	sprintf(buffer, "%s value defaulting to ", name);
	for (int i = 0; i < numFlightLevels; ++i) {
          var[i]=defaultLevels[i];
	  sprintf(buffer+strlen(buffer),"%f ", var[i]);
	}
	sprintf(buffer+strlen(buffer),"in AMLIB function mtpInit.\n");
        LogMessage(buffer);
    }
    else
    {
	sprintf(buffer, "mtp.c: %s set to ", name);
        for (int i = 0; i < numFlightLevels; ++i) {
          var[i] = tmp[i];
	  sprintf(buffer+strlen(buffer),"%f ", var[i]);
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
    raw_data_dir = getenv("RAW_DATA_DIR");
    sprintf(RCFdir, RCFDIR.c_str(), raw_data_dir, cfg.ProjectNumber().c_str());
    // FlightDate is MM/DD/YYYY, we need YYYYMMDD. This line doesn't work, but not yet
    // used. Right now cal values are in Defaults files. This line is in case later we
    // want to get them directly from the VB-generated CAL files.
    //sprintf(CALdir, CALDIR.c_str(), raw_data_dir, cfg.ProjectNumber().c_str(), cfg.FlightDate().c_str());
    // LogMessage(CALdir);

    /* Check if RCFdir exists. If not, turn off MTP processing but let nimbus continue. */
    if (boost::filesystem::exists(RCFdir)) {

      strcpy(name,"CND0");
      readDefs(name,CND0);
      strcpy(name,"GOF");
      readDefs(name,GOF);
      strcpy(name,"GEC1");
      readDefs(name,GEC1);
      strcpy(name,"GEC2");
      readDefs(name,GEC2);

      strcpy(name,"FLIGHTLEVELSKM");
      readLevels(name,FLIGHTLEVELSKM);


      /* Put together a functioning retrieval_coefficient_fileset */
      RetrievalCoefficientFileSet RCF_Set(RCFdir);
      std::vector<RetrievalCoefficientFile> _RCFs =  RCF_Set.getRCFVector();
      // Save string of RCF filenames here. This string is added as an attribute
      // to MTP vars in NetCDF.cc
      for (size_t j = 0; j < _RCFs.size(); ++j)
      {
	  sprintf(RCFfiles+strlen(RCFfiles),"%s,",basename(_RCFs[j].getFileName().c_str()));
      }
      //LogMessage(RCFfiles);

      RCF_Set.setFlightLevelsKm(FLIGHTLEVELSKM, numFlightLevels);
      Rtr = new Retriever(RCF_Set);
    } else {
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
/* The WMO definition of the first tropopause (i.e., the conventional
 * tropopause) is defined using two components:
 *   1. The lowest level at which the linear lapse rate decreases to 2 K/km 
 *   or less (i.e. the profile is more steep)
 * and 
 *   2. The average lapse rate from this level to any level within the
 *   next higher 2 km does not exceed 2 K/km.
 * The following functions determine the location in the profile where
 * these two components are met.
 */

/* Inputs:
 *      start_index - the index into the profile to start looking at (if you 
 *      	don't want to start at the lowest measurement in the profile). 
 *      i - pointer to index of lowest level where lapse rate decreases to
 *       	2K/km or less. (i is the bottom of the layer)
 * Output:
 *      lapseRate - the linear lapse rate for the layer we are working with
 */
float linearLapseRate(int start_index, int *i)
{
  float lapseRate;
  for (*i=start_index; *i<NUM_RETR_LVLS-1; (*i)++) 
  {
    if (altc[*i+1] != altc[*i]) 
    {
      lapseRate = (tempc[*i+1]-tempc[*i])/(altc[*i+1]-altc[*i]);
      if (lapseRate >= referenceLapseRate) // 2K/km decrease or less
	return(lapseRate);
    }
  }
  // If no tropopause found, return nan
  *i=0;
  return(floatNAN);
}

/* Find the temperature at a given altitude by linear interpolation *
 * Inputs: 
 *  	altInterp - altitude to interpolate temperature to
 *  	iStart 	-  index of the starting point to find the first measurement
 *  		above the interpolation point.
 * Outputs: 
 * 	return temperature at the interpolation altitude
 */
float Tinterp(float altInterp, int iStart)
{
     int i;

     // Don’t interpolate between bottom two layers (Why?)
     if (altInterp <= altc[1] + 0.01) return(0); 

     // starting at iStart, find first measurement above altInterp
     for (i = iStart; i < NUM_RETR_LVLS; i++) 
     {
        if (altc[i] >= altInterp) break;
     }
     
     // Interpolate temperature 
     float altBot = altc[i - 1];  // altitude at bottom of layer to interpolate
     float tempTop = tempc[i];    // temperature at top of layer to interpolate
     float tempBot = tempc[i - 1];// temperature at bottom of layer to interpolate

     return(tempBot+ ((tempTop-tempBot) * (altInterp-altBot)/(altc[i]-altBot)));
}

/*
 * Input:
 *     startidx - Level to begin our search for the tropopause
 *     step - width of a step through our layer
 *     also uses global referenceLayerThickness
 * Output:
 *     LRavg - average lapse rate
 */
float averageLapseRate(int startidx, float step, int baseLevel)
{
  int nlayers = referenceLayerThickness/step; // number of sub-layers to divide layer into
  float deltaTsum = 0; // running total of temperature difference
  float LRavg = floatNAN; // average lapse rate

  float altBot = altc[startidx];
  for (int i=1; i<nlayers; i++)
  {
    // Find alt at bottom and top of layer we are testing
    float altTop = altc[startidx] + step*i;

    // linear interpolation to find temperature at top and bottom altitudes (since
    // we may not have a scan there)
    float tempBot = Tinterp(altBot, baseLevel);
    float tempTop = Tinterp(altTop, baseLevel);
    if (tempTop == 0) return(floatNAN);  //Ran out of RAOB; didn't find tropopause

    // Calculate average lapse rate from the bottom of the layer to our current level
    deltaTsum = deltaTsum + (tempTop-tempBot);

    altBot = altTop; // get set for next layer
  }
  LRavg = deltaTsum/(step * nlayers);

  return(LRavg); // average lapse rate 
}

/* Find a tropopause */
float findTropopause()
{
  int startidx = 0;  // Level to begin our search for the tropopause
  int LT = 0;        // Level of best tropopause found so far
  int foundTrop = 0; // Boolean to indicate when tropopause has been located.
  float LRavg;	     // Average lapse rate over the reference layer
  int baseLevel = 1; // index of level above LT to work with

  // Locate first retrieval above lowest altitude to look for tropopause
  for (int i=1; i<NUM_RETR_LVLS; i++) 
  {
    if (altc[i] > minHt) 
    {
	startidx = i;
        break;
    }
  }

  while (!foundTrop) 
  {

    /* Starting with first measurement above 500mb, find the index (LT) of
     * the lowest level at which the linear lapse rate (LRavg) decreases to 
     * 2K/km or less. This is the index of our possible tropopause that meets
     * the first part of the WMO criteria.*/
    LRavg = linearLapseRate(startidx, &LT); 
    if (std::isnan(LRavg)) {break;}

    /* For the second part of the WMO definition, confirm that the average
     * lapse rate from our possible tropopause (LT) to any level within the
     * next higher 2 km does not exceed 2 K/km. (‘Average is >=-2). If there
     * are no measurements within the next 2km, then the average lapse rate
     * is LRavg already calculated above. */

    // Next higher measurement is less than 2km from our possible tropopause
    // if next higher measurement is more than 2km, then all we can calc is the
    // linear lapse rate, which we did above.
    if (altc[LT+1]-altc[LT] < referenceLayerThickness) 
    { 
      // loop until find measurement that is 2km above possible tropopause or
      // more, and save the level just below that (so level is measurement just
      // below top of our 2km layer.
      for (baseLevel=LT+1; baseLevel < NUM_RETR_LVLS-1; baseLevel++) 
      {
        if ((altc[baseLevel+1] - altc[LT]) > referenceLayerThickness) break; 
      }

      // Interpolate to get value at exactly 2km and calc lapse rate to that level
      float step = referenceLayerThickness;
      LRavg = averageLapseRate(LT, step, baseLevel);
      if (LRavg < referenceLapseRate) {startidx=baseLevel; continue;}

      // Now check that lapse rate is less than 2K/km from LT to *any* level within the
      // next 2KM. Do this by dividing the layer into 100 sub-layers and checking from
      // LT to each one.
      step = 0.02;
      LRavg = averageLapseRate(LT, step, baseLevel);

    }

    // Check if average lapse rate over reference layer exceeds 2K/km. If so,
    // failed WMO criteria and still haven't found tropopause.
    if (LRavg < referenceLapseRate) 
    {
	startidx = baseLevel;  // set lowest possible tropopause ht to current level and cont
    } else {
	foundTrop = 1;  // exit while loop
    }
  }

  // if we get here, we met all the criteria. Zt1 and TT1 are the altitude and
  // temperature of the first tropopause
  altctrop1 = altc[LT];
  tempctrop1 = tempc[LT]; 
  
  // Return index to level of tropopause; zero indicated none found
  return(LT);
}
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

  size_t nMissMTP; // count missing vals to determine missing rec

  /* If GGALT is missing, return missing for altc and tempc */
  if (std::isnan(ggalt))
  {
    std::fill(tempc, tempc+NUM_RETR_LVLS, floatNAN);
    std::fill(altc, altc+NUM_RETR_LVLS, floatNAN);
  }
  else {

    AtmosphericTemperatureProfile ATP;
    ATP = Rtr->Retrieve(scanbt, ggalt/1000.0); // convert m to km

    nMissMTP=0; //Initialize
    for (size_t i=0; i<NUM_RETR_LVLS;i++)
    {
      tempc[i]=ATP.Temperatures[i];
      altc[i]=ATP.Altitudes[i];
      if (std::isnan(tempc[i])) nMissMTP++;
    }

    if (nMissMTP == NUM_RETR_LVLS) {
	rcfidx = floatNAN;
	rcfalt1idx = floatNAN;
	rcfalt2idx = floatNAN;
	rcfmridx = floatNAN;
	trop1idx = floatNAN;
	altctrop1 = floatNAN;
    } else {
        rcfidx = ATP.RCFIndex;
	rcfalt1idx = ATP.RCFALT1Index;
	rcfalt2idx = ATP.RCFALT2Index;
	rcfmridx = ATP.RCFMRIndex;
	trop1idx = findTropopause();
    }

  }

  PutVector(varp, &tempc);

}      /* End sretriever */

/* -------------------------------------------------------------------- */
void sretrievealt(DERTBL *varp)
{
  PutVector(varp, &altc);

}	/* End sretrievealt */
/* -------------------------------------------------------------------- */
void sretrieveidx(DERTBL *varp)
{
  PutSample(varp, rcfidx);

}	/* End sretrieveidx */
/* -------------------------------------------------------------------- */
void sretrievealt1idx(DERTBL *varp)
{
  PutSample(varp, rcfalt1idx);

}	/* End sretrievealt1idx */
/* -------------------------------------------------------------------- */
void sretrievealt2idx(DERTBL *varp)
{
  PutSample(varp, rcfalt2idx);

}	/* End sretrievealt2idx */
/* -------------------------------------------------------------------- */
void sretrievemridx(DERTBL *varp)
{
  PutSample(varp, rcfmridx);

}	/* End sretrievemridx */
/* -------------------------------------------------------------------- */
void sretrievetrop1(DERTBL *varp)
{
  PutSample(varp, altctrop1);

}	/* End sretrievetrop1 */

/* END MTP.C */
