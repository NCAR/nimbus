/* Pms2IO.cc
 
   MVME110 with NCAR I/O channel Pms 2D interface class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Pms2IO.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Pms2IO::Pms2IO (char *gcm2, char *gtime2, char *grec2, 
                void (*stat_msg)(char* msg_str))

// Class constructor.
{
  int j;

// Set global pointers.
  g2cmd = (G2cmd*)gcm2;				// command struct
  g2time = (G2time*)gtime2;
  g2rec = (P2d_rec*)grec2;			// data buffer
  statusMsg = stat_msg;                         // status message handler

// Create the record buffer class.
  for (j = 0; j < TOG; j++) {
    buf[j] = (SerialBuf*)malloc (sizeof (SerialBuf));
    buf[j]->SerialBuf (sizeof (P2d_Phys));
  }

// Initialize buffer and display control variables.
  ptog = 0;
  gtog = 0;
  p_cnt = 0;
  c_cnt = 0;
  rec_cnt = 0;
  max_tas = STD_2D_MAX_TAS;
  setTasMode (TAS2D_AUTO);			// set auto tas mode
  setTas (TAS2D_DEFAULT);			// set default true air speed
  setRate (RATE2D_MAX);				// set default record rate

// Initially disable all probes.
  memset ((char*)probe_names, 0, sizeof(probe_names));
  setProbeControl (probeIndex (P1_STR), DISAB_2D);
  setProbeControl (probeIndex (C1_STR), DISAB_2D);
  setProbeControl (probeIndex (P2_STR), DISAB_2D);
  setProbeControl (probeIndex (C2_STR), DISAB_2D);
}
/******************************************************************************/

void Pms2IO::initProbe (char *type)
 
// Called to initialize a probe based on the probe type defined by the tape
// header, P2D_P_STR or P2D_C_STR, or P2D_H_STR.
{ 
  int idx;

  if (!strcmp (P2D_P_STR, type)) { 
    p_cnt++;
    sprintf (msg, "P%1d", p_cnt);                // build the probe id string  
  } 

// C probes and HVPS probes are both sampled through the C interface channel.
  else if (!strcmp (P2D_C_STR, type)) {  
    c_cnt++; 
    sprintf (msg, "C%1d", c_cnt);                // build the probe id string  
  } 
  else if (!strcmp (P2D_H_STR, type)) {  
    c_cnt++; 
    sprintf (msg, "H%1d", c_cnt);                // build the probe id string  

// If a HVPS probe is being sampled through this interface, the tas is limited
// to HVPS_MAX_TAS.  NOTE: This limits the tas for all probes on this 
// interface.
    max_tas = HVPS_MAX_TAS;
  } 
  else {
    sprintf (msg, "Unknown Pms 2D probe type initialization requested, %s.\n", 
             type);
    statusMsg (msg);
    return;
  }
  idx = probeIndex (msg);
  strcpy (probe_names[idx], msg);		// save the id string
  setProbeControl (idx, ENAB_2D);
} 
/******************************************************************************/

void Pms2IO::enableProbe (char *id)

// Enables a probe for operation.
{
  int j;

  if ((j = probeIndex (id)) != ERROR) {
    if (setProbeControl (j, ENAB_2D) == OK)
      sprintf (msg, "%s probe enabled.\n", id);
    else 
      sprintf (msg, "%s probe interface is not present in the DSM.\n", id);
  }
  else 
    sprintf (msg, "Unknown Pms 2D probe enable requested. %s.\n", id);

  statusMsg (msg);
}
/******************************************************************************/

void Pms2IO::disableProbe (char *id)

// Disables a probe for operation.
{
  int j;
 
  if ((j = probeIndex (id)) != ERROR) {
    if (setProbeControl (j, DISAB_2D) == OK)
      sprintf (msg, "%s probe disabled.\n", id);
    else
      sprintf (msg, "%s probe interface is not present in the ADS.\n", id);
  }     
  else
    sprintf (msg, "Unknown Pms 2D probe disable requested. %s.\n", id);

  statusMsg (msg);
}
/******************************************************************************/

void Pms2IO::setTas (int tas)

