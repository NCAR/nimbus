; Routine for reading the RAF Ozone instrument data in the PSI format,
; FName is the name of the input data file.
; to read Proffitt format, simply use Read_ASCII_Columns.

pro Read_Ozone_PSI, FName, data_out
compile_opt IDL2, obsolete

i = 1L

line1 = ''
line2 = ''
blank_str = ''

press = fltarr(2)
signals = fltarr(2)
temps = fltarr(11)
voltages = fltarr(9)
misc_data = fltarr(6) ; Current, valve stats, CPU usage
temp_buffer = fltarr(4)

data_out = {chA:0.0, chB: 0.0, press: press, temps: temps, volts: voltages, misc: misc_data}
data_out = replicate(data_out, 234000)

on_ioError, out

openr, SourceFile, /get_lun, FName
for i = 0, 9 do Readf, SourceFile, line1 ; Skip the first record.

i = 0L

while not EOF(SourceFile) do begin

;Readf, SourceFile, line1 ; Skip the blank line
Readf, SourceFile, line1 ; Date and time, cycle number
Readf, SourceFile, line2 ; Time elapsed
Readf, SourceFile, press
Readf, SourceFile, signals

date_str = strsplit(line1, " ", /extract)

temps[*] = !values.f_nan
temp_buffer[*] = !values.f_nan

if long(date_str[2]) mod 5 eq 0 then begin ;Need to convert to Long, or it will crash at 32,800.
	Readf, SourceFile, temps
	Readf, SourceFile, voltages
	Readf, SourceFile, misc_data ; Read the extended data every 5 records.
	data_out[i].misc = misc_data
	data_out[i].volts = voltages
	Readf, SourceFile, blank_str ; Skip the blank line at the end of record.
endif else begin
	Readf, SourceFile, temp_buffer
	temps[0:3] = temp_buffer
endelse

data_out[i].chA = signals[0]
data_out[i].chB = signals[1]
data_out[i].press = press
data_out[i].temps = temps

i = i + 1

endwhile

out :
print, 'Finished successfully.', i

data_out = data_out[0: i-1]

free_lun, SourceFile
end

;============================================
;Read O3 data from RW and HK files, combine all data in one file.

pro read_o3, out
compile_opt IDL2, obsolete

fname_rw = dialog_pickfile(file='*.*R')
len = strlen(fname_rw)
fname_hk = strmid(fname_rw, 0, len-1) + 'H'

	read_ascii_columns, fname_rw, rw, /struct
	read_ascii_columns, fname_hk, hk, /struct
	f_name = strmid(fname_rw, len - 12)
	if f_name lt '12080214.RW5' then begin ; Some older files may not have the COMP_T column in the RW file.
		rw = create_struct('COMP_T', hk.COMP_T, rw)
	endif else $
		if f_name lt '13010208.52R' then begin ; Some older files may not have the COMP_T column in the RW file.
			struct_delete_field, hk, 'COMP_T'
		endif

	; New naming has COMP_OP1 or COMP_OP2 in the first column name. It is present in both R and H files. Delete one of them.
	struct_delete_field, hk, field_num=0

	temp_str = define_data_fields()
	; Retrieve instrument name and add it to file name. Instrument name is recorded in the COMP_* column of data files.
	instr_name = strmid((tag_names(rw))[0],5,4)
	out = create_struct('F_NAME', instr_name + '_' + f_name, 'DESC', temp_str, rw, hk)
end

;============================================
;Data field definitions for the ozone 1s column data files.

function define_data_fields
compile_opt IDL2, obsolete
temp = ['File name', $
	'Descriptive names', $
	'Time stamp', $
	'Cycle number', $
	'Counter A', $
	'Counter B', $
	'Deta P', $
	'Abs P', $
	'Baratron P', $
	'Temp A in', $
	'Temp B in', $
	'Temp A out', $
	'Temp B out', $
	'Temp lamp', $
	'Temp detector', $
	'Temp cell', $
	'Temp fan', $
	'Temp PS plate', $
	'Temp vent', $
	'Temp PC-104', $
	'+5V Ref', $
	'+5V power', $
	'+28V', $
	'+24V', $
	'-24V', $
	'Nothing-1', $
	'Nothing-2', $
	'+12V', $
	'+5V PC-104 supply', $
	'Current 28V', $
	'Valve position', $
	'Valve expected position', $
	'Valve skips', $
	'Valve stops', $
	'CPU usage']
return, temp
end

;============================================
;Display data from a PSI data file structure.

pro show_o3_PSI, in_var, no_shift=no_shift
compile_opt IDL2, obsolete

offset = mean(in_var.chA) - mean(in_var.chB)

if keyword_set(no_shift) then begin
	display, in_var.chA, name='Chan. A', title = "NOT OFFSET CORRECTED"
endif else begin
	display, in_var.chA - offset, name='Chan. A', title = "OFFSET CORRECTED"
endelse
display, in_var.chB, /over, color=[255,0,0], name="Chan. B", /legend

good = where(finite(in_var.temps[9], /nan) ne 1)

;Temperatures, high rate
display, in_var.temps[0], name='Cell A In', /legend
display, in_var.temps[1], name='Cell B In', /over, color=[250, 50, 0]
display, in_var.temps[2], name='Cell A Out', /over, color=[50, 250, 0]
display, in_var.temps[3], name='Cell B Out', /over, color=[0, 50, 250], yrange=[32,36]

;Temperatures, low rate
display, good, in_var[good].temps[5], name='Detector', /legend, color=[250, 0, 0]
display, good, in_var[good].temps[4], name='Lamp', /over, color=[150, 0, 250]
display, good, in_var[good].temps[6], name='Scrubber', /over, color=[250, 50, 200]
display, good, in_var[good].temps[7], name='Fan', /over, color=[0, 200, 0]
display, good, in_var[good].temps[8], name='PS Panel', /over, color=[0, 10, 250]
display, good, in_var[good].temps[9], name='Box', /over, color=[250, 250, 0]
display, good, in_var[good].temps[10], name='CPU', /over, color=[20, 20, 180], yrange=[20, 36]

;Voltages
display, good, in_var[good].volts[0], name='5V Reference', /legend
display, good, in_var[good].volts[1], name='5V PS', /over, color=[250, 50, 0]
display, good, in_var[good].volts[2], name='+28V', /over, color=[150, 0, 150]
display, good, in_var[good].volts[3], name='+24V', /over, color=[250, 50, 0]
display, good, in_var[good].volts[4], name='-24V', /over, color=[50, 50, 250]
display, good, in_var[good].volts[7], name='+12V', /over, color=[50, 250, 0]
display, good, in_var[good].volts[8], name='5V PC-104', /over, color=[150, 50, 110]

