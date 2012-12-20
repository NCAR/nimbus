function secondsinyear, year, month, day, hour, minute, second
   return, (julday(month, day, year) - julday(1, 1, year) + 1) $
      * 86400 + hour * 3600 + minute * 60 + second
end

pro cpiopen, state

   A = {PIFILEHEADER, version:0U, year:0U, month:0U, imagex:0U,  $
      imagey:0U, info:bytarr(70)}
   if state.indata ne 0 then begin
      close, state.indata
      for i = 0, 999 do begin
         state.screen(i) = 0
      endfor
   endif
   on_ioerror, ioerr
   state.screennum = 0
   state.endfile = 0
   state.begfile = 1

   openr, indata, state.cpiFile, /get_lun, /swap_if_big_endian
   state.indata = indata
   readu, state.indata, A
; save file position 
   point_lun, -state.indata, filepos
   state.screen(state.screennum) = filepos
   state.year = A.year
   state.month = A.month
   goto, noerr
   ioerr: state.imagecnt = -1
   noerr:
end

pro cpidraw, state, new

   on_ioerror, ioerr
; display 2 seconds of images at one time
   finfo = {day: 0B, hour: 0B, minute: 0B, second: 0B, msecond: 0U, $
      totx: 0, toty: 0, badcount: 0, indata: 0, filepos: 0L, len: 0.0, $
      wid: 0.0, area: 0.0, per: 0.0}
   fullscreen = 0
   savex = 0
   savey = 0
   levelmaxy = 0
   firstimage = 0
   state.imagecnt = 0
   savelabel = ""
   finfo.badcount = 0
; if just selected a new file, then open that file
   if new eq 1 then cpiopen, state
   if state.imagecnt eq -1 then return
   finfo.indata = state.indata
;print, state.screennum, state.screen(state.screennum)
; if have information on file positioning from an earlier read,
; then set file position
   if state.screen(state.screennum) ne 0 then $
      point_lun, state.indata, state.screen(state.screennum)
   wset, state.displaywin
   erase
; set fullscreen to 1 when all two seconds of data have been read
; badcount keeps track of how many reads in a row didn't have a good
; record type.  if > 500 read in a row, then assume the rest of the 
; file is bad
   while fullscreen eq 0 and not eof(state.indata) and $
         (finfo.badcount lt 500) do begin
      cpiread, finfo
      if finfo.badcount eq 500 and firstimage eq 0 then begin
         state.imagecnt = -1
         return
      endif
      if firstimage eq 0 then begin
         firstimage = 1
         state.begtime = string(format= $
            '(I2.2, ":", I2.2, ":", I2.2, ".", I3.3)',  $
            finfo.hour, finfo.minute, finfo.second, finfo.msecond)
;print, state.begtime
         firstsecyear = secondsInYear(state.year, state.month, $
            finfo.day, finfo.hour, finfo.minute, finfo.second)
         state.cpiDate = string(format= $
            '(I2.2, "/", I2.2, "/", I4.4)', $
            state.month, finfo.day, state.year)
      endif
      secyear = secondsInYear(state.year, state.month, $
         finfo.day, finfo.hour, finfo.minute, finfo.second)
      if secyear gt (firstsecyear + 1) then begin
         fullscreen = 1
         state.endtime = savelabel
         state.screenheight = savey + levelmaxy
      endif else begin
         savelabel = string(format= $
            '(I2.2, ":", I2.2, ":", I2.2, ".", I3.3)',  $
            finfo.hour, finfo.minute, finfo.second, finfo.msecond)
         if finfo.totx gt 900 then checkx = 900 else checkx = finfo.totx
         if (savex + checkx) gt 900 then begin
            savey = savey + levelmaxy + 1
            savex = 0
            levelmaxy = 0
         endif
         if (finfo.toty gt levelmaxy) then levelmaxy = finfo.toty
         if (finfo.totx le 0 or finfo.toty le 0) then goto, ioerr
         roidata = bytarr(finfo.totx, finfo.toty)
;print, finfo.totx, finfo.toty, savex, savey, checkx
         readu, state.indata, roidata
         wset, state.displaywin
         tvscl, roidata, savex, savey
         savex = savex + checkx + 1
      endelse
   end
   goto, noerr
   ioerr: finfo.badcount = 500
   noerr: if eof(state.indata) or finfo.badcount ge 500 then begin
      state.endfile = 1
      state.endtime = savelabel
   endif else begin
      state.endfile = 0
      state.screen(state.screennum+1) = finfo.filepos
   endelse
   if state.screennum eq 0 then state.begfile = 1 $
      else state.begfile = 0
end

pro cpiread, finfo

   on_ioerror, ioerr
