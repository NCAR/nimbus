/*
-------------------------------------------------------------------------
OBJECT NAME:	mtp.c

FULL NAME:	MTP

ENTRY POINTS:	scal()		Convert counts to brightness temperature
                sretrieve()     Obtain the physical temperature profile from 
		                a scan. 
		smtptime()	Get the record time returned by the MTP,
				converted to seconds.

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
#include "mtp_tropopause.h"
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
// For storing the rolling gain value - one value per channel
static float _Gain[NUM_CHANNELS] = {0.0,0.0,0.0};

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
NR_TYPE rcfidx, rcfalt1idx, rcfalt2idx, rcfmridx, trop1idx, trop2idx; 
NR_TYPE altctrop1; // Altitude of first tropopause
NR_TYPE tempctrop1; // Temparature of first tropopause
NR_TYPE altctrop2; // Altitude of first tropopause
NR_TYPE tempctrop2; // Temparature of first tropopause

static Retriever *Rtr;

static NR_TYPE last_record_time = -1; // time of previous MTP scan
size_t nMissMTP; // count missing vals to determine missing rec

// Scan average values
static float mtp_sapalt_sum = 0.0;
static float mtp_sapalt_count = 0.0;

/* -------------------------------------------------------------------- */
/* Read in flight levels from the defaults file.                        */
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
    int RCFsuccess = 0; // Successfully found the required RCF files

    std::vector<std::string> filelist;

    /* Get the dir where the RCF and CAL files are located */
    raw_data_dir = getenv("RAW_DATA_DIR");
    sprintf(RCFdir, RCFDIR.c_str(), raw_data_dir, cfg.ProjectNumber().c_str());

    /* Populate filelist with values from ??Defaults file?? for this flight, if extant.
     * If not, filelist will remain empty, and constructor will get every available
     * RCF file.
     */

    /* Hardcode DEEPWAVE limited RCF files here, just for testing. Defaults files
     * can't currently handle string input, and I am still noodling over the best
     * way forward. */
    if (cfg.FlightNumber() == "rf01") {
      filelist.push_back("NRCDE067");
      filelist.push_back("NRCDF067");
      filelist.push_back("NRCDG067");
      filelist.push_back("NRCDK067");
      filelist.push_back("NRCDL067");
      filelist.push_back("NRCDU067");
      filelist.push_back("NRCDV067");
      filelist.push_back("NRCFY067");
      filelist.push_back("NRCGD067");
      filelist.push_back("NRCGF067");
      filelist.push_back("NRCGJ067");
    }
    if (cfg.FlightNumber() == "rf02") {
      filelist.push_back("NRCDE067");
      filelist.push_back("NRCDF067");
      filelist.push_back("NRCDG067");
      filelist.push_back("NRCDL067");
      filelist.push_back("NRCDV067");
      filelist.push_back("NRCDY067");
      filelist.push_back("NRCFR057");
      filelist.push_back("NRCGA067");
      filelist.push_back("NRCGD067");
      filelist.push_back("NRCGI067");
      filelist.push_back("NRCGJ067");
    }

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
      RetrievalCoefficientFileSet RCF_Set(RCFdir,filelist);
      std::vector<RetrievalCoefficientFile> _RCFs =  RCF_Set.getRCFVector();

     if (_RCFs.size() == filelist.size() || filelist.size() == 0) {
       RCFsuccess = 1;

      // Save string of RCF filenames here. This string is added as an attribute
      // to MTP vars in NetCDF.cc
      for (size_t j = 0; j < _RCFs.size(); ++j)
      {
	  sprintf(RCFfiles+strlen(RCFfiles),"%s,",basename(_RCFs[j].getFileName().c_str()));
      }
      //LogMessage(RCFfiles);

      RCF_Set.setFlightLevelsKm(FLIGHTLEVELSKM, numFlightLevels);
      Rtr = new Retriever(RCF_Set);
     }
    }

    /* Didn't find required RCF files, so turn off MTP processing. */
    if (not RCFsuccess) {
      sprintf(buffer,"MTP RCF dir %s does not exist ", RCFdir);
      sprintf(buffer+strlen(buffer),"or could not find all requested RCF files");
      sprintf(buffer+strlen(buffer)," - turning off MTP processing\n");
      LogMessage(buffer);
      varp->Output = false;
      ((DERTBL*)varp)->compute = 0;

      // Turn off calculation of MTPTIME
      if ((indx = SearchTable(derived, "MTPTIME_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      // SCANBT is used to calculate TEMPC, so turn that off as well.
      if ((indx = SearchTable(derived, "TEMPC_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      // TEMPC is used to calculate ALTC, RCFIDX, RCFALT1IDX, RCFALT2IDX,
      // RCFMRIDX, RCFTROP1, and RCFTROP2, so turn all those off too (Whew!)
      if ((indx = SearchTable(derived, "ALTC_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFIDX_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFALT1IDX_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFALT2IDX_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFMRIDX_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFTROP1_MTP")) != ERR) {
        derived[indx]->Output = false;
        ((DERTBL*)derived[indx])->compute = 0;
      }
      if ((indx = SearchTable(derived, "RCFTROP2_MTP")) != ERR) {
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
		  // channel for the noise diode turned on (stored as the
		  // first three values) and three for it turned off (the next
		  // 3).
  NR_TYPE atx = GetSample(varp, 2);  //Ambient Air Temperature, Reference
  NR_TYPE tr350cntp=GetSample(varp,3);//Platinum Multiplxr R350 Counts
  NR_TYPE tmixcntp=GetSample(varp,4); //Platinum Multiplxr Mixer Temperature Cnts
  NR_TYPE tr600cntp=GetSample(varp,5);//Platinum Multiplxr R600 Counts

  // We only receive a good record abougt once every 17 seconds. For the
  // in-between records, scnt and tcnt will be nan. Check for this here so we
  // only process when have a good scan.
  int bad_scan=0; // if we find a nan in the target scan, it's not good data
  for (size_t i=0; i<NUM_CHANNELS; i++)
  {
    if (std::isnan(tcnt[i])) {bad_scan = 1;}
  }
  if (bad_scan) {
    PutVector(varp, &scanbt);
    return;
  }

  // Convert atx to Kelvin
  atx = atx+273.15;

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
  int TargNDIndx = NUM_CHANNELS;   // Index into tcnt for the target w/ ND off
  for (size_t i=0; i<NUM_CHANNELS; i++)
  {
      Gnd[i] = (tcnt[TargIndx+i]-tcnt[TargNDIndx+i])/CND0[i];
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
  // Brightness temperature is the temperature a hypothetical black body in 
  // thermal equilibrium with its surroundings would have to be to emit an 
  // identical amount of radiation at the same wavelength. The MTP measures
  // at the wavelength of oxygen atoms in the atmosphere.
  for (size_t i=0; i<NUM_CHANNELS;i++)
  {
      for (size_t j=0;j<NUM_SCAN_ANGLES;j++)
      {
         scanbt[i*10+j]=atx+(scnt_inv[i*10+j]-scnt_inv[i*10+_LocHor])/_Gain[i];
      }
  }


  // @returns a vector of Brightness Temperatures for the scan. 
  // There are 30 elements to the vector, 3 values at each of the 10 angles
  // of the MTP's scan.
  PutVector(varp, &scanbt);
}      /* END scal */

/* -------------------------------------------------------------------- */
/* Convert the record collection time recorded by the MTP as hh, mm, ss 
 * to seconds.
 */
void smtptime(DERTBL *varp)
{
  NR_TYPE shour = GetSample(varp, 0);   // Sample collection time: Hour
  NR_TYPE smin = GetSample(varp, 1);    // Sample collection time: Min
  NR_TYPE ssec = GetSample(varp, 2);    // Sample collection time: Sec

  NR_TYPE mtptime = shour*3600 + smin*60 + ssec;

  if (!(std::isnan(mtptime))) { 

    // Handle midnight rollover
    if (last_record_time > mtptime) {mtptime = mtptime+86400;} 

    last_record_time = mtptime;
  }

  PutVector(varp, &mtptime);
} /* END smtptime */
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
  NR_TYPE palt = GetSample(varp, 1);    // Aircraft pressure altitude (MSL) meters

  int startTropIndex = 0; // index of level to begin looking for tropopause
  AtmosphericTemperatureProfile ATP;

  /* If PALT is missing or negative, return missing for altc and tempc */
  if (std::isnan(palt) || palt < 0)
  {
    std::fill(tempc, tempc+NUM_RETR_LVLS, floatNAN);
    std::fill(altc, altc+NUM_RETR_LVLS, floatNAN);
    nMissMTP = NUM_RETR_LVLS;
  }
  else {

    ATP = Rtr->Retrieve(scanbt, palt/1000.0); // convert m to km

    nMissMTP=0; //Initialize
    for (size_t i=0; i<NUM_RETR_LVLS;i++)
    {
      tempc[i]=ATP.Temperatures[i];
      altc[i]=ATP.Altitudes[i];
      if (std::isnan(tempc[i])) nMissMTP++;
    }
  }

  if (nMissMTP == NUM_RETR_LVLS) {
	rcfidx = floatNAN;
	rcfalt1idx = floatNAN;
	rcfalt2idx = floatNAN;
	rcfmridx = floatNAN;
	trop1idx = floatNAN;
	trop2idx = floatNAN;
	altctrop1 = floatNAN;
	altctrop2 = floatNAN;
	tempctrop1 = floatNAN;
	tempctrop2 = floatNAN;
  } else {
	// Found an MTP scan
        rcfidx = ATP.RCFIndex;
	rcfalt1idx = ATP.RCFALT1Index;
	rcfalt2idx = ATP.RCFALT2Index;
	rcfmridx = ATP.RCFMRIndex;
	trop1idx = findTropopause(&altctrop1,&tempctrop1,&startTropIndex);
	if (std::isnan(trop1idx)) {
	    trop1idx = floatNAN;
	    trop2idx = floatNAN;
	    altctrop2 = floatNAN;
	} else {
	  // Above call will mondify startTropIndex to be index of level to start
	  // looking for 2nd trop.
	  trop2idx = findTropopause(&altctrop2,&tempctrop2,&startTropIndex);
	  if (std::isnan(trop2idx)) {
	      trop2idx = floatNAN;
	      altctrop2 = floatNAN;
	  }
	}
  }


  PutVector(varp, &tempc);

}      /* End sretriever */

/* -------------------------------------------------------------------- */
void sscanavg(DERTBL *varp)
{
  NR_TYPE palt = GetSample(varp, 0);    // Aircraft pressure altitude (MSL) meters

  NR_TYPE sapalt; // Scan average for aircraft pressure altitude (MSL) kilometers

  if (not (std::isnan(palt))) {
    palt = palt/1000.0; // convert m to km

    mtp_sapalt_sum+=palt;
    mtp_sapalt_count++;
  }

  // Calculate scan averaged values
  if (nMissMTP == NUM_RETR_LVLS) {
	sapalt = floatNAN;
  } else {
      sapalt = mtp_sapalt_sum/mtp_sapalt_count;
      mtp_sapalt_sum=0;
      mtp_sapalt_count=0;
  }

  PutSample(varp,sapalt);
}
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
/* -------------------------------------------------------------------- */
void sretrievetrop2(DERTBL *varp)
{
  PutSample(varp, altctrop2);

}	/* End sretrievetrop2 */
/* -------------------------------------------------------------------- */

/* END MTP.C */
