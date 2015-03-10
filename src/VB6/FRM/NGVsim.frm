VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "mswinsck.ocx"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmNGVsim 
   Caption         =   "NGV Data Display"
   ClientHeight    =   5550
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   12285
   ForeColor       =   &H80000008&
   LinkTopic       =   "Form1"
   ScaleHeight     =   5550
   ScaleWidth      =   12285
   StartUpPosition =   3  'Windows Default
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   9510
      Top             =   3000
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin VB.Frame Frame 
      Caption         =   "Write Mode"
      Height          =   810
      Index           =   0
      Left            =   7035
      TabIndex        =   121
      Top             =   1560
      Width           =   3060
      Begin VB.TextBox txtLocalPort 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2445
         MultiLine       =   -1  'True
         TabIndex        =   126
         Text            =   "NGVsim.frx":0000
         Top             =   165
         Width           =   555
      End
      Begin VB.OptionButton optWriteMode 
         Caption         =   "Continuously to UDP Port"
         Height          =   255
         Index           =   0
         Left            =   90
         TabIndex        =   123
         Top             =   240
         Width           =   2205
      End
      Begin VB.OptionButton optWriteMode 
         Caption         =   "Continuously to Serial Port"
         Height          =   210
         Index           =   1
         Left            =   90
         TabIndex        =   122
         Top             =   495
         Width           =   2265
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "Timer Speed (ms)"
      Height          =   615
      Index           =   8
      Left            =   6990
      TabIndex        =   118
      Top             =   4920
      Width           =   2745
      Begin VB.TextBox txtTimerSpeed 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   119
         Top             =   240
         Width           =   555
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   255
         Left            =   720
         TabIndex        =   120
         Top             =   240
         Width           =   1980
         _ExtentX        =   3493
         _ExtentY        =   450
         _Version        =   393216
         LargeChange     =   500
         Max             =   10000
         SelStart        =   500
         Value           =   500
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "COM Port Setup"
      Height          =   2100
      Index           =   2
      Left            =   10155
      TabIndex        =   103
      Top             =   0
      Width           =   2130
      Begin VB.ComboBox cboCOMport 
         Height          =   315
         Left            =   1065
         TabIndex        =   109
         Text            =   "1"
         Top             =   240
         Width           =   1035
      End
      Begin VB.ComboBox cboBaud 
         Height          =   315
         Left            =   1065
         TabIndex        =   108
         Text            =   "9600"
         Top             =   540
         Width           =   1035
      End
      Begin VB.ComboBox cboParity 
         Height          =   315
         Left            =   1065
         TabIndex        =   107
         Text            =   "N"
         Top             =   840
         Width           =   1035
      End
      Begin VB.ComboBox cboBits 
         Height          =   315
         Left            =   1065
         TabIndex        =   106
         Text            =   "8"
         Top             =   1140
         Width           =   1035
      End
      Begin VB.TextBox txtMScomm 
         Height          =   285
         Left            =   1065
         TabIndex        =   105
         Text            =   "1,9600,N,8"
         Top             =   1755
         Width           =   1035
      End
      Begin VB.ComboBox cboStop 
         Height          =   315
         Left            =   1065
         TabIndex        =   104
         Text            =   "1"
         Top             =   1440
         Width           =   1035
      End
      Begin VB.Label Label 
         Caption         =   "Port Number"
         Height          =   255
         Index           =   42
         Left            =   120
         TabIndex        =   115
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Baud Rate"
         Height          =   255
         Index           =   43
         Left            =   120
         TabIndex        =   114
         Top             =   555
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Parity"
         Height          =   255
         Index           =   44
         Left            =   120
         TabIndex        =   113
         Top             =   885
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Data Bits"
         Height          =   225
         Index           =   45
         Left            =   120
         TabIndex        =   112
         Top             =   1185
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "MScomm$"
         Height          =   255
         Index           =   46
         Left            =   120
         TabIndex        =   111
         Top             =   1815
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Stop Bits"
         Height          =   225
         Index           =   47
         Left            =   135
         TabIndex        =   110
         Top             =   1515
         Width           =   855
      End
   End
   Begin VB.CommandButton cmdOpenUCSE 
      Caption         =   "&Open NGV File"
      Height          =   375
      Left            =   0
      TabIndex        =   97
      Top             =   4710
      Width           =   1920
   End
   Begin VB.TextBox txtFilename 
      Height          =   285
      Left            =   1995
      TabIndex        =   96
      Text            =   "C:\MTP\Data\NGV\TREX\IWG1.txt"
      Top             =   4785
      Width           =   4860
   End
   Begin VB.Frame Frame 
      Caption         =   "Slider Move"
      Height          =   735
      Index           =   10
      Left            =   7035
      TabIndex        =   93
      Top             =   2970
      Width           =   1455
      Begin VB.CommandButton cmdSliderMove 
         Caption         =   "Back"
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   95
         Top             =   240
         Width           =   615
      End
      Begin VB.CommandButton cmdSliderMove 
         Caption         =   "Fwd"
         Height          =   375
         Index           =   0
         Left            =   720
         TabIndex        =   94
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.TextBox txtSliderPosition 
      Height          =   285
      Left            =   10410
      TabIndex        =   92
      Top             =   3315
      Width           =   375
   End
   Begin VB.TextBox txt2B 
      Height          =   285
      Left            =   11955
      TabIndex        =   91
      Top             =   3315
      Width           =   375
   End
   Begin VB.TextBox txt2A 
      Height          =   285
      Left            =   11340
      TabIndex        =   90
      Top             =   3315
      Width           =   375
   End
   Begin VB.Frame Frame 
      Caption         =   "Frame"
      Height          =   1170
      Index           =   11
      Left            =   11220
      TabIndex        =   86
      Top             =   3570
      Width           =   1050
      Begin VB.CheckBox chkASCII 
         Caption         =   "ASCII"
         Height          =   255
         Left            =   135
         TabIndex        =   116
         Top             =   870
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.TextBox txtTotalFrames 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   88
         Top             =   555
         Width           =   615
      End
      Begin VB.TextBox txtFrame 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   87
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "of"
         Height          =   255
         Index           =   4
         Left            =   780
         TabIndex        =   89
         Top             =   240
         Width           =   180
      End
   End
   Begin VB.TextBox txtPacket2Bh 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   0
      TabIndex        =   85
      Top             =   4290
      Width           =   11175
   End
   Begin VB.TextBox txtPacketNGV 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   315
      Left            =   0
      TabIndex        =   84
      Top             =   3735
      Width           =   11160
   End
   Begin VB.CommandButton cmdRefresh 
      Caption         =   "&Refresh"
      Height          =   495
      Left            =   9825
      TabIndex        =   83
      Top             =   5040
      Width           =   855
   End
   Begin VB.CommandButton Command1 
      Caption         =   "E&xit"
      Height          =   495
      Left            =   11490
      TabIndex        =   82
      Top             =   5040
      Width           =   780
   End
   Begin VB.Frame Frame 
      Caption         =   "Read Mode"
      Height          =   1530
      Index           =   9
      Left            =   7020
      TabIndex        =   78
      Top             =   0
      Width           =   3075
      Begin VB.TextBox txtRemoteIP 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1785
         MultiLine       =   -1  'True
         TabIndex        =   128
         Text            =   "NGVsim.frx":0006
         Top             =   1200
         Width           =   1245
      End
      Begin VB.TextBox txtRemotePort 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2460
         MultiLine       =   -1  'True
         TabIndex        =   125
         Text            =   "NGVsim.frx":0014
         Top             =   885
         Width           =   555
      End
      Begin VB.OptionButton optReadMode 
         Caption         =   "Continuously from UDP Port"
         Height          =   255
         Index           =   1
         Left            =   90
         TabIndex        =   124
         Top             =   945
         Width           =   2295
      End
      Begin VB.OptionButton optReadMode 
         Caption         =   "Continuously from Serial Port"
         Height          =   210
         Index           =   2
         Left            =   90
         TabIndex        =   81
         Top             =   720
         Width           =   2325
      End
      Begin VB.OptionButton optReaMode 
         Caption         =   "Single Step from File"
         Height          =   195
         Index           =   1
         Left            =   90
         TabIndex        =   80
         Top             =   480
         Width           =   2730
      End
      Begin VB.OptionButton optReadMode 
         Caption         =   "Continuously from File"
         Height          =   255
         Index           =   0
         Left            =   90
         TabIndex        =   79
         Top             =   210
         Width           =   2760
      End
      Begin VB.Label Label1 
         Caption         =   "Remote IP Address"
         Height          =   210
         Left            =   390
         TabIndex        =   127
         Top             =   1215
         Width           =   1440
      End
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "&Start"
      Height          =   495
      Left            =   10680
      TabIndex        =   77
      Top             =   5040
      Width           =   795
   End
   Begin VB.CheckBox chkSynthesize 
      Caption         =   "Synthesize"
      Height          =   255
      Left            =   10155
      TabIndex        =   76
      Top             =   2280
      Width           =   1095
   End
   Begin VB.TextBox txtNGVfile 
      Height          =   285
      Left            =   10050
      TabIndex        =   75
      Text            =   "C:\MTP\Data\NGV\Nav.txt"
      Top             =   3000
      Width           =   2235
   End
   Begin VB.CheckBox chkWriteToFile 
      Caption         =   "Write NGV Data to "
      Height          =   255
      Left            =   10155
      TabIndex        =   74
      Top             =   2745
      Width           =   1635
   End
   Begin VB.CommandButton cmdOpenDC8 
      Caption         =   "Open DC-8 RAW File"
      Height          =   375
      Left            =   0
      TabIndex        =   73
      Top             =   5130
      Width           =   1920
   End
   Begin VB.TextBox txtDC8filename 
      Height          =   285
      Left            =   1965
      TabIndex        =   72
      Text            =   "C:\MTP\Data\DC8\PAVE\20050120\DC20050120.RAW"
      Top             =   5175
      Width           =   4875
   End
   Begin VB.Frame Frame 
      Caption         =   "Synthesize Mode"
      Height          =   615
      Index           =   5
      Left            =   7035
      TabIndex        =   68
      Top             =   2355
      Width           =   3060
      Begin VB.OptionButton optSynthMode 
         Caption         =   "DC-8 Flt"
         Height          =   255
         Index           =   2
         Left            =   2085
         TabIndex        =   71
         Top             =   225
         Width           =   900
      End
      Begin VB.OptionButton optSynthMode 
         Caption         =   "Simulate Flt"
         Height          =   255
         Index           =   1
         Left            =   885
         TabIndex        =   70
         Top             =   225
         Width           =   1140
      End
      Begin VB.OptionButton optSynthMode 
         Caption         =   "Static"
         Height          =   150
         Index           =   0
         Left            =   45
         TabIndex        =   69
         Top             =   255
         Width           =   750
      End
   End
   Begin VB.CheckBox chkRepeat 
      Caption         =   "Repeat Last Frame"
      Height          =   195
      Left            =   10155
      TabIndex        =   67
      Top             =   2535
      Width           =   1665
   End
   Begin VB.Timer Timer1 
      Left            =   8505
      Top             =   2985
   End
   Begin VB.Frame Frame 
      Caption         =   "Packet NGV Parameters"
      Height          =   3690
      Index           =   6
      Left            =   15
      TabIndex        =   0
      Top             =   0
      Width           =   6960
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   33
         Top             =   2400
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   32
         Top             =   255
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   31
         Top             =   525
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   30
         Top             =   795
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   29
         Top             =   1065
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   28
         Top             =   1335
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   27
         Top             =   1605
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   26
         Top             =   1875
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   25
         Top             =   2145
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   24
         Top             =   2415
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   23
         Top             =   2685
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   22
         Top             =   2955
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   21
         Top             =   3225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   20
         Top             =   210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   19
         Top             =   480
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   18
         Top             =   750
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   17
         Top             =   1020
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   16
         Top             =   1290
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   15
         Top             =   1560
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   14
         Top             =   1830
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   13
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   12
         Top             =   2670
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   11
         Top             =   2940
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   10
         Top             =   3210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   9
         Top             =   225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   8
         Top             =   495
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   7
         Top             =   765
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   6
         Top             =   1035
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   5
         Top             =   1305
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   4
         Top             =   1575
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   3
         Top             =   1845
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   2
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   1
         Top             =   2385
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "Track Angle (deg)"
         Height          =   210
         Index           =   22
         Left            =   2325
         TabIndex        =   66
         Top             =   510
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Gnd Speed (m/s)"
         Height          =   195
         Index           =   21
         Left            =   135
         TabIndex        =   65
         Top             =   2130
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "UT (sec)"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   64
         Top             =   240
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "Latitude (deg)"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   63
         Top             =   540
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Longitude (deg)"
         Height          =   195
         Index           =   2
         Left            =   120
         TabIndex        =   62
         Top             =   810
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Zgps (m)"
         Height          =   195
         Index           =   3
         Left            =   135
         TabIndex        =   61
         Top             =   1080
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Attack Angle (deg)"
         Height          =   255
         Index           =   14
         Left            =   2370
         TabIndex        =   60
         Top             =   1890
         Width           =   1410
      End
      Begin VB.Label Label 
         Caption         =   "Pitch (deg)"
         Height          =   240
         Index           =   13
         Left            =   2355
         TabIndex        =   59
         Top             =   1065
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Heading (deg)"
         Height          =   255
         Index           =   12
         Left            =   2325
         TabIndex        =   58
         Top             =   255
         Width           =   1080
      End
      Begin VB.Label Label 
         Caption         =   "Total T (C)"
         Height          =   195
         Index           =   11
         Left            =   2355
         TabIndex        =   57
         Top             =   2970
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (C)"
         Height          =   255
         Index           =   8
         Left            =   2355
         TabIndex        =   56
         Top             =   2160
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Slip Angle (deg)"
         Height          =   255
         Index           =   5
         Left            =   2370
         TabIndex        =   55
         Top             =   1620
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Static P (hPa)"
         Height          =   255
         Index           =   9
         Left            =   2370
         TabIndex        =   54
         Top             =   3225
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Roll (deg)"
         Height          =   255
         Index           =   16
         Left            =   2355
         TabIndex        =   53
         Top             =   1335
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (K)"
         Height          =   255
         Index           =   17
         Left            =   2355
         TabIndex        =   52
         Top             =   2445
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Dew Point (C)"
         Height          =   270
         Index           =   18
         Left            =   2355
         TabIndex        =   51
         Top             =   2700
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Drift Angle (deg)"
         Height          =   240
         Index           =   19
         Left            =   2340
         TabIndex        =   50
         Top             =   780
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Dir (deg)"
         Height          =   255
         Index           =   15
         Left            =   4695
         TabIndex        =   49
         Top             =   975
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Speed (m/s)"
         Height          =   255
         Index           =   10
         Left            =   4665
         TabIndex        =   48
         Top             =   720
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Zwgs (m)"
         Height          =   255
         Index           =   6
         Left            =   135
         TabIndex        =   47
         Top             =   1350
         Width           =   900
      End
      Begin VB.Label Label 
         Caption         =   "Zp (feet)"
         Height          =   225
         Index           =   48
         Left            =   150
         TabIndex        =   46
         Top             =   1605
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Zr (feet)"
         Height          =   210
         Index           =   49
         Left            =   150
         TabIndex        =   45
         Top             =   1860
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Vertical Vel (m/s)"
         Height          =   195
         Index           =   55
         Left            =   120
         TabIndex        =   44
         Top             =   3225
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "TAS (m/s)"
         Height          =   180
         Index           =   56
         Left            =   135
         TabIndex        =   43
         Top             =   2400
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "IAS (m/s)"
         Height          =   195
         Index           =   57
         Left            =   165
         TabIndex        =   42
         Top             =   2655
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "Mach Number"
         Height          =   195
         Index           =   59
         Left            =   135
         TabIndex        =   41
         Top             =   2955
         Width           =   1230
      End
      Begin VB.Label Label 
         Caption         =   "Dynamic P (hPa)"
         Height          =   195
         Index           =   60
         Left            =   4650
         TabIndex        =   40
         Top             =   225
         Width           =   1395
      End
      Begin VB.Label Label 
         Caption         =   "Cabin P (hPa)"
         Height          =   195
         Index           =   61
         Left            =   4665
         TabIndex        =   39
         Top             =   495
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Wind Ver (m/s)"
         Height          =   195
         Index           =   62
         Left            =   4680
         TabIndex        =   38
         Top             =   1215
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "SZA (deg)"
         Height          =   195
         Index           =   63
         Left            =   4665
         TabIndex        =   37
         Top             =   1470
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Elev AC (deg)"
         Height          =   195
         Index           =   64
         Left            =   4680
         TabIndex        =   36
         Top             =   1710
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az Gnd (deg)"
         Height          =   195
         Index           =   65
         Left            =   4680
         TabIndex        =   35
         Top             =   1980
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az AC (deg)"
         Height          =   195
         Index           =   66
         Left            =   4680
         TabIndex        =   34
         Top             =   2235
         Width           =   1095
      End
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   8970
      Top             =   2985
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.Slider Slider3 
      Height          =   255
      Left            =   7605
      TabIndex        =   98
      Top             =   4635
      Width           =   3630
      _ExtentX        =   6403
      _ExtentY        =   450
      _Version        =   393216
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   255
      Left            =   0
      TabIndex        =   99
      Top             =   4050
      Width           =   11175
      _ExtentX        =   19711
      _ExtentY        =   450
      _Version        =   393216
      LargeChange     =   0
      Max             =   60
      SelStart        =   1
      Value           =   1
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   11715
      Top             =   2130
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Label Label 
      Caption         =   "Frame"
      Height          =   255
      Index           =   7
      Left            =   7140
      TabIndex        =   117
      Top             =   4650
      Width           =   555
   End
   Begin VB.Label Label 
      Caption         =   "1"
      Height          =   255
      Index           =   51
      Left            =   11130
      TabIndex        =   102
      Top             =   3345
      Width           =   165
   End
   Begin VB.Label Label 
      Caption         =   "2"
      Height          =   195
      Index           =   50
      Left            =   11745
      TabIndex        =   101
      Top             =   3345
      Width           =   195
   End
   Begin VB.Label Label 
      Caption         =   "Values at Slider Position"
      Height          =   195
      Index           =   52
      Left            =   8610
      TabIndex        =   100
      Top             =   3435
      Width           =   1815
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open File"
      End
      Begin VB.Menu mnuFIleClose 
         Caption         =   "&Close File"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit Program"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "frmNGVsim"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Sub ReadNextFrameUDP()
