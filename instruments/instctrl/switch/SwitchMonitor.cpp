#include "SwitchMonitor.h"


/////////////////////////////////////////////////////////////////////
SwitchMonitor::SwitchMonitor(
		int reportRateSeconds,
		const int &fromProxies,
		const int &toSwitch,
		const int &fromSwitch,
		const int &toProxies):
_reportRateSeconds(reportRateSeconds),
_fromProxies(fromProxies),
_toSwitch(toSwitch),
_fromSwitch(fromSwitch),
_toProxies(toProxies),
_lastFromSwitch(0),
_lastToSwitch(0),
_lastFromProxies(0),
_lastToProxies(0)

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

	int fromSwitchDelta = _fromSwitch - _lastFromSwitch;
	int toSwitchDelta   = _toSwitch   - _lastToSwitch;
	int fromProxyDelta  = _fromProxies  - _lastFromProxies;
	int toProxyDelta    = _toProxies    - _lastToProxies;

	_lastFromSwitch  = _fromSwitch;
	_lastToSwitch    = _toSwitch;
	_lastFromProxies = _fromProxies;
	_lastToProxies   = _toProxies;

	// Create the report.
	QString msg = QString("Msgs from switch:%1 to proxies:%2, from proxies:%3 to switch:%4")
			.arg(fromSwitchDelta)
			.arg(toProxyDelta)
			.arg(fromProxyDelta)
			.arg(toSwitchDelta);

	// Log it.
	_logger.log(msg.toStdString());
}