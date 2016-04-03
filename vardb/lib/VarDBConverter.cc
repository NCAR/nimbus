

#include "VarDBConverter.hh"
#include <cstdlib>
#include <cstring>
#include <raf/portable.h>  // for ntohf()
#include <fstream>
#include <sstream>
#include <arpa/inet.h>     // for ntohl()

#include "vardb.h"

using std::string;
using std::ostringstream;
using std::cerr;
using std::endl;

static const char COMMENT = '#';
static const float fill_value = -32767.0;

extern long	VarDB_nRecords;



std::string
VarDBConverter::
defaultOutputPath()
{
  return projDirPath("vardb.xml");
}


void
VarDBConverter::
setupProjDir(const std::string& vardbarg)
{
  string dbdir(vardbarg);
  
  if (dbdir.find('/') != string::npos)
  {
    dbdir.erase(dbdir.rfind('/')+1, dbdir.length());
    projDir = dbdir;
  }
  // Initialize the global project configuration directory also.
  defaultProjDirPath("");
}


string
VarDBConverter::
defaultProjDirPath(const std::string& filename)
{
  if (defaultProjDir.empty())
  {
    const char* projdir = getenv("PROJ_DIR");
    if (projdir)
    {
      defaultProjDir = string(projdir) + "/Configuration/raf/";
    }
  }
  string path = defaultProjDir + filename;
  if (filename.length())
  {
    cerr << "Looking for " << filename << " at path " << path << "\n";
  }
  return path;
}


string
VarDBConverter::
projDirPath(const std::string& filename)
{
  return projDir + filename;
}



void
VarDBConverter::
checkModVars(VDBFile& vdb)
{
  char buffer[BUFSIZ+1];
  char name[100];
  string path = defaultProjDirPath("ModVars");

  FILE *fp;
  if ((fp = fopen(path.c_str(), "r")) == NULL)
  {
    return;
  }

  float vals[2];
  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    sscanf(buffer, "%s %f %f", name, &vals[0], &vals[1]);

    VDBVar* var = vdb.get_var(name);
    if (var)
    {
      ostringstream oss;
      oss << vals[0] << " " << vals[1];
      var->set_attribute(VDBVar::MODULUS_RANGE, oss.str());
    }
  }
  fclose(fp);
}


