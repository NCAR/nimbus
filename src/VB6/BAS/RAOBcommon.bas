Attribute VB_Name = "RAOBcommon"
Option Explicit
Public ZGraob!(1 To 14000)   'Geopotential Height (km)
Public PZraob!(1 To 14000)   'Pressure (mb)
Public TZraob!(1 To 14000)   'Temperature (K)
Public RZraob!(1 To 14000)   'Relative Humidity (%)
Public ZPraob!(1 To 14000)   'Pressure Altitude (km) - calculated from PZraob
Public RVraob!(1 To 14000)   'Vapor density (gm/m3)
Public Qr!(1 To 5), RR!(1 To 5), Pr!(1 To 25), Tr!(1 To 25)

Public Zplot!(1 To 14000), Tplot!(1 To 14000), LZraob%(1 To 3), Rplot!(1 To 14000), ZGplot!(1 To 14000)
Public AvgT!(1 To 20), RmsT!(1 To 20), ColdestT!, ColdestZ!, Tcoldest!, Zcoldest!

Public RAOB As RAOBrecord, WMO As WMOrecord, LRF As LargeRAOB, GP As GPrecord
Public RAOB2 As RAOBrecord2, RAOB3 As RAOBrecord3
Public CITY As CITYrecord
Public RAOBfilename$, nRAOB%, RAOBmin!, RCfilename$
Public Forward As Boolean, FileRecords&, ijkl%
Public Forward2 As Boolean
Public Iyear%, Idoy%, Imonth%, Iday%, Ihour%, Imin%, Isec%, Ihhmm%, Nlevr%
Public WMO4l As String * 4, Nedit%, DandT As Date
Public Ztrop!, Ttrop!, Ltrop%, ZtropE!, TtropE!, LtropE%, Ztrop2!, Ttrop2!, Ltrop2%, dTMRI!, zMRI1!, zMRI2!
Public RAOBgood As Boolean, TropGood As Boolean, RAOBedit%
Public EditLevel As Boolean, EditTrop As Boolean
Public UseEditLevel As Boolean, UseEditTrop As Boolean
Public SelectLevel%, RAOBtype%, OATraob!, OATraobPP!, RAOBtypeOUT%
Public CINlu%, IntRhoV!
Public OutFileRecords&, OutFileRecord&, RMSfilename$
Public iLiquid1&, iLiquid2&, RhoLvapor!


Public WMOnumber&, WMOstation$, WMOstate$, WMOcountry$, WMOregionID%, WMOlatitude!, WMOlongitude!, WMOaltitude%
Public GPcards&, GPicao$, GPstation$, GPlatitude!, GPlongitude!, GPaltitude%
Public GPregion%, GPcountry$, GPstate$, GPspare%, ICAOsite$
Public nRAOBlevels%, nRAOBlevelsOUT%
Public ReInit As Boolean, SourceFormat%

Public CityName$, CityState$, CityCountry$, CityRegionID%
Public CityLatitude!, CityLongitude!, CityAltitude%, CityPopulation&, CitySpares%(1 To 46)

Public Const RAOBtop! = 50#      'Height (km) of extended RAOB

'123456789012345678901234567890123456789012345678901234567890
' NKX   3190 72293  32.87 117.15  134  19890915 19999999  MIRAMAR NAS              CA  US
'123456789012345678901234567890123456789012345678901234567890
' NKX   3190 72293  32.87 117.15  134  19890915 19999999  MIRAMAR NAS              CA  US
'
'NOAA
'72;393;KVBG;Vandenberg Air Force Base;CA;United States;4;34-45N;120-34W;34-44N;120-33W;112;121;P
'KVBG;72;393;Vandenberg Air Force Base;CA;United States;4;34-45N;120-34W;34-44N;120-33W;112;121;P
'
'GEMPAK Station List
' http://www.unidata.ucar.edu/software/gempak/tutorial/station_tables.html
'Station tables should use the following format:<pre><FONT COLOR=#800000>
'(A8,1X,I6,1X,A32,1X,A2,1X,A2,1X,I5,1X,I6,1X,I5)</font>
' ^     ^     ^      ^     ^     ^     ^     ^
' |     |     |      |     |     |     |     |
' |     |     |      |     |     |     |     --- Elevation (meters)
' |     |     |      |     |     |     --- Longitude (west = negative)
' |     |     |      |     |     --- Latitude (south = negative)
' |     |     |      |     --- Country
' |     |     |      --- State
' |     |     --- Station Name
' |     --- Station Number
' --- Station ID
'Note that Latitude and Longitude are expressed in hundredths of decimal degrees (not DMS!).
'0        1         2         3         4         5         6         7
'1234567890123456789012345678901234567890123456789012345678901234567890123
'$$$$$$$$ %%%%%% $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $$ $$ %%%%% %%%%%% %%%%%
'VBCI      48109 COCO ISLAND                      -- BM  1412   9337     3
'VBG       72393 VANDENBERG AFB                   CA US  3465 -12057   112
Type GPrecord
  GPcards As Long               ' CARDS number = WMO*10
  GPicao As String * 8          ' Alpha name
  GPlatitude As Single          '
  GPlongitude As Single         '
  GPaltitude As Integer         ' meters MSL
  GPregion As Integer           ' WMO region
  GPcountry As String * 2       ' FIPS PUB 10-4 Country Code
  GPstate As String * 2         '
  GPstation As String * 32      '
  GPspare As Integer            ' Spare
End Type

Type CITYrecord  'Len=108
  CityName As String * 45
  CityState As String * 2
  CityCountry As String * 45
  CityRegionID As Integer
  CityLatitude As Single
  CityLongitude As Single
  CityAltitude As Integer
  CityPopulation As Long
  CitySpares(1 To 46) As Integer
End Type

Type WMOrecord
  WMOnumber As Long
  WBANnumber As Long
  WMO4l As String * 4
  WMOstation As String * 45
  WMOstate As String * 2
  WMOcountry As String * 45
  WMOregionID As Integer
  WMOlatitude As Single
  WMOlongitude As Single
  WMOaltitude As Integer
End Type

Type RAOBrecord
  WMOnumber As Long
  WMO4l As String * 4
  RAOBedit As Integer
  
  DandT As Date                 'Date and Time in IEEE 64-bit format
  Nlevr As Integer               'Number of ROAB levels
  
  dTMRI As Single               'MRI parameter
  
  Ztrop As Single               'Tropopause pressure altitude (km)
  Ttrop As Single               'Tropopause temperature (K)
  Ltrop As Integer

  ZtropE As Single              'Editted Tropopause pressure altitude (km)
  TtropE As Single              'Editted Tropopause temperature (K)
  LtropE As Integer
  
  ijkl As Integer
  OATraob As Single             'OAT at flight altitude for WCT
  OATraobPP As Single           'PP variation of OAT for 4 WCT RAOBs
  WMOlatitude As Single
  WMOlongitude As Single
  WMOaltitude As Integer
  zMRI1 As Single                'ALT km at dTMRI
  zMRI2 As Single                'ALT km at dTMRI
  
  Ztrop2 As Single               'Second Tropopause pressure altitude (km)
  Ttrop2 As Single               'Second Tropopause temperature (K)
  Ltrop2 As Integer
  IntRhoV As Single              'Integrated water vapor
  Tcoldest As Single             'Coldest temperature in RAOB (K)
  Zcoldest As Single             'Altitude of coldest temperature (km)
  
  nRAOBlevels As Integer         'Number of RAOB levels
  SpareInt As Integer
  Spare(1 To 3) As Single
  
  GZr(1 To 73) As Integer       'Geometric Altitude [m]
  PZr(1 To 73) As Integer       'Pressure altitude  [0.1mb]
  TZr(1 To 73) As Integer       'Temperatures [10 mK]
  RZr(1 To 73) As Integer       'Relative Humidity [0.01%]
  LZr(1 To 3) As Integer        'Editted level number
End Type

Type RAOBrecord2
  WMOnumber As Long
  WMO4l As String * 4
  RAOBedit As Integer
  
  DandT As Date                 'Date and Time in IEEE 64-bit format
  Nlevr As Integer              'Number of ROAB levels
  
  dTMRI As Single               'MRI parameter
  
  Ztrop As Single               'Tropopause pressure altitude (km)
  Ttrop As Single               'Tropopause temperature (K)
  Ltrop As Integer

  ZtropE As Single              'Editted Tropopause pressure altitude (km)
  TtropE As Single              'Editted Tropopause temperature (K)
  LtropE As Integer
  
  ijkl As Integer
  OATraob As Single             'OAT at flight altitude for WCT
  OATraobPP As Single           'PP variation of OAT for 4 WCT RAOBs
  WMOlatitude As Single
  WMOlongitude As Single
  WMOaltitude As Integer
  zMRI1 As Single                'ALT km at dTMRI
  zMRI2 As Single                'ALT km at dTMRI
  
  Ztrop2 As Single               'Second Tropopause pressure altitude (km)
  Ttrop2 As Single               'Second Tropopause temperature (K)
  Ltrop2 As Integer
  IntRhoV As Single              'Integrated water vapor
  Tcoldest As Single             'Coldest temperature in RAOB (K)
  Zcoldest As Single             'Altitude of coldest temperature (km)
  
  nRAOBlevels As Integer         'Number of RAOB levels
  SpareInt As Integer
  Spare(1 To 3) As Single
  
  GZr2(1 To 153) As Integer       'Geometric Altitude [m]
  PZr2(1 To 153) As Integer       'Pressure altitude  [0.1mb]
  TZr2(1 To 153) As Integer       'Temperatures [10 mK]
  RZr2(1 To 153) As Integer       'Relative Humidity [0.01%]
  LZr2(1 To 3) As Integer         'Editted level number
End Type

Type RAOBrecord3
  WMOnumber As Long
  WMO4l As String * 4
  RAOBedit As Integer
  
  DandT As Date                 'Date and Time in IEEE 64-bit format
  Nlevr As Integer              'Number of ROAB levels
  
  dTMRI As Single               'MRI parameter
  
  Ztrop As Single               'Tropopause pressure altitude (km)
  Ttrop As Single               'Tropopause temperature (K)
  Ltrop As Integer

  ZtropE As Single              'Editted Tropopause pressure altitude (km)
  TtropE As Single              'Editted Tropopause temperature (K)
  LtropE As Integer
  
  ijkl As Integer
  OATraob As Single             'OAT at flight altitude for WCT
  OATraobPP As Single           'PP variation of OAT for 4 WCT RAOBs
  WMOlatitude As Single
  WMOlongitude As Single
  WMOaltitude As Integer
  zMRI1 As Single                'ALT km at dTMRI
  zMRI2 As Single                'ALT km at dTMRI
  
  Ztrop2 As Single               'Second Tropopause pressure altitude (km)
  Ttrop2 As Single               'Second Tropopause temperature (K)
  Ltrop2 As Integer
  IntRhoV As Single              'Integrated water vapor
  Tcoldest As Single             'Coldest temperature in RAOB (K)
  Zcoldest As Single             'Altitude of coldest temperature (km)
  
  nRAOBlevels As Integer         'Number of RAOB levels
  SpareInt As Integer
  Spare(1 To 3) As Single
  
  GZr(1 To 503) As Integer       'Geometric Altitude [m]
  PZr(1 To 503) As Integer       'Pressure altitude  [0.1mb]
  TZr(1 To 503) As Integer       'Temperatures [10 mK]
  RZr(1 To 503) As Integer       'Relative Humidity [0.01%]
  LZr(1 To 3) As Integer         'Editted level number
End Type

Type LargeRAOB
  WMOnumber As Long
  WMO4l As String * 4
  RAOBedit As Integer
  
  DandT As Date                 'Date and Time in IEEE 64-bit format
  Nlevr As Integer               'Number of ROAB levels
  
  dTMRI As Single               'MRI parameter
  
  Ztrop As Single               'Tropopause pressure altitude (km)
  Ttrop As Single               'Tropopause temperature (K)
  Ltrop As Integer

  ZtropE As Single              'Editted Tropopause pressure altitude (km)
  TtropE As Single              'Editted Tropopause temperature (K)
  LtropE As Integer
  ijkl As Integer
  OATraob As Single             'OAT at flight altitude for WCT
  OATraobPP As Single           'PP variation of OAT for 4 WCT RAOBs
  WMOlatitude As Single
  WMOlongitude As Single
  WMOaltitude As Integer
  zMRI1 As Single
  zMRI2 As Single
  
  Ztrop2 As Single               'Second Tropopause pressure altitude (km)
  Ttrop2 As Single               'Second Tropopause temperature (K)
  Ltrop2 As Integer
  IntRhoV As Single              'Integrated water vapor
  Tcoldest As Single             'Coldest temperature in RAOB (K)
  Zcoldest As Single             'Altitude of coldest temperature (km)
  
  Spare(1 To 4) As Single
  
  GZr(1 To 2000) As Integer       'Geometric Altitude [m]
  PZr(1 To 2000) As Integer       'Pressure altitude  [0.1mb]
  TZr(1 To 2000) As Integer       'Temperatures [10 mK]
  RZr(1 To 2000) As Integer       'Relative Humidity [0.01%]
End Type

Sub ConvertFH()

End Sub

Sub ConvertWyomingNewRe(INPlu%)
Dim Line$, i%, yy%, i1%, i2%, Status As Boolean, Record&, dH!, Tv1!, Tv2!
'"<HTML>
'<LINK REL=""StyleSheet"" HREF=""/resources/select.css"" TYPE=""text/css"">
'<BODY BGCOLOR=""white"">
'<H2>72493 OAK Oakland Int Observations at 00Z 26 Sep 2001</H2>
'<PRE>
'-----------------------------------------------------------------------------
'   PRES   HGHT   TEMP   DWPT   RELH   MIXR   DRCT   SKNT   THTA   THTE   THTV
'    hPa     m      C      C      %    g/kg    deg   knot     K      K      K
'-----------------------------------------------------------------------------
'         1         2         3         4
'1234567890123456789012345678901234567890
'
' 1014.0      3   22.8   12.8     53   9.24    260      1  294.8  321.6  296.4
' 1000.0    126   19.4   10.4     56   7.97    285      6  292.6  315.6  294.0
'  150.0  14020  -62.7  -69.7     38   0.02    260     57  361.9  362.0  361.9
'  100.0  16510  -63.5  -70.5     38   0.03    250     34  404.8  404.9  404.8
'</PRE><H3>Station information and sounding indices</H3><PRE>
'                         Station identifier: OAK
'                             Station number: 72493
'                           Observation time: 010926/0000
'                           Station latitude: 37.73
'                          Station longitude: -122.21
'                          Station elevation: 3.0
'         1         2         3         4         5         6
'123456789012345678901234567890123456789012345678901234567890
  
  Do
    Line$ = fReadLF(INPlu)
  Loop Until Left$(Line$, 5) = "<PRE>"
  For i = 1 To 4
    Line$ = fReadLF(INPlu)
  Next i
  Do 'Search for first line with valid T and Geopotential Height
    Line$ = fReadLF(INPlu)
  Loop Until (Mid$(Line$, 16, 6) <> "      " And Mid$(Line$, 9, 6) <> "      ") And (Mid$(Line$, 16, 6) <> "-999.0" And Mid$(Line$, 9, 6) <> "-999.0")
