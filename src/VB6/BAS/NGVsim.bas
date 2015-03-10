Attribute VB_Name = "Module1"
Public Packet_NGV As String

' Packet NGV Parameters
Public gALT!, pALTft!, gALTfeet!, OATn!, OATnav!, Wspd!, Wdir!, UTsecNav&
Public Heading!, Pitch!, Roll!, HHMMSS$, pALT!, Latitude!, Longitude!
Public SynthMode%, cft_km!, UTsec&, UTks!, Ceiling!, BadFrameCounter&
Public Zero$, A$, B$, ReadMode%, WriteMode%, OUTlu%, NoComPortData As Boolean
Public FileOpen As Boolean, INPlu%, Frame&, TotalFrames&, White&, Gray&
Public Filename$, COMport%, COMbaud%, COMparity%, COMbits%
Public MScomm$, WriteHex As Boolean, V!(0 To 32)
' UDP Packet Parameters
Public UDP_Packet_In As String, UDP_Packet_Out As String
Public DatV!(0 To 32)
' New parameters
Public GroundSpeed!, TrackAngle!, P!, AttackAngle!, SideSlipAngle!
Public DriftAngle!, TAS!, IAS!, Zgps!, Mach!, Vz!, Tdew!, Ttotal!
Public Pstatic!, Pdynamic!, Pcabin!, VWS!, SZA!, SEAC!, SAG!, SAAC!
' Existing parameters
' Public pALTft!, gALTfeet!, OATn!, OATnav!, Wspd!, Wdir!, UTsecNav&
' Public Heading!, Pitch!, Roll!, Latitude!, Longitude!, HHMMSS$
' Public gALT!, rALT!, wALT!, pALT!, Wspd!,Wdir!
' Dim Packet2Ah As String, Packet2Bh(1 To 5) As String





Sub DecodeAline(A$, GoToNext As Boolean)
Dim NavSource$, X$, ALTkmu!, ALTcor!, dALT!
Static UTsecNAVold&, hhmmNavLast$, hhmmssNavLast$, ALTftLast!

  GoToNext = False
  NavSource$ = Mid$(A$, 2, 1)
  If NavSource$ = "W" And Val(yyyymmdd$) < 19990000 Then NavSource$ = "Y" 'WAM has different format
  Select Case NavSource$
  Case " ", "G", "N", "X", "W", "!"     'WB57 after WAM (ie ACCENT) is "W", for WAM is "Y"
' ER2 Format
'          1         2         3         4         5         6         7         8         9         10
' 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc yymmdd hhmmss
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc Sta yymmdd hhmmss
' AG266:22:02:32  -3.051 -159.368  64.2    .6   3.7 20479 20178 -65.9    .9   3.2 -188.9 1 255 970923 220222
    Latitude = Val(Mid$(A$, 15, 8)):   Longitude = Val(Mid$(A$, 23, 9))
    Heading = Val(Mid$(A$, 32, 6))
    Pitch = Val(Mid$(A$, 38, 6)):      Roll = Val(Mid$(A$, 44, 6))
    gALT = Val(Mid$(A$, 50, 6)) / 1000#   'gALT in km
    If gALT = 99.999 Then gALT = gALTlast          'GPS/geometric alt [km]
    gALTft = gALT * cft_km
    pALT = Val(Mid$(A$, 56, 6)) / 1000#   'pALT in km
    pALTft = pALT * cft_km
    If pALT > 23# Then pALT = 99.9
    'pALT = gALT
    'pALTft = gALTft
    OATn = Val(Mid$(A$, 62, 6))   'Default OAT is OATnav
    OATnav = OATn + cto           'Convert K
    Wspd = Val(Mid$(A$, 68, 6))
    Wdir = Val(Mid$(A$, 74, 6))
    If NavSource$ = "W" Then Wdir = 0: Wspd = 0 'xxx mjm 990422
'   DU data should always be present at end of A-line, decode from right side
    X$ = Right$(A$, 13)
    yymmddMTP$ = Left$(X$, 6)
    hhmmssMTP$ = Right$(X$, 6)  'use Right$ in case formatting error
    
  Case "Y", "!"