// Set the true air speed clock rate.
{
  if (tas_mode == TAS2D_AUTO) {
    if (tas > max_tas)
      tas = max_tas;
    g2cmd->tas = tas * 255 / 125;	
  }
}
/******************************************************************************/
 
void Pms2IO::setTasMode (int mode)
 
// Set the true air speed mode to auto or a fixed tas mode.
{
  if ((tas_mode = mode) != TAS2D_AUTO) 
    g2cmd->tas = mode * 255 / 125;		// set fixed tas
}
/******************************************************************************/

void Pms2IO::setRate (int rate)
 
// Sets the max rate at which records are acquired from the controller.
{
  rec_rate = rate;
}
/******************************************************************************/

void Pms2IO::setTime (int hour, int minute, int second)
 
// Sets the interface time by writing to the global time register.
{
  g2time->hour = hour;
  g2time->minute = minute;
  g2time->second = second;
  g2time->timeflag = TRUE;
}
/******************************************************************************/

void Pms2IO::collect (int new_second)  

// Checks for data available from the Mvme110 controller.  If so, the data is
// pulled into the local buffer.
{
  P2d_rec *rec;
  int idx;

// If this is the start of a new second, reset the record counter.
  if (new_second)
    rec_cnt = 0;

// Check if a 2d global buffer is full.  Handle no more than 1 buffer
//  at a time. This is to keep from losing any sampled data. Also, process
//  no more than rate buffers per second. 

  if ((rec_cnt < rec_rate) && !buf[ptog]->bufFull() && 
      g2rec->data[P2D_DATA-1]){
    
// Get a pointer into the current buffer position.
    rec = (P2d_rec*)buf[ptog]->getBufPtr();

    buf[ptog]->copyToBuf ((char*)g2rec, sizeof (P2d_rec)); 

    g2rec->data[P2D_DATA-1] = 0;       	// clear buffer full flag
    rec_cnt++;

// If this record is from an HVPS probe then overwrite the id word.
    idx = probeIndex ((char*)&rec->id);
    if (!strncmp (probe_names[idx], H1_STR, 1))	{ // does name start with 'H' ?
      strncpy ((char*)&rec->id, probe_names[idx], strlen (probe_names[idx]));
      printf ("H1  ");
    }

// Check for a full local buffer. Toggle buffers if so.
    if (buf[ptog]->bufFull())
      ptog = ++ptog % TOG;                     
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/

int Pms2IO::probeIndex (char *id)

// Returns the index corresponding the the passed in id, or ERROR if invalid.
{
  if (!strncmp (P1_STR, id, strlen(P1_STR)))
    return P1IDX;
  if (!strncmp (C1_STR, id, strlen(C1_STR)))
    return C1IDX;
  if (!strncmp (H1_STR, id, strlen(H1_STR)))
    return H1IDX;
  if (!strncmp (P2_STR, id, strlen(P2_STR)))
    return P2IDX;
  if (!strncmp (C2_STR, id, strlen(C2_STR)))
    return C2IDX;
  if (!strncmp (H2_STR, id, strlen(H2_STR)))
    return H2IDX;

  return ERROR;
}
/******************************************************************************/

int Pms2IO::setProbeControl (int idx, int cmd)

// Writes a command to the specified probe control word.
{

printf("%d %d %d %d; idx=%d -> cmd=%d\n", g2cmd->rec2d[0],
 g2cmd->rec2d[1], g2cmd->rec2d[2], g2cmd->rec2d[3], idx, cmd);

  if (g2cmd->rec2d[idx] != INOP_2D) {
    g2cmd->rec2d[idx] = (short)cmd;
    return OK;
  }
  return ERROR;
}
/******************************************************************************/

int Pms2IO::makeDummyData (struct P2d_rec *rec)

// Initialize a record with dummy data.
{
  int j;

  rec->hour = 12;
  rec->minute = 5;
  rec->sec = ++rec->sec % 60;
  rec->tas = 25;
  rec->overld = 500;
  if (!(rec->sec % 2))
    strncpy ((char*)rec, P1_STR, strlen(C1_STR));
  else
    strncpy ((char*)rec, C1_STR, strlen(C1_STR));

  for (j = 0; j < P2D_DATA; j += 4)
    *(int*)&rec->data[j] = j;
  rec->data[P2D_DATA-1] = 1;
}
/******************************************************************************/