Dim X$, i%

' Find NGV Packet start string
  Call FindIWG1UDP       'Get "IWG1," string
  If EOF(UDPlu) Then EOFflag = True: Exit Sub     'Skip CPK
' And read it from NGV Data File
'  Packet_NGV = "IWG1,"
'  X$ = ""
'  i = 0
'  Do
'    i = i + 1
'    Packet_NGV = Packet_NGV + Mid$(Packet_NGV, i, 1) 'get a character
'    If Len(Packet_NGV) > 2 Then X$ = Right$(Packet_NGV, 2)
'  Loop Until X$ = vbCrLf
  Text2.Text = Packet_NGV
  EOFflag = False


End Sub


Function fHex2$(Char As String)
Dim hx$

hx$ = Hex$(Asc(Char))
If Len(hx$) = 1 Then hx$ = "0" + hx$
fHex2$ = hx$

End Function

Function fHexString$(Packet$)
Dim X$, i%

  X$ = ""
  For i = 1 To Len(Packet)
    X$ = X$ + fHex2(Mid$(Packet, i, 1))
  Next i
  fHexString = X$
  
End Function

Sub ReadNextFrameSerial()
Dim i%, j%, X$
Static NGVlu%
  
' Find 2Ah Packet
  Call FindIWG1      'Get "IWG1," string

