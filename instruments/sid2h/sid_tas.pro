FUNCTION sid_tas, op, tag=tag   ;fn, starttime,stoptime,rate
   ;PRO to get TAS from the RAF netCDF files for the SID processing.
   ;starttime and stoptime entered in sfm, rate is in seconds (3=every three seconds...)
   ;All units in m, s, mb, C
   ;AB 11/2007
   
   IF n_elements(tag) eq 0 THEN tag='TASX'
      
   ;--------------------Read in the netCDF data------------------------
   id=ncdf_open(op.fn[0])
   
   ;Time
   
   ;This is the old C130 way
   ;ncdf_varget,id,'base_time',basetime     ;seconds since Jan 1, 1970
   ;ncdf_varget,id,'time_offset',timeoffset ;difference wrt basetime
   ;time_in=(basetime mod 86400)+timeoffset
   
   ;New way
   ncdf_varget,id,'Time',timeoffset
   ncdf_attget,id,'Time','units',basetime
   basetime=string(basetime)
   p=strpos(basetime,':')
   hour=strmid(basetime,p[0]-2,2)
   minute=strmid(basetime,p[0]+1,2)
   p=strpos(basetime,':',/reverse_search)
   second=strmid(basetime,p[0]+1,2)
   time_in=sid_hms2sfm(hour+minute+second)+timeoffset
   
   ncdf_varget,id,tag[0],tas_in
   bad=where(tas_in eq -32767) & IF bad[0] ne -1 THEN tas_in[bad]=!values.f_nan
   ncdf_close,id

   ;-------------------Initialize and QC inputs and variables---------------------
   starttime=long(op.starttime) & stoptime=long(op.stoptime) & rate=long(op.rate)
   IF stoptime lt starttime THEN stoptime=stoptime+86400
   numlines=stoptime-starttime       ;number of 1Hz lines to read in from the file
   num=numlines/rate + 1             ;number of records that will be stored
   time=starttime+rate*dindgen(num)  ; this is the start time for each record  


   bad=where(tas_in eq -32767) & IF bad[0] ne -1 THEN tas_in[bad]=!values.f_nan
   tas=fltarr(num)
   FOR i=0L,n_elements(time)-1 DO BEGIN
      j=where(time_in ge time[i] AND time_in lt (time[i]+rate),count)
      IF count gt 0 THEN tas[i] = mean(tas_in[j],/nan)
   ENDFOR
   return,tas
END
