/* -*- mode: C++; c-basic-offset: 2; -*- */

#ifndef __vardb_private_hh__
#define __vardb_private_hh__

// Include the vardb.hh interface definition but use the real xerces
// namespace.

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

using xercesc::XMLString;
#define _vardb_cc_
namespace xercesc_forward = xercesc;
#include "vardb.hh"

using namespace xercesc;
using namespace std;


#endif // __vardb_private_hh__
