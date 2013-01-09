function validatetime, starttime
   z = string(strtrim(0, 2))
   slen = strlen(starttime)
   if slen lt 1 then return, -1
   if slen eq 1 then $
      starttime = z + starttime
   if strpos(starttime, ":") ne -1 then begin
      parts = str_sep(starttime, ':', /trim)
      starttime = ""
      for i = 0, n_elements(parts)-1 do begin
         starttime = starttime + parts[i]
      endfor
   endif
   slen = strlen(starttime)
   for i = 0, slen[0]-1 do begin
      save = strmid(starttime, i, 1)
      if save lt 0 or save gt 9 then $
         return, -1
   endfor
   for i = slen[0], 8 do begin
      starttime = string(starttime, 0)
   endfor
   hour = strmid(starttime, 0, 2)
   min = strmid(starttime, 2, 2)
   sec = strmid(starttime, 4, 2)
   msec = strmid(starttime, 6, 3)
   hourn = strtrim(fix(hour[0]), 2)
   minn = strtrim(fix(min[0]), 2)
   secn = strtrim(fix(sec[0]), 2)
   msecn = strtrim(fix(msec[0]), 2)
   if hourn gt 23 then hourn = 23
   if minn gt 59 then minn = 59
   if secn gt 59 then secn = 59 
   hour = string(hourn)
   min = string(minn)
   sec = string(secn)
   msec = string(msecn)
   if hourn lt 10 then hour = z + hour
   if minn lt 10 then min = z + min
   if secn lt 10 then sec = z + sec
   if msecn lt 100 then msec = z + msec $
      else if msecn lt 10 then msec = z + z + msec
   starttime = hour + ":" + min + ":" + sec + "." + msec
print, starttime
   return, 0
end

PRO cpi_event, event

; event handler for all x window events
; save all state information with widget so other routines have
; access to it
   WIDGET_CONTROL, event.top, GET_UVALUE=state, /NO_COPY

   WIDGET_CONTROL, event.id,  GET_UVALUE=control
   CASE control OF

      "Open": BEGIN
         state.cpiFile = DIALOG_PICKFILE(/READ, $
            FILTER='*.roi', PATH=state.datadir, /must_exist, $
            get_path=filepath)
         widget_control, state.fButton, SENSITIVE=0
         widget_control, state.bButton, SENSITIVE=0
         if state.cpiFile ne "" then begin
            pathlen = strlen(filepath)
            state.datadir = filepath
            filename = strmid(state.cpiFile, pathlen)
            widget_control, state.cpifLabel, set_value=filename
            widget_control, /hourglass
            cpidraw, state, 1
            if state.imagecnt eq -1 then begin
               result = dialog_message("There are no images in this file", $
                  /error)
            endif else begin
               widget_control, state.cpiDraw, set_draw_view=[0,0]
;            if state.screenheight > 820 then $
;               widget_control, state.cpiDraw, scr_ysize=state.screenheight
               widget_control, state.cpidLabel, set_value=state.cpiDate
               widget_control, state.btimeLabel, set_value=state.begtime
               widget_control, state.etimeLabel, set_value=state.endtime
               widget_control, state.fButton, SENSITIVE = 1
             endelse
         endif
      END

      "Exit": WIDGET_CONTROL, event.top, /DESTROY

      "Forward": BEGIN
         state.screennum = state.screennum + 1
         widget_control, state.btimeLabel, set_value=" "
         widget_control, state.etimeLabel, set_value=" "
         widget_control, state.lenw, set_value=" "
         widget_control, state.widw, set_value=" "
         widget_control, /hourglass
         cpidraw, state, 0
