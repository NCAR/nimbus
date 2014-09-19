// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4; -*-
// vim: set shiftwidth=4 softtabstop=4 expandtab:
/*

   Copyright 2005 UCAR, NCAR, All Rights Reserved

   $LastChangedDate$

   $LastChangedRevision$

   $LastChangedBy$

   $HeadURL$

*/

#include "CMIGITS3_Serial.h"
#include <nidas/core/Parameter.h>
#include <nidas/core/PhysConstants.h>
#include <nidas/util/UTime.h>
#include <nidas/util/Logger.h>

#include <unistd.h>
#include <sstream>
#include <iomanip>

using namespace nidas::dynld::raf;
using namespace nidas::core;
using namespace std;

namespace n_u = nidas::util;

const int CM3501_SAMPLE_ID = 1;
const int CM3500_SAMPLE_ID = 2;
const int CM3502_SAMPLE_ID = 3;

/*
 *  The C-MIGITS synch word (aka message header)
 *   as well as some constants for converting values.
 */
const char CMIGITS3_Serial::SyncBytes[2] = { '\xff', '\x81' };

const uint16_t  CMIGITS3_Serial::ReadyFlag = 0x8000;    // Indication of CMIGITS Ready State

const unsigned int  CMIGITS3_Serial::CM_HDR_SIZE = 5;       // Size of the CMigits header elements
const unsigned int   CMIGITS3_Serial::CM_3500_SIZE = 16;     // Size of Msg 3500 Data Segment minus chksum
const unsigned int   CMIGITS3_Serial::CM_3501_SIZE = 22;     // Size of Msg 3501 Data Segment minus chksum
const unsigned int   CMIGITS3_Serial::CM_3502_SIZE = 16;     // Size of Msg 3502 Data Segment minus chksum
const double    CMIGITS3_Serial::cm31 = 4.656613e-10;   // Used for lat, lon, pitch, roll and true heading
const double    CMIGITS3_Serial::cm21 = 4.768372e-07;   // Used for VelN, VelE and VelUp
const double    CMIGITS3_Serial::cm16 = 1.525879e-05;   // Used for altitude 

namespace n_u = nidas::util;

/* static */
const n_u::EndianConverter* CMIGITS3_Serial::_cm2dsmcvtr =
n_u::EndianConverter::getConverter(
        n_u::EndianConverter::EC_LITTLE_ENDIAN);

/* static */
const n_u::EndianConverter* CMIGITS3_Serial::_dsm2cmcvtr =
n_u::EndianConverter::getConverter(
        n_u::EndianConverter::getHostEndianness(),
        n_u::EndianConverter::EC_LITTLE_ENDIAN);

NIDAS_CREATOR_FUNCTION_NS(raf, CMIGITS3_Serial)

CMIGITS3_Serial::CMIGITS3_Serial():DSMSerialSensor(),
    _CM3500Nvars(0), _CM3501Nvars(0), _CM3502Nvars(0),
    CM3501Id(0), CM3502Id(0), CM3500Id(0),
    _misalignedCounter(0),
    _noTrueHeadingCnt(0), _noSpeedCnt(0),
    _instrument(),
    _lever_arm_X(0), _lever_arm_Y(0), _lever_arm_Z(0),
    _gps(true),_delayCntr(0),_sendDelay(5)
{
}

CMIGITS3_Serial::~CMIGITS3_Serial()
{

}

void CMIGITS3_Serial::open(int flags) throw(n_u::IOException)
{
    DSMSerialSensor::open(flags);

    _delayCntr = 0;
    _sendDelay = 5;

    setInitMode();
    setDisconnect();

    // Fails to enable the accel packet.  Not rejected and acks.
    sendConfigForAccel();

    sendLeverArm();
    if (_instrument.compare("LAMS") == 0)
    {
        WLOG(("%s: CMIGITS init, LAMS enabled",getName().c_str()));
        sendOrientationLAMS();
    }

    // testing only.  needs to be done in derivedDataNotifier() for production.
    // sendControlInit3510(0.0, 309.0, 39.91, -105.119, 1738.25);

    if (DerivedDataReader::getInstance())
        DerivedDataReader::getInstance()->addClient(this);
    else
        n_u::Logger::getInstance()->log(LOG_WARNING,"%s: %s",
                getName().c_str(),
                "no DerivedDataReader. <dsm> tag needs a derivedData attribute");
}


void CMIGITS3_Serial::close() throw(n_u::IOException)
{
    if (DerivedDataReader::getInstance())
        DerivedDataReader::getInstance()->removeClient(this);
    DSMSerialSensor::close();
}

uint16_t CMIGITS3_Serial::checkSum(const uint16_t data[], int nWords)
{
    uint16_t ckSum = 0;
    for (int i = 0; i < nWords; i++) 
        ckSum += data[i];
    ckSum = ~ckSum + 1;

    return ckSum;
}

uint16_t CMIGITS3_Serial::checkSumRaw(const char* data, int nWords)
{
    uint16_t ckSum = 0;
    for (int i = 0; i < nWords; i++) {
        ckSum += _cm2dsmcvtr->uint16Value(data);
        data += sizeof(uint16_t);
    }
    ckSum = ~ckSum + 1;

    return ckSum;
}

uint16_t CMIGITS3_Serial::checkSumRaw(const uint16_t* data, int nWords)
{
    return checkSumRaw((const char*) data,nWords);
}

