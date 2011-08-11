PRO sid_process, op, statuswidgetid=statuswidgetid

   ;PRO to process SID2 raw files and put it in SODA format.
   ;Made for HIAPER
   ;Input option structure 'op' should have tags of 'fn_orig','autolevel',
   ;   'textfile','pthfile','fixedtas','sizegain','outdir'
   ;Aaron Bansemer, April 2007
   ;Version 1.0 March 2009.  Includes size calibrations and algorithm from AIDA 
   ;     (Dec 2008) and gain calibrations at RAF (Feb 2009).
   
 
   ;--------------Default values if not specified in op structure-----------------
   sid_update_op, op
   IF n_elements(statuswidgetid) eq 0 THEN statuswidgetid=0
   
   ;Make sure the outdir is OK and can write to it
   op.outdir=strtrim(op.outdir,2)
   IF strmid(op.outdir,strlen(op.outdir)-1,1) ne path_sep() THEN op.outdir=op.outdir+path_sep() ;Add path separator if needed
   IF (file_info(op.outdir)).write ne 1 THEN BEGIN
      infoline='Cannot write to '+op.outdir+', processing stopped.'
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid) ELSE print,infoline
      return
   ENDIF
   
   ;Warn if a fixed tas is going to be used
   IF file_test(op.pthfile) eq 0 THEN BEGIN
      infoline='No flight data found, using fixed TAS of ' + string(op.fixedtas,form='(f5.1)') + ' m/s.'
      choice='Continue'
      IF statuswidgetid ne 0 THEN choice=dialog_message(infoline,dialog_parent=statuswidgetid,/cancel) ELSE print,infoline
      IF choice eq 'Cancel' THEN return
   ENDIF
   
   ;Check for blank filename, an easy clicking error
   IF file_test(op.fn_orig[0]) eq 0 THEN BEGIN
      infoline='Empty filename, please re-check.'
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid,/info) ELSE print,infoline
      return
   ENDIF

   ;--------------Organize files into chronological order, check for existence and errors---
   nfiles=n_elements(op.fn_orig)
   lun=1
   free_lun,lun
   
   ;Check for errors
   infoline='Checking file integrity...'
   IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline ELSE print,infoline
   badfile=bytarr(nfiles)
   FOR i=0,nfiles-1 DO BEGIN
      openr,lun,op.fn_orig[i]
      IF file_test(op.fn_orig[i]) eq 0 THEN BEGIN
         infoline='Cannot find file '+op.fn_orig[i]+', this file will not be processed.'
         IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid) ELSE print,infoline
         badfile[i]=1
      ENDIF ELSE BEGIN
         p=sid_pointers(lun)
         IF (n_elements(p.footer) gt 1) or (p.footer[0] lt 100) THEN BEGIN
            infoline='Bad footer: '+op.fn_orig[0]+', this file will not be processed.'
            IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid) ELSE print,infoline
            badfile[i]=1
         ENDIF
      ENDELSE
      free_lun,lun
   ENDFOR

   ;Throw out bad files
   fn=op.fn_orig[where(badfile eq 0)]
   nfiles=n_elements(fn)
   fstarttime=ulon64arr(nfiles)
   filesize=lonarr(nfiles)
   
   ;Sort the good files
   FOR i=0,nfiles-1 DO BEGIN
      openr,lun,fn[i]
      q=fstat(lun)
      filesize[i]=q.size
      header=sid_read_next(lun)
      free_lun,lun
      fstarttime[i]=header.filestarttime
   ENDFOR
   fn=fn[sort(fstarttime)]
   IF total(where(tag_names(op) eq 'FN')) eq -1 THEN op=create_struct(op,'fn',fn)
   filesize=filesize[sort(fstarttime)]
   total_bytes=total(filesize)
   
   ;--------------Read in header and footer from first/last file-----------------
   ;Open last file, read footer
   openr,lun,fn[nfiles-1]
   p=sid_pointers(lun)
   IF (n_elements(p.footer) gt 1) or (p.footer[0] lt 100) THEN BEGIN
      infoline='Bad footer: '+fn[0]+', processing stopped.  Remove this file from the list.'
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid) ELSE print,infoline
      stop,'Bad file: ',fn[0]
   ENDIF
   point_lun,lun,p.footer  ;point to the footer
   lastfooter=sid_read_next(lun)
   free_lun,lun
   
   ;Open first file and read footer, header, and laser calibration values
   openr,lun,fn[0]
   p=sid_pointers(lun)
   IF (n_elements(p.footer) gt 1) or (p.footer[0] lt 100) THEN BEGIN
      infoline='Bad footer: '+fn[0]+', processing stopped.  Remove this file from the list.'
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid) ELSE print,infoline
      stop,'Bad file: ',fn[0]
   ENDIF
   point_lun,lun,p.footer
   firstfooter=sid_read_next(lun)
   point_lun,lun,p.header
   firstheader=sid_read_next(lun)
   point_lun,lun,p.lasercal
   lasercal=sid_read_next(lun)
   free_lun,lun
   
   ;------------Initialize variables----------------------------------------
   ;Constants
   numdetectors=28
   laser_depth=210.0   ;microns
   sa_major=700.0      ;sample area major axis DIAMETER (microns)
   sa_minor=500.0      ;minor axis (microns)
   rate=fix(op.rate)

   ;Interarrival setup
   numintbins=40
   imaxpower=2    ;log10 of longest time bin (s)
   iminpower=-6  ;log10 of shortest time bin (s)
   intendbins=10^((findgen(numintbins+1)/numintbins)*(imaxpower-iminpower)+iminpower)       ;log-spaced endbins from iminpower to maxpower
   intmidbins=10^(((findgen(numintbins)+0.5)/numintbins) *(imaxpower-iminpower)+iminpower)  ;midbins

   ;Transit time setup
   numtofbins=20
   tmaxpower=-4  ;log10 of longest time bin (s)
   tminpower=-7  ;log10 of shortest time bin (s)
   tofendbins=10^((findgen(numtofbins+1)/numtofbins)*(tmaxpower-tminpower)+tminpower)       ;log-spaced endbins from iminpower to maxpower
   tofmidbins=10^(((findgen(numtofbins)+0.5)/numtofbins) *(tmaxpower-tminpower)+tminpower)  ;midbins
   
   ;Size bin setup
   numbins=40
   smaxpower=alog10(200)  ;log10 of largest size (um)
   sminpower=alog10(1)   ;log10 of smallest size (um)
   endbins=10^((findgen(numbins+1)/numbins)*(smaxpower-sminpower)+sminpower)       ;log-spaced endbins from sminpower to smaxpower
   midbins=10^(((findgen(numbins)+0.5)/numbins) *(smaxpower-sminpower)+sminpower)  ;midbins     
   binwidth=endbins[1:numbins]-endbins[0:numbins-1]

   ;Time-based size bin setup
   tbnumbins=20
   tbmaxpower=alog10(400)  ;log10 of largest size (um)
   tbminpower=alog10(1)   ;log10 of smallest size (um)
   tbendbins=10^((findgen(tbnumbins+1)/tbnumbins)*(tbmaxpower-tbminpower)+tbminpower)       ;log-spaced endbins from sminpower to smaxpower
   tbmidbins=10^(((findgen(tbnumbins)+0.5)/tbnumbins) *(tbmaxpower-tbminpower)+tbminpower)  ;midbins     
   tbbinwidth=tbendbins[1:tbnumbins]-tbendbins[0:tbnumbins-1]

   ;Asphericity factor setup
   numafbins=25
   ;afmidbins=findgen(numafbins)*2+1
   amaxpower=alog10(50)  ;log10 of largest size (um)
   aminpower=alog10(0.1)   ;log10 of smallest size (um)
   afmidbins=10^(((findgen(numafbins)+0.5)/numafbins) *(amaxpower-aminpower)+aminpower)  ;midbins     

   ;Circular index setup
   numcibins=10
   cimidbins=findgen(numcibins)/numcibins + 1.0/numcibins/2
   ciendbins=findgen(numcibins+1)/numcibins

   ;Date and time setup
   date=(sid_date(firstheader.filestarttime))
   starttime=sid_ft2sfm(firstfooter.acqstart) 
   stoptime=sid_ft2sfm(lastfooter.fileclosetime) 
   IF op.ncappend THEN BEGIN
      ;Get start/stop from the ncdf file instead
      temp=sid_ncdf_getstartstop(op.pthfile)
      starttime=temp[0]
      stoptime=temp[1]
      op.rate=1    ;Must be the same as ncdf files (always 1Hz)
      rate=1
   ENDIF
   IF stoptime lt starttime THEN stoptime=stoptime+86400
   num=(stoptime-starttime)/rate +1 ;number of records that will be saved
   time=starttime+rate*dindgen(num)  ; this is the start time for each record 
  
   ;Variable initialization
   spec1d=fltarr(num,numbins)     ;counts
   spec2d=fltarr(num,numbins,numafbins)  ;counts
   spec_round=fltarr(num,numbins)  ;counts, round particles
   spec_irreg=fltarr(num,numbins)  ;counts, non-round particles
   intspec=fltarr(num,numintbins) ;interarrival time
   afspec=fltarr(num,numafbins)   ;asphericity factor
   aftotal=fltarr(num)            ;for computing the mean Af
   branch_count=lonarr(num,8)     ;count for number of 'branches' on a particle
   cispec=fltarr(num,numcibins)   ;circular index
   firstblock=lonarr(num)-1       ;index of the first block used in this time period
   firsti=lonarr(num)             ;the index of the first particle in the datablock
   blockpointer=lonarr(nfiles*1000);Pointers to all the data blocks
   filenum=intarr(nfiles*1000)    ;File number index for all the data blocks
   missed=lonarr(num)
   missedhist=lonarr(1000)
   transittime=fltarr(num)
   speed=fltarr(num)
   pmtgain=fltarr(num)+firstfooter.initialgain
   largetrigger=fltarr(num)+firstfooter.largetrigger
   smalltrigger=fltarr(num)+firstfooter.smalltrigger
   muxvalue=fltarr(num,11)        ;store each of the mux values
   nbmux=intarr(num,11)           ;the number of 'buffers' for each mux value
   totalscatter=lonarr(20,numdetectors)      ;keeps a running tally of scatter
   accept_count=lonarr(num)       ;counts the number of accepted particles
   reject_count=lonarr(num)       ;counts the number of rejected particles
   reject_reason=lonarr(num,8)    ;keeps track of reason for rejection
   dethist=lonarr(numdetectors,2500) ;creates a histogram of levels for each detector
   tofspec=fltarr(num,numtofbins)
   eventindex=0                   ;These keep track of various events and errors.  Will be resized as needed.
   eventtype=0
   eventstring=''
   tbspec1d=fltarr(num,tbnumbins)
   
   
   ;------------Get G-V data and TAS if it exists --------------------------
   IF file_test(op.pthfile) eq 1 THEN BEGIN
      infoline='Loading flight data...'
      IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline ELSE print,infoline
      tas=sid_tas({fn:op.pthfile,starttime:starttime,stoptime:stoptime,rate:rate},tag=op.tastag)
   ENDIF ELSE tas=fltarr(num)+op.fixedtas 
   
   ;-----------Auto leveling---------------------------------------------------
   detgain=lastfooter.cal_coeff
   detoffset=lastfooter.cal_offset
   IF op.autolevel THEN detgain=sid_autolevel(op,detoffset,statuswidgetid) 

   ;----------Set up text file-------------------------------------------------
   IF op.textfile eq 1 THEN BEGIN
      free_lun,2
      textfilename=op.outdir+date.mdy+'_'+strtrim(string(long(sid_sfm2hms(starttime))),2)+'_SID.txt'
      openw,2,textfilename
      printf,2,'Reftime: ',firstfooter.reftime
      printf,2,'Acquisition Start: ',firstfooter.acqstart
      printf,2,'time diam(um) Af Branches mux sensor tof missed scatter'
   ENDIF
  
   ;------------Read data----------------------------------------------------
   lastelaptime=0
   blocknum=0
   infoline='Processing...'+string(0,form='(i4)')+'%' 
   IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline ELSE print,infoline
   last_percent=0
   last_acqstart=0

   ;Loop through the files
   FOR ifile=0,nfiles-1 DO BEGIN 
      ;Open file and read footer and header
      openr,lun,fn[ifile]
      p=sid_pointers(lun)
      
      ;Read in header and footer
      point_lun,lun,p.footer
      footer=sid_read_next(lun)
      point_lun,lun,p.header
      header=sid_read_next(lun)
      
      ;Update anything that changed between files
      IF last_acqstart ne footer.acqstart THEN BEGIN ;only make these updates when a new *series* is started
         index=(sid_ft2sfm(footer.acqstart) - starttime)/rate
         IF (index gt 0) and (index lt (num-1)) THEN BEGIN
            pmtgain[index:*]=footer.initialgain
            smalltrigger[index:*]=footer.smalltrigger
            largetrigger[index:*]=footer.largetrigger
            last_acqstart=footer.acqstart
         ENDIF
      ENDIF
      
      ;Look for important events later on in the file
      IF p.gainchange[0] ne -4 THEN BEGIN
         FOR j=0,n_elements(p.gainchange)-1 DO BEGIN
            point_lun,lun,p.gainchange[j]
            newgain=sid_read_next(lun)
            index=(sid_ft2sfm(newgain.systime) - starttime)/rate
            IF (index gt 0) and (index lt (num-1)) THEN pmtgain[index:*]=newgain.value
         ENDFOR
      ENDIF
      
      IF p.smallpmt[0] ne -4 or p.largepmt[0] ne -4 THEN print,'Undocumented trigger change.'  ;I haven't coded this up yet...
      
      events=[p.saveon,p.saveoff,p.exception,p.lowdisk,p.m6xerror,p.m6xwarning]
      events=events[where(events ne -4,nevents)]
      FOR ievents=0,nevents-1 DO BEGIN
         point_lun,lun,events[ievents]
         ev=sid_read_next(lun)
         index=(sid_ft2sfm(ev.systime) - starttime)/rate
         IF (index gt 0) and (index lt (num-1)) THEN BEGIN
            eventindex=[eventindex,index]
            eventtype=[eventtype,ev.type]
            eventstring=[eventstring,ev.event]
         ENDIF
      ENDFOR
      
      ;Process the datablocks for this file
      numblocks=n_elements(p.datablock)
      IF p.datablock[0] eq -4 THEN numblocks=0  ;For files with no particles
      FOR ip=0,numblocks-1 DO BEGIN
         ;Save the block pointers and filenumbers for each block
         blockpointer[blocknum]=p.datablock[ip]
         filenum[blocknum]=ifile
  
         point_lun,lun,p.datablock[ip]
         b=sid_read_next(lun)
         IF b.error eq 0 THEN BEGIN
         ;Update mux values
         index=(sid_ft2sfm(b.systime) - starttime)/rate
         IF (index gt 0) and (index lt (num-1)) THEN BEGIN
            muxvalue[index,b.mux]=muxvalue[index,b.mux]+b.sensor
            nbmux[index,b.mux]=nbmux[index,b.mux]+1
         ENDIF       
         
         ;Process each particle in the datablock
         FOR i=0,b.num-1 DO BEGIN
            ;Find time index
            index=((sid_ft2sfm((b.elaptime[i]-footer.reftime)/100+footer.acqstart)) - starttime)/rate
            ;Skip over 'forced' particles
            IF (b.forced[i] eq 0) and (index gt 0) and (index lt (num-1)) THEN BEGIN  
                                            
               ;Adjust scatter based on calibrations
               scatter_adjusted=(b.scatter[i,*]-detoffset)*detgain   ;Verified correct
               
               ;Keep a histogram for each detector prior to rejection
               scatter_pos=fix(scatter_adjusted)>0<2499
               
               FOR k=0,numdetectors-1 DO dethist[k,scatter_pos[k]]=dethist[k,scatter_pos[k]]+1
               
                ;Record location so the particles can be located in analysis software                  
               IF firstblock[index] eq -1 THEN BEGIN
                  firstblock[index]=blocknum
                  firsti[index]=i
               ENDIF
              
               ;Find saturated detectors, then size
               a=sid_size(b.scatter[i,*],scatter_adjusted,pmtgain[index],b.tof[i],tas[index],op.sizegain,peak=op.peak) 
               IF op.textfile eq 1 THEN printf,2,(sid_date((b.elaptime[i]-footer.reftime)/100+footer.acqstart)).time, $
                  a.size,a.af,a.branches,b.mux[i],b.sensor[i],b.tof[i],b.missed[i],mean(scatter_adjusted), $ ;b.elaptime[i]-footer.reftime,
                  format='(f15.5,2f7.1,i3,i3,i6,i6,i6,i6)' 
                        
               ;Find interarrival time
               inttime=(b.elaptime[i]-lastelaptime)/1.0e9
               lastelaptime=b.elaptime[i]
               missed[index]=missed[index]+b.missed[i]
               missedhist[b.missed[i]<999]=missedhist[b.missed[i]<999]+1

               ;Compute size and interarrival bin indices
               IF a.size gt 0 THEN  isize=fix((alog10(a.size)-sminpower)*numbins/(smaxpower-sminpower)) ELSE isize=-1
               IF inttime gt 0 THEN iint=fix((alog10(inttime)-iminpower)*numintbins/(imaxpower-iminpower)) ELSE iint=-1
               
               ;Particle rejection           
               reject=sid_reject(a,b.tof[i],endbins,inttime,intendbins,a.nsat,op.maxsaturated,tas[index],laser_depth,speedreject=op.speedreject)
               IF reject ne 0 THEN reject_reason[index,reject]=reject_reason[index,reject]+1
               
               ;Update time-based size spectra, use only saturated particles
               IF (a.tbsize gt 0)  THEN BEGIN   ;and (a.nsat eq numdetectors)
                  itb=fix((alog10(a.tbsize)-tbminpower)*tbnumbins/(tbmaxpower-tbminpower))
                  itb=itb>0<(tbnumbins-1)
                  tbspec1d[index,itb]=tbspec1d[index,itb]+1
               ENDIF
               
               ;Update various arrays with accepted particles
               IF reject eq 0 THEN BEGIN  ;make sure we have valid time and size
                  accept_count[index]=accept_count[index]+1
                  iaf=fix((alog10(a.af)-aminpower)*numafbins/(amaxpower-aminpower)) < (numafbins-1) > 0;index for asphericity factor
                  ici=max(where(a.ci gt ciendbins)) < (numcibins-1) > 0
                  spec1d[index,isize]=spec1d[index,isize]+1
                  spec2d[index,isize,iaf]=spec2d[index,isize,iaf]+1
                  IF (a.branches eq 0) THEN spec_round[index,isize]=spec_round[index,isize]+1 ELSE $
                     spec_irreg[index,isize]=spec_irreg[index,isize]+1
                  intspec[index,iint]=intspec[index,iint]+1                  
                  afspec[index,iaf]=afspec[index,iaf]+1
                  aftotal[index]=aftotal[index]+a.af
                  cispec[index,ici]=cispec[index,ici]+1
                  itof=fix((alog10(b.tof[i]/1.0e9)-tminpower)*numtofbins/(tmaxpower-tminpower))
                  tofspec[index,itof]=tofspec[index,itof]+1
                  s=fix(mean(scatter_pos)/100)<19
                  totalscatter[s,*]=totalscatter[s,*]+scatter_adjusted             
                  transittime[index]=transittime[index]+b.tof[i]                  
                  branch_count[index,a.branches]=branch_count[index,a.branches]+1
               ENDIF ELSE reject_count[index]=reject_count[index]+1
            ENDIF ELSE BEGIN ;end 'forced particle' if
               ;The forced particles are background values that can be used as calibrations
               ;but is often noisy if a particle was in the volume.  This needs a little
               ;more error checking before being  used.  Does not make much visual difference.
               ;IF max(b.scatter[i,*]) lt 100 THEN footer.cal_offset=b.scatter[i,*]
            ENDELSE
         ENDFOR    ;end particle loop
         ENDIF     ;end 'error' in datablock if
         blocknum=blocknum+1
         
         ;Update the status in the GUI or command line
         percent_processed=fix(p.datablock[ip]/total_bytes*100)
         IF ifile gt 0 THEN percent_processed=fix((total(filesize[0:ifile-1])+p.datablock[ip])/total_bytes*100)      
         infoline='Processing...'+string(percent_processed,form='(i4)')+'%' 
         IF percent_processed ne last_percent THEN IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline ELSE print,infoline
         last_percent=percent_processed
      ENDFOR       ;end datablock loop
      free_lun,lun
      
   ENDFOR          ;end file loop

   np=total(spec1d,2)>1               ;find total # at each time
   transittime=transittime/np/1.0e9   ;make an average, convert to seconds
   w=where(transittime gt 0)
   IF w[0] ne -1 THEN speed[w]=(laser_depth/1.0e6)/transittime[w] ;assume 140um deep laser, in m/s
 
   ;-------Compute bulk parameters--------------------------------
   sa=(!pi/4*sa_major*sa_minor)/1.0e12  ;Trigger 1 area in meters^2
   deadtime=(reject_count+accept_count)*50.0e-6   ;50 microseconds dead-time per particle
   activetime=fltarr(num)+rate-deadtime  ;assume it is always active
   sv=sa*tas*activetime
   
   tbsa=(!pi/4*(sa_major+tbmidbins)*(sa_minor+tbmidbins))/1.0e12  ;Trigger 1 area in meters^2, this also uses particle size
   
   ntdead=missed/(sa*tas*deadtime)  ;number concentration during dead time
   
   conc1d=fltarr(num, numbins)  ;size spectra
   tbconc1d=fltarr(num, tbnumbins)  ;time-based size spectra
   conc_round=fltarr(num, numbins)  ;size spectra round particles
   conc_irreg=fltarr(num, numbins)  ;size spectra non-round particles
   area=fltarr(num) & lwc=fltarr(num) & nt=fltarr(num) & mvd=fltarr(num) & mnd=fltarr(num)
   area_round=fltarr(num) & lwc_round=fltarr(num) & nt_round=fltarr(num) & mvd_round=fltarr(num) & mnd_round=fltarr(num)
   area_irreg=fltarr(num) & lwc_irreg=fltarr(num) & nt_irreg=fltarr(num) & mvd_irreg=fltarr(num) & mnd_irreg=fltarr(num)
   FOR i=0l,num-1 DO BEGIN
      conc_raw=spec1d[i,*]/(sv[i])     ;for computing bulk parameters
      conc1d[i,*]=conc_raw/(binwidth/1.0e6)
      tbconc1d[i,*]=tbspec1d[i,*]/(tbsa*activetime[i]*tas[i])/(tbbinwidth/1.0e6)
      area[i]=total(!pi/4*(midbins/1.0e6)^2 * conc_raw) ;1/m
      lwc[i]=total(!pi/6*(midbins/1.0e4)^3 * conc_raw) ;g/m3
      mvd[i]=sid_mvdiam(conc_raw,midbins,/interp)
      mnd[i]=sid_meandiam(conc_raw,midbins)
      nt[i]=total(conc_raw)     ;1/m3
      
      ;Repeat above computations for round particle population
      conc_raw=spec_round[i,*]/(sv[i])     ;for computing bulk parameters
      conc_round[i,*]=conc_raw/(binwidth/1.0e6)
      area_round[i]=total(!pi/4*(midbins/1.0e6)^2 * conc_raw) ;1/m
      lwc_round[i]=total(!pi/6*(midbins/1.0e4)^3 * conc_raw) ;g/m3
      mvd_round[i]=sid_mvdiam(conc_raw,midbins,/interp)
      mnd_round[i]=sid_meandiam(conc_raw,midbins)
      nt_round[i]=total(conc_raw)     ;1/m3
      
      ;Repeat for non-round particles
      conc_raw=spec_irreg[i,*]/(sv[i])     ;for computing bulk parameters
      conc_irreg[i,*]=conc_raw/(binwidth/1.0e6)
      area_irreg[i]=total(!pi/4*(midbins/1.0e6)^2 * conc_raw) ;1/m
      lwc_irreg[i]=total(!pi/6*(midbins/1.0e4)^3 * conc_raw) ;g/m3
      mvd_irreg[i]=sid_mvdiam(conc_raw,midbins,/interp)
      mnd_irreg[i]=sid_meandiam(conc_raw,midbins)
      nt_irreg[i]=total(conc_raw)     ;1/m3
   ENDFOR   
   
   ;----------------Interpolate over missing mux values-----------------
   FOR i=0,10 DO BEGIN
      m=muxvalue[*,i]
      nb=nbmux[*,i]
      w=where(m ne 0,wcount)
      IF wcount gt 0 THEN muxvalue[*,i]=interpol(m[w]/nb[w],w,lindgen(num))
   ENDFOR
  
   mux={Tdetector:muxvalue[*,0],Tlaser:muxvalue[*,1],$
        Tlasercontrol:muxvalue[*,2],Tfpgaambient:muxvalue[*,3],$
        Theadheater:muxvalue[*,4],Theadambient:muxvalue[*,6],$
        laserpowermon:muxvalue[*,8],Tlaserpowermon:muxvalue[*,10]}
    
   ;---------------Clean up and save data--------------------------------
   IF blocknum eq 0 THEN BEGIN
      infoline='No data blocks found.'
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid,/info) ELSE print,infoline
      IF op.textfile eq 1 THEN free_lun,2
      return
   ENDIF
   blockpointer=blockpointer[0:blocknum-1]
   filenum=filenum[0:blocknum-1]
   IF n_elements(eventindex) gt 1 THEN BEGIN
      eventindex=eventindex[1:*]    ;The first value was just a dummy
      eventtype=eventtype[1:*]
      eventstring=eventstring[1:*]
   ENDIF
   
   units={time:'seconds UTC',endbins:'microns',midbins:'microns',spec1d:'counts',spec2d:'counts',conc1d:'m^-4',$
          area:'1/m',lwc:'g/m^3',mvd:'microns',mnd:'microns',nt:'m^-3',intendbins:'seconds',intmidbins:'seconds',$
          intspec:'counts',afmidbins:'unitless',afspec:'counts',meanaf:'unitless',transittime:'seconds',speed:'m/s',$
          missed:'counts',missedhist:'counts',tas:'m/s',sa:'m^2',activetime:'seconds',totalscatter:'raw units',$
          tofmidbins:'seconds',tofspec:'counts',pmtgain:'raw units',detoffset:'raw units', detgain:'unitless',$
          autooffset:'raw units',dethist:'counts',largetrigger:'raw units',smalltrigger:'raw units',$
          ntdead:'m^-3'}


   data={op:op, software_version:1.0, footer:firstfooter, header:firstheader, mux:mux, units:units, time:time, $
         date:date.dmy, date_processed:systime(), starttime:starttime, stoptime:stoptime, endbins:endbins, midbins:midbins, $
         spec1d:spec1d, spec2d:spec2d, conc1d:conc1d, area:area, lwc:lwc, mvd:mvd, mnd:mnd, nt:nt, $
         spec_round:spec_round, conc_round:conc_round, area_round:area_round, lwc_round:lwc_round, mvd_round:mvd_round, mnd_round:mnd_round, nt_round:nt_round, $
         spec_irreg:spec_irreg, conc_irreg:conc_irreg, area_irreg:area_irreg, lwc_irreg:lwc_irreg, mvd_irreg:mvd_irreg, mnd_irreg:mnd_irreg, nt_irreg:nt_irreg, $
         intendbins:intendbins, intmidbins:intmidbins, intspec:intspec, branch_count:branch_count,$
         afmidbins:afmidbins, afspec:afspec, meanaf:aftotal/(accept_count>1), transittime:transittime, speed:speed, missed:missed, missedhist:missedhist, $
         accept_count:accept_count, reject_count:reject_count, reject_reason:reject_reason, tas:tas, sa:sa, activetime:activetime,$
         totalscatter:totalscatter, tofmidbins:tofmidbins, tofspec:tofspec, cispec:cispec, cimidbins:cimidbins,$
         pmtgain:pmtgain, detoffset:detoffset, detgain:detgain, dethist:dethist,lasercal:lasercal.value, $
         firsti:firsti, firstblock:firstblock, blockpointer:blockpointer, filenum:filenum, largetrigger:largetrigger, $
         smalltrigger:smalltrigger,ntdead:ntdead,eventindex:eventindex,eventtype:eventtype,eventstring:eventstring, $
         tbspec1d:tbspec1d, tbconc1d:tbconc1d, tbendbins:tbendbins, tbmidbins:tbmidbins}
         
   IF op.createsav eq 1 THEN BEGIN
      idlfilename=op.outdir+date.mdy+'_'+string(long(sid_sfm2hms(starttime)),format='(i06)')+'_SID.dat'
      save, data, file=idlfilename, /compress
      infoline='Saved file '+idlfilename
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid,/info) ELSE print,infoline
   ENDIF
   
   IF op.createncdf eq 1 THEN BEGIN
      ncfilename=op.outdir+date.mdy+'_'+string(long(sid_sfm2hms(starttime)),format='(i06)')+'_SID.nc'
      IF op.ncappend THEN ncfilename=op.pthfile
      sid_export_ncdf,data,outfile=ncfilename,ncappend=op.ncappend
      infoline='Saved file '+ncfilename
      IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid,/info) ELSE print,infoline  
   ENDIF
   
   IF op.textfile eq 1 THEN BEGIN
     free_lun,2
     infoline='Saved file '+textfilename
     IF statuswidgetid ne 0 THEN dummy=dialog_message(infoline,dialog_parent=statuswidgetid,/info) ELSE print,infoline
   ENDIF
   
   infoline='Processing complete.'
   IF statuswidgetid eq 0 THEN print,infoline
END
   
   
  
