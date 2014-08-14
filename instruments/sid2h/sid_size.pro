FUNCTION sid_size, scatter_raw, scatter_adj, gain, transittime, tas, a30, peak=peak
   ;FUNCTION to find the size of a SID2H particle based on its scattering pattern.
   ;Also factors in gain setting and transit time or TAS.
   ;a30 is the gain to adjust based on water penetrations.
   ;nsat is the number of saturated detectors.
   ;Aaron Bansemer 4/2007
    
   ;Use only the positive, unsaturated detectors, return if fewer than 5 detectors are valid
   saturated=where(scatter_raw ge 2047,nsat)              
   good=where(scatter_adj gt 0, n)
   
   IF n gt 4 THEN scatter2=scatter_adj[good] ELSE return,{size:0, af:0, n:n, nsat:nsat, nneg:(28-n), branches:0, fftmax:0, tbsize:0} 

   ;Count branches/harmonics
   swrapped=convol(transpose(scatter_adj),[0.25,0.5,0.25],/edge_wrap)   ;smooth the particle
   f=fft(swrapped,-1)
   fftmax=max(abs(f[1:7])/f[0]*[0.5,0.5,1,1,1,1,1],maxf)  ;Underweight 1 and 2 a little
   branches=maxf+1
   IF fftmax lt 0.04 THEN branches=0   ;Water, threshold picked by eye 
   
   ;This is for early SID data where oblong coincident particles existed.
   IF branches eq 2 and fftmax lt 0.08 THEN branches=0 
                           

   ;Gain offset factor
   ;Changed 0.01618 to 0.027 based on lab calibrations in Hatfield.  Works for gain range 80-120.
   ;g=10^(2.393+0.027*gain)  
   ;again=0.062*sqrt(3.303e+6/g)      ;f0 is 0.062, g0 is 3.303e+6, from J Ulanowski calibration
   
   ;New gain curve based on lab calibrations at RAF (02/2009).  Normalized to gain of 100.
   again=4.0e11 * gain^(-5.8)
   
   ;Transit time factor for integrating detector... assume 1000ns (140m/s) is normal
   ;atime=1000.0/transittime
  
   ;Use TAS to compute transit time factor (140m/s default)
   atime=tas/140.0
   IF peak THEN atime=1.0
  
   ;Compute the mean scatter... adjust for saturated detectors
   meanscatter=mean(scatter2)*(1.0+float(nsat)/n)
   
   ;Compute the size now that all factors are known, *2 is for diameter
   size=a30*2*(atime*again*meanscatter)^0.53  
   
   ;Asphericity factor from Paul Field's article
   k=100.0/sqrt(n^2-n)
   z=total((scatter2-meanscatter)^2)   ;This may be wrong with a high nsat
   af=k*sqrt(z) / meanscatter
   
   ;Circular index from Yamamoto et al, Powder Technology 122 (2002) p205-211
   ci=float(total(scatter2))/n/max(scatter2)

   ;tbsize=((transittime*tas/1000.0))>0   ;Transit sizing, CALIBRATION ONLY, can subtract 210 for absolute sizing
   speed=(210.0/1.0e6)/(transittime/1.0e9)
   tbsize=8.9e6*(speed-tas/2+100.0)^(-2.67)  ;This is from an empirical formula comparing mnd to speed

   return,{size:size, af:af, ci:ci, n:n, nsat:nsat, nneg:(28-n), branches:branches, fftmax:fftmax, tbsize:tbsize, meanscatter:meanscatter}
END


;From J Ulanowski, 11/2007:
;size=f*sqrt(resp)
;I determined the coefficient f for maximum PMT gain to be 0.062, if size 
;(diameter of equivalent area sphere) is in microns and the average response 
;is read out in SID2 internal units. This assumes refractive index of 1.6.
;To correct for PMT gain you can use the folowing formula:
;(f/f0)2=g/g0
;where g is PMT gain and zero refers to reference value, and
;g=10^(2.393+0.01618*G)
;where G is the gain control number set in SID-2H software. 
