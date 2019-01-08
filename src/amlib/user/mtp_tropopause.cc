#include "nimbus.h"
#include "mtp.h"
#include "mtp_tropopause.h"

// Constants used in tropopause calculations
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
 *
 * (It appears that MJ's algorithm does NOT follow this reference, but instead
 * follows the WMO defintion.)
 * Reference: Roe, J. M., and W. H. Jasperson (1980), A new tropopause 
 * definition from simultaneous ozone‐temperature profiles, Tech.Rep. 
 * AFGL‐TR‐80–0289, Air Force Geophys. Lab., Hanscom Air Force Base, Mass.
 * [Available from https://apps.dtic.mil/dtic/tr/fulltext/u2/a091718.pdf]
 */

/* -------------------------------------------------------------------- */
/* Find the first linear lapse rate (between two consecutive measurements)
 * that meets or crosses the reference lapse rate cutoff.
 * Inputs:
 *      start_index - the index into the profile to start looking at (if you 
 *      	don't want to start at the lowest measurement in the profile). 
 *      i - pointer to index of lowest level where lapse rate decreases to
 *       	the reference lapse rate or less. (i is the bottom of the layer)
 *      referenceLapseRate - the cutoff lapse rate that indicates a tropopause 
 *     		has been found.
 * Output:
 *      lapseRate - the linear lapse rate for the layer we are working with
 */
float linearLapseRate(int startidx, int *i, int referenceLapseRate)
{
  float lapseRate;
  for (*i=startidx; *i<NUM_RETR_LVLS-1; (*i)++) 
  {
    if (altc[*i+1] != altc[*i]) 
    {
      lapseRate = (tempc[*i+1]-tempc[*i])/(altc[*i+1]-altc[*i]);
      if (lapseRate >= referenceLapseRate)
	return(lapseRate);
    }
  }
  // If no tropopause found, return nan
  *i=NUM_RETR_LVLS;
  return(floatNAN);
}

/* -------------------------------------------------------------------- */
/* Find the temperature at a given altitude by linear interpolation
 * Inputs: 
 *  	altInterp - altitude to interpolate temperature to
 *  	startidx  - index of the starting point to find the first measurement
 *  		above the interpolation point.
 * Outputs: 
 * 	return temperature at the interpolation altitude
 */
float Tinterp(float altInterp, int startidx)
{
     int i;

     // Don’t interpolate between bottom two layers (Why?)
     if (altInterp <= altc[1] + 0.01) return(floatNAN); 

     // starting at startidx, find first measurement above altInterp
     for (i = startidx; i < NUM_RETR_LVLS; i++) 
     {
        if (altc[i] >= altInterp) break;
     }

     if (altc[i] < altInterp) return(floatNAN); // Ran out of RAOB
     
     // Interpolate temperature 
     float altBot = altc[i - 1];  // altitude at bottom of layer to interpolate
     float tempTop = tempc[i];    // temperature at top of layer to interpolate
     float tempBot = tempc[i - 1];// temperature at bottom of layer to interpolate

     return(tempBot+ ((tempTop-tempBot) * (altInterp-altBot)/(altc[i]-altBot)));
}

/* -------------------------------------------------------------------- */
/* Find the average lapse rate from the bottom of this layer to the sub-layer
 * Input:
 *     LT       - Level to begin our search for the tropopause
 *     step     - width of a step through our layer
 *     startidx - the measurement just under 2KM above the LT
 *     also uses global referenceLayerThickness
 * Output:
 *     LRavg - average lapse rate
 */
float averageLapseRate(int LT, float step, int startidx)
{
  float deltaTsum = 0; // running total of temperature difference
  float LRavg = floatNAN; // average lapse rate
  int nlayers = referenceLayerThickness/step; // number of sub-layers to divide layer into

  float altBot = altc[LT];
  for (int i=1; i<=nlayers; i++)
  {
    // Find alt at bottom and top of layer we are testing
    float altTop = altc[LT] + step*i;

    // linear interpolation to find temperature at top and bottom altitudes (since
    // we may not have a scan there)
    float tempBot = Tinterp(altBot, startidx);
    float tempTop = Tinterp(altTop, startidx);
    if (std::isnan(tempTop)) return(floatNAN);  //Ran out of RAOB; didn't find tropopause

    // Calculate average lapse rate from the bottom of the layer to our current level
    deltaTsum = deltaTsum + (tempTop-tempBot);

    altBot = altTop; // get set for next layer
  }
  LRavg = deltaTsum/(step * nlayers);

  return(LRavg); // average lapse rate 
}

/* -------------------------------------------------------------------- */
/* If have a tropopause from a previous call to this routine, need to find a
 * break between tropopauses before can look for the next one. This break is
 * defined as a region where the lapse rate is less than -3 K/km. MJ notes 
 * that he decided to search in a 2KM layer (rather than 1 km - not sure where
 * the initial definition comes from) as 1 km is too sensitive for RAOB data
 * causing too many double (and not credible) tropopauses. 
 *
 * Output:
 * 	altBot - altitude of lowest layer to loop for next tropopause
 */
float findGap(int LT, float step, int startidx)
{
    float LRavg;
    int referenceLapseRate = -3; // -3K/km; end of a tropopause
    float altBot = altc[LT];

    while (LRavg > referenceLapseRate) 
    {

      // Find alt at bottom and top of layer we are testing
      float altTop = altBot + referenceLayerThickness;

      // linear interpolation to find temperature at top and bottom altitudes 
      // (since we may not have a scan there)
      float tempBot = Tinterp(altBot, startidx);
      float tempTop = Tinterp(altTop, startidx);
      if (std::isnan(tempTop)) {
	LRavg = floatNAN;  //Ran out of RAOB; didn't find tropopause
      } else {
        // Calculate average lapse rate from the bottom of the layer to our 
	// current level
        LRavg = (tempTop-tempBot)/referenceLayerThickness;
      }
      if (std::isnan(LRavg)) return(floatNAN); // no tropopause found

      altBot = altBot+step;
    }
}
/* -------------------------------------------------------------------- */
/* Locate the first retrieval above the lowest altitude to look for tropopause
 * Input:
 *     minidx - the lowest altitude to look for the topopause
 * Output:
 *     startidx - the first retrieval above that minimum
 */
void findStart(int *startidx,int minidx)
{
  for (int i=*startidx+1; i<NUM_RETR_LVLS; i++) 
  {
    if (altc[i] > minidx) 
    {
      *startidx = i;
      return;
    }
  }
  // Ran out of RAOB
  *startidx=-1;
}
/* -------------------------------------------------------------------- */
/* Find a tropopause
 * Input:
 *     startidx - the lowest altitude to look for the tropopause
 * Output:
 *     altctrop  - the altitude of the found tropopause
 *     tempctrop - the temperature of the found tropopause
 *     LT        - the index of the found tropopause
 */
float findTropopause(NR_TYPE *altctrop,NR_TYPE *tempctrop, int *startidx)
{
  int LT = 0;        // Level of best tropopause found so far
  int foundTrop = 0; // Boolean to indicate when tropopause has been located.
  float LRavg;	     // Average lapse rate over the reference layer
  int referenceLapseRate; // the cutoff lapse rate that indicates a transition 
      			  // has been found.
  float step = 0.02;

  // To start with, we don't know the tropopause location, so set to missing.
  // (This simplifies returning from all cases where trop can't be found.
  *altctrop = floatNAN;
  *tempctrop = floatNAN;

  // At this point, 
  // startidx will be the value set in the previous pass through.
  if (*startidx != 0)
  {

    // Locate lowest layer above gap between tropopauses.
    float altBot = findGap(LT, step, *startidx);
    if (std::isnan(altBot)) return(floatNAN); // no tropopause found

    // locate first retrieval above identified break (altBot)
    findStart(startidx,altBot);
    if (*startidx == -1) return(floatNAN); // no tropopause found

  } else {

    // locate first retrieval above lowest altitude to look for tropopause.
    findStart(startidx,minHt);
    if (*startidx == -1) return(floatNAN); // no tropopause found
 
  }

  // Find the next tropopause (could be the first)
  referenceLapseRate = -2; // -2K/km; beginning of a tropopause
  while (!foundTrop) 
  {

    /* Starting with first measurement above 500mb, find the index (LT) of
     * the lowest level at which the linear lapse rate (LRavg) decreases to 
     * 2K/km or less. This is the index of our possible tropopause that meets
     * the first part of the WMO criteria.*/
    LRavg = linearLapseRate(*startidx, &LT, referenceLapseRate); 
    if (std::isnan(LRavg)) { // no tropopause found
	*startidx = LT;
	return(floatNAN);
    }

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
      for (*startidx=LT+1; *startidx < NUM_RETR_LVLS-1; (*startidx)++) 
      {
        if ((altc[*startidx+1] - altc[LT]) > referenceLayerThickness) break; 
      }

      // Interpolate to get value at exactly 2km and calc lapse rate to that level
      LRavg = averageLapseRate(LT, referenceLayerThickness, *startidx);
      if (std::isnan(LRavg)) { // no tropopause found
        *startidx = LT;
        return(floatNAN);
      } 
      if (LRavg < referenceLapseRate) {continue;}

      // Now check that lapse rate is less than 2K/km from LT to *any* level within the
      // next 2KM. Do this by dividing the layer into 100 sub-layers and checking from
      // LT to each one.
      LRavg = averageLapseRate(LT, step, *startidx);
      if (std::isnan(LRavg)) { // no tropopause found
        *startidx = LT;
        return(floatNAN);
      } 

    }

    // Check if average lapse rate over reference layer exceeds -2K/km. If so,
    // found tropopause. If not, failed WMO criteria and still haven't found 
    // tropopause. Try again with lowest possible tropopause ht set to current 
    // level (startidx).
    if (LRavg >= referenceLapseRate) 
    {
	foundTrop = 1;  // exit while loop
    }
  }

  // if we get here, we met all the criteria. Zt1 and TT1 are the altitude and
  // temperature of the tropopause
  *altctrop = altc[LT];
  *tempctrop = tempc[LT]; 

  // Set startidx to LT in prep for looking for additional tropopauses
  *startidx = LT;
  
  // Return index to level of tropopause; zero indicated none found
  return(LT);
}
/* END MTP_TROPOPAUSE.C */
