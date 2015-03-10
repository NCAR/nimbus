VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "mswinsck.ocx"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Begin VB.Form frmUDP 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Winsock UDP"
   ClientHeight    =   7395
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7125
   Icon            =   "UDP_Tx.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7395
   ScaleWidth      =   7125
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtUpdating 
      Alignment       =   2  'Center
      BackColor       =   &H000000FF&
      Height          =   300
      Left            =   5460
      TabIndex        =   93
      Text            =   "Updating"
      Top             =   4410
      Width           =   1575
   End
   Begin VB.TextBox txtLocalPort 
      BackColor       =   &H00C0C0C0&
      Height          =   285
      Left            =   5460
      TabIndex        =   90
      Text            =   "58800"
      Top             =   4080
      Width           =   1575
   End
   Begin VB.Frame Frame2 
      Caption         =   "Transmitted IWG1 Packet"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1755
      Left            =   105
      TabIndex        =   88
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
         TabIndex        =   89
         Top             =   270
         Width           =   6690
      End
   End
   Begin VB.ComboBox cboRemoteIP 
      Height          =   315
      Left            =   5460
      TabIndex        =   87
      Text            =   "192.168.84.255"
      Top             =   3735
      Width           =   1590
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Clear"
      Height          =   390
      Left            =   3465
      TabIndex        =   86
      Top             =   4380
      Width           =   840
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   375
      Left            =   4395
      TabIndex        =   85
      Top             =   4380
      Width           =   870
   End
   Begin VB.Frame Frame3 
      Caption         =   "Nav Statistics"
      Height          =   1110
      Left            =   105
      TabIndex        =   73
      Top             =   3645
      Width           =   3330
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   765
         MultiLine       =   -1  'True
         TabIndex        =   84
         Top             =   405
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   2670
         MultiLine       =   -1  'True
         TabIndex        =   80
         Top             =   705
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   2670
         MultiLine       =   -1  'True
         TabIndex        =   79
         Top             =   405
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   78
         Top             =   705
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   1410
         MultiLine       =   -1  'True
         TabIndex        =   77
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   765
         MultiLine       =   -1  'True
         TabIndex        =   76
         Top             =   690
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   75
         Top             =   420
         Width           =   585
      End
      Begin VB.TextBox txtAvg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   1410
         MultiLine       =   -1  'True
         TabIndex        =   74
         Top             =   420
         Width           =   585
      End
      Begin VB.Label Label 
         Caption         =   "Pitch       Roll        pALT     OAT"
         Height          =   195
         Index           =   20
         Left            =   780
         TabIndex        =   83
         Top             =   180
         Width           =   2265
      End
      Begin VB.Label Label 
         Caption         =   "Std Dev"
         Height          =   270
         Index           =   7
         Left            =   105
         TabIndex        =   82
         Top             =   720
         Width           =   660
      End
      Begin VB.Label Label 
         Caption         =   "Average"
         Height          =   270
         Index           =   4
         Left            =   105
         TabIndex        =   81
         Top             =   405
         Width           =   660
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "Decoded Packet Parameters"
      Height          =   3555
      Index           =   6
      Left            =   105
      TabIndex        =   6
      Top             =   30
      Width           =   6960
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   39
         Top             =   2400
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   38
         Top             =   255
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   37
         Top             =   525
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   36
         Top             =   795
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   35
         Top             =   1065
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   34
         Top             =   1335
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   33
         Top             =   1605
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   32
         Top             =   1875
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   31
         Top             =   2145
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   30
         Top             =   2415
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   29
         Top             =   2685
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   28
         Top             =   2955
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   1395
         MultiLine       =   -1  'True
         TabIndex        =   27
         Top             =   3225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   26
         Top             =   210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   25
         Top             =   480
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   24
         Top             =   750
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   23
         Top             =   1020
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   22
         Top             =   1290
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   21
         Top             =   1560
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   20
         Top             =   1830
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   19
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   18
         Top             =   2670
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   17
         Top             =   2940
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   16
         Top             =   3210
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   15
         Top             =   225
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   14
         Top             =   495
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   13
         Top             =   765
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   12
         Top             =   1035
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   11
         Top             =   1305
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   10
         Top             =   1575
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   9
         Top             =   1845
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   8
         Top             =   2115
         Width           =   825
      End
      Begin VB.TextBox txtVar 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   6030
         MultiLine       =   -1  'True
         TabIndex        =   7
         Top             =   2385
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "Track Angle (deg)"
         Height          =   210
         Index           =   22
         Left            =   2325
         TabIndex        =   72
         Top             =   510
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Gnd Speed (m/s)"
         Height          =   195
         Index           =   21
         Left            =   135
         TabIndex        =   71
         Top             =   2130
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "UT (sec)"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   70
         Top             =   240
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "Latitude (deg)"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   69
         Top             =   540
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Longitude (deg)"
         Height          =   195
         Index           =   2
         Left            =   120
         TabIndex        =   68
         Top             =   810
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Zgps (m)"
         Height          =   195
         Index           =   3
         Left            =   135
         TabIndex        =   67
         Top             =   1080
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Attack Angle (deg)"
         Height          =   255
         Index           =   14
         Left            =   2370
         TabIndex        =   66
         Top             =   1890
         Width           =   1410
      End
      Begin VB.Label Label 
         Caption         =   "Pitch (deg)"
         Height          =   240
         Index           =   13
         Left            =   2355
         TabIndex        =   65
         Top             =   1065
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Heading (deg)"
         Height          =   255
         Index           =   12
         Left            =   2325
         TabIndex        =   64
         Top             =   255
         Width           =   1080
      End
      Begin VB.Label Label 
         Caption         =   "Total T (C)"
         Height          =   195
         Index           =   11
         Left            =   2355
         TabIndex        =   63
         Top             =   2970
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (C)"
         Height          =   255
         Index           =   8
         Left            =   2355
         TabIndex        =   62
         Top             =   2160
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Slip Angle (deg)"
         Height          =   255
         Index           =   5
         Left            =   2370
         TabIndex        =   61
         Top             =   1620
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Static P (hPa)"
         Height          =   255
         Index           =   9
         Left            =   2370
         TabIndex        =   60
         Top             =   3225
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Roll (deg)"
         Height          =   255
         Index           =   16
         Left            =   2355
         TabIndex        =   59
         Top             =   1335
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OAT (K)"
         Height          =   255
         Index           =   17
         Left            =   2355
         TabIndex        =   58
         Top             =   2445
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Dew Point (C)"
         Height          =   270
         Index           =   18
         Left            =   2355
         TabIndex        =   57
         Top             =   2700
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Drift Angle (deg)"
         Height          =   240
         Index           =   19
         Left            =   2340
         TabIndex        =   56
         Top             =   780
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Dir (deg)"
         Height          =   255
         Index           =   15
         Left            =   4695
         TabIndex        =   55
         Top             =   975
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Wind Speed (m/s)"
         Height          =   255
         Index           =   10
         Left            =   4665
         TabIndex        =   54
         Top             =   720
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Zwgs (m)"
         Height          =   255
         Index           =   6
         Left            =   135
         TabIndex        =   53
         Top             =   1350
         Width           =   900
      End
      Begin VB.Label Label 
         Caption         =   "Zp (feet)"
         Height          =   225
         Index           =   48
         Left            =   150
         TabIndex        =   52
         Top             =   1605
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Zr (feet)"
         Height          =   210
         Index           =   49
         Left            =   150
         TabIndex        =   51
         Top             =   1860
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Vertical Vel (m/s)"
         Height          =   195
         Index           =   55
         Left            =   120
         TabIndex        =   50
         Top             =   3225
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "TAS (m/s)"
         Height          =   180
         Index           =   56
         Left            =   135
         TabIndex        =   49
         Top             =   2400
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "IAS (m/s)"
         Height          =   195
         Index           =   57
         Left            =   165
         TabIndex        =   48
         Top             =   2655
         Width           =   960
      End
      Begin VB.Label Label 
         Caption         =   "Mach Number"
         Height          =   195
         Index           =   59
         Left            =   135
         TabIndex        =   47
         Top             =   2955
         Width           =   1230
      End
      Begin VB.Label Label 
         Caption         =   "Dynamic P (hPa)"
         Height          =   195
         Index           =   60
         Left            =   4650
         TabIndex        =   46
         Top             =   225
         Width           =   1395
      End
      Begin VB.Label Label 
         Caption         =   "Cabin P (hPa)"
         Height          =   195
         Index           =   61
         Left            =   4665
         TabIndex        =   45
         Top             =   495
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Wind Ver (m/s)"
         Height          =   195
         Index           =   62
         Left            =   4680
         TabIndex        =   44
         Top             =   1215
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "SZA (deg)"
         Height          =   195
         Index           =   63
         Left            =   4665
         TabIndex        =   43
         Top             =   1470
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Elev AC (deg)"
         Height          =   195
         Index           =   64
         Left            =   4680
         TabIndex        =   42
         Top             =   1710
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az Gnd (deg)"
         Height          =   195
         Index           =   65
         Left            =   4680
         TabIndex        =   41
         Top             =   1980
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Sun Az AC (deg)"
         Height          =   195
         Index           =   66
         Left            =   4680
         TabIndex        =   40
         Top             =   2235
         Width           =   1095
      End
   End
   Begin VB.CheckBox chkReopenOnEOF 
      Caption         =   "Reopen On EOF"
      Height          =   240
      Left            =   4530
      TabIndex        =   5
      Top             =   6840
      Value           =   1  'Checked
      Width           =   1635
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   405
      Left            =   6330
      TabIndex        =   4
      Top             =   6675
      Width           =   675
   End
   Begin VB.Timer Timer1 
      Interval        =   1000
      Left            =   6030
      Top             =   6705
   End
   Begin VB.CommandButton cmdOpenIWG1 
      Caption         =   "Open IWG1"
      Height          =   285
      Left            =   60
      TabIndex        =   3
      Top             =   6810
      Width           =   1200
   End
   Begin VB.TextBox txtFilename 
      Height          =   285
      Left            =   1290
      TabIndex        =   2
      Text            =   "IWG1.txt"
      Top             =   6810
      Width           =   3165
   End
   Begin ComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   285
      Left            =   0
      TabIndex        =   0
      Top             =   7110
      Width           =   7125
      _ExtentX        =   12568
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
            Object.Width           =   9922
            TextSave        =   ""
            Key             =   "DATA"
            Object.Tag             =   ""
            Object.ToolTipText     =   "The last data transfer through the modem"
         EndProperty
      EndProperty
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   5655
      Top             =   6720
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Local Port :"
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
      Left            =   3480
      TabIndex        =   92
      Top             =   4065
      Width           =   1770
   End
   Begin VB.Label Label1 
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Connect with IP :"
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
      Left            =   3480
      TabIndex        =   91
      Top             =   3750
      Width           =   1770
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "Click Open IWG1 button to start sending UDP data"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   161
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00008000&
      Height          =   210
      Left            =   90
      TabIndex        =   1
      Top             =   6570
      Width           =   3750
   End
