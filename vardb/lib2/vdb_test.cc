#include "vardb.h"

int main()
{
  VDBFile vdb("VDB.xml");

  VDBVar *zero = vdb.get_var("ZERO");
  if(zero->get_attribute("UnIts")=="none")
  {cout<<"Looks good!\n";
  }else{cout<<"Check configuration\n";}
  vdb.close();
}