' And read it from Serial Port
  X$ = ""
  
  If MSComm1.InBufferCount > 110 Then
    Packet_NGV = ""
    Do
      Packet_NGV = Packet_NGV + MSComm1.Input
'      Debug.Print " " + Hex$(Asc(Right$(Packet_NGV, 1)));
      If Len(Packet_NGV) > 4 Then X$ = Right$(Packet_NGV, 4)
    Loop Until X$ = "\r\n"
  Else
    MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
    Exit Sub
  End If
  Debug.Print ""
  Packet_NGV = Left$(Packet_NGV, Len(Packet_NGV) - 4)

' FrameNumber
  FrameNumber = FrameNumber + 1
  txtFrame.Text = FrameNumber
  If FrameNumber < Slider3.Max Then Slider3.Value = FrameNumber

End Sub





Function fGetChar()
  If ReadMode = 2 Then
    If MSComm1.InBufferCount > 0 Then
      fGetChar = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Function
    End If
  Else
    fGetChar = Asc(Input(1, INPlu)) ' Get one character.
  End If

End Function

Sub SynthesizeData()
' Read DC8 data file and synthesize NGV 1 Hz data
Dim D1$, D2$, Out$
Dim GoToNext As Boolean, Line$, i%
Static OUTlu%

  If chkRepeat.Value = 0 Then
    Do
      If EOF(INPlu) Then EOFflag = True: Exit Sub
      Line Input #INPlu, Line$
    Loop Until Left$(Line$, 1) = "A"
   
    Call DecodeAline(Line$, GoToNext)
    D1$ = Date
    D2$ = Right$(D1$, 4) + "-" + Left$(D1$, 2) + "-" + Mid$(D1$, 4, 2)
    Out$ = "Packet Parameters on " + D2$ + " at " + fSecToTstring$(UTsec, True)
    Frame(6).Caption = Out$
    DoEvents
    For i = 0 To 32: V(i) = 0#: GotV(i) = False: Next i
    V(0) = 1# * UTsec           'IWG1
    GotV(0) = True
    V(1) = OATn + 273.15        'Actually yyyy-mm-ddThh:mm:ss
    GotV(1) = True
    V(2) = Latitude
    GotV(2) = True
    V(3) = Longitude
    GotV(3) = True
    V(4) = gALT * 1000#         'GPS altitude in meters
    GotV(4) = True
    V(6) = pALT * cft_km        'Pressure altitide in feet
    GotV(6) = True
    V(13) = Heading
    GotV(13) = True
    V(16) = Pitch
    GotV(16) = True
    V(17) = Roll
    GotV(17) = True
    V(20) = OATn
    GotV(20) = True
    V(23) = fZtoP(pALT)
    GotV(23) = True
    V(26) = Wspd
    GotV(26) = True
    V(27) = Wdir
    GotV(27) = True
    For i = 0 To 32
      If GotV(i) Then
        Select Case i
        Case 0: txtVar(i).Text = Format(V(i), "#####0")
        Case 2, 3: txtVar(i).Text = Format(V(i), "###0.000")
        Case 4, 6: txtVar(i).Text = Format(V(i), "####0")
        Case 1, 13, 16, 17, 20, 23, 26, 27: txtVar(i).Text = Format(V(i), "###0.0")
        Case Else
          txtVar(i).Text = ""
        End Select
      Else
        txtVar(i).Text = ""
      End If
    Next i
    Call Encode_NGV(Packet_NGV)
  End If

  MSComm1.Output = Packet_NGV
  DoEvents
  If chkWriteToFile.Value = 1 Then
    If OUTlu = 0 Then
      OUTlu = FreeFile
      Open txtNGVfile.Text For Output As OUTlu
    Else
      Print #OUTlu, Packet_NGV
    End If
  End If
  cmdRefresh_Click
  
