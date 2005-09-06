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

class sqlTransmit
{
public:
  sqlTransmit();
  ~sqlTransmit();

  /**
   * Enqueue a string for transmission.  Strings are blocked up until
   * _timeInterval strings have been submitted.
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

  size_t _packetCounter;
};

#endif

// END TRANSMIT.H