SampleScanner* CMIGITS3_Serial::buildSampleScanner()
throw(n_u::InvalidParameterException)
{
    Scanner* scanr = new Scanner();
    scanr->setMessageParameters(0,string(SyncBytes,sizeof(SyncBytes)),false);
    scanr->setUsecsPerByte(getUsecsPerByte());
    return scanr;
}

bool CMIGITS3_Serial::readAckHandshake(uint16_t id) throw(n_u::IOException)
{
    int nsamp = 0;
    bool handshake_ok = false;
    bool done = false;

    while (!done) {
        try {
            readBuffer(1 * MSECS_PER_SEC);

            // process all samples in buffer
            for (Sample* samp = nextSample(); samp; samp = nextSample()) {

                distributeRaw(samp);        // send it on to the clients

                nsamp++;

                if (!done && samp->getDataByteLength() >= CM_HDR_SIZE * sizeof(uint16_t)) {
                    const uint16_t* msg = (const uint16_t*) samp->getConstVoidDataPtr();
                    uint16_t inId = _cm2dsmcvtr->uint16Value(msg[1]);

                    if (inId == id) {
                        switch(parseAckHandshake(msg)) {
                        case NAK:
                        case HANDSHAKE_REJECT:
                            done = true;
                            break;
                        case HANDSHAKE_OK:
                            handshake_ok = true;
                            done = true;
                            break;
                        default:
                            /* ACK ok, but not a handshake message */
                            break;
                        }
                    }
                }
            }
            if (done) break;
            if (nsamp > 20) {
                WLOG(("%s: CMIGITS readAckHandshake(): expected handshake for message %d not received",
                            getName().c_str(),id));
                done = true;
            }
        }
        catch (const n_u::IOTimeoutException& e) {
            throw e;
        }
    }
    return handshake_ok;
}

enum CMIGITS3_Serial::MsgStatus CMIGITS3_Serial::parseAckHandshake(const uint16_t* msgWords)
{
    uint16_t sync = _cm2dsmcvtr->uint16Value(msgWords[0]);
    uint16_t msgId = _cm2dsmcvtr->uint16Value(msgWords[1]);
    uint16_t nMsgWords = _cm2dsmcvtr->uint16Value(msgWords[2]);
    uint16_t flags = _cm2dsmcvtr->uint16Value(msgWords[3]);

    WLOG(("%s: CMIGITS response packet %x %d %d %x",
                getName().c_str(), sync, msgId, nMsgWords, flags));

    bool handshake = flags & (1 << 9);
    bool reject = flags & (1 << 8);
    bool nak = flags & (1 << 10);
    bool ack = flags & (1 << 11);
    std::string handshakeMsg = std::string("HANDSHAKE");
    handshakeMsg += reject ? "(cmd rejected) " : "(cmd accepted) ";

    WLOG(("%s: CMIGITS Got reply message for %d: %s%s%s%s%s",
                getName().c_str(),msgId,
                ((flags & (1 << 7)) ? "INVALID " : ""),
                (handshake ? handshakeMsg.c_str() : ""),
                (nak ? "NAK " : ""),
                (ack ? "ACK " : ""),
                ((flags & (1 << 12)) ? "(ACK request)" : "")) );

    // First response to a command is ACK or NAK. If we got a NAK, we can
    // just try the same command again.
    if (nak) {
        WLOG(("%s: CMIGITS received NAK for message %d, indicating bad data checksum",
                    getName().c_str(),msgId));
        return NAK;
    }

    // Second response to a command is a handshake reply telling whether the
    // command was accepted or rejected.
    if (handshake) {
        // message is a response to a handshake request from the HV (DSM)
        if (reject) {
            // Sometimes a command is rejected because the C-MIGITS is
            // busy reconfiguring from a previous command. Sleep briefly and try
            // the rejected command again (up to 3 times).
            WLOG(("%s: CMIGITS message id %d command handshake rejected",getName().c_str(), msgId));
            return HANDSHAKE_REJECT;
        } else {
            WLOG(("%s: CMIGITS message id %d command handshake accepted", getName().c_str(), msgId));
            return HANDSHAKE_OK;
        }
    }
    return NOT_HANDSHAKE;
}

void CMIGITS3_Serial::derivedDataNotify(const nidas::core::DerivedDataReader * s) throw()
{

    float speedValue = s->getGroundSpeed();
    if (::isnan(speedValue)) {
        if (!(_noSpeedCnt++ % 60))
            WLOG(("%s: CMIGITS Speed is NAN - skipping CMIGITS Init", getName().c_str()));
        return;
    }

    // complain about no heading every 60 seconds
    float trueHeading = s->getTrueHeading();
    if (::isnan(trueHeading)) {
        if (!(_noTrueHeadingCnt++ % 60))
            WLOG(("%s: CMIGITS True heading from INS is NAN - check INS is up", getName().c_str()));
        return;
    }

    /* Wait 2.5 minutes from first valid heading to let the INS finish aligning.
     * If speed is over 50 then assume mid air re-init, don't wait 2.5 minutes.
     */
    if (speedValue < 50.0 && _delayCntr++ < 150)
        return;

    WLOG(("%s: CMIGITS: True Heading: %f",getName().c_str(), s->getTrueHeading())); 

    try 
    {
        if (_sendDelay <= 0)
        {
            double lat = s->getLatitude();
            double lon = s->getLongitude();
            double alt = s->getAltitude();

            sendControlInit3510(speedValue, trueHeading, lat, lon, alt);

            WLOG(("%s: Sent True Heading up to CMIGITS", getName().c_str()));

            // No longer need notices of derived data
            if (DerivedDataReader::getInstance())
                DerivedDataReader::getInstance()->removeClient(this);
        }
        else
            _sendDelay--;
    }
    catch(const n_u::IOException & e)
    {
        n_u::Logger::getInstance()->log(LOG_WARNING, "%s", e.what());
        _sendDelay = 5;
    }
}