End Sub


Sub OutputSerialData(EOFflag)
Dim i%
  
'  If chkRepeat.Value = 0 Then ReadNextFrame
  cmdRefresh_Click
  MSComm1.Output = Packet_NGV
  DoEvents

End Sub




Sub Main(EOFflag As Boolean)
Dim Code As Byte, Char As String, n%, Bit$, L As Long
Dim i%, X$
Static lu%, Frame&
' Read data from file or serial port

' Clear Packets
  Packet_NGV = ""
  If ReadMode = 2 Then    'Read Continuously from serial port
    Call ReadNextFrameSerial
  Else                   'Read Continuously from file (ReadMode=0 or 1)
    Call ReadNextFrameFile
  End If
' Now have input Packet_NGV, decode it
  Decode_NGV Packet_NGV
' And update display
  UpdatePacketParmeters
  
  If ReadMode = 2 And NoComPortData Then   'ReadMode=2 is Read Serial Port
    EOFflag = True
    Exit Sub
  End If
  cmdRefresh_Click
  
End Sub


Sub UpdateMScomm()

MScomm$ = ""

Select Case cboBaud.ListIndex
Case 0: MScomm$ = MScomm$ + "300,"
Case 1: MScomm$ = MScomm$ + "4800,"
Case 2: MScomm$ = MScomm$ + "9600,"
Case 3: MScomm$ = MScomm$ + "18200,"
End Select

