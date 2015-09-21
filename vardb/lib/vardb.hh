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
#include <sstream>
#include <vector>

#ifndef _vardb_cc_
/**
 * The xercesc_forward namespace is a little bit of a hack.  Xerces hides
 * it's actual namespace name because it may not even have a namespace in
 * some configurations, so we cannot forward declare any classes in the
 * namespace.  For the purposes of the interface here, we just need binary
 * compatibility, so a pointer is a pointer is a pointer.  Only in
 * vardb_private.h are the class members in this file defined to point to
 * the actual classes defined in the xerces headers.
 *
 * Really we want to encapsulate completely the xercesc usage, but that
 * takes more work so can happen later.
 */
namespace xercesc_forward
{
  class DOMNode;
  class DOMDocument;
  class DOMElement;
  class InputSource;
  class XercesDOMParser;
}
#endif


class VDBVar;
class VDBDictionary;


/**
 * This is the top-level class for reading and writing a variable database
 * XML file.  It encapsulates the document model and provides access to the
 * variables through VDBVar instances.  The dictionary can be manipulated
 * through a VDBDictionary pointer, while the standard names and categories
 * are accessed and replaced in whole groups.
 *
 * An existing XML document can be opened using the VDBFile() constructor
 * or else the open() method, while the create() method creates an empty
 * document.
 * 
 * The VDBFile maintains the variable database in the document model, and
 * it modifies the document directly when the database is modified.  As
 * much as possible the original document is preserved, including comments
 * and whitespace text nodes.  For empty databases created from scratch, or
 * when new elements are added to an existing document, the indentation is
 * inserted also using a hard-coded number of spaces per indentation level.
 *
 * Since standard names and categories are modified as a whole, meaning it
 * is only possible to insert them as one complete list, it is not
 * currently possible to preserve comments or spacing within those sets.
 */
class VDBFile
{
public:

  /**
   * The default constructor with default arguments does not allocate any
   * resources.  A document must be created with create() or loaded with
   * open() or load() to begin retrieving or adding variables.  If a file
   * path is provided, it is opened with open().  The success of the open
   * can be tested with is_valid().
   **/
  VDBFile(const std::string& filepath = "");

  /**
   * Close the file and release all XML resources.
   **/
  ~VDBFile();

  /**
   * Create an empty XML document ready for variables and dictionary
   * entries to be added.
   **/
  void
  create();

  /**
   * Open an existing variable database XML file and load it, replacing the
   * current document.
   **/
  void
  open(const std::string& filepath);

  /**
   * Load the variable database XML from the given input stream, replacing
   * the current document, optionally assigning a URL to the document.
   **/
  void
  load(const std::string& in, const std::string& file = "");

  /**
   * Close the current XML document and release its resources.
   */
  void
  close();

  /**
   * Return false if there currently is not valid document model, meaning
   * create() has not been called or an open() or load() has not succeeded.
   * The rest of the accessor methods are invalid until a document model
   * exists.
   **/
  bool
  is_valid() const
  { 
    return _doc;
  }

  /**
   * Write the variable database to an XML document at the specified path.
   * @see save(std::ostream&).
   */
  void
  save(const std::string& filepath);

  /**
   * Write the variable database as an XML document to the output stream.
   * The native encoding is used, and so it is left unspecified in the XML
   * header.  Further, no validation schema is specified in the XML, since
   * vdb files are validated against an in-memory schema supplied by the
   * library rather than an external schema.  It is still possible to
   * specify a schema in the XML file for validating with other parsers,
   * but that will be overridden when the XML file is opened by VDBFile.
   * Perhaps this method should include the schema in the XML for use by
   * other parsers, but for now it is omitted.
   */
  void
  save(std::ostream& out);

  /**
   * Retrieve a VDBVar variable instance from this document matching the
   * given @p vname.  Unlike search_var(), the name must match exactly,
   * including case.  The pointer belongs to this VDBFile instance: it
   * should not be deleted and will be invalid once the current VDBFile
   * document is closed.  All modifications to the returned variable
   * through the VDBVar object modify the current VDBFile document.  All
   * requests for the same variable name return the same VDBVar instance.
   *
   * @param vname variable name
   * @returns VDBVar class pointer
   */
  VDBVar*
  get_var(const std::string& vname);

