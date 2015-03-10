VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmUDP 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Winsock UDP"
   ClientHeight    =   8055
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7110
   Icon            =   "UDP_Tx.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8055
   ScaleWidth      =   7110
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtStartks 
      Height          =   285
      Left            =   4140
      TabIndex        =   107
      Top             =   6525
      Width           =   600
   End
   Begin VB.CheckBox chkStartTime 
      Caption         =   "Start at"
      Height          =   195
      Left            =   3255
      TabIndex        =   106
      Top             =   6555
      Width           =   810
   End
   Begin VB.OptionButton optShowUDP 
      Caption         =   "UDP Out Packet"
      Height          =   240
      Index           =   1
      Left            =   1620
      TabIndex        =   105
      Top             =   6525
      Value           =   -1  'True
      Width           =   1515
   End
   Begin VB.OptionButton optShowUDP 
      Caption         =   "UDP In Packet"
      Height          =   240
      Index           =   0
      Left            =   105
      TabIndex        =   104
      Top             =   6510
      Width           =   1515
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   4095
      Top             =   7440
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox txtPamp 
      Alignment       =   1  'Right Justify
      Height          =   300
      Left            =   5400
      MultiLine       =   -1  'True
      TabIndex        =   96
      Text            =   "UDP_Tx.frx":0442
      Top             =   4440
      Width           =   480
   End
   Begin VB.TextBox txtRamp 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   6510
      MultiLine       =   -1  'True
      TabIndex        =   95
      Text            =   "UDP_Tx.frx":0445
      Top             =   4425
      Width           =   495
   End
   Begin VB.CheckBox chkAddPeakNoise 
      Caption         =   "Add Peak Noise"
      Height          =   210
      Left            =   4905
      TabIndex        =   94
      Top             =   4185
      Width           =   1515
   End
   Begin VB.CheckBox chkSetPR 
      Caption         =   "Set Pitch and Roll"
      Height          =   195
      Left            =   4905
      TabIndex        =   93
      Top             =   3960
      Width           =   2010
   End
   Begin VB.CheckBox chkSetZp 
      Caption         =   "Set Pressure Altitude"
      Height          =   240
      Left            =   4905
      TabIndex        =   92
      Top             =   3675
      Width           =   1860
   End
   Begin VB.TextBox txtUpdating 
      Alignment       =   2  'Center
      BackColor       =   &H000000FF&
      Height          =   300
      Left            =   3225
      TabIndex        =   91
      Text            =   "Updating"
      Top             =   7140
      Width           =   1290
   End
   Begin VB.TextBox txtLocalPort 
      BackColor       =   &H00C0C0C0&
      Height          =   285
      Left            =   1605
      TabIndex        =   88
      Text            =   "58800"
      Top             =   7455
      Width           =   1575
   End
   Begin VB.Frame Frame2 
      Caption         =   "Transmitted IWG1 Packet"
      Enabled         =   0   'False
      Height          =   1755
      Left            =   105
      TabIndex        =   86
      Top             =   4755
      Width           =   6915
      Begin VB.TextBox Text2 
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   161
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00400000&
         Height          =   1395
         Left            =   105
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   87
         Top             =   270
         Width           =   6690
      End
   End
   Begin VB.ComboBox cboRemoteIP 
      Height          =   315
      Left            =   1605
      TabIndex        =   85
      Text            =   "192.168.84.255"
      Top             =   7110
      Width           =   1590
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Clear"
      Height          =   390
      Left            =   5535
      TabIndex        =   84
      Top             =   7335
      Width           =   840
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   375
      Left            =   4635
      TabIndex        =   83
      Top             =   7350
      Visible         =   0   'False
      Width           =   870
   End
   Begin VB.Frame Frame3 
      Caption         =   "Nav Statistics"
      Height          =   1110
      Left            =   75
      TabIndex        =   72
      Top             =   3645
      Width           =   4560
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   3255
         MultiLine       =   -1  'True
         TabIndex        =   102
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   3255
         MultiLine       =   -1  'True
         TabIndex        =   101
         Top             =   405
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   3870
         MultiLine       =   -1  'True
         TabIndex        =   100
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   3870
         MultiLine       =   -1  'True
         TabIndex        =   99
         Top             =   390
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   765
         MultiLine       =   -1  'True
         TabIndex        =   82
         Top             =   405
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   2670
         MultiLine       =   -1  'True
         TabIndex        =   79
         Top             =   705
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   2670
         MultiLine       =   -1  'True
         TabIndex        =   78
         Top             =   405
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   77
         Top             =   705
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   1410
         MultiLine       =   -1  'True
         TabIndex        =   76
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   765
         MultiLine       =   -1  'True
         TabIndex        =   75
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   74
         Top             =   420
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   1410
         MultiLine       =   -1  'True
         TabIndex        =   73
         Top             =   420
         Width           =   585
      End
      Begin VB.Label Label 
         Caption         =   "Pitch       Roll        pALT     OAT      Lat        Lon"
         Height          =   195
         Index           =   20
         Left            =   780
         TabIndex        =   103
         Top             =   165
         Width           =   3645
      End
      Begin VB.Label Label 
         Caption         =   "Std Dev"
         Height          =   270
         Index           =   7
         Left            =   105
         TabIndex        =   81
         Top             =   720
         Width           =   660
      End
      Begin VB.Label Label 
         Caption         =   "Average"
         Height          =   270
         Index           =   4
         Left            =   105
         TabIndex        =   80
         Top             =   405
         Width           =   660
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "Decoded Packet Parameters"
      Height          =   3555
      Index           =   6
      Left            =   105
      TabIndex        =   5
      Top             =   30
      Width           =   6960
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   38
         Top             =   2400
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   37
         Top             =   255
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   36
         Top             =   525
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   35
         Top             =   795
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   34
         Top             =   1065
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   33
         Top             =   1335
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   32
         Top             =   1605
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   31
         Top             =   1875
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   30
         Top             =   2145
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   29
         Top             =   2415
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   28
         Top             =   2685
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   27
         Top             =   2955
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   26
         Top             =   3225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   25
         Top             =   210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   24
         Top             =   480
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   23
         Top             =   750
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   22
         Top             =   1020
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   21
         Top             =   1290
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   20
         Top             =   1560
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   19
         Top             =   1830
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   18
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   17
         Top             =   2670
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   16
         Top             =   2940
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   15
         Top             =   3210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   14
         Top             =   225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   13
         Top             =   495
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   12
         Top             =   765
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   11
         Top             =   1035
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   10
         Top             =   1305
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   9
         Top             =   1575
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   8
         Top             =   1845
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   7
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   6
         Top             =   2385
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "Track Angle (deg)"
         Height          =   210
         Index           =   22
         Left            =   2325
         TabIndex        =   71
         Top             =   510
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Gnd Speed (m/s)"
         Height          =   195
         Index           =   21
         Left            =   135
         TabIndex        =   70
         Top             =   2130
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "UT (sec)"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   69
         Top             =   240
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "Latitude (deg)"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   68
         Top             =   540
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Longitude (deg)"
         Height          =   195
         Index           =   2
         Left            =   120
         TabIndex        =   67
         Top             =   810
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Zgps (m)"
         Height          =   195
         Index           =   3
         Left            =   135
         TabIndex        =   66
         Top             =   1080
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Attack Angle (deg)"
         Height          =   255
         Index           =   14
         Left            =   2370
         TabIndex        =   65
         Top             =   1890
         Width           =   1410
      End
      Begin VB.Label Label 
         Caption         =   "Pitch (deg)"
         Height          =   240
         Index           =   13
         Left            =   2355
         TabIndex        =   64
         Top             =   1065
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Heading (deg)"
         Height          =   255
         Index           =   12
         Left            =   2325
         TabIndex        =   63
         Top             =   255
         Width           =   1080
      End
      Begin VB.Label Label 
         Caption         =   "Total T (C)"
         Height          =   195
         Index           =   11
         Left            =   2355
         TabIndex        =   62
         Top             =   2970
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (C)"
         Height          =   255
         Index           =   8
         Left            =   2355
         TabIndex        =   61
         Top             =   2160
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Slip Angle (deg)"
         Height          =   255
         Index           =   5
         Left            =   2370
         TabIndex        =   60
         Top             =   1620
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Static P (hPa)"
         Height          =   255
         Index           =   9
         Left            =   2370
         TabIndex        =   59
         Top             =   3225
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Roll (deg)"
         Height          =   255
         Index           =   16
         Left            =   2355
         TabIndex        =   58
         Top             =   1335
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (K)"
         Height          =   255
         Index           =   17
         Left            =   2355
         TabIndex        =   57
         Top             =   2445
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Dew Point (C)"
         Height          =   270
         Index           =   18
         Left            =   2355
         TabIndex        =   56
         Top             =   2700
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Drift Angle (deg)"
         Height          =   240
         Index           =   19
         Left            =   2340
         TabIndex        =   55
         Top             =   780
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Dir (deg)"
         Height          =   255
         Index           =   15
         Left            =   4695
         TabIndex        =   54
         Top             =   975
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Speed (m/s)"
         Height          =   255
         Index           =   10
         Left            =   4665
         TabIndex        =   53
         Top             =   720
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Zwgs (m)"
         Height          =   255
         Index           =   6
         Left            =   135
         TabIndex        =   52
         Top             =   1350
         Width           =   900
      End
      Begin VB.Label Label 
         Caption         =   "Zp (feet)"
         Height          =   225
         Index           =   48
         Left            =   150
         TabIndex        =   51
         Top             =   1605
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Zr (feet)"
         Height          =   210
         Index           =   49
         Left            =   150
         TabIndex        =   50
         Top             =   1860
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Vertical Vel (m/s)"
         Height          =   195
         Index           =   55
         Left            =   120
         TabIndex        =   49
         Top             =   3225
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "TAS (m/s)"
         Height          =   180
         Index           =   56
         Left            =   135
         TabIndex        =   48
         Top             =   2400
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "IAS (m/s)"
         Height          =   195
         Index           =   57
         Left            =   165
         TabIndex        =   47
         Top             =   2655
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "Mach Number"
         Height          =   195
         Index           =   59
         Left            =   135
         TabIndex        =   46
         Top             =   2955
         Width           =   1230
      End
      Begin VB.Label Label 
         Caption         =   "Dynamic P (hPa)"
         Height          =   195
         Index           =   60
         Left            =   4650
         TabIndex        =   45
         Top             =   225
         Width           =   1395
      End
      Begin VB.Label Label 
         Caption         =   "Cabin P (hPa)"
         Height          =   195
         Index           =   61
         Left            =   4665
         TabIndex        =   44
         Top             =   495
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Wind Ver (m/s)"
         Height          =   195
         Index           =   62
         Left            =   4680
         TabIndex        =   43
         Top             =   1215
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "SZA (deg)"
         Height          =   195
         Index           =   63
         Left            =   4665
         TabIndex        =   42
         Top             =   1470
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Elev AC (deg)"
         Height          =   195
         Index           =   64
         Left            =   4680
         TabIndex        =   41
         Top             =   1710
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az Gnd (deg)"
         Height          =   195
         Index           =   65
         Left            =   4680
         TabIndex        =   40
         Top             =   1980
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az AC (deg)"
         Height          =   195
         Index           =   66
         Left            =   4680
         TabIndex        =   39
         Top             =   2235
         Width           =   1095
      End
   End
   Begin VB.CheckBox chkReopenOnEOF 
      Caption         =   "Reopen On EOF"
      Height          =   240
      Left            =   5535
      TabIndex        =   4
      Top             =   6540
      Value           =   1  'Checked
      Width           =   1485
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   405
      Left            =   6405
      TabIndex        =   3
      Top             =   7320
      Width           =   675
   End
   Begin VB.Timer Timer1 
      Interval        =   1000
      Left            =   3690
      Top             =   7440
   End
   Begin VB.CommandButton cmdOpenIWG1 
      Caption         =   "Open File"
      Height          =   285
      Left            =   45
      TabIndex        =   2
      ToolTipText     =   "Open .RAW or .txt to send IWG UDP data"
      Top             =   6780
      Width           =   1200
   End
   Begin VB.TextBox txtFilename 
      Height          =   285
      Left            =   1290
      TabIndex        =   1
      Text            =   "IWG1.txt"
      Top             =   6810
      Width           =   5715
   End
   Begin ComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   285
      Left            =   0
      TabIndex        =   0
      Top             =   7770
      Width           =   7110
      _ExtentX        =   12541
      _ExtentY        =   503
      SimpleText      =   ""
      _Version        =   327682
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   2
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   2
            Object.Width           =   2566
            Text            =   "  No connection...  "
            TextSave        =   "  No connection...  "
            Key             =   "STATUS"
            Object.Tag             =   ""
            Object.ToolTipText     =   "The current status of the connection"
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   1
            Object.Width           =   9895
            TextSave        =   ""
            Key             =   "DATA"
            Object.Tag             =   ""
            Object.ToolTipText     =   "The last data transfer through the modem"
         EndProperty
      EndProperty
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   3255
      Top             =   7440
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.Label Label 
      Caption         =   "hhmm"
      Height          =   195
      Index           =   23
      Left            =   4815
      TabIndex        =   108
      Top             =   6555
      Width           =   450
   End
   Begin VB.Label Label 
      Caption         =   "Pamp"
      Height          =   270
      Index           =   25
      Left            =   4890
      TabIndex        =   98
      Top             =   4515
      Width           =   480
   End
   Begin VB.Label Label 
      Caption         =   "Ramp"
      Height          =   240
      Index           =   26
      Left            =   6015
      TabIndex        =   97
      Top             =   4500
      Width           =   525
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Local Port"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   300
      Left            =   45
      TabIndex        =   90
      Top             =   7440
      Width           =   1545
   End
   Begin VB.Label Label1 
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Destination IP"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   285
      Left            =   45
      TabIndex        =   89
      Top             =   7125
      Width           =   1530
   End