CMIGITS3_Serial::Message::Message(uint16_t messageID, uint16_t dataWordCount):
    _dwc(dataWordCount),_msg(CMIGITS3_Serial::CM_HDR_SIZE + _dwc + 1),
    _cksummed(false)
{
    ::memcpy((char *)&_msg.front(),CMIGITS3_Serial::SyncBytes,
            sizeof(CMIGITS3_Serial::SyncBytes));
    setWord(messageID, 1);

    // write data length to word 2 of header
    setWord(_dwc,2);
}

void CMIGITS3_Serial::Message::setWord(uint16_t val,unsigned int index)
{
    /* NIDAS on data systems sets NDEBUG. We'll unset it here to enable this assert. */
#ifdef NDEBUG
#define NDEBUG_TRUE
#undef NDEBUG
#endif
    assert(index < _msg.size());
    _dsm2cmcvtr->uint16Copy(val,(char*)( &_msg.front() + index));
    _cksummed = false;
#ifdef NDEBUG_TRUE
#define NDEBUG
#endif
}

void CMIGITS3_Serial::Message::setWord32(int32_t val,unsigned int index)
{
    /* NIDAS on data systems sets NDEBUG. We'll unset it here to enable this assert. */
#ifdef NDEBUG
#define NDEBUG_TRUE
#undef NDEBUG
#endif
    assert(index < _msg.size() - 1);
    _dsm2cmcvtr->int32Copy(val, (char*)( &_msg.front() + index));
    _cksummed = false;
#ifdef NDEBUG_TRUE
#define NDEBUG
#endif
}

string CMIGITS3_Serial::Message::toHexString()
{
    ostringstream ost;
    ost << hex << uppercase << setfill('0');

    const unsigned char* cp0 = getPtr();    // updates checksums if necessary

    for (const unsigned char* cp = cp0; cp < cp0 + getTotalByteLength(); cp += 2 )
    {
        uint16_t val = _cm2dsmcvtr->uint16Value(cp);
        ost << setw(4) << val << ' ';
    }
    return ost.str();
}

const unsigned char* CMIGITS3_Serial::Message::getPtr()
{
    if (!_cksummed) doCheckSums();
    return (const unsigned char*) &_msg.front();
}

void CMIGITS3_Serial::Message::doCheckSums()
{
    // compute and write header checksum to end of header
    uint16_t ckSum = CMIGITS3_Serial::checkSumRaw(&_msg.front(),
            CMIGITS3_Serial::CM_HDR_SIZE - 1);
    setWord(ckSum,CMIGITS3_Serial::CM_HDR_SIZE - 1);

    // Compute and write data portion checksum,  if there is a data portion
    if (_dwc > 0) {
        ckSum = CMIGITS3_Serial::checkSumRaw(&_msg.front() + CM_HDR_SIZE,_dwc);
        setWord(ckSum,CMIGITS3_Serial::CM_HDR_SIZE + _dwc);
    }
    _cksummed = true;
}

void CMIGITS3_Serial::setInitMode() throw(n_u::IOException)
{
    WLOG(("%s: CMIGITS setInit", getName().c_str()));

    // Create and send a 3510 (Control and Initialization) message to put
    Message msg(3510,21);   // Message 3510
    msg.setWord(0x9200,3); // Generate Ack, Handshake, Ready

    // set data validity bits
    msg.setWord(0x0001,5);     // set mode
    msg.setWord(0x0002,6);     // mode: 2 -> initialization mode

    WLOG(("%s: CMIGITS setInitMode, msg=%s",getName().c_str(),msg.toHexString().c_str()));

    // Send message
    write(msg.getPtr(),msg.getTotalByteLength());
    try {
    if (!readAckHandshake(3510))
        throw n_u::IOException(getName(),"setInitMode","ACK and handshake not received");
    }
    catch(const n_u::IOException& e) {
        cerr << e.what() << endl;
    }
}

void CMIGITS3_Serial::setDisconnect() throw(n_u::IOException)
{
    WLOG(("%s: CMIGITS setDisconnect", getName().c_str()));

    // Disconnect and revert to default messages.
    Message msg(0,0);   // Message 0, reset
    msg.setWord(0x9220,3); // Disconnect, Generate Ack, Handshake, Ready

    write(msg.getPtr(),msg.getTotalByteLength());
    try {
    readAckHandshake(0);
    }
    catch(const n_u::IOException& e) {
        cerr << e.what() << endl;
    }
}


void CMIGITS3_Serial::sendConfigForAccel() throw (n_u::IOException)
{
    // Get CMIGITS to ship message 3502 and at 10Hz so that we can derive acceleration information.

    // Create CMIGITS HEADER and DATA packet for parameter control message
    Message msg(3504,5);   // Message 3504
    msg.setWord(0x9200,3); // Handshake - generate Ack

    // uint16_t bits = 0x0020 | 0x0040;       // Engage 3501 and 3502 xmit rates.
    uint16_t bits = 0x0180;       // Engage 3512

    msg.setWord(bits, 5);

    bits = 0x0002 | 0x0008;         // Xmit both at 10 Hz
    msg.setWord(bits, 9);

    WLOG(("%s: CMIGITS sendConfigForAccel, msg=%s",getName().c_str(),msg.toHexString().c_str()));

    // Send message
    write(msg.getPtr(),msg.getTotalByteLength());
    try {
    if (!readAckHandshake(3504))
        throw n_u::IOException(getName(),"sendConfigForAccel","ACK and handshake not received");
    }
    catch(const n_u::IOException& e) {
        cerr << e.what() << endl;
    }
}

