PRO sid_export_event, ev
    uname=widget_info(ev.id,/uname)

    CASE uname OF
 
        'addfile': BEGIN ;===========================================================================
            IF ev.value eq 0 THEN BEGIN ;Add a file pressed
                fn=dialog_pickfile(/read,/multiple,filter=['*SID.dat'],dialog_parent=widget_info(ev.top,find='addfile'))
                IF file_test(fn[0]) THEN BEGIN
                   widget_control,widget_info(ev.top,find='filelist'),set_value=fn                  
                   widget_control,widget_info(ev.top,find='outdir'),get_uvalue=userchange
                   IF userchange eq 0 THEN widget_control,widget_info(ev.top,find='outdir'),set_value=file_dirname(fn[0])
                ENDIF
            ENDIF
        END


        'findoutdir': BEGIN ;===========================================================================
            a=dialog_pickfile(/read,/directory,get_path=a2,dialog_parent=widget_info(ev.top,find='addfile'))
            IF file_test(a) THEN BEGIN
               widget_control,widget_info(ev.top,find='outdir'),set_value=a2
               widget_control,widget_info(ev.top,find='outdir'),set_uvalue=1
            ENDIF
        END


        'process':BEGIN ;===========================================================================
            ;Collect data from the GUI

            ;--------Filenames
            widget_control,widget_info(ev.top,find='filelist'),get_value=fn
            widget_control,widget_info(ev.top,find='outdir'),get_value=outdir
            IF strmid(outdir,strlen(outdir)-1,1) ne path_sep() THEN outdir=outdir+path_sep() ;Add path separator if needed
            
            outfilelist=strarr(n_elements(fn))
            FOR i=0,n_elements(fn)-1 DO BEGIN
               widget_control,widget_info(ev.top,find='process'),set_value='Processing file '+strtrim(string(i+1),2)+'...'               sid_export_ncdf,fn[i],outdir=outdir,outfile=outfile
               outfilelist[i]=outfile
            ENDFOR
            widget_control,widget_info(ev.top,find='process'),set_value='EXPORT to netCDF'
            dummy=dialog_message('Wrote '+outfilelist,dialog_parent=widget_info(ev.top,find='addfile'),/info)
        END
        ELSE: dummy=0
    ENDCASE
END





PRO sid_export
    IF !version.os_family eq 'windows' THEN widget_control,default_font='Helvetica*fixed*12'
    IF !version.os_family eq 'unix' THEN widget_control,default_font='-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1'
    base = WIDGET_BASE(COLUMN=1,title='Export Data',MBar=menubarID)

    subbase5=widget_base(base,column=1,/frame)
    addfile = cw_bgroup(subbase5,['Select file...'], uname='addfile',/row,label_left='SID file to export:')
    filelist= widget_text(subbase5,uname='filelist',/editable,/scroll,ysize=5)
	 subbase5a=widget_base(subbase5,row=1)
    outdirID=cw_field(subbase5a,/string,  title='Output dir:         ',uname='outdir',xsize=52,uvalue=0)
    browse2=widget_button(subbase5a,value='Select...',uname='findoutdir')



    process = WIDGET_BUTTON(base, value='EXPORT to netCDF', uname='process')
    WIDGET_CONTROL, base, /REALIZE
    XMANAGER, 'sid_export', base, /no_block
END
