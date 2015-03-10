VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmRAOBconvert 
   BackColor       =   &H80000004&
   Caption         =   "Convert Large RAOB to Signifcant Levels"
   ClientHeight    =   4575
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   5595
   LinkTopic       =   "Form1"
   ScaleHeight     =   4575
   ScaleWidth      =   5595
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtUsed 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   840
      MultiLine       =   -1  'True
      TabIndex        =   42
      Top             =   3585
      Width           =   495
   End
   Begin VB.TextBox txtZpTop 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2880
      MultiLine       =   -1  'True
      TabIndex        =   39
      Text            =   "frmRAOBconvert.frx":0000
      Top             =   2025
      Width           =   615
   End
   Begin VB.TextBox txtZpBottom 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1935
      MultiLine       =   -1  'True
      TabIndex        =   38
      Text            =   "frmRAOBconvert.frx":0005
      Top             =   2010
      Width           =   615
   End
   Begin VB.TextBox txtRAOBcount 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1935
      MultiLine       =   -1  'True
      TabIndex        =   36
      Text            =   "frmRAOBconvert.frx":0009
      Top             =   1695
      Width           =   615
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Write RAOB2"
      Height          =   375
      Index           =   8
      Left            =   4200
      TabIndex        =   35
      Top             =   2760
      Width           =   1260
   End
   Begin VB.CheckBox chkWriteRAOB2 
      Caption         =   "Write RAOB2 file"
      Height          =   195
      Left            =   120
      TabIndex        =   34
      Top             =   780
      Width           =   1830
   End
   Begin VB.CheckBox chkShowRH 
      Caption         =   "Show RH Profile"
      Height          =   225
      Left            =   120
      TabIndex        =   33
      Top             =   525
      Width           =   1665
   End
   Begin VB.ComboBox cboRAOBformat 
      Height          =   315
      Left            =   3285
      TabIndex        =   31
      Text            =   "CLS5"
      Top             =   2985
      Width           =   855
   End
   Begin VB.CheckBox chkPause 
      Caption         =   "Pause"
      Height          =   255
      Left            =   2280
      TabIndex        =   30
      Top             =   510
      Width           =   975
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Best Fit"
      Height          =   375
      Index           =   7
      Left            =   4185
      TabIndex        =   29
      Top             =   -15
      Visible         =   0   'False
      Width           =   1260
   End
   Begin VB.CheckBox chkAutoOpen 
      Caption         =   "Auto Open"
      Height          =   195
      Left            =   2280
      TabIndex        =   28
      Top             =   285
      Value           =   1  'Checked
      Width           =   1095
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Fix >70"
      Height          =   375
      Index           =   6
      Left            =   4215
      TabIndex        =   27
      Top             =   450
      Width           =   1260
   End
   Begin VB.ComboBox cboSDthreshold 
      Height          =   315
      Left            =   1935
      TabIndex        =   26
      Text            =   "0.2"
      Top             =   1305
      Width           =   840
   End
   Begin VB.CheckBox chkFixRH 
      Caption         =   "Fix Bad RH"
      Height          =   255
      Left            =   2280
      TabIndex        =   25
      Top             =   15
      Value           =   1  'Checked
      Width           =   1215
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Write txt"
      Height          =   375
      Index           =   2
      Left            =   4215
      TabIndex        =   24
      Top             =   3240
      Width           =   1260
   End
   Begin VB.CheckBox chkAutoClear 
      Caption         =   "Automatically Clear Plot"
      Height          =   195
      Left            =   120
      TabIndex        =   23
      Top             =   270
      Value           =   1  'Checked
      Width           =   2055
   End
   Begin VB.CheckBox chkDoDir 
      Caption         =   "Process Directory"
      Height          =   195
      Left            =   135
      TabIndex        =   22
      Top             =   30
      Width           =   1575
   End
   Begin VB.ComboBox cboSamplesAveraged 
      Height          =   315
      Left            =   1950
      TabIndex        =   21
      Text            =   "7"
      Top             =   945
      Width           =   825
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Toggle Scale"
      Height          =   375
      Index           =   5
      Left            =   4215
      TabIndex        =   20
      Top             =   930
      Width           =   1260
   End
   Begin VB.TextBox txtAVG 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2265
      MultiLine       =   -1  'True
      TabIndex        =   19
      Top             =   3360
      Width           =   495
   End
   Begin VB.TextBox txtRMS 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3630
      MultiLine       =   -1  'True
      TabIndex        =   17
      Top             =   3330
      Width           =   495
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Replot"
      Height          =   375
      Index           =   4
      Left            =   4215
      TabIndex        =   15
      Top             =   1410
      Width           =   1260
   End
   Begin VB.TextBox txtNpts 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   840
      MultiLine       =   -1  'True
      TabIndex        =   14
      Top             =   3300
      Width           =   495
   End
   Begin VB.TextBox txtLR 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1575
      MultiLine       =   -1  'True
      TabIndex        =   11
      Top             =   3000
      Width           =   390
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Clear Plot"
      Height          =   375
      Index           =   3
      Left            =   4215
      TabIndex        =   9
      Top             =   1890
      Width           =   1260
   End
   Begin VB.CheckBox chkEnableAveraging 
      Caption         =   "Enable Averaging for"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   990
      Value           =   1  'Checked
      Width           =   1785
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Exit"
      Height          =   345
      Index           =   1
      Left            =   4170
      TabIndex        =   7
      Top             =   4230
      Width           =   1320
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   90
      Top             =   1560
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox txtRAOBfile 
      Height          =   285
      Left            =   150
      TabIndex        =   6
      Top             =   3900
      Width           =   5355
   End
   Begin VB.TextBox txtMask 
      Height          =   285
      Left            =   525
      TabIndex        =   4
      Text            =   "HK*.txt"
      Top             =   2685
      Width           =   3630
   End
   Begin VB.CommandButton cmdCommand 
      Caption         =   "Open RAOB"
      Height          =   375
      Index           =   0
      Left            =   4215
      TabIndex        =   2
      Top             =   2340
      Width           =   1260
   End
   Begin VB.TextBox txtPath 
      Height          =   285
      Left            =   540
      TabIndex        =   1
      Text            =   "c:\MTP\Data\NGV\TORERO\RAOB\HM\"
      Top             =   2355
      Width           =   3630
   End
   Begin VB.Label Label 
      Caption         =   "N Used="
      Height          =   255
      Index           =   11
      Left            =   120
      TabIndex        =   43
      Top             =   3585
      Width           =   735
   End
   Begin VB.Label Label 
      Caption         =   "Use Altitude Range From "
      Height          =   240
      Index           =   10
      Left            =   105
      TabIndex        =   41
      Top             =   2055
      Width           =   1815
   End
   Begin VB.Label Label 
      Caption         =   "To"
      Height          =   255
      Index           =   9
      Left            =   2610
      TabIndex        =   40
      Top             =   2055
      Width           =   330
   End
   Begin VB.Label Label 
      Caption         =   "Record"
      Height          =   285
      Index           =   4
      Left            =   1110
      TabIndex        =   37
      Top             =   1695
      Width           =   795
   End
   Begin VB.Label Label1 
      Caption         =   "RAOB Format"
      Height          =   255
      Left            =   2280
      TabIndex        =   32
      Top             =   3045
      Width           =   975
   End
   Begin VB.Label Label 
      Caption         =   "AVG(K)"
      Height          =   225
      Index           =   8
      Left            =   1545
      TabIndex        =   18
      Top             =   3405
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "RMS(K)"
      Height          =   180
      Index           =   7
      Left            =   2910
      TabIndex        =   16
      Top             =   3375
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "N Points="
      Height          =   255
      Index           =   6
      Left            =   120
      TabIndex        =   13
      Top             =   3300
      Width           =   735
   End
   Begin VB.Label Label 
      Caption         =   "Second Diff Threshold"
      Height          =   255
      Index           =   5
      Left            =   135
      TabIndex        =   12
      Top             =   1320
      Width           =   1695
   End
   Begin VB.Label Label 
      Caption         =   "Lapse Rate (2-4 km)"
      Height          =   255
      Index           =   3
      Left            =   105
      TabIndex        =   10
      Top             =   3045
      Width           =   1515
   End
   Begin VB.Label Label 
      Caption         =   "samples"
      Height          =   255
      Index           =   2
      Left            =   2790
      TabIndex        =   5
      Top             =   1035
      Width           =   735
   End
   Begin VB.Label Label 
      Caption         =   "Mask"
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   3
      Top             =   2760
      Width           =   375
   End
   Begin VB.Label Label 
      Caption         =   "Path"
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   2400
      Width           =   375
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "Options"
      Begin VB.Menu mnuOptionsFormatRAOB 
         Caption         =   "Format RAOB"
      End
   End
