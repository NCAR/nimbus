VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{FE0065C0-1B7B-11CF-9D53-00AA003C9CB6}#1.1#0"; "COMCT232.OCX"
Begin VB.Form MAPget2 
   AutoRedraw      =   -1  'True
   BackColor       =   &H8000000A&
   Caption         =   "Get PARC GIF MAP"
   ClientHeight    =   3960
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7830
   Icon            =   "MAPget.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3960
   ScaleWidth      =   7830
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "UT"
      Height          =   435
      Left            =   4650
      TabIndex        =   68
      Top             =   2130
      Width           =   1605
      Begin VB.OptionButton optUT 
         Caption         =   "no"
         Height          =   195
         Index           =   2
         Left            =   1020
         TabIndex        =   71
         Top             =   180
         Width           =   555
      End
      Begin VB.OptionButton optUT 
         Caption         =   "hr"
         Height          =   195
         Index           =   1
         Left            =   570
         TabIndex        =   70
         Top             =   180
         Width           =   465
      End
      Begin VB.OptionButton optUT 
         Caption         =   "ks"
         Height          =   195
         Index           =   0
         Left            =   30
         TabIndex        =   69
         Top             =   180
         Value           =   -1  'True
         Width           =   465
      End
   End
   Begin VB.CheckBox chkUTsecs 
      Caption         =   "UT sec"
      Height          =   195
      Left            =   2880
      TabIndex        =   67
      Top             =   1230
      Visible         =   0   'False
      Width           =   855
   End
   Begin VB.CheckBox chkFltTrack 
      Caption         =   "Track"
      Height          =   195
      Left            =   3780
      TabIndex        =   66
      Top             =   1200
      Value           =   1  'Checked
      Width           =   735
   End
   Begin VB.TextBox txtUTksColor 
      BackColor       =   &H0000FFFF&
      Height          =   285
      Left            =   5640
      TabIndex        =   64
      Text            =   "14"
      Top             =   1860
      Width           =   375
   End
   Begin VB.CheckBox chkSecond 
      Caption         =   "Show Second"
      Height          =   255
      Left            =   6360
      TabIndex        =   62
      Top             =   2400
      Width           =   1335
   End
   Begin VB.TextBox txtToKs 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   5760
      MultiLine       =   -1  'True
      TabIndex        =   60
      Text            =   "MAPget.frx":0442
      Top             =   2670
      Width           =   495
   End
   Begin VB.TextBox txtFromKs 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   4920
      MultiLine       =   -1  'True
      TabIndex        =   59
      Text            =   "MAPget.frx":0444
      Top             =   2670
      Width           =   495
   End
   Begin VB.CheckBox chkPreserveMapScale 
      Caption         =   "Preserve Scale"
      Height          =   255
      Left            =   2880
      TabIndex        =   58
      Top             =   2130
      Value           =   1  'Checked
      Width           =   1455
   End
   Begin VB.ComboBox cboPalt 
      Height          =   315
      Left            =   3480
      TabIndex        =   57
      Text            =   "km"
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CheckBox chkShowCities 
      Caption         =   "Show Cities Large Than"
      Height          =   255
      Left            =   2880
      TabIndex        =   54
      Top             =   3030
      Width           =   2055
   End
   Begin VB.ComboBox cboPopulation 
      Height          =   315
      Left            =   4920
      TabIndex        =   53
      Text            =   "100,000"
      Top             =   3030
      Width           =   1095
   End
   Begin VB.CommandButton cmdRead 
      Caption         =   "Read City File"
      Height          =   375
      Left            =   6360
      TabIndex        =   52
      Top             =   2760
      Width           =   1455
   End
   Begin VB.Frame Frame 
      Caption         =   "Out Map Size"
      Height          =   615
      Index           =   2
      Left            =   2040
      TabIndex        =   46
      Top             =   3330
      Width           =   1815
      Begin VB.TextBox txtOutWidth 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   48
         Text            =   "MAPget.frx":0446
         Top             =   240
         Width           =   615
      End
      Begin VB.TextBox txtOutHeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1080
         MultiLine       =   -1  'True
         TabIndex        =   47
         Text            =   "MAPget.frx":044D
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "by"
         Height          =   255
         Index           =   16
         Left            =   840
         TabIndex        =   50
         Top             =   240
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "by"
         Height          =   255
         Index           =   15
         Left            =   1680
         TabIndex        =   49
         Top             =   600
         Width           =   255
      End
   End
   Begin VB.CommandButton cmdResize 
      Caption         =   "Refresh"
      Height          =   375
      Left            =   6360
      TabIndex        =   42
      Top             =   2010
      Width           =   1455
   End
   Begin VB.Frame Frame 
      Caption         =   "In Map Size"
      Height          =   615
      Index           =   1
      Left            =   120
      TabIndex        =   41
      Top             =   3330
      Width           =   1815
      Begin VB.TextBox txtInHeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1080
         MultiLine       =   -1  'True
         TabIndex        =   45
         Top             =   240
         Width           =   615
      End
      Begin VB.TextBox txtInWidth 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   43
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "by"
         Height          =   255
         Index           =   14
         Left            =   840
         TabIndex        =   44
         Top             =   240
         Width           =   255
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "Show RAOBs"
      Height          =   615
      Index           =   0
      Left            =   3960
      TabIndex        =   37
      Top             =   3330
      Width           =   2415
      Begin VB.OptionButton optRAOB 
         Caption         =   "None"
         Height          =   255
         Index           =   2
         Left            =   1560
         TabIndex        =   40
         Top             =   240
         Width           =   735
      End
      Begin VB.OptionButton optRAOB 
         Caption         =   "WMO"
         Height          =   255
         Index           =   1
         Left            =   840
         TabIndex        =   39
         Top             =   240
         Width           =   735
      End
      Begin VB.OptionButton optRAOB 
         Caption         =   "ICAO"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   38
         Top             =   240
         Value           =   -1  'True
         Width           =   735
      End
   End
   Begin ComCtl2.UpDown UpDown1 
      Height          =   255
      Left            =   6000
      TabIndex        =   36
      Top             =   1620
      Width           =   240
      _ExtentX        =   423
      _ExtentY        =   450
      _Version        =   327681
      Enabled         =   -1  'True
   End
   Begin VB.TextBox txtLineColor 
      BackColor       =   &H0000FFFF&
      Height          =   285
      Left            =   5640
      TabIndex        =   35
      Text            =   "14"
      Top             =   1590
      Width           =   375
   End
   Begin VB.ComboBox cboDrawWidth 
      Height          =   315
      Left            =   5640
      TabIndex        =   32
      Text            =   "1"
      Top             =   1230
      Width           =   615
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   6480
      Top             =   3450
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CheckBox chkAutoscaleTrack 
      Caption         =   "Autoscale"
      Height          =   255
      Left            =   2880
      TabIndex        =   31
      Top             =   2430
      Width           =   1095
   End
   Begin VB.ComboBox cboTrack 
      Height          =   315
      Left            =   3480
      TabIndex        =   30
      Top             =   1440
      Width           =   1095
   End
   Begin VB.CheckBox chkHiliteSelection 
      Caption         =   "Hilite Selection from"
      Height          =   255
      Left            =   2880
      TabIndex        =   29
      Top             =   2670
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton cmdReadRAWfile 
      Caption         =   "Read RAW File"
      Height          =   375
      Left            =   6330
      TabIndex        =   24
      Top             =   1230
      Width           =   1455
   End
   Begin VB.CommandButton cmdLoadImage 
      Caption         =   "Load Map"
      Height          =   375
      Left            =   6360
      TabIndex        =   23
      Top             =   1620
      Width           =   1455
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   495
      Left            =   7080
      TabIndex        =   21
      Top             =   3450
      Width           =   735
   End
   Begin VB.Frame Frame4 
      Caption         =   "Map Window"
      Height          =   1695
      Left            =   120
      TabIndex        =   12
      Top             =   1320
      Width           =   2655
      Begin VB.TextBox txtLON3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   27
         Text            =   "MAPget.frx":0453
         Top             =   1320
         Width           =   495
      End
      Begin VB.TextBox txtLAT3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   600
         MultiLine       =   -1  'True
         TabIndex        =   25
         Text            =   "MAPget.frx":0457
         Top             =   1320
         Width           =   495
      End
      Begin VB.CommandButton cmdDefault 
         Caption         =   "Update Default"
         Height          =   315
         Left            =   1200
         TabIndex        =   22
         Top             =   240
         Width           =   1335
      End
      Begin VB.TextBox txtLAT1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   600
         MultiLine       =   -1  'True
         TabIndex        =   16
         Text            =   "MAPget.frx":045B
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtLAT2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   600
         MultiLine       =   -1  'True
         TabIndex        =   15
         Text            =   "MAPget.frx":0460
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtLON1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1320
         MultiLine       =   -1  'True
         TabIndex        =   14
         Text            =   "MAPget.frx":0465
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtLON2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   13
         Text            =   "MAPget.frx":046B
         Top             =   960
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Step"
         Height          =   255
         Index           =   13
         Left            =   1320
         TabIndex        =   28
         Top             =   1320
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Step"
         Height          =   255
         Index           =   12
         Left            =   120
         TabIndex        =   26
         Top             =   1320
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "LAT1"
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   20
         Top             =   960
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "LAT2"
         Height          =   255
         Index           =   3
         Left            =   120
         TabIndex        =   19
         Top             =   600
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "LON1"
         Height          =   255
         Index           =   5
         Left            =   1320
         TabIndex        =   18
         Top             =   600
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "LON2"
         Height          =   255
         Index           =   6
         Left            =   2040
         TabIndex        =   17
         Top             =   600
         Width           =   495
      End
      Begin VB.Line Line2 
         X1              =   1200
         X2              =   1200
         Y1              =   600
         Y2              =   1560
      End
   End
   Begin VB.TextBox txtGIFfile 
      Height          =   285
      Left            =   840
      TabIndex        =   10
      Top             =   840
      Width           =   4965
   End
   Begin VB.ComboBox cboPlatforms 
      Height          =   315
      Left            =   840
      TabIndex        =   4
      Text            =   "WB57"
      Top             =   120
      Width           =   855
   End
   Begin VB.ComboBox cboMissions 
      Height          =   315
      Left            =   2400
      TabIndex        =   3
      Text            =   "SUCCESS"
      Top             =   120
      Width           =   1215
   End
   Begin VB.ComboBox cboFltDates 
      Height          =   315
      Left            =   4560
      TabIndex        =   2
      Text            =   "19999999"
      Top             =   120
      Width           =   1215
   End
   Begin VB.ComboBox cboFltNos 
      Height          =   315
      Left            =   6600
      TabIndex        =   1
      Text            =   "19999999"
      Top             =   120
      Width           =   1215
   End
   Begin VB.ComboBox cboObjectives 
      Height          =   315
      Left            =   840
      TabIndex        =   0
      Text            =   "TBD"
      Top             =   480
      Width           =   6975
   End
   Begin ComCtl2.UpDown UpDown2 
      Height          =   255
      Left            =   6000
      TabIndex        =   65
      Top             =   1890
      Width           =   240
      _ExtentX        =   423
      _ExtentY        =   450
      _Version        =   327681
      Enabled         =   -1  'True
   End
   Begin VB.Label Label 
      Caption         =   "UT Color"
      Height          =   165
      Index           =   21
      Left            =   4680
      TabIndex        =   63
      Top             =   1890
      Width           =   855
   End
   Begin VB.Label Label 
      Caption         =   "to"
      Height          =   255
      Index           =   20
      Left            =   5520
      TabIndex        =   61
      Top             =   2670
      Width           =   255
   End
   Begin VB.Label Label 
      Caption         =   "pALT"
      Height          =   255
      Index           =   19
      Left            =   2880
      TabIndex        =   56
      Top             =   1800
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "UT"
      Height          =   255
      Index           =   18
      Left            =   2880
      TabIndex        =   55
      Top             =   1440
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Double Click Form to Save"
      Height          =   255
      Index           =   17
      Left            =   5850
      TabIndex        =   51
      Top             =   840
      Width           =   1935
   End
   Begin VB.Label Label 
      Caption         =   "Track Color"
      Height          =   195
      Index           =   8
      Left            =   4680
      TabIndex        =   34
      Top             =   1620
      Width           =   855
   End
   Begin VB.Label Label 
      Caption         =   "Track Width"
      Height          =   255
      Index           =   7
      Left            =   4680
      TabIndex        =   33
      Top             =   1230
      Width           =   975
   End
   Begin VB.Line Line3 
      X1              =   1200
      X2              =   2400
      Y1              =   2520
      Y2              =   2520
   End
   Begin VB.Line Line1 
      X1              =   0
      X2              =   7800
      Y1              =   1200
      Y2              =   1200
   End
   Begin VB.Label Label 
      Caption         =   "PNG File"
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   11
      Top             =   840
      Width           =   645
   End
   Begin VB.Label Label 
      Caption         =   "Platform"
      Height          =   255
      Index           =   9
      Left            =   120
      TabIndex        =   9
      Top             =   120
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Mission"
      Height          =   255
      Index           =   10
      Left            =   1800
      TabIndex        =   8
      Top             =   120
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Flight Date"
      Height          =   255
      Index           =   11
      Left            =   3720
      TabIndex        =   7
      Top             =   120
      Width           =   855
   End
   Begin VB.Label Label 
      Caption         =   "Flight No"
      Height          =   255
      Index           =   0
      Left            =   5880
      TabIndex        =   6
      Top             =   120
      Width           =   735
   End
   Begin VB.Label Label 
      Caption         =   "Purpose"
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   5
      Top             =   480
      Width           =   615
   End
