/* exatst.cc

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
// System header files.
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Class include files
#include <Exa.h>

Exa exa;					// Exabyte drive class

main()
{
  int j;
  int k;
  char buf[32768];

  for (j = 0; j < 32768; j++)
    buf[j] = j;

// Wait until the drive is successfully opened.
  while (exa.Open ("/dev/rmt/0l", O_RDWR, (u_char)0) == ERROR)
    sleep(1);

/**
  printf ("Calling Unbuffered\n");
  exa.Unbuffered();

  printf ("Writing the leader record.\n");
  exa.Write (buf, 20);
**/

  printf ("Calling Buffered\n");
  exa.Buffered();

  printf ("Writing 100 records.\n");
  for (j = 0; j < 100; j++) {
    exa.Write (buf, 32700);
    printf ("%d\n", j);
  }

  printf ("Calling Rewind\n");
  exa.Rewind();

  printf ("Reading 100 records.\n");
  for (j = 0; j < 100; j++) {
    memset (buf, (char)0, 32768);
    exa.Read (buf, 32700);
    for (k = 0; k < 32700; k++) {
      if (buf[k] != (char)k) {
        printf ("Data error, buf[k] = %d, (char)k = %d, k = %d.\n", 
        buf[k], (char)k, k);
        break;
      }
    }
    printf ("%d\n", j);
  }

  printf ("calling eject\n");
  exa.Eject();

  while(1)
    sleep(1);
}
