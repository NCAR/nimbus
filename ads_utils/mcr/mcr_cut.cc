/*
-------------------------------------------------------------------------
OBJECT NAME:	mcr_cut.cc

FULL NAME:	

DESCRIPTION:	Cut raw data out of MCR file for user specified time and
		channels.  Each channel goes to seperate file.

REFERENCES:	libraf++.a

NOTES:		g++ -I/home/local/libraf++ mcr_cut.cc -o mcr_cut -lraf++

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "adsIO.h"

char		buffer[65000], mcrFileName[128];
Mcr_rec		mcrRec;
ADS_DataFile	*file;

bool NextMCRfile();

/* -------------------------------------------------------------------- */
main()
{
  char	outputFile[128], tmp[32], hdr[30000];
  short	start[3], end[3];
  FILE *fp[7];
  int	i, l1, l2;
  bool	channel[7];


  /* Query user for info.
   */
  printf("Enter input file : ");
  gets(mcrFileName);

  printf("Enter output file : ");
  gets(outputFile);

  printf("Enter start time [hhmmss] : ");
  gets(buffer);
  sscanf(buffer, "%02hd%02hd%02hd\n", &start[0], &start[1], &start[2]);

  printf("Enter end time [hhmmss] : ");
  gets(buffer);
  sscanf(buffer, "%02hd%02hd%02hd\n", &end[0], &end[1], &end[2]);

  printf("Enter channels [ex: 1347] : ");
  gets(buffer);

  for (i = 0; i < 7; ++i)
    channel[i] = false;

  for (i = 0; i < strlen(buffer); ++i)
    channel[buffer[i] - '1'] = true;


  /* Open files.
   */
  file = new ADS_DataFile(mcrFileName);

  l1 = file->NextPhysicalRecord(tmp);
  l2 = file->NextPhysicalRecord(hdr);

  for (i = 0; i < 7; ++i)
    if (channel[i])
      {
      sprintf(buffer, "%s%d-%02d%02d%02d-%02d%02d%02d", outputFile, i + 1,
	start[0], start[1], start[2], end[0], end[1], end[2]);

      if ((fp[i] = fopen(buffer, "w+b")) == NULL)
        {
        fprintf(stderr, "Can't open output file %s.\n", buffer);
        exit(1);
        }

/*	Un comment to be able to view the files via mcrtd.
      fwrite(tmp, l1, 1, fp[i]);
      fwrite(hdr, l2, 1, fp[i]);
      fwrite(hdr, l2, 1, fp[i]);
*/
      }


  /* Find start time.
   */
  file->FirstMCRRecord(&mcrRec);

  while (memcmp((char *)start, (char *)&(mcrRec.hour), 6))
    {
    if (file->NextMCRRecord(&mcrRec) == false)
      if (NextMCRfile() == false)
        {
        fprintf(stderr, "Failed to locate start time.\n");
        exit(1);
        }

    if (mcrRec.id[4] == '0' && mcrRec.second == 0)
      {
      printf("%02d:%02d:%02d\r", mcrRec.hour, mcrRec.minute, mcrRec.second);
      fflush(stdout);
      }
    }


  printf("\n");


  /* Dump records until you reach end time.
   */
  while (memcmp((char *)end, (char *)&(mcrRec.hour), 6))
    {
    if (file->NextMCRRecord(&mcrRec) == false)
      if (NextMCRfile() == false)
        {
        fprintf(stderr, "Failed to reach end time, closing files, and");
        fprintf(stderr, "quitting @ %02d:%02d:%02d.\n", mcrRec.hour,
		mcrRec.minute, mcrRec.second);
        break;
        }

    if (channel[mcrRec.id[4]-'0'])
      fwrite((char *)&mcrRec, sizeof(Mcr_rec), 1, fp[mcrRec.id[4]-'0']);

    if (mcrRec.id[4] == '0' && mcrRec.second == 0)
      {
      printf("%02d:%02d:%02d\r", mcrRec.hour, mcrRec.minute, mcrRec.second);
      fflush(stdout);
      }
    }


  /* Out of here.
   */
  for (i = 0; i < 7; ++i)
    if (channel[i])
      fclose(fp[i]);

  printf("\n");

}

/* -------------------------------------------------------------------- */
bool NextMCRfile()
{
  delete file;
 
  ++(mcrFileName[strlen(mcrFileName)-1]);
 
  file = new ADS_DataFile(mcrFileName);
 
  printf("   \nChanging to MCR file %s.\n", mcrFileName);
 
  return(file->FirstMCRRecord(&mcrRec));
 
}       /* END NEXTMCRFILE */
 
/* END MCR_CUT.CC */