;Pressures
display, good, in_var[good].press[0], name='Delta P', /legend, color=[50, 50, 250]
display, good, in_var[good].press[1], name='Ambient P', /over, color=[250, 50, 0]

;Current
display, good, in_var[good].misc[0], name='PS Current', /legend


;IDL> display, /over, yrange=[1213300, 1213400]
;IDL> display, /over, yrange=[1038150, 1038250]
;IDL> display, /over, xrange=[12000, 18000]
;IDL> display, /over, xrange=[12000, 18000]

;IDL> a_shift = data_0519_460.cha - shift(data_0519_460.cha, 1)
;IDL> b_shift = data_0519_460.chb - shift(data_0519_460.chb, 1)
;IDL> display, a_shift, name='A-Shift'
;IDL> display, b_shift, name='B-Shift', /over, /legend, color=[250,0,0]

;IDL> shift_v3 = db1v3_batt_cha.cha - shift(db1v3_batt_cha.cha, 1)
;IDL> shift_v4 = db2v4_batt_chb.chb - shift(db2v4_batt_chb.chb, 1)

;This is the FFT spectrum of the time series:
;display, fft(db1v3_batt_cha[good].cha-mean(db1v3_batt_cha[good].cha))*conj(fft(db1v3_batt_cha[good].cha-mean(db1v3_batt_cha[good].cha)))

;This is the FFT spectrum of the residuals:
;display, fft(shift_v3[good])*conj(fft(shift_v3[good]))

end

;============================================
; Display ozone data from the 1-s xxR and xxH columnar files.

pro show_o3, raw, norm=norm, sps=sps
compile_opt IDL2, obsolete

if n_elements(sps) eq 0 then sps = 1.0
x = raw.cycle_no / sps

;This is the index of valve transition times, assuming that 2 samples per 10s periods are lost to valve transition,
;and that sample rate is 1/s. This only holds true to low rate data (original PSI and Proffitt formats).
if sps eq 10 then begin
	trans = where(raw.vq - shift(raw.vq, 12) ne 0, complement=ind_good) ; For 10 sps it is observed that 12 samples are lost to valve switch.
endif else trans = where((raw.vq - shift(raw.vq, 2)) ne 0, complement=ind_good)

offset = mean(raw.cnt_a) - mean(raw.cnt_b)

zero_chB = where((raw.vq mod 2) ne 0)
zero_chA = where((raw.vq mod 2) eq 0)

display, x, raw.cnt_a, name='Chan. A', title=raw.f_name, /legend, xtitle='Cycle #', xsize=1200

if keyword_set(norm) then begin
	display, /over, x, raw.cnt_b + offset, color=[255,0,0], name='Chan. B', ytitle='Counts - Normalized'
	;display, /over, x[zero_chB], raw.cnt_b[zero_chB] + offset, psym=4, symsize=0.4, color=[250,0,0], linestyle=6, name='Ch. B Zero'
endif else begin
	display, /over, x, raw.cnt_b, color=[255,0,0], name='Chan. B', ytitle='Counts'
	;display, /over, x[zero_chB], raw.cnt_b[zero_chB], psym=4, symsize=0.4, color=[250,0,0], linestyle=6, name='Ch. B Zero'
endelse

ratio = raw.cnt_a/raw.cnt_b
display, x, ratio, name='A/B Ratio', title=raw.f_name, oref=ratio_disp, xsize=1200;, psym=4
;display, /over, x[trans], ratio[trans], name='Transition points', psym=4, color=[255,0,0], linestyle=6, oref=ratio_disp

; return

display, x, raw.delp, name='Delta P', /legend, title=raw.f_name, xtitle='Cycle #', ytitle='Delta Pressure', xsize=1200
display, x, raw.pabs, name='Abs P', /legend, title=raw.f_name, xtitle='Cycle #', ytitle='Ambient Pressure', color=[0,0,255], xsize=1200

for i=9, 12 do begin ;In and Out Temperatures
	red = (i mod 2) eq 0
	display, x, raw.(i), name=raw.desc[i], /legend, over=(i ne 9), color=[255*red,0,0], title=raw.f_name, xtitle='Cycle #', ytitle='Temperature', xsize=1200
endfor

display, x, raw.i28v, name='28V current', color=[250,100,170], xsize=1200

for i=13, 18 do begin ;Housekeeping Temperatures
	red = (i mod 2) * 250
	green = ((i mod 3) eq 0) * 250
	blue = ((i mod 2) ne 1) * 50
	display, x, raw.(i), name=raw.desc[i], /legend, over=(i ne 13), color=[red,green,blue], title=raw.f_name, xtitle='Cycle #', ytitle='Temperature', xsize=1200
endfor

;show_fft, raw.cnt_a, name='Counts A', /legend, title=raw.f_name, xtitle='1 / F'
;show_fft, raw.cnt_b, /over, color=[255,0,0], name='Counts B', yrange=[0,500], xrange=[0,0.5]

end

;============================================
pro write_shifts, in_data1, in_data2, FName
compile_opt IDL2, obsolete

data_out = [[in_data1], [in_data2]]
data_out = transpose(data_out)
help, data_out
openw, DestFile, /get_lun, FName

printf, DestFile, data_out

free_lun, DestFile

end

;============================================

;IN_VAR is the input variable; GOOD is the index to limit that variable to.
;Assume 1s sampling period, which limits detectable frequency to 2 Hz.
;KEYWORDS:
;	SS - set to show FFT of second-to-second differences. This can sometimes emphasize the oscillating signal.

pro show_fft, in_var, good, ss=ss, _extra=_extra
compile_opt IDL2, obsolete

length = (n_elements(good))[0]
if length eq 0 then begin
	length = (n_elements(in_var))[0]
	good = lindgen(length)
endif

timebase = findgen(length) *  1./length
display, timebase, fft(in_var[good]-mean(in_var[good]))*conj(fft(in_var[good]-mean(in_var[good]))), _extra=_extra
display, /over, xtitle='1/F';,xrange=[0,0.5]

if keyword_set(ss) then begin
	diff = in_var[good] - shift(in_var[good], 1)
	display, timebase, fft(diff)*conj(fft(diff))
endif
end

;============================================
; Duplicate the PSI ratiometric and single channel calculation algorithms. I suspect that these are flawed but want to verify.

pro calc_o3_psi, in_var, index, mrs=mr, mra=mr_a, mrb=mr_b
compile_opt IDL2, obsolete

Sigma = 1.147e-17		; absorption cross section in cm2
Path = 20.				; optical path length in cm
Aconst = 1.3802E-10		; conversion to mixing ratio in ppbv
Kelvin = 273.15			; convert deg C to K

offset_a = 0.0
offset_b = 0.0

; If valid subset is not given, use all data points.
if n_elements(index) eq 0 then index = lindgen(n_elements(in_var.cycle_no))
len = n_elements(index)

