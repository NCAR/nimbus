; Pavel Romashkin, PhD, NCAR RAF, 04-2013
;************************************
; This function reads 1001 file, creates data record and updates or initializes a Catalog.
; Argunments: SourceFile - file location as string or LUN number, Catalog -
; array of structures of type {Date:long, Time:long, Platform:string, Instrument:string,
; DataBlock:pointer}.
; The intent of this function is to read in the ozone photometers files, and since these are
; clean of errata, some error checking is skimped.

pro op_read_1001, SourceFile, Catalog, comment
compile_opt IDL2, obsolete

		; Determine if SourceFile is name or LUN.
	if ((size(SourceFile,/type)) ne 7) then Point_lun,SourceFile,0 else begin
		SourceFileStr=string(SourceFile)

		; SourceFile becomes file handle here.
		openr,SourceFile,SourceFileStr,/Get_lun,error=ErrCode

		if (ErrCode ne 0) then begin
			print,'Error locating or opening file. Try again.'
			return
		endif
	endelse

		; Create field information for catalogs using the name of exchange file.
	FileName = fstat(SourceFile)
	FileName = FileName.Name
	FileName = strmid( FileName, strpos(FileName, "\", /reverse_search)+1 )
		; Use the following naming convention: OP1_20130422_155645.dat
		; The instrument is always the first 3 characters of the filename.
	The_Instrument = strmid(FileName,0,3)
		; Date and time are just strings.
	The_Date = strmid(FileName,4,8)
	The_Time = strmid(FileName,13,6)
	The_Platform = 'Aircraft'
	The_Project = 'Airborne project'

		; At this point the file is open and cursor is set to its beginning.
		; Read header and extract information needed to read the file.
	readf,SourceFile,DataStart,FileCode ;Now it is 1 line down.

		; If file is not 1001, quit.
	if FileCode ne 1001 then begin
		print,'This is not 1001 file.'
		Free_lun,SourceFile
		return
	endif

	for i=0,6 do readf,SourceFile,format='()' ;Skip 7 lines of metadata

		; Get the name of independant variable.
	indepVar = ''
	readf,SourceFile,IndepVar ; Reading name of independant variable.

	N_PrimVars=0
	readf,SourceFile,N_PrimVars ; Read the number of primary variables.

		; Create FLTARRs of scale factors and missing values.
	ScaleFactors=fltarr(N_PrimVars)
	MissingValues=lonarr(N_PrimVars)

		; Read scale and missing values.
	readf,SourceFile,ScaleFactors,MissingValues

		; Read descriptions of variables and use as Long Names.
	PrimVarsNames=strarr(N_PrimVars)
	readf,SourceFile,PrimVarsNames
	PrimVarsNames=[IndepVar,PrimVarsNames] ; Add 1 column for independant variable (time).

		; Go to the beginning of data and try to read column headers just before data.
	Point_lun,SourceFile,0
	for i=0,DataStart-2 do readf,SourceFile,format='()'
	ColumnNames=''
	readf,SourceFile,ColumnNames
	ColumnNames=str_sep(strcompress(strtrim(ColumnNames,2)),32b)

		; Use short PrimVarsNames if available.
	if (N_elements(ColumnNames) ne N_PrimVars+1) then begin
		print, 'Mismatch in the number of variables in metadata and data. Exiting.'
		free_lun, SourceFile
		return
	endif

	on_ioError,ResizeBuffer
		; Read data from the file. In case of big file, read by 1000 rows.
	Buffer=fltarr(N_PrimVars+1) ; Because column 0 is always time.
	FileTooShort=1

	readf,SourceFile,Buffer
	ResultArray=Transpose(Buffer)
	Buffer=fltarr(N_PrimVars+1,1000)

	TryAgain:
	while not EOF(SourceFile) do begin
		point_lun,(-SourceFile),LastPos ;Memorize start in case it fails.
		readf,SourceFile,Buffer
		ResultArray=[ResultArray,Transpose(Buffer)]
	endwhile

	free_lun,SourceFile ; Done using the file.
	Buffer=0 ; Empty Buffer as it may be large.

		; All data read into ResultArray are variable vectors.
		; Replacing missing values with Nan and apply scale factors.
	for i=1,N_PrimVars do begin
		Buffer=ResultArray[*,i] ; Go through one variable at a time.
		TempArray=where(long(Buffer) eq MissingValues[i-1])
		if (size(TempArray,/n_dim) ne 0) then Buffer[TempArray]=!Values.f_nan
		Buffer=temporary(Buffer)*ScaleFactors[i-1]
		ResultArray[*,i]=Buffer
	endfor

	TempArray=0
	Buffer=0

		; Update Catalog
	op_catalog_update, Catalog, FileName, The_Date, The_Time,The_Platform,The_Project,The_Instrument, ColumnNames, PrimVarsNames, ResultArray, comment

	;======= Buffer adjustment done here =========
	FileTooShort=0 ; In a normal run, skip this section.
	ResizeBuffer:
	if FileTooShort eq 1 then begin
		; This is to decrease buffer size when reading long files:
		Point_Lun,SourceFile,LastPos ; Go back where it failed, make Buffer 1 row in size.
		Buffer=fltarr(N_PrimVars+1,(size(Buffer))[2]/10)
		goto, TryAgain
	endif
	;======= Buffer adjustment done here =========

end

;************************************
; Update the catalog of instrument runs. The catalog is an array of structures, each of them being a separate instrument data file.

pro op_catalog_update, Catalog, FileName, The_Date, The_Time,The_Platform,The_Project,The_Instrument, varNames, varNamesLong, The_DataBlock, comment
compile_opt IDL2, obsolete

		; Establish a prototype structure.
	tempStruct = {fileName:'', Date:'', Time:'', Platform:'', Project: '', Instrument:'', Comment: '', varNames: ptr_new(), $
		varNamesLong: ptr_new(), DataBlock:ptr_new()}

		; First, if Catalog does not exist yet, create it.
	if (size(Catalog,/type) ne 8) then Catalog=tempStruct

		; See if same record exists. If yes, free heap variable and update DataBlock pointer. If no, make new record.
	RecLocation=where( (Catalog.Date eq The_Date) and (Catalog.Platform eq The_Platform) and (Catalog.Instrument eq The_Instrument) $
		and (Catalog.Time eq The_Time) )

		; Check if there are any duplicate records. Delete all duplicates if found.
	if (N_elements(RecLocation) gt 1) then begin
		for i=1,N_elements(RecLocation) do begin
			for j=0, n_tags(Catalog[i])-1 do begin ; Loop through the structure
					if size(Catalog[i].(j)) eq 10 then begin
						ptr_free, Catalog[i].(j) ; Free pointers
						Catalog[i].(j) = ptr_new()
					endif
					if size(Catalog[i].(j)) eq 7 then Catalog[i].(j) = '' ; Reset strings
			endfor
		endfor
	endif

	if (RecLocation[0] ne -1) then begin ; Replace the data in the existing record. Something was updated.
		for j=0, n_tags(Catalog[RecLocation[0]]) do begin
				if size(Catalog[RecLocation[0]].(j)) eq 10 then ptr_free, Catalog[RecLocation[0]].(j) ; Free pointers
		endfor
		Catalog[RecLocation[0]].DataBlock = ptr_new(The_DataBlock)
		Catalog[RecLocation[0]].varNames = ptr_new(varNames)
		Catalog[RecLocation[0]].varNamesLong = ptr_new(varNamesLong)
		if arg_present(comment) then Catalog[RecLocation[0]].Comment = comment
	endif else begin ; Create new record.
		NewRecord=tempStruct
		NewRecord.fileName = FileName
		NewRecord.Date = The_Date
		NewRecord.Time = The_Time
		NewRecord.Platform = The_Platform
		NewRecord.Instrument = The_Instrument
		NewRecord.Project = The_Project
		NewRecord.DataBlock = ptr_new(The_DataBlock)
		NewRecord.varNames = ptr_new(varNames)
		NewRecord.varNamesLong = ptr_new(varNamesLong)
		if arg_present(comment) then NewRecord.Comment = comment

		EmptyRecLoc = (where(Catalog.Date eq 0))[0] ; If Catalog has an empty slot, use it.

		if (EmptyRecLoc eq -1) then Catalog=[Catalog, NewRecord] else $
			Catalog[EmptyRecLoc]=NewRecord
	endelse
end

;************************************
; Display ozone data from catalog files. Catalogs are formed from the 1001 files rerocded with the C source code,
; Effective date for the format and source code change is 04-22-2013.

pro op_view, inData, norm=norm, sps=sps
compile_opt IDL2, obsolete

	if n_elements(sps) eq 0 then sps = 10
	x = op_GetVar(inData, 'Tsecs')

	; This is the index of valve transition times, assuming that 2 samples per 10s periods are lost to valve transition,
	; and that sample rate is 1/s. This only holds true to low rate data (original PSI and Proffitt formats).
	;if sps eq 10 then begin
	;	trans = where(inData.vq - shift(inData.vq, 12) ne 0, complement=ind_good) ; For 10 sps it is observed that 12 samples are lost to valve switch.
	;endif else trans = where((inData.vq - shift(inData.vq, 2)) ne 0, complement=ind_good)

	cnt_a = op_GetVar(inData, 'CountA')
	cnt_b = op_GetVar(inData, 'CountB')

	offset = mean(cnt_a) - mean(cnt_b)

	zero_chB = where((op_GetVar(inData, 'ValvePos') mod 2) ne 0, complement=zero_chA)

	mainTitle = inData.Filename

	op_DisplayVar, inData, 'Tsecs', 'CountA'

	if keyword_set(norm) then begin
		display, /over, x, cnt_b + offset, color=[255,0,0], name='Chan. B', ytitle='Counts - Normalized'
		;display, /over, x[zero_chB], inData.cnt_b[zero_chB] + offset, psym=4, symsize=0.4, color=[250,0,0], linestyle=6, name='Ch. B Zero'
	endif else begin
		op_DisplayVar, inData, 'Tsecs', 'CountB', /over, color=[255,0,0], ytitle='Counts'
		;display, /over, x, cnt_b, color=[255,0,0], name='Chan. B', ytitle='Counts'
		;display, /over, x[zero_chB], inData.cnt_b[zero_chB], psym=4, symsize=0.4, color=[250,0,0], linestyle=6, name='Ch. B Zero'
	endelse

	ratio = cnt_a/cnt_b
	display, x, ratio, name='A/B Ratio', title=mainTitle, ytitle='A/B ratio', oref=ratio_disp, xsize=1200, /legend;, psym=4
	;display, /over, x[trans], ratio[trans], name='Transition points', psym=4, color=[255,0,0], linestyle=6, oref=ratio_disp

	op_DisplayVar, inData, 'Tsecs', 'PressA', oref=PressDisplay
	op_DisplayVar, inData, 'Tsecs', 'PressB', oref=PressDisplay, /over, color=[255,0,0], ytitle='Pressure, mbar'

	;return

	op_DisplayVar, inData, 'Tsecs', 'iSleep'
	op_DisplayVar, inData, 'Tsecs', 'ValvePos'

	return

	for i=13, 18 do begin ;Housekeeping Temperatures
		red = (i mod 2) * 250
		green = ((i mod 3) eq 0) * 250
		blue = ((i mod 2) ne 1) * 50
		display, x, inData.(i), name=inData.desc[i], /legend, over=(i ne 13), color=[red,green,blue], title=mainTitle, xtitle='Cycle #', ytitle='Temperature', xsize=1200
	endfor

	;show_fft, inData.cnt_a, name='Counts A', /legend, title=mainTitle, xtitle='1 / F'
	;show_fft, inData.cnt_b, /over, color=[255,0,0], name='Counts B', yrange=[0,500], xrange=[0,0.5]

end

;************************************

function op_GetVar, inData, varName
compile_opt IDL2, obsolete

	temp = (where(*inData.varNames eq varName))[0]
	return, (*inData.DataBlock)[*, temp]

end
;************************************

pro op_DisplayVar, inData, xVarName, yVarName, _extra=_extra
compile_opt IDL2, obsolete

	display, op_GetVar(inData, xVarName), op_GetVar(inData, yVarName), name=yVarName, /legend, xtitle=xVarName, ytitle=yVarName, title=inData.fileName, xsize=1200, _extra=_extra

end
