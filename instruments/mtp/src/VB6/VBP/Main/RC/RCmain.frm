VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form RCmain 
   Caption         =   "Retrieval Coefficient Calculation"
   ClientHeight    =   4695
   ClientLeft      =   165
   ClientTop       =   450
   ClientWidth     =   8700
   LinkTopic       =   "Form1"
   ScaleHeight     =   4695
   ScaleWidth      =   8700
   Begin VB.TextBox txtRCconfig 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4695
      Left            =   6660
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   112
      Top             =   0
      Width           =   2040
   End
   Begin VB.ComboBox cboMode 
      Height          =   315
      Left            =   3900
      TabIndex        =   110
      Text            =   "Process Single File"
      Top             =   3960
      Width           =   2085
   End
   Begin VB.TextBox txtNtemplates 
      Alignment       =   1  'Right Justify
      Height          =   300
      Left            =   6000
      MultiLine       =   -1  'True
      TabIndex        =   109
      Top             =   3975
      Width           =   510
   End
   Begin VB.CommandButton cmdReloadTemplates 
      Caption         =   "RAOB2 File"
      Height          =   435
      Left            =   0
      TabIndex        =   108
      ToolTipText     =   "Click to Reload/Update Template RAOB Files"
      Top             =   3870
      Width           =   960
   End
   Begin VB.TextBox txtRAOBpath 
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
      Left            =   990
      TabIndex        =   107
      Text            =   "C:\MTP\Data\ER2\TC4\RAOB\"
      Top             =   2880
      Width           =   4305
   End
   Begin VB.ComboBox cboRAOBfiles 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   330
      Left            =   990
      TabIndex        =   93
      Top             =   3945
      Width           =   2850
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   4665
      Top             =   3420
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   5655
      TabIndex        =   80
      Top             =   4305
      Width           =   855
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "&Start"
      Height          =   375
      Left            =   4740
      TabIndex        =   64
      Top             =   4320
      Width           =   855
   End
   Begin VB.TextBox txtProgress 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   6000
      MultiLine       =   -1  'True
      TabIndex        =   50
      Top             =   3495
      Width           =   495
   End
   Begin VB.TextBox txtNraob 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   6000
      MultiLine       =   -1  'True
      TabIndex        =   47
      Top             =   2865
      Width           =   495
   End
   Begin VB.TextBox txtET 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   6000
      MultiLine       =   -1  'True
      TabIndex        =   46
      Top             =   3180
      Width           =   495
   End
   Begin VB.TextBox txtRCfile 
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
      Left            =   990
      TabIndex        =   5
      Top             =   3240
      Width           =   3660
   End
   Begin VB.TextBox txtROfile 
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
      Left            =   990
      TabIndex        =   1
      Top             =   3600
      Width           =   2850
   End
   Begin ComctlLib.ProgressBar ProgressBar1 
      Height          =   240
      Left            =   990
      TabIndex        =   0
      Top             =   4410
      Width           =   3645
      _ExtentX        =   6429
      _ExtentY        =   423
      _Version        =   327682
      Appearance      =   1
      Max             =   101
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   2775
      Left            =   30
      TabIndex        =   6
      Top             =   0
      Width           =   6525
      _ExtentX        =   11509
      _ExtentY        =   4895
      _Version        =   393216
      TabHeight       =   520
      TabCaption(0)   =   "&Setup"
      TabPicture(0)   =   "RCmain.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label(16)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label(1)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label(13)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label(15)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Label(17)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Label(14)"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Label(6)"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "Label(0)"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "Label(7)"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "Label(8)"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "Label(9)"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "Label(12)"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "Label(18)"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "Label(25)"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "Label(26)"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "cmdTBcorr"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "Drive1"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "cmdTransparency"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "chkRegressionOnly"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "cboPlatforms"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "cmbMonth(0)"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "cmbMonth(1)"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "txtRegion"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "txtRAOBmin"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "chkConvert2to3"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "cboSU"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "txtRecordStep"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "txtLR1"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "txtRCformat"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "cboMissions"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).Control(30)=   "txtRegionNumber"
      Tab(0).Control(30).Enabled=   0   'False
      Tab(0).Control(31)=   "txtZb"
      Tab(0).Control(31).Enabled=   0   'False
      Tab(0).Control(32)=   "txtLR2"
      Tab(0).Control(32).Enabled=   0   'False
      Tab(0).Control(33)=   "txtRegionNumberRC"
      Tab(0).Control(33).Enabled=   0   'False
      Tab(0).Control(34)=   "Frame"
      Tab(0).Control(34).Enabled=   0   'False
      Tab(0).Control(35)=   "cmdGetLR"
      Tab(0).Control(35).Enabled=   0   'False
      Tab(0).Control(36)=   "cmdCheckConfig"
      Tab(0).Control(36).Enabled=   0   'False
      Tab(0).Control(37)=   "txtBusy"
      Tab(0).Control(37).Enabled=   0   'False
      Tab(0).ControlCount=   38
      TabCaption(1)   =   "&Frequencies/Angles"
      TabPicture(1)   =   "RCmain.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label(11)"
      Tab(1).Control(1)=   "Label(27)"
      Tab(1).Control(2)=   "Frame4"
      Tab(1).Control(3)=   "Frame2"
      Tab(1).Control(4)=   "Frame1"
      Tab(1).Control(5)=   "picBandpass"
      Tab(1).Control(6)=   "txtLSBpercent"
      Tab(1).ControlCount=   7
      TabCaption(2)   =   "&Altitudes"
      TabPicture(2)   =   "RCmain.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "txtRAOBbias"
      Tab(2).Control(1)=   "cmdRCread"
      Tab(2).Control(2)=   "cmdLoadRaobFiles(1)"
      Tab(2).Control(3)=   "cmdLoadRaobFiles(0)"
      Tab(2).Control(4)=   "txtExcessTamplitude"
      Tab(2).Control(5)=   "chkExcessGndT"
      Tab(2).Control(6)=   "chkGndRet"
      Tab(2).Control(7)=   "cmbDz"
      Tab(2).Control(8)=   "Frame3"
      Tab(2).Control(9)=   "cmbESV"
      Tab(2).Control(10)=   "Label(29)"
      Tab(2).Control(11)=   "Label(28)"
      Tab(2).Control(12)=   "Label(10)"
      Tab(2).Control(13)=   "Label(4)"
      Tab(2).Control(14)=   "Label(24)"
      Tab(2).ControlCount=   15
      Begin VB.TextBox txtRAOBbias 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   102
         Text            =   "RCmain.frx":0054
         ToolTipText     =   "Bias that is added to read RAOBs "
         Top             =   1740
         Width           =   495
      End
      Begin VB.TextBox txtLSBpercent 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   100
         Text            =   "RCmain.frx":0056
         Top             =   2430
         Width           =   375
      End
      Begin VB.CommandButton cmdRCread 
         Caption         =   "Read RC File"
         Height          =   525
         Left            =   -69330
         TabIndex        =   98
         Top             =   2070
         Width           =   705
      End
      Begin VB.PictureBox picBandpass 
         BackColor       =   &H00FFFFFF&
         Height          =   1245
         Left            =   -74880
         ScaleHeight     =   1
         ScaleLeft       =   200
         ScaleMode       =   0  'User
         ScaleTop        =   1
         ScaleWidth      =   200
         TabIndex        =   96
         Top             =   1200
         Width           =   1935
      End
      Begin VB.TextBox txtBusy 
         BackColor       =   &H0000FF00&
         Height          =   345
         Left            =   6120
         TabIndex        =   92
         ToolTipText     =   "Busy if Red"
         Top             =   780
         Width           =   285
      End
      Begin VB.CommandButton cmdCheckConfig 
         Caption         =   "->"
         Height          =   345
         Left            =   6090
         TabIndex        =   91
         ToolTipText     =   "Show RC configuration file"
         Top             =   390
         Width           =   285
      End
      Begin VB.CommandButton cmdLoadRaobFiles 
         Caption         =   "Transfer"
         Height          =   315
         Index           =   1
         Left            =   -71550
         TabIndex        =   90
         Top             =   1050
         Width           =   975
      End
      Begin VB.CommandButton cmdLoadRaobFiles 
         Caption         =   "Load Files"
         Height          =   315
         Index           =   0
         Left            =   -72600
         TabIndex        =   89
         Top             =   1050
         Width           =   975
      End
      Begin VB.CommandButton cmdGetLR 
         Caption         =   "Get LR"
         Height          =   315
         Left            =   5730
         TabIndex        =   88
         Top             =   1740
         Width           =   675
      End
      Begin VB.Frame Frame 
         Caption         =   "Disabled Channels"
         Height          =   525
         Left            =   4050
         TabIndex        =   84
         Top             =   1170
         Width           =   2385
         Begin VB.CheckBox chkDisabledChannels 
            Caption         =   "3"
            Height          =   255
            Index           =   2
            Left            =   1650
            TabIndex        =   87
            Top             =   210
            Width           =   435
         End
         Begin VB.CheckBox chkDisabledChannels 
            Caption         =   "2"
            Height          =   255
            Index           =   1
            Left            =   870
            TabIndex        =   86
            Top             =   210
            Width           =   435
         End
         Begin VB.CheckBox chkDisabledChannels 
            Caption         =   "1"
            Height          =   255
            Index           =   0
            Left            =   150
            TabIndex        =   85
            Top             =   210
            Width           =   435
         End
      End
      Begin VB.TextBox txtRegionNumberRC 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1590
         MultiLine       =   -1  'True
         TabIndex        =   82
         Text            =   "RCmain.frx":005C
         Top             =   1200
         Width           =   255
      End
      Begin VB.TextBox txtLR2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   4890
         MultiLine       =   -1  'True
         TabIndex        =   79
         Text            =   "RCmain.frx":0060
         Top             =   2370
         Width           =   435
      End
      Begin VB.TextBox txtZb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5970
         MultiLine       =   -1  'True
         TabIndex        =   78
         Text            =   "RCmain.frx":0066
         Top             =   2070
         Width           =   435
      End
      Begin VB.TextBox txtExcessTamplitude 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   76
         Text            =   "RCmain.frx":006D
         Top             =   2040
         Width           =   495
      End
      Begin VB.CheckBox chkExcessGndT 
         Caption         =   "Include Excess Ground T of "
         Height          =   255
         Left            =   -72600
         TabIndex        =   75
         Top             =   2040
         Width           =   2295
      End
      Begin VB.TextBox txtRegionNumber 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   1320
         MultiLine       =   -1  'True
         TabIndex        =   74
         Text            =   "RCmain.frx":006F
         Top             =   1200
         Width           =   255
      End
      Begin VB.ComboBox cboMissions 
         Height          =   315
         Left            =   2670
         TabIndex        =   73
         Top             =   1200
         Width           =   1335
      End
      Begin VB.TextBox txtRCformat 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3270
         MultiLine       =   -1  'True
         TabIndex        =   71
         Text            =   "RCmain.frx":0073
         Top             =   2100
         Width           =   495
      End
      Begin VB.TextBox txtLR1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   4890
         MultiLine       =   -1  'True
         TabIndex        =   69
         Text            =   "RCmain.frx":0077
         Top             =   2040
         Width           =   435
      End
      Begin VB.TextBox txtRecordStep 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5970
         MultiLine       =   -1  'True
         TabIndex        =   67
         Text            =   "RCmain.frx":007D
         Top             =   2400
         Width           =   435
      End
      Begin VB.ComboBox cboSU 
         Height          =   315
         Left            =   3000
         TabIndex        =   60
         Top             =   840
         Width           =   1005
      End
      Begin VB.CheckBox chkConvert2to3 
         Caption         =   "Convert Format 2 to 3"
         Height          =   255
         Left            =   2040
         TabIndex        =   59
         Top             =   2460
         Width           =   1845
      End
      Begin VB.TextBox txtRAOBmin 
         BackColor       =   &H00FFFFFF&
         Height          =   285
         Left            =   1680
         TabIndex        =   57
         Top             =   2100
         Width           =   735
      End
      Begin VB.TextBox txtRegion 
         Height          =   285
         Left            =   780
         TabIndex        =   55
         Text            =   "AA"
         Top             =   1215
         Width           =   465
      End
      Begin VB.ComboBox cmbMonth 
         Height          =   315
         Index           =   1
         Left            =   3270
         TabIndex        =   52
         Top             =   1740
         Width           =   735
      End
      Begin VB.ComboBox cmbMonth 
         Height          =   315
         Index           =   0
         Left            =   1680
         TabIndex        =   51
         Top             =   1740
         Width           =   735
      End
      Begin VB.CheckBox chkGndRet 
         Caption         =   "Ground retrieval"
         Height          =   255
         Left            =   -72600
         TabIndex        =   45
         Top             =   2400
         Value           =   1  'Checked
         Width           =   1455
      End
      Begin VB.ComboBox cmbDz 
         Height          =   315
         Left            =   -72600
         TabIndex        =   40
         Top             =   600
         Width           =   1455
      End
      Begin VB.Frame Frame3 
         Caption         =   "Retrieval Altitudes [km]"
         Height          =   2295
         Left            =   -74880
         TabIndex        =   31
         Top             =   360
         Width           =   2175
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Setup"
            Height          =   330
            Index           =   4
            Left            =   1200
            TabIndex        =   105
            Top             =   1575
            Width           =   855
         End
         Begin VB.ListBox lstPalt 
            Height          =   1620
            ItemData        =   "RCmain.frx":0081
            Left            =   120
            List            =   "RCmain.frx":0083
            TabIndex        =   38
            Top             =   600
            Width           =   975
         End
         Begin VB.TextBox txtPalt 
            Height          =   285
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   37
            Top             =   240
            Width           =   975
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Add"
            Height          =   330
            Index           =   0
            Left            =   1200
            TabIndex        =   36
            Top             =   240
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Remove"
            Height          =   345
            Index           =   1
            Left            =   1200
            TabIndex        =   35
            Top             =   570
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Clear"
            Height          =   330
            Index           =   2
            Left            =   1200
            TabIndex        =   34
            Top             =   915
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Default"
            Height          =   330
            Index           =   3
            Left            =   1200
            TabIndex        =   33
            Top             =   1245
            Width           =   855
         End
         Begin VB.TextBox txtNFL 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   32
            Top             =   1920
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "NFL"
            Height          =   255
            Index           =   5
            Left            =   1200
            TabIndex        =   39
            Top             =   1920
            Width           =   375
         End
      End
      Begin VB.ComboBox cmbESV 
         Height          =   315
         Left            =   -70200
         TabIndex        =   30
         Top             =   600
         Width           =   1575
      End
      Begin VB.ComboBox cboPlatforms 
         Height          =   315
         Left            =   780
         TabIndex        =   29
         Top             =   840
         Width           =   1065
      End
      Begin VB.CheckBox chkRegressionOnly 
         Caption         =   "Do Regression ONLY"
         Height          =   255
         Left            =   120
         TabIndex        =   28
         Top             =   2460
         Width           =   1860
      End
      Begin VB.CommandButton cmdTransparency 
         Caption         =   "Calculate Transparency"
         Height          =   375
         Left            =   4050
         TabIndex        =   27
         Top             =   780
         Width           =   1995
      End
      Begin VB.DriveListBox Drive1 
         Height          =   315
         Left            =   780
         TabIndex        =   26
         Top             =   480
         Width           =   3225
      End
      Begin VB.CommandButton cmdTBcorr 
         Caption         =   "Add Sensitivity Matrix"
         Height          =   375
         Left            =   4050
         TabIndex        =   25
         Top             =   390
         Width           =   1995
      End
      Begin VB.Frame Frame1 
         Caption         =   "IF Offset and Weight"
         Height          =   825
         Left            =   -74880
         TabIndex        =   23
         Top             =   360
         Width           =   1935
         Begin VB.ListBox lstSU 
            Height          =   450
            ItemData        =   "RCmain.frx":0085
            Left            =   120
            List            =   "RCmain.frx":0087
            TabIndex        =   24
            Top             =   240
            Width           =   1695
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "LO Frequencies"
         Height          =   2055
         Left            =   -72840
         TabIndex        =   15
         Top             =   360
         Width           =   2175
         Begin VB.TextBox txtLO 
            Height          =   285
            Left            =   120
            TabIndex        =   63
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox txtNlo 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   21
            Top             =   1710
            Width           =   375
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Setup"
            Height          =   375
            Index           =   3
            Left            =   1200
            TabIndex        =   20
            Top             =   1305
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Clear"
            Height          =   375
            Index           =   2
            Left            =   1200
            TabIndex        =   19
            Top             =   960
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Remove"
            Height          =   375
            Index           =   1
            Left            =   1200
            TabIndex        =   18
            Top             =   600
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Add"
            Height          =   375
            Index           =   0
            Left            =   1200
            TabIndex        =   17
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstLO 
            Height          =   1230
            ItemData        =   "RCmain.frx":0089
            Left            =   120
            List            =   "RCmain.frx":008B
            TabIndex        =   16
            Top             =   600
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Nlo"
            Height          =   255
            Index           =   3
            Left            =   1230
            TabIndex        =   22
            Top             =   1710
            Width           =   255
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "EL Angles"
         Height          =   2295
         Left            =   -70560
         TabIndex        =   7
         Top             =   360
         Width           =   1935
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Setup"
            Height          =   375
            Index           =   4
            Left            =   960
            TabIndex        =   104
            Top             =   1590
            Width           =   855
         End
         Begin VB.TextBox txtEl 
            Height          =   285
            Left            =   120
            TabIndex        =   62
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox txtNel 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   13
            Top             =   1980
            Width           =   375
         End
         Begin VB.ListBox lstEL 
            Height          =   1620
            ItemData        =   "RCmain.frx":008D
            Left            =   120
            List            =   "RCmain.frx":008F
            TabIndex        =   12
            Top             =   600
            Width           =   735
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Add"
            Height          =   375
            Index           =   0
            Left            =   960
            TabIndex        =   11
            Top             =   150
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Remove"
            Height          =   375
            Index           =   1
            Left            =   960
            TabIndex        =   10
            Top             =   510
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Clear"
            Height          =   375
            Index           =   2
            Left            =   960
            TabIndex        =   9
            Top             =   870
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Default"
            Height          =   375
            Index           =   3
            Left            =   960
            TabIndex        =   8
            Top             =   1215
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Nel"
            Height          =   255
            Index           =   2
            Left            =   960
            TabIndex        =   14
            Top             =   1980
            Width           =   375
         End
      End
      Begin VB.Label Label 
         Caption         =   "K"
         Height          =   165
         Index           =   29
         Left            =   -69630
         TabIndex        =   103
         Top             =   1770
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Bias"
         Height          =   255
         Index           =   28
         Left            =   -71190
         TabIndex        =   101
         Top             =   1740
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "CH1 LSB RF Loss %"
         Height          =   255
         Index           =   27
         Left            =   -72810
         TabIndex        =   99
         Top             =   2460
         Width           =   1545
      End
      Begin VB.Label Label 
         Caption         =   "IF Frequency (MHz)"
         Height          =   255
         Index           =   11
         Left            =   -74610
         TabIndex        =   97
         Top             =   2460
         Width           =   1485
      End
      Begin VB.Label Label 
         Caption         =   "LR2(K/km)"
         Height          =   195
         Index           =   26
         Left            =   4050
         TabIndex        =   95
         Top             =   2430
         Width           =   795
      End
      Begin VB.Label Label 
         Caption         =   "LR1(K/km)"
         Height          =   195
         Index           =   25
         Left            =   4050
         TabIndex        =   94
         Top             =   2100
         Width           =   795
      End
      Begin VB.Label Label 
         Caption         =   "Obs RC"
         Height          =   195
         Index           =   18
         Left            =   1290
         TabIndex        =   83
         Top             =   1500
         Width           =   585
      End
      Begin VB.Label Label 
         Caption         =   "Zb (km)"
         Height          =   195
         Index           =   12
         Left            =   5400
         TabIndex        =   81
         Top             =   2100
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "K"
         Height          =   165
         Index           =   10
         Left            =   -69600
         TabIndex        =   77
         Top             =   2040
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "Mission"
         Height          =   255
         Index           =   9
         Left            =   2100
         TabIndex        =   72
         Top             =   1200
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "RC Format"
         Height          =   255
         Index           =   8
         Left            =   2490
         TabIndex        =   70
         Top             =   2130
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "Extension LR [K/km]"
         Height          =   255
         Index           =   7
         Left            =   4050
         TabIndex        =   68
         Top             =   1800
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "Step"
         Height          =   225
         Index           =   0
         Left            =   5370
         TabIndex        =   66
         Top             =   2430
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Sensor Unit"
         Height          =   255
         Index           =   6
         Left            =   2100
         TabIndex        =   61
         Top             =   840
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Pmin [mb]"
         Height          =   255
         Index           =   14
         Left            =   120
         TabIndex        =   58
         Top             =   2100
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Region"
         Height          =   255
         Index           =   17
         Left            =   120
         TabIndex        =   56
         Top             =   1200
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "to"
         Height          =   255
         Index           =   15
         Left            =   2760
         TabIndex        =   54
         Top             =   1770
         Width           =   135
      End
      Begin VB.Label Label 
         Caption         =   "Monthly Stratification"
         Height          =   375
         Index           =   13
         Left            =   120
         TabIndex        =   53
         ToolTipText     =   "Used only to create filename"
         Top             =   1740
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "Altitude Offsets [km]"
         Height          =   255
         Index           =   4
         Left            =   -72600
         TabIndex        =   44
         Top             =   360
         Width           =   1455
      End
      Begin VB.Label Label 
         Caption         =   "Observable Errors [K]"
         Height          =   255
         Index           =   24
         Left            =   -70200
         TabIndex        =   43
         Top             =   360
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "Platform"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   42
         Top             =   840
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Drive"
         Height          =   255
         Index           =   16
         Left            =   120
         TabIndex        =   41
         Top             =   480
         Width           =   585
      End
   End
   Begin VB.Label Label 
      Caption         =   "Mode"
      Height          =   255
      Index           =   20
      Left            =   3930
      TabIndex        =   111
      Top             =   3660
      Width           =   705
   End
   Begin VB.Label Label 
      Caption         =   "RAOB Path"
      Height          =   255
      Index           =   30
      Left            =   30
      TabIndex        =   106
      Top             =   2955
      Width           =   930
   End
   Begin VB.Label lblProgress 
      Caption         =   "RAOBs"
      Height          =   285
      Left            =   5415
      TabIndex        =   65
      Top             =   3525
      Width           =   585
   End
   Begin VB.Label Label 
      Caption         =   "Current"
      Height          =   255
      Index           =   23
      Left            =   5430
      TabIndex        =   49
      Top             =   2910
      Width           =   540
   End
   Begin VB.Label lblET 
      Caption         =   "Elapsed Time [m]"
      Height          =   255
      Left            =   4680
      TabIndex        =   48
      Top             =   3210
      Width           =   1290
   End
   Begin VB.Label Label 
      Caption         =   "RC File"
      Height          =   255
      Index           =   19
      Left            =   30
      TabIndex        =   4
      Top             =   3270
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Progress"
      Height          =   225
      Index           =   22
      Left            =   30
      TabIndex        =   3
      Top             =   4425
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "OBS File"
      Height          =   255
      Index           =   21
      Left            =   30
      TabIndex        =   2
      Top             =   3600
      Width           =   795
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileSelectCFG 
         Caption         =   "&Select CFG"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
