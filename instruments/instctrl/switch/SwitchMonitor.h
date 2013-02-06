#ifndef SWITCHMONITOR_H_
#define SWITCHMONITOR_H_

#include <QtCore>
#include "RicLogger.h"

/// Monitor the switch behavior, and send periodic reports to the system logger.
/// The reporting starts as soon as the class is instantiated.
class SwitchMonitor: public QObject {
	Q_OBJECT
	public:
		/// Constructor.
		/// @param reportRateSeconds
		/// @param fromSwitch Reference to a variable containing a total count of
		///    messages that were received from the remote switch.
		/// @param toProxies Reference to a variable containing a total count of
		///    messages that were sent to proxies.
		/// @param fromProxies Reference to a variable containing a total count of
		///    messages that were received from proxies.
		/// @param toSwitch Reference to a variable containing a total count of
		///    messages that were sent to the remote switch.
		SwitchMonitor(int reportRateSeconds,
				const int &fromSwitch,
				const int &toProxies,
				const int &fromProxies,
				const int &toSwitch);
		/// Destructor.
		virtual ~SwitchMonitor();

	protected slots:
		/// Produce a report.
		void reportSlot();

	protected:
		/// The system log reporting rate, in seconds.
		int _reportRateSeconds;
		/// Reference to a variable containing a total count of
		/// messages that were received from the remote switch.
		const int& _fromSwitch;
		/// Reference to a variable containing a total count of
		/// messages that were sent to the remote switch.
		const int& _toSwitch;
		/// Reference to a variable containing a total count of
		/// messages that were received from proxies.
		const int& _fromProxies;
		/// Reference to a variable containing a total count of
		/// messages that were sent to proxies.
		int _toProxies;
		/// Last count of messages that were received from proxies.
		int _lastFromProxies;
		/// Last count of messages that were sent to the remote switch.
		int _lastToSwitch;
		/// Last count of messages that were received from the remote switch.
		int _lastFromSwitch;
		/// Last count of messages that were sent to proxies.
		int _lastToProxies;
		/// System logger.
		RicLogger _logger;
		/// The report timer.
		QTimer _reportTimer;
};

#endif /* SWITCHMONITOR_H_ */