; Subscript to valid values only
temp_A = (in_var.ain[index] + in_var.aout[index])/2. + Kelvin
temp_B = (in_var.bin[index] + in_var.bout[index])/2. + Kelvin
Press = in_var.pabs[index]
cha = in_var.cnt_a[index]
chb = in_var.cnt_b[index]
x = in_var.cycle_no[index]
vq = in_var.vq[index]

;Establish valve transitions and create indeces for the zeros on each channel.
trans = where((vq - shift(vq, 1)) ne 0) ;This is the index of valve moves.
zero_chb = where((vq mod 2) ne 0)
zero_cha = where((vq mod 2) eq 0)

Temperature = temp_A ; Initialize total temperature; this is T when A is in Sample mode.
Temperature[zero_cha] = temp_B[zero_cha] ; Set T when B is in sample mode to temp_B.

ratio = cha / chb ; This is ratio of the two channels.

len_trans = n_elements(trans)
mult_R = findgen(5) + 3 ; These are factors to multiply data by to calc linear fits in the loop.
mult_L = findgen(5) + 2

; ======= RATIOMETRIC TWO CHANNEL =========
fcal = fltarr(len)
fcal[*] = !values.f_nan

for i = 2, len_trans-1 do begin
	IP = ratio[trans[i]+2 : trans[i]+6]
	sumYright = total(IP, /double)
	sumXYright = total(IP * mult_R)
	sloperight = (5. * sumXYright - 25. * sumYright) / 50.
	interceptright = (135. * sumYright - 25. * sumXYright) / 50.

	IM = reverse(ratio[trans[i]-5 : trans[i]-1])
	sumYleft = total(IM)
	sumXYleft = total(IM * mult_L)
	slopeleft = (5. * sumXYleft - 20. * sumYleft) / 50.
	interceptleft = (90. * sumYleft - 20. * sumXYleft) / 50.

	fcalright = sloperight + interceptright
	fcalleft = slopeleft + interceptleft

	if fcalright lt 0 then fcalright = 1.
	if fcalleft lt 0. then fcalleft = 1.

	fcal[trans[i]] = sqrt(fcalright * fcalleft) ; We will form the entire array, then interpolate to 1s time base.
	ratio[trans[i]] = fcalright ; PSI fills in the switch point and the next point to interpolated values.
	ratio[trans[i]+1] = fcalright + sloperight ; We do this too, here.
endfor

fcal = interpol(fcal[trans], x[trans], x) ; Inerpolate FCAL to 1s time base.

absorp = fltarr(len)
absorp[*] = !values.f_nan

absorp[zero_chb] = alog(fcal[zero_chb]/ratio[zero_chb])
absorp[zero_cha] = alog(ratio[zero_cha]/fcal[zero_cha])

display, x, ratio, name='A/B', psym=4, symsize=0.05, linestyle=6, xsize=1200, ysize=800, color=[0,0,0], title = in_var.f_name, oref=tser
display, /over, x, fcal, psym=5, symsize=0.05, color=[255,0,0], linestyle=6, name='Fcal', /legend, oref=tser

mr = aconst/sigma/path*Temperature/Press*absorp
display, x, mr, psym=4, symsize=0.05, linestyle=6, xsize=1200, ysize=800, yrange=[-20.,1000.], title = in_var.f_name, $
	name='MR PSI Ratiometric', oref=mixrat

; ======= END RATIOMETRIC TWO CHANNEL =========

; ======= SINGLE CHANNEL =========
fzeroa = cha ; Initialize channel zeros.
fzeroa[*] = !values.f_nan
fzerob = fzeroa

for i=2, len_trans-1 do begin
	IPa = cha[trans[i]+2 : trans[i]+6]
	IPb = chb[trans[i]+2 : trans[i]+6]
	IMa = reverse(cha[trans[i]-5 : trans[i]-1])
	IMb = reverse(chb[trans[i]-5 : trans[i]-1])

	if where(trans[i] eq zero_chb) eq -1 then begin ; A is in sample mode for next 10s.
		sumYright = total(IPa)
		sumXYright = total(mult_R * IPa)
		sumYleft = total(IMb)
		sumXYleft = totaL(mult_L * IMb)
		slopeleft = (5.*sumXYleft - 20.*sumYleft)/50.
		interceptleft = (90.*sumYleft - 20.*sumXYleft)/50.
		sloperight = (5.*sumXYright - 25.*sumYright)/50.
		interceptright = (135.*sumYright - 25.*sumXYright)/50.
		fzeroa[trans[i]] = sloperight + interceptright
		fzerob[trans[i]] = slopeleft + interceptleft
		fzeroa[trans[i]+1] = 2.*sloperight + interceptright
	endif else begin ; B is in sample mode for next 10s.
		sumYright = total(IPb)
		sumXYright = total(mult_R*IPb)
		sumYleft = total(IMa)
		sumXYleft = total(mult_L*IMa)
		slopeleft = (5.*sumXYleft - 20.*sumYleft)/50.
		interceptleft = (90.*sumYleft - 20.*sumXYleft)/50.
		sloperight = (5.*sumXYright - 25.*sumYright)/50.
		interceptright = (135.*sumYright - 25.*sumXYright)/50.
		fzeroa[trans[i]] = slopeleft + interceptleft
		fzerob[trans[i]] = sloperight + interceptright
		fzerob[trans[i]+1] = 2.*sloperight + interceptright
	endelse
endfor

; Transition points are not just one point valve switches; data are bad for two points. Replace them with NaN.
junk = where((vq - shift(vq, 2)) ne 0)

fzeroa = interpol(fzeroa[trans], x[trans], x) ; Interpolate fzeroa and fzerob to 1s timebase.
fzerob = interpol(fzerob[trans], x[trans], x)

display, x, cha, name='ChA', psym=4, symsize=0.05, linestyle=6, xsize=1200, ysize=800, color=[0,0,0], title = in_var.f_name
display, /over, x, fzeroa, psym=5, symsize=0.05, color=[0,0,255], linestyle=6, name='FzeroA_int', /legend
display, /over, x[trans], fzeroa[trans], psym=5, symsize=0.05, color=[255,0,0], linestyle=6, name='FzeroA', /legend

mr_a = aconst/sigma/path*Temperature/Press*alog(fzeroa/cha)
mr_b = aconst/sigma/path*Temperature/Press*alog(fzerob/chb)

mr_a[junk] = !values.f_nan
mr_b[junk] = !values.f_nan
mr_a[zero_cha] = !values.f_nan ; Delete the data when the channel was in zero mode
mr_b[zero_chb] = !values.f_nan ; Delete the data when the channel was in zero mode

