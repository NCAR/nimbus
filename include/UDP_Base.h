/*
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
  BroadcastData(const std::string & timeStamp) = 0;

protected:
  virtual
  std::vector<var_base *> readFile(const std::string & fileName) const;

  nidas::util::DatagramSocket * _socket;

  /// Destination UDP.
  nidas::util::Inet4SocketAddress * _to;

  std::vector<var_base *> _varList;

  const int UDP_PORT;
};

#endif