void CMIGITS3_Serial::sendOrientationLAMS() throw(n_u::IOException)
{

    Message msg(3511,22);   // Message 3511
    msg.setWord(0x9200,3);  // Handshake - generate Ack

    msg.setWord(0x0001, 5);   // Sensor to Body xform.

    // Rotate -90 degrees along Y axis.  Pushed forward 90 degrees.
    msg.setWord32(packFloat32( 0.0, 1), 6);
    msg.setWord32(packFloat32( 0.0, 1), 8);
    msg.setWord32(packFloat32(-1.0, 1),10);
    msg.setWord32(packFloat32( 0.0, 1),12);
    msg.setWord32(packFloat32( 1.0, 1),14);
    msg.setWord32(packFloat32( 0.0, 1),16);
    msg.setWord32(packFloat32( 1.0, 1),18);
    msg.setWord32(packFloat32( 0.0, 1),20);
    msg.setWord32(packFloat32( 0.0, 1),22);

    WLOG(("%s: CMIGITS sendOrientationLAMS, msg=%s",
                getName().c_str(),msg.toHexString().c_str()));

    // Send message
    write(msg.getPtr(),msg.getTotalByteLength());
    try {
    if (!readAckHandshake(3511))
        throw n_u::IOException(getName(),"sendOrientationLAMS","ACK and handshake not received");
    }
    catch(const n_u::IOException& e) {
        cerr << e.what() << endl;
    }
}

void CMIGITS3_Serial::sendLeverArm() throw(n_u::IOException)
{

    Message msg(3511,22);   // Message 3511
    msg.setWord(0x9200,3);  // Handshake - generate Ack

    msg.setWord(0x0002, 5);   // Lever arm values

    // Lever arm values.
    msg.setWord(_lever_arm_X, 24);   // Sensor to Body xform.
    msg.setWord(_lever_arm_Y, 25);   // Sensor to Body xform.
    msg.setWord(_lever_arm_Z, 26);   // Sensor to Body xform.

    WLOG(("%s: CMIGITS sendLeverArm, msg=%s",getName().c_str(),msg.toHexString().c_str()));

    // Send message
    write(msg.getPtr(),msg.getTotalByteLength());
    try {
    if (!readAckHandshake(3511))
        throw n_u::IOException(getName(),"sendLeverArm","ACK and handshake not received");
    }
    catch(const n_u::IOException& e) {
        cerr << e.what() << endl;
    }
}
void CMIGITS3_Serial::sendControlInit3510(double gspd, double thdg, double lat, double lon, double alt)
    throw(n_u::IOException)
{
    WLOG(("%s: CMIGITS sendControlInit3510, thdg = %f", getName().c_str(), thdg));

    // Create CMIGITS HEADER and DATA packet for Control and Initialization and a checksum
    Message msg(3510,21);   // Message 3510
    msg.setWord(0x9200,3);  // Handshake - generate Ack

    int16_t ivalue;
    uint16_t dvalidity = 0;

    if (thdg > 180.0) thdg -= 360.0;  // INS is 0 to 360, CMIGITS is -180 to 180
    thdg *= 100;

    // Note for CMIGITS III in C130 the following was 0x0011 - for lab testing that worked its 005B
    // 0x005B also means to use GPS for position and velocity initialization
    // 0x0011 means just use heading and alignment for initialization, no GPS.
    WLOG(("%s: CMIGITS expecting GPS feed=%d", getName().c_str(), _gps));
    if (_gps)
        dvalidity = 0x0051;	// Data Validity - mode command and heading data, gps antenna
    else
        dvalidity = 0x0013;	// Data Validity - mode command and position & heading data, no gps antenna

    // Position Data
    if (dvalidity & 0x0002)
    {
        ivalue = (int16_t)lat;
        msg.setWord(ivalue,7);

        ivalue = (int16_t)((lat - ivalue) * 60);
        msg.setWord(ivalue,8);

        ivalue = (int16_t)lon;
        msg.setWord(ivalue,10);

        ivalue = (int16_t)((lon - ivalue) * 60);
        msg.setWord(ivalue,11);

        int32_t ialt = (int32_t)alt;
        msg.setWord32(ialt,13);

    }

    // Velocity Data
    if (gspd > 2)
    {
        WLOG(("%s: CMIGITS performing course air alignment.",getName().c_str()));

        dvalidity |= 0x0024;     // specify align/nav sequence and velocity data.

        ivalue = (int16_t)gspd;
        msg.setWord(ivalue,15);

        ivalue = (int16_t)0x0175;   // course air alignment.
        msg.setWord(ivalue,23);

    }
    else
    {
        WLOG(("%s: CMIGITS performing fine alignment.",getName().c_str()));

        dvalidity |= 0x0020;     // specify align/nav sequence

        ivalue = (int16_t)0x0174;   // fine ground alignment
        msg.setWord(ivalue,23);
    }

    // True Heading.
    if (dvalidity & 0x0010)
    {
        ivalue = (int16_t) lrint(thdg);
        msg.setWord(ivalue,22);

        WLOG(("%s: CMIGITS True Heading After correction and signed short convert is: %d", getName().c_str(), ivalue));
    }

    // Store the Data Validity word last.
    msg.setWord(dvalidity,5);

    WLOG(("%s: CMIGITS Sending controlInit3510 : %s", getName().c_str(), msg.toHexString().c_str()));

    // Send init message
    write(msg.getPtr(),msg.getTotalByteLength());
    // don't wait for ACK, handshake
}

