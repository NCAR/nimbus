
#include "Netcdf.h"

static const float	delta_wp3 = 2.0, delta_roll = 3.0;

/* -------------------------------------------------------------------- */
AircraftData::AircraftData(const char fileName[])
{
  NcAtt		*attr;

  nSegs = currSeg = 0;

  file = new NcFile(fileName);

  if (!file->is_valid())
    {
    std::cerr << "Unable to open netCDF file " << fileName << ".\n";
    return;
    }

  attr = file->get_att("TimeInterval");
  if (attr->is_valid())
    {
    startTime = attr->as_string(0);
    endTime = strchr(attr->as_string(0), '-')+1;
    }
 
  // Bump up endTime if we have midnight roll-over.
  if (endTime < startTime)
    endTime += 86400;

  hourVP = file->get_var("HOUR");
  minVP = file->get_var("MINUTE");
  secVP = file->get_var("SECOND");
  yearVP = file->get_var("YEAR");
  monthVP = file->get_var("MONTH");
  dayVP = file->get_var("DAY");

printf("Looking for LATC....\n");
  latVP = file->get_var("LATC");
printf("Looking for LONC....\n");
  lonVP = file->get_var("LONC");
printf("Looking for TKAT....\n");
  trkVP = file->get_var("TKAT");
printf("Looking for DRFTA....\n");
  driftVP = file->get_var("DRFTA");
printf("Looking for THDG....\n");
  hdgVP = file->get_var("THDG");
printf("Looking for PALT....\n");
  altVP = file->get_var("PALT");
printf("Looking for WSC....\n");
  wsVP = file->get_var("WSC");
printf("Looking for WDC....\n");
  wdVP = file->get_var("WDC");
printf("Looking for TASX....\n");
  tasVP = file->get_var("TASX");
printf("Looking for GSF....\n");
  gspdVP = file->get_var("GSF");
printf("Looking for SOLZE....\n");
  zenVP = file->get_var("SOLZE");
printf("Looking for SOLAZ....\n");
  azimVP = file->get_var("SOLAZ");
printf("Looking for HGM232....\n");
  rad_altVP = file->get_var("HGM232");
printf("Looking for PITCH....\n");
  pitchVP = file->get_var("PITCH");
printf("Looking for ROLL....\n");
  rollVP = file->get_var("ROLL");

}

/* -------------------------------------------------------------------- */
void AircraftData::LoadCorrectionData(FlightClock& currTime)
{
  long	currentRecord = currTime.Seconds() - startTime.Seconds();

  if (currentRecord < 0)
    currentRecord += 86400;

  if (currTime.Seconds() > 86400)
    printf("LoadCorrectionData: time = %s %d %d %d\n", currTime.c_str(),
	currTime.Seconds(), startTime.Seconds(), currentRecord);

  yearVP->set_cur(currentRecord);	yearVP->get(&year, 1);
  monthVP->set_cur(currentRecord);	monthVP->get(&month, 1);
  dayVP->set_cur(currentRecord);	dayVP->get(&day, 1);
  hourVP->set_cur(currentRecord);	hourVP->get(&hour, 1);
  minVP->set_cur(currentRecord);	minVP->get(&minute, 1);
  secVP->set_cur(currentRecord);	secVP->get(&second, 1);
  gspdVP->set_cur(currentRecord);	gspdVP->get(&gspd, 1);
  driftVP->set_cur(currentRecord);	driftVP->get(&drft, 1);
  rad_altVP->set_cur(currentRecord);	rad_altVP->get(&radAlt, 1);
  pitchVP->set_cur(currentRecord);	pitchVP->get(&pitch, 1);
  rollVP->set_cur(currentRecord);	rollVP->get(&roll, 1);
  latVP->set_cur(currentRecord);	latVP->get(&lat, 1);
  lonVP->set_cur(currentRecord);	lonVP->get(&lon, 1);
  trkVP->set_cur(currentRecord);	trkVP->get(&trk, 1);
  tasVP->set_cur(currentRecord);	tasVP->get(&tas, 1);
  hdgVP->set_cur(currentRecord);	hdgVP->get(&hdg, 1);
  altVP->set_cur(currentRecord);	altVP->get(&alt, 1);
  wsVP->set_cur(currentRecord);		wsVP->get(&ws, 1);
  wdVP->set_cur(currentRecord);		wdVP->get(&wd, 1);
  zenVP->set_cur(currentRecord);	zenVP->get(&zen, 1);
  azimVP->set_cur(currentRecord);	azimVP->get(&azim, 1);

}