'  Line$ = fReadLF(INPlu)
'  If Left$(Line$, 7) = "       " Then Exit Sub     ' Pressure is missing
'  If Val(Mid$(Line$, 9, 6)) < 0 Or Val(Mid$(Line$, 16, 6)) = 0 Then '1000 mb Mandatory level may be underground!
'    Line$ = fReadLF(INPlu)
'    If Left$(Line$, 7) = "       " Then Exit Sub   ' Pressure is missing
'  End If
  Call ReformatLine(Line$)

  PZraob(1) = Val(Left$(Line$, 7))                 ' mb
  ZPraob(1) = fPtoZ(PZraob(1))
  If Mid$(Line$, 9, 6) = "      " Then Exit Sub    ' Geopotential Height is missing
  ZGraob(1) = Val(Mid$(Line$, 9, 6) / 1000#)       ' expects km
  If Mid$(Line$, 16, 6) = "      " Then Exit Sub   ' Temperature is missing
  TZraob(1) = (273.15 + Val(Mid$(Line$, 16, 6)))
  If Mid$(Line$, 33, 6) = "      " Then            ' RH is missing
    RZraob(1) = 0#
    RVraob(1) = 0#
  Else
    RZraob(1) = Val(Mid$(Line$, 33, 3))
    RVraob(1) = fRhoV(RZraob(1), TZraob(1))
  End If
  Nlevr = 1
  Line$ = fReadLF(INPlu)
  Call ReformatLine(Line$)

  Do
    Nlevr = Nlevr + 1
    If Nlevr > 2775 Then
    Debug.Print Line$
      Nlevr = Nlevr
    End If
    If Left$(Line$, 7) = "       " Then Exit Sub     ' Pressure is missing
    PZraob(Nlevr) = Val(Left$(Line$, 7))              ' mb
    ZPraob(Nlevr) = fPtoZ(PZraob(Nlevr))
    If ZPraob(Nlevr) <> ZPraob(Nlevr - 1) Then
      If (Mid$(Line$, 9, 6)) = "      " Then             ' Geopotential Height Missing
        Nlevr = Nlevr - 1
        Exit Do
      Else
        ZGraob(Nlevr) = Val(Mid$(Line$, 9, 6) / 1000#)  ' expects km
      End If
      If Mid$(Line$, 16, 6) = "      " Then Exit Sub   ' Temperature is missing
      TZraob(Nlevr) = (cTo + Val(Mid$(Line$, 16, 6)))
      If Mid$(Line$, 33, 6) = "      " Then            ' RH is missing
        RZraob(Nlevr) = 0#
        RVraob(Nlevr) = 0#
      Else
        RZraob(Nlevr) = Val(Mid$(Line$, 33, 3))
        RVraob(Nlevr) = fRhoV(RZraob(i), TZraob(i))
      End If
'   Debug.Print Nlevr; ZGraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr)
    Else  'Redundant levels cause RC program to crash
      Nlevr = Nlevr - 1
    End If
    Do
      Line$ = fReadLF(INPlu)
    Loop Until Len(Line$) > 0 And InStr(1, Line$, "-999.00") = 0 And InStr(1, Line$, "-999.0") = 0
'   762.3  2415.90  -999.0  -999.0      82 -999   -999   -999   -999   -999   -999
'   500.1  -999.00    -5.9   -19.6      34 -999   -999   -999   -999   -999   -999
'12345678901234567890123456789012345678901234567890123456789012345678901234567890
    If Left$(Line$, 6) <> "</PRE>" Then
      Call ReformatLine(Line$)
    Else
    
    i = i
    End If

  Loop Until Left$(Line$, 6) = "</PRE>" Or Nlevr >= nRAOBlevels Or EOF(INPlu) 'Or Len(Line$) = 0
  
'Calculate Geopotential Height assuming start altitude is start GPS altitude
  ZGraob(1) = ZGraob(1)
  For i = 2 To Nlevr
    Tv1 = fTv(RZraob(i - 1), PZraob(i - 1), TZraob(i - 1))
    Tv2 = fTv(RZraob(i), PZraob(i), TZraob(i))
    dH = fHypsometricEqn(Tv1, PZraob(i - 1), Tv2, PZraob(i))
    ZGraob(i) = ZGraob(i - 1) + dH
  Next i
  
'  If Left$(Line$, 6) <> "</PRE>" Then  'Throw out rest if >70 levels
'    Do: Line$ = fReadLF(INPlu)
'    Loop Until Left$(Line$, 6) = "</PRE>"
'  End If
  
 ' If Mid$(Line$, 4, 5) = "     " Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
 ' Else
 '   Ztrop = fPtoZ(Val(Mid$(Line$, 4, 5)))    'convert mb to km
 '   Ttrop = (273.15 + Val(Mid$(Line$, 15, 6)))
 '   i = 0: Ltrop = 0
 '   Do
 '     i = i + 1
 '     If PZraob(i) <= Val(Mid$(Line$, 4, 5)) Then
 '       Ltrop = i
 '       'Debug.Print Record; i; Nlevr
 '       X = X
 '     End If
 '   Loop Until Ltrop <> 0
 ' End If
  Line$ = fReadLF(INPlu)
  If Mid$(Line$, 26, 7) = "Station" Then
    WMO4l = Trim$(Mid$(Line$, 46, 4))
    Line$ = fReadLF(INPlu)
    WMOnumber = Val(Mid$(Line$, 46, 5))
  Else
    WMOnumber = Val(Mid$(Line$, 46, 5))
    Status = fGetUAsite(Str(WMOnumber), Record)
    If Status And Left$(GPicao, 4) <> "    " Then WMO4l = Left$(GPicao, 4) Else WMO4l = "XXXX"
  End If
  ' Read observation time
  Line$ = fReadLF(INPlu)
  Iyear = Val(Mid$(Line$, 46, 2))
  If Iyear < 50 Then Iyear = 2000 + Iyear Else Iyear = 1900 + Iyear
  Imonth = Val(Mid$(Line$, 48, 2))  '13
  Iday = Val(Mid$(Line$, 50, 2))    '13
  Ihhmm = Mid$(Line$, 53, 4)    '7
  Ihour = Int(Ihhmm / 100)
  Imin = Ihhmm - 100 * Ihour
  
  Line$ = fReadLF(INPlu)
  WMOlatitude = Val(Mid$(Line$, 46, 5))
  Line$ = fReadLF(INPlu)
  WMOlongitude = Val(Mid$(Line$, 46, 7))
  Line$ = fReadLF(INPlu)
  WMOaltitude = Val(Mid$(Line$, 46, 7))
  
  

  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)
'</PRE><H2>Station information and sounding indices</H2><PRE>
'                         Station identifier: VBG
'                             Station number: 72393
'                           Observation time: 010605/1200
'                           Station latitude: 34.75
'                          Station longitude: -120.56
'                          Station elevation: 121.0
'                            Showalter index: 11.50
'                               Lifted index: 15.22
'    LIFT computed using virtual temperature: 15.17
'                                SWEAT index: 67.99
'                                    K index: 4.10
'                         Cross totals index: 5.30
'                      Vertical totals index: 24.30
'                        Totals totals index: 29.60
'      Convective Available Potential Energy: 0.00
'             CAPE using virtual temperature: 0.00
'                      Convective Inhibition: 0.00
'             CINS using virtual temperature: 0.00
'                     Bulk Richardson Number: 0.00
'          Bulk Richardson Number using CAPV: 0.00
'  Temp [K] of the Lifted Condensation Level: 281.43
'Pres [KPa] of the Lifted Condensation Level: 980.92
'     Mean mixed layer potential temperature: 289.34
'              Mean mixed layer mixing ratio: 5.84
'</PRE><P><A HREF="http://www-das.uwyo.edu/cgi-bin/sounding?OUTPUT=OTEXT&TYPE=SKEWT&HOUR=12&DAY=5&MONTH=6&YEAR=2001&STATION=VBG" TARGET="_top">Old format</A>
'                  http://www-das.uwyo.edu/cgi-bin/sounding?OUTPUT=TEXT&TYPE=SKEWT&HOUR=00&DAY=05&MONTH=06&YEAR=2001&STATION=VBG
'                  http://weather.uwyo.edu/cgi-bin/snd.html?YEAR=2001&MONTH=06&DAY=05&HOUR=00&STID=VBG&OUTPUT=TEXT&TYPE=SKEWT



End Sub

Function fGetWMOnumber&(Station$)
Dim Found As Boolean, UArecord&
' Get WMOnumber corresponding to ICAO name

Found = fGetUAsite(Station$, UArecord&)
If Found Then
  fGetWMOnumber = WMOnumber
Else
  MsgBox Station$ + " not found in RAOB Site file!", vbOKOnly
  fGetWMOnumber = 0
End If

End Function

Sub GetCOSMICRangeNGV(FltDate$, Rmax!, OUTlu%, nSites%, ALTkmMin!, ZpStep!, jLast%)
Dim x!, i%, j%, RSLlu%, INPlu%, Status As Boolean, UArecord&, Prefix$, UT&
Const Ncompare = 4000              '300
Static RAOBn%(1 To Ncompare), rLat!(1 To Ncompare), rLon!(1 To Ncompare), BinHere As Boolean
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, T1!, T2!
Dim Range!(1 To Ncompare), r$(1 To Ncompare), UTks!(1 To Ncompare), Rsave!(1 To Ncompare), Found(1 To Ncompare) As Boolean
Dim UTksLast, RangeLast!(1 To Ncompare), ZpSave!(1 To Ncompare), ZpLast!(1 To Ncompare)
Dim P!, T!, Utime&, param!(1 To 75), jOut%, nCOSMIC&, ii&, ymd$
' Trex analysis which didn't have RAW files so could not use Sub GetRAOBrange()
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  If jLast = 0 And Not BinHere Then
    Print #OUTlu, "Number,Year,Month,Day,UT0,LR11,LR12,Zb1,LR21,LR22,Zb2,RAOBs,RAOBs,Ra1,Ra2,Zp(km)"
    BinHere = True
  End If
  If nSites = 0 Then
    RSLlu = FreeFile
    Filename$ = Path$ + "RAOB\" + Mission$ + "_COSMICs.RAOB3"
    Open Filename$ For Random Access Read Write As #RSLlu Len = Len(RAOB3)
    nCOSMIC = LOF(RSLlu) \ Len(RAOB3)
    ' Get Site Latitudes and Longitudes
    i = 0
    For ii = 1 To nCOSMIC
      Call RAOBreadC(RSLlu, ii)
      ymd$ = Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
      If ymd$ = FltDate$ And ZPraob(1) <= ALTkmMin Then
        i = i + 1
        RAOBn(i) = ii
      'Debug.Print i; RAOB$(i); Status
        rLat(i) = WMOlatitude
        rLon(i) = WMOlongitude
      End If
    Next ii
    nSites = i
  End If
  Debug.Print FltDate$; nSites
  
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
    
  Filename$ = Path$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  Record = 0: j = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
    For i = 1 To nSites
      Rcurrent = fGCD(rLat(i), rLon(i), Latitude, Longitude)
      If Rcurrent <= Rmax And Rcurrent >= 0 Then
        If Range(i) = 0 Then             'First time through
          Range(i) = 9999#
          RangeLast(i) = Rcurrent
          ZpLast(i) = ALTkm
          UTks(i) = UTsec / 1000#
        Else                             'Check if closer range
          If Rcurrent < Range(i) And Rcurrent < RangeLast(i) Then  'Yes
            Range(i) = Rcurrent
            ZpLast(i) = ALTkm
            UTksLast = UTsec / 1000#
            Found(i) = False
          Else                           'No, Save closest approach
            If Not Found(i) Then
              j = j + 1
              r$(j) = "C" + Format(RAOBn(i), "0000")
              UTks(j) = UTksLast
              Rsave(j) = RangeLast(i)
              ZpSave(j) = ZpLast(i)
              Range(i) = 0#              'Start over with this site
              Found(i) = True
            End If
          End If
          RangeLast(i) = Rcurrent
        End If
        'Debug.Print RAOB$(i); Rcurrent; UTsec
      End If
    Next i
    DoEvents
  Loop Until EOF(REFlu)
  Close REFlu
  
' Print out closest approach list
  jOut = 0
  For i = 1 To j
    If ZpSave(i) >= Val(ALTkmMin) And Rsave(i) < Rmax Then
      jOut = jOut + 1
      'If UTks(i) > 86.4 Then UTks(i) = UTks(i) - 86.4
      If OUTlu <> 0 Then Print #OUTlu, Format(jLast + jOut, "000") + "," + yy$ + "," + mm$ + "," + DD$ + "," + Format(UTks(i), "000.00") + "," + "0,0,0,0,0,0" + "," + r$(i) + "," + r$(i) + "," + Format(Rsave(i), "000.0") + "," + Format(Rsave(i), "000.0") + "," + Format(ZpSave(i), "000.0")
      'Debug.Print yy$ + vbTab + mm$ + vbTab + DD$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + r$(i) + vbTab + r$(i) + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(ZpSave(i), "000.0")
    End If
  Next i
  jLast = jLast + jOut


End Sub

Sub GetRAOBrangeNGV(FltDate$, Rmax!, OUTlu%, nSites%, ALTkmMin!, ZpStep!, jLast%)
Dim x!, i%, j%, RSLlu%, INPlu%, Status As Boolean, UArecord&, Prefix$, UT&
Const Ncompare = 300
Static RAOB$(1 To Ncompare), rLat!(1 To Ncompare), rLon!(1 To Ncompare)
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, T1!, T2!
Dim Range!(1 To Ncompare), r$(1 To Ncompare), UTks!(1 To Ncompare), Rsave!(1 To Ncompare), Found(1 To Ncompare) As Boolean
Dim UTksLast, RangeLast!(1 To Ncompare), ZpSave!(1 To Ncompare), ZpLast!(1 To Ncompare)
Dim P!, T!, Utime&, param!(1 To 75), jOut%
' Trex analysis which didn't have RAW files so could not use Sub GetRAOBrange()
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  If nSites = 0 Then
    RSLlu = FreeFile
    Filename$ = Path$ + "RAOB\" + Mission$ + "_RAOBrange.RSL"
    Open Filename$ For Input As RSLlu
    Do
      nSites = nSites + 1
      Input #RSLlu, RAOB$(nSites)
    Loop Until EOF(RSLlu)
    Close RSLlu
    ' Get Site Latitudes and Longitudes
    For i = 1 To nSites
      Status = fGetUAsite(RAOB$(i), UArecord&)
      'Debug.Print i; RAOB$(i); Status
      rLat(i) = GPlatitude
      rLon(i) = GPlongitude
    Next i
    If jLast = 0 Then Print #OUTlu, "Number,Year,Month,Day,UT0,LR11,LR12,Zb1,LR21,LR22,Zb2,RAOBs,RAOBs,Ra1,Ra2,Zp(km)"
  End If
  
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
    
  Filename$ = Path$ + "NG\NG" + FltDate$ + ".asc"
  Record = 0: j = 0
  Quit = False
  Do
    Record = Record + 1
    ' UTsec=-1 to get next record rather than try to sync
    Call GetNAVvalues("NG", -1, UT&, P!, T!, param!(), Filename$, Quit)
    If Quit Then
      Exit Do
    End If
    UTsec = UT
    Latitude = param(5)
    Longitude = param(6)
    ALTkm = param(9)
    For i = 1 To nSites
      Rcurrent = fGCD(rLat(i), rLon(i), Latitude, Longitude)
      If Rcurrent <= Rmax Then
        If Range(i) = 0 Then             'First time through
          Range(i) = 9999#
          RangeLast(i) = Rcurrent
          ZpLast(i) = ALTkm
        Else                             'Check if closer range
          If Rcurrent < Range(i) And Rcurrent < RangeLast(i) Then  'Yes
            Range(i) = Rcurrent
            ZpLast(i) = ALTkm
            UTksLast = UTsec / 1000#
            Found(i) = False
          Else                           'No, Save closest approach
            If Not Found(i) Then
              j = j + 1
              r$(j) = RAOB$(i)
              UTks(j) = UTksLast
              Rsave(j) = RangeLast(i)
              ZpSave(j) = ZpLast(i)
              Range(i) = 0#              'Start over with this site
              Found(i) = True
            End If
          End If
          RangeLast(i) = Rcurrent
        End If
        'Debug.Print RAOB$(i); Rcurrent; UTsec
      End If
    Next i
    DoEvents
  Loop Until 1 = 2

  
' Print out closest approach list
  jOut = 0
  For i = 1 To j
    If ZpSave(i) >= Val(ALTkmMin) And Rsave(i) < Rmax Then
      jOut = jOut + 1
      If OUTlu <> 0 Then Print #OUTlu, Format(jLast + jOut, "000") + "," + yy$ + "," + mm$ + "," + DD$ + "," + Format(UTks(i), "000.00") + "," + "0,0,0,0,0,0" + "," + r$(i) + "," + r$(i) + "," + Format(Rsave(i), "000.0") + "," + Format(Rsave(i), "000.0") + "," + Format(ZpSave(i), "000.0")
      'Debug.Print yy$ + vbTab + mm$ + vbTab + DD$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + r$(i) + vbTab + r$(i) + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(ZpSave(i), "000.0")
    End If
  Next i
  jLast = jLast + jOut


End Sub
Sub RAOBextension(TZraob!(), ZPraob!(), Nlevr%, LR1!, zLRb!, LR2!, Ztop!)
Dim j%, slope!
      
  If ZPraob(Nlevr) > zLRb And ZPraob(Nlevr) <= Ztop And Nlevr < nRAOBlevelsOUT Then
  ' zLRb < ZPraob(Nlevr) <= Ztop
    If ZPraob(Nlevr) < Ztop Then
      For j = 3 To 1 Step -1
        TZraob(Nlevr + j + 1) = TZraob(Nlevr + j)
        ZPraob(Nlevr + j + 1) = ZPraob(Nlevr + j)
        PZraob(Nlevr + j + 1) = PZraob(Nlevr + j)
      Next j
      TZraob(Nlevr + 1) = TZraob(Nlevr) + LR2 * (Ztop - ZPraob(Nlevr))
      ZPraob(Nlevr + 1) = Ztop
      PZraob(Nlevr + 1) = fZtoP(Ztop)
      RZraob(Nlevr + 1) = 0#    'Make 0 at 50 km to avoid fET overflow
      ZGraob(Nlevr + 1) = ZGraob(Nlevr) + fHypsometricEqn(TZraob(Nlevr), PZraob(Nlevr), TZraob(Nlevr + 1), PZraob(Nlevr + 1))
      Nlevr = Nlevr + 1
    End If
  Else
    If Nlevr < nRAOBlevels - 1 Then
      For j = 3 To 1 Step -1
        TZraob(Nlevr + j + 2) = TZraob(Nlevr + j)
        ZPraob(Nlevr + j + 2) = ZPraob(Nlevr + j)
        PZraob(Nlevr + j + 2) = PZraob(Nlevr + j)
      Next j
      TZraob(Nlevr + 1) = TZraob(Nlevr) + LR1 * (zLRb - ZPraob(Nlevr))
      ZPraob(Nlevr + 1) = zLRb
      PZraob(Nlevr + 1) = fZtoP(zLRb)
      ZGraob(Nlevr + 1) = ZGraob(Nlevr) + fHypsometricEqn(TZraob(Nlevr), PZraob(Nlevr), TZraob(Nlevr + 1), PZraob(Nlevr + 1))
      TZraob(Nlevr + 2) = TZraob(Nlevr + 1) + LR2 * (Ztop - ZPraob(Nlevr + 1))
      ZPraob(Nlevr + 2) = Ztop
      PZraob(Nlevr + 2) = fZtoP(Ztop)
      ZGraob(Nlevr + 2) = ZGraob(Nlevr + 1) + fHypsometricEqn(TZraob(Nlevr + 1), PZraob(Nlevr + 1), TZraob(Nlevr + 2), PZraob(Nlevr + 2))
      Nlevr = Nlevr + 2
    End If
  End If
  
  If TZraob(Nlevr) > 275 Then
    ZPraob(Nlevr + 1) = ZPraob(Nlevr)
    PZraob(Nlevr + 1) = PZraob(Nlevr)
    RZraob(Nlevr + 1) = 0  'RZraob(Nlevr)
    ZGraob(Nlevr + 1) = ZGraob(Nlevr)
    TZraob(Nlevr + 1) = 275
    slope = (TZraob(Nlevr) - TZraob(Nlevr - 1)) / (ZPraob(Nlevr) - ZPraob(Nlevr - 1))
    If slope > 0 Then ZPraob(Nlevr) = ZPraob(Nlevr - 1) + (275 - TZraob(Nlevr - 1)) / slope
    PZraob(Nlevr) = fZtoP(ZPraob(Nlevr))
     slope = (TZraob(Nlevr) - TZraob(Nlevr - 1)) / (ZGraob(Nlevr) - ZGraob(Nlevr - 1))
    If slope > 0 Then ZGraob(Nlevr) = ZGraob(Nlevr - 1) + (275 - TZraob(Nlevr - 1)) / slope
    TZraob(Nlevr) = 275
    Nlevr = Nlevr + 1
  End If

End Sub

Function fRZraob(zo)
' Do linear interpolation in z to find value of at zo
Dim i%, it%, ib%

i = 0
Do
  i = i + 1
Loop Until ZPraob(i) >= zo Or i = Nlevr
If ZPraob(i) = zo Then fRZraob = RZraob(i): Exit Function
If i > Nlevr Then fRZraob = RZraob(Nlevr): Exit Function
it = i
ib = i - 1
If ib = 0 Then
  fRZraob = RZraob(it)
Else
  fRZraob = RZraob(ib) + (RZraob(it) - RZraob(ib)) * (zo - ZPraob(ib)) / (ZPraob(it) - ZPraob(ib))
End If
End Function


Sub CITYread(lu%, Record&)
Dim a As CITYrecord, i%

  Get #lu%, Record, a
    
  CityName = Trim(a.CityName)
  CityState = Trim(a.CityState)
  CityCountry = Trim(a.CityCountry)
  CityRegionID = a.CityRegionID
  CityLatitude = a.CityLatitude
  CityLongitude = a.CityLongitude
  CityAltitude = a.CityAltitude
  CityPopulation = a.CityPopulation
    
End Sub

Sub CITYwrite(lu%, Record&)
Dim a As CITYrecord

  a.CityName = CityName
  a.CityState = CityState
  a.CityCountry = CityCountry
  a.CityRegionID = CityRegionID
  a.CityLatitude = CityLatitude
  a.CityLongitude = CityLongitude
  a.CityAltitude = CityAltitude
  a.CityPopulation = CityPopulation
  
  Put #lu%, Record, a
End Sub


Sub ConvertAndros(INPlu%)
Dim i%, a$, Line$, x$
'CAMEX-4 Program / Campaign Radiosonde Output Data
'-------------------------------------------------------------------------------------------------------
'Location:  Andros Island, the Bahamas
'           Latitude:    24 deg. 46.02 min. North
'           Longitude:  077 deg. 50.58 min. West
'Radiosonde Type: Mark II        GPS(PVT)          Serial No.:  02356056
'CAMEX-4 10-second, standard pressure level, and precipitable water output report file format
'-------------------------------------------------------------------------------------------------------
'**********   PRELIMINARY DATA ONLY!!!!    ***********
'CAUTION! relative humidity parameters at temperatures less than -40 degrees Celsius may not be valid!
'
'-------------------------------------------------------------------------------------------------------
'Flight Date:  08/17/2001    Time:  07:42:58 UTC
'-------------------------------------------------------------------------------------------------------
'CAMEX-4 Program / 10-second data levels
'     Geop                      Rel     Dew     Mixng      Spec  Wind   Wind   Wind   Wind   Rise  Elapsed
'   Height    Press    Temp     Hum   Point     Ratio       Hum   Dir  Speed    E-W    N-S   Rate     Time
'   meters      hPa   deg C     pct   deg C      g/kg      g/kg   deg    m/s    m/s    m/s    m/s  seconds
'     3.00  1019.60   27.99   82.20   24.67   19.5437   18.9483    96    3.6   -3.6    0.4    0.0      0.0
'    66.37  1012.30   28.18   82.17   24.85   19.9056   19.2884   103    6.3   -6.2    1.4    6.3     10.0
'   126.60  1005.40   27.70   83.28   24.60   19.7492   19.1414   103    6.3   -6.2    1.4    6.2     20.0
'
' 24468.82    28.23  -54.50    5.27  -75.52    0.0412    0.0412  -999 -999.9 -999.9 -999.9    6.1   4630.0
' 24534.91    27.94  -54.79    5.24  -75.76    0.0399    0.0399  -999 -999.9 -999.9 -999.9    6.2   4640.0
'
'-------------------------------------------------------------------------------------------------------
'CAMEX-4 Program / Standard Pressure Levels
'00000000011111111112222222222333333333344444444444555555555666666666677777777778888888888999999999900000000001
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
' 24468.82    28.23  -54.50    5.27  -75.52    0.0412    0.0412  -999 -999.9 -999.9 -999.9    6.1   4630.0
'-99999.99 -9999.99 -999.99 -999.99 -999.99 -999.9999 -999.9999    99   10.0   -9.9    1.6  -99.9   2830.0
'-99999.99    98.17  -69.18   30.09  -76.48    0.0095    0.0095   103    9.7   -9.5    2.2  -99.9   2840.0
  
  
  WMOlatitude = 24.767
  WMOlongitude = 77.843
  WMOaltitude = 0.003
  WMOnumber = 99999
  WMO4l = "ANDR"
  Do:  a$ = fReadLF(INPlu): Loop Until Left$(a$, 6) = "Flight"
  Iyear = Mid$(a$, 21, 4)
  Imonth = Mid$(a$, 15, 2)
  Iday = Mid$(a$, 18, 2)
  Ihour = Mid$(a$, 36, 2)
  Imin = Mid$(a$, 39, 2)
  Isec = Mid$(a$, 42, 2)

  Do:  a$ = fReadLF(INPlu): Loop Until Left$(a$, 9) = "   meters"
  
  Line$ = fReadLF(INPlu)
  ZGraob(1) = Val(Left$(Line$, 9)) / 1000#           ' km
  PZraob(1) = Val(Mid$(Line$, 11, 8))                ' mb
  TZraob(1) = Val(Mid$(Line$, 20, 7)) + cTo          ' K
  RZraob(1) = Val(Mid$(Line$, 28, 7))                ' RH %
  Nlev = 1
  Line$ = fReadLF(INPlu)
  Do
    Nlev = Nlev + 1
    x$ = Left$(Line$, 9)
    If x$ = "-99999.99" Then Nlev = Nlev - 1: GoTo SkipLine
    ZGraob(Nlev) = Val(x$) / 1000#      ' km
    x$ = Mid$(Line$, 11, 8)
    If x$ = "-9999.99" Then Nlev = Nlev - 1: GoTo SkipLine
    PZraob(Nlev) = Val(x$)           ' mb
    x$ = Mid$(Line$, 20, 7)
    If x$ = "-999.99" Then Nlev = Nlev - 1: GoTo SkipLine
    TZraob(Nlev) = Val(x$) + cTo     ' K
    x$ = Mid$(Line$, 28, 7)
    If x$ = "-999.99" Then Nlev = Nlev - 1: GoTo SkipLine
    RZraob(Nlev) = Val(Mid$(Line$, 28, 7))           ' RH %
SkipLine:
    Line$ = fReadLF(INPlu)
    'Debug.Print Line$
  Loop Until Len(Line$) < 10
  If Nlev > nRAOBlevels Then RAOBtype = 2 Else RAOBtype = 1

End Sub

Sub ConvertCLS5(INPlu%)
Dim a$, x$
  Line Input #INPlu, a$
'Data Type:                         AVAPS SOUNDING DATA, Channel 1
  Line Input #INPlu, a$
'Project ID:                        CAMEX 4, 10406
  Line Input #INPlu, a$
'Launch Site Type/Site ID:          NASA DC-8,
  Line Input #INPlu, a$
'Launch Location (lon,lat,alt):     77 35.30'W, 24 44.90'N, -77.58833, 24.74833, 11855
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
  WMOlongitude = Val(Mid$(a$, 36, 2)) + Val(Mid$(a$, 39, 5)) / 60#
  If Mid$(a$, 45, 1) = "W" Then WMOlongitude = -WMOlongitude
  WMOlatitude = Val(Mid$(a$, 48, 2)) + Val(Mid$(a$, 51, 5)) / 60#
  If Mid$(a$, 57, 1) = "S" Then WMOlatitude = -WMOlatitude
  WMOaltitude = 0#
  WMOnumber = 99999
  WMO4l = "DROP"
      
  Line Input #INPlu, a$
'GMT Launch Time (y,m,d,h,m,s):     2001, 08, 18, 18:49:32
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
  Iyear = Mid$(a$, 36, 4)
  Imonth = Mid$(a$, 42, 2)
  Iday = Mid$(a$, 46, 2)
  Ihour = Mid$(a$, 50, 2)
  Imin = Mid$(a$, 53, 2)
  Isec = Mid$(a$, 56, 2)
    
  Line Input #INPlu, a$
'Sonde Id:                          012025014
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'System Operator/Comments:          ,
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  WMO4l = "DROP"

  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
'  TXTfile$ = UCase$(Right(RAOBfile$, 3)) + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihour, "00") + ".txt"
  Line Input #INPlu, a$
' Time  Press  Temp  Dewpt  RH    Uwind  Vwind  Wspd  Dir   dZ      Lon     Lat    Rng   Az     Alt    Qp   Qt   Qh   Qu   Qv   Quv
  Line Input #INPlu, a$
'  sec    mb     C     C     %     m/s    m/s   m/s   deg   m/s     deg     deg     km   deg     m     mb   C    %    m/s  m/s  m/s
  Line Input #INPlu, a$
'------ ------ ----- ----- ----- ------ ------ ----- ----- ----- -------- ------- ----- ----- ------- ---- ---- ---- ---- ---- ----
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
' 783.8 1016.9  29.5  25.5  79.0  999.0  999.0 999.0 999.0  99.0  999.000 999.000 999.0 999.0     0.0 99.0 99.0 99.0 99.0 99.0 99.0
' 783.3 1016.2  29.5  25.5  79.0  999.0  999.0 999.0 999.0 -10.6  999.000 999.000 999.0 999.0     5.7 99.0 99.0 99.0 99.0 99.0 99.0
' 782.8 1015.6  29.4  25.5  79.1  999.0  999.0 999.0 999.0 -10.6  999.000 999.000 999.0 999.0    11.0 99.0 99.0 99.0 99.0 99.0 99.0
' 782.3 1015.0  29.4  25.5  79.2  999.0  999.0 999.0 999.0 -10.5  999.000 999.000 999.0 999.0    16.3 99.0 99.0 99.0 99.0 99.0 99.0
' 781.8 1014.4  29.3  25.4  79.2  999.0  999.0 999.0 999.0 -10.5  999.000 999.000 999.0 999.0    21.6 99.0 99.0 99.0 99.0 99.0 99.0
' 781.3 1013.8  29.3  25.3  79.0   -5.0   -1.7   5.2  71.2 -10.4  -77.598  24.755 999.0 999.0    26.8 99.0 99.0 99.0 99.0 99.0 99.0
'
'   0.3 9999.0 999.0 999.0 999.0  999.0  999.0 999.0 999.0  99.0  999.000 999.000 999.0 999.0 99999.0 99.0 99.0 99.0 99.0 99.0 99.0
'   0.0  216.5 -49.8 -56.9  45.1   -3.2    5.9   6.7 151.0  99.0  -77.588  24.748 999.0 999.0 11950.5 99.0 99.0 99.0 99.0 99.0 99.0

  Nlev = 0
  Do
    Line Input #INPlu, a$
    'Debug.Print a$
    DoEvents
    Nlev = Nlev + 1
    x$ = Mid$(a$, 8, 6)
    If x$ = "9999.0" Or a$ = "" Then Nlev = Nlev - 1: GoTo SkipRAOB
    PZraob(Nlev) = Val(x$)
    ZPraob(Nlev) = fPtoZ(PZraob(Nlev))
    x$ = Mid$(a$, 15, 5)
    If x$ = "999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    TZraob(Nlev) = Val(x$) + cTo
    'Debug.Print i; Val(x$); TZraob(i)
'    x$ = Mid$(a$, 21, 5)
'    If x$ = "999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    x$ = Mid$(a$, 94, 7)  'Zh
    If x$ = "99999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    ZGraob(Nlev) = Val(x$) / 1000#
    x$ = Mid$(a$, 27, 5)  'RH
    If x$ = "999.0" Then RZraob(Nlev) = 0# Else RZraob(Nlev) = Val(x$)
SkipRAOB:
  Loop Until EOF(INPlu)
  
  If Nlev > nRAOBlevels Then RAOBtype = 2 Else RAOBtype = 1
  'Call Zhme(PZraob(), TZraob(), RZraob(), 0, Nlev, ZGraob())
End Sub

Function ConvertGZtoInteger(x!) As Integer
' X in km
If x > 32.767 Then
  ConvertGZtoInteger = -Int(1000 * (x - 32.767) + 0.5)
Else
  ConvertGZtoInteger = Int(1000 * x + 0.5)
End If
End Function

Function ConvertIntegerToGZ(x%) As Single

If x < 0 Then
  ConvertIntegerToGZ = CSng(-x / 1000# + 32.767)
Else
  ConvertIntegerToGZ = CSng(x / 1000#)
End If

End Function

Function ConvertIntegertoZP(x%) As Single

If x < 0 Then
  ConvertIntegertoZP = CSng(-x / 1000# + 32.767) - 1
Else
  ConvertIntegertoZP = CSng(x / 1000#) - 1
End If

End Function


Sub ConvertNCAR()
Dim x$

'      35    1001
'Miloshevich , Larry
'National Center for Atmospheric Research (NCAR); milo@ucar.edu
'RADIOSONDE DATA >>> Vaisala RH is CORRECTED for sensor time-lag and bias errors
'CRYSTAL -FACE
'       1       1
'2002   7   5          2002   7   5
'6
'Elapsed time (seconds) from launch_time
'8
'     1.0     1.0     1.0     1.0     1.0     1.0     1.0     1.0
'    999.  99999.   9999.    999.    999.    999.    999.    999.
'T:  Ambient Temperature(c)
'Z:  Altitude (M)
'P:  Pressure (mb)
'RHorig:  Uncorrected (original) Relative Humidity wrt water (%RH)
'RHsm:  Smoothed version of RHorig (needed for time-lag correction)
'RHfinal:  RHsm corrected for sensor time-lag and bias errors
'Wspd:  Wind Speed (m/s)   (CONVERTED FROM KNOTS)
'Wdir:  Wind Direction(degrees)
'10
'; station_name: Key West, FL
'; launch_time: 11:07:00 GMT
'; station lat/lon/alt(m):   24.500   81.800    2.000
'; radiosonde manufacturer: VIZ B2
'; humidity sensor type: hygristor
'; serial-number: 84883594.CSN
'; radiosonde age (yrs): -999.000
'; source filetype: nws_stor
'; source filenames:  kw20020705_1107_id372.corr  EYW372R.TXT
'; RH corrections for RHfinal: none
'3
'; ONLY VAISALA RADIOSONDE RH DATA ARE CORRECTED (not VIZ)
'; SEE ~/docs/crystal_radiosonde_data.txt FOR INFO ABOUT THE RH CORRECTIONS
'   time       T        Z        P  RHorig    RHsm RHfinal    Wspd    Wdir
'    0.0   27.40      2.0  1016.40    78.0  999.00   78.00    1.54   160.0
'    6.0   27.80     20.0  1014.40    70.8  999.00   70.80  999.00   999.0
'   12.0   27.80     60.0  1009.80    71.3  999.00   71.30    0.00     0.0
'   18.0   27.60     88.0  1006.60    72.5  999.00   72.50  999.00   999.0
'   24.0   27.30    113.0  1003.80    73.3  999.00   73.30  999.00   999.0
Dim a$, i%
  For i = 1 To 7: Line Input #INPlu, a$: Next i
'2002   7   5          2002   7   5
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
  Iyear = Left$(a$, 4)
  Imonth = Mid$(a$, 7, 2)
  Iday = Mid$(a$, 11, 2)
  Do
    Line Input #INPlu, a$
  Loop Until Left$(a$, 16) = "; station_name:"
'; station_name: Key West, FL
  Do
    Select Case Left$(a$, 8)
    Case "; launch"
    Case "; statio"
    End Select
  Loop Until Left$(a$, 1) <> ";"
'Project ID:                        CAMEX 4, 10406
  Line Input #INPlu, a$
'Launch Site Type/Site ID:          NASA DC-8,
  Line Input #INPlu, a$
'Launch Location (lon,lat,alt):     77 35.30'W, 24 44.90'N, -77.58833, 24.74833, 11855
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
  WMOlongitude = Val(Mid$(a$, 36, 2)) + Val(Mid$(a$, 39, 5)) / 60#
  If Mid$(a$, 45, 1) = "W" Then WMOlongitude = -WMOlongitude
  WMOlatitude = Val(Mid$(a$, 48, 2)) + Val(Mid$(a$, 51, 5)) / 60#
  If Mid$(a$, 57, 1) = "S" Then WMOlatitude = -WMOlatitude
  WMOaltitude = 0#
  WMOnumber = 99999
  WMO4l = "DROP"
      
  Line Input #INPlu, a$
'GMT Launch Time (y,m,d,h,m,s):     2001, 08, 18, 18:49:32
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
  Iyear = Mid$(a$, 36, 4)
  Imonth = Mid$(a$, 42, 2)
  Iday = Mid$(a$, 46, 2)
  Ihour = Mid$(a$, 50, 2)
  Imin = Mid$(a$, 53, 2)
  Isec = Mid$(a$, 56, 2)
    
  Line Input #INPlu, a$
'Sonde Id:                          012025014
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'System Operator/Comments:          ,
  Line Input #INPlu, a$
'/
  Line Input #INPlu, a$
'/
  WMO4l = "DROP"

  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
'  TXTfile$ = UCase$(Right(RAOBfile$, 3)) + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihour, "00") + ".txt"
  Line Input #INPlu, a$
' Time  Press  Temp  Dewpt  RH    Uwind  Vwind  Wspd  Dir   dZ      Lon     Lat    Rng   Az     Alt    Qp   Qt   Qh   Qu   Qv   Quv
  Line Input #INPlu, a$
'  sec    mb     C     C     %     m/s    m/s   m/s   deg   m/s     deg     deg     km   deg     m     mb   C    %    m/s  m/s  m/s
  Line Input #INPlu, a$
'------ ------ ----- ----- ----- ------ ------ ----- ----- ----- -------- ------- ----- ----- ------- ---- ---- ---- ---- ---- ----
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
' 783.8 1016.9  29.5  25.5  79.0  999.0  999.0 999.0 999.0  99.0  999.000 999.000 999.0 999.0     0.0 99.0 99.0 99.0 99.0 99.0 99.0
' 783.3 1016.2  29.5  25.5  79.0  999.0  999.0 999.0 999.0 -10.6  999.000 999.000 999.0 999.0     5.7 99.0 99.0 99.0 99.0 99.0 99.0
' 782.8 1015.6  29.4  25.5  79.1  999.0  999.0 999.0 999.0 -10.6  999.000 999.000 999.0 999.0    11.0 99.0 99.0 99.0 99.0 99.0 99.0
' 782.3 1015.0  29.4  25.5  79.2  999.0  999.0 999.0 999.0 -10.5  999.000 999.000 999.0 999.0    16.3 99.0 99.0 99.0 99.0 99.0 99.0
' 781.8 1014.4  29.3  25.4  79.2  999.0  999.0 999.0 999.0 -10.5  999.000 999.000 999.0 999.0    21.6 99.0 99.0 99.0 99.0 99.0 99.0
' 781.3 1013.8  29.3  25.3  79.0   -5.0   -1.7   5.2  71.2 -10.4  -77.598  24.755 999.0 999.0    26.8 99.0 99.0 99.0 99.0 99.0 99.0
'
'   0.3 9999.0 999.0 999.0 999.0  999.0  999.0 999.0 999.0  99.0  999.000 999.000 999.0 999.0 99999.0 99.0 99.0 99.0 99.0 99.0 99.0
'   0.0  216.5 -49.8 -56.9  45.1   -3.2    5.9   6.7 151.0  99.0  -77.588  24.748 999.0 999.0 11950.5 99.0 99.0 99.0 99.0 99.0 99.0

  Nlev = 0
  Do
    Line Input #INPlu, a$
    'Debug.Print a$
    DoEvents
    Nlev = Nlev + 1
    x$ = Mid$(a$, 8, 6)
    If x$ = "9999.0" Or a$ = "" Then Nlev = Nlev - 1: GoTo SkipRAOB
    PZraob(Nlev) = Val(x$)
    ZPraob(Nlev) = fPtoZ(PZraob(Nlev))
    x$ = Mid$(a$, 15, 5)
    If x$ = "999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    TZraob(Nlev) = Val(x$) + cTo
    'Debug.Print i; Val(x$); TZraob(i)
'    x$ = Mid$(a$, 21, 5)
'    If x$ = "999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    x$ = Mid$(a$, 94, 7)  'Zh
    If x$ = "99999.0" Then Nlev = Nlev - 1: GoTo SkipRAOB
    ZGraob(Nlev) = Val(x$) / 1000#
    x$ = Mid$(a$, 27, 5)  'RH
    If x$ = "999.0" Then RZraob(Nlev) = 0# Else RZraob(Nlev) = Val(x$)
SkipRAOB:
  Loop Until EOF(INPlu)
  
  If Nlev > nRAOBlevels Then RAOBtype = 2 Else RAOBtype = 1
  'Call Zhme(PZraob(), TZraob(), RZraob(), 0, Nlev, ZGraob())
End Sub

Sub ConvertPNNL(INPlu%)
Dim zo!, Line$
'1234567890123456789012345
'HSE  1999 09 14 13 58 043
'pALT  T(K)   RH(%)
'00.00 294.61  94.6
'00.08 286.28  86.3
'00.15 285.96  86.0
 
  Line$ = fReadLF(INPlu)
  
  WMO4l = Trim$(Left$(Line$, 4))
  Iyear = Val(Mid$(Line$, 6, 4))
  Imonth = Val(Mid$(Line$, 11, 2))
  Iday = Mid$(Line$, 14, 2)
  Ihour = Mid$(Line$, 17, 2)
  Imin = Mid$(Line$, 20, 2)
  Nlev = Mid$(Line$, 23, 3)
  dTMRI = Mid$(Line$, 27, 4)

  Line$ = fReadLF(INPlu)
  If WMO4l = "HOU " Then  'Coordinates from John Neilsen-Gammon
    WMOnumber = 99999
    WMOlatitude = 29.77
    WMOlongitude = 95.36
    zo = 0.014
  Else  'HSE
    WMOnumber = 99999
    WMOlatitude = 29.39
    WMOlongitude = 95.5
    zo = 0.005
  End If
  Line$ = fReadLF(INPlu)
  ZPraob(1) = Val(Mid$(Line$, 1, 5))               ' mb
  PZraob(1) = fZtoP(ZPraob(1))               ' mb
'  ZGraob(1) = Val(Mid$(Line$, 10, 5) / 1000#)       ' expects km
  If Mid$(Line$, 6, 1) = " " Then
    TZraob(1) = Val(Mid$(Line$, 7, 6))
    RZraob(1) = Val(Mid$(Line$, 14, 5))
  Else
    TZraob(1) = Val(Mid$(Line$, 6, 6))
    RZraob(1) = Val(Mid$(Line$, 13, 5))
  End If
  RVraob(1) = fRhoV(RZraob(1), TZraob(1))
  Nlev = 1
  Line$ = fReadLF(INPlu)
  Do
    Nlev = Nlev + 1
    ZPraob(Nlev) = Val(Mid$(Line$, 1, 5))               ' mb
    PZraob(Nlev) = fZtoP(ZPraob(Nlev))
    If ZPraob(Nlev) > ZPraob(Nlev - 1) Then
      If Mid$(Line$, 6, 1) = " " Then
        TZraob(Nlev) = Val(Mid$(Line$, 7, 6))
        RZraob(Nlev) = Val(Mid$(Line$, 14, 5))
      Else
        TZraob(Nlev) = Val(Mid$(Line$, 6, 6))
        RZraob(Nlev) = Val(Mid$(Line$, 13, 5))
      End If
      RVraob(Nlev) = fRhoV(RZraob(Nlev), TZraob(Nlev))
'   Debug.Print Nlev; ZGraob(Nlev); PZraob(Nlev); TZraob(Nlev)
    Else  'Redundant levels cause RC program to crash
      Nlev = Nlev - 1
    End If
    Line$ = fReadLF(INPlu)
  Loop Until EOF(INPlu)
    
  Nlev = Nlev + 1
  ZPraob(Nlev) = Val(Mid$(Line$, 1, 5))               ' mb
  PZraob(Nlev) = fZtoP(ZPraob(Nlev))
  If ZPraob(Nlev) > ZPraob(Nlev - 1) Then
    TZraob(Nlev) = Val(Mid$(Line$, 7, 6))
    RZraob(Nlev) = Val(Mid$(Line$, 14, 5))
    RVraob(Nlev) = fRhoV(RZraob(Nlev), TZraob(Nlev))
  Else  'Redundant levels cause RC program to crash
    Nlev = Nlev - 1
  End If

  Call Zhme(PZraob(), TZraob(), RZraob(), zo, Nlev, ZGraob())
  
'  If Mid$(Line$, 4, 5) = "     " Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
'  Else
'    Ztrop = fPtoZ(Val(Mid$(Line$, 4, 5)))    'convert mb to km
'    Ttrop = (273.15 + Val(Mid$(Line$, 15, 6)))
'    i = 0: Ltrop = 0
'    Do
'      i = i + 1
'      If PZraob(i) <= Val(Mid$(Line$, 4, 5)) Then
'        Ltrop = i
'        'Debug.Print Record; i; Nlev
'      End If
'    Loop Until Ltrop <> 0
'  End If
  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlev, 0, False)


End Sub

Function ConvertZPtoInteger(x!) As Integer
' X in km
x = x + 1 'Offset 1 km to allow for high pressure (1080 hPa = -.5 km)
If x > 65.6 Then x = 1
If x > 32.767 Then
  ConvertZPtoInteger = -Int(1000 * (x - 32.767) + 0.5)
Else
  ConvertZPtoInteger = Int(1000 * x + 0.5)
End If

End Function

Sub ConvertTAMU(INPlu%)
Dim Line$, zo!
' SNPARM = PRES;HGHT;TMPK;RELH
'
'
'123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6
' STID = ELL           STNM =        1   TIME = 000901/1100
' SLAT =  29.61     SLON =   -95.17   SELV =    10.0
'
'
'      PRES     HGHT     TMPK     RELH
'   1009.76    24.99   302.11    71.13
'   1004.57    71.03   301.68    72.34
'
'    135.21 14888.93   205.24    32.01
'123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6
  Do
    Line$ = fReadLF(INPlu)
  Loop Until Mid$(Line$, 2, 4) = "STID"
  WMO4l = Trim$(Mid$(Line$, 9, 4))
  Iyear = Val(Mid$(Line$, 48, 2)) + 2000
  Imonth = Val(Mid$(Line$, 50, 2))
  Iday = Mid$(Line$, 52, 2)
  Ihour = Mid$(Line$, 55, 2)
  Imin = Mid$(Line$, 57, 2) '+ 1
'  Nlev = Mid$(Line$, 23, 3)
'  dTMRI = Mid$(Line$, 27, 4)

  Line$ = fReadLF(INPlu)
  WMOnumber = 99999
  WMOlatitude = Trim(Mid$(Line$, 9, 6))
  WMOlongitude = Trim(Mid$(Line$, 27, 8))
  zo = Trim(Mid$(Line$, 45, 7))
' Skip three lines
  Line$ = fReadLF(INPlu)
  Line$ = fReadLF(INPlu)
  Line$ = fReadLF(INPlu)
'    135.21 14888.93   205.24    32.01
'123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6
  Line$ = fReadLF(INPlu)
  PZraob(1) = Val(Mid$(Line$, 1, 10))               ' mb
  ZPraob(1) = fZtoP(PZraob(1))                      ' km
  ZGraob(1) = Val(Mid$(Line$, 12, 8) / 1000#)       ' expects km
  TZraob(1) = Val(Mid$(Line$, 22, 7))
  RZraob(1) = Val(Mid$(Line$, 32, 6))
  RVraob(1) = fRhoV(RZraob(1), TZraob(1))
  
  Nlev = 1
  Do
    Line$ = fReadLF(INPlu)
    If Len(Line$) < 10 Then Exit Do
    Nlev = Nlev + 1
    PZraob(Nlev) = Val(Mid$(Line$, 1, 10))               ' mb
    ZPraob(Nlev) = fZtoP(PZraob(Nlev))
    ZGraob(Nlev) = Val(Mid$(Line$, 12, 8) / 1000#)       ' expects km
    TZraob(Nlev) = Val(Mid$(Line$, 22, 7))
    RZraob(Nlev) = Val(Mid$(Line$, 32, 6))
    RVraob(Nlev) = fRhoV(RZraob(Nlev), TZraob(Nlev))
'   Debug.Print Nlev; ZGraob(Nlev); PZraob(Nlev); TZraob(Nlev)
  Loop Until EOF(INPlu)
    
'  ZPraob(Nlev) = Val(Mid$(Line$, 1, 5))               ' mb
'  PZraob(Nlev) = fZtoP(ZPraob(Nlev))
'  TZraob(Nlev) = Val(Mid$(Line$, 7, 6))
'  RZraob(Nlev) = Val(Mid$(Line$, 14, 5))
'  RVraob(Nlev) = fRhoV(RZraob(Nlev), TZraob(Nlev))

'  Call Zhme(PZraob(), TZraob(), RZraob(), zo, Nlev, ZGraob())
  
'  If Mid$(Line$, 4, 5) = "     " Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
'  Else
'    Ztrop = fPtoZ(Val(Mid$(Line$, 4, 5)))    'convert mb to km
'    Ttrop = (273.15 + Val(Mid$(Line$, 15, 6)))
'    i = 0: Ltrop = 0
'    Do
'      i = i + 1
'      If PZraob(i) <= Val(Mid$(Line$, 4, 5)) Then
'        Ltrop = i
'        'Debug.Print Record; i; Nlev
'      End If
'    Loop Until Ltrop <> 0
'  End If
  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlev, 0, False)

End Sub


Sub ConvertWyomingNew(INPlu%)
Dim Line$, i%, yy%, i1%, i2%, Status As Boolean, Record&
'"<HTML>
'<LINK REL=""StyleSheet"" HREF=""/resources/select.css"" TYPE=""text/css"">
'<BODY BGCOLOR=""white"">
'<H2>72493 OAK Oakland Int Observations at 00Z 26 Sep 2001</H2>
'<PRE>
'-----------------------------------------------------------------------------
'   PRES   HGHT   TEMP   DWPT   RELH   MIXR   DRCT   SKNT   THTA   THTE   THTV
'    hPa     m      C      C      %    g/kg    deg   knot     K      K      K
'-----------------------------------------------------------------------------
'         1         2         3         4
'1234567890123456789012345678901234567890
'
' 1014.0      3   22.8   12.8     53   9.24    260      1  294.8  321.6  296.4
' 1000.0    126   19.4   10.4     56   7.97    285      6  292.6  315.6  294.0
'  150.0  14020  -62.7  -69.7     38   0.02    260     57  361.9  362.0  361.9
'  100.0  16510  -63.5  -70.5     38   0.03    250     34  404.8  404.9  404.8
'</PRE><H3>Station information and sounding indices</H3><PRE>
'                         Station identifier: OAK
'                             Station number: 72493
'                           Observation time: 010926/0000
'                           Station latitude: 37.73
'                          Station longitude: -122.21
'                          Station elevation: 3.0
'         1         2         3         4         5         6
'123456789012345678901234567890123456789012345678901234567890
  
  Do
    Line$ = fReadLF(INPlu)
  Loop Until Left$(Line$, 5) = "<PRE>"
  For i = 1 To 4
    Line$ = fReadLF(INPlu)
  Next i
  Do 'Search for first line with valid T and Geopotential Height
    Line$ = fReadLF(INPlu)
  Loop Until (Mid$(Line$, 16, 6) <> "      " And Mid$(Line$, 9, 6) <> "      ") And (Mid$(Line$, 16, 6) <> "-999.0" And Mid$(Line$, 9, 6) <> "-999.0")
'  Line$ = fReadLF(INPlu)
'  If Left$(Line$, 7) = "       " Then Exit Sub     ' Pressure is missing
'  If Val(Mid$(Line$, 9, 6)) < 0 Or Val(Mid$(Line$, 16, 6)) = 0 Then '1000 mb Mandatory level may be underground!
'    Line$ = fReadLF(INPlu)
'    If Left$(Line$, 7) = "       " Then Exit Sub   ' Pressure is missing
'  End If

  PZraob(1) = Val(Left$(Line$, 7))                 ' mb
  ZPraob(1) = fPtoZ(PZraob(1))
  If Mid$(Line$, 9, 6) = "      " Then Exit Sub    ' Geopotential Height is missing
  ZGraob(1) = Val(Mid$(Line$, 9, 6) / 1000#)       ' expects km
  If Mid$(Line$, 16, 6) = "      " Then Exit Sub   ' Temperature is missing
  TZraob(1) = (273.15 + Val(Mid$(Line$, 16, 6)))
  If Mid$(Line$, 33, 6) = "      " Then            ' RH is missing
    RZraob(1) = 0#
    RVraob(1) = 0#
  Else
    RZraob(1) = Val(Mid$(Line$, 33, 3))
    RVraob(1) = fRhoV(RZraob(1), TZraob(1))
  End If
  Nlevr = 1
  Line$ = fReadLF(INPlu)
  Do
    Nlevr = Nlevr + 1
    If Nlevr > 2775 Then
    Debug.Print Line$
      Nlevr = Nlevr
    End If
    If Left$(Line$, 7) = "       " Then Exit Sub     ' Pressure is missing
    PZraob(Nlevr) = Val(Left$(Line$, 7))              ' mb
    ZPraob(Nlevr) = fPtoZ(PZraob(Nlevr))
    If ZPraob(Nlevr) <> ZPraob(Nlevr - 1) Then
      If (Mid$(Line$, 9, 6)) = "      " Then             ' Geopotential Height Missing
        Nlevr = Nlevr - 1
        Exit Do
      Else
        ZGraob(Nlevr) = Val(Mid$(Line$, 9, 6) / 1000#)  ' expects km
      End If
      If Mid$(Line$, 16, 6) = "      " Then Exit Sub   ' Temperature is missing
      TZraob(Nlevr) = (cTo + Val(Mid$(Line$, 16, 6)))
      If Mid$(Line$, 33, 6) = "      " Then            ' RH is missing
        RZraob(Nlevr) = 0#
        RVraob(Nlevr) = 0#
      Else
        RZraob(Nlevr) = Val(Mid$(Line$, 33, 3))
        RVraob(Nlevr) = fRhoV(RZraob(i), TZraob(i))
      End If
'   Debug.Print Nlevr; ZGraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr)
    Else  'Redundant levels cause RC program to crash
      Nlevr = Nlevr - 1
    End If
    Line$ = fReadLF(INPlu)
  Loop Until Left$(Line$, 6) = "</PRE>" Or Nlevr >= nRAOBlevels Or EOF(INPlu)
  
  If Left$(Line$, 6) <> "</PRE>" Then  'Throw out rest if >70 levels
    Do: Line$ = fReadLF(INPlu)
    Loop Until Left$(Line$, 6) = "</PRE>"
  End If
  
 ' If Mid$(Line$, 4, 5) = "     " Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
 ' Else
 '   Ztrop = fPtoZ(Val(Mid$(Line$, 4, 5)))    'convert mb to km
 '   Ttrop = (273.15 + Val(Mid$(Line$, 15, 6)))
 '   i = 0: Ltrop = 0
 '   Do
 '     i = i + 1
 '     If PZraob(i) <= Val(Mid$(Line$, 4, 5)) Then
 '       Ltrop = i
 '       'Debug.Print Record; i; Nlevr
 '       X = X
 '     End If
 '   Loop Until Ltrop <> 0
 ' End If
  Line$ = fReadLF(INPlu)
  If Mid$(Line$, 26, 7) = "Station" Then
    WMO4l = Trim$(Mid$(Line$, 46, 4))
    Line$ = fReadLF(INPlu)
    WMOnumber = Val(Mid$(Line$, 46, 5))
  Else
    WMOnumber = Val(Mid$(Line$, 46, 5))
    Status = fGetUAsite(Str(WMOnumber), Record)
    If Status And Left$(GPicao, 4) <> "    " Then WMO4l = Left$(GPicao, 4) Else WMO4l = "XXXX"
  End If
  ' Read observation time
  Line$ = fReadLF(INPlu)
  Iyear = Val(Mid$(Line$, 46, 2))
  If Iyear < 50 Then Iyear = 2000 + Iyear Else Iyear = 1900 + Iyear
  Imonth = Val(Mid$(Line$, 48, 2))  '13
  Iday = Val(Mid$(Line$, 50, 2))    '13
  Ihhmm = Mid$(Line$, 53, 4)    '7
  Ihour = Int(Ihhmm / 100)
  Imin = Ihhmm - 100 * Ihour
  
  Line$ = fReadLF(INPlu)
  WMOlatitude = Val(Mid$(Line$, 46, 5))
  Line$ = fReadLF(INPlu)
  WMOlongitude = Val(Mid$(Line$, 46, 7))
  Line$ = fReadLF(INPlu)
  WMOaltitude = Val(Mid$(Line$, 46, 7))
  
  

  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)
'</PRE><H2>Station information and sounding indices</H2><PRE>
'                         Station identifier: VBG
'                             Station number: 72393
'                           Observation time: 010605/1200
'                           Station latitude: 34.75
'                          Station longitude: -120.56
'                          Station elevation: 121.0
'                            Showalter index: 11.50
'                               Lifted index: 15.22
'    LIFT computed using virtual temperature: 15.17
'                                SWEAT index: 67.99
'                                    K index: 4.10
'                         Cross totals index: 5.30
'                      Vertical totals index: 24.30
'                        Totals totals index: 29.60
'      Convective Available Potential Energy: 0.00
'             CAPE using virtual temperature: 0.00
'                      Convective Inhibition: 0.00
'             CINS using virtual temperature: 0.00
'                     Bulk Richardson Number: 0.00
'          Bulk Richardson Number using CAPV: 0.00
'  Temp [K] of the Lifted Condensation Level: 281.43
'Pres [KPa] of the Lifted Condensation Level: 980.92
'     Mean mixed layer potential temperature: 289.34
'              Mean mixed layer mixing ratio: 5.84
'</PRE><P><A HREF="http://www-das.uwyo.edu/cgi-bin/sounding?OUTPUT=OTEXT&TYPE=SKEWT&HOUR=12&DAY=5&MONTH=6&YEAR=2001&STATION=VBG" TARGET="_top">Old format</A>
'                  http://www-das.uwyo.edu/cgi-bin/sounding?OUTPUT=TEXT&TYPE=SKEWT&HOUR=00&DAY=05&MONTH=06&YEAR=2001&STATION=VBG
'                  http://weather.uwyo.edu/cgi-bin/snd.html?YEAR=2001&MONTH=06&DAY=05&HOUR=00&STID=VBG&OUTPUT=TEXT&TYPE=SKEWT


End Sub

Sub ConvertWyoming(INPlu%)
Dim First$, i%, j%, yy%
'Check first line
First$ = fReadLF(INPlu)
If Mid$(First$, 2, 6) = "<HTML>" Then
  Call ConvertWyomingNew(INPlu)
Else
  Call ConvertWyomingOld(INPlu, First$)
End If

Tcoldest = 400#
For j = 1 To Nlevr    'averaging pressure altitudes
  If TZraob(j) < Tcoldest And j < Nlevr Then
    Tcoldest = TZraob(j)
    Zcoldest = ZPraob(j)
  End If
Next j
  
End Sub

Function fLFtoCRLF$(InFile$)
' Routine to fix line termination character string
' ConversionType = 0 for LF to CRLF
' ShowProgress shows % of way through input file if a text box called
' txtProgress.text has been defined on form

Dim Out$, ByteCount&, C$, CharNo&, i1%, i2%, FoundCRLF%
' AVOID using this code as it is EXTREMELY slow

  ByteCount = Len(InFile$)
  Out$ = Left$(InFile$, 1)
  CharNo = 1
  If Out$ = vbLf Then Out$ = ""
  FoundCRLF = InStr(InFile$, vbCrLf)
'  i1 = 1
'  Do
'    i2 = InStr(i1, InFile$, vbLf)
'    If i2 = 0 Then Exit Do
'    out$ = out$ + Mid$(InFile$, i1, i2 - 1) + vbCrLf
'    i1 = i2 + 1
'  Loop
'  out$ = out$ + Mid$(InFile$, i1, ByteCount - i1)
Do
  CharNo = CharNo + 1
  C$ = Mid$(InFile$, CharNo, 1)
  If C$ = vbLf Then
    Out$ = Out$ + vbCrLf
  Else
    If C$ <> vbCr Then Out$ = Out$ + C$ 'avoid double Cr
  End If
Loop Until CharNo = ByteCount

fLFtoCRLF = Out$
End Function

Sub GetRAOBrange(FltDate$, Rmax!, OUTlu%, nSites%, ALTkmMin!, ZpStep!, jLast%)
Dim x!, i%, j%, RSLlu%, Status As Boolean, UArecord&
Const Ncompare = 300
Static RAOB$(1 To Ncompare), rLat!(1 To Ncompare), rLon!(1 To Ncompare)
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, jOut%
Dim Range!(1 To Ncompare), r$(1 To Ncompare), UTks!(1 To Ncompare), Rsave!(1 To Ncompare), Found(1 To Ncompare) As Boolean
Dim UTksLast, RangeLast!(1 To Ncompare), ZpSave!(1 To Ncompare), ZpLast!(1 To Ncompare)

  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  If nSites = 0 Then
    RSLlu = FreeFile
    Filename$ = Path$ + "RAOB\" + Mission$ + "_RAOBrange.RSL"
    Open Filename$ For Input As RSLlu
    Do
      nSites = nSites + 1
      Input #RSLlu, RAOB$(nSites)
    Loop Until EOF(RSLlu)
    Close RSLlu
    ' Get Site Latitudes and Longitudes
    For i = 1 To nSites
      Status = fGetUAsite(RAOB$(i), UArecord&)
      rLat(i) = GPlatitude
      rLon(i) = GPlongitude
    Next i
    If jLast = 0 Then Print #OUTlu, "Number,Year,Month,Day,UT0,LR11,LR12,Zb1,LR21,LR22,Zb2,RAOBs,RAOBs,Ra1,Ra2,Zp(km)"
  End If
  
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
    
  Filename$ = Path$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  Record = 0: j = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
    For i = 1 To nSites
      Rcurrent = fGCD(rLat(i), rLon(i), Latitude, Longitude)
      If Rcurrent <= Rmax And Rcurrent >= 0 Then
        If Range(i) = 0 Then             'First time through
          Range(i) = 9999#
          RangeLast(i) = Rcurrent
          ZpLast(i) = ALTkm
          UTks(i) = UTsec / 1000#
        Else                             'Check if closer range
          If Rcurrent < Range(i) And Rcurrent < RangeLast(i) Then  'Yes
            Range(i) = Rcurrent
            ZpLast(i) = ALTkm
            UTksLast = UTsec / 1000#
            Found(i) = False
          Else                           'No, Save closest approach
            If Not Found(i) Then
              j = j + 1
              r$(j) = RAOB$(i)
              UTks(j) = UTksLast
              Rsave(j) = RangeLast(i)
              ZpSave(j) = ZpLast(i)
              Range(i) = 0#              'Start over with this site
              Found(i) = True
            End If
          End If
          RangeLast(i) = Rcurrent
        End If
        'Debug.Print RAOB$(i); Rcurrent; UTsec
      End If
    Next i
    DoEvents
  Loop Until EOF(REFlu)
  Close REFlu
  
' Print out closest approach list
  jOut = 0
  For i = 1 To j
    If ZpSave(i) >= Val(ALTkmMin) And Rsave(i) < Rmax Then
      jOut = jOut + 1
      If UTks(i) > 86.4 Then UTks(i) = UTks(i) - 86.4
      If OUTlu <> 0 Then Print #OUTlu, Format(jLast + jOut, "000") + "," + yy$ + "," + mm$ + "," + DD$ + "," + Format(UTks(i), "000.00") + "," + "0,0,0,0,0,0" + "," + r$(i) + "," + r$(i) + "," + Format(Rsave(i), "000.0") + "," + Format(Rsave(i), "000.0") + "," + Format(ZpSave(i), "000.0")
      'Debug.Print yy$ + vbTab + mm$ + vbTab + DD$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + r$(i) + vbTab + r$(i) + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(ZpSave(i), "000.0")
    End If
  Next i
  jLast = jLast + jOut

End Sub
Sub GetRAOBrangeZp(FltDate$, Rmax!, OUTlu%, nSites%, ALTkmMin!, ZpStep!, jLast%)
Dim x!, i%, j%, k%, RSLlu%, Status As Boolean, UArecord&, ZpThreshold!(1 To 21)
Const Ncompare = 300
Static RAOB$(1 To Ncompare), rLat!(1 To Ncompare), rLon!(1 To Ncompare)
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, jOut%, iMax%
Dim Range!(1 To Ncompare), r$(1 To Ncompare), UTks!(1 To Ncompare), Rsave!(1 To Ncompare), Found(1 To Ncompare) As Boolean
Dim UTksLast, RangeLast!(1 To Ncompare), ZpSave!(1 To Ncompare), ZpLast!(1 To Ncompare)
Static BeenHere As Boolean
' Calculate range at fixed Zp instead of closest approach as done in  GetRAOBrange()

  If ZpStep = 2 Then
    iMax = 10
    For i = 1 To iMax: ZpThreshold(i) = 2# * i: Next i
  Else
    iMax = 21
    For i = 1 To iMax: ZpThreshold(i) = i: Next i
  End If
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  If nSites = 0 Then
    RSLlu = FreeFile
    Filename$ = Path$ + "RAOB\" + Mission$ + "_RAOBrange.RSL"
    Open Filename$ For Input As RSLlu
    Do
      nSites = nSites + 1
      Input #RSLlu, RAOB$(nSites)
    Loop Until EOF(RSLlu)
    Close RSLlu
    ' Get Site Latitudes and Longitudes
    For i = 1 To nSites
      Status = fGetUAsite(RAOB$(i), UArecord&)
      rLat(i) = GPlatitude
      rLon(i) = GPlongitude
    Next i
    If jLast = 0 And Not BeenHere Then
      Print #OUTlu, "Number,Year,Month,Day,UT0,LR11,LR12,Zb1,LR21,LR22,Zb2,RAOBs,RAOBs,Ra1,Ra2,Zp(km)"
      BeenHere = True
    End If
  End If
  
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
    
  Filename$ = Path$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  Record = 0: j = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
    Debug.Print FltDate$; UTsec; ALTkm; j
    For i = 1 To nSites
      Rcurrent = fGCD(rLat(i), rLon(i), Latitude, Longitude)
      If Rcurrent <= Rmax And Rcurrent >= 0 Then
        If Range(i) = 0 Then             'First time through
          Range(i) = 9999#
          'RangeLast(i) = Rcurrent
          ZpLast(i) = ALTkm
          UTks(i) = UTsec / 1000#
          UTsecLast = UTks(i)
        Else                             'Check if within altitude threshold of 100 m (1500 ft/min = 8 m/s = 120 m/15 sec scan)
          For k = 1 To iMax
            If Abs(ALTkm - ZpThreshold(k)) < 0.1 Then
              If UTsec / 1000# - UTksLast > 0.3 Then  'was 0.03, 30 sec, instead of 300 sec = 5 min, ~60 km
                j = j + 1
                Range(j) = Rcurrent
                Rsave(j) = Rcurrent
                ZpLast(j) = ALTkm
                ZpSave(j) = ALTkm
                UTksLast = UTsec / 1000#
                r$(j) = RAOB$(i)
                UTks(j) = UTksLast
                'Range(j) = 0#              'Start over with this site
                Exit For
              End If
            End If
          Next k
          'RangeLast(i) = Rcurrent
        End If
        'Debug.Print RAOB$(i); Rcurrent; UTsec
      End If
    Next i
    DoEvents
  Loop Until EOF(REFlu)
  Close REFlu
  
' Print out closest approach list
  jOut = 0
  For i = 1 To j
'    If ZpSave(i) >= Val(ALTkmMIN) And Rsave(i) < Rmax Then
      jOut = jOut + 1
      If UTks(i) > 86.4 Then UTks(i) = UTks(i) '- 86.4
      If OUTlu <> 0 Then Print #OUTlu, Format(jLast + jOut, "000") + "," + yy$ + "," + mm$ + "," + DD$ + "," + Format(UTks(i), "000.000") + ",0,0,0,0,0,0," + r$(i) + "," + r$(i) + "," + Format(Rsave(i), "000.0") + "," + Format(Rsave(i), "000.0") + "," + Format(ZpSave(i), "000.0")
      'Debug.Print yy$ + vbTab + mm$ + vbTab + DD$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + r$(i) + vbTab + r$(i) + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(ZpSave(i), "000.0")
'    End If
  Next i
  jLast = jLast + jOut

End Sub

Function fGetUAsite(Station$, UArecord&) As Boolean
Dim lu%, Record&, WMOno&, ICAOname$
Static WMOarray&(1 To 1000), ICAOarray(1 To 1000) As String
Static ICAOindex(1 To 1000) As Integer
Static BeenHere As Boolean, ICAOcount&, WMOcount&
' Function is true if site found. Results in PUBLIC data structure GP
' Maximum number of allowed Upper Air sites is 1000
' On first pass "lu" is scanned to facilitate future calls
' BeenHere is set TRUE after first time through

fGetUAsite = False

If Not BeenHere Or ReInit Then
  lu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #lu Len = Len(GP)
  WMOcount = 0
  ICAOcount = 0
  Do
    WMOcount = WMOcount + 1
    GPread lu, WMOcount
    WMOarray(WMOcount) = WMOnumber
    If Len(ICAOsite) > 0 Then
      ICAOcount = ICAOcount + 1
      ICAOarray(ICAOcount) = ICAOsite
      ICAOindex(ICAOcount) = WMOcount
    End If
  Loop Until EOF(lu)
  BeenHere = True
  ReInit = False
  Close lu
End If
'
Record = 0
If fNumeric(Trim(Station$)) Then
  WMOno = Val(Station$)
  Do
    Record = Record + 1
    If Record > WMOcount Then Exit Function
  Loop Until WMOno = WMOarray(Record)
Else
  Do
    Record = Record + 1
    If Record > ICAOcount Then Exit Function
  Loop Until Station$ = ICAOarray(Record)
  Record = ICAOindex(Record)
End If

lu = FreeFile
Open "C:\MTP\RAOB\BIN\MTP_UA.BIN" For Random Access Read Write As #lu Len = Len(GP)
GPread lu, Record
Close lu

UArecord = Record
fGetUAsite = True

End Function

Function fRHinterp(Zi!, z!(), RH!(), Nlev%)
Dim Zt!, Zb!, Tt!, TB!, j%
j = 1
'Enter with z and exit with T from interpolation in z() & t()
  If Zi <= z(1) + 0.01 Then fRHinterp = 0#: Exit Function
' Find first level above Zlo
  Do
    j = j + 1
    If j >= Nlev Then fRHinterp = RH(Nlev): Exit Function
    Zt = z(j)
  Loop While Zi > Zt
' And interpolate temperature
  Zb = z(j - 1): Tt = RH(j): TB = RH(j - 1)
  fRHinterp = TB + (Tt - TB) * (Zi - Zb) / (Zt - Zb)
End Function

Function fTinterpG!(i%, z!)
Dim slope!, j%
' i is upper level
'Interpolate in geometric altitude
  j = i - 1
  If ZGraob(i) = ZGraob(j) Then
    fTinterpG = TZraob(j)
    Exit Function
  End If
  slope = (TZraob(i) - TZraob(j)) / (ZGraob(i) - ZGraob(j))
  fTinterpG = TZraob(j) + slope * (z - ZGraob(j))
End Function

Sub GPread(lu%, Record&)
Dim a As GPrecord

Get #lu%, Record, a

GPcards = a.GPcards
GPicao = a.GPicao
GPstation = a.GPstation
GPlatitude = a.GPlatitude
GPlongitude = a.GPlongitude
GPaltitude = a.GPaltitude
GPregion = a.GPregion
GPcountry = a.GPcountry
GPstate = a.GPstate
GPstation = a.GPstation
GPspare = a.GPspare

WMOnumber = GPcards / 10
ICAOsite = Trim(GPicao)

End Sub

Sub GPwrite(lu%, Record&)
Dim a As GPrecord

a.GPcards = GPcards
a.GPicao = GPicao
a.GPstation = GPstation
a.GPlatitude = GPlatitude
a.GPlongitude = GPlongitude
a.GPaltitude = GPaltitude
a.GPregion = GPregion
a.GPcountry = GPcountry
a.GPstate = GPstate
a.GPstation = GPstation
a.GPspare = GPspare

Put #lu%, Record, a

End Sub

Sub RAOBreadS(lu%, Record&)
Dim a As RAOBrecord, i%

  Get #lu%, Record, a
  
  RAOBedit = a.RAOBedit
  If RAOBedit And &H1 Then RAOBgood = True Else RAOBgood = False
  If RAOBedit And &H2 Then TropGood = True Else TropGood = False
  If RAOBedit And &H4 Then EditLevel = True Else EditLevel = False
  If RAOBedit And &H8 Then EditTrop = True Else EditTrop = False
  If RAOBedit And &H10 Then UseEditLevel = True Else UseEditLevel = False
  If RAOBedit And &H20 Then UseEditTrop = True Else UseEditTrop = False
  Nedit = (RAOBedit And &H300) \ 256
  
  WMOnumber = a.WMOnumber
  WMO4l = a.WMO4l
  DandT = a.DandT
  Iyear = DatePart("yyyy", DandT)
  Idoy = DatePart("y", DandT)
  Imonth = DatePart("m", DandT)
  Iday = DatePart("d", DandT)
  Ihour = DatePart("h", DandT)
  Imin = DatePart("n", DandT)
  Isec = DatePart("s", DandT)
  Ihhmm = Ihour * 100 + Imin
  Nlevr = a.Nlevr
  dTMRI = a.dTMRI
  ijkl = a.ijkl
  OATraob = a.OATraob
  OATraobPP = a.OATraobPP
  WMOlatitude = a.WMOlatitude
  WMOlongitude = a.WMOlongitude
  WMOaltitude = a.WMOaltitude
  zMRI1 = a.zMRI1
  zMRI2 = a.zMRI2
  
  Ztrop = a.Ztrop
  Ttrop = a.Ttrop
  Ltrop = a.Ltrop
  
  Ztrop2 = a.Ztrop2
  Ttrop2 = a.Ttrop2
  Ltrop2 = a.Ltrop2
  
  ZtropE = a.ZtropE   'Editted trop values
  TtropE = a.TtropE
  LtropE = a.LtropE
  
  IntRhoV = a.IntRhoV
  Tcoldest = a.Tcoldest
  Zcoldest = a.Zcoldest
  nRAOBlevels = a.nRAOBlevels
  If nRAOBlevels <> 150 Then nRAOBlevels = 70
  
  Zt1 = Ztrop
  TT1 = Ttrop
  LT1 = Ltrop
'Debug.Print Ztrop, ZtropE, Ztrop2
  For i% = 1 To Nlevr + 3: ZGraob(i%) = ConvertIntegerToGZ(a.GZr(i%)): Next i
' Check to make sure monotonic
  For i% = 2 To Nlevr
    If ZGraob(i%) < ZGraob(i - 1) Then
      ZGraob(i) = (ZGraob(i - 1) + ZGraob(i + 1)) / 2
    End If
  Next i
  For i% = 1 To Nlevr + 3
    PZraob(i%) = a.PZr(i%) / 10#   'mb
    ZPraob(i) = fPtoZ(PZraob(i))   'km
  Next i
  For i% = 1 To Nlevr + 3: TZraob(i%) = a.TZr(i%) / 100#: Next i
  For i% = 1 To Nlevr + 3
    RZraob(i%) = a.RZr(i%) / 100#
    If i <= Nlev Then RVraob(i%) = fRhoV(RZraob(i), TZraob(i)) Else RVraob(i) = 0#
  Next i
  If Nedit > 0 Then
    For i% = 1 To Nedit: LZraob(i%) = a.LZr(i%): Next i
  End If
  For i = Nlevr + 3 To nRAOBlevels
    PZraob(i%) = 0#
    ZPraob(i%) = 0#
    TZraob(i%) = 0#
    RVraob(i%) = 0#
  Next i
  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)  'Temporary
  'Debug.Print WMO4l; Nlevr; ZPraob(Nlevr); TZraob(Nlevr); ZPraob(Nlevr - 1); TZraob(Nlevr - 1); ZPraob(Nlevr - 2); TZraob(Nlevr - 2)
  
End Sub
Sub RAOBread(RAOBlu%, Cycle&)
  
  If Not EOF(RAOBlu) Then
    Select Case RAOBtype
    Case 0: Call RAOBreadSL(RAOBlu, Cycle)
    Case 1: Call RAOBreadS(RAOBlu, Cycle)
    Case 2: Call RAOBreadL(RAOBlu, Cycle)
    Case 3: Call RAOBreadC(RAOBlu, Cycle)
    End Select
  End If

End Sub

Sub RAOBreadSL(lu%, Record&)
Dim a As RAOBrecord2, i%, slope!

  Get #lu%, Record, a
  If EOF(lu) Then Exit Sub
  RAOBedit = a.RAOBedit
' xxx mjm 20060303
  If RAOBedit And &H1 Then RAOBgood = True Else RAOBgood = True
  If RAOBedit And &H2 Then TropGood = True Else TropGood = False
  If RAOBedit And &H4 Then EditLevel = True Else EditLevel = False
  If RAOBedit And &H8 Then EditTrop = True Else EditTrop = False
  If RAOBedit And &H10 Then UseEditLevel = True Else UseEditLevel = False
  If RAOBedit And &H20 Then UseEditTrop = True Else UseEditTrop = False
  Nedit = (RAOBedit And &H300) \ 256
  
  WMOnumber = a.WMOnumber
  WMO4l = a.WMO4l
  DandT = a.DandT
  Iyear = DatePart("yyyy", DandT)
  Idoy = DatePart("y", DandT)
  Imonth = DatePart("m", DandT)
  Iday = DatePart("d", DandT)
  Ihour = DatePart("h", DandT)
  Imin = DatePart("n", DandT)
  Isec = DatePart("s", DandT)
  Ihhmm = Ihour * 100 + Imin
  Nlevr = a.Nlevr
  dTMRI = a.dTMRI
  ijkl = a.ijkl
  OATraob = a.OATraob
  OATraobPP = a.OATraobPP
  WMOlatitude = a.WMOlatitude
  WMOlongitude = a.WMOlongitude
  WMOaltitude = a.WMOaltitude
  zMRI1 = a.zMRI1
  zMRI2 = a.zMRI2
  
  Ztrop = a.Ztrop
  Ttrop = a.Ttrop
  Ltrop = a.Ltrop
  
  Ztrop2 = a.Ztrop2
  Ttrop2 = a.Ttrop2
  Ltrop2 = a.Ltrop2
  
  ZtropE = a.ZtropE   'Editted trop values
  TtropE = a.TtropE
  LtropE = a.LtropE
  
  IntRhoV = a.IntRhoV
  Tcoldest = a.Tcoldest
  Zcoldest = a.Zcoldest
  nRAOBlevels = a.nRAOBlevels
  If nRAOBlevels <> 150 Then nRAOBlevels = 70
  
  Zt1 = Ztrop
  TT1 = Ttrop
  LT1 = Ltrop
'  Debug.Print Ztrop, ZtropE, Ztrop2
  For i% = 1 To Nlevr + Nedit: ZGraob(i%) = ConvertIntegerToGZ(a.GZr2(i%)): Next i
' Check to make sure Zg is monotonic
  For i% = 2 To Nlevr
    If ZGraob(i%) < ZGraob(i - 1) Then
      ZGraob(i) = (ZGraob(i - 1) + ZGraob(i + 1)) / 2
    End If
  Next i
  For i% = 1 To Nlevr + Nedit
    ZPraob(i%) = ConvertIntegertoZP(a.PZr2(i%))
    PZraob(i) = fZtoP(ZPraob(i))   'km
  Next i
  For i% = 1 To Nlevr + Nedit: TZraob(i%) = a.TZr2(i%) / 100#: Next i
  For i% = 1 To Nlevr + Nedit
    RZraob(i%) = a.RZr2(i%) / 100#
    If i <= Nlevr Then RVraob(i%) = fRhoV(RZraob(i), TZraob(i)) Else RVraob(i) = 0#
  Next i
  If Nedit > 0 Then
    For i% = 1 To Nedit: LZraob(i%) = a.LZr2(i%): Next i
  End If
' Don't allow extended soundings to get warmer than 280 K
  If TZraob(Nlevr) > 280 Then
    ZPraob(Nlevr + 1) = ZPraob(Nlevr)
    PZraob(Nlevr + 1) = PZraob(Nlevr)
    RZraob(Nlevr + 1) = RZraob(Nlevr)
    ZGraob(Nlevr + 1) = ZGraob(Nlevr)
    TZraob(Nlevr + 1) = 275
    slope = (TZraob(Nlevr) - TZraob(Nlevr - 1)) / (ZPraob(Nlevr) - ZPraob(Nlevr - 1))
    If slope > 0 Then ZPraob(Nlevr) = ZPraob(Nlevr - 1) + (275 - TZraob(Nlevr - 1)) / slope
    PZraob(Nlevr) = fZtoP(ZPraob(Nlevr))
     slope = (TZraob(Nlevr) - TZraob(Nlevr - 1)) / (ZGraob(Nlevr) - ZGraob(Nlevr - 1))
    If slope > 0 Then ZGraob(Nlevr) = ZGraob(Nlevr - 1) + (275 - TZraob(Nlevr - 1)) / slope
    TZraob(Nlevr) = 275
    Nlevr = Nlevr + 1
  End If
'  For i = Nlevr + 3 To nRAOBlevels
'    PZraob(i%) = 0#
'    ZPraob(i) = 0#
'    TZraob(i%) = 0#
'    RVraob(i%) = 0#
'  Next i
  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)  'Temporary
'  Debug.Print WMO4l; Nlevr; ZPraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr); ZGraob(Nlevr); a.GZr2(Nlevr)
  If Ztrop = 0 Then
    Ztrop = ZtropE
    Ttrop = TtropE
  End If
End Sub

Sub RAOBreadL(lu%, Record&)
Dim a As LargeRAOB, i%

  Get #lu%, Record, a
  
  RAOBedit = a.RAOBedit
  If RAOBedit And &H1 Then RAOBgood = True Else RAOBgood = False
  If RAOBedit And &H2 Then TropGood = True Else TropGood = False
  If RAOBedit And &H4 Then EditLevel = True Else EditLevel = False
  If RAOBedit And &H8 Then EditTrop = True Else EditTrop = False
  If RAOBedit And &H10 Then UseEditLevel = True Else UseEditLevel = False
  If RAOBedit And &H20 Then UseEditTrop = True Else UseEditTrop = False
  Nedit = (RAOBedit And &H300) \ 256
  
  WMOnumber = a.WMOnumber
  WMO4l = a.WMO4l
  DandT = a.DandT
  Iyear = DatePart("yyyy", DandT)
  Idoy = DatePart("y", DandT)
  Imonth = DatePart("m", DandT)
  Iday = DatePart("d", DandT)
  Ihour = DatePart("h", DandT)
  Imin = DatePart("n", DandT)
  Isec = DatePart("s", DandT)
  Ihhmm = Ihour * 100 + Imin
  Nlevr = a.Nlevr
  dTMRI = a.dTMRI
  ijkl = a.ijkl
  OATraob = a.OATraob
  OATraobPP = a.OATraobPP
  WMOlatitude = a.WMOlatitude
  WMOlongitude = a.WMOlongitude
  WMOaltitude = a.WMOaltitude
  zMRI1 = a.zMRI1
  zMRI2 = a.zMRI2
  
  Ztrop = a.Ztrop
  Ttrop = a.Ttrop
  Ltrop = a.Ltrop
  
  Ztrop2 = a.Ztrop2
  Ttrop2 = a.Ttrop2
  Ltrop2 = a.Ltrop2
  
  ZtropE = a.ZtropE   'Editted trop values
  TtropE = a.TtropE
  LtropE = a.LtropE
  
  Zt1 = Ztrop
  TT1 = Ttrop
  LT1 = Ltrop
  
  IntRhoV = a.IntRhoV
  Tcoldest = a.Tcoldest
  Zcoldest = a.Zcoldest
  
  For i% = 1 To Nlevr + 3:
    ZGraob(i%) = ConvertIntegerToGZ(a.GZr(i%))
  Next i
' Check to make sure monotonic
  For i% = 2 To Nlevr
    If ZGraob(i%) < ZGraob(i - 1) Then
      ZGraob(i) = (ZGraob(i - 1) + ZGraob(i + 1)) / 2
    End If
  Next i

  For i% = 1 To Nlevr + 3
    PZraob(i%) = a.PZr(i%) / 10#   'mb
    ZPraob(i) = fPtoZ(PZraob(i))   'km
  Next i
  For i% = 1 To Nlevr + 3: TZraob(i%) = a.TZr(i%) / 100#: Next i
  For i% = 1 To Nlevr + 3: RZraob(i%) = a.RZr(i%) / 100#: Next i
  For i = Nlevr + 3 To nRAOBlevels
    PZraob(i%) = 0#
    ZPraob(i%) = 0#
    TZraob(i%) = 0#
    RVraob(i%) = 0#
  Next i
  'If Nedit > 0 Then
  '  For i% = 1 To Nedit: LZraob(i%) = a.LZr(i%): Next i
  'End If
  'Debug.Print dTMRI
End Sub

Sub RAOBreadC(lu%, Record&)
Dim a As RAOBrecord3, i%

  Get #lu%, Record, a
  
  RAOBedit = a.RAOBedit
  If RAOBedit And &H1 Then RAOBgood = True Else RAOBgood = False
  If RAOBedit And &H2 Then TropGood = True Else TropGood = False
  If RAOBedit And &H4 Then EditLevel = True Else EditLevel = False
  If RAOBedit And &H8 Then EditTrop = True Else EditTrop = False
  If RAOBedit And &H10 Then UseEditLevel = True Else UseEditLevel = False
  If RAOBedit And &H20 Then UseEditTrop = True Else UseEditTrop = False
  Nedit = (RAOBedit And &H300) \ 256
  
  WMOnumber = a.WMOnumber
  WMO4l = a.WMO4l
  DandT = a.DandT
  Iyear = DatePart("yyyy", DandT)
  Idoy = DatePart("y", DandT)
  Imonth = DatePart("m", DandT)
  Iday = DatePart("d", DandT)
  Ihour = DatePart("h", DandT)
  Imin = DatePart("n", DandT)
  Isec = DatePart("s", DandT)
  Ihhmm = Ihour * 100 + Imin
  Nlevr = a.Nlevr
  dTMRI = a.dTMRI
  ijkl = a.ijkl
  OATraob = a.OATraob
  OATraobPP = a.OATraobPP
  WMOlatitude = a.WMOlatitude
  WMOlongitude = a.WMOlongitude
  WMOaltitude = a.WMOaltitude
  zMRI1 = a.zMRI1
  zMRI2 = a.zMRI2
  
  Ztrop = a.Ztrop
  Ttrop = a.Ttrop
  Ltrop = a.Ltrop
  
  Ztrop2 = a.Ztrop2
  Ttrop2 = a.Ttrop2
  Ltrop2 = a.Ltrop2
  
  ZtropE = a.ZtropE   'Editted trop values
  TtropE = a.TtropE
  LtropE = a.LtropE
  
  Zt1 = Ztrop
  TT1 = Ttrop
  LT1 = Ltrop
  
  IntRhoV = a.IntRhoV
  Tcoldest = a.Tcoldest
  Zcoldest = a.Zcoldest
  
  For i% = 1 To Nlevr + 3:
    ZGraob(i%) = ConvertIntegerToGZ(a.GZr(i%))
  Next i
' Check to make sure monotonic
  For i% = 2 To Nlevr
    If ZGraob(i%) < ZGraob(i - 1) Then
      ZGraob(i) = (ZGraob(i - 1) + ZGraob(i + 1)) / 2
    End If
  Next i

  For i% = 1 To Nlevr + 3
    PZraob(i%) = a.PZr(i%) / 10#   'mb
    ZPraob(i) = fPtoZ(PZraob(i))   'km
  Next i
  For i% = 1 To Nlevr + 3: TZraob(i%) = a.TZr(i%) / 100#: Next i
  For i% = 1 To Nlevr + 3: RZraob(i%) = a.RZr(i%) / 100#: Next i
  For i = Nlevr + 3 To nRAOBlevels
    PZraob(i%) = 0#
    ZPraob(i%) = 0#
    TZraob(i%) = 0#
    RVraob(i%) = 0#
  Next i
  'If Nedit > 0 Then
  '  For i% = 1 To Nedit: LZraob(i%) = a.LZr(i%): Next i
  'End If
  'Debug.Print dTMRI
End Sub


Sub RAOBwrite(RAOBlu%, Cycle&)
  
  Select Case RAOBtypeOUT
  Case 0: Call RAOBwriteSL(RAOBlu, Cycle)
  Case 1: Call RAOBwriteS(RAOBlu, Cycle)
  Case 2: Call RAOBwriteL(RAOBlu, Cycle)
  Case 3: Call RAOBwriteC(RAOBlu, Cycle)
  End Select

End Sub

Sub RAOBwriteSL(lu%, Record&)
Dim a As RAOBrecord2, i%
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop
' &H0010 ... Use Editted Levels
' &H0020 ... Use Editted Trop
' &H0300 ... Number of edit levels (2 bit = 0 to 3)
  RAOBedit = 0
  If RAOBgood Then RAOBedit = &H1
  If TropGood Then RAOBedit = RAOBedit + &H2
  If EditLevel Then RAOBedit = RAOBedit + &H4
  If EditTrop Then RAOBedit = RAOBedit + &H8
  If UseEditLevel Then RAOBedit = RAOBedit + &H10
  If UseEditTrop Then RAOBedit = RAOBedit + &H20
  RAOBedit = RAOBedit + 256 * Nedit
  
  a.RAOBedit = RAOBedit
  
  a.WMOnumber = WMOnumber
  a.WMO4l = WMO4l
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  a.DandT = DandT
  a.Nlevr = Nlevr
  a.dTMRI = dTMRI
  a.ijkl = ijkl
  a.OATraob = OATraob
  a.OATraobPP = OATraobPP
  a.WMOlatitude = WMOlatitude
  a.WMOlongitude = WMOlongitude
  a.WMOaltitude = WMOaltitude
  a.zMRI1 = zMRI1
  a.zMRI2 = zMRI2
  
  a.Ztrop = Ztrop
  a.Ttrop = Ttrop
  a.Ltrop = Ltrop
  
  a.Ztrop2 = Ztrop2
  a.Ttrop2 = Ttrop2
  a.Ltrop2 = Ltrop2
  
  a.IntRhoV = IntRhoV
  a.Tcoldest = Tcoldest
  a.Zcoldest = Zcoldest
  
  a.ZtropE = ZtropE             'Edit trop altitude
  a.TtropE = TtropE             'Edit trop temperature
  a.LtropE = LtropE             'Edit trop level
  
  a.nRAOBlevels = nRAOBlevels
  
  For i% = 1 To Nlevr + Nedit: a.GZr2(i%) = ConvertGZtoInteger(ZGraob(i%)): Next i
  For i% = 1 To Nlevr + Nedit: a.PZr2(i%) = ConvertZPtoInteger(fPtoZ(PZraob(i%))): Next i
  For i% = 1 To Nlevr + Nedit
    If TZraob(i) > 327.67 Then TZraob(i) = 327#
    'Debug.Print i; TZraob(i)
    a.TZr2(i%) = Int(100# * TZraob(i%) + 0.5):
  Next i
  For i% = 1 To Nlevr + Nedit
    If RZraob(i%) > 100 Then RZraob(i%) = 100
    a.RZr2(i%) = Int(100# * RZraob(i%) + 0.5)
  Next i
  If Nedit > 0 Then
    For i% = 1 To Nedit:  a.LZr2(i%) = LZraob(i%): Next i
  End If
'  For i = Nlevr + 3 To nRAOBlevels
'    a.GZr2(i%) = 0#
'    a.PZr2(i%) = 0#
'    a.TZr2(i%) = 0#
'    a.RZr2(i%) = 0#
'  Next i
  'Call ShowProfile(TZraob(), ZPraob(), Nlevr)
  Put #lu%, Record, a
  'Debug.Print WMO4l; Nlevr; ZPraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr); ZGraob(Nlevr); a.GZr2(Nlevr)
End Sub
Sub RAOBwriteL(lu%, Record&)
Dim a As LargeRAOB, i%
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop
' &H0010 ... Use Editted Levels
' &H0020 ... Use Editted Trop
' &H0300 ... Number of edit levels (2 bit = 0 to 3)
  RAOBedit = 0
  If RAOBgood Then RAOBedit = &H1
  If TropGood Then RAOBedit = RAOBedit + &H2
  If EditLevel Then RAOBedit = RAOBedit + &H4
  If EditTrop Then RAOBedit = RAOBedit + &H8
  If UseEditLevel Then RAOBedit = RAOBedit + &H10
  If UseEditTrop Then RAOBedit = RAOBedit + &H20
  RAOBedit = RAOBedit + 256 * Nedit
  
  a.RAOBedit = RAOBedit
  
  a.WMOnumber = WMOnumber
  a.WMO4l = WMO4l
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  a.DandT = DandT
  a.Nlevr = Nlevr
  'Debug.Print Nlev
  a.dTMRI = dTMRI
  a.ijkl = ijkl
  a.OATraob = OATraob
  a.OATraobPP = OATraobPP
  a.WMOlatitude = WMOlatitude
  a.WMOlongitude = WMOlongitude
  a.WMOaltitude = WMOaltitude
  a.zMRI1 = zMRI1
  a.zMRI2 = zMRI2
    
  a.Ztrop = Ztrop
  a.Ttrop = Ttrop
  a.Ltrop = Ltrop
  
  a.Ztrop2 = Ztrop2
  a.Ttrop2 = Ttrop2
  a.Ltrop2 = Ltrop2
  
  a.IntRhoV = IntRhoV
  a.Tcoldest = Tcoldest
  a.Zcoldest = Zcoldest
  
  a.ZtropE = ZtropE             'Edit trop altitude
  a.TtropE = TtropE             'Edit trop temperature
  a.LtropE = LtropE             'Edit trop level
  
  For i% = 1 To Nlevr + 3: a.GZr(i%) = ConvertGZtoInteger(ZGraob(i%)): Next i
  For i% = 1 To Nlevr + 3: a.PZr(i%) = Int(10# * PZraob(i%) + 0.5): Next i
  For i% = 1 To Nlevr + 3
    If TZraob(i) > 327.67 Then TZraob(i) = 327#
  'Debug.Print i; TZraob(i)
    a.TZr(i%) = Int(100# * TZraob(i%) + 0.5):
    Next i
  For i% = 1 To Nlevr + 3: a.RZr(i%) = Int(100# * RZraob(i%) + 0.5): Next i
'  If Nedit > 0 Then
'    For i% = 1 To Nedit:  a.LZr(i%) = LZraob(i%): Next i
'  End If
  For i = Nlevr + 3 To nRAOBlevels
    a.GZr(i%) = 0#
    a.PZr(i%) = 0#
    a.TZr(i%) = 0#
    a.RZr(i%) = 0#
  Next i
  Put #lu%, Record, a

End Sub

Sub RAOBwriteC(lu%, Record&)
Dim a As RAOBrecord3, i%
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop
' &H0010 ... Use Editted Levels
' &H0020 ... Use Editted Trop
' &H0300 ... Number of edit levels (2 bit = 0 to 3)
  RAOBedit = 0
  If RAOBgood Then RAOBedit = &H1
  If TropGood Then RAOBedit = RAOBedit + &H2
  If EditLevel Then RAOBedit = RAOBedit + &H4
  If EditTrop Then RAOBedit = RAOBedit + &H8
  If UseEditLevel Then RAOBedit = RAOBedit + &H10
  If UseEditTrop Then RAOBedit = RAOBedit + &H20
  RAOBedit = RAOBedit + 256 * Nedit
  
  a.RAOBedit = RAOBedit
  
  a.WMOnumber = WMOnumber
  a.WMO4l = WMO4l
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  a.DandT = DandT
  a.Nlevr = Nlevr
  'Debug.Print Nlev
  a.dTMRI = dTMRI
  a.ijkl = ijkl
  a.OATraob = OATraob
  a.OATraobPP = OATraobPP
  a.WMOlatitude = WMOlatitude
  a.WMOlongitude = WMOlongitude
  a.WMOaltitude = WMOaltitude
  a.zMRI1 = zMRI1
  a.zMRI2 = zMRI2
    
  a.Ztrop = Ztrop
  a.Ttrop = Ttrop
  a.Ltrop = Ltrop
  
  a.Ztrop2 = Ztrop2
  a.Ttrop2 = Ttrop2
  a.Ltrop2 = Ltrop2
  
  a.IntRhoV = IntRhoV
  a.Tcoldest = Tcoldest
  a.Zcoldest = Zcoldest
  
  a.ZtropE = ZtropE             'Edit trop altitude
  a.TtropE = TtropE             'Edit trop temperature
  a.LtropE = LtropE             'Edit trop level
  
  For i% = 1 To Nlevr + 3: a.GZr(i%) = ConvertGZtoInteger(ZGraob(i%)): Next i
  For i% = 1 To Nlevr + 3: a.PZr(i%) = Int(10# * PZraob(i%) + 0.5): Next i
  For i% = 1 To Nlevr + 3
    If TZraob(i) > 327.67 Then TZraob(i) = 327#
  'Debug.Print i; TZraob(i)
    a.TZr(i%) = Int(100# * TZraob(i%) + 0.5):
    Next i
  For i% = 1 To Nlevr + 3: a.RZr(i%) = Int(100# * RZraob(i%) + 0.5): Next i
'  If Nedit > 0 Then
'    For i% = 1 To Nedit:  a.LZr(i%) = LZraob(i%): Next i
'  End If
'  For i = Nlevr + 3 To nRAOBlevels
'    A.GZr(i%) = 0#
'    A.PZr(i%) = 0#
'    A.TZr(i%) = 0#
'    A.RZr(i%) = 0#
'  Next i
  Put #lu%, Record, a

End Sub

Sub RAOBwriteS(lu%, Record&)
Dim a As RAOBrecord, i%
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop
' &H0010 ... Use Editted Levels
' &H0020 ... Use Editted Trop
' &H0300 ... Number of edit levels (2 bit = 0 to 3)
  RAOBedit = 0
  If RAOBgood Then RAOBedit = &H1
  If TropGood Then RAOBedit = RAOBedit + &H2
  If EditLevel Then RAOBedit = RAOBedit + &H4
  If EditTrop Then RAOBedit = RAOBedit + &H8
  If UseEditLevel Then RAOBedit = RAOBedit + &H10
  If UseEditTrop Then RAOBedit = RAOBedit + &H20
  RAOBedit = RAOBedit + 256 * Nedit
  
  a.RAOBedit = RAOBedit
  
  a.WMOnumber = WMOnumber
  a.WMO4l = WMO4l
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  a.DandT = DandT
  a.Nlevr = Nlevr
  a.dTMRI = dTMRI
  a.ijkl = ijkl
  a.OATraob = OATraob
  a.OATraobPP = OATraobPP
  a.WMOlatitude = WMOlatitude
  a.WMOlongitude = WMOlongitude
  a.WMOaltitude = WMOaltitude
  a.zMRI1 = zMRI1
  a.zMRI2 = zMRI2
  
  a.Ztrop = Ztrop
  a.Ttrop = Ttrop
  a.Ltrop = Ltrop
  
  a.Ztrop2 = Ztrop2
  a.Ttrop2 = Ttrop2
  a.Ltrop2 = Ltrop2
  
  a.IntRhoV = IntRhoV
  a.Tcoldest = Tcoldest
  a.Zcoldest = Zcoldest
  
  a.ZtropE = ZtropE             'Edit trop altitude
  a.TtropE = TtropE             'Edit trop temperature
  a.LtropE = LtropE             'Edit trop level
  
  a.nRAOBlevels = nRAOBlevels
  
  For i% = 1 To Nlevr + 3: a.GZr(i%) = ConvertGZtoInteger(ZGraob(i%)): Next i
  For i% = 1 To Nlevr + 3: a.PZr(i%) = Int(10# * PZraob(i%) + 0.5): Next i
  For i% = 1 To Nlevr + 3
    If TZraob(i) > 327.67 Then TZraob(i) = 327#
    'Debug.Print i; TZraob(i)
    a.TZr(i%) = Int(100# * TZraob(i%) + 0.5):
  Next i
  For i% = 1 To Nlevr + 3
    If RZraob(i%) > 100 Then RZraob(i%) = 100
    a.RZr(i%) = Int(100# * RZraob(i%) + 0.5)
  Next i
  If Nedit > 0 Then
    For i% = 1 To Nedit:  a.LZr(i%) = LZraob(i%): Next i
  End If
  For i = Nlevr + 3 To nRAOBlevels
    a.GZr(i%) = 0#
    a.PZr(i%) = 0#
    a.TZr(i%) = 0#
    a.RZr(i%) = 0#
  Next i
  'Call ShowProfile(TZraob(), ZPraob(), Nlevr)
  Put #lu%, Record, a
  'Debug.Print WMO4l; Nlevr; ZPraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr); ZPraob(Nlevr - 1); ZPraob(Nlevr - 1); TZraob(Nlevr - 1); ZPraob(Nlevr - 2); TZraob(Nlevr - 2)


End Sub

Function fVB6_RAOB_INIT()
Dim i%, INPlu, a$
' Interpolate saved RAOB to specified levels, Z
' A sounding is saved by selecting the VB6 "Out Format" in RAOBman
' and then depressing the "Export RAOBs" button on the Export tab.
' If Init is TRUE RAOB file is read in

  INPlu = FreeFile
  Open "C:\MTP\RAOB\VB6_RAOB.txt" For Input As INPlu
  Input #INPlu, Nlevr
'52
'123456789012345678901234567890123456789012345678901234567890
'04320   BGDH   19971201   0000Z LAT= 76.770 LON= -18.670
'   P[mb]  Zp[m]  Zg[m]    T[K]  RH[%]
'1023.0  -0.08 0.01  252.9 91.0
'1003.0  0.09  0.16  257.5 100.0
'1000.0  0.11  0.18  258.1 97.0
'996.0 0.15  0.21  259.1 93.0
'967.0 0.39  0.44  261.1 66.0
  Input #INPlu, a$
  WMOnumber = Left$(a$, 5)
  WMO4l = Mid$(a$, 9, 4)
  Iyear = Mid$(a$, 16, 4)
  Imonth = Mid$(a$, 20, 2)
  Iday = Mid$(a$, 22, 2)
  WMOlatitude = Mid$(a$, 37, 7)
  WMOlongitude = Mid$(a$, 49, 8)
  
  Input #INPlu, a$
  For i = 1 To Nlevr
    Input #INPlu, PZraob(i), ZPraob(i), ZGraob(i), TZraob(i), RZraob(i)
  Next i
  If ZPraob(Nlevr) < 50 Then
    Nlevr = Nlevr + 1
    ZPraob(Nlevr) = 50#
    TZraob(Nlevr) = TZraob(Nlevr - 1) + 2 * (50 - ZPraob(Nlevr - 1))
    PZraob(Nlevr) = fZtoP(50)
    RZraob(Nlevr) = 0
    ZGraob(Nlevr) = ZGraob(Nlevr - 1) + fHypsometricEqn(TZraob(Nlevr - 1), PZraob(Nlevr - 1), TZraob(Nlevr), PZraob(Nlevr))
    'ZGraob(Nlevr) = ZGraob(Nlevr) + fZhToZg(ZGraob(ZGraob(Nlevr)), WMOlatitude)
  End If

  Close INPlu

  fVB6_RAOB_INIT = Nlevr

End Function

Sub ReformatLine(Inp$)
Dim i%, j%, k%, Out$, x!, temp$, ii%, jj%, a$
'-----------------------------------------------------------------------------
'   PRES   HGHT   TEMP   DWPT   RELH   MIXR   DRCT   SKNT   THTA   THTE   THTV
'    hPa     m      C      C      %    g/kg    deg   knot     K      K      K
'-----------------------------------------------------------------------------
'  700.1   3180    7.4   -3.6     46   4.21      0      8  310.6  324.1  311.4
'  606.0   4346   -2.5   -6.7     73   3.84    306     13  312.3  324.7  313.0
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6         7         8         9
'  Inp$ = "  700.0   3180    7.4   -3.6     46   4.21      0      8  310.6  324.1  311.4"
  
  i = Len(Inp$)
  For j = 1 To i
    a$ = Mid$(Inp$, j, 1)
    If Asc(a$) = 9 Then
      Mid$(Inp$, j, 1) = " "
    End If
  Next j
  i = InStr(6, Inp$, " ")
  ii = i
  Do
    ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Left(Inp, i - 1))
  Out$ = Format(x, "###0.0")
  Out$ = Space(7 - Len(Out$)) + Out$
  
  j = InStr(ii, Inp$, " ")
  jj = j
  Do
    jj = jj + 1
  Loop Until Mid$(Inp$, jj, 1) <> " "
  x = Val(Mid$(Inp$, i, j - i))
  temp$ = Format(x, "#####0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  i = InStr(jj, Inp$, " ")
  ii = i
  Do
  ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Mid(Inp, jj, i - jj))
  temp$ = Format(x, "#####0.0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  j = InStr(ii, Inp$, " ")
  jj = j
  Do
    jj = jj + 1
  Loop Until Mid$(Inp$, jj, 1) <> " "
  x = Val(Mid$(Inp$, i, j - i))
  temp$ = Format(x, "###0.0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  i = InStr(jj, Inp$, " ")
  ii = i
  Do
  ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Mid(Inp, jj, i - jj))
  temp$ = Format(x, "#####0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  i = Len(Out$)
  Out$ = Out$ + Space(77 - i + 1)
  Inp$ = Out$
  
End Sub

Sub ReformatLineTest()
Dim i%, j%, k%, Out$, x!, temp$, ii%, jj%, Inp$
'-----------------------------------------------------------------------------
'   PRES   HGHT   TEMP   DWPT   RELH   MIXR   DRCT   SKNT   THTA   THTE   THTV
'    hPa     m      C      C      %    g/kg    deg   knot     K      K      K
'-----------------------------------------------------------------------------
'  700.1   3180    7.4   -3.6     46   4.21      0      8  310.6  324.1  311.4
'  606.0   4346   -2.5   -6.7     73   3.84    306     13  312.3  324.7  313.0
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'         1         2         3         4         5         6         7         8         9
'    42.3  21581.64    -67.1   -93.6       2 -999   -999   -999   -999   -999   -999
'  Inp$ = "  700.0   3180    7.4   -3.6     46   4.21      0      8  310.6  324.1  311.4"
Inp$ = "    42.3  21581.64    -67.1   -93.6       2 -999   -999   -999   -999   -999   -999"

  i = InStr(6, Inp$, " ")
  ii = i
  Do
    ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Left(Inp, i - 1))
  Out$ = Format(x, "###0.0")
  Out$ = Space(7 - Len(Out$)) + Out$
  
  j = InStr(ii, Inp$, " ")
  jj = j
  Do
    jj = jj + 1
  Loop Until Mid$(Inp$, jj, 1) <> " "
  x = Val(Mid$(Inp$, i, j - i))
  temp$ = Format(x, "#####0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  i = InStr(jj, Inp$, " ")
  ii = i
  Do
  ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Mid(Inp, jj, i - jj))
  temp$ = Format(x, "#####0.0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  j = InStr(ii, Inp$, " ")
  jj = j
  Do
    jj = jj + 1
  Loop Until Mid$(Inp$, jj, 1) <> " "
  x = Val(Mid$(Inp$, i, j - i))
  temp$ = Format(x, "###0.0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  
  i = InStr(jj, Inp$, " ")
  ii = i
  Do
  ii = ii + 1
  Loop Until Mid$(Inp$, ii, 1) <> " "
  x = Val(Mid(Inp, jj, i - jj))
  temp$ = Format(x, "#####0")
  temp$ = Space(7 - Len(temp$)) + temp$
  Out$ = Out$ + temp$
  i = Len(Out$)
  Out$ = Out$ + Space(77 - i + 1)
  Inp$ = Out$

End Sub


Sub WMOread(lu%, Record&)
Dim a As WMOrecord, i%

  Get #lu%, Record, a
    
    WMOnumber = a.WMOnumber
    WMO4l = a.WMO4l
    WMO4l = Trim(WMO4l)
    WMOstation = a.WMOstation
    WMOstate = a.WMOstate
    WMOcountry = a.WMOcountry
    WMOregionID = a.WMOregionID
    WMOlatitude = a.WMOlatitude
    WMOlongitude = a.WMOlongitude
    WMOaltitude = a.WMOaltitude

End Sub

Sub WMOwrite()
Dim INPlu%, OUTlu%, Record&
Dim a As WMOrecord, Line$
' this routine is only used to convert .txt listing of WMO sites
' to a random access version of the same thing.
' MJM 19981224

  INPlu = FreeFile
  Open "D:\RAOB\wmosites.txt" For Input As INPlu
  OUTlu = FreeFile
  Open "D:\RAOB\wmosites.WMO" For Random Access Read Write As OUTlu Len = Len(WMO)
  Record = 0
  Do
    Do: Line Input #INPlu, Line$: Loop Until Mid$(Line$, 56, 4) <> "    "
    Record = Record + 1
'   123456789012345678901234567890123456789012345678901234567890
'   WMOstn 333 Name                ST CY Latitude Longitud Altm
'    71892 YVR VANCOUVER INTL ARPT BC CN   49.183 -123.167 0003
  
    WMOnumber = Left$(Line$, 6)
    WMO4l = Mid$(Line$, 8, 3)
    WMOstation = Mid$(Line$, 12, 19)
    WMOstate = Mid$(Line$, 22, 2)
    WMOcountry = Mid$(Line$, 35, 2)
    WMOlatitude = Mid$(Line$, 38, 8)
    WMOlongitude = Mid$(Line$, 47, 8)
    WMOaltitude = Mid$(Line$, 56, 4)

    a.WMOnumber = WMOnumber
    a.WMO4l = WMO4l
    'a.WMOcity = WMOcity
    a.WMOstate = WMOstate
    a.WMOcountry = WMOcountry
    a.WMOlatitude = WMOlatitude
    a.WMOlongitude = WMOlongitude
    a.WMOaltitude = WMOaltitude
  
    Put #OUTlu%, Record, a

  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
  
End Sub
Sub ConvertBLG(INPlu%)
Dim Line$, i%, j%, Term!, dpde!
Dim Indx%(1 To 60), Ilev%, n1%, n2%
Dim FGZ!(1 To 60), FPZ!(1 To 60), FTZ!(1 To 60), FTD!(1 To 60)
'123456789012345678901234567890
' 72469   DEN   971112  12Z   P[mb], Zp[m], Zg[m], T[C], DPD[C]
'
'#00000.0 #00000 #00000 #00000"
'   711.0   2889  99999   -4.5
'   700.0   3012   2940   -4.7
' 72387   FUJ   981120  12Z    36.62   -116.02
'
'   954.6    502 999999   -0.1
'   938.7    637 999999   -2.2
'   887.9   1100   1200   -3.3

  Line$ = fReadLF(INPlu)
  WMOnumber = Val(Left$(Line$, 6))
  WMO4l = Mid$(Line$, 10, 4)
  Iyear = 1900 + Mid$(Line$, 16, 2)
  Imonth = Mid$(Line$, 18, 2) * 100
  Iday = Mid$(Line$, 20, 2)
  Ihour = Mid$(Line$, 24, 2)
  Imin = 0
  Ihhmm = 100 * Ihour
  Line$ = fReadLF(INPlu)
  Nlev = 0
  Do
    Do:  Line$ = fReadLF(INPlu): Loop Until Line$ <> "" Or EOF(INPlu)
    If EOF(INPlu) Then
      If Nlev > nRAOBlevels Then RAOBtype = 2 Else RAOBtype = 1
      For i = 1 To Nlev
        If ZGraob(i) = 0 Then
          For j = i To 1 Step -1   'look below
            If ZGraob(j) <> 0 Then n1 = j: Exit For
          Next j
          If n1 = 0 Then ' If none below, look above
            For j = i To Nlev        ' look above
              If ZGraob(j) <> 0 Then n1 = j: Exit For
            Next j
          End If
        
          For j = n1 + 1 To Nlev      ' look above
            If ZGraob(j) <> 0 Then n2 = j: Exit For
          Next j
          If n2 = 0 Then
            n2 = n1
            For j = n2 To 1 Step -1   'look below
              If ZGraob(j) <> 0 Then n1 = j: Exit For
            Next j
          End If
          ZGraob(i) = ZGraob(n1) + (ZGraob(n2) - ZGraob(n1)) * (fPtoZ(PZraob(i)) - fPtoZ(PZraob(n1))) / (fPtoZ(PZraob(n2)) - fPtoZ(PZraob(n1)))
        End If
      Next i
      Exit Sub
    End If
    Nlev = Nlev + 1
    PZraob(Nlev) = Left$(Line$, 8)                 'mb
    
    If Mid$(Line$, 18, 5) = "99999" Then
      ZGraob(Nlev) = 0#
    Else
      ZGraob(Nlev) = Val(Mid$(Line$, 18, 5) / 1000)   ' expects km
    End If

    TZraob(Nlev) = (273.15 + Val(Mid$(Line$, 23, 7)))
    If Len(Line$) > 30 Then  'Read Dew Point Depression
      dpde = Right$(Line$, Len(Line$) - 30)
      RZraob(Nlev) = fTdTaToRH(TZraob(Nlev), TZraob(Nlev) + dpde, False)
    Else
      RZraob(Nlev) = 0#
    End If
    'Debug.Print Nlev; ZGraob(Nlev); PZraob(Nlev); TZraob(Nlev)
  Loop Until EOF(INPlu%) 'Or Nlev > NraobLevels
' if Nlev > 70 advance to next RAOB in file (add code here)
End Sub

Sub ConvertEDW(INPlu%)
Dim Line$, i%, Term!
Dim Indx%(1 To 60), Ilev%, ZtSave!
Dim FGZ!(1 To 60), FPZ!(1 To 60), FTZ!(1 To 60), FTD!(1 To 60)
'RT023142016
'TEST NBR 00456
'RADIO THEODOLITE
'EDWARDS AFB, CA
'1234567890123456
'1900Z  10 NOV 98

'  ALT  DIR   SPD  SHR  TEMP  DPT   PRESS   RH ABHUM DENSITY I/R V/S  VPS  PW
'GEOMFT DEG   KTS /SEC DEG C DEG C   MBS   PCT  G/M3   G/M3   N  KTS  MBS  MM
'
'  2372  70   1.0 .000  12.4  -2.6  936.60  35  3.82 1140.36 278 659  5.04  0
'  2500 158   3.6 .012  10.1  -6.8  932.42  30  2.80 1145.05 272 657  3.66  0
'TERMINATION       74513 GEOPFT  22712 GEOPM   36.0 MBS
'TROPOPAUSE   37080  FEET   224.60 MB  -57.9 C  -65.3 C
'
'MANDATORY LEVELS
'GEOPFT DIR KTS  TEMP   DPT PRESS   RH
'1234567890123456789012345678901234567890123456789012345
'  2717 182   3   9.1  -7.5  925.0  30
'  5010 272  11   7.4 -18.9  850.0  13
'
' 67707 278  23 -58.1 -66.0   50.0  33
'SIGNIFICANT LEVELS
'GEOMFT DIR KTS  TEMP   DPT PRESS   IR  RH
'
'  2372  70   1  12.4  -2.6  936.6 278  35
'  2805 194   4   8.9  -7.5  921.9 270  31
'
' 74778 225  17 -57.0 -65.3   36.0  13  32
'TERMINATION
  For i = 1 To 5: Line$ = fReadLF(INPlu): Next i
  WMOnumber = "72381"
  WMO4l = "EDW"
  Iyear = 1900 + Mid$(Line$, 15, 2)
  Imonth = Month(Mid$(Line$, 8, 9))
  Iday = Mid$(Line$, 8, 2)
  Ihhmm = Left$(Line$, 4)
  Ihour = Int(Ihhmm / 100)
  Imin = Ihhmm - 100 * Ihour
  Do:  Line$ = fReadLF(INPlu)
  Loop Until Left$(Line$, 11) = "TERMINATION"
  Term = Val(Mid$(Line$, 45, 6))    'mb
  Line$ = fReadLF(INPlu)
  If Mid$(Line$, 13, 6) = "999999" Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
  Else
    ZtSave = Val(Mid$(Line$, 27, 7))
  End If
  For i = 1 To 5: Line$ = fReadLF(INPlu): Next i 'skip to first Mandatory Level
  
  FGZ(1) = Val(Left$(Line$, 6)) / 3.2808 'm
  FPZ(1) = Val(Mid$(Line$, 28, 6))  'mb
  FTZ(1) = 273.15 + Val(Mid$(Line$, 15, 6)) 'K
  FTD(1) = 273.15 + Val(Mid$(Line$, 21, 6)) 'K
  FTD(1) = fTdTaToRH(FTZ(1), FTD(1), False)  'Convert Dew Point to RH %
  Ilev = 1
  Do
    Line$ = fReadLF(INPlu)
    If Left$(Line$, 11) = "SIGNIFICANT" Then   ' Skip to significant levels
      For i = 1 To 3: Line$ = fReadLF(INPlu): Next i
    End If
    If Left$(Line$, 4) <> "TERM" Then
      If Val(Mid$(Line$, 28, 6)) >= Term Then 'Skip levels above termination
        Ilev = Ilev + 1
        FGZ(Ilev) = Val(Left$(Line$, 6)) / 3.2808 'm
        FPZ(Ilev) = Val(Mid$(Line$, 28, 6))  'mb
        If FPZ(Ilev) <> FPZ(Ilev - 1) Then
          FTZ(Ilev) = 273.15 + Val(Mid$(Line$, 15, 6)) 'K
          FTD(Ilev) = 273.15 + Val(Mid$(Line$, 21, 6)) 'K
          FTD(Ilev) = fTdTaToRH(FTZ(Ilev), FTD(Ilev), False)  'Convert Dew Point to RH %
          If Val(Mid$(Line$, 28, 6)) = ZtSave Then
            Ztrop = fPtoZ(ZtSave)
            Ttrop = FTZ(Ilev)
          End If
        Else
          Ilev = Ilev - 1
        End If
      End If
    End If
  Loop Until Left$(Line$, 11) = "TERMINATION" Or Ilev = nRAOBlevels
'  Close INPlu
  Nlev = Ilev
' Sort according to increasing pressure altitude [mb]
  Call indexx(Ilev, FPZ(), Indx())
' Convert RAOB just read to proper format for Random output
  For i = 1 To Ilev    'Write out as increasing pressure altitude [m]
    If FGZ(Indx(Ilev - i + 1)) = -1 Then
      ZGraob(i) = 0#
    Else
      ZGraob(i) = FGZ(Indx(Ilev - i + 1)) / 1000#  ' expects km
    End If
    PZraob(i) = FPZ(Indx(Ilev - i + 1))
    ZPraob(i) = fPtoZ(PZraob(i))
    If PZraob(i) = ZtSave Then Ltrop = i
    TZraob(i) = FTZ(Indx(Ilev - i + 1))
    RZraob(i) = FTD(Indx(Ilev - i + 1))
  Next i
 
End Sub

Sub ConvertEvenMakky(InFile$, OUTfile$)
Dim INlu%, OUTlu%, a$, h1$, h2$, h3$, b$, V!(1 To 20)

INlu% = FreeFile
Open InFile$ For Input As INlu%
OUTlu% = FreeFile
Open OUTfile$ For Output As OUTlu

h1$ = "      IDN  DAY[SYD] TIME[HMS]      Z[M]  LEV      T[K]     TD[K]  SPD[MPS]  DIR[DEG]"
h2$ = "--------- --------- --------- --------- ---- --------- --------- --------- ---------"
h3$ = "########0 ###0.0 ###0.00 #####0.00 ######0.0 ########0"
Print #OUTlu, h1$     'write header
Print #OUTlu, h2$
Line Input #INlu, a$  'skip header
Line Input #INlu, a$

Do
b$ = ""
Line Input #INlu, a$
If Left$(a$, 6) = "Number" Then Close INlu, OUTlu: Exit Sub
b$ = Left$(a$, 30)
V(1) = -1#    'SIGT files have no Zg
If Len(Mid$(a$, 36, 9)) > 0 Then    'Read Zp
  V(2) = Val(Mid$(a$, 36, 9) / 10#)
Else
  V(2) = 0#
End If
If Len(a$) > 55 And Mid$(a$, 46, 9) <> "         " Then  'Get Td
  V(4) = Val(Mid$(a$, 46, 9) / 100#)
Else
  V(4) = 0#
End If
If Len(Mid$(a$, 56, 9)) > 0 Then  ' And Ta
  V(3) = Val(Mid$(a$, 56, 9) / 100#)
Else
  V(3) = 0#
End If
V(5) = 0#
V(6) = 0#
If V(2) > 0 Then Print #OUTlu, b$ + fUsing$(h3$, V!())
Loop Until EOF(INlu)

Close INlu, OUTlu

End Sub

Sub ConvertFSL(lu)
Dim NewRAOB As Boolean, dpt!, Tropmb!, LineType%, WindMax!
Dim SoundingLevels%, TropQuality!, DataSource!, WSunits%

Static a$
'
'      1          2          3          4          5          6           7
' LINTYP
'                                header lines
'    254        HOUR        DAY      MONTH       YEAR    (blank)     (blank)
'      1       WBAN#       WMO#        LAT        LON       ELEV       RTIME
'      2       HYDRO       MXWD      TROPL      LINES     TINDEX      SOURCE
'      3     (blank)      STAID    (blank)    (blank)      SONDE     WSUNITS
'
'                                Data lines
'      9    PRESSURE     HEIGHT       TEMP      DEWPT   WIND DIR    WIND SPD
'000000000111111111122222222223333333333444444444455555555556666666666777777
'123456789012345678901234567890123456789012345678901234567890123456789012345
'    254     11      2      JAN    1997
'      1   3197  72381  34.90 117.92    724  99999
'      2    700   1410   1410     64  99999      3
'      3           EDW                99999     ms
'      9   9320    724    150    134    220     46
'      4  10000    120  99999  99999  99999  99999
'      4   9250    790    144    115    240    113
'      6   9116    914  99999  99999    245    134
NewRAOB = True
Nlev = 0
Ztrop = 0#: Ttrop = 0#: Ltrop = 0
If a$ = "" Then a$ = fReadLF(INPlu) 'Very first record
LineType = Val(Left$(a$, 7))
If LineType <> 254 Then                'Catch up to 254 line
  Do
    a$ = fReadLF(INPlu)
    LineType = Val(Left$(a$, 7))
  Loop Until LineType = 254 Or EOF(INPlu)
  If EOF(INPlu) Then Exit Sub
End If

Do
  If Not NewRAOB Then a$ = fReadLF(INPlu)
  'Debug.Print a$; Nlev
  LineType = Val(Left$(a$, 7))
  If Not NewRAOB And LineType = 254 Then Exit Sub 'Next RAOB
  
  Select Case LineType
  Case 1 'Station ID line
    WMOnumber = Val(Mid$(a$, 17, 5))
    WMOlatitude = Val(Mid$(a$, 22, 7))
    WMOlongitude = Val(Mid$(a$, 29, 7))
    WMOaltitude = Val(Mid$(a$, 38, 5))
    
  Case 2 'Sounding checks line
    WindMax = Val(Mid$(a$, 17, 5)) / 10#
    'Tropmb = Val(Mid$(a$, 24, 5)) / 10#  ' Only seems valid if "7" line (see below)
    'Ztrop = fPtoZ(Tropmb)
    SoundingLevels = Val(Mid$(a$, 33, 3))
    TropQuality = Val(Mid$(a$, 38, 5))
    DataSource = Val(Mid$(a$, 45, 5))
    
  Case 3 'Station identifier and other indicators
    WMO4l = Trim(Mid$(a$, 18, 4))
    WSunits = Mid$(a$, 48, 2)  'ms or kt
    
  Case 4, 5, 7, 9 'Mandatory, Significant, Tropopause and surface levels
    If NewRAOB Then NewRAOB = False: Nlev = 0
    Nlev = Nlev + 1
    If Mid$(a$, 8, 7) = "  99999" Then
      PZraob(Nlev) = 0#: ZPraob(Nlev) = 0#
    Else
      PZraob(Nlev) = Val(Mid$(a$, 8, 7)) / 10#              'mb
      ZPraob(Nlev) = fPtoZ(PZraob(Nlev))
    End If
    
    If Mid$(a$, 15, 7) = "  99999" Then
      ZGraob(Nlev) = 0#
    Else
      ZGraob(Nlev) = Val(Mid$(a$, 15, 7) / 1000)   ' expects km
    End If
    
    If Val(Mid$(a$, 22, 7)) = "  99999" Then
      TZraob(Nlev) = 0#
    Else
      TZraob(Nlev) = 273.15 + Val(Mid$(a$, 22, 7)) / 10#
    End If
    ' Save Tropopause altitude and temperature
    If LineType = 7 Then Ttrop = TZraob(Nlev): Ztrop = fPtoZ(PZraob(Nlev)): Ltrop = Nlev
    
    If Val(Mid$(a$, 29, 7)) = "  99999" Then
      RZraob(Nlev) = 0#
    Else
      dpt = 273.15 + Val(Mid$(a$, 29, 7)) / 10#
      RZraob(Nlev) = fTdTaToRH(TZraob(Nlev), dpt, False)
    End If
    If TZraob(Nlev) = 0 Then  'Throw out levels with no T
      Nlev = Nlev - 1
    Else
      If Nlev > 1 Then  'Throw out redundant levels (div by zero problem)
        If ZPraob(Nlev) = ZPraob(Nlev - 1) Then Nlev = Nlev - 1
      End If
    End If
  Case 6 'Wind only level
  Case 8 'Maximum wind level
  Case 254 'New sounding with time information
    NewRAOB = False
    Iyear = Val(Right$(a$, 4))
    Imonth = Month(Mid$(a$, 20, 19))
    Iday = Val(Mid$(a$, 20, 2))
    Ihour = Val(Mid$(a$, 13, 2))
    Imin = 0
    Ihhmm = 100 * Ihour
  End Select
Loop Until Nlev = nRAOBlevels Or EOF(INPlu)
End Sub
Sub ConvertPfister(INPlu%)

End Sub
Sub ConvertSJK(INPlu%)
Dim Idum%, i%
'  26.431    20.0  222.86   0.00
'  28.308    15.0  222.86   0.00
'    2 80  2 26 12 23
'   0.000  1014.0  299.56  83.51
'   0.122  1000.0  297.96  83.76
  
  Input #INPlu%, Idum%, Iyear%, Imonth%, Iday%, Ihour%, Nlev
  For i = 1 To Nlev
    Input #INPlu%, ZGraob(i), PZraob(i), TZraob(i), RZraob(i)
    ZPraob(i) = fPtoZ(PZraob(i))
  Next i

End Sub


Sub ConvertWyomingOld(INPlu%, First$)
Dim Line$, i%, yy%, i1%, i2%, i0%
'12345678901234567890
'Searching the city database file for: DRA ...
'Date:1200Z  9 NOV 98
'Station: DRA
'WMO ident:  72387
'Latitude:   36.62
'Longitude: -116.02
'-------------------------------------------------------------------------------
'LEV PRES  HGHT  TEMP  DEWP  RH  DD   WETB DIR SPD THETA THE-V THE-W THE-E   W
'     mb     m     C     C    %   C     C  deg knt   K     K     K     K    g/kg
'-------------------------------------------------------------------------------
'  0 1000   102
'  1  925   753
'SFC  897  1003   0.6   0.2  97  0.4   0.4 110   6 282.4 283.1 278.9 294.5  4.32
'  3  888  1085   1.2  -0.1  91  1.3   0.6  78   7 283.8 284.6 279.5 295.9  4.27
'1234567890123456789012345678901234567890123456789012345678901234567890123456789
' 37   10 30450 -58.3 -68.3  27 10.0 -60.3 295  23 801.9 802.1 327.4 805.8  0.39
' 38  9.4 30840 -57.9 -67.9  27 10.0 -60.0         817.8 818.0 327.8 822.2  0.44
'TRP  432  6495 -38.7 -44.7  53  6.0 -39.1 300  36 298.1 298.1 280.7 298.6  0.17
'WND  209 11422 -48.3 -59.5  26 11.2 -48.7 295  91 351.9 351.9 297.1 352.1  0.06
  Nlevr = 0
  If Mid$(First$, 2, 5) = "Date:" Then 'Modified old format
    i0 = 7
    i1 = 13
    i2 = 20
    Line$ = First$
  Else
    i0 = 6
    i1 = 12
    i2 = 19
    Line$ = fReadLF(INPlu)
  End If
  yy% = Val(Mid$(Line$, i2, 2))    '20
  If yy% > 80 Then  'after 1980 then treat as being 20th millenium
    Iyear = 1900 + yy%
  Else
    Iyear = 2000 + yy%
  End If
  Imonth = Month(Mid$(Line$, i1, 9))  '13
  Iday = Mid$(Line$, i1, 2)    '13
  Ihhmm = Mid$(Line$, i0, 4)    '7
  Ihour = Int(Ihhmm / 100)
  Imin = Ihhmm - 100 * Ihour

  Line$ = fReadLF(INPlu)
  WMO4l = Trim$(Mid$(Line$, 10, 4))
  Line$ = fReadLF(INPlu)
  WMOnumber = Val(Mid$(Line$, 13, 5))
  Line$ = fReadLF(INPlu)
  WMOlatitude = Val(Mid$(Line$, 12, 7))
  Line$ = fReadLF(INPlu)
  WMOlongitude = Val(Mid$(Line$, 12, 7))

  ' Skip to surface
  Do: Line$ = fReadLF(INPlu): Loop Until Left$(Line$, 3) = "   "
  Line$ = fReadLF(INPlu)  'Skip ------------------ line
  Line$ = fReadLF(INPlu)
  If Left$(Line$, 3) <> "SFC" Then
    If Mid$(Line$, 15, 6) = "      " Then 'skip to SFC
      Do: Line$ = fReadLF(INPlu)
      Loop Until Left$(Line$, 3) = "SFC"
    Else
    ' RAOB must be missing low levels, start with whatever is there
    End If
  End If
  PZraob(1) = Val(Mid$(Line$, 4, 5))               ' mb
  If Mid$(Line$, 10, 5) = "     " Then Exit Sub    ' Geopotential Height is missing
  ZGraob(1) = Val(Mid$(Line$, 10, 5) / 1000#)       ' expects km
  TZraob(1) = (273.15 + Val(Mid$(Line$, 15, 6)))
  RZraob(1) = Val(Mid$(Line$, 28, 3))
  RVraob(1) = fRhoV(RZraob(1), TZraob(1))
  Nlevr = 1
  Line$ = fReadLF(INPlu)
  Do
    Nlevr = Nlevr + 1
    PZraob(Nlevr) = Val(Mid$(Line$, 4, 5))               ' mb
    ZPraob(Nlevr) = fPtoZ(PZraob(Nlevr))
    If ZPraob(Nlevr) <> ZPraob(Nlevr - 1) Then
      If Mid$(Line$, 10, 5) = "     " Then
        ZGraob(Nlevr) = 0#
      Else
        ZGraob(Nlevr) = Val(Mid$(Line$, 10, 5) / 1000#)       ' expects km
      End If
      TZraob(Nlevr) = (273.15 + Val(Mid$(Line$, 15, 6)))
      RZraob(Nlevr) = Val(Mid$(Line$, 28, 3))
      RVraob(Nlevr) = fRhoV(RZraob(Nlevr), TZraob(Nlevr))
'   Debug.Print Nlevr; ZGraob(Nlevr); PZraob(Nlevr); TZraob(Nlevr)
    Else  'Redundant levels cause RC program to crash
      Nlevr = Nlevr - 1
    End If
    Line$ = fReadLF(INPlu)
  Loop Until Left$(Line$, 3) = "TRP" Or Nlevr >= nRAOBlevels Or EOF(INPlu)
  If Nlevr > nRAOBlevels Then
    Nlevr = nRAOBlevels
    If Left$(Line$, 3) <> "TRP" Then
      Do: Line$ = fReadLF(INPlu)
      Loop Until Left$(Line$, 3) = "TRP"
    End If
  End If
  If Mid$(Line$, 4, 5) = "     " Then
    Ztrop = 0#: Ttrop = 0#: Ltrop = 0
  Else
    Ztrop = fPtoZ(Val(Mid$(Line$, 4, 5)))    'convert mb to km
    Ttrop = (273.15 + Val(Mid$(Line$, 15, 6)))
    i = 0: Ltrop = 0
    Do
      i = i + 1
      If PZraob(i) <= Val(Mid$(Line$, 4, 5)) Then
        Ltrop = i
        'Debug.Print Record; i; Nlevr
      End If
    Loop Until Ltrop <> 0
  End If
  IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)
 
End Sub

Sub FindRAOBtropOld(z!(), T!(), Nlev%, Zt!, Tt!, Lt%)
Dim i%, j%, Ztop!, Zlo!, Zhi!, Tlo!, Thi!, ThicknessCrit!
Dim Nsteps%, Zstep!, Z1!, T1!, Z2!, T2!, LRi!, LRavg!

  ThicknessCrit = 2#
  Nsteps = 40                '50 meter steps
  Zstep = ThicknessCrit / Nsteps
  Zt = 0#: Tt = 0#

' Roe uses 1.0 km, but my retrievals still have artifact inflections at low end
j = 0: Ztop = z(Nlev)
' Find first level above 500 mb = 5.6 km
Do
  j = j + 1:
  If j > Nlev Then Exit Sub
  Zlo = z(j)
Loop While Zlo < 5.6

' Check for LR>-2 K/km on any RAOB segment
TryAnotherZ:
Do
  Zlo = z(j):  Tlo = T(j)
  If j + 1 > Nlev Then Exit Sub
  Zhi = z(j + 1)
  Thi = T(j + 1)
  LRi = (Thi - Tlo) / (Zhi - Zlo)
  j = j + 1
  'Debug.Print Zlo; Zhi; Tlo; Thi; LRi
Loop Until LRi >= -2
  
' OK, now check that AVERAGE LR over next 2 km is >-2 K/km
  Zhi = Zlo + ThicknessCrit
  Thi = fTinterp(Zhi, z(), T(), Nlev)
  If Thi = 0# Then Exit Sub  'Ran out of RAOB
  LRavg = 0#
  i = 0
  For Z1 = Zlo To Zhi - Zstep / 2 Step Zstep
    i = i + 1
    Z2 = Z1 + Zstep
    T1 = fTinterp(Z1, z(), T(), Nlev)
    T2 = fTinterp(Z2, z(), T(), Nlev)
    LRi = (T2 - T1) / (Z2 - Z1)
    LRavg = LRavg + LRi
    'Debug.Print i; Z1; Z2; LRi; LRavg / i
  Next Z1
  LRavg = LRavg / Nsteps
  If LRavg <= -2 Then GoTo TryAnotherZ

  Zt = Zlo
  Tt = Tlo
  Lt = j - 1
End Sub

Sub FindRAOBtrop(z!(), T!(), Nlev%, Zt1!, TT1!, LT1%, Zt2!, TT2!, Lt2%)
Dim i%, j%, Ztop!, Zlo!, Zhi!, Tlo!, Thi!, ThicknessCrit!, DeltaTsum#
Dim Nsteps%, Zstep!, Z1!, T1!, Z2!, T2!, LRavg!, Z3!

' WMO Tropopause Definition
' -------------------------
' From "A Temperature Lapse Rate Definition of the Tropopause Based on Ozone"
' J. M. Roe and W. H. Jasperson, 1981
' In the following discussion the lapse rate is defined as -dT/dz.
' The main features of the WMO tropopause definition are as follows:
' 1) The first tropopause (i.e., the conventional tropopause) is defined as the
'    lowest level at which the lapse rate decreases to 2 K/km or less, and the
'    average lapse rate from this level to any level within the next higher 2 km
'    does not exceed 2 K/km.
' 2) If above the first tropopause the average lapse rate between any level and
'    all higher levels within 1 km exceed 3 K/km, then a second tropopause is defined
'    by the same criterion as under the statement above. This tropopause may be
'    either within or above the 1 km layer.
' 3) A level otherwise satisfying the definition of tropopause, but occuring at an
'    altitude below that of the 500 mb level will not be designated a tropopause unless it
'    is the only level satisfying the definition and the average lapse rate fails to
'    exceed 3 K/km over at least 1 km in any higher layer.
'
' In this definition, the Lapse Rate is defined as dT/dz
' The main features of the WMO tropopause definition are as follows:
' 1) The first tropopause (i.e., the conventional tropopause) is defined as the
'    lowest level at which the lapse rate increases to -2 K/km or more, and the
'    average lapse rate from this level to any level within the next higher 2 km
'    does not go below -2 K/km.
' 2) If above the first tropopause the average lapse rate between any level and
'    all higher levels within 1 km goes below -3 K/km, then a second tropopause is defined
'    by the same criterion as under the statement above. This tropopause may be
'    either within or above the 1 km layer.
' 3) A level otherwise satisfying the definition of tropopause, but occuring at an
'    altitude below that of the 500 mb level will not be designated a tropopause unless it
'    is the only level satisfying the definition and the average lapse rate fails to
'    exceed 3 K/km over at least 1 km in any higher layer.

  Zt1 = 0#: TT1 = 0#: LT1 = 0
  Zt2 = 0#: TT2 = 0#: Lt2 = 0
  Zstep = 0.02
' Roe uses 1.0 km, but my retrievals still have artifact inflections at low end
  j = 0: Ztop = z(Nlev)
' Find first level above 500 mb = 5.6 km
  Do
    j = j + 1
    If j > Nlev Then Exit Sub
    Zlo = z(j)
  Loop While Zlo < 5.6
  j = j - 1
' Check for LR>-2 K/km on any RAOB segment
TryAnotherZ:   'enter with j=jlo-1
  Do
    j = j + 1
    If j + 1 > Nlev Then Exit Sub
    If z(j + 1) <> z(j) Then LRavg = (T(j + 1) - T(j)) / (z(j + 1) - z(j))
  Loop Until LRavg >= -2
  LT1 = j             ' lower level
' Check if Zhi is >Zlo+ThicknessCrit
  If z(LT1 + 1) - z(LT1) < 2# Then 'Average is >=-2
    i = LT1
    Do
      i = i + 1                  'i = Thi index on first pass
      If i > Nlev Then Exit Sub
    Loop Until z(i + 1) > z(LT1) + 2#
  ' Now interpolate last bit
    Zhi = z(LT1) + 2#
    Thi = fTinterpNew(Zhi, z(), T(), i, Nlev)
    If Thi = 0# Then Exit Sub  'Ran out of RAOB
    LRavg = (Thi - T(LT1)) / 2#
  End If
  If LRavg < -2 Then j = LT1: GoTo TryAnotherZ
' Have now found first tropopause over 2 km, now make sure LRavg > -2 throughout
  Zlo = z(LT1): i = 0:  DeltaTsum = 0#
  Do
    i = i + 1
    Zhi = Zlo + Zstep
    Tlo = fTinterpNew(Zlo, z(), T(), LT1, Nlev)
    Thi = fTinterpNew(Zhi, z(), T(), LT1, Nlev)
    DeltaTsum = DeltaTsum + Thi - Tlo
    LRavg = DeltaTsum / (Zstep * i)
    'Debug.Print i; Tlo; Zhi; LRavg
    If LRavg < -2 Then GoTo TryAnotherZ
    Zlo = Zhi
  Loop Until Zhi + Zstep > z(LT1) + 2#
  Zt1 = z(LT1): TT1 = T(LT1)

' OK, now check for range over which LR < -3 K/km above first tropopause
' Note interval changed to 2 km (instead of 1 km) as 1 km is too sensitive
' for RAOB data ie too many double (an not credible) trops
  Z3 = z(LT1) - Zstep
  Do
    Z3 = Z3 + Zstep
    Zhi = Z3 + 2#
    Tlo = fTinterpNew(Z3, z(), T(), LT1, Nlev)
    Thi = fTinterpNew(Zhi, z(), T(), LT1, Nlev)
    If Thi = 0# Then Exit Sub
    LRavg = (Thi - Tlo) / 2#
  Loop Until LRavg < -3
' Found such a region, now look for second tropopause starting at Z3
  j = LT1 - 1
' Find first level above Z3 starting at Zt1
  Do
    j = j + 1
    If j > Nlev Then Exit Sub
  Loop While z(j + 1) < Z3
' j is level below Z3

' Check for LR>-2 K/km on any RAOB segment above Z3
TryAnotherZ2:   'enter with j=jlo-1
  Do
    j = j + 1
    If j + 1 > Nlev Then Exit Sub
    LRavg = (T(j + 1) - T(j)) / (z(j + 1) - z(j))
  ' Debug.Print Zlo; Zhi; Tlo; Thi; LRi
  Loop Until LRavg >= -2
  Lt2 = j             ' lower level
' Check if Zhi is >Zlo+ThicknessCrit
  If z(Lt2 + 1) - z(Lt2) < 2# Then 'Average is >=-2
    i = Lt2
    Do
      i = i + 1                  'i = Thi index on first pass
      If i > Nlev Then Exit Sub
    Loop Until z(i + 1) > z(Lt2) + 2#
  ' Now interpolate last bit
    Zhi = z(Lt2) + 2#
    Thi = fTinterpNew(Zhi, z(), T(), i, Nlev)
    If Thi = 0# Then Exit Sub  'Ran out of RAOB
    LRavg = (Thi - T(Lt2)) / 2#
  End If
  If LRavg < -2 Then j = Lt2: GoTo TryAnotherZ2
' Have now found second tropopause over 2 km, now make sure LRavg > -2 throughout
  Zlo = z(Lt2): i = 0: DeltaTsum = 0#
  Do
    i = i + 1
    Zhi = Zlo + Zstep
    Tlo = fTinterpNew(Zlo, z(), T(), Lt2, Nlev)
    Thi = fTinterpNew(Zhi, z(), T(), Lt2, Nlev)
    DeltaTsum = DeltaTsum + Thi - Tlo
    LRavg = DeltaTsum / (Zstep * i)
    'Debug.Print i; Zhi; LRavg
    If LRavg < -2 Then GoTo TryAnotherZ2
    Zlo = Zhi
  Loop Until Zhi + Zstep > z(Lt2) + 2#
  Zt2 = z(Lt2): TT2 = T(Lt2)
  
End Sub

Function fTinterpNew(Zi!, z!(), T!(), iStart%, Nlev%)
Dim Zt!, Zb!, Tt!, TB!, j%
  j = iStart - 1
' Enter with Zi and exit with T from interpolation in z() & t()
  If Zi <= z(1) + 0.01 Then fTinterpNew = 0#: Exit Function
' Find first level Zt above Zi
  Do
    j = j + 1
    If j > Nlev Then fTinterpNew = 0#: Exit Function
    Zt = z(j)
  Loop While Zi > Zt
' And interpolate temperature
  Zb = z(j - 1): Tt = T(j): TB = T(j - 1)
  fTinterpNew = TB + (Tt - TB) * (Zi - Zb) / (Zt - Zb)

End Function


Function fTinterp(Zi!, z!(), T!(), Nlev%)
Dim Zt!, Zb!, Tt!, TB!, j%
j = 1
'Enter with z and exit with T from interpolation in z() & t()
  If Zi <= z(1) + 0.01 Then fTinterp = 0#: Exit Function
' Find first level above Zlo
  Do
    j = j + 1
    If j >= Nlev Then fTinterp = T(Nlev): Exit Function
    Zt = z(j)
  Loop While Zi > Zt
' And interpolate temperature
  Zb = z(j - 1): Tt = T(j): TB = T(j - 1)
  fTinterp = TB + (Tt - TB) * (Zi - Zb) / (Zt - Zb)

End Function

Sub ConvertRAOB(lu%, SourceFormat%, f As Form)
Dim Z1!, Z2!

TryAgain:    'Loop here if no data
' Assume everything good with no editting when converting files
RAOBgood = True: TropGood = True
EditLevel = False: EditTrop = False
UseEditLevel = False: UseEditTrop = False
Ztrop = 0#: Ttrop = 0#: Ltrop = 0
ZtropE = 0#: TtropE = 0#: LtropE = 0

Select Case SourceFormat
Case 0: ConvertFSL (lu)      ' FSL
Case 1: ConvertWyoming (lu) ' Wyoming
Case 2: ConvertBLG (lu)      ' BLG
Case 3: ConvertSJK (lu)      ' SJK
'Case 4: ConvertPfister (lu)  ' Pfister
Case 5: ConvertEDW (lu)      ' Edwards AFB
Case 6: ConvertTTY (lu)      ' TTY (UCSD, etc)
Case 7                       ' MAKKY
' Use Menu | Option | ConvertEvenMakky , first
'  ConvertMakky1              ' Combine all Makky*.CONV files
'  ConvertMakky2              ' Write separate files for each station to RS.nnnnn
'  ConvertMakky3              ' Extract individual RAOBs for each site from RS.nnnnn files
  ConvertMakky4              ' Sort levels and write to Random Access file
  Exit Sub
Case 8: ConvertPNNL (lu)      'PNNL short text files
Case 9: ConvertAndros (lu)
Case 10: ConvertCLS5 (lu)
Case 11: ConvertTAMU (lu)
End Select

If Nlevr = 0 And Nlev = 0 Then Exit Sub 'GoTo TryAgain
If Nlev <> 0 And Nlevr = 0 Then Nlevr = Nlev
' 2003.06.20 to avoid confusion with Nlev in ATP profiles created new parameter
' Nlevr for number of RAOB levels. Wyoming routines use Nlevr, but all the other
' conversion routines use Nlev. Therefore, make Nlevr=Nlev for these routines
' so as not to waste time changing all of them to use Nlevr
'
' Check if Trop to be determined
If Ztrop = 0# Then TropGood = False ' Set FALSE if no trop given

If f.chkFindTrop.Value = 1 Then
  Call FindRAOBtrop(ZPraob(), TZraob(), Nlevr, ZtropE, TtropE, LtropE, Ztrop2, Ttrop2, Ltrop2)
  If ZtropE > 0 Then  'Only use if different from given trop
    TropGood = True
    If ZtropE <> Ztrop Then
      EditTrop = True: UseEditTrop = True
      Ttrop = TtropE: Ztrop = ZtropE: Ltrop = LtropE
    End If
  Else
    RAOBgood = False 'Bad RAOB if no trop found
  End If
End If

' Calculate dTMRI for Good RAOBs
' NB Use pALT for consistency with trop solutions, calc using GeoPot Height if necessary
  Z1 = Val(f.txtT10.Text): Z2 = Val(f.txtT17.Text)
  dTMRI = fdTMRI(ZPraob(), TZraob(), Nlevr, Z1, Z2)

End Sub
Function fdTMRI!(Zraob!(), Traob!(), Nlev%, Z1!, Z2!)
Dim TZ1!, TZ2!, T1116!
' Return with temperature difference between Z1 and Z2 km
' NB although named dTMRI other altitude pairs may be provided
' Return with 999.0 if dTMRI not defined
' NB that Zraob and Traob can be either pALT or gALT
  If Nlev > 0 Then
  If Zraob(1) < Z1 And Zraob(Nlev) > Z2 Then
    TZ1 = fTinterp(Z1, Zraob(), TZraob(), Nlev)  'T at lower
    TZ2 = fTinterp(Z2, Zraob(), TZraob(), Nlev)  'T at upper
    fdTMRI = TZ1 - TZ2
  Else
    fdTMRI = 999#
  End If
  Else
    fdTMRI = 999#
  End If
End Function
Function fDeltaT(ALT1!, ALT2!, ZPraob!(), TZraob!(), Nlev%)
Dim T10!, T17!
' Return with temperature difference between ALT1 and ALT2 [km] , ALT1 < ALT2
' Return with 999.0 if fDeltaT not defined

If ZPraob(1) < ALT1 And ZPraob(Nlev) > ALT2 Then
  T10 = fTinterp(ALT1, ZPraob(), TZraob(), Nlev)  'T at lower
  T17 = fTinterp(ALT2, ZPraob(), TZraob(), Nlev)  'T at upper
  fDeltaT = T10 - T17
Else
  fDeltaT = 999#
End If

End Function

Function fGAinterp!(PA!(), GA!(), i%, P!)
Dim slope!, j%
' Logarithmic interpolation of Geometric altitude with Pressure altitude
' i is upper level
  j = i - 1
  slope = Log(PA(j) / PA(i)) / (GA(i) - GA(j))
  fGAinterp = GA(j) - (1# / slope) * Log(P / PA(j))
End Function

Function fPAinterp!(GA!(), PA!(), i%, g!)
Dim j%, k%
' Interpolate geometric altitude to find pressure
If i = 1 Then k = 2 Else k = i
j = k - 1
If PA(k) = 0 Then
  fPAinterp = PA(j)
Else
  fPAinterp = PA(j) * Exp((GA(j) - g) / (GA(j) - GA(k)) * Log(PA(k) / PA(j)))
End If
End Function


Function fXAinterp!(GA!(), XA!(), i%, g!)
Dim slope!, j%, k%
'Linear interpolation in geometric altitude of T, RH
  If i = 1 Then k = 2 Else k = i
  j = k - 1
  If GA(k) = GA(j) Then
    slope = 0
  Else
    slope = (XA(k) - XA(j)) / (GA(k) - GA(j))
  End If
  fXAinterp = XA(j) + slope * (g - GA(j))
End Function

Function Get4l(WMOnumber) As String
Dim lu%, a$
lu = FreeFile
Open Drive$ + "\raob\wmosites.txt" For Input As lu
'1234567890
' 72591 RBL RED BLUFF MUNICIPAL CA US   40.150 -122.250 0108
' 72597 MFR MEDFORD             OR US   42.367 -122.867 0401
' 72606 PWM PORTLAND INTL JET   ME US   43.650  -70.317 0020
Do
  Line Input #lu, a$
  If Val(Left$(a$, 6)) = WMOnumber Then
    Get4l = Mid$(a$, 8, 3)
    Close lu
    DoEvents
    Exit Function
  End If
Loop Until EOF(lu)
Close lu
Get4l = "999"
End Function
Private Sub ConvertMakky4()
Dim INPlu%, INPfile$, Path$, OUTlu%, Line$, IDcount%, iD$(1 To 70)
Dim i%, YYYYMMDD$, RAOBrecord&
Dim h1$, h2$, a$, b$, V!(1 To 10), x$, OutRecord&, InRecord&
Dim OUTfile$, OUTpath$, Ext$, Indx%(1 To 70), Ilev%
Dim FGZ!(1 To 70), FPZ!(1 To 70), FTZ!(1 To 70), FTD!(1 To 70)

Path$ = Drive$ + "\DC8\Sonex\Papers\RS\"
'txtFileMask.Text = "RS1997*.*"
'INPfile$ = Dir(Path$ + txtFileMask.Text, vbNormal)
If INPfile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  INPlu% = FreeFile
  Open Path$ + INPfile$ For Input As INPlu%
  OutRecord = 0
  OUTlu = FreeFile
  OUTfile$ = Path$ + "SONEX.RAOB"
  Open OUTfile$ For Random Access Read Write As OUTlu Len = Len(RAOB)
End If
'      IDN  DAY[SYD] TIME[HMS]      Z[M]  LEV      T[K]     TD[K]  SPD[MPS]  DIR[DEG]
'--------- --------- --------- --------- ---- --------- --------- --------- ---------
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234
'     3026     97297    180000        -1   24.4  206.46      0.00       0.0         0
'     3026     97297    180000       234 1000    279.36    272.36
'     3026     97297    180000       866 925     274.76    272.36       7.7       315
'  Ilev As Integer
'  Trop As Single
'lblMakky.Caption = "Writing random Mission.RAOB file for this directory"
DoEvents
Ztrop = 0#
Ttrop = 0#
Ltrop = 0
ZtropE = 0#
TtropE = 0#
LtropE = 0

Do While INPfile$ <> ""
  OutRecord = OutRecord + 1
  WMOnumber = CLng(Val(Right$(INPfile$, 5)))
  Line$ = fReadLF(INPlu) ' Read first line of new raw RAOB file
  RAOBgood = True
  RAOBrecord = OutRecord
  Iyear = 1900 + Int(Val(Mid$(Line$, 15, 2)))
  Idoy = Int(Val(Mid$(Line$, 17, 3)))
  YYYYMMDD$ = fDOYtoYMD$(Iyear, Idoy)
  Imonth = Int(Val(Mid$(YYYYMMDD$, 5, 2)))
  Iday = Int(Val(Mid$(YYYYMMDD$, 7, 2)))
  Ihhmm = Int(Val(Mid$(Line$, 21, 9) / 100#))  'Ihour=hhmm
  Ihour = Int(Ihhmm / 100)
  Imin = Ihhmm - 100 * Ihour
  FGZ(1) = Val(Mid$(Line$, 31, 9))  'm
  FPZ(1) = Val(Mid$(Line$, 41, 6))  'mb
  FTZ(1) = Val(Mid$(Line$, 49, 6))  'K
  FTD(1) = Val(Mid$(Line$, 59, 6))  'K
  FTD(1) = fTdTaToRH(FTZ(1), FTD(1), False)  'Convert Dew Point to RH %
  Ilev = 1
  Do
    Line$ = fReadLF(INPlu)
    Ilev = Ilev + 1
    FGZ(Ilev) = Val(Mid$(Line$, 31, 9))  'm
    FPZ(Ilev) = Val(Mid$(Line$, 41, 6))  'mb
    FTZ(Ilev) = Val(Mid$(Line$, 49, 6))  'K
    FTD(Ilev) = Val(Mid$(Line$, 59, 6))  'K
    FTD(Ilev) = fTdTaToRH(FTZ(Ilev), FTD(Ilev), False)  'Convert Dew Point to RH %
  Loop Until EOF(INPlu) Or Ilev = nRAOBlevels
  Close INPlu
  Nlev = Ilev
' Sort according to increasing pressure altitude [mb]
  Call indexx(Ilev, FPZ(), Indx())
' Convert RAOB just read to proper format for Random output
'  GZr(1 To 60) As Integer                    'Geometric Altitude [m]
'  PZr(1 To 60) As Integer                    'Pressure altitude  [0.1mb]
'  TZr(1 To 60) As Integer                    'Temperatures [10 mK]
'  RZr(1 To 60) As Integer                    'Relative Humidity [0.01%]
  For i = 1 To Ilev    'Write out as increasing pressure altitude [m]
    If FGZ(Indx(Ilev - i + 1)) = -1 Then
      ZGraob(i) = 0#
    Else
      ZGraob(i) = FGZ(Indx(Ilev - i + 1)) / 1000# ' expects km
    End If
    PZraob(i) = FPZ(Indx(Ilev - i + 1))
    TZraob(i) = FTZ(Indx(Ilev - i + 1))
    RZraob(i) = FTD(Indx(Ilev - i + 1))
  Next i
' Write Random record
  Call RAOBwrite(OUTlu, OutRecord)
' Now open next input file
  INPfile$ = Dir
  If INPfile$ <> "" Then
    Open Path$ + INPfile$ For Input As INPlu%
  End If
Loop
Close OUTlu
'lblMakky.Caption = "Finished!"
DoEvents

End Sub
Sub ConvertTTY(INPlu)

End Sub
Function Tsign!(T$)
Dim Odd%
  Odd = 0
  If T$ = "///" Then Tsign = 999.9: Exit Function
  T = Val(T$) / 10
  If Val(Right$(T$, 1)) Mod 2 = 1 Then T = -T
  Tsign = T
End Function
Sub UAfromFlatfile()
Dim V!(1 To 5), OutRecord&, lu%, BINlu%, WMOnum&, a$, T1!, T2!, T3!
Dim WMOregion$

' Use WMO Flatfile to determine upper air stations
' Use Unidata/GEMPAK to get location, lat, lon, alt
' http://www.unidata.ucar.edu/software/gempak/tutorial/station_tables.html
Dim Flatfile$, INPlu%, OUTlu%, GPlu%, Field%, UAcount%, Record&, i%, j%, k%
Dim WMOarray&(1 To 1000, 1 To 3), OutCount%, x$

'GoTo PartTwo
' Set Initial directory
'cdConvertIn.InitDir = "C:\MTP\RAOB\BIN\"
' Set Size of FileName buffer
'cdConvertIn.MaxFileSize = 1024
' Set filters.
'cdConvertIn.Filter = "WMO Flatfile |*.flatfile|All Files (*.*)|*.*"
' Specify default filter.
'cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
'cdConvertIn.ShowOpen

'Flatfile$ = cdConvertIn.Filename

If Flatfile$ = "" Then
  Call MsgBox("Select a Flatfile!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  INPlu% = FreeFile
  Open Flatfile$ For Input As INPlu%
End If
'Volume A (Tab Delimited)
'RegionId  RegionName  CountryArea CountryCode StationId IndexNbr  IndexSubNbr StationName Latitude  Longitude Hp  HpFlag  Hha HhaFlag PressureDefId SO-1  SO-2  SO-3  SO-4  SO-5  SO-6  SO-7  SO-8  ObsHs UA-1  UA-2  UA-3  UA-4  ObsRems
'1 AFRICA / AFRIQUE  ALGERIA / ALGERIE 1030  57  60351 0 JIJEL ACHOUAT 36 53N  05 49E  10          X X X X X X X X H00-24  . . . . A;CLIMAT(C);EVAP;M/B
'1 AFRICA / AFRIQUE  ALGERIA / ALGERIE 1030  58  60353 0 JIJEL PORT  36 50N  05 47E  2         . . X X X X X . H06-18  . . . . C;CLIMAT(C);EVAP;M/B
'1 AFRICA / AFRIQUE ALGERIA / ALGERIE 1030 59 60354 0 EL MILIA 36 44N  03 54E  31          . . X X X X X . H06-18  . . . . CLIMAT(C);EVAP;M/B
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'Field     1           2           3           4         5         6           7           8         9         10  11      12  13      14            15    16    17    18    19    20    21    22    23    24    25    26    27    28    29
'RegionId  RegionName  CountryArea CountryCode StationId IndexNbr  IndexSubNbr StationName Latitude  Longitude Hp  HpFlag  Hha HhaFlag PressureDefId SO-1  SO-2  SO-3  SO-4  SO-5  SO-6  SO-7  SO-8  ObsHs UA-1  UA-2  UA-3  UA-4  ObsRems
'A$ = fReadLF(INPlu)
a$ = fReadLF(INPlu)
Record = 2
OutCount = 0
'lblRecord.Caption = "Flatfile Record ==>"
Do
  a$ = fReadLF(INPlu)
  Field = 1
  i = 1
  UAcount = 0
  Do
    j = InStr(i, a$, vbTab)
    Select Case Field
    Case 1
      WMOregion = Val(Left$(a$, j - 1))
    Case 6
      WMOnumber = Val(Mid$(a$, i, j - i))
    Case 25, 26, 27, 28
      x$ = Mid$(a$, i, j - i)
      If InStr(1, x$, "R") Then
        UAcount = UAcount + 1
      End If
    Case Else
    End Select
    'Debug.Print Field; Mid$(a$, i, j - i)
    i = j + 1
    Field = Field + 1
  Loop Until Field = 29
  If UAcount > 0 And WMOnumber > 0 Then
    OutCount = OutCount + 1
    WMOarray(OutCount, 1) = WMOnumber
    WMOarray(OutCount, 2) = WMOregion
    WMOarray(OutCount, 3) = UAcount
    DoEvents
  End If
  Record = Record + 1
  'txtRecord.Text = Str(Record)
  DoEvents
Loop Until EOF(INPlu)
Close INPlu
' Bubble Sort in increasing WMOnumber
For i = 1 To OutCount
  For j = i + 1 To OutCount
    If WMOarray(j, 1) < WMOarray(i, 1) Then  'Swap
      T1 = WMOarray(i, 1)
      T2 = WMOarray(i, 2)
      T3 = WMOarray(i, 3)
      WMOarray(i, 1) = WMOarray(j, 1)
      WMOarray(i, 2) = WMOarray(j, 2)
      WMOarray(i, 3) = WMOarray(j, 3)
      WMOarray(j, 1) = T1
      WMOarray(j, 2) = T2
      WMOarray(j, 3) = T3
    End If
  Next j
Next i
  
' OK now fill in the rest of the information from GEMPAK
'0        1         2         3         4         5         6         7
'1234567890123456789012345678901234567890123456789012345678901234567890123
'$$$$$$$$ %%%%%% $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $$ $$ %%%%% %%%%%% %%%%%
'VBCI      48109 COCO ISLAND                      -- BM  1412   9337     3
'VBG       72393 VANDENBERG AFB                   CA US  3465 -12057   112
'Type GPrecord
'  GPsite As String * 8
'  GPnumber As Long
'  GPstation As String * 32
'  GPstate As String * 2
'  GPcountry As String * 2
'  GPlatitude As Single
'  GPlongitude As Single
'  GPaltitude As Integer
'End Type

' Now Use this to generate GEMPAK_UA.txt file from GEMPAK.txt
  GPlu = FreeFile          'this is large file ~17,000 records
  Open "C:\MTP\RAOB\station-query.html" For Input As GPlu
  a$ = fReadLF(GPlu)  'Skip two HTML header lines
  a$ = fReadLF(GPlu)
  
  OUTlu = FreeFile
  Open "C:\MTP\RAOB\MTP_UA.TXT" For Output As OUTlu
  
  BINlu = FreeFile
  Open "C:\MTP\RAOB\MTP_UA.BIN" For Random Access Read Write As #BINlu Len = Len(GP)
  OutRecord = 0
  Record = 0
  'lblRecord.Caption = "GEMPAK Record ==>"
  Do
    a$ = fReadLF(GPlu)
    Record = Record + 1
    'txtRecord.Text = Str(Record)
    If Left$(a$, 6) = "</pre>" Then Exit Do  'Last HTML statement
    GPcards = Val(Mid$(a$, 10, 6))   ' Get GEMPAK WMO number
    For i = 1 To OutCount
      If GPcards = WMOarray(i, 1) Then  'Check if WMO site
'       Print #OUTlu, a$
        GPcards = GPcards * 10
        GPicao = Left$(a$, 8)
        GPstation = Trim(Mid$(a$, 17, 32))
        GPstate = Mid$(a$, 50, 2)
        GPcountry = Mid$(a$, 53, 2)
        GPregion = WMOarray(i, 2)
        GPlatitude = Val(Mid$(a$, 56, 5)) / 100#
        GPlongitude = Val(Mid$(a$, 62, 6)) / 100#
        GPaltitude = Val(Mid$(a$, 69, 5))
        ' MTP UA Format
        ' 123456 12345678 123456 1234567 12345 1 12 12 12345678901234567890123456789012 12
        ' %%%%%% $$$$$$$$ SNN.NN SNNN.NN NNNNN $ $$ $$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %%
        ' 12345678901234567890123456789012345678901234567890123456789012345678901234567890
        '          1         2         3         4         5         6         7         8
        x$ = Format(GPcards, "000000") + Format(GPicao, " @@@@@@@@")
        V(1) = GPlatitude:   x$ = x$ + " " + fUsing("##0.00", V())
        V(1) = GPlongitude:  x$ = x$ + " " + fUsing("###0.00", V())
        V(1) = GPaltitude:   x$ = x$ + " " + fUsing("####0", V())
        x$ = x$ + Format(GPregion, " 0") + Format(GPcountry, " @@") + Format(GPstate, " @@ ")
        x$ = x$ + GPstation + Space(32 - Len(GPstation)) + " 00"
        'Debug.Print Format(i, "000 ") + X$
        Print #OUTlu, x$
        OutRecord = OutRecord + 1
        GPwrite BINlu, OutRecord
        DoEvents
        Exit For
      End If
    Next i    'Check if GEMPAK site is next WMO site
  Loop Until EOF(GPlu)  'No more GEMPAK sites
  Close GPlu, BINlu, OUTlu
  'lblRecord.Caption = "Upper Air Records"
  'txtRecord.Text = Str(OutRecord)
End Sub
Sub indexx(n%, arr!(), Indx%())
' Index sort with Heap Sort from Numberical Recipes
' N<50
Dim M%, NSTACK%, a!
Dim i%, indxt%, ir%, itemp%, j%, jstack%, k%, L%, istack%(1 To 60)

  NSTACK = 60
  M = 7
  For j = 1 To n: Indx(j) = j: Next j
  jstack = 0
  L = 1
  ir = n
1:
  If (ir - L < M) Then
    For j = L + 1 To ir
      indxt = Indx(j)
      a = arr(indxt)
      For i = j - 1 To L Step -1
        If (arr(Indx(i)) <= a) Then GoTo 2
        Indx(i + 1) = Indx(i)
      Next i
      i = L - 1
2:
      Indx(i + 1) = indxt
    Next j
    If (jstack = 0) Then Exit Sub
    ir = istack(jstack)
    L = istack(jstack - 1)

    jstack = jstack - 2
  Else
    k = (L + ir) \ 2
    itemp = Indx(k)
    Indx(k) = Indx(L + 1)
    Indx(L + 1) = itemp
    If (arr(Indx(L)) > arr(Indx(ir))) Then
      itemp = Indx(L)
      Indx(L) = Indx(ir)
      Indx(ir) = itemp
    End If
    If (arr(Indx(L + 1)) > arr(Indx(ir))) Then
      itemp = Indx(L + 1)
      Indx(L + 1) = Indx(ir)
      Indx(ir) = itemp
    End If
    If (arr(Indx(L)) > arr(Indx(L + 1))) Then
      itemp = Indx(L)
      Indx(L) = Indx(L + 1)
      Indx(L + 1) = itemp
    End If
    i = L + 1
    j = ir
    indxt = Indx(L + 1)
    a = arr(indxt)
3:
    i = i + 1
    If (arr(Indx(i)) < a) Then GoTo 3
4:
    j = j - 1
    If (arr(Indx(j)) > a) Then GoTo 4
    If (j < i) Then GoTo 5
    itemp = Indx(i)
    Indx(i) = Indx(j)
    Indx(j) = itemp
    GoTo 3
5:
    Indx(L + 1) = Indx(j)
    Indx(j) = indxt
    jstack = jstack + 2
    If (jstack > NSTACK) Then Stop 'NSTACK too small in indexx'
    If (ir - i + 1 >= j - L) Then
      istack(jstack) = ir
      istack(jstack - 1) = i
      ir = j - 1
    Else
      istack(jstack) = j - 1
      istack(jstack - 1) = L
      L = i
    End If
  End If
  GoTo 1

End Sub
Function fTinterpP!(i%, z!)
Dim slope!, j%
' i is upper level
'Interpolate in pressure altitude
  j = i - 1
  If ZPraob(i) = ZPraob(j) Then
    fTinterpP = TZraob(j)
    Exit Function
  End If
  slope = (TZraob(i) - TZraob(j)) / (ZPraob(i) - ZPraob(j))
  fTinterpP = TZraob(j) + slope * (z - ZPraob(j))
End Function

