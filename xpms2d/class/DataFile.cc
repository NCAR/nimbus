/*
-------------------------------------------------------------------------
OBJECT NAME:	DataFile.cc

FULL NAME:	ADS Data File Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2001
-------------------------------------------------------------------------
*/

#include "DataFile.h"

#include <unistd.h>
#include <algorithm>
#include <vector>

static P2d_rec PtestRecord, CtestRecord, HtestRecord;

static unsigned long PtestParticle[] = {
0xffffffff,
0xff000134,
0x55000000,

0xfffc3fff,
0xfff81fff,
0xfff00fff,
0xfff00fff,

0xfff00fff,
0xfff00fff,
0xfff81fff,
0xfffc3fff,

0xffffffff,
0xffffffff
 };

static unsigned long CtestParticle[] = {
0xffffffff,
0xff000e00,
0x55000000,

0xfff81fff,
0xffe007ff,
0xffc003ff,
0xff8001ff,
0xff8001ff,
0xff8001ff,
0xff0000ff,
0xff0000ff,

0xff0000ff,
0xff0000ff,
0xff8001ff,
0xff8001ff,
0xff8001ff,
0xffc003ff,
0xffe007ff,
0xfff81fff,

0xffffffff,
0xffffffff
 };

static unsigned short HtestParticle[] = {
  0x8000,
  0x807a,

  0x427e,
  0x437d,
  0x447c,
  0x447c,
  0x447c,
  0x447c,
  0x437d,
  0x427e,
  };

static const size_t P2dLRpPR = 1;

