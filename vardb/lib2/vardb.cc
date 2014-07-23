#include "vardb.h"

/* -*- mode: C++; c-basic-offset: 2; -*-
 * -------------------------------------------------------------------------
 *  OBJECT NAME:    vardb.cc
 *
 *  FULL NAME:      VarDB API
 *
 *  TYPE:           API
 *
 *  DESCRIPTION:    This is an API to edit Vardb xml.
 *
 *  COPYRIGHT:      University Corporation for Atmospheric Research, 2014
 *  -------------------------------------------------------------------------
 *  */

//---------------------------------------------------------------------------------------
//Returns text associated with attribute, given by index
//input: index- attribute's position in vdb variable
//output-attribute value
std::string VDBVar::get_attribute(int index)const
{
  DOMNodeList* x=_variable->getChildNodes();
  DOMNode* holder=x->item(1);
  for (int i=0;i<index;i++)
  {
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  } 
  DOMNodeList* y=holder->getChildNodes();
  DOMNode* z=y->item(0);
  std::string answer=XMLString::transcode(z->getNodeValue());
  return answer;
};
//---------------------------------------------------------------------------------------
//Returns number of attributes associated sellected variable
int VDBVar::num_atts()
{
  DOMNodeList* x=_variable->getChildNodes();
  return x->getLength()/2;
};
//---------------------------------------------------------------------------------------
//Returns text associated with a variable's attribute
//input: attr_name: attribute name, such as units, name, or longname
//output: Text associated with variable's attribute
std::string VDBVar::get_attribute(const std::string attr_name) const
{ 
  DOMNodeList* x=_variable->getChildNodes();
  DOMNode* holder=x->item(1);

  for (int i=1; i <= x->getLength(); i++)
  {
    if (boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
    {
      DOMNodeList* y=holder->getChildNodes();
      DOMNode* z=y->item(0);
      std::string answer=XMLString::transcode(z->getNodeValue());
      return answer;
    }
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }

  return NULL;
};
//---------------------------------------------------------------------------------------
//Sets a variable's information for specified attribute
//input: attr_name: attribute name, such as units, name, or longname
void VDBVar::set_attribute(const std::string attr_name, const std::string value)
{ 
  std::string currentName;
  DOMNodeList* x=_variable->getChildNodes();
  DOMNode* holder=x->item(1);

  for (int i=1; i <= x->getLength(); i++)
  {
    currentName=XMLString::transcode(holder->getNodeName());
    if (boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
    {
      DOMNodeList* y=holder->getChildNodes();
      DOMNode* z=y->item(0);
      z->setNodeValue(XMLString::transcode(value.c_str()));
      break;
    }
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
};
//=======================================================================================
//---------------------------------------------------------------------------------------
//Saves VDB changes, releases xerces memory
void VDBFile::close()
{
  save();
  _doc->release();
};

//---------------------------------------------------------------------------------------
//This function writes the current xerces tree to a new XML file
void VDBFile::save()
{
  DOMImplementation* impl = DOMImplementation::getImplementation();
  DOMLSSerializer   *theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
  XMLCh* toTranscode = theSerializer->writeToString(_doc);
  char* xmlChar = XMLString::transcode(toTranscode);
  std::string transcodedStr;
  transcodedStr = string(xmlChar);
  XMLString::release(&xmlChar);
  XMLString::release(&toTranscode);
  theSerializer->release(); 
  ofstream outpt;
  outpt.open(_file.c_str());
  outpt<<transcodedStr<<"\n";
  outpt.close();
};

//---------------------------------------------------------------------------------------
//This function opens a given filename and initializes xerces
void VDBFile::open(const std::string file)
{
  //save location
  _file=file;

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
  XercesDOMParser* parser = new XercesDOMParser();
  parser->setValidationScheme(XercesDOMParser::Val_Always);
  parser->setDoNamespaces(true);
  parser->setDoSchema(true);
  parser->setValidationConstraintFatal(true);
  
  //open file
  parser->parse(XMLString::transcode(file.c_str()));

  if (parser-> getErrorCount()>0)
  {
    std::cerr << "ERROR 1 READING XML\n";
    return;
  }
  _doc = parser->getDocument();
  _docRootNode = _doc->getDocumentElement();
  _valid = true;
};

//---------------------------------------------------------------------------------------
//This function searches through the XML document for an element with a given name
VDBVar *VDBFile::get_var(const string var) const
{
  //Get VariableCatalog node as varCat, x is intermediate domNodeList variable
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList* x=_docRootNode->getElementsByTagName(tag);
  DOMNode* varCat=x->item(0);
  
  //search variables for name that matches input var

  DOMNodeList* elems=varCat->getChildNodes();

  DOMNode* holder=varCat->getFirstChild();
  holder=holder->getNextSibling();

  DOMNamedNodeMap* atts;
  DOMNode* name;
  for (int i=1;i<=elems->getLength()/2;i++)
  {
    atts=holder->getAttributes();
    name=atts->getNamedItem(XMLString::transcode("name"));


    if(boost::iequals(XMLString::transcode(name->getNodeValue()),var))
    {
      //Node has been found. A pointer is now created pointing to a VDBVar class 
      //with _variable initialized as a pointer to the node
      VDBVar *v = new VDBVar(holder);
      return v;
    }
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
};
//---------------------------------------------------------------------------------------
//Returns variable at input index in vardb starting at zero
VDBVar *VDBFile::get_var(int index)
{
  //Get VariableCatalog node as varCat, x is intermediate domNodeList variable
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList* x=_docRootNode->getElementsByTagName(tag);
  DOMNode* varCat=x->item(0);
  
  //search variables for name that matches input var

  DOMNodeList* elems=varCat->getChildNodes();

  DOMNode* holder=varCat->getFirstChild();
  holder=holder->getNextSibling();

  DOMNamedNodeMap* atts;
  DOMNode* name;
  for(int i=0;i<index;i++)
  {
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
  VDBVar *v = new VDBVar(holder);
  return v;
};
//---------------------------------------------------------------------------------------
//Returns number of variables in vardb
int VDBFile::num_vars()
{
  XMLCh *tag = XMLString::transcode("variableCatalog");
  DOMNodeList* x=_docRootNode->getElementsByTagName(tag);
  DOMNode* varCat=x->item(0);
  DOMNodeList* elems=varCat->getChildNodes();
  return elems->getLength()/2;
};
