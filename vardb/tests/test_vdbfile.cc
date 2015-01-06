
#include <iosfwd>
#include <gtest/gtest.h>

#include "vardb.hh"
#include "logx/Logging.h"

#include <cstdlib>
#include <algorithm>
#include <iterator>

LOGGING("test_vdbfile");

TEST (VDBFileTest, CreateEmptyDoc)
{
  VDBFile vdb;

  EXPECT_FALSE(vdb.is_valid());
  vdb.create();
  EXPECT_TRUE(vdb.is_valid());

  // There should be no standard names and no variables in the catalog.
  VDBDictionary* vdict = vdb.get_dictionary();
  ASSERT_TRUE(vdict);

  EXPECT_EQ(vdb.num_vars(), 0);
  EXPECT_EQ(vdict->num_entries(), 0);
}


// Cheat on the encoding for now since I don't know how to set that so it
// comes out the same.
const std::string emptyxml =
  "<?xml version=\"1.0\"?>\n"
  "<VarDB>\n"
  "  <categories>\n"
  "  </categories>\n"
  "  <standard_names>\n"
  "  </standard_names>\n"
  "  <variableCatalog>\n"
  "  </variableCatalog>\n"
  "  <Dictionary>\n"
  "  </Dictionary>\n"
  "</VarDB>\n";

TEST (VDBFileTest, ParseEmptyXML)
{
  VDBFile vdb;

  vdb.load(emptyxml);
  EXPECT_TRUE(vdb.is_valid());

  // There should be no standard names and no variables in the catalog.
  VDBDictionary* vdict = vdb.get_dictionary();
  ASSERT_TRUE(vdict);

  EXPECT_EQ(vdb.num_vars(), 0);
  EXPECT_EQ(vdict->num_entries(), 0);
}


TEST (VDBFileTest, SaveEmptyXML)
{
  // Loading and saving an empty doc should return the same document.
  VDBFile vdb;

  vdb.load(emptyxml);
  ASSERT_TRUE(vdb.is_valid());

  std::ostringstream out;
  vdb.save(out);
  EXPECT_EQ(emptyxml, out.str());
}


const std::string commentxml =
  "<?xml version=\"1.0\"?>\n"
  "<VarDB>\n"
  "  <categories>\n"
  "    <!-- No categories here. -->\n"
  "  </categories>\n"
  "  <standard_names>\n"
  "     <!-- One standard_name here. -->\n"
  "     <standard_name name=\"altitude\"/>\n"
  "  </standard_names>\n"
  "  <variableCatalog>\n"
  "    <variable name=\"YEAR\">\n"
  "    <units>year</units>\n"
  "    <long_name>Raw Tape Date Component</long_name>\n"
  "  </variable>\n"
  "  <variable name=\"ZERO\">\n"
  "   <!-- This variable is always last. -->\n"
  "   <units>none</units>\n"
  "   <long_name>Constant value of 0.</long_name>\n"
  "   <category>Housekeeping</category>\n"
  "  </variable>\n"
  "  </variableCatalog>\n"
  "  <Dictionary>\n"
  "  </Dictionary>\n"
  "</VarDB>\n";

TEST (VDBFileTest, PreserveXMLComments)
{
  // Loading and saving a doc with comments should preserve comments.
  VDBFile vdb;

  vdb.load(commentxml);
  ASSERT_TRUE(vdb.is_valid());

  std::ostringstream out;
  vdb.save(out);
  EXPECT_EQ(commentxml, out.str());
}


TEST (VDBFileTest, ValidateFile)
{
  VDBFile vdb("contrast_vardb.xml");
  ASSERT_TRUE(vdb.is_valid());

  EXPECT_EQ(vdb.num_vars(), 772);

  std::vector<std::string> names;
  names = vdb.get_standard_names();

  EXPECT_EQ(names.size(), 36);
  EXPECT_EQ(names[0], "altitude");
  EXPECT_EQ(names[35], "mole_fraction_of_ozone_in_air");
}


int
compare_load_save(const std::string& xml)
{
  VDBFile vdb;
  vdb.open(xml);
  std::string outpath = xml + ".saved";
  vdb.save(outpath);
  vdb.close();

  std::string cmd = "diff " + xml + " " + outpath;
  return system(cmd.c_str());
}