' WB57 Format for WAM
'          1         2         3         4         5         6         7         8         9"        10         11
' 12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123"
'   DOY:HH:MM:SS Latitude Longitud  Heading  Pitch   Roll   Z_GEO    pALT Temper ..Wspd ..Wdir sddd.d n yymmdd hhmmss"
'   ###:##:##:## S###.### S###.### S###.### S###.# S###.# S###### S###### S###.# S###.# S###.# "
'Final version:
' AW054:14:00:20 +027.681 +065.230 -128.200 +035.4 -023.8 +045323 +045323 +028.6 +158.6 +042.1
'Original version:
' AW054:17:42:53 +043.432 +122.321 +004.2 -038.2 +035.4 05941 05941 -008.6 128.4 085.5
    Latitude = Val(Mid$(A$, 16, 8)): Longitude = Val(Mid$(A$, 25, 8))
    Heading = Val(Mid$(A$, 34, 8))
    Pitch = Val(Mid$(A$, 43, 6)):    Roll = Val(Mid$(A$, 50, 6))
    gALTft = Val(Mid$(A$, 57, 7))
    gALT = gALTft / cft_km                   'gALT km
    pALTft = Val(Mid$(A$, 65, 7))
    pALT = pALTft / cft_km                   'pALT km
    OATn = Val(Mid$(A$, 73, 6))
    OATnav = OATn + cto
    Wspd = Val(Mid$(A$, 80, 5))
    Wdir = Val(Mid$(A$, 87, 5))
    Wdir = Wdir + 180
'   DU data should always be present at end of A-line, decode from right side
    X$ = Right$(A$, 13)
    yymmddMTP$ = Left$(X$, 6)
    hhmmssMTP$ = Right$(X$, 6)  'use Right$ in case formatting error
    
  Case "D", "E", "!"
' DC8 Format
'          1         2         3         4         5         6         7         8         9"
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
' AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 99999 99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
'   doy hh:mm:ss snn.lat snnn.lon   hdg ppp.p rr.r radar ALTft .OATn .Wspd .Wdir ScnDis P yymmdd hhmmss'
' TexAQS
' AE224 17:38:05  39.913 -105.117 x999 - 0.2 - 0.7 95960 05436  35.6 0007   0204   12.3 1 000811 173802
'                                 ^fix
' SOLVE
' AD 75 05:40:40  67.821   20.335  207    .3    .6 01589 02055 -13.4 0000   0000   11.9 1 000315 053809
    Latitude = Val(Mid$(A$, 15, 8)): Longitude = Val(Mid$(A$, 23, 9))
    Heading = Val(Mid$(A$, 32, 5))
    Pitch = Val(Mid$(A$, 37, 6)):    Roll = Val(Mid$(A$, 43, 6))
    gALTft = Val(Mid$(A$, 49, 6))
    If gALTft <> 99999 Then gALT = gALTft / cft_km Else gALT = gALTlast         'GPS/geometric alt [km]
    pALTft = Val(Mid$(A$, 55, 6))      'pressure altitude
    If pALTft <> 99999 Then pALT = pALTft / cft_km
    OATn = Val(Mid$(A$, 61, 6))
    OATnav = OATn + cto
    Wspd = Val(Mid$(A$, 68, 4))
    Wdir = Val(Mid$(A$, 74, 5))
    ElCor = Val(Mid$(A$, 79, 7))
    ElCorUsed = Val(Mid$(A$, 86, 2))
    yymmddMTP$ = Mid$(A$, 89, 6)
    hhmmssMTP$ = Mid$(A$, 96, 6)
 
  Case Else
    Call MsgBox("You are not using a valid A-line format!", vbOK)
    Exit Sub
  End Select
  
  If Abs(Latitude) > 90 Then Latitude = 99.999
  If Abs(Longitude) > 180 Then Longitude = 999.999
  If Heading < 0 Then Heading = Heading + 360
  If Heading > 360 Then Heading = 999
  If Abs(Pitch) > 90 Then Pitch = 999.9
  If Abs(Roll) > 90 Then Roll = 999.9
  'If Abs(Roll) > RollWarnCrit Then RollWarnFlg = True Else RollWarnFlg = False
  'xxx mjm 20000204 never used
  
  If Wspd > 999 Then Wspd = 999
  If Wdir > 999 Then Wdir = 999
  
  If gALT > Ceiling Then gALT = 99.9
  If gALT = 0 Then gALT = gALTlast Else gALTlast = gALT  'ER2 drops gALT occassionally
  If pALT > Ceiling Then pALT = 99.9
  If pALT < 0# Then pALT = 0#: pALTft = 0#