End
Attribute VB_Name = "MAPget2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function StretchBlt Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long
Private Const SRCCOPY = &HCC0020
'

Dim ThetaStart!, ThetaEnd!, ThetaStep!, ThetaLevels%
Dim UseCounts As Boolean, Xstep!, Ystep!
Dim FirstMouseX%, LastMouseX%, LastMouseXSave%, FirstMouseXSave%
Dim FirstMouseY%, LastMouseY%, LastMouseYSave%, FirstMouseYSave%
Dim EnableCXY As Boolean, EnableMXY As Boolean
Dim YC0save%, YC1save%, YC2save%, YC3save%  'CTS
Dim YG0save%, YG1save%, YG2save%, YG3save%  'GAIN
Dim YM0save%, YM1save%, YM2save%, YM3save%  'Mux
Dim YT0save%, YT1save%, YT2save%, YT3save%  'TB
Dim YP0save%, YP1save%, YP2save%, YP3save%  'PT
Dim rtt!(1 To 33), ob!(0 To 32)    'Raw observables ob 0-19 for ER2, 0-28 for DC8
Dim RT1!(1 To 33), RT2!(1 To 33), Pause As Boolean, ShowNDP As Boolean, SingleStep As Boolean
Dim SaveOutput As Boolean, DoRetrieval As Boolean, Lindex%
Dim PTWfile$, TakeOff As Boolean, FLTINFOfile$, ACX$

Public RAWfile$, CALfile$, MMSfile$, REFfile$, LOGfile$, OUTfile$, Remote%
Public RAWlu%, MMSlu%, REFlu%, CALlu%, LOGlu%, Quit As Boolean
Public ByteCount&, LOFraw&, dZg!, LOFbin&, RecordCount&, BINrecords&
Dim ArrayRead As Boolean               'TRUE if RAW File has been read into memory

Public EndRecord%           'Largest record read
Public PText$               'File extension
Dim MV!()                   'Mux Value Data Array (same as muxs!())
Dim MVindex%                'Current record
Dim MVchannels%             'Number of MUX channels
Dim MUXV%()                 'Raw mux count values (same as mux%())
Dim Tx$()                   'Text string array for Tooltip
Dim Tmtp_Tnav!()            'Tmtp - Tnav difference
Dim GV!()                   'Gain Data Array
Dim GVindex%                'Current record
Dim GVchannels%             'Number of Gain channels
Dim CV%()                   'Counts Array
Dim mA!()                   'Moving average array (i, j)
Dim EV%()                   'Moving Average Counts Array (i, j, r)
Dim Ew%()                   'Edit Word
Dim MW%()                   'Make Word
Dim NW%()                   'Nlev, Nlev1, Nlev2
Dim ZW!()                   'Z levels
Dim Tw!()                   'T levels
Dim UV&()                   'UTsec, UTsecMTP, UTsecNav
Dim UVindex%                'Current record
Dim OATV!()                 'Tnav, Tmms, Tmtp
Dim NAVV!()                 'Pitch, Roll, Latitude, Longitude, Heading, TAS, Wspd, Wdir, Elcor, ElcorUsed
Dim ALTV!()                 'pALT, gALT, rALT, mALT
Dim TTOV!()                 'Time Tag Offsets (TTO, TTO1, TTO2, TTO3)
Dim CQV() As Boolean        'Count Quality Flag - Fast
Dim NQV() As Boolean        'NAV Quality Flag
Dim CQM() As Integer        'Counts Quality Bitmap
Dim CQVS() As Boolean       'Counts Quality Flag - Slow
Dim AttitudeOK() As Boolean 'PitchOK, RollOK and fEcOK

Dim zPT!()                  'Altitude of IAC levels (1D)
Dim Tptw!()                 'Array of IAC Altitude levels (2D)

Dim Rt!(1 To 33)                    'Current ATP temperature (K)
Dim RA!(1 To 33)                    'Current ATP altitude (km)
Dim RTavg!(1 To 33)                 'Average ATP temperature
Dim RAavg!(1 To 33)                 'Average ATP altitude
Dim RTsave!(1 To 4, 1 To 33)        'Past four cycles - temperature
Dim RAsave!(1 To 4, 1 To 33)        'Past four cycles - altitude
Dim NoD!(1 To 33)                   'Number density profile
Dim dZgeo!(1 To 33)                 'Corrections for geometric altitude wrt flight level
Dim Trop1!                          'Trop 1 altitude (km)
Dim TTrop1!                         'Trop 1 temperature (K)
Dim Trop2!                          'Trop 2 altitude (km)
Dim Ttrop2!                         'Trop 2 temperature (K)
Dim defLAT1!                        'Default latitude for start of tropical to mid-latitude transition
Dim defLAT2!                        'Default latitude for end of tropical to mid-latitude transition
Dim defLAT3!                        'Default latitude for start of mid-latitude to polar transition
Dim defLAT4!                        'Default latitude for end of mid-latitude to polar transition
Dim Ccycle%                         'Count Quality Count
Dim zt0!                            'BLG tropopause height (km)
Dim Tzt0!                           'BLG tropopause temperature (K)
Dim vWB$                            'Current version of program

Dim GeqnGood(1 To 3) As Boolean     'TRUE if Gain Equation is good
Dim GnavGood(1 To 3) As Boolean     'TRUE if Nav Gain is good
Dim GndGood(1 To 3) As Boolean      'TRUE if Noise Diode Gain is good
Dim GmmsGood(1 To 3) As Boolean     'TRUE if MMS Gain is good

Dim NAVgainCount%                   'Number of good NAV gains
Dim GEgainCount%                    'Number of good Gain Equation gains
Dim NDgainCount%                    'Number of good Noise Diode gains
Dim BadCounts&                      'Number of bad counts
Dim FTPcount%                       'Number of FTP transmissions

Dim RTdrive$                        'Real Time Drive
Dim AutoScaleTA As Boolean          'Automatically scale antenna temperature display
Dim ALLlu%                          'Used in fCheckCtsQuality, but never set
Dim flag%()                         'MA counts quality flag
Dim cTs%()                          'Counts MA array
Dim MLC!()                          'Tmxr MA array
Dim TLC!()                          'Ttgt MA array
Dim WLC!()                          'Twin MA array
Dim OLC!()                          'Tnav MA array

Dim BadNr%                          'Number of bad scans
Dim pALTtakeoff!                    'Altitude at take off
Dim Scanlength!

Private Sub cmdStretchBlt_Click()
Dim num_trials As Integer
Dim trial As Integer
Dim start_time As Single
Dim fr_wid As Single
Dim fr_hgt As Single
Dim to_wid As Single
Dim to_hgt As Single

    'num_trials = CInt(txtTrials.Text)
'    fr_wid = picFrom.ScaleWidth
'    fr_hgt = picFrom.ScaleHeight
'    to_wid = picTo.ScaleWidth
'    to_hgt = picTo.ScaleHeight'

'    picTo.Cls
'    lblTime.Caption = ""
'    MousePointer = vbHourglass
'    start_time = Timer
'    For trial = 1 To num_trials
'        StretchBlt _
'            picTo.hDC, 0, 0, to_wid, to_hgt, _
'            picFrom.hDC, 0, 0, fr_wid, fr_hgt, _
'            SRCCOPY
'        DoEvents
'    Next trial
'    MousePointer = vbDefault
''
'    lblTime.Caption = Format$(Timer - start_time, "0.00") & " secs"
End Sub


Sub UpdateFlightTrack2()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, Xoffset!, Yoffset!, DotRadius!
Dim LAmin!, LAmax!, LOmin!, LOmax!, r%, iStep%, iMax%, Lat!, Lon!, iUTks%
Dim INPlu%, Record&, L!, xScale!, yScale!, x!, DayChange As Boolean
Dim iUThr%, sUThr!

