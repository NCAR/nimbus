/*
-------------------------------------------------------------------------
OBJECT NAME:	vdb2ncml.cc

FULL NAME:	Convert VarDB to NcML.

DESCRIPTION:	Converts VarDB, Categories, StandardNames, DerivedNames,
		DependTable, ModVars and AircraftSpecs to NcML.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include "lib/VarDBConverter.hh"

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;


void
usage()
{
  cerr << "Usage: vdb2xml <VarDB_path> [output xml path]\n";
  cerr << "Default output path: <VarDB_dir>/vardb.xml\n";
  cerr <<
    "PROJ_DIR must be set to locate the ModVars and DerivedNames\n"
    "files under PROJ_DIR/Configuration/raf.  The VarDB file typically\n"
    "resides in a project directory like PROJ_DIR/<project>/<platform>,\n"
    "so the project directory is derived from <VarDB_path>.\n"
    "Here are the required input files:\n"
    "  PROJ_DIR/Configuration/raf/ModVars\n"
    "  PROJ_DIR/Configuration/raf/DerivedNames\n"
    "  VarDB_dir/DependTable\n"
    "  VarDB_dir/StandardNames\n"
    "  VarDB_dir/Categories\n"
    "If VarDB_path is already in XML format, then no other files are\n"
    "needed to open that file.  The file will just be opened and then\n"
    "saved to the output path without any conversion.\n";
}


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  if (argc < 2 || argc > 3)
  {
    usage();
    return 1;
  }
  string source(argv[1]);
  VDBFile vdb;
  VarDBConverter vdbc;
  vdbc.open(&vdb, source);
  if (vdb.is_valid())
  {
    string vdbpath = vdbc.defaultOutputPath();
    if (argc > 2)
    {
      vdbpath = argv[2];
    }
    if (vdbpath == source)
    {
      cerr << "Source and output are the same path, not overwriting." << endl;
    }
    else
    {
      cout << "Converted " << source << ", writing to " << vdbpath << "\n";
      vdb.save(vdbpath);
    }
    return 0;
  }
  return 1;
}	/* END MAIN */

/* END VDB2NCML.CC */