' Fix pALT if necessary
  If pALT = 99.9 Then
    pALTft = pALTlast
    ALTft = pALTft
    ALTkm = pALTft / cft_km
  Else
    pALTlast = pALTft
    If pALTft < 0 Then pALTft = 1
    ALTkm = pALTft / cft_km
    ALTkmu = ALTkm
    'ALTcor = .046 - .000437 * ALTkmu ^ 2 + 3.315E-05 * (ALTkmu ^ 3)
    ALTkm = ALTkmu + ALTfujCONST * ALTcor
    ALTft = Int(ALTkm * cft_km + 0.5)
  End If
  
  UTsecMTP = fTstringToSec(hhmmssMTP$, False)
  If Mid$(A$, 6, 8) <> "99:99:99" Then                  'Use NAV UT if it's present
    Doy = Val(Mid$(A$, 3, 3))
    UTsecNav = fTstringToSec(Mid$(A$, 7, 8), True)      'Nav has colons
    UTsec = UTsecNav
  Else
    Doy = 999
    UTsec = UTsecMTP
  End If
' Now fix time if necessary
  
  If Mid$(A$, 6, 8) <> "99:99:99" Then  'Use NAV UT if it's present
'    Doy = Val(Mid$(a$, 3, 3))
'    UTsecNAV = fTstringToSec(Mid$(a$, 7, 8), True)      'Nav has colons
    If UTsecNav < UTsecNAVold& And UTsecNav > 60 Then
      UTsecNav = UTsecNav + 60 'Fix WB57 NAV 60 s problem
    End If
    UTsecNAVold& = UTsecNav
    hhmmssNAV$ = fSecToTstring(UTsecNav, False)         'Get rid of colons
    If hhmmNavLast$ = "" Then hhmmssNavLast$ = hhmmssNAV$
'    UTsec = UTsecNAV
  Else
    hhmmssNAV$ = "999999"
'    UTsec = UTsecMTP
    GoToNext = True ' DADS not present at end of file
    Exit Sub
  End If


'  If UTtakeoff > 0 And AC$ = "ER" And Mission$ = "CAMEX4" Then
'    ALTkm = fZnavCorr(ALTkm, (UTsec - UTtakeoff) / 1000)
'  End If
  pALT = ALTkm         'Have some sort of valid altitude at this point
  pALTft = pALT * cft_km
  If ALTftLast = 0 Then ALTftLast = pALTft
  dALT = pALTft - ALTftLast
  ALTftLast = pALTft
      
  hhmmssNAV$ = fSecToTstring(UTsecNav, False)         'Get rid of colons
  If Heading > 180 Then Heading = 180 - Heading
  If Wdir > 180 Then Wdir = 180 - Wdir
End Sub
Function fZtoP!(Z!)
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

Dim P!, T!

T = fTstd(Z)
Select Case Z
Case Is <= 11
' P = 1013.25 * (T / 288.15) ^ (-1000 * cg / (cRs * -6.5)
  P = 1013.25 * (T / 288.15) ^ (5.2558761507598)
