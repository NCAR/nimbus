#include <vector>
#include <string>
#include <iostream>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace std;
using namespace xercesc;

#ifndef __vardb_h__
#define __vardb_h__
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
//===============================================================================
//=============================VDBVAR============================================
//This class accesses a pointer to a DOMNode variable accessed by get_var in the VDBFile class.
//ability to get and set variable attributes such as long_name or units
class VDBVar
{
public:
  VDBVar(DOMNode* nod) : _variable(nod) { }

  std::string get_attribute(const std::string attr_name) const;

  void set_attribute(const std::string attr_name, const std::string value);

private:
  DOMNode* _variable;
};

//==============================================================================
//========================VDBFile===============================================
//This class is used to access VDB.xml. Ability to open file, initiate VDBVar class with pointer
//to Xerces DOMNode with that variable node, save changes by rewriting XML, and close off access to VDB.xml
class VDBFile
{
public:
  VDBFile() : _valid(false) { }

  void open(const char file[]);
  void close();
  bool is_valid() const { return _valid; };
  void save();
  VDBVar get_var(const std::string var) const ;

private:
  bool _valid;
  DOMElement* _docRootNode;
  DOMDocument* _doc;
};
#endif