display, /over, oref=mixrat, x[zero_chb], mr_a[zero_chb], psym=4, symsize=0.05, linestyle=6, name='mr_A', color=[0,0,255], /keep
display, /over, oref=mixrat, x[zero_cha], mr_b[zero_cha], psym=4, symsize=0.05, linestyle=6, name='mr_B', color=[255,0,0], /keep
; ======= END SINGLE CHANNEL =========

cal_comp_o3, in_var.f_name, x, mr_a, mr_b, offset_a, offset_b

; General algorithm formulation is below:
;conc = ln(Zero/Signal)/PATH/SIGMA
;o3_mixratio = conc * ACONST * Temperature/Press
end

;============================================

; Calculate ozone mixing ratios. In_Var is the input variable generated by read_o3; index is an optional subset of the input data.
; Set MR keywords to variable that will contain the mixing ratios from the two channels or combined mr from both channels..

pro Calc_o3, in_var, index, mrs=mr, mra=mr_a, mrb=mr_b
compile_opt IDL2, obsolete

Sigma = 1.147e-17		; absorption cross section in cm2
Path = 20.				; optical path length in cm
Aconst = 1.3802E-10		; conversion to mixing ratio in ppbv
Kelvin = 273.15			; convert deg C to K

; If valid subset is not given, use all data points.
if n_elements(index) eq 0 then index = lindgen(n_elements(in_var.cycle_no))
len = n_elements(index)

; Subscript to valid values only
temp_A = (in_var.ain[index] + in_var.aout[index])/2. + Kelvin
temp_B = (in_var.bin[index] + in_var.bout[index])/2. + Kelvin
Press = in_var.pabs[index]
cha = in_var.cnt_a[index]
chb = in_var.cnt_b[index]
x = in_var.cycle_no[index]
vq = in_var.vq[index]

; Consider possibility of constant offsets in the channels, due to dark current or non-UV light hitting the detector.
offset_a = 0.
offset_b = 0.
;offset_b = 103000. ; This offset was valid for data collected on OP-2 prior to 09-27-2012.
;Offset is about 2,000 on ChA on OP-1 and OP-2; 1800 on ch B OP-1; and was negative on OP-1 ChB prior to 10-01-2012.
cha = cha - offset_a ; Remove constant offsetts.
chb = chb - offset_b

;Establish valve transitions and create indeces for the zeros on each channel.
trans = where((vq - shift(vq, 2)) ne 0) ;This is the index of valve transition times.
zero_chb = where((vq mod 2) ne 0)
zero_chb = set_difference(zero_chb, trans) ; This is the index of channel B zeros.
zero_cha = where((vq mod 2) eq 0)
zero_cha = set_difference(zero_cha, trans) ; This is the index of channel A zeros.

Temperature = temp_A ; Initialize total temperature; this is T when A is in Sample mode.
Temperature[zero_cha] = temp_B[zero_cha] ; Set T when B is in sample mode to temp_B.

; Create a reference zero.
z_a = cha[zero_cha] ; Zeros for chan A.
z_b = chb[zero_chb] ; Zeros for chan B.
x_za = x[zero_cha] ; This is the time stamps for the zeros, in ChA and ChB respsctively.
x_zb = x[zero_chb]

ind_chunka = where(zero_cha-shift(zero_cha, 1) ne 1) ; Index of contiguous zero point intervals of zeros of A
ind_chunkb = where(zero_chb-shift(zero_chb, 1) ne 1) ; Index of contiguous zero point intervals of zeros of B

; Find which channel measured zeros first.
cha_fisrt = zero_cha[0] lt zero_chb[0] ; This will be False if Chan B was first to measure zero.

synth_zero_cha = fltarr(len) ; Initialize the synthetic zero structure with two channels.
synth_zero_cha[*] = !values.f_nan ; Set it ti NAN so that it is easier to interpolate later by just filling in the NANs.
synth_zero_chb = synth_zero_cha
;synth_zero_chb[*] = !values.f_nan

;Show progress in channel A
cha_yrange = [0.9995, 1.0005] * median(z_a)
display, x, cha, psym=4, symsize=0.1, name='ChA', /legend, linestyle=6, oref=dis_cha, xsize=1500, ysize=800, yrange=cha_yrange, title = in_var.f_name

;Show progress in channel B
chb_yrange = [0.9995, 1.0005] * median(z_b)
display, x, chb, psym=4, symsize=0.1, name='ChB', /legend, linestyle=6, oref=dis_chb, xsize=1500, ysize=800, yrange=chb_yrange, title = in_var.f_name

; Chunk loop. This is to create synthetic zero line for the entire data set.
; First, find the length of the loop. Run the chunk loop until the shorter of the chunk pairs runs out.
finish = n_elements(ind_chunka) < n_elements(ind_chunkb)
for i = 0, finish-3 do begin ; -3 is because there needs to be a chunk on the right side of the one being looked at.

	; Data chunks defined.
	lchunk_a = z_a[ind_chunka[i] : ind_chunka[i+1]-1]
	rchunk_a = z_a[ind_chunka[i+1] : ind_chunka[i+2]-1]
	lchunk_b = z_b[ind_chunkb[i] : ind_chunkb[i+1]-1]
	rchunk_b = z_b[ind_chunkb[i+1] : ind_chunkb[i+2]-1]

	; Indices of the chunks defined.
	lx_a = x_za[ind_chunka[i] : ind_chunka[i+1]-1]
	rx_a = x_za[ind_chunka[i+1] : ind_chunka[i+2]-1]
	lx_b = x_zb[ind_chunkb[i] : ind_chunkb[i+1]-1]
	rx_b = x_zb[ind_chunkb[i+1] : ind_chunkb[i+2]-1]

	; Visualize chunks locations.
;	display, /over, oref=dis_cha, lx_a, lchunk_a, psym=6, color=[0,0,200], symsize=0.3, name='Lchunk_A', /keep, xtitle='Cycle #', ytitle='Counts'
;	display, /over, oref=dis_cha, rx_a, rchunk_a, psym=6, color=[200,0,0], symsize=0.3, name='Rchunk_A', /keep
;	display, /over, oref=dis_chb, lx_b, lchunk_b, psym=6, color=[0,0,200], symsize=0.3, name='Lchunk_B', /keep, xtitle='Cycle #', ytitle='Counts'
;	display, /over, oref=dis_chb, rx_b, rchunk_b, psym=6, color=[200,0,0], symsize=0.3, name='Rchunk_B', /keep

