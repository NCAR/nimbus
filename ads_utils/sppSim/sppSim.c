#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "/opt/local/include/raf/header.h"

char	buffer[256];

ushort computeCheckSum (unsigned char *pkt, int len);

main()
{
  int	i, rc;
  char	dummy[32], *p;
  int	in, out, cntr;
  DMT100_blk	spp100;
  ushort	chkSum = 0, *usp;

  usp = (ushort *)buffer;

  for (i = 0; i < 8; ++i)
    spp100.cabinChan[i] = 5;

  spp100.rejDOF = 7;
  spp100.rejAvgTrans = 8;
  spp100.range = 0;
  spp100.AvgTransit = 9;
  spp100.FIFOfull = 0;
  spp100.resetFlag = 0;
  spp100.ADCoverflow = 0;

  for (i = 0; i < OPCCHAN; ++i)
    spp100.OPCchan[i] = 80 - (i<<1);

  chkSum = computeCheckSum((char *)&spp100, sizeof(spp100));

  if ((in = open("/dev/ttyS2", O_RDWR)) < 0)
    {
    fprintf(stderr, "Can't open in.\n");
    exit(1);
    }

  {
  struct termios        tty;
  speed_t spd;

  tcgetattr(in, &tty);
  tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  tty.c_oflag &= ~OPOST;
  tty.c_iflag |= IGNPAR;
  tty.c_cflag |= (CLOCAL | CREAD);
  cfsetispeed(&tty, B38400);
  cfsetospeed(&tty, B38400);
  tcsetattr(in, TCSANOW, &tty);
  }


  while (1)
   {
   do
     {
     read(in, dummy, 1);
     }
   while (dummy[0] != 0x1b);

   read(in, &dummy[1], 3);
//printf("%x %x\n", dummy[0], dummy[1]);

   if (dummy[1] == 0x01)
     {
     rc = 4;

     do
       {
       rc += read(in, &buffer[rc+2], 106-rc);
       }
     while (rc < 106);

printf("Rcvd init pckt, %d, ", usp[1]);
     memcpy(buffer, dummy, 4);
     usp[2] = 0x105;
     usp[53] = computeCheckSum(buffer, 106);
//printf("  %x %x %x\n", usp[53], buffer[106], buffer[107]);
     rc = write(in, buffer, 108);
printf("rc = %d\n", rc);
     }
   else
     {
     memcpy(buffer, &spp100, sizeof(spp100));
     usp[194>>1] = chkSum;
     rc = write(in, buffer, sizeof(spp100));
     }
   }

}

/*****************************************************************************/
ushort computeCheckSum (unsigned char *pkt, int len)

// Computes the packet check sum.
{
  ushort local_len;
  ushort j, sum;

// Get the length of the data bytes, minus the length of the checksum field.
  local_len = len - 2;

// Sum the byte count and data bytes;
  for (j = 0, sum = 0; j < local_len; j++) {
    sum += (ushort)pkt[j];
  }
  return sum;
}