int32_t CMIGITS3_Serial::packFloat32(double value, int binaryScaling)
{
    double fraction = value / (1 << binaryScaling);
    return int32_t(fraction * 0x80000000UL);
}

void CMIGITS3_Serial::fromDOMElement(const xercesc::DOMElement* node)
throw(n_u::InvalidParameterException)
{
    DSMSerialSensor::fromDOMElement(node);

    const Parameter *p;

    // Accepted values, "LAMS" and "GUSTPOD" are the current two choices.
    // Not required for stand alone use.
    // e.g. LAMS to modify rotation configuration.
    p = getParameter("INSTRUMENT");
    if (p)
        _instrument = p->getStringValue(0);

    p = getParameter("LEVER_ARM_X");
    if (p)
        _lever_arm_X = (int16_t)p->getNumericValue(0);

    p = getParameter("LEVER_ARM_Y");
    if (p)
        _lever_arm_Y = (int16_t)p->getNumericValue(0);

    p = getParameter("LEVER_ARM_Z");
    if (p)
        _lever_arm_Z = (int16_t)p->getNumericValue(0);

    // Is there a GPS antenna hooked up.
    p = getParameter("GPS_FEED");
    if (p)
        _gps = p->getNumericValue(0);
}


void CMIGITS3_Serial::addSampleTag(SampleTag* stag)
throw(n_u::InvalidParameterException)
{
    DSMSerialSensor::addSampleTag(stag);

    switch(stag->getSampleId()) {
    case CM3501_SAMPLE_ID:
        _CM3501Nvars = stag->getVariables().size();
        CM3501Id = stag->getId();
        break;
    case CM3502_SAMPLE_ID:
        _CM3502Nvars = stag->getVariables().size();
        CM3502Id = stag->getId();
        break;
    case CM3500_SAMPLE_ID:
        _CM3500Nvars = stag->getVariables().size();
        CM3500Id = stag->getId();
        break;
    default:
        {
            ostringstream ost;
            ost << "must be either " <<
                CM3501_SAMPLE_ID << " (CMig3501) or "  <<
                CM3502_SAMPLE_ID << " (CMig3502) or "  <<
                CM3500_SAMPLE_ID << "(CMig3500)";
            throw n_u::InvalidParameterException(getName(), "sample id",ost.str());
        }
        break;
    }
}


void CMIGITS3_Serial::parseDeltaVandTheta( const unsigned short* input,
        float *dout, int /* nvars */, dsm_time_t /* tt */) throw ()
{
    // TODO: do a checksum check
    int CdelThetaX, CdelThetaY, CdelThetaZ, CdelVelX, CdelVelY, CdelVelZ;
    double DCdelThetaX, DCdelThetaY, DCdelThetaZ, DCdelVelX, DCdelVelY, DCdelVelZ;
    int iout = 0;

    input += 4;            // Align to Delta Theta X - Skip the CMIGITS seconds of the day - seems faulty

    // NOTE: I'm guessing that the multiplier for Theta is the same as for pitch and roll
    CdelThetaX = _cm2dsmcvtr->int32Value(input);
    DCdelThetaX = 180.0 * cm31 * CdelThetaX;
    dout[iout++] = DCdelThetaX;
    input += 2;

    CdelThetaY = _cm2dsmcvtr->int32Value(input);
    DCdelThetaY = 180.0 * cm31 * CdelThetaY;
    dout[iout++] = DCdelThetaY;
    input += 2;

    CdelThetaZ = _cm2dsmcvtr->int32Value(input);
    DCdelThetaZ = 180.0 * cm31 * CdelThetaZ;
    dout[iout++] = DCdelThetaZ;
    input += 2;

    // NOTE: I'm guessing that the muliplier for delta Velocity is the same as for Velocity
    CdelVelX = _cm2dsmcvtr->int32Value(input);
    DCdelVelX = cm21 * CdelVelX;
    dout[iout++] = DCdelVelX;
    input += 2;

    CdelVelY = _cm2dsmcvtr->int32Value(input);
    DCdelVelY = cm21 * CdelVelY;
    dout[iout++] = DCdelVelY;
    input += 2;

    CdelVelZ = _cm2dsmcvtr->int32Value(input);
    DCdelVelZ = cm21 * CdelVelZ;
    dout[iout++] = DCdelVelZ;
}

