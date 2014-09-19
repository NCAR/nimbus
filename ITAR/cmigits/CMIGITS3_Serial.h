// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4; -*-
// vim: set shiftwidth=4 softtabstop=4 expandtab:
/*
    Copyright 2005 UCAR, NCAR, All Rights Reserved

    $LastChangedDate$

    $LastChangedRevision$

    $LastChangedBy$

    $HeadURL$

*/

#ifndef NIDIS_DYNLD_CMIGITS_SERIAL_H
#define NIDIS_DYNLD_CMIGITS_SERIAL_H

#include <nidas/dynld/DSMSerialSensor.h>
#include <nidas/core/DerivedDataClient.h>
#include <nidas/core/SampleScanner.h>
#include <nidas/util/EndianConverter.h>

#include <nidas/util/InvalidParameterException.h>


namespace nidas { namespace dynld { namespace raf {

using namespace nidas::core;

/**
 * A class for reading CMIGITSIII records from an instrument attached to
 * a serial port.
 */
class CMIGITS3_Serial : public DSMSerialSensor, public DerivedDataClient
{

public:
    CMIGITS3_Serial();
    ~CMIGITS3_Serial();

    enum MsgStatus { NAK, HANDSHAKE_REJECT, HANDSHAKE_OK, NOT_HANDSHAKE };

    /**
     * open the sensor and perform any initialization to the driver.
     */
    void open(int flags) throw(nidas::util::IOException);

    void close() throw(nidas::util::IOException);

    void fromDOMElement(const xercesc::DOMElement* node)
        throw(nidas::util::InvalidParameterException);

    virtual void
    derivedDataNotify(const nidas::core:: DerivedDataReader * s)
        throw();

    void addSampleTag(SampleTag* stag)
        throw(nidas::util::InvalidParameterException);

    bool process(const Sample* samp,std::list<const Sample*>& results)
    	throw();

    /**
     * Install the custom sample scanner for CMIGITS.
     */
    SampleScanner* buildSampleScanner() throw(nidas::util::InvalidParameterException);

    /**
     *  A SampleScanner for CMIGITS3, which verifies a good header checksum
     *  and parses the data length word from the header. The number of bytes
     *  specified in the data length is then read, along with the trailing
     *  checksum, before again looking for the 
     */
    class Scanner: public MessageStreamScanner
    {
    public:
        Scanner(int bufsize=8192);
        Sample* nextSampleSepBOM(DSMSensor* sensor);
    private:
        /**
         * Length of sample being currently scanned.
         * Parsed from 3rd 2-byte word in each sample.
         */
        int _thisSampleLength;

        unsigned int _badLength;
        unsigned int _badCheckSum;
    };

    /**
     * CMIGITS Message, from SDN500 documentation.
     */
    class Message {
    public:
        Message(uint16_t messageID, uint16_t dataWordCount);

        /**
         * Write a 16 bit value to word[index] in the message, where word[]
         * are 16 bit integers.
         * index 0 points to the 0x81FF sync word at the start of the header.
         * index (CMIGITS3_Serial::CM_HDR_SIZE + _dwc) points to the data checksum
         * The values for index use C convention, one less than the "Word No"
         * in the SDN500 documentation.
         */
        void setWord(uint16_t,unsigned int index);

        /**
         * Write a 32 bit value to word[index] and word[index+1] in the message.
         * Note that index is the index of the first 16-bit word where the data is
         * written. As with setWord(), these C indices are one less than the
         * "Word No" in, for example, the SDN500 documentation for Message 3511.
         */
        void setWord32(int32_t,unsigned int index);

        /**
         * Update the checksums in the message. This is automatically done in
         * toHexString() and getPtr(), so the user doesn't typically have to call
         * doCheckSums().
         */
        void doCheckSums();

        /**
         * Length in bytes of the message. If the data word count is 0,
         * as with Message 0, Universal Reset, a data checksum is not computed,
         * and this length does not include a data checksum.
         */
        unsigned int getTotalByteLength() const
        { 
            if (_dwc > 0)
                return (CMIGITS3_Serial::CM_HDR_SIZE + _dwc + 1) * sizeof(uint16_t);
            return CMIGITS3_Serial::CM_HDR_SIZE * sizeof(uint16_t);
        }

        /**
         * Convert message to a hex string for debugging. Checksums are
         * automatically updated before converting.
         */
        string toHexString();

