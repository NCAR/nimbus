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
#include <string>
#include <unistd.h>

#include <netinet/in.h>

#include </opt/local/include/raf/header.h>


char	buffer[50000];
bool	hdrsOnly = false;

int Output(char buff[]);


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc, nBytes, aCnt = 1;
  std::string	sourceFile;
  FILE *fp;

  if (strcmp(argv[aCnt], "-t") == 0)
  {
    aCnt++;
    hdrsOnly = true;
  }

  if (argc < 2)
  {
    std::cout << "Enter file : ";
    std::cin >> sourceFile;
  }
  else
    sourceFile = argv[aCnt];


  if ((fp = fopen(sourceFile.c_str(), "rb")) == NULL)
  {
    std::cerr << "Unable to open input file : " << sourceFile << std::endl;
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
int Output(char buff[])
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

  printf("  %c%c %02d:%02d:%02d.%03d, tas=%d  %x\n",
	((char*)p2d)[0], ((char*)p2d)[1],
	ntohs(p2d->hour), ntohs(p2d->minute), ntohs(p2d->second), ntohs(p2d->msec),
	ntohs(p2d->tas), ((short*)p2d->data)[0]);

  if (hdrsOnly == false)
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