End
Attribute VB_Name = "frmRAOBconvert"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Replot As Boolean, ConvertInFile$, km2%, km4%, LR24!
Public iMax%, High As Boolean, Start As Boolean, Search  As Boolean
Public txtFile$, IImin%, JJmin%, RMSmin!, RAOBfile$
Public ZpBottom!, ZpTop!
Public Sub ConvertLargeRAOBfile(RAOBfile$, fmt$)
Dim A$, InData!(3000, 7), x$, Line$, RAOBlu%, nHeader%
Dim i%, j%, lu%, LastH!, Hmax!

      RAOBlu% = FreeFile
      Open RAOBfile$ For Input As RAOBlu%
      
      Select Case fmt$
      Case "WY"
        nRAOBlevels = 14000
        Call ConvertWyomingNewRe(RAOBlu%)
        
      Case "FH"
        WMOlatitude = -0.9
        WMOlongitude = -89.62
        WMOaltitude = 8
'FH20070702_18_ALAJUELA.SONDE
        Select Case Mid$(fGetFilename(RAOBfile$), 15, 3)
        Case "GAL"
          WMO4l = "SEST"
          WMOnumber = 84008
        Case "ALA"
          WMO4l = "MROC"
          WMOnumber = 78762
        End Select
        
'2007 08 06 2007  8 28
'123456789012345678901
        Input #RAOBlu, nHeader%, A$
        i = 1
        Do
          Line Input #RAOBlu, A$
          i = i + 1
          If i = 7 Then
            Iyear = Left$(A$, 4)
            Imonth = Mid$(A$, 6, 2)
            Iday = Mid$(A$, 9, 2)
          End If
          
        Loop Until i = nHeader%
        i = 0
        LastH = -99#: Hmax = 0#
        Do
          i = i + 1
          Line Input #RAOBlu, A$
          If i = 1 Then
            UTsec = Left$(A$, 6)
            HHMMSS$ = fSecToTstring(UTsec, True)
            Ihour = Left$(HHMMSS$, 2)
            Imin = Mid$(HHMMSS$, 4, 2)
            Isec = Mid$(HHMMSS$, 7, 2)
            Ihhmm = Format(Ihour, "00") + Format(Imin, "00")
          End If
          If i > 180 Then
          i = i
          End If
'          Line Input #RAOBlu, A$
          If Len(A$) > 0 And Mid$(A$, 8, 6) <> "9999.0" Then
' GMTs  Pressure    Alt   Temp    Hum     O3     FP     H2O
'   267  1013.90   0.008  292.3   78.2  0.022  291.0  2.05E+04
'   268  1013.70   0.010  292.2   78.0  0.022  290.0  1.93E+04
'123456789012345678901234567890123456789012345678901234567890

            PZraob(i) = Mid$(A$, 9, 7)          'Pressure (hPa)
            ZPraob(i) = fPtoZ(PZraob(i))        'Pressure Altitude (km)
            TZraob(i) = Mid$(A$, 26, 5)         'Temperature (K)
            RZraob(i) = Mid$(A$, 34, 4)         'RH (%)
            ZGraob(i) = Mid$(A$, 18, 6)         'Altitude (km)
          End If
          If ZGraob(i) + 0.1 < LastH Or ZGraob(i) < Hmax - 0.2 Then
            Exit Do
          End If
          LastH = ZGraob(i)
          If ZGraob(i) > Hmax Then Hmax = ZGraob(i)
        Loop Until EOF(RAOBlu)
        
      Case "TREX"
        WMOlatitude = 36.81
        WMOlongitude = -118.2
        WMOaltitude = 1162
        WMO4l = "INCA"
        WMOnumber = 99999
        Do
          Line Input #RAOBlu, A$
          Select Case Left$(A$, 3)
          Case "UTC"
