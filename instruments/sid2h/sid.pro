PRO sid_event, ev
    uname=widget_info(ev.id,/uname)

    CASE uname OF
        'loadfile': BEGIN ;===========================================================================
            a=dialog_pickfile(/read,filter=['*SID.dat'],path=defaultpath,dialog_parent=widget_info(ev.top,find='process'))
            IF file_test(a) THEN BEGIN
            restore,a
            op=data.op
            sid_update_op,op
            
            ;--------Fill in boxes
            widget_control,widget_info(ev.top,find='rate'),set_value=op.rate
            widget_control,widget_info(ev.top,find='sizegain'),set_value=op.sizegain
            widget_control,widget_info(ev.top,find='intthreshold'),set_value=op.intthreshold
            widget_control,widget_info(ev.top,find='maxsaturated'),set_value=op.maxsaturated
            ;--------TAS stuff
            widget_control,widget_info(ev.top,find='tas'),set_value=op.fixedtas
            widget_control,widget_info(ev.top,find='pthfile'),set_value=op.pthfile
            IF total(where(tag_names(data.op) eq 'TASTAG')) ne -1 THEN $
            widget_control,widget_info(ev.top,find='tastag'),set_value=op.tastag
            IF file_test(op.pthfile) THEN widget_control,widget_info(ev.top,find='tas'),sensitive=0
            
            ;--------Checkboxes
            checkboxarray=[0,0,0,0]
            id=widget_info(ev.top,find='options')
            widget_control,id,get_uvalue=values
            IF op.autolevel THEN checkboxarray[where(values eq 'Auto-leveling')]=1
            IF op.textfile THEN checkboxarray[where(values eq 'Create PBP file')]=1
            IF op.speedreject THEN checkboxarray[where(values eq 'Speed Rejection')]=1
            IF op.peak THEN checkboxarray[where(values eq 'Peak')]=1
            
            widget_control,id,set_value=checkboxarray

            ;--------Filenames
            widget_control,widget_info(ev.top,find='filelist'),set_value=op.fn
            widget_control,widget_info(ev.top,find='outdir'),set_value=op.outdir
            ENDIF
        END


        'addfile': BEGIN ;===========================================================================
            IF ev.value eq 0 THEN BEGIN ;Add a file series pressed
                a=dialog_pickfile(/read,/multiple,path=defaultpath,filter='*srd',title='Select at least one file from each series',dialog_parent=widget_info(ev.top,find='process'))
                IF file_test(a[0]) THEN BEGIN
                   widget_control,widget_info(ev.top,find='filelist'),get_value=olda
                   IF strlen(olda[0]) gt 2 THEN a=[olda,a] 
                   basenames=file_basename(a)
                   stems=strarr(n_elements(a))
                   FOR i=0,n_elements(a)-1 DO stems[i]=strmid(basenames[i],0,strpos(basenames[i],'-'))
                   ustems=stems[uniq(stems)]
                   udirs=file_dirname(a[uniq(stems)])
                   ;widget_control,widget_info(ev.top,find='filelist'),get_value=fn
                   fn=''
                   FOR i=0,n_elements(ustems)-1 DO BEGIN                      
                      IF ustems[i] ne '' THEN BEGIN
                         f=file_search(udirs[i],ustems[i]+'*.srd')
                         IF strlen(fn[0]) gt 2 THEN fn=[fn,f] ELSE fn=f
                      ENDIF
                   ENDFOR   
                   widget_control,widget_info(ev.top,find='filelist'),set_value=fn  
                   IF !version.os_family ne 'unix' THEN widget_control,widget_info(ev.top,find='outdir'),set_value=file_dirname(fn[0])+path_sep()      
                ENDIF
            ENDIF
            IF ev.value eq 1 THEN BEGIN ;Clear files pressed
                widget_control,widget_info(ev.top,find='filelist'),set_value=''
            ENDIF
        END


        'findpthfile': BEGIN ;===========================================================================
            IF ev.value eq 0 THEN BEGIN ;Add a file pressed
               a=dialog_pickfile(/read,filter=['*.nc'],title='Select flight data file',dialog_parent=widget_info(ev.top,find='process'))
               IF file_test(a) THEN BEGIN
                  widget_control,widget_info(ev.top,find='pthfile'),set_value=a
                  widget_control,widget_info(ev.top,find='tas'),sensitive=0
               ENDIF
            ENDIF
            IF ev.value eq 1 THEN BEGIN ;Clear files pressed
                widget_control,widget_info(ev.top,find='pthfile'),set_value=''
                widget_control,widget_info(ev.top,find='tas'),sensitive=1
            ENDIF

        END

        'pthfile': BEGIN ;==========================================================================
           widget_control,widget_info(ev.top,find='pthfile'),get_value=pthfile
           IF file_test(pthfile) THEN widget_control,widget_info(ev.top,find='tas'),sensitive=0 $
           ELSE widget_control,widget_info(ev.top,find='tas'),sensitive=1
        END

        'findoutdir': BEGIN ;===========================================================================
           a=dialog_pickfile(/read,/directory,get_path=a2,title='Select output directory',dialog_parent=widget_info(ev.top,find='process'))
           IF file_test(a) THEN widget_control,widget_info(ev.top,find='outdir'),set_value=a2
        END

        'process':BEGIN ;===========================================================================
            ;Collect data from the GUI
            ;--------Boxes
            widget_control,widget_info(ev.top,find='rate'),get_value=rate
            widget_control,widget_info(ev.top,find='tas'),get_value=fixedtas
            widget_control,widget_info(ev.top,find='tastag'),get_value=tastag
            widget_control,widget_info(ev.top,find='sizegain'),get_value=sizegain
            widget_control,widget_info(ev.top,find='intthreshold'),get_value=intthreshold
            widget_control,widget_info(ev.top,find='maxsaturated'),get_value=maxsaturated

            ;--------Checkboxes
            id=widget_info(ev.top,find='options')
            widget_control,id,get_uvalue=values
            widget_control,id,get_value=iadv
            autolevel=iadv[where(values eq 'Auto-leveling')]
            textfile=iadv[where(values eq 'Create PBP file')]
            speedreject=iadv[where(values eq 'Speed Rejection')]
            peak=iadv[where(values eq 'Peak')]

            ;--------Filenames
            widget_control,widget_info(ev.top,find='filelist'),get_value=fn
            widget_control,widget_info(ev.top,find='outdir'),get_value=outdir
            widget_control,widget_info(ev.top,find='pthfile'),get_value=pthfile

            ;Can add bindistribution to this structure if desired
            op={fn_orig:fn,  rate:rate, sizegain:sizegain, intthreshold:intthreshold,$
            pthfile:pthfile, textfile:textfile, fixedtas:fixedtas, autolevel:autolevel,$            
            speedreject:speedreject, outdir:outdir, maxsaturated:maxsaturated, tastag:tastag,$
            peak:peak}

            widget_control,widget_info(ev.top,find='process'),set_value='Processing...'
            sid_process, op, statuswidgetid=widget_info(ev.top,find='process')
            widget_control,widget_info(ev.top,find='process'),set_value='BEGIN PROCESSING'
        END
        
        'browse': sid_browse
        'export': sid_export
        
        'quit': WIDGET_CONTROL, ev.TOP, /DESTROY

        ELSE: dummy=0
    ENDCASE