;         if state.screenheight > 820 then $
;            widget_control, state.cpiDraw, scr_ysize=state.screenheight
         widget_control, state.btimeLabel, set_value=state.begtime
         widget_control, state.etimeLabel, set_value=state.endtime
         if state.endfile eq 1 then $
            widget_control, state.fButton, SENSITIVE=0
         if state.begfile eq 0 then $
            widget_control, state.bButton, SENSITIVE=1
      end

      "Back": BEGIN
         state.screennum = state.screennum - 1
         widget_control, state.btimeLabel, set_value=" "
         widget_control, state.etimeLabel, set_value=" "
      widget_control, state.lenw, set_value=" "
      widget_control, state.widw, set_value=" "
         widget_control, /hourglass
         cpidraw, state, 0
;         if state.screenheight > 820 then $
;            widget_control, state.cpiDraw, scr_ysize=state.screenheight
         widget_control, state.btimeLabel, set_value=state.begtime
         widget_control, state.etimeLabel, set_value=state.endtime
         if state.endfile eq 0 then $
            widget_control, state.fButton, SENSITIVE=1
         if state.begfile eq 1 then $
            widget_control, state.bButton, SENSITIVE=0
      end

      "ImageDir": begin
         widget_control, state.pngdirl, get_value=pngdir
         idir = DIALOG_PICKFILE(/directory, PATH=pngdir, $
            /must_exist, get_path=filepath)
         widget_control, state.pngdirl, set_value=filepath
         state.pngdir = filepath
      end

      "EditTime": begin
         base2 = WIDGET_BASE(title="Enter Start Time", /column)
         WIDGET_CONTROL, /MANAGED, base2
         hourfield = cw_field(base2, title="Hour")
         minfield = cw_field(base2, title="Minute")
         secfield = cw
         widget_control, state.btimeLabel, get_value=starttime
         result = validatetime(starttime)
         if result eq -1 then begin
            result = dialog_message("Start time entered is invalid.", $
                  /error)
         endif
;         valid time for file
;         if valid save in begtime, reread file, find time
;         if invalid, set back to begtime
      end

      ELSE: 
   endcase
   if control ne "Exit" then $
      WIDGET_CONTROL, event.top, SET_UVALUE=state, /NO_COPY
END

pro cpidraw_event, event

; event handler for when left mouse button is clicked when over the
; window that contains the cpi images
   WIDGET_CONTROL, event.top, GET_UVALUE=state, /NO_COPY
;   if event.id ne state.cpiDraw then return
   if event.release eq 1 then begin
      widget_control, /hourglass
      state.pngfile = ""
      cpiimage, state, event.x, event.y, event.release
      if state.pngfile ne "" then begin
         widget_control, state.pnginfo, /append, set_value=state.pngfile
         widget_control, state.pnginfo, /append, set_value="has been created."
      endif
   endif
   if event.release eq 4 then begin
      widget_control, /hourglass
      state.len = 0.0
      state.wid = 0.0
      state.timel = ""
      cpiimage, state, event.x, event.y, event.release
      widget_control, state.lenw, set_value=state.len
      widget_control, state.widw, set_value=state.wid
      widget_control, state.timelw, set_value=state.timel
   endif
   widget_control, event.top, set_uvalue=state, /NO_COPY
end

PRO cleanupcpi, event
   close, /ALL
END

PRO cpidisplay

