Attribute VB_Name = "Physics"
Option Explicit
' Physical Constants
'                                        Units        Description
Public Const cRo = 8314.32              'J/kmole K    Universal Gas Constant (/mole)
Public Const cRd = 287.05307            'J /kg K      Gas Constant (dry air)
Public Const cRs = 287.05307            'J /kg K      Gas Constant Std Atmosphere(dry air)
Public Const cRv = 461.9066634          'J /kg K      Gas Constant (water vapor)
Public Const cGn = 0.000000000066742    'm3/kg s2     Newtonian Constant of Gravitation (NIST 2005)
Public Const cg = 9.80665               'm /s2        Acceleration of gravity(surface)
Public Const cg45 = 9.80616             'm /s2        Acceleration of gravity at Lat=45 deg
Public Const cg20 = 9.75                'm/s2         Acceleration of gravity(at 20 km)
Public Const cTa = 260#                 'K            Avg Atmospheric Temperature
Public Const cLR = -6.5                 'K/km         Avg Atmospheric Lapse Rate
Public Const cTo = 273.15               'K            Freezing Temperature of Water (1 atm)
Public Const cTs = 288.15               'K            Surface Temperature for Std Atmosphere
Public Const cAo = 6.023E+26            '/kmole       Avagadro 's Constant
Public Const cMd = 28.9644              'kg/kmole     Avg Molecular Weight (dry air)
Public Const cMw = 18.016               'kg/kmole     Molecular Weight (water vapor)
Public Const cEa = 0.622004944          '             cMw/cMd
Public Const ckft_km = 3.2808           'kft/km       kft per km
Public Const cft_km = 3280.8            'ft/km        feet per km
Public Const cC = 299792458#            'm/s          Speed of Light
Public Const cCp = 1004                 'J /kg K      Atmospheric Specific Heat (constant pressure)
Public Const cCv = 717                  'J /kg K      Atmospheric Specific Heat (constant volume)
Public Const ch = 6.63E-34              'J*s          Planck Constant
Public Const ckB = 1.38E-23             'J/K          Boltzmann Constant
Public Const cfe = 0.00335289           '             Earth's flattening factor (Clemence 1965)
Public Const cRe = 6371000#             'm            Mean Earth Radius
Public Const cRee = 6378137#            'm            Radius of Earth at equator (WGS-84)
Public Const cRep = 6356752.3142        'm            Radius of Earth at pole (WGS-84)
Public Const cMe = 5.977E+24            'kg           Mass of Earth
Public Const cSB = 0.0000000567         'W/m2 K4      Stefan-Boltzmann constant
Public Const cPs = 1013.25              'mb           Surface Pressure for Std Atmosphere
Public Const cPI = 3.14159265358979     '             Pi
Public Const rpd = 1.74532925199433E-02 '             Radians per Degree
Public Const cCso = 340.328005473541    'm/s          Speed of sound on the ground for a Standard Atmosphere
Public Const cLv = 2.54 * 10 ^ 6        'J/Kg         Latent heat of vaporization
Public Const cNm = 1852                 'm            Nautical mile, http://www1.bipm.org/en/si/si_brochure/chapter4/table8.html
Public Const cKt = 0.514444444444       'm/s          Knots in m/s
Public Const cMPH = 0.44704543607       'm/s          Miles per Hour in m/s
Public Const cKt_MPH = 1.15076545454545 '             Knots per Mile per hour
Public Const cMile = 1609.36356986101   'm            Meters per Mile

Function fAbsVsZp(ch%, Zp!)
' Calculate Absorption Vs Zp based on fits in Efold.xls
' Note that USB and LSB used with freq offset of 0.2833 GHz
' And T was for US Standard Atmosphere
' Only valid from 10 to 22 km
'  If Zp = 0# Then Exit Function
'  Select Case ch
'  Case 1 'Power Law fit was best R2=0.9803 from 10 to 22 km
'    fAbsVsZp = 16.529 * Zp ^ (-1.3424)
'  Case 2 'Power Law fit was best R2=0.9980 from 10 to 22 km
'    fAbsVsZp = 27.947 * Zp ^ (-1.1928)
'  Case 3 'Exponential fit was best R2=0.9945 from 10 to 22 km
'    fAbsVsZp = 13.866 * Exp(-0.1301 * Zp)
'  End Select
' New fits 0 to 50 km from O2absVsRH.xls
  
  Select Case ch
  Case 1 'Power Law fit was best R2=0.9803 from 10 to 22 km
    fAbsVsZp = 1.3158 - 0.1493 * Zp + 0.007 * Zp ^ 2 - 0.0001 * Zp ^ 3 + 0.000001 * Zp ^ 4
  Case 2 'Power Law fit was best R2=0.9980 from 10 to 22 km
    fAbsVsZp = 2.1981 - 0.2004 * Zp + 0.0086 * Zp ^ 2 - 0.0002 * Zp ^ 3 + 0.000001 * Zp ^ 4
  Case 3 'Exponential fit was best R2=0.9945 from 10 to 22 km
    If Zp > 11 Then
      fAbsVsZp = 1.2325 - 0.0166 * Zp + 0.0002 * Zp ^ 2 - 0.00005 * Zp ^ 3 + 0.0000003 * Zp ^ 4
    Else
      fAbsVsZp = 3.1476 - 0.1674 * Zp + 0.0032 * Zp ^ 2
    End If
  End Select
 
End Function

Function fAltimeterSetting(Pf!, Zf!, Unit%)
Dim alpha!, x!
' Alpha = 10000 Gamma45 / (Rd LRo)
' Gamma45 = 9.80665 m/s2
' Rd = cRd = 287.05307            'J /kg K      Gas Constant (dry air)
' LR0 = -6.5 K/km
' If unit =1, Pf (hPa), Zf (km)
' If unit =2, Pf (in Hg), Zf (feet)
' Do calculation in metric units
  Select Case Unit
  Case 1
  Case 2: Pf = Pf * 1013.25 / 29.92: Zf = Zf / 3280.8 'convert to hPa and km
  End Select
  alpha = 5.2558761507598
  x = (Pf ^ (1 / alpha) + 1013.25 ^ (1 / alpha) * 6.5 * Zf / 288.15) ^ alpha
  Select Case Unit
  Case 1:  fAltimeterSetting = x
  Case 2: fAltimeterSetting = x * 29.92 / 1013.25
  End Select
End Function

Sub FindXmin(n1%, n2%, x!(), y!(), y1!, y2!, Xmin!, Ymin!)
' For a profile y vs x find value of y (Ymin) at which x is is minimum (Xmin)
' With the constraint that y must satisfy y1 <= y <= y2
Dim j%

  Xmin = 10000000000#
  For j = n1 To n2
    If y(j) >= y1 And y(j) <= y2 Then 'Check that y is in required range
      If x(j) < Xmin Then  'Check for new minimum
        Xmin = x(j)
        Ymin = y(j)
      End If
    End If
  Next j

End Sub

Function fPtFromPsM(Ps!, M!)
'Calculate Pt from Ps and Mach Number
Dim gamma!
gamma = cCp / cCv

fPtFromPsM = Ps * Exp((gamma / (gamma - 1)) * Log(1 + 0.5 * (gamma - 1) * M ^ 2))
End Function

Function fPsFromPtM(pt!, M!)
'Calculate Pt from Ps and Mach Number
Dim gamma!
gamma = cCp / cCv

fPsFromPtM = pt / Exp((gamma / (gamma - 1)) * Log(1 + 0.5 * (gamma - 1) * M ^ 2))
End Function

Function fRHv(ByVal V!, ByVal T!)
' Invert fRhoV (RH,T) to get RH from V and T
' V (g/m3), T (K)

fRHv = (T / fEs(T)) * (cRd / cEa) * V / 1000#

End Function

Function fTfromTheta(Th!, P!)
If P > 0 Then fTfromTheta = Th * (1000 / P) ^ -0.286 Else fTfromTheta = 999.9
End Function

