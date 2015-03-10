Attribute VB_Name = "UDP"
' UDP Packet Parameters (MUST be declared)
' The following parameters have an "I" on the end to avoid conflict with names in MJ's programs.
Public YYYYMMDDI$, HHMMSSI$, UTsecI&, pALTI!, gALTI!, rALTI!, pALTftI!, gALTfeetI!, OATnI!, OATnavI!
Public UTsecNavI&, HeadingI!, PitchI!, RollI!, LatitudeI!, LongitudeI!, TASI!, WspdI!, WdirI!
Public Starting_yyyymmdd$

Public SetZp As Boolean, SetPR As Boolean
Public UDP_Packet_In As String, DatV!(0 To 32), GotV(0 To 32) As Boolean, UDP_Packet_Out As String

' Averaged values for A-line
Public PitchAvg!, PitchRms!, RollAvg!, RollRms!, ZpAvg!, ZpRms!, OatAvg!, OatRms!
Public LatAvg!, LatRms!, LonAvg!, LonRms!, SMcmd&, SMenc&

' Optional new parameters provided in default IWG packet
Public GroundSpeed!, TrackAngle!, AttackAngle!, SideSlipAngle!, wALT!
Public DriftAngle!, IAS!, Zgps!, Mach!, Vzac!, Tdew!, Ttotal!, Mach2!
Public Pstatic!, Pdynamic!, Pcabin!, UWS!, VWS!, WWS!, SZA!, SEAC!, SAG!, SAAC!
' Extra parameters provided by NG files
Public Ptotal!, ACINS!, ALT!, OATn0!, OATn1!, OATn2!, OATn3!, OATn4!, OATn5!, iALT!, PALT_A!
Public TOATn0!, TOATn1!, TOATn2!, TOATn3!, TOATn4!, TOATn5!, QC_A!, GSF!, GSF_A!, TASF!, PSFC!
Public ThetaAC!

' IP and Port parameters
Public RemotePort$, LocalPort$, RemoteIP$, LocalIP$, UDPupdating As Boolean
Public UseDefaultUDP As Boolean
Public LastTime As Date, CurrentTime As Date
' Things to do to implement UDP module
' - Add Winsock1 control to a form
' - Add code for Connect, DataArrival, and Close methods
' - Define RemotePort$, LocalPort$, RemoteIP$, LocalIP$
' - 1 Hz UDP uses 192.168.84.255:58800
' - Invoke Winsock1_Connect to establish connection
' - Make declarations shown above
Public Const DefaultPitch = 3#
Public Const DefaultRoll = 0#
Sub Decode_IWG(Packet_IWG As String, Reset As Boolean)
Dim i%, INP$, L%, A$, cft_km!, H%, M%, s%, y%, Mn%, D%
Static nNav!, PitchArray!(1 To 20), RollArray!(1 To 20), ZpArray!(1 To 20), OatArray!(1 To 20)
Static LatArray!(1 To 20), LonArray!(1 To 20), BeenHere As Boolean

' Need to declare two arrays in module
' Public V!(0 To 32), GotV(0 To 32) As Boolean
' IWG1 packet is comma delimited
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
' 012   Vert_Velocity      m/s                   Vzac
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
' 023   Static_Press       mb                    Pstatic
' 024   Dynamic_Press      mb                    Pdynamic
' 025   Cabin_Press        mb                    Pcabin
' 026 x Wind_Speed         m/s                   Wspd
' 027 x Wind_Dir           degt                  Wdir
' 028   Vert_Wind_Spd      m/s                   VWS
' 029   Solar_Zenith_Angle deg                   SZA
' 030   Sun_Elev_AC        deg                   SEAC
' 031   Sun_Az_Grd         degt                  SAG
' 032   Sun_Az_AC          degt                  SAAC
'       Suffix             \r\n
  INP$ = Packet_IWG   'has IWG1 at front
  If Reset Then BeenHere = False
