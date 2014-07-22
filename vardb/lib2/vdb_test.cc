#include "vardb.h"

int main(int argc, char *argv[])
{
  bool test_save = false;
  VDBFile vdb;

  if (strcmp(argv[1], "-s") == 0)
    test_save = false;

  printf("Hello Julian\n");
  vdb.open("VDB.xml"); 
  VDBVar zero = vdb.get_var("ZERO");
  cout<<zero.get_attribute("long_name")<<"\n";
  zero.set_attribute("unITs","none");
  VDBVar s = vdb.get_var("2D-C");
  cout<<s.get_attribute("long_name")<<"\n";
  s.set_attribute("reference","false");
  vdb.close();
}
