#include"vardb.h"
int main()
{
 printf("Hello Julian\n");
 VDBFile vdb;
 vdb.open("VDB.xml"); 
 VDBVar zero = vdb.get_var("ZERO");
 cout<<zero.get_attribute("long_name")<<"\n";
 VDBVar s = vdb.get_var("2D-C");
 cout<<s.get_attribute("long_name")<<"\n";
 //cout<<zero->get_attribute("units");
}
