PRO sid_windowplot,topid,p1,pinfo,pop,noset=noset
   !p.multi=[0,1,1,0,0]
   tabnum=widget_info(widget_info(topid,find='tab'),/tab_current) 
   IF keyword_set(noset) eq 0 THEN wset,(*pinfo).wid[tabnum]  ;Don't set window if we're in z-buffer for PNGs
   erase
   i=(*pinfo).i
   color1=80  ;blue
   color2=150 ;green
   color3=250 ;red
   color4=190 ;yellow
   color5=215 ;orange
   color6=110 ;light blue
   
   sizerange=[min((*p1).endbins), max((*p1).endbins)]
   afrange=[1,50]
   !p.charsize=1.2
   
   ;Temperature conversion from SID2common.h
   raw2c=-34.34
  
   ;Find if speed rejection turned on/off
   ;IF total(where(tag_names((*p1).op) eq 'SPEEDREJECT')) eq -1 THEN speedreject=0 ELSE speedreject=(*p1).op.speedreject

   CASE tabnum OF
      
      ;------------------------------------------------------
      ;---------------Distributions--------------------------
      ;------------------------------------------------------
      0:BEGIN 
         contour,(*p1).spec2d[i,*,*],(*p1).midbins,(*p1).afmidbins,/cell,nlevels=20,position=[0.4,0.4,0.95,0.95],$
            /xl,/yl,/xs,/ys,xr=sizerange,yr=afrange
         plot,(*p1).midbins,(*p1).conc1d[i,*]/1e12,/xl,/yl,/xs,xr=sizerange,yr=[1e-4,1e3],position=[0.4,0.07,0.95,0.35],/noerase,$
             xtit='Diameter (um)',ytit='Conc (#/cc/um)',/nodata
         oplot,(*p1).midbins,(*p1).conc1d[i,*]/1e12,color=color1
         plot,(*p1).afspec[i,*],(*p1).afmidbins,/yl,/ys,yr=afrange,position=[0.07,0.4,0.35,0.95],/noerase,$
             xtit='Count',ytit='Asphericity',/nodata
         oplot,(*p1).afspec[i,*],(*p1).afmidbins,color=color1

         
         xyouts,0.07,0.30,'N!dT!n(#/cc):',/norm & xyouts,0.2,0.30,strtrim(string((*p1).nt[i]/1.0e6,form='(f10.1)'),2),/normal         
         xyouts,0.07,0.27,'LWC(g/m!u3!n):',/norm & xyouts,0.2,0.27,strtrim(string((*p1).lwc[i],form='(f7.4)'),2),/normal        
         xyouts,0.07,0.24,'MVD(um):',/norm & xyouts,0.2,0.24,strtrim(string((*p1).mvd[i],form='(f5.1)'),2),/normal         
         xyouts,0.07,0.21,'MeanD(um):',/norm & xyouts,0.2,0.21,strtrim(string((*p1).mnd[i],form='(f5.1)'),2),/normal         
         xyouts,0.07,0.18,'Mean Af:',/norm & xyouts,0.2,0.18,strtrim(string((*p1).meanaf[i],form='(f5.1)'),2),/normal
         xyouts,0.07,0.12,'Accepted:',/norm & xyouts,0.2,0.12,strtrim(string((*p1).accept_count[i]),2),/normal
         xyouts,0.07,0.09,'Rejected:',/norm & xyouts,0.2,0.09,strtrim(string((*p1).reject_count[i]),2),/normal
         xyouts,0.07,0.06,'Missed:',/norm & xyouts,0.2,0.06,strtrim(string((*p1).missed[i]),2),/normal
        

         
         ;plot,(*p1).intmidbins,(*p1).intspec[i,*],/xl,/xs,position=[0.05,0.05,0.35,0.35],/noerase
      END
      
      ;------------------------------------------------------
      ;---------------Particles------------------------------
      ;------------------------------------------------------
      1:BEGIN
         ;Check for missing files
         IF (*pinfo).rawdir eq '' THEN BEGIN
            xyouts,0.5,0.5,'Raw data files not found.',align=0.5,/norm
            return
         ENDIF
       
         ;Plot axis
         plot,[0,2000],[0,1],/nodata,ysty=4,xsty=8,xtickint=200,xminor=1,pos=[0.02,0.05,0.98,0.98],xtit='Mean Raw Scatter' ;Axis
         IF (*pop).peak eq 1 THEN BEGIN
            maxsize=50>(sid_size(transpose(fltarr(28)+2000), transpose(fltarr(28)+2000), (*p1).pmtgain[i], 0, (*p1).tas[i],(*pop).sizegain,peak=(*pop).peak)).size
            labelvalues=round(10*findgen(11)/10 * maxsize)/10.0
            plot,[0,2000],[0,1],/nodata,ysty=4,xsty=8,xtickint=200,xminor=1,pos=[0.02,0.05,0.98,0.98],$
               xtit='Diameter [!4l!3m]',xtickname=string(labelvalues,form='(f4.1)'),charsize=1.4
         ENDIF   

         ;The raw data exists
         nt=(*p1).accept_count[i]+(*p1).reject_count[i] < 1000;Number of particles to plot
         IF nt eq 0 THEN return
         ;Find the right file to read, open it, and point to the right datablock
         j=(*p1).firstblock[i]
         starti=(*p1).firsti[i]
         nx=10   ;number of sizes to sort the particles into
         nplotted=bytarr(nx)  ;number plotted in each column
         tnplotted=0

         margin=[0.005,0.005,-0.005,-0.005]
         
         REPEAT BEGIN
            fn=(*pinfo).rawdir+file_basename((*pop).fn[(*p1).filenum[j]])
            openr,lun,fn,/get_lun
            point_lun,lun,(*p1).blockpointer[j]
            b=sid_read_next(lun)
            free_lun,lun

           ;Process each particle in the datablock
           FOR k=starti,b.num-1 DO BEGIN
            ;Skip over 'forced' particles
            IF (b.forced[k] eq 0) and (tnplotted lt nt) THEN BEGIN  
              scatter_adjusted=(b.scatter[k,*]-(*p1).detoffset)*(*p1).detgain 
              good=where((b.scatter[k,*] lt 2047) and (scatter_adjusted gt 0),ngood)

              IF ngood eq 0 THEN sizecol=9 ELSE $
                 sizecol=fix(mean(scatter_adjusted[good],/nan))/200 >0<(nx-1)     ;Quick sizing (0-9) to put into the correct column
                 a=sid_size(b.scatter[k,*],scatter_adjusted,(*p1).pmtgain[i],b.tof[k],(*p1).tas[i],(*pop).sizegain,peak=(*pop).peak)
                 IF (*pop).peak eq 1 THEN sizecol=max(where(a.size ge labelvalues)) < 9       
              IF nplotted[sizecol] lt 10 THEN BEGIN                              
               reject=sid_reject(a,b.tof[k],(*p1).endbins,(*p1).intendbins[1],(*p1).intendbins,a.nsat,(*pop).maxsaturated,(*p1).tas[i],210,speedreject=(*pop).speedreject)
               
               ;Plot the particle
               temp=[transpose(scatter_adjusted), scatter_adjusted[0]] 
               pos=[sizecol/10.4+0.02,nplotted[sizecol]/10.7+0.055,(sizecol+1)/10.4+0.02,(nplotted[sizecol]+1)/10.7+0.055] + margin
               plot,temp,findgen(29)/28*2*!pi,/polar,xs=4,ys=4,/iso,color=70+sizecol*(180.0/nx),line=(reject<1)*5,position=pos,/norm,/noerase 
               oplot,[0],[0],color=250,psym=1
               ;xyouts,0.1,0.1,' '+strtrim(string(round(a.size)),2) + ' ' +strtrim(string(round(a.branches)),2),charsize=1 
               xyouts,0.1,0.1,' '+strtrim(string(round(a.branches)),2),charsize=1
               nplotted[sizecol]=nplotted[sizecol]+1 
              ENDIF               
               tnplotted=tnplotted+1
            ENDIF  ;end 'forced particle' if
          ENDFOR    ;end particle loop
         j=j+1
         starti=0
         ENDREP UNTIL tnplotted ge nt         
       END  
  
        
      ;------------------------------------------------------
      ;---------------Timing---------------------------------
      ;------------------------------------------------------
      2:BEGIN
         !p.multi=[0,1,2,0,0]
         plot,(*p1).intmidbins,(*p1).intspec[i,*],/xl,/xs,xtit='Interarrival Time (s)',ytit='Count',xr=[1e-6,0.1],/nodata
         oplot,(*p1).intmidbins,(*p1).intspec[i,*],color=color1
         plot,(*p1).tofmidbins,(*p1).tofspec[i,*],/xl,/xs,xtit='Transit Time (s)',ytit='Count',/nodata
         oplot,(*p1).tofmidbins,(*p1).tofspec[i,*],color=color3
      END
      
      ;------------------------------------------------------
      ;---------------Detectors------------------------------
      ;------------------------------------------------------
      3:BEGIN
         x=indgen(2500)
         y=smooth(total((*p1).dethist,1)/28.0,9)
         plot,x,y,yr=[1,max(y)],/yl,xtitle='Detector Value',ytitle='Count'
         widget_control,widget_info(topid,find='detbox'),get_value=detbox
         w=where(detbox eq 1,n)
         FOR i=0,n-1 DO BEGIN
            oplot,x,smooth((*p1).dethist[w[i],*],9),color=70+w[i]*6.5
         ENDFOR
 
         wset,(*pinfo).wp
         x=findgen(29)/28*2*!pi
         IF (*pinfo).show_correction THEN title='Corrected' ELSE title='Uncorrected'
         plot,[-2,2],[-2,2],/nodata,position=[0.1,0.1,0.9,0.9],title=title
         oplot,[-2,2],[0,0],/line
         oplot,[0,0],[-2,2],/line
         FOR i=0,n-1 DO BEGIN  ;Plot radials
            oplot,findgen(10),fltarr(10)+w[i]/28.0*2*!pi,color=70+w[i]*6.5,thick=2,/polar
         ENDFOR

         nx=(size((*p1).totalscatter,/dim))[0]
         FOR i=0,nx-1 DO BEGIN  ;Plot for each size
            y=reform([(*p1).totalscatter[i,*]]/(mean((*p1).totalscatter[i,*])>1))
            IF (*pinfo).show_correction eq 0 THEN y=y/(*p1).detgain
            oplot,[y,y[0]],x,color=70+i*(180.0/nx),/polar
         ENDFOR      
         ;Plot overall average
         y=total((*p1).totalscatter,1)/mean(total((*p1).totalscatter,1))
         IF (*pinfo).show_correction eq 0 THEN y=y/(*p1).detgain
         oplot,[y,y[0]],x,/polar,thick=4
         
      END
      ;------------------------------------------------------
      ;---------------Time Series----------------------------
      ;------------------------------------------------------
      4:BEGIN
         ;Get plot type
         widget_control,widget_info(topid,find='ts_type1'),get_value=plottype
         id=[0,0]
         id[0]=widget_info(widget_info(topid,find='ts_type1'),/droplist_select)
         id[1]=widget_info(widget_info(topid,find='ts_type2'),/droplist_select)
         xunit=widget_info(widget_info(topid,find='ts_units'),/droplist_select)
         
         ;Get start/stop times
         a=(*pinfo).i1
         b=(*pinfo).i2
         
         IF xunit eq 1 THEN BEGIN
            !x.title='Time (s)'
            x=(*p1).time[a:b]
            !x.tickformat='(i6)'
         ENDIF ELSE BEGIN
            !x.title='Time'
            ;x=(*p1).time[a:b]/3600.0
            ;!x.tickformat='(f5.2)'
            x=((*p1).time[a:b]+43200)/86400d
            date_label=label_date(date_form='%H:%I:%S')
            !x.tickformat='label_date'
            !x.tickunits=['Seconds']
            ;!x.ticks=
         ENDELSE
         !x.style=1         
         !p.multi=[2,1,2,0,0]
        FOR p=0,1 DO BEGIN ;make both plots
            CASE plottype[id[p]] OF
               'LWC':BEGIN
                  plot,x,(*p1).lwc[a:b],ytitle='LWC (g/m!u3!n)',/nodata
                  oplot,x,(*p1).lwc[a:b],color=color1
                  IF total(tag_names((*p1)) eq 'LWC_ROUND') THEN oplot,x,(*p1).lwc_round[a:b],color=color2
                  IF total(tag_names((*p1)) eq 'LWC_IRREG') THEN oplot,x,(*p1).lwc_irreg[a:b],color=color3
                  sid_legend,['All','Round','Non-Round'],line=0,color=[color1,color2,color3]
               END
               'Total Concentration':BEGIN
                  y=(*p1).nt[a:b]/1.0e6
                  IF max(y) lt 10 THEN ylog=0 ELSE ylog=1
                  IF max(y) lt 10 THEN yr=[0, max(y)] ELSE yr=[0.1,max(y,/nan)>1]
                  plot,x,(*p1).nt[a:b]/1.0e6,ytitle='N!lt!n (#/cc)',ylog=ylog,yr=yr,/nodata
                  oplot,x,(*p1).nt[a:b]/1.0e6,color=color1
                  IF total(tag_names((*p1)) eq 'NT_ROUND') THEN oplot,x,(*p1).nt_round[a:b]/1.0e6,color=color2
                  IF total(tag_names((*p1)) eq 'NT_IRREG') THEN oplot,x,(*p1).nt_irreg[a:b]/1.0e6,color=color3
                  sid_legend,['All','Round','Non-Round'],line=0,color=[color1,color2,color3]
               END
               'Extinction':BEGIN
                  y=(*p1).area[a:b]*2000
                  IF max(y) lt 10 THEN ylog=0 ELSE ylog=1
                  IF max(y) lt 10 THEN yr=[0, max(y)] ELSE yr=[0.1,max(y,/nan)>0.1<1000]
                  plot,x,y,ytitle='Extinction (1/km)',ylog=ylog,yr=yr,/nodata
                  oplot,x,y,color=color1
                  IF total(tag_names((*p1)) eq 'AREA_ROUND') THEN oplot,x,(*p1).area_round[a:b]*2000,color=color2
                  IF total(tag_names((*p1)) eq 'AREA_IRREG') THEN oplot,x,(*p1).area_irreg[a:b]*2000,color=color3
                  sid_legend,['All','Round','Non-Round'],line=0,color=[color1,color2,color3]
               END
               'Asphericity':BEGIN
                  plot,x,(*p1).meanaf[a:b],ytitle='Mean Asphericity',/yl,yr=afrange,/nodata
                  oplot,x,(*p1).meanaf[a:b],color=color4
               END
               'Harmonics':BEGIN
                  acount=(*p1).accept_count[a:b]>1
                  plot,x,float((*p1).branch_count[a:b,0])/acount*100,ytitle='Percent of Particles',yr=[0,105],/ys,psym=1,symsize=0.3
                  oplot,x,total(float((*p1).branch_count[a:b,[1,2,3,7]]),2)/acount*100,color=color1,psym=1,symsize=0.3
                  oplot,x,float((*p1).branch_count[a:b,4])/acount*100,color=color2,psym=1,symsize=0.3
                  oplot,x,float((*p1).branch_count[a:b,5]+(*p1).branch_count[a:b,6])/acount*100,color=color3,psym=1,symsize=0.3
                  sid_legend,['Round','Irregular (1,2,3,7)','Column (4)','Plate (5,6)'],line=0,color=[255,color1,color2,color3]                
               END
               'Diameter':BEGIN             
                  plot,x,(*p1).mnd[a:b],ytitle='Diameter (um)',/yl,yr=sizerange,/ys,/nodata
                  oplot,x,(*p1).mnd[a:b],color=color1
                  oplot,x,(*p1).mvd[a:b],color=color3
                  sid_legend,['Mean','Median Volume'],line=0,color=[color1,color3],charsize=1.0
               END
               'Triggers-Gain':BEGIN
                  plot,x,(*p1).largetrigger[a:b],ytitle='Raw Value',yr=[0,255],/ys,/nodata
                  oplot,x,(*p1).largetrigger[a:b],color=color1,thick=3
                  oplot,x,(*p1).smalltrigger[a:b],color=color2
                  oplot,x,(*p1).pmtgain[a:b],color=color3
                  sid_legend,['LargeTrig','SmallTrig','PMTGain'],line=0,color=[color1,color2,color3],charsize=1.0
               END
               'Rejection Codes':BEGIN
                  total_count=float((*p1).reject_count[a:b]+(*p1).accept_count[a:b])>1
                  plot,x,(*p1).reject_reason[a:b,1]/total_count*100,ytitle='Percent Rejected',yr=[0,105],/ys,psym=1,symsize=0.3
                  FOR i=1,7 DO oplot,x,(*p1).reject_reason[a:b,i]/total_count*100,color=70+(i-1)*30,psym=1,symsize=0.3
                  sid_legend,['Negative Det.','Time Range','Size Range','Interarrival','Saturation','Transit Time','TAS'],$
                      line=0,color=70+findgen(7)*30,charsize=1.0
               END
               'Particle Counts':BEGIN
                  maxy=max([(*p1).missed[a:b],(*p1).accept_count[a:b],(*p1).reject_count[a:b]])
                  plot,x,(*p1).missed[a:b],ytitle='Particle Count',yr=[0,maxy]
                  oplot,x,(*p1).missed[a:b],color=color4
                  oplot,x,(*p1).accept_count[a:b],color=color2
                  oplot,x,(*p1).reject_count[a:b],color=color3
                  sid_legend,['Accepted','Rejected','Missed'],line=0,color=[color2,color3,color4],charsize=1.0
               END
               'Speed':BEGIN
                  maxy=max([(*p1).speed[a:b],(*p1).tas[a:b]])>200
                  plot,x,(*p1).speed[a:b],ytitle='Velocity (m/s)',psym=3,yr=[0,maxy]
                  oplot,x,(*p1).tas[a:b],color=color3
                  sid_legend,['Particles','TAS'],color=[255,color3],line=[1,0],charsize=1.0
               END
               'Color Concentration':BEGIN
                   conc=alog10((*p1).conc1d[a:b,*] > 1)
                   contour,conc,x,(*p1).midbins,/cell,nlevels=20,ytitle='Diameter (um)',/yl,/ys,yr=sizerange,min_val=max(conc)-10
               END
               'Color Concentration (Round)':BEGIN
                   IF total(tag_names((*p1)) eq 'CONC_ROUND') THEN BEGIN
                      conc=alog10((*p1).conc_round[a:b,*] > 1)
                      contour,conc,x,(*p1).midbins,/cell,nlevels=20,ytitle='Diameter (um)',/yl,/ys,yr=sizerange,min_val=max(conc)-10
                   ENDIF
               END
               'Color Concentration (Non-Round)':BEGIN
                   IF total(tag_names((*p1)) eq 'CONC_IRREG') THEN BEGIN
                      conc=alog10((*p1).conc_irreg[a:b,*] > 1)
                      contour,conc,x,(*p1).midbins,/cell,nlevels=20,ytitle='Diameter (um)',/yl,/ys,yr=sizerange,min_val=max(conc)-10
                   ENDIF
               END
               'Color Asphericity':BEGIN
                   conc=alog10(((*p1).afspec[a:b,*]*1000) > 1)
                   contour,conc,x,(*p1).afmidbins,/cell,nlevels=20,ytitle='Asphericity',/yl,/ys,yr=afrange
               END
               'Color Circular Index':BEGIN
                   conc=alog10(((*p1).cispec[a:b,*]*1000) > 1)
                   contour,conc,x,(*p1).cimidbins,/cell,nlevels=20,ytitle='Circular Index',/ys,yr=[0,1]
               END
               'Color TB Concentration':BEGIN
                   tbsizerange=[min((*p1).tbendbins), max((*p1).tbendbins)]
                   conc=alog10((*p1).tbconc1d[a:b,*] > 1)
                   contour,conc,x,(*p1).tbmidbins,/cell,nlevels=20,ytitle='Diameter (um)',/yl,/ys,yr=tbsizerange,min_val=max(conc)-10            
               END               
               'Housekeeping1':BEGIN     
                   ;Compute laser power as in SIDConnect.c from Richard's code
                   IF total(tag_names((*p1)) eq 'LASERCAL') THEN lasercal=(*p1).lasercal ELSE lasercal=[0.0, 9.37e-05, -0.00042,0.969304, -100.0, 2047.0]
                   ft=(*p1).mux.tlaserpowermon/(raw2c)
                   fLPowCF = lasercal[1]*ft^2 + lasercal[2]*fT + lasercal[3]; 
						 fLPowCorrected = (*p1).mux.laserpowermon/fLPowCF;
						 laserpower = (fLPowCorrected*lasercal[4]/lasercal[5])>0
                
                   all=[laserpower,(*p1).mux.tlaser[a:b]/(raw2c),(*p1).mux.tlasercontrol[a:b]/(raw2c),(*p1).mux.tlaserpowermon[a:b]/(raw2c)]              
                   plot,x,(*p1).mux.tlaser[a:b]/(raw2c),yr=[min(all),max(all)]
                   oplot,x,(*p1).mux.tlasercontrol[a:b]/(raw2c),color=color1
                   oplot,x,(*p1).mux.tlaserpowermon[a:b]/(raw2c),color=color2
                   ;oplot,x,(*p1).mux.laserpowermon[a:b]/100,color=color3
                   oplot,x,laserpower[a:b],color=color3
                   sid_legend,['TLas','TLasCon','TLasPowMon','LasPow[mW]'],line=0,color=[255,color1,color2,color3],charsize=1.0
               END
               'Housekeeping2':BEGIN                   
                   all=[(*p1).mux.tdetector[a:b]/(raw2c),(*p1).mux.tfpgaambient[a:b]/(raw2c),(*p1).mux.theadambient[a:b]/(raw2c),(*p1).mux.theadambient[a:b]/(raw2c)]
                   plot,x,(*p1).mux.tdetector[a:b]/(raw2c),yr=[min(all),max(all)]
                   oplot,x,(*p1).mux.tfpgaambient[a:b]/(raw2c),color=color1
                   oplot,x,(*p1).mux.theadheater[a:b]/(raw2c),color=color2
                   oplot,x,(*p1).mux.theadambient[a:b]/(raw2c),color=color3
                   sid_legend,['TDet','TFPGAmb','THdHtr','THdAmb'],line=0,color=[255,color1,color2,color3],charsize=1.0
               END
               ELSE:dummy=0
            ENDCASE
         ENDFOR
         !x.style=0 & !x.tickformat='' & !x.title='' & !x.tickunits='' & !x.ticks=0 & !p.multi=[0,1,1,0,0]   
      END
   ENDCASE
END

