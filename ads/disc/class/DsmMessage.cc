/* DsmMessage.cc
   Manages sending and receiving status and command messages.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmMessage.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
#ifndef VXWORKS
DsmMessage::DsmMessage (DsmNet &dn) : dsm_net (dn)
#else 
DsmMessage::DsmMessage (MsgQueue &tQ, MsgQueue &rQ) :tx_msgQ (tQ), rx_msgQ (rQ)
#endif

// Constructor.
{
#ifndef VXWORKS
  time_t      ct;

  logFP = fopen("/jnet/local/dev/winds/log/dsmAsync.log", "a");

  if (logFP == NULL)
    logFP = fopen("/jnet/local/winds/log/dsmAsync.log", "a");

  if (logFP)
    {
    ct = time(NULL);
    fprintf(logFP, "dsmAsync: reset %s", ctime(&ct));
    }
  else
    fprintf(stderr, "dsmAsync:DsmMessage can't open logfile.\n");

#endif
}

/*****************************************************************************/
int DsmMessage::readMsg ()

// Checks for a message available from the net, and copies it into the local
// MsgPacket. Returns true if an new message is available.
{
#ifndef VXWORKS
  if (dsm_net.readNet () == DSM_MSG_DATA) {
    strcpy (rx_msg, dsm_net.dsm_buf->getBuf());		// get the message
    dsm_net.dsm_buf->releaseBuf();
#else
  if (rx_msgQ.msgRecv (rx_msg, DSM_MSG_MAX_LEN) != ERROR) {
//  printf ("rx_msg = %s\n", rx_msg);
#endif

    switch (atoi(rx_msg)) {		// parse the message depending on type

      case ANALOG_MSG:
        parseAnalogMsg();
        break;

      case DATE_MSG:
        parseDateMsg();
        break;
    
      case DSMTIME_MSG:
        parseDSMTimeMsg();
        break;
    
      case DIGOUT_MSG:
        parseDigOutMsg();
        break;
    
      case FLIGHT_MSG:
        parseFlightMsg();
        break;

      case MCR_MSG:
        parseMcrMsg();
        break;
    
      case NET_MSG:
        parseNetMsg();
        break;
    
      case PMS1_MSG:
        parsePms1Msg();
        break;
    
      case PMS2_MSG:
        parsePms2Msg();
        break;
    
      case THDG_MSG:
        parseThdgMsg();
        break;
    
      case STATUS_MSG:
        parseStatusMsg();
        break;
    
      case TAPE_MSG:
        parseTapeMsg();
        break;

      case TASALT_MSG:
        parseTasAltMsg();
        break;

      case TIME_MSG:
        parseTimeMsg();
        break;

      case LOG_MSG:
        parseLogMsg();
        break;

      default:
        fprintf (stderr,"DsmMessage: Unknown message type received.\n");
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

/*****************************************************************************/
void DsmMessage::sendAnalogMsg (int action, char *locn, int adr, int volt, 
                                int gain, int offset, char *msg_str)
{
  sprintf(tx_msg, "%2d %1d %11s %2d %2d %2d %2d %s", ANALOG_MSG, 
           action, locn, adr, volt, gain, offset, msg_str);
//printf("sendAnalogMsg: %s\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendFlightMsg (char *flight_number)
{
  sprintf(tx_msg, "%2d %s", FLIGHT_MSG, flight_number);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendTHDG (float thdg)
{
  sprintf(tx_msg, "%2d %f", THDG_MSG, thdg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendDigOutMsg(
		int action, char *locn,
		int connector, int channel, 
		char *msg_str)
 
// Builds and sends a net message.
{
  sprintf(tx_msg, "%2d %1d %11s %1d %1d %s", DIGOUT_MSG, action,
		locn, connector, channel, msg_str);

//printf("sendDigOutMsg: %sT\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendDSMTimeMsg(Hdr_blk *buf)
{
  sprintf(tx_msg, "%2d %s %2d %2d %2d", DSMTIME_MSG, buf->dsm_locn,
	ntohs(buf->hour), ntohs(buf->minute), ntohs(buf->second));
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendMcrMsg (int action, int value, char *locn, char *msg_str)
{
  sprintf(tx_msg, "%2d %1d %6d %11s %s", MCR_MSG, action, value, locn,msg_str);
//printf("sendMcrMsg: %sT\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendNetMsg (int action, char *name, char *msg_str)
{
  sprintf(tx_msg, "%2d %1d %11s %s", NET_MSG, action, name, msg_str);
//printf("sendNetMsg: %s\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendPms1Msg (int action, char *name, char *locn, char *msg_str)
{
  sprintf(tx_msg,"%2d %1d %11s %11s %s", PMS1_MSG, action, name, locn, msg_str);
//printf("sendPms1Msg: %s\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendPms2Msg (int action, int value, char *name, char *locn, 
                              char *msg_str)
{
  sprintf(tx_msg, "%2d %1d %1d %11s %11s %s", PMS2_MSG,
           action, value, name, locn, msg_str);
//printf("sendPms2Msg: %s\n", tx_msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendStatusMsg (char *msg)
{
  sprintf(tx_msg, "%2d %s", STATUS_MSG, msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendLogMsg (char *msg, char *name)
{
  sprintf(tx_msg, "%2d %11s %s", LOG_MSG, name, msg);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendTapeMsg (int action, int drive, char *msg_str)
{
  sprintf (tx_msg, "%2d %1d %1d %s", TAPE_MSG, action, drive, msg_str);
  sendMsg();
}

/*****************************************************************************/
void DsmMessage::sendTimeMsg(
                 int year, int month, int day, int hour, int minute, int second)
{
  sprintf(tx_msg, "%2d %2d %2d %2d %2d %2d %2d", TIME_MSG,
           year, month, day, hour, minute, second);
  sendMsg();
}

/******************************************************************************
** Private Functions
******************************************************************************/
 
void DsmMessage::parseAnalogMsg ()
 
// Parses an analog message to extract the message fields.
// Received fields are type, action, locn, adr, volt, gain, offset, msg. 
{
//printf ("parseAnalogMsg: %s\n", rx_msg);

  sscanf (rx_msg, "%d %d %s %d %d %d %d", &rx_type, &rx_action, rx_locn,
          &i_data1, &i_data2, &i_data3, &i_data4);
  strcpy (rx_string, rx_msg + 29);
}

/*****************************************************************************/
void DsmMessage::parseDateMsg ()
{
  sscanf(rx_msg, "%d %d %d %d", &rx_type, &rx_year, &rx_month, &rx_day);
}

/*****************************************************************************/
void DsmMessage::parseDSMTimeMsg ()
{
  sscanf(rx_msg, "%d %s %d %d %d", &rx_type, rx_name, &rx_hour, &rx_minute, &rx_second);
}

/*****************************************************************************/
void DsmMessage::parseThdgMsg ()
{
  sscanf(rx_msg, "%d %f", &rx_type, &rx_thdg);
}

/*****************************************************************************/
void DsmMessage::parseFlightMsg ()
{
  sscanf(rx_msg, "%d %s", &rx_type, rx_flight);
}

/*****************************************************************************/
void DsmMessage::parseDigOutMsg ()
{
  sscanf(rx_msg, "%d %d %s %d %d",
          &rx_type, &rx_action, rx_locn, &i_data1, &i_data2);

  strcpy(rx_string, rx_msg + 21);
}

/*****************************************************************************/
void DsmMessage::parseMcrMsg ()
{
//printf("parseMcrMsg: %s\n", rx_msg);
  sscanf(rx_msg, "%d %d %d %s", &rx_type, &rx_action, &rx_value, rx_locn);
  strcpy(rx_string, rx_msg + 24);
}

/*****************************************************************************/
void DsmMessage::parseNetMsg ()
{
//printf("parseNetMsg: %s\n", rx_msg);
  sscanf(rx_msg, "%d %d %s", &rx_type, &rx_action, rx_name);
  strcpy(rx_string, rx_msg + 17);
}

/*****************************************************************************/
void DsmMessage::parsePms1Msg ()
{
//printf("parsePms1Msg: %s\n", rx_msg);
  sscanf(rx_msg, "%d %d %s %s", &rx_type, &rx_action, rx_name, rx_locn);
  strcpy(rx_string, rx_msg + 29);
}

/*****************************************************************************/
void DsmMessage::parsePms2Msg ()
{
//printf("parsePms2Msg: %s\n", rx_msg);
  sscanf(rx_msg, "%d %d %d %s %s", 
          &rx_type, &rx_action, &rx_value, rx_name, rx_locn);
  strcpy(rx_string, rx_msg + 31);
}

/*****************************************************************************/
void DsmMessage::parseStatusMsg ()
{
  sscanf(rx_msg, "%d", &rx_type);
  strcpy(rx_string, rx_msg + 3);
}

/*****************************************************************************/
void DsmMessage::parseLogMsg ()
{
  sscanf(rx_msg, "%d %s", &rx_type, &rx_name);
  strcpy(rx_string, rx_msg + 15);
}

/*****************************************************************************/
void DsmMessage::parseTapeMsg ()
{
  sscanf(rx_msg, "%d %d %d", &rx_type, &rx_action, &rx_drive);
  strcpy(rx_string, rx_msg + 7);
}

/*****************************************************************************/
void DsmMessage::parseTasAltMsg ()
{
  sscanf(rx_msg, "%d %f %f", &rx_type, &rx_tas, &rx_alt);
//printf("rx_type = %d, rx_tas = %f, rx_alt = %f.\n", rx_type, rx_tas, rx_alt);
}

/*****************************************************************************/
void DsmMessage::parseTimeMsg ()
{
  sscanf(rx_msg, "%d %d %d %d %d %d %d", 
    &rx_type, &rx_year, &rx_month, &rx_day, &rx_hour, &rx_minute, &rx_second);
}

/*****************************************************************************/
#ifndef VXWORKS
void DsmMessage::logMsgToFile(char *s)
{
  if (logFP) {
    fprintf(logFP, "%s: %s\n", rx_name, s);
    fflush(logFP);
    }
}
#endif
