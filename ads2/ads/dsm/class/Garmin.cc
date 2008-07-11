/*
   Garmin.cc

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#include <Garmin.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Garmin::Garmin (const char *base, const int port, 
                    void (*stat_msg)(char* msg_str),
                    void (*set_time)(const int year, const int month,
                                     const int day, const int hour,
                                     const int minute, const int second)) :
                    IsioPort(base, port, GARMIN_BAUD, GARMIN_BITS, 
                    GARMIN_PARITY, GARMIN_STOP, GARMIN_FLOW, stat_msg), 
                    statusMsg (stat_msg), setTime (set_time)
        
// Class constructor.
{
  memset ((char*)&garmin_blk, 0, sizeof(garmin_blk));
  time_rqst = FALSE;
  date_rqst = FALSE;
  date_time = FALSE;
  ptog = 0;
  gtog = 0;
  term_str[0] = '\r';
  term_str[1] = '\n';
  term_str[2] = '\0';

  garmin_day = garmin_month = garmin_year = 0;

//  clrbuf();
//  igetstr (term_str);
}

/*****************************************************************************/
void Garmin::initGarmin()
{

  igetstr (term_str);                   // issue first read

}

/*****************************************************************************/
void Garmin::checkStatus()

// Checks the status of the gps receiver and isio port.
{
// Check the isio channels. 

  if (time_rqst && date_rqst &! date_time) {
    setTime(garmin_year, garmin_month, garmin_day, garmin_hour,
            garmin_minute, garmin_second);
    date_time = TRUE;
  printf("System date/time set from GPS\n");
  printf("Garmin %d/%d/%d  %d:%d:%d\n",garmin_year, garmin_month, garmin_day, 
          garmin_hour,garmin_minute, garmin_second);
  printf("\n");
  }

  if (checkPort() != TRUE) {
    igetstr (term_str);		// new read if an error or dropped interrupt
    statusMsg ("Garmin: Dropped isio interrupt.\n");
  }
}

/*****************************************************************************/
void Garmin::secondAlign ()

// This routine is to be called at each 1 second clock tick. The garmin_blk
// buffers are toggled.
{

  gtog = ptog;
  ptog = 1 - gtog;
  memset ((char*)&garmin_blk[ptog], 0, sizeof(Garmin_blk));
}

/*****************************************************************************/
void Garmin::isr()

// Isr to read the Garmin GPS stream via the ISIO-2. 
{
  char *datap;
  register int j;
  char utctime[8];
  float f1, f2, f3, f4, f5, f6, f7, f8;
  ushort u1, u2, u3;
  char c1, c2, c3, c4, sentence_name[8];
  int i;

//  logMsg("Garmin ISR\n",0,0,0,0,0,0);
// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

  if (*datap == GARMIN_START) {


//    memcpy (sentence_name, (char *)datap[1], 5);

    if (!strncmp (GPGGA, datap, strlen(GPGGA))) { 

      for (j = 0; j < 6; j++)
        utctime[j] = datap[j + 7];
      utctime[6] = '\0';
      sscanf (&datap[14], "%f,%c,%f,%c,%hu,%hu,%f,%f,%f",
              &f1,&c1,&f2,&c2,&u1,&u2,&f3,&f4,&f5);
      garmin_blk[ptog].utctime = utctime;
      garmin_hour = (utctime[0] - '0') * 10 + (utctime[1] - '0');
      garmin_minute = (utctime[2] - '0') * 10 + (utctime[3] - '0');
      garmin_second = (utctime[4] - '0') * 10 + (utctime[5] - '0');
      time_rqst = TRUE;

      if (garmin_hour == 0 && garmin_minute == 0 && garmin_second < 5)
        date_rqst = true;

      garmin_blk[ptog].glat = f1;
      garmin_blk[ptog].north = c1;
      garmin_blk[ptog].glon = f2;
      garmin_blk[ptog].east = c2;
      garmin_blk[ptog].quality = u1;
      garmin_blk[ptog].nsat = u2;
      garmin_blk[ptog].dilution = f3;
      garmin_blk[ptog].height = f4;
      garmin_height = f4;
      garmin_blk[ptog].geoidal_height = f5;

    }

    else if (!strncmp (GPRMC, datap, strlen(GPRMC))) {
      sscanf (&datap[14], "%c", &c1);
      garmin_blk[ptog].status = datap[14];
      if (datap[14] == 'A') 
        date_rqst = TRUE;
      else {
        date_rqst = FALSE;
        date_time = FALSE;
      }
      sscanf (&datap[41], "%f,%f", &f1,&f2);
      garmin_blk[ptog].ground_speed = f1;
      garmin_blk[ptog].course = f2;
      for (j = 0; j < 6; j++)
        utctime[j] = datap[j + 53];
      utctime[6] = '\0';
      garmin_blk[ptog].utcdate = utctime;
      garmin_day = (utctime[0] - '0') * 10 + (utctime[1] - '0');
      garmin_month = (utctime[2] - '0') * 10 + (utctime[3] - '0');
      garmin_year = (utctime[4] - '0') * 10 + (utctime[5] - '0');
    }
/*  
    else if (!strncmp (GPGLL, datap, strlen(GPGLL))) {
//      logMsg ("Garmin GPGLLpacket \n",0,0,0,0,0,0);

    }
*/
    else {
//      logMsg (" Bad Garmin packet \n",0,0,0,0,0,0);
    }    

  }

// Issue the next serial port read.
  igetstr (term_str);

}
/******************************************************************************
** Private Functions
******************************************************************************/
 
/*****************************************************************************/
