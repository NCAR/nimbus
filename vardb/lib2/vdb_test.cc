#include"Vardb.h"
int main()
{
 printf("Hello Julian\n");
 VDBFile vdb;
 vdb.open("VDB.xml"); 
 VDBVar zero = vdb.get_var("ZERO");
 cout<<zero.get_attribute("long_name")<<"\n";
 zero.set_attribute("unITs","none");
 VDBVar s = vdb.get_var("2D-C");
 cout<<s.get_attribute("long_name")<<"\n";
 s.set_attribute("reference","false");
 vdb.close();
}
