/*
-------------------------------------------------------------------------
OBJECT NAME:	ncReorder

FULL NAME:	

ENTRY POINTS:	main

STATIC FNS:	none

DESCRIPTION:	Reorder an existing netCDF file, changing the UNLIMITED
		dimension to a fixed dimension.  This has the affect of
		making all the data for given variable contiguous on disk.

INPUT:		netCDF file

OUTPUT:		netCDF file

REFERENCES:	libnetcdf_c++.a

REFERENCED BY:	Command line

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 2004
-------------------------------------------------------------------------
*/


#include "netcdf.hh"

float	data[50000000];

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  NcFile	inFile(argv[1]);

  if (argc < 3)
  {
    fprintf(stderr, "Usage: ncReorder infile.nc outfile.nc\n");
    exit(1);
  }


  if (!inFile.is_valid())
  {
    fprintf(stderr, "ncReorder: Invalid input file, exiting.\n");
    exit(1);
  }

  if (!inFile.get_dim("Time")->is_unlimited())
  {
    fprintf(stderr,
	"ncReorder: 'Time' dimension is not UNLIMITED, reorder unnecessary...\n");
    exit(1);
  }

  NcFile	outFile(argv[2], NcFile::Replace);

  outFile.set_fill(NcFile::NoFill);

  if (!outFile.is_valid())
  {
    fprintf(stderr, "ncReorder: Unable to create/destroy output file, exiting.\n");
    exit(1);
  }

  // Transfer dimensions.
  for (int i = 0; i < inFile.num_dims(); ++i)
    outFile.add_dim(inFile.get_dim(i)->name(), inFile.get_dim(i)->size());


  // Transfer global attributes.
  for (int i = 0; i < inFile.num_atts(); ++i)
    outFile.add_att(inFile.get_att(i)->name(), inFile.get_att(i)->num_vals(),
	(const char *)inFile.get_att(i)->values()->base());


  // Transfer Variables.
  NcVar	*var;
  NcDim *dims[6];
  for (int i = 0; i < inFile.num_vars(); ++i)
  {
    var = inFile.get_var(i);
//printf("%s\n", var->name());

    // Transfer dims for the var.
    for (int j = 0; j < var->num_dims(); ++j)
      for (int k = 0; k < outFile.num_dims(); ++k)
        if (var->get_dim(j)->size() == outFile.get_dim(k)->size())
        {
          dims[j] = outFile.get_dim(k);
          break;
        }

    outFile.add_var(var->name(), var->type(), var->num_dims(), (const NcDim**)dims);


    // Transfer variable attributes.
    for (int j = 0; j < var->num_atts(); ++j)
    {
      NcAtt  *att = var->get_att(j);

      switch (att->type())
      {
        case ncChar:
          outFile.get_var(i)->add_att(att->name(), att->num_vals(),
		(const char *)att->values()->base());
          break;

        case ncFloat:
          outFile.get_var(i)->add_att(att->name(), att->num_vals(),
		(const float *)att->values()->base());
          break;

        case ncInt:
          outFile.get_var(i)->add_att(att->name(), att->num_vals(),
		(const long *)att->values()->base());
          break;

        default:
          fprintf(stderr, "Currently unsupported data type in var attr transfer.\n");
      }
    }
  }



  // Transfer data.
  long	*edges;
  for (int i = 0; i < inFile.num_vars(); ++i)
  {
    edges = inFile.get_var(i)->edges();

printf("%s\n", inFile.get_var(i)->name());

    inFile.get_var(i)->get(data, inFile.get_var(i)->edges());
    outFile.get_var(i)->put(data, outFile.get_var(i)->edges());
  }

  inFile.close();
  outFile.close();

}	/* END MAIN */

/* END NCFAST.CC */