;	if i eq 5 then return

	; >>>>>>>> THIS IS CHANNEL A Synthetic Zeros <<<<<<<<
	coefs_a = linfit([lx_a,rx_a], [lchunk_a,rchunk_a], /double, yfit=calc_a)
	; Define what makes up the mid chunk depending on whether the zeros started with channel B or A.
	if cha_fisrt eq 0 then begin
		b_chunk = rchunk_b
		b_x = rx_b
	endif else begin
		b_chunk = lchunk_b
		b_x = lx_b
	endelse
	; Fit lines across side chunks
	; Scale response of B to value of A. Noise is presumably proportional to magnitude of signal.
	loc_midchunk = (where(x eq b_x[0]))[0] ; Create index for inserting mid chunk into synthetic_zero.
	scale = mean([lchunk_a,rchunk_a])/mean(b_chunk)
	b_chunk_scaled = b_chunk*scale
	; Detrend other channel...
	coefs_b = linfit(b_x, b_chunk_scaled, /double, yfit=calc_b)
	resid_b = b_chunk_scaled - calc_b
	; ...And move noise from other channel in between side chunks.
	mid_chunk = coefs_a[0] + coefs_a[1]*b_x + resid_b
	; Assign values to Cha Syn Zeros.
	synth_zero_cha[zero_cha[ind_chunka[i]]] = lchunk_a ; Simply make synthetic zero same as measured zero for side chunks...
	synth_zero_cha[zero_cha[ind_chunka[i+1]]] = rchunk_a
	synth_zero_cha[loc_midchunk] = mid_chunk ; ...and set it to zero from other channel re-trended to side chunks line fit.
	; >>>>>>>> END OF CHANNEL A Synthetic Zeros <<<<<<<<

	; Visualize CHA processing. Careful! This will visualize every increment, make sure loop is reduced to 5-10 iterations, or takes forever.
;	display, /over, [lx_a,rx_a], calc_a, name='A-calc', psym=1, symsize=0.1, color=[125,0,200], oref=dis_cha, /keep
;	display, /over, b_x, b_chunk_scaled, color=[0,200,250], name='B-raw', psym=6, symsize=0.1, oref=dis_cha, /keep
;	display, /over, b_x, calc_b, color=[125,200,250], name='B-calc', psym=1, symsize=0.1, oref=dis_cha, /keep
;	display, /over, b_x, mid_chunk, color=[130,20,10], psym=2, symsize=0.1, name='Mid chunk', linestyle=6, oref=dis_cha, /keep
;	display, /over, x, synth_zero_cha, symsize=0.1, color=[0,50,250], oref=dis_cha, name='SynthZero', /keep

	; >>>>>>>> THIS IS CHANNEL B Synthetic Zeros <<<<<<<<
	coefs_b = linfit([lx_b,rx_b], [lchunk_b,rchunk_b], /double, yfit=calc_b)
	; Define what makes up the mid chunk depending on whether the zeros started with channel B or A.
	if cha_fisrt eq 0 then begin
		a_chunk = lchunk_a
		a_x = lx_a
	endif else begin
		a_chunk = rchunk_a
		a_x = rx_a
	endelse
	; Fit lines across side chunks
	; Scale response of B to value of A. Noise is presumably proportional to magnitude of signal.
	loc_midchunk = (where(x eq a_x[0]))[0] ; Create index for inserting mid chunk into synthetic_zero.
	scale = mean([lchunk_b,rchunk_b])/mean(a_chunk)
	a_chunk_scaled = a_chunk*scale
	; Detrend other channel...
	coefs_a = linfit(a_x, a_chunk_scaled, /double, yfit=calc_a)
	resid_a = a_chunk_scaled - calc_a
	; ...And move noise from other channel in between side chunks.
	mid_chunk = coefs_b[0] + coefs_b[1]*a_x + resid_a
	; Assign values to Cha Syn Zeros.
	synth_zero_chb[zero_chb[ind_chunkb[i]]] = lchunk_b ; Simply make synthetic zero same as measured zero for side chunks...
	synth_zero_chb[zero_chb[ind_chunkb[i+1]]] = rchunk_b
	synth_zero_chb[loc_midchunk] = mid_chunk ; ...and set it to zero from other channel re-trended to side chunks line fit.
	; >>>>>>>> END OF CHANNEL B Synthetic Zeros <<<<<<<<

	; Visualize CHB processing. Careful! This will visualize every increment, make sure loop is reduced to 5-10 iterations, or takes forever.
;	display, /over, [lx_b,rx_b], calc_b, name='B-calc', psym=1, symsize=0.1, color=[125,0,200], oref=dis_chb, /keep
;	display, /over, a_x, a_chunk_scaled, color=[0,200,250], name='A-raw', psym=6, symsize=0.1, oref=dis_chb, /keep
;	display, /over, a_x, calc_a, color=[125,200,250], name='A-calc', psym=1, symsize=0.1, oref=dis_chb, /keep
;	display, /over, a_x, mid_chunk, color=[130,20,10], psym=2, symsize=0.1, name='Mid chunk', linestyle=6, oref=dis_chb, /keep
;	display, /over, x, synth_zero_chb, symsize=0.1, color=[0,50,250], oref=dis_chb, name='SynthZero', /keep
;	if i eq 7 then return

endfor

; Synthetic zero has still NANs at switch-over points. It will be complete after this last interpolation. Interpolation is done for CHA and CHB in turn.
junk = where(finite(synth_zero_cha, /nan))
good = where(finite(synth_zero_cha))
synth_zero_cha[junk] = interpol(synth_zero_cha[good], x[good], x[junk])
;display, /over, oref=dis_cha, x[junk], synth_zero_cha[junk], color=[250,0,0], psym=1, symsize=0.1, linestyle=6, /keep, name='SynthZero-filled'

junk = where(finite(synth_zero_chb, /nan))
good = where(finite(synth_zero_chb))
synth_zero_chb[junk] = interpol(synth_zero_chb[good], x[good], x[junk])

synth_zero_cha = smooth(synth_zero_cha, 5)
synth_zero_chb = smooth(synth_zero_chb, 5)

; Visualize the two synthetic zeros.
display, /over, x, synth_zero_cha, symsize=0.1, color=[0,50,250], psym=2, oref=dis_cha, name='SynthZero', /keep
display, /over, x, synth_zero_chb, symsize=0.1, color=[0,50,250], psym=2, oref=dis_chb, name='SynthZero', /keep
;display, /over, oref=dis_chb, x[junk], synth_zero_chb[junk], color=[250,0,0], psym=1, symsize=0.1, linestyle=6, /keep, name='SynthZero-filled'

; Crude calculation of absorbance ratios. This is done by shifting the time seties 10s to use the zeros following the data on the same channel.
;cha_ratio = (shift(cha, 10) / cha) ; These only include the ratio at the time respective channel was in Sample mode.
;chb_ratio = (shift(chb, 10) / chb)

cha_ratio = synth_zero_cha / cha
chb_ratio = synth_zero_chb / chb

