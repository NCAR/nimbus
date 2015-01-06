/*
-------------------------------------------------------------------------
OBJECT NAME:	vdb2ncml.cc

FULL NAME:	Convert VarDB to NcML.

DESCRIPTION:	Converts VarDB, Categories, StandardNames, DerivedNames,
		DependTable, ModVars and AircraftSpecs to NcML.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libgen.h>

#include <raf/vardb.h>
#include <raf/vardb.hh>

#include <raf/portable.h>
#include <fstream>
#include <sstream>
#include <string>

using std::string;
using std::ostringstream;
using std::cerr;

string defaultProjDir;
string projDir;

void
setupProjDir(const std::string& vardbarg)
{
  string dbdir(vardbarg);
  
  if (dbdir.find('/') != string::npos)
  {
    dbdir.erase(dbdir.rfind('/')+1, dbdir.length());
    projDir = dbdir;
  }
}


string
defaultProjDirPath(const std::string& filename)
{
  if (defaultProjDir.empty())
  {
    defaultProjDir = string(getenv("PROJ_DIR")) + "/Configuration/raf/";
  }
  string path = defaultProjDir + filename;
  return path;
}


string
projDirPath(const std::string& filename)
{
  return projDir + filename;
}


static const char COMMENT = '#';
static const float fill_value = -32767.0;

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
void
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

/* -------------------------------------------------------------------- */
void
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

/* -------------------------------------------------------------------- */
void
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

#ifdef notdef
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

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i = 1;

  if (argc < 2)
  {
    std::cerr << "Usage: vdb2xml [proj_num | VarDB_filename]\n";
    return(1);
  }

  string vdbpath = "vardb.xml";
  VDBFile vdb;
  vdb.create();

  // First categories and standard names.
  VDBFile::categories_type categories;
  categories = VDBFile::readCategories("Categories");
  vdb.set_categories(categories);

  VDBFile::standard_names_type standard_names;
  standard_names = VDBFile::readStandardNames("StandardNames");
  vdb.set_standard_names(standard_names);

  // Finally variables themselves, retrieved through the original 
  // VarDB interface.

  if (InitializeVarDB(argv[i]) == ERR)
  {
    fprintf(stderr, "vdb2xml: Initialize failure.\n");
    return(1);
  }

  setupProjDir(argv[i]);

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

  // Done with the binary variable database, the rest comes from text files.
  ReleaseVarDB();

  vdb.save(vdbpath);
  return(0);

}	/* END MAIN */

/* END VDB2NCML.CC */
