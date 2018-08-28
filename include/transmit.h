/*
-------------------------------------------------------------------------
OBJECT NAME:	transmit.h

DESCRIPTION:	Class to compress and transmit SQL statements to the
		ground.
-------------------------------------------------------------------------
*/

#ifndef _transmit_h_
#define _transmit_h_

#include <string>

/**
 * Class for queueing and sending strings to an ldm via pqinsert.  Queue
 * interval can be set to a number of strings to queue before sending.
 */
class sqlTransmit
{
public:
  sqlTransmit(const std::string ac);
  ~sqlTransmit();

  /**
   * Transmit string.
   */
  void
  sendString(const std::string& str);

  /**
   * Enqueue a string for transmission.  Strings are blocked up until
   * _timeInterval strings have been submitted.
   */
  void
  queueString(const std::string& str);

  /**
   * Set the frequency of which to accumulate and then transmit data to
   * the ground/LDM.  Default is 5 seconds.
   */
  void
  setTimeInterval(size_t seconds) { _timeInterval = seconds; }

  size_t
  timeInterval() const { return _timeInterval; }

  bool
  oneCommandLeft() const { return (_timeInterval - _counter) == 0; }

protected:
  std::string _q;

  /**
   * How often, in seconds, to Q up the data, then send it to the LDM.
   */
  size_t _timeInterval;

  /**
   * Count of # of seconds of data accumulated so far.
   * @see _timeInterval.
   */
  size_t _counter;

  /**
   * Total running count of transmitted strings.  Used to build the
   * file name sequence counter.
   */
  size_t _packetCounter;

  /**
   * Aircraft identifier.  Will be first line in all files transmitted.
   */
  std::string _aircraft;
};

#endif

// END TRANSMIT.H