End
Attribute VB_Name = "RCmain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim FileNames$, seconds!, ROfile$, DATfile$, RAOBpath$, StartTime$, EndTime$

Sub CalculateSmatrix()
Dim z0!, i%, j%, k%, L%, M%, iRC%, RCpath$, RCfile$, lu%, filen!, bpv!, site$, Filename$
Dim A$, X!(1 To 3), y!(1 To 3), Coeff!(1 To 3), mm$, Ext$, Tref!(1 To 33)
Dim TAref!(1 To 3, 1 To 10), TAp!(1 To 3, 1 To 10), TAm!(1 To 3, 1 To 10), TAz!(1 To 3, 1 To 10)
Dim ob!(1 To 30), Orms!(1 To 30), RAOBlu%, Tbias!
' Smatrix calculates sensitivity of brightness temperatures to a bias (Tbias)
' Linear and quadratic terms are used
  Tbias = 20#
  For M = 1 To lstPalt.ListCount  'Do Retrieval altitudes
    z0 = Val(lstPalt.List(M - 1))
    iRC = 0
  ' Get Archive Average Temperature Profile and Observables for this RC set and FL
    RCpath$ = txtRAOBpath.Text + "RC\"
    Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumberRC.Text)))
    mm$ = Hex$(Month("1999 " + cmbMonth(0).Text)) + Hex$(Month("1999 " + cmbMonth(1).Text))
    RCfile$ = Left$(AC$, 1) + "RC" + Region$ + mm$ + "." + Format$(z0 * 100, "0000")
    RCfilename$ = RCpath$ + RCfile$
    
    Call ReadArcAvg_OBS_ATP(RCfilename$, Orms!(), ob(), Nobs%, ZPraob(), PZraob!(), TZraob!(), ZGraob!(), Nret%)
    For i = 1 To Nret: Tref(i) = TZraob(i): Next i
    Call MapOBtoTA(0, TAref(), ob!(), Channels%, Nel%)
    Nlevr = Nret
  ' Calculate "zero" TAs
    For k = 1 To Nlevr: TZraob(k) = Tref(k): Next k
    Call DoRetObsCalculationTBerr(z0, TAz!())
    
  ' Calculate "minus" TAs
    For k = 1 To Nlevr: TZraob(k) = Tref(k) - Tbias: Next k
    Call DoRetObsCalculationTBerr(z0, TAm!())
    
  ' Calculate "plus" TAs
    For k = 1 To Nlevr: TZraob(k) = Tref(k) + Tbias: Next k
    Call DoRetObsCalculationTBerr(z0, TAp!())
    
    'Debug.Print z0; Tref(16); TAm(1, 6) - Tref(16) + Tbias; TAz(2, 6) - Tref(16); TAp(3, 6) - Tref(16) - Tbias
    
' Now calculate TA changes
    X(1) = -Tbias: X(2) = 0: X(3) = Tbias: y(2) = 0
    For i = 1 To Channels
      For j = 1 To Nel
        y(1) = TAm(i, j) - TAz(i, j) + Tbias
        y(3) = TAp(i, j) - TAz(i, j) - Tbias
        Call Qfit(X!(), y!(), 3, Coeff!(), 3)
        Smatrix(0, M, i, j, 1) = Coeff(2)
        Smatrix(0, M, i, j, 2) = Coeff(3)
        'Debug.Print Coeff(1); Coeff(2); Coeff(3);
      Next j
    Next i
    'Debug.Print ""
  Next M
  
  For i = 1 To Nel: El(i) = lstEL.List(i - 1):  Next i

End Sub

Private Sub cmdReadFLTINFO_Click()
Dim FLTINFOfile$, i%
Static SAVEyyyymmdd$

  FLTINFOfile$ = "C:\MTP\FLTINFO.REF"
  REFreadHEADER FLTINFOfile$

  SAVEyyyymmdd$ = YYYYMMDD$
  For i = 0 To cboPlatforms.ListCount - 1
    If cboPlatforms.List(i) = Mid$(Rdir$, 2, Len(Rdir$) - 2) Then cboPlatforms.ListIndex = i: Exit For
  Next i
  AC$ = Mid$(Rdir$, 2, 2)
  Root$ = Drive$ + Rdir$
  yymmdd$ = Right$(YYYYMMDD$, 6)
  Path$ = Root$ + Mission$ + "\" + YYYYMMDD$ + "\"
  txtRCfile.Text = Root$ + Mission$ + "\RC\"
  For i = 0 To cboMissions.ListCount
    If Trim(cboMissions.List(i)) = Mission$ Then cboMissions.ListIndex = i: Exit For
  Next i
  Call cmdPalt_Click(4)
  UpdateForm
  
End Sub

' 20000216
' Fix RC code to get observables in "natural" order; that is, for each frequency
' go through elevation angles 1 through 5 followed by 7 through 10.
' Best approach is to reorder the observables and retrievables in the .obs files
' and then calculate new RCs. Be sure to reorder weights in RCmain!!!!

Sub CalcAvgAtFL(AvgT!(), RmsT!(), RAOBlu%, RecordStep%)
Dim n%, i%, j%, T1sum!(1 To 20), T2sum!(1 To 20), Tj!, Arg!, IKT&, Cycle&
Dim Ncold%(1 To 20), Nwarm%(1 To 20), Tavg!, TjSum!, TwSum!, TcSum!

  For i = 1 To NFL: Zr(i) = RCmain.lstPalt.List(i - 1): Next i

  For j = 1 To NFL
    T1sum(j) = 0#: T2sum(j) = 0#: AvgT(j) = 0#
  Next j


  Cycle = 0     'Location in RAOB file
  IKT = 0       'Number of RAOBs actually used

Do
  Call GetValidRAOB(RAOBlu, IKT, Cycle, RecordStep)
  If EOF(RAOBlu%) Then Exit Do

  IKT = IKT + 1 'Increment only if RAOB read
  
  For j = 1 To NFL
    Tj = fLinterp(ZPraob(), TZraob(), Nlevr, Zr(j))
    If j = 1 Then Debug.Print j; Format(Tj, "000.00"); " "; Format(IKT, "000"); " "; Format(Cycle, "000")
    T1sum(j) = T1sum(j) + Tj
    T2sum(j) = T2sum(j) + Tj ^ 2     'Sum square of temperature differences
  Next j
  
Loop Until EOF(RAOBlu) Or IKT > NraobMax

' Take average and calculate T statistics
  For j = 1 To NFL
    AvgT(j) = T1sum(j) / IKT  'Calculate average
    Arg = T2sum(j) - (T1sum(j) ^ 2) / IKT
    If Arg >= 0# And IKT > 1 Then
      RmsT(j) = Sqr(Arg / (IKT - 1))  'Calculate RMS
    Else
      RmsT(j) = 0#
    End If
    'If j = 1 Then Debug.Print Zr(j); AvgT(j); RmsT(j); T2sum(j); T1sum(j); (T1sum(j) ^ 2)
  Next j

For i = 1 To NFL
  Ncold(i) = 0: Nwarm(i) = 0: IKT = 0: Tavg = AvgT(i): TjSum = 0: Cycle = 0
  TwSum = 0#: TcSum = 0#
  Call RAOBread(RAOBlu, 1)
  Do
    Call GetValidRAOB(RAOBlu, IKT, Cycle, RecordStep)  'Increments "Cycle"
    If EOF(RAOBlu%) Then Exit Do
    IKT = IKT + 1
    Tj = fLinterp(ZPraob(), TZraob(), Nlevr, Zr(i))
    If Tj < AvgT(i) Then Ncold(i) = Ncold(i) + 1: TcSum = TcSum + Tj   'Cold RAOBs
    If Tj >= AvgT(i) Then Nwarm(i) = Nwarm(i) + 1: TwSum = TwSum + Tj  'Warm RAOBs
    TjSum = TjSum + Tj
  Loop Until EOF(RAOBlu)
  'Debug.Print Zr(i); Format(AvgT(i), "000.00"); "  "; Format(TcSum / Ncold(i), "000.00"); "  "; Format(TwSum / Nwarm(i), "000.00"); "  "; Format(TjSum / IKT, "000.00"); Ncold(i); Nwarm(i); Ncold(i) + Nwarm(i); IKT; Cycle
Next i

End Sub

Sub CalcInit()
Dim i%, j%, dR!
' xxx mjm 2000123 Put elevation angles in scan order (ie don't reverse negative Scan Angles)
' Code below assumes that lstEL entries are in sorted zenith to nadir order
  j = 0
  Select Case RCformatUsed
  Case 2
    For i = 0 To lstEL.ListCount - 1
      If Val(lstEL.List(i)) <> 0 Then 'Skip horizon scan angle
        j = j + 1
        El(j) = Val(lstEL.List(i))
      End If
    Next i
    
  Case 3
    For i = 0 To lstEL.ListCount - 1
      El(i + 1) = Val(lstEL.List(i))
    Next i
  
  End Select
' Code below is reversed order
'' Get Postive scan angle from highest to lowest
'  i = 0
'  Do: i = i + 1
'    El(i) = lstEL.List(i - 1)
'  Loop Until lstEL.List(i) <= 0#
'' Get Negative Scan angles from lowest to highest
'  j = lstEL.ListCount
'  Do
'    i = i + 1
'    j = j - 1
'    El(i) = lstEL.List(j)
'  Loop Until lstEL.List(j - 1) >= 0#

' EL now has all scan angle except the horizon view

' Calculate the Nif x 2 Side Band frequencies
' First Nif are for LSB from Low to High frequency in RF
' Second Nif are for USB from Low to High frequency in RF
For i = 1 To Nlo
' Calculate the Nif x 2 Side Band frequencies
  For j = 1 To Nif
      LSBUSB(i, j) = -IFoff(i, Nif + 1 - j)           'LSB offsets - Low to High
      LSBUSB(i, Nif + j) = IFoff(i, j)                'USB offsets - Low to High
  Next j
' Correct for RF waveguide slope across Ch1 LSB if Ch1 LO = 55.51 GHz
' Use linear correction: dF is IF passband with normalized response R=1
' Therefore, Full area = dF*R = dF
'            Sub  area = dF*dR/2
' %Area = Sub area / Full area *100 = dR*50
' So dR = %Area/50
' Modified response i: R = 1 -(Nif-j)*dR = 1 - (Nif -j)*(%Area/50)
' If j=Nif, R=1: if j=1, R=1 -(Nif-1)*dR
  For j = 1 To Nif
'    If chkFixChnRF(i - 1).value = 1 Then
      'BandWt(i, j) = IFwt(i, Nif - j + 1) * (1 + CHnLSBloss(i) / 100) '(1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
      If CHnLSBloss(i) >= 0 Then
        BandWt(i, j) = IFwt(i, Nif - j + 1) * (1 - (Nif - j + 1) * CHnLSBloss(i) / 50#)
      Else
        BandWt(i, j) = IFwt(i, Nif - j + 1) * (-(Nif - j + 1) * CHnLSBloss(i) / 50#)
      End If
      'Debug.Print i; j; IFwt(i, Nif - j + 1); IFwt(i, Nif - j + 1) * (1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif); (1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif); (-(Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
'    Else
'      BandWt(i, j) = IFwt(i, Nif - j + 1)
'    End If
    BandWt(i, Nif + j) = IFwt(i, j)
  Next j
  
  BandSum(i) = 0#: BandSumLSB(i) = 0#: BandSumUSB(i) = 0#
  For j = 1 To Nif
    BandSumLSB(i) = BandSumLSB(i) + BandWt(i, j)
    BandSumUSB(i) = BandSumUSB(i) + BandWt(i, j + Nif)
  Next j
  BandSum(i) = BandSumLSB(i) + BandSumUSB(i)
Next i

End Sub

Public Sub DGAUSSJ(A#(), n%, NP%, b#(), M%, MP%, Singular As Boolean)
' Linear equation solution by Gauss-Jordan elimination, equation (2.1.1)
' of "Numerical Recipes". A is an input matrix of N by N elements, stored
' in an array of physical dimensions NP by NP. B is an input matrix of
' N by M containing the M right-hand side vectors, stored in an array of
' physical dimensions NP by MP. On output, A is replaced by its matrix
' inverse, and B is replaced by the corresponding set of solution vectors.
' Double-precision version.
'
Dim Ipivot%(1024), IndexR%(1024), IndexC%(1024)
Dim i%, j%, k%, L%, ll%, iRow%, iCol%
Dim BIG#, DUM#, PIVINV#

  Singular = False

  For j = 1 To n: Ipivot(j) = 0: Next j
  For i = 1 To n
    BIG = 0#
    For j = 1 To n
      If (Ipivot(j) <> 1) Then
        For k = 1 To n
          Select Case Ipivot(k)
          Case Is = 0
            Select Case A(i, j) 'Way to do DABS function
            Case Is >= 0
              If A(j, k) >= BIG Then BIG = A(j, k)
            Case Is < 0
              If A(j, k) < -BIG Then BIG = -A(j, k)
            End Select
            iRow = j: iCol = k
          Case Is > 1: Singular = True: Exit Sub
          End Select
        Next k
      End If
    Next j
    
    Ipivot(iCol) = Ipivot(iCol) + 1
    If (iRow <> iCol) Then
      For L = 1 To n
        DUM = A(iRow, L)
        A(iRow, L) = A(iCol, L)
        A(iCol, L) = DUM
      Next L
      For L = 1 To M
        DUM = b(iRow, L)
        b(iRow, L) = b(iCol, L)
        b(iCol, L) = DUM
      Next L
    End If
    IndexR(i) = iRow
    IndexC(i) = iCol

    If (A(iCol, iCol) = 0#) Then Singular = True:  Exit Sub
    
    PIVINV = 1# / A(iCol, iCol)
    A(iCol, iCol) = 1#
    For L = 1 To n: A(iCol, L) = A(iCol, L) * PIVINV: Next L
    For L = 1 To M: b(iCol, L) = b(iCol, L) * PIVINV: Next L
    For ll = 1 To n
      If (ll <> iCol) Then
        DUM = A(ll, iCol)
        A(ll, iCol) = 0#
        For L = 1 To n: A(ll, L) = A(ll, L) - A(iCol, L) * DUM: Next L
        For L = 1 To M: b(ll, L) = b(ll, L) - b(iCol, L) * DUM: Next L
      End If
    Next ll
  Next i
  
  For L = n To 1 Step -1
    If (IndexR(L) <> IndexC(L)) Then
      For k = 1 To n
        DUM = A(k, IndexR(L))
        A(k, IndexR(L)) = A(k, IndexC(L))
        A(k, IndexC(L)) = DUM
      Next k
    End If
  Next L

End Sub
Sub DoRegressionMJ(z0!)
'*****************************************************************
' THIS PROGRAM GENERATES TEMPERATURE PROFILE RETRIEVAL COEFFICIENTS
' AND PREDICTED PERFORMANCE UTILIZING THE OBSERVABLE AND RETRIEVABLE
' DATA SETS GENERATED BY THE MTP PROGRAM MTPOBER2UP.FOR (UPWARD
' LOOKING) AND MTPOBER2DN.FOR. THE OUTPUT FILES FROM THESE PROGRAMS
' MUST BE ASSIGNED TO 'INPUTUP' AND 'INPUTDN' BEFORE EXECUTION OF RTVER2.
' Nobs=19 ER2 and 28 DC8 AND Nret=33 (FIXED).
'*****************************************************************
' Subs called: DGAUSSJ, SameFormat, fUSING
'*****************************************************************
  Dim Dret#(33, 2000), Dobs#(33, 2000), Tt#(33, 33), DD#(33, 33), Td#(33, 33)
  Dim dT#(33, 33), T123#(33, 33), TFNL#(33, 33), BDUM#(28, 12)
  Dim Frequency!(360), Elevation!(360), Pretlvl!(33), DDINV#(28, 28)
  Dim RET#(33, 2000), OBS#(28, 2000), EE#(28, 28), COEF#(33, 33), TVAR#(33)
  Dim TAPRI#(33), RETmean#(33), OBSmean#(28), TB#(52), Singular As Boolean
  Dim i%, j%, k%, L%, DUM#, Dum1#, Dum2#, M%, n%, V!(28)
  Dim OBSmeans!(28), OBfile$, RCfile$, UPlu%, DNlu%, OUTlu%, Pz!, IKT&, IRB%
  Dim JYR%, JMTH%, JDAY%, JHR%, kk%, ESVs!(1 To 52), OK As Boolean
  Dim NDAT%, XNDAT!, Sum#, ii%, ABC#, XYZ#, fmt$, mm$, Tc!, S1!, s2!, s0!, pALTindex%
  ' Dim Iobs%(52)
'*************************************************************
'                      ARRAY IDENTIFICATION
'
' COEF(I,J)   = THE COMPUTED RETRIEVAL COEFFICIENT RELATING THE
'               Ith RETRIEVABLE TO THE Jth SELECTED OBSERVABLE.
' DDINV(I,J)  = THE COVARIANCE BETWEEN THE Ith AND Jth
'               SELECTED OBSERVABLES. THE ARRAY NAME REMAINS
'               THE SAME AFTER THIS MATRIX IS INVERTED.
' EE(I,J)     = THE COVARIANCE BETWEEN THE UNCERTAINTIES OF
'               THE Ith AND Jth SELECTED OBSERVABLES.
' Elevation(I)= THE ELEVATION ANGLE OF THE Ith INPUTTED
'               BRIGHTNESS TEMPERATURE OBSERVABLE.
' OBrms(I)    = THE 1 SIGMA UNCERTAINTY ASSIGNED TO THE Ith
'               SELECTED OBSERVABLE.
' Frequency(I)= FREQUENCY IN GHZ OF THE Ith INPUTTED
'               BRIGHTNESS TEMPERATURE OBSERVABLE.
' HRET(J)     = THE HEIGHT IN KM ABOVE OR BELOW THE
'               POINT OF OBSERVATION FOR THE Jth RETRIEVABLE.
' IOBS(I)     = THE INDEX OF THE Ith SELECTED OBSERVABLE WHICH
'               IDENTIFIES WHICH OF THE INPUT OBSERVABLES ARE TO BE USED.
' OB(I,J)     = THE Jth DATA SET VALUE OF THE Ith SELECTED OBSERVABLE.
' OBmean(I)   = THE DATA SET AVERAGE OF OBSERVABLE I.
' RET(I,J)    = THE Jth DATA SET VALUE OF THE Ith RETRIEVABLE.
' RETmean(I)  = THE DATA SET AVERAGE OF RETRIEVABLE I.
' TAPRI(I)    = THE PREDICTED RMS ERROR FOR RETRIEVABLE I.
'               (THE REGRESSION RESIDUAL)
' TAU(I)      = THE ZENITH OPACITY OF THE Ith BRIGHTNESS
'               TEMPERATURE OBSERVABLE, FROM THE INPUT FILE.
'               (FOR THE DOWNWARD LOOKING CASE, TAU(I) IS
'               THE OPACITY BETWEEN THE AIRPLANE AND GROUND.)
' TB(I)       = THE Ith OBSERVABLE IN THE INPUTTED DATA SET.
' TVAR(I)     = THE STATISTICAL STANDARD DEVIATION OF THE
'               Ith RETRIEVABLE OF THE INPUTTED DATA SET.
'*************************************************************
'*************************************************************
'                 KEY NON-DIMENSIONED VARIABLES.
'
' HZ          = THE OBSERVATION (AIRPLANE) HEIGHT ABOVE GROUND
'               IN KM.
' KK          = THE TOTAL NUMBER OF RADIOSONDE OBSERVABLE,
'               RETRIEVABLE DATA SETS WHICH MET THE USER-
'               INPUTTED SPECIFICATIONS.
' MonthMin,
' MonthMax    = THE MINIMUM AND MAXIMUM MONTN NUMBERS TO BE INCLUDED
'               IN THE RETRIEVAL MODEL.
' Nobs        = THE TOTAL NUMBER OF SELECTED OBSERVABLES.
' Nret        = THE TOTAL NUMBER OF RETRIEVED TEMPERATURE
'               HEIGHTS CONTAINED IN THE INPUT FILE.
' NTB         = THE TOTAL NUMBER OF BRIGHTNESS TEMPERATURE
'               OBSERVABLES CONTAINED IN THE INPUT FILE.
' NTBP2       = NTB+2 = THE TOTAL NUMBER OF CANDIDATE
'               OBSERVABLES CONTAINED IN THE INPUT FILE.
'               (THE TEMPERATURE AND PRESSURE AT THE OBSERVATION
'               POINT ARE ALWAYS INCLUDED AS THE LAST 2
'               OBSERVABLES IN THE INPUT FILE.)
'*****************************************************************
'          GENERATE COVARIANCE MATRIX FOR OBSERVABLE ERRORS.
'*****************************************************************
Dim f As Form

  Set f = RCmain
  pALTindex = fGetPaltIndex(z0)
  
  OK = fSet_Apriori_OBrms(f) ' Get the apriori observable errors
  If Not OK Then
    MsgBox "The apriori observable file could not be found!", vbOKOnly
    Exit Sub
  End If
  
  If RCformatUsed <> 2 Then
    MsgBox "The RCformat from the RMS file is not 2. Fix it!", vbOKOnly
    Exit Sub
  End If
  
  If cboMissions.Text = "TEXAQS" Then Call ChangeOBrmsTexAQS(z0!, pALTindex%)

  For i = 1 To Nobs
    For j = 1 To Nobs: EE(i, j) = 0#: Next j
    EE(i, i) = OBrms(pALTindex, i) * OBrms(pALTindex, i)
    If z0 = Zr(NFL) And chkGndRet.Value = 1 Then
      For k = 1 To Channels
        For L = 6 To 9
          n = L + (k - 1) * (Nel - 1)
          EE(n, n) = 9999#   'set wt of down channels low if near ground
        Next L
      Next k
      'If i > 10 And i < 19 Then EE(i, i) = 10000#
      'xxx Changed i>10 to i>9 19990910
    End If
  Next i
  
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumber.Text)))
  RAOBpath$ = RTpath$
  UPlu% = FreeFile  '13
  OBpath$ = RSpath$ + "OBS\"
  OBfile$ = Region$ + "_" + Format$(z0 * 100, "0000") + ".obs"
  Open OBpath$ + OBfile$ For Input As UPlu%
  txtROfile.Text = OBpath$ + OBfile$
  
  RCpath$ = RAOBpath$ + "RC\"
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumberRC.Text)))
  mm$ = Hex$(Month("1999 " + cmbMonth(0).Text)) + Hex$(Month("1999 " + cmbMonth(1).Text))
  RCfile$ = Left$(AC$, 1) + "RC" + Region$ + mm$ + "." + Format$(z0 * 100, "0000")
  RCfilename$ = RCpath$ + RCfile$
  txtRCfile.Text = RCfile$
  OUTlu = FreeFile
  Open RSpath$ + RCfile$ For Output As OUTlu% '12

