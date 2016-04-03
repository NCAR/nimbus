
// Read sync records from an excerpt of a NIDAS dat file and verify the
// results.

#include "gtest/gtest.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "sync_reader.hh"
#include <nidas/util/EOFException.h>
#include <jsoncpp/json/json.h>

#include <nidas/core/NidasApp.h>
#include <nidas/core/Project.h>
#include <nidas/core/DSMSensor.h>
#include <nidas/core/SampleTag.h>
#include <nidas/dynld/raf/Aircraft.h>
#include <nidas/core/XMLParser.h>
#include <nidas/util/Process.h>
#include <memory>

// To get the functions for translating JSON sync records
#include <nidas/dynld/raf/SyncRecordReader.h>

using nidas::core::DSMSensor;
using nidas::core::Project;
using nidas::core::SampleTag;
using nidas::dynld::raf::Aircraft;
using nidas::core::parseXMLConfigFile;
using nidas::core::XMLImplementation;

using namespace nidas::dynld::raf;

// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// Usable AlmostEqual function

typedef union
{
  int asInt;
  float asFloat;
}
  twos_complement_t;
  

bool AlmostEqual2sComplement(float A, float B, int maxUlps=2)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    twos_complement_t a;
    a.asFloat = A;
    int aInt = a.asInt;
    //    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    twos_complement_t b;
    b.asFloat = B;
    int bInt = b.asInt;
    //    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}


TEST(AlmostEqual, SmokeTest)
{
  EXPECT_EQ(AlmostEqual2sComplement(1.0, 1.0), true);
  EXPECT_EQ(AlmostEqual2sComplement(1.0, -1.0), false);
  EXPECT_EQ(AlmostEqual2sComplement(-1.0, -1.0), true);
  EXPECT_EQ(AlmostEqual2sComplement(1.01, 1.02), false);
  EXPECT_EQ(AlmostEqual2sComplement(-1.01, -1.02), false);
  EXPECT_EQ(AlmostEqual2sComplement(1234556789, 1234556789.1), true);
  EXPECT_EQ(AlmostEqual2sComplement(-1234556789, -1234556789.1), true);
}


std::string
next_json_object(std::istream& in)
{
  // First look for an opening brace, then collect everything until the
  // next closing brace.  For now, assume the braces are the first character
  // in each line.
  bool inobject = false;
  bool linestart = true;
  std::string object;

  char c;
  while (in)
  {
    c = in.get();
    if (linestart && c == '{')
    {
      inobject = true;
    }
    if (inobject)
    {
      object.push_back(c);
      if (linestart && c == '}')
      {
	inobject = false;
	break;
      }
    }
    linestart = false;
    if (c == '\n')
      linestart = true;
  }
  // If object not completed before stream failed, then return nothing.
  if (inobject)
  {
    object.clear();
  }
  return object;
}


TEST(JsonTest, MissingJsonObject)
{
  {
    std::istringstream in;
    EXPECT_EQ(next_json_object(in), "");
  }
  {
    std::istringstream in("{ ...");
    EXPECT_EQ(next_json_object(in), "");
  }
  {
    std::istringstream in("\n{ ...");
    EXPECT_EQ(next_json_object(in), "");
  }
  {
    std::istringstream in("... \n}");
    EXPECT_EQ(next_json_object(in), "");
  }
}

TEST(JsonTest, NextJsonObject)
{
  {
    std::istringstream in("{ Hello World \n}");
    EXPECT_EQ(next_json_object(in), "{ Hello World \n}");
  }
  {
    std::istringstream in("junk here \n{ Hello World \n}");
    EXPECT_EQ(next_json_object(in), "{ Hello World \n}");
  }
  {
    std::istringstream in("junk here \n{ Hello World \n} and here");
    EXPECT_EQ(next_json_object(in), "{ Hello World \n}");
  }
  {
    std::istringstream in;
    in.str("junk here \n{ Hello World \n} and here \n{ second object \n}\n");
    EXPECT_EQ(next_json_object(in), "{ Hello World \n}");
    EXPECT_EQ(next_json_object(in), "{ second object \n}");
  }
}