End
Attribute VB_Name = "frmUDP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private IgnoreText As Boolean, UDPlu%, EOFflag As Boolean, UDP_Transmit As Boolean
Private HHMMSS$



Sub UpdatePacketParameters()
Dim D1$, i%, j%, k%, Out$, v!, Amp!
    
'  Exit Sub
  D1$ = HHMMSS$
  Out$ = "Packet Parameters on " + D1$ + " at " + fSecToTstringX$(UTsecI, True)
  
  j = InStr(1, UDP_Packet_In, ",")            'skip IWG1
  k = InStr(j + 1, UDP_Packet_In, ",")
  Out$ = Left$(UDP_Packet_In, k)
  txtVar(0).Text = Format(DatV(0), "####0")
  txtVar(1).Text = Format(DatV(1), "###0.00")
  
  For i = 2 To 32
    If GotV(i) Then
      Select Case i
      Case 2, 3
        txtVar(i).Text = Format(DatV(i), "####0.0000")
        Out = Out + Format(DatV(i), "####0.0000") + ","
      Case 4, 5, 6, 7
        txtVar(i).Text = Format(DatV(i), "####0")
        Out = Out + Format(DatV(i), "####0") + ","
      Case 16, 17  'Pitch and Roll
        If chkSetPR.Value = 0 Then
          txtVar(i).Text = Format(DatV(i), "###0.00")
        Else
          If chkAddPeakNoise.Value = 1 Then
            If i = 16 Then Amp = Val(txtPamp.Text) Else Amp = Val(txtRamp.Text)
            v = v + 2 * Amp * Rnd - Amp
            Out = Out + Format(Val(txtVar(i).Text) + v, "###0.00") + ","
          Else
            Out = Out + Format(Val(txtVar(i).Text), "###0.00") + ","
          End If
        End If
      Case Else
        txtVar(i).Text = Format(DatV(i), "###0.00")
        Out = Out + Format(DatV(i), "###0.00") + ","
      End Select
    Else
      txtVar(i).Text = ""
      Out = Out + ","

    End If
  Next i
  If chkAddPeakNoise.Value = 1 Then UDP_Packet_Out = Out$
  
