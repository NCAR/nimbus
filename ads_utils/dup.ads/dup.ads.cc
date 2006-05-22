/* Duplicate an ADS image.  Generally used to remove records at beginning
 * of file with bad time-stamps.
 * "if" tests can be placed in the main loop to remove specific problems.
 */
#include "adsIO.h"
#include <cstdio>

char    buffer[65999];

int main(int argc, char *argv[])
{
  int	len;
  int	skipNrecords = 0;
  char	sourceFile[512], destFile[512];
  FILE	*outFP;
  Hdr_blk * h = (Hdr_blk *)buffer;

  printf("Enter input ADS file : ");
  fgets(sourceFile, 500, stdin);
  sourceFile[strlen(sourceFile)-1] = 0;

  printf("Enter output ADS file : ");
  fgets(destFile, 500, stdin);
  destFile[strlen(destFile)-1] = 0;

  printf("Enter number of records to skip : ");
  fgets(buffer, 10, stdin);
  skipNrecords = atoi(buffer);

  ADS_DataFile	source(sourceFile);

  if ((outFP = fopen(destFile, "w+b")) == NULL)
    {
    fprintf(stderr, "Can't open output file %s, errno=%d\n", destFile);
    return(1);
    }

  /* Copy the header records.  Required.
   */
  for (int i = 0; i < 3; ++i)
    {
    len = source.NextPhysicalRecord(buffer);
    fwrite(buffer, len, 1, outFP);
    }

  /* Records to skip past.
   */
  for (int i = 0; i < skipNrecords; ++i)
    source.NextPhysicalRecord(buffer);


  /* Let 'er rip.
   */
  for (int cntr = 0; (len = source.NextPhysicalRecord(buffer)) > 0; ++cntr)
  {
    // Place your 'if' tests here to skip records.
//    if (cntr > 26000 && ntohs(h->hour) == 18)
    if (false)
    {
      printf("Disposing of record #%d\n", cntr);
      continue;
    }

    fwrite(buffer, len, 1, outFP);
  }

  fclose(outFP);
  return(0);
}
