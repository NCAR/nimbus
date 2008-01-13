/*
-------------------------------------------------------------------------
OBJECT NAME:	Hex.cc

FULL NAME:	

ENTRY POINTS:	Update()

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "Hex.h"

/* -------------------------------------------------------------------- */
Hex::Hex(const Widget parent) : TextWindow(parent, "hex")
{

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void Hex::Update(size_t nBuffs, P2d_rec sets[])
{
  Clear();

  /* Title */
  strcpy(buffer, "      ");
  for (size_t j = 0; j < nBuffs; ++j)
    sprintf(&buffer[strlen(buffer)], "   %s    ", (char *)&sets[j].id);

  strcat(buffer, "\n");
  Append(buffer);


  /* Records */
  for (size_t i = 0; i < RecordLen; ++i)
    {
    sprintf(buffer, "%4d  ", i);

    for (size_t j = 0; j < nBuffs; ++j)
      sprintf(&buffer[strlen(buffer)], "%08lX ", *(long *)(&sets[j].data[i<<2]));

    strcat(buffer, "\n");
    Append(buffer);
    }

  MoveTo(0);

}	/* END UPDATE */

/* END HEX.CC */