End Sub

Private Sub cmdClear_Click()
On Error Resume Next
'Text1 = ""
With Text2
   'Clear the text window and
   .Text = " "
   'return the focus
   .SetFocus
End With
End Sub


Private Sub chkSetPR_Click()
  
  If chkSetPR.Value = 1 Then
    txtVar(16).Enabled = True
    txtVar(17).Enabled = True
    txtVar(16).BackColor = &H80FF80
    txtVar(17).BackColor = &H80FF80
    SetPR = True
  Else
    txtVar(16).Enabled = False
    txtVar(17).Enabled = False
    txtVar(16).BackColor = &H80000005
    txtVar(17).BackColor = &H80000005
    SetPR = False
  End If

End Sub

Private Sub chkSetZp_Click()
  
  If chkSetZp.Value = 1 Then
    txtVar(6).Enabled = True
    txtVar(6).BackColor = &H80FF80
    SetZp = True
  Else
    txtVar(6).Enabled = False
    txtVar(6).BackColor = &H80000005
    SetZp = False
  End If

End Sub


Private Sub cmdConnect_Click()
' Establish Winsock connection - required code
  LastTime = Date + Time
  LocalPort = Trim(txtLocalPort.Text)
'  RemotePort = Trim(txtRemotePort.Text)
  RemoteIP = Trim(cboRemoteIP.Text)
  Winsock1_Connect
  
