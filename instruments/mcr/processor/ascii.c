
#include "header.h"

#include <stdio.h>
#include <string.h>

char	buffer[0x8000];

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int		i, channel, cntr = 0, pt, ct;
  FILE		*inFP, *outFP;
  Mcr_rec	*mptr = (Mcr_rec *)buffer;

  if (argc < 3)
    {
    fprintf(stderr, "Usage: ascii input_file output_file\n");
    return(1);
    }


  if ((inFP = fopen(argv[1], "rb")) == NULL)
    {
    fprintf(stderr, "Can't open input file %s.\n", argv[1]);
    return(1);
    }

  if ((outFP = fopen(argv[2], "w+")) == NULL)
    {
    fprintf(stderr, "Can't open output file %s.\n", argv[2]);
    return(1);
    }


// +400 for first cut, which had IRS data, remove eventually.

  while (fread(buffer, sizeof(Mcr_rec) + 400, 1, inFP) > 0)
    {
    if (strncmp(buffer, "MCR", 3))
      continue;

    channel = atoi(&buffer[4]);
/*
    if (channel == 0)
      {
      ct = mptr->hour * 3600 + mptr->minute * 60 + mptr->second;

      if (ct != pt +1)
        printf("non-consecutive time pt = %d, ct = %d\n", pt, ct);
      
      pt = ct;
      }
*/

printf("%s - %02d:%02d:%02d\n", mptr->id, mptr->hour, mptr->minute, mptr->second);
    for (i = 0; i < 5000; ++i)
      fprintf(outFP[channel], "%d\n", ((short *)mptr->data)[i]);

    if (++cntr > 500 && channel == 6)
      break;
    }

printf("Out of main loop.\n"); fflush(stdout);
  fclose(inFP);
  fclose(outFP);

}	/* END MAIN */