; Calculate mixing ratios from absorbance ratios, T and P.
mr_a = fltarr(len) ; MR arrays are same length as X.
mr_a[*] = !values.f_nan
mr_b = mr_a
mr = fltarr(len)
mr[*] = !values.f_nan
mr_a[zero_chb] = aconst / sigma * (temp_A/Press) * alog(cha_ratio[zero_chb]) / path
mr_b[zero_cha] = aconst / sigma * (temp_B/Press) * alog(chb_ratio[zero_cha]) / path
mr[zero_chb] = mr_a[zero_chb]
mr[zero_cha] = mr_b[zero_cha]

;display, [0,1000], [0,1000], color=[0,0,255], name='1-1 line'
;display, /over, mr_a[ind], mr_b[ind], psym=4, symsize=0.1, linestyle=6

display, x, mr_a, psym=4, symsize=0.05, linestyle=6, xsize=1200, ysize=800, color=[255,0,0], yrange=[-20.,1000.], title = in_var.f_name
display, /over, x, mr_b, psym=4, symsize=0.05, linestyle=6, color=[0,0,255], /keep

cal_comp_o3, in_var.f_name, x, mr_a, mr_b, offset_a, offset_b

; General algorithm formulation is below:
;conc = ln(Zero/Signal)/PATH/SIGMA
;o3_mixratio = conc * ACONST * Temperature/Press

end

;============================================
; Linear function to use for fitting with MPFIT. P is a 2-element vector. Use both to fit and plot the fit.
function fit_line, p, x=x, y=y
	if n_elements(y) eq 0 then return, p[0] * X + P[1]
	p = double(p)
	return, y - (p[0] * X + P[1])
end
;============================================
; Calibration intercomparison experiment data are entered in here for specific dates when the instruments were run against the
; ThermoScientific calibrated ozone source.

pro cal_comp_o3, f_name, x, mr_a, mr_b, offset_a, offset_b
compile_opt IDL2, obsolete

; Create calibration source data for individual experiments.

if strpos(f_name, '121005') ne -1 then begin ; This one is for 10-05-12, OP-1.
	; These were the setpoints for the OP-1 10-05-12 experiment:
	refs = [0., 900., 800., 600., 200., 75.]
	; Setpoint ranges:
	ranges = { $
		MR0: [100, 1100], $
		MR900: [1350, 2350], $
		MR800: [3500, 4500], $
		MR600: [5200, 6200], $
		MR200: [6400, 7400], $
		MR75: [7800, 8800]}
endif else begin

if strpos(f_name, '121010') ne -1 then begin ; This one is for 10-10-12, OP-1 and OP-2 simultaneously.
	; These were the setpoints for the 10-10-12 experiment:
	refs = [0., 900., 800., 600., 400., 200., 150., 100., 75., 50., 25., 0]
	; Setpoint ranges for OP-2; Time base for OP-1 is exactly 1100s behind OP-2, this will be taken care of below.
	ranges_op2 = { $
		MR0: [4950, 7600], $
		MR900: [7700, 11100], $
		MR800: [11200, 13450], $
		MR600: [13550, 15350], $
		MR400: [15450, 18000], $
		MR200: [18100, 20300], $
		MR150: [20400, 22500], $
		MR100: [22700, 24200], $
		MR75: [24300,27000], $
		MR50: [27100, 28800], $
		MR25: [28900, 31000], $
		MR00: [31050, 31950]}

	ranges_op1 = ranges_op2
	for i=0, n_elements(refs)-1 do ranges_op1.(i) = ranges_op2.(i) - 1100 ; Time base for OP-1 is exactly 1100s behind OP-2.
	if strpos(f_name, 'OP-1') ne -1 then begin
		ranges = ranges_op1
		; There are some bad points in OP-1. Delete them.
		mr_a[21593 : 21610] = !values.f_nan
		mr_b[21593 : 21610] = !values.f_nan
		mr_a[23319 : 23320] = !values.f_nan
	endif else ranges = ranges_op2
endif else return
endelse

len = n_elements(refs)

means = fltarr(2,len)
sdevs = fltarr(2,len)

;display, x, mr_b, psym=4, linestyle=6, symsize=0.1, xsize=1100, ysize=800, oref=timeser, yrange=[0,500], xrange=[20000,27000]

mr_a[where(mr_a lt -500.)] = !values.f_nan
mr_b[where(mr_b lt -500.)] = !values.f_nan

for i = 0, len-1 do begin
	means[0,i] = mean(mr_a[ranges.(i)[0] : ranges.(i)[1]], /nan)
	means[1,i] = mean(mr_b[ranges.(i)[0] : ranges.(i)[1]], /nan)
	sdevs[0,i] = stddev(mr_a[ranges.(i)[0] : ranges.(i)[1]], /nan)
	sdevs[1,i] = stddev(mr_b[ranges.(i)[0] : ranges.(i)[1]], /nan)
;	display, /over, oref=timeser, /keep, x[ranges.(i)[0] : ranges.(i)[1]], mr_b[ranges.(i)[0] : ranges.(i)[1]], color=[255,0,0], $
;		psym=4, linestyle=6, symsize=0.1, name='Curr_chunk'
endfor

means_str = reform(strtrim(refs) + ' ' $
	+ strtrim(means[0,*]) + ' ' + strtrim(sdevs[0,*]) + ' ' $
	+ strtrim(means[1,*]) + ' ' + strtrim(sdevs[1,*]))
means_str = ['      Setpoint            Ch.A          Stdev.A          Ch.B          Stdev.B', means_str]

display, refs, means[0,*], psym=4, linestyle=6, symsize=0.2, xsize=1200, ysize=800, name='Cha', xtitle='Reference ozone, ppb', $
	ytitle='RAF OP ozone, ppb', title=f_name + ', offsets: '+strtrim(offset_a)+' '+strtrim(offset_b)

display, /over, refs, means[1,*], psym=5, color=[250, 50,0], linestyle=6, symsize=0.2, name='Chb', /legend

display, /over, [0,1000], [0,1000], color=[0,0,255], name='1-1 line'
display, /over, name='Cha', fit_name='Fit A', fit_func='fit_line', coefs_in=[2.0D, 0.5D], coefs_out=resulting_fit_coefs_A, /mp_fit
display, /over, name='Chb', fit_name='Fit B', fit_func='fit_line', coefs_in=[2.0D, 0.5D], coefs_out=resulting_fit_coefs_B, /mp_fit, color=[250,0,0]
display, /over, /static, /norm, text=means_str, name='Means', location=[0.65,0.5]

end

;============================================
;Print some statistics of the input ozone counts.
;SPS defines sample rate. If it is not 1 then transition index points are defined differently than in the
;old 1/s sample rate case when 2 points were being dropped per valve transition event.
;At 10/s it appears that 4 points are affected.

pro o3_stats, in_var, index, sps=sps
compile_opt IDL2, obsolete

