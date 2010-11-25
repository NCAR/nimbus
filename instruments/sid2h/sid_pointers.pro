FUNCTION sid_pointers, lun
   ;Function to find the pointers to all various SID data
   ;in a srd file.
   
   q=fstat(lun)
   point_lun,lun,0 ;reset pointer to top
   z=ulonarr(q.size/4)
   readu,lun,z
   point_lun,lun,q.cur_ptr ;reset pointer to original location
   
   header=where(z eq 'ff000000'x)*4
   footer=where(z eq 'ffffffff'x)*4
   datablock=where(z eq 'ff000001'x)*4
   
   saveon=where(z eq 'ff000010'x)*4
   saveoff=where(z eq 'ff000011'x)*4
   online=where(z eq 'ff000013'x)*4
   gainchange=where(z eq 'ff000014'x)*4
   smallpmt=where(z eq 'ff000015'x)*4
   largepmt=where(z eq 'ff000016'x)*4
   airspeed=where(z eq 'ff000017'x)*4
   exception=where(z eq 'ff000018'x)*4
   lowdisk=where(z eq 'ff000019'x)*4
   m6xwarning=where(z eq 'ff00001a'x)*4
   m6xerror=where(z eq 'ff00001b'x)*4
   altitude=where(z eq 'ff00001c'x)*4
   lasercal=where(z eq 'ff000002'x)*4

   return,{header:header,footer:footer,datablock:datablock,gainchange:gainchange,$
           lasercal:lasercal,smallpmt:smallpmt,largepmt:largepmt,online:online,$
           m6xwarning:m6xwarning,m6xerror:m6xerror,saveon:saveon,saveoff:saveoff,$
           exception:exception,lowdisk:lowdisk}
END
