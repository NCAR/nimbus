# nimbus
Primary EOL/RAF aircraft data processor.

Aircraft data derivation and quality-control processing. NIMBUS is the workhorse of aircraft data integrity and computation. It runs in real-time on the airplane during flights and it also reprocesses flights on the ground into netCDF as quality-control and processing parameters are fine-tuned post-flight.

Proccessing algorithm document can be found here: <https://github.com/NCAR/aircraft_ProcessingAlgorithms/blob/V2019.1/ProcessingAlgorithms.pdf>

Additional documentation is in the [Wiki](https://github.com/NCAR/nimbus/wiki) associated with this repo.

History:  
Developed in the early 1990's to combine the Cray GENPRO processing package and the RAF quick look processor DAP into the primary RAF data processor.  This code repository only goes back to about 2003/2004.

Older versions of nimbus can be found at <https://github.com/NCAR/nimbus_1996-2003>