if n_elements(index) eq 0 then index = lindgen(n_elements(in_var.(2)))
cha = in_var.cnt_a[index]
chb = in_var.cnt_b[index]

;Establish valve transitions and create indeces for the zeros on each channel.
vq = in_var.vq[index]

;This is the index of valve transition times, assuming that 2 samples per 10s periods are lost to valve transition,
;and that sample rate is 1/s. This only holds true to low rate data (original PSI and Proffitt formats).
if sps eq 10 then begin
	trans = where(raw.vq - shift(raw.vq, 12) ne 0, complement=ind_good) ; For 10 sps it is observed that 12 samples are lost to valve switch.
endif else trans = where((raw.vq - shift(raw.vq, 2)) ne 0, complement=ind_good)

;display, index, cha, psym=4, oref=trans_plot, title='Transition points', /legend, name='Channel A', xsize=1100
;display, /over, oref=trans_plot, index[trans], cha[trans], psym=4, linestyle=6, color=[255,0,0], name='Eliminated points A'
;display, /over, index, chb, psym=5, oref=trans_plot, name='Channel B'
;display, /over, oref=trans_plot, index[trans], chb[trans], psym=5, linestyle=6, color=[255,0,0], name='Eliminated points B'

cha[trans] = !values.f_nan
chb[trans] = !values.f_nan

junk = detrend(cha, cha_d, order=1)
junk = detrend(chb, chb_d, order=1)

; ========= Channel A =========
ss_a = (cha_d - shift(cha_d, 1))[1:*]
sigma_a = stddev(ss_a, /nan)
avg_a = mean(ss_a, /nan)
print, 'sec to sec Ch. A: stdev:', sigma_a, ', mean: ', avg_a

hist = histogram(ss_a, location=locs, binsize=1, /nan)
display, locs, hist, title=in_var.f_name, name='s-to-s A', /legend, oref=hist_disp, yrange=[0,50], xrange=[-60,60]

gf =gaussfit(locs, hist, coefs) ; Fit once to get reasonable estimates on coefs, then do actual fit.
gf =gaussfit(locs, hist, coefs, estimates=coefs)
print, coefs
fwhm_a = 2*SQRT(2*ALOG(2))*coefs[2]
display, /over, oref=hist_disp, locs, gf, linestyle=2, name='A-fit'

display, index[1:*], ss_a, name='Ch. A ss diffs', title=in_var.f_name, oref=trace_disp
; ========= End Channel A =========

; ========= Channel B =========
ss_b = (chb_d - shift(chb_d, 1))[1:*]
sigma_b = stddev(ss_b, /nan)
avg_b = mean(ss_b, /nan)
print, 'sec to sec Ch. B: stdev:',  sigma_b, ', mean: ', avg_b

hist = histogram(ss_b, location=locs, binsize=1, /nan)
display, locs, hist, title=in_var.f_name, name='s-to-s B', /legend, oref=hist_disp, /over, color=[255,0,0], /keep

;gf =gaussfit(locs, hist, coefs);, estimates=est)
fwhm_b = 2*SQRT(2*ALOG(2))*coefs[2]
gf =gaussfit(locs, hist, coefs, estimates=coefs)
print, coefs
display, /over, oref=hist_disp, locs, gf, linestyle=2, name='B-fit', color=[255,0,0], xrange=[-60,60]

display, /over, index[1:*], ss_b, color=[255,0,0], name='Ch. B ss diffs', /legend, oref=trace_disp, yrange=[-50,50]
; ========= End Channel B =========

; ========= Start Ratio ===========
ratio = cha / chb
print, 'Mean ratio: ', mean(ratio, /nan)
print, 'Stdev Ratio: ', stddev(ratio, /nan)
; ========= End Ratio =============

stat_str = 'Stats.      Chan. A      Chan. B'
stat_str = [stat_str, 'Avg.            ' + string(avg_a, format='(f6.2)') + '       ' + string(avg_b, format='(f6.2)')]
stat_str = [stat_str, 'Stdev.         ' + string(sigma_a, format='(f6.2)') + '       ' + string(sigma_b, format='(f6.2)')]
stat_str = [stat_str, 'Fwhm         ' + string(fwhm_a, format='(f6.2)') + '      ' + string(fwhm_b, format='(f6.2)')]
stat_str = [stat_str, 'Range:      ' + string(min(index, max=top)+1, format='(i6)') + '      ' + string(top+1, format='(i6)')]
display, /over, oref=hist_disp, text=stat_str, /static, location=[0.65,0.9]

end

;============================================
; Display valve position information for the housekeeping data file.

pro house_o3, in_var, index
compile_opt IDL2, obsolete

if n_elements(index) eq 0 then index = lindgen(n_elements(in_var.cycle_no))
cha = in_var.cnt_a[index]
chb = in_var.cnt_b[index]
x = in_var.cycle_no[index]
vq = in_var.vq[index]

;junk = detrend(cha, cha_d, order=1)
;junk = detrend(chb, chb_d, order=1)

trans = where((vq - shift(vq, 2)) ne 0) ;These are valve transition times.
zero_chb = where((vq mod 2) ne 0)
zero_chb = set_difference(zero_chb, trans)
zero_cha = where((in_var.vq mod 2) eq 0)
zero_cha = set_difference(zero_cha, trans)

display, x, in_var.vq, name='Valve Quad', /legend, xsize=1200, ysize=700, xtitle='Cycle #', ytitle='Valve position'
display, x, in_var.veq, name='Expected Quad', /over, color=[250,0,0]
display, x[1:*], (in_var.vsq-shift(in_var.vsq, 1))[1:*], name='Valve Skip', /over, color=[0,0,250]
display, in_var.vst-shift(in_var.vst, 1)-1, name='Valve Stop', /over, color=[0,250,0]

end

;============================================
; Display the bandpass function for the filter used in the OP instruments.

pro bp_filter
compile_opt IDL2, obsolete

cw = 254.
fwhm = cw * 0.05 ; Bandwidth is 5% of center wavelength.
sigma = fwhm / (2.*sqrt(2.*alog(2.)))

a = randomn(seed, 100000)*sigma + cw
h = histogram(a, locations=x, binsize=0.1)
g = gaussfit(x, h, coefs) ; Get rough initial coefficients
g = gaussfit(x, h, coefs, estimates=coefs)

;g = g / coefs[0] * 100.

display, x, g, name='Spectrum', xsize=1000, ysize=650, title='Barr BP filter performance', ytitle='%', xtitle='Wavelength, nm'
;display, /over, color=[250,0,0], x, g, name='Gauss'

fwhm = coefs[2] * (2.*sqrt(2.*alog(2.)))

display, /over, text=['FWHM: ' + strtrim(fwhm, 2), 'Center: ' + strtrim(cw, 2)], /static, location=[0.65,0.9]