'         12345678901234567890123456789012345678901234567890
' INP$ = "IWG1,2007-11-27T15:13:14,+37.3456,-120.2345,12.123,"
  L = Len(INP$)
  i = 1                              'variable number
  j = 0                              'before comma index
  k = 0                              'after comma index
  j = InStr(1, INP$, ",")            'skip IWG1
  k = InStr(j + 1, INP$, ",")
  A$ = Mid$(INP$, j + 1, k - j - 1)
' Decode date and time (handles only two of three possible formats)
  If InStr(1, A$, ":") Then
    YYYYMMDDI$ = Left(A$, 4) + Mid$(A$, 6, 2) + Mid$(A$, 9, 2)  'decode yyyy-mm-ddThh:mm:ss
    UTsecI = fTstringToSecX(Right$(A$, 8), True)
  Else
    YYYYMMDDI$ = Left(A$, 4) + Mid$(A$, 5, 2) + Mid$(A$, 7, 2)  'decode yyyymmddThhmmss
    UTsecI = fTstringToSecX(Right$(A$, 6), False)
  End If
' Following line used only with 90 C reference target in lab. Should not be here as two forms are referenced
'  If frmMTPbin.chkShowChannels(12).Value = 1 Then yyyymmddI$ = Trim(frmFLTINFO.cboFltDates.Text)  '"20100930"
  If UTsecI < 100 Then
  x = x
  End If
  ' Don't allow the yyyymmdd change!
  If BeenHere Then
    If UTsecI < UTsecIlast Then
      UTsecI = UTsecI + 86400
    End If
    YYYYMMDDI$ = Starting_yyyymmdd$
  Else
    Starting_yyyymmdd$ = YYYYMMDDI$
    BeenHere = True
  End If
  UTsecIlast = UTsecI
 
  y = Val(Left$(YYYYMMDDI$, 4))
  Mn = Val(Mid$(YYYYMMDDI$, 5, 2))
  D = Val(Right$(YYYYMMDDI$, 2))
  HHMMSSI$ = fSecToTstringX$(UTsecI, True)
  H = Left$(HHMMSSI$, 2)
  M = Val(Mid$(HHMMSSI$, 4, 2))
  s = Val(Right$(HHMMSSI$, 2))
  CurrentTime = DateSerial(y, Mn, D) + TimeSerial(H, M, s)

' Save UTsec in DatV(0) and OAT(K) in DatV(1)
  DatV(0) = UTsecI:   GotV(0) = True
  
' Decode remaining comma delimited parameters
  Do
    i = i + 1
    j = k
    k = InStr(j + 1, INP$, ",")
    If k = 0 Then Exit Do
    DatV(i) = Val(Mid$(INP$, j + 1, k - j - 1))
    If k = j + 1 Then GotV(i) = False Else GotV(i) = True 'consecutive commas
    If k = j + 4 Then
      If Mid$(INP$, j + 1, k - j - 1) = "nan" Then GotV(i) = False
    End If
    
    If SetZp Then
      If i = 6 Then  'Use inserted value
        DatV(i) = Val(frmUDP.txtVar(i).Text)
        Mid$(INP$, j + 1, k - j - 1) = Format(DatV(i), "###00.00")
      End If
    End If
    
    If SetPR Then
      Select Case i
      Case 16, 17  'Use inserted Pitch and Roll
        DatV(i) = Val(frmUDP.txtVar(i).Text)
        Mid$(INP$, j + 1, k - j - 1) = Format(DatV(i), "##0.000000")
      End Select
    End If
  Loop
  
  UDP_Packet_Out = INP$  'Has substituded P, R, and Zp but not noise
' Map DatV() array to program variables
  Map_DatV
  DatV(1) = DatV(20) + 273.15  '=OATnavI
  GotV(1) = True
  
