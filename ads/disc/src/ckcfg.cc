/* ckcfg.cc
   Checks the format of the configuration files.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <stdlib.h>
#include <stream.h>

#include <DsmConfig.h>
#include <VarConfig.h>

DsmConfig dsm_config;				// network config class

main (int argc, char *argv[])
{
  int stat;
  int stat1;

  if (argc < 3) {
    cout << "Usage: ckcfg host location\n";
    exit (0);
  }


  cout << "dsm_config.count() = " << dsm_config.count() << "\n";

  for (stat = dsm_config.firstDsm(); stat; stat = dsm_config.nextDsm()) {
    printf ("index = %d\n", dsm_config.index());
    printf ("hostName = %s, location = %s, dsmHeaderName = %s\n",
      dsm_config.hostName(),dsm_config.location(), dsm_config.dsmHeaderName());
    printf ("port = %d, time_master = %d, localRecord = %d, standalone = %d\n",
      dsm_config.port(), dsm_config.timeMaster(), dsm_config.localRecord(),
      dsm_config.standalone()); 
    printf ("isCtlLocation = %d, isWindsLocation = %d, isDsmLocation = %d\n",
      dsm_config.isCtlLocation(), dsm_config.isWindsLocation(), 
      dsm_config.isDsmLocation());
    putchar ('\n');
  }

  VarConfig var_config (argv[1], argv[2]);

  for (stat = var_config.firstVar(); stat; stat = var_config.nextVar()) {
    cout << "var_name = " << var_config.varName();
    cout << " depends = ";
    for (stat1 = var_config.firstDepend(); stat1; 
         stat1 = var_config.nextDepend()) 
      cout << var_config.dependName() << " ";
    cout << '\n';
  }

}
