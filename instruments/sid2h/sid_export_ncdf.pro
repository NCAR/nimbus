PRO sid_export_ncdf, data, outfile=outfile, ncappend=ncappend, finalprocessing=finalprocessing
   ;PRO to export a netCDF file with the variables contained in a data structure.
   ;Aaron Bansemer, NCAR, 9/2007
   ;Updated 7/2009 for ncpp compatibility
   ;Updated 4/2011 to append to existing ncdf file
   
   !quiet=1  ;Suppress annoying messages from ncdf routines
   
   ;Append SID data to existing ncdf file.
   ;This assumes that the data rate and start/stop times are
   ;already synced up with the preexisting file.
   IF (ncappend eq 1) THEN BEGIN
      suffix='_SID'   ;Suffix for appending to other files
      
      ;Open the file for writing
      id=ncdf_open(outfile[0],/write)
      
      ;Get pre-existing dimensions
      xdimid=ncdf_dimid(id,'Time')
      sdimid=ncdf_dimid(id,'sps1')
      
      ncdf_control,id,/redef                ;put in define mode
      
      ;Make new dimensions, this is for numbins
      name='Vector'+strtrim(string(n_elements(data.endbins)),2)
      ydimid_size=ncdf_dimid(id, name)
      IF ydimid_size eq -1 THEN ydimid_size=ncdf_dimdef(id,name,n_elements(data.endbins)) 
         
      ;This is for branch counts
      name='Vector8'
      ydimid_branches=ncdf_dimid(id, name)
      IF ydimid_branches eq -1 THEN ydimid_branches=ncdf_dimdef(id,name,8)
         
      IF not(finalprocessing) THEN BEGIN  ;Skip the rest of this for final processing
         ;This is for interarrival
         name='Vector'+strtrim(string(n_elements(data.intmidbins)),2)
         ydimid_int=ncdf_dimid(id, name)
         IF ydimid_int eq -1 THEN ydimid_int=ncdf_dimdef(id,name,n_elements(data.intmidbins)) 
         
         ;This is for asphericity
         name='Vector'+strtrim(string(n_elements(data.afmidbins)),2)
         ydimid_af=ncdf_dimid(id, name)
         IF ydimid_af eq -1 THEN ydimid_af=ncdf_dimdef(id,name,n_elements(data.afmidbins)) 
         
         ;This is for time-based size
         name='Vector'+strtrim(string(n_elements(data.tbendbins)),2)
         ydimid_tb=ncdf_dimid(id, name)
         IF ydimid_tb eq -1 THEN ydimid_tb=ncdf_dimdef(id,name,n_elements(data.tbendbins)) 
         
         varid=ncdf_varid(id, 'SID_ATTRIBUTES')
         IF varid eq -1 THEN varid=ncdf_vardef(id,'SID_ATTRIBUTES',/char)
         ncdf_control,id,/endef                ;put in data mode
         ncdf_varput,id,varid,0
         ncdf_control,id,/redef                ;return to define mode
         opnames=tag_names(data.op)                  
         FOR i=0,n_elements(opnames)-1 DO BEGIN
            IF string(data.op.(i)[0]) eq '' THEN data.op.(i)[0]='none' ;To avoid an ncdf error (empty string)
            IF size(data.op.(i)[0], /TYPE) eq 2 THEN BEGIN
                ncdf_attput,id,varid,opnames[i],data.op.(i)[0],/LONG
            ENDIF ELSE BEGIN
                ncdf_attput,id,varid,opnames[i],data.op.(i)[0]
            ENDELSE
         ENDFOR
      ENDIF  ;Final processing

   ENDIF ELSE BEGIN
      ;-----------Create new file instead-----------------
      
      suffix='_SID'
      ;Create the file
      id=ncdf_create(outfile[0],/clobber)
      
      
      ;Define the x-dimension, should be used in all variables
      xdimid=ncdf_dimdef(id,'Time',n_elements(data.time))
      ;Samples per second
      sdimid=ncdf_dimdef(id,'sps1',(1.0/data.op.rate)>1)
      ;This is for numbins
      name='Vector'+strtrim(string(n_elements(data.endbins)),2)
      ydimid_size=ncdf_dimdef(id,name,n_elements(data.endbins))
      ;This is for interarrival
      name='Vector'+strtrim(string(n_elements(data.intmidbins)),2)
      ydimid_int=ncdf_dimdef(id,name,n_elements(data.intmidbins))
      ;This is for asphericity
      name='Vector'+strtrim(string(n_elements(data.afmidbins)),2)
      ydimid_af=ncdf_dimdef(id,name,n_elements(data.afmidbins))
      ;This is for branch counts
      name='Vector8'
      ydimid_branches=ncdf_dimdef(id,name,8)
      ;This is for time-based size
      name='Vector'+strtrim(string(n_elements(data.tbendbins)),2)
      ydimid_tb=ncdf_dimdef(id,name,n_elements(data.tbendbins))
          
      ;These are for ncplot compatibility
      opnames=tag_names(data.op)
      ms=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec']
      month=string(where(ms eq strmid(data.date,2,3))+1,format='(i02)')
      day=strmid(data.date,0,2)
      year=strmid(data.date,5,4)
      days1970=julday(month,day,year)-julday(1,1,1970) 
      flightdate=month+'/'+day+'/'+year
    
      tb='0000'+strtrim(string(sid_sfm2hms(min(data.time))),2)
      te='0000'+strtrim(string(sid_sfm2hms(max(data.time))),2)
      starttime=strmid(tb,5,2,/r)+':'+strmid(tb,3,2,/r)+':'+strmid(tb,1,2,/r)
      stoptime=strmid(te,5,2,/r)+':'+strmid(te,3,2,/r)+':'+strmid(te,1,2,/r)
      str=starttime+'-'+stoptime
    
      ;Create global attributes
      ncdf_attput,id,'Source','NCAR SID-2H Processing Software',/global
      ncdf_attput,id,'Conventions', 'NCAR-RAF/nimbus', /global
      ncdf_attput,id,'DateCreated',systime(),/global
      ncdf_attput,id,'FlightDate',flightdate,/global
      ncdf_attput,id,'DateProcessed',data.date_processed,/global
      ncdf_attput,id,'TimeInterval',str,/global
      opnames=tag_names(data.op)                  
      FOR i=0,n_elements(opnames)-1 DO BEGIN
          IF string(data.op.(i)[0]) eq '' THEN data.op.(i)[0]='none' ;To avoid an ncdf error (empty string)
          ncdf_attput,id,opnames[i],data.op.(i)[0],/global      
      ENDFOR
    
    
      
      attname=['long_name','units']
      attvalue=['Time','seconds since '+year+'-'+month+'-'+day+' '+starttime+' +0000']
      timeid=ncdf_vardef(id,'Time',xdimid,/long) 
      FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,timeid,attname[k],attvalue[k]
    
      attvalue=['Base time','seconds since 01/01/1970']
      baseid=ncdf_vardef(id,'base_time',/long)
      FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,baseid,attname[k],attvalue[k]
    
      attvalue=['UTC time','seconds from midnight of start date']
      utcid=ncdf_vardef(id,'utc_time',xdimid,/long)
      FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,utcid,attname[k],attvalue[k]
    
              
      ncdf_control,id,/endef                ;put in data mode
      ncdf_varput,id,utcid,data.time 
      ncdf_varput,id,baseid,data.time[0]+days1970*86400l
      ncdf_varput,id,timeid,data.time-data.time[0]  
      ncdf_control,id,/redef                ;return to define mode
   ENDELSE    
   
   
   ;-------Write data to file, start with main structure tags------------------------    
   tags=tag_names(data)     
   
   FOR j=0,n_elements(tags)-1 DO BEGIN
      ;Write each variable
      skiptag=0
      dims=xdimid
      attname=['long_name','units']
      attvalue=['Unknown','Unknown']
      unitadjust=1.0
      currentdata=data.(j)
      tagname=tags[j]+suffix
         
      CASE tags[j] OF
         'AREA':BEGIN
            attvalue={a1:'SID-2H Total Area',a2:'1/m'}
         END
         'AREA_ROUND':BEGIN
            attvalue={a1:'SID-2H Total Area, Round Particles',a2:'1/m'}
         END
         'AREA_IRREG':BEGIN
            attvalue={a1:'SID-2H Total Area, Irregular Particles',a2:'1/m'}
         END
         'LWC':BEGIN
             attvalue={a1:'SID-2H Liquid Water Content',a2:'gram/m3'}
             tagname='PLWCS'+suffix
         END
         'LWC_ROUND': BEGIN
             attvalue={a1:'SID-2H Liquid Water Content, Round Particles',a2:'gram/m3'}
             tagname='PLWCS_ROUND'+suffix
         END
         'LWC_IRREG': BEGIN
             attvalue={a1:'SID-2H Liquid Water Content, Irregular Particles',a2:'gram/m3'}
             tagname='PLWCS_IRREG'+suffix
         END
         'MVD':BEGIN
            attvalue={a1:'SID-2H Median Volume Diameter',a2:'um'}
         END
         'MVD_ROUND':BEGIN
            attvalue={a1:'SID-2H Median Volume Diameter, Round Particles',a2:'um'}
         END
         'MVD_IRREG':BEGIN
            attvalue={a1:'SID-2H Median Volume Diameter, Irregular Particles',a2:'um'}
         END
         'MND':BEGIN
            attvalue={a1:'SID-2H Mean Diameter',a2:'um'}
            tagname='DBARS'+suffix
         END
         'MND_ROUND':BEGIN
            attvalue={a1:'SID-2H Mean Diameter, Round Particles',a2:'um'}
            tagname='DBARS_ROUND'+suffix
         END
         'MND_IRREG':BEGIN
            attvalue={a1:'SID-2H Mean Diameter, Irregular Particles',a2:'um'}
            tagname='DBARS_IRREG'+suffix
         END
         'TRANSITTIME': BEGIN
            attvalue={a1:'SID-2H Average transit time',a2:'s'}
            tagname='AVGTRNS'+suffix
         END
         'MEANAF':BEGIN
            attvalue={a1:'SID-2H Mean asphericity',a2:'none'}
            tagname='ASPH'+suffix
            IF finalprocessing THEN skiptag=1
         END
         'MISSED':BEGIN
            attvalue={a1:'SID-2H Missed particle count',a2:'#'}
            IF finalprocessing THEN skiptag=1
         END
         'ACCEPT_COUNT':BEGIN
            attvalue={a1:'SID-2H Accepted particle count',a2:'#'}
            IF finalprocessing THEN skiptag=1
         END
         'REJECT_COUNT':BEGIN
            attvalue={a1:'SID-2H Rejected particle count',a2:'#'}
            IF finalprocessing THEN skiptag=1
         END
         'TAS':BEGIN
            attvalue={a1:'SID-2H Air speed used',a2:'m/s'}
            IF finalprocessing THEN skiptag=1
         END
         'ACTIVETIME':BEGIN
            attvalue={a1:'SID-2H Probe activity time',a2:'s'}
            IF finalprocessing THEN skiptag=1
         END
         'PMTGAIN':BEGIN
            attvalue={a1:'SID-2H Photodetector gain setting',a2:'raw unit'}
            IF finalprocessing THEN skiptag=1
         END
         'LARGETRIGGER':BEGIN
            attvalue={a1:'SID-2H Large trigger setting',a2:'raw unit'}
            IF finalprocessing THEN skiptag=1
         END
         'SMALLTRIGGER':BEGIN
            attvalue={a1:'SID-2H Small trigger setting',a2:'raw unit'}
            IF finalprocessing THEN skiptag=1
         END
         'NT': BEGIN
            attvalue={a1:'SID-2H Total Number Concentration',a2:'#/cm3'}
            unitadjust=1.0e-6
            tagname='CONCS'+suffix
         END
         'NT_ROUND': BEGIN
            attvalue={a1:'SID-2H Total Number Concentration, Round Particles',a2:'#/cm3'}
            unitadjust=1.0e-6
            tagname='CONCS_ROUND'+suffix
         END
         'NT_IRREG': BEGIN
            attvalue={a1:'SID-2H Total Number Concentration, Irregular Particles',a2:'#/cm3'}
            unitadjust=1.0e-6
            tagname='CONCS_IRREG'+suffix
         END
         'CONC1D': BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Concentration Per Bin, Normalized by Bin Width',a2:'#/cm3',$
			a3:1,a4:fix(n_elements(data.midbins)),a5:data.endbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
            conc_unnorm=sid_unnormalize(data.conc1d,data.endbins)
            conc=fltarr(n_elements(data.time),n_elements(data.endbins))
            conc[*,1:*]=conc_unnorm/1.0e6
            currentdata=reform(transpose(conc),n_elements(data.endbins),1,n_elements(data.time))
            tagname='CSID'+suffix
         END
         'SPEC1D':BEGIN
            attname=['_FillValue','long_name','units','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Count Per Size Bin',a2:'count',a3:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, transpose, and reform to 3-dimensions
            spec=fltarr(n_elements(data.time),n_elements(data.endbins))
            spec[*,1:*]=data.spec1d
            currentdata=reform(transpose(spec),n_elements(data.endbins),1,n_elements(data.time))
            tagname='ASID'+suffix
         END
         'CONC_ROUND': BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Concentration Per Bin, Normalized by Bin Width, Round Particles',a2:'#/cm3',$
			a3:1,a4:fix(n_elements(data.midbins)),a5:data.endbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
            conc_unnorm=sid_unnormalize(data.conc_round,data.endbins)
            conc=fltarr(n_elements(data.time),n_elements(data.endbins))
            conc[*,1:*]=conc_unnorm/1.0e6
            currentdata=reform(transpose(conc),n_elements(data.endbins),1,n_elements(data.time))
            tagname='CSID_ROUND'+suffix
         END
         'SPEC_ROUND':BEGIN
            attname=['_FillValue','long_name','units','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Count Per Size Bin, Round Particles',a2:'count',a3:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, transpose, and reform to 3-dimensions
            spec=fltarr(n_elements(data.time),n_elements(data.endbins))
            spec[*,1:*]=data.spec_round
            currentdata=reform(transpose(spec),n_elements(data.endbins),1,n_elements(data.time))
            tagname='ASID_ROUND'+suffix
         END
         'CONC_IRREG': BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Concentration Per Bin, Normalized by Bin Width, Irregular Particles',a2:'#/cm3',$
			a3:1,a4:fix(n_elements(data.midbins)),a5:data.endbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
            conc_unnorm=sid_unnormalize(data.conc_irreg,data.endbins)
            conc=fltarr(n_elements(data.time),n_elements(data.endbins))
            conc[*,1:*]=conc_unnorm/1.0e6
            currentdata=reform(transpose(conc),n_elements(data.endbins),1,n_elements(data.time))
            tagname='CSID_IRREG'+suffix
         END
         'SPEC_IRREG':BEGIN
            attname=['_FillValue','long_name','units','Category']
            attvalue={a0:-32767.,a1:'SID-2H Particle Count Per Size Bin, Irregular Particles',a2:'count',a3:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            ;Pad first bin with zeros, transpose, and reform to 3-dimensions
            spec=fltarr(n_elements(data.time),n_elements(data.endbins))
            spec[*,1:*]=data.spec_irreg
            currentdata=reform(transpose(spec),n_elements(data.endbins),1,n_elements(data.time))
            tagname='ASID_IRREG'+suffix
         END
         'TBCONC1D': BEGIN
            IF not(finalprocessing) THEN BEGIN
               attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
               attvalue={a0:-32767.,a1:'SID-2H Time-based Particle Concentration Per Bin, Normalized by Bin Width',a2:'#/cm3',$
			a3:1,a4:fix(n_elements(data.tbmidbins)),a5:data.tbendbins,a6:'micrometers',a7:'PMS Probe'}
               dims=[ydimid_tb,sdimid,xdimid]
               ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
               conc_unnorm=sid_unnormalize(data.tbconc1d,data.tbendbins)
               conc=fltarr(n_elements(data.time),n_elements(data.tbendbins))
               conc[*,1:*]=conc_unnorm/1.0e6
               currentdata=reform(transpose(conc),n_elements(data.tbendbins),1,n_elements(data.time))
               tagname='CSIDTB'+suffix
            ENDIF ELSE skiptag=1
         END
         'TBSPEC1D':BEGIN
            IF not(finalprocessing) THEN BEGIN
               attname=['_FillValue','long_name','units','Category']
               attvalue={a0:-32767.,a1:'SID-2H Time-based Particle Count Per Size Bin',a2:'count',a3:'PMS Probe'}
               dims=[ydimid_tb,sdimid,xdimid]
               ;Pad first bin with zeros, transpose, and reform to 3-dimensions
               spec=fltarr(n_elements(data.time),n_elements(data.tbendbins))
               spec[*,1:*]=data.tbspec1d
               currentdata=reform(transpose(spec),n_elements(data.tbendbins),1,n_elements(data.time))
               tagname='ASIDTB'+suffix
            ENDIF ELSE skiptag=1
         END
         'AFSPEC':BEGIN
            IF not(finalprocessing) THEN BEGIN
               attvalue={a1:'SID-2H Particle Count Per Af Bin',a2:'#'}
               dims=[ydimid_af,xdimid]
               currentdata=transpose(currentdata)
            ENDIF ELSE skiptag=1
         END
         'INTSPEC':BEGIN
            IF not(finalprocessing) THEN BEGIN
               attvalue={a1:'SID-2H Particle Count Per Interarrival Bin',a2:'#'}
               dims=[ydimid_int,xdimid]
               currentdata=transpose(currentdata)
            ENDIF ELSE skiptag=1
         END
         'MIDBINS':BEGIN
            attvalue={a1:'SID-2H Size Bin Mid-points',a2:'um'}
            dims=ydimid_size
         END
         'INTMIDBINS':BEGIN
            IF not(finalprocessing) THEN BEGIN
               attvalue={a1:'SID-2H Interarrival Bin Mid-points',a2:'s'}
               dims=ydimid_int
            ENDIF ELSE skiptag=1
         END
         'AFMIDBINS':BEGIN
            IF not(finalprocessing) THEN BEGIN
               attvalue={a1:'SID-2H Af Bin Mid-points',a2:'unitless'}
               dims=ydimid_af
            ENDIF ELSE skiptag=1
         END
         'BRANCH_COUNT':BEGIN
            attvalue={a1:'SID-2H Particle Count by Peak Wave Number (Branches)',a2:'#'}
            dims=[ydimid_branches,xdimid]
            currentdata=transpose(currentdata)
         END
         
         ELSE:skiptag=1  
      ENDCASE
        
      IF not(skiptag) THEN BEGIN                                              
         varid=ncdf_varid(id,tagname)  ;Check if this variable already exists
         IF varid eq -1 THEN varid=ncdf_vardef(id,tagname,dims,/float)         
         FOR k=0,n_elements(attname)-1 DO BEGIN
            IF size(attvalue.(k), /TYPE) eq 2 THEN BEGIN
              ncdf_attput,id,varid,attname[k],attvalue.(k),/LONG
            ENDIF ELSE BEGIN
              ncdf_attput,id,varid,attname[k],attvalue.(k)
            ENDELSE
         ENDFOR
          
         ncdf_control,id,/endef                ;put in data mode
         ncdf_varput,id,varid,currentdata*unitadjust   
         ncdf_control,id,/redef                ;return to define mode
      ENDIF
   ENDFOR
   
   ;--------------Sub-structure tags------------------------------
   tags=tag_names(data.mux)     
   FOR j=0,n_elements(tags)-1 DO BEGIN
      ;Write each variable
      skiptag=0
      IF finalprocessing THEN skiptag=1  ;Skip ALL housekeeping for final processing
      dims=xdimid
      attvalue=['Unknown','Unknown']
      unitadjust=1.0
      tagname=tags[j]+suffix
         
      CASE tags[j] OF
         'TDETECTOR': BEGIN
            attvalue=['SID-2H Detector Temperature','deg_C']
            unitadjust=1/(-34.34)
         END
         'TLASER': BEGIN
            attvalue=['SID-2H Laser Temperature','deg_C']
            unitadjust=1/(-34.34)
         END          
         'TLASERCONTROL': BEGIN
            attvalue=['SID-2H Laser Control Temperature','deg_C']
            unitadjust=1/(-34.34)
         END         
         'TFPGAAMBIENT': BEGIN
            attvalue=['SID-2H FPGA Temperature','deg_C']
            unitadjust=1/(-34.34)
         END          
         'THEADHEATER': BEGIN
            attvalue=['SID-2H Head Heater Temperature','deg_C']
            unitadjust=1/(-34.34)
         END          
         'THEADAMBIENT': BEGIN
            attvalue=['SID-2H Ambient Head Temperature','deg_C']
            unitadjust=1/(-34.34)
         END          
         'LASERPOWERMON': BEGIN
            attvalue=['SID-2H Laser Power Monitor','mW']
            ;Compute laser power as in SIDConnect.c from Richard's code
            IF total(tag_names(data) eq 'LASERCAL') THEN lasercal=data.lasercal ELSE lasercal=[0.0, 9.37e-05, -0.00042,0.969304, -100.0, 2047.0]
            ft=data.mux.tlaserpowermon/(-34.34)
            fLPowCF = lasercal[1]*ft^2 + lasercal[2]*fT + lasercal[3]; 
				fLPowCorrected = data.mux.laserpowermon/fLPowCF;
				data.mux.laserpowermon = (fLPowCorrected*lasercal[4]/lasercal[5])>0
            unitadjust=1.0 
         END         
         'TLASERPOWERMON': BEGIN
            attvalue=['SID-2H Laser Power Monitor Temperature','deg_C']
            unitadjust=1/(-34.34)
         END          
         ELSE:skiptag=1  
      ENDCASE
        
      IF not(skiptag) THEN BEGIN                                      
         varid=ncdf_varid(id,tagname)  ;Check if this variable already exists
         IF varid eq -1 THEN varid=ncdf_vardef(id,tagname,dims,/float) 
         FOR k=0,n_elements(attname)-1 DO BEGIN
            IF size(attvalue[k], /TYPE) eq 2 THEN BEGIN
              ncdf_attput,id,varid,attname[k],attvalue[k],/LONG
            ENDIF ELSE BEGIN
              ncdf_attput,id,varid,attname[k],attvalue[k]
            ENDELSE
         ENDFOR
          
         ncdf_control,id,/endef                ;put in data mode
         ncdf_varput,id,varid,data.mux.(j)*unitadjust   
         ncdf_control,id,/redef                ;return to define mode
      ENDIF
   ENDFOR

   ;--------------Habit recognition tags------------------------------
   tags=['HABIT_ROUND','HABIT_IRREGULAR','HABIT_COLUMN','HABIT_PLATE']     
   acount=data.accept_count>1
   FOR j=0,n_elements(tags)-1 DO BEGIN
      ;Write each variable
      skiptag=0
      dims=xdimid
      attvalue=['Unknown','Unknown']
      unitadjust=1.0
      tagname=tags[j]+suffix
         
      CASE tags[j] OF
         'HABIT_ROUND':BEGIN
            attvalue=['SID-2H Round particle percentage','%']
            currentdata=float(data.branch_count[*,0])/acount*100
         END
         'HABIT_IRREGULAR':BEGIN
            attvalue=['SID-2H Irregular particle percentage','%']
            currentdata=total(float(data.branch_count[*,[1,2,3,7]]),2)/acount*100
         END
         'HABIT_COLUMN':BEGIN
            attvalue=['SID-2H Column particle percentage','%']
            currentdata=float(data.branch_count[*,4])/acount*100
         END
         'HABIT_PLATE':BEGIN
            attvalue=['SID-2H Plate particle percentage','%']
            currentdata=total(float(data.branch_count[*,[5,6]]),2)/acount*100
         END
      ENDCASE

      varid=ncdf_varid(id,tagname)  ;Check if this variable already exists
      IF varid eq -1 THEN varid=ncdf_vardef(id,tagname,dims,/float) 
      FOR k=0,n_elements(attname)-1 DO BEGIN
         IF size(attvalue[k], /TYPE) eq 2 THEN BEGIN
            ncdf_attput,id,varid,attname[k],attvalue[k],/LONG
         ENDIF ELSE BEGIN
            ncdf_attput,id,varid,attname[k],attvalue[k]
         ENDELSE
      ENDFOR

      ncdf_control,id,/endef                ;put in data mode
      ncdf_varput,id,varid,currentdata  
      ncdf_control,id,/redef                ;return to define mode
   ENDFOR
   
   ;Close the file
   ncdf_close,id
   
END