void CMIGITS3_Serial::parseNavSoln(const unsigned short* input,
        float *dout, int /* nvars */, dsm_time_t /* tt */) throw ()
{
    // TODO: do a checksum check
    int Clatitude, Clongitude, Caltitude, Cvelocity_north, Cvelocity_east, Cvelocity_up, Cpitch, Croll, Ctrue_heading;
    double DClatitude, DClongitude, DCaltitude, DCvelocity_north, DCvelocity_east, DCvelocity_up, DCpitch, DCroll, DCtrue_heading;
    int iout=0;

    input += 4;            // Align to Latitude - Skip the CMIGITS seconds of the day - seems faulty

    Clatitude = _cm2dsmcvtr->int32Value(input);
    DClatitude = 180.0 * cm31 * Clatitude;
    dout[iout++] = DClatitude;
    input += 2;

    Clongitude = _cm2dsmcvtr->int32Value(input);
    DClongitude = 180.0 * cm31 * Clongitude;
    dout[iout++] = DClongitude;
    input += 2;

    Caltitude = _cm2dsmcvtr->int32Value(input);
    DCaltitude = cm16 * Caltitude;
    dout[iout++] = DCaltitude;
    input += 2;

    Cvelocity_north = _cm2dsmcvtr->int32Value(input);
    DCvelocity_north = cm21 * Cvelocity_north;
    dout[iout++] = DCvelocity_north;
    input += 2;

    Cvelocity_east = _cm2dsmcvtr->int32Value(input);
    DCvelocity_east = cm21 * Cvelocity_east;
    dout[iout++] = DCvelocity_east;
    input += 2;

    Cvelocity_up = _cm2dsmcvtr->int32Value(input);
    DCvelocity_up = cm21 * Cvelocity_up;
    dout[iout++] = DCvelocity_up;
    input += 2;

    Cpitch = _cm2dsmcvtr->int32Value(input);
    DCpitch = 180.0 * cm31 * Cpitch;
    dout[iout++] = DCpitch;
    input += 2;

    Croll = _cm2dsmcvtr->int32Value(input);
    DCroll = 180.0 * cm31 * Croll;
    dout[iout++] = DCroll;
    input += 2;

    Ctrue_heading = _cm2dsmcvtr->int32Value(input);
    DCtrue_heading = 180.0 * cm31 * Ctrue_heading;
    dout[iout++] = DCtrue_heading;
}


void CMIGITS3_Serial::parseSysStat(const unsigned short* input,
        float *dout, int /* nvars */, dsm_time_t /* tt */) throw ()
{
    // TODO: do a checksum check
    short CMode, CStatVal, CNumSat, CNumPosProc, CNumVelProc, CFOM;
    int CHorizErr, CVertErr, CVelErr;
    double DCHorizErr, DCVertErr, DCVelErr;
    int iout=0;

    input += 4;            // Align to Latitude - Skip the CMIGITS seconds of the day - seems faulty

    CMode = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CMode;
    input += 1;

    CStatVal = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CStatVal;
    input += 1;

    CNumSat = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CNumSat;
    input += 1;

    CNumPosProc = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CNumPosProc;
    input += 1;

    CNumVelProc = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CNumVelProc;
    input += 1;

    CFOM = _cm2dsmcvtr->int16Value(input);
    dout[iout++] = CFOM;
    input += 1;

    CHorizErr = _cm2dsmcvtr->int32Value(input);
    DCHorizErr = cm16 * CHorizErr;
    dout[iout++] = DCHorizErr;
    input += 2;

    CVertErr = _cm2dsmcvtr->int32Value(input);
    DCVertErr = cm16 * CVertErr;
    dout[iout++] = DCVertErr;
    input += 2;

    CVelErr = _cm2dsmcvtr->int32Value(input);
    DCVelErr = cm21 * CVelErr;
    dout[iout++] = DCVelErr;
}