Select Case cboParity.ListIndex
Case 0: MScomm$ = MScomm$ + "N,"
Case 1: MScomm$ = MScomm$ + "O,"
Case 2: MScomm$ = MScomm$ + "E,"
Case 3: MScomm$ = MScomm$ + "M,"
Case 4: MScomm$ = MScomm$ + "S,"
End Select

Select Case cboBits.ListIndex
Case 0: MScomm$ = MScomm$ + "7,"
Case 1: MScomm$ = MScomm$ + "8,"
End Select

Select Case cboStop.ListIndex
Case 0: MScomm$ = MScomm$ + "1"
Case 1: MScomm$ = MScomm$ + "2"
End Select

COMport = cboCOMport.ListIndex + 1

txtMScomm.Text = MScomm$

End Sub



Sub UpdatePacketParmeters()
Dim D1$, i%, Out$
    
  D1$ = HHMMSS$
  Out$ = "Packet Parameters on " + D1$ + " at " + fSecToTstring$(UTsec, True)
  For i = 0 To 32
    If GotV(i) Then
      Select Case i
      Case 2, 3, 11
        txtVar(i).Text = Format(V(i), "###0.000")
      Case 0, 4, 5, 6, 7
        txtVar(i).Text = Format(V(i), "####0")
      Case Else
        txtVar(i).Text = Format(V(i), "###0.00")
      End Select
    Else
      txtVar(i).Text = ""
    End If
  Next i