'  Latitude = NAVV(3, r)
'  Longitude = NAVV(4, r)
frmPic2.Cls
frmPic2.Caption = "MTP " + ACX$ + " " + Mission$ + " Flight Track for " + Trim(cboFltDates.Text)
'frmPic2.picTrack2.Cls
'Plot Flight Track in picture window
'  With frmPic2
'  .picTrack2.Cls
'  .picTrack2.ScaleMode = 0
'  .picTrack2.ScaleTop = 0
'  .picTrack2.ScaleHeight = 15
'  End With

' Set temperature display scale
frmPic2.picTrack2.DrawWidth = 1
If chkAutoscaleTrack Then
    LOmin = Int(LONmin - 1)
    LOmax = Int(LONmax + 1)
    frmPic2.picTrack2.ScaleWidth = LOmax - LOmin
    frmPic2.picTrack2.ScaleLeft = LOmin
    txtLON1.Text = Format(LOmin, "##0")
    txtLON2.Text = Format(LOmax, "##0")

    LAmin = Int(LATmin - 1)
    LAmax = Int(LATmax + 1)
    frmPic2.picTrack2.ScaleHeight = LAmin - LAmax
    frmPic2.picTrack2.ScaleTop = LAmax
    txtLAT1.Text = Format(LAmin, "##0")
    txtLAT2.Text = Format(LAmax, "##0")
  Else
    LOmin = Val(txtLON1.Text)
    frmPic2.picTrack2.ScaleLeft = LOmin
    frmPic2.picTrack2.ScaleWidth = Val(txtLON2.Text) - Val(txtLON1.Text)
    LOmax = frmPic2.picTrack2.ScaleLeft + frmPic2.picTrack2.ScaleWidth
    
    
    LAmax = Val(txtLAT2.Text)
    LAmin = Val(txtLAT1.Text)
    If LAmax > LAmin Then
      frmPic2.picTrack2.ScaleTop = LAmax
      frmPic2.picTrack2.ScaleHeight = Val(txtLAT1.Text) - Val(txtLAT2.Text)
      LAmin = frmPic2.picTrack2.ScaleTop + frmPic2.picTrack2.ScaleHeight
    Else
      frmPic2.picTrack2.ScaleTop = LAmin
      LAmax = LAmin
      frmPic2.picTrack2.ScaleHeight = Val(txtLAT2.Text) - Val(txtLAT1.Text)
      LAmin = frmPic2.picTrack2.ScaleTop + frmPic2.picTrack2.ScaleHeight
    End If
  End If
  
' Draw Longitude-tic marks
  For L = LAmin To LAmax Step Val(txtLAT3.Text)
    frmPic2.picTrack2.PSet (LOmin, L)
    frmPic2.picTrack2.Line -(LOmax, L), QBColor(5)
  Next L
  iMax = (LOmax - LOmin) / Val(txtLON3.Text)
  xScale = frmPic2.picTrack2.Width
  Xstep = xScale / iMax
  frmPic2.FontSize = 10
  frmPic2.Font.Bold = True
  Yoffset! = 0.5 * frmPic2.TextHeight("2")
  Xoffset = 1.5 * frmPic2.TextWidth("2")
  For i = 0 To iMax
    frmPic2.CurrentY = frmPic2.picTrack2.Top + frmPic2.picTrack2.Height + Yoffset
    frmPic2.CurrentX = frmPic2.picTrack2.Left + Xstep * i - Xoffset
    Lon = LOmin + Val(txtLON3.Text) * i
    If Int(Val(txtLON3.Text)) = Val(txtLON3.Text) Then
      If Lon < 10 Then
        frmPic2.Print Format$(Lon, "##0")
      Else
        frmPic2.Print Format$(Lon, "##0#")
      End If
    Else
      If Lon < 10 Then
        frmPic2.Print Format$(Lon, "##0.0")
      Else
        frmPic2.Print Format$(Lon, "##0.0")
      End If
    End If
  Next i

'00 gray
'01
'02
'03
'04
'05 purple
'06 lgreen
'07 l gray
'08 gray
'09 blue
'10 green
'11 cyan
'12 red
'13 magenta
'14 yellow
'15 white
' Draw Longitude-tic marks
  For L = LOmin To LOmax Step Val(txtLON3.Text)
    frmPic2.picTrack2.PSet (L, LAmin)
    frmPic2.picTrack2.Line -(L, LAmax), QBColor(5)
  Next L

  iMax = (LAmax - LAmin) / Val(txtLAT3.Text)
  xScale = -frmPic2.picTrack2.Height
  Xstep = xScale / iMax
  frmPic2.FontSize = 10
  frmPic2.Font.Bold = True
  Yoffset! = frmPic2.TextHeight("2") / 2
  Xoffset = frmPic2.picTrack2.Left - 25
  For i = 0 To iMax
    frmPic2.CurrentX = Xoffset
    frmPic2.CurrentY = frmPic2.picTrack2.Top + frmPic2.picTrack2.Height - Yoffset + Xstep * i
    Lat = LAmin + Val(txtLAT3.Text) * i
    If Int(Val(txtLAT3.Text)) = Val(txtLAT3.Text) Then
      If Abs(Lat) < 10 Then
        frmPic2.Print Format$(Lat, "0#")
      Else
        frmPic2.Print Format$(Lat, "##")
      End If
    Else
      frmPic2.CurrentX = frmPic2.picTrack2.Left - 30
      If Lat < 10 Then
        frmPic2.Print Format$(Lat, "0#.0")
      Else
        frmPic2.Print Format$(Lat, "##.0")
      End If
    End If
  Next i
  With frmPic2
  .FontSize = 8
  .Font.Bold = False
  .CurrentX = Xoffset
  .CurrentY = frmPic2.picTrack2.Top + frmPic2.picTrack2.Height + 3.5 * Yoffset
  End With
  frmPic2.Print "mjm " + Date$
  If chkFltTrack.Value = 0 Then GoTo RAOBs

' Draw flight track
  frmPic2.FontSize = 10
  frmPic2.Font.Bold = True
  frmPic2.picTrack2.DrawWidth = Val(Trim(cboDrawWidth.Text))
  frmPic2.picTrack2.ForeColor = txtLineColor.BackColor
  frmPic2.picTrack2.PSet (NAVV(4, 1), NAVV(3, 1))
  If chkHiliteSelection.Value = 1 Then
    For r = 2 To RawCycle
      If UV(1, r) < Val(txtFromKs.Text) * 1000 Or UV(1, r) > Val(txtToKs.Text) * 1000 Then
        LineC = frmPic2.picTrack2.ForeColor
      Else
        LineC = QBColor(12)
      End If
      frmPic2.picTrack2.Line -(NAVV(4, r), NAVV(3, r)), LineC  'And continue from there
      If NAVV(3, r) < 24 Then
      x = x
      End If
    Next r
  Else
    For r = 2 To TotalCycles
      frmPic2.picTrack2.Line -(NAVV(4, r), NAVV(3, r)), frmPic2.picTrack2.ForeColor  'And continue from there
    Next r
  End If


If optUT(0).Value = True Then
' Draw UT ks markers
  iUTks = Int(UV(1, 1) / 1000)
  iUTks = iUTks + 1
  DayChange = False
  frmPic2.picTrack2.FillStyle = 0
  DotRadius = (Val(txtLAT2.Text) - Val(txtLAT1.Text)) / 200#
  r = 1
  Do
    r = r + 1
    If UV(1, r) < UV(1, r - 1) Then
      DayChange = True
    End If
    If DayChange Then UV(1, r) = UV(1, r) + 86400
    If UV(1, r) / 1000 >= iUTks Then
      frmPic2.picTrack2.ForeColor = QBColor(9)
      frmPic2.picTrack2.FillColor = QBColor(9)
      frmPic2.picTrack2.Circle (NAVV(4, r), NAVV(3, r)), Abs(DotRadius)
      frmPic2.picTrack2.CurrentX = NAVV(4, r) + 0.04
      frmPic2.picTrack2.CurrentY = NAVV(3, r)
      frmPic2.picTrack2.ForeColor = QBColor(Val(txtUTksColor.Text))
      frmPic2.picTrack2.Print Format(iUTks, "00")
      iUTks = iUTks + 1
    End If
  Loop Until r = TotalCycles
End If

If optUT(1).Value = True Then
' Draw UT hr markers
  iUThr = Int(UV(1, 1) / 3600)
  sUThr = 0.25 * Int(UV(1, 1) / 900)  'Quarter hour marks
  iUThr = iUThr + 1
  DayChange = False
  frmPic2.picTrack2.FillStyle = 0
  DotRadius = (Val(txtLAT2.Text) - Val(txtLAT1.Text)) / 200#
  r = 1
  Do
    r = r + 1
    If UV(1, r) < UV(1, r - 1) Then
      DayChange = True
    End If
    If DayChange Then UV(1, r) = UV(1, r) + 86400
    If UV(1, r) / 3600 >= sUThr Then
      ' Draw circle
      frmPic2.picTrack2.ForeColor = QBColor(9)
      frmPic2.picTrack2.FillColor = QBColor(9)
      frmPic2.picTrack2.Circle (NAVV(4, r), NAVV(3, r)), Abs(DotRadius)
      sUThr = sUThr + 0.25
      ' Draw UT
      frmPic2.picTrack2.CurrentX = NAVV(4, r) + 0.04
      frmPic2.picTrack2.CurrentY = NAVV(3, r)
      frmPic2.picTrack2.ForeColor = QBColor(Val(txtUTksColor.Text))
      If UV(1, r) / 3600 >= iUThr Then
        If iUThr < 10 Then
          frmPic2.picTrack2.Print Format(iUThr, "0")
        Else
          frmPic2.picTrack2.Print Format(iUThr, "00")
        End If
        iUThr = iUThr + 1
      End If
    End If
  Loop Until r = TotalCycles
End If

RAOBs:
' Draw RAOB Sites
  If optRAOB(2).Value = False Then

  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.bin" For Random Access Read Write As INPlu Len = Len(GP)
  
  Record = 0
  frmPic2.picTrack2.ForeColor = QBColor(12)
  Do
    Record = Record + 1
    GPread INPlu, Record
    If GPlatitude > Val(txtLAT1.Text) And GPlatitude < Val(txtLAT2.Text) Then
      If Val(txtLON1.Text) < -180 Then  'Fix for IDT
        If GPlongitude > 0 Then GPlongitude = GPlongitude - 360
      End If
        If GPlongitude > Val(txtLON1.Text) And GPlongitude < Val(txtLON2.Text) Then
    Debug.Print Record; WMOnumber; ICAOsite; GPlatitude; GPlongitude
          frmPic2.picTrack2.CurrentX = GPlongitude
          frmPic2.picTrack2.CurrentY = GPlatitude
          frmPic2.picTrack2.Circle (GPlongitude, GPlatitude), DotRadius
          frmPic2.picTrack2.FillColor = QBColor(12)
          frmPic2.picTrack2.CurrentX = GPlongitude + 2 * DotRadius
          If GPicao <> "        " And optRAOB(0).Value = True Then
            frmPic2.picTrack2.Print ICAOsite 'Labels
          Else
            frmPic2.picTrack2.Print WMOnumber 'Labels
          End If
        End If
    End If
  Loop Until EOF(INPlu)
  Close INPlu
  End If
  
  If Mission$ = "TexAQS" Then