#ifdef notdef
// These tests fail, and they indicate that jsoncpp seems to parse the
// whole stream up to eof and then return only the first object.  Thus the
// necessity of the next_json_object() function.
TEST(JsonTest, ObjectStream)
{
  std::istringstream in("{ \"id\": 0 }\n{ \"id\": 1}\n{ \"id\": 2 }\n");
  Json::Value value;
  Json::Reader reader;
  EXPECT_TRUE(reader.parse(in, value))
    << "failed to parse id 0: " << reader.getFormattedErrorMessages();
  EXPECT_EQ(value["id"].asInt(), 0);
  EXPECT_FALSE(in) << " istream is at eof.";
  EXPECT_TRUE(reader.parse(in, value))
    << "failed to parse id 1: " << reader.getFormattedErrorMessages();
  EXPECT_EQ(value["id"].asInt(), 1);
  EXPECT_TRUE(reader.parse(in, value))
    << "failed to parse id 2: " << reader.getFormattedErrorMessages();
  EXPECT_EQ(value["id"].asInt(), 2);
  EXPECT_FALSE(in);
}
#endif


static void
setup_logging()
{
  static bool done = false;
  if (!done)
  {
    static nidas::core::NidasApp napp("test_sync_reader");
    napp.parseLogLevel("debug");
    nidas::util::Logger *logger;
    logger = nidas::util::Logger::createInstance(&std::cerr);
    nidas::util::LogConfig lc;
    nidas::util::LogScheme ls("test_sync_reader");
    ls.setShowFields("time,level,thread,function,file,message");
    lc.level = napp.logLevel();
    logger->setScheme(ls.addConfig(lc));
  }
}



TEST(NidasProject, AircraftTest)
{
  setup_logging();
  Project* project = Project::getInstance();

  std::string xml_path = "$PROJ_DIR/IDEAS-4/GV_N677F/nidas/default_RF02.xml";
  xml_path = nidas::util::Process::expandEnvVars(xml_path);

  xercesc::DOMDocument* doc = parseXMLConfigFile(xml_path);
  project->fromDOMElement(doc->getDocumentElement());
  // Make sure to delete the doc before the terminate() below.
  delete doc;

  Aircraft* aircraft = Aircraft::getAircraft(project);

  ASSERT_TRUE(aircraft);
  EXPECT_EQ(aircraft->getTailNumber(), "N677F");

  Project::destroyInstance();
  XMLImplementation::terminate();
}



void
setup_environment(const std::string& flight = "unknown_flight")
{
  // Make the test harness set the PROJ_DIR
  if (0)
  {
    char buffer[1024] = "";
    getcwd(buffer, sizeof(buffer));
    //setenv("TMP_PROJ_DIR", buffer, 1);
    //setenv("PROJ_DIR", buffer, 1);
    //ASSERT_EQ(std::string(buffer), getenv("TMP_PROJ_DIR"));
  }
  setenv("FLIGHT", flight.c_str(), 1);

  ILOG(("TMP_PROJ_DIR=") << getenv("TMP_PROJ_DIR"));
  ILOG(("PROJ_DIR=") << getenv("PROJ_DIR"));
}


TEST(NidasProject, SerialNumbers)
{
  setup_logging();
  Project* project = Project::getInstance();

  std::string xml_path = "DC3-default.xml";
  xercesc::DOMDocument* doc = parseXMLConfigFile(xml_path);
  project->fromDOMElement(doc->getDocumentElement());
  delete doc;

  Aircraft* aircraft = Aircraft::getAircraft(project);

  ASSERT_TRUE(aircraft);
  EXPECT_EQ(aircraft->getTailNumber(), "N677F");

  // Retrieve metadata from the project configuration.
  std::list<const nidas::core::Variable*> variables;
  variables = selectVariablesFromProject(project);

  // I think both approaches should work: getting sensor from the SampleTag
  // or looking up the sensor in the Project by SampleTag.
  nidas::core::Variable* var;
  var = lookup_variable(variables, "ACDP_LWII");
  ASSERT_TRUE(var);
  const SampleTag* tag = var->getSampleTag();
  const DSMSensor* sensor = project->findSensor(tag);
  ASSERT_TRUE(sensor);
  EXPECT_EQ(getSerialNumber(var), "CDP058");

  var = lookup_variable(variables, "A1DC_LWIO");
  ASSERT_TRUE(var);
  sensor = var->getSampleTag()->getDSMSensor();
  ASSERT_TRUE(sensor);
  EXPECT_EQ(getSerialNumber(var), "F2DC001");

  Project::destroyInstance();
  XMLImplementation::terminate();
}



