PRO procsid2h, directoryname, pthfile=pthfile, textfile=textfile, fixedtas=fixedtas,$
               autolevel=autolevel, speedreject=speedreject, outdir=outdir, $
               maxsaturated=maxsaturated, tastag=tastag, peak=peak, sizegain=sizegain
             
   ;Command line only version of sid.pro.  This is mainly for running the
   ;program straight from a terminal/console command line.  
   ;AB 4/2011

   ;Get all .srd filenames from the input directory
   fn=file_search(directoryname,'*.srd')  

   ;Set default values of arguments not specified in command line
   IF not(keyword_set(pthfile)) THEN pthfile='none'
   IF not(keyword_set(textfile)) THEN textfile=0
   IF not(keyword_set(fixedtas)) THEN fixedtas=150
   IF not(keyword_set(autolevel)) THEN autolevel=1
   IF not(keyword_set(speedreject)) THEN speedreject=1
   IF not(keyword_set(outdir)) THEN outdir=getenv('PWD')+path_sep()
   IF not(keyword_set(maxsaturated)) THEN maxsaturated=10
   IF not(keyword_set(tastag)) THEN tastag='TASX'
   IF not(keyword_set(peak)) THEN peak=1
   IF not(keyword_set(sizegain)) THEN sizegain=1.4
   
   op={fn_orig:fn,  rate:1, sizegain:sizegain, pthfile:pthfile, textfile:textfile, $
       fixedtas:fixedtas, autolevel:autolevel, speedreject:speedreject, outdir:outdir, $
       maxsaturated:maxsaturated, tastag:tastag, peak:peak, createncdf:1, createsav:0}
   
   sid_process, op
END