'-95.36  29.77 HOU
'-95.05  29.39 HSE
    frmPic2.picTrack2.CurrentX = -95.36
    frmPic2.picTrack2.CurrentY = 29.77
    frmPic2.picTrack2.Circle (-95.36, 29.77), DotRadius
    frmPic2.picTrack2.FillColor = QBColor(12)
    frmPic2.picTrack2.CurrentX = -95.36 + 2 * DotRadius
    frmPic2.picTrack2.Print "HOU"
    frmPic2.picTrack2.CurrentX = -95.05
    frmPic2.picTrack2.CurrentY = 29.39
    frmPic2.picTrack2.Circle (-95.05, 29.39), DotRadius
    frmPic2.picTrack2.FillColor = QBColor(12)
    frmPic2.picTrack2.CurrentX = -95.05 + 2 * DotRadius
    frmPic2.picTrack2.Print "HSE"
    
  End If
  ' Draw Cities
  If chkShowCities.Value = 1 Then

  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\CityFile.bin" For Random Access Read Write As INPlu Len = Len(CITY)
  
  Record = 0
  frmPic2.picTrack2.ForeColor = QBColor(12)
  Do
    Record = Record + 1
    CITYread INPlu, Record
    If CityPopulation > Val(Trim(cboPopulation.Text)) Then
      If CityLatitude > Val(txtLAT1.Text) And CityLatitude < Val(txtLAT2.Text) Then
        If Val(txtLON1.Text) < -180 Then  'Fix for IDT
          If CityLongitude > 0 Then CityLongitude = CityLongitude - 360
        End If
        If CityLongitude > Val(txtLON1.Text) And CityLongitude < Val(txtLON2.Text) Then
          frmPic2.picTrack2.CurrentX = CityLongitude
          frmPic2.picTrack2.CurrentY = CityLatitude
          frmPic2.picTrack2.Circle (CityLongitude, CityLatitude), DotRadius
          frmPic2.picTrack2.FillColor = QBColor(12)
          frmPic2.picTrack2.CurrentX = CityLongitude + 2 * DotRadius
          frmPic2.picTrack2.Print CityName
        End If
      End If
    End If
  Loop Until EOF(INPlu)
  Close INPlu
  End If
  
  End Sub

Sub UpdateForm()
Dim i%, n$, LI, RCcmd$

  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$
  
  For i = 0 To cboPlatforms.ListCount - 1
    If cboPlatforms.List(i) = Mid$(Rdir$, 2, Len(Rdir$) - 2) Then cboPlatforms.ListIndex = i: Exit For
  Next i
  
  'LoadComboBox frmFLTINFO.cboMissions, Trim(cboPlatforms.Text)

End Sub


Sub UpdateSourceFile()

Select Case AC$
Case "ER": ACX$ = "ER-2"
Case "DC": ACX$ = "DC-8"
Case "WB": ACX$ = "WB-57"
Case "EL": ACX$ = "Electra"
End Select
'yyyymmdd$ = Trim(cboFltDates.Text)
'yymmdd$ = Right$(yyyymmdd$, 6)
'txtGIFfile.Text = DataDrive$ + Rdir$ + Mission$ + "\" + Trim(cboFltDates.Text) + "\Map" + Trim(cboFltDates.Text) + ".gif"
txtGIFfile.Text = MNpath$ + "PNG\Track_" + Trim(cboFltDates.Text) + AC$ + ".png"
'txtDestinationFile.Text = DataDrive$ + Rdir$ + Mission$ + "\" + yyyymmdd$ + "\" + AC$ + yyyymmdd$ + "." + cboDestination.Text
frmPic2.Caption = "MTP " + ACX$ + " " + Mission$ + " Flight Track for " + Trim(cboFltDates.Text)
End Sub


Sub ReadRAWrecordX(Quit As Boolean)
Dim i%, j%, A$, b$, UTks!, x$, UT&, Th!, U!, V!, W!, c1$
Dim GoToNext As Boolean, TtgtFix As Boolean
Static UTsecLast&, Day, Change As Boolean, ALTftLast&, lu%, Poff!
  
  If RawCycle = 0 Then UTsecLast = 0  'Make sure reset in case of multiple passes
  GoSub ReadABCDElines               'Read five lines of input data

' Note: Cycle only counts good cycles, but ML and TL use all cycles.
  RawCycle% = RawCycle% + 1                     'Increment total raw cycle count
  Cycle = RawCycle
  Aline = False
  WriteArray (RawCycle) 'Call BINwrite(BINlu, RawCycle)
  CheckLimits (RawCycle) 'Lat Lon check after fix in WriteArray
EndOfSub:
Exit Sub

ReadABCDElines:
TryAgain:
  If EOF(RAWlu) Then
    If Not RealTime Then Quit = True
    GoTo EndOfSub
  End If
' Get an input line from RAW File
  Input #RAWlu%, A$
  If Len(A$) < 5 Then GoTo TryAgain
    
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" Then
    GoTo TryAgain
    End If
    Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
    If UTsecLast > UTsec Then UTsec = UTsec + 86400
    pALT = ALTkm
  
  Case Else
    GoTo TryAgain
  End Select

Return

End Sub
Sub ReadRAWrecord(Quit As Boolean)
Dim i%, j%, A$, b$, UTks!, x$, UT&, c1$, c2$
Dim GoToNext As Boolean, CycleTakeoff%
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, Lt$
Dim Wu!, Wv!, Ww!, param!(1 To 16), P!, T!, Th!, ALTlast!, dALT!
Static UTsecLast&, Day, Change As Boolean, pALTftLast&, lu%, Poff!, UTsecMTPlast&, dtMTP_NAV&
  
  If RawCycle = 0 Then UTsecLast = 0   'Make sure reset in case of multiple passes
  If AC$ = "NG" Then
    GoSub ReadGVscan
  Else
    GoSub ReadABCDElines               'Read five or six lines of input data
  End If

' Note: Cycle only counts good cycles, but ML and TL use all cycles.
  RawCycle% = RawCycle% + 1            'Increment total raw cycle count
  Cycle = RawCycle
  If NavQual Then NQV(RawCycle) = True Else NQV(RawCycle) = False
' Save raw Thi/Tlo values in ch 5 and 6 but change Muxs(0) if
' Ttgt is editted  'Needs to be after Cycle INC!
'  If chkEditTtgt.Value = 1 Then   'xxx mjm 20001023 Was AND not OR
  ' 20020515 mjm Ttgt corrupted after scan 30 on flight 20020514 on WB57
'    Ttgt = fTtgtFix(EditTtgt, TtgtFix, Ttgt, RawCycle%)
'    If TtgtFix Then TtgtPC = TtgtPC + 1: frmFLTINFO.txtPC(14) = TtgtPC
'    Muxs(0) = Ttgt
'  End If
  Aline = False
'  If chkZgZp.Value = 1 Then
'    If RawCycle = 1 Then
'      lu = FreeFile
'      Open Drive$ + Rdir2$ + Mission$ + "\" + yyyymmdd$ + "\ZgZp" + yyyymmdd$ + ".txt" For Output As lu
'      Print #lu, " UTks Zp  Zg  Zg-Zp"
'    Else
'      Print #lu, UTsec / 1000; pALT; gALT; (gALT - pALT) * 1000
'    End If
'  End If
' Get rid of ch1 if necessary
'  Select Case Mission$
'  Case "EUPLEX"
'    Select Case yyyymmdd$
'    Case "20030115", "20030119"
'      For i = 1 To 2
'        For j = 1 To 12
'          c(i, j) = c(i + 1, j)
'        Next j
'        CS(i) = c(i + 1, LocHor): CB(i + 1) = c(i, 12): dND(i + 1) = c(i + 1, 11) - CB(i + 1)
'      Next i
'    End Select
'  End Select

' Write current cycle to memory
  WriteArray (RawCycle) 'Call BINwrite(BINlu, RawCycle)
  CheckLimits (RawCycle) 'Lat Lon check after fix in WriteArray
EndOfSub:
Exit Sub

' Process GV scan
ReadGVscan:
  If Cycle > 0 Then GoTo ProcessNextAlineGV
TryAgainGV:
  If EOF(RAWlu) Then
    If Not RealTime Then Quit = True
    Exit Sub
  End If
' Get an input line from RAW File
  Line Input #RAWlu%, A$
'  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
'  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  DoEvents
  If Len(A$) < 5 Then GoTo TryAgainGV
    
  Select Case Mid$(A$, 2, 1):
  Case " ", "n", "W", "0", "t"                  'Valid second characters
  Case Else: GoTo TryAgainGV
  End Select
  
ProcessNextAlineGV:
  'If all scan lines have not been read,
  If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then
  ' Loop through here until all the necessary lines have been read for a scan
    GoSub ProcessInputLineGV
    If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then GoTo TryAgainGV
  End If
Return

ProcessInputLineGV:
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" Then Return
    'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
    'Un-comment a$ line above to simulate bad DADS data
    NavQual = True
    pALT = ZpAvg
    ALTlast = pALT
    UTsecMTPlast = UTsecMTP          'Save in case UT time correction needed

    Call DecodeGVline(A$)            'Get UTsec from RAW file

    dALT = pALT - ALTlast            'Look for Takeoff
    If pALT > 0.006 And dALT > 0.015 And Not TakeOff And Cycle > 0 Then
      CycleTakeoff = Cycle
      TakeOff = True
'      Landing = False
      UTtakeoff = UTsecLast
      pALTtakeoff = pALT
      hhmmssTO$ = hhmmssNavLast$
    End If
      
'    If TakeOff And Not Landing Then
'      If pALT < 2 And Cycle > 1000 And dALT < -0.015 Then
'        Landing = True
'        UTlanding = UTsec
'        If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
'      End If
'    End If

    If UTsecLast > UTsec Then UTsec = UTsec + 86400
' Apply modelled correction to Znav if necessary
    If Pcorr <> 99 Then
      Select Case AC$
      Case "ER"
        Select Case Mission$
        Case "CAMEX4"
          pALT = fPtoZ(fZtoP(pALT) + Pcorr) + (17 * (UTsec - UTtakeoff) / 3600 + 135 * (pALT / 20) ^ 2 + 130 * (pALT / 20) ^ 10) / 1000#
        Case "TC4"
        '  There is both a constant P correction and an altitude dependent term
          If RawCycle < TotalCycles / 2 Then
            pALT = fPtoZ(fZtoP(pALT) - Pcorr + 12 * (20 - gALT) / 20)    'Ascent correction
          Else
            pALT = fPtoZ(fZtoP(pALT) - Pcorr - 24 * (20 - gALT) / 20)    'Descent correction
          End If
        Case Else
          pALT = fPtoZ(fZtoP(pALT) - Pcorr)  'Correct Nav pressure to agree with MMS which is missing
        End Select
      Case "WB"
      Case "DC"
      End Select
    End If
    ALTkmft = pALT * cft_km
    pALTft = pALT * cft_km

      
