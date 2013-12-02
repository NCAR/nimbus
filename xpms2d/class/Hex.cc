/*
-------------------------------------------------------------------------
OBJECT NAME:	Hex.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-2009
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
    {
    const char * p = (char *)&sets[j].id;
    if (((char *)&sets[j].id)[1] >= '4')	// 64 diode data, hex long-long
      sprintf(&buffer[strlen(buffer)], "       %c%c        ", p[0], p[1]);
    else					// 32 diode data, hex long.
      sprintf(&buffer[strlen(buffer)], "   %c%c    ", p[0], p[1]);
    }

  strcat(buffer, "\n");
  Append(buffer);


  /* Records */
  for (size_t i = 0; i < nSlices_32bit; ++i)
    {
    sprintf(buffer, "%4zu  ", i);

    for (size_t j = 0; j < nBuffs; ++j)
      if (((char *)&sets[j].id)[1] >= '4')
        {
          if (i < nSlices_64bit)
          sprintf(&buffer[strlen(buffer)], "%016llX ", *(long long *)(&sets[j].data[i<<3]));
        }
      else
        sprintf(&buffer[strlen(buffer)], "%08lX ", *(long *)(&sets[j].data[i<<2]));

    strcat(buffer, "\n");
    Append(buffer);
    }

  MoveTo(0);

}	/* END UPDATE */

/* END HEX.CC */
