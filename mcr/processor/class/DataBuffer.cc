/*
-------------------------------------------------------------------------
OBJECT NAME:	DataBuffer.cc

FULL NAME:	

ENTRY POINTS:	DataBuffer()

STATIC FNS:	

DESCRIPTION:	Process input file.

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-2001
-------------------------------------------------------------------------
*/

#include "DataBuffer.h"


/* -------------------------------------------------------------------- */
DataBuffer::DataBuffer(Header *hdr, int chan) : channel(chan), adsHdr(hdr)
{
  char	buffer[64], *p;
  FILE	*fp;
  int	i;

  scanIdx = 0;
  outSampleIdx = 0;
  inSampleIdx = 0;
  polyOrder = 0;
  Vref = Bref = 0.0;
  projNum = atoi(adsHdr->ProjectNumber());

  sprintf(buffer, "MCR%d", channel + 1);
  theVP = (Sh *)adsHdr->GetSDI(buffer);

  if ((fp = fopen("mcrCals", "r")))
    {
    for (i = 0; i < 7; ++i)
      {
      fgets(buffer, 64, fp);

      if (i == channel)
        {
        p = strtok(buffer, ", ");
        for (polyOrder = 0; p; ++polyOrder)
          {
          cals[polyOrder] = atof(p);
          p = strtok(NULL, ", ");
          }
        }
      }

    fclose(fp);
    }

  printf("Channel %d cals = ", channel);

  for (i = 0; i < polyOrder; ++i)
    printf("%e ", cals[i]);

  printf("\n");

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void DataBuffer::ProcessRecord(Mcr_rec *rec)
{
  int	corder, k, i, endIdx;
  float	x, out;

  // Convert to voltage.
  for (i = 0; i < SampleRate; ++i)
    {
    volts[i] = (float)((short)ntohs(((short *)rec->data)[i]) -
     adsHdr->AtoDconversionOffset(theVP)) * adsHdr->AtoDconversionFactor(theVP);
    }

  if (outSampleIdx > 0 && inSampleIdx == 0)
    endIdx = SearchEndScan(volts, inSampleIdx);

  for (; inSampleIdx < SampleRate; )
    {
    if (outSampleIdx == 0)
      {
      inSampleIdx = SearchStartNextScan(volts, inSampleIdx);
      endIdx = SearchEndScan(volts, inSampleIdx+340);
      milliSec[scanIdx] = inSampleIdx / 5;
      SearchHousekeeping(inSampleIdx);
      }

    while (inSampleIdx < endIdx && inSampleIdx < SampleRate)
      {
      // Apply cal coes.
      corder = polyOrder - 1;
      out    = cals[corder];

      if (channel < 5)
        x = volts[inSampleIdx++];
      else
        x = volts[inSampleIdx++] - Vref;

      for (k = 1; k < polyOrder; k++)
        out = cals[corder-k] + x * out;

      if (channel == 6)
        out += Bref;

      scan[scanIdx][outSampleIdx++].pixel = out;

      if (outSampleIdx >= MAX_SAMPLES)
        {
        printf("\n%s scan longer than %d samples, skipping.\n", rec->id, MAX_SAMPLES);
        break;
        }
      }

    if (outSampleIdx >= MAX_SAMPLES)
      {
      outSampleIdx = 360;

      for (k = 0; k < outSampleIdx; ++k)
        scan[scanIdx][k].pixel = 0.0;
      }

    if (inSampleIdx < SampleRate)
      {
      if (outSampleIdx > 375)
        printf("\n%s scan longer than 375 nSamples = %d.\n", rec->id, outSampleIdx);

      nSamplesPerScan[scanIdx] = outSampleIdx - 2;
      ++scanIdx;		// Next image scan.
      outSampleIdx = 0;
      }
    }


  if (inSampleIdx >= SampleRate)
    inSampleIdx -= SampleRate;

}	/* END PROCESSRECORDS */

/* -------------------------------------------------------------------- */
void DataBuffer::ComputePositionForData(AircraftData& netCDF)
{
  int		i, j;
  double	r, y, theta, tpr, sp, sd, st, cp, cd, ct;
  double	pitch, roll, drift, spdIncrmnt;

  spdIncrmnt = netCDF.GSpeed() / SampleRate;

  for (i = 0; i < nScans(); ++i)
    {
    theta = 225.0;

    for (j = 0; j < nSamplesPerScan[i]; ++j)
      {
      pitch = netCDF.Pitch();
      roll = netCDF.Roll();
      drift = netCDF.Drift();

      // Compute distance straight to ground.
      y = netCDF.Alt() / cos(roll * DegToRad);

      theta -= 90.0 / nSamplesPerScan[i];
      tpr = theta + roll;

      sp = sin(pitch * DegToRad);
      sd = sin(drift * DegToRad);
      st = sin(tpr * DegToRad);

      cp = cos(pitch * DegToRad);
      cd = cos(drift * DegToRad);
      ct = cos(tpr * DegToRad);

      // Compute distance to current sample on the ground.
      r = fabs(y / ct);
      scan[i][j].x = r * ((ct * sd * sp) + (cd * st));
      scan[i][j].y = r * ((-ct * cd * sp) + (sd * st)) + (j * spdIncrmnt);

//printf("x, y = %.2f %.2f, INS: P=%.2f, R=%.2f, D=%.2f, A=%.1f\n", scan[i][j].x, scan[i][j].y, pitch, roll, drift, netCDF.RadarAlt());
      }
    }

}	/* END COMPUTEPOSITIONFORDATA */

/* -------------------------------------------------------------------- */
int DataBuffer::SearchStartNextScan(float v[], int idx)
{
  // Search for next negative sample (which signifies beginning of scan).
  while (idx < SampleRate && v[idx++] >= -9.5)
    ;

  while (idx < SampleRate && v[idx++] < -5.0)
    ;

  if (idx >= SampleRate)
    return(SampleRate);
  else
    return(idx + 3);

}	/* END SEARCHNEXTSCAN */

/* -------------------------------------------------------------------- */
int DataBuffer::SearchEndScan(float v[], int idx)
{
  if (idx >= SampleRate)
    return(SampleRate);

  while (idx < SampleRate && v[idx++] < 9.5)
    ;

  if (idx >= SampleRate)
    return(SampleRate);

  return(idx - 3);

}

/* -------------------------------------------------------------------- */
void DataBuffer::SearchHousekeeping(int idx)
{
  if (idx > 3950)
    return;

  if (channel >= 5)	// Average thermal body for channel 6 cal.
    {
    idx += 350;  // skip to end of active scan.

    // Wait for 10 volt spike (S/F)
    while (volts[idx++] < 9.5)
      ;

    // Wait for drop below 10 volts.
    while (volts[idx++] > 9.0)
      ;

    Vref = 0.0;
    idx += 10;
    for (int i = 0; i < 250 ; ++i)
      Vref += volts[idx++];

    Vref /= 250;
    }
  else
    idx += 600;  // skip to end of thermal body.


  // Wait for 10 volt spike (C/T).
  while (volts[idx++] < 9.5)
    ;

  // Wait for drop below 10 volts.
  while (volts[idx++] > 9.0)
    ;

  if (projNum == 101)
    {
    idx += 57;	// move 10 ms. + middle of samples.

    for (int i = 0; i < 24; ++i)
      {
      hskp[i] = volts[idx + (int)(i * 12.5)];

      if (i < 21)
        hskp[i] = hskp[i] * 100.0 - 273.15;
      }
    }
  else
    {
    idx += 50 + 11;       // move 10 ms. + middle of samples.

    for (int i = 0; i < 14; ++i)
      {
      hskp[i] = volts[idx + (int)(i * 23.22)];

      if (i == 10)
        hskp[i] = hskp[i] * -6.80 + 73.0;
      else
        hskp[i] = hskp[i] * 100.0 - 273.15;
      }
    }

  if (channel == 6)
    {
    Bref = 124.189 / (exp(1320.0 / (hskp[0]+273.15)) - 1.0);
    }

}	/* END SEARCHHOUSEKEEPING */

/* -------------------------------------------------------------------- */
void DataBuffer::ClearAllScans()
{
  scanIdx = 0;
  outSampleIdx = 0;
  inSampleIdx = 0;

}       /* END REMOVEFULLSCANS */

/* -------------------------------------------------------------------- */
void DataBuffer::RemoveFullScans()
{
  scan[0] = scan[scanIdx];
  scanIdx = 0;

}       /* END REMOVEFULLSCANS */

/* END DATABUFFER.CC */
