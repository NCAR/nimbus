FUNCTION sid_header, lun
   ;Function to return the header values from a SID raw file.
   ;AB 4/2007
   
   a={series:0ul, filenum:0ul, format:0ul, revision:0ul, buffsize:0l,$
      saving:0ul, filestarttime:0ull, stringlength:0ul}
   readu,lun,a

   ;Need to pad this to nearest word
   id=bytarr(a.stringlength)
   readu,lun,id
   
   ;ID is padded with spaces to the nearest word.  Skip over these...
   padlength=4-(a.stringlength mod 4)
   IF padlength ne 4 THEN BEGIN
      point_lun,-lun,p
      point_lun,lun,p+padlength  ;just to advance the pointer
   ENDIF
   
   
   return,create_struct(a,'id',string(id),'type',2)
END
