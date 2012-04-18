PRO procsid2h, directoryname, pthfile=pthfile, textfile=textfile, fixedtas=fixedtas,$
               autolevel=autolevel, speedreject=speedreject, outdir=outdir, $
               maxsaturated=maxsaturated, tastag=tastag, peak=peak, sizegain=sizegain,$
               ncappend=ncappend, finalprocessing=finalprocessing, starttime=starttime,$
               stoptime=stoptime, rate=rate, createsav=createsav, createncdf=createncdf
             
   ;Command line only version of sid.pro.  This is mainly for running the
   ;program straight from a terminal/console command line.  
   ;AB 4/2011
   
   !except=0  ;Suppress floating point error messages
   !quiet=1   ;Suppress annoying messages from ncdf routines

   ;Get all .srd filenames from the input directory
   fn=file_search(directoryname,'*.srd')  

   ;Set default values of arguments not specified in command line

   ; pthfile is the netCDF file to read TAS from and output data.
   IF n_elements(pthfile) eq 0 THEN pthfile='none'

   IF n_elements(textfile) eq 0 THEN textfile=0

   ; If using a fixed true airspeed
   IF n_elements(fixedtas) eq 0 THEN fixedtas=150
   IF n_elements(autolevel) eq 0 THEN autolevel=1
   IF n_elements(speedreject) eq 0 THEN speedreject=1
   IF n_elements(outdir) eq 0 THEN outdir=getenv('PWD')+path_sep()
   IF n_elements(maxsaturated) eq 0 THEN maxsaturated=10
   IF n_elements(rate) eq 0 THEN rate=1

   ; True Airspeed variable to use from netCDF file.
   IF n_elements(tastag) eq 0 THEN tastag='TASX'
   IF n_elements(peak) eq 0 THEN peak=1
   IF n_elements(sizegain) eq 0 THEN sizegain=1.4
   IF n_elements(createsav) eq 0 THEN createsav=0
   IF n_elements(createncdf) eq 0 THEN createncdf=1
   IF n_elements(ncappend) eq 0 THEN ncappend=1
   IF n_elements(finalprocessing) eq 0 THEN finalprocessing=0
   IF n_elements(starttime) eq 0 THEN starttime=-1
   IF n_elements(stoptime) eq 0 THEN stoptime=-1
   
   op={fn_orig:fn, rate:rate, sizegain:sizegain, pthfile:pthfile, textfile:textfile, $
       fixedtas:fixedtas, autolevel:autolevel, speedreject:speedreject, outdir:outdir, $
       maxsaturated:maxsaturated, tastag:tastag, peak:peak, createncdf:1, createsav:createsav, $
       createncdf:createncdf, ncappend:ncappend, finalprocessing:finalprocessing, $
       starttime:starttime, stoptime:stoptime}
   
   sid_process, op
END
