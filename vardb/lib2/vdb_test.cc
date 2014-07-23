#include "vardb.h"

std::string fileName("VDB.xml");

int main()
{
  VDBFile vdb(fileName);

  if (vdb.is_valid() == false)
  {
    std::cerr << "Failed to open " << fileName << endl;
    return 1;
  }

  VDBVar *zero = vdb.get_var("ZERO");
  if(zero->get_attribute("UnIts")=="none")
  {cout<<"Looks good!\n";
  }else{cout<<"Check configuration\n";}

  vdb.close();
  return 0;
}
