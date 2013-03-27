/*
-------------------------------------------------------------------------
OBJECT NAME:	oapinfo.cc

FULL NAME:	OAP File Info

DESCRIPTION:	Display information from a NCAR PMS2D/OAP file, including
		indivdual record headers.

COPYRIGHT:	University Corporation for Atmospheric Research, 2012
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

#include <netinet/in.h>

#include "/opt/local/include/raf/header.h"

using namespace std;

char	buffer[50000];

class Config
{
public:
  Config() : fullHex(false), histo(false) { }

  bool	fullHex;
  bool	histo;

  string sourceFile;
} cfg;

void Output(char buff[]);
void ParticleCount(P2d_rec *p2d, int nDiodes);


void Usage()
{
  std::cerr << "Usage: oapinfo [-h] [-c] file.2d" << std::endl;
  std::cerr << "  -x: full hex dump of each record." << std::endl;
  std::cerr << "  -c: histogram count totals, per record and per second." << std::endl;
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
  int	rc, nBytes;
  FILE *fp;

  processArgs(argc, argv);

  if ((fp = fopen(cfg.sourceFile.c_str(), "rb")) == NULL)
  {
    std::cerr << "Unable to open input file : " << cfg.sourceFile << std::endl;
    exit(1);
  }


  rc = 0;	/* Return Code	*/

  do
  {
    fgets(buffer, 5000, fp);
    std::cout << buffer;
  }
  while (strstr(buffer, "</OAP>") == 0);


  while (fread(buffer, sizeof(P2d_rec), 1, fp) == 1)
    Output(buffer);

  fclose(fp);

  return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void Output(char buff[])
{
  P2d_rec *p2d = (P2d_rec *)buffer;
  int nDiodes = 0;

  switch (ntohs(*(unsigned short *)buff))
  {
    case 0x4331:	// PMS2D C Probes.
    case 0x4332:
    case 0x5031:	// PMS2D P Probes.
    case 0x5032:
      nDiodes = 32;
      break;
    case 0x4334:	// USB 25um C probe.
    case 0x4336:	// USB 10um C probe.
    case 0x5034:	// USB P Probe.
      nDiodes = 64;
      break;
//    case 0x4831:	// HVPS
//    case 0x4832:
    case 0x3348:	// 3V-CPI / 2DS Horizontal.
    case 0x3356:	// 3V-CPI / 2DS Vertical.
      nDiodes = 128;
      break;
    default:
      std::cout << "Unrecognized record id=" << buff[0] << buff[1] << std::endl;
  }

  if (cfg.histo)
    ParticleCount(p2d, nDiodes);
  else
    printf("  %c%c %02d:%02d:%02d.%03d, tas=%d  %x\n",
	((char*)p2d)[0], ((char*)p2d)[1],
	ntohs(p2d->hour), ntohs(p2d->minute), ntohs(p2d->second), ntohs(p2d->msec),
	ntohs(p2d->tas), ((short*)p2d->data)[0]);

  if (cfg.fullHex)
  {
    int bytesPerSlice = nDiodes / 8;
    int nSlices = 4096 / bytesPerSlice;
    for (int i = 0; i < nSlices; ++i)
    {
      for (int j = 0; j < bytesPerSlice; ++j)
      {
        printf("%02X", p2d->data[(i*bytesPerSlice) + j]);
      }
      printf("\n");
    }
  }
}

/* -------------------------------------------------------------------- */
void OutputParticleCount(P2d_hdr *p, size_t counts[], size_t n)
{
  int total = 0;

  printf("%02d:%02d:%02d - ", ntohs(p->hour), ntohs(p->minute), ntohs(p->second));
  for (int i = 0; i < n; ++i)
  {
     printf("%d ", counts[i]);
     total += counts[i];
  }

  printf(" - total = %d\n", total);
}

/* -------------------------------------------------------------------- */
static const char syncWord[] = { 0xAA, 0xAA, 0xAA };
static const size_t nSyncB = 3;


void ParticleCount(P2d_rec *p2d, int nDiodes)
{
  static bool firstTime = true;
  static size_t	perSecondCounts[512], sizeCounter = 0;
  static P2d_hdr prevRec;
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
    OutputParticleCount(&prevRec, perSecondCounts, nDiodes);
    memset((void *)perSecondCounts, 0, sizeof(perSecondCounts));
    printf("\n");
  }

  int bytesPerSlice = nDiodes / 8;
  int nSlices = 4096 / bytesPerSlice;
  for (int i = 0; i < nSlices; ++i)
  {
    // Check both little and big endian.
    if (memcmp(&p2d->data[i*bytesPerSlice+bytesPerSlice-3], syncWord, nSyncB) == 0 ||
        memcmp(&p2d->data[i*bytesPerSlice], syncWord, nSyncB) == 0)
    {
      if (sizeCounter > 511) sizeCounter = 511;
      ++perSecondCounts[sizeCounter];
      ++recordCounts[sizeCounter];
      sizeCounter = 0;
    }
    else
      ++sizeCounter;
  }

  printf("%c%c - ", ((char*)p2d)[0], ((char*)p2d)[1]);
  OutputParticleCount((P2d_hdr *)p2d, recordCounts, nDiodes);
  memcpy((void *)&prevRec, (void *)p2d, sizeof(P2d_hdr));
}