' Remaining code is form dependent
  Frame2.Caption = Winsock1.LocalIP
' Make sure that the user can't change the local port
  txtLocalPort.Locked = True
' Show the current status of the connection in the status bar
  StatusBar1.Panels(1).Text = "  Connected to " & Winsock1.RemoteHost & "  "

'  Frame1.Enabled = True
  Frame2.Enabled = True
'  Label4.Visible = True
  Text2.SetFocus

' Start timer to  send data periodically
  With Timer1
  .Interval = 1000  ' 1 Second
  .Enabled = True
  End With
End Sub


Private Sub cmdExit_Click()
  
  Close UDPlu
  Winsock1_Close
  Timer1.Enabled = False
  frmUDP.Hide
  End

End Sub

Private Sub cmdOpenIWG1_Click()
Dim Filename$, UTstart&

' Open an existing IWG or RAW file and use it to transmit IWG records
' Set Initial directory
  CommonDialog1.InitDir = "C:\MTP\Data\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "RAW Files (*.RAW)|*.RAW|IWG Files (*.txt)|*.txt|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  txtFilename.Text = Filename$

' Open file that is to be sent via UPD
  UDPlu = FreeFile
'  Open App.Path + "\" + Filename$ For Input As UDPlu
  Open Filename$ For Input As UDPlu

  If chkStartTime.Value = 1 Then
    UTstart = Left$(txtStartks.Text, 2) * 3600 + Right$(txtStartks.Text, 2) * 60
    Do
      Do
        Line Input #UDPlu, UDP_Packet_In
      Loop Until Left$(UDP_Packet_In, 1) = "I" Or (Len(UDP_Packet_In) > 1 And Mid$(UDP_Packet_In, 2, 3) = "IWG") Or EOF(UDPlu)
      Decode_NGV UDP_Packet_In     'This substitutes P and R if chkSetPR is true
      If UTsecI > UTstart Then
        cmdConnect_Click
        Exit Sub
      End If
    Loop
  End If
