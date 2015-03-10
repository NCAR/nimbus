Attribute VB_Name = "NGVTcal"
Public pALT!, Pitch!, Roll!, Latitude!, Longitude!, UTsec!, UTks!

Public ALTkm!, Nlev%, Nlevr%, FltDate$, ZT1!, TT1!, LT1!
Public nLatitude!, nLongitude!, nPitch!, nRoll!, nUTsec&, nZp!, nP!, nTr!, nTa!


Function fUsing(ByVal fmt$, ByRef V!()) As String
Dim L%, M%, i%, j%, k%, f$, out$, n$
' Generate a string using the format in fmt$ and the order parameters in V()
  
  M% = 1
  L% = Len(fmt$)
  i% = InStr(1, fmt$, "#", 1) ' find first field start =#
  out$ = Space(i% - 1)        ' save leading spaces before # if any
  
  Do
    j% = InStr(i%, fmt$, " ", 1)   'find next field location (delimited by space)
    If j% = 0 Then j% = L% + 1     'none if end of fmt$
    k% = j% - i%                   'number of character in current field
    f$ = Mid$(fmt$, i%, k%)
    n$ = Format$(V(M%), f$)        'format number
    M% = M% + 1
    If k - Len(n$) >= 0 Then
      out$ = out$ + Space(k% - Len(n$)) + n$  ' add spaces so field has correct length
    End If
    i% = InStr(j%, fmt$, "#", 1)  'Get next field
    If i% <> 0 Then out$ = out$ + Space(i% - j%)
  Loop Until i% = 0
  fUsing$ = out$

End Function
Function fDOYtoYMD(Iyear%, Doy%) As String
'       SUBROUTINE W3FS26(JLDAYN, Iyear, Month, Iday, IDAYWK, IDAYYR)
'C$$$   SUBPROGRAM  DOCUMENTATION  BLOCK
'c
'C SUBPROGRAM: W3FS26         YEAR, MONTH, DAY FROM JULIAN DAY NUMBER
'C   AUTHOR: JONES,R.E.       ORG: W342       DATE: 87-03-29
'c
'C ABSTRACT: COMPUTES YEAR (4 DIGITS), MONTH, DAY, DAY OF WEEK, DAY
'C   OF YEAR FROM JULIAN DAY NUMBER. THIS SUBROUTINE WILL WORK
'C   FROM 1583 A.D. TO 3300 A.D.
'c
'C PROGRAM HISTORY LOG:
'C   87-03-29  R.E.JONES
'C   88-07-06  R.E.JONES  CHANGE TO MICROSOFT FORTRAN 4.10
'C   90-06-11  R.E.JONES  CHANGE TO SUN FORTRAN 1.3
'C   91-03-29  R.E.JONES  CONVERT TO SiliconGraphics FORTRAN
'c
'c USAGE:  Call W3FS26(JLDAYN, Iyear, Month, Iday, IDAYWK, IDAYYR)
'c
'C   INPUT VARIABLES:
'C     NAMES  INTERFACE DESCRIPTION OF VARIABLES AND TYPES
'C     ------ --------- -----------------------------------------------
'C     JLDAYN ARG LIST  INTEGER*4 JULIAN DAY NUMBER
'c
'C   OUTPUT VARIABLES:
'C     NAMES  INTERFACE DESCRIPTION OF VARIABLES AND TYPES
'C     ------ --------- -----------------------------------------------
'C     IYEAR  ARG LIST  INTEGER*4 YEAR  (4 DIGITS)
'C     MONTH  ARG LIST  INTEGER*4 MONTH
'C     IDAY   ARG LIST  INTEGER*4 DAY
'C     IDAYWK ARG LIST  INTEGER*4 DAY OF WEEK (1 IS SUNDAY, 7 IS SAT)
'C     IDAYYR ARG LIST  INTEGER*4 DAY OF YEAR (1 TO 366)
'c
'C   REMARKS: A JULIAN DAY NUMBER CAN BE COMPUTED BY USING ONE OF THE
'C     FOLLOWING STATEMENT FUNCTIONS. A DAY OF WEEK CAN BE COMPUTED
'C     FROM THE JULIAN DAY NUMBER. A DAY OF YEAR CAN BE COMPUTED FROM
'C     A JULIAN DAY NUMBER AND YEAR.
'c
'C      IYEAR (4 DIGITS)
'c
'c JDN(Iyear, Month, Iday) = Iday - 32075
'c X + 1461 * (Iyear + 4800 + (Month - 14) / 12) / 4
'c y + 367 * (Month - 2 - (Month - 14) / 12 * 12) / 12
'c z - 3 * ((Iyear + 4900 + (Month - 14) / 12) / 100) / 4
'c
'C      IYR (4 DIGITS) , IDYR(1-366) DAY OF YEAR
'c
'c JULIAN(IYR, IDYR) = -31739 + 1461 * (IYR + 4799) / 4
'c X - 3 * ((IYR + 4899) / 100) / 4 + IDYR
'c
'C      DAY OF WEEK FROM JULIAN DAY NUMBER, 1 IS SUNDAY, 7 IS SATURDAY.
'c
'C      JDAYWK(JLDAYN) = MOD((JLDAYN + 1),7) + 1
'c
'C      DAY OF YEAR FROM JULIAN DAY NUMBER AND 4 DIGIT YEAR.
'c
'C      JDAYYR(JLDAYN,IYEAR) = JLDAYN -
'c X(-31739 + 1461 * (Iyear + 4799) / 4 - 3 * ((Iyear + 4899) / 100) / 4)
'c
'C      THE FIRST FUNCTION WAS IN A LETTER TO THE EDITOR COMMUNICATIONS
'C      OF THE ACM  VOLUME 11 / NUMBER 10 / OCTOBER, 1968. THE 2ND
'C      FUNCTION WAS DERIVED FROM THE FIRST. THIS SUBROUTINE WAS ALSO
'C      INCLUDED IN THE SAME LETTER. JULIAN DAY NUMBER 1 IS
'C      JAN 1,4713 B.C. A JULIAN DAY NUMBER CAN BE USED TO REPLACE A
'C      DAY OF CENTURY, THIS WILL TAKE CARE OF THE DATE PROBLEM IN
'C      THE YEAR 2000, OR REDUCE PROGRAM CHANGES TO ONE LINE CHANGE
'C      OF 1900 TO 2000. JULIAN DAY NUMBERS CAN BE USED FOR FINDING
'C      RECORD NUMBERS IN AN ARCHIVE OR DAY OF WEEK, OR DAY OF YEAR.
'c
'c Attributes:
'C   LANGUAGE: SiliconGraphics 3.3 FORTRAN 77
'C   MACHINE:  SiliconGraphics IRIS-4D/25
'c
'C$$$
'c
'c
'       Save
'c
Dim JLDAYN&, L&, n&, i&, j&, IDAYWK&, IDAYYR&, Month&
Select Case Iyear
Case 1997: JLDAYN = 2450448.5
Case 1998: JLDAYN = 2450813.5
Case 1999: JLDAYN = 2451178.5
Case Else
  Call MsgBox(Str$(Iyear) + " is not supported by this function!", vbOKOnly)
  Exit Function