End
Attribute VB_Name = "frmUDP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private IgnoreText As Boolean, UDPlu%, EOFflag As Boolean
Private UTsec&, HHMMSS$, yyyymmdd$


Sub UpdatePacketParmeters()
Dim D1$, i%, Out$
    
  D1$ = HHMMSS$
  Out$ = "Packet Parameters on " + D1$ + " at " + fSecToTstring$(UTsec, True)
  For i = 0 To 32
    If GotV(i) Then
      Select Case i
      Case 2, 3, 11
        txtVar(i).Text = Format(DatV(i), "###0.000")
      Case 0, 4, 5, 6, 7
        txtVar(i).Text = Format(DatV(i), "####0")
      Case Else
        txtVar(i).Text = Format(DatV(i), "###0.00")
      End Select
    Else
      txtVar(i).Text = ""
    End If
  Next i

End Sub

Private Sub cmdClear_Click()
On Error Resume Next
Text1 = ""
With Text2
   'Clear the text window and
   .Text = " "
   'return the focus
   .SetFocus
End With
End Sub


Private Sub cmdConnect_Click()
' Establish Winsock connection - required code
  LastTime = Date + Time
  LocalPort = Trim(txtLocalPort.Text)
  RemotePort = Trim(txtRemotePort.Text)
  RemoteIP = Trim(cboRemoteIP.Text)
  Winsock1_Connect
  
