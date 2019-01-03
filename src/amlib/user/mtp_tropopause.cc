#include "nimbus.h"
#include "mtp.h"
#include "mtp_tropopause.h"

// Constants used in tropopause calculations
int referenceLapseRate = -2; // 2K/km decrease 
float referenceLayerThickness = 2; //2km
float minHt = 5.6; // Lowest altitude to look for tropopause (500mb = 5.6km)

extern NR_TYPE altc[NUM_RETR_LVLS];
extern NR_TYPE tempc[NUM_RETR_LVLS];
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

/* -------------------------------------------------------------------- */
/* The WMO definition of the first tropopause (i.e., the conventional
 * Inputs:
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

/* -------------------------------------------------------------------- */
/* The WMO definition of the first tropopause (i.e., the conventional
 * Find the temperature at a given altitude by linear interpolation *
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
float findTropopause(NR_TYPE *altctrop1,NR_TYPE *tempctrop1)
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
  *altctrop1 = altc[LT];
  *tempctrop1 = tempc[LT]; 
  
  // Return index to level of tropopause; zero indicated none found
  return(LT);
}
/* END MTP_TROPOPAUSE.C */
