#include "SwitchMonitor.h"


/////////////////////////////////////////////////////////////////////
SwitchMonitor::SwitchMonitor(
		int reportRateSeconds,
		const int &fromSwitchCount,
		const int &toSwitchCount,
		const int &fromProxyCount,
		const int &toProxyCount):
_reportRateSeconds(reportRateSeconds),
_fromSwitchCount(fromSwitchCount),
_toSwitchCount(toSwitchCount),
_fromProxyCount(fromProxyCount),
_toProxyCount(toProxyCount),
_lastFromSwitchCount(0),
_lastToSwitchCount(0),
_lastFromProxyCount(0),
_lastToProxyCount(0)

{
	// Don't allow nonsense reporting rates
	if (_reportRateSeconds <= 0) {
		_reportRateSeconds = 1;
	}

	// Configure timer.
	_reportTimer.setInterval(1000*_reportRateSeconds);
	connect(&_reportTimer, SIGNAL(timeout()), this, SLOT(reportSlot()));

	// Start timer.
	_reportTimer.start();

}

/////////////////////////////////////////////////////////////////////
SwitchMonitor::~SwitchMonitor() {

}

/////////////////////////////////////////////////////////////////////
void SwitchMonitor::reportSlot() {

	int fromSwitchDelta = _fromSwitchCount - _lastFromSwitchCount;
	int toSwitchDelta   = _toSwitchCount   - _lastToSwitchCount;
	int fromProxyDelta  = _fromProxyCount  - _lastFromProxyCount;
	int toProxyDelta    = _toProxyCount    - _lastToProxyCount;

	_lastFromSwitchCount = _fromSwitchCount;
	_lastToSwitchCount   = _toSwitchCount;
	_lastFromProxyCount  = _fromProxyCount;
	_lastToProxyCount    = _toProxyCount;

	// Create the report.
	QString msg = QString("Rates from switch:%1 to switch:%2  from proxies:%3 to proxies:%4")
			.arg(fromSwitchDelta)
			.arg(toSwitchDelta)
			.arg(fromProxyDelta)
			.arg(toProxyDelta);

	// Log it.
	_logger.log(msg.toStdString());
}
