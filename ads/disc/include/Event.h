

#ifndef INCEvent_hh
#define INCEvent_hh

#include "DsmConfig.h"
#include "UdpSocket.h"
#include "Events.h"

class Event
  {
  public:
			Event(DsmConfig *dsm_config);
    void		checkEvents();
    struct EVTPKT	*getEvent();
    inline int		count()		{ return(cnt); }

  private:
    static const int	EVT_BUFFS = 16;

    UdpSocket		*Evt_sock;
    struct EVTPKT	evtBuff[EVT_BUFFS];
    int			cnt;

} ;

#endif
