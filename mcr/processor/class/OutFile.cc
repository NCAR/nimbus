//#define CHAR_FILE

#define N_RECS	200

#include "OutFile.h"

struct _storage {
        char    name[8];
        float   waveLength;
        float   bandWidth;
        float   ifov;
    } channelInfo[] = {
        { "1", 0.64, 0.064, 0.007 },
        { "2", 0.761, 0.001, 0.007 },
        { "3", 0.763, 0.001, 0.007 },
        { "4", 1.061, 0.066, 0.007 },
        { "5", 1.64, 0.047, 0.007 },
        { "6", 2.16, 0.08, 0.007 },
        { "7", 10.9, 0.85, 0.007 } } ;

#include <algorithm>

static const int	recLength = sizeof(struct _recP);
static const int	nPixInWidth = MAX_WIDTH;


/* -------------------------------------------------------------------- */
OutFile::OutFile(const char fileName[], FlightClock& start, FlightClock& end)
{
  char		outName[1024], *p;
  NcAtt		*attr;

  scanNumber = 0;

  if (end < start)
    nOutRecords = end.Seconds() + (86400 - start.Seconds());
  else
    nOutRecords = end.Seconds() - start.Seconds();

  data = NULL;
  firstRecord = nRecords = lastRecUsed = 0;
  planeOffset = 0.0;

  sprintf(outName, "%s.%s-%s", fileName, start.c_str(), end.c_str());

  /* Remove ':' for the Winblows crowd.
   */
  while ((p = strchr(outName, ':')))
    strcpy(p, p+1);


  if ((outFP = fopen(outName, "w+b")) == NULL)
    {
    std::cerr << "Can't open output file " << outName << ".\n";
    }

  startPos = sizeof(MCR_HDR) + (sizeof(MCR_CHAN_INFO) * N_CHANNELS);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void OutFile::WriteHeader(Header *ADSheader, AircraftData& acData)
{
  int	i, j;
  MCR_CHAN_INFO chan;
 
  strcpy(hdr.instrumentName, "MCR");
  strcpy(hdr.projectName, ADSheader->ProjectName());
  strcpy(hdr.platformName, ADSheader->Aircraft());
  strcpy(hdr.flightNumber, ADSheader->FlightNumber());
 
  hdr.nBytes_header = sizeof(MCR_HDR) + (N_CHANNELS * sizeof(MCR_CHAN_INFO));
  hdr.recordLength = sizeof(struct _recP);
  hdr.pixPerRecord = nPixInWidth;
  hdr.byteOrdering = 1;		// big endian.
  hdr.missingValue = -32767;
  hdr.nChannels = N_CHANNELS;
 
  rewind(outFP);
  fwrite((void *)&hdr, sizeof(MCR_HDR), 1, outFP);
 
  for (i = 0; i < N_CHANNELS; ++i)
    {
    strcpy(chan.name, channelInfo[i].name);
    chan.waveLength = channelInfo[i].waveLength;
    chan.bandWidth = channelInfo[i].bandWidth;
    chan.ifov = channelInfo[i].ifov;
    chan.swathWidth = 2.0 * acData.MeanAlt() *
			cos(acData.MeanRoll() * M_PI / 180.0);
 
    chan.dataType = 4;
    chan.descriptor = 0;
    chan.slope = 0;
    chan.intercept = 0;
 
    chan.headerOffset =
		i * (sizeof(MCR_CHANNEL_HDR) + sizeof(float) * nPixInWidth);
    chan.dataOffset = chan.headerOffset + sizeof(MCR_CHANNEL_HDR);
 
    fwrite((void *)&chan, sizeof(MCR_CHAN_INFO), 1, outFP);
 
 
    // Also init Channel headers while we're here.
    strcpy(nextRec.chan[i].chHdr.name, channelInfo[i].name);
    nextRec.chan[i].chHdr.waveLength = channelInfo[i].waveLength;
    nextRec.chan[i].chHdr.dataQuality = 0;
    }


  pixLength = pixWidth = 2.0 * acData.MeanAlt() *
			cos(acData.MeanRoll() * M_PI / 180.0) / nPixInWidth;

  if (pixLength > 20)
    {
    pixLength += 1.0;
    pixWidth += 1.0;
    }

  // Create blank record for MoreRecords().
  for (i = 0; i < nPixInWidth; ++i)
    for(j = 0; j < N_CHANNELS; ++j)
      nextRec.chan[j].dat[i] = -32767.0;
 
  nextRec.recHdr.recordNumber = 0;
  nextRec.recHdr.scanRate = 3.47;
  nextRec.recHdr.pixWidth = pixWidth;
  nextRec.recHdr.pixLength = pixLength;

std::cout << "  file(R, C) = " << nOutRecords*4 << ", " << nPixInWidth << "\n";
std::cout << "  pix(W, L) = " << pixWidth << ", " << pixLength << "\n";
std::cout << "  recLength = " << recLength << "\n";

  nOutRecords = 0;

  LoadRecords(0, 300);

}       /* END WRITEHEADER */

/* -------------------------------------------------------------------- */
void OutFile::ApplyScanToFile(DataBuffer *scans[], AircraftData& acData)
{
  int	i, j, scan, chan, whichCol, idx;
  long	startRecNum, endRecNum, whichRec;
  float	minMeters, maxMeters;
  struct _recP *recP;

  for (scan = 0; scan < scans[0]->nScans(); ++scan)
    {
    minMeters = FLT_MAX;
    maxMeters = -FLT_MAX;

    // Find min/max meter offsets, so we can figure out which block of
    // records we need.
    for (i = 0; i < scans[0]->nSamplesPerScan[scan]; ++i)
      {
      minMeters = std::min(minMeters, scans[0]->scan[scan][i].y);
      maxMeters = std::max(maxMeters, scans[0]->scan[scan][i].y);
      }

    // First time through determine recordOffset, we don't want 50 records
    // of blank crap at the beginning of each file.  Add slop of 3 records.
    if (scanNumber == 0)
      planeOffset = -minMeters + (pixLength * 3);

    startRecNum = (int)((planeOffset + minMeters) / pixLength);
    endRecNum = (int)((planeOffset + maxMeters) / pixLength) + 2;

    LoadRecords(startRecNum, endRecNum);
    recP = (struct _recP *)data;

    for (i = 0; i < scans[0]->nSamplesPerScan[scan]; ++i)
      {
      whichRec = (int)((planeOffset + scans[0]->scan[scan][i].y) / pixLength +
			0.5) - firstRecord;

      whichCol = (int)(scans[0]->scan[scan][i].x / pixWidth + 0.5);

      if (whichRec < 0 || whichRec >= nRecords)
        {
        printf(" >> exceeded Rec#, strt=%d, end=%d, nRecs=%d, whichRec=%d\n",
		startRecNum, endRecNum, nRecords, whichRec);
        continue;
        }

      idx = (nPixInWidth >> 1) + whichCol;

      if (idx <= 0 || recP[whichRec].recHdr.year == 0)
        fillACdata(&recP[whichRec].recHdr, acData);

      if (idx < 0 || idx >= nPixInWidth)
        continue;

      lastRecUsed = std::max(whichRec, lastRecUsed);

      for (chan = 0; chan < N_CHANNELS; ++chan)
        {
        recP[whichRec].chan[chan].dat[idx] = scans[chan]->scan[scan][i].pixel;
        }
      }

    planeOffset += acData.GSpeed() / 3.47;	// Plane movement
    ++scanNumber;
    }

}	/* END APPLYSCANTOFILE */

/* -------------------------------------------------------------------- */
void OutFile::InterpMissingData(int processChannel[])
{
  int	i, j, k, l, m, diff, cnt;
  float	x, av;
  long	startRecNum, endRecNum, start, end, prevRec = -1;
  char	*data1;
  struct _recP	*inP, *outP;

  // Flush any records left in memory.
  nOutRecords = firstRecord + lastRecUsed + 1;
  FlushRecords();
printf("  nOutRecords = %d\n", nOutRecords);

#ifdef CHAR_FILE

char	charBuf[MAX_WIDTH];
FILE	*char_fp = fopen("/home/data/charFile", "w+");

#endif

  if ((data = new char [N_RECS * recLength]) == NULL)
    {
    std::cerr << "OutFile::CleanUpFile: out of memory\n";
    exit(1);
    }

  if ((data1 = new char [N_RECS * recLength]) == NULL)
    {
    std::cerr << "OutFile::CleanUpFile: out of memory\n";
    exit(1);
    }


  startRecNum = 0;
  endRecNum = std::min(nOutRecords, (long)N_RECS);

  inP = (struct _recP *)data1;
  outP = (struct _recP *)data;


  while (startRecNum < nOutRecords)
    {
    nRecords = endRecNum - startRecNum;
    fseek(outFP, startPos + startRecNum * recLength, SEEK_SET);
    fread(data, recLength, nRecords, outFP);

    memcpy(data1, data, nRecords * recLength);

    for (i = 2; i < nRecords-2; ++i)
      {
//printf("record # = %d, %02d:%02d:%02d\n", inP[i].recHdr.recordNumber,
//	inP[i].recHdr.hour, inP[i].recHdr.minute, inP[i].recHdr.second);
      for (k = 2; k < nPixInWidth-2; ++k)
        {
        if (inP[i].chan[0].dat[k] == -32767.0)
          {
          for (j = 0; j < N_CHANNELS; ++j)
            {
            if (processChannel[i] == false)
              continue;

            av = 0.0;
            cnt = 0;

            for (l = i-2; l < i+2; ++l)
              for (m = k-2; m < k+2; ++m)
                if (inP[l].chan[j].dat[m] != -32767.0)
                  {
                  av += inP[l].chan[j].dat[m];
                  ++cnt;

                  if (l != 0 && m != 0 && l != 4 && m != 4)
                    {
                    av += inP[l].chan[j].dat[m];
                    ++cnt;
                    }
                  }

            if (cnt)
              outP[i].chan[j].dat[k] = av / cnt;
            }
          }
        }
      }

#ifdef CHAR_FILE

for (i = 0; i < nRecords-2; ++i)
  if (outP[i].recHdr.recordNumber > prevRec)
    {
    for (k = 0; k < nPixInWidth; ++k)
      if (outP[i].chan[0].dat[k] == -32767.0)
        charBuf[k] = 119;
      else
        charBuf[k] = (char)(outP[i].chan[0].dat[k] * 15.0);

    fwrite(charBuf, nPixInWidth, 1, char_fp);
    prevRec = outP[i].recHdr.recordNumber;
    }

#endif

    fseek(outFP, startPos + startRecNum * recLength, SEEK_SET);
    fwrite(data, recLength, nRecords, outFP);

    startRecNum += N_RECS - 4;
    endRecNum += N_RECS - 4;

    if (endRecNum >= nOutRecords)
      endRecNum = nOutRecords - 1;
    }

#ifdef CHAR_FILE
fclose(char_fp);
#endif

  delete [] data;
  delete [] data1;
  data = NULL;

}	/* END INTERPMISSINGDATA */

/* -------------------------------------------------------------------- */
OutFile::~OutFile()
{
  fclose(outFP);

  if (data)
    delete [] data;
}

/* -------------------------------------------------------------------- */
void OutFile::FlushRecords()
{
  if (data)
    {
    int	rc;

    fseek(outFP, startPos + firstRecord * recLength, SEEK_SET);
    if ((rc = fwrite(data, recLength, lastRecUsed+1, outFP)) != lastRecUsed+1)
      {
      if (errno == ENOSPC)
        {
        std::cerr << "\nERROR: Disk full.\n";
        exit(1);
        }
      }

    delete [] data;
    }

  data = NULL;

}	/* END FLUSHRECORDS */

/* -------------------------------------------------------------------- */
void OutFile::LoadRecords(int start, int end)
{
  int	rc;

  if (start >= firstRecord && end <= firstRecord + nRecords)
    return;

  // Ok, we went outside records currently in memory, write em out, and load
  // new ones.
  FlushRecords();

  // Load more than requested.
  start -= 2;
  end += N_RECS;

  if (start < 0)
    start = 0;

  if (start > firstRecord + lastRecUsed)
    start = firstRecord + lastRecUsed + 1;

  firstRecord = start;
  nRecords = end - firstRecord;

  if ((data = new char [nRecords * recLength]) == NULL)
    {
    std::cerr << "OutFile::LoadRecords: out of memory\n";
    exit(1);
    }

  fseek(outFP, startPos + (firstRecord * recLength), SEEK_SET);

  if (ftell(outFP) != startPos + (firstRecord * recLength))
    {
    std::cerr << "fseek failure, current pos " << ftell(outFP) << ", requested "
	<< startPos + (firstRecord * recLength) << ".\n";

    std::cerr << startPos << " + " << firstRecord << " * " << recLength << "\n";
    }

  if ((rc = fread(data, recLength, nRecords, outFP)) != nRecords)
    {
    nOutRecords = (ftell(outFP) - startPos) / recLength;
    MoreRecords(rc, nRecords);
    }

  lastRecUsed = 0;
 
}       /* END LOADRECORDS */

/* -------------------------------------------------------------------- */
void OutFile::MoreRecords(int nRead, int nRequested)
{
  int	i, j;

  nextRec.recHdr.recordNumber = nOutRecords;

  for (i = nRead; i < nRequested; ++i)
    {
    memcpy(&data[i*recLength], (char *)&nextRec, recLength);
    nextRec.recHdr.recordNumber++;
    }
 
  nOutRecords += nRequested - nRead;

}	/* END MORERECORDS */

/* -------------------------------------------------------------------- */
void OutFile::fillACdata(MCR_REC_HDR *recHdr, AircraftData& acData)
{
  recHdr->year = (short)acData.Year();
  recHdr->month = (short)acData.Month();
  recHdr->day = (short)acData.Day();
  recHdr->hour = (short)acData.Hour();
  recHdr->minute = (short)acData.Minute();
  recHdr->second = (short)acData.Second();

  recHdr->latitude = acData.Lat();
  recHdr->longitude = acData.Lon();
  recHdr->track = acData.Track();
  recHdr->heading = acData.Heading();
  recHdr->AGL_alt = 0.0;
  recHdr->MSL_alt = acData.Alt();
  recHdr->ws = acData.WS();
  recHdr->wd = acData.WD();
  recHdr->tas = acData.TAS();
  recHdr->gspd = acData.GSpeed();
  recHdr->solarZenith = acData.Zenith();
  recHdr->solarAzimuth = acData.Azimuth();

}	/* END FILLACDATA */

/* END OUTFILE.CC */
