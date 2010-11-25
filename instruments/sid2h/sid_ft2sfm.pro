FUNCTION sid_ft2sfm, ft
   ;FUNCTION to convert Windows filetime structure to
   ;seconds.
   
   t1970=116444736000000000  ;time from 1601 to 1970, for converting from Windows filetime

   sec=long((ft-t1970)/10000000) mod 86400
   return,sec   
END