'         1         2         3         4         5         6
'123456789012345678901234567890123456789012345678901234567890
'UTC Release Time (y,m,d,h,m,s):    2006, 03, 03, 23:02:00
            Iyear = Mid$(A$, 36, 4)
            Imonth = Mid$(A$, 42, 2)
            Iday = Mid$(A$, 46, 2)
            Ihour = Mid$(A$, 50, 2)
            Imin = Mid$(A$, 53, 2)
            Isec = Mid$(A$, 56, 2)
            Ihhmm = Format(Ihour, "00") + Format(Imin, "00")
          Case Else
          End Select
          
        Loop Until Left$(A$, 3) = "---"
'Data Type:                         University of Leeds Sounding/Ascending
'Project iD:                        T -Rex
'Release Site Type/Site ID:         Independence Airport, CA
'Release Location (lon,lat,alt):    118 12.00'W, 36 48.60'N, -118.200, 36.810, 1162.0
'UTC Release Time (y,m,d,h,m,s):    2006, 03, 03, 23:02:00
'/
'/
'/
'/
'/
'/
'Nominal Release Time (y,m,d,h,m,s): 2006, 03, 03, 23:02:00
' Time  Press  Temp  Dewpt  RH    Ucmp   Vcmp   spd   dir   Wcmp     Lon     Lat    Ele   Azi   Alt    Qp   Qt   Qrh  Qu   Qv   QdZ
'  sec    mb     C     C     %     m/s    m/s   m/s   deg   m/s      deg     deg    deg   deg    m    code code code code code code
'------ ------ ----- ----- ----- ------ ------ ----- ----- ----- -------- ------- ----- ----- ------- ---- ---- ---- ---- ---- ----
'   0.0  874.2   6.7 -12.0  25.0   12.3   -3.5  12.8 286.0 999.0 9999.000 999.000 999.0 999.0  1163.0  3.0  3.0  3.0  1.0  1.0  9.0
'   2.0  873.2   7.7 -12.2  23.0   12.3   -3.3  12.7 285.0   5.0 9999.000 999.000 999.0 999.0  1173.0  3.0  3.0  3.0  1.0  1.0 99.0
'   4.0  872.1   7.4 -13.0  22.0   12.1   -3.2  12.5 285.0   5.0 9999.000 999.000 999.0 999.0  1183.0  1.0  1.0  1.0  1.0  1.0 99.0
'         1         2         3         4         5         6         7         8         9        10        11        12        13
'1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'       ZGraob(i), PZraob(i), TZraob(i), RZraob(i)
        i = 0
        Do
          Line Input #RAOBlu, A$
          If Len(A$) > 0 And Mid$(A$, 8, 6) <> "9999.0" Then
            i = i + 1
            If i > 903 Then
             x = x
            End If
            PZraob(i) = Mid$(A$, 8, 6)          'Pressure (hPa)
            ZPraob(i) = fPtoZ(PZraob(i))        'Pressure Altitude (km)
            TZraob(i) = Mid$(A$, 15, 5) + cTo   'Temperature (K)
            RZraob(i) = Mid$(A$, 27, 5)         'RH (%)
            ZGraob(i) = Mid$(A$, 94, 6) / 1000# 'Altitude (km)
          End If
        Loop Until EOF(RAOBlu)
'        Imax = i
'        Close RAOBlu
        
      Case "PNNL"
        i = InStr(1, RAOBfile$, ".", vbTextCompare)
        WMO4l = Right$(RAOBfile$, 3)
        Iyear = 2000
        Imonth = Val(Mid$(RAOBfile$, i - 7, 1))
        Iday = Val(Mid$(RAOBfile$, i - 6, 2))
        Ihour = Val(Mid$(RAOBfile$, i - 4, 2))
        Imin = Val(Mid$(RAOBfile$, i - 2, 2))
        Isec = 0
        Ihour = Ihour + 5 'Convert to UT
        If Ihour > 24 Then
          Ihour = Ihour - 24
          Iday = Iday + 1
          If Iday > 30 Then Iday = 1: Imonth = Imonth + 1
        End If
        DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
        txtFile$ = UCase$(Right(RAOBfile$, 3)) + "2000" + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihour, "00") + ".txt"
        Line Input #RAOBlu, A$
        i = 0
        Do
          i = i + 1
          For j = 1 To 7: Input #RAOBlu, InData(i, j): Next j
        Loop Until EOF(RAOBlu)
        Close RAOBlu

        iMax = i
'       Idum%, Iyear%, Imonth%, Iday%, Ihour%, Nlev
'       ZGraob(i), PZraob(i), TZraob(i), RZraob(i)
'       2 80  2 26 12 23
'       0.000  1014.0  299.56  83.51
        km2 = 0: km4 = 0
        For i = 1 To iMax
          ZGraob(i) = InData(i, 6) / 1000#
          ZPraob(i) = fPtoZ(InData(i, 4))
          TZraob(i) = InData(i, 2) + cTo
