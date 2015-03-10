VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomct2.ocx"
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form frmMTPsetup 
   Caption         =   "MTP Setup"
   ClientHeight    =   4920
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7620
   Icon            =   "frmMTPsetup.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4920
   ScaleWidth      =   7620
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   375
      Left            =   1215
      TabIndex        =   83
      Top             =   4500
      Width           =   1050
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   375
      Left            =   90
      TabIndex        =   82
      Top             =   4500
      Width           =   1050
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   345
      Left            =   6615
      TabIndex        =   54
      Top             =   4545
      Width           =   960
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4455
      Left            =   45
      TabIndex        =   0
      Top             =   30
      Width           =   7515
      _ExtentX        =   13256
      _ExtentY        =   7858
      _Version        =   393216
      Tabs            =   5
      Tab             =   3
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   "Setup"
      TabPicture(0)   =   "frmMTPsetup.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Frame(13)"
      Tab(0).Control(1)=   "Picture1"
      Tab(0).Control(2)=   "cmdSetup"
      Tab(0).ControlCount=   3
      TabCaption(1)   =   "File"
      TabPicture(1)   =   "frmMTPsetup.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label(4)"
      Tab(1).Control(1)=   "Label(2)"
      Tab(1).Control(2)=   "Label(3)"
      Tab(1).Control(3)=   "Label(5)"
      Tab(1).Control(4)=   "Label(16)"
      Tab(1).Control(5)=   "CommonDialog1"
      Tab(1).Control(6)=   "chkReOpen"
      Tab(1).Control(7)=   "cmdOpenFile"
      Tab(1).Control(8)=   "cboPlatforms"
      Tab(1).Control(9)=   "cboMTPcurrent"
      Tab(1).Control(10)=   "lstCurrentRecord"
      Tab(1).Control(11)=   "txtCurrentRecord"
      Tab(1).Control(12)=   "txtFilename"
      Tab(1).Control(13)=   "tmrRealTimeSim"
      Tab(1).Control(14)=   "cboSimTime"
      Tab(1).ControlCount=   15
      TabCaption(2)   =   "FTP"
      TabPicture(2)   =   "frmMTPsetup.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Label(8)"
      Tab(2).Control(1)=   "Label(7)"
      Tab(2).Control(2)=   "Label(6)"
      Tab(2).Control(3)=   "Label(9)"
      Tab(2).Control(4)=   "Label(10)"
      Tab(2).Control(5)=   "Label(11)"
      Tab(2).Control(6)=   "Label(12)"
      Tab(2).Control(7)=   "Label(13)"
      Tab(2).Control(8)=   "Label(14)"
      Tab(2).Control(9)=   "lblStatuss"
      Tab(2).Control(10)=   "MonthView1"
      Tab(2).Control(11)=   "txtCurrent"
      Tab(2).Control(12)=   "txtFilesRead"
      Tab(2).Control(13)=   "chkUseLatest"
      Tab(2).Control(14)=   "cmdStartFTP"
      Tab(2).Control(15)=   "Timer1"
      Tab(2).Control(16)=   "txtFilenameDate"
      Tab(2).Control(17)=   "txtLocalFolder"
      Tab(2).Control(18)=   "txtPassword"
      Tab(2).Control(19)=   "txtUsername"
      Tab(2).Control(20)=   "txtRemoteFolder"
      Tab(2).Control(21)=   "txtFTPsite"
      Tab(2).Control(22)=   "Frame1"
      Tab(2).Control(23)=   "Frame2"
      Tab(2).Control(24)=   "ctlInternet"
      Tab(2).ControlCount=   25
      TabCaption(3)   =   "UDP"
      TabPicture(3)   =   "frmMTPsetup.frx":0496
      Tab(3).ControlEnabled=   -1  'True
      Tab(3).Control(0)=   "Label2"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "Label3"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "lblStatus(0)"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "lblStatus(1)"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).Control(4)=   "lblStatus(2)"
      Tab(3).Control(4).Enabled=   0   'False
      Tab(3).Control(5)=   "lblStatus(3)"
      Tab(3).Control(5).Enabled=   0   'False
      Tab(3).Control(6)=   "txtUDPin"
      Tab(3).Control(6).Enabled=   0   'False
      Tab(3).Control(7)=   "cmdConnect"
      Tab(3).Control(7).Enabled=   0   'False
      Tab(3).Control(8)=   "Winsock1"
      Tab(3).Control(8).Enabled=   0   'False
      Tab(3).Control(9)=   "StatusBar1"
      Tab(3).Control(9).Enabled=   0   'False
      Tab(3).Control(10)=   "cboRemotePort"
      Tab(3).Control(10).Enabled=   0   'False
      Tab(3).Control(11)=   "chkAllpackets"
      Tab(3).Control(11).Enabled=   0   'False
      Tab(3).Control(12)=   "chkLogPackets"
      Tab(3).Control(12).Enabled=   0   'False
      Tab(3).Control(13)=   "chkLogRaw"
      Tab(3).Control(13).Enabled=   0   'False
      Tab(3).Control(14)=   "cboStatusPort"
      Tab(3).Control(14).Enabled=   0   'False
      Tab(3).Control(15)=   "Winsock2"
      Tab(3).Control(15).Enabled=   0   'False
      Tab(3).Control(16)=   "txtStatusPackets"
      Tab(3).Control(16).Enabled=   0   'False
      Tab(3).ControlCount=   17
      TabCaption(4)   =   "Watchdog"
      TabPicture(4)   =   "frmMTPsetup.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "tmrWatchdog"
      Tab(4).Control(1)=   "chkMTPcurrentRAW"
      Tab(4).Control(2)=   "chkLOchanging"
      Tab(4).Control(3)=   "txtMTPcurrentRAW"
      Tab(4).Control(4)=   "txtLOchanging"
      Tab(4).ControlCount=   5
      Begin VB.TextBox txtStatusPackets 
         Height          =   285
         Left            =   90
         TabIndex        =   77
         Top             =   2925
         Width           =   7350
      End
      Begin MSWinsockLib.Winsock Winsock2 
         Left            =   6525
         Top             =   3600
         _ExtentX        =   741
         _ExtentY        =   741
         _Version        =   393216
         Protocol        =   1
      End
      Begin VB.ComboBox cboStatusPort 
         Height          =   315
         Left            =   945
         TabIndex        =   76
         Text            =   "5100"
         Top             =   2610
         Width           =   870
      End
      Begin VB.CheckBox chkLogRaw 
         Caption         =   "Log RAW file"
         Height          =   195
         Left            =   3150
         TabIndex        =   74
         Top             =   450
         Width           =   1365
      End
      Begin VB.TextBox txtLOchanging 
         BackColor       =   &H0000FF00&
         Height          =   315
         Left            =   -74580
         TabIndex        =   71
         Top             =   2010
         Width           =   7005
      End
      Begin VB.TextBox txtMTPcurrentRAW 
         BackColor       =   &H0000FF00&
         Height          =   315
         Left            =   -74580
         TabIndex        =   70
         Text            =   "Last Update Time"
         Top             =   1290
         Width           =   7005
      End
      Begin VB.CheckBox chkLOchanging 
         Caption         =   "Is LO synthesizer changing frequency?"
         Height          =   315
         Left            =   -74850
         TabIndex        =   69
         Top             =   1710
         Value           =   1  'Checked
         Width           =   3405
      End
      Begin VB.CheckBox chkMTPcurrentRAW 
         Caption         =   "Is MTPcurrent.RAW updating?"
         Height          =   285
         Left            =   -74850
         TabIndex        =   68
         Top             =   990
         Value           =   1  'Checked
         Width           =   3255
      End
      Begin VB.Timer tmrWatchdog 
         Interval        =   2000
         Left            =   -74670
         Top             =   3180
      End
      Begin VB.CheckBox chkLogPackets 
         Caption         =   "Log Packets"
         Height          =   195
         Left            =   1800
         TabIndex        =   67
         Top             =   450
         Width           =   1245
      End
      Begin VB.CheckBox chkAllpackets 
         Caption         =   "View All packets"
         Height          =   195
         Left            =   1845
         TabIndex        =   66
         Top             =   2655
         Width           =   1485
      End
      Begin VB.ComboBox cboRemotePort 
         Height          =   315
         Left            =   855
         TabIndex        =   65
         Text            =   "10200"
         ToolTipText     =   "Local Host will use the same port number"
         Top             =   360
         Width           =   870
      End
      Begin MSComctlLib.StatusBar StatusBar1 
         Height          =   255
         Left            =   60
         TabIndex        =   64
         Top             =   4110
         Width           =   6345
         _ExtentX        =   11192
         _ExtentY        =   450
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   3
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   4939
               MinWidth        =   4939
            EndProperty
            BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3175
               MinWidth        =   3175
            EndProperty
            BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   2999
               MinWidth        =   2999
            EndProperty
         EndProperty
      End
      Begin MSWinsockLib.Winsock Winsock1 
         Left            =   6975
         Top             =   3600
         _ExtentX        =   741
         _ExtentY        =   741
         _Version        =   393216
         Protocol        =   1
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "Connect"
         Height          =   375
         Left            =   6480
         TabIndex        =   63
         Top             =   4005
         Width           =   945
      End
      Begin VB.TextBox txtUDPin 
         Height          =   1815
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   61
         Top             =   720
         Width           =   7275
      End
      Begin VB.ComboBox cboSimTime 
         Height          =   315
         Left            =   -72810
         TabIndex        =   60
         Text            =   "5"
         Top             =   1050
         Width           =   645
      End
      Begin VB.CommandButton cmdSetup 
         Caption         =   "&Update"
         Height          =   375
         Left            =   -68490
         TabIndex        =   58
         Top             =   3330
         Width           =   915
      End
      Begin VB.PictureBox Picture1 
         Height          =   2865
         Left            =   -74880
         Picture         =   "frmMTPsetup.frx":04CE
         ScaleHeight     =   187
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   114
         TabIndex        =   55
         Top             =   420
         Width           =   1770
      End
      Begin InetCtlsObjects.Inet ctlInternet 
         Left            =   -68160
         Top             =   2730
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
      End
      Begin VB.Frame Frame2 
         Caption         =   "Files to FTP "
         Height          =   780
         Left            =   -74925
         TabIndex        =   41
         Top             =   2880
         Width           =   2160
         Begin VB.OptionButton optXferFiles 
            Caption         =   "MTPcurrent.RAW"
            Height          =   195
            Index           =   1
            Left            =   105
            TabIndex        =   43
            ToolTipText     =   "Perfom operations on only this file"
            Top             =   480
            Width           =   1590
         End
         Begin VB.OptionButton optXferFiles 
            Caption         =   "All .txt Files"
            Height          =   195
            Index           =   0
            Left            =   105
            TabIndex        =   42
            ToolTipText     =   "Perform operations on All text files"
            Top             =   240
            Value           =   -1  'True
            Width           =   1170
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "FTP Operation "
         Height          =   810
         Left            =   -72705
         TabIndex        =   38
         Top             =   2865
         Width           =   1665
         Begin VB.OptionButton optFTPdirection 
            Caption         =   "Receive Files"
            Height          =   195
            Index           =   0
            Left            =   105
            TabIndex        =   40
            Top             =   270
            Value           =   -1  'True
            Width           =   1260
         End
         Begin VB.OptionButton optFTPdirection 
            Caption         =   "Send Files"
            Height          =   195
            Index           =   1
            Left            =   105
            TabIndex        =   39
            Top             =   525
            Width           =   1035
         End
      End
      Begin VB.TextBox txtFTPsite 
         Height          =   285
         Left            =   -73785
         TabIndex        =   36
         Text            =   "ftp.eol.ucar.edu"
         Top             =   360
         Width           =   2760
      End
      Begin VB.TextBox txtRemoteFolder 
         Height          =   285
         Left            =   -73785
         TabIndex        =   35
         Text            =   "/pub/temp/users/haggerty"
         Top             =   675
         Width           =   2760
      End
      Begin VB.TextBox txtUsername 
         Height          =   300
         Left            =   -73785
         TabIndex        =   34
         Text            =   "anonymous"
         Top             =   990
         Width           =   2760
      End
      Begin VB.TextBox txtPassword 
         Height          =   285
         Left            =   -73785
         TabIndex        =   33
         Text            =   "Michael.J.Mahoney@jpl.nasa.gov"
         Top             =   1320
         Width           =   2760
      End
      Begin VB.TextBox txtLocalFolder 
         Height          =   285
         Left            =   -73785
         TabIndex        =   32
         Text            =   "C:/MTP/RTA/RAW/"
         Top             =   1635
         Width           =   2775
      End
      Begin VB.TextBox txtFilenameDate 
         Height          =   285
         Left            =   -73785
         TabIndex        =   31
         Text            =   "MTPcurrent_20080512"
         Top             =   1950
         Width           =   2775
      End
      Begin VB.Timer Timer1 
         Left            =   -68610
         Top             =   2850
      End
      Begin VB.CommandButton cmdStartFTP 
         Caption         =   "&Start"
         Height          =   375
         Left            =   -68520
         TabIndex        =   30
         Top             =   3345
         Width           =   960
      End
      Begin VB.CheckBox chkUseLatest 
         Caption         =   "Use Latest File"
         Height          =   225
         Left            =   -70260
         TabIndex        =   29
         Top             =   2775
         Value           =   1  'Checked
         Width           =   1350
      End
      Begin VB.TextBox txtFilesRead 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73785
         MultiLine       =   -1  'True
         TabIndex        =   28
         Top             =   2550
         Width           =   480
      End
      Begin VB.TextBox txtCurrent 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72705
         MultiLine       =   -1  'True
         TabIndex        =   27
         Top             =   2550
         Width           =   480
      End
      Begin VB.Timer tmrRealTimeSim 
         Left            =   -74415
         Top             =   3255
      End
      Begin VB.TextBox txtFilename 
         Height          =   285
         Left            =   -74010
         TabIndex        =   22
         Text            =   "C:\MTP\Data\GH\GloPac\RAW\GH20100307.0512"
         Top             =   390
         Width           =   5895
      End
      Begin VB.TextBox txtCurrentRecord 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68775
         MultiLine       =   -1  'True
         TabIndex        =   21
         Text            =   "frmMTPsetup.frx":8DFF
         Top             =   1065
         Width           =   645
      End
      Begin VB.ListBox lstCurrentRecord 
         Height          =   1815
         ItemData        =   "frmMTPsetup.frx":8E01
         Left            =   -74910
         List            =   "frmMTPsetup.frx":8E03
         TabIndex        =   20
         Top             =   1410
         Width           =   7350
      End
      Begin VB.ComboBox cboMTPcurrent 
         Height          =   315
         Left            =   -74025
         TabIndex        =   19
         Text            =   "C:\MTP\RTA\RAW\"
         Top             =   705
         Width           =   5910
      End
      Begin VB.ComboBox cboPlatforms 
         Height          =   315
         Left            =   -71460
         TabIndex        =   18
         Text            =   "NGV"
         Top             =   1050
         Width           =   1200
      End
      Begin VB.CommandButton cmdOpenFile 
         Caption         =   "Open File"
         Height          =   330
         Left            =   -68520
         TabIndex        =   17
         Top             =   3360
         Width           =   960
      End
      Begin VB.CheckBox chkReOpen 
         Caption         =   "Reopen at EOF"
         Height          =   255
         Left            =   -70665
         TabIndex        =   16
         Top             =   3390
         Value           =   1  'Checked
         Width           =   1455
      End
      Begin VB.Frame Frame 
         Caption         =   "MTPsetup.INI"
         Height          =   2955
         Index           =   13
         Left            =   -73080
         TabIndex        =   1
         Top             =   345
         Width           =   5520
         Begin VB.ComboBox cboDefaultRCs 
            Height          =   315
            Left            =   4470
            TabIndex        =   73
            Text            =   "FALSE"
            Top             =   2550
            Width           =   975
         End
         Begin VB.ComboBox cboDestination2 
            Height          =   315
            Left            =   1425
            TabIndex        =   56
            Text            =   "cboDestination2"
            Top             =   2220
            Width           =   4020
         End
         Begin VB.TextBox txtAlternateRoot 
            Height          =   315
            Left            =   1425
            TabIndex        =   8
            Top             =   2565
            Width           =   1335
         End
         Begin VB.TextBox txtUID 
            Height          =   285
            Left            =   1425
            TabIndex        =   7
            Text            =   "mjm"
            Top             =   900
            Width           =   735
         End
         Begin VB.DriveListBox drvData 
            Height          =   315
            Left            =   1425
            TabIndex        =   6
            Top             =   555
            Width           =   2115
         End
         Begin VB.DriveListBox drvProgram 
            Height          =   315
            Left            =   1425
            TabIndex        =   5
            Top             =   225
            Width           =   2115
         End
         Begin VB.ComboBox cboRTdefault 
            Height          =   315
            Left            =   1425
            TabIndex        =   4
            Text            =   "FALSE"
            Top             =   1230
            Width           =   975
         End
         Begin VB.ComboBox cboDataSource 
            Height          =   315
            Left            =   1425
            TabIndex        =   3
            Text            =   "cboDataSource"
            Top             =   1560
            Width           =   4020
         End
         Begin VB.ComboBox cboDestination 
            Height          =   315
            Left            =   1425
            TabIndex        =   2
            Text            =   "cboDestination"
            Top             =   1890
            Width           =   4020
         End
         Begin VB.Label Label 
            Caption         =   "Use Default RCs?"
            Height          =   255
            Index           =   17
            Left            =   3030
            TabIndex        =   72
            Top             =   2610
            Width           =   1380
         End
         Begin VB.Label Label 
            Caption         =   "Data Destination 2"
            Height          =   240
            Index           =   15
            Left            =   90
            TabIndex        =   57
            Top             =   2265
            Width           =   1380
         End
         Begin VB.Label Label 
            Caption         =   "Alternate Root"
            Height          =   240
            Index           =   161
            Left            =   120
            TabIndex        =   15
            Top             =   2625
            Width           =   1035
         End
         Begin VB.Label Label 
            Caption         =   "Program Drive"
            Height          =   255
            Index           =   131
            Left            =   90
            TabIndex        =   14
            Top             =   285
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Data Drive"
            Height          =   255
            Index           =   132
            Left            =   90
            TabIndex        =   13
            Top             =   645
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "User Initials"
            Height          =   210
            Index           =   133
            Left            =   90
            TabIndex        =   12
            Top             =   960
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Realtime?"
            Height          =   255
            Index           =   159
            Left            =   90
            TabIndex        =   11
            Top             =   1260
            Width           =   1110
         End
         Begin VB.Label Label 
            Caption         =   "Data Source"
            Height          =   240
            Index           =   0
            Left            =   90
            TabIndex        =   10
            Top             =   1605
            Width           =   1065
         End
         Begin VB.Label Label 
            Caption         =   "Data Destination"
            Height          =   240
            Index           =   1
            Left            =   90
            TabIndex        =   9
            Top             =   1950
            Width           =   1215
         End
      End
      Begin MSComDlg.CommonDialog CommonDialog1 
         Left            =   -74895
         Top             =   3225
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin MSComCtl2.MonthView MonthView1 
         Height          =   2370
         Left            =   -70290
         TabIndex        =   37
         Top             =   360
         Width           =   2700
         _ExtentX        =   4763
         _ExtentY        =   4180
         _Version        =   393216
         ForeColor       =   -2147483630
         BackColor       =   -2147483633
         Appearance      =   1
         StartOfWeek     =   16187393
         TitleBackColor  =   -2147483638
         CurrentDate     =   39612
      End
      Begin VB.Label lblStatus 
         Caption         =   "Horizon:  00000  00000  00000  TND:  000.0"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   3
         Left            =   135
         TabIndex        =   81
         Top             =   3870
         Width           =   6225
      End
      Begin VB.Label lblStatus 
         Caption         =   "TGT:      00000  00000  00000  TWIN: 000.0"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Index           =   2
         Left            =   135
         TabIndex        =   80
         Top             =   3690
         Width           =   6315
      End
      Begin VB.Label lblStatus 
         Caption         =   "TGT+ND:   00000  00000  00000  TTGT: 000.0"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Index           =   1
         Left            =   135
         TabIndex        =   79
         Top             =   3465
         Width           =   6225
      End
      Begin VB.Label lblStatus 
         Caption         =   "Status: 259  -  20111029T061898.259  -  Packet No: 0000"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Index           =   0
         Left            =   135
         TabIndex        =   78
         Top             =   3240
         Width           =   6810
      End
      Begin VB.Label Label3 
         Caption         =   "Status Port"
         Height          =   195
         Left            =   90
         TabIndex        =   75
         Top             =   2655
         Width           =   825
      End
      Begin VB.Label Label2 
         Caption         =   "MTP Port"
         Height          =   195
         Left            =   135
         TabIndex        =   62
         Top             =   405
         Width           =   915
      End
      Begin VB.Label Label 
         Caption         =   "Platform"
         Height          =   195
         Index           =   16
         Left            =   -72090
         TabIndex        =   59
         Top             =   1110
         Width           =   735
      End
      Begin VB.Label lblStatuss 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         BorderStyle     =   1  'Fixed Single
         Caption         =   "#"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   -73785
         TabIndex        =   53
         Top             =   2265
         Width           =   2775
      End
      Begin VB.Label Label 
         Caption         =   "Password"
         Height          =   195
         Index           =   14
         Left            =   -74925
         TabIndex        =   52
         Top             =   1335
         Width           =   1035
      End
      Begin VB.Label Label 
         Caption         =   "Remote Folder"
         Height          =   195
         Index           =   13
         Left            =   -74925
         TabIndex        =   51
         Top             =   720
         Width           =   1110
      End
      Begin VB.Label Label 
         Caption         =   "Username"
         Height          =   195
         Index           =   12
         Left            =   -74925
         TabIndex        =   50
         Top             =   1020
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "FTP Site"
         Height          =   225
         Index           =   11
         Left            =   -74925
         TabIndex        =   49
         Top             =   390
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Local Folder"
         Height          =   210
         Index           =   10
         Left            =   -74925
         TabIndex        =   48
         Top             =   1635
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Filename_Date"
         Height          =   210
         Index           =   9
         Left            =   -74925
         TabIndex        =   47
         Top             =   1950
         Width           =   1125
      End
      Begin VB.Label Label 
         Caption         =   "Status"
         Height          =   210
         Index           =   6
         Left            =   -74925
         TabIndex        =   46
         Top             =   2265
         Width           =   750
      End
      Begin VB.Label Label 
         Caption         =   "Files Read"
         Height          =   210
         Index           =   7
         Left            =   -74925
         TabIndex        =   45
         Top             =   2580
         Width           =   750
      End
      Begin VB.Label Label 
         Caption         =   "Current"
         Height          =   210
         Index           =   8
         Left            =   -73275
         TabIndex        =   44
         Top             =   2580
         Width           =   525
      End
      Begin VB.Label Label 
         Caption         =   "Write Interval (s)"
         Height          =   195
         Index           =   5
         Left            =   -74040
         TabIndex        =   26
         Top             =   1110
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "Records Written"
         Height          =   225
         Index           =   3
         Left            =   -70095
         TabIndex        =   25
         Top             =   1110
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "MTPcurrent"
         Height          =   195
         Index           =   2
         Left            =   -74910
         TabIndex        =   24
         Top             =   735
         Width           =   885
      End
      Begin VB.Label Label 
         Caption         =   "Source File"
         Height          =   195
         Index           =   4
         Left            =   -74925
         TabIndex        =   23
         Top             =   420
         Width           =   885
      End
   End
