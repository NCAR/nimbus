PRO procsid2h, directoryname, pthfile=pthfile, textfile=textfile, fixedtas=fixedtas,$
               autolevel=autolevel, speedreject=speedreject, outdir=outdir, $
               maxsaturated=maxsaturated, tastag=tastag, peak=peak, sizegain=sizegain,$
               ncappend=ncappend
             
   ;Command line only version of sid.pro.  This is mainly for running the
   ;program straight from a terminal/console command line.  
   ;AB 4/2011

   ;Get all .srd filenames from the input directory
   fn=file_search(directoryname,'*.srd')  

   ;Set default values of arguments not specified in command line
   IF n_elements(pthfile) eq 0 THEN pthfile='none'
   IF n_elements(textfile) eq 0 THEN textfile=0
   IF n_elements(fixedtas) eq 0 THEN fixedtas=150
   IF n_elements(autolevel) eq 0 THEN autolevel=1
   IF n_elements(speedreject) eq 0 THEN speedreject=1
   IF n_elements(outdir) eq 0 THEN outdir=getenv('PWD')+path_sep()
   IF n_elements(maxsaturated) eq 0 THEN maxsaturated=10
   IF n_elements(tastag) eq 0 THEN tastag='TASX'
   IF n_elements(peak) eq 0 THEN peak=1
   IF n_elements(sizegain) eq 0 THEN sizegain=1.4
   IF n_elements(ncappend) eq 0 THEN ncappend=1
   
   op={fn_orig:fn,  rate:1, sizegain:sizegain, pthfile:pthfile, textfile:textfile, $
       fixedtas:fixedtas, autolevel:autolevel, speedreject:speedreject, outdir:outdir, $
       maxsaturated:maxsaturated, tastag:tastag, peak:peak, createncdf:1, createsav:0, $
       ncappend:ncappend}
   
   sid_process, op
END