'         RZraob(i) = fRH_TwTd(InData(i, 3) + cTo, InData(i, 2) + cTo)
          RZraob(i) = fRHw(InData(i, 4), TZraob(i), InData(i, 7) / 1000#)
          If chkFixRH.Value = 1 Then
            If RZraob(i) > 60 And ZPraob(i) > 4 Then RZraob(i) = RZraob(i - 1)
            If i = 1 And RZraob(i) > 100 Then RZraob(i) = 100
            If RZraob(i) > 100 Then RZraob(i) = RZraob(i - 1)
          End If
          If km2 = 0 And ZGraob(i) > 2 Then km2 = i
          If km4 = 0 And ZGraob(i) > 4 Then km4 = i
          If i > 1 Then
            If ZPraob(i) > 4 And ZPraob(i) < ZPraob(i - 1) Then Exit For
          End If
        Next i
        iMax = i - 1
        If km2 > 0 And km4 > 0 Then
          LR24 = (TZraob(km2) - TZraob(km4)) / (ZPraob(km2) - ZPraob(km4))
        Else
          LR24 = 0
        End If
        txtLR.Text = Format(LR24, "#0.00")

      Case "CLS5"
        Line Input #RAOBlu, A$
'Data Type:                         AVAPS SOUNDING DATA, Channel 1
        Line Input #RAOBlu, A$
'Project ID:                        CAMEX 4, 10406
        Line Input #RAOBlu, A$
'Launch Site Type/Site ID:          NASA DC-8,
        Line Input #RAOBlu, A$
'Launch Location (lon,lat,alt):     77 35.30'W, 24 44.90'N, -77.58833, 24.74833, 11855
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
        WMOlongitude = Val(Mid$(A$, 36, 2)) + Val(Mid$(A$, 39, 5)) / 60#
        If Mid$(A$, 45, 1) = "W" Then WMOlongitude = -WMOlongitude
        WMOlatitude = Val(Mid$(A$, 48, 2)) + Val(Mid$(A$, 51, 5)) / 60#
        If Mid$(A$, 57, 1) = "S" Then WMOlatitude = -WMOlatitude
      
        Line Input #RAOBlu, A$
'GMT Launch Time (y,m,d,h,m,s):     2001, 08, 18, 18:49:32
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
        Iyear = Mid$(A$, 36, 4)
        Imonth = Mid$(A$, 42, 2)
        Iday = Mid$(A$, 46, 2)
        Ihour = Mid$(A$, 50, 2)
        Imin = Mid$(A$, 53, 2)
        Isec = Mid$(A$, 56, 2)
      
        Line Input #RAOBlu, A$
'Sonde Id:                          012025014
        Line Input #RAOBlu, A$
'/
        Line Input #RAOBlu, A$
'/
        Line Input #RAOBlu, A$
'/
        Line Input #RAOBlu, A$
'System Operator/Comments:          ,
        Line Input #RAOBlu, A$
'/
        Line Input #RAOBlu, A$
'/
        WMO4l = "AERO"

        DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
        txtFile$ = UCase$(Right(RAOBfile$, 3)) + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihour, "00") + ".txt"
        Line Input #RAOBlu, A$
' Time  Press  Temp  Dewpt  RH    Uwind  Vwind  Wspd  Dir   dZ      Lon     Lat    Rng   Az     Alt    Qp   Qt   Qh   Qu   Qv   Quv
        Line Input #RAOBlu, A$
'  sec    mb     C     C     %     m/s    m/s   m/s   deg   m/s     deg     deg     km   deg     m     mb   C    %    m/s  m/s  m/s
        Line Input #RAOBlu, A$
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
        i = 0
        Do
          Line Input #RAOBlu, A$
          i = i + 1
          x$ = Mid$(A$, 8, 6)
          If x$ = "9999.0" Then i = i - 1: GoTo SkipRAOB
          PZraob(i) = Val(x$)
          x$ = Mid$(A$, 15, 5)
          If x$ = "999.0" Then i = i - 1: GoTo SkipRAOB
          TZraob(i) = Val(x$) + cTo
          'Debug.Print i; Val(X$); TZraob(i)
          x$ = Mid$(A$, 21, 5)
          ZPraob(i) = fPtoZ(PZraob(i))
          If x$ = "999.0" Then i = i - 1: GoTo SkipRAOB
          x$ = Mid$(A$, 27, 5)
          If x$ = "999.0" Then i = i - 1: GoTo SkipRAOB
          RZraob(i) = Val(x$)
SkipRAOB:
        Loop Until EOF(RAOBlu)

      End Select
        iMax = Nlevr
        Close RAOBlu

End Sub


