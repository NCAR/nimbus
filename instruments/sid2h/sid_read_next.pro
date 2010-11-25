FUNCTION sid_read_next, lun
   ;FUNCTION to read the next header, datablock, event, or footer in a SID raw file.
   ;AB 4/2007
   
   flag=0ul
   readu,lun,flag
   systime=0ull
  
   CASE flag OF
      ;----------Data-----------------
      'ff000000'x : a=sid_header(lun)
      'ffffffff'x : a=sid_footer(lun)
      'ff000001'x : a=sid_datablock(lun)
      
      ;----------Events--------------
      'ff000010'x : BEGIN
         event='save on'
         readu,lun,systime
         a={type:'10'x, event:event, systime:systime}
      END
      
      'ff000011'x : BEGIN
         event='save off'
         readu,lun,systime
         a={type:'11'x, event:event, systime:systime}
      END
      
      'ff000013'x : BEGIN
         event='online'
         readu,lun,systime
         a={type:'13'x, event:event, systime:systime}
      END

      'ff000014'x : BEGIN
         event='detector gain change'
         readu,lun,systime
         value=0ul
         readu,lun,value
         a={type:'14'x, event:event, systime:systime, value:value}
      END
        
      'ff000015'x : BEGIN
         event='small pmt voltage'
         readu,lun,systime
         value=0ul
         readu,lun,value
         a={type:'15'x, event:event, systime:systime, value:value}
      END

      'ff000016'x : BEGIN
         event='large pmt voltage'
         readu,lun,systime
         value=0ul
         readu,lun,value
         a={type:'16'x, event:event, systime:systime, value:value}
      END

      'ff000017'x : BEGIN
         event='air speed'
         readu,lun,systime
         value=0ul
         readu,lun,value
         a={type:'17'x, event:event, systime:systime, value:value}
      END

      'ff000018'x : BEGIN
         event='exception'
         readu,lun,systime
         value=0ul
         readu,lun,value
         t=0ull
         readu,lun,t ; when the exception was generated... don't save
         a={type:'18'x, event:event, systime:systime, value:value}
      END

      'ff000019'x : BEGIN
         event='low disk space'
         readu,lun,systime
         a={type:'19'x, event:event, systime:systime}
      END
 
      'ff00001a'x : BEGIN
         event='m6x warning'
         readu,lun,systime
         value=ulonarr(2)
         readu,lun,value
         a={type:'1a'x, event:event, systime:systime, value:value}
      END
    
      'ff00001b'x : BEGIN
         event='m6x error'
         readu,lun,systime
         value=ulonarr(2)
         readu,lun,value
         a={type:'1b'x, event:event, systime:systime, value:value}
      END
      
      'ff00001c'x : BEGIN
         event='altitude'
         readu,lun,systime
         value=0ul
         readu,lun,value
         a={type:'1c'x, event:event, systime:systime, value:value}
      END
     
      'ff000002'x : BEGIN
         event='laser calibration'
         value=fltarr(6)
         readu,lun,value
         a={type:'20'x, event:event, value:value}
      END
   ENDCASE
   
   return,a
END

