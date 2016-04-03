// -*- C++ -*-

#ifndef _sync_reader_h_
#define _sync_reader_h_

#include <set>
#include <string>
#include <vector>
#include <nidas/dynld/raf/SyncRecordReader.h>
#include <nidas/core/Version.h>
#include <nidas/dynld/raf/Aircraft.h>
#include <list>
#include <algorithm>

class RAWTBL;

void
CheckSyncServer();

void
ShutdownSyncServer();

void
CloseSyncReader();

void
SetSyncXMLPath(const std::string& path);

void
SyncReaderSetTimeWindow(time_t begin, time_t end);

nidas::dynld::raf::SyncRecordReader* 
StartSyncReader(const std::set<std::string>& headerfiles, bool postprocess);

nidas::dynld::raf::SyncRecordReader* 
GetSyncReader();

nidas::dynld::raf::SyncRecordReader* 
StartReadingSyncRecords();

void
LoadCalibration(nidas::core::Variable* var, time_t startTime,
		std::vector<double>& coefs);

/**
 * Translate the RAWTBL variable to the NIDAS Project variable and replace
 * the current VariableConverter with the new calibration coefficients.
 **/
void
SetCalibration(RAWTBL* rp);

typedef std::list<const nidas::core::Variable*> varlist_t;

std::list<const nidas::core::Variable*>
selectVariablesFromProject(nidas::core::Project* project);


std::string
getSerialNumber(const nidas::core::Variable* variable);


template <typename T>
struct match_variable_name
{
  std::string _name;

  match_variable_name(const std::string& name)
  {
    _name = name;
  }

  bool
  operator()(const T* var)
  {
    return _name == var->getName();
  }
};


template <typename T>
T*
lookup_variable(const std::list<const T*>& vars, const std::string& name)
{
  typename std::list<const T*>::const_iterator vi;
  vi = std::find_if(vars.begin(), vars.end(), match_variable_name<T>(name));
  T* var = 0;
  if (vi != vars.end())
  {
    var = const_cast<T*>(*vi);
  }
  return var;
}


/**
 * This is only meant for testing.  In real-time, the dsm_server feeds the
 * sync record samples.  In post-processing, nimbus instantiates a
 * SyncServer within the its own process.
 **/
void
StartSyncServerProcess(const std::set<std::string>& headerfiles,
		       const std::string& xml_path);



#endif // _sync_reader_h_