End Select

  L = JLDAYN + 68569 + Doy + 1

  n = (4 * L) \ 146097
  L = L - (146097 * n + 3) \ 4
  i = 4000 * (L + 1) \ 1461001
  L = L - 1461 * i \ 4 + 31
  j = 80 * L \ 2447
  Iday = L - 2447 * j \ 80
  L = j \ 11
  Month = j + 2 - 12 * L
  Iyear = 100 * (n - 49) + i + L
  IDAYWK = ((JLDAYN + 1) Mod 7) + 1
  IDAYYR = JLDAYN - (-31739 + 1461 * (Iyear + 4799) / 4 - 3 * ((Iyear + 4899) / 100) / 4)
  fDOYtoYMD = Format(Iyear, "0000") + Format(Month, "00") + Format(Iday, "00")
End Function

Function fTvRH(P!, T!, RH!)
' Calculate virtual temperature from p, T, and RH
' Need function fEs to calculate water vapor saturation mixing ratio

fTvRH = T / (1 - (RH / 100) * (fEs(T) / P) * (1 - cEa))

End Function
Function fEs(ByVal Tk!) As Single
Dim dT!, es!, i%
Static a!(0 To 6), Init As Boolean, cTo!
'Water vapor saturation mixing ratio
'Tk in Kelvin, Es in mb
'See Flatau et al., 1992, J. App. Meteorol.,31,1507-1513.

  If Not Init Then
    a(0) = 6.1117675       'Saturation vapor pressure (hPa)
    a(1) = 0.443986062
    a(2) = 0.0143053301
    a(3) = 0.000265027242
    a(4) = 0.00000302246994
    a(5) = 2.03886313E-08
    a(6) = 6.38780966E-11
    cTo = 273.15
    Init = True
  End If

  dT = Tk - cTo
  es = a(0)
  For i = 1 To 6
    es = es + a(i) * dT ^ i
  Next i

  fEs = es

End Function
Function fEv(ByVal RH!, ByVal T!) As Single
' Vapor pressure (mb) given RH (%) and T (K)
  fEv = RH * fEs(T) / 100#

End Function
Function fLinterp(z!(), V!(), n%, zo!) As Single
' Do linear interpolation in z to find value of at zo
Dim i%, it%, ib%

  i = 0
  Do
    i = i + 1
  Loop Until z(i) >= zo Or i = n
  If z(i) = zo Then fLinterp = V(i): Exit Function
  If i > n Then fLinterp = V(n): Exit Function
  it = i
  ib = i - 1
  If ib = 0 Then
    fLinterp = V(it)
  Else
    fLinterp = V(ib) + (V(it) - V(ib)) * (zo - z(ib)) / (z(it) - z(ib))
  End If