/* -------------------------------------------------------------------- */
ADS_DataFile::ADS_DataFile(char fName[])
{
  _hdr = 0;
  _fileName = fName;

  if (_fileName.find(".gz", 0) != std::string::npos)
    {
    gzipped = true;
    printf("We have gzipped file.\n");
    }
  else
    gzipped = false;

#ifdef PNG
  if (gzipped)
    gz_fd = gzopen(_fileName.c_str(), "rb");
  else
#endif
    fp = fopen(_fileName.c_str(), "rb");

  if ((gzipped && gz_fd <= 0) || (!gzipped && fp == NULL))
    {
    sprintf(buffer, "Can't open file %s", _fileName.c_str());
    ErrorMsg(buffer);
    return;
    }

  useTestRecord = false;

#ifdef PNG
  if (gzipped)
    {
    gzread(gz_fd, buffer, 100);
    gzseek(gz_fd, 0, SEEK_SET);	// gzrewind does not seem to work on
    }
  else
#endif
    {
    fread(buffer, 100, 1, fp);
    rewind(fp);
    }

  if (strstr(buffer, "<PMS2D>") )
    initADS3();	// the XML header file.
  else
  if (isValidProbe(buffer))
    {
    _fileHeaderType = NoHeader;
    printf("No RAF header found, assuming raw PMS2D file.\n");
    }
  else
    initADS2();

  buildIndices();

  currLR = -1; currPhys = 0;

  if (ProjectNumber().compare("812") == 0)
    fprintf(stderr, "DataFile.cc: Lake-ICE - Bit shift correction will be performed on all records.\n");

  if (ProjectNumber().compare("135") == 0)
    fprintf(stderr, "DataFile.cc: RICO - Stuck bit correction will be performed.\n");

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void ADS_DataFile::initADS2()
{
  int	Ccnt, Pcnt, Hcnt;
  Ccnt = Pcnt = Hcnt = 0;

  _fileHeaderType = ADS2;

  _hdr = new Header(_fileName.c_str());
  _projectName = _hdr->ProjectNumber();
  _flightNumber = _hdr->FlightNumber();
  _flightDate = _hdr->FlightDate();

  if (_hdr->isValid() == false)
    return;

  for (const void * p = _hdr->GetFirst("PMS2D"); p; p = _hdr->GetNext("PMS2D"))
    {
    const char * name = _hdr->VariableName((Pms2 *)p);

    if (name[3] == 'P')
      _probeList.push_back(new Probe(_hdr, (Pms2 *)p, ++Pcnt));
    else
    if (name[3] == 'C')
      _probeList.push_back(new Probe(_hdr, (Pms2 *)p, ++Ccnt));
    else
    if (name[3] == 'H')
      _probeList.push_back(new Probe(_hdr, (Pms2 *)p, ++Hcnt));
    }
}

/* -------------------------------------------------------------------- */
void ADS_DataFile::initADS3()
{
  std::string XMLgetElementValue(const char s[]);

  _fileHeaderType = PMS2D;

  while (fgets(buffer, 512, fp))
    {
    if ( strstr(buffer, "</PMS2D>\n") )
      break;
    if ( strstr(buffer, "<Project>") )
      _projectName = XMLgetElementValue(buffer);
    else
    if ( strstr(buffer, "<FlightNumber>") )
      _flightNumber = XMLgetElementValue(buffer);
    else
    if ( strstr(buffer, "<FlightDate>") )
      _flightDate = XMLgetElementValue(buffer);
    else
    if ( strstr(buffer, "<probe") )
      _probeList.push_back(new Probe(buffer, PMS2_SIZE));
    }
}

/* -------------------------------------------------------------------- */
void ADS_DataFile::ToggleSyntheticData()
{
  int		i, j;
  unsigned long		*p;
  unsigned short	*s;

  useTestRecord = 1 - useTestRecord;

  strcpy((char *)&CtestRecord.id, "C1");
  CtestRecord.hour = 12;
  CtestRecord.minute = 30;
  CtestRecord.second = 30;
  CtestRecord.tas = 100 * 255 / 125;
  CtestRecord.msec = 100;
  CtestRecord.overld = 36;

  p = (unsigned long *)CtestRecord.data;
  for (i = 0; i < 48; ++i)
    for (j = 0; j < 21; ++j)
      *p++ = CtestParticle[j];

  for (j = 0; j < 16; ++j)
    *p++ = CtestParticle[j];


  strcpy((char *)&PtestRecord.id, "P1");
  PtestRecord.hour = 12;
  PtestRecord.minute = 30;
  PtestRecord.second = 30;
  PtestRecord.tas = 100 * 255 / 125;
  PtestRecord.msec = 100;
  PtestRecord.overld = 30;

  p = (unsigned long *)PtestRecord.data;
  for (i = 0; i < 78; ++i)
    for (j = 0; j < 13; ++j)
      *p++ = PtestParticle[j];

  for (j = 0; j < 10; ++j)
    *p++ = PtestParticle[j];


  strcpy((char *)&HtestRecord.id, "H1");
  HtestRecord.hour = 12;
  HtestRecord.minute = 30;
  HtestRecord.second = 30;
  HtestRecord.tas = 100 * 255 / 125;
  HtestRecord.msec = 100;
  HtestRecord.overld = 30;

  s = (unsigned short *)HtestRecord.data;
  for (i = 0; i < 204; ++i)
    for (j = 0; j < 10; ++j)
      *s++ = HtestParticle[j];

  for (j = 0; j < 8; ++j)
    *s++ = HtestParticle[j];

}	/* END TOGGLESYNTHETICDATA */

/* -------------------------------------------------------------------- */
void ADS_DataFile::SetPosition(int position)
{
  size_t	pos;
  P2d_rec	buff;

  pos = nIndices * position / 100;

  if (pos == nIndices)
    --pos;

  int hour = ntohs(indices[pos].time[0]);

  if (ntohs(indices[0].time[0]) > 12 && hour < 12)
    hour += 24;

  LocatePMS2dRecord(&buff, hour,
	ntohs(indices[pos].time[1]), ntohs(indices[pos].time[2]));

}	/* END SETPOSITION */

/* -------------------------------------------------------------------- */
bool ADS_DataFile::LocatePMS2dRecord(P2d_rec *buff, int hour, int minute, int second)
{
  int	i;
  bool	rc, startPreMidnight = False;

  if (ntohs(indices[0].time[0]) > 12)
    startPreMidnight = True;

  for (i = 1; indices[i].index > 0 && ntohs(indices[i].time[0]) < hour; ++i)
    if (startPreMidnight && ntohs(indices[i].time[0]) < 12 && hour > 12)
      hour -= 24;

  for (; indices[i].index > 0; ++i)
    {
    if (ntohs(indices[i].time[0]) < hour)
      continue;
    if (ntohs(indices[i].time[1]) < minute)
      continue;

    break;
    }

  for (; indices[i].index > 0 && ntohs(indices[i].time[2]) < second; ++i)
    ;

  if (indices[i].index == -1)
    return(false);

  currPhys = std::max(0, i - 2);
  currLR = P2dLRpPR;
  rc = NextPMS2dRecord(buff);

  while (rc && ntohs(buff->second) < second)
    rc = NextPMS2dRecord(buff);

  SwapPMS2D(buff);
  return(rc);

}	/* END LOCATEPMS2DRECORD */

/* -------------------------------------------------------------------- */
bool ADS_DataFile::FirstPMS2dRecord(P2d_rec *buff)
{
  currPhys = currLR = 0;

  if (useTestRecord)
    {
    memcpy((char *)buff, (char *)&CtestRecord, sizeof(P2d_rec));
    return(true);
    }

  if (indices[0].index == -1)	// No 2d records in file.
    return(false);

#ifdef PNG
  if (gzipped)
    {
    gzseek(gz_fd, indices[0].index, SEEK_SET);
    gzread(gz_fd, physRecord, sizeof(P2d_rec) * P2dLRpPR);
    }
  else
#endif
    {
    fseek(fp, indices[0].index, SEEK_SET);
    fread(physRecord, sizeof(P2d_rec), P2dLRpPR, fp);
    }

  memcpy((char *)buff, (char *)physRecord, sizeof(P2d_rec));
  SwapPMS2D(buff);
  return(true);

}	/* END FIRSTPMS2DRECORD */

/* -------------------------------------------------------------------- */
bool ADS_DataFile::NextPMS2dRecord(P2d_rec *buff)
{
  if (useTestRecord)
    {
    if (testRecP == &CtestRecord)
      testRecP = &PtestRecord;
    else
    if (testRecP == &PtestRecord)
      testRecP = &HtestRecord;
    else
      testRecP = &CtestRecord;

    testRecP->msec += 80;
    if (testRecP->msec >= 1000)
      {
      testRecP->msec -= 1000;
      if (++testRecP->second > 59)
        ++testRecP->minute;
      }

    memcpy((char *)buff, (char *)testRecP, sizeof(P2d_rec));
    return(true);
    }

  if (indices[currPhys].index == -1)
    return(false);

  if (++currLR >= P2dLRpPR)
    {
    currLR = 0;

    if (indices[++currPhys].index == -1)
      {
      --currPhys;
      return(false);
      }

#ifdef PNG
    if (gzipped)
      {
      gzseek(gz_fd, indices[currPhys].index, SEEK_SET);
      gzread(gz_fd, physRecord, sizeof(P2d_rec) * P2dLRpPR);
      }
    else
#endif
      {
      fseek(fp, indices[currPhys].index, SEEK_SET);
      fread(physRecord, sizeof(P2d_rec), P2dLRpPR, fp);
      }
    }

  memcpy((char *)buff, (char *)&physRecord[currLR], sizeof(P2d_rec));
  SwapPMS2D(buff);
  return(true);

}	/* END NEXTPMS2DRECORD */

/* -------------------------------------------------------------------- */
bool ADS_DataFile::PrevPMS2dRecord(P2d_rec *buff)
{
  if (useTestRecord)
    {
    if (testRecP == &CtestRecord)
      testRecP = &PtestRecord;
    else
      testRecP = &CtestRecord;

    testRecP->msec -= 80;
    if (testRecP->msec < 0)
      {
      testRecP->msec += 1000;
      if (--testRecP->second < 0)
        --testRecP->minute;
      }

    memcpy((char *)buff, (char *)testRecP, sizeof(P2d_rec));
    return(false);
    }

  if (--currLR < 0)
    {
    currLR = P2dLRpPR-1;

    if (--currPhys < 0)
      {
      currLR = 0;
      currPhys = 0;
      return(false);
      }

#ifdef PNG
    if (gzipped)
      {
      gzseek(gz_fd, indices[currPhys].index, SEEK_SET);
      gzread(gz_fd, physRecord, sizeof(P2d_rec) * P2dLRpPR);
      }
    else
#endif
      {
      fseek(fp, indices[currPhys].index, SEEK_SET);
      fread(physRecord, sizeof(P2d_rec), P2dLRpPR, fp);
      }
    }

  memcpy((char *)buff, (char *)&physRecord[currLR], sizeof(P2d_rec));
  SwapPMS2D(buff);
  return(true);

}	/* END PREVPMS2DRECORD */

/* -------------------------------------------------------------------- */
int ADS_DataFile::NextPhysicalRecord(char buff[])
{
  int	rc, idWord;
  int	size = sizeof(short);

#ifdef PNG
  if (gzipped)
    {
    savePos = gztell(gz_fd);
    rc = gzread(gz_fd, buff, size) / size;
    }
  else
#endif
    {
    savePos = ftell(fp);
//  if ((nBytes = read(fileno(fp), buff, size)) == 0 && GetNextADSfile())
    rc = fread(buff, size, 1, fp);
    }

  if (rc != 1)
    return(0);

  idWord = ntohs(*((unsigned short *)buff));

  switch (idWord)
    {
    case SDI_WORD:
      size = (_hdr->lrLength() * _hdr->lrPpr()) - sizeof(short);
      break;

    case ADS_WORD:
      size = 18;
      break;

    case HDR_WORD:
      size = _hdr->HeaderLength() - sizeof(short);
      break;

    case 0x4d43:      // MCR
      size = sizeof(Mcr_rec) - sizeof(short);
      break;

    case PMS2DC1: case PMS2DC2: // PMS2D
    case PMS2DP1: case PMS2DP2:
    case PMS2DH1: case PMS2DH2: // HVPS
    case PMS2DC4: case PMS2DC6:	// 64 bit 2DC.
      size = (P2dLRpPR * sizeof(P2d_rec)) - sizeof(short);
      break;

    case PMS2DG1: case PMS2DG2: /* GrayScale */
      size = 32000 - sizeof(short);
      break;

    default:
      size = 0;
    }

#ifdef PNG
  if (gzipped)
    {
    if (gzread(gz_fd, &buff[sizeof(short)], size) != size)
      size = 0;
    }
  else
#endif
    {
    if (fread(&buff[sizeof(short)], size, 1, fp) != 1)
      size = 0;
    }

  return(size + sizeof(short));

}	/* END NEXTPHYSICALRECORD */

/* -------------------------------------------------------------------- */
void ADS_DataFile::buildIndices()
{
  size_t	cnt;
  int	rc;
  short	*word;
  FILE	*fpI;
  char	buffer[0x8000], *p, tmpFile[256];


  strcpy(tmpFile, _fileName.c_str());

  if ((p = strstr(tmpFile, ".2d")) == NULL)
    strcat(tmpFile, ".2Didx");
  else
    strcpy(p, ".2Didx");


  // Check for indices file....
  if ( (fpI = fopen(tmpFile, "rb")) )
    {
    long	len;

    fseek(fpI, 0, SEEK_END);
    len = ftell(fpI);

    if ((indices = (Index *)malloc(len)) == NULL)
      {
      fprintf(stderr, "buildIndices: Memory allocation error, fatal.\n");
      exit(1);
      }

    printf("Reading indices file, %s.\n", tmpFile);

    rewind(fpI);
    fread(indices, len, 1, fpI);
    fclose(fpI);

    if (5 != ntohl(5))		// If Intel architecture, swap bytes.
      for (size_t i = 0; i < len / sizeof(Index); ++i)
        indices[i].index = ntohl(indices[i].index);

    nIndices = (len / sizeof(Index)) - 1;
    return;
    }


  printf("Building indices...."); fflush(stdout);
  FlushEvents();

  word = (short *)buffer;

  if ((indices = (Index *)malloc(8000000 * sizeof(Index))) == NULL)
    {
    fprintf(stderr, "buildIndices: Memory allocation error, fatal.\n");
    exit(1);
    }

  if (_fileHeaderType != PMS2D)
    {
#ifdef PNG
    if (gzipped)
      gzseek(gz_fd, 0, SEEK_SET);
    else
#endif
      rewind(fp);
    }


  for (cnt = 0; (rc = NextPhysicalRecord(buffer)); )
    {
    size_t i;

    if (_fileHeaderType != NoHeader)
      {
      for (i = 0; i < _probeList.size(); ++i)
        if (strcmp(_probeList[i]->Code(), buffer) == 0)
          break;

      if (i == _probeList.size())	// shouldn't get here?
        continue;
      }
    else
      {
      for (i = 0; i < _probeList.size(); ++i)
        if (strcmp(_probeList[i]->Code(), buffer) == 0)
          break;

      // Sanity check.
      if (!isValidProbe(buffer))
        continue;

      if (i == _probeList.size())
        _probeList.push_back(new Probe(buffer));
      }

    for (i = 0; i < 1; ++i)
      {
      indices[cnt].index = savePos + (sizeof(P2d_rec) * i);
      memcpy(indices[cnt].time, &buffer[2], 6);
      ++cnt;
      }
    }

  if (_fileHeaderType != PMS2D)
    {
#ifdef PNG
    if (gzipped)
      gzseek(gz_fd, 0, SEEK_SET);
    else
#endif
      rewind(fp);
    }

  printf("\n%d 2d records were found.\n", cnt);

  indices[cnt].index = -1;

  if ((indices = (Index *)realloc(indices, (cnt+1) * sizeof(Index))) == NULL)
    {
    fprintf(stderr, "buildIndices: Memory re-allocation error, fatal.\n");
    exit(1);
    }

//  SortIndices(cnt);

/* Write new 2d file.  Used to fix IDEAS-III files that were out of time order.
    {

    }
 */


  // Write indices to a file for future use.
  if ( _fileHeaderType != NoHeader && (fpI = fopen(tmpFile, "w+b")) )
    {
    printf("Writing indices file, %s.\n", tmpFile);

    if (5 != ntohl(5))		// If Intel architecture, swap bytes.
      for (size_t i = 0; i < cnt+1; ++i)
        indices[i].index = htonl(indices[i].index);

    fwrite(indices, (cnt+1) * sizeof(Index), 1, fpI);
    fclose(fpI);

    if (5 != ntohl(5))		// Swap em back for current run.
      for (size_t i = 0; i < cnt+1; ++i)
        indices[i].index = ntohl(indices[i].index);
    }

  nIndices = cnt;

}	/* END BUILDINDICES */

/* -------------------------------------------------------------------- */
void ADS_DataFile::SwapPMS2D(P2d_rec *buff)
{
  // Perform byte swapping on whole [data] record if required.
  if (1 != ntohs(1))
  {
    unsigned long       *p;
    unsigned short       *sp = (unsigned short *)buff;

    for (int i = 1; i < 10; ++i)
      sp[i] = ntohs(sp[i]);


    if (htons(buff->id) == PMS2DC4 || htons(buff->id) == PMS2DC6)	// Fast 2DC
    {
      long long *lp = (long long *)buff->data;
      for (size_t i = 0; i < 512; ++i, ++lp)
        *lp = ntohll(lp);
    }
    else
    if (((char *)&buff->id)[0] == 'H')	// HVPS
    {
      sp = (unsigned short *)buff->data;
      for (size_t i = 0; i < 2048; ++i, ++sp)
        *sp = ntohs(*sp);
    }
    else
    {
      p = (unsigned long *)buff->data;
      for (size_t i = 0; i < RecordLen; ++i, ++p)
        *p = ntohl(*p);
    }
  }

  /* Code for Lake-ICE data, which was shifted 1 bit from overclocking
   * the data lines.  Clocking was fine, except the data lines from the
   * wing tips was too long, so we had to cut in half.  This was the
   * first project with ADS2 PMS2D card.
   */
  if (ProjectNumber().compare("812") == 0)
  {
    unsigned long *p = (unsigned long *)buff->data;

    for (size_t i = 0; i < RecordLen; ++i, ++p)
    {
      // It only matters to fix the sync & timing words.
      if (*p == 0xff800000)
      {
        *p = SyncWordMask;
        *(p-1) <<= *(p-1);
      }
    }
  }

  /* RICO stuck bit cleanup.
   */
  if (ProjectNumber().compare("135") == 0)
  {
    check_rico_half_buff(buff, 0, RecordLen/2);
    check_rico_half_buff(buff, RecordLen/2, RecordLen);
  }

}

/* -------------------------------------------------------------------- */
bool ADS_DataFile::isValidProbe(char *pr)
{
  // Sanity check.
  if ((pr[0] == 'C' || pr[0] == 'P' || pr[0] == 'H') && isdigit(pr[1]))
    return(true);

  return(false);
}

/* -------------------------------------------------------------------- */
long long ADS_DataFile::ntohll(long long * p) const
{
    union {
      long long v;
      char b[8];
    } u;
    const char* cp = (const char*)p;
    for (int i = 7; i >= 0; i--) u.b[i] = *cp++;
    return u.v;
}

/* -------------------------------------------------------------------- */
void ADS_DataFile::SortIndices(int cnt)
{
  sort_the_table(0, cnt-1);

}       /* SORTTABLE */

/* -------------------------------------------------------------------- */
void ADS_DataFile::sort_the_table(int beg, int end)
{
  Index	*mid, temp;
  int	x = beg, y = end;

  mid = &indices[(x + y) / 2];

  while (x <= y)
    {
    while (memcmp(indices[x].time, mid->time, 6) < 0)
      ++x;

    while (memcmp(indices[y].time, mid->time, 6) > 0)
      --y;

    if (x <= y)
      {
      temp = indices[x];
      indices[x] = indices[y];
      indices[y] = temp;

      ++x;
      --y;
      }
    }

  if (beg < y)
    sort_the_table(beg, y);

  if (x < end)
    sort_the_table(x, end);
}

/* -------------------------------------------------------------------- */
void ADS_DataFile::check_rico_half_buff(P2d_rec *buff, size_t beg, size_t end)
{
  std::vector<size_t> spectra, sorted_spectra;

  for (size_t j = 0; j < 32; ++j)
    spectra.push_back(0);

  // Generate spectra.
  unsigned long *p = (unsigned long *)buff->data;
  bool firstSyncWord = beg == 0 ? false : true;

  for (size_t i = beg; i < end; ++i, ++p)
  {
    // There seemed to be lots of splatter at the start of the buffer,
    // skip until first sync word appears.
    if (!firstSyncWord)
    {
      if ((*p & SyncWordMask) == 0x55000000)
        firstSyncWord = true;
      else
        continue;
    }

    if ((*p & SyncWordMask) == 0x55000000 || *p == 0xffffffff)
      continue;

    unsigned long slice = ~(*p);
    for (size_t j = 0; j < 32; ++j)
      if (((slice >> j) & 0x01) == 0x01)
        ++spectra[j];
  }

  // Sort the spectra and compare the last bin to the one next too it and see
  // if there is a large descrepency.  If so, probably a bad 1/2 buffer.
  sorted_spectra = spectra;
  sort(sorted_spectra.begin(), sorted_spectra.end());

  if ((sorted_spectra[30] * 2 < sorted_spectra[31]))
  {
    int stuck_bin = -1;
    for (size_t j = 0; j < 32; ++j)
      if (spectra[j] == sorted_spectra[31])
        stuck_bin = j;

    fprintf(stderr,
	"DataFile.cc: %02d:%02d:%02d.%d - Performing stuck bit correction, bit %d, ",
	buff->hour, buff->minute, buff->second, buff->msec, stuck_bin);

    if (beg == 0)
      fprintf(stderr, "first half.\n");
    else
      fprintf(stderr, "second half.\n");

    if (stuck_bin == -1)
    {
      fprintf(stderr, "DataFile.cc:  Impossible.\n");
      exit(1);
    }

    unsigned long mask1 = (0x01 << stuck_bin);
    unsigned long mask2 = (0x07 << stuck_bin-1);
    unsigned long *p = (unsigned long *)buff->data;
    for (size_t i = beg; i < end; ++i, ++p)
    {
      if ((*p & SyncWordMask) == 0x55000000 || *p == 0xffffffff)
        continue;

      unsigned long slice = ~(*p);
      if ((slice & mask2) == mask1)
        *p = ~(slice & ~mask1);
    }
  }
}

/* -------------------------------------------------------------------- */
ADS_DataFile::~ADS_DataFile()
{
  free(indices);

  delete _hdr;
  _hdr = 0;

  for (size_t i = 0; i < _probeList.size(); ++i)
    delete _probeList[i];

#ifdef PNG
  if (gzipped)
    gzclose(gz_fd);
  else
#endif
    fclose(fp);

}	/* END DESTRUCTOR */

/* END DATAFILE.CC */
