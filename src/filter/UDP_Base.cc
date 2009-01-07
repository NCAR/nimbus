/*
-------------------------------------------------------------------------
OBJECT NAME:	UDP_Base.cc

FULL NAME:	UDP base class for broadcast

ENTRY POINTS:	

DESCRIPTION:	Class to produce 1 per second ascii string to broadcast
		to ground.

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "UDP_Base.h"

#include "nimbus.h"

/* -------------------------------------------------------------------- */
UDP_Base::UDP_Base(int port) : UDP_PORT(port)
{
}

/* -------------------------------------------------------------------- */
std::vector<var_base *> UDP_Base::readFile(const std::string & fileName) const
{
  char	*varList[512];
  std::vector<var_base *> list;

  ReadTextFile(fileName, varList);

  for (int i = 0; varList[i]; ++i)
  {
    char target[BUFSIZ], *p;
    strcpy(target, varList[i]);

    p = strtok(target, ", \t\n");
    while (p)
    {
      int index;

      if ((index = SearchTable(raw, p)) != ERR)
      {
        list.push_back(raw[index]);
      }
      else
      if ((index = SearchTable(derived, p)) != ERR)
      {
        list.push_back(derived[index]);
      }
      else
      {
        char msg[100];
        sprintf(msg, "UDP_Base: Variable [%s] not found, substituting ZERO.\n", p);
        LogMessage(msg);
        list.push_back(0);
      }
      p = strtok(NULL, ", \t\n");
    }
  }

  FreeTextFile(varList);

  return list;
}
