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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/util/XMLString.hpp>

#define _vardb_cc_
namespace xercesc_forward = xercesc;
#include "vardb.hh"

using namespace xercesc;
using namespace std;

#include <boost/algorithm/string.hpp>

using xercesc::XMLString;

#include "vdbschema.xsd.cc"

namespace
{
  /**
   * A class for easily interchanging between std::string and XMLCh*.  It
   * stores the current value as a std::string, then transcodes to XMLCh*
   * as needed.  Likewise the value of the string can be changed by
   * assigning XMLCh* to it.
   **/
  class xstring : public std::string
  {
  public:
    typedef std::string string;

    const XMLCh *
    xc() const
    {
      xercesc::XMLString::release(&mxc);
      mxc = XMLString::transcode(this->c_str());
      return mxc;
    }

    operator const XMLCh *() const
    {
      return xc();
    }

    xstring (const XMLCh *xc)
    {
      mxc = 0;
      assign (xc);
    }

    xstring ()
    {
      mxc = 0;
    }

    xstring (const char s[])
    {
      mxc = 0;
      string::operator= (s);
    }

    xstring (const xstring &xs) : string (xs)
    {
      mxc = 0;
    }

    xstring (const string &xs) : string (xs)
    {
      mxc = 0;
    }

    xstring &operator= (const XMLCh *xc)
    {
      return assign (xc);
    }

    xstring &operator= (const xstring &xs)
    {
      string::operator= (xs);
      return *this;
    }

    xstring &assign (const XMLCh *xc)
    {
      if (xc)
      {
	char *p = XMLString::transcode(xc);
	string::operator= (p);
        xercesc::XMLString::release(&p);
      }
      else
      {
	string::operator= ("");
      }
      return *this;
    }

    ~xstring ()
    {
      xercesc::XMLString::release(&mxc);
      mxc = 0;
    }

  private:
    mutable XMLCh *mxc;
  };

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
	return new xercesc::MemBufInputSource
	  ((const XMLByte*)VDBSCHEMA, strlen(VDBSCHEMA), xsd);
      }
      return 0;
    }

    virtual
    ~EmbeddedEntityResolver()
    {}

  };

}


//---------------------------------------------------------------------------
// Returns number of entries in dictionary
int VDBDictionary::num_entries() const
{
  DOMNodeList* elms=_docRoot->getChildNodes();
  return elms->getLength()/2;
}

