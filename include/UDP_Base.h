/* -*- C++ -*-
-------------------------------------------------------------------------
OBJECT NAME:	UDP_Base.h

FULL NAME:	UDP Broadcast output.

DESCRIPTION:	Base class for UDP broadcast.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#ifndef _UDP_Base_h_
#define	_UDP_Base_h_

#include <string>
#include <vector>

#include <nidas/util/Socket.h>
#include <nidas/core/Sample.h>

class var_base;

/* -------------------------------------------------------------------- */
class UDP_Base
{
public:
  UDP_Base(int port);

  virtual
  ~UDP_Base() { }

  /** 
   * Generate broadcast string and send.
   */
  virtual void
  BroadcastData(nidas::core::dsm_time_t tt) = 0;

  std::string
  formatTimestamp(nidas::core::dsm_time_t tt);

  /**
   * Pull data for each of the variables in the _varList, updating the values that
   * should be output according to whether the variable is a missing value and it's
   * latch setting.
   **/
  void
  updateData(nidas::core::dsm_time_t tt);

  /**
   * Format the current value for variable i into a string suitable for insertion into the
   * UDP message.  For IWG1 this means blanks for missing values, and conversion of altitude units.
   * The ground feed must account for vector variables.
   **/
  virtual
  std::string
  formatVariable(int i);

protected:
  virtual
  bool
  readFile(const std::string & fileName);

  nidas::util::MulticastSocket * _socket;

  /// Destination UDP.
  nidas::util::Inet4SocketAddress * _to;

  std::vector<var_base *> _varList;
  std::vector<int> _latch_seconds;

  /**
   * Deal with the occasional existance of NaNs in the data that is about to be sent.
   */
  std::vector< std::vector<double> > _lastGoodData;
  std::vector<nidas::core::dsm_time_t> _lastGoodTime;

  int _default_latch_seconds;

  const int UDP_PORT;
};

#endif
