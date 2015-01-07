/* -*- mode: C++; c-basic-offset: 2; -*-
 * -------------------------------------------------------------------------
 *  OBJECT NAME:    vardb.cc
 *
 *  FULL NAME:      VarDB C++ API
 *
 *  TYPE:           API
 *
 *  DESCRIPTION:    This is an API to edit Vardb xml.
 *
 *  COPYRIGHT:      University Corporation for Atmospheric Research, 2014
 *  -------------------------------------------------------------------------
 */

#include "vardb_private.hh"

#include "domx/XML.h"
#include "logx/Logging.h"

LOGGING("VDBFile");

using namespace xercesc;
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

using xercesc::XMLString;
using domx::xstring;

#include "vdbschema.xsd.cc"

#include <xercesc/sax/EntityResolver.hpp>

namespace
{
  class EmbeddedEntityResolver : public xercesc::EntityResolver
  {
  public:
    xercesc::InputSource*
    resolveEntity (const XMLCh* const publicId, 
		   const XMLCh* const systemId)
    {
      xstring sysid(systemId);
      xstring xsd("vardb.xsd");
      if (sysid == xsd)
      {
	DLOG << "resolver returning embedded entity: " << sysid;
	return new xercesc::MemBufInputSource
	  ((const XMLByte*)VDBSCHEMA, strlen(VDBSCHEMA), xsd);
      }
      return 0;
    }

    virtual
    ~EmbeddedEntityResolver()
    {}

  };

  std::string default_document =
    "<?xml version='1.0'?>\n"
    "<VarDB>\n"
    "  <categories/>\n"
    "  <standard_names/>\n"
    "  <variableCatalog/>\n"
    "  <Dictionary/>\n"
    "</VarDB>\n";
}


const std::string 
VDBFile::
INDENT = "  ";

std::string
VDBFile::
nextLineIndent(int level)
{
  string indent = "\n";
  for (int i = 0; i < level; ++i)
  {
    indent += INDENT;
  }
  return indent;
}

DOMNode*
VDBFile::
newlineNode(DOMDocument* doc, int level)
{
  return doc->createTextNode(xstring(nextLineIndent(level)));
}

DOMNode*
VDBFile::
indentNode(DOMDocument* doc)
{
  return doc->createTextNode(xstring(INDENT));
}


std::string
VDBFile::
getTextValue(xercesc_forward::DOMNode* element)
{
  string value = domx::getTextElement(element);
  boost::trim(value);
  return value;
}


std::string
VDBFile::
trimVariableSuffix(const std::string& vname)
{
  return vname.substr(0, vname.find_last_of('_'));
}



VDBFile::
VDBFile(const std::string& filepath) :
  _doc(0),
  _dictionary(0),
  _parser(0)
{
  try
  { 
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException& toCatch) 
  {
    xstring message(toCatch.getMessage());
    std::cerr << "Error during initialization! :\n"
	      << message << "\n";
  };

  if (! filepath.empty())
  {
    open(filepath);
  }
}


VDBFile::
~VDBFile()
{
  close();
  XMLPlatformUtils::Terminate();
}


void
VDBFile::
create()
{
  close();
  load(default_document);
}


void
VDBFile::
close()
{
  // Release everything created when the document was parsed.
  delete _dictionary;
  _dictionary = 0;

  std::vector<VDBVar*>::iterator it;
  for (it = _variables.begin(); it != _variables.end(); ++it)
  {
    delete *it;
  }
  _variables.clear();

  if (_parser)
  {
    ErrorHandler *errReporter = _parser->getErrorHandler();
    _parser->setErrorHandler(0);
    delete errReporter;

    EntityResolver* resolver = _parser->getEntityResolver();
    _parser->setEntityResolver(0);
    delete resolver;
  }
  delete _parser;
  // The document is owned by the parser, so after deleting the parser both
  // pointers are invalid.
  _parser = 0;
  _doc = 0;
}


void
VDBFile::
save(const std::string& filepath)
{
  ofstream outpt;
  outpt.open(filepath.c_str());
  save(outpt);
  outpt.close();
}


