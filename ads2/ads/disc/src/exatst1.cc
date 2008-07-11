/* exatst1.cc

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
// System header files.
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <header.h>

main()
{
  int exa_id;                                   // drive fd
  int len;
  char buf[32768];
  Fl fl;
  Hdr_blk *hdr;
  int j;

// Wait until the drive is successfully opened.
  printf ("Opening the drive.\n");
  while ((exa_id = open ("/dev/rmt/0l", O_RDWR, (u_char)0)) == -1)
    sleep(1);

  printf ("Reading the leader record.\n");
  len = read (exa_id, buf, 32768);
  printf ("leader len = %d, %s\n", len, buf);
    
  printf ("Reading the header records.\n");
  len = read (exa_id, buf, 32768);
  printf ("header len = %d\n", len);
  len = read (exa_id, buf, 32768);
  printf ("header len = %d\n", len);

  memcpy ((char*)&fl, buf, sizeof(Fl));
  printf ("fltnum = %s, date = %s, time = %s\n", fl.fltnum, fl.date, fl.time);
  printf ("lr_len = %d, lrppr = %d\n", fl.lrlen, fl.lrppr);

  printf ("Reading records.\n");

  while ((len = read (exa_id, buf, 32768)) > 0) {
    printf ("%d bytes read\n", len);
    for (j = 0; j < len; j += fl.lrlen) {
      hdr = (Hdr_blk*)&buf[j];
      printf ("id = 0x%X, %02d:%02d:%02d\n", 
              hdr->id, hdr->hour, hdr->minute, hdr->sec);
    }
  }
  close (exa_id);
}