' Remaining code is form dependent
  Frame2.Caption = Winsock1.LocalIP
' Make sure that the user can't change the local port
  txtLocalPort.Locked = True
' Show the current status of the connection in the status bar
  StatusBar1.Panels(1).Text = "  Connected to " & Winsock1.RemoteHost & "  "

  Frame1.Enabled = True
  Frame2.Enabled = True
  Label4.Visible = True
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
'  End
'  frmMTPbin.cmdREF_Click (10)

End Sub

Private Sub cmdOpenIWG1_Click()
Dim Filename$

' Open file that is to be sent via UPD
  Filename$ = txtFilename.Text
  UDPlu = FreeFile
  Open Filename$ For Input As UDPlu
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
Show
'MsgBox "Winsock UDT Chat" & vbCrLf & "by Theo Kandiliotis (ionikh@hol.gr)" & vbCrLf & vbCrLf & "F1 for help.", vbInformation
'txtRemoteIP = Winsock1.LocalIP
With cboRemoteIP
.AddItem "128.117.84.255"
.AddItem "192.168.84.255"
.AddItem Winsock1.LocalIP
.ListIndex = 2
End With

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
  If EOFflag Then Close UDPlu:  Exit Sub    'Skip CPK
' And read it from NGV Data File
  UDP_Packet_Out = "IWG1,"
  X$ = ""
  Do
    UDP_Packet_Out = UDP_Packet_Out + Input(1, UDPlu)   'get a character
    If Len(UDP_Packet_Out) > 4 Then X$ = Right$(UDP_Packet_Out, 4)
  Loop Until X$ = "\r\n"  'vbCR+vbLF
  UDP_Packet_Out = Left$(UDP_Packet_Out, Len(UDP_Packet_Out) - 4) + vbCrLf
  Text2.Text = UDP_Packet_Out
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
Static Last_Line_Feed As Long
' The new line of text
Dim New_Line As String, Filename$

  Timer1.Enabled = False
  If UDPlu = 0 Then Exit Sub
  If EOFflag Then
  ' Check if file should be reopened on EOF
    If chkReopenOnEOF.Value = True Then
      Filename$ = txtFilename.Text
      UDPlu = FreeFile
      Open Filename$ For Input As UDPlu
      EOFflag = False
    End If
  End If
  ReadNextFrameFile

