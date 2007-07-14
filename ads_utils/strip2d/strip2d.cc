/* strip 2d records from an ADS file and produce a 2d file (including headers).
 */
#include <raf/adsIO.h>
#include <errno.h>
#include <stdio.h>

char    buffer[0x8000];

main()
{
  int		i, len;
  char		sourceFile[512], destFile[512];
  FILE		*outFP;
  P2d_rec	rec;

  printf("Enter source ADS file : ");
  gets(sourceFile);

  printf("Enter destination 2D file : ");
  gets(destFile);


  ADS_DataFile	source(sourceFile);

  if ((outFP = fopen(destFile, "w+b")) == NULL)
    {
    fprintf(stderr, "Can't open output file %s, errno=%d\n", destFile);
    exit(1);
    }


  for (i = 0; i < 3; ++i)
    {
    len = source.NextPhysicalRecord(buffer);
    fwrite(buffer, len, 1, outFP);
    }

  while (source.NextPMS2dRecord(&rec))
    fwrite(&rec, sizeof(P2d_rec), 1, outFP);

  fclose(outFP);

}