TEST (VDBFileTest, PreserveFile)
{
  EXPECT_EQ(compare_load_save("contrast_vardb.xml"), 0);
  EXPECT_EQ(compare_load_save("deepwave_vardb.xml"), 0);
}


TEST (VDBFileTest, ReadDefinitions)
{
  // The deepwave example has definitions, so just make sure they can be
  // read same as before, even though otherwise they are not used.
  VDBFile vdb;
  vdb.open("deepwave_vardb.xml");
  ASSERT_TRUE(vdb.is_valid());

  VDBDictionary* vdbdict = vdb.get_dictionary();
  ASSERT_TRUE(vdbdict);
  ASSERT_EQ(vdbdict->num_entries(), 13);

  EXPECT_EQ(vdbdict->get_entry(1), "long_name");
  EXPECT_EQ(vdbdict->get_definition(vdbdict->get_entry(1)),
	    "Description of signal");
  EXPECT_EQ(vdbdict->get_entry(12), "reference");
  EXPECT_EQ(vdbdict->get_definition(vdbdict->get_entry(12)),
	    "The definition of the reference attribute.");
}


const std::string brokenxml =
  "<?xml version=\"1.0\"?>\n"
  "<VarDB>\n"
  "  <categories>\n"
  "  </categories>\n"
  "  <standard_names>\n"
  "  </standard_names>\n"
  "  <variableCatalog>\n"
  "    <variable><unit>m</unit></variable>\n"
  "  </variableCatalog>\n"
  "  <Dictionary>\n"
  "  </Dictionary>\n"
  "</VarDB>";


TEST (VDBFileTest, ReportErrors)
{
  VDBFile vdb;

  vdb.load(brokenxml);
  EXPECT_FALSE(vdb.is_valid());
}


const std::string akrdxml =
  "<?xml version=\"1.0\"?>\n"
  "<VarDB>\n"
  "  <categories>\n"
  "    <category name='Aircraft State'/>\n"
  "    <category name='Atmos. State'/>\n"
  "  </categories>\n"
  "  <standard_names>\n"
  "     <standard_name name='altitude'/>\n"
  "  </standard_names>\n"
  "  <variableCatalog>\n"
  "  <variable name='AKRD'>\n"
  "    <units>degree</units>\n"
  "    <long_name>Attack Angle, Radome Diff. Pressure</long_name>\n"
  "    <is_analog>false</is_analog>\n"
  "    <category>Aircraft State</category>\n"
  "    <dependencies>\n"
  "       ADIFR QCF PSF\n"
  "    </dependencies>\n"
  "    <reference>false</reference>\n"
  "  </variable>\n"
  "  </variableCatalog>\n"
  "  <Dictionary>\n"
  "  </Dictionary>\n"
  "</VarDB>";


TEST (VDBFileTest, ReadVariable)
{
  VDBFile vdb;
  vdb.load(akrdxml);
  ASSERT_TRUE(vdb.is_valid());

  VDBVar* akrd = vdb.get_var("AKRD");
  ASSERT_TRUE(akrd);

  EXPECT_EQ(akrd->name(), "AKRD");
  EXPECT_EQ(akrd->num_atts(), 6);
  EXPECT_EQ(akrd->get_attribute("units"), "degree");
  EXPECT_EQ(akrd->get_attribute(2), "is_analog");
  EXPECT_EQ(akrd->get_attribute(akrd->get_attribute(2)), "false");
  EXPECT_EQ(akrd->get_attribute_value<bool>(VDBVar::REFERENCE), false);
  EXPECT_EQ(akrd->get_attribute("long_name"),
	    "Attack Angle, Radome Diff. Pressure");
  EXPECT_EQ(akrd->get_attribute("dependencies"), "ADIFR QCF PSF");

  // A missing attribute should return an empty value.
  EXPECT_EQ(akrd->get_attribute(VDBVar::VOLTAGE_RANGE), "");
}


