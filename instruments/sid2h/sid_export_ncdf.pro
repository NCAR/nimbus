PRO sid_export_ncdf, infile, outdir=outdir, outfile=outfile
   ;PRO to export a netCDF file with the variables contained in a data structure.
   ;Aaron Bansemer, NCAR, 9/2007
   ;Updated 7/2009 for ncpp compatibility
   
   restore,infile
   infile_base=file_basename(infile)
   p=strpos(infile_base,'.dat')
   filename=strmid(infile_base,0,p)+'.nc'
   outfile=outdir+filename  ;data.date+'_'+strtrim(string(long(sid_sfm2hms(data.starttime))),2)+'_SID.nc'
  
   ;Create the file
   id=ncdf_create(outfile[0],/clobber)
   
   
   ;Define the x-dimension, should be used in all variables
   xdimid=ncdf_dimdef(id,'Time',n_elements(data.time))
   ;Samples per second
   sdimid=ncdf_dimdef(id,'sps1',(1.0/data.op.rate)>1)
   ;This is for numbins
   ydimid_size=ncdf_dimdef(id,'Vector41',n_elements(data.endbins))
   ;This is for interarrival
   ydimid_int=ncdf_dimdef(id,'VectorInterarrivalBinSizes',n_elements(data.intmidbins))
   ;This is for asphericity
   ydimid_af=ncdf_dimdef(id,'VectorAsphericityBinSizes',n_elements(data.afmidbins))
   ;This is for branch counts
   ydimid_branches=ncdf_dimdef(id,'WaveNumberPeakValue',8)
   ;This is for time-based size
   ydimid_tb=ncdf_dimdef(id,'Vectortb',n_elements(data.tbendbins))
      
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
      
  ;-------Main structure tags------------------------    
  tags=tag_names(data)     
  FOR j=0,n_elements(tags)-1 DO BEGIN
      ;Write each variable
      skiptag=0
      dims=xdimid
      attname=['long_name','units']
      attvalue=['Unknown','Unknown']
      unitadjust=1.0
      currentdata=data.(j)
      tagname=tags[j]
         
      CASE tags[j] OF
         'AREA':attvalue={a1:'Total Area',a2:'1/m'}
         'LWC':attvalue={a1:'Liquid Water Content',a2:'g/m3'}
         'MVD':attvalue={a1:'Median Volume Diameter',a2:'um'}
         'MND':attvalue={a1:'Mean Diameter',a2:'um'}
         'GAIN':attvalue={a1:'PMT Gain',a2:'raw units'}
         'TRANSITTIME':attvalue={a1:'Average transit time',a2:'s'}
         'MISSED':attvalue={a1:'Number of missed particles',a2:'#'}
         'REJECTCOUNT':attvalue={a1:'Number of rejected particles',a2:'#'}
         'TAS':attvalue={a1:'True air speed',a2:'m/s'}
         'TRANSITTIME':attvalue={a1:'Average transit time',a2:'s'}
         'MEANAF':attvalue={a1:'Mean asphericity',a2:'none'}
         'MISSED':attvalue={a1:'Missed particle count',a2:'#'}
         'ACCEPT_COUNT':attvalue={a1:'Accepted particle count',a2:'#'}
         'REJECT_COUNT':attvalue={a1:'Rejected particle count',a2:'#'}
         'TAS':attvalue={a1:'Air speed used',a2:'m/s'}
         'ACTIVETIME':attvalue={a1:'Probe activity time',a2:'s'}
         'PMTGAIN':attvalue={a1:'Photodetector gain setting',a2:'raw unit'}
         'LARGETRIGGER':attvalue={a1:'Large trigger setting',a2:'raw unit'}
         'SMALLTRIGGER':attvalue={a1:'Small trigger setting',a2:'raw unit'}
         'NT': BEGIN
            attvalue={a1:'Total Number Concentration',a2:'#/cm3'}
            unitadjust=1.0e-6
         END
         'CONC1D': BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'Particle Concentration Per Bin, Normalized by Bin Width',a2:'#/cm3',a3:1,a4:fix(n_elements(data.midbins)),$
                      a5:data.endbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            currentdata=transpose(currentdata)
            ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
            conc_unnorm=sid_unnormalize(data.conc1d,data.endbins)
            conc=fltarr(n_elements(data.time),n_elements(data.endbins))
            conc[*,1:*]=conc_unnorm/1.0e6
            currentdata=reform(transpose(conc),n_elements(data.endbins),1,n_elements(data.time))
            tagname='CSID_RWO'
         END
         'SPEC1D':BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'Particle Count Per Size Bin',a2:'count',a3:1,a4:fix(n_elements(data.midbins)),$
                      a5:data.endbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_size,sdimid,xdimid]
            currentdata=transpose(currentdata)
            ;Pad first bin with zeros, transpose, and reform to 3-dimensions
            spec=fltarr(n_elements(data.time),n_elements(data.endbins))
            spec[*,1:*]=data.spec1d
            currentdata=reform(transpose(spec),n_elements(data.endbins),1,n_elements(data.time))
            tagname='ASID_RWO'
         END
         'TBCONC1D': BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'Time-based Particle Concentration Per Bin, Normalized by Bin Width',a2:'#/cm3',a3:1,a4:fix(n_elements(data.tbmidbins)),$
                      a5:data.tbendbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_tb,sdimid,xdimid]
            currentdata=transpose(currentdata)
            ;Pad first bin with zeros, convert to #/cc, transpose, and reform to 3-dimensions
            conc_unnorm=sid_unnormalize(data.tbconc1d,data.tbendbins)
            conc=fltarr(n_elements(data.time),n_elements(data.tbendbins))
            conc[*,1:*]=conc_unnorm/1.0e6
            currentdata=reform(transpose(conc),n_elements(data.tbendbins),1,n_elements(data.time))
            tagname='CSIDTB_RWO'
         END
         'TBSPEC1D':BEGIN
            attname=['_FillValue','long_name','units','FirstBin','LastBin','CellSizes','CellSizeUnits','Category']
            attvalue={a0:-32767.,a1:'Time-based Particle Count Per Size Bin',a2:'count',a3:1,a4:fix(n_elements(data.tbmidbins)),$
                      a5:data.tbendbins,a6:'micrometers',a7:'PMS Probe'}
            dims=[ydimid_tb,sdimid,xdimid]
            currentdata=transpose(currentdata)
            ;Pad first bin with zeros, transpose, and reform to 3-dimensions
            spec=fltarr(n_elements(data.time),n_elements(data.tbendbins))
            spec[*,1:*]=data.tbspec1d
            currentdata=reform(transpose(spec),n_elements(data.tbendbins),1,n_elements(data.time))
            tagname='ASIDTB_RWO'
         END
         'AFSPEC':BEGIN
            attvalue={a1:'Particle Count Per Af Bin',a2:'#'}
            dims=[ydimid_af,xdimid]
            currentdata=transpose(currentdata)
         END
         'INTSPEC':BEGIN
            attvalue={a1:'Particle Count Per Interarrival Bin',a2:'#'}
            dims=[ydimid_int,xdimid]
            currentdata=transpose(currentdata)
         END
         'MIDBINS':BEGIN
            attvalue={a1:'Size Bin Mid-points',a2:'um'}
            dims=ydimid_size
         END
         'INTMIDBINS':BEGIN
            attvalue={a1:'Interarrival Bin Mid-points',a2:'s'}
            dims=ydimid_int
         END
         'AFMIDBINS':BEGIN
            attvalue={a1:'Af Bin Mid-points',a2:'unitless'}
            dims=ydimid_af
         END
         'BRANCH_COUNT':BEGIN
            attvalue={a1:'Particle Count by Peak Wave Number (Branches)',a2:'#'}
            dims=[ydimid_branches,xdimid]
            currentdata=transpose(currentdata)
         END
         
         ELSE:skiptag=1  
      ENDCASE
        
      IF not(skiptag) THEN BEGIN                                              
         varid=ncdf_vardef(id,tagname,dims,/float) 
         FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,varid,attname[k],attvalue.(k)
          
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
      dims=xdimid
      attvalue=['Unknown','Unknown']
      unitadjust=1.0
         
      CASE tags[j] OF
         'TDETECTOR': BEGIN
            attvalue=['Detector Temperature','degrees C']
            unitadjust=1/(-34.34)
         END
         'TLASER': BEGIN
            attvalue=['Laser Temperature','degrees C']
            unitadjust=1/(-34.34)
         END          
         'TLASERCONTROL': BEGIN
            attvalue=['Laser Control Temperature','degrees C']
            unitadjust=1/(-34.34)
         END         
         'TFPGAAMBIENT': BEGIN
            attvalue=['FPGA Temperature','degrees C']
            unitadjust=1/(-34.34)
         END          
         'THEADHEATER': BEGIN
            attvalue=['Head Heater Temperature','degrees C']
            unitadjust=1/(-34.34)
         END          
         'THEADAMBIENT': BEGIN
            attvalue=['Ambient Head Temperature','degrees C']
            unitadjust=1/(-34.34)
         END          
         'LASERPOWERMON': BEGIN
            attvalue=['Laser Power Monitor','mW']
            ;Compute laser power as in SIDConnect.c from Richard's code
            IF total(tag_names(data) eq 'LASERCAL') THEN lasercal=data.lasercal ELSE lasercal=[0.0, 9.37e-05, -0.00042,0.969304, -100.0, 2047.0]
            ft=data.mux.tlaserpowermon/(-34.34)
            fLPowCF = lasercal[1]*ft^2 + lasercal[2]*fT + lasercal[3]; 
				fLPowCorrected = data.mux.laserpowermon/fLPowCF;
				data.mux.laserpowermon = (fLPowCorrected*lasercal[4]/lasercal[5])>0
            unitadjust=1.0 
         END         
         'TLASERPOWERMON': BEGIN
            attvalue=['Laser Power Monitor Temperature','degrees C']
            unitadjust=1/(-34.34)
         END          
         ELSE:skiptag=1  
      ENDCASE
        
      IF not(skiptag) THEN BEGIN                                      
         varid=ncdf_vardef(id,tags[j],dims,/float) 
         FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,varid,attname[k],attvalue[k]
          
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
         
      CASE tags[j] OF
         'HABIT_ROUND':BEGIN
            attvalue=['Round particle percentage','%']
            currentdata=float(data.branch_count[*,0])/acount*100
         END
         'HABIT_IRREGULAR':BEGIN
            attvalue=['Irregular particle percentage','%']
            currentdata=total(float(data.branch_count[*,[1,2,3,7]]),2)/acount*100
         END
         'HABIT_COLUMN':BEGIN
            attvalue=['Column particle percentage','%']
            currentdata=float(data.branch_count[*,4])/acount*100
         END
         'HABIT_PLATE':BEGIN
            attvalue=['Plate particle percentage','%']
            currentdata=total(float(data.branch_count[*,[5,6]]),2)/acount*100
         END
      ENDCASE

      varid=ncdf_vardef(id,tags[j],dims,/float) 
      FOR k=0,n_elements(attname)-1 DO ncdf_attput,id,varid,attname[k],attvalue[k]          
      ncdf_control,id,/endef                ;put in data mode
      ncdf_varput,id,varid,currentdata  
      ncdf_control,id,/redef                ;return to define mode
   ENDFOR
   
   ;Close the file
   ncdf_close,id
   
END
