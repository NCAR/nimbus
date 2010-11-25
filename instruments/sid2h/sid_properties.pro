PRO sid_properties,p1,pop,top
    
    base = WIDGET_BASE(COLUMN=1,title='File Properties',MBar=menubarID,group_leader=top)
    subbase1=widget_base(base,column=2,/frame)
    dummy=widget_label(subbase1,value='Flight date: '+(*p1).date,/align_left)
    dummy=widget_label(subbase1,value='Data start (s): '+strtrim(string((*p1).starttime),2),/align_left)
    dummy=widget_label(subbase1,value='Data stop (s): '+strtrim(string((*p1).stoptime),2),/align_left)
    dummy=widget_label(subbase1,value='Date processed: '+(*p1).date_processed,/align_left)
    dummy=widget_label(subbase1,value='Sample area: '+strtrim(string((*p1).sa,form='(e10.3)'),2)+' m^2',/align_left)
    dummy=widget_label(subbase1,value='Size Gain: '+strtrim(string((*pop).sizegain,form='(f8.5)'),2),/align_left)
    dummy=widget_label(subbase1,value='Max Saturated: '+strtrim(string((*pop).maxsaturated,form='(i3)'),2),/align_left)
    dummy=widget_label(subbase1,value='Peak Detectors: '+strtrim(string((*pop).peak,form='(i3)'),2),/align_left)

    subbase1a=widget_base(base,column=1,/frame)
    dummy=widget_label(subbase1a,value='Detector settings: ',/align_left)
    dummy=widget_table(subbase1a,column_labels=strtrim(string(bindgen(28)+1),2),row_labels=['Offset','Gain'],$
          value=[[(*p1).detoffset],[(*p1).detgain]],x_scroll_size=6,format='(f7.3)',/frame,/scroll)
 
    subbase2=widget_base(base,column=1,/frame)
    dummy=widget_label(subbase2,value='Raw data files: ',/align_left)
    filelist= widget_text(subbase2,/scroll,uname='filelist',value=(*pop).fn,ysize=6)
    subbase3=widget_base(base,column=1,/frame)
    events=strarr(n_elements((*p1).eventstring))
    FOR i=0,n_elements((*p1).eventstring)-1 DO events[i]=string((*p1).time[(*p1).eventindex[i]],form='(i7)')+'     '+(*p1).eventstring[i]
    dummy=widget_label(subbase3,value='Recorded events: ',/align_left)
    eventlist= widget_text(subbase3,/scroll,uname='filelist',value=events,ysize=6)
    
    WIDGET_CONTROL, base, /REALIZE
END