'   At this point OATn and pALT are NAV values
    OAT = cTo + OATn + OATnavCOR
    If OAT > 999.9 Then OAT = 999.9
    Tac = Int(10 * OAT) / 10#          'Temperature at aircraft
    ALTkmLast = pALT
    UTsecLast = UTsec
    
    LineCurr(1) = A$
    Aline% = True
    Bline% = 0: M01line% = 0: M02line% = 0: Ptline = 0: Eline% = 0: IWGline% = 0

  Case Else  'Lines other than A
    Select Case Left$(A$, 1)
    Case "B": LineCurr$(2) = A$
    Case "E": LineCurr$(6) = A$
    End Select
    
    Select Case Left$(A$, 3)
    Case "M01": LineCurr$(3) = A$
    Case "M02": LineCurr$(4) = A$
    Case "Pt:": LineCurr$(5) = A$

    End Select
    Call DecodeGVline(A$)
  
  End Select

  ByteCount = Loc(RAWlu%) * 128
'  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  UTsecLast = UTsec

Return


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Process non-GV data
ReadABCDElines:
  If Cycle > 0 Then GoTo ProcessNextAline
TryAgain:
  If EOF(RAWlu) Then
'    If chkZgZp.Value = 1 Then Close lu
    If Not RealTime Then Quit = True
    Exit Sub
  End If
' Get an input line from RAW File
  Input #RAWlu%, A$
  'Debug.Print A$
  
'  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
'  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  DoEvents
  
  If Len(A$) < 5 Then GoTo TryAgain
    
  Select Case Mid$(A$, 2, 1):
  Case "E", " ", "G", "N", "9", "D", "X", "W", "!"  'Valid second characters
  Case Else: GoTo TryAgain
  End Select
  
ProcessNextAline:
  If Aline% * Bline% * Cline% * Dline% * Eline% = 0 Then
    GoSub ProcessInputLine
    If Aline% * Bline% * Cline% * Dline% * Eline% = 0 Then GoTo TryAgain
  End If
  Return


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ProcessInputLine:
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" Then Return
    'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
    'Un-comment a$ line above to simulate bad DADS data
    If Len(A$) < 70 Then
      If Mid$(A$, 9, 4) = "2001" Then  'Deal with ROOF data: A-line=A 06-27-2001 01:15:16
        UTsec = fTstringToSec&(Mid$(A$, 14, 8), True)
        pALT = 0#: pALTft = 0#: OATn = 30#
      End If
    
    Else
      NavQual = True
      ALTlast = pALT
      UTsecMTPlast = UTsecMTP          'Save in case UT time correction needed

      Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file

      dALT = pALT - ALTlast   'Look for Takeoff
      'Debug.Print Cycle; UTsec; pALT; dALT
      If pALT > 0.006 And dALT > 0.015 And Not TakeOff And Cycle > 0 Then
        CycleTakeoff = Cycle
        TakeOff = True
'        Landing = False
        UTtakeoff = UTsecLast
        pALTtakeoff = pALT
        hhmmssTO$ = hhmmssNavLast$
      End If
      
    End If
  
    If UTsecLast > UTsec Then UTsec = UTsec + 86400
' Apply modelled correction to Znav if necessary
    If Pcorr <> 99 Then
      Select Case AC$
      Case "ER"
        Select Case Mission$
        Case "CAMEX4"
          pALT = fPtoZ(fZtoP(pALT) + Pcorr) + (17 * (UTsec - UTtakeoff) / 3600 + 135 * (pALT / 20) ^ 2 + 130 * (pALT / 20) ^ 10) / 1000#
        Case "TC4"
        '  There is both a constant P correction and an altitude dependent term
          If RawCycle < TotalCycles / 2 Then
            pALT = fPtoZ(fZtoP(pALT) - Pcorr + 12 * (20 - gALT) / 20)    'Ascent correction
          Else
            pALT = fPtoZ(fZtoP(pALT) - Pcorr - 24 * (20 - gALT) / 20)    'Descent correction
          End If
        Case Else
          pALT = fPtoZ(fZtoP(pALT) - Pcorr)  'Correct Nav pressure to agree with MMS which is missing
        End Select
      Case "WB"
      Case "DC"
      End Select
    End If
    ALTkmft = pALT * cft_km
    pALTft = pALT * cft_km
    
'   At this point OATn and pALT are NAV values; check if met file is to be used
               
    If OATn < -130# Or OATn > 100# Then OATn = 999.9
    ' Check that OATn is available, else assume on ground and use Twin
    ' Need this for DADStgt gain calibration 971020 mjm
    ' Set Twin to 15 C for first cycle since MUX data is decoded later.
    If OATn > 90 Then
      If Cycle% > 0 Then OATn = Twin Else OATn = 15 'xxx mjm 19991125
    End If
    OAT = cTo + OATn + OATnavCOR
    If OAT > 999.9 Then OAT = 999.9
    Tac = Int(10 * OAT) / 10#
    ALTkmLast = pALT
    UTsecLast = UTsec
    
    If yyyymmdd$ <> "20020716" And yyyymmdd$ <> "20030115" Then 'MTP Clocks off by 12 hours
      If (Abs(UTsecNav - UTsecMTP) < 1000 And ElCorUsed = 1) Then NavQual = True Else NavQual = False
    End If
    LineCurr(1) = A$
    Aline% = True
'    Debug.Print NavQual; UTsecMTP
'   UpdateCommLine
    Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0
'   If chkNextAline.value = 1 Then Fline% = 0 Else Fline% = -1

  Case Else
    'Check for FECerr messages in CRYSTAL-FACE test flight data
    c1$ = Left$(A$, 1)
    If c1$ = "B" Or c1$ = "C" Then
      If Len(A$) < 61 Then
        b$ = A$
        Input #RAWlu%, A$
        If Left$(A$, 6) = "FECerr" Then
          Input #RAWlu%, A$
          A$ = b$ + " " + A$
        Else
          If Not DoAll Then MsgBox fSecToTstring(UTsec, True) + "  Length of " + c1$ + "-line <61 and this is not an fEc error!", vbOKOnly
          
          GoTo ReadABCDElines
        End If
      End If
    End If
    Select Case c1$
    Case "B": LineCurr$(2) = A$
    Case "C": LineCurr$(3) = A$
    Case "d": LineCurr$(4) = A$
    Case "D": LineCurr$(5) = A$
    Case "E": LineCurr$(6) = A$
    End Select
    Call DecodeBCDElines(A$, frmFLTINFO.cboSU.Text, Lt$, GoToNext)
  
  End Select

  ByteCount = Loc(RAWlu%) * 128
'  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'  txtBadCounts.Text = Str(BadCycle)
'  txtFileRecords.Text = Str(Cycle)
  UTsecLast = UTsec

Return

End Sub
Function fTTO(ALTkm!)
' Calculate Time Tag Offset to location of MTP measurement
' Includes scan time, flight time
Dim TTO!
' Check for where Sensor Unit is to correct time tag offset for MTP measurement
' On Starboard (RHS) of A/C after 960325
  Select Case AC$
  Case "DC"
    If RHS Then                              'Time Tag offset (time to scan to
'      TTO = 15 + 0.28 * (pALTft / 1000 - 35) 'horizon (posn 6 on RHS and posn 5
      TTO = 15 + 0.28 * (ALTkm * kft_km - 35) 'horizon (posn 6 on RHS and posn 5
    Else                                     'on LHS+ time to reach measurement
'      TTO = 14 + 0.28 * (pALTft / 1000 - 35) 'location ahead of A/C) Equation
      TTO = 14 + 0.28 * (ALTkm * kft_km - 35) 'location ahead of A/C) Equation
    End If                                   'corrects for altitude dependence
  Case Else                                  'of absorption coefficient)
    TTO = 13                'ER2/WB57 - need to add flight time(8) +scan time(5)
  End Select
  
' Flight to MTP time assumes channel wts of 0.9, 1, 1 and sideband distances
' of 3.85 & 2.17 (Ch1), 1.33 & 1.06 (Ch2), and 0.57 & 0.60 (Ch3), and an A/C
' speed of 230 m/s. Based on regression against DADS where time variable OATs
' were present, MTP ahead by 0.8 cycle. 0.8*16.7 + 6 s = 19.6 +- 4 s when a
' scan.t.to.horizon time of 6 s was used. Our prediction is: 7.9+6.7 = 14.6 s
' In excellent agreement with 19.6 +- 4 from regression analysis.
  
'  UTsec = UTsecMTP + utMTPcorf       'Set MTP clock to NAV clock
'  UTsec = Int(UTsec + TTO + 0.5)               'Time Tag Offset
'  If UTsec > 86400 Then UTsec = UTsec - 86400
  
' Now, UTsec times in exchange file correspond to horizon data corrected for
' MTP clock drift, scan time to horizon, and flight time to MTP measurement

'  If UTsecLast = 0 Then UTsecLast = UTsec - CycleTime
'  If UTsec < 20 And UTsecLast > 86380 Then
'    UTsecLast = UTsecLast - 86400
'  End If

fTTO = TTO

End Function



Sub WriteArray(r%)
Dim i%, j%, x%

  CQV(r) = True    'RAW data has not been editted yet by CalculateArrayMA
  NQV(r) = NavQual
  
  Cycle = r
  UV(1, r) = UTsec
  UV(2, r) = UTsecMTP
  UV(3, r) = UTsecNav
  UV(4, r) = TTO
  
  ALTV(1, r) = pALT
  ALTV(2, r) = gALT
  ALTV(3, r) = rALT
  ALTV(4, r) = mALT
  
  NAVV(1, r) = Pitch
  NAVV(2, r) = Roll
  NAVV(3, r) = Latitude
  NAVV(4, r) = Longitude
  If r > 1 Then
    If Abs(NAVV(3, r - 1)) > 85 Then NAVV(4, r) = NAVV(4, r - 1): Longitude = NAVV(4, r)
    'If Abs(NAVV(4, R - 1) - NAVV(4, R)) > 1 Then NAVV(4, R) = NAVV(4, R - 1): Longitude = NAVV(4, R)
    'If Abs(NAVV(3, R - 1) - NAVV(3, R)) > 1 Then NAVV(3, R) = NAVV(3, R - 1): Latitude = NAVV(3, R)
  End If

  NAVV(5, r) = Heading
  NAVV(6, r) = TAS
  NAVV(7, r) = Wspd
  NAVV(8, r) = Wdir
  NAVV(9, r) = Elcor
  NAVV(10, r) = ElCorUsed
  
  OATV(1, r) = OATnav
  OATV(2, r) = OATmms
  OATV(3, r) = OATmtp
    
