/*
-------------------------------------------------------------------------
OBJECT NAME:	DataBuffer.cc

FULL NAME:	

ENTRY POINTS:	DataBuffer()

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#include "DataBuffer.h"

#include <algorithm>
#include <cmath>
#include <netinet/in.h>


/* -------------------------------------------------------------------- */
DataBuffer::DataBuffer(const char proj_num[])
{
  char  *proj_dir;
  char	buffer[1024];

  currentChannel = 0;
  imageIdx = outSampleIdx = 0;
  inSampleIdx = 0;
  Vref = 0.0;
  projNum = atoi(proj_num);	// Need this because housekeeping was changed

  if ((proj_dir = getenv("PROJ_DIR")) == NULL)
    {
    fprintf(stderr, "PROJ_DIR undefined.\n");
    exit(1);
    }

  if (isdigit(proj_num[0]))
    sprintf(buffer, "%s/%s/header", proj_dir, proj_num);
  else
    strcpy(buffer, proj_num);
 
  hdr = new Header(buffer);
  fp = NULL;
 
  for (int i = 0; i < 7;  ++i)
    {
    sprintf(buffer, "MCR%d", i + 1);
    vp[i] = (Sh *)hdr->GetSDI(buffer);

    startVolt[i] = 0.0;
    endVolt[i] = 5.0;

    if (i > 5)
      {
      startVolt[i] = -5.0;
      endVolt[i] = 5.0;
      }
    }

  theVP = vp[0];

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void DataBuffer::SetChannel(int newChannel)
{
  if (newChannel >= 0 && newChannel < 8);
    theVP = vp[(currentChannel = newChannel)];
  
  imageIdx = 0;
  outSampleIdx = 0;

}	/* END SETCHANNEL */

/* -------------------------------------------------------------------- */
void DataBuffer::CountsToVolts(Mcr_rec *rec)
{
  int	i;

  // Convert to voltage.
  for (i = 0; i < SampleRate; ++i)
    {
//    volts[i] = (((short *)rec->data)[i] -
//	hdr->AtoDconversionOffset(theVP)) * hdr->AtoDconversionFactor(theVP);
    volts[i] = ((short)ntohs(((short *)rec->data)[i]) -
	hdr->AtoDconversionOffset(theVP)) * hdr->AtoDconversionFactor(theVP);
    }

//printf("%d %f\n", hdr->AtoDconversionOffset(theVP), hdr->AtoDconversionFactor(theVP));
}	/* END COUNTSTOVOLTS */

/* -------------------------------------------------------------------- */
bool DataBuffer::MakeImages()
{
  int	corder, k;
  float	x, out, voltDiff, colorFactor;

  voltDiff = endVolt[currentChannel] - startVolt[currentChannel];
  colorFactor = 120 / voltDiff;		// nColors / voltage difference.
  peak = -11.0;

  for (; inSampleIdx < SampleRate; )
    {
    if (outSampleIdx == 0)
      {
      inSampleIdx = SearchNextScan(volts, inSampleIdx);
      SearchHousekeeping(inSampleIdx);
      }

    while (outSampleIdx < nSamplesPerScan && inSampleIdx < SampleRate)
      {
      // Apply cal coes.
      corder = hdr->CalibrationOrder(theVP) - 1;
      out    = hdr->CalibrationCoefficient(theVP, corder);

      if (currentChannel < 5)
        x = volts[inSampleIdx++];
      else
        x = volts[inSampleIdx++] - Vref;

      for (k = 1; k < hdr->CalibrationOrder(theVP); k++)
        out = hdr->CalibrationCoefficient(theVP, corder-k) + x * out;

      if (currentChannel == 6)
        out += Bref;

      peak = std::max(peak, out);

      out -= startVolt[currentChannel];

      if (out < 0.0)
        out = 0.0;

      imageData[imageIdx][outSampleIdx++] = (char)(out * colorFactor);
      }

    if (outSampleIdx == nSamplesPerScan)
      {
      ++imageIdx;		// Next image scan.
      outSampleIdx = 0;
      }

    if (imageIdx > 19)
      break;
    }


  if (inSampleIdx >= SampleRate)
    inSampleIdx -= SampleRate;

  if (imageIdx > 10)
    return(true);	// Buffers filling up, time to display.
  else
    return(false);

}	/* END MAKEIMAGES */

/* -------------------------------------------------------------------- */
int DataBuffer::SearchNextScan(float v[], int idx)
{
  // Search for next negative sample (which signifies beginning of scan).
  while (idx < SampleRate && v[idx++] >= -9.0)
    ;

  while (idx < SampleRate && v[idx++] < -5.0)
    ;

  if (idx >= SampleRate)
    return(SampleRate);
  else
    return(idx + 11);

}	/* END SEARCHNEXTSCAN */

/* -------------------------------------------------------------------- */
void DataBuffer::SearchHousekeeping(int idx)
{
  if (idx > 3950)
    return;

  if (currentChannel < 5)
    idx += 600;  // skip active scan & most of next section.
  else
    {
    idx += 340;

    // Wait for 10 volt spike (S/F).
    while (volts[idx++] < 9.5)
      ;

    // Wait for drop below 10 volts.
    while (volts[idx++] > 9.0)
      ;

    /* Average 2nd part of scan (S/F) as baseline to subtract from active scan.
     */
    Vref = 0.0;
    idx += 10;
    for (int i = 0; i < 170; ++i)
      Vref += volts[idx++];

    Vref /= 170;
//printf("Vref = %f\n", Vref);
    }


  // Wait for 10 volt spike (C/T).
  while (volts[idx++] < 9.5)
    ;

  // Wait for drop below 10 volts.
  while (volts[idx++] > 9.0)
    ;

  if (projNum == 101)
    {
    idx += 57;  // move 10 ms. + middle of samples.

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

  if (currentChannel == 6)
    {
    Bref = 124.189 / (exp(1320.0 / (hskp[0]+273.15)) - 1.0);
//printf("     Bref = %f, %f\n", Bref, hskp[0]);
    }

}	/* END SEARCHHOUSEKEEPING */

/* -------------------------------------------------------------------- */
void DataBuffer::StartRecording(char *dataFile)
{
  char	buff[1024];

  sprintf(buff, "%s-%d", dataFile, currentChannel+1);

  if ((fp = fopen(buff, "w+")) == NULL)
    fprintf(stderr, "mcrtd: Can't open ASCII file %s\n", buff);

  recordCounter = 0;

}	/* END STARTRECORDING */

/* -------------------------------------------------------------------- */
void DataBuffer::StopRecording()
{
  if (fp)
    {
    fclose(fp);
    fp = NULL;
    }

  recordCounter = 0;

}	/* END STOPRECORDING */

/* -------------------------------------------------------------------- */
void DataBuffer::RecordData(Mcr_rec *rec)
{
  if (fp == NULL)
    return;


  // Only record a specific # of records.
  if (recordCounter++ > RecordsToRecord)
    {
    StopRecording();
    return;
    }


  int	i, channel = rec->id[4] - '0';
  float v[5000];
  static int inputPos[7], activeCtr[7];


  if (recordCounter == 0)
    {
    for (i = 0; i < 7; ++i)
      inputPos[i] = activeCtr[i] = 0;
    }


  for (i = 0; i < SampleRate; )
    {
    if (activeCtr[channel] == 0)
      i = inputPos[channel] = SearchNextScan(volts, inputPos[channel]);

    for (; i < SampleRate && activeCtr[channel] < nSamplesPerScan;
			++i, ++activeCtr[channel], ++inputPos[channel])
      {
      fprintf(fp, "%02d:%02d:%02d.%-4d   %e",
		rec->hour, rec->minute, rec->second,
		inputPos[channel] << 1, volts[inputPos[channel]]);

      // Print Thermal/Near IR
      if (activeCtr[channel] < 288 && i+380 < SampleRate)
        fprintf(fp, "  %e", volts[inputPos[channel]+380]);

      if (activeCtr[channel] == 0)
        PrintThermalVolts(activeCtr[channel]);

      fprintf(fp, "\n");
      }

    if (activeCtr[channel] == nSamplesPerScan)
      activeCtr[channel] = 0;
    }

  if (inputPos[channel] >= SampleRate)
    inputPos[channel] -= SampleRate;

}	/* END RECORDDATA */

/* -------------------------------------------------------------------- */
void DataBuffer::RemoveFullScans()
{
  memcpy((char *)imageData[0],
	 (char *)imageData[imageIdx],
	 outSampleIdx * sizeof(float));

  imageIdx = 0;

}	/* END REMOVEFULLSCANS */

/* -------------------------------------------------------------------- */
void DataBuffer::PrintThermalVolts(int idx)
{
  if (idx > 3950)
    return;
 
  idx += 600;  // skip active scan & most of next section.
 
  // Wait for 10 volt spike (C/T).
  while (volts[idx++] < 9.5)
    ;
 
  // Wait for drop below 10 volts.
  while (volts[idx++] > 9.0)
    ;
 
  fprintf(fp, "  %e  %e", volts[idx+13], volts[idx + (int)(13*26.79)]);
 
}       /* END SEARCHHOUSEKEEPING */

/* END DATABUFFER.CC */
