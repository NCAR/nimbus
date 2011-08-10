PRO sid_browse_event, ev
    uname=widget_info(ev.id,/uname)
    ;Get pointers to the restored data
    widget_control,widget_info(ev.top,find='load'),get_uvalue=p1
    widget_control,widget_info(ev.top,find='properties'),get_uvalue=pop
    widget_control,widget_info(ev.top,find='base'),get_uvalue=pinfo
    widget_control,widget_info(ev.top,find='tab'),get_uvalue=screen
    screen_x=screen[0]
    screen_y=screen[1]
    
    ;The draw windows have a different event structure.  Ignore unless it is a release event
    IF (uname eq 'w1') or (uname eq 'w2') or (uname eq 'w3') or (uname eq 'wt') THEN BEGIN
    ENDIF

    CASE 1 OF
        ;====================================================================================================
        uname eq 'load':BEGIN
            fn=dialog_pickfile(/read,filter=['*SID.dat'],dialog_parent=widget_info(ev.top,find='tab'))
            IF file_test(fn) eq 0 THEN return
            
            ;Get window IDs to store in pinfo
            widget_control,widget_info(ev.top,find='w1'),get_value=w1
            widget_control,widget_info(ev.top,find='w2'),get_value=w2
            widget_control,widget_info(ev.top,find='w3'),get_value=w3
            widget_control,widget_info(ev.top,find='w4'),get_value=w4
            widget_control,widget_info(ev.top,find='w5'),get_value=w5
            widget_control,widget_info(ev.top,find='wt'),get_value=wt
            widget_control,widget_info(ev.top,find='wp'),get_value=wp
            wid=[w1,w2,w3,w4,w5]

            outdir=file_dirname(fn)+path_sep()           
            info={i:0L,i1:0L,i2:0L,b1i:-1L,fn:fn,gotfile:0,timeformat:1,wid:wid,wt:wt,wp:wp,$
               bmp:bytarr(float(screen_x),50),outdir:outdir,rawdir:'',show_correction:1} ; Reset pinfo
            
            ;Restore files, set pointers to the restored data           
            restore,fn
            op=data.op
            sid_update_op,op
            IF ptr_valid(p1) THEN ptr_free,p1   
            IF ptr_valid(pinfo) THEN ptr_free,pinfo
            IF ptr_valid(pop) THEN ptr_free,pop
            p1=ptr_new(data)
            pop=ptr_new(op)
      
            info.gotfile=1
            widget_control,widget_info(ev.top,find='load'),set_uvalue=p1
            widget_control,widget_info(ev.top,find='properties'),set_uvalue=pop
            
            ;Check to see if the raw data files are still in the same location
            ft=file_test(data.op.fn[0])
            IF ft eq 0 THEN BEGIN
               info.rawdir=dialog_pickfile(/read,/directory,title='Can''t find the raw data, where is it?',dialog_parent=widget_info(ev.top,find='tab'))
               IF file_test(info.rawdir+file_basename(data.op.fn[0])) eq 0 THEN info.rawdir=''  ;User selected the wrong directory
            ENDIF ELSE info.rawdir=file_dirname(data.op.fn[0])+path_sep()
                                   
            ;Plot LWC in the time bar window
            wset,wt
            plot,smooth(data.lwc,5,/nan),xsty=5,ysty=5,pos=[0,0,1,1],yr=[0.0001,10],/yl
            info.bmp=tvrd()     ;The raw plot
            tsbmp=info.bmp
            tsbmp[info.i,*]=150 ;Add a time bar at the beginning
            tv,tsbmp
            
            info.i2=n_elements(data.time)-1
            pinfo=ptr_new(info)
            widget_control,widget_info(ev.top,find='base'),set_uvalue=pinfo
            
            ;Make the rest of the GUI sensitive
            widget_control,widget_info(ev.top,find='tab'),sensitive=1
            widget_control,widget_info(ev.top,find='time'),sensitive=1
            widget_control,widget_info(ev.top,find='timeformat'),sensitive=1
            widget_control,widget_info(ev.top,find='png'),sensitive=1
            widget_control,widget_info(ev.top,find='wt'),sensitive=1
            widget_control,widget_info(ev.top,find='properties'),sensitive=1
            widget_control,widget_info(ev.top,find='color_invert'),sensitive=1
            widget_control,widget_info(ev.top,find='filedisplay'),set_value=file_basename(fn)

            ;Plot new data
            sid_windowplot,ev.top,p1,pinfo,pop
            
         END
        ;====================================================================================================
        (uname eq 'wt')  or (uname eq 'time') or (uname eq 'w1') or (uname eq 'w2') or (uname eq 'w3') or (uname eq 'w5'):BEGIN      
            IF (uname ne 'time') THEN IF ((ev.release eq 0) and (ev.type ne 7)) THEN return ;Ignore non-release events
            tabnum=widget_info(widget_info(ev.top,find='tab'),/tab_current)
            (*pinfo).b1i=-1 ;Reset particle buffer count
            
            advance=0
            i=(*pinfo).i
            i1=(*pinfo).i1
            i2=(*pinfo).i2
            IF (uname eq 'w1') or (uname eq 'w2') or (uname eq 'w3') THEN BEGIN               
               ;Left click, middle click, or scroll down
               IF (ev.release eq 1) or (ev.release eq 2) or (ev.release eq 16) THEN advance=1
               ;Right click or scroll up
               IF (ev.release eq 4) or (ev.release eq 8) THEN advance=-1           
               IF (ev.type eq 7) THEN advance=-(ev.clicks)   ;For Windows compatibility    
            ENDIF
            IF (uname eq 'w5')  THEN BEGIN                              
               xnorm=ev.x/float(screen_x)
               xnormplot=(xnorm-!x.window[0])/(!x.window[1]-!x.window[0])  ;Norm coord within the plot
               IF (xnormplot le 0) or  (xnormplot ge 1) THEN return    ;Out of the plot range, return
               IF (ev.release eq 8) or (ev.release eq 16) THEN return  ;Ignore scroll events here
               IF (ev.release eq 1) THEN i1=xnormplot*(i2-i1)+i1
               IF (ev.release eq 4) THEN i2=xnormplot*(i2-i1)+i1        
            ENDIF
            IF uname eq 'time' THEN BEGIN
               widget_control,widget_info(ev.top,find='time'),get_value=texttime
               time=long(texttime)
               IF (*pinfo).timeformat eq 1 THEN time=sid_hms2sfm(time)
               w=where((*p1).time eq time[0])
               IF w[0] ne -1 THEN i=w[0]
            ENDIF
            IF uname eq 'wt' THEN BEGIN   ;Left click on the wt window moves to the click location
               IF tabnum eq 4 THEN BEGIN
                  IF (ev.release eq 1) THEN i1=ev.x/float(screen_x) * n_elements((*p1).time)
                  IF (ev.release eq 4) THEN i2=ev.x/float(screen_x) * n_elements((*p1).time)
               ENDIF ELSE BEGIN
                  IF (ev.release eq 1) THEN i=ev.x/float(screen_x) * n_elements((*p1).time)
               ENDELSE
               IF (ev.release eq 8) THEN advance =-1
               IF (ev.release eq 16) THEN advance=1  
               IF (ev.type eq 7) THEN advance=-(ev.clicks)   ;For Windows compatibility
            ENDIF
            
            i=i + advance > 0 < (n_elements((*p1).time)-1)         

            i1=i1 < (n_elements((*p1).time)-2) > 0
            i2=i2 > (i1+1) < (n_elements((*p1).time)-1)
            (*pinfo).i=i
            (*pinfo).i1=i1
            (*pinfo).i2=i2
            ;Update the time indicator
            IF (*pinfo).timeformat eq 1 THEN texttime=sid_sfm2hms((*p1).time[i]) ELSE texttime=(*p1).time[i]
            widget_control,widget_info(ev.top,find='time'),set_value=strtrim(string(long(texttime)),2)
            
            ;Update the time plot
            tsbmp=(*pinfo).bmp
            it=((*p1).time[i]-(*p1).starttime)/((*p1).stoptime-(*p1).starttime)*(screen_x-1)
            it1=((*p1).time[i1]-(*p1).starttime)/((*p1).stoptime-(*p1).starttime)*(screen_x-1)
            it2=((*p1).time[i2]-(*p1).starttime)/((*p1).stoptime-(*p1).starttime)*(screen_x-1)
            tsbmp[it1,indgen(25)*2]=150
            tsbmp[it2,indgen(25)*2]=250
            tsbmp[it,*]=100
            tsbmp[(it+1)<(screen_x-1),*]=70
            tsbmp[(it-1)>0,*]=70
            wset,(*pinfo).wt
            tv,tsbmp
        
            ;Plot new data         
            sid_windowplot,ev.top,p1,pinfo,pop

        END
        ;====================================================================================================
        uname eq 'tab': BEGIN
           ;The current tab has been changed
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        uname eq 'reset_range': BEGIN
           (*pinfo).i1=0
           (*pinfo).i2=n_elements((*p1).time)-1
            i=(*pinfo).i
            it=((*p1).time[i]-(*p1).starttime)/((*p1).stoptime-(*p1).starttime)*(screen_x-1)
            tsbmp=(*pinfo).bmp
            tsbmp[0,indgen(25)*2]=150
            tsbmp[screen_x-1,indgen(25)*2]=250
            tsbmp[it,*]=100
            tsbmp[(it+1)<(screen_x-1),*]=70
            tsbmp[(it-1)>0,*]=70
            wset,(*pinfo).wt
            tv,tsbmp
          sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        uname eq 'properties': BEGIN
           sid_properties,p1,pop,ev.top
        END
        ;====================================================================================================
        uname eq 'quit': BEGIN
           IF ptr_valid(p1) THEN ptr_free,p1   
           IF ptr_valid(pinfo) THEN ptr_free,pinfo      
           WIDGET_CONTROL, ev.TOP, /DESTROY
        END
        ;====================================================================================================
        uname eq 'timeformat': BEGIN
           IF (*pinfo).timeformat eq 1 THEN BEGIN
              (*pinfo).timeformat=0 
              widget_control,widget_info(ev.top,find='timeformat'),set_value='SFM'
           ENDIF ELSE BEGIN
              (*pinfo).timeformat=1
              widget_control,widget_info(ev.top,find='timeformat'),set_value='HMS'
           ENDELSE
           ;Update time
           IF (*pinfo).timeformat eq 1 THEN texttime=sid_sfm2hms((*p1).time[(*pinfo).i]) ELSE texttime=(*p1).time[(*pinfo).i]
           widget_control,widget_info(ev.top,find='time'),set_value=strtrim(string(long(texttime)),2)

        END
        ;====================================================================================================
        uname eq 'png': BEGIN
           tabnum=widget_info(widget_info(ev.top,find='tab'),/tab_current) 
           wset,(*pinfo).wid[tabnum]
           image=tvrd(/true)           
           
           ;set_plot,'z'
           ;device,set_resolution=[1000,1000]
           ;sid_windowplot,ev.top,p1,pinfo,/noset           
           ;image=tvrd() 
           ;set_plot,'x'  
           

           IF tabnum eq 0 THEN base='psd'
           IF tabnum eq 1 THEN base='particles'
           IF tabnum eq 2 THEN base='timing'
           IF tabnum eq 3 THEN BEGIN
              base='detectors'
              wset,(*pinfo).wp        ;Get the detector polar plot 
              image2=tvrd(/true)
              temp=bytarr(3,screen_x,screen_y)+byte(!p.background)  ;Concatenate it with the main plot 
              temp[*,*,screen_y/2:screen_y-1]=image   ;350:699
              temp[*,0:screen_y/2-1,0:screen_y/2-1]=image2  ;349
              image=temp
           ENDIF
           IF tabnum eq 4 THEN BEGIN  ;Use the top plot to name it
              widget_control,widget_info(ev.top,find='ts_type1'),get_value=plottype
              id=widget_info(widget_info(ev.top,find='ts_type1'),/droplist_select)
              base=strlowcase(strcompress(plottype[id],/remove_all))
           ENDIF
           
           file=base+'_'+(*p1).date+'_'+strtrim(string(sid_sfm2hms((*p1).time[(*pinfo).i])),2)
           IF file_test((*pinfo).outdir,/write) eq 0 THEN $
               (*pinfo).outdir=dialog_pickfile(/read,/directory,title='Select output directory',dialog_parent=widget_info(ev.top,find='tab'))
           write_png,(*pinfo).outdir+file+'.png',image
           dummy=dialog_message('Wrote '+(*pinfo).outdir+file+'.png',dialog_parent=widget_info(ev.top,find='tab'),/info)
        END
        ;====================================================================================================
        uname eq 'detbox': BEGIN
           ;The detector selection changed
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        (uname eq 'ts_type1') or (uname eq 'ts_type2') or (uname eq 'ts_units'): BEGIN
           ;The time series plot selection changed
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        (uname eq 'selectall') or (uname eq 'clearall'): BEGIN
           ;The detector selection changed
           IF uname eq 'selectall' THEN detbox=intarr(28)+1
           IF uname eq 'clearall' THEN detbox=intarr(28)
           widget_control,widget_info(ev.top,find='detbox'),set_value=detbox
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        (uname eq 'toggle_leveling'): BEGIN
           ;Switch to corrected/uncorrected polar diagnostic plot
           IF (*pinfo).show_correction eq 1 THEN (*pinfo).show_correction=0 ELSE (*pinfo).show_correction=1
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
        (uname eq 'color_invert'): BEGIN
           IF !p.background eq 255 THEN BEGIN
              !p.background=0
              !p.color=255
              !p.thick=1
           ENDIF ELSE BEGIN
              !p.background=255
              !p.color=0
              !p.thick=2
           ENDELSE         
           sid_windowplot,ev.top,p1,pinfo,pop
        END
        ;====================================================================================================
      
       ELSE: dummy=0
    ENDCASE
