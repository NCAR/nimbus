/*
-------------------------------------------------------------------------
OBJECT NAME:	ncReorder

FULL NAME:	netCDF file re-order

ENTRY POINTS:	main

STATIC FNS:	none

DESCRIPTION:	Reorder an existing netCDF file, changing the UNLIMITED
		dimension to a fixed dimension.  This has the affect of
		making all the data for given variable contiguous on disk.

COPYRIGHT:	University Corporation for Atmospheric Research, 2004-10
-------------------------------------------------------------------------
*/

#include <netcdf.hh>

#include <cstring>

using namespace std;

bool	verbose = false;

/* -------------------------------------------------------------------- */
int usage(char *argv0)
{
  cerr << "Usage: ncReorder [-v] infile.nc outfile.nc\n";
  return 1;
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int		argIndx = 1;

  if (argc < 3)
    return usage(argv[0]);

  if (strcmp(argv[argIndx], "-v") == 0)
  {
    verbose = true;
    ++argIndx;
  }

  NcFile	inFile(argv[argIndx++]);

  if (!inFile.is_valid())
  {
    cerr << "ncReorder: Invalid input file, exiting.\n";
    return 1;
  }

  if (!inFile.get_dim("Time")->is_unlimited())
  {
    cerr << "ncReorder: 'Time' dimension is not UNLIMITED, reorder unnecessary...\n";
    return 1;
  }


  NcFile	outFile(argv[argIndx++], NcFile::Replace);

  outFile.set_fill(NcFile::NoFill);

  if (!outFile.is_valid())
  {
    cerr << "ncReorder: Unable to create/destroy output file, exiting.\n";
    return 1;
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
//cerr << var->name()<<endl;

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
          cerr << "Currently unsupported data type in var attr transfer.\n";
      }
    }
  }



  // Transfer data.
  long	nVars = inFile.num_vars();
  for (int i = 0; i < nVars; ++i)
  {
    if (verbose)
      cout << inFile.get_var(i)->name() << ", nDims = " << inFile.get_var(i)->num_dims() << endl;
    else
    {
      cout << '\r' << (int)(100 * ((float)i / nVars)) << '%';
      flush(cout);
    }

    if (inFile.get_var(i)->num_dims() == 0)
    {
      int	v = inFile.get_var(i)->as_int(0);
      long	l = 1;

      outFile.get_var(i)->put(&v, &l);
    }
    else
    {
      long *edges = inFile.get_var(i)->edges();

      if (inFile.get_var(i)->type() == ncFloat)
      {
        float *data = new float[inFile.get_var(i)->num_vals()];

        inFile.get_var(i)->get(data, inFile.get_var(i)->edges());
        outFile.get_var(i)->put(data, outFile.get_var(i)->edges());
        delete [] data;
      }
      else
      if (inFile.get_var(i)->type() == ncInt)
      {
        int *data = new int[inFile.get_var(i)->num_vals()];

        inFile.get_var(i)->get(data, inFile.get_var(i)->edges());
        outFile.get_var(i)->put(data, outFile.get_var(i)->edges());
        delete [] data;
      }

      delete [] edges;
    }
  }

  cout << endl;
  inFile.close();
  outFile.close();

  return(0);

}	/* END MAIN */

/* END NCREORDER.CC */
