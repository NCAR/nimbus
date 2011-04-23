FUNCTION sid_ncdf_getstartstop, fn
   ;PRO to get start/stop time from the RAF netCDF files for the SID processing.
   ;AB 4/2011
   
   id=ncdf_open(fn[0])
   ncdf_attget,id,'TimeInterval',time,/global
   
   x=strsplit(string(time),'-',/extract)
   starttime=sid_hms2sfm(strjoin(strsplit(x[0],':',/extract)))
   stoptime=sid_hms2sfm(strjoin(strsplit(x[1],':',/extract)))
   
   ncdf_close,id
   return,[starttime,stoptime]
END