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

#ifndef __vardb_hh__
#define __vardb_hh__

#include <string>
#include <iostream>
#include <fstream>

#ifndef _vardb_cc_
/*
 * This is a little bit of a hack.  Xerces hides it's actual namespace name
 * because it may not even have a namespace in some configurations, we
 * cannot forward declare any classes in the namespace.  For the purposes
 * of the interface here, we just need binary compatibility, so a pointer
 * is a pointer is a pointer.  Only in vardb.cc are the class members in
 * this file defined to point to the actual classes defined in the xerces
 * headers.
 *
 * Really we want to encapsulate completely the xercesc usage, but that
 * takes more work so can happen later.
 */
namespace xercesc_forward
{
  class DOMNode;
  class DOMDocument;
  class DOMElement;
}
#endif

//========================VDBDictionary======================================
/**
 * Accesses catalog information in vdb
 */
class VDBDictionary
{
public:
  VDBDictionary(xercesc_forward::DOMNode* Root, std::string dictionary_name):
    _docRoot(Root), _dictionary_name(dictionary_name)
  {};

  int
  num_entries() const;

  std::string
  get_entry(int index);

private:
  xercesc_forward::DOMNode* _docRoot;
  std::string _dictionary_name;
};

//===========================================================================
//=============================VDBVAR========================================

/**
 * This class accesses a pointer to a DOMNode variable accessed by get_var
 * in the VDBFile class.  ability to get and set variable attributes such
 * as long_name or units
 */
class VDBVar
{
public:
  /**
   * @param nod points to xerces xml tree  
   */
  VDBVar(xercesc_forward::DOMNode* nod) :
    _variable(nod)
  { }

  /**
   * Return variable element by variable name.
   * @param attr_name: name of attribute requested
   * @returns the requested attribute value
   */
  std::string
  get_attribute(const std::string attr_name) const;

  /**
   * @returns variable name
   */
  std::string
  name();

  /**
   * Return variable element by variable index number.  Used for looping
   * over list.

   * @param attr_name: name of attribute requested
   * @returns the requested attribute value
   */
  std::string
  get_attribute(int index) const;

  /**
   * @param attr_name: name of attribute requested
   * @param value is what to set as the new attribute value
   * see VDBFile::get_var(var)
   */
  void
  set_attribute(const std::string attr_name, const std::string value);

  /**
   *@returns # of attributes
   */
  int
  num_atts() const;

private:
  xercesc_forward::DOMNode* _variable;
};

//===========================================================================
//===========================================================================
//========================VDBFile============================================
/**
 * This class is used to access VDB.xml. Ability to open file, initiate
 * VDBVar class with pointer to Xerces DOMNode with that variable node,
 * save changes by rewriting XML, and close off access to VDB.xml
 */
class VDBFile
{
public:
  VDBFile(const std::string file) : _valid(false) { open(file); }

  ~VDBFile() { close(); }

  /**
   * @param file[] is filename to open containing vardb xml. Vardb
   * references a schema, which is important.
   */
  void
  open(const std::string file);

  /**
   * severs contact with xerces xml tree
   */
  void
  close();

  // file validation check
  bool
  is_valid() const
  { 
    return _valid;
  }

  /**
   * writes xerces tree to xml file
   */
  void
  save();

  /**
   * Gets variable node from xml tree.  Caller is responsible for freeing
   * memory.
   * @param variable name, not case-sensitive
   * @returns VDBVar class pointer
   */
  VDBVar*
  get_var(const std::string var) const;

  /**
   * @param variable position in varDB
   * @returns VDBVar class pointer
   */
  VDBVar*
  get_var(int index);

  /**
   *@returns # of attributes
   */
  int
  num_vars() const;

  VDBDictionary*
  get_dictionary(const std::string dictionary_name) const;

private:
  VDBVar * search_for_var(const std::string var) const;
  bool _valid;
  xercesc_forward::DOMElement* _docRootNode;
  xercesc_forward::DOMDocument* _doc;
  std::string _file;
};

#endif