' Reset the position of the last line feed if the
' user has cleared the chat window
  If Trim(Text2) = vbNullString Then Last_Line_Feed = 0
' Save the position of the current linefeed
  Last_Line_Feed = Text2.SelStart
' Send the new text across the socket
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
  txtAvg(0).Text = Format(PitchAvg, "#0.00")
  txtAvg(1).Text = Format(RollAvg, "#0.00")
  txtAvg(2).Text = Format(ZpAvg, "#0.00")
  txtAvg(3).Text = Format(OatAvg, "000.00")
  txtRMS(0).Text = Format(PitchRms, "#0.00")
  txtRMS(1).Text = Format(RollRms, "#0.00")
  txtRMS(2).Text = Format(ZpRms, "#0.00")
  txtRMS(3).Text = Format(OatRms, "#0.00")
  
' Show the new text
  Text1.SelText = UDP_Packet_In
  Frame1.Caption = Winsock1.RemoteHostIP
' Show the byte size of this transmission in the statusbar
  StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
' And update display
  UpdatePacketParmeters
  If UDPupdating Then
    txtUpdating.BackColor = &HFF00&      'Green
  Else
    txtUpdating.BackColor = &HFF&        'Red
  End If
'  Text2.Text = UDP_Packet_In

'IWG1,20010920T151645,14.642,-96.4235,
'1234567890123456789012345678901234567890
  D$ = Left$(yyyymmdd$, 4) + "-" + Mid$(yyyymmdd$, 5, 2) + "-" + Right$(yyyymmdd$, 2)
  D$ = "Packet Parameters on " + D$ + " at " + fSecToTstring$(UTsec, True)
  Frame(6).Caption = D$

End Sub

