/*
-------------------------------------------------------------------------
OBJECT NAME:	UDP_Base.cc

FULL NAME:	UDP base class for broadcast

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "UDP_Base.h"

#include "nimbus.h"
#include "parseInt.cc"
#include <algorithm>

#include <nidas/util/UTime.h>

/* -------------------------------------------------------------------- */
UDP_Base::
UDP_Base(int port) : 
  _default_latch_seconds(0),
  UDP_PORT(port)
{
}


/* -------------------------------------------------------------------- */
bool
UDP_Base::
readFile(const std::string & fileName)
{
  char	*varList[512];
  std::vector<var_base *> list;

  if (!ReadTextFile(fileName, varList))
    return false;

  for (int i = 0; varList[i]; ++i)
  {
    int ifield = 0;
    char target[BUFSIZ], *p;
    strcpy(target, varList[i]);

    p = strtok(target, ", \t\n");
    while (p)
    {
      var_base* vp;
      int index;
      int v;

      // ifield just guards against a latch time argument appearing at the
      // start of a line, when really we want a line to start with a
      // variable.
      ++ifield;
      if (ifield == 2 && parseInt(p, v))
      {
	// Take this as a latch argument for the previous field.
	*_latch_seconds.rbegin() = v;
	std::ostringstream msg;
	msg << "latch time for slot " << _latch_seconds.size()
	    << " set to " << v << " seconds from line: " << varList[i] << "\n";
        LogMessage(msg.str().c_str());
      }
      else if (((index = SearchTable(raw, p)) != ERR && (vp = raw[index])) ||
	       ((index = SearchTable(derived, p)) != ERR && (vp = derived[index])))
      {
        list.push_back(vp);
	_latch_seconds.push_back(_default_latch_seconds);
	_lastGoodData.push_back(std::vector<double>(vp->Length, nan("")));
	_lastGoodTime.push_back(0);
	std::ostringstream msg;
	msg << "Added variable " << vp->name << " to IWG1 feed in slot " << list.size() << ".\n";
        LogMessage(msg.str().c_str());
      }
      else
      {
        list.push_back(0);
	_latch_seconds.push_back(_default_latch_seconds);
	_lastGoodData.push_back(std::vector<double>(1, nan("")));
	_lastGoodTime.push_back(0);
	std::ostringstream msg;
        msg << "UDP_Base: Variable [" << p << "] not found.  Slot " << list.size() << " will be missing.\n";
        LogMessage(msg.str().c_str());
      }
      p = strtok(NULL, ", \t\n");
    }
  }

  FreeTextFile(varList);
  _varList = list;
  return true;
}


std::string
UDP_Base::
formatTimestamp(nidas::core::dsm_time_t tt)
{
  nidas::util::UTime ut(tt);
  return ut.format(true, "%Y%m%dT%H%M%S");
}


void
UDP_Base::
updateData(nidas::core::dsm_time_t tt)
{
  extern NR_TYPE * AveragedData;

  for (int i = 0; i < (int)_varList.size(); ++i)
  {
    var_base* vp = _varList[i];
    if (vp)
    {
      // If a value is good, update the current value.  If missing, do not
      // replace the current value unless it is too old.  To check if a bin
      // sample is missing, it's enough to check if any of the bins are
      // missing, but check the last one to avoid any ambiguity from the
      // zero'th bin.
      const NR_TYPE& current = AveragedData[vp->LRstart];
      const NR_TYPE& last = AveragedData[vp->LRstart + vp->Length - 1];
      if (!isnan(last))
      {
	std::copy(&current, &last + 1, _lastGoodData[i].begin());
	_lastGoodTime[i] = tt;
      }
      else if (_lastGoodTime[i] + (_latch_seconds[i]*1e6) < tt)
      {
	std::copy(&current, &last + 1, _lastGoodData[i].begin());
      }
    }
  }
}


std::string
UDP_Base::
formatVariable(int i)
{
  std::ostringstream text;
  var_base* vp = _varList[i];
  if (vp && vp->Length > 1)
  {
    std::vector<double>::iterator iv;
    text << "'{";
    bool first = true;
    for (iv = _lastGoodData[i].begin();
	 iv != _lastGoodData[i].end(); ++iv, first = false)
    {
      if (!first)
	text << ",";
      if (isnan(*iv))
	text << -32767;
      else
	text << *iv;
    }
    text << "}'";
  }
  else
  {
    if (isnan(_lastGoodData[i][0]))
      text << -32767;
    else
      text << _lastGoodData[i][0];
  }
  return text.str();
}