        /**
         * Return a pointer to the beginning of the message. Checksums
         * are automatically updated.
         */
        const unsigned char* getPtr();

    private:

        /** Data word count of the packet, from the SDN500 doc */
        uint16_t _dwc;

        /** Contents */
        vector<uint16_t> _msg;

        /** Have the checksums been updated */
        bool _cksummed;
    };

private:

//@{
    void parseNavSoln(const unsigned short* input, float *dout, int nvars, dsm_time_t tt)
    	throw();

    void parseDeltaVandTheta( const unsigned short* input, float *dout, int nvars, dsm_time_t tt) 
        throw ();
  
    void parseSysStat(const unsigned short* input, float *dout, int nvars, dsm_time_t tt) 
        throw ();
//@}


//@{
    /// Place CMIGITS into init mode.
    void setInitMode() throw(nidas::util::IOException);
    /// Command CMIGITS to revert to default messages.
    void setDisconnect() throw(nidas::util::IOException);
    /// Engage the 3502 message.  Which does not seem to work on the GUSTPOD CMIGITS
    /// Ack and handshake are successful, but no messages come out...
    void sendConfigForAccel() throw (nidas::util::IOException);

    /// Change orientation for installations where it is not mounted
    /// in default mode.
    void sendOrientationLAMS() throw (nidas::util::IOException);

    // Send lever arm data.
    void sendLeverArm() throw (nidas::util::IOException);


    /** 
     * Send the CMIGITS 3510 packet with position and heading data to the
     * CMIGITS and switch to NAV mode.
     * @param gpsd is the ground speed value to send.
     * @param thdg is the true heading value to send.
     */
    void sendControlInit3510(double gspd, double thdg, double lat, double lon, double alt)
                    throw(nidas::util::IOException);

    /**
     * Read samples until either a good or rejected handshake sample has arrived,
     * or a timeout, or many samples were received, but no handshake.
     */
    bool readAckHandshake(uint16_t id) throw(nidas::util::IOException);

    /**
     * Parse a message for NAK and handshake status.
     */
    enum MsgStatus parseAckHandshake(const uint16_t* msg);

    /// Pack Float for CMIGITS.  @see sendOrientation
    int32_t packFloat32(double value, int binaryScaling);
//@}

    /**
     * Compute checksum on array that has been converted to
     * host endianness.
     */
    static uint16_t checkSum(const uint16_t data[], int nWords);

    /**
     * Compute checksum on raw data, not converted to host
     * endianness.
     */
    static uint16_t checkSumRaw(const char* data, int nWords);

    /**
     * Same as checkSumRaw(const char*,int), but with pointer to uint16_6.
     */
    static uint16_t checkSumRaw(const uint16_t* data, int nWords);

    /**
     * Number of variables requested from the 3501, and 3500 records
     */
    int _CM3500Nvars,_CM3501Nvars, _CM3502Nvars;

    /**
     * Full sample id of CM3500, CM3501 and CM3502 variables.
     */
    dsm_sample_id_t CM3501Id, CM3502Id, CM3500Id;


    int _misalignedCounter;

    /**
     * Some constants used for decoding the CMIGITS samples
     */
    static const char SyncBytes[2];
    static const uint16_t   ReadyFlag;
    static const unsigned int   CM_HDR_SIZE;
    static const unsigned int    CM_3501_SIZE;
    static const unsigned int    CM_3502_SIZE;
    static const unsigned int    CM_3500_SIZE;
    static const double     cm31;
    static const double     cm21;
    static const double     cm16;

    static const nidas::util::EndianConverter* _cm2dsmcvtr;
    static const nidas::util::EndianConverter* _dsm2cmcvtr;

    unsigned int _noTrueHeadingCnt;
    unsigned int _noSpeedCnt;

    /// Parameter from XML file to associate this CMIGITS with a larger instrument.
    /// e.g. LAMS or GUSTPOD.
    std::string _instrument;

    /// Lever arm values, X, Y, Z distance in centimeters from CMIGITS to antenna
    int16_t _lever_arm_X;
    int16_t _lever_arm_Y;
    int16_t _lever_arm_Z;

    /**
     * Parameter from XML file specifying whether CMIGITS is getting a GPS antenna feed.
     * Defaults to true.
     */
    bool _gps;

    int _delayCntr;

    int _sendDelay;
};

}}}	// namespace nidas namespace dynld raf

#endif
