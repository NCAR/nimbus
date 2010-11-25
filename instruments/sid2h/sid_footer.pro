FUNCTION sid_footer, lun
   ;Function to return the footer values from a SID raw file.
   ;AB 4/2007
   
   a={last:0ul, fileclosetime:0ull, sidstartlow:0ul, sidstarthigh:0ul,$
      acqstart:0ull, dummy:0l, initialgain:0l, largetrigger:0l, smalltrigger:0l,$
      instrumentID:0l, firmwareversion:0l, numdetectors:0l}
   readu,lun,a
         
   ;Compute reftime, the 20MHz clock at acquisition start time.
   ;Subtract this from other clock times, then add to acqstart to get true time.
   bintime=[(sid_dec2bin(a.sidstarthigh))[12:31] ,(sid_dec2bin(a.sidstartlow))[12:31]]
   reftime=sid_bin2dec64(bintime)*50 ;convert to nanoseconds, 20MHz clock

   cal_coeff=fltarr(a.numdetectors)
   cal_offset=fltarr(a.numdetectors)
   
   readu,lun,cal_coeff
   readu,lun,cal_offset 
   
   return,create_struct(a,'cal_coeff',cal_coeff,'cal_offset',cal_offset,'reftime',reftime,'type',3)
END
