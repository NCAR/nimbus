/* Cmigits3.cc

   Cmigits-III class

   Original Author: Mike Spowart 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Cmigits3.h>

struct initPckt
  {
  ushort sync;
  ushort msg_id;
  ushort word_cnt;
  ushort flags;
  ushort hdr_cksum;
  ushort data_valid;
  ushort mode;
  short lat_deg;
  short lat_min;
  short lat_sec;
  short lon_deg;
  short lon_min;
  short lon_sec;
  long  alt;
  short gnd_speed;
  short gnd_track;
  short year;
  short julian_day;
  short hour;
  short minute;
  short second;
  short true_hdg;
  short sequence;
  long  reserved;
  ushort data_cksum;
  };

static struct initPckt cmigits_init = CMIGITS_INIT;
   
/******************************************************************************
** Public Functions
******************************************************************************/

Cmigits3::Cmigits3 (const char *base, const int port,
                    void (*stat_msg)(char* msg_str)) :
                    IsioPort(base, port, CMIGITS_BAUD, CMIGITS_BITS,
                    CMIGITS_PARITY, CMIGITS_STOP, CMIGITS_FLOW, stat_msg),
                    statusMsg (stat_msg)
{

  ptog = 0;
  gtog = 1;
  state = 0;
  term_str[0] = 0xFF;
  term_str[1] = 0x81;
  Cmig_sem = TRUE;
  num_data[gtog] = 0;
  num_data[ptog] = 0;
  newSecond = FALSE;
  buffer_add = data_buffer[ptog];
  memset ((char*)&cmigits3_blk, 0, sizeof(Cmigits3_blk));
  clrbuf();
}

/*****************************************************************************/
void Cmigits3::initCmigits3()
{
//  static char init_pkt[] = IRS_RESET;

  clrbuf();
  num_data[gtog] = 0;
  num_data[ptog] = 0;
  state = 0;
  Cmig_sem = TRUE;
  igetstr (term_str);                 // issue next read 6 bytes

}

/*****************************************************************************/
void Cmigits3::secondAlign()

// Called at the start of a second to align the indices and toggle buffers.
{
  newSecond = TRUE;
  memset ((char*)&cmigits3_blk, 0, sizeof(Cmigits3_blk));
}
/*****************************************************************************/

void Cmigits3::checkStatus()

// Checks the status of the irs receiver and isio port.
{
// Check the isio channels.
  if (checkPort() != TRUE) {
    statusMsg ("Cmigits3: Dropped isio interrupt.\n");
    clrbuf();
    igetstr (term_str);
  }
}