class SyncReaderTest : public testing::TestWithParam<bool>
{
protected:
  std::string xml_path;
  std::string header_file;
  std::string test_data_path;
  nidas::dynld::raf::SyncRecordReader* syncRecReader;
  bool postprocess;
  std::set<std::string> headers;
  std::list<const nidas::dynld::raf::SyncRecordVariable*> vars;

  virtual void SetUp()
  {
    setup_logging();
    SetSyncXMLPath("");
    postprocess = GetParam();
    header_file = "IDEAS-4-rf02-190000/20130926_190000_rf02_extract.ads";
    test_data_path = "IDEAS-4-rf02-190000/rf02.json";
    setup_environment("rf02");
    // In real-time the xml path should come from the sample input stream
    // config name.  Likewise for post-processing.
    if (false && postprocess)
    {
      xml_path = "IDEAS-4/GV_N677F/nidas/default_RF02.xml";
      SetSyncXMLPath(xml_path);
    }
  }

  void
  startSyncReader()
  {
    headers.insert(header_file);
    // If this is testing real-time and not post-process, then we must
    // first start the sync_server process to feed sync record samples.
    if (!postprocess)
    {
      StartSyncServerProcess(headers, xml_path);
    }
    syncRecReader = StartSyncReader(headers, postprocess);
    StartReadingSyncRecords();
  }

  void
  check_coefficients(const std::string& name,
		     const std::string& units, 
		     float c0,
		     float c1,
		     float c2 = -99999,
		     float c3 = -99999,
		     float c4 = -99999);

  virtual void TearDown()
  {
    CloseSyncReader();
    Project::destroyInstance();
    XMLImplementation::terminate();
  }
};
  


void
SyncReaderTest::
check_coefficients(const std::string& name,
		   const std::string& units, 
		   float c0, float c1, float c2, float c3, float c4)
{
  nidas::dynld::raf::SyncRecordVariable *var;
  var = lookup_variable(vars, name);
  ASSERT_TRUE(var) << "Variable " << name << " must exist.";
  float xca[] = { c0, c1, c2, c3, c4 };
  std::vector<float> xcoefs;
  int i = 0;
  while (i < 5 && xca[i] != -99999)
    xcoefs.push_back(xca[i++]);
  nidas::core::VariableConverter* converter =
    const_cast<nidas::core::VariableConverter*>(var->getConverter());
  ASSERT_TRUE(converter) << "Variable " << var->getName()
			 << " should have a VariableConverter";

  nidas::core::Polynomial* poly = 
    dynamic_cast<nidas::core::Polynomial*>(converter);
  nidas::core::Linear* linear = 
    dynamic_cast<nidas::core::Linear*>(converter);
  ASSERT_TRUE(poly || linear);

  std::vector<float> coefs;
  if (poly)
  {
    coefs = poly->getCoefficients();
  }
  else
  {
    coefs.push_back(linear->getIntercept());
    coefs.push_back(linear->getSlope());
  }
  EXPECT_EQ(xcoefs.size(), coefs.size())
    << var->getName() << " unexpected number of coefficients";
  for (unsigned int ic = 0; ic < coefs.size() && ic < xcoefs.size(); ++ic)
  {
    EXPECT_FLOAT_EQ(xcoefs[ic], coefs[ic])
      << var->getName() << " coefficients differ at " << ic;
  }
  EXPECT_EQ(units, converter->getUnits()) 
    << var->getName() << " units differ";
}


