FUNCTION sid_datablock, lun
   ;Function to return a datablock from a SID raw file.
   ;AB 4/2007
   
   point_lun,-lun,cur_pointer  ;store current pointer

   q={systime:0ull, numwords:0ul}
   readu,lun,q
   
   num=q.numwords/21  ;the number of particles in this block

   scatter=intarr(num,28)
   tof=lonarr(num)
   missed=lonarr(num)
   sensor=lonarr(num)
   pmttemp=lonarr(num)
   forced=bytarr(num)
   mux=bytarr(num)
   crc=ulonarr(num)
   elaptime=ulon64arr(num) ;time from probe start
   
   a=ulonarr(21)
   
   ;Detect if this datablock contained errors.  If so, try some offsets or return error code.
   ;Seems that 21-word records sometimes get split between datablocks
   ;Best way to check is to look for negative interarrival times:
   offset=0
   point_lun,-lun,datastart ;store the beginning of 
   
   ;Try to read data until the correct offset is found.  Usually zero unless there's a problem
   REPEAT BEGIN 
    point_lun,lun,datastart+offset*4
    lastbintime=0
    errorfound=0
   
    FOR i=0,num-1 DO BEGIN
      readu,lun,a
      
      bintime=ishft(ulong64(a[9]) and '00000000000fffff'x,20)+(a[4] and '000fffff'x)
      elaptime[i]=bintime*50
      IF bintime lt lastbintime THEN errorfound=1
      lastbintime=bintime
         
      one=fix(ishft(a and '00fff000'x,-8))/16  ;shift these both to the integer position, then divide to preserve sign
      two=fix(ishft(a and '00000fff'x, 4))/16
      scatter[i,*]=[one[0],two[0],one[1],two[1],one[2],two[2],one[3],two[3],one[5],two[5],one[6],two[6],$
            one[7],two[7],one[8],two[8],one[10],two[10],one[11],two[11],one[12],two[12],one[13],$
            two[13],one[15],one[16],one[17],one[18]]
      
      tof[i]=ishft(a[14] and '0007fc00'x,-10) * 50 ;convert to nanoseconds
      missed[i]=(a[14] and '000003ff'x)   
      sensor[i]=two[17]
      pmttemp[i]=two[18]
      
      forced[i]=ishft(a[19] and '00000010'x,-4)    ;forced digitization... if 1 then no real particle
      mux[i]=(a[19] and '0000000f'x)               ;which mux value is recorded in 'sensor'

      ;Here is the polynomial in hex: '01864cfb'x
      crc[i]=a[20]  
    ENDFOR
    offset=offset+1
   ENDREP UNTIL (errorfound eq 0) or (offset eq 21)
  
   return,{systime:q.systime,elaptime:elaptime,numwords:q.numwords,scatter:scatter,tof:tof,missed:missed,$
           sensor:sensor,pmttemp:pmttemp,forced:forced,mux:mux,crc:crc,cur_pointer:cur_pointer,num:num,type:1,error:errorfound}
END
