/* Spp.cc
 
   SPP class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
#include <Spp.h>

struct reqPckt
  {
  char esc;
  char id;
  ushort cksum;
  };

static const struct reqPckt get_data = SPP_SEND_DATA;

/******************************************************************************
** Public Functions
******************************************************************************/

Spp::Spp(const char *base, TapeHeader &hdr, int probetype, 
	void (*statMsg)(char* msg)) : 
	IsioPort (base, hdr.spp_intf_chan(), SPP_BAUD, SPP_BITS, SPP_PARITY,
	SPP_STOP, SPP_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this
// class.
{
  int j;
  which_probe = probetype;
  ptog = 0;
  gtog = 0;
  good_pkt = FALSE;
  new_data = TRUE;
  memset((char *)spp_blk, 0, sizeof(spp_blk));

  firstSample();
  reply_pkt[0] = 0x1b01;
  reply_pkt[2] = swapushort(0x105);
  switch (which_probe) {
    case 0:
      setup100_pkt.id = 0x1b01;
      setup100_pkt.model = 100;
      setup100_pkt.trig_thresh = hdr.spp_threshold();
//      setup100_pkt.trig_thresh = 0;
      setup100_pkt.transRej = hdr.spp_transRej();
      setup100_pkt.chanCnt = hdr.spp_bins();
      setup_bins = setup100_pkt.chanCnt;
      setup100_pkt.dofRej = hdr.spp_dofRej();
      setup100_pkt.range = hdr.spp_range();
      setup_range = setup100_pkt.range;
      setup100_pkt.avTranWe = hdr.spp_avgTransWt();
      setup100_pkt.attAccept = hdr.spp_attAccept() ;
      setup100_pkt.divFlag = hdr.spp_divisorFlag();
      setup100_pkt.ct_method = hdr.spp_ct_method();
      setup100_pkt.max_width = hdr.spp_max_width();
//      setup100_pkt.max_width = 0x7fff;
      hdr.spp_OPCthesholds(setup100_pkt.OPCthreshold);
      reply_pkt[1] = swapushort(100);
      break;


    case 1:
      setup200_pkt.id = 0x1b01;
      setup200_pkt.model = 200;
//      setup200_pkt.trig_thresh = hdr.spp_threshold();
      setup200_pkt.trig_thresh = 80;
      setup200_pkt.chanCnt = hdr.spp_bins();
      setup_bins = setup200_pkt.chanCnt;
      setup200_pkt.range = hdr.spp_range();
      setup_range = setup200_pkt.range;
      setup200_pkt.avTranWe = hdr.spp_avgTransWt();
      setup200_pkt.divFlag = 0x02;
//      setup200_pkt.divFlag = hdr.spp_divisorFlag();
      setup200_pkt.max_width = 0xFFFF;
      hdr.spp_OPCthesholds(setup200_pkt.OPCthreshold);
      for (j = 0; j < 4; j++) 
        setup200_pkt.spares[j] = 0;
      reply_pkt[1] = swapushort(200);
      break;

    case 2:
      setup300_pkt.id = 0x1b01;
      setup300_pkt.model = 300;
      setup300_pkt.trig_thresh = hdr.spp_threshold();
      setup300_pkt.chanCnt = hdr.spp_bins();
      setup_bins = setup300_pkt.chanCnt;
      setup300_pkt.dofRej = hdr.spp_dofRej();
      setup300_pkt.range = hdr.spp_range();
      setup_range = setup300_pkt.range;
      setup300_pkt.avTranWe = hdr.spp_avgTransWt();
      setup300_pkt.divFlag = hdr.spp_divisorFlag();
      setup300_pkt.max_width = 1000;
      hdr.spp_OPCthesholds(setup300_pkt.OPCthreshold);
      for (j = 0; j < 3; j++) 
        setup300_pkt.spares[j] = 0;
      reply_pkt[1] = swapushort(300);
      break;

    default:
      statusMsg("SPP: Incorrect Spp probe.\n");
  }
  switch (setup_bins) {
    case 10:
      smp_length = SPP_SMP_10;
      break;
    case 20:
      smp_length = SPP_SMP_20;
      break;
    case 30:
      if(which_probe == 1)
        smp_length = SPP_SMP_200_30;
      else
        smp_length = SPP_SMP_30;
      break;
    case 40:
      smp_length = SPP_SMP_40;
      break;
    default:
      statusMsg("SPP: Incorrect number of bins.\n");
  }
  chan = hdr.spp_intf_chan();
  printf("Probe = %d, %s, Port = %d, bins = %d, range = %d\n",which_probe,
         hdr.name(),hdr.spp_intf_chan(), setup_bins, setup_range);

}

/*****************************************************************************/
int Spp::initSpp (int rng)

// Sends initialization packets to the Spp probe.
// Returns TRUE when packet was successfully been sent.
{
  short j;
  static short init_pkt[SPP100_LENGTH];

  memset((char *)init_pkt, 0, sizeof(init_pkt));
//  range = rng;
  range = setup_range;
  length = SPP100_LENGTH;
  if (which_probe == 0) {
    length_back = SPP100_LENGTH;
    memcpy((char*)init_pkt, (char*)&setup100_pkt, length * 2);
    spp_first_len = SPP100_FIRST_LEN;
  }
  else if (which_probe == 1) {
    length_back = SPP100_LENGTH - 4;
    memcpy((char*)init_pkt, (char*)&setup200_pkt, length * 2);
    spp_first_len = SPP200_FIRST_LEN;
  }
  else {
    length_back = SPP100_LENGTH - 3;
    memcpy((char*)init_pkt, (char*)&setup300_pkt, length * 2);
    spp_first_len = SPP300_FIRST_LEN;
  } 

  for (j = 0; j < length - 2; j++) {
    init_pkt[j + 1] = swapushort(init_pkt[j + 1]);
//    printf("int_pkt[%d + 1] = %x\n",j,init_pkt[j+1]);
  }

  init_pkt[length - 1] = computeCheckSum((unsigned char*)init_pkt,length * 2);
  init_pkt[length - 1] = swapushort(init_pkt[length - 1]);

//  printf("length*2 = %d\n",length * 2);
  for (j = 3; j < length; j++) 
    reply_pkt[j] = init_pkt[j - 1];
  pputcnt ((char*)init_pkt, length * 2);
  igetcnt (SPP_INIT_ACK);
  new_data = TRUE;
  return 1;
}

/*****************************************************************************/
ushort Spp::computeCheckSum (unsigned char *pkt, int len)

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

/*****************************************************************************/
void Spp::checkStatus()

// Checks the status of the SPP probe isio port.
{
  int temp;

  if (checkPort() != TRUE) {
    clrbuf ();
    statusMsg ("Spp: Dropped isio interrupt.\n");
  }

  if ((good_pkt != TRUE) || (new_data != TRUE)) {
    printf("checkStatus re-initSpp[%d]\n",which_probe);
//    temp = abort (chan);
//    clrbuf ();
    initSpp(range);
  }
  new_data = FALSE;
}

/*****************************************************************************/
void Spp::secondAlign ()

// This routine is to be called at each 1 second clock tick. The spp_blk
// buffers are toggled.
{
  int ttog = 1 - ptog;

  firstSample();
  gtog = ptog;
  ptog = ttog;
  memset ((char*)&spp_blk[ptog], 0, sizeof(Spp_blk));
  full = FALSE;
}

/*****************************************************************************/
void Spp::isr()

// Pulls in the SPP sample block, checks for data alignment, issues a
// new read command.
{
  ushort j,checksum,sum;
  short test0;
  char	*datap;
  DMT200_blk *p;

//  logMsg("In Spp Isr\n",0,0,0,0,0,0);
// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

//  test0 = *(short*)datap;
  
//  logMsg("first byte = %x\n",test0,0,0,0,0,0);
  if (*(short*)datap == SPP_ACK_ACK) {
    memcpy ((void*)&spp_reply, datap, SPP_INIT_ACK);

      logMsg("%d, %x, %d, %d, %d, %d\n",
             swapushort(*((ushort*)&spp_reply.spp_data[2])),
             swapushort(*((ushort*)&spp_reply.spp_data[4])),
             swapushort(*((ushort*)&spp_reply.spp_data[6])),
             swapushort(*((ushort*)&spp_reply.spp_data[8])),
             swapushort(*((ushort*)&spp_reply.spp_data[10])),
             swapushort(*((ushort*)&spp_reply.spp_data[12])));

      logMsg("%d, %x, %d, %d, %d, %d\n",swapushort(*((ushort*)&reply_pkt[1])),
             swapushort(*((ushort*)&reply_pkt[2])),
             swapushort(*((ushort*)&reply_pkt[3])),
             swapushort(*((ushort*)&reply_pkt[4])),
             swapushort(*((ushort*)&reply_pkt[5])),
             swapushort(*((ushort*)&reply_pkt[6])));

    if(memcmp ((void*)&spp_reply, (void*)&reply_pkt, length_back - 2)) {
      good_pkt = FALSE;
    }
    else {
      logMsg("Initialize acknowledge[%d]\n",which_probe,0,0,0,0,0);
    good_pkt = TRUE;
    }
    return;
  }

// Pull in the data. 

  if (!full) {
    memcpy ((void*)&spp_blk[ptog].spp[idx], datap, spp_first_len);
    (int)spp_blk[ptog].spp[idx].spp_data[spp_first_len] = swapushort(range);
    if (which_probe == 1) {
      (int)spp_blk[ptog].spp[idx].spp_data[spp_first_len + 1] = 
      swapushort(range);
      memcpy ((void*)&spp_blk[ptog].spp[idx].spp_data[spp_first_len + 4],
              &datap[spp_first_len] , smp_length - spp_first_len);
    }
    else 
      memcpy ((void*)&spp_blk[ptog].spp[idx].spp_data[spp_first_len + 2],
              &datap[spp_first_len] , smp_length - spp_first_len);

    p = (DMT200_blk *)datap;
/*
    logMsg("%d, %d, %d, %d, %d, %d\n", p->cabinChan[0],p->cabinChan[1],p->cabinChan[2],
            p->cabinChan[3],p->cabinChan[4],p->cabinChan[5]);

    logMsg("%d, %d, %d, %d, %d, %d\n",p->OPCchan[1],p->OPCchan[3],p->OPCchan[5],
            p->OPCchan[7],p->OPCchan[9],p->OPCchan[15]);

    logMsg("%d, %d, %d, %d, %d, %d\n",datap[65],datap[66],datap[67],
            datap[68],datap[69],datap[70]);
    logMsg("%d, %d, %d, %d, %d, %d\n",datap[71],datap[72],datap[73],
            datap[74],datap[75],datap[76]);
*/
/*
    logMsg("%d, %d, %d, %d, %d, %d\n",spp_blk[ptog].spp[idx].spp_data[36],spp_blk[ptog].spp[idx].spp_data[37],spp_blk[ptog].spp[idx].spp_data[38],spp_blk[ptog].spp[idx].spp_data[39],spp_blk[ptog].spp[idx].spp_data[40],spp_blk[ptog].spp[idx].spp_data[41]);

    logMsg("%d, %d, %d, %d, %d, %d\n",spp_blk[ptog].spp[idx].spp_data[40],spp_blk[ptog].spp[idx].spp_data[41],spp_blk[ptog].spp[idx].spp_data[42],spp_blk[ptog].spp[idx].spp_data[43],spp_blk[ptog].spp[idx].spp_data[44],spp_blk[ptog].spp[idx].spp_data[45]);
    logMsg("%d, %d, %d, %d, %d, %d\n",spp_blk[ptog].spp[idx].spp_data[50],spp_blk[ptog].spp[idx].spp_data[51],spp_blk[ptog].spp[idx].spp_data[52],spp_blk[ptog].spp[idx].spp_data[53],spp_blk[ptog].spp[idx].spp_data[54],spp_blk[ptog].spp[idx].spp_data[55]);
    logMsg("%d, %d, %d, %d, %d, %d\n",spp_blk[ptog].spp[idx].spp_data[60],spp_blk[ptog].spp[idx].spp_data[61],spp_blk[ptog].spp[idx].spp_data[62],spp_blk[ptog].spp[idx].spp_data[63],spp_blk[ptog].spp[idx].spp_data[64],spp_blk[ptog].spp[idx].spp_data[65]);
*/
  }

  if (!nextSample())
    full = TRUE;

  checksum =*((ushort*)&datap[smp_length - 2]);
  checksum = swapushort(checksum);
// Get the checksum. 
  if ((sum = computeCheckSum((unsigned char*)datap, smp_length)) != checksum) { 
    logMsg("checksum = %x, sum = %x\n",checksum,sum,0,0,0,0);
    good_pkt = FALSE;
    if(checkPort() != TRUE) clrbuf();
  }
  else {
    good_pkt = TRUE;
//    logMsg("Data In\n",0,0,0,0,0,0);
  }
  clrbuf();
  new_data = TRUE; 
}
/*****************************************************************************/
void Spp::getSppData ()

// This routine is to be called to get data (normally at 10 Hz)

{
  if(good_pkt == TRUE) {
    pputcnt((char *)&get_data, sizeof(get_data));
    igetcnt (smp_length);       // issue next read
  }
}
/*****************************************************************************/
const char *Spp::buffer ()

// This routine is called to swap words.

{
    swab((char *)&spp_blk[gtog], out, sizeof(Spp_blk));
    return((const char*)out);
}
/*****************************************************************************/
