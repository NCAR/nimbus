Attribute VB_Name = "TBcalc"
' Arrays holding P, T, and RH at integration levels I
Public HI!(1 To 100), PrI!(1 To 100), Ti!(1 To 100), Ri!(1 To 100)
Public DelHI!(1 To 100), Hz!, Pz!, TZ!, Rz!, KnuAC!, sjkPextension As Boolean
Public userTextension As Boolean

' Arrays holding Z, P and T at retrieval levels
Public Hret!(1 To 33), Tret!(1 To 33), Pret!(1 To 33)

Public TB!(1 To 24), XKnu!(1 To 24, 1 To 100), Tau!(1 To 24, 1 To 100)
Public UP As Boolean, Htop!

' Frequency related parameters
Public Fif!(1 To 24)             'Up to 24 actual IF bandpass frequencies
Public LSBUSB!(1 To 3, 1 To 32)  'Up to 24 IF frequency offsets
Public BandWt!(1 To 3, 1 To 32)  'Weight assigned to each IF bandpass frequency
Public BandSum!(1 To 3)          'Sum of Band Weights
Public BandSumLSB!(1 To 3)
Public BandSumUSB!(1 To 3)
Public ExcessGndT!       'Excess ground temperature over lowest RAOB level

' RC Calculation Parameters
Public PRmin!    'RAOB must go higher than PRmin to be used (eg 25 mb)
Public NTB%      'Total number of frequencies = Nel*Nlo*Nnu
Public MonthMin% 'First month to use (1-12)

Public MonthMax% 'Last month to use (1-12 and >MonthMin)
Public NraobMax  'Maximum number of RAOBs to use
Public ALTs$
Public ExtendRAOBdown As Boolean

Const NHmax = 100
Sub Calc_Hret_Tret()
Dim j%
' This routine interpolates RAOB geometric height to obtain the geometric height (Hret)
' and temperature (Tret) at the 33 retrieval levels. Level 16 is the flight altitude.
' Note: RAOBs are converted from geopotential to geometric heights in GetValidRAOB

' First check to make sure first RAOB level is below aircraft
' If it isn't (eg RAOB may launch from a site above a/c level) then extrapolate down
  If Pz <= PZraob(1) Then  'Is a/c above first RAOB level?
    ExtendRAOBdown = False 'Yes
  Else
    ExtendRAOBdown = True  'No
  End If

' Calculate array of RETRIEVAL HEIGHTS AND TEMPERATURES
  i = 1   'Retrieval level index 1-33
  j = 0   'RAOB level index
  Do
    j = j + 1                               ' Get next RAOB level
    If j > Nlevr Then     ' Beyond top of RAOB
    ' Use Hypsometric Equation
    ' H = (Rd/Gamma45) 0.5*(Tv1+Tv2) ln(p1/p2)
    ' Rd/Gamma45 = 287.05307 J/kg K /9.80665 m/s2 = 0.02927
    ' alphaC = 0.02927 * Log(Pz / PZraob(Nlevr))
      Do
        alphaC = 0.02927 * Log(Pret(i) / PZraob(Nlevr))
        Hret(i) = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
        Tret(i) = TZraob(Nlevr) + LR2 * (Hret(i) - ZGraob(Nlevr))
        i = i + 1
      Loop Until i > Nret
      Exit Sub
    End If
    
    Do
      If Pret(i) < PZraob(j) Then Exit Do   ' Looking for RAOB level above Pret(i)
      Hret(i) = fZinterp(j, Pret(i))        ' j is RAOB level above Pret(i)
      Tret(i) = fTinterp2(j, Hret(i))
      If i = 16 Then
        Hz = Hret(16)                       ' Geometric altitude at flight level
        TZ = Tret(16)                       ' Temperature at Hz
      End If
      i = i + 1                             ' Get next retrieval level
      If i > Nret Then Exit Sub             ' Reached top retrieval level (Nret=33)
    Loop                                    ' Exit when i>Nret
  Loop                                      ' Exit when j>Nlevr
  Exit Sub
  

  
