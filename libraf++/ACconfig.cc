/*
-------------------------------------------------------------------------
OBJECT NAME:	ACconfig.cc

FULL NAME:	

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2004
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "ACconfig.h"

using namespace libraf;

const std::string ACconfig::NO_KEY = "** No key for this tail number **";
const std::string ACconfig::NO_TAILNUM = "** Non-existent tail number **";
const std::string ACconfig::NO_SERVER = "** No matching server **";

/* -------------------------------------------------------------------- */
ACconfig::ACconfig(std::string fileName)
{
  int	cnt;
  char	buffer[512], *p;
  FILE	*fp;

  if ((fp = fopen(fileName.c_str(), "r")) == NULL)
  {
    std::cerr << "ACconfig: Can't open " << fileName << ".\n";
    exit(1);
  }


  std::vector<std::string> _file;

  /* Read file and strip out comments.
   */
  for (cnt = 0; fgets(buffer, 512, fp); )
  {
    if (buffer[0] == COMMENT)
      continue;

    buffer[strlen(buffer)] = '\0';	/* Ditch newline	*/

    if ( (p = strchr(buffer, COMMENT)) )
      *p = '\0';
    else
      p = &buffer[strlen(buffer)];

    while ( isspace(*(--p)) )	/* Strip trailing white-space	*/
      *p = '\0';

    if ( isspace(buffer[0]) )	/* Belongs to previous line	*/
      _file.back().append(buffer);
    else			/* Fresh line		*/
      _file.push_back(std::string(buffer));
  }

  fclose(fp);


  for (int i = 0; i < _file.size(); ++i)
  {
    if (_file[i].compare(0, 5, "START") == 0)
    {
      int idx = _file[i].find_first_not_of(" \t", 5);
      std::string  thisTail = _file[i].substr(idx);

      _tailNumList.push_back(thisTail);

      for (++i; i < _file.size(); ++i)
      {
        if (_file[i].compare("END_AIRCRAFT") == 0)
          break;

        int end = _file[i].find_first_of(" \t");
        std::string	key = thisTail + "." + _file[i].substr(0, end);
        
        int start = _file[i].find_first_not_of(" \t", end);
        std::string value = _file[i].substr(start);

        _attrs[key] = value;
      }
    }
  }

}	/* END ACCONFIG */

/* -------------------------------------------------------------------- */
std::string ACconfig::GetParameter(std::string tailNumber, std::string target)
{
  int	i;

  for (i = 0; i < _tailNumList.size(); ++i)
    if (_tailNumList[i] == tailNumber)
      break;

  if (i == _tailNumList.size())
    return(NO_TAILNUM);

  std::string key = tailNumber + "." + target;

  std::map<std::string, std::string>::const_iterator iter = _attrs.find(key);
  return((iter == _attrs.end()) ? NO_KEY : iter->second);

}	/* END GETPARAMETER */

/* -------------------------------------------------------------------- */
std::string ACconfig::LocateMyTailNumber(std::string serverName)
{
  for (std::map<std::string,std::string>::iterator i = _attrs.begin(); i != _attrs.end(); ++i)
  {
    std::string key = i->first;
    std::string value = i->second;

    if (value == serverName)
    {
      return(key.substr(0, key.find_first_of('.')));
    }
  } 

  return(NO_SERVER);

}	/* END LOCATEMYTAILNUMBER */

/* END ACCONFIG.CC */