'  MV(32, r) = Zt1
'  MV(33, r) = LRac
'  MV(34, r) = LRac2
'  MV(35, r) = LRac3
  

End Sub

Sub ReadRAWfile()
Dim Mux%(0 To 16), TXTlu%, txtFile$, TTTlu%, TTTfile$
Dim i%, j%, A$, UTks!, x$, UTsecLast&
Dim DirPath$, ans, RAWfile$, ByteCount&, FirstAline As Boolean
Dim a10!(1 To 25), T10!(1 To 25), a10Last!, T10last!, MLsum!, TLsum!, WLsum!
Dim Temp1!(1 To 3500), Temp2!(1 To 3500), Temp3!(1 To 3500), UT&
Dim P!, T!, Th!, U!, V!, W!, PTfile$, Navg%, n2%, n3%, GoToNext As Boolean
Dim TtgtFix As Boolean, Zpp!(1 To 70), Zdif!(1 To 70), Zgh!, pALTcut!
Dim Lt$, PText$
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3), ByteCount1&
Dim f As Form

Static FP!(1 To 4)

  GoSub Initialize
  CheckLimitsInit
  TakeOff = False

' START
  Do
    ReadRAWrecord (Quit)  'Quit is TRUE if EOF and NOT Realtime
  Loop Until EOF(RAWlu) And Not RealTime
EndOfSub:
  Close RAWlu  'Close RAW file
  ArrayRead = True
  txtLAT1.Text = Format(LATmin, "##0.00")
  txtLAT2.Text = Format(LATmax, "##0.00")
  txtLON1.Text = Format(LONmin, "###0.00")
  txtLON2.Text = Format(LONmax, "###0.00")
  
 Exit Sub


SyncToExternalData:
' Open external PT data file if needed


Initialize:
  RawCycle = 0
  GoodCycle = 0
  Cycle = 0
  UTsec = 0
  UTsecLast = 0
' Handle aircraft dependent parameters
  Select Case AC$
  Case "ER", "WB", "M5"
    Ceiling = 24#             'ER2 Service ceiling
  Case "DC"
    Ceiling = 15              'DC8 service ceiling
    Remote = True             'Allow remote control commands
    If Val(yyyymmdd$) < 19960325 Then RHS = False: LocHor% = 5
  End Select
  
  DoEvents
'  DirPath$ = Drive$ + Rdir$ + Mission$ + "\"
  Set f = MAPget2
  
  If Not fDirExists(MNpath$, Trim(f.cboFltDates.Text)) Then
    ans = MsgBox("Path: " + MNpath$ + Trim(f.cboFltDates.Text) + "\" + " does not exist!" + vbLf + vbCrLf + "Do you wish to create it?", vbYesNo)
    If ans = vbYes Then
      MkDir MNpath$ + Trim(f.cboFltDates.Text)
    Else
      Exit Sub
    End If
  End If
  AC$ = Left$(Trim(cboPlatforms.Text), 2)
  RAWfile$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".RAW"
  TotalCycles = fRAWscans(RAWfile$)
  ReDim MV!(0 To MVchannels, 1 To TotalCycles), CV%(1 To Channels, 1 To 13, 1 To TotalCycles)
  ReDim EV%(1 To Channels, 1 To 13, 1 To TotalCycles)
  ReDim GV!(0 To GVchannels, 1 To Channels, 1 To TotalCycles)
  ReDim UV&(1 To 4, 1 To TotalCycles), NAVV!(1 To 10, 1 To TotalCycles), OATV!(1 To 10, 1 To TotalCycles)
  ReDim ALTV!(1 To 4, 1 To TotalCycles), MUXV%(1 To 16, 1 To TotalCycles)
  ReDim CQV(1 To TotalCycles) As Boolean, CQM%(1 To Channels, 1 To TotalCycles)
  ReDim NQV(1 To TotalCycles) As Boolean
  
  RAWlu = FreeFile
  Open RAWfile$ For Input As RAWlu
  LOFraw = LOF(RAWlu%)
  Record = 0
  FirstAline = True
  PText$ = fACext(AC$)

  Call CheckLimitsInit   'Reset Limit Information
  InitializeGainLimits
  Return
  
End Sub
Sub ReadRAWfileNGV()
Dim Mux%(0 To 16), TXTlu%, txtFile$, TTTlu%, TTTfile$
Dim i%, j%, A$, UTks!, x$, UTsecLast&
Dim DirPath$, ans, RAWfile$, ByteCount&, FirstAline As Boolean
Dim a10!(1 To 25), T10!(1 To 25), a10Last!, T10last!, MLsum!, TLsum!, WLsum!
Dim Temp1!(1 To 3500), Temp2!(1 To 3500), Temp3!(1 To 3500), UT&
Dim P!, T!, Th!, U!, V!, W!, PTfile$, Navg%, n2%, n3%, GoToNext As Boolean
Dim TtgtFix As Boolean, Zpp!(1 To 70), Zdif!(1 To 70), Zgh!, pALTcut!
Dim Lt$, PText$
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3), ByteCount1&
Dim f As Form

Static FP!(1 To 4)

  GoSub Initialize
  CheckLimitsInit
  TakeOff = False

' START
  Input #RAWlu, A$  'skip header line
  Do
'    ReadRAWrecord (Quit)  'Quit is TRUE if EOF and NOT Realtime
    Call GetNGline(RAWlu, UTsec&, P, OATnav, OATmms, Latitude, Longitude, gALT, pALT, Pitch, Roll)
    RawCycle% = RawCycle% + 1                     'Increment total raw cycle count
    Cycle = RawCycle
'  Aline = False
   WriteArray (RawCycle) 'Call BINwrite(BINlu, RawCycle)
   CheckLimits (RawCycle) 'Lat Lon check after fix in WriteArray
  Loop Until EOF(RAWlu) And Not RealTime
EndOfSub:
  Close RAWlu  'Close RAW file
  ArrayRead = True
  txtLAT1.Text = Format(LATmin, "##0.00")
  txtLAT2.Text = Format(LATmax, "##0.00")
  txtLON1.Text = Format(LONmin, "###0.00")
  txtLON2.Text = Format(LONmax, "###0.00")
  
 Exit Sub


SyncToExternalData:
' Open external PT data file if needed


Initialize:
  RawCycle = 0
  GoodCycle = 0
  Cycle = 0
  UTsec = 0
  UTsecLast = 0
  Ceiling = 15              'NGV service ceiling
  Remote = True             'Allow remote control commands
  DoEvents
'  DirPath$ = Drive$ + Rdir$ + Mission$ + "\"
  Set f = MAPget2
  
'  If Not fDirExists(MNpath$, Trim(f.cboFltDates.Text)) Then
'    ans = MsgBox("Path: " + MNpath$ + Trim(f.cboFltDates.Text) + "\" + " does not exist!" + vbLf + vbCrLf + "Do you wish to create it?", vbYesNo)
'    If ans = vbYes Then
'      MkDir MNpath$ + Trim(f.cboFltDates.Text)
'    Else
'      Exit Sub
'    End If
'  End If
  AC$ = Left$(Trim(cboPlatforms.Text), 2)
  RAWfile$ = MNpath$ + "NG\" + AC$ + Trim(f.cboFltDates.Text) + ".asc"
  TotalCycles = 4000     'fRAWscans(RAWfile$)
  ReDim MV!(0 To MVchannels, 1 To TotalCycles), CV%(1 To Channels, 1 To 13, 1 To TotalCycles)
  ReDim EV%(1 To Channels, 1 To 13, 1 To TotalCycles)
  ReDim GV!(0 To GVchannels, 1 To Channels, 1 To TotalCycles)
  ReDim UV&(1 To 4, 1 To TotalCycles), NAVV!(1 To 10, 1 To TotalCycles), OATV!(1 To 10, 1 To TotalCycles)
  ReDim ALTV!(1 To 4, 1 To TotalCycles), MUXV%(1 To 16, 1 To TotalCycles)
  ReDim CQV(1 To TotalCycles) As Boolean, CQM%(1 To Channels, 1 To TotalCycles)
  ReDim NQV(1 To TotalCycles) As Boolean
  
  RAWlu = FreeFile
  Open RAWfile$ For Input As RAWlu
  LOFraw = LOF(RAWlu%)
  Record = 0
  FirstAline = True
  PText$ = fACext(AC$)

  Call CheckLimitsInit   'Reset Limit Information
  InitializeGainLimits
  Return
  
End Sub

Function fDirExists(DirPath$, SearchName$) As Boolean
Dim DirName$

DirName = Dir(DirPath, vbDirectory) ' Retrieve the first entry.
Do While DirName <> "" ' Start the loop.
  ' Ignore the current directory and the encompassing directory.
  If DirName = SearchName$ Then
    fDirExists = True
    Exit Function               ' SearchName$ already exists
  End If
  DirName = Dir                 ' Get next entry.
Loop
fDirExists = False              ' Failed to find SearchName$
End Function

Sub UpdateFlightTrack1()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, Xoffset!, Yoffset!, DotRadius!
Dim LAmin!, LAmax!, LOmin!, LOmax!, r%, iStep%, iMax%, Lat!, Lon!, iUTks%
Dim INPlu%, Record&, L!, xScale!, yScale!, x!

If chkFltTrack.Value = 0 Then GoTo RAOBs
'  Latitude = NAVV(3, r)
'  Longitude = NAVV(4, r)
frmPic1.Cls
frmPic1.Caption = "MTP " + ACX$ + " " + Mission$ + " Flight Track for " + Trim(cboFltDates.Text)

'Plot Flight Track in picture window
'  With frmPic1
'  .picTrack1.Cls
'  .picTrack1.ScaleMode = 0
'  .picTrack1.ScaleTop = 0
'  .picTrack1.ScaleHeight = 15
'  End With

' Set temperature display scale
  If chkAutoscaleTrack Then
    LOmin = Int(LONmin - 1)
    LOmax = Int(LONmax + 1)
    frmPic1.picTrack1.ScaleWidth = LOmax - LOmin
    frmPic1.picTrack1.ScaleLeft = LOmin
    txtLON1.Text = Format(LOmin, "##0")
    txtLON2.Text = Format(LOmax, "##0")

    LAmin = Int(LATmin - 1)
    LAmax = Int(LATmax + 1)
    frmPic1.picTrack1.ScaleHeight = LAmin - LAmax
    frmPic1.picTrack1.ScaleTop = LAmax
    txtLAT1.Text = Format(LAmin, "##0")
    txtLAT2.Text = Format(LAmax, "##0")
  Else
    LOmin = Val(txtLON1.Text)
    frmPic1.picTrack1.ScaleLeft = LOmin
    frmPic1.picTrack1.ScaleWidth = Val(txtLON2.Text) - Val(txtLON1.Text)
    LOmax = frmPic1.picTrack1.ScaleLeft + frmPic1.picTrack1.ScaleWidth
    
    LAmax = Val(txtLAT2.Text)
    frmPic1.picTrack1.ScaleTop = LAmax
    frmPic1.picTrack1.ScaleHeight = Val(txtLAT1.Text) - Val(txtLAT2.Text)
    LAmin = frmPic1.picTrack1.ScaleTop + frmPic1.picTrack1.ScaleHeight
  
  End If
  