SetACparameters:

End Sub


Sub Calc_Hret_TretTBcorr()
Dim j%

' Interpolated RAOB at offset pressure levels to calculate Hret and Tret
  Hz = ZGraob(16)   'fZinterp(j, Pz)  'Geometric altitude
  TZ = TZraob(16)   'fTinterp2(j, Hz)  'Temperature at Hz
  For i = 1 To Nret
    Hret(i) = ZGraob(i)
    Tret(i) = TZraob(i)
  Next i
  Exit Sub
  GoTo 710
      
705:
  alphaC = 0.02927 * Log(Pz / PZraob(Nlevr))
  Hz = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
  TZ = TZraob(Nlevr) + 2# * (Hz - ZGraob(Nlevr)) ' 2 K/KM GRADIENT ABOVE RAOB

710:
' Calculate array of RETRIEVAL HEIGHTS AND TEMPERATURES
  i = 1   'Retrieval level index 1-33
  j = 1   'RAOB level index
720:
  j = j + 1
  If j > Nlevr Then GoTo 725   ' Beyond top of RAOB

722:
  If Pret(i) < PZraob(j) Then GoTo 720

  Hret(i) = fZinterp(j, Pret(i))
  Tret(i) = fTinterp2(j, Hret(i))
  i = i + 1
  If i > Nret Then Exit Sub
  GoTo 722

725:
  Do
    alphaC = 0.02927 * Log(Pret(i) / PZraob(Nlevr))
    Hret(i) = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
    Tret(i) = TZraob(Nlevr) + 2# * (Hret(i) - ZGraob(Nlevr))
    i = i + 1
  Loop Until i > Nret

End Sub