' Connnect to port
  cmdConnect_Click
  
End Sub
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = vbKeyF1 Then
ChDir App.Path
Shell "notepad.exe readme.txt", vbNormalFocus
End If

End Sub

Private Sub Form_Load()
Dim i%

  Show
' MsgBox "Winsock UDT Chat" & vbCrLf & "by Theo Kandiliotis (ionikh@hol.gr)" & vbCrLf & vbCrLf & "F1 for help.", vbInformation
' txtRemoteIP = Winsock1.LocalIP
  With cboRemoteIP
  .AddItem "128.117.1.255"
  .AddItem "192.168.84.255"
  .AddItem Winsock1.LocalIP
  .ListIndex = 1
  End With
  RemotePort = 58800

  For i = 0 To 32
    txtVar(i).Enabled = False
  Next i

  UDP_Transmit = True
  
End Sub

Private Sub Text2_KeyPress(KeyAscii As Integer)

'The position of the last linefeed within the text
Static Last_Line_Feed As Long

'The new line of text
Dim New_Line As String
'Reset the position of the last line feed if the
'user has clear the chat window
If Trim(Text2) = vbNullString Then Last_Line_Feed = 0
'If the user pressed Enter...
If KeyAscii = 13 Then
   'Get the new line of text
   New_Line = Mid(Text2, Last_Line_Feed + 1)
   'Save the position of the current linefeed
   Last_Line_Feed = Text2.SelStart
   'Send the new text across the socket
   Winsock1.SendData New_Line
   StatusBar1.Panels(2).Text = "  Sent " & (LenB(New_Line) / 2) & " bytes  "