TEST_P(SyncReaderTest, CheckCurrentCalCoefficients)
{
  startSyncReader();

  EXPECT_EQ(syncRecReader->getProjectName(), "IDEAS-4");
  EXPECT_EQ(syncRecReader->getTailNumber(), "N677F");
  EXPECT_EQ(syncRecReader->getFlightName(), "rf02");

  time_t t = syncRecReader->getStartTime();

  nidas::util::UTime ut(t);
  ut.setFormat("%Y%m%d-%H:%M:%S");
  EXPECT_EQ(ut.format(), "20130926-19:00:00");

  vars = syncRecReader->getVariables();

  nidas::dynld::raf::SyncRecordVariable *var;
  var = lookup_variable(vars, "F2DIO1_RWOI");
  ASSERT_TRUE(var);
  EXPECT_EQ(var->getSampleRate(), 10);
  EXPECT_EQ(var->getLength(), 1);
  EXPECT_EQ(var->getUnits(), "V");
  EXPECT_EQ(var->getLongName(), "Fast 2DC End Diode Voltage #1");
  check_coefficients(var->getName(), "V", 0, 1);
  if (0)
  {
    // I don't think these are actually written to the sync header, and
    // therefore they come through as 0.
    EXPECT_EQ(var->getSampleTag()->getDSMId(), 86);
    EXPECT_EQ(var->getSampleTag()->getSampleId(), 2);
  }

  var = lookup_variable(vars, "RTF1");
  ASSERT_TRUE(var);
  EXPECT_EQ(var->getSampleRate(), 100);
  EXPECT_EQ(var->getLength(), 1);
  EXPECT_EQ(var->getUnits(), "V");
  EXPECT_EQ(var->getLongName(), "Recovery Air Temperature, Radome Left");
  check_coefficients(var->getName(), "deg_C", -84.8767, 26.4716, 0.295478);

  check_coefficients("CVPCN", "mb", -5.817, 342.05, -0.0354);
  // PITCH.dat has three coefficients but the XML declares a linear
  // converter, so only the first two coefficients are read from the file.
  check_coefficients("PITCH_IRS3", "degree", 0, 1);
  check_coefficients("STRTEMP_ITR", "deg_C", 
		     -82.7496, 2.80542E-3, 1.24777E-9);
  check_coefficients("BENTEMP_ITR", "deg_C", 
		     -82.7496, 2.80542E-3, 1.24777E-9);
  check_coefficients("TCAB", "deg_C", 0, 100);
  check_coefficients("PLWC", "W", 0, 10);
  check_coefficients("NOSETMP", "deg_C", -50.0, 10.0);
  check_coefficients("PDUMPPL", "hPa", 3.86, 103.76, 0.0555);
  check_coefficients("PDUMPPR", "hPa", -2.5, 103.93, 0.0235);
  check_coefficients("SNKTEMP_ITR", "deg_C", -82.7496, 2.80542E-3, 1.24777E-9);
  check_coefficients("RTH1", "deg_C", -82.4031f, 22.6579f, 0.293203f);
  check_coefficients("RTH2", "deg_C", -82.0533f, 22.6136f, 0.30347f);
}


void
remove_parameter(std::string& header, const std::string& parameter)
{
  // Find the start position and remove up until the newline.
  std::string::size_type pos = header.find(parameter);
  if (pos != std::string::npos)
  {
    std::string::size_type p2 = header.find("\n", pos);
    header.replace(pos, p2 - pos, "");
  }
}

bool
compare_headers(std::string& xheader, std::string& header)
{
  // Erase the trailing null.
  if (! *(header.end()-1))
  {
    header.erase(header.end()-1);
  }
  // Remove the software revision line.
  // software_version \"7104M\"\n
  remove_parameter(header, "software_version");
  remove_parameter(xheader, "software_version");

  // Remove any trailing whitespace.
  while (header.rbegin() != header.rend() && isspace(*header.rbegin()))
    header.erase(header.length()-1);
  while (xheader.rbegin() != xheader.rend() && isspace(*xheader.rbegin()))
    xheader.erase(xheader.length()-1);

  std::string::iterator hi = header.begin();
  std::string::iterator xi = xheader.begin();
  while (hi != header.end() && xi != xheader.end() && *hi == *xi)
  {
    ++hi, ++xi;
  }
  if (xi != xheader.end() || hi != header.end())
  {
    std::cerr << "received header did not match expected at position "
	      << xi - xheader.begin() << ":\n";
    std::cerr << "Expected header line:\n"
	      << xheader.substr(xi-xheader.begin(), 80) << "...\n";
    std::cerr << "Actual header line:\n"
	      << header.substr(hi-header.begin(), 80) << "...\n";
  }
  // std::cerr << ">>>>>>>>>> Received header:\n" << header;
  // std::cerr << "<<<<<<<<<< Expected header:\n" << xheader;
  return !(xi != xheader.end() || hi != header.end());
}


