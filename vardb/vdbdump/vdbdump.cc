/*
-------------------------------------------------------------------------
OBJECT NAME:	vdbdump.cc

FULL NAME:	VarDB dump

DESCRIPTION:	This program does a basic text dump of a Variable DataBase
		file.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2014
-------------------------------------------------------------------------
*/

#include <iostream>
#include <cstdio>
#include <cstring>

#include <raf/vardb.hh>
#include <raf/VarDBConverter.hh>

bool tracker = false;

/* -------------------------------------------------------------------- */
int
main(int argc, char *argv[])
{
  int i = 1;

  if (argc < 2)
  {
    std::cerr << "Usage: vdbdump [-a] [proj_num | VarDB_filename]\n";
    return(1);
  }

  if (strcmp(argv[i], "-a") == 0)
  {
     tracker = true;
     i++;
  }

  VDBFile file;
  VarDBConverter vdbc;
  vdbc.open(&file, argv[i]);
  if (file.is_valid() == false)
  {
    std::cerr << "vdbdump: Initialize failure.\n";
    return(1);
  }


  //printf("num vars=%d\n", file.num_vars());

  for (i = 0; i < file.num_vars(); ++i)
  {
    VDBVar *var = file.get_var(i);

    if (tracker)
      printf("%-12.12s%-8.8s %-60.60s",
                    var->name().c_str(),
                    var->get_attribute("units").c_str(),
                    var->get_attribute("long_name").c_str());
    else
    {
      printf("%-12.12s %-12.12s %-50.50s",
                    var->name().c_str(),
                    var->get_attribute("units").c_str(),
                    var->get_attribute("long_name").c_str());
/*
      if (ntohl(((struct var_v2 *)VarDB)[i].is_analog) == true)
      {
        ulong	p;

        printf("%4d sps, ", ntohl(((struct var_v2 *)VarDB)[i].defaultSampleRate));
        printf("%d %d Vdc",
		ntohl(((struct var_v2 *)VarDB)[i].voltageRange[0]),
		ntohl(((struct var_v2 *)VarDB)[i].voltageRange[1]));
      }
*/
      printf("\n");
    }
  }

  return(0);
}
