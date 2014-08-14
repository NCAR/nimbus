/* -*- mode: C++; c-basic-offset: 2; -*-
 * -------------------------------------------------------------------------
 *  OBJECT NAME:    vardb.h
 *
 *  FULL NAME:      VarDB API header file
 *
 *  TYPE:           API header
 *
 *  DESCRIPTION:    This is the header for an API to edit Vardb xml.
 *
 *  COPYRIGHT:      University Corporation for Atmospheric Research, 2014
 *  -------------------------------------------------------------------------
 */

#ifndef __vardb_h__
#define __vardb_h__

#include <string>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace std;
using namespace xercesc;

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

//========================VDBDictionary=========================================
/**
 * Accesses catalog information in vdb
 */
class VDBDictionary
{
   public:
      VDBDictionary(DOMNode* Root, string dictionary_name): _docRoot(Root), _dictionary_name(dictionary_name) {};
      int num_entries() const;
      std::string get_entry(int index);

   private:
      DOMNode* _docRoot;
      std::string _dictionary_name;

};
//===============================================================================
//=============================VDBVAR============================================

/**
 * This class accesses a pointer to a DOMNode variable accessed by get_var in the VDBFile class.
 * ability to get and set variable attributes such as long_name or units
 */
class VDBVar
{
public:
  /**
   * @param nod points to xerces xml tree  
   */
  VDBVar(DOMNode* nod) : _variable(nod) { }

  /**
   * Return variable element by variable name.
   * @param attr_name: name of attribute requested
   * @returns the requested attribute value
   */
  std::string get_attribute(const std::string attr_name) const;

  /**
   * @returns variable name
   */
  std::string name();

  /**
   * Return variable element by variable index number.  Used for looping ove list.
   * @param attr_name: name of attribute requested
   * @returns the requested attribute value
   */
  std::string get_attribute(int index) const;

  /**
   * @param attr_name: name of attribute requested
   * @param value is what to set as the new attribute value
   * see VDBFile::get_var(var)
   */
  void set_attribute(const std::string attr_name, const std::string value);

  /**
   *@returns # of attributes
   */
  int num_atts() const;

private:
  DOMNode* _variable;
};

//==============================================================================
//==============================================================================
//========================VDBFile===============================================
/**
 * This class is used to access VDB.xml. Ability to open file, initiate VDBVar class
 * with pointer to Xerces DOMNode with that variable node, save changes by rewriting
 * XML, and close off access to VDB.xml
 */
class VDBFile
{
public:
  VDBFile(const std::string file) : _valid(false) { open(file); }

  ~VDBFile() { close(); }

  /**
   * @param file[] is filename to open containing vardb xml. Vardb references a schema, 
   * which is important.
   */
  void open(const std::string file);

  /**
   * severs contact with xerces xml tree
   */
  void close();

  // file validation check
  bool is_valid() const { return _valid; };

  /**
   * writes xerces tree to xml file
   */
  void save();

  /**
   * Gets variable node from xml tree.  Caller is responsible for freeing memory.
   * @param variable name, not case-sensitive
   * @returns VDBVar class pointer
   */
  VDBVar *get_var(const std::string var) const ;

  /**
   * @param variable position in varDB
   * @returns VDBVar class pointer
 */
  VDBVar * get_var(int index);


  /**
 *@returns # of attributes
 */
  int num_vars() const;

  VDBDictionary * get_dictionary(const std::string dictionary_name) const;

private:
  VDBVar * search_for_var(const std::string var) const;
  bool _valid;
  DOMElement* _docRootNode;
  DOMDocument* _doc;
  std::string _file;
};

#endif