End If

End Sub

Sub ReadNextFrameFile()
Dim i%, j%, X$

' Find NGV Packet start string
  Call FindIWG1file       'Get "IWG1," string
  If EOFflag Then
    Close UDPlu
    UDPlu = 0
    Exit Sub              'Skip CPK
  End If
' And read it from NGV Data File
  UDP_Packet_In = "IWG1,"
  X$ = ""
  Do
    UDP_Packet_In = UDP_Packet_In + Input(1, UDPlu)   'get a character
    If Len(UDP_Packet_In) > 4 Then X$ = Right$(UDP_Packet_In, 4)
  Loop Until X$ = "\r\n"  'vbCR+vbLF
  UDP_Packet_In = Left$(UDP_Packet_In, Len(UDP_Packet_In) - 4) + vbCrLf
'  Text2.Text = UDP_Packet_Out
  EOFflag = False

End Sub

Sub FindIWG1file()
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
'  If RunMode = 2 Then                'Read data from serial port
'    If MSComm1.InBufferCount > 0 Then
'      IWG1$ = IWG1$ + MSComm1.Input
'    Else
'      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
'      NoComPortData = True
'      Exit Sub
'    End If
'  Else                               'Read data from file
    If EOF(UDPlu) Then
      Close UDPlu
      EOFflag = True
      Exit Sub
    End If
    IWG1$ = IWG1$ + Input(1, UDPlu)        'Get one character.
'  End If
  Return
End Sub

Private Sub Timer1_Timer()
' The position of the last linefeed within the text
Dim Line$
Static Last_Line_Feed As Long
' The new line of text
Dim New_Line As String, Filename$

  Timer1.Enabled = False
  If UDPlu = 0 Then Exit Sub
  Do
    Line Input #UDPlu, UDP_Packet_In