; main routine that creates the main window and all widgets contained
; in that window
   state = {datadir: "", cpifLabel: 0, cpiFile: "", cpidLabel: 0, $
      cpiDate: "", indata: 0L, bButton: 0, fButton: 0, begfile: 0U, $
      endfile: 0U, btimeLabel: 0L, etimeLabel: 0L, begtime: "", $
      endtime: "", cpiDraw: 0, displaywin: 0, screennum: 0U, $
      screenheight: 0U, screen: lonarr(1000), month: 0U, year: 0U, $
      pngdirl: 0, pnginfo: 0, pngfile: "", pngdir: "", imagecnt: 0U, $
      lenw: 0L, widw: 0L, len: 0.0, wid: 0.0, timelw: 0L, timel: ""}
   state.datadir = getenv('RAW_DATA_DIR')
   pngdir = getenv('TMPDIR')
   if pngdir eq "" then pngdir = "/tmp"
   state.pngdir = pngdir + "/"
   separator = "==============================="
   blanks = "                               "
   base = WIDGET_BASE(title="CPI", mbar=cMenuBar, /ROW)
   WIDGET_CONTROL, /MANAGED, base
   cpiControl = WIDGET_BASE(base, /COLUMN)
   cFileMenu = WIDGET_BUTTON(cMenuBar, VALUE='File', /MENU)
   cFileOpen = WIDGET_BUTTON(cFileMenu, VALUE='Open', UVALUE='Open')
   cExit = WIDGET_BUTTON(cFileMenu, VALUE='Exit', UVALUE='Exit')
   device, retain=2
   cpiDraw = WIDGET_DRAW(base, XSIZE=900, YSIZE=3000, X_SCROLL_SIZE=900, $
      Y_SCROLL_SIZE=700, /BUTTON_EVENTS, EVENT_PRO="cpidraw_event")
   state.cpiDraw = cpiDraw
   state.cpidLabel = WIDGET_LABEL(cpiControl, value=blanks)
   state.cpifLabel = widget_label(cpiControl, value=blanks)
   fButton = WIDGET_BUTTON(cpiControl, value='Step Forward', $
      UVALUE='Forward')
   widget_control, fButton, SENSITIVE = 0
   state.fButton = fButton
   bButton = WIDGET_BUTTON(cpiControl, value='Step Back', $
      UVALUE='Back')
   widget_control, bButton, SENSITIVE = 0
   state.bButton = bButton
   state.btimeLabel = cw_field(cpiControl, /noedit, title="Start")
   state.etimeLabel = cw_field(cpiControl, /noedit, title="End  ")
;   wb0 = widget_button(cpiControl, /align_center, $
;      uvalue = "EditTime", value = "Change Start Time")
   wl00 = widget_label(cpiControl, value=separator, /align_center)
   scale = widget_draw(cpiControl, xsize=122, ysize=42)
   wl0 = widget_label(cpiControl, value=separator, /align_center)
   wl1 = widget_label(cpiControl, value="Left click on image", $
      /align_center)
   wl2 = widget_label(cpiControl, value="to save as png file in", $
      /align_center)
   state.pngdirl = widget_label(cpiControl, value=blanks)
   wb1 = widget_button(cpiControl, /align_center, $
      uvalue = "ImageDir", value = "Change Directory")
   wl3 = widget_label(cpiControl, value="Messages", /align_center)
   state.pnginfo = widget_text(cpiControl, /scroll, ysize=3)
   wl4 = widget_label(cpiControl, value=separator, /align_center)
   wl5 = widget_label(cpiControl, value="Right click on image to", $
      /align_center)
   wl6 = widget_label(cpiControl, value="display size of box enclosing", $
      /align_center)
   wl7 = widget_label(cpiControl, value="the image in microns.", $
      /align_center)
   state.lenw = cw_field(cpiControl, /floating, /noedit, title="X Length")
   state.widw = cw_field(cpiControl, /floating, /noedit, title="Y Length")
   state.timelw = cw_field(cpiControl, /noedit, title="Time  ")
   wl7 = widget_label(cpiControl, value=separator, /align_center)
   image = ""
   if findfile('scale.png') ne "" then $
      image = "scale.png" $
   else if findfile(state.datadir + "/scale.png") ne "" then $
      image = state.datadir + "/scale.png"
   if image ne "" then begin
      scaleimage = read_png (image)
      scaleimage = bytscl(scaleimage)
   endif
   WIDGET_CONTROL, base,  /REALIZE
   widget_control, cpiDraw, get_value = displaywin
   state.displaywin = displaywin
   widget_control, state.pngdirl, set_value=pngdir
   if image ne "" then begin
      widget_control, scale, get_value=scalewin
      wset, scalewin
      tv, scaleimage
   endif
   WIDGET_CONTROL, base, SET_UVALUE = state, /NO_COPY
   XManager, 'Cpi', base, EVENT_HANDLER="cpi_event", /NO_BLOCK, $
      CLEANUP="cleanupcpi"
END
