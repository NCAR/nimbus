FUNCTION sid_autolevel,op,detoffset,statuswidgetid
   ;Function to find the levels required to make the ensemble of all particles round
   
   numdetectors=28
   lun=1
   gainscatter=lon64arr(numdetectors)  ;Size by numdetectors
   autooffset=lonarr(numdetectors)
   autogain=fltarr(numdetectors)+1.0
   scount=0LL
   
   infoline='Finding auto-levels...'
   flipchar=['.   ','..  ','... ','....']
   flipchar=['-','\','|','/']
   flipchar=[':...','.:..','..:.','...:']
   flipcount=0L
   IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline ELSE print,infoline   
   
   FOR ifile=0,n_elements(op.fn)-1 DO BEGIN 
         ;Open file and read footer and header
         free_lun,lun
         openr,lun,op.fn[ifile]
         p=sid_pointers(lun)
         IF p.datablock[0] ne -4 THEN BEGIN   ;Skip files with no data blocks
          FOR ip=0,n_elements(p.datablock)-1 DO BEGIN
            point_lun,lun,p.datablock[ip]
            b=sid_read_next(lun)
            IF b.error eq 0 THEN BEGIN
            FOR ii=0,b.num-1 DO BEGIN
               tempscatter=(b.scatter[ii,*])       ;-lastfooter.cal_offset)*lastfooter.cal_coeff               
               IF (b.forced[ii] eq 0) and (max(tempscatter) lt 2047) and (min(tempscatter) gt 50) THEN BEGIN
                  ;Keep a running sum of scatter for various sized particles, this is for finding the gain
                  gainscatter=gainscatter+tempscatter      
                  scount=scount+1
               ENDIF
            ENDFOR
            ENDIF 
            IF ip mod 20 eq 0 THEN BEGIN
               flipcount=flipcount+1
               iflip=flipcount mod 4
               infoline=flipchar[3-iflip]+' Finding auto-levels '+flipchar[iflip]
               IF statuswidgetid ne 0 THEN widget_control,statuswidgetid,set_value=infoline   
            ENDIF   
          ENDFOR
         ENDIF
   ENDFOR
   free_lun,lun
    
   IF scount gt 10 THEN BEGIN
      tempscatter=gainscatter-detoffset*scount
      IF min(tempscatter) gt 0 THEN autogain=mean(tempscatter)/tempscatter 
   ENDIF
   return, autogain
END