End Function
Sub ReadNGV(ngvlu%)
Dim a$, i%, j%, HHMMSS$


'UTC      ATRL  AT_A  GGLAT  GGLON  PALT  PSFC  Time
'15:35:04 -68.1329 -67.1145 40.0492 -107.456 12191.3 187.418 1504
'15:35:14 -68.1442 -67.1961 40.0502 -107.48 12193.9 187.339 1514

Input #ngvlu, a$
i = InStr(1, a$, " ")
HHMMSS$ = Left$(a$, i - 1)
nUTsec = fTstringToSec(HHMMSS$, True)
j = InStr(i + 1, a$, " ")
nTr = Val(Mid$(a$, i + 1, j - i - 1))
i = InStr(j + 1, a$, " ")
nTa = Val(Mid$(a$, j + 1, i - j - 1))
j = InStr(i + 1, a$, " ")
nLatitude = Val(Mid$(a$, i + 1, j - i - 1))
i = InStr(j + 1, a$, " ")
nLongitude = Val(Mid$(a$, j + 1, i - j - 1))
j = InStr(i + 1, a$, " ")
nZp = Val(Mid$(a$, i + 1, j - i - 1)) / 1000#
i = InStr(j + 1, a$, " ")
nP = Val(Mid$(a$, j + 1, i - j - 1))



End Sub

Function fTstringToSec(ByVal T$, ByVal ColonFlag As Boolean) As Long
Dim i%, Hr%, Min%, Sec%
'Convert a time string (T$) to seconds
'If Colon_Flag is TRUE, T$ format is assumed to be hh:mm:ss; otherwise, hhmmss
  
  If ColonFlag Then i% = 3 Else i% = 2
  Hr = Val(Mid$(T$, 1, 2))
  Min = Val(Mid$(T$, 1 + i%, 2))
  Sec = Val(Mid$(T$, 1 + 2 * i%, 2))
  fTstringToSec& = 3600# * Hr + 60# * Min + Sec

End Function
Sub Zhme(P!(), T!(), RH!(), zo!, n%, Zg!())
Dim i%, Tv!(1 To 2000), GammaA!

GammaA = 1000 * 2 * cg / cRd ' 1000* to get km, 2 for average Tv
Zg(1) = zo
Tv(1) = fTvRH(P(1), T(1), RH(1))
For i = 2 To n
  Tv(i) = fTvRH(P(i), T(i), RH(i))
  Zg(i) = Zg(i - 1) - (Tv(i) + Tv(i - 1)) * Log(P(i) / P(i - 1)) / GammaA
Next i

End Sub

Function fRhoV(ByVal RH!, ByVal T!) As Single
' Vapor density (gm/m3) given RH (%) and T (K)
' Using ideal gas law
  If T = 0 Then T = 273.15
  fRhoV = 100000 * cEa * fEv(RH, T) / (cRd * T)

End Function
Function fHypsometricEqn(ByVal T1!, ByVal p1!, ByVal T2!, ByVal p2!) As Single
' Change in geopotential height (km) between pressure levels P1 and p2 (hPa/mb)
' with corresponding temperatures T1 and T2 (K)
' cRd = 287.05307            'J /kg K      Gas Constant (dry air)
' cg = 9.80665               'm /s2        Acceleration of gravity(surface)
'
  fHypsometricEqn = cRd * (T1 + T2) * Log(p1 / p2) / cg / 2000  'Delta km

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





Function fReadLF(lu%) As String
Dim c$, out$
out$ = ""
Do
  c$ = Input(1, lu)
  Select Case c$
  Case vbLf
  Case vbCr
  Case Else
    out$ = out$ + c$
  End Select
Loop Until c$ = vbLf Or EOF(lu)
fReadLF = out$
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


Function fNumeric(ByVal X$) As Boolean
Dim i%, n%
' Checks if x$ is an unsigned integer number

  For i = 1 To Len(X$)
    n = Asc(Mid$(X$, i, 1))
    If n < 48 Or n > 57 Then  '0 thru 9
      fNumeric = False
      Exit Function
    End If
  Next i
  fNumeric = True
  
End Function
Function fGCD(LAT1!, LON1!, LAT2!, LON2!) As Single
' Calculate Great Circle Distance between two points in km
Dim D!

D = ACN(Sin(LAT1 * rpd) * Sin(LAT2 * rpd) + Cos(LAT1 * rpd) * Cos(LAT2 * rpd) * Cos((LON1 - LON2) * rpd))
fGCD = 1.852 * 60 * D / rpd 'km (1.852 km / nm, and 1 minute = 1 nm)

End Function


Function ACN(ByVal X!) As Single
' Take arccosine of a number (x!)
' Valid only for +- 90 degrees
Dim y!

  If Abs(X) < 0.00001 Then
    Select Case Sgn(X)
    Case Is < 0: ACN = -Pi / 2
    Case Else: ACN = Pi / 2
    End Select
  Else
    ACN = Atn(Sqr(1 - X ^ 2) / X)
  End If

End Function
