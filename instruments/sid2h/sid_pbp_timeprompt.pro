;PRO to get user input of start/stop times when
;  creating a pbp file.  Keeps pbp file size more manageable.
;AB 11/2011

PRO sid_pbp_timeprompt_event, ev
   uname=widget_info(ev.id,/uname)
   CASE 1 OF
      (uname eq 'OK'):BEGIN
         ;Get start/stop time
         id=widget_info(ev.top,find='start')
         widget_control,id,get_value=newstart
         id=widget_info(ev.top,find='stop')
         widget_control,id,get_value=newstop
         
         widget_control,ev.top,get_uvalue=ptr
         (*ptr).start=hms2sfm(newstart)
         (*ptr).stop=hms2sfm(newstop)        
         widget_control, ev.top, /destroy ;Close GUI and go past XMANAGER command
      END
      ELSE: dummy=0
   ENDCASE
END 
   
FUNCTION sid_pbp_timeprompt, starttime, stoptime
   IF !version.os_family eq 'windows' THEN widget_control,default_font='Helvetica*fixed*12'
   IF !version.os_family eq 'unix' THEN widget_control,default_font='-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1'
   ptr=ptr_new({start:sfm2hms(starttime), stop:sfm2hms(stoptime)})
   
   ;----------Main widget setup-------------------------------------------
   base = WIDGET_BASE(COLUMN=1,title='SID-2H Processing Software',xoffset=200,yoffset=200,/kbrd_focus_events)
   subbase1=widget_base(base,column=1)
   dummy=widget_label(subbase1,value='Choose time range for Particle-by-Particle data:',/align_left)
   time1=cw_field(subbase1,/long, title='   Start (hhmmss):',uname='start', xsize=8, value=(*ptr).start)
   time2=cw_field(subbase1,/long, title='   Stop (hhmmss):',uname='stop', xsize=8, value=(*ptr).stop)
   
   
   process = WIDGET_BUTTON(base, value='   OK   ', uname='OK')
   
   WIDGET_CONTROL, base, /REALIZE, set_uvalue=ptr
   XMANAGER, 'sid_pbp_timeprompt', base; , /no_block
   return,*ptr
   
END