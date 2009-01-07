/*
-------------------------------------------------------------------------
OBJECT NAME:	grnd_feed.h

FULL NAME:	Ground Feed

DESCRIPTION:	Transmit data to ground host via UDP.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#ifndef _GroundFeed_h_
#define	_GroundFeed_h_

#include "UDP_Base.h"


/* -------------------------------------------------------------------- */
class GroundFeed : public UDP_Base
{
public:
  GroundFeed(int rate);

  void	BroadcastData(const std::string & timeStamp);

protected:
  /**
   * Determine coordinates attribute for real-time database.  Use ground
   * feed list, so that valid variables in ground database will be chosen.
   */
  void setCoordinatesFrom(const std::vector<var_base *> & list) const;

  /// Destination host address.
  static const std::string DEST_HOST_ADDR;

  /**
   * Send data to ground this often, in seconds.
   */
  const size_t _dataRate;

  /**
   * Sum the AveragedData in preparation for sending average over GroundFeedDataRate
   */
  std::vector<NR_TYPE> _summedData;
  std::vector<int> _summedDataCount;		// for averaging
  std::vector<NR_TYPE> _lastGoodData;   	// expunge occasional NaNs in data
  std::vector<int> _lastGoodDataIncrement;	// if NaN for a long time send NaN
};

#endif