Sub Main()
Dim RAOBlu%, A$, i%, j%, nMax%, Istep%, Ioff%, Iused%
Dim s!, SS!, AVG!, RMS!, T1!, T2!
Dim Zavg!(14000), Tavg!(14000), FD!(14000), SD!(14000)
Dim Tf!(300), Zf!(300), Ff!(14000)
Dim Tsave!(1 To 14000)
' Red    - average over Istep consecutive samples
' Yellow - raw T profile
' Cyan   - fitted T profile
''''''''''''''''''''''''''''''''' Plot RAW data
  ZpBottom = Val(txtZpBottom.Text)
  ZpTop = Val(txtZpTop.Text)
  Nlevr = iMax
  Nlev = iMax
' Plot T profile
  For i = 1 To iMax
    Zplot(i) = ZPraob(i)
    Tplot(i) = TZraob(i)
    Rplot(i) = RZraob(i)
  Next i
  With frmRAOB     'Plot T profile
  .PlotT0 = True
  .LineColor = 14  'Light Yellow
  .PlotSingleScan = False
  .RAOBupdate
  End With

' Plot RH Profile
  If chkShowRH.Value = 1 Then
    For i = 1 To iMax
      Zplot(i) = ZPraob(i)
      Tplot(i) = RZraob(i) + 200
    Next i
    With frmRAOB    'Plot RH profile offset by 200K
    .LineColor = 8  'Gray
    .PlotSingleScan = False
    .RAOBupdate
    End With
  End If
  
  If chkEnableAveraging.Value = 1 Then
  ' Calculate average over Istep consecutive samples
    Istep = Val(cboSamplesAveraged.Text)
    Ioff = Int(Istep / 2)
    For i = 1 To Ioff
      Zavg(i) = ZPraob(i)
      Tavg(i) = TZraob(i)
    Next i
    For i = Ioff + 1 To Nlev - Ioff
      For j = 1 To Istep
        Zavg(i) = Zavg(i) + ZPraob(i + j - Ioff - 1)
        Tavg(i) = Tavg(i) + TZraob(i + j - Ioff - 1)
      Next j
      Zavg(i) = Zavg(i) / Istep
      Tavg(i) = Tavg(i) / Istep
    Next i
    For i = Nlev - Ioff + 1 To Nlev
      Zavg(i) = ZPraob(i)
      Tavg(i) = TZraob(i)
    Next i
  
    For i = 1 To iMax
      Zplot(i) = Zavg(i)
      Tplot(i) = Tavg(i)
    Next i
    With frmRAOB
      .LineColor = 12   'Light Red
      .PlotSingleScan = False
      .RAOBupdate
    End With
    
'   Now take first differences of averages over Istep samples
    For i = Ioff + 1 To iMax - Ioff
      FD(i) = Tavg(i + Ioff) - Tavg(i - Ioff)
    Next i
    For i = 1 To Ioff
      FD(i) = FD(Ioff + 1)
      FD(iMax - i + 1) = FD(iMax - Ioff)
    Next i
    
'   Now take second differences
'   If LR is linear, first differences FD might be large, but
'   second differences SD will be small
    For i = Ioff + 1 To iMax - Ioff
      SD(i) = FD(i + Ioff) - FD(i - Ioff)
    Next i
    For i = 1 To Ioff
      SD(i) = SD(Ioff + 1)
      SD(iMax - i + 1) = SD(iMax - Ioff)
    Next i
    
'   Now ID max and min values
    SDT = Val(cboSDthreshold.Text)
    n = 1       'Number of levels with SD greater than threshold
    Ff(1) = 1   'Array containing level number of SD maxs and mins above threshold
    For i = Ioff + 1 To iMax - Ioff
    ' Check if SD(i) is less than neighbors and greater than threshold
      If SD(i) < SD(i - 1) And SD(i) < SD(i + 1) And Abs(SD(i)) >= SDT Then
        n = n + 1
        Ff(n) = i
      End If
    ' Check if SD(i) is greater than neighbors and greater than threshold
      If SD(i) >= SD(i - 1) And SD(i) >= SD(i + 1) And Abs(SD(i)) >= SDT Then
        n = n + 1
        Ff(n) = i
      End If
    Next i
    If n = 1 Then n = 2: Ff(2) = iMax
    nMax = n   'Number of maxs and mins greater than threshold
    If Ff(n) < iMax Then nMax = nMax + 1: Ff(nMax) = iMax  'Include highest point
    i = 0
    For n = 1 To nMax
      Tplot(n) = TZraob(Ff(n))
      Zplot(n) = ZPraob(Ff(n))
      Rplot(n) = RZraob(Ff(n))
      ZGplot(n) = ZGraob(Ff(n))
    Next n
    IntRhoV = fRhoInt(Zplot(), Rplot(), nMax, 0, False)
    nRAOBlevels = nMax
  End If
  
  If chkEnableAveraging.Value = 1 Then
' Calculate RMS fit
    s = 0#: SS = 0: Iused = 0
    For i = 1 To iMax
      If ZPraob(i) >= ZpBottom And ZPraob(i) <= ZpTop Then
        Iused = Iused + 1
        T1 = TZraob(i)
        T2 = fLinterp(Zplot(), Tplot(), nMax, ZPraob(i)) 'RAOB level interpolated to fit levels
        s = s + T2 - T1
        SS = SS + (T2 - T1) ^ 2
      End If
    Next i
    txtUsed.Text = Iused
    AVG = s / Iused
    RMS = Sqr((SS - Iused * AVG ^ 2) / (Iused - 1))

'   Temperature at match point
'    i = 0
'    Do: i = i + 1
'    Loop Until Pr(i) > Zplot(nMax)

'    iavg = 12 ' iAvg = i
'    Tm = fLinterp(Pr(), Tr(), nMax, Zplot(nMax))
'    dT = Tplot(nMax) - Tm
'    For i = iavg To 11
'      nMax = nMax + 1
'      Zplot(nMax) = Pr(i)
'      Tplot(nMax) = Tr(i) + dT
'      Rplot(nMax) = fLinterp(Qr(), Rr(), 5, Zplot(nMax))
'    Next i
    Nlevr = nMax
    txtNpts.Text = Str(nMax)
    txtAVG.Text = Format(AVG, "#0.00")
    txtRMS.Text = Format(RMS, "#0.00")
    DoEvents

    With frmRAOB
    .LineColor = 11   'Light Cyan
    .PlotSingleScan = False
    .ShowDots = True
    .RAOBupdate
    .ShowDots = False
    End With
    If chkShowRH.Value = 1 Then
      For i = 1 To Nlev
        Tsave(i) = Tplot(i)
        Tplot(i) = Rplot(i) + 200
      Next i
      Nlevr = Nlev
    
      With frmRAOB
      .LineColor = 12   'Light Red
      .PlotSingleScan = False
      .ShowDots = True
      .RAOBupdate
      .ShowDots = False
      End With
      For i = 1 To Nlev
        Tplot(i) = Tsave(i)
      Next i
    End If
    
  End If
  Replot = False
  
End Sub

Sub Main2()
Dim RAOBlu%, A$, i%, j%, nMax%, Istep%, Ioff%
Dim s!, SS!, AVG!, RMS!, T1!, T2!
Dim Zavg!(14000), Tavg!(14000), FD!(14000), SD!(14000)
Dim Tf!(300), Zf!(300), Ff!(4000)
Dim Tsave!(1 To 14000), Tplot2!(1 To 14000), Zplot2!(1 To 14000), Rplot2!(1 To 14000), ZGplot!(1 To 14000)


' This routine only called on Replot
''''''''''''''''''''''''''''''''' Plot RAW data
  ZpBottom = Val(txtZpBottom.Text)
  ZpTop = Val(txtZpTop.Text)
  Nlevr = iMax
  Nlev = iMax
