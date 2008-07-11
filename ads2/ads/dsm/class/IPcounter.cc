/* IPcounter.cc

   Dick Schillawski's Industry Pack Counter card driver.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <IPcounter.h>
#include <stdio.h>

/* -------------------------------------------------------------------- */
IPcounter::IPcounter(char *base_adr)
{
  unsigned long output = 320000;
  unsigned char *p = (unsigned char *)&output;
  int  i,j;

// printf("Starting Constructor for IPcounter\n"); fflush(stdout);

// Set the register pointers.
  ip_ctl = (unsigned char *)(base_adr + IP_CTL_REG_OFFSET);
  ip_slaves_data = (unsigned short *)(base_adr + IP_SLAVES_DATA_OFFSET);
  ip_slaves_ctl  = (unsigned short *)(base_adr + IP_SLAVES_CTL_OFFSET);
  ip_master_data_u = (unsigned char *)(base_adr + IP_MASTER_DATA_OFFSET);
  ip_master_ctl  = (unsigned char *)(base_adr + IP_MASTER_CTL_OFFSET);

// Init Master.
  *ip_master_ctl = 0x20;           // reset master counter
  *ip_master_ctl = 0x69;           // write input control register
  *ip_master_ctl = 0xB0;           // write output control register
  *ip_master_ctl = 0xC0;           // write quadrature register

// Init Counters 0 & 1.
  ip_slaves_ctl[0] = 0x2020;       // reset counters
  ip_slaves_ctl[0] = 0x6969;       // write input control registers
  ip_slaves_ctl[0] = 0x8080;       // write output control registers
  ip_slaves_ctl[0] = 0xC0C0;       // write quadrature registers

// Init Counters 2 & 3.
  ip_slaves_ctl[2] = 0x2020;
  ip_slaves_ctl[2] = 0x6969;
  ip_slaves_ctl[2] = 0x8080;
  ip_slaves_ctl[2] = 0xC0C0;

// Move preset 320,000 (8Mhz / 25hz) into master counter preset register.
  *ip_master_ctl  = 0x01;
  *ip_master_data_u = p[3];
  *ip_master_data_u = p[2];
  *ip_master_data_u = p[1];
//  *ip_master_ctl  = 0x08;

// Initialize "time" - 0..24, index of 25th of second periods
  time = -1;

// printf("IPcounter::IPcounter: 
//         p[0]=%x, p[1]=%x, p[2]=%x, p[3]=%x.\n",p[0],p[1],p[2],p[3]);

}

/* -------------------------------------------------------------------- */
void IPcounter::readCounters(int time)
{
  unsigned long d;

  ip_slaves_ctl[0] = 0x0101;          // reset OL register pointer, counters 0 & 1

  d = ip_slaves_data[0];              // read low-order byte
  data[0] = d & 0x000000FF;
  data[1] = (d >> 8)& 0x000000FF;

  d = ip_slaves_data[0];              // read middle-order byte
  data[0] |= (d << 8) & 0x0000FF00;
  data[1] |= d & 0x0000FF00;

  d = ip_slaves_data[0];              // read high-order byte
  data[0] |= (d << 16) & 0x00FF0000;
  data[1] |= (d << 8) & 0x00FF0000;

  ip_slaves_ctl[2] = 0x0101;          // reset OL register pointer, counters 2 & 3

  d = ip_slaves_data[2];              // read bytes, counters 2 & 3
  data[2] = d & 0x000000FF;
  data[3] = (d >> 8)& 0x000000FF;

  d = ip_slaves_data[2];
  data[2] |= (d << 8) & 0x0000FF00;
  data[3] |= d & 0x0000FF00;

  d = ip_slaves_data[2];
  data[2] |= (d << 16) & 0x00FF0000;
  data[3] |= (d << 8) & 0x00FF0000;


// Read master counter.

// *ip_master_ctl = 0x69;           // write input control register

  *ip_master_ctl  = 0x01;
  d = *ip_master_data_u;
  data[4] = d & 0x000000FF;
  d = *ip_master_data_u;
  data[4] |= (d << 8) & 0x0000FF00;
  d = *ip_master_data_u;
  data[4] |= (d << 16) & 0x00FF0000;

// logMsg("S[0]=%u, M=%u, set=%u, time=%i\n",data[0],data[4],data[5],time,0,0);

  if (time % 5 == 1) {
    adjustMaster();

// logMsg("S[2]=%u, M=%u, set=%u, time=%i\n",data[2],data[4],data[5],time,0,0);

  }
}

/* -------------------------------------------------------------------- */
void IPcounter::adjustMaster()
{
  long latchedCounts;
  long output = 320000;
  long adjustment;
  unsigned char *p = (unsigned char *)&output;
  static long lastOutput = 320000;
  static long bestGuess = 320000;

  *ip_master_ctl = 0x01;
  latchedCounts = *ip_master_data_u & 0x000000FF;
  latchedCounts |= (*ip_master_data_u << 8) & 0x0000FF00;
  latchedCounts |= (*ip_master_data_u << 16) & 0x00FF0000;

  adjustment = (latchedCounts-lastOutput/2)/40;

  output = bestGuess + adjustment;
  if (output > 330000) output = 330000;
  if (output < 310000) output = 310000;

  if (adjustment > 15) bestGuess++;
  if (adjustment < -15) bestGuess--;
  
  *ip_master_ctl  = 0x01;
  *ip_master_data_u = p[3];
  *ip_master_data_u = p[2];
  *ip_master_data_u = p[1];

  lastOutput = output;
  data[5] = output;
}

/* -------------------------------------------------------------------- */
void IPcounter::readProm(char *base_addr)
{
  int i = 0;

printf("Reading IP PROM.\n");
  while (i < base_addr[21]) {
     if (i < 4) printf("%i-%c ",i,base_addr[(i<<1)+1]);
     else printf("%i-%x ",i,(int *)base_addr[(i<<1)+1]);
     i++;
  }
  printf("\n");
}
