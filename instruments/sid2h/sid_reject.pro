FUNCTION sid_reject, a, tof, endbins, inttime, intbins, nsat, maxsaturated, tas, $
            laser_depth, min_meanscatter, speedreject=speedreject
   ;FUNCTION to decide whether a SID particle is accepted or rejected.
   ;a is the structure that comes from sid_size.pro
   ;Aaron Bansemer 12/2007

   ;Rejection criteria
   reject=0               
   IF (a.n lt 20) THEN reject=1                         ;Required number of detectors to have a positive reading
   IF (inttime lt intbins[0]) or (inttime gt max(intbins)) THEN reject=2 ;Out of intbins range
   IF (a.size lt endbins[0]) or (a.size gt max(endbins)) THEN reject=3   ;Out of endbins range
   IF (a.branches eq 1) THEN reject=4                   ;Vignettes
   IF nsat gt maxsaturated THEN reject=5                ;Too many saturated detectors
   IF tof le 550 THEN reject=6                          ;Get rid of extremely short TOF (minimum possible is 550)
   IF speedreject THEN BEGIN
      ispeed=(laser_depth/1.0e6) / (tof/1.0e9)          ;Use width of laser to compute speed
      IF (ispeed gt tas*3.0) or (ispeed lt tas/3.0) THEN reject=6  ;Transit time too fast/slow
   ENDIF
   IF (finite(tas) eq 0) or (tas lt 10) THEN reject=7   ;Unknown/slow air speed
   IF a.meanscatter lt min_meanscatter THEN reject=8    ;Require a minimum raw scattering value
   return,reject
END
