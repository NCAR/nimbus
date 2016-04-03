/**
 * Interface to access older binary VarDB files using the newer C++ interface.
 **/


#ifndef _vardb_VDBConverter_hh_
#define _vardb_VDBConverter_hh_

#include "vardb.hh"

/**
 * Open a VarDB file in binary format or XML, transparently converting from
 * VarDB to XML if necessary.
 **/
class VarDBConverter
{
public:

  VDBFile*
  open(VDBFile* vdb, const std::string& path);

  std::string
  defaultOutputPath();

  std::string
  projDirPath(const std::string& filename);

  std::string
  defaultProjDirPath(const std::string& filename);
  
  void
  setupProjDir(const std::string& vardbarg);

  void
  checkDependencies(VDBFile& vdb);

  void
  checkDerivedNames(VDBFile& vdb);

  void
  checkModVars(VDBFile& vdb);

private:

  std::string defaultProjDir;
  std::string projDir;



};





#endif // _vardb_VDBConverter_hh_