END











PRO sid, h=h
    ;------CHANGE DEFAULT VALUES HERE--------------------------------------
    ;----------------------------------------------------------------------
    defaultgain=1.4
    defaultrate=1
    defaultinterarrival=1.0e-6
    defaultsaturation=10
    ;----------------------------------------------------------------------
    ;----------------------------------------------------------------------
    
    IF keyword_set(h) THEN print,'=======Parameter /h successfully passed to sid.pro========='
    IF !version.os_family eq 'windows' THEN widget_control,default_font='Helvetica*fixed*12'
    IF !version.os_family eq 'unix' THEN widget_control,default_font='-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1'
   
    ;----------Main widget setup-------------------------------------------
    base = WIDGET_BASE(COLUMN=1,title='SID-2H Processing Software Version 1.0',MBar=menubarID)

    fileID=widget_button(menubarID, value='File', /menu)
    loadfile=widget_button(fileID, value='Load settings...',uname='loadfile')
    quitID=widget_button(fileID, value='Quit',uname='quit')

    actionID=widget_button(menubarID, value='Other actions', /menu)
    browseID=widget_button(actionID, value='Browse data...',uname='browse')
    browseID=widget_button(actionID, value='Export data...',uname='export')
 
    helpID=widget_button(menubarID, value='Help', /menu,/align_right)

    ;-----------File names widget block------------------------------------
    subbase1=widget_base(base,column=1,/frame)
    addfile = cw_bgroup(subbase1,['Add file series...','Clear files'], uname='addfile',/row,label_left='Raw data file(s):')
    filelist= widget_text(subbase1,/scroll,uname='filelist',ysize=5,xsize=62,/editable) ;/editable
 
 
    ;------------TAS widget block ------------------------------------
    subbase3=widget_base(base,column=1,/frame)
    subbase3a=widget_base(subbase3,row=1)
    subbase3b=widget_base(subbase3,row=1)
    addpthfile=cw_bgroup(subbase3a,['Select...','Clear'], uname='findpthfile',/row,label_left='Flight data file (netCDF):')
    tastag=cw_field(subbase3a, title='TAS tag:',uname='tastag', value='TASX', xsize=5)
    pthfile=widget_text(subbase3,uname='pthfile',/editable,xsize=62,/all_events) 
    tas=cw_field(subbase3,/int, title='or use fixed TAS of (m/s):',uname='tas', value='150', xsize=4)
   

   ;-----------Processing options widget block----------------------------
    subbase2=widget_base(base,column=1,/frame)
    subbase2a=widget_base(subbase2,row=1)
    rate=cw_field(subbase2a,/int, title='Averaging Time (s):',uname='rate' , xsize=4, value=defaultrate)
    sizegain=cw_field(subbase2a,/float, title='Size Gain:',uname='sizegain', value=defaultgain, xsize=5)
    intthreshold=cw_field(subbase2a,/float, title='Min Interarrival (s):',uname='intthreshold', value=defaultinterarrival, xsize=12)
    subbase2b=widget_base(subbase2,row=1)
    rate=cw_field(subbase2b,/int, title='Saturation Max (#):',uname='maxsaturated' , xsize=4, value=defaultsaturation)
    
    vals=['Speed Rejection','Auto-leveling','Create PBP file','Peak']
    advanced=cw_bgroup(subbase2b,vals,uname='options',/row,/nonexclusive,uval=vals,set_value=[1,1,0,1]) ;label_left='Options:',


	 ;---------Output directory and process button-------------------------
	 subbase4=widget_base(base,row=1)
	 cd,current=currentdir
    
    outdirID=cw_field(subbase4,/string,  title='Output directory: ',uname='outdir',xsize=52,value=currentdir+path_sep())
    browse2=widget_button(subbase4,value='Select...',uname='findoutdir')
    
    process = WIDGET_BUTTON(base, value='BEGIN PROCESSING', uname='process')

    WIDGET_CONTROL, base, /REALIZE
    XMANAGER, 'sid', base, /no_block
END