End
Attribute VB_Name = "frmMTPsetup"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim ProgramDrive$, DataDrive$, UID$, Rdir$, DefaultRTmode%, RealTime As Boolean
Dim DataSourceMode%, DataDestinationMode%, DataDestinationMode2%, UseDefaultRCs%
Dim cDataSource$(0 To 9), cUsername0$(0 To 9), cPassword0$(0 To 9), cboN0%
Dim cDestination$(0 To 9), cUsername$(0 To 9), cPassword$(0 To 9), cboN%
Dim cDestination2$(0 To 9), cUsername2$(0 To 9), cPassword2$(0 To 9), cboN2%
Dim LastIridiumPacketStamp As Date

' MTPrtsim
Public RTSlu%, Rawcycle&, AC$, nUDPpackets&

Const cAppTitle = "FTP Using Internet Transfer Control"

Dim gsServer   As String
Dim gsUser     As String
Dim gsPswd     As String
Dim gsLocalDir As String
Dim gsFTPDir   As String
Dim DIRresult  As String

Sub ParseCSV(ByRef StrgIn$, ByRef s$(), ByRef n%)
'*************************************************************************
' MAY 1993, R. F. Denning
' PARSES THE INPUT STRGIN$ INTO N SUBSTRINGS(S$()), CONTAINING CONSECUTIVE
' CHARS BOUNDED BY ANY DELIMITING CHARS FROM PAR$. CONSECUTIVE DELIMITERS
' (FOR EXAMPLE TWO SPACES IN A ROW) ARE TREATED AS ONE, EXCEPT THAT  ADJACENT
' COMMAS ARE DETECTED AND WILL PRODUCE NULL STRINGS. COMMAS FOLLOWING OTHER
' DELIMITERS ARE SKIPPED.
'
' AN EXAMPLE MIGHT BE:
' STRGIN$ = "A 12345%54321, 66666,,,     93/05/10,09:11:32 RANDOM TEXT"
' PAR$="% /,:" 'I.E. PERCENT, SPACE, SLASH, COMMA, COLON (IN ANY ORDER)
' PARSE STRGIN$, PAR$, S$(), N
' FOR I=1 TO N
' PRINT S$(I)
' NEXT I
'
' THIS WILL PRODUCE THE FOLLOWING OUTPUT:
' A
' 12345
' 54321
' 66666
' [NULL$]
' [NULL$]
' 93
' 05
' 10
' 09
' 11
' 32
' RANDOM
' TEXT
'
' RETURNING WITH N=0 MEANS NO SUBSTRINGS COULD BE FOUND. N=1 means strgin$ was
' all one string without delimiters.
' strgin$, par$ are not modified, s$() is cleared on entry.
'
'*************************************************************************
Dim Delim%, CharsFound%, C$, LastC$, i%, Par$

  Par$ = ","
  For i = 0 To 20: s$(i) = "": Next  'CLEAR OUT OLD MATRIX
  n = 1
  Delim = 1
  CharsFound = 0

  For i = 1 To Len(StrgIn$)
    C$ = Mid$(StrgIn$, i, 1)
    If InStr(Par$, C$) > 0 Then   'Is this character a delimiter?
      If (C$ = ",") And (LastC$ = ",") Then Delim = 0 ': n = n + 1 'Yes
      Delim = 1
      LastC$ = C$
      n = n + 1
    Else                                                 'No
      Delim = 0
      s$(n) = s$(n) + C$                                 'Add character to S$
      CharsFound = CharsFound + 1
    End If
    Debug.Print i; n; s$(n)
  Next i
  If CharsFound = 0 Then n = 0