; read until an roi image is found, then return
   B = {BLOCKTYPE, hdrblk:0U}
   C = {IMAGEBLK, blksize:0UL, version:0U, numrois:0U, tot:0UL,  $
      day:0B, hour:0B, minute:0B, second:0B, msecond:0U,  $
      type:0U, startx:0U, starty:0U, endx:0U, endy:0U, $
      info:bytarr(74)} 
   H = {HOUSEKEEPING, blksize:0UL, version:0U, info:bytarr(98)} 
   R = {ROIIMAGE, blksize:0UL, version:0U, startx:0U, starty:0U,  $
      endx:0U, endy:0U, pixbytes:0, flags:0U, length:0.0, $
      startlen:0UL, endlen:0UL, width:0.0, startwidth:0UL, $
      endwidth:0UL, roidepth:0U, area:0.0, perimeter:0.0}
   done = 0
   while done eq 0 do begin
      readu, finfo.indata, B

      case B.hdrblk of 
; Image Block
         'A3D5'XU:  BEGIN 
            finfo.badcount = 0 
            point_lun, -finfo.indata, filepos
            finfo.filepos = filepos - 2
            readu, finfo.indata, C
         END
; Housekeeping
         'A1D7'XU: BEGIN
            finfo.badcount = 0
            readu, finfo.indata, H
         END
; ROI Image
         'B2E6'XU: BEGIN
            done = 1
            finfo.badcount = 0
; only save if just read an image block
            if C.numrois ne 0 then begin 
               finfo.day = C.day
               finfo.hour = C.hour
               finfo.minute = C.minute
               finfo.second = C.second
               finfo.msecond = C.msecond
            endif
            readu, finfo.indata, R
            finfo.totx = R.endx - R.startx + 1
            finfo.toty = R.endy - R.starty + 1
         END
         else: finfo.badcount = finfo.badcount + 1
      endcase
   end
   goto, noerr
   ioerr: finfo.badcount = 500
   noerr:
end

pro cpiimage, state, x, y, select

; finds the image that has been selected and creates a png image
   finfo = {day: 0B, hour: 0B, minute: 0B, second: 0B, msecond: 0U, $
      totx: 0, toty: 0, badcount: 0, indata: 0, filepos: 0L, len: 0.0, $
      wid: 0.0}
   savelabel = ""
   finfo.badcount = 0
   foundimage = 0
   savex = 0
   savey = 0
   levelmaxy = 0
   numimages = 0
   fullscreen = 0
   finfo.indata = state.indata
   if state.screen(state.screennum) ne 0 then $
      point_lun, state.indata, state.screen(state.screennum)
   wset, state.displaywin
   while foundimage eq 0 and fullscreen eq 0 and $
         not eof(state.indata) and (finfo.badcount lt 500) do begin
      cpiread, finfo
      if numimages eq 0 then begin
         firstsecyear = secondsInYear(state.year, state.month, $
            finfo.day, finfo.hour, finfo.minute, finfo.second)
      endif
      numimages = numimages + 1
      secyear = secondsInYear(state.year, state.month, $
         finfo.day, finfo.hour, finfo.minute, finfo.second)
      if secyear gt (firstsecyear + 1) then begin
         fullscreen = 1
      endif else begin
         savelabel = string(format= '(I2.2, I2.2, I2.2, I3.3)', $
            finfo.hour, finfo.minute, finfo.second, finfo.msecond)
         savet = string(format= '(I2.2, ":", I2.2, ":", I2.2, ".", I3.3)', $
            finfo.hour, finfo.minute, finfo.second, finfo.msecond)
      endelse
      if finfo.totx gt 900 then checkx = 900 else checkx = finfo.totx
      if (savex + checkx) gt 900 then begin
         savey = savey + levelmaxy + 1
         savex = 0
         levelmaxy = 0
      endif
      if (finfo.toty gt levelmaxy) then levelmaxy = finfo.toty
      roidata = bytarr(finfo.totx, finfo.toty)
      readu, state.indata, roidata
      if (x ge savex) and (x le savex + checkx + 1) and $
            (y ge savey) and (y le (savey + finfo.toty + 1)) then begin
         foundimage = 1
         if select eq 1 then begin
            image = string(format='(I3.3, ".png")', numimages)
            filename = state.pngdir + savelabel + image
            state.pngfile = savelabel + image
            write_png, filename, tvrd(savex,savey,finfo.totx,finfo.toty)
         endif else if select eq 4 then begin
            state.len = finfo.totx * 2.3
            state.wid = finfo.toty * 2.3
            state.timel = savet
         endif
      endif 
      savex = savex + checkx + 1 
   end
end
