/* -*- mode: C++; c-basic-offset: 2; -*- */


#include "vardb_private.hh"

#include <domx/XML.h>

using domx::xstring;

VDBDictionary::
VDBDictionary(xercesc_forward::DOMNode* dictnode):
  _dictionary(dictnode)
{
};

//---------------------------------------------------------------------------
// Returns number of entries in dictionary
int
VDBDictionary::
num_entries() const
{
  int nelem = 0;
  // Count the definition elements.
  DOMNodeList *children = _dictionary->getChildNodes();

  for (unsigned int i = 0; i < children->getLength(); ++i)
  {
    DOMElement* elem = domx::asElement(children->item(i));
    if (elem && xstring(elem->getTagName()) == "definition")
    {
      ++nelem;
    }
  }
  return nelem;
}


std::string
VDBDictionary::
get_entry(int index)
{
  int ndef = 0;
  DOMNodeList *children = _dictionary->getChildNodes();

  for (unsigned int i = 0; i < children->getLength(); ++i)
  {
    DOMElement* elem = domx::asElement(children->item(i));
    if (elem && xstring(elem->getTagName()) == "definition" && ndef++ == index)
    {
      xstring name;
      domx::getAttribute(elem, "name", &name);
      return name;
    }
  }
  return "";
}


std::string
VDBDictionary::
get_definition(const std::string& target)
{
  DOMNodeList *children = _dictionary->getChildNodes();

  for (unsigned int i = 0; i < children->getLength(); ++i)
  {
    DOMElement* elem = domx::asElement(children->item(i));
    if (elem && xstring(elem->getTagName()) == "definition")
    {
      xstring name;
      domx::getAttribute(elem, "name", &name);
      if (name == target)
      {
	return VDBFile::getTextValue(elem);
      }
    }
  }
  return "";
}

