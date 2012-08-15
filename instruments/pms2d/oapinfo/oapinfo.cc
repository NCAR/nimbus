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

int Output(char buff[]);


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc, nBytes;
  std::string	sourceFile;
  FILE *fp;

  if (argc < 2)
  {
    std::cout << "Enter file : ";
    std::cin >> sourceFile;
  }
  else
    sourceFile = argv[1];

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

  switch (ntohs(*(unsigned short *)buff))
  {
    case 0x4331:	// PMS2D C Probes.
    case 0x4332:
    case 0x4334:	// USB 25um C probe.
    case 0x4336:	// USB 10um C probe.
    case 0x5031:	// PMS2D P Probes.
    case 0x5032:
    case 0x5034:	// USB P Probe.
    case 0x4731:	// Greyscale
    case 0x4732:
    case 0x4831:	// HVPS
    case 0x4832:
    case 0x3348:	// 3V-CPI / 2DS Horizontal.
    case 0x3356:	// 3V-CPI / 2DS Vertical.
      printf("  %c%c %02d:%02d:%02d.%03d, tas=%d  %x\n",
	((char*)p2d)[0], ((char*)p2d)[1],
	ntohs(p2d->hour), ntohs(p2d->minute), ntohs(p2d->second), ntohs(p2d->msec),
	ntohs(p2d->tas), ((short*)p2d->data)[0]);
      break;

    default:
      std::cout << "Unrecognized record id=" << buff[0] << buff[1] << std::endl;
  }
}