TEST (VDBFileTest, MatchVariable)
{
  // Make sure case-insensitive and suffix-qualified name searches work.
  VDBFile vdb;
  vdb.open("deepwave_vardb.xml");
  ASSERT_TRUE(vdb.is_valid());

  VDBVar* akrd = vdb.get_var("AKRD");
  ASSERT_TRUE(akrd);
  EXPECT_EQ(akrd->name(), "AKRD");

  VDBVar* akrd2 = vdb.get_var("akrd");
  EXPECT_FALSE(akrd2);

  akrd2 = vdb.get_var("akrd_RI");
  EXPECT_FALSE(akrd2);

  // While we're at it make sure add_var returns the same instance.
  akrd2 = vdb.add_var("AKRD");
  EXPECT_EQ(akrd, akrd2);

  VDBVar* wigp = vdb.get_var("WI_GP");
  ASSERT_TRUE(wigp);
  EXPECT_EQ(wigp->name(), "WI_GP");

  VDBVar* wi = vdb.get_var("WI");
  ASSERT_TRUE(wi);
  EXPECT_EQ(wi->name(), "WI");

  wigp = vdb.search_var("WI_GP");
  ASSERT_TRUE(wigp);
  EXPECT_EQ(wigp->name(), "WI_GP");

  wi = vdb.search_var("WI");
  ASSERT_TRUE(wi);
  EXPECT_EQ(wi->name(), "WI");

  VDBVar* qcf_x = vdb.get_var("QCF_X");
  EXPECT_FALSE(qcf_x);
  qcf_x = vdb.search_var("qcf_x");
  EXPECT_TRUE(qcf_x);
  EXPECT_EQ(qcf_x->name(), "QCF");

  EXPECT_EQ(VDBFile::trimVariableSuffix("a_b_c"), "a_b");
  EXPECT_EQ(VDBFile::trimVariableSuffix("abc"), "abc");
}


TEST (VDBFileTest, SetStandardNames)
{
  VDBFile vdb;
  vdb.load(akrdxml);
  ASSERT_TRUE(vdb.is_valid());

  std::vector<std::string> oldnames = vdb.get_standard_names();
  EXPECT_EQ(oldnames.size(), 1);
  EXPECT_EQ(oldnames[0], "altitude");

  // Make up our own standard_names
  const char* names[] = {
    "one", "two", "three"
  };
  VDBFile::standard_names_type newnames(names, names+3);
  vdb.set_standard_names(newnames);

  // Now save, load, and compare.
  std::ostringstream out;
  vdb.save(out);
  DLOG << out.str();

  vdb.close();
  vdb.load(out.str());
  ASSERT_TRUE(vdb.is_valid());
  oldnames = vdb.get_standard_names();
  ASSERT_EQ(oldnames.size(), newnames.size());
  for (unsigned int i = 0; i < oldnames.size(); ++i)
  {
    EXPECT_EQ(oldnames[i], newnames[i])
      << " compare names at index=" << i;
  }
}


TEST (VDBFileTest, SetCategories)
{
  VDBFile vdb;
  vdb.load(akrdxml);
  ASSERT_TRUE(vdb.is_valid());

  std::vector<std::string> oldnames = vdb.get_categories();
  EXPECT_EQ(oldnames.size(), 2);
  EXPECT_EQ(oldnames[0], "Aircraft State");
  EXPECT_EQ(oldnames[1], "Atmos. State");

  // Add new categories
  const char* names[] = {
    "Housekeeping",
    "Chemistry",
    "Radiation"
  };
  VDBFile::categories_type newnames(oldnames);
  std::copy(names, names+3, std::back_inserter(newnames));
  vdb.set_categories(newnames);

  // Now save, load, and compare.
  std::ostringstream out;
  vdb.save(out);
  DLOG << out.str();

  vdb.close();
  vdb.load(out.str());
  ASSERT_TRUE(vdb.is_valid());
  VDBFile::categories_type newcats = vdb.get_categories();
  ASSERT_EQ(newcats.size(), newnames.size());
  for (unsigned int i = 0; i < newcats.size(); ++i)
  {
    EXPECT_EQ(newcats[i], newnames[i]) 
      << " compare categories at index=" << i;
  }
  EXPECT_EQ(newcats[0], oldnames[0]);
  EXPECT_EQ(newcats[4], names[2]);
}