Function fTsFromTtMR(Tt!, M!, r!)
'Calcuate Tt from Ts, Mach Number, and recovery factor
Dim gamma!
gamma = cCp / cCv
fTsFromTtMR = Tt / (1 + r * ((gamma - 1) / 2#) * M ^ 2)

End Function
Function fTtFromTsMR(Ts!, M!, r!)
'Calcuate Tt from Ts, Mach Number, and recovery factor
Dim gamma!
gamma = cCp / cCv

fTtFromTsMR = Ts * (1 + r * ((gamma - 1) / 2#) * M ^ 2)

End Function


Function fCalculate_Layer_Avg_T(z!(), T!(), Nr%, Zp!, Thickness!)
' Calculate the average temperature in atmospheric layer of Thickness centered on Zp
'
Dim ZpB!, zPT!, ib%, it%, i%, Tbot!, Ttop!, Tsum!, Zsum!

  ZpB = Zp - Thickness / 2
  zPT = Zp + Thickness / 2
  i = 0        'RAOB index
  ib% = 0      'RAOB bottom index
  Do
    ib = ib + 1
  Loop Until z(ib) >= ZpB
  ib = ib - 1
  If ib = 0 Then ib = 1: ZpB = z(1)
' Find RAOB top index
  it = ib      'RAOB top index
  Do
    it = it + 1
  Loop Until z(it) >= zPT Or it = Nr
  
  Tbot = T(ib) + (T(ib + 1) - T(ib)) * (ZpB - z(ib)) / (z(ib + 1) - z(ib))
  Ttop = T(it - 1) + (T(it) - T(it - 1)) * (zPT - z(it - 1)) / (z(it) - z(it - 1))
' Calculate layer average T
  If it = ib + 1 Then  'No data points in layer
    fCalculate_Layer_Avg_T = 0.5 * (Tbot + Ttop)
  Else
    Tsum = (T(ib + 1) + Tbot) * (z(ib + 1) - ZpB)
    Zsum = (z(ib + 1) - ZpB)
    For i = ib + 1 To it - 2
      Tsum = Tsum + (T(i) + T(i + 1)) * (z(i + 1) - z(i))
      Zsum = Zsum + (z(i + 1) - z(i))
    Next i
    Tsum = Tsum + (T(it - 1) + Ttop) * (zPT - z(it - 1))
    Zsum = Zsum + (zPT - z(it - 1))
    fCalculate_Layer_Avg_T = 0.5 * Tsum / Zsum
  End If
  
End Function

Sub Calculate_MLS_Levels(Zmls!())
' mjm ... 20050214
' Calculate MLS L3 retrieval levels
' Zmls() is indexed from 0 to 48
' Even indices correspond to retrieval levels
' Odd indices correspond to levels between retrieval levels
' These are needed so that layer averaging of MTP profiles can be done
' See: http://auc.dfd.dlr.de/MLS/MLS_dataset.html#9.
' P(i) = 1000 x 10**(-i/6) mb, where i=0,1,2,...24
' P(i) = 1000 x 10**(-i/3) mb, where i=25, 29, ...36
' UARS retrievals 10-85 km (48 levels goes to 0.1 hPa = 65 km)
' Also: MTP_Retrieval_Levels.xls
Dim i%

  For i = 0 To 48                             'MLS levels 1 to 25 are even numbers
    Zmls(i) = fPtoZ(1000# * 10# ^ (-i / 12#))
  Next i
  For i = 49 To 73
    Zmls(i) = fPtoZ(1000# * 10# ^ (-i / 6#))  'MLS levels 26 to 37 are even numbers
  Next i

End Sub
Sub Calculate_TES_Levels(Ztes!())
' mjm ... 20050214
' Calculate TES L3 retrieval levels
' Above 1000 hPa, P(i) = 1000 x 10**(-i/24) mb, where i=0,1,2,...37
' P(i) = 1000 x 10**(-i/3) mb, where i=25, 29, ...36
Dim i%

  For i = 0 To 37                             'MLS levels 1 to 25 are even numbers
    Ztes(i) = fPtoZ(1000# * 10# ^ (-i / 24#))
  Next i

End Sub
Sub Calculate_MLS_Profile(z!(), T!(), Nr%, Zmls!(), Tmls!(), Nmls%)
' Enter with T-profile
' Return with MLS profile, Zmls(), Tmls()
Dim i%, ib%, it%, j%, k%, L%, LB%, Lt%, Ttop!, Tbot!, Tsum!, Zsum!
Static Initialized As Boolean, Zm!(0 To 73)

  If Not Initialized Then
    Call Calculate_MLS_Levels(Zm()) 'Even numbers are MLS levels, Odd intermediate levels
    Initialized = True              'Index 48 is 65 km = 0.0001 hPa
  End If
  For i = 1 To 37: Zmls(i) = 0#: Next i
' Find MLS inter-layer with RAOB data below it
  i = 0        'RAOB index
  ib% = 1      'RAOB bottom index
  L = 0        'MLS out index
  LB = -1      'Bottom inter-layer index
  Do
    LB = LB + 2
  Loop Until Zm(LB) >= z(ib)
  Do
    ib = ib + 1
  Loop Until z(ib) >= Zm(LB)
  ib = ib - 1
  Lt = LB + 2  'Top inter-layer index
' Find RAOB top index
  it = ib      'RAOB top index
  Do
    it = it + 1
  Loop Until z(it) >= Zm(Lt)
  Tbot = T(ib) + (T(ib + 1) - T(ib)) * (Zm(LB) - z(ib)) / (z(ib + 1) - z(ib))
  Ttop = T(it - 1) + (T(it) - T(it - 1)) * (Zm(Lt) - z(it - 1)) / (z(it) - z(it - 1))
' Calculate layer average T
  
  Tsum = (T(ib + 1) + Tbot) * (z(ib + 1) - Zm(LB))
  Zsum = (z(ib + 1) - Zm(LB))
  For i = ib + 1 To it - 2
    Tsum = Tsum + (T(i) + T(i + 1)) * (z(i + 1) - z(i))
    Zsum = Zsum + (z(i + 1) - z(i))
  Next i
  Tsum = Tsum + (T(it - 1) + Ttop) * (Zm(Lt) - z(it - 1))
  Zsum = Zsum + (Zm(Lt) - z(it - 1))
  L = L + 1
  Tmls(L) = 0.5 * Tsum / Zsum '(Zm(Lt) - Zm(Lb))
  Zmls(L) = Zm(LB + 1)
  
  Do
    LB = Lt
    Lt = LB + 2  'Top inter-layer index
    ' Find RAOB top index
    If z(ib + 1) < Zm(Lt) Then 'Make sure there is a RAOB point in the layer
      it = ib
      Do
        it = it + 1
      Loop Until z(it) >= Zm(Lt)
    End If
    Tbot = T(ib) + (T(ib + 1) - T(ib)) * (Zm(LB) - z(ib)) / (z(ib + 1) - z(ib))
    Ttop = T(it - 1) + (T(it) - T(it - 1)) * (Zm(Lt) - z(it - 1)) / (z(it) - z(it - 1))
'   Calculate layer average T
    If it - ib > 2 Then
      Tsum = (T(ib + 1) + Tbot) * (z(ib + 1) - Zm(LB))
      For i = ib + 1 To it - 2
        Tsum = Tsum + (T(i) + T(i + 1)) * (z(i + 1) - z(i))
      Next i
      Tsum = Tsum + (T(it - 1) + Ttop) * (Zm(Lt) - z(it - 1))
    Else
      Tsum = (Tbot + Ttop) * (Zm(Lt) - Zm(LB))
    End If
    L = L + 1
    Tmls(L) = 0.5 * Tsum / (Zm(Lt) - Zm(LB))
    Zmls(L) = Zm(LB + 1)
    Nmls = L
    Debug.Print L; Zmls(L); Tmls(L)
    ib = it - 1    'RAOB bottom index
  Loop Until Zm(Lt + 2) > z(Nr)
  
End Sub


Function fET(ByVal W!, ByVal T) As Single
Dim x!
' Equivalent Temperature
  If T = 0 Then T = 300
  x = (cLv * W) / (cCp * T)
  If x < 50 Then
    fET = T * Exp(x)
  Else
    fET = T
  End If
End Function


Function fgnSMT(Latitude!)
' Gravity vs Latitude from Smithsonian Meteorological Tables (page 488)
Dim Cos2L!
  Cos2L = Cos(2 * Latitude * rpd)
  fgnSMT = 9.80616 * (1 - 0.0026373 * Cos2L + 0.0000059 * Cos2L ^ 2)
End Function

Function fPypsometricEqn(ByVal T1!, ByVal p1!, ByVal T2!, ByVal dZg!) As Single
' Get pressure corresponding to an increase in geopotential height, dZg, above p1
' 1 is lower level, 2 is upper level
' dZg in km, p1 in hPa, and T in K

  fPypsometricEqn = p1 * Exp(-2000# * cg * dZg / (cRd * (T1 + T2)))
End Function


Function fgS(ByVal phi!) As Single
' Somigliana's Equation for Normal Gravity on the surface of
' an ellipsoid of revolution
Dim sP!, cge!, E!, ks!

  cge = 9.7803253359
  E = 0.081819
  ks = 0.001931853
  sP = Sin(phi * rpd) ^ 2
  fgS = cge * ((1 + ks * sP) / (Sqr(1 - E ^ 2 * sP)))

End Function

Function fHypsometricEqn(ByVal Tv1!, ByVal p1!, ByVal Tv2!, ByVal p2!) As Single
' Change in geopotential height (km) between pressure levels P1 and p2 (hPa/mb)
' with corresponding virtual temperatures Tv1 and Tv2 (K)
' cRd = 287.05307            'J /kg K      Gas Constant (dry air)
' cg = 9.80665               'm /s2        Acceleration of gravity(surface)
'
  fHypsometricEqn = cRd * (Tv1 + Tv2) * Log(p1 / p2) / cg / 2000  'Delta km

End Function

Function fMachNumber(ByVal Ps!, ByVal pt!) As Single
Dim gamma!, x!
' Mach Number from static and total pressure
  gamma = cCp / cCv
  x = (2 / (gamma - 1)) * ((pt / Ps) ^ ((gamma - 1) / gamma) - 1)
  If x >= 0 Then
    fMachNumber = Sqr(x)
  Else
    fMachNumber = 0#
  End If

End Function


Function fW(RH!, P!, T!)
' Water Vapor Mixing Ratio, w (note is neither a volume or mass mixing ratio)
' p .... hPa
' T .... K
' RH ... %
Dim E!, q!

' paritial pressure of water vapor (hPa)
  E = RH! * fEs(T) / 100#
' specific humidity (gm/gm), mass mixing ratio
  q = cEa * E / (P - (1 - cEa) * E)
' Water Vapor Mixing ratio
  fW = q / (1 - q)
End Function

Function fRtoT(ByVal Ohms!) As Single
Dim a1!, a2!, a3!, a4!
' Convert platinum resistance to temperature
'fRtoT = 5 * 10 ^ (-5) * Ohms ^ 2 + 0.4527 * Ohms - 239.23
' see T_vs_R.xls for fit from -100 C to +45 C
  a1 = -239.529603289315
  a2 = 0.450385127204133
  a3 = 6.71883132179858E-05
  a4 = -1.96781369379622E-08
  fRtoT = a1 + a2 * Ohms + a3 * Ohms ^ 2 + a4 * Ohms ^ 3
End Function

Function fRphi(phi!)
' Ad Hoc Radius based on Somigliana's Formula
' Rphi = -2 g(phi)/(d/d(g(phi)))
  fRphi = 6378.137 / (1.006803 - 0.006706 * Sin(phi * rpd) ^ 2)

End Function

Function fZgToZghWGS(Zg!, Latitude!)
Dim r!
' Zg in km
' Latitude in deg
' Result in km
  r = fRwgs(Latitude)
  fZgToZghWGS = r * (fgnWGS(Latitude) / 9.80665) * (Zg / (r + Zg))
End Function

Function fRwgs(Latitude!)
' Unlike fRsmt this routine use Somigliana's normal gravity and WGS84
' R(phi)= a / (1 +f +mr -2 f sin(phi)^2)
' fRwgs in km
fRwgs = 6378.137 / (1.0068025972 - 0.0067056213 * Sin(Latitude * rpd) ^ 2)

End Function

Function fZghToZgSMT(Zgh!, Latitude!)
Dim r!
' Convert Zgh to Zg using Smithsonian Meteorological Tables
' See Eqn (13) at http://mtp.jpl.nasa.gov/notes/altitude/altitude.html
' Zgh in km
' Latitude in deg
' Result in km
  r = fRsmt(Latitude)
  fZghToZgSMT = r * Zgh / (fgnSMT(Latitude) * r / 9.80665 - Zgh)
End Function

Function fRsmt(ByVal Latitude!) As Single
' Smithsonian Meteorological Tables p.218 Equation 6
' Ginned up earth radius to compensate for centrifugal force
' variation with latitide
' Note that this is not the earth ellipsoid radius!!!
  fRsmt = -2# * fgnSMT(Latitude) / fdgdzSMT(Latitude) / 1000#

End Function

Function fdgdzSMT(Latitude!)
Dim Cos2L!, Cos4L!
' Rate of change of gravity with altitude
' From the Smithsonian Meteorological Tables p.218 Equation 7
' rpd = radians per degree = 1.74532925199433E-02
  Cos2L = Cos(2 * rpd * Latitude)
  Cos4L = Cos(4 * rpd * Latitude)
  fdgdzSMT = -(3.085462 * 10 ^ (-6) + 2.27 * 10 ^ (-9) * Cos2L - 2 * 10 ^ (-12) * Cos4L)

End Function


Function fdgdzWGS(Latitude!)
' R(phi) =  a / (1 + f + mr - 2 f sin(phi)^2
' a = 6378.1370 km
' f = 1/298.257223563
' mr= omega^2 a^2 b / GM = 0.003449787
' GM= 3986004.418 10^8 m^3/s^2
' omega = 7292115.0 10^11 rad/s
  fdgdzWGS = (-2# * fgnWGS(Latitude) / 6378137#) * (1.006802597 - 0.006705621 * Sin(Latitude * rpd) ^ 2)

End Function

Function fgnWGS(Latitude!)
Dim SinL!
' Calculate gravity at specified latitude
' Using Somigliana's Equation
' GammaS = GammaE((1 + kS sin(latitude)^2)/(sqrt(1 - e^2 sin(latitude)^2)))
' GammaE = 9.7803253359 m/s2
' kS = 0.00190117
' e = 0.081819
'
SinL = Sin(Latitude * rpd)
fgnWGS = 9.7803253359 * (1 + 0.00193185265241 * SinL ^ 2) / Sqr(1 - (0.081819190842622 * SinL) ^ 2)
End Function

Function fgn(Latitude!)
Dim SinL!
' Same as fgnWGS
' Calculate gravity at specified latitude
' Using Somigliana's Equation
' GammaS = GammaE((1 + kS sin(latitude)^2)/(sqrt(1 - e^2 sin(latitude)^2)))
' GammaE = 9.7803253359 m/s2
' kS = 0.00190117
' e = 0.081819
'
SinL = Sin(Latitude * rpd)
fgn = 9.7803253359 * (1 + 0.00193185265241 * SinL ^ 2) / Sqr(1 - (0.081819190842622 * SinL) ^ 2)
End Function

Function fCs(Tk!) As Single
Dim gamma!
' Speed of sound at temperature Tk in Kelvin
  fCs = Sqr((cCp / cCv) * cRd * Tk)
  
End Function

Function fDALR() As Single
'cg ... gravity, m/s2
'cCp .. specific heat at constant pressure J/kg K
  fDALR = 1000 * cg / cCp 'K/km

End Function
Function fDP(CASk!) As Single
' Pressure difference between pitot tube and static port for
' Indicated Air Speed IAS m/s
  fDP = cPs * ((1 + 0.2 * (CASk / cCso) ^ 2) ^ (3.5) - 1)
End Function

Function fEs(ByVal Tk!) As Single
Dim dT!, es!, i%
Static A!(0 To 6), Init As Boolean, cTo!
'Water vapor saturation mixing ratio
'Tk in Kelvin, Es in mb
'See Flatau et al., 1992, J. App. Meteorol.,31,1507-1513.

  If Not Init Then
    A(0) = 6.1117675       'Saturation vapor pressure (hPa)
    A(1) = 0.443986062
    A(2) = 0.0143053301
    A(3) = 0.000265027242
    A(4) = 0.00000302246994
    A(5) = 2.03886313E-08
    A(6) = 6.38780966E-11
    cTo = 273.15
    Init = True
  End If

  dT = Tk - cTo
  es = A(0)
  For i = 1 To 6
    es = es + A(i) * dT ^ i
  Next i

  fEs = es

End Function

Function fEsi(ByVal Tk!) As Single
Dim dT!, es!, i%
Static A!(0 To 6), Init As Boolean, cTo!
'Tk in Kelvin, Esi, saturation vapor pressure over ice in mb
'See Flatau et al., 1992, J. App. Meteorol.,31,1507-1513.
  If Not Init Then
    A(0) = 6.10952665
    A(1) = 0.501948366
    A(2) = 0.0186288989
    A(3) = 0.000403488906
    A(4) = 0.00000539797852
    A(5) = 4.20713632E-08
    A(6) = 1.47271071E-10
    cTo = 273.15
    Init = True
  End If

  es = A(0)
  dT = Tk - cTo
  For i = 1 To 6
    es = es + A(i) * dT ^ i
  Next i

  fEsi = es

End Function


Function fEv(ByVal RH!, ByVal T!) As Single
' Vapor pressure (mb) given RH (%) and T (K)
  fEv = RH * fEs(T) / 100#

End Function

Function fEvs1(ByVal Tk!) As Single
' From J.M. Richards,  J.Phys.D:Appl. Phys., 1971, Vol 4 L15-L18 and correction on p876 for first coefficient
' Evs [mb] = 1013.25 exp(EEW), where EEW= 13.3185 t -1.976 t^2 - 0.6445 t^3 - 0.1299 t^4, with t=1-373.15/T [K]
Dim Ts!, EEW!
  Ts = 1 - 373.15 / Tk
  EEW = 13.3185 * Ts - 1.976 * Ts ^ 2 - 0.6445 * Ts ^ 3 - 0.1299 * Ts ^ 4
  fEvs1 = 1013.25 * Exp(EEW)

End Function



Function fEvsMB(ByVal Tk!) As Single
Dim T!, E!
' From J.M. Richards,  J.Phys.D:Appl. Phys., 1971, Vol 4 L15-L18 and correction on p876 for first coefficient
' Evs [mb] = 1013.25 exp(EEW), where EEW= 13.3185 t -1.976 t^2 - 0.6445 t^3 - 0.1299 t^4, with t=1-373.15/T [K]
  T = 1 - 373.15 / Tk
  E = (((0.1299 * T + 0.6445) * T + 1.976) * (-T) + 13.3185) * T
  fEvsMB = 1013.25 * Exp(E)

End Function

Function fEvsGM3(TA!)

  fEvsGM3 = (100000 / cRv) * (fEvsMB(TA) / TA)

End Function

Function fH2Oppmv(ByVal TA!, ByVal PA!, ByVal RH!) As Single
' Calculate water vapor mixing ratio in ppmv
' From APCA Journal, 30 (4), April 1980, Gregory J McRae
  fH2Oppmv = 10000 * RH * fEvsMB(TA) / PA

End Function


Function fLiv(Tk!)
Dim T!
' T in Kelvin
  T = Tk - cTo
  fLiv = 2.8341 - 0.0003 * T - 0.000004 * T ^ 2

End Function

Function fLlv(Tk!)
Dim T!
' T in Kelvin
  T = Tk - cTo
  fLlv = 2.502 - 0.0024 * T
End Function

Function fMach(ByVal z!, ByVal IAS!) As Single
' Mach number given z (km) and IAS (m/s)
  fMach = Sqr(5 * ((fDP(IAS) / fZtoP(z) + 1#) ^ (2 / 7) - 1#))

End Function

Function fOATk(ByVal IATk!, ByVal Mach!, ByVal r!) As Single
  
  fOATk = IATk / (1# + 0.2 * r * Mach ^ 2)
  
End Function

Function fRH_TwTd(ByVal Tw!, ByVal Td!) As Single
Dim Ew!, Ed!, Ea!
' Tw, Td in K, RH=%
' Calculate the saturation vapor pressure (E) for both the dry-bulb (Td) and wet-bulb (Tw) temperatures using the following equations:

'  Ew = 0.61078 * Exp((17.269 * Tw) / (Tw + 237.3)) * 10    'Tw C
'  Ed = 0.61078 * Exp((17.269 * Td) / (Td + 237.3)) * 10    '=Es, Td C
   Ew = fEs(Tw)
   Ed = fEs(Td)
'  In the above equations the temperatures units ar26e Celsius and the saturation vapor pressure units are millibars.
'  Then you need to calculate that actual vapor pressure (Ea) using the following equation:

   Ea = Ew - 0.63 * (Td - Tw)

'  Relative Humidity is then calculated using the following equation:

   fRH_TwTd = (Ea / Ed) * 100

'   The units of relative humidity are in percent.

'   Here is an example of the using the equations:

'   Assume that your dry-bulb temperature (Td) = 30 C and your wet-bulb temperature (Tw) = 26 C.

'   Ew = 0.61078 * Exp((17.269 * 26) / (26 + 237.3)) * 10
'   Ew = 33.61 millibars

'   Ed = 0.61078 * Exp((17.269 * 30) / (30 + 237.3)) * 10
'   Ed = 42.44 millibars

'   Ea = 33.61 - 0.63 * (30 - 26)
'   Ea = 31.09 millibars

'   RH = (31.09 / 42.44) * 100
'   RH = 73.3 %

End Function
Function fRhoV(ByVal RH!, ByVal T!) As Single
' Vapor density (gm/m3) given RH (%) and T (K)
' Using ideal gas law
  If T = 0 Then T = 273.15
  fRhoV = 100000 * cEa * fEv(RH, T) / (cRd * T)

End Function

Function fRhoVvmr(P!, T!, VMR!)
' Calculate the vapor density at p and T assuming a volume mixing ratio VMR
' VMR in gm/kg, p in mb and T in K,

fRhoVvmr = (VMR * P * 100000) / (cRv * T) 'gm/m3

End Function


Function fRHw(P!, T!, W!)

fRHw = (100 / fEs(T)) * W * P / (W + cEa)

End Function

Function fSAT(ByVal Tt!, ByVal M!) As Single

  fSAT = Tt / (1 + ((cCp / cCv - 1) / 2) * M ^ 2)

End Function

Function fTAS(ByVal Tr!, ByVal M!) As Single
Dim gamma!

  gamma = cCp / cCv
  fTAS = M * Sqr(gamma * cRd * Tr / (1 + (gamma - 1) * M ^ 2 / 2))

End Function



Function ftest(Zgh!, Latitude!)
Dim CL
CL = Cos(2 * Latitude * 3.14159 / 180)
ftest = (1 + 0.002644 * CL) * Zgh + (1 + 0.0089 * CL) * Zgh ^ 2 / 6245

End Function

Function fTevs(ByVal evs!) As Single
Dim T1!, TN!, tnp1!
' es in mb
' Find Td
  T1 = Log(evs / 1013.25) / 13.3185

  tnp1 = T1 + ((0.1299 * T1 + 0.6445) * T1 + 1.976) * T1 ^ 2 / 13.3185

  Do
    TN = tnp1
    tnp1 = T1 + ((0.1299 * TN + 0.6445) * TN + 1.976) * TN ^ 2 / 13.3185
  Loop Until tnp1 - TN < 0.00000000001

  fTevs = 373.15 / (1 - tnp1)

End Function

Function FTD(W!, P!)
'B = 5420       'K
'A = 253000000# 'kPa


End Function

Function ABS_H2O(ByVal T!, ByVal P!, ByVal VD!, ByVal f!) As Single
'
' PURPOSE- COMPUTE ABSORPTION COEF IN ATMOSPHERE DUE TO WATER VAPOR
'
' CALLING SEQUENCE PARAMETERS-
' SPECIFICATIONS
' NAME    UNITS     I/O  DESCRIPTON              VALID RANGE
' T       KELVIN     I   TEMPERATURE
' P       MILLIBAR   I   PRESSURE                .1 TO 1000
' VD      G/M**3     I   WATER VAPOR DENSITY
' F       GHZ        I   FREQUENCY                0 TO 800
' ABS_H2O NEPERS/KM  O   ABSORPTION COEFFICIENT
'
' REFERENCES-
' P.W. ROSENKRANZ, RADIO SCIENCE V.33, PP.919-928 (1998).
'
' LINE INTENSITIES SELECTION THRESHOLD = HALF OF CONTINUUM ABSORPTION AT 1000 MB.
' WIDTHS MEASURED AT 22,183,380 GHZ, OTHERS CALCULATED.
' A.BAUER ET AL. ASA WORKSHOP (SEPT. 1989) (380GHz).
'
' REVISION HISTORY-
' DATE- OCT.6, 1988  P.W.ROSENKRANZ - EQS AS PUBL. IN 1993.
'       OCT.4, 1995  PWR- USE CLOUGH'S DEFINITION OF LOCAL LINE
'                    CONTRIBUTION,  HITRAN INTENSITIES, ADD 7 LINES.
'       OCT. 24, 95  PWR -ADD 1 LINE.
'       JULY 7, 97   PWR -SEPARATE COEFF. FOR SELF-BROADENING,
'                    REVISED CONTINUUM.
'       DEC. 11, 98  PWR - ADDED COMMENTS
'
' LOCAL VARIABLES:
Dim NLINES%, i%, j%
NLINES = 15

Dim df!(1 To 2)
Static S1!(1 To 15), b2!(1 To 15), W3!(1 To 15), FL!(1 To 15), x!(1 To 15), WS(1 To 15), XS(1 To 15)
Static Init
Dim Pvap!, Pda!, den!, Ti!, TI2!, Sum!, Width!, WSQ!, s!, BASE!, RES!, CON!
' LINE FREQUENCIES:
If Init = 0 Then
  FL(1) = 22.2351: FL(2) = 183.3101: FL(3) = 321.2256: FL(4) = 325.1529: FL(5) = 380.1974
  FL(6) = 439.1508: FL(7) = 443.0183: FL(8) = 448.0011: FL(9) = 470.889: FL(10) = 474.6891
  FL(11) = 488.4911: FL(12) = 556.936: FL(13) = 620.7008: FL(14) = 752.0332: FL(15) = 916.1712
' LINE INTENSITIES AT 300K:
  S1(1) = 1.31E-14: S1(2) = 0.000000000002273: S1(3) = 8.036E-14: S1(4) = 0.000000000002694: S1(5) = 0.00000000002438
  S1(6) = 0.000000000002179: S1(7) = 4.624E-13: S1(8) = 0.00000000002562: S1(9) = 8.369E-13: S1(10) = 0.000000000003263
  S1(11) = 6.659E-13: S1(12) = 0.000000001531: S1(13) = 0.00000000001707: S1(14) = 0.000000001011: S1(15) = 0.00000000004227
' T COEFF. OF INTENSITIES:
  b2(1) = 2.144: b2(2) = 0.668: b2(3) = 6.179: b2(4) = 1.541: b2(5) = 1.048
  b2(6) = 3.595: b2(7) = 5.048: b2(8) = 1.405: b2(9) = 3.597: b2(10) = 2.379
  b2(11) = 2.852: b2(12) = 0.159: b2(13) = 2.391: b2(14) = 0.396: b2(15) = 1.441
' AIR-BROADENED WIDTH PARAMETERS AT 300K:
  W3(1) = 0.00281: W3(2) = 0.00281: W3(3) = 0.0023: W3(4) = 0.00278: W3(5) = 0.00287
  W3(6) = 0.0021: W3(7) = 0.00186: W3(8) = 0.00263: W3(9) = 0.00215: W3(10) = 0.00236
  W3(11) = 0.0026: W3(12) = 0.00321: W3(13) = 0.00244: W3(14) = 0.00306: W3(15) = 0.00267
' T-EXPONENT OF AIR-BROADENING:
  x(1) = 0.69: x(2) = 0.64: x(3) = 0.67: x(4) = 0.68: x(5) = 0.54
  x(6) = 0.63: x(7) = 0.6: x(8) = 0.66: x(9) = 0.66: x(10) = 0.65
  x(11) = 0.69: x(12) = 0.69: x(13) = 0.71: x(14) = 0.68: x(15) = 0.7
' SELF-BROADENED WIDTH PARAMETERS AT 300K:
  WS(1) = 0.01349: WS(2) = 0.01491: WS(3) = 0.0108: WS(4) = 0.0135: WS(5) = 0.01541
  WS(6) = 0.009: WS(7) = 0.00788: WS(8) = 0.01275: WS(9) = 0.00983: WS(10) = 0.01095
  WS(11) = 0.01313: WS(12) = 0.0132: WS(13) = 0.0114: WS(14) = 0.01253: WS(15) = 0.01275
' T-EXPONENT OF SELF-BROADENING:
  XS(1) = 0.61: XS(2) = 0.85: XS(3) = 0.54: XS(4) = 0.74: XS(5) = 0.89
  XS(6) = 0.52: XS(7) = 0.5: XS(8) = 0.67: XS(9) = 0.65: XS(10) = 0.64
  XS(11) = 0.72: XS(12) = 1#: XS(13) = 0.68: XS(14) = 0.84: XS(15) = 0.78
  Init = 1
End If
  
  If VD <= 0# Then ABS_H2O = 0#: Exit Function
  Pvap = VD * T / 217#
  Pda = P - Pvap
  den = 3.335E+16 * VD
  Ti = 300# / T
  TI2 = Ti ^ 2.5

' CONTINUUM TERMS
  CON = (0.000000000543 * Pda * Ti ^ 3 + 0.000000018 * Pvap * Ti ^ 7.5) * Pvap * f * f

' ADD RESONANCES
  Sum = 0#
  For i = 1 To NLINES
    Width = W3(i) * Pda * Ti ^ x(i) + WS(i) * Pvap * Ti ^ XS(i)
    WSQ = Width * Width
    s = S1(i) * TI2 * Exp(b2(i) * (1# - Ti))
    df(1) = f - FL(i)
    df(2) = f + FL(i)
' USE CLOUGH'S DEFINITION OF LOCAL LINE CONTRIBUTION
    BASE = Width / (562500# + WSQ)
' DO FOR POSITIVE AND NEGATIVE RESONANCES
    RES = 0#
    For j = 1 To 2
      If Abs(df(j)) < 750# Then RES = RES + Width / (df(j) ^ 2 + WSQ) - BASE
    Next j
    Sum = Sum + s * RES * (f / FL(i)) ^ 2
  Next i
 
  ABS_H2O = 0.00003183 * den * Sum + CON
      
End Function

Function ABS_N2(ByVal T!, ByVal P!, ByVal f!)
'ABSN2 = ABSORPTION COEFFICIENT DUE TO NITROGEN IN AIR (NEPER/KM)
'    T = TEMPERATURE (K)
'    P = PRESSURE (MB)
'    F = FREQUENCY (GHZ)
'
Dim Th!
  Th = 300# / T
  ABS_N2 = 0.000000000000064 * P * P * f * f * Th ^ 3.55
End Function

Function ABS_LIQ(ByVal T!, ByVal VD!, ByVal f!) As Single
' COMPUTES ABSORPTION IN NEPERS/KM BY SUSPENDED WATER DROPLETS
' ARGUMENTS (INPUT):
' VD IN G/M**3
' F IN GHZ     (VALID FROM 0 TO 1000 GHZ)
' T IN KELVIN
'
' REFERENCES:
' LIEBE, HUFFORD AND MANABE, INT. J. IR & MM WAVES V.12, pp.659-675
' (1991);  Liebe et al, AGARD Conf. Proc. 542, May 1993.
'
' REVISION HISTORY:
' PWR 8/3/92   original version
' PWR 12/14/98 temp. dependence of EPS2 eliminated to agree with MPM93
'
' COMPLEX EPS,RE
  Dim EPS0!, EPS1!, EPS2!, c1!, c2!, D1!, D2!, FP!, FS!, Theta1!, A!, b!
  
  If (VD <= 0#) Then ABS_LIQ = 0#: Exit Function
  Theta1 = 1# - 300# / T
  EPS0 = 77.66 - 103.3 * Theta1
  EPS1 = 0.0671 * EPS0
  EPS2 = 3.52
  FP = (316# * Theta1 + 146.4) * Theta1 + 20.2
  FS = 39.8 * FP
  c1 = EPS0 - EPS1
  c2 = EPS1 - EPS2
  D1 = 1 + (f / FP) ^ 2
  D2 = 1 + (f / FS) ^ 2
  A = c1 / D1 + c2 / D2 + EPS2
  b = (c1 / D1) * (f / FP) + (c2 / D2) * (f / FS)
  
    
'  EPS = (EPS0 - EPS1) / CMPLX(1#, f / FP) + (EPS1 - EPS2) / CMPLX(1#, f / FS) + EPS2
'  Re = (EPS - 1#) / (EPS + 2#)
'  ABS_LIQ = -0.06286 * AIMAG(RE) * f * VD
' AIMAG = imaginary part of Complex number
  ABS_LIQ = 0.06286 * f * VD * 3# * b / ((A + 2) ^ 2 + b ^ 2)
End Function



Function ABS_TOT(ByVal T!, ByVal P!, ByVal RhoV!, ByVal RhoL!, ByVal f!, ByRef flag() As Boolean) As Single

ABS_TOT = 0#
If flag(1) Then ABS_TOT = ABS_O2(T!, P!, RhoV!, f!)
If flag(2) Then ABS_TOT = ABS_TOT + ABS_N2(T!, P!, f!)
If flag(3) Then ABS_TOT = ABS_TOT + ABS_H2O(T!, P!, RhoV!, f!)
If flag(4) Then ABS_TOT = ABS_TOT + ABS_LIQ(T!, RhoL!, f!)

End Function


Function fDstd(z!) As Single
' Calculate density of 1976 Standard Atmosphere
' Input  z ......... km
' Output Density ... kg/m3
  fDstd = 100 * fZtoP(z) / (fTstd(z) * cRs)

End Function

Function fFusb(ByVal T!, ByVal P!, ByVal VD!, ByVal f!) As Single
Dim Tau1!, Tau2!, df!, Fusb!, i%, dFscale!
' Find USB F which has same absorption as LSB F
  
  Tau1 = ABS_O2(T, P, VD, f)
  Fusb = 61 + (61 - f)
  df = 0.001
  i = 0
  If f > 54 Then dFscale = 1 Else dFscale = 10  'Make bigger f steps in low opacity

  Do
    i = i + 1
    Tau2 = ABS_O2(T, P, VD, Fusb)
    Select Case Abs(Tau2 - Tau1)
    Case Is < 0.001: df = 0.0001 * dFscale
    Case Is < 0.01: df = 0.001 * dFscale
    Case Is < 0.1: df = 0.01 * dFscale
    Case Is < 1: df = 0.1 * dFscale
    Case Else: df = 1 * dFscale
    End Select
  
    If Tau2 < Tau1 Then Fusb = Fusb - df Else Fusb = Fusb + df

    DoEvents
  Loop Until Abs(Tau1 - Tau2) < 0.0001
  
  fFusb = Fusb

End Function

Function fGeoidRadius(LatDeg!)
' Calculate radius of WGS-84 Geoid in km
Dim rLat!

  If LatDeg = 90 Then
    fGeoidRadius = cRep / 1000# 'km
  Else
    rLat = LatDeg * rpd
    fGeoidRadius = (cRee * cRep * Sqr((1 + Tan(rLat) ^ 2) / (cRep ^ 2 + cRee ^ 2 * Tan(rLat) ^ 2))) / 1000#
  End If

End Function

Function fGeoPot(Z1!, Z2!, Lat!, EPS!)
' Calculate geopotential height form z1 to z2 at latitude (Lat) to precision EPS
' Integral from 0 to Z of g(z) dz / 9.8
' MJ Mahoney 19990517

Dim SinL2!, gn!, jmax%, j%, N%, it%, i%
Dim s!, os!, ost!, st!, DeltaZ!, Sum!, TNM!, z!

' First calculate surface gravity at Lat
'  SinL2 = Sin(Lat * RpD) ^ 2
'  gn = 9.7803185 * (1 + 0.005278895 * SinL2 - 0.000023462 * SinL2 ^ 2)
  gn = fgnSMT(Lat)

' Now implement QSIMP from Numerical Recipes
  jmax = 20
  ost = -1E+30
  os = -1E+30
  For j = 1 To jmax
    GoSub trapzd
    s = (4# * st - ost) / 3#
    If (Abs(s - os) < EPS * Abs(os)) Then fGeoPot = s / 9.8: Exit Function
    If (s = 0# And os = 0# And j > 6) Then fGeoPot = s / 9.8: Exit Function
    os = s
    ost = st
  Next j
  Call MsgBox("Too many steps in qsimp", vbOKOnly)
  Exit Function

trapzd:
  If j = 1 Then
'   s = 0.5 * (Z2 - Z1) * (2# * gn - 0.003086 * (Z1 + Z2))
    st = (Z2 - Z1) * gn - 0.001543 * (Z2 * Z2 - Z1 * Z1)
  Else
    it = 2 ^ (j - 2)
    TNM = it
    DeltaZ = (Z2 - Z1) / TNM
    z = Z1 + 0.5 * DeltaZ
    Sum = 0#
    For i = 1 To it
      Sum = Sum + gn - 0.003086 * z
      z = z + DeltaZ
    Next i
    st = 0.5 * (st + (Z2 - Z1) * Sum / TNM)
  End If
Return

End Function

Function fVToR(Volts!, Src$)
'DC8 Platinum Resistor Calibration
Select Case Src$
Case "Ttgt1": fVToR = 77.301 * Volts + 289.93
Case "Ttgt2": fVToR = 77.453 * Volts + 288.66
Case "Twin": fVToR = 77.434 * Volts + 291.57
End Select
End Function

Function fZghToZgSMT98(Zgh!, Latitude!)
Dim r!
' Same as SMT with g=9.8
  r = fRsmt(Latitude)
  fZghToZgSMT98 = r * Zgh / (fgnSMT(Latitude) * r / 9.8 - Zgh)

End Function

Function fZgZpDiffRAOB(P!(), Zh!(), Nlevr%, Latitude!, Zpr!, FirstPass As Boolean)
' Calculate Zg-Zp at a specified pressure altitude Zpr give RAOB P(), Zh() and Latitude
' Enter with:
' P() .... RAOB pressure profile
' Zh() ... RAOB Geopotential Height (km)
' Nlevr .. Number of RAOB levels
' Zp   ... Pressure Altitude (km) at which Zg-Zp is needed
' Latitude ... Latitude at which difference is required
' FirstPass .. if TRUE calculate Zp() profile, if FALSE use static Zp() profile
' Return with Zg-Zp at Zp
Dim i%, Zg1!, Zg2!, Zp1!, Zp2!
Static Zp!()

  If FirstPass Then  'Convert RAOB pressure profile to Zp() profile
    ReDim Zp(1 To Nlevr)
    For i = 1 To Nlevr
      Zp(i) = fPtoZ(P(i))
    Next i
  End If
  
' Find first Zp() level above Zp
  i = 0
  Do
  Loop Until Zp(i) > Zpr    'Zpr is pressure altitude at which Zg in needed
   
  Zp1 = Zp(i - 1)                           'Lower Zp
  Zp2 = Zp(i)                               'Upper Zp
' Geopotential height Zh() and pressure altitude Zp() levels have same index
  Zg1 = fZghToZgSMT(Zh(i - 1), Latitude)    'Lower Zh level converted to Zg
  Zg2 = fZghToZgSMT(Zh(i), Latitude)        'Upper Zh level converted to Zg
  fZgZpDiffRAOB = Zg1 + (Zpr - Zp1) * (Zg2 - Zg1) / (Zp2 - Zp1)

End Function

Function fZhToZg(Zgh!, Latitude!)
Dim r!
' This conversion differs from fZhToZgSMT in two ways:
' Somigliana's Normal Gravity is used instead of the SMT value (differ by parts in 10^6)
' 9.80665 is used for gravity at 45 degress, rather than 9.8

r = fRwgs(Latitude)
fZhToZg = r * Zgh / (fgS(Latitude) * r / 9.80665 - Zgh)

End Function


Function fZgToZgh(Zg!, Latitude!)
' Convert geometric altitude to geopotential height
' Zg ........ geometric altitude (km)
' Latitude .. degrees
' Return .... geopotential height
Dim r!
  r = fRwgs(Latitude)
  fZgToZgh = (fgnWGS(Latitude) * r / 9.80665) * (Zg / (Zg + r))
End Function


Function fIntHE(ByVal g0!, ByVal T0!, ByVal LR0!, ByVal P0!, ByVal P!) As Single
' Integrate Hydrostatic Equation from a point Z0, T0, P0 to a point P
' Assuming a fixed lapse rate LR0 from the P0 to P
' g0 .... accelertion of gravity at altitude and latitude of integration
' PO  ... initial pressure in mb
' T0  ... virtual temperature at initial point
' LRO ... constant lapse rate from PO to P
' P   ... end point of integration
' Function returns DeltaZ in km from P0 to P
' Written: MJM 19990507
' cRd     the gas constant for dry air = 287.05307  J /kg K
' cg      the acceleration of gravity  = 9.80665    m /s2

If LR0 = 0 Then
' fIntHE = - (cRd * T0 / (cg * 1000#)) * Log(P / P0)
' fIntHE = - 2.92712669464088E-02 * T0 * Log(P / P0)   'fixed g
  fIntHE = -0.28705307 * T0 * Log(P / P0) / g0         'allow g to vary

Else
' fIntHE = + (T0 / LR0) * (1# - (P / P0) ^ (-cRd * LR0 / (cg * 1000#)))
' fIntHE = - (T0 / LR0) * (1# - (P / P0) ^ (-2.92712669464088E-02 * LR0))
  fIntHE = -(T0 / LR0) * (1# - (P / P0) ^ (-0.28705307 * LR0 / g0))

End If

End Function

Function fNDmj(ByVal RH!, ByVal P!, ByVal T!) As Single
Dim Tv!
' RH %, P [mb], T [K]
  Tv = fTv(RH, P, T)
  fNDmj = 100 * P * cAo / (cRd * Tv * cMd)

End Function

Function fPressure(ByVal pt!, ByVal T!) As Single
' Find pressure given PT and T
  fPressure = 1000# * (T / pt) ^ (3.4965035)
End Function

Function fStandardDensity(ByVal Imonth%, ByVal pALT!, ByVal Latitude!) As Single
Dim lu%, Filename$, iLAT%, Line1$, Line2$, iZ%
Dim Rho1!, Rho2!, x1!, x2!, f!, i%, j%
Static NDPmonth%, Rho!(0 To 25, 0 To 13)

' Only read file if month has changed
  If NDPmonth <> Imonth Then
    NDPmonth = Imonth
    Filename$ = "C:\MTP\Setup\DENSITY\Std_Atm_Den." + Format(Imonth, "000")
    lu = FreeFile
    Open Filename$ For Input As lu
    'Skip two header lines
    Input #lu, Line1$
    Input #lu, Line1$
  
' Reference Atmosphere Density (kg/m**3)   Month=           4
'       -90.    -75.    -60.    -45.    -30.    -15.      0.     15.     30.     45.     60.     75.     90.
' 0. 1.39810 1.33850 1.27950 1.24740 1.20630 1.18160 1.17160 1.18570 1.21130 1.26780 1.31160 1.38840 1.42970
'         1         2         3         4         5         6         7         8         9         1      7
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
'   1234567812345678
' 2. 1.06170 1.04250 1.02400 1.01220 0.98598 0.96775 0.96565 0.97223 0.98903 1.01170 1.03750 1.06350 1.07450
' 4. 0.84294 0.83111 0.81419 0.81810 0.80015 0.79044 0.78992 0.79159 0.80172 0.81340 0.81648 0.84225 0.84831
' 6. 0.66221 0.66134 0.65407 0.65738 0.64897 0.64133 0.64096 0.64388 0.64947 0.65615 0.65709 0.66274 0.66313
' 8. 0.51425 0.51955 0.51931 0.52270 0.52471 0.51764 0.51728 0.52202 0.52446 0.52336 0.52213 0.51557 0.51285
'10. 0.37877 0.38648 0.40696 0.41081 0.41912 0.41790 0.41737 0.41823 0.41832 0.41224 0.39711 0.37924 0.37669
' ...
'50 ....last line
    For i = 0 To 25
      For j = 0 To 13
        Input #lu, Rho(i, j)
      Next j
    Next i
    Close lu
  End If

  For i = 1 To 13
    If -90 + (i - 1) * 15 >= Latitude Then Exit For
  Next i
  iLAT = i - 1         'Lower latitude
' Bracket altitude [km]
  iZ = Int(Abs(pALT) / 2)  'Lower altitude
' Interpolate in Latitude
  f = 1 + (Latitude + 90) / 15 - iLAT
  Rho1 = (1 - f) * Rho(iZ, iLAT) + f * Rho(iZ, iLAT + 1)
  Rho2 = (1 - f) * Rho(iZ + 1, iLAT) + f * Rho(iZ + 1, iLAT + 1)
  'Interpolate in altitude
  f = (pALT / 2 - iZ)
  fStandardDensity = (1# - f) * Rho1 + f * Rho2
  
End Function

Function fRhoVsjk(RH!, Tk!)
Dim Tc!

Tc = Tk - cTo      'Temperature in Celcius
fRhoVsjk = 2.17 * (RH / Tk) * 10 ^ ((26# + Tc) / 33# + ((Tc - 7#) / 90#) ^ 2) ' Vapor density [g/m3]

End Function

Function fTB(El!, T!(), Tau!(), Levels%)
' This routine allows TB calculations for different Elevation angles and fixed Tau(i,j) array
Dim j%, jm1%, Sum#, SinEL!
  
  SinEL = Sin(El * Pi / 180#)
  
  Sum = 0#
  For j = 2 To Levels
    Sum = Sum + (T(jm1) + T(j)) * (Exp(-Tau(jm1) / SinEL) - Exp(-Tau(j) / SinEL))
  Next j
  
  If El >= 0 Then
    fTB = 0.5 * Sum + 2.75 * Exp(-Tau(Levels) / SinEL) 'Add CMB
  Else
    fTB = 0#
  End If
End Function

Function fTBraob(f!, E!, zo!, z!(), T!(), RhoV!(), RhoL!(), BI!, Nlev%, EnableAbs() As Boolean)
Dim dP!, Pstep!, Zmax!, RsinE!, P!, r!, DeltaTB!, Levels%
Dim i%, j%, jm1%, jm%, Done As Boolean, DeltaZ!, SumZ!, SumP!, RhoVj!, RhoVjm1!
Dim Tjm1!, Tj!, Zjm1!, Zj!, PAjm1!, PAj!, Kjm1!, Kj!, Taujm1!, Tauj!, RhoLj!, RhoLjm1!
Dim Sum!

' Note that z is geometric altitude, but that pressure steps use pressure altitude
' This needs to be fixed!!!!

  If E = 0 Then fTBraob = T(1): Exit Function
  
  RsinE = 1 / Sin(E * 3.14159 / 180)
' Create equal steps in Ln(P) to 50 km
  Levels = 200
  
  If zo = z(1) Then
    Zjm1 = z(1)
    Tjm1 = T(1)
    P = fZtoP(z(1))
    RhoVjm1 = RhoV(1)
    RhoLjm1 = RhoL(1)
  Else
    Zjm1 = zo
    Tjm1 = fLinterp(z(), T(), Nlev, zo)
    P = fZtoP(zo)
    RhoVjm1 = fLinterp(z(), RhoV(), Nlev, zo)
    RhoLjm1 = fLinterp(z(), RhoL(), Nlev, zo)
  End If
  Kjm1 = ABS_TOT(Tjm1, P, RhoVjm1, RhoLjm1, f, EnableAbs())
  If Kjm1 = 0 Then Zmax = 30 Else Zmax = 20 / Kjm1 / RsinE 'Integrate to 5 optical depths
  dP = P - fZtoP(Zjm1 + Zmax)
  Pstep = dP / Levels        'Pressure step in zenith direction

  Sum = 0#
  Taujm1 = 0#
  For j = 2 To Levels
    P = P - Pstep
    Zj = fPtoZ(P)
    Tj = fLinterp(z(), T(), Nlev, Zj)
    RhoVj = fLinterp(z(), RhoV(), Nlev, Zj)
    RhoLj = fLinterp(z(), RhoL(), Nlev, Zj)
    Kj = ABS_TOT(Tj, P, RhoVj, RhoLj, f, EnableAbs())
    Tauj = Taujm1 + 0.5 * (Kj + Kjm1) * (Zj - Zjm1) * RsinE
    Sum = Sum + (Tjm1 + Tj) * (Exp(-Taujm1) - Exp(-Tauj))
    Tjm1 = Tj: Zjm1 = Zj: Kjm1 = Kj: Taujm1 = Tauj
  Next j
  fTBraob = 0.5 * Sum + 2.75 * Exp(-Tauj) 'Add CMB
End Function

Function fTdTaToRH(TA!, Td!, Celsius As Boolean)
'Ta is ambient temperature [C]
'Td is dew point temperature [C]
'RH is in percent
Dim es!, E!

If Td = 0 Then fTdTaToRH = 0#: Exit Function
If Celsius Then
  es = 6.11 * (10# ^ (7.5 * TA / (237.7 + TA)))
  E = 6.11 * (10# ^ (7.5 * Td / (237.7 + Td)))
Else 'assume Kelvin
  es = 6.11 * (10# ^ (7.5 * (TA - cTo) / (TA - 35.45)))
  E = 6.11 * (10# ^ (7.5 * (Td - cTo) / (Td - 35.45)))
End If
fTdTaToRH = (E / es) * 100#
End Function

Function fTvirtual50!(T!)
   If T < 170 Or T > 320 Then
     fTvirtual50 = 300
   Else
     fTvirtual50 = T + 10 ^ (0.0365 * (T - 283)) 'BLG fit at RH=50%
   End If
End Function

Function fNumberDensity(ByVal P!, ByVal T!) As Single
Dim M!
' P [mb], T [K]
  If T < 170 Or T > 330 Then T = 300
  M = 28.97 * (1 - 10 ^ (0.034 * (T - 355)))  'g at 10 km
  fNumberDensity = 348 * (P / T) * 6.023E+23 / M
  
End Function

Function fND(ByVal P!, ByVal T!) As Single
Dim M!
' Number density from pressure and temperature
  M = 28.97 * (1 - 10 ^ (0.034 * (T - 355)))  'g at 10 km
  fND = 348 * (P / T) * 6.023E+23 / M
End Function

Function fRhoLabs(ByVal LWC!, ByVal T!, ByVal Nu!) As Single
' Calculate liquid water absorption in Np/km
' LWC [g/m3], T [K], Nu [GHz]
  fRhoLabs = 0.075 * LWC * (280 / T) ^ 6.87 * (Nu / 22.2) ^ 1.9

End Function

Function Calc_TB(Elevation!, Zoff!, Tsurface!, LapseRate!, StandardT As Boolean, Ztrop!, RhoVsurface!, CM%, Zbase!, Ztops!, Nu!, EPS!)
Dim OldS!, j%, jmax%, s!, k%, AlphaO2!, AlphaLH2O!
Dim TNM%, Sum!, Wsum!, r!, DEL!, r1!, r2!, Wr!, LWC!
Dim Tr!, Pr!, RhoVr!, z!, Absorption!, dR!, it%, rstep%

' First calculate absorption at zero elevation
  Tr = fTatZ(Zoff, Tsurface, LapseRate, Ztrop, StandardT)
  Pr = fZtoP(Zoff)
  RhoVr = RhoVsurface * Exp(-Zoff / 2#)
  Absorption = O2ABS(Tr, Pr, RhoVr, Nu)

  r1 = 0: r2 = 5 / Absorption 'Range = 5 optical depths
  dR = r2 - r1
  jmax = 20
  OldS = 1E-30
  it = 1

  LWC = 0
  AlphaO2 = O2ABS(Tr, Pr, RhoVr, Nu)
  AlphaLH2O = fRhoLabs(LWC, Tr, Nu)
  s = Tr * WtFunction(r1, AlphaO2, AlphaLH2O) * dR

For j = 1 To jmax
' This is Trapezoidal integration routine
    TNM = it
    rstep = dR / TNM
    r = r1 + rstep / 2
    Wsum = 0#
    Sum = 0#
    For k = 1 To it
' Calculate applicable altitude, Z
      z = Zoff + r * Sin(Elevation * mPI / 180)
' Calculate T, P, RhoV, and LWC at Z
      Tr = fTatZ(z, Tsurface, LapseRate, Ztrop, StandardT)
      Pr = fZtoP(z)
      RhoVr = RhoVsurface * Exp(-z / 2#)
' Determine Cloud Model to use
      Select Case CM
      Case 0
        LWC = 0
      Case 1
        If z > Zbase And z < Ztops Then
          LWC = 0.5 * (RhoVsurface * Exp(-Zbase / 2#) - RhoVsurface * Exp(-z / 2#)) 'g/m3
        Else
          LWC = 0
        End If
      End Select
' Calculate O2 and LH2O absorption at Z, and weighting function
      AlphaO2 = O2ABS(Tr, Pr, RhoVr, Nu)
      AlphaLH2O = fRhoLabs(LWC, Tr, Nu)
      Wr = WtFunction(r, AlphaO2, AlphaLH2O)
' Then do integral over range
      Wsum = Wsum + Wr * rstep
      Sum = Sum + Tr * Wr * rstep
      r = r + rstep
    Next
    
    s = (s + Sum) / 2
    it = 2 * it
' End of integration
  If Abs(s - OldS) < EPS * Abs(OldS) Then Exit For
  OldS = s
 ' Debug.Print j; S / Wsum
Next

If j = jmax Then Stop
Calc_TB = s / Wsum

End Function


Sub CalculateZarray(Latitude!, zo!, n1%, n2%, Pz!(), TZ!(), RHZ!(), Mode%, Zx!())
Dim z!, i%, g0!, T0!, P0!, P!, LR0!, Tv!, RH0!, T!, x!, p1!, T1!, RH1!
' Calculate Geopotential Height for arrays of P,T, and RH
' Inputs: Latitude
'         Geometric altitude for initial point (Geopotential if mode=2)
'         Pressure array
'         Dimension of P,T,RH and Z arrays
'         Temperature array
'         Relative Humidity array
'         Mode = 0  no g variation with height (use g at lat and Zo), no useful use!
'                1  geometric height (g varies with height)
'                2  geopotential height (g = 9.8 at all altitudes)
' Output: Z array corresponding to mode
  z = zo
  Select Case Mode   ' Set g for case where it doesn't vary
  Case 0: g0 = fg(zo, Latitude)
  Case 2: g0 = 9.8
  End Select

  For i = n1 To n2 - 1
    T0 = TZ(i)
    P0 = Pz(i)
    LR0 = (TZ(i + 1) - TZ(i)) / (fPtoZ(Pz(i + 1)) - fPtoZ(Pz(i)))
    RH0 = (RHZ(i) + RHZ(i + 1)) / 2 'averaging doesn't make much difference
    Tv = fTv(RH0, P0, T0) 'Average virtual temperature
    If Mode = 1 Then g0 = fg(z, Latitude)
    P = Pz(i + 1)
    z = z + fIntHE!(g0, Tv, LR0, P0, P)
    Zx(i + 1) = z
  Next i

End Sub
Function fTvRH(P!, T!, RH!)
' Calculate virtual temperature from p, T, and RH
' Need function fEs to calculate water vapor saturation mixing ratio
' cEa = 0.622004944 = cMw/cMd
' cMd = 28.9644 kg/kmole  -   Avg Molecular Weight (dry air)
' cMw = 18.0160 kg/kmole  -   Molecular Weight (water vapor)

  fTvRH = T / (1# - (RH / 100#) * (fEs(T) / P) * (1# - cEa))

End Function

Function fZgmToZgp!(Zgm!, gn!)
' Calculate geopotential height corresponding to geometric height Zgm
' with normal (surface at Lat) gravity of gn
' Note: Specify gn instead of Latitude to avoid repetitive calculation
  fZgmToZgp = (gn * Zgm - 0.001543 * Zgm * Zgm) / 9.8

End Function


Function fZghToZgWGS(Zgh!, Latitude!)
Dim r!
' Zgh in km
' Latitude in deg
' Result in km
r = fRwgs(Latitude)
fZghToZgWGS = r * Zgh / (fgnWGS(Latitude) * r / 9.80665 - Zgh)

End Function
Function fZgpToZgm!(Zgp!, gn!)
Dim x!
' Calculate geometric height corresponding to geopotential height Zgp
' with normal (surface at Lat) gravity of gn
' Note: Specify gn instead of Latitude to avoid repetitive calculation
 
' fZgpToZgm = (gn - Sqr(gn * gn - 2 * 0.003086 * 9.8 * Zgp)) / 0.003086
  fZgpToZgm = 324.044069993519 * (gn - Sqr(gn * gn - 0.0604856 * Zgp))

End Function


Function fZhtoZg24(Zgh!, Latitude!)
Dim cos2phi#
' Taylor Series expansion of conversion between H and Zg
' See equation (24) on web page:
' http://mtp.jpl.nasa.gov/notes/altitude/altitude.html
' Use exact equation given by: fZhToZg(Zgh, Latitude)

cos2phi = Cos(2# * Latitude * rpd)
fZhtoZg24 = (1# + 0.002644 * cos2phi) * Zgh + (1# + 0.0089 * cos2phi) * Zgh ^ 2 / 6245#

End Function

Function fZhtoZgSMT98(Zgh!, Latitude!)
Dim r#
' Convert Geopotential Height (Zgh) to Geometric Height (Zg)
' Verified against Smithsonian Meteorological Tables (page 222-223)

r = fRsmt(Latitude)                'Ad hoc radius
fZhtoZgSMT98 = r * Zgh / (fgnSMT(Latitude) * r / 9.8 - Zgh)

End Function

Function fZhtoZgSMT(Zgh!, Latitude!)
Dim r#
' Convert Geopotential Height (Zgh) to Geometric Height (Zg)
' Zgh ......... Geopotential height (km)
' Latitude .... degrees
' Return ...... Geometric altitude (km)
'
' Verified against Smithsonian Meteorological Tables (page 222-223)
' if 9.80665 set to 9.8 as in tables
  r = fRsmt(Latitude)                'Ad hoc radius
  fZhtoZgSMT = r * Zgh / (fgnSMT(Latitude) * r / 9.80665 - Zgh)
End Function

Function fZhtoZg14(Zgh!, Latitude!)
Dim cos2phi#
' Taylor Series expansion of conversion between H and Zg
' See equation (12) on web page:
' http://mtp.jpl.nasa.gov/notes/altitude/altitude.html
' Use exact equation given by: fZhToZg(Zgh, Latitude)

cos2phi = Cos(2# * Latitude * rpd)
fZhtoZg14 = (1# + 0.0026373 * cos2phi) * Zgh + (1# + 0.0086476 * cos2phi) * Zgh ^ 2 / 6356.6818

End Function

Function fZgToZghSMT(Zg!, Latitude!)
Dim r!
' Convert geometric altitude to geopotential height
' Zg ........ Geometric altitude (km)
' Latitude .. Degrees
' Return .... Geopotential height
'
' Verified against Smithsonian Meteorological Tables (page 220-221)
' if 9.80665 set to 9.8 as in tables
  r = fRsmt(Latitude)
  fZgToZghSMT = r * (fgnSMT(Latitude) / 9.80665) * (Zg / (Zg + r))
End Function

Sub IntegrateHE(Nlev%, z!(), T!(), dZg!(), NoD!())
Dim L%, dZgi!, z0!, T0!, P0!, Tv0!
Dim Plast!, Tlast!, ZL!, TL!, PL!, TvL!, TvLast!
' Checked against GOSUB version 19990404 mjm with same results
' z() ...... contains pALT [km]
' T() ...... contains Temperatures [K] at pALT
' dZg() .... contains geometric altitude offsets wrt flight level
' NoD() .... contains Number Density profile
' Nret ..... is number of levels with z>0.1
' L=1 is lowest level, L=16 is a/c level, and L=33 is highest level
' Exit when z<0.1, so Nlev may be <33
'
' First integrate up
  L = 16: dZgi = 0: z0! = z(L): T0! = T(L): dZg(L) = 0
  P0 = fZtoP(z0): Tv0 = fTvirtual50(T0)
  NoD(L) = fNumberDensity(P0, T0)
  Plast = P0: TvLast = Tv0
  Do   'Integrate above a/c
    L = L + 1
    ZL = z(L): TL = T(L)
    PL = fZtoP(ZL): TvL = fTvirtual50(TL)
    NoD(L) = fNumberDensity(PL, TL)
    If P0 = 0 Or PL = 0 Then Stop
    ' 0.001*29.38*0.5 = 1.469E-1 NB 29.38 = 1/(R * g)
    dZgi = -0.01469 * (TvL + TvLast) * Log(PL / Plast)
    dZg(L) = dZg(L - 1) + dZgi
    TvLast = TvL: Plast = PL
  Loop Until L = 33
  
' Then integrate down from a/c
  L = 16: Plast = P0: TvLast = Tv0
  Do
    L = L - 1
    ZL = z(L): TL = T(L)
    PL = fZtoP(ZL): TvL = fTvirtual50(TL)
    NoD(L) = fNumberDensity(PL, TL)
    If P0! = 0 Or PL! = 0 Then Stop
    dZgi = -0.01469 * (TvL + TvLast) * Log(PL / Plast)
    dZg(L) = dZg(L + 1) + dZgi
    TvLast = TvL: Plast = PL
  Loop Until L = 1 Or 34 - L = Nlev

End Sub

Function O2ABS!(T!, P!, V!, f!)
' This is the OXLIEB93 model, same as that function
Dim x!, y!
Static FZ!(1 To 44), a1!(1 To 44), a2!(1 To 44), a3!(1 To 44)
Static a4!(1 To 44), A5!(1 To 44), A6!(1 To 44), Init%

If Init = 0 Then
FZ(1) = 50.474238: FZ(2) = 50.987749: FZ(3) = 51.50335: FZ(4) = 52.02141
FZ(5) = 52.542394: FZ(6) = 53.066907: FZ(7) = 53.595749: FZ(8) = 54.13
FZ(9) = 54.671159: FZ(10) = 55.221367: FZ(11) = 55.783802: FZ(12) = 56.264775
FZ(13) = 56.363389: FZ(14) = 56.968206: FZ(15) = 57.612484: FZ(16) = 58.323877
FZ(17) = 58.44659: FZ(18) = 59.164207: FZ(19) = 59.590983: FZ(20) = 60.306061
FZ(21) = 60.434776: FZ(22) = 61.15056: FZ(23) = 61.800154: FZ(24) = 62.411215
FZ(25) = 62.48626: FZ(26) = 62.997977: FZ(27) = 63.568518: FZ(28) = 64.127767
FZ(29) = 64.678903: FZ(30) = 65.224071: FZ(31) = 65.764772: FZ(32) = 66.302091
FZ(33) = 66.83683: FZ(34) = 67.369598: FZ(35) = 67.900867: FZ(36) = 68.431005
FZ(37) = 68.960311: FZ(38) = 118.750343: FZ(39) = 368.49835: FZ(40) = 424.763124
FZ(41) = 487.24937: FZ(42) = 715.39315: FZ(43) = 773.839675: FZ(44) = 834.14533

a1(1) = 0.000000094: a1(2) = 0.000000246: a1(3) = 0.000000608: a1(4) = 0.00000141
a1(5) = 0.0000031: a1(6) = 0.00000641: a1(7) = 0.0000125: a1(8) = 0.0000228
a1(9) = 0.0000392: a1(10) = 0.0000632: a1(11) = 0.0000954: a1(12) = 0.0000549
a1(13) = 0.000134: a1(14) = 0.000176: a1(15) = 0.000214: a1(16) = 0.000239
a1(17) = 0.000146: a1(18) = 0.00024: a1(19) = 0.000211: a1(20) = 0.000212
a1(21) = 0.000246: a1(22) = 0.00025: a1(23) = 0.00023: a1(24) = 0.000193
a1(25) = 0.000152: a1(26) = 0.00015: a1(27) = 0.000109: a1(28) = 0.0000734
a1(29) = 0.0000464: a1(30) = 0.0000275: a1(31) = 0.0000153: a1(32) = 0.00000801
a1(33) = 0.00000395: a1(34) = 0.00000183: a1(35) = 0.000000801: a1(36) = 0.00000033
a1(37) = 0.000000128: a1(38) = 0.0000945: a1(39) = 0.00000679: a1(40) = 0.0000638
a1(41) = 0.0000235: a1(42) = 0.00000996: a1(43) = 0.0000671: a1(44) = 0.000018

a2(1) = 9.694: a2(2) = 8.694: a2(3) = 7.744: a2(4) = 6.844
a2(5) = 6.004: a2(6) = 5.224: a2(7) = 4.484: a2(8) = 3.814
a2(9) = 3.194: a2(10) = 2.624: a2(11) = 2.119: a2(12) = 0.015
a2(13) = 1.66: a2(14) = 1.26: a2(15) = 0.915: a2(16) = 0.626
a2(17) = 0.084: a2(18) = 0.391: a2(19) = 0.212: a2(20) = 0.212
a2(21) = 0.391: a2(22) = 0.626: a2(23) = 0.915: a2(24) = 1.26
a2(25) = 0.083: a2(26) = 1.665: a2(27) = 2.115: a2(28) = 2.62
a2(29) = 3.195: a2(30) = 3.815: a2(31) = 4.485: a2(32) = 5.225
a2(33) = 6.005: a2(34) = 6.845: a2(35) = 7.745: a2(36) = 8.695
a2(37) = 9.695: a2(38) = 0.009: a2(39) = 0.049: a2(40) = 0.044
a2(41) = 0.049: a2(42) = 0.145: a2(43) = 0.13: a2(44) = 0.147

a3(1) = 0.89: a3(2) = 0.91: a3(3) = 0.94: a3(4) = 0.97
a3(5) = 0.99: a3(6) = 1.02: a3(7) = 1.05: a3(8) = 1.07
a3(9) = 1.1: a3(10) = 1.13: a3(11) = 1.17: a3(12) = 1.73
a3(13) = 1.2: a3(14) = 1.24: a3(15) = 1.28: a3(16) = 1.33
a3(17) = 1.52: a3(18) = 1.39: a3(19) = 1.43: a3(20) = 1.45
a3(21) = 1.36: a3(22) = 1.31: a3(23) = 1.27: a3(24) = 1.23
a3(25) = 1.54: a3(26) = 1.2: a3(27) = 1.17: a3(28) = 1.13
a3(29) = 1.1: a3(30) = 1.07: a3(31) = 1.05: a3(32) = 1.02
a3(33) = 0.99: a3(34) = 0.97: a3(35) = 0.94: a3(36) = 0.92
a3(37) = 0.9: a3(38) = 1.63: a3(39) = 1.92: a3(40) = 1.93
a3(41) = 1.92: a3(42) = 1.81: a3(43) = 1.82: a3(44) = 1.81

a4(1) = 0.8: a4(2) = 0.8: a4(3) = 0.8: a4(4) = 0.8
a4(5) = 0.8: a4(6) = 0.8: a4(7) = 0.8: a4(8) = 0.8
a4(9) = 0.8: a4(10) = 0.8: a4(11) = 0.8: a4(12) = 0.8
a4(13) = 0.8: a4(14) = 0.8: a4(15) = 0.8: a4(16) = 0.8
a4(17) = 0.8: a4(18) = 0.8: a4(19) = 0.8: a4(20) = 0.8
a4(21) = 0.8: a4(22) = 0.8: a4(23) = 0.8: a4(24) = 0.8
a4(25) = 0.8: a4(26) = 0.8: a4(27) = 0.8: a4(28) = 0.8
a4(29) = 0.8: a4(30) = 0.8: a4(31) = 0.8: a4(32) = 0.8
a4(33) = 0.8: a4(34) = 0.8: a4(35) = 0.8: a4(36) = 0.8
a4(37) = 0.8: a4(38) = 0.8: a4(39) = 0.2: a4(40) = 0.2
a4(41) = 0.2: a4(42) = 0.2: a4(43) = 0.2: a4(44) = 0.2

A5(1) = 0.24: A5(2) = 0.22: A5(3) = 0.197: A5(4) = 0.166
A5(5) = 0.136: A5(6) = 0.131: A5(7) = 0.23: A5(8) = 0.335
A5(9) = 0.374: A5(10) = 0.258: A5(11) = -0.166: A5(12) = 0.39
A5(13) = -0.297: A5(14) = -0.416: A5(15) = -0.613: A5(16) = -0.205
A5(17) = 0.748: A5(18) = -0.722: A5(19) = 0.765: A5(20) = -0.705
A5(21) = 0.697: A5(22) = 0.104: A5(23) = 0.57: A5(24) = 0.36
A5(25) = -0.498: A5(26) = 0.239: A5(27) = 0.108: A5(28) = -0.311
A5(29) = -0.421: A5(30) = -0.375: A5(31) = -0.267: A5(32) = -0.168
A5(33) = -0.169: A5(34) = -0.2: A5(35) = -0.228: A5(36) = -0.24
A5(37) = -0.25: A5(38) = -0.036: A5(39) = 0#: A5(40) = 0#
A5(41) = 0#: A5(42) = 0#: A5(43) = 0#: A5(44) = 0#

A6(1) = 0.79: A6(2) = 0.78: A6(3) = 0.774: A6(4) = 0.764
A6(5) = 0.751: A6(6) = 0.714: A6(7) = 0.584: A6(8) = 0.431
A6(9) = 0.305: A6(10) = 0.339: A6(11) = 0.705: A6(12) = -0.113
A6(13) = 0.753: A6(14) = 0.742: A6(15) = 0.697: A6(16) = 0.051
A6(17) = -0.146: A6(18) = 0.266: A6(19) = -0.09: A6(20) = 0.081
A6(21) = -0.324: A6(22) = -0.067: A6(23) = -0.761: A6(24) = -0.777
A6(25) = 0.097: A6(26) = -0.768: A6(27) = -0.706: A6(28) = -0.332
A6(29) = -0.298: A6(30) = -0.423: A6(31) = -0.575: A6(32) = -0.7
A6(33) = -0.735: A6(34) = -0.744: A6(35) = -0.753: A6(36) = -0.76
A6(37) = -0.765: A6(38) = 0.009: A6(39) = 0#: A6(40) = 0#
A6(41) = 0#: A6(42) = 0#: A6(43) = 0#: A6(44) = 0#:
Init = 1
End If

Dim Pvap!, Pdry!, GammaZ!, Sox!, Foxim!, Sni!, FFni!, Cont!
Dim s!, Sum!, gamma!, Delta!, Fpp!, OXres!, i%, Tp!

Tp = 300 / T
Pvap = V / 0.7217 / Tp
Pdry = P - Pvap
GammaZ = 0.00056 * P * Tp ^ 0.8
Sox = 0.0000614 * Pdry * Tp ^ 2
Foxim = f * GammaZ / (f ^ 2 + GammaZ ^ 2)
Sni = 0.0000000000014 * Pdry ^ 2 * Tp ^ 3.5
FFni = f / (1 + 0.000019 * f ^ 1.5)
Cont = 0.182 * f * (Sox * Foxim + Sni * FFni)
Sum = 0
For i = 1 To 44
   s = a1(i) * Pdry * Tp ^ 3 * Exp(a2(i) * (1 - Tp))
   gamma = 0.001 * a3(i) * (Pdry * Tp ^ a4(i) + 1.1 * Pvap * Tp)
   Delta = 0.001 * (A5(i) + A6(i) * Tp) * P * Tp ^ 0.8
   x = (FZ(i) - f) ^ 2 + gamma ^ 2
   y = (FZ(i) + f) ^ 2 + gamma ^ 2
   Fpp = (1 / x + 1 / y) * gamma * f / FZ(i) - Delta * (f / FZ(i)) * ((FZ(i) - f) / x + (FZ(i) + f) / y)
   Sum = Sum + s * Fpp
Next i
OXres = 0.182 * f * Sum
O2ABS = (Cont + OXres) / 4.343  '[nepers/km]

End Function

Function ABS_O2(ByVal T!, ByVal P!, ByVal VD!, ByVal f!) As Single
'
' PURPOSE: RETURNS ABSORPTION COEFFICIENT DUE TO OXYGEN IN AIR, IN NEPERS/KM
'
' 5/1/95  P. Rosenkranz
' 11/5/97  P. Rosenkranz - 1- line modification.
' 12/16/98 pwr - updated submm freq's and intensities from HITRAN96
'
' ARGUMENTS:
'
'     NAME    UNITS     DESCRIPTION          VALID RANGE
'
'     T       KELVIN    TEMPERATURE          UNCERTAIN, but believed to be
'                                            valid for atmosphere
'     P       MILLIBARS PRESSURE             3 TO 1000
'     VD      G/M**3    WATER VAPOR DENSITY  ENTERS LINEWIDTH CALCULATION DUE TO GREATER BROADENING EFFICIENCY OF H2O
'     F       GHZ       FREQUENCY            0 TO 900
'
'     REFERENCES FOR EQUATIONS AND COEFFICIENTS:
'     P.W. Rosenkranz, CHAP. 2 and appendix, in ATMOSPHERIC REMOTE SENSING
'      BY MICROWAVE RADIOMETRY (M.A. Janssen, ed., 1993).
'     H.J. Liebe et al, JQSRT V.48, PP.629-643 (1992).
'     M.J. Schwartz, Ph.D. thesis, M.I.T. (1997).
'     SUBMILLIMETER LINE INTENSITIES FROM HITRAN96.
'     This version differs from Liebe's MPM92 in two significant respects:
'     1. It uses the modification of the 1- line width temperature dependence
'     recommended by Schwartz: (1/T).
'     2. It uses the same temperature dependence (X) for submillimeter
'     line widths as in the 60 GHz band: (1/T)**0.8
'
Dim y!
Static W300!(1 To 40), fr!(1 To 40), Y300!(1 To 40), S300!(1 To 40)  'X!, WB300!
Static V!(1 To 40), BE!(1 To 40), Init%
'C      LINES ARE ARRANGED 1-,1+,3-,3+,ETC. IN SPIN-ROTATION SPECTRUM
If Init = 0 Then
  fr(1) = 118.7503: fr(2) = 56.2648: fr(3) = 62.4863: fr(4) = 58.4466: fr(5) = 60.3061: fr(6) = 59.591
  fr(7) = 59.1642: fr(8) = 60.4348: fr(9) = 58.3239: fr(10) = 61.1506: fr(11) = 57.6125: fr(12) = 61.8002
  fr(13) = 56.9682: fr(14) = 62.4112: fr(15) = 56.3634: fr(16) = 62.998: fr(17) = 55.7838: fr(18) = 63.5685
  fr(19) = 55.2214: fr(20) = 64.1278: fr(21) = 54.6712: fr(22) = 64.6789: fr(23) = 54.13: fr(24) = 65.2241
  fr(25) = 53.5957: fr(26) = 65.7648: fr(27) = 53.0669: fr(28) = 66.3021: fr(29) = 52.5424: fr(30) = 66.8368
  fr(31) = 52.0214: fr(32) = 67.3696: fr(33) = 51.5034: fr(34) = 67.9009: fr(35) = 368.4984: fr(36) = 424.7632
  fr(37) = 487.2494: fr(38) = 715.3931: fr(39) = 773.8397: fr(40) = 834.1458
  
  S300(1) = 2.936E-15: S300(2) = 8.079E-16: S300(3) = 2.48E-15: S300(4) = 2.228E-15
  S300(5) = 3.351E-15: S300(6) = 3.292E-15: S300(7) = 3.721E-15: S300(8) = 3.891E-15
  S300(9) = 3.64E-15: S300(10) = 4.005E-15: S300(11) = 3.227E-15: S300(12) = 3.715E-15
  S300(13) = 2.627E-15: S300(14) = 3.156E-15: S300(15) = 1.982E-15: S300(16) = 2.477E-15
  S300(17) = 1.391E-15: S300(18) = 1.808E-15: S300(19) = 9.124E-16: S300(20) = 1.23E-15
  S300(21) = 5.603E-16: S300(22) = 7.842E-16: S300(23) = 3.228E-16: S300(24) = 4.689E-16
  S300(25) = 1.748E-16: S300(26) = 2.632E-16: S300(27) = 8.898E-17: S300(28) = 1.389E-16
  S300(29) = 4.264E-17: S300(30) = 6.899E-17: S300(31) = 1.924E-17: S300(32) = 3.229E-17
  S300(33) = 8.191E-18: S300(34) = 1.423E-17: S300(35) = 6.494E-16: S300(36) = 7.083E-15
  S300(37) = 3.025E-15: S300(38) = 1.835E-15: S300(39) = 1.158E-14: S300(40) = 3.993E-15
'  S300(17) = S300(17) * 1.0002
'  S300(19) = S300(19) * 1.0002
'Call BubbleSort(S300(), 40)
  BE(1) = 0.009: BE(2) = 0.015: BE(3) = 0.083: BE(4) = 0.084: BE(5) = 0.212: BE(6) = 0.212
  BE(7) = 0.391: BE(8) = 0.391: BE(9) = 0.626: BE(10) = 0.626: BE(11) = 0.915: BE(12) = 0.915
  BE(13) = 1.26: BE(14) = 1.26: BE(15) = 1.66: BE(16) = 1.665: BE(17) = 2.119: BE(18) = 2.115
  BE(19) = 2.624: BE(20) = 2.625: BE(21) = 3.194: BE(22) = 3.194: BE(23) = 3.814: BE(24) = 3.814
  BE(25) = 4.484: BE(26) = 4.484: BE(27) = 5.224: BE(28) = 5.224: BE(29) = 6.004: BE(30) = 6.004
  BE(31) = 6.844: BE(32) = 6.844: BE(33) = 7.744: BE(34) = 7.744: BE(35) = 0.048: BE(36) = 0.044
  BE(37) = 0.049: BE(38) = 0.145: BE(39) = 0.141: BE(40) = 0.145
  
'C      WIDTHS IN MHZ/MB'
'  WB300 = 0.56: X = 0.8
  W300(1) = 1.63: W300(2) = 1.646: W300(3) = 1.468: W300(4) = 1.449: W300(5) = 1.382: W300(6) = 1.36
  W300(7) = 1.319: W300(8) = 1.297: W300(9) = 1.266: W300(10) = 1.248: W300(11) = 1.221: W300(12) = 1.207
  W300(13) = 1.181: W300(14) = 1.171: W300(15) = 1.144: W300(16) = 1.139: W300(17) = 1.11: W300(18) = 1.108
  W300(19) = 1.079: W300(20) = 1.078: W300(21) = 1.05: W300(22) = 1.05: W300(23) = 1.02: W300(24) = 1.02
  W300(25) = 1#: W300(26) = 1#: W300(27) = 0.97: W300(28) = 0.97: W300(29) = 0.94: W300(30) = 0.94
  W300(31) = 0.92: W300(32) = 0.92: W300(33) = 0.89: W300(34) = 0.89: W300(35) = 1.92: W300(36) = 1.92
  W300(37) = 1.92: W300(38) = 1.81: W300(39) = 1.81: W300(40) = 1.81
  
  Y300(1) = -0.0233: Y300(2) = 0.2408: Y300(3) = -0.3486: Y300(4) = 0.5227: Y300(5) = -0.543
  Y300(6) = 0.5877: Y300(7) = -0.397: Y300(8) = 0.3237: Y300(9) = -0.1348: Y300(10) = 0.0311
  Y300(11) = 0.0725: Y300(12) = -0.1663: Y300(13) = 0.2832: Y300(14) = -0.3629: Y300(15) = 0.397
  Y300(16) = -0.4599: Y300(17) = 0.4695: Y300(18) = -0.5199: Y300(19) = 0.5187: Y300(20) = -0.5597
  Y300(21) = 0.5903: Y300(22) = -0.6246: Y300(23) = 0.6656: Y300(24) = -0.6942: Y300(25) = 0.7086
  Y300(26) = -0.7325: Y300(27) = 0.7348: Y300(28) = -0.7546: Y300(29) = 0.7702: Y300(30) = -0.7864
  Y300(31) = 0.8083: Y300(32) = -0.821: Y300(33) = 0.8439: Y300(34) = -0.8529: Y300(35) = 0#
  Y300(36) = 0#: Y300(37) = 0#: Y300(38) = 0#: Y300(39) = 0#: Y300(40) = 0#
  
  V(1) = 0.0079: V(2) = -0.0978: V(3) = 0.0844: V(4) = -0.1273:  V(5) = 0.0699
  V(6) = -0.0776: V(7) = 0.2309: V(8) = -0.2825: V(9) = 0.0436: V(10) = -0.0584
  V(11) = 0.6056: V(12) = -0.6619: V(13) = 0.6451: V(14) = -0.6759: V(15) = 0.6547
  V(16) = -0.6675: V(17) = 0.6135: V(18) = -0.6139: V(19) = 0.2952: V(20) = -0.2895
  V(21) = 0.2654: V(22) = -0.259:  V(23) = 0.375: V(24) = -0.368: V(25) = 0.5085
  V(26) = -0.5002: V(27) = 0.6206: V(28) = -0.6091:  V(29) = 0.6526: V(30) = -0.6393
  V(31) = 0.664: V(32) = -0.6475: V(33) = 0.6729: V(34) = -0.6545: V(35) = 0#
  V(36) = 0#: V(37) = 0#: V(38) = 0#: V(39) = 0#: V(40) = 0#
  Init = 1
End If
Dim Th!, Th1!, b!, PresWV!, PresDA!, den!, Dens!, dFnr!, Sum!, k%, df!, Str!, Sf1!, Sf2!

  Th = 300# / T
  Th1 = Th - 1#
  b = Th ^ 0.8   'X=0.8
  PresWV = VD * T / 217#
  PresDA = P - PresWV
  den = 0.001 * (PresDA * b + 1.1 * PresWV * Th)
  Dens = 0.001 * (PresDA + 1.1 * PresWV) * Th
  dFnr = 0.56 * den  'WB300=0.56
  Sum = 1.6E-17 * f * f * dFnr / (Th * (f * f + dFnr * dFnr))
  For k = 1 To 40
    If k = 1 Then 'exception for 1- line
      df = W300(1) * Dens
    Else
      df = W300(k) * den
    End If
    y = 0.001 * P * b * (Y300(k) + V(k) * Th1)
    Str = S300(k) * Exp(-BE(k) * Th1)
    Sf1 = (df + (f - fr(k)) * y) / ((f - fr(k)) ^ 2 + df * df)
    Sf2 = (df - (f + fr(k)) * y) / ((f + fr(k)) ^ 2 + df * df)
    Sum = Sum + Str * (Sf1 + Sf2) * (f / fr(k)) ^ 2
  Next k
  ABS_O2 = 503400000000# * Sum * PresDA * Th ^ 3 / 3.14159

End Function

Function OXLIEB93(T!, P!, V!, f!)
Static FZ!(1 To 44), a1!(1 To 44), a2!(1 To 44), a3!(1 To 44)
Static a4!(1 To 44), A5!(1 To 44), A6!(1 To 44), Init%
Dim Pvap!, Pdry!, GammaZ!, Sox!, Foxim!, Sni!, FFni!, Cont!
Dim s!, Sum!, gamma!, Delta!, Fpp!, OXres!, i%, Tp!, x!, y!

If Init = 0 Then
FZ(1) = 50.474238: FZ(2) = 50.987749: FZ(3) = 51.50335: FZ(4) = 52.02141
FZ(5) = 52.542394: FZ(6) = 53.066907: FZ(7) = 53.595749: FZ(8) = 54.13
FZ(9) = 54.671159: FZ(10) = 55.221367: FZ(11) = 55.783802: FZ(12) = 56.264775
FZ(13) = 56.363389: FZ(14) = 56.968206: FZ(15) = 57.612484: FZ(16) = 58.323877
FZ(17) = 58.44659: FZ(18) = 59.164207: FZ(19) = 59.590983: FZ(20) = 60.306061
FZ(21) = 60.434776: FZ(22) = 61.15056: FZ(23) = 61.800154: FZ(24) = 62.411215
FZ(25) = 62.48626: FZ(26) = 62.997977: FZ(27) = 63.568518: FZ(28) = 64.127767
FZ(29) = 64.678903: FZ(30) = 65.224071: FZ(31) = 65.764772: FZ(32) = 66.302091
FZ(33) = 66.83683: FZ(34) = 67.369598: FZ(35) = 67.900867: FZ(36) = 68.431005
FZ(37) = 68.960311: FZ(38) = 118.750343: FZ(39) = 368.49835: FZ(40) = 424.763124
FZ(41) = 487.24937: FZ(42) = 715.39315: FZ(43) = 773.839675: FZ(44) = 834.14533

a1(1) = 0.000000094: a1(2) = 0.000000246: a1(3) = 0.000000608: a1(4) = 0.00000141
a1(5) = 0.0000031: a1(6) = 0.00000641: a1(7) = 0.0000125: a1(8) = 0.0000228
a1(9) = 0.0000392: a1(10) = 0.0000632: a1(11) = 0.0000954: a1(12) = 0.0000549
a1(13) = 0.000134: a1(14) = 0.000176: a1(15) = 0.000214: a1(16) = 0.000239
a1(17) = 0.000146: a1(18) = 0.00024: a1(19) = 0.000211: a1(20) = 0.000212
a1(21) = 0.000246: a1(22) = 0.00025: a1(23) = 0.00023: a1(24) = 0.000193
a1(25) = 0.000152: a1(26) = 0.00015: a1(27) = 0.000109: a1(28) = 0.0000734
a1(29) = 0.0000464: a1(30) = 0.0000275: a1(31) = 0.0000153: a1(32) = 0.00000801
a1(33) = 0.00000395: a1(34) = 0.00000183: a1(35) = 0.000000801: a1(36) = 0.00000033
a1(37) = 0.000000128: a1(38) = 0.0000945: a1(39) = 0.00000679: a1(40) = 0.0000638
a1(41) = 0.0000235: a1(42) = 0.00000996: a1(43) = 0.0000671: a1(44) = 0.000018

a2(1) = 9.694: a2(2) = 8.694: a2(3) = 7.744: a2(4) = 6.844
a2(5) = 6.004: a2(6) = 5.224: a2(7) = 4.484: a2(8) = 3.814
a2(9) = 3.194: a2(10) = 2.624: a2(11) = 2.119: a2(12) = 0.015
a2(13) = 1.66: a2(14) = 1.26: a2(15) = 0.915: a2(16) = 0.626
a2(17) = 0.084: a2(18) = 0.391: a2(19) = 0.212: a2(20) = 0.212
a2(21) = 0.391: a2(22) = 0.626: a2(23) = 0.915: a2(24) = 1.26
a2(25) = 0.083: a2(26) = 1.665: a2(27) = 2.115: a2(28) = 2.62
a2(29) = 3.195: a2(30) = 3.815: a2(31) = 4.485: a2(32) = 5.225
a2(33) = 6.005: a2(34) = 6.845: a2(35) = 7.745: a2(36) = 8.695
a2(37) = 9.695: a2(38) = 0.009: a2(39) = 0.049: a2(40) = 0.044
a2(41) = 0.049: a2(42) = 0.145: a2(43) = 0.13: a2(44) = 0.147

a3(1) = 0.89: a3(2) = 0.91: a3(3) = 0.94: a3(4) = 0.97
a3(5) = 0.99: a3(6) = 1.02: a3(7) = 1.05: a3(8) = 1.07
a3(9) = 1.1: a3(10) = 1.13: a3(11) = 1.17: a3(12) = 1.73
a3(13) = 1.2: a3(14) = 1.24: a3(15) = 1.28: a3(16) = 1.33
a3(17) = 1.52: a3(18) = 1.39: a3(19) = 1.43: a3(20) = 1.45
a3(21) = 1.36: a3(22) = 1.31: a3(23) = 1.27: a3(24) = 1.23
a3(25) = 1.54: a3(26) = 1.2: a3(27) = 1.17: a3(28) = 1.13
a3(29) = 1.1: a3(30) = 1.07: a3(31) = 1.05: a3(32) = 1.02
a3(33) = 0.99: a3(34) = 0.97: a3(35) = 0.94: a3(36) = 0.92
a3(37) = 0.9: a3(38) = 1.63: a3(39) = 1.92: a3(40) = 1.93
a3(41) = 1.92: a3(42) = 1.81: a3(43) = 1.82: a3(44) = 1.81

a4(1) = 0.8: a4(2) = 0.8: a4(3) = 0.8: a4(4) = 0.8
a4(5) = 0.8: a4(6) = 0.8: a4(7) = 0.8: a4(8) = 0.8
a4(9) = 0.8: a4(10) = 0.8: a4(11) = 0.8: a4(12) = 0.8
a4(13) = 0.8: a4(14) = 0.8: a4(15) = 0.8: a4(16) = 0.8
a4(17) = 0.8: a4(18) = 0.8: a4(19) = 0.8: a4(20) = 0.8
a4(21) = 0.8: a4(22) = 0.8: a4(23) = 0.8: a4(24) = 0.8
a4(25) = 0.8: a4(26) = 0.8: a4(27) = 0.8: a4(28) = 0.8
a4(29) = 0.8: a4(30) = 0.8: a4(31) = 0.8: a4(32) = 0.8
a4(33) = 0.8: a4(34) = 0.8: a4(35) = 0.8: a4(36) = 0.8
a4(37) = 0.8: a4(38) = 0.8: a4(39) = 0.2: a4(40) = 0.2
a4(41) = 0.2: a4(42) = 0.2: a4(43) = 0.2: a4(44) = 0.2

A5(1) = 0.24: A5(2) = 0.22: A5(3) = 0.197: A5(4) = 0.166
A5(5) = 0.136: A5(6) = 0.131: A5(7) = 0.23: A5(8) = 0.335
A5(9) = 0.374: A5(10) = 0.258: A5(11) = -0.166: A5(12) = 0.39
A5(13) = -0.297: A5(14) = -0.416: A5(15) = -0.613: A5(16) = -0.205
A5(17) = 0.748: A5(18) = -0.722: A5(19) = 0.765: A5(20) = -0.705
A5(21) = 0.697: A5(22) = 0.104: A5(23) = 0.57: A5(24) = 0.36
A5(25) = -0.498: A5(26) = 0.239: A5(27) = 0.108: A5(28) = -0.311
A5(29) = -0.421: A5(30) = -0.375: A5(31) = -0.267: A5(32) = -0.168
A5(33) = -0.169: A5(34) = -0.2: A5(35) = -0.228: A5(36) = -0.24
A5(37) = -0.25: A5(38) = -0.036: A5(39) = 0#: A5(40) = 0#
A5(41) = 0#: A5(42) = 0#: A5(43) = 0#: A5(44) = 0#

A6(1) = 0.79: A6(2) = 0.78: A6(3) = 0.774: A6(4) = 0.764
A6(5) = 0.751: A6(6) = 0.714: A6(7) = 0.584: A6(8) = 0.431
A6(9) = 0.305: A6(10) = 0.339: A6(11) = 0.705: A6(12) = -0.113
A6(13) = 0.753: A6(14) = 0.742: A6(15) = 0.697: A6(16) = 0.051
A6(17) = -0.146: A6(18) = 0.266: A6(19) = -0.09: A6(20) = 0.081
A6(21) = -0.324: A6(22) = -0.067: A6(23) = -0.761: A6(24) = -0.777
A6(25) = 0.097: A6(26) = -0.768: A6(27) = -0.706: A6(28) = -0.332
A6(29) = -0.298: A6(30) = -0.423: A6(31) = -0.575: A6(32) = -0.7
A6(33) = -0.735: A6(34) = -0.744: A6(35) = -0.753: A6(36) = -0.76
A6(37) = -0.765: A6(38) = 0.009: A6(39) = 0#: A6(40) = 0#
A6(41) = 0#: A6(42) = 0#: A6(43) = 0#: A6(44) = 0#:
Init = 1
End If

Tp = 300 / T
Pvap = V / 0.7217 / Tp
Pdry = P - Pvap
GammaZ = 0.00056 * P * Tp ^ 0.8
Sox = 0.0000614 * Pdry * Tp ^ 2
Foxim = f * GammaZ / (f ^ 2 + GammaZ ^ 2)
Sni = 0.0000000000014 * Pdry ^ 2 * Tp ^ 3.5
FFni = f / (1 + 0.000019 * f ^ 1.5)
Cont = 0.182 * f * (Sox * Foxim + Sni * FFni)
Sum = 0
For i = 1 To 44
   s = a1(i) * Pdry * Tp ^ 3 * Exp(a2(i) * (1 - Tp))
   gamma = 0.001 * a3(i) * (Pdry * Tp ^ a4(i) + 1.1 * Pvap * Tp)
   Delta = 0.001 * (A5(i) + A6(i) * Tp) * P * Tp ^ 0.8
   x = (FZ(i) - f) ^ 2 + gamma ^ 2
   y = (FZ(i) + f) ^ 2 + gamma ^ 2
   Fpp = (1 / x + 1 / y) * gamma * f / FZ(i) - Delta * (f / FZ(i)) * ((FZ(i) - f) / x + (FZ(i) + f) / y)
   Sum = Sum + s * Fpp
Next i
OXres = 0.182 * f * Sum
OXLIEB93 = (Cont + OXres) / 4.343  '[nepers/km]
End Function
Function fVliebe!(T!, P!, V!, f!)
' COMPUTE VAPOR ABSORPTION AS A FUNCTION OF FREQUENCY (F),
' VAPOR DENSITY (V), PRESSURE (P), TEMPERATURE (T), LINE
' STRENGTH FACTOR (CL), CONTINUUM STRENGTH FACTOR (CC),
' AND LINE WIDTH FACTOR (CW).
' RESULT IN NEPERS
Dim FZ!, CL!, CW!, cC!, Pvap!, Pdry!, W!
Dim Term1!, Term2!, Term3!, Tp!

  FZ = 22.235
  CL = 1.05  '1.   Were Liebe '87 Model SJK corrected
  CW = 1#    '1.   to current values for TOPEX
  cC = 1.3   '1.2  Liebe 93 Model agrees w/sjk from 18-40 GHz
' NOMINAL LIEBE '87 MODEL
  Tp = 300# / T
  Pvap = V / 0.7223 / Tp
  Pdry = P - Pvap
  W = CW * 0.002784 * (Pdry * Tp ^ 0.6 + 4.8 * Pvap * Tp ^ 1.1)
  Term1 = CL * 0.0109 * Pvap * Tp ^ 3.5 * Exp(2.143 * (1# - Tp))
  Term2 = (W / FZ) * (1# / ((FZ - f) ^ 2 + W ^ 2) + 1# / ((FZ + f) ^ 2 + W ^ 2))
  Term3 = cC * 0.1 * (0.000000113 * Pdry * Tp ^ 0.5 + 0.00000357 * Pvap * Tp ^ 8) * Pvap * Tp ^ 2.5
  fVliebe = (1# / 4.34) * 0.182 * f ^ 2 * (Term1 * Term2 + Term3)
End Function

Function fTatZ!(z!, Ts!, LR!, Zt!, StandardT As Boolean)
Dim TZ!

  If StandardT Then
    TZ = fTstd(z)
  Else
    If z < Zt Then
      TZ = Ts + LR * z
    Else
      TZ = Ts + LR * Zt
    End If
  End If

  fTatZ = TZ

End Function


Function fRH(ByVal RhoV!, ByVal Tk!) As Single
Dim Tc!, Rw!
' RhoV .... Vapor density [g/m3]
' Tk ...... Temperature [K]
  Rw = 461.5           'Gas constant for water [J/kg K]
  Tc = Tk - cTo        'Temperature in Celcius
  fRH = (RhoV * Tk * Rw) / (6110 * 10 ^ (7.5 * Tc / (237.7 + Tc)))

End Function

Function fRhoVold!(RH!, Tk!)
' Tk is temperature in Kelvin
' Tc is temperature in Celcius
' RH is percent relative humidity (0 to 100)
' fRhoV is vapour density in g/m3
Dim Tc!
Tc = Tk - cTo        'Temperature in Celcius
fRhoVold = (RH / (Tk * cRv)) * 6110# * 10 ^ (7.5 * Tc / (237.7 + Tc)) ' Vapor density [g/m3]
End Function
Function Calc_O2abs(Elevation!, z!, Tsurface!, LapseRate!, StandardT As Boolean, Ztrop!, RhoVsurface!, Nu!, Nepers!) As Single
Dim T!, P!, r!, Absorption!, Zsum!, Asum!, A!, Aavg!
Dim Rsum!, a0!, SignEL%, i%, DRange!

' First calculate absorption at zero elevation
  T = fTatZ(z, Tsurface, LapseRate, Ztrop, StandardT)
  P = fZtoP(z)
  r = RhoVsurface * Exp(-z / 2#)
  Absorption = O2ABS(T, P, r, Nu)

If Elevation <> 0 Then
  If Elevation < 0 Then SignEL = -1 Else SignEL = 1
  Zsum = 0#: Rsum = 0#: Asum = 0#
  a0 = Absorption
  i = 0
  Ztrop = 11.5

' Algorithm is to integrate the absorption in range
' with a step size equal to 1/25 of the range at El=0
' Until the integrated absorption is 1 neper
  Do
    i = i + 1              'Cycle counter (for debugging only)
    DRange = (1 / a0) / 50 'step 1/25 of El=0 e-folding distance
    Zsum = Zsum + DRange * Sin(Elevation * mPI / 180)
    Rsum = Rsum + DRange
    ' Calculate temperature at top of slice
    T = fTatZ(z + Zsum, Tsurface, LapseRate, Ztrop, StandardT)
    P = fZtoP(z + Zsum) 'Pressure at top of slice
    r = RhoVsurface * Exp(-(z + Zsum) / 2#)
    A = O2ABS(T, P, r, Nu)
    Aavg = (A + a0) / 2
    Asum = Asum + DRange * Aavg
    'Debug.Print i; T; P; A; DRange; Zsum; Rsum; Asum
    a0 = A
  Loop Until Asum >= Nepers Or (z + Zsum) > 100
  
  Rsum = Rsum - (Asum - Nepers) / Aavg
  Absorption = 1 / Rsum
End If

Calc_O2abs = Absorption

End Function


Function fTstd!(z!)
' Temperature structure of 1976 US Standard Atmosphere
' z in km, fTstd in K
' MJ Mahoney JPL 19980510
Select Case z
Case Is <= 11: fTstd = 288.15 - 6.5 * z
Case Is <= 20: fTstd = 216.65
Case Is <= 32: fTstd = 216.65 + (z - 20)
Case Is <= 47: fTstd = 228.65 + 2.8 * (z - 32)
Case Is <= 51: fTstd = 270.65
Case Is <= 71: fTstd = 270.65 - 2.8 * (z - 51)
Case Is <= 84.852: fTstd = 214.65 - 2# * (z - 71)
Case Is <= 90: fTstd = 186.946
Case Is <= 95: fTstd = 186.946 + 0.2908 * (z - 90)
Case Is <= 100: fTstd = 188.4 + 1.34 * (z - 95)
Case Is <= 105: fTstd = 195.1 + 2.74 * (z - 100)
Case Is <= 110: fTstd = 208.8 + 6.24 * (z - 105)
Case Is <= 120: fTstd = 240 + 12 * (z - 110)
Case Else: fTstd = 360 + 12 * (z - 120)
End Select

End Function

Function fLRstd!(z!)
' Determine Lapse Rate (K/km) for a US Standard Atmosphere
' z in km
  Select Case z
  Case Is < 11: fLRstd = -6.5
  Case Is < 20: fLRstd = 0#
  Case Is < 32: fLRstd = 1#
  Case Is < 47: fLRstd = 2.8
  Case Is < 51: fLRstd = 0#
  Case Is < 71: fLRstd = -2.8
  Case Is < 84.852: fLRstd = -2#
  Case Is < 90: fLRstd = 0#
  Case Is < 95: fLRstd = 0.2908
  Case Is < 100: fLRstd = 1.34
  Case Is < 105: fLRstd = 2.74
  Case Is < 110: fLRstd = 6.24
  Case Else: fLRstd = 12#
  End Select

End Function
Function OXrosencran91!(T!, P!, RhoV!, Nu!)
' Rosencranz 91 Model (replaced by Lieve 93 Model)
' Calculate air absorption coefficient due to oxygen, in nepers/km
' T ...... K
' P ...... mb
' Rhov .... g/m3 (WATER VAPOR DENSITY - ENTERS LINEWIDTH CALCULATION
'             DUE TO GREATER BROADENING EFFICIENCY OF H2O)
' Nu ...... GHz
'
' Reference for equations and coefficients:
' P.W. ROSENKRANZ, Ch. 2 AND Appendix, in ATMOSPHERIC REMOTE SENSING
' BY MICROWAVE RADIOMETRY (M.A. JANSSEN, ED.)
'
  Static W300!(1 To 34), f!(1 To 34), Y300!(1 To 34), S300!(1 To 34), V!(1 To 34)
  Dim Str!, Sf1!, Sf2!
  Dim WB300!, x!, Theta!, b!, PWV!, Pda!, Rho!, dFnr!, Sum!, bFac!, df!, y!
  Dim i%, k%, k2%
  Static Init%
  x = 0
  If Init = 0 Then
' LINEWIDTHS FROM:
' LIEBE ET AL, IEEE T-AP 25, P.327 (1977), MULT BY 1.04;
' EXCEPT 1- FROM SETZER & PICKETT, J.CHEM.PHYS. 67, P.340 (1977).
' Y300 AND V FROM O2SOLVEA WITH LAGM =95, 279 TO 327K.
' LINES ARE ARRANGED 1-,1+,3-,3+,ETC.
'
  f(1) = 118.7503: f(2) = 56.2648: f(3) = 62.4863: f(4) = 58.4466
  f(5) = 60.3061: f(6) = 59.591: f(7) = 59.1642: f(8) = 60.4348
  f(9) = 58.3239: f(10) = 61.1506: f(11) = 57.6125: f(12) = 61.8002
  f(13) = 56.9682: f(14) = 62.4112: f(15) = 56.3634: f(16) = 62.998
  f(17) = 55.7838: f(18) = 63.5685: f(19) = 55.2214: f(20) = 64.1278
  f(21) = 54.6712: f(22) = 64.6789: f(23) = 54.13: f(24) = 65.2241
  f(25) = 53.5957: f(26) = 65.7648: f(27) = 53.0669: f(28) = 66.3021
  f(29) = 52.5424: f(30) = 66.8368: f(31) = 52.0214: f(32) = 67.3696
  f(33) = 51.5034: f(34) = 67.9009

' Widths in MHz/mb
  W300(1) = 1.63: W300(2) = 1.646: W300(3) = 1.468: W300(4) = 1.449
  W300(5) = 1.382: W300(6) = 1.36: W300(7) = 1.319: W300(8) = 1.297
  W300(9) = 1.266: W300(10) = 1.248: W300(11) = 1.221: W300(12) = 1.207
  W300(13) = 1.181: W300(14) = 1.171: W300(15) = 1.144: W300(16) = 1.139
  W300(17) = 1.11: W300(18) = 1.108: W300(19) = 1.079: W300(20) = 1.078
  W300(21) = 1.05: W300(22) = 1.05: W300(23) = 1.02: W300(24) = 1.02
  W300(25) = 1#: W300(26) = 1#: W300(27) = 0.97: W300(28) = 0.97
  W300(29) = 0.94: W300(30) = 0.94: W300(31) = 0.92: W300(32) = 0.92
  W300(33) = 0.89: W300(34) = 0.89

  Y300(1) = -0.0233: Y300(2) = 0.2408: Y300(3) = -0.3486: Y300(4) = 0.5227
  Y300(5) = -0.543: Y300(6) = 0.5877: Y300(7) = -0.397: Y300(8) = 0.3237
  Y300(9) = -0.1348: Y300(10) = 0.0311: Y300(11) = 0.0725: Y300(12) = -0.1663
  Y300(13) = 0.2832: Y300(14) = -0.3629: Y300(15) = 0.397: Y300(16) = -0.4599
  Y300(17) = 0.4695: Y300(18) = -0.5199: Y300(19) = 0.5187: Y300(20) = -0.5597
  Y300(21) = 0.5903: Y300(22) = -0.6246: Y300(23) = 0.6656: Y300(24) = -0.6942
  Y300(25) = 0.7086: Y300(26) = -0.7325: Y300(27) = 0.7348: Y300(28) = -0.7546
  Y300(29) = 0.7702: Y300(30) = -0.7864: Y300(31) = 0.8083: Y300(32) = -0.821
  Y300(33) = 0.8439: Y300(34) = -0.8529

  V(1) = 0.0079: V(2) = -0.0978: V(3) = 0.0844: V(4) = -0.1273
  V(5) = 0.0699: V(6) = -0.0776: V(7) = 0.2309: V(8) = -0.2825
  V(9) = 0.0436: V(10) = -0.0584: V(11) = 0.6056: V(12) = -0.6619
  V(13) = 0.6451: V(14) = -0.6759: V(15) = 0.6547: V(16) = -0.6675
  V(17) = 0.6135: V(18) = -0.6139: V(19) = 0.2952: V(20) = -0.2895
  V(21) = 0.2654: V(22) = -0.259: V(23) = 0.375: V(24) = -0.368
  V(25) = 0.5085: V(26) = -0.5002: V(27) = 0.6206: V(28) = -0.6091
  V(29) = 0.6526: V(30) = -0.6393: V(31) = 0.664: V(32) = -0.6475
  V(33) = 0.6729: V(34) = -0.6545
   
  S300(1) = 2.936E-15: S300(2) = 8.079E-16: S300(3) = 2.48E-15: S300(4) = 2.228E-15
  S300(5) = 3.351E-15: S300(6) = 3.292E-15: S300(7) = 3.721E-15: S300(8) = 3.891E-15
  S300(9) = 3.64E-15: S300(10) = 4.005E-15: S300(11) = 3.227E-15: S300(12) = 3.715E-15
  S300(13) = 2.627E-15: S300(14) = 3.156E-15: S300(15) = 1.982E-15: S300(16) = 2.477E-15
  S300(17) = 1.391E-15: S300(18) = 1.808E-15: S300(19) = 9.124E-16: S300(20) = 1.23E-15
  S300(21) = 5.603E-16: S300(22) = 7.842E-16: S300(23) = 3.228E-16: S300(24) = 4.689E-16
  S300(25) = 1.748E-16: S300(26) = 2.632E-16: S300(27) = 8.898E-17: S300(28) = 1.389E-16
  S300(29) = 4.264E-17: S300(30) = 6.899E-17: S300(31) = 1.924E-17: S300(32) = 3.229E-17
  S300(33) = 8.191E-18: S300(34) = 1.423E-17
  
  Init = 1
End If
'
   WB300 = 0.56: x = 0.8
   Theta = 300# / T
   b = Theta ^ x
   PWV = RhoV * T / 217#
   Pda = P - PWV
   Rho = 0.001 * (Pda * b + 1.1 * PWV * Theta)
   dFnr = WB300 * Rho
   Sum = 1.6E-17 * Nu * Nu * dFnr / (Theta * (Nu * Nu + dFnr * dFnr))
   For k = 1 To 34
     k2 = k / 2
     If (k2 * 2 <> k) Then bFac = Exp(-0.00689526 * k * (k + 1) * (Theta - 1))
     
     df = W300(k) * Rho
     y = 0.001 * P * b * (Y300(k) + V(k) * (Theta - 1))
     Str = S300(k) * bFac
     Sf1 = (df + (Nu - f(k)) * y) / ((Nu - f(k)) ^ 2 + df * df)
     Sf2 = (df - (Nu + f(k)) * y) / ((Nu + f(k)) ^ 2 + df * df)
     Sum = Sum + Str * (Sf1 + Sf2) * (Nu / f(k)) ^ 2
   Next k
   x = 503400000000# * Sum * Pda * Theta ^ 3 / cPI
  If x = 0 Then x = 0.0000001
  OXrosencran91 = x
End Function

Function fKelvin(ByVal pt!, ByVal P!) As Single
' Convert PT and P to Kelvin
  fKelvin = pt * (P / 1000) ^ 0.286
End Function

Function fPtoZblg(ByVal P!) As Single
Dim x!, y!, z!, alpha!
' P in [mb] to Z in [km] using standard atmosphere

Select Case P
Case Is < 0
  z = 100
Case Is <= 54.748
  x = P / 54.748: alpha = -1 / 34.16319474:
  If x <> 0 Then y = 1 - x ^ alpha
  z = 20 - 216.65 * y

Case Is <= 226.32
  x = P / 226.32: y = Log(x)
  z = 11 - 6.341620028 * y

Case Else
  alpha = 1 / 5.255876114: x = P / 1013.25
  z = (288.15 / 6.5) * (1 - x ^ alpha)
End Select

fPtoZblg = z

End Function


Function fTheta!(T!, P!)
  If P > 0 Then fTheta = T * (1000 / P) ^ 0.286 Else fTheta = 999.9
End Function

Sub ICEsat(RA!(), RT!(), Zice!, dTice!, Trop!, DoNAT As Boolean)
Dim j%, Zi!, Ti!, Tice!, dTi!

' Locate maximum ice saturation level (or NAT saturation level if DONAT=True)
Zice = 999.9: dTice = 999.9
For j = 1 To 33
  Zi = RA(j): Ti = RT(j) '  [km] & [Kelvin]
  If DoNAT Then Tice = fTnat(Zi) Else Tice = fTice(Zi)
  dTi = Ti - Tice
  If Zi > Trop And dTi < dTice Then dTice = dTi: Zice = Zi
Next j

End Sub

Function fTice!(z!)
' z [km]
  fTice = 188.5 - 0.89 * (z - 20)  'Ice saturation, assuming 4.5 ppmv
End Function

Function fTnat!(z!)
' z [km]
' f.Tnat = 213.58 - .8648 * z      'ice sat'n, 10 ppbv HNO3 & 5 ppmv H2O
  fTnat = 195.5 - 0.8626 * (z - 20) 'fit for 8 ppbv HNO3 & 4.5 ppmv H2O
End Function

Function fZtoPblg!(z!)
' Z [km] to P [mb]
Dim P!
  
  Select Case z
  Case Is <= 11: P = 1013.25 * Exp(5.255 * Log(1 - 6.5 * z / 288.15))
  Case Is < 20: P = 226.32 * Exp((11 - z) / 6.341620028)
  Case Else: P = 54.748 * Exp(-34.16319474 * Log(1 - (20 - z) / 216.65))
  End Select
  fZtoPblg = P
End Function

Function fZtoP!(z!)
' Convert US Standard Atmosphere 1976 from Pressure Altitude [km] to Pressure [mb]
' Uses Function fTstd (z) to calculate US standard temperatures
' This approach is taken as it reduces number of calculations
' MJ Mahoney JPL 1990507
'  Z         T          P           D
' km         K         mb           kg/m3
'  0      288.15  1013.25          1.22499919057116       1
' 11      216.65   226.3206        0.363917777824827      2
' 20      216.65    54.74888       8.80347996750117E-02   3
' 32      228.65     8.680185      1.32249977610308E-02   4
' 47      270.65     1.109063      1.42753221375531E-03   5
' 51      270.65     0.6693885     8.61604682553416E-04   6
' 71      214.65     3.956419E-02  6.42109635061132E-05   7
' 84.852  186.946    3.733836E-03  6.95787870826203E-06   8
' 90      186.946 9  0.2908
' 95      188.4  10  1.34
' 100     195.1  11  2.74
' 105     208.8  12  6.24
' 110     240.0  13 12.0
' 120     360.0  14

Dim P!, T!

T = fTstd(z)
Select Case z
Case Is <= 11
' P = P(1)*(T/T(1))^(-1000*cg/(cRs*LR(1)))
' P = 1013.25 * (T / 288.15) ^ (-1000 * cg / (cRs * -6.5)
  P = 1013.25 * (T / 288.15) ^ (5.2558761507598)
Case Is <= 20
' P = 226.3206 * Exp(-1000 * cg * (z - 11.) / (cRs * 216.65))
  P = 226.3206 * Exp(-0.157688414400825 * (z - 11#))
Case Is <= 32
' P = 54.74888 * (T / 216.65) ^ (-1000 * cg / (cRs * 1.)
  P = 54.74888 * (T / 216.65) ^ (-34.1631949799387)
Case Is <= 47
' P = 8.680185 * (T / 228.65) ^ (-1000 * cg / (cRs * 2.8)
  P = 8.680185 * (T / 228.65) ^ (-12.2011410642638)
Case Is <= 51
' P = 1.109063 * Exp(-1000 * cg * (Zs(i) - Zs(i - 1)) / (cRs * 270.65))
  P = 1.109063 * Exp(-0.126226473230884 * (z - 47))
Case Is <= 71
' P = 0.6693885 * (T / 270.65) ^ (-1000 * cg / (cRs * -2.8)
  P = 0.6693885 * (T / 270.65) ^ (12.2011410642638)
Case Is <= 84.852
' P = 0.03956419 * (T / 214.65) ^ (-1000 * cg / (cRs * -2.0)
  P = 0.039564189818084 * (T / 214.65) ^ (17.0815974899694)
Case Is <= 90
' P = 0.003733834 * Exp(-1000 * cg * (z - 84.852) / (cRs * 186.946))
  P = 0.003733834 * Exp(-0.182743653140151 * (z - 84.852))
Case Is <= 95
' P = 0.001457425 * (T / 184.946) ^ (-1000 * cg / (cRs * 0.2908)
  P = 1.45742511874549E-03 * (T / 186.946) ^ (-117.480037757699)
Case Is <= 100
' P = P(11)*(T/T(11))^(-1000*cg/(cRs*LR(11)))
' P = 0.0005808211 * (T / 188.4) ^ (-1000 * cg / (cRs * 1.34)
  P = 5.8654139565495E-04 * (T / 188.4) ^ (-25.4949216268199)
Case Is <= 105
' P = 0.0001815319 * (T / 195.1) ^ (-1000 * cg / (cRs * 2.74)
  P = 2.40645796828482E-04 * (T / 195.1) ^ (-12.4683193357411)
Case Is <= 110
' P = 0.00007788813 * (T / 208.8) ^ (-1000 * cg / (cRs * 6.24)
  P = 1.03251578598705E-04 * (T / 208.8) ^ (-5.4748709903748)
Case Else
' P = 0.00003633683 * (T / 240.) ^ (-1000 * cg / (cRs * 12)
  P = 4.81695302325482E-05 * (T / 240#) ^ (-2.84693291499489)
End Select
fZtoP = P
End Function
Function fPtoZ(ByVal P!) As Single
Dim z!
' Convert US Standard Atmosphere 1976 from Pressure [mb] to Pressure Altitude [km]
' MJ Mahoney JPL 1990507
'  Z         T          P           D                     N  LR
' km         K         mb           kg/m3                    K/km
'  0      288.15  1013.25          1.22499919057116       1  -6.5
' 11      216.65   226.3206        0.363917777824827      2   0.0
' 20      216.65    54.74888       8.80347996750117E-02   3   1.0
' 32      228.65     8.680185      1.32249977610308E-02   4   2.8
' 47      270.65     1.109063      1.42753221375531E-03   5   0.0
' 51      270.65     0.6693885     8.61604682553416E-04   6  -2.8
' 71      214.65     3.956419E-02  6.42109635061132E-05   7  -2.0
' 84.852  186.946    3.733836E-03  6.95787870826203E-06   8   0.0
' 90      186.946    1.45742511874549E-03                 9  0.2908
' 95      188.4      5.8654139565495E-04                 10  1.34
' 100     195.1      2.40645796828482E-04                11  2.74
' 105     208.8      1.03251578598705E-04                12  6.24
' 110     240.0      4.81695302325482E-05                13 12.0
' 120     360.0                                          14
Select Case P
Case Is > 226.3206     '<11 km
' z = Zs(1) + (Ts(1) / LRs(1)) * (1.-((P / Ps(1)) ^ (-(cRs * LRs(1))/(1000.*cg))))
  z = (44.3307692307692) * (1# - (P / 1013.25) ^ (0.190263235151657))
Case Is > 54.74888     '<20 km
' z = Zs(2) - (cRs * Ts(2) / (cg * 1000)) * ln(P / Ps(2))
  z = 11# - 6.34161998393947 * Log(P / 226.3206)
Case Is > 8.680185     '<32 km
' z = Zs(3) - (Ts(3) / LRs(3)) * (1.-((P / Ps(3)) ^ (-(cRs * LRs(3))/(1000.*cg))))
  z = 20 - 216.65 * (1# - (P / 54.74888) ^ (-2.92712669464088E-02))
Case Is > 1.109063     '<47 km
' z = Zs(4) - (Ts(4) / LRs(4)) * (1.-((P / Ps(4)) ^ ((cRs * LRs(4))/(1000.*cg))))
  z = 32 - 81.6607142857143 * (1# - (P / 8.680185) ^ (-8.19595474499447E-02))
Case Is > 0.6693885    '<51 km
' z = Zs(5) - (cRs * Ts(5) / (cg * 1000)) * ln(P / Ps(5))
  z = 47# - 7.92226839904554 * Log(P / 1.109063)
Case Is > 0.03956419   '<71 km
' z = Zs(6) - (Ts(6) / LRs(6)) * (1.-((P / Ps(6)) ^ ((cRs * LRs(6))/(1000.*cg))))
  z = 51 + 96.6607142857143 * (1# - (P / 0.6693885) ^ (8.19595474499447E-02))
Case Is > 0.003733834  '<84.852 km
' z = Zs(7) - (Ts(7) / LRs(7)) * (1.-((P / Ps(7)) ^ (-(cRs * LRs(7))/(1000.*cg))))
  z = 71 + 107.325 * (1# - (P / 0.03956419) ^ (5.85425338928176E-02))
Case Is > 1.45742511874549E-03   '<90 km
' z = Zs(8) - (cRs * Ts(8) / (cg * 1000)) * ln(P / Ps(8))
  z = 84.852 - 5.47214624555127 * Log(P / 0.003733834)
Case Is > 5.8654139565495E-04       '<95 km
' z = Zs(9) - (Ts(9) / LRs(9)) * (1.-((P / Ps(9)) ^ ((cRs * LRs(9))/(1000.*cg))))
  z = 90# - 642.8679 * (1# - (P / 1.45742511874549E-03) ^ (-8.51208458015383E-03))
Case Is > 2.40645796828482E-04  '<100 km
' z = Zs(4) - (Ts(10) / LRs(10)) * (1.-((P / Ps(10)) ^ ((cRs * LRs(10))/(1000.*cg))))
  z = 95# - 140.597 * (1# - (P / 5.8654139565495E-04) ^ (-3.92234986852218E-02))
Case Is > 1.03251578598705E-04 '<105 km
' z = Zs(4) - (Ts(4) / LRs(4)) * (1.-((P / Ps(4)) ^ ((cRs * LRs(4))/(1000.*cg))))
  z = 100# - 71.20438 * (1# - (P / 2.40645796828482E-04) ^ (-8.02032717123127E-02))
Case Is > 4.81695302325482E-05 '<110 km
' z = Zs(12) - (Ts(12) / LRs(12)) * (1.-((P / Ps(12)) ^ ((cRs * LRs(12))/(1000.*cg))))
  z = 105# - 33.46154 * (1# - (P / 1.03251578598705E-04) ^ (-0.18265269904593))
Case Else
' z = Zs(4) - (Ts(4) / LRs(4)) * (1.-((P / Ps(4)) ^ ((cRs * LRs(4))/(1000.*cg))))
  If P <= 0 Then P = 0.000001
  z = 110 - 20# * (1# - (P / 4.81695302325482E-05) ^ (-0.351255203356906))
End Select
fPtoZ = z
End Function
Function fTBarray(Freq!, El!, z!(), P!(), T!(), RH!(), RhoL!, BI!, Nlev%, Init As Boolean)
Dim SinEL!, f!, dP!, Pstep!, Rang!
Dim Knu!(1 To 200), Tau!(1 To 200), pALT!
Dim r!, fGndTB!, Sum!
Dim j%, jm1%, Mode%, Levels!
  
  SinEL = Sin(El * 3.14159 / 180)

' Create equal steps in Ln(P) to 50 km

  f = Freq
  pALT = z(1)
  T(1) = fTstd(pALT)
  P(1) = fZtoP(pALT)
  If SinEL = 0 Then fGndTB = T(1): Exit Function
  Knu(1) = OXLIEB93(T(1), P(1), 0#, f) + fVliebe(T(1), P(1), 0#, f)
  Rang = 10# / Knu(1)         'Integrate to 10 optical depths
  dP = P(1) - fZtoP(pALT + Rang * SinEL)
  Pstep = dP / Levels         'Pressure step in zenith direction
  Tau(1) = 0#
  Sum = 0#
  For j = 2 To Levels
    jm1 = j - 1
    P(j) = P(jm1) - Pstep    '
    z(j) = fPtoZ(P(j))
    T(j) = fTmdl(z(j), Mode)
    r = 0 ' fRhoVsjk(RI(j), T)
    Knu(j) = OXLIEB93(T(j), P(j), r, f) + fVliebe(T(j), P(j), r, f)
    Tau(j) = Tau(jm1) + 0.5 * (Knu(j) + Knu(jm1)) * (z(j) - z(jm1))
    Sum = Sum + (T(jm1) + T(j)) * (Exp(-Tau(jm1) / SinEL) - Exp(-Tau(j) / SinEL))
  Next j
  fGndTB = 0.5 * Sum + 2.75 * Exp(-Tau(Levels) / SinEL) 'Add CMB

End Function

Function fRhoInt(ByRef z!(), ByRef Rho!(), ByVal Nlev%, ByVal Zmax!, ZmaxFlag As Boolean) As Single
' Integrate liquid or vapor density (g/m3) to get column (cm)
' If ZmaxFlag is TRUE, integrate only to zo (km)
' Else integrate entire profile
Dim Sum!, i%, RhoZmax!

  Sum = 0#
  If ZmaxFlag Then  'Integrate to Zmax only
    i = 1
    Do While (z(i + 1) < Zmax) And (i + 1 <= Nlev)
      i = i + 1
      Sum = Sum + 0.5 * (Rho(i - 1) + Rho(i)) * (z(i) - z(i - 1))
    Loop
    RhoZmax = fLinterp(z(), Rho(), Nlev, Zmax)
    Sum = Sum + 0.5 * (RhoZmax + Rho(i)) * (Zmax - z(i))
  
  Else              'Integrate entire profile
    For i = 2 To Nlev
      Sum = Sum + 0.5 * (Rho(i - 1) + Rho(i)) * (z(i) - z(i - 1))
    Next i
  End If

  fRhoInt = Sum / 10 'cm, density of water = 1000000 g/m3 and 100000 cm/km

End Function

Function fVsound(T!)
Dim gamma!
gamma = cCp / cCv
fVsound = Sqr(gamma * cRd * T)
End Function

Function WtFunction(r!, AlphaO2!, AlphaLH20!)

WtFunction = (AlphaO2 + AlphaLH20) * Exp(-(AlphaO2 + AlphaLH20) * r)

End Function

Function fQ(ByVal RH!, ByVal P!, ByVal T!) As Single
Dim Rho!, RhoV!
' Calculate specific humidity
  RhoV = fRhoV(RH, T)   'gm/m3
  Rho = 100000 * P / (cRd * T) + RhoV
  fQ = RhoV / Rho

End Function

Function fTv(RH!, P!, T!)
Dim Rho!, RhoV!
' Calculate virtual temperature
' RH %
' P  mb
' T  K

RhoV = fRhoV(RH, T) / 1000         'kg/m3
Rho = 100 * P / (cRd * T) + RhoV   'kg/m3
fTv = T * (1# + 0.608 * RhoV / Rho)
End Function
Function fTvSJK!(RH!, P!, T!)
Dim Rho!, RhoV!
RhoV = fRhoVsjk(RH, T) / 1000      'kg/m3
Rho = 100 * P / (cRd * T) + RhoV   'kg/m3
fTvSJK = T * (1# + 0.608 * RhoV / Rho)
End Function
Function fg!(Zg!, Lat!)
' Reference http://www.mines.edu/fs_home/tboyd/GP311/MODULES/GRAV/main.html
  fg = fgnSMT(Lat!) - 0.003086 * Zg

End Function

Function fRell(Latitude!)
'Radius of ellipsoid based on WGS 84
'Rell=a/sqrt(1+e^2*sin(Lat))
'a 6378137
'b 6356752.314
'e^2 0.00669438

fRell = 6378.137 / Sqr(1 + 0.00669438 * Sin(Latitude) ^ 2)
End Function

Function fRe(ByVal Latitude!) As Single
' Earth's radius in km
  fRe = cRee * (1# - cfe * Sin(rpd * Latitude) ^ 2)

End Function
Function fTBmjm(pALT!, El!, Freq!, Levels%, Mode%)
Dim SinEL!, f!, dP!, Pstep!, Rang!, Sumlast!
Dim z!(1 To 200), Knu!(1 To 200), Tau!(1 To 200), T!(1 To 200)
Dim P!(1 To 200), r!, DeltaTB!, Sum!
Dim j%, jm1%, jm%, Done As Boolean, DeltaZ!, SumZ!, SumP!
SinEL = Sin(El * 3.14159 / 180)

' Create equal steps in Ln(P) to 50 km

  f = Freq
  z(1) = pALT
  T(1) = fTstd(pALT)
  P(1) = fZtoP(pALT)
  If SinEL = 0 Then fTBmjm = T(1): Exit Function
  Knu(1) = OXLIEB93(T(1), P(1), 0#, f) + fVliebe(T(1), P(1), 0#, f)
  Rang = 50# '/ Knu(1)         'Integrate to 10 optical depths
  dP = P(1) - fZtoP(pALT + Rang)
  Pstep = dP / (Levels - 1)       'Pressure step in zenith direction

  Tau(1) = 0#
  Sum = 0#
  SumZ = 0#
  SumP = 0#
  Done = False
  For j = 2 To Levels
    Sumlast = Sum
    jm1 = j - 1
    P(j) = P(jm1) - Pstep
    SumP = SumP + Pstep
    DeltaZ = fPtoZ(P(jm1)) - fPtoZ(P(j))
    SumZ = SumZ - DeltaZ
    'Debug.Print j; DeltaZ; SumZ; SumP
    z(j) = fPtoZ(P(j))
    T(j) = fTmdl(z(j), Mode)
    r = 0 ' fRhoVsjk(RI(j), T)
    Knu(j) = OXLIEB93(T(j), P(j), r, f) + fVliebe(T(j), P(j), r, f)
    Tau(j) = Tau(jm1) + 0.5 * (Knu(j) + Knu(jm1)) * (z(j) - z(jm1))
    DeltaTB = (T(jm1) + T(j)) * (Exp(-Tau(jm1) / SinEL) - Exp(-Tau(j) / SinEL))
    Sum = Sum + DeltaTB
    If DeltaTB < 0.00001 Then
    ' Debug.Print Levels; j; Format(100 * j / Levels, "#00"); Sum / 2# + 2.75 * Exp(-Tau(j) / SinEL);
      'Exit For
    End If
  Next j
  fTBmjm = 0.5 * Sum + 2.75 * Exp(-Tau(Levels) / SinEL) 'Add CMB
End Function

Function fTBsjk(pALT!, El!, Freq!, Levels%, Mode%)
Dim HI!(1 To 50), j%, jj%, i%, Ti!(1 To 50), Knu!(1 To 50), Tau!(1 To 50)
Static DelHI!(1 To 50)
Dim T!, r!, P!, f!, SinEL!, LastSum!, DeltaTB!, jm1%, Sum!, argUP!, argDN!, Tavg!

f = Freq
SinEL = Sin(El * 3.14159 / 180)
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
      If (HI(j) >= 100#) Then NH = j: Exit For
    NH = j
    Next j

  
  For j = 1 To NH
    T = Ti(j)
    r = 0# 'fRhoVsjk(RI(j), T)
    P = fZtoP(HI(j))
    
    Knu(j) = OXLIEB93(T, P, r, f) + fVliebe(T, P, r, f)
  Next j

' Compute opacity profile in the vertical for each integration level
  Tau(1) = 0#
  For j = 2 To NH
    jm1 = j - 1
    Tau(j) = Tau(jm1) + 0.5 * (Knu(j) + Knu(jm1)) * (HI(j) - HI(jm1))
  Next j
  
  Sum = 0#
  LastSum = 0
  For j = 1 To NH - 1
    argUP = Tau(j + 1) / SinEL
    argDN = Tau(j) / SinEL
    Tavg = (Ti(j) + Ti(j + 1)) / 2#
    DeltaTB = Tavg * (Exp(-argDN) - Exp(-argUP))
    If DeltaTB < 0.0001 Then
      'Exit For
    End If
    Sum = Sum + DeltaTB
  Next j

  fTBsjk = Sum + 2.75 * Exp(-Tau(NH) / SinEL)   'Add CMB

End Function

Function fTBtest(pALT!, El!, Freq!, Levels%, Mode%)
Dim TB1!, TB2!, dTB!, z!

For z = 0 To 50 Step 5
  TB1 = fTBmjm(z, El, Freq, Levels, 0)
  TB2 = fTBsjk(z, El, Freq, 0, 0)
  dTB = TB1 - TB2
  'Debug.Print z; TB1; TB2; Format(dTB, "#0.000")
Next z

End Function


Function fTmdl(z!, Mode%)
Dim Zbase!, Ztop!, LRinv!

Select Case Mode%
Case 0: fTmdl = fTstd(z): Exit Function
Case 1
  Zbase = 0.6
  Ztop = 0.9
  LRinv = 10
Case 2
  Zbase = 0.6
  Ztop = 0.9
  LRinv = 5
Case 3
  Zbase = 1#
  Ztop = 1.3
  LRinv = 10
End Select
  
Select Case z
Case Is <= Zbase: fTmdl = 288.15 - 6.5 * z
Case Is <= Ztop: fTmdl = 288.15 - 6.5 * Zbase + LRinv * (z - Zbase)
Case Is <= 11#: fTmdl = 288.15 - 6.5 * Zbase + LRinv * (Ztop - Zbase) - 6.5 * (z - Ztop)
Case Else: fTmdl = fTstd(z)
End Select

End Function
Sub DPDwvd(temp, dpde, WVD, RH)
'This "SUB procedure" calc's vapor density [g/m3] from inputted
'  temperature [K] and dew point depression [C].
'It is not valid for high altitudes because it has no pressure dependence.
Dim E!, Esat!, WVDsat!
  
  E = (26 + temp - 273.16 - dpde) / 33 - ((temp - 273.16 - dpde - 7) / 90) ^ 2
  WVD = (217 / (temp - dpde)) * Exp(2.3026 * E) '  water vapor density [g/m3]
  Esat = (26 + temp - 273.16) / 33 - ((temp - 273.16 - 7) / 90) ^ 2
  If temp <> 0 Then
    WVDsat = (217 / temp) * Exp(2.3026 * Esat) ' sat'n water vapor density [g/m3]
  Else
    WVDsat = 9999
  End If
  If WVDsat = 0 Then Stop
  RH = 100 * WVD / WVDsat  '  Relative Humidity [%]

End Sub
Function GetDPD!(x$)
Dim dpd!
dpd = Val(x$)
If dpd < 51 Then GetDPD = dpd / 10# Else GetDPD = dpd - 50#
End Function
Sub Zhme(P!(), T!(), RH!(), zo!, N%, Zg!())
Dim i%, Tv!(1 To 2000), GammaA!

GammaA = 1000 * 2 * cg / cRd ' 1000* to get km, 2 for average Tv
Zg(1) = zo
Tv(1) = fTvRH(P(1), T(1), RH(1))
For i = 2 To N
  Tv(i) = fTvRH(P(i), T(i), RH(i))
  Zg(i) = Zg(i - 1) - (Tv(i) + Tv(i - 1)) * Log(P(i) / P(i - 1)) / GammaA
Next i

End Sub

Function Ztopx(z!)
' Convert US Standard Atmosphere 1976 from Pressure Altitude [km] to Pressure [mb]
' Uses Function fTstd (z) to calculate US standard temperatures
' This approach is taken as it reduces number of calculations
' MJ Mahoney JPL 1990507
'GPH         T          P           D
' km         K         mb           kg/m3
'  0      288.15  1013.25          1.22499919057116       1
' 11      216.65   226.3206        0.363917777824827      2
' 20      216.65    54.74888       8.80347996750117E-02   3
' 32      228.65     8.680185      1.32249977610308E-02   4
' 47      270.65     1.109063      1.42753221375531E-03   5
' 51      270.65     0.6693885     8.61604682553416E-04   6
' 71      214.65     3.956419E-02  6.42109635061132E-05   7
' 84.852  186.946    3.733836E-03  6.95787870826203E-06   8
' Note Z is Geopotential Height
Dim Init As Boolean, GH!(0 To 7), LR!(0 To 7), r!, g!, Rstar!
Dim M!, k!, i%

'Case Is <= 11
' P = 1013.25 * (T / 288.15) ^ (-1000 * cg / (cRs * -6.5)
'  P = 1013.25 * (T / 288.15) ^ (5.2558761507598)
'Case Is <= 20
' P = 226.3206 * Exp(-1000 * cg * (z - 11.) / (cRs * 216.65))
'  P = 226.3206 * Exp(-0.157688414400825 * (Z - 11#))

If Not Init Then
  GH(0) = 0#: LR(0) = -6.5
  GH(1) = 11#: LR(1) = 0#
  GH(2) = 20#: LR(2) = 1#
  GH(3) = 32#: LR(3) = 2.8
  GH(4) = 47#: LR(4) = 0
  GH(5) = 51#: LR(5) = -2.8
  GH(6) = 71#: LR(6) = -2#
  GH(7) = 84.852: LR(7) = 0#
  Init = True
  g = 9.80665        'm / s2
  Rstar = 0.00831432 'N m / kmol K
  M = 28.9522        'kg / kmol
  r = Rstar / M      'N m / kg K
  k = g / r
  
End If

i = 0
Do:  i = i + 1
Loop Until z >= GH(i)

End Function