' TEST CODE:
'StrgIn$ = "Now:Is%The/Time,( 00:00:00, 5/29/1993 ), For All         Good Men To Come To The Aid Of Their Country"
'par$ = "% /,:()"'I.E. SPACE,SLASH, COMMA, COLON, PARENS (IN ANY ORDER)
'parse strgin$, par$, s$(), n
'CLS
'PRINT "Input String:"
'PRINT strgin$
'FOR i = 1 TO n
'PRINT s$(i),
'NEXT i
'PRINT
'PRINT

'PRINT "Parsing TIME$, DATE$:"
'PRINT "Hours, Minutes Seconds now:"
'parse TIME$, ":", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT

'PRINT "Month, Date, Year now:"
'parse DATE$, "-", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT

'PRINT "Parsing DATE$+TIME$"
'parse DATE$ + "/" + TIME$, ":-/", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT


End Sub

Function fSubstituteCRLF(a$, s$) As String
Dim i%, j%, B$, C$
  
  C$ = ""
  j = 1
  Do
    i = InStr(j, a$, s$)
    If i = 0 Then
      C$ = C$ + Right$(a$, Len(a$) - j + 1)
      Exit Do
    End If
    B$ = Mid$(a$, j, i - j)
    C$ = C$ + B$ + vbCrLf
    j = i + 1
  Loop
  fSubstituteCRLF = C$
    
End Function

