FUNCTION sid_sfm2hms, sfm
   ;FUNCTION to convert seconds from midnight to HHMMSS
   ;hms (returned) is a long integer, floats don't work very well due
   ;to round-off errors.
   sfm=long(sfm)
   hour=sfm/3600l
   minute=(sfm mod 3600)/60
   sec=(sfm mod 3600) mod 60
   hms=10000*hour+100*minute+sec
   return,hms
END