' Plot raw profile
  For i = 1 To iMax
    Zplot(i) = ZPraob(i)
    Tplot(i) = TZraob(i)
    Rplot(i) = RZraob(i)
  Next i
  
      With frmRAOB
      .PlotT0 = True
      .LineColor = 14   'Light Yellow
      .PlotSingleScan = False
      .RAOBupdate
      End With
      DoEvents
  '   Plot RH
      If chkShowRH.Value = 1 Then
        For i = 1 To iMax
          Zplot(i) = ZPraob(i)
          Tplot(i) = RZraob(i) + 200
        Next i
        With frmRAOB
        .LineColor = 8    'Gray
        .PlotSingleScan = False
        .RAOBupdate
        End With
      End If
      DoEvents
      
  For i = 1 To 2000
    Zavg(i) = 0#: Tavg(i) = 0#
    Tsave(i) = 0#: Tplot2(i) = 0#: Zplot2(i) = 0#: Rplot2(i) = 0
  Next i
  
  If chkEnableAveraging.Value = 1 Then
    Istep = Val(cboSamplesAveraged.Text)
    Ioff = Int(Istep / 2)
    For i = 1 To Ioff
      Zavg(i) = ZPraob(i)
      Tavg(i) = TZraob(i)
    Next i
    For i = Ioff + 1 To Nlev - Ioff
      For j = 1 To Istep
        Zavg(i) = Zavg(i) + ZPraob(i + j - Ioff - 1)
        Tavg(i) = Tavg(i) + TZraob(i + j - Ioff - 1)
      Next j
      Zavg(i) = Zavg(i) / Istep
      Tavg(i) = Tavg(i) / Istep
    Next i
    For i = Nlev - Ioff + 1 To Nlev
      Zavg(i) = ZPraob(i)
      Tavg(i) = TZraob(i)
    Next i
  
    For i = 1 To iMax
      Zplot(i) = Zavg(i)
      Tplot(i) = Tavg(i)
    Next i
    Nlev = iMax
    With frmRAOB
      .LineColor = 12  'Light Red
      .PlotSingleScan = False
      .RAOBupdate
    End With
'   Now take first differences
    For i = Ioff + 1 To iMax - Ioff
      FD(i) = Tavg(i + Ioff) - Tavg(i - Ioff)
    Next i
    For i = 1 To Ioff
      FD(i) = FD(Ioff + 1)
      FD(iMax - i + 1) = FD(iMax - Ioff)
    Next i
'   Now take second differences
    For i = Ioff + 1 To iMax - Ioff
      SD(i) = FD(i + Ioff) - FD(i - Ioff)
    Next i
    For i = 1 To Ioff
      SD(i) = SD(Ioff + 1)
      SD(iMax - i + 1) = SD(iMax - Ioff)
    Next i
'   Now ID max and min values
    SDT = Val(cboSDthreshold.Text)
    n = 1
    Ff(1) = 1
    For i = Ioff + 1 To iMax - Ioff
      If SD(i) < SD(i - 1) And SD(i) < SD(i + 1) And Abs(SD(i)) >= SDT Then n = n + 1: Ff(n) = i
      If SD(i) >= SD(i - 1) And SD(i) >= SD(i + 1) And Abs(SD(i)) >= SDT Then n = n + 1: Ff(n) = i
    Next i
    If n = 1 Then n = 2: Ff(2) = iMax
    nMax = n
    If Ff(n) < iMax Then nMax = nMax + 1: Ff(nMax) = iMax  'Include highest point
    i = 0
    For n = 1 To nMax
      Tplot2(n) = TZraob(Ff(n))
      Zplot2(n) = ZPraob(Ff(n))
      Rplot2(n) = RZraob(Ff(n))
    Next n
  End If
  
  If chkEnableAveraging.Value = 1 Then
' Calculate RMS fit
'  S = 0#: SS = 0#
'  For i = 1 To Imax
'    T1 = TZraob(i)
'    T2 = fLinterp(Zplot2(), Tplot2(), nMax, ZPraob(i))
'    S = S + T2 - T1
'    SS = SS + (T2 - T1) ^ 2
'  Next i
'  AVG = S / Imax
'  RMS = Sqr((SS - Imax * AVG ^ 2) / (Imax - 1))


    s = 0#: SS = 0: Iused = 0
    For i = 1 To iMax
      If ZPraob(i) >= ZpBottom And ZPraob(i) <= ZpTop Then
        Iused = Iused + 1
        T1 = TZraob(i)
        T2 = fLinterp(Zplot2(), Tplot2(), nMax, ZPraob(i)) 'RAOB level interpolated to fit levels
        s = s + T2 - T1
        SS = SS + (T2 - T1) ^ 2
      End If
    Next i
    txtUsed.Text = Iused
    AVG = s / Iused
    RMS = Sqr((SS - Iused * AVG ^ 2) / (Iused - 1))

' Temperature at match point
'  i = 0
'  Do: i = i + 1
'  Loop Until Pr(i) > Zplot2(nMax)
' Extend profile above 5 km
'  iavg = 12 'iavg = i
'  Tm = fLinterp(Pr(), Tr(), nMax, Zplot2(nMax))
'  dT = Tplot2(nMax) - Tm
'  For i = iavg To 11
'    nMax = nMax + 1
'    Zplot2(nMax) = Pr(i)
'    Tplot2(nMax) = Tr(i) + dT
'    Rplot2(nMax) = fLinterp(Qr(), Rr(), 5, Zplot2(nMax))
'  Next i
  Nlev = nMax
  txtNpts.Text = Str(nMax)
  txtAVG.Text = Format(AVG, "#0.00")
  DoEvents

  If Val(txtNpts.Text) < 71 Then

    If RMS < RMSmin Then
      RMSmin = RMS
      txtRMS.Text = Format(RMS, "0.00")
      DoEvents
      cmdCommand_Click (3) 'Clear Plot
      IImin = cboSamplesAveraged.ListIndex
      JJmin = cboSDthreshold.ListIndex

'     Plot better fit
      For i = 1 To iMax
        Zplot(i) = Zplot2(i)
        Tplot(i) = Tplot2(i)
      Next i
      With frmRAOB
      .LineColor = 11   'Light Cyan
      .PlotSingleScan = False
      .ShowDots = True
      .RAOBupdate          'Plot T
      .ShowDots = False
      End With
      DoEvents
            
      If chkShowRH.Value = 1 Then
        For i = 1 To Nlev
          Tsave(i) = Tplot2(i)
          Tplot(i) = Rplot2(i) + 200
        Next i
  
        With frmRAOB
        .LineColor = 12   'Light Red
        .PlotSingleScan = False
        .ShowDots = True
        .RAOBupdate          'Plot RH
        .ShowDots = False
        End With
      End If
      
      DoEvents
      For i = 1 To Nlev
        Tplot(i) = Tsave(i)  'Restore T
      Next i
    End If
  End If
  End If
  