'*****************************************************************
' INPUT HEADER INFORMATION FROM OBSERVABLE/RETRIEVABLE INPUT FILE.
' Skip through header info
'*****************************************************************
  Input #UPlu%, Pz, NTB, Nret
  For i = 1 To Nret: Input #UPlu%, Pretlvl(i): Next i
  DoEvents
  IKT = 1
'*****************************************************************
' BEGIN LOOP OF OBSERVABLE/RETRIEVABLE INPUT AND SELECT
' PREVIOUSLY SPECIFIED OBSERVABLES TO BE UTILIZED FOR
' RETRIEVAL COEFFICIENT GENERATION.
'*****************************************************************
  On Error GoTo FinishedReading
  Do
    Input #UPlu, IRB, JYR, JMTH, JDAY, JHR, WMOnumber, WMO4l
    For j = 1 To Nret: Input #UPlu%, RET(j, IKT): Next j
    For j = 1 To Nlo * (Nel - 1) + 2: Input #UPlu%, TB(j): Next j
    'NB This combined UP and DN observable file goes UP1 DN1 UP2 DN2 UP3 DN3 HOR
    '             while old separate UP DN file went UP1 UP2 UP3 DN1 DN2 DN3 HOR
    'NB OAT and Pz are read as last observable in above line
    
    'If (JMTH < MonthMin) Or (JMTH > MonthMax) Then GoTo GetNextRAOB
    For L = 1 To Nret
      If (RET(L, IKT) > 350#) Then Debug.Print IRB; L; RET(L, IKT)
      If (RET(L, IKT) < 100#) Then Debug.Print IRB; L; RET(L, IKT)
    Next L
    'If (RET(l, IKT) > 350#) Or (RET(l, IKT) < 100#) Then GoTo GetNextRAOB
    For i = 1 To Nobs:  OBS(i, IKT) = TB(i): Next i
    IKT = IKT + 1
GetNextRAOB:
  Loop Until EOF(UPlu%)

FinishedReading:
  Close UPlu
  DoEvents

'****************************************************************
'  INPUT OF OBSERVABLE/RETRIEVABLE DATA ARCHIVE COMPLETED.
'  READY FOR LINEAR REGRESSION.
'*****************************************************************
  kk = IKT - 1
  If kk < 2 Then
  ' 345 WRITE(6,347)
  ' 347 FORMAT(//10X,45HLESS THAN 2 RAOBS FOUND TO SPECIFIED CRITERIA/)
    GoTo 99
  End If
  NDAT = kk
  XNDAT = NDAT
' RETRIEVABLE AVERAGES.
  For M = 1 To Nret
    Sum = 0#
    For k = 1 To NDAT: Sum = Sum + RET(M, k): Next k
    RETmean(M) = Sum / XNDAT
  Next M
' Observable (ie TB) AVERAGES.
  For n = 1 To Nobs
    Sum = 0#
    For k = 1 To NDAT: Sum = Sum + OBS(n, k): Next k
    OBSmean(n) = Sum / XNDAT
  Next n
' CALCULATE DIFFERENCES FROM MEANS.
  For k = 1 To NDAT
    For M = 1 To Nret: Dret(M, k) = RET(M, k) - RETmean(M): Next M
    For n = 1 To Nobs: Dobs(n, k) = OBS(n, k) - OBSmean(n): Next n
  Next k
' CALCULATE Nret BY Nret SYMMETRIC MATRIX TT.
  For i = 1 To Nret
    For j = 1 To Nret
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dret(i, k) * Dret(j, k): Next k
      Tt(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
  For i = 2 To Nret
    For j = 1 To i - 1: Tt(i, j) = Tt(j, i): Next j
  Next i
' CALCULATE Nobs BY Nobs SYMMETRIC MATRIX DD.
  For i = 1 To Nobs
    For j = 1 To Nobs
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dobs(i, k) * Dobs(j, k): Next k
      DD(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
  For i = 2 To Nobs
    For j = 1 To i - 1: DD(i, j) = DD(j, i): Next j
  Next i
  For i = 1 To Nobs
    For j = 1 To Nobs:  DD(i, j) = DD(i, j) + EE(i, j): Next j
  Next i
' CALCULATE Nret BY Nobs MATRIX TD.
  For i = 1 To Nret
    For j = 1 To Nobs
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dret(i, k) * Dobs(j, k): Next k
      Td(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
' CALCULATE Nobs BY Nret MATRIX DT FROM THE Nret BY Nobs MATRIX TD.
  For i = 1 To Nobs
    For j = 1 To Nret: dT(i, j) = Td(j, i): Next j
  Next i
' INVERT DD MATRIX.
  For i = 1 To Nobs
    For j = 1 To Nobs: DDINV(i, j) = DD(i, j): Next j
  Next i
  
  Call DGAUSSJ(DDINV(), Nobs, 19, BDUM, 1, 12, Singular)

' MATRIX MULTIPLICATION OF TD AND DDINV TO OBTAIN COEF(Nret,Nobs)
  For i = 1 To Nret
    For j = 1 To Nobs
      Sum = 0#
      For ii = 1 To Nobs:   Sum = Sum + Td(i, ii) * DDINV(ii, j): Next ii
      COEF(i, j) = Sum
    Next j
  Next i

' MATRIX MULTIPLICATION OF T12 AND DT TO OBTAIN T123(Nret,Nret)
  For i = 1 To Nret
    For j = 1 To Nret
      Sum = 0#
      For ii = 1 To Nobs: Sum = Sum + COEF(i, ii) * dT(ii, j): Next ii
      T123(i, j) = Sum
    Next j
  Next i
  
  For i = 1 To Nret
    For j = 1 To Nret: TFNL(i, j) = Tt(i, j) - T123(i, j): Next j
    ABC = Tt(i, i)
    XYZ = TFNL(i, i)
    TVAR(i) = Sqr(ABC)
    If XYZ <= 0 Then
      Debug.Print "XYZ="; XYZ; " I="; i
      TAPRI(i) = 0#
    Else
      TAPRI(i) = Sqr(XYZ)
    End If
  Next i

'*****************************************************************
' OUTPUT FILE OF RETRIEVAL COEFFICIENTS
'*****************************************************************
' Comments show corresponding MTPbin variable names as used in ReadInRC
' The i index is the flight level index. It doesn't occur in RCmain because
' Only one flight level at a time is calculated. Also, iRC refers to the RC set number
  V(1) = Pz: V(2) = kk
'        bpv        RAOBcount
'  Print #OUTlu, File$ + fUsing(" ###0.00 #000", V()) + " Generated: " + Date$ + " " + Time$
'  Print #OUTlu, ""
'  V(1) = bpv: V(2) = RAOBcount
  Print #OUTlu, RCpath$ + RCfile$ + fUsing(" ###0.00 #000", V()) + " Generated: " + Date$ + " " + Time$
  Print #OUTlu, ""
    For j = 1 To Nobs: ESVs(j) = CSng(OBrms(pALTindex, j)): Next j   'xxxxx
  Call SameFormat(OUTlu%, " ###0.00", 9, ESVs(), Nobs)
'                                        OBrms(i,j)
  For j = 1 To Nobs: OBSmeans(j) = CSng(OBSmean(j)): Next j
  Call SameFormat(OUTlu%, " ##0.00", 9, OBSmeans(), Nobs)
'                                       OBav(iRC,i,j)
  fmt$ = " ###0.00 ###0.00 ###0.00 ###0.00"
  For k = 1 To Nret
    V(1) = Pretlvl(k): V(2) = RETmean(k): V(3) = TVAR(k): V(4) = TAPRI(k)
'          bp(iRC,i,k)        RTav(iRC, i,k)     RMSa(iRC,i,k)   RMSe(iRC,i,k)
    Print #OUTlu%, fUsing(fmt$, V())
    For L = 1 To Nobs: V(L) = CSng(COEF(k, L)): Next L
'                                  rc(iRC,i,k,l)
    Call SameFormat(OUTlu%, " ##0.00000", 6, V(), Nobs)
  Next k
' Correspondence with MTPbin variables
' Note that MTPbin variables have extra dimension for each RC set
'

99:
  Close OUTlu
  

End Sub

Sub DoRegressionMJ2(z0!)
'*****************************************************************
' THIS PROGRAM GENERATES TEMPERATURE PROFILE RETRIEVAL COEFFICIENTS
' AND PREDICTED PERFORMANCE UTILIZING THE OBSERVABLE AND RETRIEVABLE
' DATA SETS GENERATED BY THE MTP PROGRAM MTPOBER2UP.FOR (UPWARD
' LOOKING) AND MTPOBER2DN.FOR. THE OUTPUT FILES FROM THESE PROGRAMS
' MUST BE ASSIGNED TO 'INPUTUP' AND 'INPUTDN' BEFORE EXECUTION OF RTVER2.
' Nobs=20 ER2 and 30 DC8 AND Nret=33 (FIXED).
'*****************************************************************
' Subs called: DGAUSSJ, SameFormat, fUSING
'*****************************************************************
' CALCULATION OF STATISTICAL RETRIEVAL COEFFICIENTS
'
' Given:
' N = Number of RAOBs
' L = Number of retrievable properties
' M = Number of observables
' A physical model for calculating observables from RAOBs (e.g. MTM)
'
' Find:
' Retrieval coefficient (RC) matrix R such that T' = <T> + R*(O'-<O>)
' where:
' T'   = best estimate of the physical temperature
' <T>  = calculate average temperature value
' R    = the retrieval coefficient matrix
' O'   = the measured observables
' <O>  = the calculated average observable values
'
' Solution:
' R = TD(T,O) * INV[DD(O,O)]
' where:
' TD(T,O)ij = Sum(n=1 to N)[(T(i,n)-<T(i)>)*(O(j,n)-<O(j)>)]/(N-1)
'
' TD(T,O)ij is the MxM cross-covarience matrix
' T(i,n) = ith temperature calculated for nth RAOB
' O(j,n) = jth observable calculated for nth RAOB
' And <T(i)> and <O(j)> are their averages for N RAOBs
'
' DD(O,O)ij = Sum(n=1 to N)[(O(i,n)-<O(j)>)*(O(j,n)-<O(j)>)]/(N-1) + OBrms(i,j)
'
' DD(O,O)ij is the LxM auto-covariance matrix
' O(j,n) and <O(j)> are as above, and
' OBrms(i,j) is MxM the observable error matrix
' INV[DD(O,O)] is just the matrix inverse of DD(O,O)

  Dim Dret#(33, 2000), Dobs#(33, 2000), Tt#(33, 33), DD#(33, 33), Td#(33, 33)
  Dim dT#(33, 33), T123#(33, 33), TFNL#(33, 33), BDUM#(30, 12)
  Dim Frequency!(360), Elevation!(360)
  Dim DDINV#(30, 30)
  Dim RET#(33, 2000)
  Dim OBS#(30, 2000), EE#(30, 30), COEF#(33, 33)
  Dim RMSa2#(1 To 33)
  Dim RMSe2#(1 To 33)
  Dim RTav2#(1 To 33)                  'Avg T profile for RAOB set
  Dim OBav2#(1 To 30)
  Dim OBav1!(1 To 30)
  Dim TB#(52)
  Dim Singular As Boolean
  Dim i%, j%, k%, L%, DUM#, Dum1#, Dum2#, M%, n%, V!(31)
  Dim OBfile$, RCfile$, UPlu%, DNlu%, OUTlu%, bpv!, IKT&, IRB%
  Dim JYR%, JMTH%, JDAY%, JHR%
  Dim OBrms1!(1 To 30)
  Dim OK As Boolean
  Dim NDAT%, XNDAT!, Sum#, ii%, ABC#, XYZ#, fmt$, mm$, Tc!, S1!, s2!, s0!, pALTindex%
'
'*************************************************************
'                      ARRAY IDENTIFICATION
'
' COEF(I,J)   = THE COMPUTED RETRIEVAL COEFFICIENT RELATING THE
'               Ith RETRIEVABLE TO THE Jth SELECTED OBSERVABLE.
' DDINV(I,J)  = THE COVARIANCE BETWEEN THE Ith AND Jth
'               SELECTED OBSERVABLES. THE ARRAY NAME REMAINS
'               THE SAME AFTER THIS MATRIX IS INVERTED.
' EE(I,J)     = THE COVARIANCE BETWEEN THE UNCERTAINTIES OF
'               THE Ith AND Jth SELECTED OBSERVABLES.
' Elevation(I)= THE ELEVATION ANGLE OF THE Ith INPUTTED
'               BRIGHTNESS TEMPERATURE OBSERVABLE.
' OBrms(I)    = THE 1 SIGMA UNCERTAINTY ASSIGNED TO THE Ith
'               SELECTED OBSERVABLE.
' Frequency(I)= FREQUENCY IN GHZ OF THE Ith INPUTTED
'               BRIGHTNESS TEMPERATURE OBSERVABLE.
' HRET(J)     = THE HEIGHT IN KM ABOVE OR BELOW THE
'               POINT OF OBSERVATION FOR THE Jth RETRIEVABLE.
' IOBS(I)     = THE INDEX OF THE Ith SELECTED OBSERVABLE WHICH
'               IDENTIFIES WHICH OF THE INPUT OBSERVABLES ARE TO BE USED.
' OB(I,J)     = THE Jth DATA SET VALUE OF THE Ith SELECTED OBSERVABLE.
' OBmean(I)   = THE DATA SET AVERAGE OF OBSERVABLE I.
' RET(I,J)    = THE Jth DATA SET VALUE OF THE Ith RETRIEVABLE.
' RTav2(I)    = THE DATA SET AVERAGE OF RETRIEVABLE I.
' RMSe2(I)    = THE PREDICTED RMS ERROR FOR RETRIEVABLE I.
'               (THE REGRESSION RESIDUAL)
' TAU(I)      = THE ZENITH OPACITY OF THE Ith BRIGHTNESS
'               TEMPERATURE OBSERVABLE, FROM THE INPUT FILE.
'               (FOR THE DOWNWARD LOOKING CASE, TAU(I) IS
'               THE OPACITY BETWEEN THE AIRPLANE AND GROUND.)
' TB(I)       = THE Ith OBSERVABLE IN THE INPUTTED DATA SET.
' RMSa2(I)    = THE STATISTICAL STANDARD DEVIATION OF THE
'               Ith RETRIEVABLE OF THE INPUTTED DATA SET.
'*************************************************************
'*************************************************************
'                 KEY NON-DIMENSIONED VARIABLES.
'
' HZ          = THE OBSERVATION (AIRPLANE) HEIGHT ABOVE GROUND
'               IN KM.
' RAOBcount   = THE TOTAL NUMBER OF RADIOSONDE OBSERVABLE,
'               RETRIEVABLE DATA SETS WHICH MET THE USER-
'               INPUTTED SPECIFICATIONS.
' MonthMin,
' MonthMax    = THE MINIMUM AND MAXIMUM MONTN NUMBERS TO BE INCLUDED
'               IN THE RETRIEVAL MODEL.
' Nobs        = THE TOTAL NUMBER OF SELECTED OBSERVABLES.
' Nret        = THE TOTAL NUMBER OF RETRIEVED TEMPERATURE
'               HEIGHTS CONTAINED IN THE INPUT FILE.
' NTB         = THE TOTAL NUMBER OF BRIGHTNESS TEMPERATURE
'               OBSERVABLES CONTAINED IN THE INPUT FILE.
' NTBP2       = NTB+2 = THE TOTAL NUMBER OF CANDIDATE
'               OBSERVABLES CONTAINED IN THE INPUT FILE.
'               (THE TEMPERATURE AND PRESSURE AT THE OBSERVATION
'               POINT ARE ALWAYS INCLUDED AS THE LAST 2
'               OBSERVABLES IN THE INPUT FILE.)
'*****************************************************************
'          GENERATE COVARIANCE MATRIX FOR OBSERVABLE ERRORS.
'*****************************************************************
Dim f As Form

  Set f = RCmain
  pALTindex = fGetPaltIndex(z0)
  
  OK = fSet_Apriori_OBrms(f) ' Get the apriori observable errors
  If Not OK Then
    MsgBox "The apriori observable file could not be found!", vbOKOnly
    Exit Sub
  End If
  
  If RCformatUsed <> 3 Then
    MsgBox "The RCformat from the RMS file is not 3. Fix it!", vbOKOnly
    Exit Sub
  End If
  If cboMissions.Text = "TEXAQS" Then Call ChangeOBrmsTexAQS(z0!, pALTindex%)

  For i = 1 To Nobs
    For j = 1 To Nobs: EE(i, j) = 0#: Next j
    EE(i, i) = OBrms(pALTindex, i) * OBrms(pALTindex, i)
    If z0 = Zr(NFL) And chkGndRet.Value = 1 Then
      For k = 1 To Channels
        For L = 7 To 10
          n = L + (k - 1) * (Nel - 1)
          EE(n, n) = 9999#   'set wt of down channels low if near ground
        Next L
      Next k
    End If
  Next i

' Assign OBS file
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumber.Text)))
  UPlu% = FreeFile  '13
  OBpath$ = RAOBpath$ + "OBS\"
  OBfile$ = Region$ + "_" + Format$(z0 * 100, "0000") + ".obs"
  Open OBpath$ + OBfile$ For Input As UPlu%
  txtROfile.Text = OBfile$
' Assign RC file
  RCpath$ = RAOBpath$ + "RC\"
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumberRC.Text)))
  mm$ = Hex$(Month("1999 " + cmbMonth(0).Text)) + Hex$(Month("1999 " + cmbMonth(1).Text))
  RCfile$ = Left$(AC$, 1) + "RC" + Region$ + mm$ + "." + Format$(z0 * 100, "0000")
  RCfilename$ = RCpath$ + RCfile$
  OUTlu = FreeFile
  Open RCpath$ + RCfile$ For Output As OUTlu% '12
  txtRCfile.Text = RCfile$

