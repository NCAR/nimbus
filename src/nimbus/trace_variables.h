// -*- C++ -*-

#include <nidas/util/Logger.h>
#include <nidas/util/UTime.h>
#include "sync_reader.hh"
#include "timeseg.h"

#include <cmath>

template <typename T, typename V>
inline
void
stream_variable(T& out, V* values, unsigned int nvalues)
{
  bool zeros = true;
  for (unsigned int i = 0; i < nvalues; ++i)
  {
    if (values[i] == 0)
    {
      out << ".";
      zeros = true;
    }
    else if (std::isnan(values[i]))
    {
      out << "x";
    }
    else if (zeros)
    {
      out << values[i];
      zeros = false;
    }
    else
    {
      out << ",";
      out << values[i];
    }
  }
}


/**
 * Use the nidas logging API to select variables for tracing and print
 * their values at various stages.  The variables to be traced are listed
 * in the 'trace_variables' log scheme parameter.  To activate tracing, the
 * variables must be specified and the log point must be enabled, such as
 * with config string 'level=verbose,tag=trace_variables'.  The RAWTBL must
 * already be initialized when the instance is created, since the
 * constructor immediately looks up the variable names in the table.
 **/
class TraceVariables
{
public:
  TraceVariables();

  inline
  RAWTBL*
  lookupRawVariable(const std::string& vname)
  {
    for (size_t i = 0; i < raw.size(); ++i)
    {
      if (vname == raw[i]->name)
	return raw[i];
    }
    return 0;
  }

  inline
  const nidas::dynld::raf::SyncRecordVariable*
  lookupSyncVariable(const std::string& vname)
  {
    nidas::dynld::raf::SyncRecordReader* reader = GetSyncReader();
    if (reader == nullptr)
      return nullptr;
    const nidas::dynld::raf::SyncRecordVariable* variable =
      lookup_variable(reader->getVariables(), vname);
    return variable;
  }

  inline std::string
  format_time(time_t ttime)
  {
    static std::string tfmt("%Y/%m/%d;%H:%M:%S");
    return nidas::util::UTime(ttime).format(tfmt);
  }

  /**
   * Return true if this trace is active, meaning there are variables to
   * trace and the log point is enabled.
   **/
  inline bool
  active()
  {
    return _context.active() && _nvars;
  }

  inline
  void
  trace(const std::string& flag, double* rec, bool lowrate=false)
  {
    if (!active())
    {
      return;
    }

    // For sample rate buffers we can take the time directly from the
    // buffer, otherwise we have to use the global SampledData pointer
    // directly to get the time of the current record.
    time_t thisTime = 0;
    if (lowrate)
    {
      extern NR_TYPE* SampledData;
      thisTime = SampledDataTimeToSeconds(SampledData);
    }
    else
    {
      thisTime = SampledDataTimeToSeconds(rec);
    }

    std::vector<const RAWTBL*>::iterator it;
    for (it = _variables.begin(); it != _variables.end(); ++it)
    {
      const RAWTBL* vp = *it;
      double* values = rec+vp->SRstart;
      if (lowrate)
	values = rec+vp->LRstart;
      log_variable(flag, vp->name, thisTime, values, vp->Length);
    }
  }

  /**
   * Public entry point to log a single variable, after first testing that
   * the variable is being trace.
   **/
  template <typename T>
  inline void
  trace_variable(const std::string& flag, const std::string& vname,
		 time_t ttime, T* values, int nvalues)
  {
    if (active() &&
	std::find(_vnames.begin(), _vnames.end(), vname) != _vnames.end())
    {
      log_variable(flag, vname, ttime, values, nvalues);
    }
  }

  inline void
  trace_sync_record(const std::string& flag, nidas::core::dsm_time_t tt,
		    double* record, int nvalues)
  {
    if (!active())
    {
      return;
    }
    std::vector<const nidas::dynld::raf::SyncRecordVariable*>::iterator it;
    for (it = _syncvars.begin(); it != _syncvars.end(); ++it)
    {
      const nidas::dynld::raf::SyncRecordVariable* var = *it;
      size_t varoffset = var->getSyncRecOffset();
      int irate = (int)ceil(var->getSampleRate());
      int vlen = var->getLength();

      log_variable(flag, var->getName(), tt / USECS_PER_SEC,
		   &(record[varoffset]), vlen*irate);
    }
  }

private:

  template <typename T>
  inline void
  log_variable(const std::string& flag, const std::string& vname,
	       time_t ttime, T* values, int nvalues)
  {
    _msg << "@" << format_time(ttime) << ", ";
    _msg << flag << ": " << vname << "=";
    stream_variable(_msg, values, nvalues);
    _msg << nidas::util::endlog;
  }


  nidas::util::LogContext _context;
  nidas::util::LogMessage _msg;
  std::vector<const RAWTBL*> _variables;
  std::vector<const nidas::dynld::raf::SyncRecordVariable*> _syncvars;
  std::vector<std::string> _vnames;
  int _nvars;
};


inline
TraceVariables::
TraceVariables() :
  _context(LOG_VERBOSE, "trace_variables"),
  _msg(&_context),
  _nvars(0)
{
  nidas::util::Logger* logger = nidas::util::Logger::getInstance();
  nidas::util::LogScheme scheme = logger->getScheme();
  std::string value = scheme.getParameter("trace_variables");

//  if (value.empty())
//    return;

  std::string::size_type at = 0;
  do {
    std::string::size_type comma = value.find(',', at);
    if (comma == std::string::npos)
      comma = value.length();
    std::string vname = value.substr(at, comma-at);
    RAWTBL* vp = lookupRawVariable(vname);
    if (vp)
    {
      _variables.push_back(vp);
    }
    const nidas::dynld::raf::SyncRecordVariable* sv = lookupSyncVariable(vname);
    if (sv)
    {
      _syncvars.push_back(sv);
    }
    if (vp || sv)
    {
      _vnames.push_back(vname);
    }
    at = comma+1;
  }
  while (at < value.length());
  _nvars = _vnames.size();
}