'  Loop Until Left$(UDP_Packet_In, 1) = "I" Or (Len(UDP_Packet_In) > 1 And Mid$(UDP_Packet_In, 2, 3) = "IWG") Or EOF(UDPlu)
  Loop Until (Len(UDP_Packet_In) > 1 And Mid$(UDP_Packet_In, 1, 3) = "IWG") Or EOF(UDPlu)
  If Left$(UDP_Packet_In, 1) = """" Then
    UDP_Packet_In = Right$(UDP_Packet_In, Len(UDP_Packet_In) - 1)
  End If

  If EOF(UDPlu) Then           'EOFflag set in ReadNextFrameFile and file is closed
  ' Check if file should be reopened on EOF
    If chkReopenOnEOF.Value = 1 Then
      Filename$ = txtFilename.Text
      UDPlu = FreeFile
      Open App.Path + "\" + Filename$ For Input As UDPlu
      EOFflag = False
    End If
  End If

' Reset the position of the last line feed if the
' user has cleared the chat window
  If Trim(Text2) = vbNullString Then Last_Line_Feed = 0
' Save the position of the current linefeed
  Last_Line_Feed = Text2.SelStart

' Send the new text across the socket
  Decode_NGV UDP_Packet_In     'This substitutes P and R if chkSetPR is true
'  UDP_Packet_Out = UDP_Packet_In

  UpdatePacketParameters  'Add P & R noise

' Display In or Out packet in text box
  If optShowUDP(0).Value = True Then
    frmUDP.Text2.Text = UDP_Packet_In
  Else
'    If chkAddPeakNoise.Value = 0 Then
'      frmUDP.Text2.Text = UDP_Packet_In
'    Else
      frmUDP.Text2.Text = UDP_Packet_Out
'    End If
  End If
  
' Code below here is form dependent
' Show nav statistics
  txtAvg(0).Text = Format(PitchAvg, "+#0.00;-#0.00")
  txtAvg(1).Text = Format(RollAvg, "+#0.00;-#0.00")
  txtAvg(2).Text = Format(ZpAvg, "+#0.00;-#0.00")
  txtAvg(3).Text = Format(OatAvg, "000.00")
  txtAvg(4).Text = Format(LatAvg, "+#0.000;-#0.000")
  txtAvg(5).Text = Format(LonAvg, "+##0.000;-##0.000")
  
  txtRMS(0).Text = Format(PitchRms, "#0.00")
  txtRMS(1).Text = Format(RollRms, "#0.00")
  txtRMS(2).Text = Format(ZpRms, "0.00")
  txtRMS(3).Text = Format(OatRms, "0.00")
  txtRMS(4).Text = Format(LatRms, "0.000")
  txtRMS(5).Text = Format(LonRms, "0.000")
    
  Winsock1.SendData UDP_Packet_Out

  StatusBar1.Panels(2).Text = "  Sent " & (LenB(UDP_Packet_In) / 2) & " bytes  "

  Timer1.Enabled = True
  
End Sub

Private Sub Winsock1_Close()
    
  Do Until Winsock1.State = sckClosed
    Winsock1.Close
    DoEvents
  Loop

End Sub

Private Sub Winsock1_Connect()

On Error GoTo errhandler

  With Winsock1
' Set the remotehost property
  .RemoteHost = RemoteIP
' Set the remoteport property.
' This should be equal to the localhost property of the remote machine.
  .RemotePort = RemotePort
' The localport property cannot be changed,so check if it has already been set
    If .LocalPort = Empty Then
      .LocalPort = LocalPort
      .Bind .LocalPort
    End If
  End With
  
  Exit Sub

errhandler:
  MsgBox "Winsock failed to establish connection with remote server", vbCritical

End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'New_Text is the text that has just arrived from across the socket
Dim D$, CurrentTime As Date

  If UDP_Transmit Then Exit Sub
  
  Winsock1.GetData UDP_Packet_In
' Read next UDP frame and confirm "IWG1," at start and vbCrLf at end
  ReadNextFrameUDP
' Now have input UDP_Packet_In, decode it
  Decode_NGV UDP_Packet_In
' Check that time is changing
  CurrentTime = Date + Time
  If CurrentTime > LastTime Then UDPupdating = True Else UDPupdating = False
  LastTime = CurrentTime
  
' Code below here is form dependent
' Show nav statistics
  txtAvg(0).Text = Format(PitchAvg, "+#0.00;-#0.00")
  txtAvg(1).Text = Format(RollAvg, "+#0.00;-#0.00")
  txtAvg(2).Text = Format(ZpAvg, "+#0.00;-#0.00")
  txtAvg(3).Text = Format(OatAvg, "000.00")
  txtAvg(4).Text = Format(LatAvg, "+#0.000;-#0.000")
  txtAvg(5).Text = Format(LonAvg, "+##0.000;-##0.000")
  
  txtRMS(0).Text = Format(PitchRms, "00.00")
  txtRMS(1).Text = Format(RollRms, "00.00")
  txtRMS(2).Text = Format(ZpRms, "0.00")
  txtRMS(3).Text = Format(OatRms, "0.00")
  txtRMS(4).Text = Format(LatRms, "0.000")
  txtRMS(5).Text = Format(LonRms, "0.000")
  
' Show the new text
'  Text1.SelText = UDP_Packet_In
'  Frame1.Caption = Winsock1.RemoteHostIP
' Show the byte size of this transmission in the statusbar
  StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
' And update display
  UpdatePacketParameters
  If UDPupdating Then
    txtUpdating.BackColor = &HFF00&      'Green
  Else
    txtUpdating.BackColor = &HFF&        'Red
  End If
'  Text2.Text = UDP_Packet_In

'IWG1,20010920T151645,14.642,-96.4235,
'1234567890123456789012345678901234567890
  D$ = Left$(yyyymmddI$, 4) + "-" + Mid$(yyyymmddI$, 5, 2) + "-" + Right$(yyyymmddI$, 2)
  D$ = "Packet Parameters on " + D$ + " at " + fSecToTstringX$(UTsecI, True)
'  Frame(6).Caption = D$

End Sub