void
VDBFile::
save(std::ostream& out)
{
  DOMImplementation *impl = 
    DOMImplementationRegistry::getDOMImplementation(xstring("LS"));
  DOMLSSerializer* theSerializer;
  theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();

  // DOMLSSerializer always encodes as UTF-16, but we immediately transcode
  // it to the native code page, meaning we somehow need to adjust the 
  // XML declaration written out by the serializer.  It does not help 
  // to disable the declaration header, because then the serializer just
  // adds things like namespaces attributes to the root element.
  // Instead, just rewrite the XML declaration.
  if (0)
  {
    DOMConfiguration* config = theSerializer->getDomConfig();
    config->setParameter(XMLUni::fgDOMXMLDeclaration, false);
  }

  xstring text;
  text.take(theSerializer->writeToString(_doc));
  theSerializer->release(); 

  boost::smatch matches;
  if (boost::regex_search(text, matches, boost::regex("<?[^?>]*?>")))
  {
    // We deliberately leave off the 'standalone' and 'encoding'
    // pseudo-attributes in the XML declaration.  'standalone' is
    // irrelevant when validating with XML Schema, and we are not encoding
    // to a particular code so leave it off lest the setting become
    // inconsistent.
    std::string xmldecl = "<?xml version=\"1.0\"?>\n";
    text.replace(0, matches.suffix().first - text.begin(), xmldecl);
  }

  out << text;
  out << "\n";
}


void
VDBFile::
open(const std::string& file)
{
  xstring xs (file);
  LocalFileInputSource source (xs);
  parse(file, source);
}


void
VDBFile::
load(const std::string& in, const std::string& file)
{
  MemBufInputSource source ((const XMLByte*)in.c_str(), in.length(),
			    "VDBFile::load()");
  parse(file, source);
}

void
VDBFile::
parse(const std::string& file, xercesc::InputSource& source)
{ 
  close();

  //save location
  _file = file;

  ostringstream errs;
  try 
  {
    _parser = new XercesDOMParser;
    _parser->setValidationScheme(XercesDOMParser::Val_Always);
    _parser->setDoNamespaces(true);
    _parser->setDoSchema(true);
    _parser->setLoadSchema(true);
    _parser->setIncludeIgnorableWhitespace(true);
    ErrorHandler *errReporter = new domx::StreamErrorLogger();
    _parser->setErrorHandler(errReporter);
    _parser->setCreateEntityReferenceNodes(false);

    // Add our own EntityResolver so the schema does not need to be on
    // disk, and make sure the external schema refers to it.
    EntityResolver* resolver = new EmbeddedEntityResolver();
    _parser->setEntityResolver(resolver);
    _parser->setExternalNoNamespaceSchemaLocation("vardb.xsd");
    DLOG << "parsing...";
    _parser->parse(source);
    _doc = _parser->getDocument();
    DLOG << "Document parsed.";

    // Now find the child elements we need and create their adapters.
    DOMElement* root = _doc->getDocumentElement();
    DOMElement* dictionary = domx::findElement(root, "Dictionary");
    _dictionary = new VDBDictionary(dictionary);
    load_variables();
  }
  catch (const SAXException& sx)
  {
    errs << "SAXException: " << xstring(sx.getMessage()) << "\n";
  }
  catch (const XMLException& xe)
  {
    errs << "XMLException: " << xstring(xe.getMessage()) << "\n";
  }
  catch (const DOMException& de)
  {
    errs << "DOMException: " << xstring(de.getMessage()) << "\n";
  }
  if (errs.str().size() > 0)
  {
    close();
    std::cerr << errs.str();
  }
}


void
VDBFile::
load_variables()
{
  DOMElement* root = _doc->getDocumentElement();
  DOMElement* catalog = domx::findElement(root, "variableCatalog");
  DOMNodeList *children = catalog->getChildNodes();

  _variables.clear();
  for (unsigned int i = 0; i <= children->getLength(); i++)
  {
    DOMElement* ve = domx::asElement(children->item(i));
    if (ve && xstring(ve->getTagName()) == "variable")
    {
      _variables.push_back(new VDBVar(ve));
    }
  }
}


//---------------------------------------------------------------------------
// This function searches through the XML document for an element with a
// given name
VDBVar*
VDBFile::
search_for_var(const std::string& var, bool expanded)
{
  VDBVar* xvar = 0;
  string basename = var;
  if (expanded)
  {
    basename = trimVariableSuffix(var);
  }

  std::vector<VDBVar*>::iterator it;
  for (it = _variables.begin(); it != _variables.end(); ++it)
  {
    if ((*it)->name() == var)
    {
      xvar = (*it);
      break;
    }
    if (expanded && !xvar && 
	(boost::iequals((*it)->name(), var) ||
	 boost::iequals((*it)->name(), basename)))
    {
      xvar = (*it);
    }
  }
  return xvar;
}


VDBVar*
VDBFile::
get_var(const std::string& var)
{
  return search_for_var(var, false);
}


VDBVar*
VDBFile::
search_var(const std::string& var)
{
  return search_for_var(var, true);
}


VDBVar*
VDBFile::
get_var(int ix)
{
  if (ix >= 0 && (unsigned int)ix < _variables.size())
  {
    return _variables[ix];
  }
  return 0;
}