TEST_P(SyncReaderTest, CompareHeaderAndRecords)
{
  test_data_path = "IDEAS-4-rf02-190000/rf02.json";

  startSyncReader();

  EXPECT_EQ(syncRecReader->getProjectName(), "IDEAS-4");
  EXPECT_EQ(syncRecReader->getTailNumber(), "N677F");
  time_t t = syncRecReader->getStartTime();

  nidas::util::UTime ut(t);
  ut.setFormat("%Y%m%d-%H:%M:%S");
  EXPECT_EQ(ut.format(), "20130926-19:00:00") << "comparing getStartTime()";

  std::string header = syncRecReader->textHeader();

  // Start loading the test data to compare against.
  Json::Value root;
  Json::Reader reader;
  std::ifstream test_data(test_data_path.c_str());

  // Write what we get to another json file to simplify comparisons.
  std::string oname = std::string(postprocess ? "pp" : "rt") +
    std::string("-compareheaderandrecords.json");
  std::ofstream json(oname.c_str());
  write_sync_record_header_as_json(json, header);

  ASSERT_TRUE(reader.parse(next_json_object(test_data), root))
    << "failed to parse header object: " << reader.getFormattedErrorMessages();
  std::string xheader = json_sync_record_header_as_string(root);
  
  EXPECT_TRUE(compare_headers(xheader, header));

  typedef const std::list<const SyncRecordVariable*> varlist_t;
  varlist_t variables = syncRecReader->getVariables();

  int ncompared = 0;
  int len = syncRecReader->getNumValues();
  std::vector<double> record(len);
  nidas::core::dsm_time_t tt;

  // Do not generate a message about every failure, since that can be slow
  // and unwieldy.  Instead include messages only about the first few, then
  // skip the rest so we can at least write the full json output for
  // comparison.
  int nfail = 0;
  int maxfails = 10;
  while (test_data)
  {
    std::string object = next_json_object(test_data);
    if (object.empty())
      break;
    if (0)
    {
      std::cerr << "found next object: " << object.substr(0, 30)
		<< "..." 
		<< object.substr(std::max((int)(object.length() - 30), 30), 30)
		<< std::endl;
    }
    ASSERT_TRUE(reader.parse(object, root))
      << "Failed to parse data object: " 
      << reader.getFormattedErrorMessages();
    int rc = syncRecReader->read(&tt, &record.front(), len);
    write_sync_record_data_as_json(json, tt, &record.front(), len);

    std::cerr << "====> Sync record with "
	      << len << " values at time "
	      << nidas::util::UTime(tt).format()
	      << " <====" << std::endl; 

    EXPECT_EQ(rc, len);
    EXPECT_EQ(tt, root["time"].asUInt64());
    EXPECT_EQ(len, root["numValues"].asInt());
    EXPECT_EQ(len, root["data"].size());

    // Finally compare the actual data values.
    Json::Value& data = root["data"];
    for (varlist_t::const_iterator it = variables.begin();
	 it != variables.end(); ++it)
    {
      const SyncRecordVariable* const &var = (*it);
      int nvalues = var->getLength();
      int offset = var->getSyncRecOffset();

      for (int j = 0; j < nvalues; ++j)
      {
	double xf = strtod(data[offset+j].asCString(), NULL);
	double gotf = record[offset+j];
	if (isnan(xf))
	{
	  nfail += !isnan(gotf);
	  if (nfail <= maxfails)
	  {
	    EXPECT_TRUE(isnan(gotf))
	      << "variable " << var->getName()
	      << "[" << j << "] @ time " << tt;
	  }
	}
	else if (!AlmostEqual2sComplement(xf, gotf))
	{
	  // NIDAS sensor fixed GGREFID, so accept it here until the good
	  // test data are regenerated.
	  if (var->getName() != "GGREFID" || gotf != 1008)
	  {
	    ++nfail;
	    if (nfail <= maxfails)
	    {
	      EXPECT_FLOAT_EQ(xf, gotf)
		<< "variable " << var->getName()
		<< "[" << j << "] @ time " << tt;
	    }
	  }
	}
	++ncompared;
      }
    }
  }
  // Last read should raise EOF exception.
  EXPECT_THROW(syncRecReader->read(&tt, &record.front(), len),
	       nidas::util::EOFException);

  test_data.close();
}


INSTANTIATE_TEST_CASE_P(PostProcess, SyncReaderTest, testing::Values(true)); 
INSTANTIATE_TEST_CASE_P(RealTime, SyncReaderTest, testing::Values(false)); 
