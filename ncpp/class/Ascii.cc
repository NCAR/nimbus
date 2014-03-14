/*
-------------------------------------------------------------------------
OBJECT NAME:	Ascii.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2009
-------------------------------------------------------------------------
*/

#include "Ascii.h"

static const char *dashes = "        ---";

/* -------------------------------------------------------------------- */
Ascii::Ascii(const Widget parent) : TextWindow(parent, "ascii")
{

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void Ascii::Update(SetManager& sets, PlotManager *plotMgr)
{
  int	i, j;
  DataSet	*set;

  Clear();

  Append(plotMgr->Title().c_str()); Append(", ");
  Append(plotMgr->SubTitle().c_str()); Append("\n\n");

  for (set = sets.FirstSet(); set; set = sets.NextSet())
    {
    strcpy(buffer, set->probe()->Name().c_str());
 
    strcat(buffer, "\n  Cell diameter end points (um)\n");

    for (i = 0; i < set->probe()->VectorLength(); ++i)
      sprintf(&buffer[strlen(buffer)], "%11.3f", set->probe()->CellSize(i));

    strcat(buffer, "\n");
    Append(buffer);
    buffer[0] = '\0';


    if (sets.DataTypes() & COUNTS)
      {
      strcat(buffer, "\n  Raw Counts\n");

      for (i = 0; i < sets.NumberRecords(); ++i)
        {
        long sum = 0;
        for (j = 0; j < set->probe()->VectorLength(); ++j)
          {
          sprintf(&buffer[strlen(buffer)], "%11.0f", set->Accumulation(i, j));
          sum += (long)set->Accumulation(i, j);
          }

        sprintf(&buffer[strlen(buffer)], "  total=%7d\n", sum);
        Append(buffer);
        buffer[0] = '\0';
        }
      }

    if (sets.DataTypes() & CONCENTRATION)
      {
      strcat(buffer, "\n  Concentrations ");
      strcat(buffer, MakeYAxisLabel(sets.GetNormalization(), sets.DataTypes()).c_str());
      strcat(buffer, "\n");

      for (i = 0; i < sets.NumberRecords(); ++i)
        {
        double sum = 0.0;
        for (j = 0; j < set->probe()->FirstBin(); ++j)
          strcat(buffer, dashes);

        for (; j <= set->probe()->LastBin(); ++j)
          {
          sprintf(&buffer[strlen(buffer)], "%11.3e", set->Concentration(i, j));
          sum += set->Concentration(i, j);
          }

        for (; j < set->probe()->VectorLength(); ++j)
          strcat(buffer, dashes);

        sprintf(&buffer[strlen(buffer)], "  total=%e\n", sum);
        Append(buffer);
        buffer[0] = '\0';
        }
      }

    if (sets.DataTypes() & SURFACE)
      {
      strcat(buffer, "\n  Surface ");
      strcat(buffer, MakeYAxisLabel(sets.GetNormalization(), sets.DataTypes()).c_str());
      strcat(buffer, "\n");

      for (i = 0; i < sets.NumberRecords(); ++i)
        {
        double sum = 0.0;
        for (j = 0; j < set->probe()->FirstBin(); ++j)
          strcat(buffer, dashes);

        for (; j <= set->probe()->LastBin(); ++j)
          {
          sprintf(&buffer[strlen(buffer)], "%11.3e", set->Surface(i, j));
          sum += set->Surface(i, j);
          }

        for (; j < set->probe()->VectorLength(); ++j)
          strcat(buffer, dashes);

        sprintf(&buffer[strlen(buffer)], "  total=%e\n", sum);
        Append(buffer);
        buffer[0] = '\0';
        }
      }

    if (sets.DataTypes() & VOLUME)
      {
      strcat(buffer, "\n  Volume ");
      strcat(buffer, MakeYAxisLabel(sets.GetNormalization(), sets.DataTypes()).c_str());
      strcat(buffer, "\n");

      for (i = 0; i < sets.NumberRecords(); ++i)
        {
        double sum = 0.0;
        for (j = 0; j < set->probe()->FirstBin(); ++j)
          strcat(buffer, dashes);

        for (; j <= set->probe()->LastBin(); ++j)
          {
          sprintf(&buffer[strlen(buffer)], "%11.3e", set->Volume(i, j));
          sum += set->Volume(i, j);
          }

        for (; j < set->probe()->VectorLength(); ++j)
          strcat(buffer, dashes);

        sprintf(&buffer[strlen(buffer)], "  total=%e\n", sum);
        Append(buffer);
        buffer[0] = '\0';
        }
      }

    Append("\n");
    }

  MoveTo(0);

}	/* END UPDATE */

/* END ASCII.CC */