Sub FTPreceive(DIRresult As String)
Dim bSingleFile As Boolean
Dim sText$, sError$, sFileType$, sMsg$, sSql$
Dim oFTP As New FTP
Static DirectoryLengthLast As Long

  On Error GoTo cmdReceiveError

' See what file(s) to transfer.
  lblStatuss = ""
  If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
  Else
    bSingleFile = False
    sText = "All .txt files"
    sFileType = ".txt"
  End If

' Confirm the transfer.
' If MsgBox("Receive " & sText & "?", vbQuestion + vbYesNo, cAppTitle) = vbNo Then Exit Sub

' Inform the user.
  Screen.MousePointer = vbHourglass
  lblStatuss = "Receiving " & sText

' Do the FTP operation.
  sMsg = "File transfer successful."
  gsLocalDir = txtLocalFolder.Text
  With oFTP
    ' Initialize the FTP connection.
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then GoTo cmdReceiveError
    ' Transfer the file(s)
    If bSingleFile Then
        Call .FTPGet(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdReceiveError
    Else
        Call .FTPGetAll(gsLocalDir, Left$(txtFilenameDate.Text, 19) + sFileType, gsFTPDir, sError, FileList(), nFiles, DirectoryLengthLast)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdReceiveError
        End If
    End If
    ' Close the connection.
    Call .FTPClose
  End With
  lblStatuss = sMsg

' House cleaning.
  Set oFTP = Nothing
  Screen.MousePointer = vbDefault
Exit Sub

cmdReceiveError:
  If sError = "" Then sError = "Error receiving data."
  Call pDisplayError(sError)
  lblStatuss = ""
  Set oFTP = Nothing

End Sub

Private Sub pDisplayError(ByVal sError As String)
Dim sMsg As String

Screen.MousePointer = vbDefault
If Trim$(sError) = "" Then
    sMsg = Err.Description
Else
    sMsg = sError & vbCrLf & vbCrLf & Err.Description
End If
If Err.Number = 0 Then
    MsgBox sMsg, vbCritical, cAppTitle
Else
    MsgBox sMsg & " (" & CStr(Err.Number) & ")", vbCritical, cAppTitle
End If
End Sub

Sub FTPsend()
Dim bSingleFile As Boolean
Dim sText       As String
Dim sError      As String
Dim sFileType   As String
Dim sMsg        As String
Dim oFTP        As New FTP


On Error GoTo cmdSendError
'
' See what file(s) to transfer.
'
lblStatuss = ""
If optXferFiles(1) Then
    bSingleFile = True
    sText = "MTPcurrent.RAW"
    sFileType = "MTPcurrent.RAW"
Else
    bSingleFile = False
    sText = "all .txt files"
    sFileType = ".txt"
End If

'
' Confirm the transfer.
'
If MsgBox("Send " & sText & "?", vbQuestion + vbYesNo, _
   cAppTitle) = vbNo Then Exit Sub
   
'
' Inform the user.
'
Screen.MousePointer = vbHourglass
lblStatuss = "Sending " & sText

'
' Do the FTP operation.
'
sMsg = "File transfer successful."
With oFTP
    '
    ' Initialize the FTP connection.
    '
    If Not .FTPInit(gsServer, gsUser, gsPswd, 0, 0, "", sError) Then
        GoTo cmdSendError
    End If

    '
    ' Transfer the file(s)
    '
    If bSingleFile Then
        Call .FTPPut(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then GoTo cmdSendError
    Else
        Call .FTPPutAll(gsLocalDir, sFileType, gsFTPDir, sError)
        If sError <> "" Then
            sMsg = sError
            GoTo cmdSendError
        End If
    End If
    '
    ' Close the connection.
    '
    Call .FTPClose
End With
lblStatuss = sMsg
'
' House cleaning.
'
Set oFTP = Nothing
Screen.MousePointer = vbDefault
Exit Sub

cmdSendError:
    If sError = "" Then sError = "Error sending data."
    Call pDisplayError(sError)
    lblStatuss = ""
    Set oFTP = Nothing

End Sub

Function fGetFilename(ByVal File$) As String
' Get Filename from last "/" or "\"
Dim i%, j%
  
  fGetFilename = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "\" Or Mid$(File$, i, 1) = "/" Then
      fGetFilename = Mid$(File$, i + 1, Len(File$))
      Exit Function
    End If
  Loop Until i = 1
  
End Function


Sub LoadComboBoxH(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, Cmd$, V$, INIfile$
  
  INIfile$ = "C:\MTP\Setup\Hardware.INI"
  
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0
  C.Clear
  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, Cmd$                       'Read a line
    If Left$(Cmd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, Cmd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(Cmd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, Cmd$                       'Read a line
          If Cmd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Cmd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub


Sub ReadSETUPo(Program$)
  Dim i0%, i1%, i2%, lu%, Cmd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\MTPsetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\MTPsetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, Cmd$                       'Read a line
  
    If Left$(Cmd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, Cmd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
        Exit Sub
      End If
      If Mid$(Cmd$, 2, i0% - 2) = Program$ Then    'Read setup info for only the requested program
        Select Case Mid$(Cmd$, 2, i0% - 2)
        Case "SYSTEM"
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, "=")
            If i1% > 0 Then                     'Look for a command line
              i2% = InStr(i1%, Cmd$, " ")   'and end of its value
              If i2% = 0 Then i2% = Len(Cmd$) + 1
              V$ = Mid$(Cmd$, i1% + 1, i2% - i1% - 1)
              'Debug.Print cmd$
              Select Case Left$(Cmd$, i1% - 1)
              Case "ProgramDrive$":  ProgramDrive$ = V$
              Case "DataDrive$":  DataDrive$ = V$
              Case "UID$": UID$ = V$
              Case "DefaultRTmode"
                DefaultRTmode = Val(V$)
                If DefaultRTmode = 0 Then RealTime = False Else RealTime = True
              Case "DataSourceMode"
                DataSourceMode = Val(V$)
'               cboDataSource.ListIndex = DataSourceMode
              Case "DataDestinationMode"
                DataDestinationMode = Val(V$)
'              cboDestination.ListIndex = DataDestinationMode
              Case "DataDestinationMode2"
                DataDestinationMode2 = Val(V$)
              Case "AlternateRoot"
                If V$ = """""" Then Rdir1$ = "" Else Rdir1$ = Val(V$)
              Case Else
              End Select
            End If
          Loop
        Case "cboDestination"
          cboN = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN = cboN + 1
              cDestination$(cboN) = Left$(Cmd$, i1 - 1)
              i1% = InStr(i1%, Cmd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, Cmd$, " ")
                cUsername(cboN) = Mid$(Cmd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, Cmd$, "=")     'PASSWORD
                i2% = Len(Cmd$)
                cPassword(cboN) = Mid$(Cmd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        Case "cboDestination2"
          cboN2 = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN2 = cboN2 + 1
              cDestination2$(cboN2) = Left$(Cmd$, i1 - 1)
              i1% = InStr(i1%, Cmd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, Cmd$, " ")
                cUsername2(cboN2) = Mid$(Cmd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, Cmd$, "=")     'PASSWORD
                i2% = Len(Cmd$)
                cPassword2(cboN2) = Mid$(Cmd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        
        Case Else
        End Select
      End If
    End If
  Loop
  
Exit_Sub:
  Close (lu%)
End Sub
Sub ReadSETUP(Program$)
  Dim i0%, i1%, i2%, lu%, Cmd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\MTPsetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\MTPsetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
  Do
NextCategory:
    If EOF(lu%) Then GoTo Exit_Sub

    FIsize% = FIsize% + 1
    Input #lu%, Cmd$                       'Read a line
  
    If Left$(Cmd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, Cmd$, "]")
      If i0% = 0 Then
        MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
        Exit Sub
      End If
      If Mid$(Cmd$, 2, i0% - 2) = Program$ Then    'Read setup info for only the requested program
        Select Case Mid$(Cmd$, 2, i0% - 2)
        Case "SYSTEM"
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, "=")
            If i1% > 0 Then                     'Look for a command line
              i2% = InStr(i1%, Cmd$, " ")   'and end of its value
              If i2% = 0 Then i2% = Len(Cmd$) + 1
              V$ = Mid$(Cmd$, i1% + 1, i2% - i1% - 1)
              'Debug.Print cmd$
              Select Case Left$(Cmd$, i1% - 1)
              Case "ProgramDrive$":  ProgramDrive$ = V$
              Case "DataDrive$":  DataDrive$ = V$
              Case "UID$": UID$ = V$
              Case "DefaultRTmode"
                DefaultRTmode = Val(V$)
                If DefaultRTmode = 0 Then RealTime = False Else RealTime = True
              Case "DataSourceMode"
                DataSourceMode = Val(V$)
'               cboDataSource.ListIndex = DataSourceMode
              Case "DataDestinationMode"
                DataDestinationMode = Val(V$)
'              cboDestination.ListIndex = DataDestinationMode
              Case "DataDestinationMode2"
                DataDestinationMode2 = Val(V$)
              Case "AlternateRoot"
                If V$ = """""" Then Rdir1$ = "" Else Rdir1$ = Val(V$)
              Case "UseDefaultRCs"
                UseDefaultRCs = Val(V$)
              Case Else
              End Select
            End If
          Loop
        Case "cboDataSource"
          cboN0 = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN0 = cboN0 + 1
              cDataSource$(cboN0) = Left$(Cmd$, i1 - 1)
              i1% = InStr(i1%, Cmd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, Cmd$, " ")
                cUsername0(cboN0) = Mid$(Cmd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, Cmd$, "=")     'PASSWORD
                i2% = Len(Cmd$)
                cPassword0(cboN0) = Mid$(Cmd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
          
        Case "cboDestination"
          cboN = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN = cboN + 1
              cDestination$(cboN) = Left$(Cmd$, i1 - 1)
              i1% = InStr(i1%, Cmd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, Cmd$, " ")
                cUsername(cboN) = Mid$(Cmd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, Cmd$, "=")     'PASSWORD
                i2% = Len(Cmd$)
                cPassword(cboN) = Mid$(Cmd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        Case "cboDestination2"
          cboN2 = -1
          Do
            If EOF(lu%) Then GoTo Exit_Sub
            FIsize% = FIsize% + 1
            Input #lu%, Cmd$                       'Read a line
            'PRINT cmd$
            If Len(Cmd$) = 0 Then GoTo NextCategory

            i1% = InStr(1, Cmd$, " ")           'Look for first space
            If i1% > 0 Then                     'Look for a command line
              cboN2 = cboN2 + 1
              cDestination2$(cboN2) = Left$(Cmd$, i1 - 1)
              i1% = InStr(i1%, Cmd$, "=")       'USERNAME
              If i1% > 0 Then
                i2% = InStr(i1 + 1, Cmd$, " ")
                cUsername2(cboN2) = Mid$(Cmd$, i1 + 1, i2 - i1 - 1)
                i1% = InStr(i2% + 1, Cmd$, "=")     'PASSWORD
                i2% = Len(Cmd$)
                cPassword2(cboN2) = Mid$(Cmd$, i1 + 1, i2 - i1)
              End If
            End If
          Loop
        
        Case Else
        End Select
      End If
    End If
  Loop
  
Exit_Sub:
  Close (lu%)
End Sub


Sub WriteMTPcurrent(a$)
Dim OUTlu%
Static RAWlu%

  OUTlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPcurrent.RAW" For Output As OUTlu
  Print #OUTlu, a$
  Close OUTlu
  
  If chkLogRaw.Value = 1 Then
    If RAWlu = 0 Then
      RAWlu = FreeFile
      Open "C:\MTP\Data\GH\ATTREX\RAW\GH.RAW" For Output As RAWlu
    End If
    Print #OUTlu, a$
    Print #OUTlu, ""
  End If
  
End Sub

Sub WriteMTPstatus(a$)
Dim OUTlu%

  OUTlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Output As OUTlu
  Print #OUTlu, a$
  Close OUTlu
  
End Sub


Private Sub cmdConnect_Click()
On Error GoTo ErrHandler
Dim LOGlu%

With Winsock1
   'Set the remoteport property.
   .RemoteHost = "130.134.161.26"
   'This should be equal to the localhost property of the remote machine.
   .RemotePort = Trim(cboRemotePort.Text)
   'The localport property cannot be changed,so check if it has already been set.
   If .LocalPort = Empty Then
      .LocalPort = Trim(cboRemotePort.Text)
      .Bind .LocalPort
   End If
End With

With Winsock2
   'Set the remoteport property.
   'This should be equal to the localhost property of the remote machine.
   .RemotePort = Trim(cboStatusPort.Text)
   'The localport property cannot be changed,so check if it has already been set.
   If .LocalPort = Empty Then
      .LocalPort = Trim(cboStatusPort.Text)
      .Bind .LocalPort
   End If
End With


'Make sure that the user can't change the local port
'txtLocalPort.Locked = True

'Show the current status of the connection in the status bar
'StatusBar1.Panels(1).Text = "  Connected to " & Winsock1.RemoteHost & "  "

nUDPpackets = 0
Exit Sub

ErrHandler:
MsgBox "Winsock failed to establish connection with remote server", vbCritical
Resume

End Sub

Sub RealtimeHTML(FQFN$)
Dim lu%, STAlu%
Dim StatusPacket$, a$, s$(0 To 20), Status$                'GH Status Packet

  lu = FreeFile
  Open FQFN$ For Output As lu
  
  Print #lu, "<html>"
  Print #lu, "<head>"
  Print #lu, "<meta content=""Text/html; Charset=ISO-8859-1"""
  Print #lu, "http-equiv=""content-type"">"
  Print #lu, "<title>MTP Realtime</title>"
  Print #lu, "<meta http-equiv=Refresh content=5>"
  Print #lu, "</head>"
  Print #lu, "<body style=""background-image: url(Emily_Luna.jpeg);"">"
  Print #lu, "<h1 style=""color: rgb(255, 255, 255);"">JPL MTP Realtime Temperature Profile</h1>"
'  Print #lu, "<br>"
'  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Universal Time: " + fSecToTstring(UTsec, True) + "</span>&nbsp; "
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Universal Time: " + "121314" + "</span>&nbsp; "
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Latitude: " + Format(Latitude, "#0.###") + "</span>&nbsp;"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Longitude: " + Format(Longitude, "##0.###") + "</span><Br>"
'  Print #lu, "<br>"
'  Print #lu, "<img alt=""MTPprofile.PNG"" src=""MTPprofile.PNG"" style=""width: 1024px; height: 750px;""><br>"
  Print #lu, "<img alt=""MTPprofile.PNG"" src=""MTPprofile.PNG"" style=""width: 800px; height: 600px;""><br>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Status Packet: </span><Br>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">"
  
  STAlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Input As #STAlu
  Line Input #STAlu, StatusPacket$   'Read Status Packet
  Close STAlu
  
  Print #lu, StatusPacket$
  Print #lu, "</span><Br>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">"

  
  a$ = StatusPacket$
  a$ = "MTP,20111029T071925.000,259,10499,12549,14511,9209,10895,13001,7279,8565,11083,54.0098,-60.8687,45.8913,9,4011"

  Call ParseCSV(a$, s$(), n%)
  Status$ = "Status:  " + s(3) + " -  " + s(2) + "  -  Packet no: " + Format(Val(s(17)), "0000")
  Print #lu, Status$
  Print #lu, "          CH1    CH2    CH3"
  Print #lu, Status$
  Status$ = "TGT+ND:  " + Format(s(4), "00000") + "  " + Format(s(5), "00000") + "  " + Format(s(6), "00000")
  Status$ = Status$ + "  TTGT: +" + Format(Val(s(13)), "00.0")
  Print #lu, Status$
  Status$ = "TGT:     " + Format(s(7), "00000") + "  " + Format(s(8), "00000") + "  " + Format(s(9), "00000")
  y = Val(s(14))
  If y < 0# Then Twin$ = Format(y, "#00.0") Else Twin$ = Format(y, "00.0")
  Status$ = Status$ + "  TWIN: " + Twin$
  Print #lu, Status$
  Status$ = "Horizon: " + Format(s(10), "00000") + "  " + Format(s(11), "00000") + "  " + Format(s(12), "00000")
  Status$ = Status$ + "  TND:  +" + Format(Val(s(15)), "00.0")
  Print #lu, Status$
  Print #lu, "</span><Br>"
  
  Print #lu, "</body>"
  Print #lu, "</html>"

  Close lu
End Sub


Private Sub cmdExit_Click()
  
  Unload Me
  End

End Sub

Private Sub cmdOpenFile_Click()
  
  If cmdOpenFile.Caption = "Open File" Then
'   Source$ = "S"  'Realtime Simulation file
'   Open an existing RAW file and use it to write records to the MTPcurrent.RAW file used
'   for real time data analysis. This simulates the real time process of writing to MTPcurrent.RAW
'   Set Initial directory
    CommonDialog1.InitDir = "C:\MTP\Data\"
'   Set Size of FileName buffer
    CommonDialog1.MaxFileSize = 1024
'   Set filters.
    CommonDialog1.Filter = "RAW Files (*.RAW)|*.RAW|All Files (*.*)|*.*"
'   Specify default filter.
    CommonDialog1.FilterIndex = 1
    CommonDialog1.ShowOpen
    Filename$ = CommonDialog1.Filename
    txtFilename.Text = Filename$
    AC$ = Left$(fGetFilename(Filename$), 2)
    For i = 0 To cboPlatforms.ListCount - 1
      If Left$(cboPlatforms.List(i), 2) = AC$ Then Exit For
    Next i
    cboPlatforms.ListIndex = i
    
    RTSlu = FreeFile
    Open Filename$ For Input As RTSlu
    Rawcycle = 0
    
    cmdOpenFile.Caption = "Stop"
    AC$ = Left$(Trim(cboPlatforms.Text), 2)
    tmrRealTimeSim.Interval = 200
    tmrRealTimeSim.Enabled = True
  
  Else
    tmrRealTimeSim.Enabled = False
    cmdOpenFile.Caption = "Open File"
    Close RTSlu
  
  End If

End Sub

Private Sub cmdSetup_Click()
  
  ProgramDrive$ = Left$(drvProgram.Drive, 2)
  DataDrive$ = Left$(drvData.Drive, 2)
  UID$ = txtUID.Text
  DefaultRTmode = Trim(Str(cboRTdefault.ListIndex))
  Rdir$ = Trim(txtAlternateRoot.Text)
  DataSourceMode = Trim(Str(cboDataSource.ListIndex))
  DataDestinationMode = Trim(Str(cboDestination.ListIndex))
  DataDestinationMode2 = Trim(Str(cboDestination2.ListIndex))
  
  WriteSETUP ("SYSTEM")
  
End Sub

Sub WriteSETUP(Program$)
Dim i0%, i1%, i2%, INPlu%, OUTlu%, Cmd$, x$

  If Len(Dir("C:\MTP\Setup\MTPsetup.SAV")) > 0 Then Kill "C:\MTP\Setup\MTPsetup.SAV"
  Name "C:\MTP\Setup\MTPsetup.INI" As "C:\MTP\Setup\MTPsetup.SAV"

  OUTlu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Output As OUTlu%

' Write header info
  Call PrintStr(OUTlu%, "' C:\MTP\Setup\MTPsetup.INI" + " was last written on " + Date$ + " at " + Time$, "", "")
  Call PrintStr(OUTlu%, "' This INI-file follows a few simple but mandatory format requirements.", "", "")
  Call PrintStr(OUTlu%, "' Settings are entered into categories which occupy a single line and", "", "")
  Call PrintStr(OUTlu%, "' must be bracketed by square brackets (e.g. [PATH]). Variables for which", "", "")
  Call PrintStr(OUTlu%, "' settings are assigned must have exactly the same name in the program in", "", "")
  Call PrintStr(OUTlu%, "' which they are used and be followed by an equal sign (=). Spaces are", "", "")
  Call PrintStr(OUTlu%, "' not allowed. Anything beyond the first space in a line is ignored.", "", "")
  Call PrintStr(OUTlu%, "' Blank lines are also ignored, but are required betweeen categories.", "", "")
  Call PrintStr(OUTlu%, "", "", "")

' Now read old file until requested Program is found
  Call PrintStr(OUTlu%, "[SYSTEM]", "", "")
  x$ = Left$(drvProgram, 2)
  Call PrintStr(OUTlu%, "ProgramDrive$", x$, "Default Program hard drive")
  x$ = Left$(drvData, 2)
  Call PrintStr(OUTlu%, "DataDrive$", x$, "Default Data hard drive")
  x$ = txtUID.Text
  Call PrintStr(OUTlu%, "UID$", x$, "Default user ID (initials)")
  x$ = Str(cboRTdefault.ListIndex)
  Call PrintStr(OUTlu%, "DefaultRTmode", Trim(x$), "Default Real Time Mode (0=FALSE, 1=TRUE)")
  x$ = Str(cboDataSource.ListIndex)
  Call PrintStr(OUTlu%, "DataSourceMode", Trim(x$), "Where is MTPcurrent.RAW?")
  x$ = Str(cboDestination.ListIndex)
  Call PrintStr(OUTlu%, "DataDestinationMode", Trim(x$), "Data Destination 1")
  x$ = Str(cboDestination2.ListIndex)
  Call PrintStr(OUTlu%, "DataDestinationMode2", Trim(x$), "Data Destination 2")
  x$ = txtAlternateRoot.Text
  If Len(x$) = 0 Then x$ = """"""
  Call PrintStr(OUTlu%, "AlternateRoot", x$, "")
  x$ = Str(cboDefaultRCs.ListIndex)
  Call PrintStr(OUTlu%, "UseDefaultRCs", Trim(x$), "Use whatever RCs are in the /Mission/RC/ folder (0=FALSE, 1=TRUE)")
  Call PrintStr(OUTlu%, "", "", "")
  
  Call PrintStr(OUTlu%, "[cboDataSource]", "", "")
  For i = 0 To cboN0
    x$ = cboDataSource.List(i) + " USERNAME=" + Trim(cUsername0(i)) + " PASSWORD=" + Trim(cPassword0(i))
    Print #OUTlu, x$
  Next i
  Print #OUTlu, ""
  
  Call PrintStr(OUTlu%, "[cboDestination]", "", "")
  For i = 0 To cboN
    x$ = cboDestination.List(i) + " USERNAME=" + Trim(cUsername(i)) + " PASSWORD=" + Trim(cPassword(i))
    Print #OUTlu, x$
  Next i
  Print #OUTlu, ""
  
  Call PrintStr(OUTlu%, "[cboDestination2]", "", "")
  For i = 0 To cboN2
    x$ = cboDestination2.List(i) + " USERNAME=" + Trim(cUsername2(i)) + " PASSWORD=" + Trim(cPassword2(i))
    Print #OUTlu, x$
  Next i
  Print #OUTlu, ""
  
  Print #OUTlu, "[EOF]"
  Close OUTlu%

End Sub
Sub PrintStr(lu%, x1$, x$, x2$)
' Print to lu%, x1$=x$         'x2$
' eg            PgmDrive$=c:   'Program Drive
Dim fmt$, L%

  If x$ = "" Then
    If lu% = 0 Then Debug.Print x1$ Else Print #lu%, x1$
  Else
    fmt$ = x1$ + "=" + x$
    L% = 30 - Len(fmt$)
    If L% < 0 Then L% = 0
    fmt$ = fmt$ + Space$(L%) + "'" + x2$
    If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$
  End If

End Sub


Private Sub cmdStartFTP_Click()
  
  If optFTPdirection(0).Value = True Then
    Timer1.Interval = 1000
    Timer1.Enabled = True
    FileIndex = 0
    nFiles = 0
    'Call FTPreceive
  Else
    Timer1.Enabled = True
    FileIndex = 0
    nFiles = 0
    Call FTPsend
  End If

End Sub



Private Sub Command1_Click()
Dim a$, s$(0 To 20), n%, x$, y!, Twin$

  a$ = "MTP,20111029T071925.000,259,10499,12549,14511,9209,10895,13001,7279,8565,11083,54.0098,-60.8687,45.8913,9,4011"

      Call ParseCSV(a$, s$(), n%)
      lblStatus(0).Caption = "Status:  " + s(3) + " -  " + s(2) + "  -  Packet no: " + Format(Val(s(17)), "0000")
      x$ = "TGT+ND:  " + Format(s(4), "00000") + "  " + Format(s(5), "00000") + "  " + Format(s(6), "00000")
      lblStatus(1).Caption = x$ + "  TTGT: +" + Format(Val(s(13)), "00.0")
      x$ = "TGT:     " + Format(s(7), "00000") + "  " + Format(s(8), "00000") + "  " + Format(s(9), "00000")
      y = Val(s(14))
      If y < 0# Then Twin$ = Format(y, "#00.0") Else Twin$ = Format(y, "00.0")
      lblStatus(2).Caption = x$ + "  TWIN: " + Twin$
      x$ = "Horizon: " + Format(s(10), "00000") + "  " + Format(s(11), "00000") + "  " + Format(s(12), "00000")
      lblStatus(3).Caption = x$ + "  TND:  +" + Format(Val(s(15)), "00.0")


End Sub

Private Sub Command2_Click()
  Call RealtimeHTML("C:\MTP\RTA\RAW\index.html")
End Sub

Private Sub Form_DblClick()

  Call CaptureFormMJ(Me, "", 3, True)

End Sub

Private Sub Form_Load()
Dim i%, Filename$
  
  With Me
'  .Width = 4980
'  .Height = 3225
  .Left = (Screen.Width - .Width) / 2
  .Top = (Screen.Height - .Height) / 2
  End With

  With cboRTdefault
  .AddItem "FALSE"
  .AddItem "TRUE"
  .ListIndex = 0
  End With
  
  With cboDefaultRCs
  .AddItem "FALSE"
  .AddItem "TRUE"
  .ListIndex = 0
  End With
  
  With cboSimTime
  .AddItem "1"
  .AddItem "2"
  .AddItem "5"
  .AddItem "10"
  .AddItem "15"
  .AddItem "30"
  .AddItem "60"
  .ListIndex = 2
  End With
  
'  With cboDataSource
'  .AddItem "Offline: Existing File"              'F
'  .AddItem "Offline: Test File"                  'T
'  .AddItem "Realtime: Data Unit"                 'D
'  .AddItem "Realtime: MTPcurrent.RAW"            'S
' .AddItem "Realtime: REVEAL"                    'R
'  .ListIndex = 0
'  End With
  
  ReadSETUP ("cboDataSource")   'Only location where MTPsetup.INI is read
  For i = 0 To cboN0
  With cboDataSource
  .AddItem cDataSource(i)
  End With
  Next i
  
' Edit MTPsetup.INI if address, username or password change
  ReadSETUP ("cboDestination")   'Only location where MTPsetup.INI is read
  For i = 0 To cboN
  With cboDestination
  .AddItem cDestination(i)
'  .AddItem "C:\www\missions\realtime\"
'  .AddItem "ftp://137.79.98.116/home/mahoney/"
'  .AddItem "ftp://192.168.84.1/var/www/html/mtp/"
'  .AddItem "Y:\jpl\group\mtp\www\missions\realtime\"
  End With
  Next i
  
  ReadSETUP ("cboDestination2")   'Only location where MTPsetup.INI is read
  For i = 0 To cboN2
  With cboDestination2
  .AddItem cDestination2(i)
  End With
  Next i

' Get default drives
  ReadSETUP ("SYSTEM")   'Only location where MTPsetup.INI is read
  cboDataSource.ListIndex = DataSourceMode
  cboDestination.ListIndex = DataDestinationMode
  cboDestination2.ListIndex = DataDestinationMode2
  cboDefaultRCs.ListIndex = UseDefaultRCs
' Set Drive List Boxes to reflect default drives
  For i = 0 To drvProgram.ListCount
    If Left$(drvProgram.List(i), 2) = ProgramDrive$ Then drvProgram.ListIndex = i
  Next i
  For i = 0 To drvData.ListCount
    If Left$(drvData.List(i), 2) = DataDrive$ Then drvData.ListIndex = i
  Next i
  
  cboRTdefault.ListIndex = DefaultRTmode

' User ID
  txtUID.Text = UID$

' Alternate Root Path, normally ""
  txtAlternateRoot.Text = Rdir$
  
' MTPrtsim
  
  LoadComboBoxH cboPlatforms, "PLATFORM"

  With cboMTPcurrent
  .AddItem "C:\MTP\RTA\RAW\MTPcurrent.RAW"               'Local computer
  .AddItem "K:\jpl\group\mtp\www\missions\realtime\MTPcurrent.RAW"     'Mapped drive
  .AddItem "ftp://137.79.98.116/home/mahoney/MTPcurrent.RAW"           'FTP Server
  .ListIndex = 0
  End With
  
' MTPftp
'
' Initialize Logon/FTP parameters.
' User should be prompted for logon info.
'
  gsUser = txtUsername.Text         '"anonymous"    'User ID to logon to FTP server.
  gsPswd = txtPassword.Text         '"Michael.J.Mahoney@jpl.nasa.gov"   'Password to logon to FTP server.

  gsServer = txtFTPsite.Text        '"ftp.eol.ucar.edu"       'FTP server name.
  gsLocalDir = txtLocalFolder.Text  '"C:\Temp"  'Folder on local client holding files to FTP.
  gsFTPDir = txtRemoteFolder.Text   '"/pub/temp/users/haggerty"     'Folder on FTP server to xfer files to/from

  Timer1.Interval = 60000           'check once a minute
  Timer1.Enabled = False
  MonthView1.Value = Date
  MonthView1_DateClick Date
    
  With cboRemotePort
  .AddItem "10200"
  .AddItem "10201"    'Port for GH Ku Status packets
  .AddItem "10210"
  .AddItem "10300"
  .AddItem "10400"
  .ListIndex = 0
  End With
  
  tmrWatchdog.Enabled = True
End Sub



Private Sub MonthView1_DateClick(ByVal DateClicked As Date)
Dim YYYYMMDD$
Dim fso As New FileSystemObject

  YYYYMMDD$ = Format(Year(DateClicked), "0000") + Format(Month(DateClicked), "00") + Format(Day(DateClicked), "00")
' Debug.Print DateClicked; "  "; yyyymmdd$
  txtFilenameDate.Text = "MTPcurrent_" & YYYYMMDD$
  If Not fso.FolderExists(txtLocalFolder.Text + YYYYMMDD$ + "/") Then
    fso.CreateFolder (txtLocalFolder.Text + YYYYMMDD$ + "/")
  End If
  txtLocalFolder.Text = txtLocalFolder.Text + YYYYMMDD$ + "/"

End Sub

Private Sub Timer1_Timer()
Dim INPlu%, OUTlu%, Line$
Static nFilesLast%

  Timer1.Enabled = False
  nFiles = Val(txtFilesRead.Text)
  If nFiles > 0 Then
  ' Set appropriate FileIndex
    If FileIndex = 0 Then
      If chkUseLatest.Value = 1 Then
        FileIndex = nFiles
      Else
        FileIndex = 1
        Timer1.Interval = 4000
      End If
    End If
    If FileIndex = nFiles Then Timer1.Interval = 60000
  End If
  
' Copy next RAW file to MTPcurrent.RAW
  If nFiles > FileIndex Then
    INPlu = FreeFile
    Open txtLocalFolder.Text + FileList(FileIndex) For Input As #INPlu
    OUTlu = FreeFile
    Open "C:\MTP\RTA\RAW\MTPcurrent.RAW" For Output As #OUTlu
    Do
      Line Input #INPlu, Line$
      Print #OUTlu, Line$
    Loop Until EOF(INPlu)
    Close INPlu, OUTlu
    txtFilenameDate.Text = FileList(FileIndex)
    FileIndex = FileIndex + 1
    DoEvents
  End If
  
' Check for more files
  Call FTPreceive(DIRresult)
  txtFilesRead.Text = Format(nFiles, "0")
  txtCurrent.Text = Format(FileIndex, "0")
  'Debug.Print "FileIndex= "; FileIndex; " "; Date; " "; Time; FileDateTime("C:\MTP\RTA\RAW\MTPcurrent.RAW")
  Timer1.Enabled = True

End Sub

Private Sub tmrRealTimeSim_Timer()
Dim a$, B$, C$, D$, E$, small_d$, IWG1$, Pt$, RAWlu%
'WB57, ER2 and M55 Format
'AG037:16:30:34 +9.8492 -83.9317 153.3   6.3    .6  6984  6570 -11.3  8.2  70.6  -183.4 1 128 060206 162826
'B 13694 13566 13694 13669 13523 13668 13649 13516 13674 13625 13487 13650 13607 13477 13643
'C 13580 13466 13641 13543 13450 13636 13505 13428 13631 13464 13419 13623 13403 13392 13621
'D 4B7 4C4 543 47A FFF 9F2 2E8 B6C 4E4 7CA 653 5CA 481 7E3 7E4 24C
'E 15005 15096 15251 14046 13984 14094

'DC8 Format
'Instrument Start at 01-20-2005 19:37:40-JPL Phoenix MTP- v.20021120
'AD020:19:42:55 +34.918 -117.868  106 +00.0 +00.0 02281 02059 +12.0 0000   0000 +180.0 1 000 050120 194430
'B 08672 08137 07723 08671 08135 07719 08671 08140 07729 08676 08134 07718 08668 08115 07724
'C 08672 08122 07728 08699 08161 07750 08723 08195 07790 08738 08206 07778 08727 08202 07784
'd 40.48 + 47.44 + 32.6 + 40.37 + 22.1 + 21.96 + 1.01 + 21.74 + 28.83 + 1.96 + 2.04 + 30.28 + 35.75 + 2.48 + 1.9 + 41.87 + 1.01
'D 528 424 64C 501 CD0 CD2 812 C82 6BA 7AE 7F1 68D 5B7 997 76B 4DB 806
'E 15665 14130 11035 08755 08239 07821

'NGV Format
'A 2007-12-18 23:21:40 002.30 00.05 001.11 00.10 01.30 0.01 267.00 0.20
'IWG1,20071218T232140,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
'B 009653 013837 014684 009729 013910 014784 010241 014390 015307 010302 014467 015363 010306 014442 015380 010310 014441 015373 010310 014453 015369 010299 014442 015366 010292 014442 015369 009755 013976 014663
'M01: 2943 2109 2884 3097 3086 2954 2479 2936
'M02: 2101 1247 1429 4095 1531 1191 4095 1060
'Pt: 2436 14145 14143 16383 14187 14163 14028 14768
'E 011467 015347 016344 009836 013969 014870

  tmrRealTimeSim.Enabled = False

  Do
    Line Input #RTSlu, a$
  Loop Until Left$(a$, 1) = "A" Or EOF(RTSlu)
  
  If EOF(RTSlu) Then
    Close RTSlu
    If chkReOpen.Value = 1 Then
      RTSlu = FreeFile
      Open txtFilename.Text For Input As RTSlu
      Rawcycle = 0
      tmrRealTimeSim.Enabled = True
    Else
      tmrRealTimeSim.Enabled = False
    End If
    Exit Sub
  End If
  
  If AC$ = "NG" Then Line Input #RTSlu, IWG1$
  Line Input #RTSlu, B$
  Line Input #RTSlu, C$
  If AC$ = "DC" Then Line Input #RTSlu, small_d$
  Line Input #RTSlu, D$
  If AC$ = "NG" Then Line Input #RTSlu, Pt$
  Line Input #RTSlu, E$

  RAWlu = FreeFile
  Open cboMTPcurrent.Text For Output As RAWlu
  lstCurrentRecord.Clear
  Print #RAWlu, a$
  lstCurrentRecord.AddItem a$
  
  If AC$ = "NG" Then
    Print #RAWlu, IWG1$
    lstCurrentRecord.AddItem IWG1$
  End If
  
  Print #RAWlu, B$
  lstCurrentRecord.AddItem B$
  
  Print #RAWlu, C$
  lstCurrentRecord.AddItem C$
  
  If AC$ = "DC" Then
    Print #RAWlu, small_d$
    lstCurrentRecord.AddItem small_d$
  End If
  
  Print #RAWlu, D$
  lstCurrentRecord.AddItem D$
  
  If AC$ = "NG" Then
    Print #RAWlu, Pt$
    lstCurrentRecord.AddItem Pt$
  End If
  
  Print #RAWlu, E$
  lstCurrentRecord.AddItem E$
  
  Print #RAWlu, ""
  Close RAWlu
  
  Rawcycle = Rawcycle + 1
  txtCurrentRecord = Str(Rawcycle)
  tmrRealTimeSim.Interval = Val(cboSimTime.Text) * 1000
  tmrRealTimeSim.Enabled = True

End Sub


Private Sub tmrWatchdog_Timer()
' Watchdog timer to make sure that MTP is working properly
Dim INPlu%, CurrentSec&, PathRAW$, FilenameRAW$, a$, B$, CurrentStamp As Date
Dim CountDifference%, i%, j%, k%, n1%, n2%, n3%, dn%, LOok As Boolean, dSec&
Static LastSec&, LastStamp As Date, Pass As Boolean

  tmrWatchdog.Enabled = False
  dn = 30 'count threshold
  PathRAW = "C:/MTP/RTA/RAW/"
  FilenameRAW = "MTPcurrent.RAW"
  If Len(Dir(PathRAW + FilenameRAW)) = 0 Then
    If Not Pass Then MsgBox PathRAW + FilenameRAW + " does not exist!", vbOK
    Pass = True
    tmrWatchdog.Enabled = True
    Exit Sub
  End If
  
  If chkMTPcurrentRAW.Value = 1 Then
    CurrentStamp = FileDateTime(PathRAW + FilenameRAW)
    If LastStamp = Empty Then LastStamp = CurrentStamp
    If CurrentStamp > LastStamp Then dSec = 0: LastSec = 0
    CurrentSec = Timer
    If LastSec = 0 Then LastSec = CurrentSec
    If CurrentSec > LastSec Then
      dSec = CurrentSec - LastSec
    End If
    If dSec > 16 And LastStamp = CurrentStamp Then
      txtMTPcurrentRAW.BackColor = &HFF&   'red
    Else
      txtMTPcurrentRAW.BackColor = &HFF00& 'green
      
    End If
    txtMTPcurrentRAW.Text = Str(CurrentStamp) + " -- Elapsed Time: " + Str(dSec) + " sec"
    DoEvents
    LastStamp = CurrentStamp
  End If
'Non-GV
'AG103:13:59:11 +35.126 -117.508 341.2  -7.5   9.3  5574  5483   0.0  +0.0 +0.00 -175.1 1 224 100414 135828
'B  8013  9599 11819  8071  9621 11823  8093  9628 11823  8150  9665 11840  8176  9689 11856
'C  8200  9698 11845  8238  9729 11874  8257  9744 11877  8284  9766 11912  8299  9780 11929
'D 474 599 60F 46B D5E 000 821 911 690 825 655 695 34E 7D3 7D3 7DA
'E  9613 11527 13516  8320  9827 11977
'
'GV
'A 20100316 20:32:19 -01.04 00.00 +00.31 00.00 +01.60 0.00 290.79 00.28 +39.913 +0.000 -105.118 +0.000 +072574 +072560
'IWG1,20100316T203218,39.9128,-105.118,1718.66,,5249.65,,1.00288,0.447474,0.784894,0.00130787,-0.0011557,36.4897,nan,nan,-1.04252,0.30899,0,0,17.9387,-14.856,18.1453,835.156,0.001,,0,0,0,45.6079,44.3921,,
'B 018492 020137 019264 018510 020150 019284 018529 020165 019297 018551 020183 019315 018603 020237 019383 018669 020325 019464 018909 020580 019766 018957 020634 019818 018945 020622 019807 018906 020579 019764
'M01: 2928 2337 2928 3087 1922 2922 2434 2948
'M02: 2011 1224 2131 2194 2317 1179 4095 1367
'Pt: 2157 13799 13808 10995 13404 13383 13254 14443
'E 020928 022670 022208 019199 020912 020145

  If chkLOchanging.Value = 1 Then
    INPlu = FreeFile
    Open PathRAW + FilenameRAW For Input As INPlu
    If EOF(INPlu) Then
      MsgBox PathRAW + FilenameRAW + " has reached an EOF!", vbOKOnly
      tmrWatchdog.Enabled = True
      Close INPlu
      Exit Sub
    End If
    Do
      Line Input #INPlu, a$
    Loop Until Left$(a$, 1) = "B" Or EOF(INPlu)
    Close INPlu
    If Len(a$) < 30 Then
      tmrWatchdog.Enabled = True
      Exit Sub
    End If
    txtLOchanging.Text = a$
    i% = InStr(4, a$, " ")
    n1 = Val(Mid$(a$, 3, i - 3))
    j% = InStr(i + 2, a$, " ")
    n2 = Val(Mid$(a$, i + 1, j - i - 1))
    k% = InStr(j + 2, a$, " ")
    n3 = Val(Mid$(a$, j + 1, k - j - 1))
    LOok = True
    If Abs(n1 - n2) < dn And Abs(n2 - n3) < dn Then LOok = False
    If Not LOok Then
      txtLOchanging.BackColor = &HFF&   'red
    Else
      txtLOchanging.BackColor = &HFF00& 'green
    End If
  End If
  
  tmrWatchdog.Enabled = True
  
End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'A$ is the text that has just arrived from across the socket
Dim a$, INPlu$, Status_Packet$, s$(0 To 20), x$
Static LOGlu%

  If chkLogPackets.Value = 1 And nUDPpackets = 0 Then
    LOGlu = FreeFile
    Open "C:\MTP\RTA\RAW\LowrateData_Log.txt" For Output As LOGlu
  End If

' Get the new text
  Winsock1.GetData a$
  If chkLogPackets.Value = 1 Then Print #LOGlu, a$
  
' Show the new text
  If Left$(a$, 5) = "MTP,A" Then
    a$ = Right$(a$, Len(a$) - InStr(1, a$, ","))  'strip "MTP,"
    a$ = fSubstituteCRLF(a$, ",")
    txtUDPin.Text = a$
    Debug.Print a$
    Call WriteMTPcurrent(a$)
  End If
  DoEvents
' Show the byte size of this transmission and packet count in the statusbar
  StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
  nUDPpackets = nUDPpackets + 1
  StatusBar1.Panels(3).Text = " UDP Packets = " & Str(nUDPpackets)
  
' Write a Status Packet if they are not being received by Iridium
  If 86400 * (Date + Time - LastIridiumPacketStamp) > 20 Then
    INPlu = FreeFile
    Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Input As INPlu
    Line Input #INPlu, Status_Packet
    Close INPlu
    Call ParseCSV(Status_Packet, s$(), n%)
'MTP,20111029T071925.000,259,10499,12549,14511,9209,10895,13001,7279,8565,11083,54.0098,-60.8687,45.8913,9,4011
'MTP,20090603T210816.000,3,15412,14875,15082,14388,13715,13824,15883,15335,15339,-15.3,-40.5,34.5,14,322
'1   2                   3 4     5     6     7     8     9     10     11    12    13    14    15   16 17
    lblStatus(0).Caption = "Status:  " + s(3) + " -  " + s(2) + "  -  Packet no: " + Format(Val(s(17)), "0000")
    x$ = "TGT+ND:  " + Format(s(4), "00000") + "  " + Format(s(5), "00000") + "  " + Format(s(6), "00000")
    lblStatus(1).Caption = x$ + "  TTGT: +" + Format(Val(s(13)), "00.0")
    x$ = "TGT:     " + Format(s(7), "00000") + "  " + Format(s(8), "00000") + "  " + Format(s(9), "00000")
    y = Val(s(14))
    If y < 0# Then Twin$ = Format(y, "#00.0") Else Twin$ = Format(y, "00.0")
    lblStatus(2).Caption = x$ + "  TWIN: " + Twin$
    x$ = "Horizon: " + Format(s(10), "00000") + "  " + Format(s(11), "00000") + "  " + Format(s(12), "00000")
    lblStatus(3).Caption = x$ + "  TND:  +" + Format(Val(s(15)), "00.0")
  End If

End Sub


Private Sub Winsock2_DataArrival(ByVal bytesTotal As Long)
'A$ is the text that has just arrived from across the socket
Dim a$, s$(0 To 20), n%, x$, y!, DandT As Date, YYYYMMDD$, HHMMSS$

' Get the new text
  Winsock2.GetData a$
'  If chkLogPackets.Value = 1 Then Print #LOGlu, A$
  
' Show the new text
    If chkAllpackets.Value = 1 Then
      txtStatusPackets.Text = a$
    End If
    If Left$(a$, 5) = "MTP,2" Then
      txtStatusPackets.Text = a$
      Call WriteMTPstatus(a$)
      Call ParseCSV(a$, s$(), n%)
'MTP,20111029T071925.000,259,10499,12549,14511,9209,10895,13001,7279,8565,11083,54.0098,-60.8687,45.8913,9,4011
'MTP,20090603T210816.000,3,15412,14875,15082,14388,13715,13824,15883,15335,15339,-15.3,-40.5,34.5,14,322
'1   2                   3 4     5     6     7     8     9     10     11    12    13    14    15   16 17
      lblStatus(0).Caption = "Status:  " + s(3) + " -  " + s(2) + "  -  Packet no: " + Format(Val(s(17)), "0000")
      x$ = "TGT+ND:  " + Format(s(4), "00000") + "  " + Format(s(5), "00000") + "  " + Format(s(6), "00000")
      lblStatus(1).Caption = x$ + "  TTGT: +" + Format(Val(s(13)), "00.0")
      x$ = "TGT:     " + Format(s(7), "00000") + "  " + Format(s(8), "00000") + "  " + Format(s(9), "00000")
      y = Val(s(14))
      If y < 0# Then Twin$ = Format(y, "#00.0") Else Twin$ = Format(y, "00.0")
      lblStatus(2).Caption = x$ + "  TWIN: " + Twin$
      x$ = "Horizon: " + Format(s(10), "00000") + "  " + Format(s(11), "00000") + "  " + Format(s(12), "00000")
      lblStatus(3).Caption = x$ + "  TND:  +" + Format(Val(s(15)), "00.0")
      YYYYMMDD$ = Left$(s(2), 8)
      HHMMSS$ = Mid$(s(2), 10, 6)
      Call StringsToDate(YYYYMMDD$, HHMMSS$, DandT)
      LastIridiumPacketStamp = DandT
    End If
  DoEvents
' Show the byte size of this transmission and packet count in the statusbar
'  StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
'  nUDPpackets = nUDPpackets + 1
'  StatusBar1.Panels(3).Text = " UDP Packets = " & Str(nUDPpackets)

End Sub

Sub StringsToDate(YYYYMMDD$, HHMMSS$, DandT As Date)
Dim Iyear%, Imonth%, Iday%, Ihour%, Imin%, Isec%
' Convert YYYYMMDD$ and HHMMSS$ strings to DandT Date type
  Iyear = Left$(YYYYMMDD$, 4)
  Imonth = Mid$(YYYYMMDD$, 5, 2)
  Iday = Right$(YYYYMMDD$, 2)
  Ihour = Left$(HHMMSS$, 2)
  Imin = Mid$(HHMMSS$, 3, 2)
  Isec = Right$(HHMMSS$, 2)
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)

End Sub