//---------------------------------------------------------------------------
// Returns Dictionary index starting at 0
std::string VDBDictionary::get_entry(int index)
{
  DOMNodeList *elms = _docRoot->getChildNodes();
  DOMNode *holder = _docRoot->getFirstChild();
  holder = holder->getNextSibling();
  if (index > (int)elms->getLength()/2-1)
  {
    return "";
  }
  for (int i = 0; i < index; i++)
  {
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
  DOMNamedNodeMap *atts = holder->getAttributes();
  DOMNode *att = atts->getNamedItem(XMLString::transcode("name"));
  return XMLString::transcode(att->getNodeValue());
}

//---------------------------------------------------------------------------
// This function initiates a dictionary.
VDBDictionary* VDBFile::get_dictionary(const std::string dictionary_name) const
{
  XMLCh *tag = XMLString::transcode(dictionary_name.c_str());
  DOMNodeList* x = _docRootNode->getElementsByTagName(tag);
  DOMNode* Dictionary = x->item(0);
  VDBDictionary *vd = new VDBDictionary(Dictionary,dictionary_name);
  return vd;
}

//---------------------------------------------------------------------------
// returns variable name
std::string VDBVar::name()
{
  DOMNamedNodeMap* atts = _variable->getAttributes();
  DOMNode*  name = atts->getNamedItem(XMLString::transcode("name"));
  return XMLString::transcode(name->getNodeValue());
}

//---------------------------------------------------------------------------
// Returns text associated with attribute, given by index
// input: index- attribute's position in vdb variable
// output-attribute value
std::string VDBVar::get_attribute(int index)const
{
  DOMNodeList* x = _variable->getChildNodes();
  DOMNode* holder = x->item(1);
  if (index > (int)x->getLength()/2-1)
  {
    return "";
  }
  for (int i = 0; i < index; i++)
  {
    holder = holder->getNextSibling();
    holder = holder->getNextSibling();
  } 
  holder = holder->getPreviousSibling();
  DOMNodeList *y = holder->getChildNodes();
  DOMNode *z = y->item(0);
  std::string answer = XMLString::transcode(z->getNodeValue());
  return answer;
}

//---------------------------------------------------------------------------
// Returns number of attributes associated sellected variable
int VDBVar::num_atts() const
{
  DOMNodeList* x = _variable->getChildNodes();
  return x->getLength()/2;
}

//---------------------------------------------------------------------------
// Returns text associated with a variable's attribute
// input: attr_name: attribute name, such as units, name, or longname
// output: Text associated with variable's attribute
std::string VDBVar::get_attribute(const std::string attr_name) const
{ 
  DOMNodeList* x = _variable->getChildNodes();
  DOMNode* holder = x->item(1);
  std::string answer;

  for (int i = 0; i < int(x->getLength())/2; i++)
  {
    if (boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
    {
      DOMNodeList *y = holder->getChildNodes();
      DOMNode *z = y->item(0);
      if (z)
        answer = XMLString::transcode(z->getNodeValue());
      return answer;
    }
    holder = holder->getNextSibling();
    holder = holder->getNextSibling();
  }
  return answer;
}
//---------------------------------------------------------------------------
//Sets a variable's information for specified attribute
//input: attr_name: attribute name, such as units, name, or longname
void VDBVar::set_attribute(const std::string attr_name, const std::string value)
{ 
  std::string currentName;
  DOMNodeList *x = _variable->getChildNodes();
  DOMNode *holder = x->item(1);

  for (int i = 1; i <= (int)x->getLength(); i++)
  {
    currentName = XMLString::transcode(holder->getNodeName());
    if (boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
    {
      DOMNodeList *y = holder->getChildNodes();
      DOMNode *z = y->item(0);
      z->setNodeValue(XMLString::transcode(value.c_str()));
      break;
    }
    holder = holder->getNextSibling();
    holder = holder->getNextSibling();
  }
}

//---------------------------------------------------------------------------
// Saves VDB changes, releases xerces memory
void VDBFile::close()
{
  _doc->release();
}

//---------------------------------------------------------------------------
//This function writes the current xerces tree to a new XML file
void VDBFile::save()
{
  DOMImplementation *impl = DOMImplementation::getImplementation();
  DOMLSSerializer *theSerializer = 
    ((DOMImplementationLS*)impl)->createLSSerializer();
  XMLCh* toTranscode = theSerializer->writeToString(_doc);
  char* xmlChar = XMLString::transcode(toTranscode);
  std::string transcodedStr;
  transcodedStr = std::string(xmlChar);
  XMLString::release(&xmlChar);
  XMLString::release(&toTranscode);
  theSerializer->release(); 
  ofstream outpt;
  outpt.open(_file.c_str());
  outpt << transcodedStr << "\n";
  outpt.close();
}

//---------------------------------------------------------------------------
// This function opens a given filename and initializes xerces
void VDBFile::open(const std::string file)
{
  //save location
  _file = file;

  //initialize xerces
  try { XMLPlatformUtils::Initialize(); }
  catch (const XMLException& toCatch) 
  {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Error during initialization! :\n"
         << message << "\n";
    XMLString::release(&message);
    return;
  };

  //Create parser
  XercesDOMParser *parser = new XercesDOMParser();
  parser->setValidationScheme(XercesDOMParser::Val_Always);
  parser->setDoNamespaces(true);
  parser->setDoSchema(true);
  parser->setValidationConstraintFatal(true);
  
  //open file
  parser->parse(XMLString::transcode(file.c_str()));

  if (parser->getErrorCount() > 0)
  {
    cerr << "VarDB::VDBFile::open failed to initialize file : " << file << endl;
    cerr << "Error count = " << parser-> getErrorCount() << endl;
    return;
  }
  _doc = parser->getDocument();
  _docRootNode = _doc->getDocumentElement();
  _valid = true;
}

//---------------------------------------------------------------------------
// This function searches through the XML document for an element with a
// given name
VDBVar *VDBFile::search_for_var(const std::string var) const
{
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList* x = _docRootNode->getElementsByTagName(tag);
  DOMNode* varCat = x->item(0);
  
  //search variables for name that matches input var
  DOMNodeList *elems = varCat->getChildNodes();
  DOMNode *holder = varCat->getFirstChild();
  holder = holder->getNextSibling();
  std::string posName = var;

  for (int i = 1;i <= (int)elems->getLength()/2; i++)
  {
    DOMNamedNodeMap *atts = holder->getAttributes();
    DOMNode *name = atts->getNamedItem(XMLString::transcode("name"));


    if(boost::iequals(XMLString::transcode(name->getNodeValue()),var))
    {
      //Node has been found. A pointer is now created pointing to a VDBVar
      //class with _variable initialized as a pointer to the node
      VDBVar *v = new VDBVar(holder);
      return v;
    }
    holder = holder->getNextSibling();
    holder = holder->getNextSibling();
  }
  return NULL;
}

//---------------------------------------------------------------------------
// This function searches through the XML document for an element with a
// given name, correcting for _'s
VDBVar *VDBFile::get_var(const string var) const
{
  VDBVar *rc = NULL;

  // Search first for variable as given.
  if ((rc = search_for_var(var)) == NULL)
  {
    // If not found, try without a suffix (if one exists.
    std::string posName = var.substr(0, var.find_last_of('_'));
    rc = search_for_var(posName);
  }

  return rc;
}

//---------------------------------------------------------------------------
// Returns variable at input index in vardb starting at zero
VDBVar *VDBFile::get_var(int index)
{
  //Get VariableCatalog node as varCat, x is intermediate domNodeList variable
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList *x = _docRootNode->getElementsByTagName(tag);
  DOMNode *varCat = x->item(0);
  
  //search variables for name that matches input var

  DOMNodeList *elems = varCat->getChildNodes();

  DOMNode *holder = varCat->getFirstChild();
  holder = holder->getNextSibling();

  if (index > (int)elems->getLength()/2-1)
  {
    return NULL;
  }
  for(int i = 0; i < index;i++)
  {
    holder = holder->getNextSibling();
    holder = holder->getNextSibling();
  }
  VDBVar *v = new VDBVar(holder);
  return v;
};

//---------------------------------------------------------------------------
// Returns number of variables in vardb
int VDBFile::num_vars() const
{
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList *x = _docRootNode->getElementsByTagName(tag);
  DOMNode* varCat = x->item(0);
  DOMNodeList* elems = varCat->getChildNodes();
  return elems->getLength()/2;
};

