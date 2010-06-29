@echo on

set LOCAL_CLASSPATH=.\nc2asc.jar;.\netcdfUI-4.1.jar;.\nlog4j-1.2.25.jar;.\commons-logging-1.1.jar

java -cp %LOCAL_CLASSPATH% edu.ucar.eol.nc2asc.NC2A  %*