  /**
   * Same as get_var(), except if no variable exists with the given name,
   * then this method removes any apparent suffix (from the last underscore
   * to the end) and searches again, using case insensitive name
   * comparisons.
   **/
  VDBVar*
  search_var(const std::string& vname);

  /**
   * Add a variable to this document using the given name and return it.
   * If the variable name already exists, then the existing variable is
   * returned instead.
   **/
  VDBVar*
  add_var(const std::string& vname);

  /**
   * @param index Index position in varDB, first position is 0.
   * @returns VDBVar class pointer
   */
  VDBVar*
  get_var(int index);

  /**
   *@returns number of variables
   */
  int
  num_vars() const;

  /**
   * Return a pointer to this document's dictionary.  Any modifications
   * through the VDBDictionary are made to the current VDBFile document.
   * The VDBDictionary pointer is owned by this VDBFile and is only valid
   * until the document is closed.
   */
  VDBDictionary*
  get_dictionary();

  typedef std::vector<std::string> standard_names_type;

  /**
   * Get and set standard names as a whole unit, since it is not a large
   * number of items and will usually be accessed or changed altogether.
   **/
  standard_names_type
  get_standard_names();

  void
  set_standard_names(const standard_names_type& names);

  typedef std::vector<std::string> categories_type;

  /**
   * Get and set categories as a whole unit, since it is not a large number
   * of items and will usually be accessed or changed altogether.
   **/
  categories_type
  get_categories();

  /**
   * Replace the current list of categories in this document, preserving
   * the order.
   **/
  void
  set_categories(const categories_type& names);

  /**
   * Read the categories from a Categories file in RAF text format.
   **/
  static categories_type
  readCategories(const std::string& filepath);

  /**
   * Read the standard names from a StandardNames file in RAF text format.
   **/
  static standard_names_type
  readStandardNames(const std::string& filepath);

  /** 
   * String of spaces for indenting each XML element level.
   */
  static const std::string INDENT;

  /** 
   * Return a string with a newline and the right indentation to the given
   * level, where level 0 is no indentation.
   */
  static std::string
  nextLineIndent(int level);

  static xercesc_forward::DOMNode*
  newlineNode(xercesc_forward::DOMDocument* doc, int level);

  static xercesc_forward::DOMNode*
  indentNode(xercesc_forward::DOMDocument* doc);

  /**
   * VDBFile components use this method to retrieve a text value from the
   * child nodes of an element, such as variable attributes and
   * definitions.  The returned text value is trimmed of leading and
   * trailing whitespace.
   **/
  static std::string
  getTextValue(xercesc_forward::DOMNode* element);

  /**
   * search_var() uses this function to trim a suffix from a variable name.
   **/
  static std::string
  trimVariableSuffix(const std::string& vname);

private:

  void
  parse(const std::string& file, xercesc_forward::InputSource& source);

  VDBVar* 
  search_for_var(const std::string& var, bool expanded=false);

  xercesc_forward::DOMDocument* _doc;
  std::string _file;
  VDBDictionary* _dictionary;

  // Need to keep the parser around because it owns the documents it
  // parses.
  xercesc_forward::XercesDOMParser* _parser;

  void
  load_variables();

  // Keep the vector of VDBVar instances for the current document.
  std::vector<VDBVar*> _variables;
};


//========================VDBDictionary======================================
/**
 * Accesses catalog information in vdb
 */
class VDBDictionary
{
public:
  /**
   * Return the number of dictionary entries.
   **/
  int
  num_entries() const;

  /**
   * Return the name of the definition at @p index.
   **/
  std::string
  get_entry(int index);

  /**
   * Return the definition for @p name.
   **/
  std::string
  get_definition(const std::string& name);

private:

  VDBDictionary(xercesc_forward::DOMNode* dictnode);

  xercesc_forward::DOMNode* _dictionary;