'*****************************************************************
' INPUT HEADER INFORMATION FROM OBSERVABLE/RETRIEVABLE INPUT FILE.
' Skip through header info
'*****************************************************************
  Input #UPlu%, bpv, NTB, Nret
  For i = 1 To Nret: Input #UPlu%, BP(0, pALTindex, i): Next i
  DoEvents
  IKT = 1
'*****************************************************************
' BEGIN LOOP OF OBSERVABLE/RETRIEVABLE INPUT AND SELECT
' PREVIOUSLY SPECIFIED OBSERVABLES TO BE UTILIZED FOR
' RETRIEVAL COEFFICIENT GENERATION.
'*****************************************************************
  On Error GoTo FinishedReading
  Do
    Input #UPlu, IRB, JYR, JMTH, JDAY, JHR, WMOnumber, WMO4l
    For j = 1 To Nret: Input #UPlu%, RET(j, IKT): Next j
    For L = 1 To Nret
      If (RET(L, IKT) > 350#) Then Debug.Print IRB; L; RET(L, IKT)
      If (RET(L, IKT) < 100#) Then Debug.Print IRB; L; RET(L, IKT)
    Next L
    
    If chkConvert2to3.Value = 0 Then  'Read 30 observables
      For j = 1 To Nlo * Nel + 1: Input #UPlu%, TB(j): Next j
    ' NB This observable file does Ch1:1->10, Ch2:1->10, Ch3:1->10
    '    bpv (Pz) is read as last observable in above line
      For i = 1 To Nobs:
      OBS(i, IKT) = TB(i)
      If TB(i) < 100 Then
      Debug.Print IKT; i; TB(i)
      End If
      Next i
    Else ' Need to read 28 observables (RCformat=1) and convert to 30 (RCformat=3)
      For j = 1 To Nlo * (Nel - 1) + 2: Input #UPlu%, TB(j): Next j
    ' NB This observable file does Ch1:1-5,7-10 Ch2:1-5,7-10, Ch3:1-5,7-10
    '    OAT and bpv (Pz) are read as last observable in above line
      For i = 1 To 5:  OBS(i, IKT) = TB(i): Next i
      For i = 7 To 10:  OBS(i, IKT) = TB(i - 1): Next i
      For i = 11 To 15:  OBS(i, IKT) = TB(i - 1): Next i
      For i = 17 To 20:  OBS(i, IKT) = TB(i - 2): Next i
      For i = 21 To 25:  OBS(i, IKT) = TB(i - 2): Next i
      For i = 27 To 30:  OBS(i, IKT) = TB(i - 3): Next i
      OBS(6, IKT) = TB(28)
      OBS(16, IKT) = TB(28)
      OBS(26, IKT) = TB(28)
    End If
    IKT = IKT + 1
GetNextRAOB:
  Loop Until EOF(UPlu%)

FinishedReading:
  Close UPlu
  DoEvents

'****************************************************************
'  INPUT OF OBSERVABLE/RETRIEVABLE DATA ARCHIVE COMPLETED.
'  READY FOR LINEAR REGRESSION.
'*****************************************************************
  RAOBcount = IKT - 1
  If RAOBcount < 2 Then
    MsgBox "There are <2 RAOBs that meet the specified criteria", vbOKOnly
    Close OUTlu
    Exit Sub
  End If
  NDAT = RAOBcount
  XNDAT = NDAT
' RETRIEVABLE AVERAGES.
  For M = 1 To Nret
    Sum = 0#
    For k = 1 To NDAT: Sum = Sum + RET(M, k): Next k
    RTav2(M) = Sum / XNDAT
  Next M
' Observable (ie TB) AVERAGES.
  For n = 1 To Nobs
    Sum = 0#
    For k = 1 To NDAT: Sum = Sum + OBS(n, k): Next k
    OBav2(n) = Sum / XNDAT
  Next n
' CALCULATE DIFFERENCES FROM MEANS.
  For k = 1 To NDAT
    For M = 1 To Nret: Dret(M, k) = RET(M, k) - RTav2(M): Next M
    For n = 1 To Nobs: Dobs(n, k) = OBS(n, k) - OBav2(n): Next n
  Next k
' CALCULATE Nret BY Nret SYMMETRIC MATRIX TT.
  For i = 1 To Nret
    For j = 1 To Nret
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dret(i, k) * Dret(j, k): Next k
      Tt(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
  For i = 2 To Nret
    For j = 1 To i - 1: Tt(i, j) = Tt(j, i): Next j
  Next i
' CALCULATE Nobs BY Nobs SYMMETRIC MATRIX DD.
  For i = 1 To Nobs
    For j = 1 To Nobs
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dobs(i, k) * Dobs(j, k): Next k
      DD(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
  For i = 2 To Nobs
    For j = 1 To i - 1: DD(i, j) = DD(j, i): Next j
  Next i
  For i = 1 To Nobs
    For j = 1 To Nobs:  DD(i, j) = DD(i, j) + EE(i, j): Next j
  Next i
' CALCULATE Nret BY Nobs MATRIX TD.
  For i = 1 To Nret
    For j = 1 To Nobs
      Sum = 0#
      For k = 1 To NDAT: Sum = Sum + Dret(i, k) * Dobs(j, k): Next k
      Td(i, j) = Sum / (XNDAT - 1#)
    Next j
  Next i
' CALCULATE Nobs BY Nret MATRIX DT FROM THE Nret BY Nobs MATRIX TD.
  For i = 1 To Nobs
    For j = 1 To Nret: dT(i, j) = Td(j, i): Next j
  Next i
' INVERT DD MATRIX.
  For i = 1 To Nobs
    For j = 1 To Nobs: DDINV(i, j) = DD(i, j): Next j
  Next i
  
  Call DGAUSSJ(DDINV(), Nobs, 19, BDUM, 1, 12, Singular)

' MATRIX MULTIPLICATION OF TD AND DDINV TO OBTAIN COEF(Nret,Nobs)
  For i = 1 To Nret
    For j = 1 To Nobs
      Sum = 0#
      For ii = 1 To Nobs:   Sum = Sum + Td(i, ii) * DDINV(ii, j): Next ii
      COEF(i, j) = Sum
    Next j
  Next i

' MATRIX MULTIPLICATION OF T12 AND DT TO OBTAIN T123(Nret,Nret)
  For i = 1 To Nret
    For j = 1 To Nret
      Sum = 0#
      For ii = 1 To Nobs: Sum = Sum + COEF(i, ii) * dT(ii, j): Next ii
      T123(i, j) = Sum
    Next j
  Next i
  
  For i = 1 To Nret
    For j = 1 To Nret: TFNL(i, j) = Tt(i, j) - T123(i, j): Next j
    ABC = Tt(i, i)
    XYZ = TFNL(i, i)
    RMSa2(i) = Sqr(ABC)
    If XYZ <= 0 Then
      RMSe2(i) = 0#
    Else
      RMSe2(i) = Sqr(XYZ)
    End If
  Next i

'*****************************************************************
' OUTPUT FILE OF RETRIEVAL COEFFICIENTS
'*****************************************************************
' Comments show corresponding MTPbin variable names as used in ReadInRC
' The i index is the flight level index. It doesn't occur in RCmain because
' Only one flight level at a time is calculated. Also, iRC refers to the RC set number
  V(1) = bpv: V(2) = RAOBcount
  Print #OUTlu, RCpath$ + RCfile$ + fUsing(" ###0.00 #000", V()) + " Generated: " + Date$ + " " + Time$
  Print #OUTlu, ""
  For j = 1 To Nobs
    OBrms1(j) = CSng(OBrms(pALTindex, j))
  Next j   'xxxxx
  Debug.Print pALTindex; OBrms1(1)
  Call SameFormat(OUTlu%, " ###0.00", 9, OBrms1(), Nobs)
  
  For j = 1 To Nobs
    OBav1(j) = CSng(OBav2(j))
    OBav(0, pALTindex, j) = OBav1(j)
  Next j
  Call SameFormat(OUTlu%, " ##0.00", 9, OBav1(), Nobs)
  
  fmt$ = " ###0.00 ###0.00 ###0.00 ###0.00"
  For k = 1 To Nret
    V(1) = BP(0, pALTindex, k)
    V(2) = RTav2(k)
    V(3) = RMSa2(k)
    V(4) = RMSe2(k)
    RTav(0, pALTindex, k) = RTav2(k)
    RMSa(0, pALTindex, k) = RMSa2(k)
    RMSe(0, pALTindex, k) = RMSe2(k)
    Print #OUTlu%, fUsing(fmt$, V())
    For L = 1 To Nobs
      V(L) = CSng(COEF(k, L))
      rc(0, pALTindex, k, L) = V(L)
    Next L
    Call SameFormat(OUTlu%, " ##0.00000", 6, V(), Nobs)
  Next k
  Close OUTlu

End Sub


Sub DoRetObsCalculationMJ(z0!, RAOBlu%)
Dim OUTfile$, fmt$, jj%, ROpath$
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, M%, iL%, X%
Dim TRANGD!, Cycle&, Fif!(1 To 24), Plus As Boolean, RecordStep%
'****************************************************************
' MTPOBERU.FOR COMPUTES OXYGEN BAND BRIGHTNESS TEMPS.,
' AS VIEWED UPWARDLY FROM A SPECIFIED HEIGHT ABOVE SURFACE,
' FOR SELECTED FREQUENCY, ELEVATION ANGLE COMBINATIONS
' UTILIZING RADIOSONDE DATA AND ABSORPTION MODELS.
' PHYSICAL TEMPERATURES AT SELECTED RETRIEVAL HEIGHTS,
' AS WELL AS TEMPERATURE AND PRESSURE AT THE OBSERVING
' HEIGHT, ARE ALSO COMPUTED AND OUTPUTTED.
' UP TO 250 FREQUENCY, ELEVATION ANGLE COMBINATIONS MAY
' BE SPECIFIED.
' UPDATE OF FEB., 1998 FOR WAM CAMPAIGN
'****************************************************************
' Uses: MTPOBERU,OXLIEB93,fVLIEBE,TPRPRFDC8
'****************************************************************

Dim TBvsEl!(1 To 31), Sum!, Dfreq!, ROlu%
Dim jp1%, jm1%, NHm1%, V!(1 To 20), argUP!, argDN!

Dim Temperature!, Pressure!, RhoV!, f!, alpha!, alphaC!, Tbg!, Tj!
Dim IKT&, ans
Static Overwrite As Boolean

'******************************************************************
'                       ARRAY IDENTIFICATION
'
' DELHI(J) = THE NODE DISTANCE BETWEEN THE J-1 AND J LEVELS
'            AS MEASURED FROM THE OBSERVATION POINT, IN KM.
' Elevation(I) = ELEVATION ANGLE IN DEGREES OF Ith BRIGHTNESS TEMP.
'            OBSERVABLE. MEASURED ABOVE HORIZON.
' Frequency(I)  = FREQUENCY IN GHZ OF THE Ith BRIGHTNESS TEMPERATURE
'            OBSERVABLE.
' HI(J)    = NOMINAL HEIGHT LEVELS (IN KM) USED TO CALCULATE
'            BRIGHTNESS TEMPERATURES NUMERICALLY. THESE VALUES ARE
'            FIXED (RELATIVE TO THE OBSERVATION POINT) BY
'            THE NODE SPACINGS, DELHI(J).
' PrI(J)  = TOTAL PRESSURE (MB) AT THE JTH MODEL HEIGHT
'            LEVEL.
' RI(J)    = RELATIVE HUMIDITY (%) AT THE Jth MODEL HEIGHT
'            LEVEL.
' ZGraobJ)  = HEIGHT ABOVE SURFACE IN KM OF THE Jth RADIOSONDE
'            LEVEL. UP TO 80 RADIOSONDE LEVELS ARE PERMISSIBLE.
' PZraob(J) = PRESSURE (MB) AT THE JTH RADISONDE HEIGHT.
' RZraob(J) = RELATIVE HUMIDITY AT THE JTH RADIOSONDE HEIGHT.
' TZraob(J) = TEMPERATURE (K) AT THE Jth RADIOSONDE HEIGHT.
' TAU(I,J) = OPACITY AT FREQUENCY Frequency(I) BETWEEN THE
'            AIRCRAFT AND HEIGHT HI(J) (IN NEPERS).
' TB(J)    = THE Jth OBSERVABLE. THE LAST 2 OBSERVABLES ARE
'            ALWAYS TEMPERATURE AND PRESSURE AT THE OBSERVATION
'            POINT. ALL OTHERS ARE BRIGHTNESS TEMPERATURES.
' TI(J)   = THE PHYSICAL TEMPERATURE AT MODEL HEIGHT
'            LEVEL HI(J).
' VD(J)    = VAPOR DENSITY (GM/M**3) AT THE MODEL HEIGHT LEVEL
'            HI(J).
' XKNU(I,J)= TOTAL ABSORPTION COEFFICIENT AT THE MODEL
'            HEIGHT HI(J) FOR FREQUENCY Frequency(I)
'****************************************************************
'****************************************************************
'            KEY NON-DIMENSIONED VARIABLES
'
' ABSOX  = ABSORPTION DUE TO OXYGEN (NEPERS/KM).
' ABSV   = ABSORPTION DUE TO WATER VAPOR (NEPERS/KM).
' Nlevr   = NUMBER OF RADIOSONDE HEIGHT LEVELS.
' NH     = NUMBER OF NOMINAL HEIGHT LEVELS.
' NraobMax    = MAXIMUM NUMBER OF RADIOSONDES FOR WHICH OBSERVABLES
'          AND RETRIEVABLES ARE TO BE COMPUTED.
' NTB    = NUMBER OF BRIGHTNESS TEMPERATURE OBSERVABLES.
' NTBP2  = TOTAL NUMBER OF OBSERVABLES (INCLUDING OBSERVATION
'          HEIGHT TEMPERATURE AND PRESSURE).
'****************************************************************
  Pz = fZtoP(z0)
  RecordStep = Val(txtRecordStep.Text)
  LR1 = Val(txtLR1.Text)  'see Calc_Hret_Tret
  LR2 = Val(txtLR2.Text)  'see Calc_Hret_Tret
  zLRb = Val(txtZb.Text)  'see Calc_Hret_Tret
  
' Open I/O files
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumber.Text)))

'  ROpath$ = Left$(fGetPath(txtRCfile.Text), Len(fGetPath(txtRCfile.Text)) - 3) + "OBS\"
  ROpath$ = txtRAOBpath.Text + "OBS\"
  If Dir(ROpath$, vbDirectory) = "" Then MkDir ROpath$  'Make /OBS/ directory if it doesn't exist
  ROlu = FreeFile
  ROfile$ = Region$ + "_" + Format$(z0 * 100, "0000") + ".OBS"

' Check if .OBS files already exists. If so, make sure it's ok to overwrite them.
  If Len(Dir(ROpath$ + ROfile$)) > 0 And Not Overwrite Then
    ans = MsgBox(ROpath$ + ROfile$ + " already exists!" + vbCrLf + "Do you really want to overwrite it?", vbYesNo)
    If ans = vbYes Then
      Overwrite = True
    Else
      End
    End If
  End If
  Open ROpath$ + ROfile$ For Output As ROlu
  txtROfile.Text = ROfile$

' Write header information to file
  fmt$ = "###0.00 #000 #00"
  V(1) = Pz: V(2) = NTB: V(3) = Nret
  Print #ROlu, fUsing(fmt$, V())

  CalcInit   'Get EL angles, IF frequencies and Weights
  Call CalcRetLevels(z0)        'Initialize retrieval pressure levels, Pret()
  Call SameFormat(ROlu, " ###0.00", 9, Pret!(), Nret)
  
  IKT = 1
  Cycle = 0
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'Main loop to read raobs and calculate TBs and retrievables
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
X = Rnd(-1) 'Initialize random number generator
Do
SkipRAOB:
  Call GetValidRAOB(RAOBlu, IKT, Cycle, RecordStep)
  If EOF(RAOBlu%) Then Exit Do
  Tj = fLinterp(ZPraob(), TZraob(), Nlevr, z0)
  
'  If chkUseTwoSets.value = 1 Then 'Check if RAOBs are to be split into warm and cold sets
'    If Tavg >= 0 And Tj < Tavg Then GoTo SkipRAOB  'Cold RAOBs
'    If Tavg < 0 And Tj >= -Tavg Then GoTo SkipRAOB 'Warm RAOBs
'  End If
  If chkExcessGndT.Value = 1 Then ExcessGndT = ExcessTamplitude * Rnd(1) Else ExcessGndT = 0#

' Interpolate RAOBs at geometric retrieval levels wrt a/c for T and P
' Enter with Pz which has a different Hz for each RAOB
  Calc_Hret_Tret

' Do calculation for each LO frequency
  For M = 1 To Nlo
  ' Do calculation for each IF frequency at each LO
    For j = 1 To Nnu
      Fif(j) = LO(M) + LSBUSB(M, j)
      If Fif(j) < 0 Then
      X = X
      End If
    Next j
     
'   Nothing depended on elevation angle to this point
    ' Calculate NH geometric integration levels HI(i) wrt a/c
    Call CalcZgLevels(El(1), False)    'Depends only on sign of Elevation Angle
    ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
    ' Call TPRPRFDC8(Nlevr)
      Call TPRatZgLevels(Nlevr)
    ' Calculate Absorption at each frequency in bandpass
    Call CalcBandTau(Fif())      'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
    ' Assume elevation angles go from + to -
    Plus = True
    For L = 1 To Nel - 1  'Do all El's but horizon position
      If El(L) < 0 And Plus Then  'Elevation angle sign change! Need new HI() levels, etc
        Plus = False
      ' Calculate NH geometric integration levels HI(i) wrt a/c
        Call CalcZgLevels(El(L), False)
      ' Establish T,P,RH vs all height levels by interpolation of values at
      ' RAOB height levels.
       ' Call TPRPRFDC8(Nlevr)
        Call TPRatZgLevels(Nlevr)
      
' Calculate Absorption at each frequency in bandpass
        Call CalcBandTau(Fif())
      End If
    ' Calculate Brightness Temperatures along line of sight at each IF frequency
    ' and for each elevation angle
      Call CalcBandTBs(El(L), TB())
   
    ' Calculate Bandpass weighted TBs at each elevation.
      Sum = 0#
      For i = 1 To 2 * Nif
        Sum = Sum + BandWt(M, i) * TB(i)
      Next i
       TBvsEl((M - 1) * (Nel - 1) + L) = Sum / BandSum(M)
    Next L
 '   Debug.Print Sum / BandSum
  Next M
' Last 2 observables are temperature and pressure at the a/c altitude.
  TBvsEl((Nel - 1) * Nlo + 1) = Tj  'Ti(NH)    ' OAT
  TBvsEl((Nel - 1) * Nlo + 2) = PrI(NH)   ' Pressure altitude
'  Debug.Print IKT; WMO4l
'  For i = 1 To 30: Debug.Print TBvsEl(i);: Next i
'  Debug.Print ""

' Output retrievables and observables based on this RAOB
  V(1) = IKT: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihour
  fmt$ = "###00 ###0 #0 #0 #0"
  Print #ROlu%, fUsing(fmt$, V()) + " " + Format(WMOnumber, "00000") + " " + WMO4l
  Call SameFormat(ROlu, " ###0.00", 8, Tret!(), Nret)
  Call SameFormat(ROlu, " ###0.00", 8, TBvsEl!(), (Nel - 1) * Nlo + 2) '''''die here'
  IKT = IKT + 1
Loop Until EOF(RAOBlu) Or IKT > NraobMax

Debug.Print "COMPLETION OF OBSERVABLE AND RETRIEVABLE CALCULATIONS FOR " + Str$(IKT) + "  RAOBS."
Debug.Print "Processing took from: " + StartTime$ + " To " + Str$(Time)
Close ROlu, RAOBlu

End Sub


Sub DoRetObsCalculationMJ2(z0!, RAOBlu%)
Dim OUTfile$, fmt$, jj%, ROpath$
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, M%, iL%, X%
Dim TRANGD!, Cycle&, Fif!(1 To 24), Plus As Boolean
'****************************************************************
' MTPOBERU.FOR COMPUTES OXYGEN BAND BRIGHTNESS TEMPS.,
' AS VIEWED UPWARDLY FROM A SPECIFIED HEIGHT ABOVE SURFACE,
' FOR SELECTED FREQUENCY, ELEVATION ANGLE COMBINATIONS
' UTILIZING RADIOSONDE DATA AND ABSORPTION MODELS.
' PHYSICAL TEMPERATURES AT SELECTED RETRIEVAL HEIGHTS,
' AS WELL AS TEMPERATURE AND PRESSURE AT THE OBSERVING
' HEIGHT, ARE ALSO COMPUTED AND OUTPUTTED.
' UP TO 250 FREQUENCY, ELEVATION ANGLE COMBIN
' UPDATE OF FEB., 1998 FOR WAM CAMPAIGNATIONS MAY
' BE SPECIFIED.
'****************************************************************
' Uses: MTPOBERU,OXLIEB93,fVLIEBE,TPRPRFDC8
'****************************************************************

Dim TBvsEl!(1 To 31), Sum!, Dfreq!, ROlu%
Dim jp1%, jm1%, NHm1%, V!(1 To 20), argUP!, argDN!

Dim Temperature!, Pressure!, RhoV!, f!, alpha!, alphaC!, Tbg!, Tj!
Dim IKT&, ans
Static Overwrite As Boolean

'******************************************************************
'                       ARRAY IDENTIFICATION
'
' DELHI(J) = THE NODE DISTANCE BETWEEN THE J-1 AND J LEVELS
'            AS MEASURED FROM THE OBSERVATION POINT, IN KM.
' Elevation(I) = ELEVATION ANGLE IN DEGREES OF Ith BRIGHTNESS TEMP.
'            OBSERVABLE. MEASURED ABOVE HORIZON.
' Frequency(I)  = FREQUENCY IN GHZ OF THE Ith BRIGHTNESS TEMPERATURE
'            OBSERVABLE.
' HI(J)    = NOMINAL HEIGHT LEVELS (IN KM) USED TO CALCULATE
'            BRIGHTNESS TEMPERATURES NUMERICALLY. THESE VALUES ARE
'            FIXED (RELATIVE TO THE OBSERVATION POINT) BY
'            THE NODE SPACINGS, DELHI(J).
' PrI(J)  = TOTAL PRESSURE (MB) AT THE JTH MODEL HEIGHT
'            LEVEL.
' RI(J)    = RELATIVE HUMIDITY (%) AT THE Jth MODEL HEIGHT
'            LEVEL.
' ZGraobJ)  = HEIGHT ABOVE SURFACE IN KM OF THE Jth RADIOSONDE
'            LEVEL. UP TO 80 RADIOSONDE LEVELS ARE PERMISSIBLE.
' PZraob(J) = PRESSURE (MB) AT THE JTH RADISONDE HEIGHT.
' RZraob(J) = RELATIVE HUMIDITY AT THE JTH RADIOSONDE HEIGHT.
' TZraob(J) = TEMPERATURE (K) AT THE Jth RADIOSONDE HEIGHT.
' TAU(I,J) = OPACITY AT FREQUENCY Frequency(I) BETWEEN THE
'            AIRCRAFT AND HEIGHT HI(J) (IN NEPERS).
' TB(J)    = THE Jth OBSERVABLE. THE LAST 2 OBSERVABLES ARE
'            ALWAYS TEMPERATURE AND PRESSURE AT THE OBSERVATION
'            POINT. ALL OTHERS ARE BRIGHTNESS TEMPERATURES.
' TI(J)   = THE PHYSICAL TEMPERATURE AT MODEL HEIGHT
'            LEVEL HI(J).
' VD(J)    = VAPOR DENSITY (GM/M**3) AT THE MODEL HEIGHT LEVEL
'            HI(J).
' XKNU(I,J)= TOTAL ABSORPTION COEFFICIENT AT THE MODEL
'            HEIGHT HI(J) FOR FREQUENCY Frequency(I)
'****************************************************************
'****************************************************************
'            KEY NON-DIMENSIONED VARIABLES
'
' ABSOX  = ABSORPTION DUE TO OXYGEN (NEPERS/KM).
' ABSV   = ABSORPTION DUE TO WATER VAPOR (NEPERS/KM).
' Nlevr   = NUMBER OF RADIOSONDE HEIGHT LEVELS.
' NH     = NUMBER OF NOMINAL HEIGHT LEVELS.
' NraobMax    = MAXIMUM NUMBER OF RADIOSONDES FOR WHICH OBSERVABLES
'          AND RETRIEVABLES ARE TO BE COMPUTED.
' NTB    = NUMBER OF BRIGHTNESS TEMPERATURE OBSERVABLES.
' NTBP2  = TOTAL NUMBER OF OBSERVABLES (INCLUDING OBSERVATION
'          HEIGHT TEMPERATURE AND PRESSURE).
'****************************************************************
  Pz = fZtoP(z0)
  RecordStep = Val(txtRecordStep.Text)
  LR1 = Val(txtLR1.Text)  'see Calc_Hret_Tret
  LR2 = Val(txtLR2.Text)  'see Calc_Hret_Tret
  zLRb = Val(txtZb.Text)  'see Calc_Hret_Tret
  MinimumRAOBz = Val(txtRAOBmin.Text)
  PRmin = fZtoP(MinimumRAOBz)
' Open I/O files
  Region$ = Trim(txtRegion.Text) + Trim(Str(Val(txtRegionNumber.Text)))

  ROpath$ = txtRAOBpath.Text + "OBS\"
' Make \OBS\ directory if it doesn't exist
  If Dir(ROpath$, vbDirectory) = "" Then MkDir ROpath$
  ROlu = FreeFile
  ROfile$ = Region$ + "_" + Format$(z0 * 100, "0000") + ".OBS"

' Check if .OBS files already exists. If so, make sure it's ok to overwrite them.
  If Len(Dir(ROpath$ + ROfile$)) > 0 And Not Overwrite Then
    ans = MsgBox(ROpath$ + ROfile$ + " already exists!" + vbCrLf + "Do you really want to overwrite it?", vbYesNo)
    If ans = vbYes Then
      Overwrite = True
    Else
      End
    End If
  End If
  Open ROpath$ + ROfile$ For Output As ROlu
  txtROfile.Text = ROfile$

' Write header information to file
  fmt$ = "###0.00 #000 #00"
  V(1) = Pz: V(2) = NTB: V(3) = Nret
  Print #ROlu, fUsing(fmt$, V())

  CalcInit   'Get EL angles, IF frequencies and Weights
  Call CalcRetLevels(z0)        'Initialize retrieval pressure levels, Pret()
  Call SameFormat(ROlu, " ###0.00", 9, Pret!(), Nret)
  
  IKT = 1
  Cycle = 0
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'Main loop to read raobs and calculate TBs and retrievables
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
X = Rnd(-1) 'Initialize random number generator
Do
SkipRAOB:
  Call GetValidRAOB(RAOBlu, IKT, Cycle, RecordStep)
  If EOF(RAOBlu%) Then Exit Do
  If IKT = 56 Then
  IKT = IKT
  End If
  Tj = fLinterp(ZPraob(), TZraob(), Nlevr, z0)
  
'  If chkUseTwoSets.value = 1 Then 'Check if RAOBs are to be split into warm and cold sets
'    If Tavg >= 0 And Tj < Tavg Then GoTo SkipRAOB  'Cold RAOBs
'    If Tavg < 0 And Tj >= -Tavg Then GoTo SkipRAOB 'Warm RAOBs
'  End If
  If chkExcessGndT.Value = 1 Then ExcessGndT = ExcessTamplitude * Rnd(1) Else ExcessGndT = 0#

' Interpolate RAOBs at geometric retrieval levels wrt a/c for T and P
' Enter with Pz which has a different Hz for each RAOB
  Calc_Hret_Tret

' Do calculation for each LO frequency
  For M = 1 To Nlo
  ' Do calculation for each IF frequency at each LO
    For j = 1 To Nnu
      Fif(j) = LO(M) + LSBUSB(M, j)
    Next j
     
'   Nothing depended on elevation angle to this point
    ' Calculate NH geometric integration levels HI(i) wrt a/c
    Call CalcZgLevels(El(1), False)    'Depends only on sign of Elevation Angle
    ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
'    Call TPRPRFDC8(Nlevr)         'Needs HI() from CalcZgLevels
    Call TPRatZgLevels(Nlevr)         'Needs HI() from CalcZgLevels
    ' Calculate Absorption at each frequency in bandpass
    Call CalcBandTau(Fif())      'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
    ' Assume elevation angles go from + to -
    Plus = True
    For L = 1 To Nel      'Do all El's but horizon position
      If El(L) <> 0 Then
        If El(L) < 0 And Plus Then  'Elevation angle sign change! Need new HI() levels, etc
          Plus = False
        ' Calculate NH geometric integration levels HI(i) wrt a/c
          Call CalcZgLevels(El(L), False)
        ' Establish T,P,RH vs all height levels by interpolation of values at
        ' RAOB height levels.
        ' Call TPRPRFDC8(Nlevr)         'Needs HI() from CalcZgLevels
          Call TPRatZgLevels(Nlevr)         'Needs HI() from CalcZgLevels
        ' Calculate Absorption at each frequency in bandpass
          Call CalcBandTau(Fif())
        End If
      ' Calculate Brightness Temperatures along line of sight at each IF frequency
      ' and for each elevation angle
        Call CalcBandTBs(El(L), TB())
    
      ' Calculate Bandpass weighted TBs at each elevation.
        Sum = 0#
        For i = 1 To 2 * Nif
          Sum = Sum + BandWt(M, i) * TB(i)
        Next i
        TBvsEl((M - 1) * Nel + L) = Sum / BandSum(M)
        'Debug.Print Sum / BandSum(M)
      Else
        TBvsEl((M - 1) * Nel + L) = Tj     ' OAT
      End If
    'Debug.Print M; L; TBvsEl((M - 1) * Nel + L)
    Next L
  Next M
' Last observable is pressure at the a/c altitude.
'  TBvsEl((Nel - 1) * Nlo + 1) = Ti(NH)    ' OAT
  TBvsEl(Nel * Nlo + 1) = PrI(NH)   ' Pressure altitude

' Output retrievables and observables based on this RAOB
  V(1) = IKT: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihour
  fmt$ = "###00 ###0 #0 #0 #0"
  Print #ROlu%, fUsing(fmt$, V()) + " " + Format(WMOnumber, "00000") + " " + WMO4l
  Call SameFormat(ROlu, " ###0.00", 8, Tret!(), Nret)
  Call SameFormat(ROlu, " ###0.00", 8, TBvsEl!(), Nel * Nlo + 1)
  IKT = IKT + 1
Loop Until EOF(RAOBlu) Or IKT > NraobMax

Debug.Print "COMPLETION OF OBSERVABLE AND RETRIEVABLE CALCULATIONS FOR " + Str$(IKT) + "  RAOBS."
Debug.Print "Processing took from: " + StartTime$ + " To " + Str$(Time)
Close ROlu, RAOBlu
End Sub


Sub DoRetObsCalculationTBerr(z0!, TA!())
Dim OUTfile$, fmt$, jj%
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, M%, iL%, X%
Dim TRANGD!, Cycle&, Fif!(1 To 24), Plus As Boolean, RecordStep%
'****************************************************************
' MTPOBERU.FOR COMPUTES OXYGEN BAND BRIGHTNESS TEMPS.,
' AS VIEWED UPWARDLY FROM A SPECIFIED HEIGHT ABOVE SURFACE,
' FOR SELECTED FREQUENCY, ELEVATION ANGLE COMBINATIONS
' UTILIZING RADIOSONDE DATA AND ABSORPTION MODELS.
' PHYSICAL TEMPERATURES AT SELECTED RETRIEVAL HEIGHTS,
' AS WELL AS TEMPERATURE AND PRESSURE AT THE OBSERVING
' HEIGHT, ARE ALSO COMPUTED AND OUTPUTTED.
' UP TO 250 FREQUENCY, ELEVATION ANGLE COMBINATIONS MAY
' BE SPECIFIED.
' UPDATE OF FEB., 1998 FOR WAM CAMPAIGN
'****************************************************************
' Uses: MTPOBERU,OXLIEB93,fVLIEBE,TPRPRFDC8
'****************************************************************

Dim TBvsEl!(1 To 31), Sum!, Dfreq!, ROlu%
Dim jp1%, jm1%, NHm1%, V!(1 To 20), argUP!, argDN!

Dim Temperature!, Pressure!, RhoV!, f!, alpha!, alphaC!, Tbg!, Tj!
Dim IKT&

'******************************************************************
'                       ARRAY IDENTIFICATION
'
' DELHI(J) = THE NODE DISTANCE BETWEEN THE J-1 AND J LEVELS
'            AS MEASURED FROM THE OBSERVATION POINT, IN KM.
' Elevation(I) = ELEVATION ANGLE IN DEGREES OF Ith BRIGHTNESS TEMP.
'            OBSERVABLE. MEASURED ABOVE HORIZON.
' Frequency(I)  = FREQUENCY IN GHZ OF THE Ith BRIGHTNESS TEMPERATURE
'            OBSERVABLE.
' HI(J)    = NOMINAL HEIGHT LEVELS (IN KM) USED TO CALCULATE
'            BRIGHTNESS TEMPERATURES NUMERICALLY. THESE VALUES ARE
'            FIXED (RELATIVE TO THE OBSERVATION POINT) BY
'            THE NODE SPACINGS, DELHI(J).
' PrI(J)  = TOTAL PRESSURE (MB) AT THE JTH MODEL HEIGHT
'            LEVEL.
' RI(J)    = RELATIVE HUMIDITY (%) AT THE Jth MODEL HEIGHT
'            LEVEL.
' ZGraobJ)  = HEIGHT ABOVE SURFACE IN KM OF THE Jth RADIOSONDE
'            LEVEL. UP TO 80 RADIOSONDE LEVELS ARE PERMISSIBLE.
' PZraob(J) = PRESSURE (MB) AT THE JTH RADISONDE HEIGHT.
' RZraob(J) = RELATIVE HUMIDITY AT THE JTH RADIOSONDE HEIGHT.
' TZraob(J) = TEMPERATURE (K) AT THE Jth RADIOSONDE HEIGHT.
' TAU(I,J) = OPACITY AT FREQUENCY Frequency(I) BETWEEN THE
'            AIRCRAFT AND HEIGHT HI(J) (IN NEPERS).
' TB(J)    = THE Jth OBSERVABLE. THE LAST 2 OBSERVABLES ARE
'            ALWAYS TEMPERATURE AND PRESSURE AT THE OBSERVATION
'            POINT. ALL OTHERS ARE BRIGHTNESS TEMPERATURES.
' TI(J)   = THE PHYSICAL TEMPERATURE AT MODEL HEIGHT
'            LEVEL HI(J).
' VD(J)    = VAPOR DENSITY (GM/M**3) AT THE MODEL HEIGHT LEVEL
'            HI(J).
' XKNU(I,J)= TOTAL ABSORPTION COEFFICIENT AT THE MODEL
'            HEIGHT HI(J) FOR FREQUENCY Frequency(I)
'****************************************************************
'****************************************************************
'            KEY NON-DIMENSIONED VARIABLES
'
' ABSOX  = ABSORPTION DUE TO OXYGEN (NEPERS/KM).
' ABSV   = ABSORPTION DUE TO WATER VAPOR (NEPERS/KM).
' Nlevr   = NUMBER OF RADIOSONDE HEIGHT LEVELS.
' NH     = NUMBER OF NOMINAL HEIGHT LEVELS.
' NraobMax    = MAXIMUM NUMBER OF RADIOSONDES FOR WHICH OBSERVABLES
'          AND RETRIEVABLES ARE TO BE COMPUTED.
' NTB    = NUMBER OF BRIGHTNESS TEMPERATURE OBSERVABLES.
' NTBP2  = TOTAL NUMBER OF OBSERVABLES (INCLUDING OBSERVATION
'          HEIGHT TEMPERATURE AND PRESSURE).
'****************************************************************
 Pz = fZtoP(z0)
  TAmax = 0: TAmin = 500
  CalcInit                      'Get EL angles, IF frequencies and Weights
  
  Call CalcRetLevels(z0)        'Initialize retrieval pressure levels wrt A/C z0, Pret(i), i=1 - 33
  
' Interpolate RAOBs at geometric retrieval levels wrt A/C for Tret and Hret (geometric altitude)
' Enter with Pz which has a different Hz and Hret set for each RAOB
  Calc_Hret_Tret

' Do calculation in order which avoids recalculating optical depths unnecessarily
' First, do calculation for each LO frequency
  For M = 1 To Channels
  ' Do calculation for each IF frequency at each LO
    For j = 1 To Nnu
      Fif(j) = LO(M) + LSBUSB(M, j)
    Next j
'   Nothing depended on elevation angle to this point
    
    ' Calculate NH geometric integration levels HI(i) wrt a/c
    Call CalcZgLevels(El(1), False)         'Depends only on sign of Elevation Angle
    ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
    Call TPRatZgLevels(Nlevr)         'Needs HI() from CalcZgLevels
    ' Calculate Absorption at each frequency in bandpass
    Call CalcBandTau(Fif())           'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
    ' Assume elevation angles go from + to -
    Plus = True
    For L = 1 To Nel
      If El(L) < 0 And Plus Then      'Elevation angle sign change! Need new HI() levels, etc
        Plus = False                  'Only go thru here on elevation angle sign change
      ' Calculate NH geometric integration levels HI(i) wrt a/c
        Call CalcZgLevels(El(L), False)     'Depends only on sign of Elevation Angle
      ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
        Call TPRatZgLevels(Nlevr)     'Needs HI() from CalcZgLevels
      ' Calculate Absorption at each frequency in bandpass
        Call CalcBandTau(Fif())       'Needs NH, PI(), TI(), RI()
      End If
      
      If El(L) = 0 Then
        TA(M, L) = Ti(NH)             'Use OAT, corresponds to L=10!
      Else
      ' Calculate TBs along line of sight at each IF frequency and for each elevation angle
        Call CalcBandTBs(El(L), TB())
      ' Calculate Bandpass weighted TBs at each elevation.
        Sum = 0#
        For i = 1 To 2 * Nif: Sum = Sum + BandWt(M, i) * TB(i): Next i
        TA(M, L) = Sum / BandSum(M)
      End If
      
      DoEvents
    Next L
  Next M
  
  If LocHor = 0 Then LocHor = 6
  OAT = TA(1, 10)
  For i = 1 To Channels
    For j = LocHor + 1 To Nel
      TA(i, j) = TA(i, j - 1)
    Next j
    TA(i, LocHor) = OAT
  Next i
  
End Sub


Sub DrawBandpass()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, A!
Dim Xmin!, Xmax!, Ymin!, Ymax!, r%, iStep%
'  Latitude = NAVV(3, r)
'  Longitude = NAVV(4, r)
  If IFoff(1, 1) < 0.1 Then
    Xmin = 0
    Xmax = 250
  Else
    Xmin = 200
    Xmax = 450
  End If
  Ymin = 0
  Ymax = 1

'Plot Flight Track in picture window
  With picBandpass
    .Cls
    .ScaleMode = 0
    .ScaleLeft = Xmin
    .ScaleWidth = Xmax - Xmin
    .ScaleTop = Ymax
    .ScaleHeight = Ymin - Ymax
  End With
  
' Draw y marks
  For A = 0 To 1 Step 0.2
    picBandpass.PSet (Xmin, A)
    picBandpass.Line -(Xmax, A), QBColor(0)
  Next A
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
' Draw X marks
  For i = Xmin To Xmax Step 50
    picBandpass.PSet (i, Ymin)
    picBandpass.Line -(i, Ymax), QBColor(0)
  Next i

' Draw bandpass
  For i = 1 To Nlo
    picBandpass.PSet (IFoff(i, 1), IFwt(i, 1))
    Debug.Print IFoff(i, 1), IFwt(i, 1)
    For j = 1 To Nif
      picBandpass.Line -(1000 * IFoff(i, j), IFwt(i, j)), QBColor(9) 'And continue from there
    Next j
  Next i
End Sub

Function fElapsedSeconds!(start As Boolean, Index%)
Static StartTime(3) As Date, seconds&

If start Then StartTime(Index) = Time: Exit Function
seconds = (Time - StartTime(Index)) * 86400#
If seconds < 0 Then seconds = seconds + 86400
fElapsedSeconds = seconds

End Function


Function fGetPaltIndex(z0) As Integer
Dim i%
  For i = 1 To lstPalt.ListCount  'Do Retrieval altitudes
    If z0 = Val(lstPalt.List(i - 1)) Then Exit For
  Next i
  fGetPaltIndex = i
End Function


Sub ChangeOBrmsTexAQS(z0!, pALTindex%)
Dim s0!, S1!, s2!, i%, j%
' Weight downward observables for TexAQS 2000
' Look distance on ground= 819, 500, 348 meters

'  Tc = 200 * 0.819 / (Exp(z0 / 8) * kft_km)  55.51 GHz
'  OBrms(6) = Tc * Sin(12 * cPI / 180): OBrms(7) = Tc * Sin(25 * cPI / 180): OBrms(8) = Tc * Sin(52 * cPI / 180): OBrms(9) = Tc * Sin(80 * cPI / 180)
'  Tc = 200 * 0.5 / (Exp(z0 / 8) * kft_km)    56.65 GHz
'  OBrms(15) = Tc * Sin(12 * cPI / 180): OBrms(16) = Tc * Sin(25 * cPI / 180): OBrms(17) = Tc * Sin(52 * cPI / 180): OBrms(18) = Tc * Sin(80 * cPI / 180)
'  Tc = 200 * 0.348 / (Exp(z0 / 8) * kft_km)  58.80 GHz
'  OBrms(24) = Tc * Sin(12 * cPI / 180): OBrms(25) = Tc * Sin(25 * cPI / 180): OBrms(26) = Tc * Sin(52 * cPI / 180): OBrms(27) = Tc * Sin(80 * cPI / 180)
'If number below is >1 then <5 e-folding distances are reached
'    10   300   600   900  1500  2100  2700  3400  4600  6100     FL(meters)
' 85.14  2.92  1.51  1.03  0.66  0.50  0.42  0.36  0.30  0.28     El=-12 deg
'173.06  5.94  3.06  2.10  1.34  1.02  0.85  0.73  0.62  0.56        -25
'274.01  9.40  4.85  3.33  2.13  1.62  1.34  1.15  0.98  0.89        -42
'403.28  13.84 7.14  4.90  3.13  2.38  1.98  1.70  1.44  1.31        -80

    s0 = 1.2: S1 = 500 / 819: s2 = 350 / 819 'Scale at each frequency (55.51,56.65, 58.8)
    Select Case z0
    Case 0.01
      j = 10
      OBrms(j, 6) = s0 * 85.14: OBrms(j, 15) = OBrms(j, 6) * S1: OBrms(j, 24) = OBrms(j, 6) * s2
      OBrms(j, 7) = s0 * 173.06: OBrms(j, 16) = OBrms(j, 7) * S1: OBrms(j, 25) = OBrms(j, 7) * s2
      OBrms(j, 8) = s0 * 274.01: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 403.28: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 0.3
      j = 9
      OBrms(j, 6) = s0 * 2.92: OBrms(j, 15) = OBrms(j, 6) * S1: OBrms(j, 24) = OBrms(j, 6) * s2
      OBrms(j, 7) = s0 * 5.94: OBrms(j, 16) = OBrms(j, 7) * S1: OBrms(j, 25) = OBrms(j, 7) * s2
      OBrms(j, 8) = s0 * 9.4: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 13.84: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 0.61
      j = 8
      OBrms(j, 6) = s0 * 1.51: OBrms(j, 15) = OBrms(j, 6) * S1: OBrms(j, 24) = OBrms(j, 6) * s2
      OBrms(j, 7) = s0 * 3.06: OBrms(j, 16) = OBrms(j, 7) * S1: OBrms(j, 25) = OBrms(j, 7) * s2
      OBrms(j, 8) = s0 * 4.85: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 7.14: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 0.9
      j = 7
      OBrms(j, 6) = s0 * 1.03: OBrms(j, 15) = OBrms(j, 6) * S1: OBrms(j, 24) = OBrms(j, 6) * s2
      OBrms(j, 7) = s0 * 2.1: OBrms(j, 16) = OBrms(j, 7) * S1: OBrms(j, 25) = OBrms(j, 7) * s2
      OBrms(j, 8) = s0 * 3.33: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 4.9: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 1.5
      j = 6
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0 * 1.34: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0 * 2.13: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 3.13: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 2.1
      j = 5
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0 * 1.02: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0 * 1.62: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 2.38: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 2.7
      j = 4
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0 * 1.62: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 2.38: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 3.4
      j = 3
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0 * 1.15: OBrms(j, 17) = OBrms(j, 8) * S1: OBrms(j, 26) = OBrms(j, 8) * s2
      OBrms(j, 9) = s0 * 1.7: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 4.6
      j = 2
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0: OBrms(j, 17) = OBrms(j, 8): OBrms(j, 26) = OBrms(j, 8)
      OBrms(j, 9) = s0 * 1.44: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
    Case 6.1
      j = 1
      OBrms(j, 6) = s0: OBrms(j, 15) = OBrms(j, 6): OBrms(j, 24) = OBrms(j, 6)
      OBrms(j, 7) = s0: OBrms(j, 16) = OBrms(j, 7): OBrms(j, 25) = OBrms(j, 7)
      OBrms(j, 8) = s0: OBrms(j, 17) = OBrms(j, 8): OBrms(j, 26) = OBrms(j, 8)
      OBrms(j, 9) = s0 * 1.31: OBrms(j, 18) = OBrms(j, 9) * S1: OBrms(j, 27) = OBrms(j, 9) * s2
  Case Else
    j = fGetPaltIndex(z0)
  End Select
  
  pALTindex = j

  For i = 1 To 30
    If OBrms(pALTindex, i) < 0.3 Then OBrms(pALTindex, i) = 0.3
  Next i

End Sub


Sub GetValidRAOB(RAOBlu%, IKT&, Cycle&, RecordStep%)
Dim seconds&, i%

If IKT = 0 Then StartTime$ = Time
NextRAOB:
      If EOF(RAOBlu%) Then Exit Sub
  seconds = fElapsedSeconds(False, 1)
  Select Case seconds
  Case Is < 60  'Show seconds
    lblET.Caption = "Elapsed Time [s]"
    txtET.Text = Format$(seconds, "00")
  Case Is < 3600 'Show minutes
    lblET.Caption = "Elapsed Time [m]"
    txtET.Text = Format$(seconds / 60#, "#0.0")
  Case Else     'Show hours
    lblET.Caption = "Elapsed Time [h]"
    txtET.Text = Format$(seconds / 3600#, "#0.00")
  End Select
'  ProgressBar1.value = ProgressBar1.value + 1
  
  Cycle = Cycle + RecordStep
  txtNraob.Text = Str$(Cycle)   'Was IKT before RecordStep
  DoEvents
  
  Call RAOBread(RAOBlu, Cycle)
  If EOF(RAOBlu) Then Cycle = Cycle - RecordStep: Exit Sub
  If UseEditLevel And EditLevel Then   'Edit levels
    For i% = 1 To Nedit
      TZraob(LZraob(i%)) = TZraob(Nlevr + i) + RAOBbias
    Next i%
  End If

' Change Zraob to ZGraob and Praob to PZraob Traob to Z
' CHECK IF RADIOSONDE DATA REACHES PRMIN PRESSURE
  If Not RAOBgood Then GoTo NextRAOB
  If Nlevr < 10 Then GoTo NextRAOB
'  If (PZraob(Nlevr) >= PRmin) Then GoTo NextRAOB
  'If (ZGraob(Nlevr) >= Htop) Then GoTo NextRAOB
  'If (Imonth < MonthMin) Then GoTo NextRAOB
  'If (Imonth > MonthMax) Then GoTo NextRAOB
  If ZPraob(Nlevr) < Zr(1) Then GoTo NextRAOB  'ZPraob(1) > Zr(NFL) Or
  
  ' Convert Geopotential Height to Geometric Height
  For i = 1 To Nlevr
    ZGraob(i) = fZghToZgWGS(ZGraob(i), WMOlatitude)
    TZraob(i) = TZraob(i) + RAOBbias   'Add bias if necessary
  Next i
  
End Sub







Sub Main()
Dim i%, j%, z0!, NFlu%, RAOBlu%, RecordStep%, FileRecords&, Filename$
Dim f As Form

  Set f = RCmain
  ExcessTamplitude = Val(txtExcessTamplitude.Text)
  RAOBbias = Val(txtRAOBbias.Text)
  
  RecordStep = Val(txtRecordStep.Text)
  RAOBfilename$ = RTpath$ + cboRAOBfiles.Text
  Select Case fExtension(UCase$(RAOBfilename$))
  Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
  Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
  Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
  Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
  End Select
  
  Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
  ProgressBar1.Min = 0
  ProgressBar1.Max = NFL * FileRecords + 10
  seconds = fElapsedSeconds!(True, 1)  'Start time counter #1
  
  For i = 1 To lstPalt.ListCount       'Do Retrieval altitudes
    z0 = Val(lstPalt.List(i - 1))
    If chkRegressionOnly.Value = 0 Then
      Close RAOBlu
      Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
      ProgressBar1.Value = (i - 1) * FileRecords
      txtProgress.Text = FileRecords
'     Calculate observables
      Select Case RCformatUsed
      Case 2: DoRetObsCalculationMJ z0, RAOBlu
      Case 3: DoRetObsCalculationMJ2 z0, RAOBlu
      End Select
      Close RAOBlu
    Else
      UpdateParameters
    End If
'   Do regression to calculate retrieval coefficients (RCs)
    Select Case RCformatUsed
    Case 2: DoRegressionMJ z0
    Case 3: DoRegressionMJ2 z0
    End Select
  Next i

  Call CalculateSmatrix        'Calculate Sensitivity Matrix
  Call WriteRCconfig(f)        'Write CFG file
' Write binary format RC file
  Filename$ = MNpath$ + "RC\" + txtRCfile.Text
  Filename$ = fReplaceExtension(Filename$, "RCF")
  Call RCwrite(0, Filename$)

End Sub
Sub MainAll()
' This routine calculates RCs for ALL RAOB2 files in Templates folder
Dim i%, j%, k%, kk%, z0!, NFlu%, RAOBlu%, RecordStep%, FileRecords&, Filename$, A$
Dim f As Form, RegionInitial As String, RegionCurrent$, RegionLeft$, RegionRight$, iL%, ir%
Dim INPlu%, Line$

  Set f = RCmain
  ExcessTamplitude = Val(txtExcessTamplitude.Text)
  RAOBbias = Val(txtRAOBbias.Text)
  
  RecordStep = Val(txtRecordStep.Text)
  
' Before running this program, use the FLTinfo Ad Hoc tab Template Map button
' to generate a fresh Mission/RAOB/Templates/TemplateMap.txt file
' Note that existing RCF files in Mission/RC folder will be overwritten if they are
' not saved after generating the TemplateMap.txt file

  k = 1
  INPlu = FreeFile
  Open RTpath$ + "TemplateMap.txt" For Input As INPlu
  Line Input #INPlu, Line$
'1234567890123456789012345678901234567890
'35 FJ - SGF___2008062700.RAOB2 133 +2.0 +2.0 24.0

  Do While Not EOF(INPlu)
    txtRegion.Text = Mid$(Line$, 4, 2)
    txtNtemplates = Str(k)
    RAOBfilename$ = RTpath$ + Mid$(Line$, 9, 23)
    cboRAOBfiles.Text = RAOBfilename$
    Select Case fExtension(UCase$(RAOBfilename$))
    Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
    Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
    Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
    Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
    End Select

'    Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
'    ProgressBar1.Min = 0
'    ProgressBar1.Max = NFL * FileRecords + 10
'    seconds = fElapsedSeconds!(True, 1)  'Start time counter #1
'    If k > 0 Then
'      ir = ir + 1
'      If ir > 90 Then ir = 65: iL = iL + 1
'      txtRegion.Text = Chr(iL) & Chr(ir)
'    End If
    For i = 1 To lstPalt.ListCount       'Do Retrieval altitudes
      z0 = Val(lstPalt.List(i - 1))
      If chkRegressionOnly.Value = 0 Then
'        Close RAOBlu
        Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
        ProgressBar1.Value = (i - 1) * FileRecords
        txtProgress.Text = FileRecords
'       Calculate observables
        Select Case RCformatUsed
        Case 2: DoRetObsCalculationMJ z0, RAOBlu
        Case 3: DoRetObsCalculationMJ2 z0, RAOBlu
        End Select
        Close RAOBlu
      Else
        UpdateParameters
      End If
'     Do regression to calculate retrieval coefficients (RCs)
      Select Case RCformatUsed
      Case 2: DoRegressionMJ z0
      Case 3: DoRegressionMJ2 z0
      End Select
    Next i

    Call CalculateSmatrix        'Calculate Sensitivity Matrix
    Call WriteRCconfig(f)        'Write CFG file
'   Write binary format RC file
    Filename$ = MNpath$ + "RC\" + txtRCfile.Text
    Filename$ = fReplaceExtension(Filename$, "RCF")
    Call RCwrite(0, Filename$)
    
    Line Input #INPlu, Line$
    k = k + 1
  Loop
  Close INPlu
  
End Sub
Sub MainAll2()
' This routine calculates RCs for ALL RAOB2 files in Templates folder
Dim i%, j%, k%, kk%, z0!, NFlu%, RAOBlu%, RecordStep%, FileRecords&, Filename$, A$
Dim f As Form, RegionInitial As String, RegionCurrent$, RegionLeft$, RegionRight$, iL%, ir%
Dim INPlu%, Line$

  Set f = RCmain
  ExcessTamplitude = Val(txtExcessTamplitude.Text)
  RAOBbias = Val(txtRAOBbias.Text)
  
  RecordStep = Val(txtRecordStep.Text)
  
' Before running this program, use the FLTinfo Ad Hoc tab Template Map button
' to generate a fresh Mission/RAOB/Templates/TemplateMap.txt file
' Note that existing RCF files in Mission/RC folder will be overwritten if they are
' not saved after generating the TemplateMap.txt file

  RegionInitial = txtRegion.Text
  RegionLeft$ = Left$(RegionInitial, 1)
  RegionRight$ = Right$(RegionInitial, 1)
  iL = Asc(RegionLeft$)
  ir = Asc(RegionRight$)
  txtNtemplates = Str(cboRAOBfiles.ListCount)
  ProgressBar1.Min = 0
  ProgressBar1.Max = 1
  For k = 0 To cboRAOBfiles.ListCount - 1
    cboRAOBfiles.Text = cboRAOBfiles.List(k)
    RAOBfilename$ = RTpath$ + cboRAOBfiles.Text
    Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
    ProgressBar1.Max = NFL * FileRecords + ProgressBar1.Max
  Next k
  ProgressBar1.Value = 0
  For k = 0 To cboRAOBfiles.ListCount - 1
    cboRAOBfiles.Text = cboRAOBfiles.List(k)
    RAOBfilename$ = RTpath$ + cboRAOBfiles.Text
  
    Select Case fExtension(UCase$(RAOBfilename$))
    Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
    Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
    Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
    Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
    End Select

    seconds = fElapsedSeconds!(True, 1)  'Start time counter #1
    If k > 0 Then   'k=0 on first pass
      ir = ir + 1
      If ir > 90 Then ir = 65: iL = iL + 1   'ASC("A")=65
      txtRegion.Text = Chr(iL) & Chr(ir)
    End If
    
    For i = 1 To lstPalt.ListCount       'Do Retrieval altitudes
       z0 = Val(lstPalt.List(i - 1))
        If chkRegressionOnly.Value = 0 Then
        Call OpenRAOBfile(RAOBfilename$, RAOBlu%, FileRecords&)
        ProgressBar1.Value = ProgressBar1.Value + FileRecords
        txtProgress.Text = FileRecords
'       Calculate observables
        Select Case RCformatUsed
        Case 2: DoRetObsCalculationMJ z0, RAOBlu
        Case 3: DoRetObsCalculationMJ2 z0, RAOBlu
        End Select
        Close RAOBlu
      Else
        UpdateParameters
      End If
'     Do regression to calculate retrieval coefficients (RCs)
      Select Case RCformatUsed
      Case 2: DoRegressionMJ z0
      Case 3: DoRegressionMJ2 z0
      End Select
    Next i

    Call CalculateSmatrix        'Calculate Sensitivity Matrix
    Call WriteRCconfig(f)        'Write CFG file
'   Write binary format RC file
    Filename$ = MNpath$ + "RC\" + txtRCfile.Text
    Filename$ = fReplaceExtension(Filename$, "RCF")
    Call RCwrite(0, Filename$)
    
  Next k
  Close INPlu
  
End Sub




Sub OpenRAOBfile(Filename$, RAOBlu%, FileRecords&)

  RAOBlu% = FreeFile
  Select Case RAOBtype
  Case 0
    Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
    FileRecords = LOF(RAOBlu%) \ Len(RAOB2)
  Case 1
    Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB)
    FileRecords = LOF(RAOBlu%) \ Len(RAOB)
  Case 2
    Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(LRF)
    FileRecords = LOF(RAOBlu%) \ Len(LRF)
  End Select

End Sub


Sub ReadArcAvg_OBS_ATP(Filename$, Orms!(), Oavg!(), Nobs%, Zp!(), Pz!(), TZ!(), Zg!(), Nret%)
Dim lu%, i%, j%, k%, L%, filen!, bpv!, site$, A$, rc!(1 To 33, 1 To 30)
Dim BP!(1 To 33), RTav!(1 To 33), RAav!(1 To 33), RMSa!(1 To 33), RMSe!(1 To 33)

  lu% = FreeFile
  Open Filename$ For Input As #lu%
  Input #lu%, filen!, bpv!, RAOBcount%, site$
  Input #lu%, A$
  For j = 1 To Nobs: Input #lu%, Orms(j): Next j
  For j = 1 To Nobs: Input #lu%, Oavg(j): Next j
  For k = 1 To Nret
    Input #lu%, BP(k), RTav(k), RMSa(k), RMSe(k)
    RAav(k) = fPtoZ(BP(k))
    For L = 1 To Nobs
      Input #lu%, rc(k, L)
     'Debug.Print k; L; rc(k, L)
    Next L
    Zp(k) = RAav(k)
    TZ(k) = RTav(k)
    Pz(k) = fZtoP(Zp(k))
    If k = 1 Then
      Zg(k) = Zp(k)
    Else
      Zg(k) = Zg(k - 1) + fHypsometricEqn(TZ(k - 1), Pz(k - 1), TZ(k), Pz(k))
    End If
  Next k
  Close #lu%
  DoEvents
End Sub

Sub UpdateDrive(Drive$)
Dim i%, j%
j% = Len(Drive$)
i% = Len(txtRCfile.Text) - j%
txtRCfile.Text = Drive$ + Right$(txtRCfile.Text, i%)
i% = Len(cboRAOBfiles.Text) - j%
cboRAOBfiles.Text = Drive$ + Right$(cboRAOBfiles.Text, i%)
i% = Len(txtROfile.Text) - j%
txtROfile.Text = Drive$ + Right$(txtROfile.Text, i%)
End Sub

Sub UpdateForm()
Dim i%, ans As Variant

  MinimumRAOBz = Val(txtRAOBmin.Text)
  PRmin = fZtoP(MinimumRAOBz)
  
  For i = 0 To lstLO.ListCount - 1
      LO(i + 1) = Val(lstLO.List(i))
  Next i
  SURC = cboSU.Text
  ROfile$ = txtRCfile.Text + txtRegion.Text + "_" + ALTs$ + ".OBS"
  txtROfile.Text = ROfile$
  NTB = Nlo * Nnu * Nel    'Number of freq, IF, and el angle combos
  
End Sub




Sub UpdateParameters()
  
  RecordStep = Val(txtRecordStep.Text)
  LR1 = Val(txtLR1.Text)  'see Calc_Hret_Tret
  LR2 = Val(txtLR2.Text)  'see Calc_Hret_Tret
  zLRb = Val(txtZb.Text)  'see Calc_Hret_Tret
  MinimumRAOBz = Val(txtRAOBmin.Text)        'Minimum acceptable RAOB altitude
  ExcessTamplitude = Val(txtExcessTamplitude.Text)    'Random Excess Noise Level on Ground
  RCformat(0) = Val(txtRCformat.Text)

End Sub


Sub VaryLOoverIF(z0!)
Dim OUTfile$, RAOBfile$, fmt$, jj%
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, iL%
Dim TRANGD!, M%
Dim TB!(504), VD!(50), XKnu!(504, 50), Hz!, Pz!, TZ!
Dim Tau!(504, 50), DelHP!(50), Hret!(50), Tret!(50)
Dim TBvsEl!(21)
Dim ROlu%, RAOBlu%, cPI!
Dim jp1%, jm1%, NHm1%, V!(1 To 20), argUP!, argDN!, Tavg!
Dim Sum!, Dfreq!
Dim Temperature!, Pressure!, RhoV!, f!, alpha!, alphaC!, Tbg!, Nnn%
Dim IKT&, Iyr%, Imth%, Ihr%, Idum%, ilo%, LO!(1 To 3)

'******************************************************************
  Pz = fZtoP(z0)
  Nnn = 21                      'Number of LO steps
  For M = 1 To Nlo
  For i = 1 To Nif
    BandWt(M, i) = IFwt(M, Nif - i + 1)
    BandWt(M, 2 * Nif - i + 1) = IFwt(M, Nif - i + 1)
  Next i
  BandSum(M) = 0#
  For i = 1 To 2 * Nif: BandSum(M) = BandSum(M) + BandWt(M, i): Next i

' Calculate the Nif x 2 Side Band frequencies
  For j = 1 To Nif
    LSBUSB(M, j) = -IFoff(M, Nif + 1 - j)
    LSBUSB(M, 2 * Nif + 1 - j) = IFoff(M, Nif + 1 - j)
  Next j
  Next M
'****************************************************************
    For i = 1 To 10: DelHI(i) = 0.1: Next i
    For i = 11 To 20: DelHI(i) = 0.2: Next i
    For i = 21 To 25: DelHI(i) = 0.3: Next i
    For i = 26 To 30: DelHI(i) = 0.5: Next i
    For i = 31 To 33: DelHI(i) = 1#: Next i
    For i = 34 To 37: DelHI(i) = 2#: Next i
    For i = 38 To 50: DelHI(i) = 4#: Next i
'****************************************************************
'  OPEN I/O FILES.
'****************************************************************
  RAOBlu = FreeFile
'  Open cboRAOBfiles.Text For Input As RAOBlu
   Open cboRAOBfiles.Text For Random Access Read Write As RAOBlu Len = Len(RAOB)
 
  ROlu = FreeFile
  ROfile$ = txtRCfile.Text + txtRegion.Text + "LO" + Format$(z0, "00") + "." + Mission$
  Open ROfile$ For Output As ROlu
  txtROfile.Text = ROfile$

'****************************************************************
' Use Standard Atmosphere for T, P and RhoV
'****************************************************************
StartTime$ = Time
  IKT = 1
  Cycle = 0
NextRAOB:
  DoEvents
  If (IKT > NraobMax) Then GoTo 999
  If EOF(RAOBlu%) Then GoTo 999
  Cycle = Cycle + 1
'  Call RAOBread(RAOBlu, Cycle)
'  Input #RAOBlu, Idum, Iyr, Imth, Iday, Ihr, Nlevr  'END=999
'  For j = 1 To Nlevr
'     Input #RAOBlu, Zraob(j), Praob(j), Traob(j), RHraob(j)
'  Next j
' CHECK IF RADIOSONDE DATA REACHES PRMIN PRESSURE
  If (PZraob(Nlevr) >= PRmin) Then GoTo NextRAOB
  If (ZGraob(Nlevr) >= Htop) Then GoTo NextRAOB
  If (Imonth < MonthMin) Then GoTo NextRAOB
  If (Imonth > MonthMax) Then GoTo NextRAOB

'****************************************************************
' DETERMINE AIRCRAFT HEIGHT (HZ, geo!) AND TEMPERATURE RETRIEVAL
' HEIGHTS (HRET(J=1,Nret)) BY INTERPOLATION BETWEEN RAOB LEVELS.
'*****************************************************************
' AIRCRAFT HEIGHT AND TEMPERATURE
  j = 1
  Do: j = j + 1: If j > Nlevr Then GoTo 705
  Loop Until Pz >= PZraob(j)
      
'  Hz = fZinterp(j, Pz)
'  Tz = fTinterp(j, Hz)
  GoTo 710
      
705:
  alphaC = 0.02927 * Log(Pz / PZraob(Nlevr))
  Hz = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
  TZ = TZraob(Nlevr) + 2# * (Hz - ZGraob(Nlevr)) ' 2 K/KM GRADIENT ABOVE RAOB

710:
' RETRIEVAL HEIGHTS AND TEMPERATURES
  j = 1
  i = 1
720:
  j = j + 1
  If j > Nlevr Then GoTo 725

722:
  If Pret(i) >= PZraob(j) Then GoTo 721
  GoTo 720

721:
'  Hret(i) = fZinterp(j, Pret(i))
'  Tret(i) = fTinterp(j, Hret(i))
  i = i + 1
  If i > Nret Then GoTo 730
  GoTo 722

725:
  Do
    alphaC = 0.02927 * Log(Pret(i) / PZraob(Nlevr))
    Hret(i) = ZGraob(Nlevr) - alphaC * TZraob(Nlevr) / (1# + 0.5 * alphaC)
    Tret(i) = TZraob(Nlevr) + 2# * (Hret(i) - ZGraob(Nlevr))
    i = i + 1
'  If i > Nret Then GoTo 730
  Loop Until i > Nret

730:

' ****************************************************************
' DETERMINE HEIGHTS ABOVE A/C TO BE USED IN NUMERICAL TB CALCULATIONS.
' HEIGHTS ABOVE HTOP KM ARE NOT USED.
' ****************************************************************
    HI(1) = Hz
    For j = 2 To 50
      HI(j) = HI(j - 1) + DelHI(j - 1)
      If (HI(j) >= Htop) Then Exit For
    Next j
    NH = j
    NHm1 = NH - 1

' ESTABLISH T,P,RH VS. ALL HEIGHT LEVELS BY INTERPOLATION
' OF VALUES AT RAOB HEIGHT LEVELS.
'****************************************************************

'  Call TPRPRFDC8(Nlevr, NH)
' Debug.Print "YYY"; PrI(1); TI(1)
'***************************************************************
' CONVERT TEMP, RELATIVE HUMIDITY PROFILE TO VAPOR DENSITY PROFILE.
'****************************************************************
  For j = 1 To NH
    Tcal = Ti(j) - 273#
    Ecal = (26# + Tcal) / 33# - 1# * ((Tcal - 7#) / 90#) ^ 2
    VD(j) = (2.17 * Ri(j) * 10# ^ Ecal) / Ti(j)
  Next j
'****************************************************************
' CALCULATE ABSORPTION COEFFICIENT PROFILE.
'****************************************************************
For ilo = 1 To 3          'Loop thru LO frequencies
  LO(ilo) = Val(lstLO.List(ilo - 1))
  For j = 1 To Nnn        'LO step
    For i = 1 To 2 * Nif  'IF steps
'      Frequency((j - 1) * 2 * Nif + i) = LO(ilo) + 0.005 * (j - 11) + USBLSB(i) 'Step LO +- 50 MHz
    Next i
  Next j
  fmt$ = "#0.0 ###0.00 #0.000"
  V(1) = z0: V(2) = Pz: V(3) = LO(ilo)
  Print #ROlu, fUsing(fmt$, V())

  For i = 1 To Nnn * 2 * Nif
'    f = Frequency(i)
    For j = 1 To NH
      Pressure = PrI(j): Temperature = Ti(j): RhoV = VD(j)
      ABSox = OXLIEB93(Temperature, Pressure, RhoV, f)
      ABSv = fVliebe(Temperature, Pressure, RhoV, f)
      XKnu(i, j) = ABSox + ABSv
    Next j
    txtProgress.Text = Format$(100# * i / (Nnn * 2 * Nif), "#00")
    DoEvents
  Next i
'****************************************************************
' COMPUTE OPACITY PROFILES.
'****************************************************************
' Do i=1 case separately
    Tau(1, 1) = 0#
    For j = 2 To NH
      jm1 = j - 1
      Tau(1, j) = Tau(1, jm1) + 0.5 * (XKnu(1, j) + XKnu(1, jm1)) * (HI(j) - HI(jm1))
    Next j

    For i = 2 To Nnn * 2 * Nif
      Tau(i, 1) = 0#
      For j = 2 To NH
        jm1 = j - 1
        Tau(i, j) = Tau(i, jm1) + 0.5 * (XKnu(i, j) + XKnu(i, jm1)) * (HI(j) - HI(jm1))
      Next j
    Next i
'****************************************************************
' CALCULATE BRIGHTNESS TEMPERATURES.
'****************************************************************

    For i = 1 To Nnn * 2 * Nif
      Sum = 0#
      For j = 1 To NH - 1
        Sum = Sum + (Ti(j) + Ti(j + 1)) * (Exp(-Tau(i, j)) - Exp(-Tau(i, j + 1))) / 2
      Next j

      TB(i) = Sum + 2.75 * Exp(-Tau(i, NH))  'Add CMB
'      If i < 6 Then Debug.Print i; Elevation(i); TB(i); 2.75 * Exp(-Tau(i, NH) / SinEl)
    Next i
'''''''''''''''''''''''
' CALCULATE BANDPASS WEIGHTED TBS AT EACH ELEV.
  For L = 1 To Nnn
    Sum = 0#
    For i = 1 To 2 * Nif
      iL = (L - 1) * 2 * Nif + i
      Sum = Sum + BandWt(ilo, i) * TB(iL)
    Next i
    TBvsEl(L) = Sum / BandSum(ilo)
  Next L
' LAST 2 OBSERVABLES ARE TEMPERATURE AND PRESSURE AT THE A/C ALTITUDE.
'    TB(NTBp1) = TI(1)
'    TB(NTBp2) = PrI(1)
'****************************************************************
' FILE OUTPUT
'****************************************************************
'  v(1) = IKT: v(2) = Iyr: v(3) = Imth: v(4) = Iday: v(5) = Ihr
'  fmt$ = "###00 #0 #0 #0 #0"
'  Print #ROlu%, fUsing(fmt$, v())
'  If UP Then Call SameFormat(ROlu, " ###0.00", 8, Tret!(), Nret)
  'TBvsEl(Nnn + 1) = z0
  'TBvsEl(Nnn + 2) = LO(ilo)
  For i = 1 To Nnn
  Sum = LO(ilo) + 0.005 * (i - 11)
  Print #ROlu, Format$(Sum, "#0.000") + vbTab + Format$(TBvsEl(i), "#00.00")
  Next i
Next ilo
  
'  GoTo NextRAOB

999:
  Debug.Print "COMPLETION OF OBSERVABLE AND RETRIEVABLE CALCULATIONS FOR " + Str$(IKT) + "  RAOBS."
  Debug.Print "Processing took from: " + StartTime$ + " To " + Str$(Time)
  Close ROlu
  Close RAOBlu

End Sub




Private Sub cboMissions_Click()

  Mission$ = Trim(cboMissions.Text)
  If Dir(RCpath$, vbDirectory) = "" Then Call CreateDirectoryTree(RCpath$, True)
  cmdLoadRaobFiles_Click (0)
  DoEvents
  
End Sub


Sub cboMode_Click()
Dim FilePath$, FileMask$, ConvertInFile$

Select Case cboMode.ListIndex
Case 0
Case 1
' Get all RAOB template files and load into combo box (if there are any)
  cboRAOBfiles.Clear
  FilePath$ = RTpath$
  FileMask$ = "?????_*.RAOB2"
  ConvertInFile$ = Dir(FilePath$ + FileMask, vbNormal)
' Check that at least one file exists
  If ConvertInFile$ = "" Then
    MsgBox "No files were found in path: " + FilePath$ + vbCrLf + "Satisfying the mask: " + FileMask + vbCrLf + "This is not essential to calculate RCs!"
    Exit Sub
  End If
' One does, so now check the rest of the directory
  Do
    cboRAOBfiles.AddItem ConvertInFile$
    ConvertInFile$ = Dir
  Loop Until ConvertInFile$ = ""
  cboRAOBfiles.Text = cboRAOBfiles.List(0)

Case 2 'non-Templates
' Get all RAOB non-template files and load into combo box (if there are any)
  cboRAOBfiles.Clear
  FilePath$ = RSpath$ + "Non-Templates\"
  FileMask$ = "?????_*.RAOB2"
  ConvertInFile$ = Dir(FilePath$ + FileMask, vbNormal)
' Check that at least one file exists
  If ConvertInFile$ = "" Then
    MsgBox "No files were found in path: " + FilePath$ + vbCrLf + "Satisfying the mask: " + FileMask + vbCrLf + "This is not essential to calculate RCs!"
    Exit Sub
  End If
' One does, so now check the rest of the directory
  Do
    cboRAOBfiles.AddItem ConvertInFile$
    ConvertInFile$ = Dir
  Loop Until ConvertInFile$ = ""
  cboRAOBfiles.Text = cboRAOBfiles.List(0)

Case 3
End Select

End Sub


Sub cboPlatforms_Click()
Dim i%, f As Form

  Set f = RCmain
  Platform$ = Trim(cboPlatforms.Text)
  Rdir$ = "\" + Platform$ + "\"
  AC$ = Left$(Platform$, 2)
' Load Missions list for appropriate platform
  LoadComboBoxM RCmain.cboMissions, Trim(cboPlatforms.Text)
  For i = 1 To cboMissions.ListCount
    If Trim(cboMissions.List(i - 1)) = Mission$ Then Exit For
  Next i
  cboMissions.ListIndex = i - 1
  
  cmdEL_Click (4)   'Use MISSION_ELA.txt file by default
  Nel = RCmain.lstEL.ListCount
  txtNel.Text = Str(Nel)
  
  cmdLO_Click (3)
  Nlo = RCmain.lstLO.ListCount
  txtNlo.Text = Str$(Nlo)
  Nobs = Nlo * Nel
  
  cmdPalt_Click (4)
  NFL = RCmain.lstPalt.ListCount
  txtNFL.Text = Str(NFL)
  For i = 1 To NFL: Zr(i) = RCmain.lstPalt.List(i - 1): Next i
  For i = 1 To NFL: Pr(i) = fZtoP(Zr(i)): Next i
  
  LoadComboBoxH RCmain.cboSU, "SU"
  For i = 1 To cboSU.ListCount
    If Trim(cboSU.List(i - 1)) = SU$ Then Exit For
  Next i
  cboSU.ListIndex = i - 1

  UpdateForm
  
End Sub




Private Sub cboRAOBfiles_Click()
  cmdLoadRaobFiles_Click (1)
End Sub





Private Sub cmbMonth_Click(Index As Integer)
  Select Case Index
  Case 0
    MonthMin = cmbMonth(Index).ListIndex + 1
    cmbMonth(1).ListIndex = cmbMonth(0).ListIndex
  Case 1
    MonthMax = cmbMonth(Index).ListIndex + 1
  End Select

End Sub


Private Sub cboSU_Click()
Dim ans, f As Form, i%, j%, XXXfile$

' Accomodate different forms
  Set f = RCmain
  XXXfile$ = MNpath$ + "Setup\" + Mission$ + "_RCS" + ".txt"
  If Dir$(XXXfile$) <> "" Then
    Call ReadMissionRCS(Mission$, "GENERAL", False)
    cboSU.Text = SU$
    Call ReadMissionRCS(Mission$, "IF_BANDPASS", False)
    
    Call ReadMissionRCS(Mission$, "ZP_OFFSETS", False)
    If Nret = 0 Then 'Need this for backward compatability before 2011-04-12 mjm
      Nret = 33      'Nret was not in RCS file and dZ() values were hard wired
      dZ(1) = -9.5: dZ(2) = -8#: dZ(3) = -6.5: dZ(4) = -5.1: dZ(5) = -4#: dZ(6) = -3.2
      dZ(7) = -2.5: dZ(8) = -2#: dZ(9) = -1.6: dZ(10) = -1.3: dZ(11) = -1#: dZ(12) = -0.7
      dZ(13) = -0.5: dZ(14) = -0.3: dZ(15) = -0.15: dZ(16) = 0#: dZ(17) = 0.15: dZ(18) = 0.3
      dZ(19) = 0.5:  dZ(20) = 0.7: dZ(21) = 1#: dZ(22) = 1.3: dZ(23) = 1.6: dZ(24) = 2#
      dZ(25) = 2.5: dZ(26) = 3.2: dZ(27) = 4#: dZ(28) = 5.2: dZ(29) = 6.9: dZ(30) = 9#
      dZ(31) = 11.4: dZ(32) = 14.5: dZ(33) = 18#
    End If
    For i = 1 To Nret: cmbDz.AddItem Format$(i, "00") + " " + Format$(dZ(i), "##0.00"): Next i

  Else
    If Val(YYYYMMDD$) > 20050601 Then
      XXXfile$ = MNpath$ + "Setup\" + Mission$ + "_" + SU$ + "_IFB" + ".txt"
    Else
      XXXfile$ = MNpath$ + "Setup\" + Mission$ + "_IFB" + ".txt"
    End If
  ' Change IF frequencies and weights ONLY
    If Not fReadFile_XXX("IFB", Trim(cboSU.Text)) Then
      MsgBox "Unable to read IF bandpass file for this sensor unit!", vbOKOnly
      Exit Sub
    End If
  End If
  
  SURC = cboSU.Text
  txtLSBpercent.Text = Str(CH1LSBloss)
' And list in combo box
  Nnu = 2 * Nif
  lstSU.Clear
  For i = 1 To Nlo
    For j = 1 To Nif
      lstSU.AddItem Format$(j, "00") + "  " + Format$(IFoff(i, j), "0.000") + "  " + Format$(IFwt(i, j), "0.0000")
    Next j
  Next i
  DrawBandpass
  
' Get the apriori observable errors
  If Not fSet_Apriori_OBrms(f) Then Exit Sub
  Select Case RCformatUsed
  Case 2: txtRCformat.Text = "2"
  Case 3: txtRCformat.Text = "3"
  End Select
  
' And list in combo box
  cmbESV.Clear
  For i = 1 To Nobs
    cmbESV.AddItem Format$(i, "00") + "  " + Format$(OBrms(1, i), "0.00")
  Next i
  
  UpdateForm

End Sub




Private Sub cmdCheckConfig_Click()
Dim f As Form, A$, b$
Dim CFGlu%, i%, j%, k%, L%, CFGfile$, mm$, MyDate As Variant
  
  Set f = RCmain
  f.ScaleMode = 1    'twips
  If cmdCheckConfig.Caption = "->" Then
    cmdCheckConfig.Caption = "<-"
    Width = Screen.Width / 2
    txtRCconfig.Width = Screen.Width / 2 - txtRCconfig.Left - 120
  Else
    cmdCheckConfig.Caption = "->"
    Width = 6660
  End If
  With RCmain
    .Left = (Screen.Width - .Width) / 2
    .Top = (Screen.Height - .Height) / 2
    .SSTab1.Tab = 0
  End With
  
  mm$ = Hex$(Month("1999 " + cmbMonth(0).Text)) + Hex$(Month("1999 " + cmbMonth(1).Text))
  CFGfile$ = txtRAOBpath + Left$(AC$, 1) + "RC" + f.txtRegion.Text + Left$(f.txtRegionNumberRC.Text, 1) + mm$ + ".cfg"
    
  RAOBfile$ = RTpath$ + cboRAOBfiles.Text
  RAOBlu = FreeFile
  Open RAOBfile$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
  RAOBcount = LOF(RAOBlu%) \ Len(RAOB2)
  Close RAOBlu
  
  A$ = ""
  txtRCconfig.Text = A$
  A$ = A$ + "' RC Configuration file: Created- " + Date$ + " " + Time$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  A$ = A$ + "[RCformat]" + vbCrLf
  A$ = A$ + Str(Val(f.txtRCformat.Text)) + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[RAOB]" + vbCrLf
  A$ = A$ + "RAOB File Used:            " + Trim(f.cboRAOBfiles.Text) + vbCrLf
  A$ = A$ + "Number of available RAOBs:" + Str(RAOBcount) + vbCrLf
  A$ = A$ + "LR above top of RAOB:     " + Str(Val(f.txtLR1.Text)) + vbCrLf
  A$ = A$ + "LR break altitude:        " + Str(Val(f.txtZb.Text)) + vbCrLf
  A$ = A$ + "LR above break altitude:  " + Str(Val(f.txtLR2.Text)) + vbCrLf
  A$ = A$ + "Record Step through RAOBs:" + Str(Val(f.txtRecordStep.Text)) + vbCrLf
  A$ = A$ + "Minimum RAOB P altitude:  " + Str(Val(f.txtRAOBmin.Text)) + vbCrLf
  If f.chkExcessGndT.Value = True Then
    b$ = Str(Val(f.txtExcessTamplitude)) 'Random Excess Noise Level on Ground
  Else
    b$ = "0"                             'Random Excess Noise Level on Ground
  End If
  A$ = A$ + "Excess Ground T added (K): " + b$ + vbCrLf
  A$ = A$ + "Sensor Unit IF Bandpass:   " + SURC$ + vbCrLf
  A$ = A$ + "" + vbCrLf

  A$ = A$ + "[Nobs A Priori Observable Errors] - Number of Observables" + vbCrLf
  A$ = A$ + Str(Nobs) + vbCrLf
  For i = 1 To Nobs  'Read observable errors
    b$ = b$ + Format(OBrms(1, i), "0.00") + " "
  Next i
  A$ = A$ + b$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[Nret] - Retrieval Altitude Offsets (km)" + vbCrLf
  A$ = A$ + Str(Nret) + vbCrLf
  For i = 1 To Nret  'Write retrieval offset levels
    b$ = b$ + Format(dZ(i), "##0.0") + " "
  Next i
  A$ = A$ + b$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[NFL] - Number of Flight Levels with RCs" + vbCrLf
  A$ = A$ + Str(NFL) + vbCrLf
  For i = 1 To NFL  'Write flight levels
    b$ = b$ + Format(Zr(i), "#0.00") + " "
  Next i
  A$ = A$ + b$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[Nlo] - Number of LO Frequencies (GHz)" + vbCrLf
  A$ = A$ + Str(Nlo) + vbCrLf
  For i = 1 To Nlo      'Write LO Frequencies
    b$ = b$ + Format(LO(i), "#00.000") + " "
  Next i
  A$ = A$ + b$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[Nel] - Number of Elevation Angeles (deg)" + vbCrLf
  A$ = A$ + Str(Nel) + vbCrLf
  For i = 1 To Nel      'Write LO Frequencies
    b$ = b$ + Format(El(i), "#00.0") + " "
  Next i
  A$ = A$ + b$ + vbCrLf
  A$ = A$ + "" + vbCrLf
  
  A$ = A$ + "[Nif] - Number of IF Bandpass Segments and Their Weights" + vbCrLf
  A$ = A$ + Str(Nif) + vbCrLf
  For j = 1 To 3
  b$ = "IF Offset (GHz): "
    For i = 1 To Nif      'Write IF offset frequencies (GHz)
      b$ = b$ + formats(IFoff(j, i), "#0.0000") + " "
    Next i
    A$ = A$ + b$ + vbCrLf
  Next j
  
  For j = 1 To 3
  b$ = "IF Weight:       "
    For i = 1 To Nif      'Write IF weights
      b$ = b$ + formats(IFwt(j, i), "#0.0000") + " "
    Next i
    A$ = A$ + b$ + vbCrLf
  Next j
  txtRCconfig.Text = A$

End Sub

Private Sub cmdEL_Click(Index As Integer)
Dim i%, lu%, File$

Select Case Index
Case 0  'Add
  lstEL.AddItem txtEl.Text
  
Case 1  'Remove
  lstEL.RemoveItem (lstEL.ListIndex)
  
Case 2  'Clear
  lstEL.Clear
  
Case 3  'Default (from HARDWARE.INI file)
  LoadListBoxH RCmain.lstEL, "ELEV_" + Platform$, 1, 1#
  
Case 4  'Default (from MISSION_RCS.txt file)
  File$ = MNpath$ + "Setup\" + Mission$ + "_RCS.txt"
  If Dir$(File$) <> "" Then
    Call ReadMissionRCS(Mission$, "ELEVATION_ANGLES", False)
    lstEL.Clear
    For i = 1 To Nel
      lstEL.AddItem Format$(El(i), "#0.0")
    Next i
  Else
    File$ = MNpath$ + "Setup\" + Mission$ + "_RCS.txt"
    If Dir$(File$) <> "" Then
      lstEL.Clear
      lu = FreeFile
      Open File$ For Input As lu
      Input #lu, Nel
      For i = 1 To Nel
        Input #lu, El(i)
        lstEL.AddItem Format$(El(i), "#0.0")
      Next i
    Else
      MsgBox File$ + " does not exist!", vbOKOnly
    End If
  End If
End Select

txtNel.Text = Str(lstEL.ListCount)
Nel = Val(txtNel.Text)
txtEl.Text = ""
Nobs = Nlo * Nel  '(Nel - 1) + 1 'Assume 1 OAT level

End Sub

Private Sub cmdExit_Click()
Unload Me
End

End Sub

Private Sub cmdGetLR_Click()
Dim RMSfilename$, LR1!, LR2!, zLRb!, Found As Boolean

' Open Mission_RAOBrangeUsed.txt file and set LR extension parameters
'  RMSfilename$ = fGetFilename(cboRAOBfiles.Text)  'strip off path to file
  RMSfilename$ = cboRAOBfiles.Text
  Found = fGetLRextension(RMSfilename$, LR1!, LR2!, zLRb!)
  If Found Then
    txtLR1.Text = LR1
    txtLR2.Text = LR2
    txtZb.Text = zLRb
  Else
    MsgBox RMSfilename$ + " was not found in " + Mission$ + "_RAOBrangeUsed.txt" + vbCrLf + "You must enter LR1, LR2, and zLRb manually!", vbOKOnly
  End If
End Sub

Private Sub cmdLO_Click(Index As Integer)
Dim i%

  Select Case Index
  Case 0 'Add
    lstLO.AddItem txtLO.Text
    txtLO.Text = ""
  
  Case 1 'Remove
    lstLO.RemoveItem lstLO.ListIndex
  
  Case 2 'Clear
    lstLO.Clear
  
  Case 3 'Default
    lstLO.Clear
    Call ReadMissionRCS(Mission$, "LO_FREQUENCIES", False)
    For i = 1 To Nlo
      lstLO.AddItem Format(LO(i), "00.000")
    Next i
    txtNlo.Text = Str(lstLO.ListCount)
    Channels = Nlo
  End Select
End Sub

Private Sub cmdLoadRaobFiles_Click(Index As Integer)
Dim Filename$, FilePath$, FileMask$, ConvertInFile$

Select Case Index
Case 0
' Get all RAOB template files and load into combo box (if there are any)
  cboRAOBfiles.Clear
  FilePath$ = RTpath$
  FileMask$ = "?????_*.RAOB2"
  ConvertInFile$ = Dir(FilePath$ + FileMask, vbNormal)
' Check that at least one file exists
  If ConvertInFile$ = "" Then
    MsgBox "No files were found in path: " + FilePath$ + vbCrLf + "Satisfying the mask: " + FileMask + vbCrLf + "This is not essential to calculate RCs!"
    Exit Sub
  End If
' One does, so now check the rest of the directory
  Do
    cboRAOBfiles.AddItem ConvertInFile$
    ConvertInFile$ = Dir
  Loop Until ConvertInFile$ = ""
  cboRAOBfiles.Text = cboRAOBfiles.List(0)

Case 1
  cmdGetLR_Click
  SSTab1.Tab = 0
  
End Select

End Sub

Private Sub cmdPalt_Click(Index As Integer)
Dim X!, i%, FLAfile$

Select Case Index
Case 0  'Add
  lstPalt.AddItem txtPalt.Text

Case 1  'Remove
  lstPalt.RemoveItem (lstPalt.ListIndex)

Case 2  'Clear
  lstPalt.Clear

Case 3  'Default (HARDWARE.INI file)
  FLAfile$ = SUpath$ + Mission$ + "_RCS.txt"
  If Len(Dir$(FLAfile)) = 0 Then
    LoadListBoxH RCmain.lstPalt, "ALT_" + Platform$, 2, 1#
    NFL = RCmain.lstPalt.ListCount
    txtNFL.Text = Str(NFL)
  Else
    Call ReadMissionRCS(Mission$, "RC_ALTITUDES", False)
  End If

Case 4  'Setup FLA file
  lstPalt.Clear
  FLAfile$ = SUpath$ + Mission$ + "_RCS.txt"
  If Len(Dir$(FLAfile)) = 0 Then
    Call ReadFLAfile(Platform$, Mission$)
    For i = 1 To NFL: lstPalt.AddItem Format$(FLA(i) / 100, "#0.000"): Next i
  Else
    Call ReadMissionRCS(Mission$, "RC_ALTITUDES", False)
    For i = 1 To NFL: lstPalt.AddItem Format$(FLA(i) / 100, "#0.000"): Next i
  End If

End Select

txtPalt.Text = ""
txtNFL.Text = lstPalt.ListCount
NFL = Val(txtNFL.Text)
For i = 1 To NFL: Zr(i) = RCmain.lstPalt.List(i - 1): Next i
For i = 1 To NFL: Pr(i) = fZtoP(Zr(i)): Next i

End Sub


Private Sub cmdRCread_Click()

  Call RCread(0, "C:\MTP\RAOB\Missions\PAVE\RC\DRCAA012.RCF")
  
End Sub

Private Sub cmdReloadTemplates_Click()
  cmdLoadRaobFiles_Click (0)
End Sub

Private Sub cmdStart_Click()
Dim lu%, i%, j%

' Write parameters for this run
  lu = FreeFile
  Open "C:\MTP\Setup\RCcalc.INI" For Output As lu
  Print #lu, Left$(txtRegion.Text, 2)             'txtRegion
  Print #lu, cmbMonth(0).ListIndex         'cboMonth(0) index
  Print #lu, cmbMonth(1).ListIndex         'cboMonth(1) index
  Close lu
    
  txtBusy.BackColor = &HFF&                'Red
  RCformatUsed = Val(txtRCformat.Text)
  
  Select Case cboMode.ListIndex
  Case 0
    Main
  Case 1
    MainAll2                                'Calc RCs for ALL template files
  Case 2
    MainAll2                                'Calc RCs for selected template file
  Case 3
    MainAll
  End Select
  
  txtBusy.BackColor = &HFF00&              'Green

End Sub

Private Sub cmdTBcorr_Click()
Dim z0!, i%, j%, k%, L%, M%, iRC%, RCpath$, lu%, filen!, bpv!, site$, Filename$
Dim A$, X!(1 To 3), y!(1 To 3), Coeff!(1 To 3), mm$, Ext$, Tref!(1 To 33)
Dim TAref!(1 To 3, 1 To 10), TAp!(1 To 3, 1 To 10), TAm!(1 To 3, 1 To 10), TAz!(1 To 3, 1 To 10)
Dim ob!(1 To 30), Orms!(1 To 30), RAOBlu%

  
  Call ReadRCconfig(0, txtRCfile.Text)
  
  If RAOBcount = 0 Then
    RAOBfile$ = RTpath$ + cboRAOBfiles.Text
    RAOBlu = FreeFile
    Open RAOBfile$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
    RAOBcount = LOF(RAOBlu%) \ Len(RAOB2)
    Close RAOBlu
  End If
  
  If RAOBfile$ = "" Then RAOBfile$ = RTpath$ + cboRAOBfiles.Text
  
  Filename$ = fGetFilename(txtRCfile.Text)
  txtRegion.Text = Mid$(Filename$, 4, 2)
  txtRegionNumber.Text = Mid$(Filename$, 6, 1)
  cmbMonth(0).ListIndex = Mid$(Filename$, 7, 1) - 1
  cmbMonth(1).ListIndex = Mid$(Filename$, 8, 1) - 1
  cboRAOBfiles.Text = RAOBfile$
  If RAOBcount = 0 Then
    RAOBfile$ = RTpath$ + cboRAOBfiles.Text
    RAOBlu = FreeFile
    Open RAOBfile$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
    RAOBcount = LOF(RAOBlu%) \ Len(RAOB2)
    Close RAOBlu
  End If

  txtProgress.Text = Str(RAOBcount)
  txtLR1.Text = Str(LR1)                             'LR above top of RAOB
  txtZb.Text = Str(zLRb)                             'LR break altitude
  txtLR2.Text = Str(LR2)                             'LR above break altitude
  txtRecordStep.Text = Str(RecordStep)               'Record Step through available RAOBs
  txtRAOBmin.Text = Str(MinimumRAOBz)                'Minimum acceptable RAOB altitude
  txtExcessTamplitude.Text = Str(ExcessTamplitude)   'Random Excess Noise Level on Ground
  
  RCfilename$ = txtRAOBpath.Text + txtRCfile.Text
  Call CalculateSmatrix
  
  Call WriteRCconfig(RCmain)

End Sub

Private Sub cmdTransparency_Click()
Dim z0!

    z0 = Val(txtPalt.Text)
'    DoDatGenCalculation (z0!)
    VaryLOoverIF z0

End Sub



Private Sub Drive1_Change()

Drive$ = Left$(Drive1.Drive, 2)
txtRCfile.Text = Drive$ + Right$(txtRCfile.Text, Len(txtRCfile.Text) - 2)
cboRAOBfiles.Text = Drive$ + Right$(cboRAOBfiles.Text, Len(cboRAOBfiles.Text) - 2)
txtROfile.Text = Drive$ + Right$(txtROfile.Text, Len(txtROfile.Text) - 2)

End Sub

Private Sub Form_DblClick()
  
  CaptureFormMJ Me, "", 3, True
  
End Sub

Private Sub Form_Load()
Dim i%, j%, lu%, f$

  RCmain.Height = 5385
  RCmain.Width = 6660
  With RCmain
  .Left = (Screen.Width - RCmain.Width) / 2
  .Top = (Screen.Height - RCmain.Height) / 2
  .SSTab1.Tab = 0
  End With
  
  With cboMode
  .AddItem "Process Single File"
  .AddItem "Process Templates"
  .AddItem "Process Non-Templates"
  .AddItem "Process Template Map"
  .ListIndex = 0    'default to "process single file"; otherwise, ListIndex=-1
  End With

  'ReadSETUP ("SYSTEM")
  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"
  
  RAOBpath$ = RSpath$
  txtRAOBpath.Text = RSpath$
  RCpath$ = RAOBpath$ + "RC\"
  'cboRAOBfiles.Text = Mission$ + ".RAOB2"
  DoEvents
  
  RAOBtype = 0          'Use short format RAOBs only
' Pressure altitude offsets Dz wrt to retrieval level, z0
' Nret = 33
'  Call ReadMissionRCS(Mission$, "ZP_OFFSETS", False)
'  If Nret = 0 Then 'Need this for backward compatability
'    Nret = 33
'    dZ(1) = -9.5: dZ(2) = -8#: dZ(3) = -6.5: dZ(4) = -5.1: dZ(5) = -4#: dZ(6) = -3.2
'    dZ(7) = -2.5: dZ(8) = -2#: dZ(9) = -1.6: dZ(10) = -1.3: dZ(11) = -1#: dZ(12) = -0.7
'    dZ(13) = -0.5: dZ(14) = -0.3: dZ(15) = -0.15: dZ(16) = 0#: dZ(17) = 0.15: dZ(18) = 0.3
'    dZ(19) = 0.5:  dZ(20) = 0.7: dZ(21) = 1#: dZ(22) = 1.3: dZ(23) = 1.6: dZ(24) = 2#
'    dZ(25) = 2.5: dZ(26) = 3.2: dZ(27) = 4#: dZ(28) = 5.2: dZ(29) = 6.9: dZ(30) = 9#
'    dZ(31) = 11.4: dZ(32) = 14.5: dZ(33) = 18#
'    For i = 1 To Nret: cmbDz.AddItem Format$(i, "00") + " " + Format$(dZ(i), "##0.00"): Next i
'  End If
  
  For i = 0 To 1
    cmbMonth(i).AddItem "JAN"
    cmbMonth(i).AddItem "FEB"
    cmbMonth(i).AddItem "MAR"
    cmbMonth(i).AddItem "APR"
    cmbMonth(i).AddItem "MAY"
    cmbMonth(i).AddItem "JUN"
    cmbMonth(i).AddItem "JUL"
    cmbMonth(i).AddItem "AUG"
    cmbMonth(i).AddItem "SEP"
    cmbMonth(i).AddItem "OCT"
    cmbMonth(i).AddItem "NOV"
    cmbMonth(i).AddItem "DEC"
  Next i
  cmbMonth(0).ListIndex = 0
  cmbMonth(1).ListIndex = 1

' Load Platforms
  LoadComboBoxH RCmain.cboPlatforms, "PLATFORM"  'Sets AC$
  For i = 1 To cboPlatforms.ListCount
    If Left$(Trim(cboPlatforms.List(i - 1)), 2) = AC$ Then Exit For
  Next i
  cboPlatforms.ListIndex = i - 1  'This triggers loading all the other combo boxes

  MinimumRAOBz = 22#              'ROABs must go to this mb level to be used = 22 km
  txtRAOBmin.Text = Str$(MinimumRAOBz)
  PRmin = fZtoP(MinimumRAOBz)
  Htop = 50#             ' Top of numberical integration [km]
  Nret = 33              ' Nret
  NraobMax = 5000        ' Maximum number of retrievals
  DoEvents
  
' Load parameters from last run
  lu = FreeFile
  Open "C:\MTP\Setup\RCcalc.INI" For Input As lu
  Input #lu, Region$   'txtRegion
  Input #lu, i         'cboMonth(0) index
  Input #lu, j         'cboMonth(1) index
  Close lu
  txtRegion.Text = Region$
  cmbMonth(0).ListIndex = i
  cmbMonth(1).ListIndex = j
  
End Sub



Private Sub lstPalt_Click()
  txtPalt.Text = lstPalt.List(lstPalt.ListIndex)
  ALTs$ = Format$(txtPalt.Text, "00")
  UpdateForm
End Sub


Private Sub mnuFileSelectCFG_Click()
Dim CFGfile$, Filename$

' Set Initial directory
CommonDialog1.InitDir = Drive$ + "\RAOB\" + Mission$ + "\RC\"
' Set Size of FileName buffer
CommonDialog1.MaxFileSize = 1024
' Set filters.
CommonDialog1.Filter = "CFG Files (*.CFG)|*.CFG|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

CFGfile$ = CommonDialog1.Filename

If CFGfile$ = "" Then
  Exit Sub
Else
  txtRCfile.Text = CFGfile$
' DRCSQ012.cfg
' 123456789012345678901234567890
  Filename$ = Right$(CFGfile$, 12)
  txtRegion.Text = Mid$(Filename$, 4, 2)
  
  txtRegionNumber.Text = Mid$(Filename$, 6, 1)
  cmbMonth(0).Text = Mid$(Filename$, 7, 1)
  cmbMonth(1).Text = Mid$(Filename$, 8, 1)
  Call ReadRCconfig(0, CFGfile$)
  cboRAOBfiles.Text = RAOBfile$
  txtProgress.Text = Str(RAOBcount)
  txtLR1.Text = Str(LR1)                             'LR above top of RAOB
  txtZb.Text = Str(zLRb)                             'LR break altitude
  txtLR2.Text = Str(LR2)                             'LR above break altitude
  txtRecordStep.Text = Str(RecordStep)               'Record Step through available RAOBs
  txtRAOBmin.Text = Str(MinimumRAOBz)                'Minimum acceptable RAOB altitude
  txtExcessTamplitude.Text = Str(ExcessTamplitude)   'Random Excess Noise Level on Ground
  cmdTBcorr_Click
End If

End Sub

Private Sub picBandpass_MouseMove(Button As Integer, Shift As Integer, X As Single, y As Single)
Dim i%, j%, k%, k1%, k2%, iUT&, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, iRec%, Track$
  
  iX = Int(X): iY = Int(y): iRec = 0: iUT = 0
'  Xthreshold = 10 * (Val(txtFTX2.Text) - Val(txtFTX1.Text)) / picBandpass.Width
'  Ythreshold = 10 * (Val(txtFTY2.Text) - Val(txtFTY1.Text)) / picBandpass.Height
  picBandpass.ToolTipText = "X=" + Format(X, "##0.0") + "," + "Y=" + Format(y, "##0.0")

End Sub


Private Sub SSTab1_Click(PreviousTab As Integer)

If SSTab1.Tab = 1 Then DrawBandpass

End Sub

Private Sub txtRAOBmin_Change()
UpdateForm
End Sub


Private Sub txtRegion_Change()

  If Len(txtRegion.Text) > 2 Then
    MsgBox "There are more than 2 character here!", vbOKOnly
  End If

End Sub

