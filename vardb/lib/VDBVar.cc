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

#include <boost/algorithm/string.hpp>

#include "domx/XML.h"
using domx::xstring;

const std::string VDBVar::UNITS = "units";
const std::string VDBVar::LONG_NAME = "long_name";
const std::string VDBVar::IS_ANALOG = "is_analog";
const std::string VDBVar::VOLTAGE_RANGE = "voltage_range";
const std::string VDBVar::DEFAULT_SAMPLE_RATE = "default_sample_rate";
const std::string VDBVar::MIN_LIMIT = "min_limit";
const std::string VDBVar::MAX_LIMIT = "max_limit";
const std::string VDBVar::CATEGORY = "category";
const std::string VDBVar::MODULUS_RANGE = "modulus_range";
const std::string VDBVar::DERIVE = "derive";
const std::string VDBVar::DEPENDENCIES = "dependencies";
const std::string VDBVar::STANDARD_NAME = "standard_name";
const std::string VDBVar::REFERENCE = "reference";


VDBVar::
VDBVar(xercesc_forward::DOMElement* node) :
  _variable(node)
{
}


std::string
VDBVar::
name() const
{
  DOMNamedNodeMap* atts = _variable->getAttributes();
  DOMNode*  name = atts->getNamedItem(xstring("name"));
  return xstring(name->getNodeValue());
}


std::string
VDBVar::
get_attribute(int index) const
{
  xstring name;
  DOMNodeList* x = _variable->getChildNodes();
  int nelem = 0;

  for (unsigned int i = 0; i < x->getLength(); ++i)
  {
    // Only ELEMENT children are attributes.
    DOMElement* elem = domx::asElement(x->item(i));
    if (!elem)
    {
      continue;
    }
    if (nelem == index)
    {
      name = elem->getTagName();
      break;
    }
    ++nelem;
  }
  return name;
}


int
VDBVar::
num_atts() const
{
  DOMNodeList* x = _variable->getChildNodes();
  int nelem = 0;

  for (unsigned int i = 0; i < x->getLength(); ++i)
  {
    // Only ELEMENT children are attributes.
    if (domx::asElement(x->item(i)))
    {
      ++nelem;
    }
  }
  return nelem;
}


std::string
VDBVar::
get_attribute(const std::string& attr_name, const std::string& dfault) const
{ 
  DOMElement* attnode = domx::findElement(_variable, attr_name);
  std::string answer = dfault;

  if (attnode)
  {
    answer = VDBFile::getTextValue(attnode);
  }
  return answer;
}


bool
VDBVar::
has_attribute(const std::string& name)
{
  return bool(domx::findElement(_variable, name));
}


template<>
void
VDBVar::
set_attribute(const std::string& attr_name, const std::string& value)
{ 
  DOMDocument* doc = _variable->getOwnerDocument();
  DOMElement* attnode = domx::findElement(_variable, attr_name);
  if (!_variable->getFirstChild())
  {
    _variable->appendChild(VDBFile::newlineNode(doc, 2));
  }
  if (!attnode)
  {
    attnode = doc->createElement(xstring(attr_name));
    _variable->appendChild(VDBFile::indentNode(doc));
    _variable->appendChild(attnode);
    _variable->appendChild(VDBFile::newlineNode(doc, 2));
  }
  // Replace any current value in this node.
  domx::removeChildren(attnode);
  attnode->appendChild(doc->createTextNode(xstring(value)));
}