' Draw Longitude-tic marks
  For L = LAmin To LAmax Step Val(txtLAT3.Text)
    frmPic1.picTrack1.PSet (LOmin, L)
    frmPic1.picTrack1.Line -(LOmax, L), QBColor(5)
  Next L
  iMax = (LOmax - LOmin) / Val(txtLON3.Text)
  xScale = frmPic1.picTrack1.Width
  Xstep = xScale / iMax
  frmPic1.FontSize = 10
  frmPic1.Font.Bold = True
  Yoffset! = 0.5 * frmPic1.TextHeight("2")
  Xoffset = 1.5 * frmPic1.TextWidth("2")
  For i = 0 To iMax
    frmPic1.CurrentY = frmPic1.picTrack1.Top + frmPic1.picTrack1.Height + Yoffset
    frmPic1.CurrentX = frmPic1.picTrack1.Left + Xstep * i - Xoffset
    Lon = LOmin + Val(txtLON3.Text) * i
    If Lon < 10 Then
      frmPic1.Print Format$(Lon, "###")
    Else
      frmPic1.Print Format$(Lon, "###")
    End If
  Next i

'00 gray
'01
'02
'03
'04
'05 purple
'06 lgreen
'07 l gray
'08 gray
'09 blue
'10 green
'11 cyan
'12 red
'13 magenta
'14 yellow
'15 white
' Draw Longitude-tic marks
  For L = LOmin To LOmax Step Val(txtLON3.Text)
    frmPic1.picTrack1.PSet (L, LAmin)
    frmPic1.picTrack1.Line -(L, LAmax), QBColor(5)
  Next L

  iMax = (LAmax - LAmin) / Val(txtLAT3.Text)
  xScale = -frmPic1.picTrack1.Height
  Xstep = xScale / iMax
  frmPic1.FontSize = 10
  frmPic1.Font.Bold = True
  Yoffset! = frmPic1.TextHeight("2") / 2
  Xoffset = frmPic1.picTrack1.Left - 25
  For i = 0 To iMax
    frmPic1.CurrentX = Xoffset
    frmPic1.CurrentY = frmPic1.picTrack1.Top + frmPic1.picTrack1.Height - Yoffset + Xstep * i
    Lat = LAmin + Val(txtLAT3.Text) * i
    If Int(Val(txtLAT3.Text)) = Val(txtLAT3.Text) Then
      If Lat < 10 Then
        frmPic1.Print Format$(Lat, "0#")
      Else
        frmPic1.Print Format$(Lat, "##")
      End If
    Else
      frmPic1.CurrentX = frmPic1.picTrack1.Left - 30
      If Lat < 10 Then
        frmPic1.Print Format$(Lat, "0#.0")
      Else
        frmPic1.Print Format$(Lat, "##.0")
      End If
    End If
  Next i


' Draw flight track
  frmPic1.picTrack1.DrawWidth = Val(Trim(cboDrawWidth.Text))
  frmPic1.picTrack1.ForeColor = txtLineColor.BackColor
  frmPic1.picTrack1.PSet (NAVV(4, 1), NAVV(3, 1))
  If chkHiliteSelection.Value = 1 Then
    For r = 2 To RawCycle
      If r < FirstMouseX Or r > LastMouseX Then LineC = frmPic1.picTrack1.ForeColor Else LineC = QBColor(12)
      frmPic1.picTrack1.Line -(NAVV(4, r), NAVV(3, r)), LineC  'And continue from there
      If NAVV(3, r) < 24 Then
      x = x
      End If
    Next r
  Else
    For r = 2 To TotalCycles
      frmPic1.picTrack1.Line -(NAVV(4, r), NAVV(3, r)), frmPic1.picTrack1.ForeColor  'And continue from there
    Next r
  End If

' Draw UT ks markers
  iUTks = Int(UV(1, 1) / 1000)
  iUTks = iUTks + 1
  frmPic1.picTrack1.ForeColor = QBColor(9)
  frmPic1.picTrack1.FillColor = QBColor(9)
  frmPic1.picTrack1.FillStyle = 0
  DotRadius = (Val(txtLAT2.Text) - Val(txtLAT1.Text)) / 200#
  For r = 2 To TotalCycles
    If UV(1, r) / 1000 >= iUTks Then
      frmPic1.picTrack1.Circle (NAVV(4, r), NAVV(3, r)), DotRadius
      frmPic1.picTrack1.CurrentX = NAVV(4, r) + 0.04
      frmPic1.picTrack1.CurrentY = NAVV(3, r)
      frmPic1.picTrack1.Print Format(iUTks, "00")
      iUTks = iUTks + 1
    End If
  Next r
  
RAOBs:
' Draw RAOB Sites
  If optRAOB(2).Value = False Then

  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.bin" For Random Access Read Write As INPlu Len = Len(GP)
  
  Record = 0
  frmPic1.picTrack1.ForeColor = QBColor(12)
  Do
    Record = Record + 1
    GPread INPlu, Record
   
    If GPlatitude > Val(txtLAT1.Text) And GPlatitude < Val(txtLAT2.Text) Then
      If Val(txtLON1.Text) < -180 Then  'Fix for IDT
        If GPlongitude > 0 Then GPlongitude = GPlongitude - 360
      End If
        If GPlongitude > Val(txtLON1.Text) And GPlongitude < Val(txtLON2.Text) Then
          frmPic1.picTrack1.CurrentX = GPlongitude
          frmPic1.picTrack1.CurrentY = GPlatitude
          frmPic1.picTrack1.Circle (GPlongitude, GPlatitude), DotRadius
          frmPic1.picTrack1.FillColor = QBColor(12)
          frmPic1.picTrack1.CurrentX = GPlongitude + 2 * DotRadius
          If GPicao <> "        " And optRAOB(0).Value = True Then
            frmPic1.picTrack1.Print ICAOsite 'Labels
          Else
            frmPic1.picTrack1.Print WMOnumber 'Labels
          End If
        End If
    End If
  Loop Until EOF(INPlu)
  Close INPlu
  End If

  ' Draw Cities
  If chkShowCities.Value = True Then

  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\CityFile.bin" For Random Access Read Write As INPlu Len = Len(CITY)
  
  Record = 0
  frmPic2.picTrack2.ForeColor = QBColor(12)
  Do
    Record = Record + 1
    CITYread INPlu, Record
    If CityPopulation > Val(Trim(cboPopulation.Text)) Then
      If CityLatitude > Val(txtLAT1.Text) And CityLatitude < Val(txtLAT2.Text) Then
        If Val(txtLON1.Text) < -180 Then  'Fix for IDT
          If CityLongitude > 0 Then CityLongitude = CityLongitude - 360
        End If
        If CityLongitude > Val(txtLON1.Text) And CityLongitude < Val(txtLON2.Text) Then
          frmPic2.picTrack2.CurrentX = CityLongitude
          frmPic2.picTrack2.CurrentY = CityLatitude
          frmPic2.picTrack2.Circle (CityLongitude, CityLatitude), DotRadius
          frmPic2.picTrack2.FillColor = QBColor(12)
          frmPic2.picTrack2.CurrentX = CityLongitude + 2 * DotRadius
          frmPic2.picTrack2.Print CityName
        End If
      End If
    End If
  Loop Until EOF(INPlu)
  Close INPlu
  End If
  'frmPic1.lblPic2.Caption = "mjm " + Date$
End Sub

Private Sub cboFltDates_Click()
Dim LI%

LI = cboFltDates.ListIndex
cboFltNos.Text = cboFltNos.List(LI)
cboObjectives.Text = cboObjectives.List(LI)
UpdateSourceFile

End Sub


Private Sub cboFltNos_Click()
Dim LI%

LI = cboFltNos.ListIndex
cboFltDates.Text = cboFltDates.List(LI)
cboObjectives.Text = cboObjectives.List(LI)
End Sub


Private Sub cboMissions_Click()
UpdateFltDates MAPget2
Mission$ = Trim(cboMissions.Text)
UpdateSourceFile
End Sub


Private Sub cboObjectives_Click()
Dim LI%
LI = cboObjectives.ListIndex
cboFltNos.Text = cboFltNos.List(LI)
cboFltDates.Text = cboFltDates.List(LI)
UpdateSourceFile
End Sub

Private Sub cboPlatforms_Click()
  LoadComboBox MAPget2.cboMissions, Trim(cboPlatforms.Text)
  Platform$ = Trim(cboPlatforms.Text)
  Rdir$ = "\" + Platform$ + "\"
  UpdateSourceFile

End Sub



Sub cboTrack_Change()

End Sub



Private Sub chkAutoscaleTrack_Click()
UpdateFlightTrack2
End Sub

Private Sub chkHiliteSelection_Click()
UpdateFlightTrack2
End Sub


Private Sub chkPreserveScale_Click()

End Sub

Private Sub cmdDefault_Click()
Dim Ht!, Wd!, Lat!, Lon!, rLat!, rLon!, iHt%, iWd%

  Ht = Val(txtLAT2.Text - txtLAT1.Text)
  Wd = Val(txtLON2.Text - txtLON1.Text)
  Lat = Val(txtLAT1.Text) + Ht / 2
  Lon = Val(txtLON1.Text) + Wd / 2
  rLon = Wd '* Cos(Val(txtLAT1.Text) * RpD)
  rLat = Ht
  If rLat > rLon Then
    iHt = 600
    iWd = Int(600 * (rLon / rLat))
    If iWd > 800 Then iHt = Int(600 * (800 / iWd)): iWd = 800
  Else
    iWd = 800
    iHt = Int(800 * (rLat / rLon))
    If iHt > 600 Then iWd = Int(800 * (600 / iHt)): iHt = 600
  End If
'  txtWd2.Text = Str(iWd): txtHt2.Text = Str(iHt)
  
End Sub


Private Sub cmdExit_Click()
  Unload frmPic2
  Unload Me
  End
End Sub

Private Sub cmdLoadImage_Click()
Dim lu%, Filename$, North As Boolean, East As Boolean, LAT1!, LAT2!, LON1!, LON2!
Dim AR!
' Set Initial directory
CommonDialog1.InitDir = "C:\MTP\MAPS\"
' Set filters.
CommonDialog1.Filter = "JPEG Files (*.JPG)|*.JPG|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

Filename$ = CommonDialog1.Filename