End Sub

Private Sub cboBaud_Click()
UpdateMScomm
End Sub


Private Sub cboBits_Click()
UpdateMScomm
End Sub


Private Sub cboCOMport_Click()
UpdateMScomm
End Sub


Private Sub cboParity_Click()
UpdateMScomm
End Sub








Private Sub cmdOpenDC8_Click()
  Filename$ = txtDC8filename.Text
  INPlu = FreeFile
  Open Filename$ For Input As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 412
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  FrameNumber = 0
  chkSynthesize.Value = 1
  optReadMode(3).Value = True
  cmdStart_Click

End Sub

Private Sub cmdOpenUCSE_Click()
Dim Filename$

  Filename$ = txtFilename.Text
  INPlu = FreeFile
  Open Filename$ For Input As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 120   'guess
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
  FrameNumber = 0
  chkSynthesize.Value = 0
  optReadMode(0).Value = True
  cmdStart_Click
  
End Sub


Private Sub cmdRefresh_Click()
Dim Code As Byte, Char As String, n%, Bit$, L As Long
Dim i%, j%, X$
Static NGVlu%

' Find NGV Packet
  If chkASCII.Value = 0 Then   '
    Char = ""
    For i = 1 To Len(Packet_NGV)
      X$ = Hex(Asc(Mid$(Packet_NGV, i, 1)))
      If Len(X$) = 1 Then X$ = "0" + X$
      Char = Char + X$
    Next i
  Else                    'Write ASCII
  End If
  txtPacketNGV.Text = Packet_NGV
  Call Decode_NGV(Packet_NGV)
  
  DoEvents
  
  ' Only write good frames
  If chkWriteToFile.Value = 1 Then