Function fExpInterpolationY(V!(), y!(), Yo, idn%, iup%)
Dim alpha!
  
  If y(iup) - y(idn) = 0 Then
    alpha = 0
  Else
    alpha = Log(V(idn) / V(iup)) / (y(iup) - y(idn))
  End If
  fExpInterpolationY = V(idn) * Exp(-1# * alpha * (Yo - y(idn)))

End Function

Function fRHinterp2!(i%, z!)
Dim slope!, j%
  j = i - 1
  If ZGraob(i) = ZGraob(j) Then
    fRHinterp2 = TZraob(j)
    Exit Function
  End If
  If ExtendRAOBdown And Pz > PZraob(j) Then
    fRHinterp2 = RZraob(j)
  Else
    slope = (RZraob(i) - RZraob(j)) / (ZGraob(i) - ZGraob(j))
    fRHinterp2 = RZraob(j) + slope * (z - ZGraob(j))
  End If
End Function

Function fTBsjk(pALT!, Elev!, Freq!, Levels%, Mode%)
Dim HI!(1 To 50), j%, jj%, i%, Ti!(1 To 50), Knu!(1 To 50), Tau!(1 To 50)
Static DelHI!(1 To 50), jm1%, Sum!, argUP!, argDN!, Tavg!
Dim T!, r!, P!, f!, SinEL!, NH%

f = Freq
SinEL = Sin(Elev * 3.14159 / 180)
' Parameters in COMMON: HI()
    For i = 1 To 10: DelHI(i) = 0.1: Next i
    For i = 11 To 20: DelHI(i) = 0.2: Next i
    For i = 21 To 25: DelHI(i) = 0.3: Next i
    For i = 26 To 30: DelHI(i) = 0.5: Next i
    For i = 31 To 33: DelHI(i) = 1#: Next i
    For i = 34 To 37: DelHI(i) = 2#: Next i
    For i = 38 To 50: DelHI(i) = 4#: Next i
    HI(1) = pALT
    Ti(1) = fTmdl(pALT, Mode)
    For j = 2 To 50
      HI(j) = HI(j - 1) + DelHI(j - 1)
      Ti(j) = fTmdl(HI(j), Mode)
      If (HI(j) >= 50#) Then Exit For
    Next j
    NH = j
  
  For j = 1 To NH
    T = Ti(j)
    P = fZtoP(HI(j))
    r = 0 'fRhoVsjk(RI(j), T)
    Knu(j) = OXLIEB93(T, P, r, f) + fVliebe(T, P, r, f)
  Next j

' Compute opacity profile in the vertical for each integration level
  Tau(1) = 0#
  For j = 2 To NH
    jm1 = j - 1
    Tau(j) = Tau(jm1) + 0.5 * (Knu(j) + Knu(jm1)) * (HI(j) - HI(jm1))
  Next j
  
  Sum = 0#
  For j = 1 To NH - 1
    argUP = Tau(j + 1) / SinEL
    argDN = Tau(j) / SinEL
    Tavg = (Ti(j) + Ti(j + 1)) / 2#
    Sum = Sum + Tavg * (Exp(-argDN) - Exp(-argUP))
  Next j

  fTBsjk = Sum + 2.75 * Exp(-Tau(NH) / SinEL)   'Add CMB

End Function



Function fCalcTB(Channel%, z0!, E!, f!)
' z0 is flight pressure altitude in km
' E  is elevation angle being calculated
' F  is LO frequency
Dim i%, Sum!, El!

' Calculate Brightness Temperatures along line of sight at each IF frequency
' and for each elevation angle
  El = E
  Call CalcBandTBs(El, TB())

' Calculate Bandpass weighted TBs at each elevation.
  Sum = 0#
  For i = 1 To 2 * Nif
    Sum = Sum + BandWt(Channel, i) * TB(i)
  Next i
  fCalcTB = Sum / BandSum(Channel)

End Function



Function fTinterp2!(i%, z!)
Dim slope!, j%
' i is top level
  j = i - 1
  If ZGraob(i) = ZGraob(j) Then
    fTinterp2 = TZraob(j)
    Exit Function
  End If
  If i <= Nlevr Then  'Interpolate
    If ExtendRAOBdown And Pz > PZraob(j) Then
      slope = -6.5
      fTinterp2 = TZraob(j) + slope * (z - ZGraob(j))
    Else
      slope = (TZraob(i) - TZraob(j)) / (ZGraob(i) - ZGraob(j))
      fTinterp2 = TZraob(j) + slope * (z - ZGraob(j))
    End If
  Else
    If ZGraob(i) < 50 Then    'Use slope of LR2 K/km to 50 km
      fTinterp2 = TZraob(j) + LR2 * (z - ZGraob(j))
    Else                      'Use US Standard atmosphere above 50 km
      fTinterp2 = fTstd(z)
    End If
  End If
End Function

Function fZinterp!(i%, Pz!)
Dim slope!, j%
' Interpolate ln(p) vs Z
' j is lower level, i is upper level
'
  j = i - 1
  If j = 0 Then i = 2: j = 1
  If ZGraob(i) = ZGraob(j) Then
    fZinterp = ZGraob(j)
    Exit Function
  End If
  If ExtendRAOBdown And Pz > PZraob(j) Then  'Flight level is above lower RAOB level
    slope = Log(1013.25 / PZraob(i)) / ZGraob(i)
    fZinterp = -(1# / slope) * Log(Pz / 1013.25)
  Else
    slope = Log(PZraob(j) / PZraob(i)) / (ZGraob(i) - ZGraob(j))
    fZinterp = ZGraob(j) - (1# / slope) * Log(Pz / PZraob(j))
  End If
End Function


Sub Calc_Hret_Tret_SJK()
Dim j%
' This routine assume 2 K/km above A/C
' This is not valid for low altitude retrieval levels!!!!!
' Fix this.
' This routine is based on Steve Keihm's MTPOBERD.FOR program
' Note: RAOBs are converted from geopotential to geometric heights in GetValidRAOB
'
' First check to make sure first RAOB level is below aircraft
' If it isn't (eg RAOB may launch from a site above a/c level) then extrapolate down
' Use Hypsometric Equation
' H = (Rd/Gamma45) 0.5*(Tv1+Tv2) ln(p1/p2)
'
' Rd/Gamma45 = 287.05307 J/kg K /9.80665 m/s2 = 0.02927
'  alphaC = 0.02927 * Log(Pz / PZraob(Nlevr))

  If Pz <= PZraob(1) Then  'Is a/c above first RAOB level?
    ExtendRAOBdown = False 'Yes
  Else
    ExtendRAOBdown = True  'No
  End If

' Interpolated RAOB at offset pressure levels to calculate Hret and Tret
  j = 1   'RAOB level
  Do: j = j + 1: If j > Nlevr Then GoTo 705
  Loop Until Pz > PZraob(j)  'Find first RAOB level above A/C (=Pz)
' First RAOB level above Pz
  Hz = fZinterp(j, Pz)   'Geometric altitude at flight level
  TZ = fTinterp2(j, Hz)  'Temperature at Hz
  Rz = fRHinterp2(j, Hz) 'RH at Hz
  GoTo 710
      
705: ' Beyond top to sounding
  Hz = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
  TZ = TZraob(Nlevr) + LR2 * (Hz - ZGraob(Nlevr)) ' LR2 K/KM GRADIENT ABOVE RAOB
  Rz = fRHinterp2(j, Hz)

710:
' Calculate array of RETRIEVAL HEIGHTS AND TEMPERATURES
  i = 1   'Retrieval level index 1-33
  j = 1   'RAOB level index
720:
  j = j + 1
  If j > Nlevr Then GoTo 725            ' Beyond top of RAOB

722:
  If Pret(i) < PZraob(j) Then GoTo 720  ' Looking for RAOB level above Pret(i)

  Hret(i) = fZinterp(j, Pret(i))        ' j is RAOB level above Pret(i)
  Tret(i) = fTinterp2(j, Hret(i))
  i = i + 1
  If i > Nret Then Exit Sub
  GoTo 722

725:  'Beyond top of RAOB (j>Nlevr)
  Do
    alphaC = 0.02927 * Log(Pret(i) / PZraob(Nlevr))
    Hret(i) = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
    Tret(i) = TZraob(Nlevr) + LR2 * (Hret(i) - ZGraob(Nlevr))
    i = i + 1
  Loop Until i > Nret

End Sub

Sub TPRatZgLevels(Nlevr%)
Dim iup%, idn%, jf%, j%, i%, slope!, alpha!
' This subroutine computes the temperature,pressure and relative humidity
' profiles at the discrete geometric height levels used for brightness
' temperature calculations. Temperature and relative humidity values are
' calculated by linear interpolation from the radiosonde height levels.
' Pressures are computed by exponential interpolation using the hydrostatic
' equation with adjustment for air temperature.
' Enter with:
' Nlevr ...... RAOB levels
' ZGraob(), TZraob(), RZraob(), PZraob() are Global RAOB variables
' Integration levels HI() are Global Variables from earlier CALL to CalcZgLevels()
' Exit with:
' TI(), RI() and PrI() at integration levels for NH geometric height levels
  
  j = 1:  iup = 2
  Do
    Do 'Find index (iup) of first RAOB level above HI(j)
      If (HI(j) <= ZGraob(iup)) Then Exit Do
      iup = iup + 1
    Loop Until iup > Nlevr Or j = NH

    If (HI(j) <= ZGraob(iup)) Then
      idn = iup - 1
      Ti(j) = fLinInterpolationY(TZraob(), ZGraob(), HI(j), idn%, iup%)
      Ri(j) = fLinInterpolationY(RZraob(), ZGraob(), HI(j), idn%, iup%)
      PrI(j) = fExpInterpolationY(PZraob(), ZGraob(), HI(j), idn%, iup%)
      j = j + 1
      If j > NH Then Exit Sub   'Finished
      jf = j

    Else  'Extend RAOB profiles and exit sub
      For j = jf To NH  'Extend Temperature and RH
        'Need flag to distinguish User Extension of temperature profile vs Default
        '(2 K/km to 50 km and US Standard Atmosphere above 50 km)
        If userTextension Then
          If ZGraob(Nlevr) > zLRb Then
            Ti(j) = TZraob(Nlevr) + LR2 * (HI(j) - ZGraob(Nlevr))
          Else
            Ti(j) = TZraob(Nlevr) + LR1 * (zLRb - ZGraob(Nlevr)) + LR2 * (HI(j) - zLRb)
          End If
        Else
          If HI(j) < 50 Then 'Use Lapse Rate of 2 K/km
            Ti(j) = Ti(j - 1) + 2 * (HI(j) - HI(j - 1))
          Else                       'Use US Standard Atmosphere
            Ti(j) = fTstd(HI(j))
          End If
        End If
        Ri(j) = 0#
      Next j
      If jf < 1 Then jf = 1

      If sjkPextension Then 'Use last alpha value through remaining levels
        alpha = Log(PZraob(Nlevr - 1) / PZraob(Nlevr)) / (ZGraob(Nlevr) - ZGraob(Nlevr - 1))
        For j = jf To NH
          PrI(j) = PZraob(Nlevr) * Exp(-1# * alpha * (HI(j) - ZGraob(Nlevr)))
        Next j
      Else                'Use Hypsometric Equation to get P for given Zg change
        For j = jf To NH
          PrI(j) = fPypsometricEqn(Ti(j - 1), PrI(j - 1), Ti(j), HI(j) - HI(j - 1))
        Next j
      End If
      Exit Sub
    End If
  Loop

End Sub
Sub TPRPRFDC8(Nlevr%)
' Enter with:
' Nlevr ...... RAOB levels
' ZGraob(), TZraob(), RZraob(), PZraob() in COMMON
' Integration levels HI() in COMMON from earlier CALL to CalcZgLevels()
' Exit with:
' TI(), RI() and PrI() at integration levels for NH levels
'****************************************************************
' THIS SUBROUTINE COMPUTES THE TEMPERATURE, PRESSURE,
' AND RELATIVE HUMIDITY PROFILES AT THE DISCRETE HEIGHT
' LEVELS USED FOR BRIGHTNESS TEMP. CALCULATIONS. TEMPERATURE
' AND RELATIVE HUMIDITY VALUES ARE CALCULATED BY LINEAR
' INTERPOLATION FROM THE RADISONDE HEIGHT LEVELS.
' PRESSURES ARE COMPUTED BY EXPONENTIAL INTERPOLATION
' USING THE HYDROSTATIC EQUATION WITH ADJUSTMENT FOR
' AIR TEMPERATURE.
'***************************************************************
Dim iup%, idn%, jf%, j%, i%, slope!, alpha!
  j = 1
  iup = 2
21:
  Do
    'Debug.Print j; HI(j); Iup; ZGraob(Iup)
    If (HI(j) <= ZGraob(iup)) Then Exit Do
    iup = iup + 1
    If (iup > Nlevr) Then
      GoTo 25
    End If
  Loop
'  TI(j) = fTinterp(iup, HI(j))

  idn = iup - 1
  If ZGraob(iup) - ZGraob(idn) = 0 Then
    slope = 0
  Else
    slope = (TZraob(iup) - TZraob(idn)) / (ZGraob(iup) - ZGraob(idn))
  End If
  If Abs(slope) > 20 Then slope = 0 'Crash with neg T if delta ZG too small
  Ti(j) = TZraob(idn) + slope * (HI(j) - ZGraob(idn))
  If ZGraob(iup) - ZGraob(idn) = 0 Then
    slope = 0
  Else
    slope = (RZraob(iup) - RZraob(idn)) / (ZGraob(iup) - ZGraob(idn))
  End If
  Ri(j) = RZraob(idn) + slope * (HI(j) - ZGraob(idn))
'  PrI(j) = fZinterp(iup, HI(j))
  If ZGraob(iup) - ZGraob(idn) = 0 Then
    alpha = 0
  Else
    alpha = Log(PZraob(idn) / PZraob(iup)) / (ZGraob(iup) - ZGraob(idn))
  End If
  PrI(j) = PZraob(idn) * Exp(-1# * alpha * (HI(j) - ZGraob(idn)))
  j = j + 1
  If j > NH Then Exit Sub   'Above A/C
  jf = j
  GoTo 21

25:
  If ZGraob(Nlevr) = ZGraob(Nlevr - 1) Then
    alpha = 0
  Else
    alpha = Log(PZraob(Nlevr - 1) / PZraob(Nlevr)) / (ZGraob(Nlevr) - ZGraob(Nlevr - 1))
  End If
  If jf < 1 Then jf = 1
  For j = jf To NH
'   TI(J)=RTEM(Nlevr)                          ' ISOTHERM. EXTRAP. FOR 45S TO 60 S RAOBS
    If ZPraob(Nlevr) > zLRb Then
      Ti(j) = TZraob(Nlevr) + LR2 * (HI(j) - ZGraob(Nlevr))
    Else
      Ti(j) = TZraob(Nlevr) + LR1 * (zLRb - ZGraob(Nlevr)) + LR2 * (HI(j) - zLRb)
    End If
    If Ti(j) > 275 Then Ti(j) = 275
    Ri(j) = 0#
    PrI(j) = PZraob(Nlevr) * Exp(-1# * alpha * (HI(j) - ZGraob(Nlevr)))
  Next j

End Sub

Function fLinInterpolationY(V!(), y!(), Yo, idn%, iup%)
Dim slope!
      
  idn = iup - 1
  If y(iup) - y(idn) = 0 Then
    slope = 0
  Else
    slope = (V(iup) - V(idn)) / (y(iup) - y(idn))
  End If
  If Abs(slope) > 30 Then slope = 0 'Crash with neg T if delta ZG too small
  fLinInterpolationY = V(idn) + slope * (Yo - y(idn))

End Function

Sub CalcBandTau(Fif!())
' Integrate absorption over levels for each frequency Fif(i) in bandpass
Dim T!, P!, r!, f!, Knu!(1 To 24, 1 To 100), i%, j%, jm1%, Rl!, flag(1 To 4) As Boolean
Rl = 0#: flag(1) = True: flag(2) = True: flag(3) = True: flag(4) = False

For i = 1 To 2 * Nif
  f = Fif(i)
  For j = 1 To NH
    T = Ti(j)
    r = fRhoVsjk(Ri(j), T)
    P = PrI(j)
    Knu(i, j) = ABS_TOT(T, P, r, Rl, f, flag())
'   Knu(i, j) = OXLIEB93(T, P, R, f) + fVliebe(T, P, R, f)
    'Debug.Print i; j; Knu(i, j)
  Next j

' Compute opacity profile in the vertical for each integration level
  Tau(i, 1) = 0#
  For j = 2 To NH
    jm1 = j - 1
    Tau(i, j) = Tau(i, jm1) + 0.5 * (Knu(i, j) + Knu(i, jm1)) * (HI(j) - HI(jm1))
    'Debug.Print i; j; HI(j); Tau(i, j)
  Next j
  DoEvents
Next i

End Sub

Sub CalcBandTBs(El!, TB!())
Dim i%, j%, SinEL!, Sum!, argUP!, argDN!, Tavg!, TRANGD!

  SinEL = Sin(El * 3.1415927 / 180#)
  If El > 0 Then
    For i = 1 To 2 * Nif
      Sum = 0#
      For j = 1 To NH - 1
        argUP = Tau(i, j + 1) / SinEL
        argDN = Tau(i, j) / SinEL
        Tavg = (Ti(j) + Ti(j + 1)) / 2#
        Sum = Sum + Tavg * (Exp(-argDN) - Exp(-argUP))
      Next j
      TB(i) = Sum + 2.75 * Exp(-Tau(i, NH) / SinEL)  'Add CMB
If El = 80 Then
      'Debug.Print i; TB(i); argUP; argDN
      j = j
      End If
'      If i < 6 Then Debug.Print i; Elevation(i); TB(i); 2.75 * Exp(-Tau(i, NH) / SinEl)
    Next i
  Else
    For i = 1 To 2 * Nif
      Sum = 0#
      For j = 1 To NH - 1
        argUP = (Tau(i, NH) - Tau(i, j + 1)) / SinEL
        argDN = (Tau(i, NH) - Tau(i, j)) / SinEL
        Tavg = (Ti(j) + Ti(j + 1)) / 2#
        If argDN > 50 Then argDN = 10
        Sum = Sum + Tavg * (Exp(argUP) - Exp(argDN))
      Next j
      TRANGD = Tau(i, NH) / SinEL
      If (TRANGD > 20#) Then TRANGD = 20#
      TB(i) = Sum + (Ti(1) + ExcessGndT) * Exp(TRANGD)
    Next i
  End If
  

End Sub
Sub CalcRetLevels(z0!)
Dim j%, alphaC!, Plowest!
' Enter with Pz and Pret(i)
' Exit with corresponding Hz and Tz (geometric) at a/c
' Parameters in COMMON: Zr(),NFL,Pret(),Nret, Pz, Hz, Tz, TXraob arrays
' and Retrieval Heights Hret(i) and Temperatures Tret(i)

' Pressure altitude offsets Dz wrt to retrieval level, z0
  Nret = 33
  dZ(1) = -9.5: dZ(2) = -8#: dZ(3) = -6.5: dZ(4) = -5.1: dZ(5) = -4#: dZ(6) = -3.2
  dZ(7) = -2.5: dZ(8) = -2#: dZ(9) = -1.6: dZ(10) = -1.3: dZ(11) = -1#: dZ(12) = -0.7
  dZ(13) = -0.5: dZ(14) = -0.3: dZ(15) = -0.15: dZ(16) = 0#: dZ(17) = 0.15: dZ(18) = 0.3
  dZ(19) = 0.5:  dZ(20) = 0.7: dZ(21) = 1#: dZ(22) = 1.3: dZ(23) = 1.6: dZ(24) = 2#
  dZ(25) = 2.5: dZ(26) = 3.2: dZ(27) = 4#: dZ(28) = 5.2: dZ(29) = 6.9: dZ(30) = 9#
  dZ(31) = 11.4: dZ(32) = 14.5: dZ(33) = 18#

  Plowest = fZtoP(Zr(NFL)) 'Pressure at lowest retrieved altitude
  
' Convert pressure altitude offset wrt z0 to pressures
  For j = Nret To 1 Step -1         'Highest to lowest
    Pret(j) = Int(fZtoP(z0 + dZ(j)) * 100#) / 100#
    'Debug.Print j; Pret(j)
    If Pret(j) > 1013.25 Then Pret(j) = 1013.25
    'xxx 19990910
  Next j

End Sub

Sub CalcZgLevels(Elevation!, SinElCorrection As Boolean)
Dim HDOWN!(1 To 100), j%, jj%, i%, ScaleFactor!, Segments%, r1!, r2!, k!, SinEL!
' Parameters in COMMON: HI()

' SPECIFY HEIGHT INCREMENTS FOR NUMERICAL INTEGRATION.
' THESE VALUES HAVE BEEN CHOSEN TO INSURE NUMERICAL ACCURACY
' FOR A RANGE OF FEASIBLE OXYGEN CHANNEL WEIGHTING FUNCTIONS.
  If SinElCorrection Then
    If Abs(Elevation) > 0 Then ScaleFactor = Abs(Sin(Elevation * Atn(1) / 45#)) Else ScaleFactor = 0.05
    ScaleFactor = Int(ScaleFactor * 10000#) / 10000#  'avoid numerical noise
  Else
    ScaleFactor = 1
  End If
  If Elevation >= 0 Then
    For i = 1 To 10: DelHI(i) = 0.1 * ScaleFactor: Next i    '1 km
    For i = 11 To 20: DelHI(i) = 0.2 * ScaleFactor: Next i   '2 km
    For i = 21 To 25: DelHI(i) = 0.3 * ScaleFactor: Next i   '1.5 km
    For i = 26 To 30: DelHI(i) = 0.5 * ScaleFactor: Next i   '2.5 km
    For i = 31 To 33: DelHI(i) = 1# * ScaleFactor: Next i    '3
    For i = 34 To 37: DelHI(i) = 2# * ScaleFactor: Next i    '8
    For i = 38 To 50: DelHI(i) = 4# * ScaleFactor: Next i    '52
  Else
    For i = 1 To 10: DelHI(i) = 0.1 * ScaleFactor: Next i    '1
    For i = 11 To 20: DelHI(i) = 0.2 * ScaleFactor: Next i   '2
    For i = 21 To 25: DelHI(i) = 0.3 * ScaleFactor: Next i   '1.5
    For i = 26 To 32: DelHI(i) = 0.5 * ScaleFactor: Next i   '3.5
    For i = 33 To 38: DelHI(i) = 1# * ScaleFactor: Next i    '6
    For i = 39 To 41: DelHI(i) = 2# * ScaleFactor: Next i    '6
    DelHI(42) = 4#                                           '4
    For i = 43 To 50: DelHI(i) = 0#: Next i
  End If
  
  GoTo NextStep
  Segments = 50
  SinEL = Sin(Elevation * 3.14159 / 180)
  k = 1 * KnuAC * SinEL
  If Elevation > 0 Then
    r1 = -Log(1 - 1 / Segments) / k
    DelHI(1) = SinEL * r1
    For i = 2 To 50
      r2 = -Log(Exp(-k * r1) - 0.9999 / Segments) / k
      DelHI(i) = (r2 - r1) * SinEL
      r1 = r2
    Next i
    NH = 50
  Else
    GoTo NextStep
    DelHI(1) = -Log(1 - 1 / Segments) / k
    r1 = DelHI(1)
    For i = 2 To NHmax
      r2 = -Log(Exp(-k * r) - 0.9999 / Segments) / k
      DelHI(i) = r2 - r1
      r1 = r2
      If r1 * Sin(Elevation * 3.14159 / 180) >= Hz Then NH = i: Exit For
    Next i
  End If
  If NH <> NHmax Then
    For i = NH + 1 To NHmax: DelHI(i) = 0#: Next i
  End If
NextStep:
' Calculate Heights HI(i) for numerical integration
' Enter with Hz, geo height at a/c
' Exit with HI(i) height above or below a/c arranged so i=1 is at a/c
  If Elevation > 0 Then
' ****************************************************************
' DETERMINE HEIGHTS ABOVE GROUND TO BE USED IN NUMERICAL TB CALCULATIONS.
' HEIGHTS ABOVE HTOP KM ARE NOT USED.
' ****************************************************************
    HI(1) = Hz
    For j = 2 To 50 'NH
      HI(j) = HI(j - 1) + DelHI(j - 1)
      'Debug.Print j; HI(j)
      If (j = 50) Then Exit For
    Next j
    NH = j
  
  Else
' ************************************************************
' DETERMINE NODE DISTANCES IN KM BELOW AIRCRAFT FOR TB CALCULATIONS.
' *************************************************************
    HDOWN(1) = 0#
    For j = 2 To 43
      HDOWN(j) = HDOWN(j - 1) + DelHI(j - 1)
      If (HDOWN(j) >= (Hz - 0.01)) Then Exit For
      'Debug.Print j; HDOWN(j)
    Next j
    NH = j
    HDOWN(NH) = Hz
'   CONVERT DOWNWARD HEIGHT NODES INTO NORMAL UPWARD HEIGHT NODES.
    For j = 1 To NH: jj = NH + 1 - j: HI(j) = Hz - HDOWN(jj): Next j
  End If

End Sub


