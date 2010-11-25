FUNCTION sid_date, ft
   ;Finds the date of a time (Windows filetime format) from the SID probe.
   ;Aaron Bansemer, 11/2007
   
   ms=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec']
   
   j1970=2440588
   t1970=116444736000000000  ;time from 1601 to 1970, for converting from Windows filetime

   x=(ft-t1970)/10000000 ;Seconds since 1970
   jdate=float(x/86400) + j1970
   caldat,jdate,m,d,y
   date_dmy=string(d,ms[m-1],y,format='(i02,a3,i4)')    ;Format ddMMMyyyy
   date_mdy=string(m,d,y,format='(2i02,i4)')            ;Numeric format mmddyyyy
   
   time=((ft-t1970) mod 864000000000ll)/double(1.0e7)
   
   return,{dmy:date_dmy,mdy:date_mdy,time:time}
END
   