Case Is <= 20
' P = 226.3206 * Exp(-1000 * cg * (z - 11.) / (cRs * 216.65))
  P = 226.3206 * Exp(-0.157688414400825 * (Z - 11#))
Case Is <= 32
' P = 54.74888 * (T / 216.65) ^ (-1000 * cg / (cRs * 1.)
  P = 54.74888 * (T / 216.65) ^ (-34.1631949799387)
Case Is <= 47
' P = 8.680185 * (T / 228.65) ^ (-1000 * cg / (cRs * 2.8)
  P = 8.680185 * (T / 228.65) ^ (-12.2011410642638)
Case Is <= 51
' P = 1.109063 * Exp(-1000 * cg * (Zs(i) - Zs(i - 1)) / (cRs * 270.65))
  P = 1.109063 * Exp(-0.126226473230884 * (Z - 47))
Case Is <= 71
' P = 0.6693885 * (T / 270.65) ^ (-1000 * cg / (cRs * -2.8)
  P = 0.6693885 * (T / 270.65) ^ (12.2011410642638)
Case Is <= 84.852
' P = 0.03956419 * (T / 214.65) ^ (-1000 * cg / (cRs * -2.0)
  P = 0.03956419 * (T / 214.65) ^ (17.0815974899694)
Case Else
  P = 0#
End Select
fZtoP = P
End Function
Function fTstd!(Z!)
' Temperature structure of 1976 US Standard Atmosphere
' z in km, fTstd in K
' MJ Mahoney JPL 19980510
Select Case Z
Case Is <= 11: fTstd = 288.15 - 6.5 * Z
Case Is <= 20: fTstd = 216.65
Case Is <= 32: fTstd = 216.65 + (Z - 20)
Case Is <= 47: fTstd = 228.65 + 2.8 * (Z - 32)
Case Is <= 51: fTstd = 270.65
Case Is <= 71: fTstd = 270.65 - 2.8 * (Z - 51)
Case Is <= 84.852: fTstd = 214.65 - 2# * (Z - 71)
Case Else: fTstd = 184.946
End Select

End Function


Function fSecToTstringx$(hms&, ColonFlag As Boolean)
Dim hr!, Min!, sec!, D&, T$, X$, C$
' Convert time in seconds to  HHMMSS string with colon if colon.flag% is TRUE
  hr = Int(hms / 3600)
  Min = Int((hms - 3600 * hr) / 60)
  sec = Int(hms - 3600 * hr - 60 * Min + 0.5)
  If sec = 60 Then sec = 0: Min = Min + 1
  If Min = 60 Then Min = 0: hr = hr + 1
  If ColonFlag Then C$ = ":" Else C$ = ""
  D = 1000000 + 10000 * hr + 100 * Min + sec   'Avoid loosing leading zeros!
  T$ = Str$(D): T$ = Right$(T$, 6)
  X$ = Left$(T$, 2) + C$ + Mid$(T$, 3, 2) + C$ + Right$(T$, 2)
  fSecToTstringx$ = Left$(T$, 2) + C$ + Mid$(T$, 3, 2) + C$ + Right$(T$, 2)
'  L$ = Left$(T$, 2): LL$ = Left$(T$, 1)
'  T$ = T$
End Function
Function fTstringToSecx&(T$, ColonFlag As Boolean)
Dim i%, hr%, Min%, sec%
'Convert a time string to seconds
'If colon_flag is TRUE, format is assumed to be hh:mm:ss; otherwise, hhmmss
  If ColonFlag Then i% = 3 Else i% = 2
  hr = Val(Mid$(T$, 1, 2))
  Min = Val(Mid$(T$, 1 + i%, 2))
  sec = Val(Mid$(T$, 1 + 2 * i%, 2))
  fTstringToSecx& = 3600# * hr + 60# * Min + sec
End Function


Sub FindIWG1()
Dim IWG1$
' Look for character string "IWG1," on INPlu
  EOFflag = False
Repeat:
  IWG1$ = ""
  Do                                   'Look for first "F"
    GoSub GetChar                       'Get next character
    If IWG1$ = "I" Then                'Check for I
      GoSub GetChar
      If IWG1$ = "IW" Then             'Check for IW
        GoSub GetChar
        If IWG1$ = "IWG" Then          'Check for IWG
          GoSub GetChar
          If IWG1$ = "IWG1" Then       'Check for IWG1
            GoSub GetChar
            If IWG1$ = "IWG1," Then    'Check for IWG1,
              Exit Sub
            Else
              GoSub Repeat
            End If
          Else
            GoSub Repeat
          End If
        Else
          GoSub Repeat
        End If
      Else
        GoSub Repeat
      End If
    Else
      GoSub Repeat
    End If
  Loop
Exit Sub

GetChar:
  If ReadMode = 2 Then                'Read data from serial port
    If MSComm1.InBufferCount > 0 Then
      IWG1$ = IWG1$ + MSComm1.Input
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Sub
    End If
  Else                               'Read data from file
    If EOF(INPlu) Then
      EOFflag = True
      Exit Sub
    End If
    IWG1$ = IWG1$ + Input(1, INPlu)        'Get one character.
  End If
  Return
End Sub

Sub ReadNextFrameFile()
Dim i%, j%, X$
Static NGVlu%

' Find NGV Packet start string
  Call FindIWG1       'Get "IWG1," string
  If EOF(INPlu) Then EOFflag = True: Exit Sub     'Skip CPK
' And read it from NGV Data File
  Packet_NGV = "IWG1,"
  X$ = ""
  Do
    Packet_NGV = Packet_NGV + Input(1, INPlu)   'get a character
'    If WriteHex Then Debug.Print fHex2$(Right$(Packet_NGV, 1));
    If Len(Packet_NGV) > 4 Then X$ = Right$(Packet_NGV, 4)
  Loop Until X$ = "\r\n"  'vbCR+vbLF
  If WriteHex Then Debug.Print ""
'  Packet_NGV = Left$(Packet_NGV, Len(Packet_NGV) - 4)
  If WriteHex Then Print #OUTlu, Packet_NGV
  EOFflag = False

' Frame Number
  FrameNumber = FrameNumber + 1
'  txtFrame.Text = FrameNumber
'  If FrameNumber < Slider3.Max Then Slider3.Value = FrameNumber
Exit Sub

End Sub


Function GetSerialIn$()
Static MTPin$
Dim COM1$, Char$, i%, L%

Do
  If MSComm1.InBufferCount > 0 Then
    COM1$ = MSComm1.Input
    L% = Len(COM1$)
    Select Case MSComm1.InputLen
    Case 0                                                   'Entire buffer read into COM1$
        For i% = 1 To L%                                'Check for CR in buffer
          Char$ = Mid$(COM1$, i%, 1)
          Select Case Char$
          Case vbCr
            GetSerialIn$ = MTPin$
            List1.AddItem MTPin$
            MTPin$ = Right$(COM1$, L% - i% - 1) 'Save leftovers
            Exit Function                                  'And exit
          Case vbLf                                         'Ignore LF
          Case Else
            MTPin$ = MTPin$ + Char$               'No CR, save everything
          End Select
        Next
    Case 1
      Select Case COM1$
      Case vbCr
        GetSerialIn$ = MTPin$
        List1.AddItem MTPin$
        MTPin$ = ""
        Exit Function
      Case vbLf
        ' Ignore linefeeds
      Case Else
        MTPin$ = MTPin$ + COM1$
      End Select
    End Select
    DoEvents
  End If
Loop
End Function

Sub Encode_NGV(Packet_NGV As String)
Dim i%, Out$, D1$, D2$
' This routine is only used to encode DC8 data for testing
' Num x Parameter          Unit                  Variable
' 000   Prefix             IWG1
' 001 x Date/Time          yyyy-mm-ddThh:mm:ss   UTsec
' 002 x Latitude           deg                   Latitude
' 003 x Longitude          deg                   Longitude
' 004 x GPS_MSL_Alt        m                     gALT, Zg
' 005   WGS_84_Alt         m                     wALT, Zw
' 006 x Press_Alt          ft                    pALT, Zp
' 007   Radar_Alt          ft                    rALT, Zr
' 008   Grnd_Spd           m/s                   GroundSpeed
' 009   True_Airspeed      m/s                   TAS
' 010   Indicated_Airspeed kts                   IAS
' 011   Mach_Number        ---                   Mach
' 012   Vert_Velocity      m/s                   Vz
' 013 x True_Hdg           degt                  Heading
' 014   Track              degt                  TrackAngle
' 015   Drift              deg                   DriftAngle
' 016 x Pitch              deg                   Pitch
' 017 x Roll               deg                   Roll
' 018   Side_slip          deg                   SideSlipAngle
' 019   Angle_of_Attack    deg                   AttackAngle
' 020 x Ambient_Temp       C                     OATn (OATnav = OATn + cTo)
' 021   Dew_Point          C                     Tdew
' 022   Total_Temp         C                     Ttotal
' 023   Static_Press       mb                    Ps
' 024   Dynamic_Press      mb                    Pd
' 025   Cabin_Press        mb                    Pc
' 026 x Wind_Speed         m/s                   Wspd
' 027 x Wind_Dir           degt                  Wdir
' 028   Vert_Wind_Spd      m/s                   VWS
' 029   Solar_Zenith_Angle deg                   SZA
' 030   Sun_Elev_AC        deg                   SEAC
' 031   Sun_Az_Grd         degt                  SAG
' 032   Sun_Az_AC          degt                  SAAC
'       Suffix             \r\n
  Out$ = "IWG1"
  For i = 1 To 32
    Select Case i
    Case 1  'yyyy-mm-ddThh:mm:ss
      D1$ = Date
      D2$ = Right$(D1$, 4) + "-" + Left$(D1$, 2) + "-" + Mid$(D1$, 4, 2)
      Out$ = Out$ + "," + D2$ + "T" + fSecToTstring$(UTsec, True)
    Case 2  'Latitude
      Out$ = Out$ + "," + Format(Latitude, "##0.000")
    Case 3  'Longitude
      Out$ = Out$ + "," + Format(Longitude, "###0.000")
    Case 4  'gALT
      Out$ = Out$ + "," + Format(gALT * 1000#, "####0")   'm
    Case 6  'pALT
      Out$ = Out$ + "," + Format(pALT * cft_km, "####0")  'feet
    Case 13 'Heading
      Out$ = Out$ + "," + Format(Heading, "##0.00")
    Case 16 'Pitch
      Out$ = Out$ + "," + Format(Pitch, "##0.00")
    Case 17 'Roll
      Out$ = Out$ + "," + Format(Roll, "##0.00")
    Case 20 'OATnav
      Out$ = Out$ + "," + Format(OATn, "##0.00")
    Case 26 'Wspd
      Out$ = Out$ + "," + Format(Wspd, "##0.0")
    Case 27 'Wdir
      Out$ = Out$ + "," + Format(Wdir, "##0.0")
    Case Else
      Out$ = Out$ + ","
    End Select
  Next i
  Packet_NGV = Out$ + "\r\n"
End Sub