'    If NGVlu = 0 Then
'      NGVlu = FreeFile
'      Open txtNGVfile.Text For Output As NGVlu
'      X$ = "Frame" + vbTab + "UTks" + vbTab + "pALT" + vbTab + "Zgps" + vbTab
'      X$ = X$ + "OAT" + vbTab + "Latitude" + vbTab + "Longitude" + vbTab + "Pitch" + vbTab + "Roll" + vbTab
'      X$ = X$ + "Wspd" + vbTab + "Wdir"
'      Print #NGVlu, X$
'    End If
'      If UTks <> 0 Then
'        Print #NGVlu, Frame, vbTab, UTks, vbTab, pALT / 1000, vbTab, Zgps / 1000, vbTab,
'        Print #NGVlu, OAT + 273.15, vbTab, Latitude, vbTab, Longitude, vbTab, Pitch, vbTab,
'        Print #NGVlu, Roll, vbTab, WindSpeed, vbTab, WindDir
'      Else
'      'Debug.Print Frame, CC1 And 150
'      End If
  End If

End Sub

Private Sub cmdSliderMove_Click(Index As Integer)
Select Case Index
Case 0
  Slider2.Value = Slider2.Value + 1
Case 1
  Slider2.Value = Slider2.Value - 1
End Select
End Sub

Private Sub cmdStart_Click()

If Not FileOpen And ReadMode <> 2 Then
  MsgBox "You must open a file before starting!", vbOKOnly
  Exit Sub
End If

UpdateMScomm

Select Case ReadMode
Case 0  'Continuous from file
  Timer1.Enabled = True
  
Case 1  'Single Step from file
  cmdStart.Caption = "&Next"
  Timer1.Enabled = True

Case 2  'Read Continuously from Serial Port
' Use COM1.
  MSComm1.CommPort = COMport
' 9600 baud, no parity, 8 data, and 1 stop bit.
  MSComm1.Settings = MScomm$   '"9600,N,8," + Format(COMport, "0")
' Tell the control to read entire buffer when Input is used.
'  If ReadLine Then
'    MSComm1.InputLen = 0       'Read entire input buffer
'  Else
    MSComm1.InputLen = 1       'Read single character
'  End If
' Open the port.
  MSComm1.PortOpen = True
  Timer1.Enabled = True

Case 3  'Write to Serial Port
' Use COM1.
  MSComm1.CommPort = COMport
' 9600 baud, no parity, 8 data, and 1 stop bit.
  MSComm1.Settings = MScomm$   '"9600,N,8," + Format(COMport, "0")
' Open port
  MSComm1.PortOpen = True
 FrameNumber = 0
  Timer1.Enabled = True
  
End Select

End Sub



Private Sub Command1_Click()
Close All
End
End Sub




Private Sub Form_Load()
Timer1.Interval = 1000
txtTimerSpeed.Text = Timer1.Interval
Slider1.Value = Timer1.Interval
Timer1.Enabled = False
WriteHex = False

' Set default option controls
optReadMode(1).Value = True
optSynthMode(2).Value = True
ReadMode = 1
StartUP = True

With cboCOMport
.AddItem "1"
.AddItem "2"
.AddItem "3"
.AddItem "4"
.ListIndex = 3
End With

With cboBaud
.AddItem "300"
.AddItem "4800"
.AddItem "9600"
.AddItem "18200"
.ListIndex = 2
End With

With cboParity
.AddItem "None"
.AddItem "Odd"
.AddItem "Even"
.AddItem "Mark"
.AddItem "Space"
.ListIndex = 1
End With

With cboBits
.AddItem "7"
.AddItem "8"
.ListIndex = 1
End With

With cboStop
.AddItem "1"
.AddItem "2"
.ListIndex = 0
End With

White = &HFFFFFF
Gray = &HC0C0C0

DLE = Chr(&H10)
ETX = Chr(&H3)
FF = Chr(&HFF)
Zero = Chr(&H0)
A = Chr(&H2A)
B = Chr(&H2B)
NoComPortData = False
cft_km = 3280.8
Ceiling = 25

UpdateMScomm

End Sub