' Calculate Pitch and Roll statistics
  nNav = nNav + 1
  If nNav > 15 Then nNav = 15  'Allow up to 15 samples = scan time
  For i = nNav To 2 Step -1
    PitchArray(i) = PitchArray(i - 1)
    RollArray(i) = RollArray(i - 1)
    ZpArray(i) = ZpArray(i - 1)
    OatArray(i) = OatArray(i - 1)
    LatArray(i) = LatArray(i - 1)
    LonArray(i) = LonArray(i - 1)
  Next i
  
  PitchArray(1) = PitchI
  RollArray(1) = RollI
  ZpArray(1) = pALTI
  OatArray(1) = OATnavI
  LatArray(1) = LatitudeI
  LonArray(1) = LongitudeI
  Call AvgRMSx(PitchArray!(), nNav, PitchAvg, PitchRms)
  Call AvgRMSx(RollArray!(), nNav, RollAvg, RollRms)
  Call AvgRMSx(ZpArray!(), nNav, ZpAvg, ZpRms)
  Call AvgRMSx(OatArray!(), nNav, OatAvg, OatRms)
  Call AvgRMSx(LatArray!(), nNav, LatAvg, LatRms)
  Call AvgRMSx(LonArray!(), nNav, LonAvg, LonRms)
End Sub

Sub Decode_IWG_Date_Time(INP$, YYYYMMDDI$, HHMMSSI$, UTsecI&, DandT As Date, k%)
Dim L%, j%, x$
'         12345678901234567890123456789012345678901234567890
' INP$ = "IWG1,2007-11-27T15:13:14,+37.3456,-120.2345,12.123,"
  L = Len(INP$)
  j = 0                              'before comma index
  k = 0                              'after comma index
  j = InStr(1, INP$, ",")            'skip IWG1
  k = InStr(j + 1, INP$, ",")
  x$ = Mid$(INP$, j + 1, k - j - 1)
' Decode date and time (handles only two of three possible formats)
  If InStr(1, x$, ":") Then
    YYYYMMDDI$ = Left(x$, 4) + Mid$(x$, 6, 2) + Mid$(x$, 9, 2)  'decode yyyy-mm-ddThh:mm:ss
    UTsecI = fTstringToSecX(Mid$(x$, 12, 8), True)
  Else
    YYYYMMDDI$ = Left(x$, 4) + Mid$(x$, 5, 2) + Mid$(x$, 7, 2)  'decode yyyymmddThhmmss
    UTsecI = fTstringToSecX(Right$(x$, 6), False)
  End If
  HHMMSSI$ = fSecToTstringX$(UTsecI, False)
  Call StringsToDate(YYYYMMDDI$, HHMMSSI$, DandT)

End Sub

Function fSecToTstringX$(UTs&, ColonFlag As Boolean)
Dim hr!, Min!, sec!, D&, T$, x$, C$
' Convert time in seconds to  HHMMSS string with colon if colon.flag% is TRUE
  hr = Int(UTs / 3600)
  Min = Int((UTs - 3600 * hr) / 60)
  sec = Int(UTs - 3600 * hr - 60 * Min + 0.5)
  If sec = 60 Then sec = 0: Min = Min + 1
  If Min = 60 Then Min = 0: hr = hr + 1
  If ColonFlag Then C$ = ":" Else C$ = ""
  D = 1000000 + 10000 * hr + 100 * Min + sec   'Avoid loosing leading zeros!
  T$ = Str$(D): T$ = Right$(T$, 6)
  x$ = Left$(T$, 2) + C$ + Mid$(T$, 3, 2) + C$ + Right$(T$, 2)
  fSecToTstringX$ = Left$(T$, 2) + C$ + Mid$(T$, 3, 2) + C$ + Right$(T$, 2)
End Function