bool CMIGITS3_Serial::process(const Sample* samp,list<const Sample*>& results)
throw()
{
    const unsigned short *packet = (const unsigned short *) samp->getConstVoidDataPtr();
    /*
       int data_byte_size = samp->getDataByteLength();

       if (data_byte_size != (CM_HDR_SIZE + CM_3501_SIZE + 1)*sizeof(short) &&
       data_byte_size != (CM_HDR_SIZE + CM_3502_SIZE + 1)*sizeof(short) &&
       data_byte_size != (CM_HDR_SIZE + CM_3500_SIZE + 1)*sizeof(short))
       return false;
       */

    if (::memcmp(packet,SyncBytes,sizeof(SyncBytes)))
    {
        ++_misalignedCounter;
        WLOG(("%s: CMIGITS Encountered record alignment error number at %s: %d  First Short: %hx",
                    getName().c_str(), 
                    n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str(),
                    _misalignedCounter, _cm2dsmcvtr->uint16Value(packet)));
        return false;
    }

    unsigned short  j=0, message_id, flag_word;
    unsigned short  cm_hdr[CM_HDR_SIZE], data_size, ckSum;
    size_t data_length = samp->getDataLength();

    if (data_length < CM_HDR_SIZE) {
        WLOG(("%s: CMIGITS record length less than expected header size at %s",
                    getName().c_str(),
                    n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str()));
        return false;
    }

    for (j = 0; j < CM_HDR_SIZE; j++) 
        cm_hdr[j] = _cm2dsmcvtr->uint16Value(packet++);

    // Compute header checksum on converted data
    ckSum = checkSum(cm_hdr, 4);
    if (ckSum != cm_hdr[4])
    {
        WLOG(("%s: CMIGITS header CkSum mismatch at %s: %X != %X, discarding.", getName().c_str(),
                    n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str(),ckSum, cm_hdr[4]));
        return false;
    }

    message_id = cm_hdr[1];
    data_size = cm_hdr[2];
    flag_word = cm_hdr[3];

/** Comment out by default.  3623 will spew this non-stop.  And interpretation of the manual
 * says it will always be true anyways.
    if (flag_word != ReadyFlag)
    {
        WLOG(("%s: CMIGITS Flag word at %s: %X for message id: %d - indicates other than only Ready State", 
                    getName().c_str(),
                    n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str(),
                    flag_word, message_id));
    }
*/

    //WLOG(("%s: CMIGITS message id: %d", getName().c_str(), message_id));
    if (message_id == 3502)    // Delta Velocity and Delta Theta packet
    {
        if (data_size < CM_3502_SIZE || _CM3502Nvars == 0)
            return false;

        SampleT<float>* outs = getSample<float>(_CM3502Nvars);
        outs->setId(CM3502Id);
        outs->setTimeTag(samp->getTimeTag());
        parseDeltaVandTheta(packet, outs->getDataPtr(), _CM3502Nvars, samp->getTimeTag());
        results.push_back(outs);
        return true;
    }

    if (message_id == 3501)    // navigation solution - 10 Hz packet 
    {
        if (data_size < CM_3501_SIZE || _CM3501Nvars == 0)
            return false;

        ckSum = checkSum(packet, 22);
        if (ckSum != packet[22])
        {
            WLOG(("%s: CMIGITS NAV packet CkSum mismatch at %s: %X != %X, discarding.",
                        getName().c_str(),
                        n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str(),
                        ckSum, packet[22]));
            return false;
        }

        SampleT<float>* outs = getSample<float>(_CM3501Nvars);
        outs->setId(CM3501Id);
        outs->setTimeTag(samp->getTimeTag());
        parseNavSoln(packet, outs->getDataPtr(), _CM3501Nvars, samp->getTimeTag());
        results.push_back(outs);
        return true;
    } 

    if (message_id == 3500) //System Status - 1 Hz packet
    {
        if (data_size < CM_3500_SIZE || _CM3500Nvars == 0)
            return false;

        ckSum = checkSum(packet, 16);
        if (ckSum != packet[16])
        {
            WLOG(("%s: CMIGITS status packet CkSum mismatch at %s: %X != %X, discarding.",
                        getName().c_str(),
                        n_u::UTime(samp->getTimeTag()).format(true,"%Y %m %d %H:%M:%S.%3f").c_str(),
                        ckSum, packet[16]));
            return false;
        }

        SampleT<float>* outs = getSample<float>(_CM3500Nvars);
        outs->setId(CM3500Id);
        outs->setTimeTag(samp->getTimeTag());
        parseSysStat(packet, outs->getDataPtr(), _CM3500Nvars, samp->getTimeTag());
        results.push_back(outs);
        return true;
    }

    //    WLOG(("%s: CMIGITS message id: %d - is not being processed", getName().c_str(), message_id));
    return false;
}

CMIGITS3_Serial::Scanner::Scanner(int bufsize):
    MessageStreamScanner(bufsize),_thisSampleLength(-1),
    _badLength(0),_badCheckSum(0)
{
}