TEST (VDBFileTest, AddVar)
{
  VDBFile vdb;

  vdb.create();
  EXPECT_EQ(vdb.num_vars(), 0);
  VDBVar* qcf = vdb.add_var("QCF");
  ASSERT_TRUE(qcf);
  EXPECT_EQ(vdb.num_vars(), 1);
  qcf->set_attribute(VDBVar::LONG_NAME, "Raw Dynamic Pressure, Fuselage");
  qcf->set_attribute(VDBVar::UNITS, "hPa");
  qcf->set_attribute(VDBVar::STANDARD_NAME, "air_pressure");
  qcf->set_attribute(VDBVar::REFERENCE, false);
  qcf->set_attribute(VDBVar::IS_ANALOG, true);
  EXPECT_EQ(qcf->get_attribute_value<bool>(VDBVar::IS_ANALOG), true);
  qcf->set_attribute(VDBVar::VOLTAGE_RANGE, "0 10");
  qcf->set_attribute(VDBVar::DEFAULT_SAMPLE_RATE, 100);
  EXPECT_EQ(qcf->get_attribute_value<int>(VDBVar::DEFAULT_SAMPLE_RATE), 100);
  qcf->set_attribute(VDBVar::CATEGORY, "Uncorr'd Raw");

  EXPECT_EQ(qcf->num_atts(), 8);
  EXPECT_EQ(qcf->get_attribute(VDBVar::LONG_NAME),
	    "Raw Dynamic Pressure, Fuselage");
  EXPECT_EQ(qcf->get_attribute(VDBVar::UNITS), "hPa");
  EXPECT_EQ(qcf->get_attribute(VDBVar::STANDARD_NAME), "air_pressure");
  EXPECT_EQ(qcf->get_attribute(VDBVar::REFERENCE), "false");
  EXPECT_EQ(qcf->get_attribute(VDBVar::IS_ANALOG), "true");
  EXPECT_EQ(qcf->get_attribute(VDBVar::VOLTAGE_RANGE), "0 10");
  EXPECT_EQ(qcf->get_attribute(VDBVar::DEFAULT_SAMPLE_RATE), "100");
  EXPECT_EQ(qcf->get_attribute(VDBVar::CATEGORY), "Uncorr'd Raw");
  // Missing attribute returns empty string.
  EXPECT_EQ(qcf->get_attribute(VDBVar::MODULUS_RANGE), "");

  std::ostringstream out;
  vdb.save(out);
  DLOG << out.str();
  vdb.load(out.str());
  qcf = vdb.get_var("QCF");
  ASSERT_TRUE(qcf);

  EXPECT_EQ(qcf->get_attribute(VDBVar::LONG_NAME),
	    "Raw Dynamic Pressure, Fuselage");
  EXPECT_EQ(qcf->get_attribute(VDBVar::UNITS), "hPa");
  EXPECT_EQ(qcf->get_attribute(VDBVar::STANDARD_NAME), "air_pressure");
  EXPECT_EQ(qcf->get_attribute(VDBVar::REFERENCE), "false");
  EXPECT_EQ(qcf->get_attribute(VDBVar::IS_ANALOG), "true");
  EXPECT_EQ(qcf->get_attribute(VDBVar::VOLTAGE_RANGE), "0 10");
  EXPECT_EQ(qcf->get_attribute(VDBVar::DEFAULT_SAMPLE_RATE), "100");
  EXPECT_EQ(qcf->get_attribute(VDBVar::CATEGORY), "Uncorr'd Raw");

}

TEST (VDBFileTest, ReadCategories)
{
  // Test reading category names from a Categories text file.
  VDBFile::categories_type categories;
  categories = VDBFile::readCategories("Categories");
  ASSERT_EQ(categories.size(), 12);
  EXPECT_EQ(categories[0], "Position");
  EXPECT_EQ(categories[5], "Uncorr'd Raw");
  EXPECT_EQ(categories[6], "Wind");
  EXPECT_EQ(categories[8], "Housekeeping");
  EXPECT_EQ(categories[11], "Non-Standard");
}

TEST (VDBFileTest, ReadStandardNames)
{
  // Test reading category names from a Categories text file.
  VDBFile::standard_names_type snames;
  snames = VDBFile::readStandardNames("StandardNames");
  ASSERT_EQ(snames.size(), 36);
  EXPECT_EQ(snames[0], "altitude");
  EXPECT_EQ(snames[31], "atmosphere_number_content_of_aerosol_particles");
  EXPECT_EQ(snames[35], "mole_fraction_of_ozone_in_air");
}



int
main(int argc, char **argv)
{
  ::logx::ParseLogArgs(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