void
VarDBConverter::
checkDerivedNames(VDBFile& vdb)
{
  string path = defaultProjDirPath("DerivedNames");
  char buffer[1000];

  FILE *fp;
  if ((fp = fopen(path.c_str(), "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    char* p = strtok(buffer, " \t");

    VDBVar* var = vdb.get_var(p);
    if (var)
    {
      p = strtok(NULL, "\n");
      while (isspace(*p))
      {
	++p;
      }
      var->set_attribute(VDBVar::DERIVE, p);
    }
  }

  fclose(fp);
}


void
VarDBConverter::
checkDependencies(VDBFile& vdb)
{
  char buffer[1000], *p;

  string path = projDirPath("DependTable");

  FILE *fp;
  if ((fp = fopen(path.c_str(), "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    p = strtok(buffer, " \t\n");

    if (!p)
    {
      continue;
    }
    VDBVar* var = vdb.get_var(p);
    if (var)
    {
      if ( (p = strtok(NULL, "\n")) )
      {
	if (var->get_attribute(VDBVar::DEPENDENCIES).size())
	{
	  // There is more than one entry in the DependTable
          cerr << "Multiple entries for " << var->name()
	       << " in DependTable found, repair.  Fatal.\n";
          exit(1);
        }
        while (isspace(*p))
	{ 
	  ++p;
	}
	var->set_attribute(VDBVar::DEPENDENCIES, p);
      }
    }
  }
  fclose(fp);
}



VDBFile*
VarDBConverter::
open(VDBFile* vdb_in, const std::string& path)
{
  VDBFile& vdb = *vdb_in;

  if (path.rfind(".xml") == path.length() - 4)
  {
    cerr << "Opening " << path << " directly as XML..." << endl;
    vdb.open(path);
    return vdb_in;
  }

  // Initialize the project directory to be the directory containing the
  // vardb file in path.
  setupProjDir(path);

  if (defaultProjDir.empty())
  {
    cerr << "PROJ_DIR is not set, cannot convert to XML." << endl;
    return vdb_in;
  }

  vdb.create();

  // First categories and standard names.
  VDBFile::categories_type categories;
  std::string cpath = projDirPath("Categories");
  cerr << "Loading categories from " << cpath << "\n";
  categories = VDBFile::readCategories(cpath);
  vdb.set_categories(categories);

  std::string snamespath = projDirPath("StandardNames");
  VDBFile::standard_names_type standard_names;
  cerr << "Loading standard names from " << snamespath << "\n";
  standard_names = VDBFile::readStandardNames(snamespath);
  vdb.set_standard_names(standard_names);

  // Finally variables themselves, retrieved through the original 
  // VarDB interface.
  if (InitializeVarDB(path.c_str()) == ERR)
  {
    cerr << "VarDBConverter: Initialize failure." << endl;
    vdb.close();
    return vdb_in;
  }

  int i;
  for (i = 0; i < VarDB_nRecords; ++i)
  {
    struct var_v2 * vp = &((struct var_v2 *)VarDB)[i];

    if (isdigit(vp->Name[0]))
    {
      continue;
    }

    VDBVar* var = vdb.add_var(vp->Name);
    var->set_attribute(VDBVar::UNITS, vp->Units);
    var->set_attribute(VDBVar::LONG_NAME, vp->Title);
    if (ntohl(vp->is_analog) != 0)
    {
      // The ntohl() returns unsigned type, but the voltage range values
      // are actually signed.
      var->set_attribute(VDBVar::IS_ANALOG, true);
      ostringstream oss;
      oss << (int)ntohl(vp->voltageRange[0]) << " " 
	  << (int)ntohl(vp->voltageRange[1]);
      var->set_attribute(VDBVar::VOLTAGE_RANGE, oss.str());
      var->set_attribute(VDBVar::DEFAULT_SAMPLE_RATE, 
			 ntohl(vp->defaultSampleRate));
    }
    else
    {
      var->set_attribute(VDBVar::IS_ANALOG, false);
    }
    if(vp->MaxLimit - vp->MinLimit != 0)
    {
      var->set_attribute(VDBVar::MIN_LIMIT, ntohf(vp->MinLimit));
      var->set_attribute(VDBVar::MAX_LIMIT, ntohf(vp->MaxLimit));
    }
  
    string category = VarDB_GetCategoryName(vp->Name);
    if (category != "None")
    {
      var->set_attribute(VDBVar::CATEGORY, category);
    }
  }

  checkModVars(vdb);
  checkDerivedNames(vdb);
  checkDependencies(vdb);

  // Set the reference and standard_name attributes last just to be
  // consistent with the ordering used before introducing the VDBFile API.
  for (i = 0; i < VarDB_nRecords; ++i)
  {
    struct var_v2 * vp = &((struct var_v2 *)VarDB)[i];

    if (isdigit(vp->Name[0]))
    {
      continue;
    }

    VDBVar* var = vdb.get_var(vp->Name);
    string standard_name = VarDB_GetStandardNameName(vp->Name);
    if (standard_name != "None")
    {
      var->set_attribute(VDBVar::STANDARD_NAME, standard_name);
    }
    var->set_attribute(VDBVar::REFERENCE, bool(ntohl(vp->reference) != 0));
  }

  // What about Spikes & Lags?  Put in aircraft specific?
  // What about default global_attrs; coordinates, etc.

  // Done with the binary variable database.
  ReleaseVarDB();
  return vdb_in;
}


#ifdef notdef
// Copied from vdb2xml when it's functionality was moved into the library.
// Not sure it will ever be needed.
void
update_dictionary()
{
  // We could at this point search for the Dictionary file and insert those
  // lines into the XML file.  However, since the Dictionary is not used
  // anymore (not since the schema was fixed rather than generated), it's
  // not clear how to use it and where it should be maintained.
  fprintf(vdb,"  <Dictionary>\n");
  std::string dictionaryLocation =
    "/home/local/raf/vardb/utils/vdb2xml/Dictionary";
  std::ifstream dictionaryNames;
  dictionaryNames.open(dictionaryLocation.c_str());
  
  std::string raw_line,named,definition;
  while(std::getline(dictionaryNames,raw_line))
  {
    if (raw_line[0]!='#' and !raw_line.empty())
    {
      named="";
      definition="";
      std::stringstream iss(raw_line);
      std::getline(iss,named,',');
      std::getline(iss,definition,',');
      //std::cout<<named<<":: "<<definition<<"\n";
      fprintf(vdb,"    <definition name=\"%s\">%s</definition>\n",
	      named.c_str(),definition.c_str());
    }
  }
  fprintf(vdb,"  </Dictionary>\n");
}
#endif

