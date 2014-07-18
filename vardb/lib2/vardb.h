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
                  //holder=x->item(2);
                  for(int i=1;i<=x->getLength();i++)
                  {
                    if(XMLString::transcode(holder->getNodeName())==attr_name)
                    {
                      DOMNodeList* y=holder->getChildNodes();
                      DOMNode* z=y->item(0);
                      std::string answer=XMLString::transcode(z->getNodeValue());
                      return answer;
                    }
                    holder=holder->getNextSibling();
                    holder=holder->getNextSibling();
                  }
                  return "Hello Julian\n";
                };

  void set_attribute(const std::string attr_name, const std::string value)
		{ _attrs[attr_name] = value; } ;

private:
  std::map<std::string, std::string> _attrs;

  DOMNode* _variable;
};
//------------------------------------------------------------------------------
//===============================================================================
//This class is used to access VDB.xml
class VDBFile
{
public:

  VDBFile() : _valid(false) 
{ 

};

  void open(const char file[]);
  void close();
  bool is_valid() { return _valid; };

  void save(char* var);

  //VDBVar* get_var(const std::string variable);
  VDBVar get_var(const string var);
  
  //should eventually return DOMElement* element  

private:
  bool _valid;
  DOMElement* _docRootNode;
  DOMDocument* _doc;
};

//---------------------------------------------------------------------------------------
//This function opens a given filename and initializes xerces
void VDBFile::open(const char file[])
{
  printf("opening %s\n",file);

  //Made using tutorial http://blog.f85.net/2012/02/xerces-c-tutorial.html


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
  //parser->parse(XMLString::transcode(file.c_str()));
  parser->parse(XMLString::transcode("VDB.xml"));

  if (parser-> getErrorCount()>0)
  {
    printf("ERROR 1 READING XML\n");
  }

  _doc = parser->getDocument();

  if (parser-> getErrorCount()>0)
  {
    printf("ERROR 2 READING XML\n");
  }

  _docRootNode = _doc->getDocumentElement();

  if (parser-> getErrorCount()>0)
  {
    printf("ERROR 3 READING XML\n");
  }
};
//---------------------------------------------------------------------------------------
//This function searches through the XML document for an element with a given name

VDBVar VDBFile::get_var(const string var)
{
  cout<<"finding "<<var<<"\n";

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


    if(XMLString::transcode(name->getNodeValue())==var)
    {
      cout<<"Node is "<<XMLString::transcode(name->getNodeValue())<<"\n";
      
      //Node has been found. A pointer is now created pointing to a VDBVar class 
      //with _variable initialized as a pointer to the node

      VDBVar v =holder;

      return v;
    }

  //  cout<<"Current Node: "<<XMLString::transcode(name->getNodeValue())<<" and i is "<<i<<"/"<<elems->getLength()/2<<"\n";
  
    holder=holder->getNextSibling();
    holder=holder->getNextSibling();
  }
};

//==================================================================================

#endif
