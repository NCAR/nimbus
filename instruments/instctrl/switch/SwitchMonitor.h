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
		/// @param fromSwitchCount Reference to a variable containing a total count of
		///    messages that were received from the remote switch.
		/// @param toSwitchCount Reference to a variable containing a total count of
		///    messages that were sent to the remote switch.
		/// @param fromProxyCount Reference to a variable containing a total count of
		///    messages that were received from proxies.
		/// @param toProxyCount Reference to a variable containing a total count of
		///    messages that were sent to proxies.
		SwitchMonitor(int reportRateSeconds,
				const int &fromSwitchCount,
				const int &toSwitchCount,
				const int &fromProxyCount,
				const int &toProxyCount);
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
		const int& _fromSwitchCount;
		/// Reference to a variable containing a total count of
		/// messages that were sent to the remote switch.
		const int& _toSwitchCount;
		/// Reference to a variable containing a total count of
		/// messages that were received from proxies.
		const int& _fromProxyCount;
		/// Reference to a variable containing a total count of
		/// messages that were sent to proxies.
		int _toProxyCount;
		/// Last count of messages that were received from the remote switch.
		int _lastFromSwitchCount;
		/// Last count of messages that were sent to the remote switch.
		int _lastToSwitchCount;
		/// Last count of
		/// messages that were received from proxies.
		int _lastFromProxyCount;
		/// Last count of messages that were sent to proxies.
		int _lastToProxyCount;
		/// System logger.
		RicLogger _logger;
		/// The report timer.
		QTimer _reportTimer;
};

#endif /* SWITCHMONITOR_H_ */
