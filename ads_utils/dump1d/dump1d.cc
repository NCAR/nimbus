#include "adsIO.h"

char	buffer[MX_PHYS];

main(int argc, char *argv[])
{
  int		i, j;
  Hdr_blk	*h = (Hdr_blk *)buffer;
  Pms1v2	*pp;
  P16v2_blk	*dp;

  ADS_DataFile file("/home/data/101tf01.ads");


  file.FirstSyncRecord(buffer);


  do
    {
    printf("%02d:%02d:%02d -----------\n", h->hour, h->minute, h->second);

    for (pp = file.hdr->GetFirst("PMS1V2"); pp; pp = file.hdr->GetNext("PMS1V2"))
      {
      dp = (P16v2_blk*)(&buffer[file.hdr->Start(pp)]);
      printf("%s\n", file.hdr->VariableName(pp));

      for (i = 0; i < file.hdr->SampleRate(pp); ++i)
        {
        for (j = 0; j < 8; ++j)
          printf("%5d ", dp[i].aux[j]);

        printf("\n");
        }
      }
    }
  while (file.NextSyncRecord(buffer));

}