Private Sub mnuFIleClose_Click()
Close All

End Sub

Private Sub mnuFileExit_Click()
Close All
End
End Sub

Private Sub mnuFileOpen_Click()

If SSTab1.Tab = 0 Then
' Set Initial directory
  CommonDialog1.InitDir = "C:\ER2\Euplex\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  If Len(Filename$) = 0 Then Exit Sub
  txtFilename.Text = Filename$
Else
' Set Initial directory
  CommonDialog1.InitDir = "G:\ER2\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "RAW Files (*.RAW)|*.RAW"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  If Len(Filename$) = 0 Then Exit Sub
  txtER2filename.Text = Filename$
End If
  
  INPlu = FreeFile
  Open Filename$ For Binary As INPlu
  FileOpen = True
  FileLength = LOF(INPlu)
  TotalFrames = FileLength / 412
  txtTotalFrames.Text = Str(Int(TotalFrames))
  Slider3.Min = 1
  Slider3.Max = TotalFrames + 2
  Slider3.Value = 1
 FrameNumber = 0
End Sub


Private Sub mnuHelpAbout_Click()
  Load frmAbout
  'Set defaults
  With frmAbout
    .lblApplication.Caption = "UCSE Simulator"
    .lblHeading.Caption = "MJ Mahoney"
    .lblVersion.Caption = "Version: 1.0 Beta"
    .lblCopyright.Caption = "Copyright © 2002"
    .Show
  End With

End Sub





Private Sub optReadMode_Click(Index As Integer)
ReadMode = Index
End Sub

Private Sub optSynthMode_Click(Index As Integer)
SynthMode = Index
End Sub

Private Sub Slider1_Change()
Timer1.Interval = Slider1.Value
txtTimerSpeed.Text = Slider1.Value
End Sub

Private Sub Slider2_Change()
Dim X$
If Slider2.Value = 0 Then Slider2.Value = 1
If Slider2.Value = 61 Then Slider2.Value = 60
txtSliderPosition.Text = Slider2.Value
X$ = Hex(Asc(Mid$(Packet_NGV, Slider2.Value, 1)))
If Len(X$) = 1 Then txt2A.Text = "0" + X$ Else txt2A.Text = X$
'If Slider2.Value < 33 Then
'  X$ = Hex(Asc(Mid$(Packet_2Bh(Val(cbo2Bh.Text)), Slider2.Value, 1)))
'Else
'  X$ = ""
'End If
If Len(X$) = 1 Then txt2B.Text = "0" + X$ Else txt2B.Text = X$

End Sub

Private Sub Timer1_Timer()
Dim EOFflag As Boolean
'Run Mode
' 0   Read continuously from NGV file
' 1   Read single record from NGV file
' 2   Read continously  from Serial Port
' 3   Write continuously to Serial Port

  Timer1.Enabled = False
  If ReadMode = 3 Then               'Write continuously to Serial Port
    If chkSynthesize.Value = 1 Then 'Use synthesized DC8 data
      Call SynthesizeData
    Else                            'Use data from NGV file
      Call OutputSerialData(EOFflag)
    End If
  Else                              'Read data from from file continuously(0), from file next (1), from serial port (2)
    If Not EOFflag Then
      Call Main(EOFflag)
    End If
  End If
  
  If EOFflag = False And cmdStart.Caption <> "&Next" Then
    Timer1.Enabled = True           'Get next frame
  Else
    If EOFflag = False Then
      Timer1.Enabled = False        'Use next button to get next frame
      Slider2_Change
      Slider3.Value = FrameNumber
    Else
      Close All
      MSComm1.PortOpen = False
      Exit Sub
    End If
  End If

End Sub


Private Sub txtTimerSpeed_Change()
Timer1.Interval = Val(txtTimerSpeed.Text)
End Sub



Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'New_Text is the text that has just arrived from across the socket
Dim D$

  Winsock1.GetData Packet_NGV
' Show the new text
  Text1.SelText = Packet_NGV
  Frame1.Caption = Winsock1.RemoteHostIP
' Show the byte size of this transmission in the statusbar
  StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
' Read next UDP frame
  ReadNextFrameUDP
' Now have input Packet_NGV, decode it
  Decode_NGV Packet_NGV
' And update display
  UpdatePacketParmeters
'IWG1,20010920T151645,14.642,-96.4235,
'1234567890123456789012345678901234567890
  D$ = Left$(yyyymmdd$, 4) + "-" + Mid$(yyyymmdd$, 5, 2) + "-" + Right$(yyyymmdd$, 2)
  D$ = "Packet Parameters on " + D$ + " at " + fSecToTstring$(UTsec, True)
  Frame(6).Caption = D$

End Sub