Sub Map_DatV()
  For i = 2 To 32
    If GotV(i) Then
      Select Case i
      Case (2): LatitudeI = DatV(i)       'deg
      Case (3): LongitudeI = DatV(i)      'deg
      Case (4): gALTI = DatV(i) / 1000#   'km
      Case (5): wALT = DatV(i) / 1000#    'km
      Case (6): pALTftI = DatV(i)         'ft
      Case (7): rALTft = DatV(i)          'ft
      Case (8): GroundSpeed = DatV(i)     'm/s
      Case (9): TASI = DatV(i)            'm/s
      Case (10): IAS = DatV(i)            'kts
      Case (11): Mach = DatV(i)           '
      Case (12): Vzac = DatV(i)           'm/s
      Case (13): HeadingI = DatV(i)       'deg
      Case (14): TrackAngle = DatV(i)     'deg
      Case (15): DriftAngle = DatV(i)     'deg
      Case (16): PitchI = DatV(i)         'deg
      Case (17): RollI = DatV(i)          'deg
      Case (18): SideSlipAngle = DatV(i)  'deg
      Case (19): AttackAngle = DatV(i)    'deg
      Case (20): OATnI = DatV(i)          'C
      Case (21): Tdew = DatV(i)           'C
      Case (22): Ttotal = DatV(i)         'C
      Case (23): Pstatic = DatV(i)        'hPa
      Case (24): Pdynamic = DatV(i)       'hPa
      Case (25): Pcabin = DatV(i)         'hPa
      Case (26): WspdI = DatV(i)          'm/s
      Case (27): WdirI = DatV(i)          'deg
      Case (28): VWS = DatV(i)            'm/s
      Case (29): SZA = DatV(i)            'deg
      Case (30): SEAC = DatV(i)           'deg
      Case (31): SAG = DatV(i)            'deg
      Case (32): SAAC = DatV(i)           'deg
      End Select
    End If
  Next i
  ft_km = 3280.8                          'ft/km
  gALTft = gALT * ft_km                   'ft
  pALTI = pALTftI / ft_km                 'km
  rALTI = rALTft / ft_km                  'km
  OATnavI = OATnI + 273.15                'K
End Sub



Sub ReadNextFrameUDP()
Dim x$, i%
'IWG1,20010920T151645,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,\r\n
' Find NGV Packet start string
  Call FindIWG1UDP                              'Get "IWG1," string

  If Right$(UDP_Packet_In, 2) <> vbCrLf Then    'Find first \r\n
    x$ = ""
    i = 0
    Do
      i = i + 1
      UDP_Packet_In = Mid$(UDP_Packet_In, i, 2) 'Get next two characters
    Loop Until x$ = vbCrLf
    UDP_Packet_In = Left$(UDP_Packet_In, i + 1)
  End If

End Sub

Sub FindIWG1UDP()
Dim IWG1$, i%, j%
' Look for character string "IWG1," on INPlu
  EOFflag = False
  j = 0
Repeat:
  IWG1$ = ""
  i = 0
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
              If i = 5 Then Exit Sub   'Normal exit
              'Strip off characters before IWG1,
              UDP_Packet_In = Right$(UDP_Packet_In, Len(UDP_Packet_In) - i + 5)
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
  i = i + 1
  j = j + 1
  IWG1$ = IWG1$ + Mid$(UDP_Packet_In, i, 1)     'Get one character.
  If j > 1000 Then
    MsgBox "IWG1 not found in 1000 characters", vbOKOnly
  End If
Return

End Sub


Sub AvgRMSx(ByRef A!(), ByVal n%, ByRef AVG!, ByRef RMS!)
Dim Sum1!, Sum2!, x!, i%
' Calculate average and rms of n numbers in A()
  For i = 1 To n
    x = A(i)
    Sum1 = Sum1 + x         'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  AVG = Sum1 / n            'Calculate average
  If Sum2 - AVG ^ 2 * n > 0 And n > 1 Then RMS = Sqr((Sum2 - AVG ^ 2 * n) / (n - 1)) Else RMS = 0# 'Calculate RMS
  
End Sub

Function fTstringToSecX&(T$, ColonFlag As Boolean)
Dim i%, hr%, Min%, sec%
'Convert a time string to seconds
'If colon_flag is TRUE, format is assumed to be hh:mm:ss; otherwise, hhmmss
  If ColonFlag Then i% = 3 Else i% = 2
  hr = Val(Mid$(T$, 1, 2))
  Min = Val(Mid$(T$, 1 + i%, 2))
  sec = Val(Mid$(T$, 1 + 2 * i%, 2))
  fTstringToSecX& = 3600# * hr + 60# * Min + sec
End Function