;read_ascii_columns, 'D:\Pavel Documents\Ozone\Literature\xsect_248_260.txt', xsect, /struct


end

;============================================
;SPS is the sample per second scaling factor.
; This is a diagnostiv plotting function that was used when the instruments had double counts. The issue was eventually resolved
; by correcting the hardware latching operation.

pro dblcounts, in_var, sps
compile_opt IDL2, obsolete

if n_elements(sps) eq 0 then sps = 1.0

display, in_var.cycle_no/sps, in_var.pct, title=in_var.f_name, name='CPU usage', xsize=1000, psym=4, /legend
display, in_var.cycle_no/sps, in_var.cnt_a, name='Ch.A', oref=chans, xsize=1000, psym=4, /legend, title=in_var.f_name
display, /over, oref=chans, in_var.cycle_no/sps, in_var.cnt_b, name='Ch.B', color=[255,0,0], psym=4
end

;============================================
; Ozone mixing ratio calculation for single channel data referenced to a zero channel.

pro Calc_o3_RefChan, in_var, index, mrs=mr, mra=mr_a, mrb=mr_b
compile_opt IDL2, obsolete

Sigma = 1.147e-17		; absorption cross section in cm2
Path = 20.				; optical path length in cm
Aconst = 1.3802E-10		; conversion to mixing ratio in ppbv
Kelvin = 273.15			; convert deg C to K

; If valid subset is not given, use all data points.
if n_elements(index) eq 0 then index = lindgen(n_elements(in_var.cycle_no))
len = n_elements(index)

; Subscript to valid values only
temp_A = (in_var.ain[index] + in_var.aout[index])/2. + Kelvin
temp_B = (in_var.bin[index] + in_var.bout[index])/2. + Kelvin
Press = in_var.pabs[index]
cha = in_var.cnt_a[index]
chb = in_var.cnt_b[index]
x = in_var.cycle_no[index]
vq = in_var.vq[index]

;Establish valve transitions and create indeces for the zeros on each channel.
trans = where((vq - shift(vq, 12)) ne 0) ;This is the index of valve transition times for 10/s sample rate. Empirical value, 12 samples are wasted..
zero_chb = where((vq mod 2) ne 0)
zero_chb = set_difference(zero_chb, trans) ; This is the index of channel B zeros.
zero_cha = where((vq mod 2) eq 0)
zero_cha = set_difference(zero_cha, trans) ; This is the index of channel A zeros.

Temperature = temp_A ; Initialize total temperature; this is T when A is in Sample mode.
Temperature[zero_cha] = temp_B[zero_cha] ; Set T when B is in sample mode to temp_B.

; Create a reference zero.
z_a = cha[zero_cha] ; Zeros for chan A.
z_b = chb[zero_chb] ; Zeros for chan B.
x_za = x[zero_cha] ; This is the time stamps for the zeros, in ChA and ChB respsctively.
x_zb = x[zero_chb]

ind_chunka = where(zero_cha-shift(zero_cha, 1) ne 1) ; Index of contiguous zero point intervals of zeros of A
ind_chunkb = where(zero_chb-shift(zero_chb, 1) ne 1) ; Index of contiguous zero point intervals of zeros of B

; Find which channel measured zeros first.
cha_fisrt = zero_cha[0] lt zero_chb[0] ; This will be False if Chan B was first to measure zero.

synth_zero_cha = fltarr(len) ; Initialize the synthetic zero structure with two channels.
synth_zero_cha[*] = !values.f_nan ; Set it ti NAN so that it is easier to interpolate later by just filling in the NANs.
synth_zero_chb = synth_zero_cha

ab_ratio = 1.19705 ; This comes from a zero-definition experiment.
synth_zero_chb[zero_cha] = z_a / ab_ratio

; Synthetic zero has still NANs at switch-over points. It will be complete after this last interpolation. Interpolation is done for CHA and CHB in turn.
;junk = where(finite(synth_zero_cha, /nan))
;good = where(finite(synth_zero_cha))
;synth_zero_cha[junk] = interpol(synth_zero_cha[good], x[good], x[junk])
;display, /over, oref=dis_cha, x[junk], synth_zero_cha[junk], color=[250,0,0], psym=1, symsize=0.1, linestyle=6, /keep, name='SynthZero-filled'

junk = where(finite(synth_zero_chb, /nan))
good = where(finite(synth_zero_chb))
synth_zero_chb[junk] = interpol(synth_zero_chb[good], x[good], x[junk])

;synth_zero_cha = smooth(synth_zero_cha, 5)
;synth_zero_chb = smooth(synth_zero_chb, 5)

; Visualize the two synthetic zeros.
display, /over, x, synth_zero_cha, symsize=0.1, color=[0,50,250], psym=2, oref=dis_cha, name='SynthZero', /keep
display, /over, x, synth_zero_chb, symsize=0.1, color=[0,50,250], psym=2, oref=dis_chb, name='SynthZero', /keep
;display, /over, oref=dis_chb, x[junk], synth_zero_chb[junk], color=[250,0,0], psym=1, symsize=0.1, linestyle=6, /keep, name='SynthZero-filled'

; Crude calculation of absorbance ratios. This is done by shifting the time seties 10s to use the zeros following the data on the same channel.
;cha_ratio = (shift(cha, 10) / cha) ; These only include the ratio at the time respective channel was in Sample mode.
;chb_ratio = (shift(chb, 10) / chb)

;cha_ratio = synth_zero_cha / cha
chb_ratio = synth_zero_chb / chb

; Calculate mixing ratios from absorbance ratios, T and P.
mr_a = fltarr(len) ; MR arrays are same length as X.
mr_a[*] = !values.f_nan
mr_b = mr_a
mr = mr_a
;mr_a[zero_chb] = aconst / sigma * (temp_A/Press) * alog(cha_ratio[zero_chb]) / path
mr_b[zero_cha] = aconst / sigma * (temp_B/Press) * alog(chb_ratio[zero_cha]) / path
;mr[zero_chb] = mr_a[zero_chb]
;mr[zero_cha] = mr_b[zero_cha]

;display, [0,1000], [0,1000], color=[0,0,255], name='1-1 line'
;display, /over, mr_a[ind], mr_b[ind], psym=4, symsize=0.1, linestyle=6

display, x, mr_b, xsize=1200, ysize=800, color=[255,0,0], yrange=[-20.,1000.], title = in_var.f_name, name='Ch.B MR';, psym=4, symsize=0.05, linestyle=6
;display, /over, x, mr_a, psym=4, symsize=0.05, linestyle=6, color=[0,0,255], /keep

; General algorithm formulation is below:
;conc = ln(Zero/Signal)/PATH/SIGMA
;o3_mixratio = conc * ACONST * Temperature/Press

end

;============================================