int
VDBFile::
num_vars() const
{
  return _variables.size();
};


VDBVar*
VDBFile::
add_var(const std::string& vname)
{
  // Append a new variable element to the catalog and return a VDBVar
  // instance for it.
  VDBVar* var = get_var(vname);
  if (var)
  {
    return var;
  }

  DOMElement* root = _doc->getDocumentElement();
  DOMElement* catalog = domx::findElement(root, "variableCatalog");

  if (!catalog->getFirstChild())
  {
    catalog->appendChild(newlineNode(_doc, 1));
  }

  DOMElement* ve = _doc->createElement(xstring("variable"));
  domx::setAttribute(ve, "name", vname);
  catalog->appendChild(indentNode(_doc));
  catalog->appendChild(ve);
  catalog->appendChild(newlineNode(_doc, 1));
  var = new VDBVar(ve);
  _variables.push_back(var);
  return var;
}


VDBDictionary*
VDBFile::
get_dictionary()
{
  return _dictionary;
}


VDBFile::standard_names_type
VDBFile::
get_standard_names()
{
  DOMElement* root = _doc->getDocumentElement();
  DOMElement* snames = domx::findElement(root, "standard_names");
  DOMNodeList *children = snames->getChildNodes();

  standard_names_type names;

  for (unsigned int i = 0; i < children->getLength(); i++)
  {
    DOMElement* elem = domx::asElement(children->item(i));
    if (elem && xstring(elem->getTagName()) == "standard_name")
    {
      xstring value;
      domx::getAttribute(elem, "name", &value);
      names.push_back(value);
    }
  }
  return names;
}


void
VDBFile::
set_standard_names(const VDBFile::standard_names_type& names)
{
  // Completely replace the children of the standard_names element.
  DOMElement* root = _doc->getDocumentElement();
  DOMElement* snames = domx::findElement(root, "standard_names");
  domx::removeChildren(snames);

  if (names.size())
  {
    snames->appendChild(newlineNode(_doc, 1));
  }

  standard_names_type::const_iterator it;
  for (it = names.begin(); it != names.end(); ++it)
  {
    DOMElement* sname = _doc->createElement(xstring("standard_name"));
    domx::setAttribute(sname, "name", *it);
    snames->appendChild(indentNode(_doc));
    snames->appendChild(sname);
    snames->appendChild(newlineNode(_doc, 1));
    DLOG << "added standard_name " << *it;
  }
}

VDBFile::categories_type
VDBFile::
get_categories()
{
  DOMElement* root = _doc->getDocumentElement();
  DOMElement* cnames = domx::findElement(root, "categories");
  DOMNodeList *children = cnames->getChildNodes();

  categories_type names;

  for (unsigned int i = 0; i < children->getLength(); i++)
  {
    DOMElement* elem = domx::asElement(children->item(i));
    if (elem && xstring(elem->getTagName()) == "category")
    {
      xstring value;
      domx::getAttribute(elem, "name", &value);
      names.push_back(value);
    }
  }
  return names;
}


void
VDBFile::
set_categories(const VDBFile::categories_type& names)
{
  // Completely replace the children of the categories element.
  DOMElement* root = _doc->getDocumentElement();
  DOMElement* cnames = domx::findElement(root, "categories");
  domx::removeChildren(cnames);

  if (names.size())
  {
    cnames->appendChild(newlineNode(_doc, 1));
  }

  categories_type::const_iterator it;
  for (it = names.begin(); it != names.end(); ++it)
  {
    DOMElement* cname = _doc->createElement(xstring("category"));
    domx::setAttribute(cname, "name", *it);
    cnames->appendChild(indentNode(_doc));
    cnames->appendChild(cname);
    cnames->appendChild(newlineNode(_doc, 1));
    DLOG << "added category " << *it;
  }
}


VDBFile::categories_type
VDBFile::
readCategories(const std::string& filepath)
{
  categories_type categories;
  std::ifstream cats;
  cats.open(filepath.c_str());

  std::string raw_line, idnumber, definition;
  while (std::getline(cats, raw_line))
  {
    if (raw_line[0] != '#' && !raw_line.empty())
    {
      std::istringstream iss(raw_line);
      std::getline(iss, idnumber, ',');
      std::getline(iss, definition,',');

      // Trim any leading and trailing spaces
      boost::trim(definition);
      if (definition != "None")
      {
	categories.push_back(definition);
      }
    }
  }
  return categories;
}


VDBFile::standard_names_type
VDBFile::
readStandardNames(const std::string& filepath)
{
  return readCategories(filepath);
}

