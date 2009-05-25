@echo on

set LOCAL_CLASSPATH=.\nc2asc.jar;.\netcdfUI-2.2.22.jar;.\nlog4j-1.2.25.jar;.\commons-logging-1.1.jar

java -cp %LOCAL_CLASSPATH% edu.ucar.eol.nc2Asc.NC2A  %*