Replot = False
End Sub


Private Sub cboSamplesAveraged_Click()
If Not Start Then cmdCommand_Click (4)
End Sub

Private Sub cboSDthreshold_Click()
If Not Start Then cmdCommand_Click (4)
End Sub

Private Sub chkDoDir_Click()
  ConvertInFile$ = Dir(txtPath.Text + txtMask.Text, vbNormal)

End Sub



Private Sub cmdCommand_Click(Index As Integer)
Dim A$, InData!(3000, 7), x$, Line$, fmt$, ans As Variant
Dim i%, j%, lu%, FirstPass As Boolean, OUTlu%, TT1#(153), TT2#(153)
Static nDir&, RAOB2lu%, nRAOB2count&
'Command:
' 0  Open RAOB file
' 1  Exit
' 2  Write .txt
' 3  Clear Plot
' 4  Replot
' 5  Toggle Scale
' 6  Fix 73
' 7  Search/Best Fit
' 8
'Leeds_200603012_2316.txt
Select Case Index
Case 0   'Open RAOB file
  FirstPass = True
  If chkAutoClear.Value = 1 Then cmdCommand_Click (3)
  nRAOB2count = Val(txtRAOBcount.Text)
  
  RAOB2lu = FreeFile
  Open txtPath.Text + "HKM.RAOB2" For Random As RAOB2lu Len = Len(RAOB2)

NextRAOB:
  If Not Replot Then
    If chkDoDir.Value = 1 Then
      ConvertInFile$ = txtPath.Text + txtMask.Text
      If FirstPass Then
        RAOBfile$ = txtPath.Text + Dir(ConvertInFile$)
        
        FirstPass = False
        nDir = 0
      Else
        RAOBfile$ = txtPath.Text + Dir
      End If
      If RAOBfile$ = "" Then
        Close RAOB2lu
        Exit Sub
      End If
    Else
    ' Set Initial directory
      CommonDialog1.InitDir = txtPath.Text
    ' Set Size of FileName buffer
      CommonDialog1.MaxFileSize = 1024
    ' Set filters.
      CommonDialog1.Filter = "All Files (*.*)|*.*"
    ' Specify default filter.
      CommonDialog1.FilterIndex = 0
    ' Display the File Open dialog box.
      CommonDialog1.ShowOpen

      RAOBfile$ = CommonDialog1.Filename
    End If
    
    If RAOBfile$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
      Exit Sub
    Else
'     On Error GoTo errhandler
      txtRAOBfile.Text = RAOBfile$
      fmt$ = cboRAOBformat.Text
      Call ConvertLargeRAOBfile(RAOBfile$, fmt$)
    End If
    Start = False
    Main
    cmdCommand_Click (7)  ' Do best fit
    ' Best fit makes successive calls to Main
    If chkPause.Value = 1 Then
      Do
        DoEvents
      Loop Until chkPause.Value = 0
    End If
  
  End If
'  cmdCommand_Click (2)   'cmdWrite_Click
  If chkWriteRAOB2.Value = 1 Then
    nDir = nDir + 1
    RAOBgood = True
    Call RAOBwriteSL(RAOB2lu, nDir)
  End If
  DoEvents
  If chkDoDir.Value = 1 Then GoTo NextRAOB
'  If Val(txtNpts.Text) > 70 Or Val(txtNpts.Text) < 30 Or Val(txtRMS.Text) > 0.15 Then cmdSearch_Click
  
Case 1   'Exit
  Unload frmRAOB
  Unload Me
  End

Case 2   'Write .txt
  If Nlev > 150 Then
    Call MsgBox("RAOB has >150 levels; this is too many!", vbOKOnly)
    Exit Sub
  End If
  OUTlu = FreeFile
  Open txtPath.Text + txtFile$ For Output As OUTlu
  
  If UCase$(WMO4l) = "HOU " Then  'Coordinates from John Neilsen-Gammon
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

  x$ = UCase$(WMO4l) + Left$(RAOBfile$, 1) + " " + "2000" + " " + Format(Imonth, "00") + " " + Format(Iday, "00") + " " + Format(Ihour, "00") + " " + Format(Imin, "00") + " " + Format(Nlev, "000") + " " + Format(Val(txtRMS.Text), "0.00")
  Print #OUTlu, x$
  Print #OUTlu, "pALT  T(K)   RH(%)"
  
  For i = 1 To Nlev
      If Zplot(i) < 0 Then
        x$ = Format(Zplot(i), "#0.00") + " "
      Else
        x$ = Format(Zplot(i), "00.00") + " "
      End If
      x$ = x$ + Format(Tplot(i), "000.00") + " "
      x$ = x$ + Format(Rplot(i), "000.0")
      Print #OUTlu, x$
  Next i
  Close OUTlu
  
  If chkAutoOpen.Value = 1 Then cmdCommand_Click (0)
  
Case 3  'Clear Plot
  With frmRAOB
  .SetDefaultScales
  .RAOB_Plot_Set
  .RAOB_Plot_Axes_Labels
  .RAOB_Plot_Grid_Marks
  .RAOB_Plot_Tic_Marks
  End With

Case 4  'Replot
  If Not Search Then cmdCommand_Click (3) 'Clear plot
  Replot = True

  If Search Then Main2 Else Main

Case 5  'Toggle Scale
  If High Then
    With frmRAOB
    .ztkm = 6
    .zbkm = 0         'NB Default values changed when file is opened
    .ztkft = 20
    .zbkft = 0
    .T1K = 260
    .T2K = 310          'XV, YV, etc
    End With
    High = False
  Else
    With frmRAOB
    .ztkm = 25
    .zbkm = 0         'NB Default values changed when file is opened
    .ztkft = 90
    .zbkft = 0
    .T1K = 180
    .T2K = 310          'XV, YV, etc
    End With
    High = True
  End If

  If Not Start Then cmdCommand_Click (4) 'Replot

