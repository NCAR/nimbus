#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace std;
using namespace xercesc;

#ifndef __vardb_h__
#define __vardb_h__
#include <stdio.h>
#include<iostream>
#include<map>
#include<fstream>
#include <boost/algorithm/string.hpp>
//===============================================================================
//=============================VDBVAR============================================
//This class accesses a pointer to a DOMNode variable accessed by get_var in the VDBFile class.
//ability to get and set variable attributes such as long_name or units
class VDBVar
{
public:
  VDBVar(DOMNode* nod)
  {
    _variable=nod;
  };

  std::string get_attribute(const std::string attr_name) const
		{ 
                  DOMNodeList* x=_variable->getChildNodes();
                  DOMNode* holder=x->item(1);
                  for(int i=1;i<=x->getLength();i++)
                  {
                    if(boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
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

  void set_attribute(const std::string attr_name, const std::string value)
		{ 
                  std::string currentName;
                  DOMNodeList* x=_variable->getChildNodes();
                  DOMNode* holder=x->item(1);
                  for(int i=1;i<=x->getLength();i++)
                  {
                    currentName=XMLString::transcode(holder->getNodeName());
                    if(boost::iequals(XMLString::transcode(holder->getNodeName()),attr_name))
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
private:
//  std::map<std::string, std::string> _attrs;
  DOMNode* _variable;
};
//------------------------------------------------------------------------------
//==============================================================================
//========================VDBFile===============================================
//This class is used to access VDB.xml. Ability to open file, initiate VDBVar class with pointer
//to Xerces DOMNode with that variable node, save changes by rewriting XML, and close off access to VDB.xml
class VDBFile
{
public:

  VDBFile() : _valid(false) 
{ 

};
  void open(const char file[]);
  void close();
  bool is_valid() { return _valid; };
  void save();
  VDBVar get_var(const string var);
private:
  bool _valid;
  DOMElement* _docRootNode;
  DOMDocument* _doc;
};
//---------------------------------------------------------------------------------------
//Releases xerces memory
void VDBFile::close()
{
  save();
  _doc->release();
};
//---------------------------------------------------------------------------------------
//This function writes the xerces document to a new XML file
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
   outpt.open("VDB.xml");
   outpt<<transcodedStr<<"\n";
   outpt.close();
};
//---------------------------------------------------------------------------------------
//This function opens a given filename and initializes xerces
void VDBFile::open(const char file[])
{
  //initialize xerces
  try { XMLPlatformUtils::Initialize(); }
  catch (const XMLException& toCatch) 
    {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Error during initialization! :\n"
         << message << "\n";
    XMLString::release(&message);
    };

  //Create parser
  XercesDOMParser* parser = new XercesDOMParser();
  parser->setValidationScheme(XercesDOMParser::Val_Always);
  parser->setDoNamespaces(true);
  parser->setDoSchema(true);
  parser->setValidationConstraintFatal(true);
  
  //open file
  parser->parse(XMLString::transcode("VDB.xml"));

  if (parser-> getErrorCount()>0)
  {
    printf("ERROR 1 READING XML\n");
  }
  _doc = parser->getDocument();
  _docRootNode = _doc->getDocumentElement();
};
//---------------------------------------------------------------------------------------
//This function searches through the XML document for an element with a given name

VDBVar VDBFile::get_var(const string var)
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
      cout<<"Node is "<<XMLString::transcode(name->getNodeValue())<<"\n";
      
      //Node has been found. A pointer is now created pointing to a VDBVar class 
      //with _variable initialized as a pointer to the node
      VDBVar v =holder;

      return v;
    }
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
};

//==================================================================================

#endif
