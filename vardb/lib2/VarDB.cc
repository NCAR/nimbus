#include"vardb.h"
int main()
{
 printf("Hello Julian\n");
 VDBFile vdb;
 vdb.open("VDB.xml"); 
 vdb.get_var("2DEE1");
}