END



PRO sid_browse_cleanup,tlb
    heap_gc  ;Ugly, but it works for now....    
END




PRO sid_browse
    device,decompose=0,get_screen_size=screen_size     ;Set to 8-bit color
    IF !version.os_family eq 'windows' THEN widget_control,default_font='Helvetica*fixed*12'
    IF !version.os_family eq 'unix' THEN widget_control,default_font='-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1'
    !except=0  ;Suppress floating point error messages
    
    ;Make sure GUI will fit on the screen
    screen_x=(screen_size[0]-50)  <900
    screen_y=(screen_size[1]-300) <900
    
    ;Main widget
    base = WIDGET_BASE(COLUMN=1,title='Browse Processed Data',MBar=menubarID)
    fileID=widget_button(menubarID, value='File', /menu,uname='base') ;uvalue=pinfo,
    loadID=widget_button(fileID, value='Load...',uname='load')
    propID=widget_button(fileID, value='Properties',uname='properties',sensitive=0)
    colorID=widget_button(fileID, value='Invert colors',uname='color_invert',sensitive=0)
    quitID=widget_button(fileID, value='Quit',uname='quit')

    
    tab=widget_tab(base,uname='tab',sensitive=0,uvalue=[screen_x, screen_y])
    
    ;Tab 1
    drawbase=widget_base(tab,row=1,title='Distributions',uname='tab1')
    plot1=widget_draw(drawbase,xsize=screen_x,ysize=screen_y,uname='w1',/button_events,/wheel_events)

    ;Tab 2
    drawbase2=widget_base(tab,row=1,title='Particles',uname='tab2')
    plot2=widget_draw(drawbase2,xsize=screen_x,ysize=screen_y,uname='w2',/button_events,/wheel_events)
    
    ;Tab 3
    drawbase3=widget_base(tab,row=1,title='Timing',uname='tab3')
    plot3=widget_draw(drawbase3,xsize=screen_x,ysize=screen_y,uname='w3',/button_events,/wheel_events)
    
    ;Tab 4
    drawbase4=widget_base(tab,column=1,title='Detectors',uname='tab4')
    plot4=widget_draw(drawbase4,xsize=screen_x,ysize=screen_y/2,uname='w4',frame=3)
    drawbase4b=widget_base(drawbase4,row=1)
    plot4b=widget_draw(drawbase4b,xsize=screen_y/2,ysize=screen_y/2,uname='wp',frame=3)
    drawbase4b1=widget_base(drawbase4b,column=1)
    detbox=cw_bgroup(drawbase4b1,string(strtrim(indgen(28)+1,2),format='(a2)'),/frame,row=4,xpad=20,$
              /nonexclusive,uname='detbox')
    checkall=widget_button(drawbase4b1,uname='selectall',value='Select All')
    clearall=widget_button(drawbase4b1,uname='clearall',value='Clear All ')
    toggle_leveling=widget_button(drawbase4b1,uname='toggle_leveling',value='Toggle Leveling')
   
    ;Tab 5
    drawbase5=widget_base(tab,column=1,title='Time Series',uname='tab5')
    plot5=widget_draw(drawbase5,xsize=screen_x,ysize=screen_y-30,uname='w5',/button_events)
    drawbase5b=widget_base(drawbase5,row=1)
    plottypes=['Total Concentration','LWC','Extinction','Asphericity','Harmonics','Diameter',$
        'Color Concentration','Color Concentration (Round)','Color Concentration (Non-Round)',$
       'Color TB Concentration','Color Asphericity','Color Circular Index','Rejection Codes',$
       'Particle Counts','Speed','Triggers-Gain','Housekeeping1','Housekeeping2']
    ts_type1=widget_droplist(drawbase5b,uname='ts_type1',value=plottypes,title='Plot 1',/frame)
    ts_type2=widget_droplist(drawbase5b,uname='ts_type2',value=plottypes,title='Plot 2',/frame)
    widget_control,ts_type2,set_droplist_select=1
    ts_units=widget_droplist(drawbase5b,uname='ts_units',value=['Hours','Seconds'],title='X-units',/frame)
    reset_range=widget_button(drawbase5b,uname='reset_range',value=' Reset Range ',/frame)
    
    
    
    ;Time series bar
    tsbase=widget_base(base,row=1)
    tsID=widget_draw(tsbase,xsize=screen_x,ysize=50,uname='wt',/button_events,/wheel_events,sensitive=0,frame=3)
    
    ;Time indicator and other info
    timebarbase=widget_base(base,row=1)
    timeID=widget_text(timebarbase,uname='time',value='',xsize=8,/editable,sensitive=0)
    timeformatID=widget_button(timebarbase,uname='timeformat',value=' HMS ',sensitive=0)
    pngID=widget_button(timebarbase,uname='png',value=' Create PNG ',sensitive=0)
    infoID=widget_label(timebarbase,uname='filedisplay',value='',xsize=300,ysize=25)
    
    loadct,39    ;A color table that works for Linux....
    ;tvlct,r,g,b,/get
    ;r=r/1.5 & g=g/1.5 & b=b/1.5
    ;r[1]=200 & g[1]=200 & b[1]=200  ;add a grey color
    ;tvlct,r,g,b
    !p.background=0
    !p.color=255
    !p.charsize=1.3
    
    WIDGET_CONTROL, base, /REALIZE
    XMANAGER, 'sid_browse', base, cleanup='sid_browse_cleanup', /no_block
END
