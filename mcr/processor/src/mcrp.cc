/*
-------------------------------------------------------------------------
OBJECT NAME:	mcrp.cc

FULL NAME:	MCR Processor

ENTRY POINTS:	main()

STATIC FNS:	NextMCRfile()

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-2001
-------------------------------------------------------------------------
*/

#include "mcr.h"
#include "adsIO.h"
#include "DataBuffer.h"
#include "Netcdf.h"
#include "OutFile.h"
#include <unistd.h>

Mcr_rec	mcrRec;

static char	netCDFfileName[1024], mcrFileName[1024], outputFileName[1024],
		buffer[2048];
static ADS_DataFile	*file;

static bool NextMCRfile();

static FlightClock	prevTime;


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	i, j, processChannel[N_CHANNELS];
  FlightClock	StartTime, EndTime, currTime;
  DataBuffer	*scan[N_CHANNELS];

  nice(4);

  std::cout << "RAF MCR processor.\n\n";

  std::cout << "Enter MCR file name (e.g. /jnet/data/101rf05) :";
  gets(mcrFileName);

  std::cout << "Enter netCDF file name (e.g. /jnet/productiondata/101rf05) :";
  gets(netCDFfileName);

  std::cout << "Enter output file name (e.g. /jnet/data/MCRrf16) :";
  gets(outputFileName);
/*
strcpy(mcrFileName, "/jnet/data/migs/krista/101rf01");
strcpy(netCDFfileName, "/jnet/data/migs/krista/101Lrf01b");
strcpy(outputFileName, "/jnet/data/cjw");
*/
  if (strstr(mcrFileName, ".rawMCRa") == NULL)
    strcat(mcrFileName, ".rawMCRa");

  if (strstr(netCDFfileName, ".nc") == NULL)
    strcat(netCDFfileName, ".nc");


  std::cout << "Which channels would you like processed (e.g. 13456) :";
  gets(buffer);

  for (i = 0; i < N_CHANNELS; ++i)
    processChannel[i] = false;

  for (i = 0; i < strlen(buffer); ++i)
    processChannel[buffer[i]-'1'] = true;


  AircraftData	netCDF(netCDFfileName);
  netCDF.SearchStraightAndLevel();

  file = new ADS_DataFile(mcrFileName);

  for (i = 0; i < N_CHANNELS; ++i)
    scan[i] = new DataBuffer(file->hdr, i);


  file->FirstMCRRecord(&mcrRec);

  // Blast past first minute of data (bad timestamps).
  for (i = 0; i < 7*60; ++i)
    file->NextMCRRecord(&mcrRec);
/*
NextMCRfile();
file->setPos(223372556);
file->NextMCRRecord(&mcrRec);
*/

  // Loop through each straight & Level segment.
  do
    {
    // Get next segment, and position input file there.
    if (!netCDF.NextTimeSegment(StartTime, EndTime))
      break;

    std::cout << "Processing " << StartTime.c_str() << " - " << EndTime.c_str()
		<< ".\n locating first MCR record.\n";

    // Get to start of segment.
    while (ntohs(mcrRec.hour) > 23 ||
		(currTime = file->HdrBlkToSeconds(mcrRec)) < StartTime)
      {
      if (file->NextMCRRecord(&mcrRec) == false)
        if (NextMCRfile() == false)
          {
          std::cerr<<"Failed to locate start of segment at "<<StartTime.c_str()<<"\n";
          return(0);
          }
        else
          std::cerr << "Next MCR file " << currTime.c_str() << "\n";
      }

std::cout << " pos = " << file->filePos() << ", @ " << currTime.c_str() << "\n";
    prevTime = currTime - 1;

    OutFile	outFile(outputFileName, StartTime, EndTime);
    outFile.WriteHeader(file->hdr, netCDF);

    std::cout << " starting processing.\n";

    do
      {
      std::cout << " \r" << currTime.c_str(); fflush(stdout);

      if (currTime != prevTime + 1 && (prevTime != 86399L && currTime != 0))
        {
        FlightClock	tpTime = prevTime;

        for (int seqCnt = 0; seqCnt < 3; )
          {
          for (j = 0; j < 7; ++j)
            if (file->NextMCRRecord(&mcrRec) == false)
              if (NextMCRfile() == false)
                break;

          if ((currTime = file->HdrBlkToSeconds(mcrRec)) == tpTime + 1)
            ++seqCnt;
          else
            seqCnt = 0;

          tpTime = currTime;
          }

        std::cerr << "\nA time anomaly has occured, @ " << prevTime.c_str() <<
		", advancing to " << currTime.c_str() << ".\n";

        outFile.IncrementCurrentScan(netCDF.GSpeed() * (currTime - prevTime));

        for (i = 0; i < N_CHANNELS; ++i)
          scan[i]->ClearAllScans();
        }

      prevTime = currTime;

      // Process all channels for each second.
      while (currTime == file->HdrBlkToSeconds(mcrRec))
        {
        // Channels 4&5 were switched in hardware for INDOEX, switch em back.
/*
        if (strcmp(file->hdr->ProjectNumber(), "102") == 0)
          {
          if (mcrRec.id[4] == '3') mcrRec.id[4] = '4';
          if (mcrRec.id[4] == '4') mcrRec.id[4] = '3';
          }
*/
        if (processChannel[mcrRec.id[4] - '0'])
          scan[mcrRec.id[4] - '0']->ProcessRecord(&mcrRec);

        if (file->NextMCRRecord(&mcrRec) == false)
          if (NextMCRfile() == false)
            break;

        // Channels 4&5 were switched in hardware for INDOEX, switch em back.
/*
        if (strcmp(file->hdr->ProjectNumber(), "102") == 0)
          {
          if (mcrRec.id[4] == '3') mcrRec.id[4] = '4';
          if (mcrRec.id[4] == '4') mcrRec.id[4] = '3';
          }
*/
        // Firing order in SEHBA was 1032546, all others, 0123456.
        if (strcmp(file->hdr->ProjectNumber(), "101") == 0)
          {
          if (mcrRec.id[4] == '1')
            break;
          }
        else if (mcrRec.id[4] == '0')
          break;
        }

      if (currTime < StartTime)
        currTime += 86400;

      netCDF.LoadCorrectionData(currTime);

      // Perform pitch/roll/heading corrections on current scans.  We only
      // need to do this for channel 0, the rest are the same.
      scan[0]->ComputePositionForData(netCDF);
      outFile.ApplyScanToFile(scan, netCDF);

      for (i = 0; i < N_CHANNELS; ++i)
        scan[i]->RemoveFullScans();
      }
    while ((currTime = file->HdrBlkToSeconds(mcrRec)) <= EndTime ||
		(StartTime > EndTime && currTime > 70000));

    std::cout << "\n cleaning up file.\n"; fflush(stdout);
    outFile.InterpMissingData(processChannel);
    }
  while (1);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
static bool NextMCRfile()
{
  delete file;

  ++(mcrFileName[strlen(mcrFileName)-1]);

  file = new ADS_DataFile(mcrFileName);

  std::cout << "   \nChanging to MCR file " << mcrFileName << ".\n";

  return(file->FirstMCRRecord(&mcrRec));

}	/* END NEXTMCRFILE */

/* END MCRP.CC */