Sample* CMIGITS3_Serial::Scanner::nextSampleSepBOM(DSMSensor* sensor)
{
    Sample* result = 0;

    const unsigned int wsize = sizeof(uint16_t);    // size of 2-byte word

    // size of header in 2-byte words
    const unsigned int hsizew = CMIGITS3_Serial::CM_HDR_SIZE;

    /*
     * scanner will be in one of these states:
     * 1. first call, no chars scanned, _osamp==NULL
     * 2. last call resulted in output sample.
     *    No chars have been read in this sample: _osamp==NULL
     * 3. We are scanning for the record separator: 0xff 0x81
     *    _separatorCnt < _separatorLen
     * 3. last call scanned an entire BOM separator and returned
     *    the sample previous to the separator. Therefore we're
     *    currently reading the portion after the separator. If
     *    getMessageLength() > 0, memcpy available characters, up to
     *    the message length, then start scanning for the next BOM.
     *    _osamp != NULL.
     * 4. last call returned 0, meaning there was a partial sample
     *      at end of the previous buffer. osamp then contains
     *      a partial sample. We may or may not be done scanning
     *      for the BOM separator.  _osamp != NULL.
     */

    if (!_osamp) {

        if (_buftail == _bufhead) return 0;

        // If we have characters, allocate next sample, and set its default timetag
        _osamp = getSample<char>(_sampleLengthAlloc);
        _osamp->setId(sensor->getId());
        _outSampDataPtr = (char*) _osamp->getVoidDataPtr();
        _outSampLengthAlloc = _osamp->getAllocByteLength();
        // set default timetag in case we never find a BOM
        _osamp->setTimeTag(_tfirstchar + _buftail * getUsecsPerByte());
        _outSampRead = 0;
        _separatorCnt = 0;
        _thisSampleLength = -1;
    }

    if (_separatorCnt == _separatorLen) {
        // BOM separator has been scanned.  If we haven't yet done so,
        // read the rest of the 10 byte header (5 two byte words).
        // The 3rd two-byte word is the number of 2-byte words in the data section.
        // The total record size in bytes is:
        //      10 (header) + data_len*2 + 2 (final checksum)
        // Header checksum is the 5th 2-byte word in the header.
        //

        if (_thisSampleLength < 0) {

            assert((hsizew * wsize) >= _outSampRead);

            int nc = std::min(_bufhead - _buftail,
                    (hsizew * wsize) - _outSampRead);
            if (nc > 0) {
                if (_outSampRead + nc > _outSampLengthAlloc &&
                        (result = requestBiggerSample(nc))) return result;
                ::memcpy(_outSampDataPtr+_outSampRead,_buffer+_buftail,nc);
                _outSampRead += nc;
                _buftail += nc;
            }
            if (_outSampRead == hsizew * wsize) {  // header has been read

                uint16_t cksum = CMIGITS3_Serial::checkSumRaw(_outSampDataPtr,hsizew-1);
                if (cksum == _cm2dsmcvtr->uint16Value(_outSampDataPtr + (hsizew-1) * wsize)) {

                    // read data word count in 3rd word
                    unsigned int l =
                        _cm2dsmcvtr->uint16Value(_outSampDataPtr + 2 * wsize);
                    if (l < 1024) {
                        if (l == 0)
                            // if data word count is 0, there is no data checksum
                            _thisSampleLength = hsizew * wsize;
                        else
                            _thisSampleLength = (hsizew + l + 1) * wsize;
                    }
                    else if (!(_badLength++ % 100))
                        WLOG(("%s: CMIGITS %u records with excessive length",
                                    sensor->getName().c_str(),_badLength));
                }
                else if (!(_badCheckSum++ % 100) && _badCheckSum > 1)
                    WLOG(("%s: CMIGITS %u records with bad header check sums",
                                sensor->getName().c_str(),_badCheckSum));
            }
            else return 0;  // header not completely read yet, no chars left in buffer

            if (_thisSampleLength < 0) {
                // bad header checksum or wacky data length. Send bad sample along
                _osamp->setTimeTag(_bomtt);
                _osamp->setDataLength(_outSampRead);
                addSampleToStats(_outSampRead);
                result = _osamp;
                _osamp = 0;
                return result;
            }
        }

        // at this point, _thisSampleLength > 0
        assert(_thisSampleLength > 0);
        assert((unsigned)_thisSampleLength >= _outSampRead);

        // How much left to read
        unsigned int nc = _thisSampleLength - _outSampRead;
        if (nc > 0) {
            nc = std::min(_bufhead-_buftail,nc);
            if (nc == 0) return 0;

            if (_outSampRead + nc > _outSampLengthAlloc &&
                    (result = requestBiggerSample(nc))) return result;
            ::memcpy(_outSampDataPtr+_outSampRead,_buffer+_buftail,nc);
            _outSampRead += nc;
            _buftail += nc;

            // all bytes consumed, but still not finished with this sample
            if (_outSampRead < (unsigned)_thisSampleLength) return 0;
        }

        // finished with this sample
        _osamp->setTimeTag(_bomtt);
        _osamp->setDataLength(_outSampRead);
        addSampleToStats(_outSampRead);
        result = _osamp;

        // adjust size of next sample to request, if it needs changing
        if (_outSampRead > _sampleLengthAlloc) {
            _sampleLengthAlloc = std::min(_outSampRead + 16,MAX_MESSAGE_STREAM_SAMPLE_SIZE);
        }
        _osamp = 0;
        return result;
    }

    // empty space needed in sample before processing next character
    int space = _separatorLen;

    // At this point we are scanning for the message separator
    // at the beginning of the next message.
    for (;_buftail < _bufhead;) {
        register char c = _buffer[_buftail];

        if (_outSampRead + space > _outSampLengthAlloc) {
            // We're gonna need a bigger boat
            if ((result = requestBiggerSample(space))) return result;
        }

        if (c == _separator[_separatorCnt]) {
            // We now have a character match to the record separator.
            // increment the separator counter.

            // the receipt time of the initial separator character
            // is the timetag for the sample. Save this time
            // in case the entire separator is not in this buffer.
            if (_separatorCnt == 0) _bomtt =
                _tfirstchar + _buftail * getUsecsPerByte();

            _buftail++;      //  consume character
            _outSampDataPtr[_outSampRead++] = c;

            if (++_separatorCnt == _separatorLen) {

                if (_outSampRead > (unsigned)_separatorLen) {
                    // bad junk at beginning of sample
                    // send sample containing the bad junk out
                    _outSampRead -= _separatorLen;
                    _osamp->setDataLength(_outSampRead);
                    _osamp->setTimeTag(_bomtt - _outSampRead * getUsecsPerByte());
                    addSampleToStats(_outSampRead);
                    result = _osamp;

                    // current sample
                    _osamp = getSample<char>(_sampleLengthAlloc);
                    _osamp->setId(sensor->getId());
                    _osamp->setTimeTag(_bomtt);
                    _outSampDataPtr = (char*) _osamp->getVoidDataPtr();
                    _outSampLengthAlloc = _osamp->getAllocByteLength();
                    _thisSampleLength = -1;
                    ::memcpy(_outSampDataPtr,_separator,_separatorCnt);
                    _outSampRead = _separatorCnt;
                    return result;
                }

                // Do a recursive call (or a goto to the beginning
                // of this function).  It won't be infinitely recursive,
                // even if the sensor was only sending out BOM strings, because
                // _separatorCnt is now equal to _separatorLen
                return nextSampleSepBOM(sensor);
            }
        }
        else {
            /* we're looking for the BOM separator, and
             * the current character fails a match with the BOM string
             */

            if (_separatorCnt > 0) {    
                /*
                 * previous partial match.  Since CMIGITS separator is 2 characters,
                 * _separatorCnt will be 1 here.
                 */

                // copy partial match to sample
                ::memcpy(_outSampDataPtr + _outSampRead,_separator,_separatorCnt);
                _outSampRead += _separatorCnt;

                // start scanning for BOM again, starting with this character.
                // This won't infinitely loop because _separatorCnt has been set to 0.
                _separatorCnt = 0;

            }
            else {              // no match to first character in separator
                _outSampDataPtr[_outSampRead++] = c;
                _buftail++;      // used character
            }
        }
    }

    return result;
}