  // Only a VDBFile is allowed to create instances of a VDBDictionary.
  friend class VDBFile;
};


/**
 * A VDBVar instance accesses and manipulates a variable element in a
 * VDBFile document.  The methods can get and set variable attributes such
 * as long_name and units.
 *
 * There are several standard attributes for variables, and these are
 * defined here to use with the set_attribute() and get_attribute()
 * methods.  Note that an 'attribute' of a variable is actually a child
 * element in the XML document, and not an XML attribute.
 **/
class VDBVar
{
public:

  static const std::string UNITS;
  static const std::string LONG_NAME;
  static const std::string IS_ANALOG;
  static const std::string VOLTAGE_RANGE;
  static const std::string DEFAULT_SAMPLE_RATE;
  static const std::string MIN_LIMIT;
  static const std::string MAX_LIMIT;
  static const std::string CATEGORY;
  static const std::string MODULUS_RANGE;
  static const std::string DERIVE;
  static const std::string DEPENDENCIES;
  static const std::string STANDARD_NAME;
  static const std::string REFERENCE;

  /**
   * Return value of variable attribute named @p attr_name, or else return
   * @p value if no such attribute exists.
   *
   * @param attr_name: name of attribute requested
   * @param value: a default value to return if the attribute is not present
   * @returns the value of the requested attribute or the default
   */
  std::string
  get_attribute(const std::string& attr_name,
		const std::string& value = "") const;

  /**
   * @returns variable name
   */
  std::string
  name() const;

  /**
   * Return the name of the variable attribute at @p index, starting at 0.
   * Used for looping over the attribute list.  The name can then be used
   * to retrieve a value with get_attribute_value().
   *
   * @returns the requested attribute name
   */
  std::string
  get_attribute(int index) const;

  /**
   * Return true if this attribute exists for this variable.
   **/
  bool
  has_attribute(const std::string& name);

  /**
   * Set a value for the attribute named @p attr_name.  Non-string types
   * such as boolean and integer will be converted for representation in
   * XML.
   *
   * @param attr_name: name of attribute requested
   * @param value is what to set as the new attribute value
   * see VDBFile::get_var(var)
   */
  template <typename T>
  void
  set_attribute(const std::string& attr_name, const T& value);

  /**
   * Return the value of the attribute named @p attr_name converted from
   * the XML representation to type T.  If the attribute does not exist,
   * then @p dfault is returned instead.
   **/
  template <typename T>
  T
  get_attribute_value(const std::string& attr_name, const T& dfault = T());

  /**
   *@return Number of attributes for this variable.
   */
  int
  num_atts() const;

private:

  /**
   * @param nod points to xerces xml tree  
   */
  VDBVar(xercesc_forward::DOMElement* node);

  xercesc_forward::DOMElement* _variable;

  friend class VDBFile;
};

template <typename T>
inline void
VDBVar::
set_attribute(const std::string& attr_name, const T& value)
{
  std::ostringstream oss;
  oss << value;
  set_attribute(attr_name, oss.str());
}

template <>
inline void
VDBVar::
set_attribute(const std::string& attr_name, const bool& value)
{
  if (value)
    set_attribute(attr_name, "true");
  else
    set_attribute(attr_name, "false");
}

template <>
void
VDBVar::
set_attribute(const std::string& attr_name, const std::string& value);


template <typename T>
inline T
VDBVar::
get_attribute_value(const std::string& attr_name, const T& dfault)
{
  if (!has_attribute(attr_name))
  {
    return dfault;
  }
  T value;
  std::string text = get_attribute(attr_name);
  std::istringstream iss(text);
  iss >> value;
  return value;
}


template <>
inline bool
VDBVar::
get_attribute_value(const std::string& attr_name, const bool& dfault)
{
  if (!has_attribute(attr_name))
  {
    return dfault;
  }
  std::string text = get_attribute(attr_name);
  return bool(text == "true");
}


template <>
inline std::string
VDBVar::
get_attribute_value(const std::string& attr_name, const std::string& dfault)
{
  return get_attribute(attr_name, dfault);
}


//===========================================================================
#endif