Case 6  'Fix 73
  pass = False
  OUTlu = FreeFile
  Open txtPath.Text + "Greater70.text" For Output As OUTlu
  Do
    If Not pass Then
      ConvertInFile$ = Dir(txtPath.Text + "*.txt", vbNormal)
      pass = True
    Else
      If ConvertInFile$ = "" Then Exit Sub
    End If
    RAOBfile$ = txtPath.Text + ConvertInFile$
    lu = FreeFile
    Open RAOBfile$ For Input As lu
    Line$ = fReadLF(lu)
  
    Nlev = Mid$(Line$, 23, 3)
    If Nlev > 70 Then Print #OUTlu, ConvertInFile$; Nlev
      
    Close lu
    ConvertInFile$ = Dir
  Loop Until ConvertInFile$ = ""

  Close OUTlu
  
Case 7  'Best Fit/Search
  RMSmin = 1000#
  Search = True
  For j = 0 To cboSDthreshold.ListCount - 1
    cboSDthreshold.ListIndex = j
    For i = 0 To cboSamplesAveraged.ListCount - 1
      cboSamplesAveraged.ListIndex = i
      If Val(txtNpts.Text) < 153 Then
        If Val(txtRMS.Text) < RMSmin Then
         End If
'        Debug.Print IImin; JJmin; RMSmin
      
      End If
    Next i
  Next j
  Search = False
  cboSamplesAveraged.ListIndex = IImin
  cboSDthreshold.ListIndex = JJmin
  
Case 8  'Write .RAOB2 file
  If Nlevr > 153 Then
    ans = MsgBox("RAOB has " + Format(Nlevr, "000") + " levels!" + vbCrLf + "Zp(153)=" + Format(Zplot(153), "#0.00") + " km. Keep it?", vbYesNo, "Warning")
    If ans = vbNo Then Exit Sub
    Nlevr = 153
  End If
    
    For n = 1 To Nlevr
      TZraob(n) = Tplot(n)
      ZPraob(n) = Zplot(n)
      PZraob(n) = fZtoP(ZPraob(n))
      RZraob(n) = Rplot(n)
      ZGraob(n) = ZGplot(n)
    Next n

' Figure out value of z for which lapse rate is -2.
  Call FindRAOBtrop(ZPraob(), TZraob(), Nlevr, Ztrop, Ttrop!, Ltrop, Ztrop2!, Ttrop2!, Ltrop2)
' Find cold point Z and T
  Call FindXmin(1, Nlevr, TZraob(), ZPraob(), 6#, 19#, Tcp, Zcp)
  Tcoldest = Tcp
  Zcoldest = Zcp
  nRAOB2count = nRAOB2count + 1
  txtRAOBcount.Text = Str(nRAOB2count)
  Call RAOBwriteSL(RAOB2lu, nRAOB2count)

End Select

End Sub


















Private Sub Form_Load()
  
  With Me
  .Top = 0
  .Left = 0
  .Height = 5265
  End With
  Start = True
  Search = False
  
  With frmRAOB
  .RAOB_Initialize
  .Top = 0
  .Left = Width
  .Width = Screen.Width - Width
  .Height = Height * 2
  .LineColor = 14   'Light Yellow
  .LineThickness = 1
  .PlotSingleScan = True
  End With
  
  ztkm = 6: zbkm = 0         'NB Default values changed when file is opened
  ztkft = 90: zbkft = 0
  T1K = 260: T2K = 310          'XV, YV, etc

  Set frmRAOB.ExtForm = frmRAOBconvert

  Load frmFormatRAOB                  'Load but make invisible
  With frmFormatRAOB
  .Visible = False
  .chkAircraft.Value = 0
  .chkTropopause.Value = 0
  .chkAircraft.Value = 0
'frmFormatRAOB.chkSelectLevel.value = 1
  .optYcoordl(0).Value = 1
  .optYcoordr(0).Value = 1
  .Hide
  End With

' AvgRAOB for Extending Data
  Pr(1) = 0: Tr(1) = 301.5
  Pr(2) = 4: Tr(2) = 276
  Pr(3) = 6: Tr(3) = 264.1
  Pr(4) = 8: Tr(4) = 249.4
  Pr(5) = 12: Tr(5) = 217.3
  Pr(6) = 14: Tr(6) = 204.5
  Pr(7) = 15: Tr(7) = 200.4
  Pr(8) = 16: Tr(8) = 200
  Pr(9) = 18: Tr(9) = 205.5
  Pr(10) = 21: Tr(10) = 213.1
  Pr(11) = 25: Tr(11) = 220.6

  Qr(1) = 0: RR(1) = 67.9
  Qr(2) = 7: RR(2) = 26
  Qr(3) = 16: RR(3) = 26
  Qr(4) = 21: RR(4) = 11
  Qr(5) = 25: RR(5) = 8.8

  With cboSamplesAveraged
'  .AddItem "1"
'  .AddItem "3"
'  .AddItem "5"
'  .AddItem "7"
  .AddItem "9"
  .AddItem "11"
  .AddItem "13"
  .AddItem "15"
  .ListIndex = 3
  End With
  
  With cboSDthreshold
'  .AddItem "0.05"
'  .AddItem "0.1"
  .AddItem "0.15"
  .AddItem "0.2"
  .AddItem "0.25"
  .AddItem "0.3"
  .AddItem "0.4"
'  .AddItem "0.5"
'  .AddItem "0.6"
'  .AddItem "0.7"
  .ListIndex = 3
  End With

  With cboRAOBformat
  .AddItem "CLS5"
  .AddItem "PNNL"
  .AddItem "TREX"
  .AddItem "FH"
  .AddItem "WY"
  .ListIndex = 4
  End With

Replot = False
High = False

cmdCommand_Click (5)  'Toggle Scale

End Sub


Private Sub mnuOptionsFormatRAOB_Click()
  frmFormatRAOB.Top = frmRAOB.Top + (frmRAOB.Height - frmFormatRAOB.Height) / 2
  frmFormatRAOB.Left = frmRAOB.Left + (frmRAOB.Width - frmFormatRAOB.Width) / 2
  frmFormatRAOB.Visible = True

End Sub