/*****************************************************************************/
void Cmigits3::readIsr()	// Sampling isr. 
{
  char	*datap;
  short j;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
//  logMsg("Cmid ISR state = %d\n",state,0,0,0,0,0);
  switch (state)
  {
    case 0:     // Sync word
      state = 1;
      memcpy(buffer_add, datap, 2);
      buffer_add += 2;
      num_data[ptog] += 2;
      igetcnt(4);
//  logMsg("num_data = %d, state = %d\n",num_data[ptog],state,0,0,0,0);
      break;

    case 1:     // Type & Len
      state = 2;
      memcpy(buffer_add, datap, 4);
      buffer_add += 4;
      num_data[ptog] += 4;
      pcktLen = (swapushort(*((ushort*)&datap[2])) * 2) + 6;
/*
      if ((pcktLen < 20) | (pcktLen > 300)) { 
        state = 0;
	logMsg("Bad Cmid. pcktLen\n",0,0,0,0,0,0);
        igetstr(term_str);
      }
      else 
*/
        igetcnt(pcktLen);
//  logMsg("pcktLen = %d, state = %d\n",pcktLen,state,0,0,0,0);
      break;

    case 2:
      state = 0;
      memcpy(buffer_add, datap, pcktLen);
      buffer_add += pcktLen;
      num_data[ptog] += pcktLen;
      if (newSecond) {
        newSecond = FALSE;
	Cmig_sem = TRUE;
      }
      igetstr(term_str);                 // issue next read 6 bytes
//  logMsg("num_data = %d, state = %d\n",num_data[ptog],state,0,0,0,0);
      break;

    default:
      state = 0;
      igetstr(term_str);
      break;
  }

/*

  j = 0;
  *buffer_add++ = datap[j];
  num_data[ptog]++;

  do { 
    j++;
    *buffer_add++ = datap[j];
    num_data[ptog]++;
  } while(!((datap[j] == SYNC2) && (datap[j-1] == SYNC1))); 

  if (newSecond) {
    newSecond = FALSE;
    ptog = 1 - gtog;
    buffer_add = data_buffer[gtog];
    num_data[ptog] = 0;
  }
  igetstr (term_str);                 // issue next read 6 bytes
*/
}
/*****************************************************************************/
char *Cmigits3::buffer()	// Sort the data. 
{

  unsigned short  i, j, data_count, rate, message_id;
  double temp1,temp2;
  short  *data;
  unsigned short  tbuf[CMIGITS_HDR], count;

  unsigned short *p, *p1, temp0;

  rate = 0;
  data_count = 0;
  data = (short*)data_buffer[gtog];
//  printf("num_data = %d\n",num_data[gtog]);
//  printf("num_data = %d, data[0] = %hx, data[1] = %hx, data[2] = %hx\n",
//         num_data[gtog],data[0],data[1],data[2]); 

  do {
    for (j = 0; j < CMIGITS_HDR; j++) {
      tbuf[j] = *data;
      data += 1;
      data_count += 2;
    } 

    while ((tbuf[0] != SYNC) & (data_count < num_data[gtog]))  { 
      tbuf[0] = tbuf[1];
      tbuf[1] = tbuf[2];
      tbuf[2] = *data;
      data +=  1;
      data_count += 2;
    }
    message_id = swapushort(*((ushort*)&tbuf[1])); 
    count = swapushort(*((ushort*)&tbuf[2])) + 1; 
    data_count += (count * 2) + 4;

// Process packet data here:
    switch (message_id) {
    case 3:                      // 1 Hz packet
      data += 26;
      cmigits3_blk.Caccel_east = *(long*)data;
      data += 2;
      cmigits3_blk.Caccel_north = *(long*)data;
      data += 2;
      cmigits3_blk.Caccel_up = *(long*)data;
      data += 1;
      break;

    case 3501:                    // 10 Hz packet
      data += 6;
      cmigits3_blk.Clatitude[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Clongitude[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Caltitude[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Cvelocity_north[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Cvelocity_east[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Cvelocity_up[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Cpitch[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Croll[rate] = *(long*)data;
      data += 2;
      cmigits3_blk.Ctrue_heading[rate] = *(long*)data;
      data += 3;
/*
      if (rate == 0) {
	 p = (ushort *)&cmigits3_blk.Clongitude[0];
         p[0] = swapushort(p[0]); p[1] = swapushort(p[1]);
         temp0 = p[0]; p[0] = p[1]; p[1] = temp0;
//         printf("lat = %lx\n", cmigits3_blk.Clongitude[0]);

	 temp1 = 8.381903e-8 * cmigits3_blk.Clongitude[0];

         printf("lat = %g\n", temp1);
      }	    
*/
      rate++;
      break;

      default:
      data += count + 2;
    }

//    printf("count = %d id = %d cnt = %d dcnt = %d\n", data_count, 
//	message_id, count, num_data[gtog] - data_count);
//      printf("Message ID = %d\n",message_id);

  } while (data_count < num_data[gtog]);
  printf("rate = %d\n",rate);

  num_data[gtog] = 0;
  gtog = ptog;                   	// toggle the get buffer index 
  ptog = 1 - gtog;
  buffer_add = data_buffer[ptog];
  return((char *)&cmigits3_blk);
}

/* END CMIDGETS3.CC */
