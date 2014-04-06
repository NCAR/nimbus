/*
-------------------------------------------------------------------------
OBJECT NAME:	padsinfo.cc

FULL NAME:	PADS CIP File Info

DESCRIPTION:	Display information from a NCAR PMS2D/OAP file, including
		indivdual record headers.

COPYRIGHT:	University Corporation for Atmospheric Research, 2012-14
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>

#include <netinet/in.h>

struct pads_rec {
  int16_t year;
  int16_t month;
  int16_t day;
  int16_t hour;
  int16_t minute;
  int16_t second;
  int16_t msec;
  int16_t wday;
  unsigned char data[4096];		/* image buffer */
};
typedef struct pads_rec pads_rec;


using namespace std;

unsigned char buffer[50000];

class Config
{
public:
  Config() : fullHex(false), histo(false) { }

  bool	fullHex;
  bool	histo;

  string sourceFile;
} cfg;

void Output(const unsigned char buff[]);
void ParticleCount(pads_rec *p2d, size_t nDiodes);


void Usage()
{
  cerr << "Usage: padsinfo [-h] [-x] [-c] file.2d" << endl;
  cerr << "  -x: Full hex dump of each record." << endl;
  cerr << "  -c: histogram count totals, per record and per second." << endl;
  exit(1);
}

/* -------------------------------------------------------------------- */
void processArgs(int argc, char *argv[])
{
  if (argc < 2)
    Usage();

  for (size_t aCnt = 1; aCnt < argc; ++aCnt)
  {
    if (argv[aCnt][0] == '-')
    {
      if (strcmp(argv[aCnt], "-x") == 0)
        cfg.fullHex = true;
      else
      if (strcmp(argv[aCnt], "-c") == 0)
        cfg.histo = true;
      else
        Usage();
    }
    else
      cfg.sourceFile = argv[aCnt];
  }

  if (cfg.sourceFile.length() == 0)
    Usage();
}


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  FILE *fp;

  processArgs(argc, argv);

  if ((fp = fopen(cfg.sourceFile.c_str(), "rb")) == NULL)
  {
    cerr << "Unable to open input file : " << cfg.sourceFile << endl;
    exit(1);
  }

  while (fread(buffer, sizeof(pads_rec), 1, fp) == 1)
    Output(buffer);

  fclose(fp);

  return 0;

}	/* END MAIN */

/* -------------------------------------------------------------------- */
// DMT CIP/PIP probes are run length encoded.  Decode here.
int uncompress(unsigned char *dest, const unsigned char src[], int nbytes)
{
  int nslices = 0, d_idx = 0;

  static size_t nResidualBytes = 0;
  static unsigned char residualBytes[16];

  if (nResidualBytes)
  {
    memcpy(dest, residualBytes, nResidualBytes);
    d_idx = nResidualBytes;
    nResidualBytes = 0;
  }

  for (int i = 0; i < 4096; ++i)
  {
    unsigned char b = src[i];

    int nBytes = (b & 0x1F) + 1;

    if ((b & 0x20))	// This is a dummy byte; for alignment purposes.
    {
      continue;
    }

    if ((b & 0xE0) == 0)
    {
      memcpy(&dest[d_idx], &src[i+1], nBytes);
      d_idx += nBytes;
      i += nBytes;
    }

    if ((b & 0x80))
    {
      memset(&dest[d_idx], 0, nBytes);
      d_idx += nBytes;
    }
    else
    if ((b & 0x40))
    {
      memset(&dest[d_idx], 0xFF, nBytes);
      d_idx += nBytes;
    }
  }

  if (d_idx % 8)
  {
    size_t idx = d_idx / 8 * 8;
    nResidualBytes = d_idx % 8;
    memcpy(residualBytes, &dest[idx], nResidualBytes);
  }

  return d_idx / 8;	// return number of slices.
}

/* -------------------------------------------------------------------- */
void Output(const unsigned char buff[])
{
  pads_rec *p2d = (pads_rec *)buffer;
  size_t nDiodes = 64;

  if (cfg.histo)
    ParticleCount(p2d, nDiodes);
  else
  {
    cout << dec;
    cout << setfill('0') << setw(4) << p2d->year << '/';
    cout << setfill('0') << setw(2) << p2d->month << '/';
    cout << setfill('0') << setw(2) << p2d->day << " ";
    cout << setfill('0') << setw(2) << p2d->hour << ':';
    cout << setfill('0') << setw(2) << p2d->minute << ':';
    cout << setfill('0') << setw(2) << p2d->second;
    cout << '.' << setfill('0') << setw(3) << (p2d->msec) << endl;
  }

  if (cfg.fullHex)
  {
    size_t bytesPerSlice = nDiodes / 8;
    unsigned char image[60000];

    size_t nSlices = uncompress(image, p2d->data, 4096);
cout << "nSlices="<<nSlices<<endl;

    for (int i = 0; i < nSlices; ++i)
    {
      for (int j = bytesPerSlice-1; j >= 0; --j)
      {
        cout << hex << setw(2) << setfill('0') << (int)image[(i*bytesPerSlice) + j];
      }
      cout << endl;
    }
  }
}

/* -------------------------------------------------------------------- */
void OutputParticleCount(pads_rec *p, size_t counts[], size_t n, bool output_msec)
{
  int total = 0;

  cout << setw(2) << setfill('0') << p->hour << ':';
  cout << setw(2) << setfill('0') << p->minute << ':';
  cout << setw(2) << setfill('0') << p->second;

  if (output_msec)
    cout << "." << setw(3) << setfill('0') << p->msec;
  cout << " - ";

  for (int i = 0; i < n; ++i)
  {
     cout << counts[i] << ' ';
     total += counts[i];
  }

  cout << " - total = " << total << endl;
}

/* -------------------------------------------------------------------- */
static const unsigned char syncWord[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
static const size_t nSyncB = 8;


void ParticleCount(pads_rec *p2d, size_t nDiodes)
{
  static bool firstTime = true;
  static size_t	perSecondCounts[512], sizeCounter = 0;
  static pads_rec prevRec;
  size_t recordCounts[512];


  if (firstTime)
  {
    firstTime = false;
    memset((void *)perSecondCounts, 0, sizeof(perSecondCounts));
  }

  memset(recordCounts, 0, sizeof(recordCounts));
  
  // Output 1 second totals.
  if (memcmp((void *)&p2d->hour, (void *)&prevRec.hour, 6) != 0)
  {
    OutputParticleCount(&prevRec, perSecondCounts, nDiodes, false);
    memset((void *)perSecondCounts, 0, sizeof(perSecondCounts));
    cout << endl;
  }

  int bytesPerSlice = nDiodes / 8;
  unsigned char image[60000];
  size_t nSlices = uncompress(image, p2d->data, 4096);

  for (int i = 0; i < nSlices; ++i)
  {
    if (memcmp(&image[i*bytesPerSlice], syncWord, nSyncB) == 0 && sizeCounter > 0)
    {
      if (sizeCounter > 511) sizeCounter = 511;
      ++perSecondCounts[sizeCounter-1];	// -1 to compensate for seperate timing word.
      ++recordCounts[sizeCounter-1];
      sizeCounter = 0;
    }
    else
      ++sizeCounter;
  }

  OutputParticleCount((pads_rec *)p2d, recordCounts, nDiodes, true);
  memcpy((void *)&prevRec, (void *)p2d, sizeof(pads_rec));
}