/* -------------------------------------------------------------------- */
bool AircraftData::NextTimeSegment(FlightClock& start, FlightClock& end)
{
  if (currSeg < nSegs)
    {
    start = startSeg[currSeg];
    end = endSeg[currSeg];

    ++currSeg;

    return(true);
    }
  else
    {
    return(false);
    }

}	/* END NEXTTIMESEGMENT */

/* -------------------------------------------------------------------- */
void AircraftData::SearchStraightAndLevel()
{
  int	i, j, nRecords, wp3Cnt, rollCnt, nSex;
  NcValues	*alt, *roll, *hour, *second, *minute, *wp3, *gspd, *track;

  float	altMean, altMin, altMax, altSigma;
  float	hdgMean, hdgMin, hdgMax, hdgSigma;
  float	spdMean, spdMin, spdMax, spdSigma;
  float	trackMean, trackMin, trackMax, trackSigma;

  const int START_CNT = 30;

  nRecords = file->rec_dim()->size();

  std::cout << "Number records in netCDF = " << nRecords << ".\n";
  std::cout << "Reading data" << std::flush;

  wp3 = file->get_var("WP3")->values();
  std::cout << "." << std::flush;

  alt = file->get_var("PALT")->values();
  std::cout << "." << std::flush;

  roll = file->get_var("ROLL")->values();
  std::cout << "." << std::flush;

  gspd = file->get_var("GSF")->values();
  std::cout << ".\n" << std::flush;

  track = file->get_var("TKAT")->values();
  std::cout << ".\n" << std::flush;


  for (i = 180; i < nRecords; ++i)
    if (gspd->as_float(i) > startGSF)
      break;

  for (; i < nRecords; ++i)
    if (alt->as_float(i) > startALT)
      break;

  if (i == nRecords)
    {
    std::cerr << "Never surpassed " << startALT << " meters, exiting....\n";
    exit(1);
    }

  hour = file->get_var("HOUR")->get_rec(i);
  minute = file->get_var("MINUTE")->get_rec(i);
  second = file->get_var("SECOND")->get_rec(i);

  printf("Altitude surpassed %d meters @ %02d:%02d:%02d\n\n", (int)startALT,
	hour->as_short(0), minute->as_short(0), second->as_short(0));


  std::cout << "\t\t\tMean    Sigma       Min        Max\n";

  do
    {
    wp3Cnt = 0;
    rollCnt = 0;

    // Locate 20 seconds of reasonable data.
    for (; i < nRecords && (wp3Cnt < START_CNT || rollCnt < START_CNT); ++i)
      {
      if (fabs(wp3->as_float(i)) < delta_wp3)
        ++wp3Cnt;
      else
        wp3Cnt = 0;

      if (fabs(roll->as_float(i)) < delta_roll)
        ++rollCnt;
      else
        rollCnt = 0;
      }

    if (i >= nRecords)
      break;

    i -= START_CNT;

    for (; roll->as_float(i) == -32767.0; ++i)
      ;

    hour = file->get_var("HOUR")->get_rec(i);
    minute = file->get_var("MINUTE")->get_rec(i);
    second = file->get_var("SECOND")->get_rec(i);

    printf("%02d:%02d:%02d - ",
	hour->as_short(0), minute->as_short(0), second->as_short(0));

    startSeg[nSegs] = hour->as_short(0) * 3600 + minute->as_short(0) * 60
	+ second->as_short(0);

    /* Found 15 seconds of reasonably flat data.
     */
    altMean = 0.0;
    altMin = FLT_MAX;
    altMax = -FLT_MAX;

    hdgMean = 0.0;
    hdgMin = FLT_MAX;
    hdgMax = -FLT_MAX;

    spdMean = 0.0;
    spdMin = FLT_MAX;
    spdMax = -FLT_MAX;

    trackMean = 0.0;
    trackMin = FLT_MAX;
    trackMax = -FLT_MAX;

    wp3Cnt = rollCnt = nSex = 0;

    for (; i < nRecords && wp3Cnt < 15 && rollCnt < 5; ++i, ++nSex)
      {
      if (roll->as_float(i) == -32767.0)
        break;

      if (alt->as_float(i) < 200)	// Bail below 200 meters.
        break;

      altMean += alt->as_float(i);
      altMin = std::min(altMin, alt->as_float(i));
      altMax = std::max(altMax, alt->as_float(i));

      hdgMean += roll->as_float(i);
      hdgMin = std::min(hdgMin, roll->as_float(i));
      hdgMax = std::max(hdgMax, roll->as_float(i));

      spdMean += gspd->as_float(i);
      spdMin = std::min(spdMin, gspd->as_float(i));
      spdMax = std::max(spdMax, gspd->as_float(i));

      trackMean += track->as_float(i);
      trackMin = std::min(trackMin, track->as_float(i));
      trackMax = std::max(trackMax, track->as_float(i));

      if (fabs(wp3->as_float(i)) >= delta_wp3)
        ++wp3Cnt;
      else
        wp3Cnt = 0;

      if (fabs(roll->as_float(i)) >= delta_roll)
        ++rollCnt;
      else
        rollCnt = 0;
      }

    altMean /= nSex;
    hdgMean /= nSex;
    spdMean /= nSex;
    trackMean /= nSex;

    altSigma = hdgSigma = spdSigma = trackSigma = 0.0;

    for (j = nSex; j > 0; --j)
      {
      altSigma += pow(alt->as_float(i-j) - altMean, 2.0);
      hdgSigma += pow(roll->as_float(i-j) - hdgMean, 2.0);
      spdSigma += pow(gspd->as_float(i-j) - spdMean, 2.0);
      trackSigma += pow(track->as_float(i-j) - trackMean, 2.0);
      }

    if (wp3Cnt == 15)
      i -= 15;
    else
    if (rollCnt == 5)
      i -= 5;
    else
    if (nSex > 0)
      --i;

    hour = file->get_var("HOUR")->get_rec(i);
    minute = file->get_var("MINUTE")->get_rec(i);
    second = file->get_var("SECOND")->get_rec(i);

    printf("%02d:%02d:%02d",
	hour->as_short(0), minute->as_short(0), second->as_short(0));

    endSeg[nSegs] = hour->as_short(0) * 3600 + minute->as_short(0) * 60
	+ second->as_short(0);


    if (alt->as_float(i) < 200.0)
      {
      std::cout << ", broken by low altitude.";

      while (alt->as_float(++i) < 200.0)
        ;
      }

    if (wp3Cnt == 15)
      {
      i += 15;
      std::cout << ", broken by Altitude.";
      }
    else
    if (rollCnt == 5)
      {
      i += 5;
      std::cout << ", broken by Roll.";
      }
    else
      {
      i++;
      std::cout << ", broken possibly by time gap.\n";
      }

    meanTrack[nSegs] = trackMean;
    meanGSpeed[nSegs] = spdMean;
    meanRoll[nSegs] = hdgMean;
    meanAlt[nSegs] = altMean;
meanAlt[nSegs] = altMax;

    if (nSex > 60)
      {
      printf("\n\t\tAlt: %7.2f %10.4f %10.2f %10.2f\n",
		altMean, (float)sqrt(altSigma / (nSex - 1)), altMin, altMax);

      printf("\t\tRol: %7.2f %10.4f %10.2f %10.2f\n",
		hdgMean, (float)sqrt(hdgSigma / (nSex - 1)), hdgMin, hdgMax);

      printf("\t\tSpd: %7.2f %10.4f %10.2f %10.2f\n",
		spdMean, (float)sqrt(spdSigma / (nSex - 1)), spdMin, spdMax);

      printf("\t\tTrk: %7.2f %10.4f %10.2f %10.2f\n\n",
		trackMean, (float)sqrt(trackSigma / (nSex - 1)),
		trackMin, trackMax);

      ++nSegs;
      }
    else
      printf("  Rejected.\n");
    }
  while (i < nRecords);

}