If Filename$ = "" Then Exit Sub
' On Error GoTo errhandler
If Mid$(Filename$, 17, 1) = "N" Then North = True Else North = False
LAT1 = Val(Mid$(Filename$, 18, 3)) / 10#
If Not North Then LAT1 = -LAT1
txtLAT1.Text = Format(LAT1, "##0.0")

If Mid$(Filename$, 21, 1) = "N" Then North = True Else North = False
LAT2 = Val(Mid$(Filename$, 22, 3)) / 10#
If Not North Then LAT2 = -LAT2
txtLAT2.Text = Format(LAT2, "##0.0")

Select Case Abs(LAT2 - LAT1)
Case Is < 2: txtLAT3.Text = 0.5
Case Is < 10: txtLAT3.Text = 1
Case Is < 20: txtLAT3.Text = 2
Case Else: txtLAT3.Text = 5
End Select

If Mid$(Filename$, 26, 1) = "E" Then East = True Else East = False
LON1 = Val(Mid$(Filename$, 27, 4)) / 10#
If Not East Then LON1 = -LON1
txtLON1.Text = Format(LON1, "##0.0")

If Mid$(Filename$, 31, 1) = "E" Then East = True Else East = False
LON2 = Val(Mid$(Filename$, 32, 4)) / 10#
If Not East Then LON2 = -LON2
txtLON2.Text = Format(LON2, "##0.0")

Select Case LON2 - LON1
Case Is < 2: txtLON3.Text = 0.5
Case Is < 10: txtLON3.Text = 1
Case Is < 20: txtLON3.Text = 2
Case Else: txtLON3.Text = 5
End Select
With frmPic1
.Hide
.picTrack1.Picture = LoadPicture(Filename$)
'.Show
End With
    MAPget2.txtInWidth = frmPic1.picTrack1.ScaleWidth
    MAPget2.txtInHeight = frmPic1.picTrack1.ScaleHeight
AR = frmPic1.picTrack1.ScaleWidth / frmPic1.picTrack1.ScaleHeight
If chkPreserveMapScale.Value = 1 Then
  If AR >= 1# Then     'Width is limiting axis
    txtOutWidth.Text = Format(700, "0000")
    txtOutHeight.Text = Format(700 / AR, "0000")
  Else
    txtOutHeight.Text = Format(600, "0000")
    txtOutWidth.Text = Format(600 / AR, "0000")
  End If
Else

End If
cmdResize_Click
End Sub

Private Sub cmdRead_Click()
Dim INPlu%, OUTlu%, A$, b$(1 To 20), n%, n1%, n2%, i%, BlankLine As Boolean
Dim Record&

INPlu = FreeFile
Open "C:\TEMP\Population\US.txt" For Input As INPlu
OUTlu = FreeFile
Open "C:\MTP\RAOB\BIN\CityFile.bin" For Random Access Read Write As OUTlu Len = Len(CITY)

Do
  Input #INPlu, A$
Loop Until Left$(A$, 4) = "Date"

Do
  Input #INPlu, A$
  'Debug.Print a$
  If EOF(INPlu) Then Exit Do
  If Len(A$) > 0 And BlankLine Then
    n = 0
    Call ParseString(A$, b$(), n%)
    If n >= 4 Then
      n1 = n
      Input #INPlu, A$
      Call ParseString(A$, b$(), n%)
      n2 = 6
    If Len(b$(2)) > 1 Then
      If Left$(b$(2), 1) <> "(" Then b$(1) = b$(1) + " " + b$(2)
      n2 = 7
    End If
    CityName = b$(1)
    CityPopulation = Val(b$(n2))
    CityLatitude = Val(b$(n - 1))
    CityLongitude = Val(b$(n))
    Record = Record + 1
    Call CITYwrite(OUTlu, Record)
    
    Debug.Print b$(1); "   "; b$(n2); "   "; b$(n - 1); "   "; b$(n); "   ";
    Debug.Print n
    BlankLine = False
    End If
  Else
    BlankLine = True
  End If

Loop Until EOF(INPlu)
Close INPlu, OUTlu

End Sub

Sub ParseString(A$, b$(), n%)
Dim i%, j%, k%, Length%
Length = Len(A$)

j = n
k = 1
For i = 1 To Length
  If Mid$(A$, i, 1) = " " Then
    j% = j% + 1
    b$(j) = Mid$(A$, k, i - k)
    k = i + 1
  End If
Next i
j = j + 1
b(j) = Mid$(A$, k, i - k)
n = j

End Sub

Private Sub cmdReadRAWfile_Click()

'If Platform$ = "NGV" Then
'  ReadRAWfileNGV    'for Dick Friesen TREX asc files
'Else
  ReadRAWfile
'End If

End Sub


Private Sub cmdRefresh_Click()
frmPic1.Show
frmPic1.Cls
frmPic1.picTrack1.Cls
UpdateFlightTrack1
End Sub

Private Sub cmdRefresh2_Click()
UpdateFlightTrack2
End Sub

Private Sub cmdResize_Click()
Dim fr_wid As Single
Dim fr_hgt As Single
Dim to_wid As Single
Dim to_hgt As Single
Dim border_thickness As Single
Dim wid As Single
Dim hgt As Single
Dim x!, y!

    border_thickness = 0  'frmPic1.picTrack1.Width - frmPic1.picTrack1.ScaleWidth
    x = Val(txtOutWidth.Text) / Val(txtInWidth.Text)
    y = Val(txtOutHeight.Text) / Val(txtInHeight.Text)
    frmPic2.picTrack2.Width = x * frmPic1.picTrack1.ScaleWidth + border_thickness
    frmPic2.picTrack2.Height = y * frmPic1.picTrack1.ScaleHeight + border_thickness

    'Width = wid + Width - ScaleX(ScaleWidth, vbPixels, vbTwips)
    'Height = hgt + Height - ScaleY(ScaleHeight, vbPixels, vbTwips)
    fr_wid = frmPic1.picTrack1.ScaleWidth
    fr_hgt = frmPic1.picTrack1.ScaleHeight
    to_wid = frmPic2.picTrack2.Width '* Screen.TwipsPerPixelX 'Val(txtOutWidth)
    to_hgt = frmPic2.picTrack2.Height '* Screen.TwipsPerPixelY 'Val(txtOutHeight)
    frmPic2.ScaleMode = vbPixels
    frmPic2.AutoRedraw = True
    frmPic2.Show
    frmPic2.picTrack2.AutoSize = True
    'frmPic2.picTrack2.ScaleMode = vbPixels
    frmPic2.picTrack2.Width = to_wid
    frmPic2.picTrack2.Height = to_hgt
    wid = ScaleX(2 * frmPic2.picTrack2.Left + frmPic2.picTrack2.Width, vbPixels, vbTwips)
    hgt = ScaleY(10 * frmPic2.picTrack2.Top + frmPic2.picTrack2.Height, vbPixels, vbTwips)
    frmPic2.Width = wid
    frmPic2.Height = hgt
    'frmPic2.picTrack2.Cls
    StretchBlt frmPic2.picTrack2.hdc, 0, 0, to_wid, to_hgt, frmPic1.picTrack1.hdc, 0, 0, fr_wid, fr_hgt, SRCCOPY
    DoEvents

UpdateFlightTrack2
End Sub

Private Sub Command1_Click()
UpdateFlightTrack2
End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
Dim i%, SAVEyyyymmdd$

  Set CallingForm = MAPget2
  MAPget2.Top = 0
  MAPget2.Left = 0
' Get default drives
  ReadSETUP ("SYSTEM")

  FLTINFOfile$ = "C:\MTP\Setup\FLTINFO.REF"

  REFreadHEADER FLTINFOfile$
'  REFreadLIMITS FLTINFOfile$

  SAVEyyyymmdd$ = yyyymmdd$
'  UpdateForm
  LoadComboBox MAPget2.cboPlatforms, "PLATFORM"
  UpdateForm
  
  For i = 0 To cboMissions.ListCount
    If Trim(cboMissions.List(i)) = Mission$ Then cboMissions.ListIndex = i: Exit For
  Next i

  For i = 0 To cboFltDates.ListCount
    If Trim(cboFltDates.List(i)) = SAVEyyyymmdd$ Then Exit For
  Next i
  
  cboFltDates.Text = cboFltDates.List(i)
  cboFltNos.Text = cboFltNos.List(i)
  cboObjectives.Text = cboObjectives.List(i)
  UpdateSourceFile

'cboSize.AddItem "640 x 480"   '0
'cboSize.AddItem "800 x 600"   '1
'cboSize.AddItem "1024 x 768"  '2
'cboSize.AddItem "1280 x 960"  '3
'cboSize.AddItem "1600 x 1200"  '4
'cboSize.ListIndex = 1
  
  With cboTrack
    .AddItem "Rec No"
    .AddItem "ks"
    .AddItem "seconds"
    .AddItem "hhmmss"
    .AddItem "hh:mm:ss"
    .ListIndex = 4
  End With
  
  With cboDrawWidth
    .AddItem "1"
    .AddItem "2"
    .AddItem "3"
    .ListIndex = 1
  End With
  
  With cboPopulation
    .AddItem "100000"
    .AddItem "200000"
    .AddItem "500000"
    .AddItem "1000000"
    .AddItem "2000000"
    .ListIndex = 2
  End With

  With cboPalt
    .AddItem "km"
    .AddItem "kft"
    .ListIndex = 0
  End With
'  frmPic2.picTrack1.LineDrawWidth = Val(Trim(cboDrawWidth.Text))

End Sub



Private Sub UpDown1_DownClick()

Select Case Val(txtLineColor.Text)
Case 1: txtLineColor.Text = 15
Case Is > 1: txtLineColor.Text = txtLineColor.Text - 1
End Select
txtLineColor.BackColor = QBColor(Val(txtLineColor.Text))
frmPic2.picTrack2.ForeColor = QBColor(Val(txtLineColor.Text))

End Sub


Private Sub UpDown1_UpClick()
Select Case Val(txtLineColor.Text)
Case Is < 15: txtLineColor.Text = txtLineColor.Text + 1
Case Else: txtLineColor.Text = 0
End Select
txtLineColor.BackColor = QBColor(Val(txtLineColor.Text))
frmPic2.picTrack2.ForeColor = QBColor(Val(txtLineColor.Text))

End Sub


Private Sub UpDown2_DownClick()
Select Case Val(txtUTksColor.Text)
Case 1: txtUTksColor.Text = 15
Case Is > 1: txtUTksColor.Text = txtUTksColor.Text - 1
End Select
txtUTksColor.BackColor = QBColor(Val(txtUTksColor.Text))

End Sub


Private Sub UpDown2_UpClick()
Select Case Val(txtUTksColor.Text)
Case Is < 15: txtUTksColor.Text = txtUTksColor.Text + 1
Case Else: txtUTksColor.Text = 0
End Select
txtUTksColor.BackColor = QBColor(Val(txtUTksColor.Text))

End Sub


