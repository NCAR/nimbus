VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.1#0"; "MSCOMCTL.OCX"
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmMTPbin 
   BackColor       =   &H8000000A&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "MTP Data Analysis"
   ClientHeight    =   5136
   ClientLeft      =   120
   ClientTop       =   744
   ClientWidth     =   15264
   ForeColor       =   &H00C0C0C0&
   Icon            =   "MTPbin.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5130
   ScaleMode       =   0  'User
   ScaleWidth      =   15420.6
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdDirection 
      Caption         =   "&Repeat"
      Height          =   375
      Index           =   3
      Left            =   11160
      TabIndex        =   929
      Top             =   4725
      Width           =   780
   End
   Begin MSComctlLib.ProgressBar ProgressBar1 
      Height          =   255
      Left            =   720
      TabIndex        =   851
      Top             =   4770
      Width           =   5085
      _ExtentX        =   8975
      _ExtentY        =   445
      _Version        =   393216
      Appearance      =   1
   End
   Begin VB.TextBox txtCommLine 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.4
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   105
      TabIndex        =   829
      Text            =   "Text1"
      Top             =   4365
      Width           =   5670
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "Open &Tools"
      Height          =   375
      Index           =   14
      Left            =   14175
      TabIndex        =   675
      Top             =   420
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "    Open    &Next Flight"
      Height          =   525
      Index           =   13
      Left            =   5940
      TabIndex        =   673
      Top             =   4560
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "  Write &FLTINFO"
      Height          =   525
      Index           =   6
      Left            =   5940
      TabIndex        =   672
      Top             =   4050
      Width           =   1095
   End
   Begin VB.CommandButton cmdCopyRaw 
      Caption         =   "Copy Flight Files"
      Height          =   525
      Left            =   14160
      TabIndex        =   524
      ToolTipText     =   "Copy RAW, INI and CAL files"
      Top             =   1410
      Width           =   1095
   End
   Begin VB.TextBox txtGoodTrop 
      Alignment       =   2  'Center
      BackColor       =   &H0000C000&
      ForeColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   13095
      MultiLine       =   -1  'True
      TabIndex        =   393
      Text            =   "MTPbin.frx":0442
      Top             =   4800
      Width           =   975
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&View REF"
      Height          =   360
      Index           =   2
      Left            =   14160
      TabIndex        =   381
      Top             =   840
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Plot CTC"
      Height          =   375
      Index           =   1
      Left            =   14160
      TabIndex        =   380
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Retrieve"
      Height          =   360
      Index           =   11
      Left            =   14130
      TabIndex        =   342
      ToolTipText     =   "Perform retrieval on entire RAW file"
      Top             =   2430
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Write MP"
      Height          =   375
      Index           =   9
      Left            =   14160
      TabIndex        =   323
      Top             =   3690
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Edit REF"
      Height          =   360
      Index           =   7
      Left            =   14160
      TabIndex        =   321
      Top             =   2850
      Width           =   1095
   End
   Begin VB.TextBox txtBadCounts 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.4
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   8280
      MultiLine       =   -1  'True
      TabIndex        =   19
      Top             =   4440
      Width           =   615
   End
   Begin VB.TextBox txtFileRecords 
      Alignment       =   1  'Right Justify
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.4
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   8280
      MultiLine       =   -1  'True
      TabIndex        =   17
      Top             =   4800
      Width           =   615
   End
   Begin VB.TextBox txtRecord 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.4
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   8280
      MultiLine       =   -1  'True
      TabIndex        =   16
      Top             =   4080
      Width           =   615
   End
   Begin VB.Timer tmrSerialStart 
      Interval        =   1000
      Left            =   14160
      Top             =   3960
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&COM In"
      Height          =   375
      Index           =   5
      Left            =   14160
      TabIndex        =   15
      Top             =   0
      Width           =   1095
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Open RAW"
      Height          =   360
      Index           =   0
      Left            =   14145
      TabIndex        =   14
      Top             =   2010
      Width           =   1095
   End
   Begin VB.Timer tmrGetCycle 
      Left            =   14640
      Top             =   3930
   End
   Begin VB.Frame Frame 
      Caption         =   "Universal Time"
      Height          =   615
      Index           =   12
      Left            =   12015
      TabIndex        =   11
      Top             =   4080
      Width           =   2055
      Begin VB.TextBox txtUT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   13
         Top             =   240
         Width           =   735
      End
      Begin VB.ComboBox cboUT 
         Height          =   315
         Left            =   840
         TabIndex        =   12
         Text            =   "hh:mm:ss"
         Top             =   240
         Width           =   1095
      End
   End
   Begin VB.CommandButton cmdDirection 
      Caption         =   "&Stop"
      Height          =   375
      Index           =   1
      Left            =   9615
      TabIndex        =   10
      Top             =   4740
      Width           =   825
   End
   Begin VB.Frame frmGOTO 
      Caption         =   "Go To Record"
      Height          =   615
      Left            =   9000
      TabIndex        =   6
      Top             =   4080
      Width           =   2955
      Begin VB.TextBox txtGOTO 
         Height          =   285
         Left            =   120
         TabIndex        =   9
         Top             =   240
         Width           =   855
      End
      Begin VB.ComboBox cboGOTO 
         Height          =   315
         Left            =   1080
         TabIndex        =   8
         Text            =   "Rec No"
         Top             =   240
         Width           =   1230
      End
      Begin VB.CommandButton cmdGO 
         Caption         =   "&GO"
         Height          =   495
         Left            =   2370
         TabIndex        =   7
         Top             =   120
         Width           =   540
      End
   End
   Begin VB.CommandButton cmdDirection 
      Caption         =   "&Fwd"
      Height          =   375
      Index           =   2
      Left            =   10500
      TabIndex        =   5
      Top             =   4740
      Width           =   600
   End
   Begin VB.CommandButton cmdDirection 
      Caption         =   "&Back"
      Height          =   375
      Index           =   0
      Left            =   9015
      TabIndex        =   4
      Top             =   4740
      Width           =   555
   End
   Begin VB.TextBox txtGoodScan 
      Alignment       =   2  'Center
      BackColor       =   &H0000C000&
      ForeColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   12015
      MultiLine       =   -1  'True
      TabIndex        =   3
      Text            =   "MTPbin.frx":0449
      Top             =   4800
      Width           =   975
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "E&xit"
      Height          =   375
      Index           =   3
      Left            =   14190
      TabIndex        =   0
      Top             =   4740
      Width           =   1095
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   14310
      Top             =   4125
      _ExtentX        =   995
      _ExtentY        =   995
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4035
      Left            =   0
      TabIndex        =   21
      ToolTipText     =   "Lab Cal with 90 C target at Loc 6"
      Top             =   0
      Width           =   14085
      _ExtentX        =   24850
      _ExtentY        =   7112
      _Version        =   393216
      Tabs            =   11
      TabsPerRow      =   11
      TabHeight       =   520
      BackColor       =   12632256
      TabCaption(0)   =   "&Pgm Setup"
      TabPicture(0)   =   "MTPbin.frx":0450
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "label(130)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "label(20)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "label(41)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Frame(8)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "txtTavg"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "chkPrintDiagnostics"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "txtPrintUTsec"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "Frame(0)"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "txtTimerSpeed"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "txtREFfile"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "cmdREF(8)"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "Frame(1)"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "tmrRealTime"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "tmrRealTimeSim"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "chkFixTO"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "cmdStrip"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "cmdRemoveFECerr"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "cmdAssorted(2)"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "cmdAssorted(4)"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "cmdRTcatchup"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "cmdAssorted(3)"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "cmdAssorted(5)"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "cmdAssorted(6)"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "cboAverageNcycles"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "cmdAssorted(7)"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "cmdREF(10)"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "cmdREF(12)"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "Winsock1"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "cmdAssorted(0)"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "txtATP_RAOB2"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).ControlCount=   30
      TabCaption(1)   =   "ABCDE"
      TabPicture(1)   =   "MTPbin.frx":046C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "txtLine(5)"
      Tab(1).Control(1)=   "txtLine(4)"
      Tab(1).Control(2)=   "txtLine(3)"
      Tab(1).Control(3)=   "txtLine(2)"
      Tab(1).Control(4)=   "txtLine(1)"
      Tab(1).Control(5)=   "txtLine(0)"
      Tab(1).Control(6)=   "label(143)"
      Tab(1).Control(7)=   "label(67)"
      Tab(1).Control(8)=   "label(57)"
      Tab(1).Control(9)=   "label(55)"
      Tab(1).Control(10)=   "label(54)"
      Tab(1).Control(11)=   "label(42)"
      Tab(1).ControlCount=   12
      TabCaption(2)   =   "&IWG1"
      TabPicture(2)   =   "MTPbin.frx":0488
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "tmrGoIWG"
      Tab(2).Control(1)=   "Frame2"
      Tab(2).Control(2)=   "Frame(7)"
      Tab(2).Control(3)=   "tmrNDP"
      Tab(2).Control(4)=   "Command9"
      Tab(2).Control(5)=   "Command8"
      Tab(2).Control(6)=   "cmdCheckPointing"
      Tab(2).Control(7)=   "txtUTpoint(0)"
      Tab(2).Control(8)=   "txtUTpoint(1)"
      Tab(2).Control(9)=   "chkProcesses(6)"
      Tab(2).Control(10)=   "Frame(4)"
      Tab(2).Control(11)=   "Frame(5)"
      Tab(2).Control(12)=   "CommonDialog1"
      Tab(2).Control(13)=   "label(122)"
      Tab(2).Control(14)=   "label(123)"
      Tab(2).ControlCount=   15
      TabCaption(3)   =   "&Cts/Track"
      TabPicture(3)   =   "MTPbin.frx":04A4
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "label(142)"
      Tab(3).Control(1)=   "label(141)"
      Tab(3).Control(2)=   "label(140)"
      Tab(3).Control(3)=   "label(73)"
      Tab(3).Control(4)=   "label(74)"
      Tab(3).Control(5)=   "label(75)"
      Tab(3).Control(6)=   "label(76)"
      Tab(3).Control(7)=   "label(117)"
      Tab(3).Control(8)=   "label(124)"
      Tab(3).Control(9)=   "label(125)"
      Tab(3).Control(10)=   "label(126)"
      Tab(3).Control(11)=   "label(127)"
      Tab(3).Control(12)=   "label(128)"
      Tab(3).Control(13)=   "label(153)"
      Tab(3).Control(14)=   "Label1"
      Tab(3).Control(15)=   "Frame(2)"
      Tab(3).Control(16)=   "txtCounts1(0)"
      Tab(3).Control(17)=   "txtCounts2(0)"
      Tab(3).Control(18)=   "txtCounts2(1)"
      Tab(3).Control(19)=   "txtCounts2(2)"
      Tab(3).Control(20)=   "txtCounts2(3)"
      Tab(3).Control(21)=   "txtCounts2(4)"
      Tab(3).Control(22)=   "txtCounts2(5)"
      Tab(3).Control(23)=   "txtCounts2(6)"
      Tab(3).Control(24)=   "txtCounts2(7)"
      Tab(3).Control(25)=   "txtCounts2(8)"
      Tab(3).Control(26)=   "txtCounts2(9)"
      Tab(3).Control(27)=   "txtCounts2(10)"
      Tab(3).Control(28)=   "txtCounts2(11)"
      Tab(3).Control(29)=   "txtCounts1(1)"
      Tab(3).Control(30)=   "txtCounts1(2)"
      Tab(3).Control(31)=   "txtCounts1(3)"
      Tab(3).Control(32)=   "txtCounts1(4)"
      Tab(3).Control(33)=   "txtCounts1(5)"
      Tab(3).Control(34)=   "txtCounts1(6)"
      Tab(3).Control(35)=   "txtCounts1(7)"
      Tab(3).Control(36)=   "txtCounts1(8)"
      Tab(3).Control(37)=   "txtCounts1(9)"
      Tab(3).Control(38)=   "txtCounts1(10)"
      Tab(3).Control(39)=   "txtCounts1(11)"
      Tab(3).Control(40)=   "txtCounts3(0)"
      Tab(3).Control(41)=   "txtCounts3(1)"
      Tab(3).Control(42)=   "txtCounts3(2)"
      Tab(3).Control(43)=   "txtCounts3(3)"
      Tab(3).Control(44)=   "txtCounts3(4)"
      Tab(3).Control(45)=   "txtCounts3(5)"
      Tab(3).Control(46)=   "txtCounts3(6)"
      Tab(3).Control(47)=   "txtCounts3(7)"
      Tab(3).Control(48)=   "txtCounts3(8)"
      Tab(3).Control(49)=   "txtCounts3(9)"
      Tab(3).Control(50)=   "txtCounts3(10)"
      Tab(3).Control(51)=   "txtCounts3(11)"
      Tab(3).Control(52)=   "txtCB(0)"
      Tab(3).Control(53)=   "txtScanRMS(0)"
      Tab(3).Control(54)=   "txtScanRMS(1)"
      Tab(3).Control(55)=   "txtScanRMS(2)"
      Tab(3).Control(56)=   "txtCB(1)"
      Tab(3).Control(57)=   "txtCB(2)"
      Tab(3).Control(58)=   "txtCS(0)"
      Tab(3).Control(59)=   "txtCS(1)"
      Tab(3).Control(60)=   "txtCS(2)"
      Tab(3).Control(61)=   "txtCN(0)"
      Tab(3).Control(62)=   "txtCN(1)"
      Tab(3).Control(63)=   "txtCN(2)"
      Tab(3).Control(64)=   "Frame(3)"
      Tab(3).Control(65)=   "picTrack"
      Tab(3).Control(66)=   "cmdAssorted(1)"
      Tab(3).Control(67)=   "txtFTX1"
      Tab(3).Control(68)=   "txtFTX2"
      Tab(3).Control(69)=   "txtFTY1"
      Tab(3).Control(70)=   "txtFTY2"
      Tab(3).Control(71)=   "chkAutoscaleTrack"
      Tab(3).Control(72)=   "cboTrack"
      Tab(3).Control(73)=   "chkHiliteSelection"
      Tab(3).Control(74)=   "txtFTX3"
      Tab(3).Control(75)=   "txtFTY3"
      Tab(3).Control(76)=   "cmdCTS(8)"
      Tab(3).Control(77)=   "cmdCTS(9)"
      Tab(3).Control(78)=   "cmdCTS(10)"
      Tab(3).Control(79)=   "chkDisplayKelvin"
      Tab(3).Control(80)=   "cboLoc6"
      Tab(3).Control(81)=   "cboLoc5"
      Tab(3).Control(82)=   "chkFlagBad"
      Tab(3).Control(83)=   "txtCounts3(12)"
      Tab(3).Control(84)=   "txtCounts1(12)"
      Tab(3).Control(85)=   "txtCounts2(12)"
      Tab(3).Control(86)=   "cmdCTS(12)"
      Tab(3).Control(87)=   "txtTrms"
      Tab(3).Control(88)=   "chkAutoRemoveTrms"
      Tab(3).ControlCount=   89
      TabCaption(4)   =   "&CTS"
      TabPicture(4)   =   "MTPbin.frx":04C0
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "chkUseCQVS"
      Tab(4).Control(1)=   "chkPlotND"
      Tab(4).Control(2)=   "chkPlotBadScans"
      Tab(4).Control(3)=   "cmdCTS(11)"
      Tab(4).Control(4)=   "cmdCTS(7)"
      Tab(4).Control(5)=   "cmdCTS(5)"
      Tab(4).Control(6)=   "cmdCTS(4)"
      Tab(4).Control(7)=   "chkPlotBadEls"
      Tab(4).Control(8)=   "chkCH(2)"
      Tab(4).Control(9)=   "chkCH(1)"
      Tab(4).Control(10)=   "chkCH(0)"
      Tab(4).Control(11)=   "cmdCTS(6)"
      Tab(4).Control(12)=   "cmdCTS(3)"
      Tab(4).Control(13)=   "chkEdit"
      Tab(4).Control(14)=   "chkRAW"
      Tab(4).Control(15)=   "cmdCTS(2)"
      Tab(4).Control(16)=   "cboX3(0)"
      Tab(4).Control(17)=   "cboCTS3"
      Tab(4).Control(18)=   "cmdCTS(1)"
      Tab(4).Control(19)=   "cboUTunit(0)"
      Tab(4).Control(20)=   "txtX2(0)"
      Tab(4).Control(21)=   "txtX1(0)"
      Tab(4).Control(22)=   "cmdCTS(0)"
      Tab(4).Control(23)=   "chkCTS(25)"
      Tab(4).Control(24)=   "chkCTS(24)"
      Tab(4).Control(25)=   "chkCTS(23)"
      Tab(4).Control(26)=   "chkCTS(22)"
      Tab(4).Control(27)=   "chkCTS(21)"
      Tab(4).Control(28)=   "chkCTS(20)"
      Tab(4).Control(29)=   "chkCTS(19)"
      Tab(4).Control(30)=   "chkCTS(18)"
      Tab(4).Control(31)=   "chkCTS(17)"
      Tab(4).Control(32)=   "chkCTS(16)"
      Tab(4).Control(33)=   "chkCTS(15)"
      Tab(4).Control(34)=   "chkCTS(14)"
      Tab(4).Control(35)=   "chkCTS(13)"
      Tab(4).Control(36)=   "chkCTS(12)"
      Tab(4).Control(37)=   "chkCTS(11)"
      Tab(4).Control(38)=   "chkCTS(10)"
      Tab(4).Control(39)=   "chkCTS(9)"
      Tab(4).Control(40)=   "chkCTS(8)"
      Tab(4).Control(41)=   "chkCTS(7)"
      Tab(4).Control(42)=   "chkCTS(6)"
      Tab(4).Control(43)=   "chkCTS(5)"
      Tab(4).Control(44)=   "chkCTS(4)"
      Tab(4).Control(45)=   "chkCTS(3)"
      Tab(4).Control(46)=   "chkCTS(2)"
      Tab(4).Control(47)=   "chkCTS(1)"
      Tab(4).Control(48)=   "chkCTS(0)"
      Tab(4).Control(49)=   "cboCTS1"
      Tab(4).Control(50)=   "cboCTS2"
      Tab(4).Control(51)=   "picCTS"
      Tab(4).Control(52)=   "label(70)"
      Tab(4).Control(53)=   "label(69)"
      Tab(4).ControlCount=   54
      TabCaption(5)   =   "&MUX/NAV"
      TabPicture(5)   =   "MTPbin.frx":04DC
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "cmdMUX(7)"
      Tab(5).Control(1)=   "cmdMUX(5)"
      Tab(5).Control(2)=   "cmdMUX(4)"
      Tab(5).Control(3)=   "SSTab2"
      Tab(5).Control(4)=   "cmdMUX(6)"
      Tab(5).Control(5)=   "cboMUX3"
      Tab(5).Control(6)=   "cboMUX1"
      Tab(5).Control(7)=   "cboMUX2"
      Tab(5).Control(8)=   "cmdMUX(2)"
      Tab(5).Control(9)=   "cmdMUX(0)"
      Tab(5).Control(10)=   "txtX1(1)"
      Tab(5).Control(11)=   "txtX2(1)"
      Tab(5).Control(12)=   "cboUTunit(1)"
      Tab(5).Control(13)=   "cmdMUX(1)"
      Tab(5).Control(14)=   "cboX3(1)"
      Tab(5).Control(15)=   "cmdMUX(3)"
      Tab(5).Control(16)=   "picMUX"
      Tab(5).Control(17)=   "label(72)"
      Tab(5).Control(18)=   "label(71)"
      Tab(5).ControlCount=   19
      TabCaption(6)   =   "&TBs"
      TabPicture(6)   =   "MTPbin.frx":04F8
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "cboAlgorithm"
      Tab(6).Control(1)=   "chkDown"
      Tab(6).Control(2)=   "chkBiasMode"
      Tab(6).Control(3)=   "chkRemoveAAbias"
      Tab(6).Control(4)=   "chkOATmtpCOR"
      Tab(6).Control(5)=   "chkTotalError"
      Tab(6).Control(6)=   "txtOATmtpCor"
      Tab(6).Control(7)=   "chkShowFL"
      Tab(6).Control(8)=   "chkUseSmatrix"
      Tab(6).Control(9)=   "chkRemoveOBbias"
      Tab(6).Control(10)=   "chkOriginalMRI"
      Tab(6).Control(11)=   "chkShowOnlyGoodscans"
      Tab(6).Control(12)=   "Frame(13)"
      Tab(6).Control(13)=   "Frame(6)"
      Tab(6).Control(14)=   "Frame(9)"
      Tab(6).Control(15)=   "chkShowTBs"
      Tab(6).Control(16)=   "txtTA1(0)"
      Tab(6).Control(17)=   "txtTA1(1)"
      Tab(6).Control(18)=   "txtTA1(2)"
      Tab(6).Control(19)=   "txtTA1(3)"
      Tab(6).Control(20)=   "txtTA1(4)"
      Tab(6).Control(21)=   "txtTA2(0)"
      Tab(6).Control(22)=   "txtTA1(5)"
      Tab(6).Control(23)=   "txtTA1(6)"
      Tab(6).Control(24)=   "txtTA1(7)"
      Tab(6).Control(25)=   "txtTA1(8)"
      Tab(6).Control(26)=   "txtTA1(9)"
      Tab(6).Control(27)=   "txtTA2(1)"
      Tab(6).Control(28)=   "txtTA2(2)"
      Tab(6).Control(29)=   "txtTA2(3)"
      Tab(6).Control(30)=   "txtTA2(4)"
      Tab(6).Control(31)=   "txtTA2(5)"
      Tab(6).Control(32)=   "txtTA2(6)"
      Tab(6).Control(33)=   "txtTA2(7)"
      Tab(6).Control(34)=   "txtTA2(8)"
      Tab(6).Control(35)=   "txtTA2(9)"
      Tab(6).Control(36)=   "txtTA3(0)"
      Tab(6).Control(37)=   "txtTA3(1)"
      Tab(6).Control(38)=   "txtTA3(2)"
      Tab(6).Control(39)=   "txtTA3(3)"
      Tab(6).Control(40)=   "txtTA3(4)"
      Tab(6).Control(41)=   "txtTA3(5)"
      Tab(6).Control(42)=   "txtTA3(6)"
      Tab(6).Control(43)=   "txtTA3(7)"
      Tab(6).Control(44)=   "txtTA3(8)"
      Tab(6).Control(45)=   "txtTA3(9)"
      Tab(6).Control(46)=   "picTA"
      Tab(6).Control(47)=   "txtTC"
      Tab(6).Control(48)=   "txtT1"
      Tab(6).Control(49)=   "txtT2"
      Tab(6).Control(50)=   "cmdAutoScaleTA"
      Tab(6).Control(51)=   "chkShowArcAvg"
      Tab(6).Control(52)=   "label(194)"
      Tab(6).Control(53)=   "label(193)"
      Tab(6).Control(54)=   "label(192)"
      Tab(6).Control(55)=   "label(191)"
      Tab(6).Control(56)=   "label(190)"
      Tab(6).Control(57)=   "label(189)"
      Tab(6).Control(58)=   "label(188)"
      Tab(6).Control(59)=   "label(152)"
      Tab(6).Control(60)=   "label(150)"
      Tab(6).Control(61)=   "label(68)"
      Tab(6).Control(62)=   "label(134)"
      Tab(6).Control(63)=   "label(135)"
      Tab(6).Control(64)=   "label(136)"
      Tab(6).ControlCount=   65
      TabCaption(7)   =   "&Gain"
      TabPicture(7)   =   "MTPbin.frx":0514
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "chkYgrid"
      Tab(7).Control(1)=   "chkXgrid"
      Tab(7).Control(2)=   "txtGain(18)"
      Tab(7).Control(3)=   "txtGain(19)"
      Tab(7).Control(4)=   "txtGain(20)"
      Tab(7).Control(5)=   "chkGain(6)"
      Tab(7).Control(6)=   "chkGain(5)"
      Tab(7).Control(7)=   "cmdGAIN(5)"
      Tab(7).Control(8)=   "cmdGAIN(1)"
      Tab(7).Control(9)=   "cmdGAIN(6)"
      Tab(7).Control(10)=   "cboGAIN3"
      Tab(7).Control(11)=   "cboGAIN1"
      Tab(7).Control(12)=   "cboGAIN2"
      Tab(7).Control(13)=   "cboX3(3)"
      Tab(7).Control(14)=   "cboUTunit(3)"
      Tab(7).Control(15)=   "txtX2(3)"
      Tab(7).Control(16)=   "txtX1(3)"
      Tab(7).Control(17)=   "cmdGAIN(3)"
      Tab(7).Control(18)=   "Frame(10)"
      Tab(7).Control(19)=   "picGain"
      Tab(7).Control(20)=   "label(148)"
      Tab(7).Control(21)=   "label(115)"
      Tab(7).Control(22)=   "label(105)"
      Tab(7).Control(23)=   "label(104)"
      Tab(7).Control(24)=   "label(103)"
      Tab(7).Control(25)=   "label(102)"
      Tab(7).Control(26)=   "label(101)"
      Tab(7).Control(27)=   "label(100)"
      Tab(7).Control(28)=   "label(99)"
      Tab(7).Control(29)=   "label(98)"
      Tab(7).Control(30)=   "label(97)"
      Tab(7).Control(31)=   "label(96)"
      Tab(7).Control(32)=   "label(81)"
      Tab(7).Control(33)=   "label(80)"
      Tab(7).Control(34)=   "label(79)"
      Tab(7).Control(35)=   "label(78)"
      Tab(7).Control(36)=   "label(77)"
      Tab(7).ControlCount=   37
      TabCaption(8)   =   "TB Fit"
      TabPicture(8)   =   "MTPbin.frx":0530
      Tab(8).ControlEnabled=   0   'False
      Tab(8).Control(0)=   "label(21)"
      Tab(8).Control(1)=   "label(27)"
      Tab(8).Control(2)=   "label(28)"
      Tab(8).Control(3)=   "label(29)"
      Tab(8).Control(4)=   "label(30)"
      Tab(8).Control(5)=   "label(31)"
      Tab(8).Control(6)=   "label(34)"
      Tab(8).Control(7)=   "label(35)"
      Tab(8).Control(8)=   "label(39)"
      Tab(8).Control(9)=   "label(116)"
      Tab(8).Control(10)=   "label(119)"
      Tab(8).Control(11)=   "label(120)"
      Tab(8).Control(12)=   "label(121)"
      Tab(8).Control(13)=   "Line2"
      Tab(8).Control(14)=   "label(144)"
      Tab(8).Control(15)=   "cboTB3"
      Tab(8).Control(16)=   "cboTB1"
      Tab(8).Control(17)=   "cboTB2"
      Tab(8).Control(18)=   "txtX1(2)"
      Tab(8).Control(19)=   "txtX2(2)"
      Tab(8).Control(20)=   "cboUTunit(2)"
      Tab(8).Control(21)=   "cboX3(2)"
      Tab(8).Control(22)=   "picTB"
      Tab(8).Control(23)=   "cmdTB(3)"
      Tab(8).Control(24)=   "chkShowChannels(0)"
      Tab(8).Control(25)=   "txtDTb(0)"
      Tab(8).Control(26)=   "txtDTa(0)"
      Tab(8).Control(27)=   "txtDTc(0)"
      Tab(8).Control(28)=   "txtDTb(1)"
      Tab(8).Control(29)=   "txtDTa(1)"
      Tab(8).Control(30)=   "txtDTc(1)"
      Tab(8).Control(31)=   "txtDTb(2)"
      Tab(8).Control(32)=   "txtDTa(2)"
      Tab(8).Control(33)=   "txtDTc(2)"
      Tab(8).Control(34)=   "txtDTb(3)"
      Tab(8).Control(35)=   "txtDTa(3)"
      Tab(8).Control(36)=   "txtDTc(3)"
      Tab(8).Control(37)=   "chkShowChannels(1)"
      Tab(8).Control(38)=   "chkShowChannels(2)"
      Tab(8).Control(39)=   "chkShowChannels(3)"
      Tab(8).Control(40)=   "chkShowChannels(4)"
      Tab(8).Control(41)=   "cmdTB(6)"
      Tab(8).Control(42)=   "cmdTB(1)"
      Tab(8).Control(43)=   "cmdTB(5)"
      Tab(8).Control(44)=   "txtDTc(4)"
      Tab(8).Control(45)=   "txtDTa(4)"
      Tab(8).Control(46)=   "txtDTb(4)"
      Tab(8).Control(47)=   "txtDTc(5)"
      Tab(8).Control(48)=   "txtDTa(5)"
      Tab(8).Control(49)=   "txtDTb(5)"
      Tab(8).Control(50)=   "txtDTc(6)"
      Tab(8).Control(51)=   "txtDTa(6)"
      Tab(8).Control(52)=   "txtDTb(6)"
      Tab(8).Control(53)=   "chkShowChannels(5)"
      Tab(8).Control(54)=   "chkShowChannels(9)"
      Tab(8).Control(55)=   "cmdTB(0)"
      Tab(8).Control(56)=   "cmdTB(2)"
      Tab(8).Control(57)=   "cmdTB(4)"
      Tab(8).Control(58)=   "cmdTB(7)"
      Tab(8).Control(59)=   "cmdTB(8)"
      Tab(8).Control(60)=   "txtDTb(7)"
      Tab(8).Control(61)=   "txtDTa(7)"
      Tab(8).Control(62)=   "txtDTc(7)"
      Tab(8).Control(63)=   "chkShowChannels(6)"
      Tab(8).Control(64)=   "chkShowChannels(7)"
      Tab(8).Control(65)=   "chkShowChannels(8)"
      Tab(8).Control(66)=   "chkShowChannels(10)"
      Tab(8).Control(67)=   "chkShowChannels(12)"
      Tab(8).Control(68)=   "txtTtgt"
      Tab(8).Control(69)=   "cmdTB(9)"
      Tab(8).ControlCount=   70
      TabCaption(9)   =   "T/PT"
      TabPicture(9)   =   "MTPbin.frx":054C
      Tab(9).ControlEnabled=   0   'False
      Tab(9).Control(0)=   "cboTPT4"
      Tab(9).Control(1)=   "cboParms(3)"
      Tab(9).Control(2)=   "cmdTPT(5)"
      Tab(9).Control(3)=   "cboParms(2)"
      Tab(9).Control(4)=   "cboParms(1)"
      Tab(9).Control(5)=   "cboParms(0)"
      Tab(9).Control(6)=   "Frame(11)"
      Tab(9).Control(7)=   "cmdTPT(3)"
      Tab(9).Control(8)=   "picTPT"
      Tab(9).Control(9)=   "txtX1(4)"
      Tab(9).Control(10)=   "txtX2(4)"
      Tab(9).Control(11)=   "cboUTunit(4)"
      Tab(9).Control(12)=   "cboX3(4)"
      Tab(9).Control(13)=   "cmdTPT(1)"
      Tab(9).Control(14)=   "cboTPT3"
      Tab(9).Control(15)=   "cboTPT1"
      Tab(9).Control(16)=   "cboTPT2"
      Tab(9).Control(17)=   "cmdTPT(6)"
      Tab(9).Control(18)=   "label(114)"
      Tab(9).Control(19)=   "label(113)"
      Tab(9).Control(20)=   "label(112)"
      Tab(9).Control(21)=   "label(111)"
      Tab(9).Control(22)=   "label(110)"
      Tab(9).Control(23)=   "label(109)"
      Tab(9).Control(24)=   "label(108)"
      Tab(9).Control(25)=   "label(107)"
      Tab(9).Control(26)=   "label(106)"
      Tab(9).ControlCount=   27
      TabCaption(10)  =   "&Spare"
      TabPicture(10)  =   "MTPbin.frx":0568
      Tab(10).ControlEnabled=   0   'False
      Tab(10).ControlCount=   0
      Begin VB.ComboBox cboAlgorithm 
         Height          =   315
         Left            =   -69060
         TabIndex        =   931
         Top             =   3690
         Width           =   1890
      End
      Begin VB.CheckBox chkDown 
         Caption         =   "Show Down Stats"
         Height          =   240
         Left            =   -64785
         TabIndex        =   930
         Top             =   3735
         Width           =   1635
      End
      Begin VB.CheckBox chkBiasMode 
         Caption         =   "Use only FL Bias"
         Height          =   195
         Left            =   -72435
         TabIndex        =   928
         Top             =   3060
         Width           =   1905
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "&TTO-E"
         Height          =   345
         Index           =   9
         Left            =   -64410
         TabIndex        =   913
         Top             =   3630
         Width           =   885
      End
      Begin VB.Timer tmrGoIWG 
         Enabled         =   0   'False
         Interval        =   1000
         Left            =   -71550
         Top             =   2190
      End
      Begin VB.Frame Frame2 
         Caption         =   "IWG Control"
         Height          =   2265
         Left            =   -72660
         TabIndex        =   889
         Top             =   390
         Width           =   2625
         Begin VB.TextBox txtIWGrecord 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1740
            MultiLine       =   -1  'True
            TabIndex        =   901
            Text            =   "MTPbin.frx":0584
            Top             =   1350
            Width           =   735
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Open NG"
            Height          =   405
            Index           =   5
            Left            =   60
            TabIndex        =   899
            Top             =   1800
            Width           =   1035
         End
         Begin VB.ComboBox cboSpeed 
            Height          =   315
            Left            =   1740
            TabIndex        =   898
            Text            =   "1X"
            Top             =   1020
            Width           =   765
         End
         Begin VB.CheckBox chkTinK 
            Caption         =   "Ts in Kelvin"
            Height          =   285
            Left            =   90
            TabIndex        =   897
            Top             =   1020
            Value           =   1  'Checked
            Width           =   1185
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Continue"
            Height          =   405
            Index           =   4
            Left            =   840
            TabIndex        =   896
            Top             =   570
            Width           =   945
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Open IWG"
            Height          =   405
            Index           =   0
            Left            =   1530
            TabIndex        =   895
            Top             =   1800
            Width           =   1035
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Fwd"
            Height          =   405
            Index           =   1
            Left            =   1770
            TabIndex        =   894
            Top             =   570
            Width           =   735
         End
         Begin VB.ComboBox cboGoIWG 
            Height          =   315
            Left            =   930
            TabIndex        =   893
            Text            =   "seconds"
            Top             =   240
            Width           =   1065
         End
         Begin VB.TextBox txtGoIWG 
            Height          =   345
            Left            =   90
            TabIndex        =   892
            Text            =   "47000"
            Top             =   240
            Width           =   825
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Go"
            Height          =   405
            Index           =   2
            Left            =   1980
            TabIndex        =   891
            Top             =   180
            Width           =   525
         End
         Begin VB.CommandButton cmdTestIWG 
            Caption         =   "Back"
            Height          =   405
            Index           =   3
            Left            =   90
            TabIndex        =   890
            Top             =   570
            Width           =   735
         End
         Begin VB.Label label 
            Caption         =   "Record"
            Height          =   195
            Index           =   40
            Left            =   1110
            TabIndex        =   900
            Top             =   1350
            Width           =   645
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "IWG Parameters"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.6
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3615
         Index           =   7
         Left            =   -69990
         TabIndex        =   854
         Top             =   360
         Width           =   8985
         Begin VB.Label label 
            Caption         =   "34. Spare"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   187
            Left            =   5790
            TabIndex        =   888
            Top             =   3270
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "33. Spare"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   186
            Left            =   5790
            TabIndex        =   887
            Top             =   3000
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "32. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   185
            Left            =   5790
            TabIndex        =   886
            Top             =   2730
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "31. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   184
            Left            =   5790
            TabIndex        =   885
            Top             =   2460
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "30. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   183
            Left            =   5790
            TabIndex        =   884
            Top             =   2190
            Width           =   3165
         End
         Begin VB.Label label 
            Caption         =   "29. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   182
            Left            =   5790
            TabIndex        =   883
            Top             =   1920
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "28. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   181
            Left            =   5790
            TabIndex        =   882
            Top             =   1650
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "27. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   180
            Left            =   5790
            TabIndex        =   881
            Top             =   1380
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "26. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   179
            Left            =   5790
            TabIndex        =   880
            Top             =   1110
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "25. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   178
            Left            =   5790
            TabIndex        =   879
            Top             =   810
            Width           =   3135
         End
         Begin VB.Label label 
            Caption         =   "24. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   177
            Left            =   5790
            TabIndex        =   878
            Top             =   540
            Width           =   3135
         End
         Begin VB.Line Line4 
            X1              =   5760
            X2              =   5760
            Y1              =   510
            Y2              =   3540
         End
         Begin VB.Label label 
            Caption         =   "23. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   176
            Left            =   3030
            TabIndex        =   877
            Top             =   3270
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "22. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   285
            Index           =   175
            Left            =   3030
            TabIndex        =   876
            Top             =   3000
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "21. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   174
            Left            =   3030
            TabIndex        =   875
            Top             =   2730
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "20. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   173
            Left            =   3030
            TabIndex        =   874
            Top             =   2460
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "19. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   172
            Left            =   3000
            TabIndex        =   873
            Top             =   2190
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "18. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   171
            Left            =   3000
            TabIndex        =   872
            Top             =   1920
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "17. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   170
            Left            =   3000
            TabIndex        =   871
            Top             =   1650
            Width           =   2655
         End
         Begin VB.Label label 
            Caption         =   "16. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   169
            Left            =   3000
            TabIndex        =   870
            Top             =   1380
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "15. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   168
            Left            =   3000
            TabIndex        =   869
            Top             =   1080
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "14. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   167
            Left            =   3000
            TabIndex        =   868
            Top             =   810
            Width           =   2745
         End
         Begin VB.Label label 
            Caption         =   "13. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   166
            Left            =   3000
            TabIndex        =   867
            Top             =   510
            Width           =   2745
         End
         Begin VB.Line Line1 
            X1              =   2970
            X2              =   2970
            Y1              =   510
            Y2              =   3540
         End
         Begin VB.Label label 
            Caption         =   "12. Vertical Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   165
            Left            =   90
            TabIndex        =   866
            Top             =   3300
            Width           =   2775
         End
         Begin VB.Label label 
            Caption         =   "11. Mach Number"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   164
            Left            =   90
            TabIndex        =   865
            Top             =   3030
            Width           =   2805
         End
         Begin VB.Label label 
            Caption         =   "10. Indicated Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   163
            Left            =   90
            TabIndex        =   864
            Top             =   2760
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 9. True Air Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   162
            Left            =   90
            TabIndex        =   863
            Top             =   2490
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 8. Ground Speed"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   161
            Left            =   90
            TabIndex        =   862
            Top             =   2220
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 7. Radar Altitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   160
            Left            =   90
            TabIndex        =   861
            Top             =   1950
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 6. Pressure Altitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   159
            Left            =   90
            TabIndex        =   860
            Top             =   1680
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 5. GPS Altitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   158
            Left            =   90
            TabIndex        =   859
            Top             =   1410
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 4. Geometric Altitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   157
            Left            =   90
            TabIndex        =   858
            Top             =   1140
            Width           =   2835
         End
         Begin VB.Label label 
            Caption         =   " 3. Longitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   315
            Index           =   156
            Left            =   90
            TabIndex        =   857
            Top             =   840
            Width           =   2805
         End
         Begin VB.Label label 
            Caption         =   " 2. Latitude"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   195
            Index           =   155
            Left            =   90
            TabIndex        =   856
            Top             =   570
            Width           =   2805
         End
         Begin VB.Label label 
            Caption         =   " 1. Date and Time"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   10.8
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Index           =   154
            Left            =   30
            TabIndex        =   855
            Top             =   240
            Width           =   6405
         End
      End
      Begin VB.TextBox txtATP_RAOB2 
         Height          =   315
         Left            =   2370
         TabIndex        =   844
         Text            =   "C:\MTP\Data\NGV\HIPPO-2\RAOB\HIPPO-2_ATP.RAOB2"
         Top             =   3330
         Width           =   5265
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "Save ATP as RAOB2"
         Height          =   375
         Index           =   0
         Left            =   5100
         TabIndex        =   843
         Top             =   2970
         Width           =   2505
      End
      Begin VB.TextBox txtTtgt 
         Alignment       =   1  'Right Justify
         Height          =   345
         Left            =   -65850
         MultiLine       =   -1  'True
         TabIndex        =   841
         Text            =   "MTPbin.frx":0589
         Top             =   3600
         Width           =   345
      End
      Begin VB.CheckBox chkShowChannels 
         Caption         =   "Loc 6 is Target"
         Height          =   195
         Index           =   12
         Left            =   -67260
         TabIndex        =   840
         Top             =   3660
         Width           =   1425
      End
      Begin VB.CheckBox chkRemoveAAbias 
         Caption         =   "Remvoe AA Bias"
         Height          =   225
         Left            =   -66405
         TabIndex        =   839
         Top             =   3735
         Width           =   1575
      End
      Begin VB.CheckBox chkAutoRemoveTrms 
         Caption         =   "Auto Remove Trms"
         Height          =   270
         Left            =   -72600
         TabIndex        =   838
         ToolTipText     =   "Remove mesoscale T variations (derived from OAT) from total population RMS"
         Top             =   2895
         Value           =   1  'Checked
         Width           =   1815
      End
      Begin VB.TextBox txtTrms 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   836
         ToolTipText     =   "This is the mesocale temperature variation"
         Top             =   3180
         Width           =   510
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "Write Flt Date RMS"
         Height          =   435
         Index           =   12
         Left            =   -72600
         TabIndex        =   835
         ToolTipText     =   "Write RMS radiometer noise to Mission/RAOB/RMSerror/RMSyyyymmdd.txt file"
         Top             =   3465
         Width           =   1665
      End
      Begin VB.Timer tmrNDP 
         Enabled         =   0   'False
         Left            =   -73920
         Top             =   390
      End
      Begin VB.CommandButton Command9 
         Caption         =   "Test FTP"
         Height          =   375
         Left            =   -74880
         TabIndex        =   834
         Top             =   780
         Width           =   915
      End
      Begin VB.CommandButton Command8 
         Caption         =   "Edit GV"
         Height          =   375
         Left            =   -74880
         TabIndex        =   833
         Top             =   390
         Width           =   915
      End
      Begin MSWinsockLib.Winsock Winsock1 
         Left            =   2520
         Top             =   2880
         _ExtentX        =   593
         _ExtentY        =   593
         _Version        =   393216
      End
      Begin VB.CommandButton cmdREF 
         Caption         =   "Fix MP"
         Height          =   345
         Index           =   12
         Left            =   1710
         TabIndex        =   832
         Top             =   1530
         Width           =   1095
      End
      Begin VB.CheckBox chkOATmtpCOR 
         Caption         =   "OAT MTP Corr"
         Height          =   255
         Left            =   -72450
         TabIndex        =   831
         Top             =   3705
         Width           =   1365
      End
      Begin VB.CheckBox chkTotalError 
         Caption         =   "Use Total Error for MRI"
         Height          =   210
         Left            =   -72450
         TabIndex        =   830
         Top             =   1950
         Value           =   1  'Checked
         Width           =   2010
      End
      Begin VB.CommandButton cmdREF 
         Caption         =   "Send &UDP"
         Height          =   345
         Index           =   10
         Left            =   7695
         TabIndex        =   730
         Top             =   2970
         Width           =   1095
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "NGV Format"
         Height          =   330
         Index           =   7
         Left            =   7695
         TabIndex        =   729
         Top             =   3645
         Width           =   1095
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   727
         Top             =   3675
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   726
         Top             =   3675
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   725
         Top             =   3675
         Width           =   615
      End
      Begin VB.CheckBox chkYgrid 
         Caption         =   "Y Grid"
         Height          =   225
         Left            =   -72150
         TabIndex        =   723
         Top             =   2730
         Width           =   765
      End
      Begin VB.CheckBox chkXgrid 
         Caption         =   "X Grid"
         Height          =   195
         Left            =   -70530
         TabIndex        =   722
         Top             =   3450
         Width           =   885
      End
      Begin VB.TextBox txtGain 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74070
         MultiLine       =   -1  'True
         TabIndex        =   720
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtGain 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74070
         MultiLine       =   -1  'True
         TabIndex        =   719
         Top             =   3240
         Width           =   495
      End
      Begin VB.TextBox txtGain 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -74070
         MultiLine       =   -1  'True
         TabIndex        =   718
         Top             =   3540
         Width           =   495
      End
      Begin VB.CheckBox chkGain 
         Caption         =   "OAT"
         Height          =   255
         Index           =   6
         Left            =   -74850
         TabIndex        =   717
         Top             =   3270
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox chkGain 
         Caption         =   "Zp"
         Height          =   255
         Index           =   5
         Left            =   -74850
         TabIndex        =   716
         Top             =   3000
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.TextBox txtOATmtpCor 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71085
         MultiLine       =   -1  'True
         TabIndex        =   715
         Text            =   "MTPbin.frx":058C
         ToolTipText     =   "MTP OAT correction is applied to retrieved temperature profile"
         Top             =   3705
         Width           =   495
      End
      Begin VB.ComboBox cboAverageNcycles 
         Height          =   315
         Left            =   9600
         TabIndex        =   714
         Text            =   "1"
         Top             =   3510
         Width           =   495
      End
      Begin VB.CheckBox chkShowFL 
         Caption         =   "Show Flight Level"
         Height          =   195
         Left            =   -72450
         TabIndex        =   713
         Top             =   3510
         Value           =   1  'Checked
         Width           =   1935
      End
      Begin VB.CheckBox chkShowChannels 
         Caption         =   "Te Fit"
         Height          =   195
         Index           =   10
         Left            =   -61680
         TabIndex        =   712
         Top             =   3750
         Width           =   705
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "Zp"
         ForeColor       =   &H00C00000&
         Height          =   195
         Index           =   8
         Left            =   -61680
         TabIndex        =   711
         Top             =   3540
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "1Hz"
         ForeColor       =   &H0000C000&
         Height          =   195
         Index           =   7
         Left            =   -62640
         TabIndex        =   710
         Top             =   3540
         Width           =   615
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "Exp"
         ForeColor       =   &H0000FFFF&
         Height          =   195
         Index           =   6
         Left            =   -63420
         TabIndex        =   709
         Top             =   3540
         Width           =   615
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "3 CH TB"
         Height          =   405
         Index           =   6
         Left            =   3420
         TabIndex        =   708
         Top             =   2745
         Width           =   1560
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "RAW PT MM"
         Height          =   405
         Index           =   5
         Left            =   3420
         TabIndex        =   703
         Top             =   2310
         Width           =   1575
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "RAW, RPT,RMM"
         Height          =   405
         Index           =   3
         Left            =   3420
         TabIndex        =   702
         Top             =   1890
         Width           =   1575
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   701
         Top             =   2760
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   700
         Top             =   2760
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   699
         Top             =   2760
         Width           =   495
      End
      Begin VB.CommandButton cmdCheckPointing 
         Caption         =   "Check Pointing"
         Height          =   375
         Left            =   -74910
         TabIndex        =   697
         Top             =   1170
         Width           =   1335
      End
      Begin VB.TextBox txtUTpoint 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   694
         Text            =   "MTPbin.frx":0591
         Top             =   1860
         Width           =   555
      End
      Begin VB.TextBox txtUTpoint 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -73260
         MultiLine       =   -1  'True
         TabIndex        =   693
         Text            =   "MTPbin.frx":0596
         Top             =   2280
         Width           =   555
      End
      Begin VB.CommandButton cmdRTcatchup 
         Caption         =   "RT Catchup"
         Height          =   375
         Left            =   120
         TabIndex        =   692
         Top             =   1515
         Width           =   1545
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   5
         Left            =   -74640
         TabIndex        =   690
         Top             =   1680
         Width           =   13455
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "TTO"
         Height          =   345
         Index           =   8
         Left            =   -64410
         TabIndex        =   688
         ToolTipText     =   "Use Tmtp - Tnav to flag bad scans due to stepper motor sticking"
         Top             =   2550
         Width           =   885
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "Tm - Tn"
         Height          =   345
         Index           =   7
         Left            =   -64410
         TabIndex        =   678
         ToolTipText     =   "Use Tmtp - Tnav to flag bad scans due to stepper motor sticking"
         Top             =   1080
         Width           =   885
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "PT vs Z"
         Height          =   375
         Index           =   4
         Left            =   3420
         TabIndex        =   677
         Top             =   1470
         Width           =   1575
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "Test"
         Height          =   330
         Index           =   2
         Left            =   7695
         TabIndex        =   676
         Top             =   3330
         Width           =   1095
      End
      Begin VB.CheckBox chkUseCQVS 
         Caption         =   "Slow Cts Affect Sky Cts"
         Height          =   255
         Left            =   -63090
         TabIndex        =   674
         ToolTipText     =   "When using MA for SLOW parameters, do not flag CQV as BAD"
         Top             =   1890
         Value           =   1  'Checked
         Width           =   2055
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "&Get Fit"
         Height          =   375
         Index           =   4
         Left            =   -64410
         TabIndex        =   671
         ToolTipText     =   "Save Coordinates of Fit Region"
         Top             =   1440
         Width           =   885
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "&Save Fit"
         Height          =   345
         Index           =   2
         Left            =   -64410
         TabIndex        =   670
         ToolTipText     =   "Save Coordinates of Fit Region"
         Top             =   360
         Width           =   885
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "&Auto Fit"
         Height          =   375
         Index           =   0
         Left            =   -64410
         TabIndex        =   669
         ToolTipText     =   "Automatically perform TB fit to selected region to remove bias error"
         Top             =   2880
         Width           =   885
      End
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Get RAOBs within  range of"
         Height          =   255
         Index           =   6
         Left            =   -65370
         TabIndex        =   668
         Top             =   5400
         Width           =   2295
      End
      Begin VB.CheckBox chkUseSmatrix 
         Caption         =   "Apply S matrix"
         Height          =   255
         Left            =   -72450
         TabIndex        =   667
         Top             =   3225
         Width           =   1845
      End
      Begin VB.CheckBox chkRemoveOBbias 
         Caption         =   "Re-move/store OB bias"
         Height          =   225
         Left            =   -72450
         TabIndex        =   666
         Top             =   2835
         Width           =   2100
      End
      Begin VB.CheckBox chkOriginalMRI 
         Caption         =   "Use Original MRI"
         Height          =   225
         Left            =   -72450
         TabIndex        =   665
         Top             =   2625
         Width           =   1935
      End
      Begin VB.CheckBox chkShowChannels 
         Caption         =   "Ignore Bad Things"
         Height          =   195
         Index           =   9
         Left            =   -63420
         TabIndex        =   548
         Top             =   3750
         Width           =   1605
      End
      Begin VB.CommandButton cmdRemoveFECerr 
         Caption         =   "Remove fEc Errors"
         Height          =   375
         Left            =   3420
         TabIndex        =   546
         Top             =   1050
         Width           =   1575
      End
      Begin VB.CommandButton cmdStrip 
         Caption         =   "Strip non-A-E Lines      from RAW File"
         Height          =   615
         Left            =   3420
         TabIndex        =   545
         Top             =   390
         Width           =   1575
      End
      Begin VB.CheckBox chkFixTO 
         Caption         =   "Fix Takeoff and Landing"
         Height          =   195
         Left            =   150
         TabIndex        =   544
         Top             =   1920
         Width           =   2175
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "P+R"
         ForeColor       =   &H00000000&
         Height          =   195
         Index           =   5
         Left            =   -61680
         TabIndex        =   543
         Top             =   3120
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.Timer tmrRealTimeSim 
         Enabled         =   0   'False
         Interval        =   1000
         Left            =   2835
         Top             =   420
      End
      Begin VB.Timer tmrRealTime 
         Enabled         =   0   'False
         Left            =   2820
         Top             =   945
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   541
         Top             =   2460
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   540
         Top             =   2460
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   539
         Top             =   2460
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   538
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   537
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   536
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   532
         Top             =   1860
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   531
         Top             =   1860
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   530
         Top             =   1860
         Width           =   495
      End
      Begin VB.CheckBox chkPlotND 
         Caption         =   "Plot dND if no RFI"
         Height          =   255
         Left            =   -63090
         TabIndex        =   523
         Top             =   2640
         Width           =   1695
      End
      Begin VB.CheckBox chkPlotBadScans 
         Caption         =   "Plot Bad Scans"
         Height          =   195
         Left            =   -63090
         TabIndex        =   522
         Top             =   2160
         Value           =   1  'Checked
         Width           =   1815
      End
      Begin VB.CheckBox chkFlagBad 
         Caption         =   "Include Bad Scans"
         Height          =   210
         Left            =   -72600
         TabIndex        =   521
         Top             =   2430
         Width           =   1665
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Scale"
         Height          =   375
         Index           =   11
         Left            =   -74895
         TabIndex        =   520
         ToolTipText     =   "Scale Y-axis to Min-Max values"
         Top             =   2445
         Width           =   855
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Scale"
         Height          =   375
         Index           =   7
         Left            =   -74925
         TabIndex        =   519
         ToolTipText     =   "Scale Y-axis to Min-Max values"
         Top             =   2400
         Width           =   735
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Set Tab"
         Height          =   375
         Index           =   5
         Left            =   -63960
         TabIndex        =   518
         Top             =   3225
         Width           =   855
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Clear Tab"
         Height          =   375
         Index           =   4
         Left            =   -64800
         TabIndex        =   517
         Top             =   3225
         Width           =   855
      End
      Begin TabDlg.SSTab SSTab2 
         Height          =   2865
         Left            =   -64830
         TabIndex        =   468
         Top             =   360
         Width           =   3855
         _ExtentX        =   6816
         _ExtentY        =   5059
         _Version        =   393216
         Tabs            =   4
         Tab             =   1
         TabsPerRow      =   4
         TabHeight       =   520
         BackColor       =   12632256
         TabCaption(0)   =   "Mux"
         TabPicture(0)   =   "MTPbin.frx":059D
         Tab(0).ControlEnabled=   0   'False
         Tab(0).Control(0)=   "chkMux(21)"
         Tab(0).Control(1)=   "txtMux(21)"
         Tab(0).Control(2)=   "chkMux(20)"
         Tab(0).Control(3)=   "txtMux(20)"
         Tab(0).Control(4)=   "txtMux(24)"
         Tab(0).Control(5)=   "chkMux(24)"
         Tab(0).Control(6)=   "txtMux(23)"
         Tab(0).Control(7)=   "chkMux(23)"
         Tab(0).Control(8)=   "chkMux(22)"
         Tab(0).Control(9)=   "txtMux(22)"
         Tab(0).Control(10)=   "txtMux(19)"
         Tab(0).Control(11)=   "chkMux(19)"
         Tab(0).Control(12)=   "txtMux(18)"
         Tab(0).Control(13)=   "chkMux(18)"
         Tab(0).Control(14)=   "txtMux(17)"
         Tab(0).Control(15)=   "chkMux(17)"
         Tab(0).Control(16)=   "chkMux(11)"
         Tab(0).Control(17)=   "chkMux(12)"
         Tab(0).Control(18)=   "chkMux(13)"
         Tab(0).Control(19)=   "chkMux(14)"
         Tab(0).Control(20)=   "chkMux(15)"
         Tab(0).Control(21)=   "chkMux(16)"
         Tab(0).Control(22)=   "txtMux(16)"
         Tab(0).Control(23)=   "txtMux(15)"
         Tab(0).Control(24)=   "txtMux(14)"
         Tab(0).Control(25)=   "txtMux(13)"
         Tab(0).Control(26)=   "txtMux(12)"
         Tab(0).Control(27)=   "txtMux(11)"
         Tab(0).Control(28)=   "txtMux(0)"
         Tab(0).Control(29)=   "chkMux(2)"
         Tab(0).Control(30)=   "chkMux(3)"
         Tab(0).Control(31)=   "chkMux(5)"
         Tab(0).Control(32)=   "chkMux(6)"
         Tab(0).Control(33)=   "chkMux(8)"
         Tab(0).Control(34)=   "chkMux(9)"
         Tab(0).Control(35)=   "chkMux(10)"
         Tab(0).Control(36)=   "chkMux(7)"
         Tab(0).Control(37)=   "chkMux(4)"
         Tab(0).Control(38)=   "chkMux(1)"
         Tab(0).Control(39)=   "chkMux(0)"
         Tab(0).Control(40)=   "txtMux(10)"
         Tab(0).Control(41)=   "txtMux(9)"
         Tab(0).Control(42)=   "txtMux(8)"
         Tab(0).Control(43)=   "txtMux(7)"
         Tab(0).Control(44)=   "txtMux(6)"
         Tab(0).Control(45)=   "txtMux(5)"
         Tab(0).Control(46)=   "txtMux(4)"
         Tab(0).Control(47)=   "txtMux(3)"
         Tab(0).Control(48)=   "txtMux(2)"
         Tab(0).Control(49)=   "txtMux(1)"
         Tab(0).ControlCount=   50
         TabCaption(1)   =   "Nav"
         TabPicture(1)   =   "MTPbin.frx":05B9
         Tab(1).ControlEnabled=   -1  'True
         Tab(1).Control(0)=   "label(129)"
         Tab(1).Control(0).Enabled=   0   'False
         Tab(1).Control(1)=   "txtMux(31)"
         Tab(1).Control(1).Enabled=   0   'False
         Tab(1).Control(2)=   "chkMux(31)"
         Tab(1).Control(2).Enabled=   0   'False
         Tab(1).Control(3)=   "txtMux(30)"
         Tab(1).Control(3).Enabled=   0   'False
         Tab(1).Control(4)=   "chkMux(30)"
         Tab(1).Control(4).Enabled=   0   'False
         Tab(1).Control(5)=   "txtMux(29)"
         Tab(1).Control(5).Enabled=   0   'False
         Tab(1).Control(6)=   "chkMux(29)"
         Tab(1).Control(6).Enabled=   0   'False
         Tab(1).Control(7)=   "txtMux(28)"
         Tab(1).Control(7).Enabled=   0   'False
         Tab(1).Control(8)=   "chkMux(28)"
         Tab(1).Control(8).Enabled=   0   'False
         Tab(1).Control(9)=   "txtMux(25)"
         Tab(1).Control(9).Enabled=   0   'False
         Tab(1).Control(10)=   "chkMux(25)"
         Tab(1).Control(10).Enabled=   0   'False
         Tab(1).Control(11)=   "chkMux(26)"
         Tab(1).Control(11).Enabled=   0   'False
         Tab(1).Control(12)=   "txtMux(26)"
         Tab(1).Control(12).Enabled=   0   'False
         Tab(1).Control(13)=   "chkMux(27)"
         Tab(1).Control(13).Enabled=   0   'False
         Tab(1).Control(14)=   "txtMux(27)"
         Tab(1).Control(14).Enabled=   0   'False
         Tab(1).Control(15)=   "txtTnavRMS"
         Tab(1).Control(15).Enabled=   0   'False
         Tab(1).Control(16)=   "txtTnavAVG"
         Tab(1).Control(16).Enabled=   0   'False
         Tab(1).Control(17)=   "chkMux(32)"
         Tab(1).Control(17).Enabled=   0   'False
         Tab(1).Control(18)=   "txtMux(32)"
         Tab(1).Control(18).Enabled=   0   'False
         Tab(1).Control(19)=   "txtMux(33)"
         Tab(1).Control(19).Enabled=   0   'False
         Tab(1).Control(20)=   "txtMux(34)"
         Tab(1).Control(20).Enabled=   0   'False
         Tab(1).Control(21)=   "txtMux(35)"
         Tab(1).Control(21).Enabled=   0   'False
         Tab(1).Control(22)=   "chkMux(33)"
         Tab(1).Control(22).Enabled=   0   'False
         Tab(1).Control(23)=   "chkMux(34)"
         Tab(1).Control(23).Enabled=   0   'False
         Tab(1).Control(24)=   "chkMux(35)"
         Tab(1).Control(24).Enabled=   0   'False
         Tab(1).Control(25)=   "chkMux(38)"
         Tab(1).Control(25).Enabled=   0   'False
         Tab(1).Control(26)=   "txtMux(38)"
         Tab(1).Control(26).Enabled=   0   'False
         Tab(1).Control(27)=   "chkMux(37)"
         Tab(1).Control(27).Enabled=   0   'False
         Tab(1).Control(28)=   "txtMux(37)"
         Tab(1).Control(28).Enabled=   0   'False
         Tab(1).Control(29)=   "chkMux(36)"
         Tab(1).Control(29).Enabled=   0   'False
         Tab(1).Control(30)=   "txtMux(36)"
         Tab(1).Control(30).Enabled=   0   'False
         Tab(1).Control(31)=   "txtMux(40)"
         Tab(1).Control(31).Enabled=   0   'False
         Tab(1).Control(32)=   "chkMux(40)"
         Tab(1).Control(32).Enabled=   0   'False
         Tab(1).Control(33)=   "txtMux(39)"
         Tab(1).Control(33).Enabled=   0   'False
         Tab(1).Control(34)=   "chkMux(39)"
         Tab(1).Control(34).Enabled=   0   'False
         Tab(1).Control(35)=   "txtMux(46)"
         Tab(1).Control(35).Enabled=   0   'False
         Tab(1).Control(36)=   "chkMux(46)"
         Tab(1).Control(36).Enabled=   0   'False
         Tab(1).Control(37)=   "txtMux(45)"
         Tab(1).Control(37).Enabled=   0   'False
         Tab(1).Control(38)=   "chkMux(45)"
         Tab(1).Control(38).Enabled=   0   'False
         Tab(1).Control(39)=   "txtMux(44)"
         Tab(1).Control(39).Enabled=   0   'False
         Tab(1).Control(40)=   "chkMux(44)"
         Tab(1).Control(40).Enabled=   0   'False
         Tab(1).Control(41)=   "txtMux(43)"
         Tab(1).Control(41).Enabled=   0   'False
         Tab(1).Control(42)=   "chkMux(43)"
         Tab(1).Control(42).Enabled=   0   'False
         Tab(1).Control(43)=   "txtMux(42)"
         Tab(1).Control(43).Enabled=   0   'False
         Tab(1).Control(44)=   "chkMux(42)"
         Tab(1).Control(44).Enabled=   0   'False
         Tab(1).Control(45)=   "txtMux(41)"
         Tab(1).Control(45).Enabled=   0   'False
         Tab(1).Control(46)=   "chkMux(41)"
         Tab(1).Control(46).Enabled=   0   'False
         Tab(1).Control(47)=   "chkMux(47)"
         Tab(1).Control(47).Enabled=   0   'False
         Tab(1).Control(48)=   "txtMux(47)"
         Tab(1).Control(48).Enabled=   0   'False
         Tab(1).Control(49)=   "chkMux(48)"
         Tab(1).Control(49).Enabled=   0   'False
         Tab(1).Control(50)=   "txtMux(48)"
         Tab(1).Control(50).Enabled=   0   'False
         Tab(1).ControlCount=   51
         TabCaption(2)   =   "Misc"
         TabPicture(2)   =   "MTPbin.frx":05D5
         Tab(2).ControlEnabled=   0   'False
         Tab(2).Control(0)=   "txtMux(64)"
         Tab(2).Control(1)=   "chkMux(64)"
         Tab(2).Control(2)=   "txtMux(63)"
         Tab(2).Control(3)=   "chkMux(63)"
         Tab(2).Control(4)=   "txtMux(62)"
         Tab(2).Control(5)=   "chkMux(62)"
         Tab(2).Control(6)=   "txtMux(61)"
         Tab(2).Control(7)=   "chkMux(61)"
         Tab(2).Control(8)=   "txtMux(60)"
         Tab(2).Control(9)=   "chkMux(60)"
         Tab(2).Control(10)=   "txtMux(59)"
         Tab(2).Control(11)=   "chkMux(59)"
         Tab(2).Control(12)=   "txtMux(58)"
         Tab(2).Control(13)=   "chkMux(58)"
         Tab(2).Control(14)=   "txtMux(57)"
         Tab(2).Control(15)=   "chkMux(57)"
         Tab(2).Control(16)=   "txtMux(56)"
         Tab(2).Control(17)=   "chkMux(56)"
         Tab(2).Control(18)=   "txtMux(55)"
         Tab(2).Control(19)=   "chkMux(55)"
         Tab(2).Control(20)=   "txtMux(54)"
         Tab(2).Control(21)=   "chkMux(54)"
         Tab(2).Control(22)=   "txtMux(53)"
         Tab(2).Control(23)=   "chkMux(53)"
         Tab(2).Control(24)=   "txtMux(52)"
         Tab(2).Control(25)=   "chkMux(52)"
         Tab(2).Control(26)=   "txtMux(51)"
         Tab(2).Control(27)=   "chkMux(51)"
         Tab(2).Control(28)=   "txtMux(50)"
         Tab(2).Control(29)=   "chkMux(50)"
         Tab(2).Control(30)=   "txtMux(49)"
         Tab(2).Control(31)=   "chkMux(49)"
         Tab(2).ControlCount=   32
         TabCaption(3)   =   "Status"
         TabPicture(3)   =   "MTPbin.frx":05F1
         Tab(3).ControlEnabled=   0   'False
         Tab(3).Control(0)=   "txtMux(72)"
         Tab(3).Control(1)=   "chkMux(72)"
         Tab(3).Control(2)=   "txtMux(71)"
         Tab(3).Control(3)=   "chkMux(71)"
         Tab(3).Control(4)=   "chkMux(70)"
         Tab(3).Control(5)=   "txtMux(70)"
         Tab(3).Control(6)=   "chkMux(69)"
         Tab(3).Control(7)=   "txtMux(69)"
         Tab(3).Control(8)=   "chkMux(68)"
         Tab(3).Control(9)=   "txtMux(68)"
         Tab(3).Control(10)=   "chkMux(67)"
         Tab(3).Control(11)=   "txtMux(67)"
         Tab(3).Control(12)=   "chkMux(66)"
         Tab(3).Control(13)=   "txtMux(66)"
         Tab(3).Control(14)=   "chkMux(65)"
         Tab(3).Control(15)=   "txtMux(65)"
         Tab(3).ControlCount=   16
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   64
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   828
            Top             =   2115
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "PLO"
            Height          =   255
            Index           =   64
            Left            =   -73455
            TabIndex        =   827
            Top             =   2130
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   63
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   826
            Top             =   1860
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "EC"
            Height          =   255
            Index           =   63
            Left            =   -73455
            TabIndex        =   825
            Top             =   1875
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   62
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   824
            Top             =   1605
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP3"
            Height          =   255
            Index           =   62
            Left            =   -73455
            TabIndex        =   823
            Top             =   1620
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   61
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   822
            Top             =   1365
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP2"
            Height          =   255
            Index           =   61
            Left            =   -73455
            TabIndex        =   821
            Top             =   1380
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   60
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   820
            Top             =   1110
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP1"
            Height          =   255
            Index           =   60
            Left            =   -73455
            TabIndex        =   819
            Top             =   1125
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   59
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   818
            Top             =   855
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SU"
            Height          =   255
            Index           =   59
            Left            =   -73455
            TabIndex        =   817
            Top             =   870
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   58
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   816
            Top             =   600
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "DU"
            Height          =   255
            Index           =   58
            Left            =   -73455
            TabIndex        =   815
            Top             =   615
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   57
            Left            =   -72645
            MultiLine       =   -1  'True
            TabIndex        =   814
            Top             =   345
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Gs"
            Height          =   255
            Index           =   57
            Left            =   -73455
            TabIndex        =   813
            Top             =   360
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   48
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   812
            Top             =   1845
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SAA"
            Height          =   240
            Index           =   48
            Left            =   2655
            TabIndex        =   811
            Top             =   1875
            Width           =   675
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   47
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   810
            Top             =   1590
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SZA"
            Height          =   315
            Index           =   47
            Left            =   2655
            TabIndex        =   809
            Top             =   1560
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Mach"
            Height          =   255
            Index           =   41
            Left            =   1395
            TabIndex        =   808
            Top             =   1860
            Width           =   750
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   41
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   807
            Top             =   1860
            Width           =   465
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Vzac"
            Height          =   255
            Index           =   42
            Left            =   2640
            TabIndex        =   806
            Top             =   345
            Width           =   690
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   330
            Index           =   42
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   805
            Top             =   345
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Tdw"
            Height          =   255
            Index           =   43
            Left            =   2655
            TabIndex        =   804
            Top             =   585
            Width           =   690
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   330
            Index           =   43
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   803
            Top             =   585
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Ttot"
            Height          =   285
            Index           =   44
            Left            =   2655
            TabIndex        =   802
            Top             =   825
            Width           =   675
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   330
            Index           =   44
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   801
            Top             =   825
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Psta"
            Height          =   240
            Index           =   45
            Left            =   2655
            TabIndex        =   800
            Top             =   1110
            Width           =   675
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   330
            Index           =   45
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   799
            Top             =   1065
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Pdn"
            Height          =   210
            Index           =   46
            Left            =   2655
            TabIndex        =   798
            Top             =   1350
            Width           =   675
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   46
            Left            =   3315
            MultiLine       =   -1  'True
            TabIndex        =   797
            Top             =   1350
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   300
            Index           =   72
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   796
            Top             =   2145
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "PLO"
            Height          =   240
            Index           =   72
            Left            =   -74925
            TabIndex        =   795
            Top             =   2160
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   71
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   794
            Top             =   1905
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "EC"
            Height          =   255
            Index           =   71
            Left            =   -74925
            TabIndex        =   793
            Top             =   1905
            Width           =   615
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP3"
            Height          =   255
            Index           =   70
            Left            =   -74925
            TabIndex        =   792
            Top             =   1650
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   70
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   791
            Top             =   1650
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP2"
            Height          =   255
            Index           =   69
            Left            =   -74925
            TabIndex        =   790
            Top             =   1395
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   69
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   789
            Top             =   1395
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SP1"
            Height          =   255
            Index           =   68
            Left            =   -74925
            TabIndex        =   788
            Top             =   1140
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   68
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   787
            Top             =   1140
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "SU"
            Height          =   255
            Index           =   67
            Left            =   -74925
            TabIndex        =   786
            Top             =   885
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   67
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   785
            Top             =   885
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "DU"
            Height          =   255
            Index           =   66
            Left            =   -74925
            TabIndex        =   784
            Top             =   630
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   66
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   783
            Top             =   630
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Gs"
            Height          =   255
            Index           =   65
            Left            =   -74925
            TabIndex        =   782
            Top             =   375
            Width           =   615
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   65
            Left            =   -74325
            MultiLine       =   -1  'True
            TabIndex        =   781
            Top             =   375
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   56
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   780
            Top             =   2070
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   56
            Left            =   -74910
            TabIndex        =   779
            Top             =   2085
            Width           =   870
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   55
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   778
            Top             =   1815
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   55
            Left            =   -74910
            TabIndex        =   777
            Top             =   1830
            Width           =   870
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   54
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   776
            Top             =   1575
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   54
            Left            =   -74910
            TabIndex        =   775
            Top             =   1590
            Width           =   870
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   53
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   774
            Top             =   1350
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   53
            Left            =   -74910
            TabIndex        =   773
            Top             =   1350
            Width           =   885
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   52
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   772
            Top             =   1110
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   52
            Left            =   -74910
            TabIndex        =   771
            Top             =   1110
            Width           =   885
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   51
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   770
            Top             =   870
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   51
            Left            =   -74910
            TabIndex        =   769
            Top             =   870
            Width           =   885
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   50
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   768
            Top             =   615
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   240
            Index           =   50
            Left            =   -74910
            TabIndex        =   767
            Top             =   630
            Width           =   885
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   49
            Left            =   -74010
            MultiLine       =   -1  'True
            TabIndex        =   766
            Top             =   375
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   49
            Left            =   -74910
            TabIndex        =   765
            Top             =   375
            Width           =   885
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "TAS"
            Height          =   255
            Index           =   39
            Left            =   1395
            TabIndex        =   764
            Top             =   1365
            Width           =   750
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   39
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   763
            Top             =   1380
            Width           =   465
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Hdg"
            Height          =   255
            Index           =   40
            Left            =   1395
            TabIndex        =   762
            Top             =   1605
            Width           =   750
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   40
            Left            =   2160
            MultiLine       =   -1  'True
            TabIndex        =   761
            Top             =   1590
            Width           =   450
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   36
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   760
            Top             =   600
            Width           =   465
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Wdir"
            Height          =   255
            Index           =   36
            Left            =   1395
            TabIndex        =   759
            Top             =   600
            Width           =   750
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   37
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   758
            Top             =   855
            Width           =   465
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "VWS"
            Height          =   255
            Index           =   37
            Left            =   1395
            TabIndex        =   757
            Top             =   855
            Width           =   750
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   38
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   756
            Top             =   1110
            Width           =   465
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Gspd"
            Height          =   255
            Index           =   38
            Left            =   1395
            TabIndex        =   755
            Top             =   1110
            Width           =   750
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Wspd"
            Height          =   255
            Index           =   35
            Left            =   1395
            TabIndex        =   754
            Top             =   345
            Width           =   750
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Roll"
            Height          =   255
            Index           =   34
            Left            =   75
            TabIndex        =   753
            Top             =   2550
            Width           =   780
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Pitch"
            Height          =   255
            Index           =   33
            Left            =   75
            TabIndex        =   752
            Top             =   2295
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   35
            Left            =   2145
            MultiLine       =   -1  'True
            TabIndex        =   751
            Top             =   345
            Width           =   465
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   34
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   750
            Top             =   2550
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   33
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   749
            Top             =   2295
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   32
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   748
            Top             =   2040
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "rALT"
            Height          =   255
            Index           =   32
            Left            =   75
            TabIndex        =   747
            Top             =   2040
            Width           =   780
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   21
            Left            =   -72585
            TabIndex        =   746
            Top             =   1560
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   21
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   745
            Top             =   1545
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   20
            Left            =   -72585
            TabIndex        =   744
            ToolTipText     =   "Tnav MA"
            Top             =   1305
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   20
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   743
            Top             =   1305
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   24
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   742
            Top             =   2265
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   24
            Left            =   -72570
            TabIndex        =   741
            Top             =   2265
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   23
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   740
            Top             =   2025
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   23
            Left            =   -72585
            TabIndex        =   739
            Top             =   2025
            Width           =   735
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   22
            Left            =   -72585
            TabIndex        =   738
            Top             =   1785
            Value           =   1  'Checked
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   22
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   737
            Top             =   1785
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   19
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   736
            Top             =   1065
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   19
            Left            =   -72585
            TabIndex        =   735
            Top             =   1065
            Value           =   1  'Checked
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   18
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   734
            Top             =   825
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   18
            Left            =   -72555
            TabIndex        =   733
            Top             =   825
            Value           =   1  'Checked
            Width           =   735
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   -71865
            MultiLine       =   -1  'True
            TabIndex        =   732
            Top             =   585
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   17
            Left            =   -72585
            TabIndex        =   731
            Top             =   585
            Value           =   1  'Checked
            Width           =   735
         End
         Begin VB.TextBox txtTnavAVG 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2460
            MultiLine       =   -1  'True
            TabIndex        =   687
            Top             =   2490
            Width           =   495
         End
         Begin VB.TextBox txtTnavRMS 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   686
            Top             =   2490
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   27
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   516
            Top             =   825
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Tnav"
            Height          =   255
            Index           =   27
            Left            =   75
            TabIndex        =   515
            Top             =   825
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   26
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   514
            Top             =   585
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Zt1"
            Height          =   255
            Index           =   26
            Left            =   75
            TabIndex        =   513
            Top             =   585
            Value           =   1  'Checked
            Width           =   780
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "To"
            Height          =   255
            Index           =   25
            Left            =   75
            TabIndex        =   512
            ToolTipText     =   "TOMA"
            Top             =   345
            Value           =   1  'Checked
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   25
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   511
            Top             =   345
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Tmms"
            Height          =   255
            Index           =   28
            Left            =   75
            TabIndex        =   510
            Top             =   1080
            Value           =   1  'Checked
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   28
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   509
            Top             =   1080
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Tmtp"
            Height          =   255
            Index           =   29
            Left            =   75
            TabIndex        =   508
            Top             =   1320
            Value           =   1  'Checked
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   29
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   507
            Top             =   1320
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "pALT"
            Height          =   255
            Index           =   30
            Left            =   75
            TabIndex        =   506
            Top             =   1575
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   30
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   505
            Top             =   1575
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "gALT"
            Height          =   255
            Index           =   31
            Left            =   75
            TabIndex        =   504
            Top             =   1815
            Width           =   780
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   31
            Left            =   855
            MultiLine       =   -1  'True
            TabIndex        =   503
            Top             =   1815
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Vref"
            Height          =   255
            Index           =   11
            Left            =   -73710
            TabIndex        =   502
            Top             =   1065
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "DC1"
            Height          =   255
            Index           =   12
            Left            =   -73710
            TabIndex        =   501
            Top             =   1305
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "DC2"
            Height          =   255
            Index           =   13
            Left            =   -73710
            TabIndex        =   500
            Top             =   1545
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "5 V"
            Height          =   255
            Index           =   14
            Left            =   -73710
            TabIndex        =   499
            Top             =   1785
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "15V"
            Height          =   255
            Index           =   15
            Left            =   -73710
            TabIndex        =   498
            Top             =   2025
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Height          =   255
            Index           =   16
            Left            =   -73710
            TabIndex        =   497
            Top             =   2265
            Width           =   660
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   496
            Top             =   2265
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   495
            Top             =   2025
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   494
            Top             =   1785
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   493
            Top             =   1545
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   492
            Top             =   1305
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   491
            Top             =   1065
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   -74235
            MultiLine       =   -1  'True
            TabIndex        =   490
            Top             =   585
            Width           =   495
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "LO1"
            Height          =   255
            Index           =   2
            Left            =   -74880
            TabIndex        =   489
            Top             =   1065
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "LO2"
            Height          =   255
            Index           =   3
            Left            =   -74880
            TabIndex        =   488
            Top             =   1305
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Tlo"
            Height          =   255
            Index           =   5
            Left            =   -74880
            TabIndex        =   487
            Top             =   1785
            Value           =   1  'Checked
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Thi"
            Height          =   255
            Index           =   6
            Left            =   -74880
            TabIndex        =   486
            Top             =   2025
            Value           =   1  'Checked
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Win"
            Height          =   255
            Index           =   8
            Left            =   -74880
            TabIndex        =   485
            Top             =   2505
            Value           =   1  'Checked
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Mtr"
            Height          =   255
            Index           =   9
            Left            =   -73710
            TabIndex        =   484
            Top             =   585
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "28v"
            Height          =   255
            Index           =   10
            Left            =   -73710
            TabIndex        =   483
            Top             =   825
            Width           =   660
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "0.1g"
            Height          =   255
            Index           =   7
            Left            =   -74880
            TabIndex        =   482
            Top             =   2265
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "IFA"
            Height          =   255
            Index           =   4
            Left            =   -74880
            TabIndex        =   481
            Top             =   1545
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "ND"
            Height          =   255
            Index           =   1
            Left            =   -74880
            TabIndex        =   480
            Top             =   825
            Width           =   675
         End
         Begin VB.CheckBox chkMux 
            BackColor       =   &H00808080&
            Caption         =   "Ttgt"
            Height          =   255
            Index           =   0
            Left            =   -74880
            TabIndex        =   479
            Top             =   585
            Value           =   1  'Checked
            Width           =   675
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   478
            Top             =   825
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   -73050
            MultiLine       =   -1  'True
            TabIndex        =   477
            Top             =   585
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   476
            Top             =   2505
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   475
            Top             =   2265
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   474
            Top             =   2025
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   473
            Top             =   1785
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   472
            Top             =   1545
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   471
            Top             =   1305
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   470
            Top             =   1065
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   -74220
            MultiLine       =   -1  'True
            TabIndex        =   469
            Top             =   825
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "Tnav-To RMS"
            Height          =   195
            Index           =   129
            Left            =   2460
            TabIndex        =   685
            Top             =   2310
            Width           =   1020
         End
      End
      Begin VB.ComboBox cboLoc5 
         Height          =   315
         Left            =   -72600
         TabIndex        =   467
         Text            =   "5"
         Top             =   1500
         Width           =   615
      End
      Begin VB.ComboBox cboLoc6 
         Height          =   315
         Left            =   -71520
         TabIndex        =   465
         Text            =   "6"
         Top             =   1500
         Width           =   615
      End
      Begin VB.CheckBox chkDisplayKelvin 
         Caption         =   "Display Kelvin"
         Height          =   225
         Left            =   -72600
         TabIndex        =   464
         ToolTipText     =   "AVG and RMS in Kelvin based on ND"
         Top             =   2670
         Value           =   1  'Checked
         Width           =   1335
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Scan-to-Scan RMS"
         Height          =   375
         Index           =   10
         Left            =   -72600
         TabIndex        =   463
         Top             =   1035
         Width           =   1665
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Scan-to-Scan AVG"
         Height          =   375
         Index           =   9
         Left            =   -72600
         TabIndex        =   462
         Top             =   630
         Width           =   1665
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "        &Loc-to-Loc               AVG and RMS"
         Height          =   525
         Index           =   8
         Left            =   -72630
         TabIndex        =   461
         Top             =   1845
         Width           =   1665
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Scan-to-Scan RMS"
         Height          =   375
         Index           =   7
         Left            =   -63090
         TabIndex        =   460
         Top             =   960
         Width           =   2055
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Loc-to-Loc AVG and RMS"
         Height          =   375
         Index           =   5
         Left            =   -63090
         TabIndex        =   459
         Top             =   1440
         Width           =   2055
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Scan-to-Scan AVG"
         Height          =   375
         Index           =   4
         Left            =   -63090
         TabIndex        =   458
         Top             =   480
         Width           =   2055
      End
      Begin VB.CheckBox chkPlotBadEls 
         Caption         =   "Plot Bad Elevations"
         Height          =   255
         Left            =   -63090
         TabIndex        =   457
         Top             =   2400
         Width           =   1935
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "S&cale"
         Height          =   345
         Index           =   5
         Left            =   -64410
         TabIndex        =   455
         Top             =   720
         Width           =   885
      End
      Begin VB.CommandButton cmdGAIN 
         Caption         =   "&Scale"
         Height          =   375
         Index           =   5
         Left            =   -72120
         TabIndex        =   453
         Top             =   2280
         Width           =   735
      End
      Begin VB.ComboBox cboTPT4 
         Height          =   315
         Left            =   -74880
         TabIndex        =   451
         Text            =   "km"
         Top             =   1440
         Width           =   735
      End
      Begin VB.ComboBox cboParms 
         Height          =   315
         Index           =   3
         Left            =   -61740
         TabIndex        =   448
         Text            =   "10"
         Top             =   1560
         Width           =   735
      End
      Begin VB.CommandButton cmdTPT 
         Caption         =   "Calculate"
         Height          =   375
         Index           =   5
         Left            =   -62340
         TabIndex        =   447
         Top             =   2880
         Width           =   1335
      End
      Begin VB.ComboBox cboParms 
         Height          =   315
         Index           =   2
         Left            =   -61740
         TabIndex        =   446
         Text            =   "10"
         Top             =   840
         Width           =   735
      End
      Begin VB.ComboBox cboParms 
         Height          =   315
         Index           =   1
         Left            =   -61740
         TabIndex        =   445
         Text            =   "380"
         Top             =   1200
         Width           =   735
      End
      Begin VB.ComboBox cboParms 
         Height          =   315
         Index           =   0
         Left            =   -61740
         TabIndex        =   444
         Text            =   "300"
         Top             =   480
         Width           =   735
      End
      Begin VB.Frame Frame 
         Caption         =   "Plot Parameter"
         Height          =   855
         Index           =   11
         Left            =   -62340
         TabIndex        =   441
         Top             =   1920
         Width           =   1335
         Begin VB.OptionButton optParms 
            Caption         =   "Isentropes"
            Height          =   195
            Index           =   1
            Left            =   120
            TabIndex        =   443
            Top             =   480
            Value           =   -1  'True
            Width           =   1095
         End
         Begin VB.OptionButton optParms 
            Caption         =   "Isotherms"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   442
            Top             =   240
            Width           =   975
         End
      End
      Begin VB.CommandButton cmdTPT 
         Caption         =   "Update"
         Height          =   375
         Index           =   3
         Left            =   -61740
         TabIndex        =   437
         Top             =   3360
         Width           =   735
      End
      Begin VB.PictureBox picTPT 
         BackColor       =   &H00C0C0C0&
         Height          =   3015
         Left            =   -74160
         ScaleHeight     =   2964
         ScaleWidth      =   11664
         TabIndex        =   435
         Top             =   360
         Width           =   11715
         Begin VB.Shape shpTPT 
            Height          =   375
            Left            =   240
            Top             =   120
            Width           =   495
         End
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -74160
         MultiLine       =   -1  'True
         TabIndex        =   432
         Top             =   3360
         Width           =   735
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -63180
         MultiLine       =   -1  'True
         TabIndex        =   431
         Top             =   3360
         Width           =   735
      End
      Begin VB.ComboBox cboUTunit 
         Height          =   315
         Index           =   4
         Left            =   -69480
         TabIndex        =   430
         Top             =   3480
         Width           =   1095
      End
      Begin VB.ComboBox cboX3 
         Height          =   315
         Index           =   4
         Left            =   -67800
         TabIndex        =   429
         Text            =   "20"
         Top             =   3480
         Width           =   855
      End
      Begin VB.CommandButton cmdTPT 
         Caption         =   "&Reset X"
         Height          =   375
         Index           =   1
         Left            =   -66840
         TabIndex        =   428
         Top             =   3480
         Width           =   1215
      End
      Begin VB.ComboBox cboTPT3 
         Height          =   315
         Left            =   -74880
         TabIndex        =   427
         Text            =   "2"
         Top             =   2040
         Width           =   735
      End
      Begin VB.ComboBox cboTPT1 
         Height          =   315
         Left            =   -74880
         TabIndex        =   426
         Text            =   "0"
         Top             =   3060
         Width           =   735
      End
      Begin VB.ComboBox cboTPT2 
         Height          =   315
         Left            =   -74880
         TabIndex        =   425
         Text            =   "28"
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton cmdTPT 
         Caption         =   "&Reset Y"
         Height          =   375
         Index           =   6
         Left            =   -74880
         TabIndex        =   424
         Top             =   2280
         Width           =   735
      End
      Begin VB.CommandButton cmdGAIN 
         Caption         =   "Reset &X"
         Height          =   375
         Index           =   1
         Left            =   -65160
         TabIndex        =   423
         Top             =   3480
         Width           =   735
      End
      Begin VB.CommandButton cmdGAIN 
         Caption         =   "Reset &Y"
         Height          =   375
         Index           =   6
         Left            =   -72120
         TabIndex        =   422
         Top             =   1800
         Width           =   735
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "Reset &X"
         Height          =   345
         Index           =   1
         Left            =   -64410
         TabIndex        =   421
         Top             =   1830
         Width           =   885
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "Reset &Y"
         Height          =   345
         Index           =   6
         Left            =   -64410
         TabIndex        =   420
         Top             =   2190
         Width           =   885
      End
      Begin VB.ComboBox cboGAIN3 
         Height          =   315
         Left            =   -72120
         TabIndex        =   419
         Text            =   "1"
         Top             =   1440
         Width           =   735
      End
      Begin VB.ComboBox cboGAIN1 
         Height          =   315
         Left            =   -72120
         TabIndex        =   418
         Text            =   "5"
         Top             =   3000
         Width           =   735
      End
      Begin VB.ComboBox cboGAIN2 
         Height          =   315
         Left            =   -72120
         TabIndex        =   417
         Text            =   "15"
         Top             =   600
         Width           =   735
      End
      Begin VB.ComboBox cboX3 
         Height          =   315
         Index           =   3
         Left            =   -66120
         TabIndex        =   404
         Text            =   "20"
         Top             =   3480
         Width           =   855
      End
      Begin VB.ComboBox cboUTunit 
         Height          =   315
         Index           =   3
         Left            =   -67800
         TabIndex        =   403
         Top             =   3480
         Width           =   1095
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   402
         Top             =   3360
         Width           =   735
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -71400
         MultiLine       =   -1  'True
         TabIndex        =   401
         Top             =   3360
         Width           =   735
      End
      Begin VB.CheckBox chkShowOnlyGoodscans 
         Caption         =   "Show Only Good Scans"
         Height          =   195
         Left            =   -72450
         TabIndex        =   379
         Top             =   2415
         Value           =   1  'Checked
         Width           =   1995
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "Tnav"
         ForeColor       =   &H00000000&
         Height          =   255
         Index           =   4
         Left            =   -62640
         TabIndex        =   378
         Top             =   3090
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "CH3"
         ForeColor       =   &H00FF0000&
         Height          =   195
         Index           =   3
         Left            =   -61680
         TabIndex        =   377
         Top             =   3330
         Width           =   615
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "CH2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   2
         Left            =   -62640
         TabIndex        =   376
         Top             =   3300
         Width           =   615
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "CH1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   1
         Left            =   -63420
         TabIndex        =   375
         Top             =   3300
         Width           =   615
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   374
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   373
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   372
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   371
         Top             =   900
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   370
         Top             =   900
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   369
         Top             =   900
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   368
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   367
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   366
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtDTc 
         Alignment       =   1  'Right Justify
         BackColor       =   &H0080FF80&
         Height          =   285
         Index           =   0
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   365
         Top             =   1500
         Width           =   495
      End
      Begin VB.TextBox txtDTa 
         Alignment       =   1  'Right Justify
         BackColor       =   &H0080FF80&
         Height          =   285
         Index           =   0
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   364
         Top             =   1500
         Width           =   495
      End
      Begin VB.TextBox txtDTb 
         Alignment       =   1  'Right Justify
         BackColor       =   &H0080FF80&
         Height          =   285
         Index           =   0
         Left            =   -62280
         MultiLine       =   -1  'True
         TabIndex        =   363
         Top             =   1500
         Width           =   495
      End
      Begin VB.CheckBox chkShowChannels 
         BackColor       =   &H8000000A&
         Caption         =   "Tmtp"
         ForeColor       =   &H00FF00FF&
         Height          =   255
         Index           =   0
         Left            =   -63420
         TabIndex        =   357
         Top             =   3090
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CommandButton cmdGAIN 
         Caption         =   "&Update"
         Height          =   375
         Index           =   3
         Left            =   -61860
         TabIndex        =   356
         Top             =   3600
         Width           =   735
      End
      Begin VB.CommandButton cmdTB 
         Caption         =   "&Update"
         Height          =   345
         Index           =   3
         Left            =   -64410
         TabIndex        =   353
         Top             =   3270
         Width           =   885
      End
      Begin VB.PictureBox picTB 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00C0C0C0&
         Height          =   3195
         Left            =   -74265
         ScaleHeight     =   3144
         ScaleWidth      =   9780
         TabIndex        =   350
         Top             =   360
         Width           =   9825
         Begin VB.Shape shpTB 
            Height          =   375
            Left            =   180
            Top             =   180
            Width           =   675
         End
      End
      Begin VB.ComboBox cboX3 
         Height          =   315
         Index           =   2
         Left            =   -68400
         TabIndex        =   349
         Text            =   "20"
         Top             =   3630
         Width           =   855
      End
      Begin VB.ComboBox cboUTunit 
         Height          =   315
         Index           =   2
         Left            =   -70080
         TabIndex        =   348
         Top             =   3630
         Width           =   1095
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -65100
         MultiLine       =   -1  'True
         TabIndex        =   347
         Top             =   3540
         Width           =   645
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -74280
         MultiLine       =   -1  'True
         TabIndex        =   346
         Top             =   3540
         Width           =   645
      End
      Begin VB.ComboBox cboTB2 
         Height          =   315
         Left            =   -74880
         TabIndex        =   345
         Text            =   "220"
         Top             =   360
         Width           =   645
      End
      Begin VB.ComboBox cboTB1 
         Height          =   315
         Left            =   -74880
         TabIndex        =   344
         Text            =   "200"
         Top             =   3240
         Width           =   645
      End
      Begin VB.ComboBox cboTB3 
         Height          =   315
         Left            =   -74880
         TabIndex        =   343
         Text            =   "2"
         Top             =   1950
         Width           =   645
      End
      Begin VB.TextBox txtFTY3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -65280
         MultiLine       =   -1  'True
         TabIndex        =   338
         Text            =   "MTPbin.frx":060D
         Top             =   1920
         Width           =   255
      End
      Begin VB.TextBox txtFTX3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -62880
         MultiLine       =   -1  'True
         TabIndex        =   337
         Text            =   "MTPbin.frx":060F
         Top             =   3480
         Width           =   255
      End
      Begin VB.CheckBox chkHiliteSelection 
         Caption         =   "Hilite Selection"
         Height          =   255
         Left            =   -66720
         TabIndex        =   336
         Top             =   3480
         Value           =   1  'Checked
         Width           =   1455
      End
      Begin VB.ComboBox cboTrack 
         Height          =   315
         Left            =   -66720
         TabIndex        =   335
         Top             =   2880
         Width           =   1095
      End
      Begin VB.CheckBox chkAutoscaleTrack 
         Caption         =   "Autoscale"
         Height          =   255
         Left            =   -66720
         TabIndex        =   334
         Top             =   3240
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.TextBox txtFTY2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -65400
         MultiLine       =   -1  'True
         TabIndex        =   333
         Text            =   "MTPbin.frx":0611
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtFTY1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -65400
         MultiLine       =   -1  'True
         TabIndex        =   332
         Text            =   "MTPbin.frx":0613
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtFTX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -61680
         MultiLine       =   -1  'True
         TabIndex        =   331
         Text            =   "MTPbin.frx":0615
         Top             =   3360
         Width           =   495
      End
      Begin VB.TextBox txtFTX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -64920
         MultiLine       =   -1  'True
         TabIndex        =   330
         Text            =   "MTPbin.frx":0617
         Top             =   3360
         Width           =   495
      End
      Begin VB.CommandButton cmdAssorted 
         Caption         =   "Refresh"
         Height          =   375
         Index           =   1
         Left            =   -66735
         TabIndex        =   329
         Top             =   2400
         Width           =   855
      End
      Begin VB.PictureBox picTrack 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00FFFFFF&
         Height          =   2775
         Left            =   -64890
         ScaleHeight     =   2724
         ScaleWidth      =   3684
         TabIndex        =   327
         Top             =   585
         Width           =   3735
      End
      Begin VB.Frame Frame 
         Caption         =   "MP Write Options"
         Height          =   2505
         Index           =   1
         Left            =   5070
         TabIndex        =   324
         Top             =   405
         Width           =   3735
         Begin VB.ComboBox cboRevComment 
            Height          =   315
            Left            =   1530
            TabIndex        =   921
            Text            =   "Preliminary Data"
            Top             =   1755
            Width           =   2130
         End
         Begin VB.CheckBox chkWriteICT 
            Caption         =   "Write ICT format Rev. No."
            Height          =   240
            Left            =   135
            TabIndex        =   919
            Top             =   1440
            Value           =   1  'Checked
            Width           =   2265
         End
         Begin VB.ComboBox cboICTrev 
            Height          =   315
            Left            =   2385
            TabIndex        =   918
            Text            =   "0"
            Top             =   1395
            Width           =   645
         End
         Begin VB.CommandButton cmdMerge 
            Caption         =   "Merge MP Files"
            Height          =   285
            Left            =   120
            TabIndex        =   902
            Top             =   2160
            Width           =   3555
         End
         Begin VB.TextBox txtMPmri 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2385
            MultiLine       =   -1  'True
            TabIndex        =   853
            Text            =   "MTPbin.frx":0619
            Top             =   930
            Width           =   375
         End
         Begin VB.CheckBox chkMRIedit 
            Caption         =   "Discard scans with MRI > "
            Height          =   255
            Left            =   120
            TabIndex        =   852
            Top             =   960
            Width           =   2205
         End
         Begin VB.CommandButton cmdREF 
            Caption         =   "&Remove BL"
            Height          =   375
            Index           =   15
            Left            =   2385
            TabIndex        =   684
            ToolTipText     =   "Remove Blank Lines from all MP Files"
            Top             =   540
            Width           =   1245
         End
         Begin VB.CheckBox chkUTseconds 
            Caption         =   "UT(s)"
            Height          =   195
            Left            =   135
            TabIndex        =   549
            Top             =   1215
            Value           =   1  'Checked
            Width           =   735
         End
         Begin VB.CommandButton cmdREF 
            Caption         =   "&Write TS File"
            Height          =   375
            Index           =   4
            Left            =   2385
            TabIndex        =   547
            ToolTipText     =   "Write OAT time series only"
            Top             =   150
            Width           =   1245
         End
         Begin VB.TextBox txtDiscard 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   528
            Text            =   "MTPbin.frx":061D
            Top             =   720
            Width           =   375
         End
         Begin VB.CheckBox chkDiscard 
            Caption         =   "Discard Scans <"
            Height          =   255
            Left            =   120
            TabIndex        =   527
            Top             =   720
            Width           =   1575
         End
         Begin VB.CheckBox chkWriteBadHeaders 
            Caption         =   "Write Bad Headers"
            Height          =   255
            Left            =   120
            TabIndex        =   326
            Top             =   480
            Value           =   1  'Checked
            Width           =   1935
         End
         Begin VB.CheckBox chkGoodScansOnly 
            Caption         =   "Write only good scans"
            Height          =   255
            Left            =   120
            TabIndex        =   325
            Top             =   240
            Value           =   1  'Checked
            Width           =   1935
         End
         Begin VB.Label label 
            Caption         =   "Revision Comment"
            Height          =   195
            Index           =   195
            Left            =   135
            TabIndex        =   920
            Top             =   1845
            Width           =   1320
         End
         Begin VB.Label label 
            Caption         =   "km"
            Height          =   210
            Index           =   118
            Left            =   2115
            TabIndex        =   529
            Top             =   765
            Width           =   255
         End
      End
      Begin VB.CommandButton cmdREF 
         Caption         =   "E&xit"
         Height          =   375
         Index           =   8
         Left            =   10800
         TabIndex        =   322
         Top             =   4080
         Width           =   1095
      End
      Begin VB.CheckBox chkCH 
         Caption         =   "Ch3"
         Height          =   255
         Index           =   2
         Left            =   -61650
         TabIndex        =   313
         Top             =   3255
         Width           =   615
      End
      Begin VB.CheckBox chkCH 
         Caption         =   "Ch2"
         Height          =   255
         Index           =   1
         Left            =   -62370
         TabIndex        =   312
         Top             =   3255
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCH 
         Caption         =   "Ch1"
         Height          =   255
         Index           =   0
         Left            =   -63090
         TabIndex        =   311
         Top             =   3255
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Reset Y"
         Height          =   375
         Index           =   6
         Left            =   -74895
         TabIndex        =   310
         Top             =   2085
         Width           =   855
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Update"
         Height          =   375
         Index           =   3
         Left            =   -61890
         TabIndex        =   309
         Top             =   3585
         Width           =   855
      End
      Begin VB.CheckBox chkEdit 
         Caption         =   "MA"
         Height          =   255
         Left            =   -63735
         TabIndex        =   308
         Top             =   3705
         Value           =   1  'Checked
         Width           =   540
      End
      Begin VB.CheckBox chkRAW 
         Caption         =   "RAW"
         Height          =   255
         Left            =   -64440
         TabIndex        =   307
         Top             =   3705
         Value           =   1  'Checked
         Width           =   855
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Set"
         Height          =   375
         Index           =   2
         Left            =   -63090
         TabIndex        =   306
         Top             =   3585
         Width           =   615
      End
      Begin VB.ComboBox cboX3 
         Height          =   315
         Index           =   0
         Left            =   -69615
         TabIndex        =   305
         Text            =   "20"
         Top             =   3645
         Width           =   855
      End
      Begin VB.ComboBox cboCTS3 
         Height          =   315
         Left            =   -74895
         TabIndex        =   304
         Text            =   "500"
         Top             =   1725
         Width           =   855
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Reset X"
         Height          =   345
         Index           =   1
         Left            =   -68655
         TabIndex        =   303
         Top             =   3630
         Width           =   1215
      End
      Begin VB.ComboBox cboUTunit 
         Height          =   315
         Index           =   0
         Left            =   -71175
         TabIndex        =   302
         Top             =   3645
         Width           =   1095
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -65220
         MultiLine       =   -1  'True
         TabIndex        =   301
         Top             =   3660
         Width           =   735
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   300
         Top             =   3660
         Width           =   735
      End
      Begin VB.CommandButton cmdCTS 
         Caption         =   "&Clear"
         Height          =   375
         Index           =   0
         Left            =   -62490
         TabIndex        =   299
         Top             =   3585
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "ND"
         Height          =   255
         Index           =   25
         Left            =   -63840
         TabIndex        =   298
         Top             =   3240
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E12"
         Height          =   255
         Index           =   24
         Left            =   -63840
         TabIndex        =   297
         Top             =   3000
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E11"
         Height          =   255
         Index           =   23
         Left            =   -63840
         TabIndex        =   296
         Top             =   2760
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E10"
         Height          =   255
         Index           =   22
         Left            =   -63840
         TabIndex        =   295
         Top             =   2520
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E09"
         Height          =   255
         Index           =   21
         Left            =   -63840
         TabIndex        =   294
         Top             =   2280
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E08"
         Height          =   255
         Index           =   20
         Left            =   -63840
         TabIndex        =   293
         Top             =   2040
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E07"
         Height          =   255
         Index           =   19
         Left            =   -63840
         TabIndex        =   292
         Top             =   1800
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E06"
         Height          =   255
         Index           =   18
         Left            =   -63840
         TabIndex        =   291
         Top             =   1560
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E05"
         Height          =   255
         Index           =   17
         Left            =   -63840
         TabIndex        =   290
         Top             =   1320
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E04"
         Height          =   255
         Index           =   16
         Left            =   -63840
         TabIndex        =   289
         Top             =   1080
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E03"
         Height          =   255
         Index           =   15
         Left            =   -63840
         TabIndex        =   288
         Top             =   840
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E02"
         Height          =   255
         Index           =   14
         Left            =   -63840
         TabIndex        =   287
         Top             =   600
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "E01"
         Height          =   255
         Index           =   13
         Left            =   -63840
         TabIndex        =   286
         Top             =   360
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "ND"
         Height          =   255
         Index           =   12
         Left            =   -64440
         TabIndex        =   285
         Top             =   3240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L12"
         Height          =   255
         Index           =   11
         Left            =   -64440
         TabIndex        =   284
         Top             =   3000
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L11"
         Height          =   255
         Index           =   10
         Left            =   -64440
         TabIndex        =   283
         Top             =   2760
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L10"
         Height          =   255
         Index           =   9
         Left            =   -64440
         TabIndex        =   282
         Top             =   2520
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L09"
         Height          =   255
         Index           =   8
         Left            =   -64440
         TabIndex        =   281
         Top             =   2280
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L08"
         Height          =   255
         Index           =   7
         Left            =   -64440
         TabIndex        =   280
         Top             =   2040
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L07"
         Height          =   255
         Index           =   6
         Left            =   -64440
         TabIndex        =   279
         Top             =   1800
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L06"
         Height          =   255
         Index           =   5
         Left            =   -64440
         TabIndex        =   278
         Top             =   1560
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L05"
         Height          =   255
         Index           =   4
         Left            =   -64440
         TabIndex        =   277
         Top             =   1320
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L04"
         Height          =   255
         Index           =   3
         Left            =   -64440
         TabIndex        =   276
         Top             =   1080
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L03"
         Height          =   255
         Index           =   2
         Left            =   -64440
         TabIndex        =   275
         Top             =   840
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L02"
         Height          =   255
         Index           =   1
         Left            =   -64440
         TabIndex        =   274
         Top             =   600
         Width           =   615
      End
      Begin VB.CheckBox chkCTS 
         BackColor       =   &H00808080&
         Caption         =   "L01"
         Height          =   255
         Index           =   0
         Left            =   -64440
         TabIndex        =   273
         Top             =   360
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.ComboBox cboCTS1 
         Height          =   315
         Left            =   -74895
         TabIndex        =   272
         Top             =   3300
         Width           =   855
      End
      Begin VB.ComboBox cboCTS2 
         Height          =   315
         Left            =   -74895
         TabIndex        =   271
         Top             =   360
         Width           =   855
      End
      Begin VB.PictureBox picCTS 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00808080&
         Height          =   3255
         Left            =   -74040
         ScaleHeight     =   3204
         ScaleWidth      =   9504
         TabIndex        =   270
         Top             =   360
         Width           =   9555
         Begin VB.Shape shpCTS 
            Height          =   375
            Left            =   120
            Top             =   60
            Width           =   495
         End
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Reset Y"
         Height          =   375
         Index           =   6
         Left            =   -74925
         TabIndex        =   265
         Top             =   2040
         Width           =   735
      End
      Begin VB.ComboBox cboMUX3 
         Height          =   315
         Left            =   -74925
         TabIndex        =   264
         Text            =   "500"
         Top             =   1680
         Width           =   735
      End
      Begin VB.ComboBox cboMUX1 
         Height          =   315
         Left            =   -74925
         TabIndex        =   263
         Text            =   "-30"
         Top             =   3165
         Width           =   735
      End
      Begin VB.ComboBox cboMUX2 
         Height          =   315
         Left            =   -74925
         TabIndex        =   262
         Text            =   "60"
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Set All"
         Height          =   375
         Index           =   2
         Left            =   -63960
         TabIndex        =   261
         Top             =   3585
         Width           =   855
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Clear All"
         Height          =   375
         Index           =   0
         Left            =   -64800
         TabIndex        =   260
         Top             =   3585
         Width           =   855
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74160
         MultiLine       =   -1  'True
         TabIndex        =   259
         Top             =   3630
         Width           =   735
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -65550
         MultiLine       =   -1  'True
         TabIndex        =   258
         Top             =   3630
         Width           =   735
      End
      Begin VB.ComboBox cboUTunit 
         Height          =   315
         Index           =   1
         Left            =   -70920
         TabIndex        =   257
         Top             =   3630
         Width           =   1095
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Reset X"
         Height          =   375
         Index           =   1
         Left            =   -68280
         TabIndex        =   256
         Top             =   3630
         Width           =   1215
      End
      Begin VB.ComboBox cboX3 
         Height          =   315
         Index           =   1
         Left            =   -69360
         TabIndex        =   255
         Text            =   "20"
         Top             =   3630
         Width           =   855
      End
      Begin VB.CommandButton cmdMUX 
         Caption         =   "&Update"
         Height          =   735
         Index           =   3
         Left            =   -63090
         TabIndex        =   254
         Top             =   3225
         Width           =   2115
      End
      Begin VB.PictureBox picMUX 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00808080&
         Height          =   3240
         Left            =   -74175
         ScaleHeight     =   3192
         ScaleWidth      =   9312
         TabIndex        =   253
         Top             =   375
         Width           =   9360
         Begin VB.Shape shpMUX 
            Height          =   435
            Left            =   120
            Top             =   180
            Width           =   495
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Stats"
         Height          =   1575
         Index           =   13
         Left            =   -72450
         TabIndex        =   238
         Top             =   360
         Width           =   1950
         Begin VB.TextBox txtOBavg 
            Enabled         =   0   'False
            Height          =   285
            Index           =   1
            Left            =   900
            TabIndex        =   927
            ToolTipText     =   "OB Avg for RC Set #2"
            Top             =   1260
            Width           =   495
         End
         Begin VB.TextBox txtOBavg 
            Enabled         =   0   'False
            Height          =   285
            Index           =   0
            Left            =   900
            TabIndex        =   926
            ToolTipText     =   "OB Avg for RC Set #1"
            Top             =   945
            Width           =   495
         End
         Begin VB.TextBox txtMRI2 
            Alignment       =   1  'Right Justify
            BackColor       =   &H00C0C0FF&
            Height          =   285
            Left            =   1395
            MultiLine       =   -1  'True
            TabIndex        =   925
            ToolTipText     =   "MRI for RC Set #2"
            Top             =   1260
            Width           =   495
         End
         Begin VB.TextBox txtMRI1 
            Alignment       =   1  'Right Justify
            BackColor       =   &H00C0FFC0&
            Height          =   285
            Left            =   1380
            MultiLine       =   -1  'True
            TabIndex        =   634
            ToolTipText     =   "MRI for RC Set #1"
            Top             =   945
            Width           =   495
         End
         Begin VB.TextBox txtMRIavg 
            Enabled         =   0   'False
            Height          =   285
            Left            =   900
            TabIndex        =   242
            Top             =   630
            Width           =   495
         End
         Begin VB.TextBox txtMRIrms 
            Enabled         =   0   'False
            Height          =   285
            Left            =   1380
            TabIndex        =   241
            Top             =   630
            Width           =   495
         End
         Begin VB.TextBox txtDTrms 
            Enabled         =   0   'False
            Height          =   285
            Left            =   1380
            TabIndex        =   240
            Top             =   315
            Width           =   495
         End
         Begin VB.TextBox txtDTavg 
            Enabled         =   0   'False
            Height          =   285
            Left            =   900
            TabIndex        =   239
            Top             =   315
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "RCset #2"
            Height          =   210
            Index           =   197
            Left            =   90
            TabIndex        =   924
            Top             =   1260
            Width           =   945
         End
         Begin VB.Label label 
            Caption         =   "RCset #1"
            Height          =   210
            Index           =   196
            Left            =   90
            TabIndex        =   923
            Top             =   945
            Width           =   945
         End
         Begin VB.Label label 
            Caption         =   "MRI"
            Height          =   255
            Index           =   18
            Left            =   120
            TabIndex        =   245
            Top             =   630
            Width           =   375
         End
         Begin VB.Label label 
            Caption         =   "DT [s]"
            Height          =   255
            Index           =   17
            Left            =   120
            TabIndex        =   244
            Top             =   315
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "AVG     RMS"
            Height          =   255
            Index           =   19
            Left            =   900
            TabIndex        =   243
            Top             =   120
            Width           =   975
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Ret Avg and RMS"
         Height          =   3420
         Index           =   6
         Left            =   -66390
         TabIndex        =   237
         Top             =   330
         Width           =   1575
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   664
            Top             =   210
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   663
            Top             =   420
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   662
            Top             =   630
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   661
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   660
            Top             =   1050
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   659
            Top             =   1260
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   658
            Top             =   1470
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   657
            Top             =   1680
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   656
            Top             =   1890
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   655
            Top             =   2100
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   654
            Top             =   2310
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   653
            Top             =   2520
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   652
            Top             =   2730
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   651
            Top             =   2940
            Width           =   615
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   650
            Top             =   3150
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   649
            Top             =   2940
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   648
            Top             =   3150
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   647
            Top             =   210
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   646
            Top             =   420
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   645
            Top             =   630
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   644
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   643
            Top             =   1050
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   642
            Top             =   1260
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   641
            Top             =   1470
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   640
            Top             =   1680
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   639
            Top             =   1890
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   638
            Top             =   2100
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   637
            Top             =   2310
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   636
            Top             =   2520
            Width           =   615
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   635
            Top             =   2730
            Width           =   615
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Obs Avg, RMS and lnP"
         Height          =   3420
         Index           =   9
         Left            =   -64770
         TabIndex        =   230
         Top             =   330
         Width           =   3765
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   633
            Top             =   3120
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   632
            Top             =   3120
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   631
            Top             =   3120
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   630
            Top             =   2910
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   629
            Top             =   2910
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   628
            Top             =   2910
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   627
            Top             =   2700
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   626
            Top             =   2700
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   625
            Top             =   2700
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   624
            Top             =   2490
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   623
            Top             =   2490
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   622
            Top             =   2490
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   621
            Top             =   2280
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   620
            Top             =   2280
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   619
            Top             =   2280
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   618
            Top             =   2070
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   617
            Top             =   2070
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   616
            Top             =   2070
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   615
            Top             =   1860
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   614
            Top             =   1860
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   613
            Top             =   1860
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   612
            Top             =   1650
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   611
            Top             =   1650
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   610
            Top             =   1650
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   609
            Top             =   1440
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   608
            Top             =   1440
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   607
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   606
            Top             =   1230
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   605
            Top             =   1230
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   604
            Top             =   1230
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   603
            Top             =   1020
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   602
            Top             =   1020
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   601
            Top             =   1020
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   600
            Top             =   810
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   599
            Top             =   810
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   598
            Top             =   810
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   597
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   596
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   595
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   594
            Top             =   390
            Width           =   615
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   593
            Top             =   390
            Width           =   615
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   592
            Top             =   390
            Width           =   495
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   14
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   591
            Top             =   3120
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   14
            Left            =   2280
            TabIndex        =   590
            Top             =   3120
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   13
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   589
            Top             =   2910
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   13
            Left            =   2280
            TabIndex        =   588
            Top             =   2910
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   12
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   587
            Top             =   2700
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   12
            Left            =   2280
            TabIndex        =   586
            Top             =   2700
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   11
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   585
            Top             =   2490
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   11
            Left            =   2280
            TabIndex        =   584
            Top             =   2490
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   10
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   583
            Top             =   2280
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   10
            Left            =   2280
            TabIndex        =   582
            Top             =   2280
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   9
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   581
            Top             =   2070
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   9
            Left            =   2280
            TabIndex        =   580
            Top             =   2070
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   8
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   579
            Top             =   1860
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   8
            Left            =   2280
            TabIndex        =   578
            Top             =   1860
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   7
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   577
            Top             =   1650
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   7
            Left            =   2280
            TabIndex        =   576
            Top             =   1650
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   6
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   575
            Top             =   1440
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   6
            Left            =   2280
            TabIndex        =   574
            Top             =   1440
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   5
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   573
            Top             =   1230
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   5
            Left            =   2280
            TabIndex        =   572
            Top             =   1230
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   4
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   571
            Top             =   1020
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   4
            Left            =   2280
            TabIndex        =   570
            Top             =   1020
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   3
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   569
            Top             =   810
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   3
            Left            =   2280
            TabIndex        =   568
            Top             =   810
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   2
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   567
            Top             =   600
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   2
            Left            =   2280
            TabIndex        =   566
            Top             =   600
            Width           =   135
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   565
            Top             =   390
            Width           =   615
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   1
            Left            =   2280
            TabIndex        =   564
            Top             =   390
            Width           =   135
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   235
            Top             =   210
            Width           =   495
         End
         Begin VB.CheckBox chkRC 
            Caption         =   "Check1"
            Height          =   195
            Index           =   0
            Left            =   2280
            TabIndex        =   234
            Top             =   210
            Width           =   135
         End
         Begin VB.TextBox txtlnP 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   233
            Top             =   210
            Width           =   615
         End
         Begin VB.TextBox txtTBrms 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   232
            Top             =   210
            Width           =   615
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            BeginProperty Font 
               Name            =   "MS Serif"
               Size            =   6.6
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   231
            Top             =   210
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   14
            Left            =   3120
            TabIndex        =   563
            Top             =   3150
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   13
            Left            =   3120
            TabIndex        =   562
            Top             =   2940
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   12
            Left            =   3120
            TabIndex        =   561
            Top             =   2730
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   11
            Left            =   3120
            TabIndex        =   560
            Top             =   2520
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   10
            Left            =   3120
            TabIndex        =   559
            Top             =   2310
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   9
            Left            =   3120
            TabIndex        =   558
            Top             =   2100
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   8
            Left            =   3120
            TabIndex        =   557
            Top             =   1890
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   7
            Left            =   3120
            TabIndex        =   556
            Top             =   1680
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   6
            Left            =   3120
            TabIndex        =   555
            Top             =   1470
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   5
            Left            =   3120
            TabIndex        =   554
            Top             =   1260
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   4
            Left            =   3120
            TabIndex        =   553
            Top             =   1050
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   3
            Left            =   3120
            TabIndex        =   552
            Top             =   840
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   2
            Left            =   3120
            TabIndex        =   551
            Top             =   630
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   1
            Left            =   3120
            TabIndex        =   550
            Top             =   420
            Width           =   615
         End
         Begin VB.Label lblRCname 
            Caption         =   "RCNxx"
            BeginProperty Font 
               Name            =   "Courier New"
               Size            =   8.4
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   165
            Index           =   0
            Left            =   3120
            TabIndex        =   236
            Top             =   210
            Width           =   615
         End
      End
      Begin VB.CheckBox chkShowTBs 
         Caption         =   "Show Est TBs"
         Height          =   255
         Left            =   -74850
         TabIndex        =   229
         Top             =   3720
         Width           =   1335
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   228
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   227
         Top             =   1020
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   226
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   225
         Top             =   1620
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   224
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   223
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   222
         Top             =   2220
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   221
         Top             =   2520
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   220
         Top             =   2820
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   219
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -74850
         MultiLine       =   -1  'True
         TabIndex        =   218
         Top             =   3420
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   217
         Top             =   1020
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   216
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   215
         Top             =   1620
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   214
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   213
         Top             =   2220
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   212
         Top             =   2520
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   211
         Top             =   2820
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   210
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   209
         Top             =   3420
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   208
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   207
         Top             =   1020
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   206
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   205
         Top             =   1620
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   204
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   203
         Top             =   2220
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   202
         Top             =   2520
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   201
         Top             =   2820
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   200
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -73230
         MultiLine       =   -1  'True
         TabIndex        =   199
         Top             =   3420
         Width           =   615
      End
      Begin VB.PictureBox picTA 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00808080&
         Height          =   3240
         Left            =   -70170
         ScaleHeight     =   3192
         ScaleWidth      =   3384
         TabIndex        =   198
         Top             =   420
         Width           =   3435
      End
      Begin VB.TextBox txtTC 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   -69465
         MultiLine       =   -1  'True
         TabIndex        =   197
         Top             =   3690
         Width           =   375
      End
      Begin VB.TextBox txtT1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70170
         MultiLine       =   -1  'True
         TabIndex        =   196
         Top             =   3675
         Width           =   375
      End
      Begin VB.TextBox txtT2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -67110
         MultiLine       =   -1  'True
         TabIndex        =   195
         Top             =   3675
         Width           =   375
      End
      Begin VB.CommandButton cmdAutoScaleTA 
         Caption         =   "M"
         Height          =   315
         Left            =   -69780
         TabIndex        =   194
         ToolTipText     =   "Auto (A) or Manual (M) Scale"
         Top             =   3690
         Width           =   315
      End
      Begin VB.CheckBox chkShowArcAvg 
         Caption         =   "Show Archive Avg"
         Height          =   195
         Left            =   -72450
         TabIndex        =   193
         Top             =   2175
         Value           =   1  'Checked
         Width           =   1815
      End
      Begin VB.Frame Frame 
         Caption         =   "Gains"
         Height          =   2175
         Index           =   10
         Left            =   -74880
         TabIndex        =   164
         Top             =   480
         Width           =   2655
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   187
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   186
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   185
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            ForeColor       =   &H00008000&
            Height          =   285
            Index           =   3
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   184
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   183
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   182
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   181
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   180
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   179
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   178
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   177
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   176
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            ForeColor       =   &H000000FF&
            Height          =   285
            Index           =   12
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   175
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   174
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   173
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   172
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   171
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   170
            Top             =   1800
            Width           =   495
         End
         Begin VB.CheckBox chkGain 
            Caption         =   "Use"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   169
            Top             =   360
            Width           =   615
         End
         Begin VB.CheckBox chkGain 
            Caption         =   "Eqn"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   168
            Top             =   840
            Width           =   615
         End
         Begin VB.CheckBox chkGain 
            Caption         =   "ND"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   167
            Top             =   1080
            Width           =   615
         End
         Begin VB.CheckBox chkGain 
            Caption         =   "MMS"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   166
            Top             =   1320
            Width           =   735
         End
         Begin VB.CheckBox chkGain 
            Caption         =   "NAV"
            ForeColor       =   &H00000000&
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   165
            Top             =   1560
            Width           =   735
         End
         Begin VB.Label label 
            Caption         =   "Ch 1"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   139
            Left            =   840
            TabIndex        =   190
            Top             =   120
            Width           =   375
         End
         Begin VB.Label label 
            Caption         =   "Ch 2"
            ForeColor       =   &H00FFFFFF&
            Height          =   255
            Index           =   138
            Left            =   1560
            TabIndex        =   189
            Top             =   120
            Width           =   375
         End
         Begin VB.Label label 
            Caption         =   "Ch 3"
            ForeColor       =   &H00FF0000&
            Height          =   255
            Index           =   137
            Left            =   2160
            TabIndex        =   188
            Top             =   120
            Width           =   375
         End
      End
      Begin VB.PictureBox picGain 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00C0C0C0&
         Height          =   2775
         Left            =   -71400
         ScaleHeight     =   2724
         ScaleWidth      =   9204
         TabIndex        =   163
         Top             =   600
         Width           =   9255
         Begin VB.Shape shpGAIN 
            Height          =   435
            Left            =   240
            Top             =   240
            Width           =   435
         End
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   4
         Left            =   -74640
         TabIndex        =   155
         Top             =   2400
         Width           =   13455
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   3
         Left            =   -74640
         TabIndex        =   154
         Top             =   2040
         Width           =   13455
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   2
         Left            =   -74640
         TabIndex        =   153
         Top             =   1320
         Width           =   13455
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   1
         Left            =   -74640
         TabIndex        =   152
         Top             =   960
         Width           =   13455
      End
      Begin VB.TextBox txtLine 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   0
         Left            =   -74640
         TabIndex        =   151
         Top             =   600
         Width           =   13455
      End
      Begin VB.Frame Frame 
         Caption         =   "Moving Avg Ints"
         Height          =   2295
         Index           =   3
         Left            =   -68400
         TabIndex        =   134
         Top             =   480
         Width           =   1575
         Begin VB.TextBox txtCBMAint 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   142
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtCSMAint 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   141
            Top             =   480
            Width           =   495
         End
         Begin VB.TextBox txtCNMAint 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   140
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtCRMAint 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   139
            Top             =   960
            Width           =   495
         End
         Begin VB.TextBox txtTtgtMA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   138
            Top             =   1200
            Width           =   495
         End
         Begin VB.TextBox txtTifaMA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   137
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtTsp1MA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   136
            Top             =   1680
            Width           =   495
         End
         Begin VB.TextBox txtTsp2MA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   135
            Top             =   1920
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "Base Cts"
            Height          =   255
            Index           =   88
            Left            =   120
            TabIndex        =   150
            Top             =   240
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "Sky Cts"
            Height          =   255
            Index           =   89
            Left            =   120
            TabIndex        =   149
            Top             =   480
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "ND Cts"
            Height          =   255
            Index           =   90
            Left            =   120
            TabIndex        =   148
            Top             =   720
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "RMS Cts"
            Height          =   255
            Index           =   91
            Left            =   120
            TabIndex        =   147
            Top             =   960
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "T target"
            Height          =   255
            Index           =   92
            Left            =   120
            TabIndex        =   146
            Top             =   1200
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "T IF Amp"
            Height          =   255
            Index           =   93
            Left            =   120
            TabIndex        =   145
            Top             =   1440
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "T spare"
            Height          =   255
            Index           =   94
            Left            =   120
            TabIndex        =   144
            Top             =   1680
            Width           =   855
         End
         Begin VB.Label label 
            Caption         =   "T spare"
            Height          =   255
            Index           =   95
            Left            =   120
            TabIndex        =   143
            Top             =   1920
            Width           =   855
         End
      End
      Begin VB.TextBox txtCN 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -69090
         MultiLine       =   -1  'True
         TabIndex        =   116
         Top             =   1320
         Width           =   495
      End
      Begin VB.TextBox txtCN 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69090
         MultiLine       =   -1  'True
         TabIndex        =   115
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtCN 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69090
         MultiLine       =   -1  'True
         TabIndex        =   114
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtCS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -69570
         MultiLine       =   -1  'True
         TabIndex        =   113
         Top             =   1320
         Width           =   495
      End
      Begin VB.TextBox txtCS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69570
         MultiLine       =   -1  'True
         TabIndex        =   112
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtCS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69570
         MultiLine       =   -1  'True
         TabIndex        =   111
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtCB 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -70050
         MultiLine       =   -1  'True
         TabIndex        =   110
         Top             =   1320
         Width           =   495
      End
      Begin VB.TextBox txtCB 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70050
         MultiLine       =   -1  'True
         TabIndex        =   109
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtScanRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -70530
         MultiLine       =   -1  'True
         TabIndex        =   108
         Top             =   1320
         Width           =   495
      End
      Begin VB.TextBox txtScanRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70530
         MultiLine       =   -1  'True
         TabIndex        =   107
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtScanRMS 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70530
         MultiLine       =   -1  'True
         TabIndex        =   106
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtCB 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70050
         MultiLine       =   -1  'True
         TabIndex        =   105
         Top             =   600
         Width           =   495
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   104
         Top             =   3420
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   103
         Top             =   3180
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   102
         Top             =   2925
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   101
         Top             =   2670
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   100
         Top             =   2415
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   99
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   98
         Top             =   1905
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   97
         Top             =   1650
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   96
         Top             =   1395
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   95
         Top             =   1140
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -73290
         MultiLine       =   -1  'True
         TabIndex        =   94
         Top             =   885
         Width           =   615
      End
      Begin VB.TextBox txtCounts3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -73305
         MultiLine       =   -1  'True
         TabIndex        =   93
         Top             =   630
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   92
         Top             =   3420
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   91
         Top             =   3180
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   90
         Top             =   2925
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   89
         Top             =   2670
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   88
         Top             =   2415
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   87
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   86
         Top             =   1905
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   85
         Top             =   1650
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   84
         Top             =   1395
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   83
         Top             =   1140
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   82
         Top             =   885
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   81
         Top             =   3420
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   80
         Top             =   3180
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   79
         Top             =   2925
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   78
         Top             =   2670
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   77
         Top             =   2415
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   76
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   75
         Top             =   1905
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   74
         Top             =   1650
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   73
         Top             =   1395
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   72
         Top             =   1140
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   71
         Top             =   885
         Width           =   615
      End
      Begin VB.TextBox txtCounts2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -73980
         MultiLine       =   -1  'True
         TabIndex        =   70
         Top             =   630
         Width           =   615
      End
      Begin VB.TextBox txtCounts1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74670
         MultiLine       =   -1  'True
         TabIndex        =   69
         Top             =   615
         Width           =   615
      End
      Begin VB.Frame Frame 
         Caption         =   "UT"
         Height          =   975
         Index           =   4
         Left            =   -74880
         TabIndex        =   66
         Top             =   1620
         Width           =   975
         Begin VB.TextBox txtUT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   68
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox txtUT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   67
            Top             =   600
            Width           =   735
         End
      End
      Begin VB.TextBox txtREFfile 
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.4
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   960
         TabIndex        =   65
         Top             =   3705
         Width           =   6675
      End
      Begin VB.Frame Frame 
         Height          =   1215
         Index           =   2
         Left            =   -70860
         TabIndex        =   51
         Top             =   1560
         Width           =   2415
         Begin VB.TextBox txtCRmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   57
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtCRmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   56
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtCRmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   55
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtCBmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   54
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtCSmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   53
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtCNmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   52
            Top             =   360
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "3"
            Height          =   255
            Index           =   87
            Left            =   120
            TabIndex        =   64
            Top             =   840
            Width           =   135
         End
         Begin VB.Label label 
            Caption         =   "2"
            Height          =   255
            Index           =   86
            Left            =   120
            TabIndex        =   63
            Top             =   600
            Width           =   135
         End
         Begin VB.Label label 
            Caption         =   "1"
            Height          =   255
            Index           =   85
            Left            =   120
            TabIndex        =   62
            Top             =   360
            Width           =   135
         End
         Begin VB.Label label 
            Caption         =   "Ch   RMS"
            Height          =   255
            Index           =   132
            Left            =   120
            TabIndex        =   61
            Top             =   120
            Width           =   735
         End
         Begin VB.Label label 
            Caption         =   "Base"
            Height          =   255
            Index           =   82
            Left            =   840
            TabIndex        =   60
            Top             =   120
            Width           =   375
         End
         Begin VB.Label label 
            Caption         =   "Sky"
            Height          =   255
            Index           =   83
            Left            =   1320
            TabIndex        =   59
            Top             =   120
            Width           =   375
         End
         Begin VB.Label label 
            Caption         =   "ND"
            Height          =   255
            Index           =   84
            Left            =   1920
            TabIndex        =   58
            Top             =   120
            Width           =   375
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "ND Averaging"
         Height          =   1335
         Index           =   5
         Left            =   -74940
         TabIndex        =   36
         Top             =   2610
         Width           =   4905
         Begin VB.CommandButton cmdNDP 
            Caption         =   "Stop"
            Height          =   375
            Index           =   1
            Left            =   4200
            TabIndex        =   45
            Top             =   840
            Width           =   615
         End
         Begin VB.CommandButton cmdNDP 
            Caption         =   "Start"
            Height          =   375
            Index           =   0
            Left            =   4200
            TabIndex        =   44
            Top             =   480
            Width           =   615
         End
         Begin VB.TextBox txtPassword 
            Height          =   285
            Left            =   3090
            TabIndex        =   43
            Text            =   "mtp@mtp"
            Top             =   960
            Width           =   1095
         End
         Begin VB.TextBox txtUsername 
            Height          =   285
            Left            =   930
            TabIndex        =   42
            Text            =   "anonymous"
            Top             =   960
            Width           =   1215
         End
         Begin VB.TextBox txtURL 
            Height          =   285
            Left            =   930
            TabIndex        =   41
            Text            =   "ftp://192.108.217.205/"
            Top             =   600
            Width           =   3255
         End
         Begin VB.CheckBox chkFTPtoDIAL 
            Caption         =   "FTP to"
            Height          =   255
            Left            =   120
            TabIndex        =   40
            Top             =   600
            Width           =   855
         End
         Begin VB.TextBox txtFTPinterval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3810
            MultiLine       =   -1  'True
            TabIndex        =   39
            Text            =   "MTPbin.frx":0620
            Top             =   240
            Width           =   375
         End
         Begin VB.TextBox txtNDavgCount 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2220
            MultiLine       =   -1  'True
            TabIndex        =   38
            Text            =   "MTPbin.frx":0624
            Top             =   240
            Width           =   375
         End
         Begin VB.CheckBox chkWriteND 
            Caption         =   "Write ND"
            Height          =   255
            Left            =   120
            TabIndex        =   37
            Top             =   240
            Width           =   975
         End
         Begin InetCtlsObjects.Inet Inet1 
            Left            =   1800
            Top             =   840
            _ExtentX        =   995
            _ExtentY        =   995
            _Version        =   393216
         End
         Begin VB.Label label 
            Caption         =   "Timer"
            Height          =   255
            Index           =   9
            Left            =   4320
            TabIndex        =   50
            Top             =   240
            Width           =   495
         End
         Begin VB.Label label 
            Caption         =   "Password"
            Height          =   255
            Index           =   59
            Left            =   2370
            TabIndex        =   49
            Top             =   960
            Width           =   735
         End
         Begin VB.Label label 
            Caption         =   "Username"
            Height          =   255
            Index           =   60
            Left            =   120
            TabIndex        =   48
            Top             =   960
            Width           =   735
         End
         Begin VB.Label label 
            Caption         =   "Frequency [min]"
            Height          =   255
            Index           =   131
            Left            =   2670
            TabIndex        =   47
            Top             =   240
            Width           =   1335
         End
         Begin VB.Label label 
            Caption         =   "Average Count"
            Height          =   255
            Index           =   61
            Left            =   1140
            TabIndex        =   46
            Top             =   240
            Width           =   1095
         End
      End
      Begin VB.TextBox txtTimerSpeed 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   35
         Text            =   "MTPbin.frx":0629
         Top             =   840
         Width           =   615
      End
      Begin VB.Frame Frame 
         Caption         =   "Program Options"
         Height          =   3615
         Index           =   0
         Left            =   8880
         TabIndex        =   28
         Top             =   360
         Width           =   5085
         Begin VB.CheckBox chkWriteATP 
            Caption         =   "Write ATP to Immediate"
            Height          =   255
            Left            =   135
            TabIndex        =   922
            ToolTipText     =   "Write Temperature Profile to Immediate Window"
            Top             =   2835
            Width           =   2025
         End
         Begin VB.CheckBox chkGenerateStatusPacket 
            Caption         =   "Generate Status Packet"
            Height          =   240
            Left            =   2970
            TabIndex        =   917
            Top             =   3015
            Width           =   2040
         End
         Begin VB.ComboBox cboAverageTBs 
            Height          =   315
            Left            =   4365
            TabIndex        =   915
            Text            =   "4"
            Top             =   1980
            Width           =   555
         End
         Begin VB.CheckBox chkAverageTBs 
            Caption         =   "Average TBs"
            Height          =   240
            Left            =   2970
            TabIndex        =   914
            Top             =   1980
            Width           =   1275
         End
         Begin VB.CheckBox chkEditTtgt 
            Caption         =   "Edit Ttgt"
            Height          =   255
            Left            =   2970
            TabIndex        =   850
            ToolTipText     =   "Normally RFI does not produce negative counts deflections. Check this box to flag them."
            Top             =   990
            Width           =   1065
         End
         Begin VB.CheckBox chkZgZp 
            Caption         =   "Write Zg-Zp"
            Height          =   255
            Left            =   2970
            TabIndex        =   849
            ToolTipText     =   "Write Zg-Zp differences to allow P corrections for CAMEX-4"
            Top             =   750
            Width           =   1275
         End
         Begin VB.CheckBox chkAllowNegativeRFI 
            Caption         =   "Allow Negative RFI"
            Height          =   225
            Left            =   2970
            TabIndex        =   848
            ToolTipText     =   "Normally RFI does not produce negative counts deflections. Check this box to flag them."
            Top             =   510
            Width           =   1665
         End
         Begin VB.ComboBox cboPTfile 
            Height          =   315
            Left            =   2310
            TabIndex        =   846
            Text            =   "NO"
            Top             =   1710
            Width           =   615
         End
         Begin VB.CheckBox chkUseSubstitutedRAWfile 
            Caption         =   "Use Substituted RAW"
            Height          =   195
            Left            =   2970
            TabIndex        =   845
            Top             =   1740
            Width           =   1875
         End
         Begin VB.CheckBox chkLOchanging 
            Caption         =   "Check if LO is Changing"
            Height          =   255
            Left            =   120
            TabIndex        =   842
            Top             =   1170
            Width           =   2205
         End
         Begin VB.CheckBox chkSkipTemplateRAOBs 
            Caption         =   "Skip Template RAOBs"
            Height          =   195
            Left            =   2970
            TabIndex        =   704
            Top             =   270
            Width           =   1965
         End
         Begin VB.CheckBox chkShowStatus 
            Caption         =   "Show Status"
            Height          =   195
            Left            =   120
            TabIndex        =   689
            Top             =   2160
            Value           =   1  'Checked
            Width           =   1695
         End
         Begin VB.CheckBox chkDisableCh2 
            Caption         =   "Disable Ch2 if dT>1.5 K"
            Height          =   195
            Left            =   120
            TabIndex        =   542
            ToolTipText     =   "Eliminate CAMEX-4 ER2 Ch2 data during launch"
            Top             =   510
            Width           =   2175
         End
         Begin VB.CheckBox chkDisableBlending 
            Caption         =   "Disable Blending"
            Height          =   195
            Left            =   120
            TabIndex        =   526
            Top             =   2640
            Width           =   2055
         End
         Begin VB.CheckBox chkRetrieveSingleScan 
            Caption         =   "Retrieve Single Scan"
            Height          =   195
            Left            =   120
            TabIndex        =   525
            Top             =   1680
            Width           =   2055
         End
         Begin VB.CheckBox chkNewCalFormat 
            Caption         =   "Use New Cal File Format"
            Height          =   195
            Left            =   120
            TabIndex        =   133
            Top             =   2400
            Width           =   2055
         End
         Begin VB.CheckBox chkDisplayBadScans 
            Caption         =   "Display Bad Scans"
            Height          =   255
            Left            =   120
            TabIndex        =   34
            Top             =   240
            Width           =   1695
         End
         Begin VB.CheckBox chkNextAline 
            Caption         =   "Check Next A-line"
            Height          =   195
            Left            =   2970
            TabIndex        =   33
            Top             =   1260
            Value           =   1  'Checked
            Width           =   1575
         End
         Begin VB.CheckBox chkWriteBadCts 
            Caption         =   "Write Bad Counts"
            Height          =   195
            Left            =   120
            TabIndex        =   32
            Top             =   720
            Value           =   1  'Checked
            Width           =   1575
         End
         Begin VB.CheckBox chkEnableDT 
            Caption         =   "Allow Double Trops"
            Height          =   195
            Left            =   120
            TabIndex        =   31
            Top             =   960
            Value           =   1  'Checked
            Width           =   1695
         End
         Begin VB.CheckBox chkEnableFit 
            Caption         =   "Spline Fit ATP"
            Height          =   195
            Left            =   120
            TabIndex        =   30
            Top             =   1440
            Width           =   1335
         End
         Begin VB.CheckBox chkCh2Only 
            Caption         =   "Channel 2 Only"
            Height          =   195
            Left            =   120
            TabIndex        =   29
            ToolTipText     =   "Process data using Ch2 only (PEM Tropics B)"
            Top             =   1920
            Width           =   1575
         End
         Begin VB.Label lblResponse 
            Caption         =   "Realtime IO Status"
            Height          =   195
            Left            =   2970
            TabIndex        =   916
            Top             =   3285
            Width           =   1995
         End
         Begin VB.Label label 
            Caption         =   "Source"
            Height          =   165
            Index           =   7
            Left            =   2310
            TabIndex        =   847
            ToolTipText     =   "Source of alternate P and T: NO=default=Nav, MM=MMS, PT=PTW"
            Top             =   1500
            Width           =   615
         End
         Begin VB.Label label 
            Caption         =   "Average"
            Height          =   255
            Index           =   147
            Left            =   90
            TabIndex        =   707
            Top             =   3210
            Width           =   615
         End
         Begin VB.Label label 
            Caption         =   "Cycles"
            Height          =   225
            Index           =   145
            Left            =   1230
            TabIndex        =   705
            Top             =   3210
            Width           =   495
         End
      End
      Begin VB.TextBox txtPrintUTsec 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   27
         Text            =   "MTPbin.frx":062C
         Top             =   480
         Width           =   615
      End
      Begin VB.CheckBox chkPrintDiagnostics 
         Caption         =   "Print Diagnostics at"
         Height          =   255
         Left            =   120
         TabIndex        =   26
         ToolTipText     =   "Record number at which to start printing diagnostics (where coded)"
         Top             =   480
         Width           =   1695
      End
      Begin VB.TextBox txtTavg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   25
         Text            =   "MTPbin.frx":0631
         Top             =   1200
         Width           =   615
      End
      Begin VB.Frame Frame 
         Caption         =   "Data Source"
         Height          =   975
         Index           =   8
         Left            =   105
         TabIndex        =   22
         Top             =   2700
         Width           =   2235
         Begin VB.ComboBox cboSource 
            Height          =   315
            Left            =   90
            TabIndex        =   24
            Text            =   "Existing File"
            Top             =   240
            Width           =   2070
         End
         Begin VB.TextBox txtCOM1 
            Alignment       =   2  'Center
            BackColor       =   &H00FFFFFF&
            Height          =   285
            Left            =   1020
            MultiLine       =   -1  'True
            TabIndex        =   23
            Top             =   600
            Width           =   1140
         End
         Begin VB.Label label 
            Caption         =   "COM Status"
            Height          =   195
            Index           =   151
            Left            =   120
            TabIndex        =   724
            Top             =   630
            Width           =   1005
         End
      End
      Begin MSComDlg.CommonDialog CommonDialog1 
         Left            =   -73440
         Top             =   360
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.Label label 
         Caption         =   "6"
         Height          =   255
         Index           =   194
         Left            =   -66660
         TabIndex        =   912
         Top             =   2070
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "1"
         Height          =   255
         Index           =   193
         Left            =   -66660
         TabIndex        =   911
         Top             =   570
         Width           =   165
      End
      Begin VB.Label label 
         Caption         =   "10"
         Height          =   255
         Index           =   192
         Left            =   -66690
         TabIndex        =   910
         Top             =   3120
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "20"
         Height          =   255
         Index           =   191
         Left            =   -70410
         TabIndex        =   909
         Top             =   630
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "16"
         Height          =   255
         Index           =   190
         Left            =   -70410
         TabIndex        =   908
         Top             =   1200
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "12"
         Height          =   255
         Index           =   189
         Left            =   -70410
         TabIndex        =   907
         Top             =   1740
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "8"
         Height          =   255
         Index           =   188
         Left            =   -70335
         TabIndex        =   906
         Top             =   2310
         Width           =   165
      End
      Begin VB.Label label 
         Caption         =   "4"
         Height          =   255
         Index           =   152
         Left            =   -70335
         TabIndex        =   905
         Top             =   2880
         Width           =   165
      End
      Begin VB.Label label 
         Caption         =   "Loc"
         Height          =   255
         Index           =   150
         Left            =   -66720
         TabIndex        =   904
         Top             =   3420
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "FL"
         Height          =   255
         Index           =   68
         Left            =   -70380
         TabIndex        =   903
         Top             =   3420
         Width           =   225
      End
      Begin VB.Label Label1 
         Caption         =   "Trms"
         Height          =   180
         Left            =   -72000
         TabIndex        =   837
         Top             =   3210
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "ND"
         Height          =   225
         Index           =   153
         Left            =   -74910
         TabIndex        =   728
         Top             =   3705
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "Record"
         Height          =   255
         Index           =   148
         Left            =   -74850
         TabIndex        =   721
         Top             =   3570
         Width           =   585
      End
      Begin VB.Label label 
         Caption         =   "OAT"
         Height          =   165
         Index           =   144
         Left            =   -63360
         TabIndex        =   698
         Top             =   2820
         Width           =   315
      End
      Begin VB.Label label 
         Caption         =   "Start UT"
         Height          =   255
         Index           =   122
         Left            =   -73860
         TabIndex        =   696
         Top             =   1890
         Width           =   615
      End
      Begin VB.Label label 
         Caption         =   "End UT"
         Height          =   255
         Index           =   123
         Left            =   -73860
         TabIndex        =   695
         Top             =   2310
         Width           =   615
      End
      Begin VB.Label label 
         Caption         =   "d"
         Height          =   255
         Index           =   143
         Left            =   -74880
         TabIndex        =   691
         Top             =   1710
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "6"
         Height          =   225
         Index           =   128
         Left            =   -74880
         TabIndex        =   683
         Top             =   1935
         Width           =   165
      End
      Begin VB.Label label 
         Caption         =   "12"
         Height          =   225
         Index           =   127
         Left            =   -74910
         TabIndex        =   682
         Top             =   3450
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "10"
         Height          =   225
         Index           =   126
         Left            =   -74910
         TabIndex        =   681
         Top             =   2985
         Width           =   195
      End
      Begin VB.Label label 
         Caption         =   "5"
         Height          =   225
         Index           =   125
         Left            =   -74880
         TabIndex        =   680
         Top             =   1695
         Width           =   165
      End
      Begin VB.Label label 
         Caption         =   "1"
         Height          =   225
         Index           =   124
         Left            =   -74880
         TabIndex        =   679
         Top             =   660
         Width           =   165
      End
      Begin VB.Line Line2 
         X1              =   -63480
         X2              =   -63480
         Y1              =   420
         Y2              =   3930
      End
      Begin VB.Label label 
         Caption         =   "CH2-3"
         Height          =   165
         Index           =   121
         Left            =   -63360
         TabIndex        =   535
         Top             =   2490
         Width           =   555
      End
      Begin VB.Label label 
         Caption         =   "CH1-3"
         Height          =   165
         Index           =   120
         Left            =   -63360
         TabIndex        =   534
         Top             =   2160
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "CH1-2"
         Height          =   165
         Index           =   119
         Left            =   -63360
         TabIndex        =   533
         Top             =   1860
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "minus"
         Height          =   255
         Index           =   117
         Left            =   -72000
         TabIndex        =   466
         Top             =   1530
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   116
         Left            =   -74880
         TabIndex        =   456
         Top             =   1710
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Step"
         ForeColor       =   &H00000000&
         Height          =   255
         Index           =   115
         Left            =   -72120
         TabIndex        =   454
         Top             =   1200
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   114
         Left            =   -74880
         TabIndex        =   452
         Top             =   1200
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   113
         Left            =   -74880
         TabIndex        =   450
         Top             =   1800
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "Avg"
         Height          =   255
         Index           =   112
         Left            =   -62340
         TabIndex        =   449
         Top             =   1560
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "End"
         Height          =   255
         Index           =   111
         Left            =   -62340
         TabIndex        =   440
         Top             =   1200
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   110
         Left            =   -62340
         TabIndex        =   439
         Top             =   840
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Start"
         Height          =   255
         Index           =   109
         Left            =   -62340
         TabIndex        =   438
         Top             =   480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "pALT"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Index           =   108
         Left            =   -74760
         TabIndex        =   436
         Top             =   840
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   107
         Left            =   -68280
         TabIndex        =   434
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   106
         Left            =   -69840
         TabIndex        =   433
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "  6"
         Height          =   255
         Index           =   105
         Left            =   -61440
         TabIndex        =   416
         Top             =   2520
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "18"
         Height          =   255
         Index           =   104
         Left            =   -61440
         TabIndex        =   415
         Top             =   1200
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "210"
         Height          =   255
         Index           =   103
         Left            =   -62010
         TabIndex        =   414
         Top             =   2520
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "270"
         Height          =   255
         Index           =   102
         Left            =   -62010
         TabIndex        =   413
         Top             =   1200
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "  0"
         Height          =   255
         Index           =   101
         Left            =   -61440
         TabIndex        =   412
         Top             =   3120
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "12"
         Height          =   255
         Index           =   100
         Left            =   -61440
         TabIndex        =   411
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "24"
         Height          =   255
         Index           =   99
         Left            =   -61440
         TabIndex        =   410
         Top             =   600
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "240"
         Height          =   255
         Index           =   98
         Left            =   -62010
         TabIndex        =   409
         Top             =   1920
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "180"
         Height          =   255
         Index           =   97
         Left            =   -62010
         TabIndex        =   408
         Top             =   3120
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "300"
         Height          =   255
         Index           =   96
         Left            =   -62010
         TabIndex        =   407
         Top             =   600
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Tic"
         Height          =   255
         Index           =   81
         Left            =   -66480
         TabIndex        =   406
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   80
         Left            =   -68160
         TabIndex        =   405
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Gain"
         ForeColor       =   &H00FF00FF&
         Height          =   255
         Index           =   79
         Left            =   -72090
         TabIndex        =   400
         Top             =   360
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Z(km)"
         ForeColor       =   &H00FFFF80&
         Height          =   255
         Index           =   78
         Left            =   -61590
         TabIndex        =   399
         Top             =   360
         Width           =   405
      End
      Begin VB.Label label 
         Caption         =   "T(K)"
         ForeColor       =   &H0080FFFF&
         Height          =   255
         Index           =   77
         Left            =   -62040
         TabIndex        =   398
         Top             =   360
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "3"
         Height          =   255
         Index           =   76
         Left            =   -70770
         TabIndex        =   397
         Top             =   1320
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "2"
         Height          =   255
         Index           =   75
         Left            =   -70770
         TabIndex        =   396
         Top             =   960
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "1"
         Height          =   255
         Index           =   74
         Left            =   -70770
         TabIndex        =   395
         Top             =   600
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "Crms   CB      CS      CN"
         Height          =   255
         Index           =   73
         Left            =   -70530
         TabIndex        =   394
         Top             =   360
         Width           =   1935
      End
      Begin VB.Label label 
         Caption         =   "Tic"
         Height          =   255
         Index           =   72
         Left            =   -69720
         TabIndex        =   392
         Top             =   3630
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   71
         Left            =   -71280
         TabIndex        =   391
         Top             =   3630
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Tic"
         Height          =   255
         Index           =   70
         Left            =   -69975
         TabIndex        =   390
         Top             =   3705
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   69
         Left            =   -71535
         TabIndex        =   389
         Top             =   3705
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "E"
         Height          =   255
         Index           =   67
         Left            =   -74880
         TabIndex        =   388
         Top             =   2430
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "D"
         Height          =   255
         Index           =   57
         Left            =   -74880
         TabIndex        =   387
         Top             =   2070
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "C"
         Height          =   255
         Index           =   55
         Left            =   -74880
         TabIndex        =   386
         Top             =   1320
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "B"
         Height          =   255
         Index           =   54
         Left            =   -74880
         TabIndex        =   385
         Top             =   960
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "A"
         Height          =   255
         Index           =   42
         Left            =   -74880
         TabIndex        =   384
         Top             =   600
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "Scans to Average"
         Height          =   255
         Index           =   41
         Left            =   120
         TabIndex        =   383
         ToolTipText     =   "Number of T to Average, Not currently used!"
         Top             =   1200
         Width           =   1815
      End
      Begin VB.Label label 
         Caption         =   "Timer Speed (ms)"
         Height          =   255
         Index           =   20
         Left            =   120
         TabIndex        =   382
         ToolTipText     =   "Wait between scans when performing a retrieval"
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label label 
         Caption         =   "Wtd"
         Height          =   255
         Index           =   39
         Left            =   -63360
         TabIndex        =   362
         Top             =   1500
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "CH3"
         Height          =   255
         Index           =   35
         Left            =   -63360
         TabIndex        =   361
         Top             =   1200
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "CH2"
         Height          =   255
         Index           =   34
         Left            =   -63360
         TabIndex        =   360
         Top             =   900
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "CH1"
         Height          =   255
         Index           =   31
         Left            =   -63360
         TabIndex        =   359
         Top             =   600
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "dTtotal"
         Height          =   255
         Index           =   30
         Left            =   -61680
         TabIndex        =   358
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "dTrms"
         Height          =   255
         Index           =   29
         Left            =   -62280
         TabIndex        =   355
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "dTavg"
         Height          =   255
         Index           =   28
         Left            =   -62880
         TabIndex        =   354
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   27
         Left            =   -70440
         TabIndex        =   352
         Top             =   3630
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   21
         Left            =   -68880
         TabIndex        =   351
         Top             =   3630
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Cursor Units"
         Height          =   255
         Index           =   15
         Left            =   -69000
         TabIndex        =   341
         Top             =   2880
         Width           =   855
      End
      Begin VB.Label label 
         Caption         =   "Longitude Tic (deg)"
         Height          =   375
         Index           =   14
         Left            =   -65040
         TabIndex        =   340
         Top             =   3360
         Width           =   855
      End
      Begin VB.Label label 
         Caption         =   "Latitude Tic (deg)"
         Height          =   495
         Index           =   2
         Left            =   -66960
         TabIndex        =   339
         Top             =   1440
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "Flight Track"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   1
         Left            =   -64920
         TabIndex        =   328
         Top             =   360
         Width           =   1215
      End
      Begin VB.Label label 
         Caption         =   "Cycles Averaged"
         Height          =   255
         Index           =   65
         Left            =   -64560
         TabIndex        =   319
         Top             =   1200
         Width           =   1455
      End
      Begin VB.Label label 
         Caption         =   "2nd Pass Threshold"
         Height          =   255
         Index           =   64
         Left            =   -64560
         TabIndex        =   318
         Top             =   840
         Width           =   1455
      End
      Begin VB.Label label 
         Caption         =   "1st Pass Threshold"
         Height          =   255
         Index           =   63
         Left            =   -64560
         TabIndex        =   317
         Top             =   480
         Width           =   1455
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   24
         Left            =   -72120
         TabIndex        =   316
         Top             =   3420
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   23
         Left            =   -74880
         TabIndex        =   315
         Top             =   1320
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   6
         Left            =   -70560
         TabIndex        =   314
         Top             =   3420
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Threshold"
         Height          =   255
         Index           =   66
         Left            =   -63720
         TabIndex        =   269
         Top             =   3480
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "Step"
         Height          =   255
         Index           =   58
         Left            =   -71040
         TabIndex        =   268
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "Unit"
         Height          =   255
         Index           =   25
         Left            =   -72600
         TabIndex        =   267
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "MUX Data Selection           NAV  Data"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   10
         Left            =   -66120
         TabIndex        =   266
         Top             =   360
         Width           =   3375
      End
      Begin VB.Label label 
         BackColor       =   &H8000000B&
         Caption         =   "Channel 1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   134
         Left            =   -74880
         TabIndex        =   252
         Top             =   480
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "Channel 2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   135
         Left            =   -74070
         TabIndex        =   251
         Top             =   480
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "Channel 3"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Index           =   136
         Left            =   -73260
         TabIndex        =   250
         Top             =   480
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "1"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   3
         Left            =   -70440
         TabIndex        =   249
         Top             =   600
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "5"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   4
         Left            =   -70440
         TabIndex        =   248
         Top             =   1680
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "10"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   5
         Left            =   -70440
         TabIndex        =   247
         Top             =   2640
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "Scan Angle vs TB"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   8
         Left            =   -70080
         TabIndex        =   246
         Top             =   480
         Width           =   1575
      End
      Begin VB.Label label 
         Caption         =   "Cycles in Window"
         Height          =   255
         Index           =   11
         Left            =   -71760
         TabIndex        =   192
         Top             =   3120
         Width           =   1335
      End
      Begin VB.Label label 
         Caption         =   "Gain"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   7.8
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   12
         Left            =   -72240
         TabIndex        =   191
         Top             =   1680
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "TTO (s)"
         Height          =   255
         Index           =   56
         Left            =   -74880
         TabIndex        =   162
         Top             =   3120
         Width           =   1095
      End
      Begin VB.Label label 
         Caption         =   "E"
         Height          =   255
         Index           =   52
         Left            =   -74880
         TabIndex        =   161
         Top             =   2040
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "D"
         Height          =   255
         Index           =   51
         Left            =   -74880
         TabIndex        =   160
         Top             =   1680
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "C"
         Height          =   255
         Index           =   50
         Left            =   -74880
         TabIndex        =   159
         Top             =   1320
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "B"
         Height          =   255
         Index           =   49
         Left            =   -74880
         TabIndex        =   158
         Top             =   960
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "Line"
         Height          =   255
         Index           =   53
         Left            =   -74880
         TabIndex        =   157
         Top             =   360
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "A"
         Height          =   255
         Index           =   48
         Left            =   -74880
         TabIndex        =   156
         Top             =   600
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "CN Ch"
         Height          =   255
         Index           =   43
         Left            =   -69360
         TabIndex        =   132
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "CS Ch"
         Height          =   255
         Index           =   46
         Left            =   -69840
         TabIndex        =   131
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "CB Ch"
         Height          =   255
         Index           =   45
         Left            =   -70320
         TabIndex        =   130
         Top             =   360
         Width           =   495
      End
      Begin VB.Label label 
         Caption         =   "RMS"
         Height          =   255
         Index           =   32
         Left            =   -70800
         TabIndex        =   129
         Top             =   360
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "CH3"
         ForeColor       =   &H00FF0000&
         Height          =   195
         Index           =   140
         Left            =   -73125
         TabIndex        =   128
         Top             =   390
         Width           =   375
      End
      Begin VB.Label label 
         Caption         =   "CH2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   141
         Left            =   -73845
         TabIndex        =   127
         Top             =   390
         Width           =   405
      End
      Begin VB.Label label 
         Caption         =   "CH1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   142
         Left            =   -74535
         TabIndex        =   126
         Top             =   390
         Width           =   405
      End
      Begin VB.Label label 
         Caption         =   "Output File"
         Height          =   225
         Index           =   130
         Left            =   120
         TabIndex        =   125
         Top             =   3705
         Width           =   855
      End
      Begin VB.Label label 
         Caption         =   "1"
         Height          =   255
         Index           =   33
         Left            =   -71040
         TabIndex        =   124
         Top             =   600
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "2"
         Height          =   255
         Index           =   38
         Left            =   -71040
         TabIndex        =   123
         Top             =   960
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "3"
         Height          =   255
         Index           =   44
         Left            =   -71040
         TabIndex        =   122
         Top             =   1320
         Width           =   135
      End
      Begin VB.Label label 
         Caption         =   "Ch"
         Height          =   255
         Index           =   47
         Left            =   -71160
         TabIndex        =   121
         Top             =   360
         Width           =   255
      End
      Begin VB.Label label 
         Caption         =   "Timer Speed (ms)"
         Height          =   255
         Index           =   62
         Left            =   -74880
         TabIndex        =   120
         Top             =   1440
         Width           =   1335
      End
      Begin VB.Label label 
         Caption         =   "UT [sec]"
         Height          =   255
         Index           =   26
         Left            =   -72960
         TabIndex        =   119
         Top             =   840
         Width           =   735
      End
      Begin VB.Label label 
         Caption         =   "Lead/Lag Cycles Avg'd"
         Height          =   255
         Index           =   36
         Left            =   -74880
         TabIndex        =   118
         Top             =   1800
         Width           =   1695
      End
      Begin VB.Label label 
         Caption         =   "Horizon Scan Position"
         Height          =   255
         Index           =   37
         Left            =   -74880
         TabIndex        =   117
         Top             =   2160
         Width           =   1575
      End
   End
   Begin VB.Line Line3 
      BorderWidth     =   2
      X1              =   14314.37
      X2              =   15390.29
      Y1              =   1305.473
      Y2              =   1305.473
   End
   Begin VB.Label label 
      Height          =   225
      Index           =   146
      Left            =   9480
      TabIndex        =   706
      Top             =   3210
      Width           =   495
   End
   Begin VB.Label label 
      Caption         =   "Total Cycles"
      Height          =   255
      Index           =   0
      Left            =   7200
      TabIndex        =   320
      Top             =   4800
      Width           =   975
   End
   Begin VB.Label label 
      Caption         =   "Bad Cycles"
      Height          =   255
      Index           =   22
      Left            =   7200
      TabIndex        =   20
      Top             =   4440
      Width           =   855
   End
   Begin VB.Label label 
      Caption         =   "Current Cycle"
      Height          =   255
      Index           =   133
      Left            =   7200
      TabIndex        =   18
      Top             =   4080
      Width           =   975
   End
   Begin VB.Label lblProgress 
      Caption         =   "Position"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   4800
      Width           =   615
   End
   Begin VB.Label label 
      Caption         =   "UTsec pALT gALT    Lat     Lon Hdg Pitch  Roll OAT_K"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.4
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   13
      Left            =   210
      TabIndex        =   1
      Top             =   4080
      Width           =   5535
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuFormat 
      Caption         =   "Format"
      Begin VB.Menu mnuFormatATP 
         Caption         =   "ATP"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuFormatCTC 
         Caption         =   "CTC"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "View"
      Begin VB.Menu mnuViewFltInfo 
         Caption         =   "Flight Information"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuViewATP 
         Caption         =   "Altitude Temperature Profile"
      End
      Begin VB.Menu mnuViewNDP 
         Caption         =   "Number Density Profile"
      End
      Begin VB.Menu mnuViewTBs 
         Caption         =   "Brightness Temperatures"
      End
      Begin VB.Menu mnuViewData 
         Caption         =   "Diagnostics"
      End
      Begin VB.Menu mnuViewLimits 
         Caption         =   "Limits Summary"
      End
      Begin VB.Menu mnuViewEditBits 
         Caption         =   "Edit Bits"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "Help"
      Begin VB.Menu mnuHelpUsersGuide 
         Caption         =   "&User's Guide"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmMTPbin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Done - Need to mod ReadArrayLeadLag for NGV to handle Tifa and Ttgt properly
' nOATn in ReadNextAline does not use aNAVoffset while OAT in ReadArrayLeadLag does
' Search for "Latitude >50" and check that code is valid
'
'
' UID yyyymmdd Comment
' mjm 20080409 replace all occurances of LoadFormAsPNG
'
' mjm 20080409 Commented out  'ReadSETUP ("SYSTEM") in REF2writeFLTINFO
                              'Drive$ = DataDrive$
                              'PgmDrive$ = ProgramDrive$
' mjm 20070321 Saved existing code before removing OPENbin, OPENref buttons
'              on frmMTPbin. Removed USEfile!, USE5$ etc because SE now
'              calculated from RC info. Looking at data prior to RCF files
'              might require using this code. Started working on removing references to
'              AC$ in CASE structures.
' mjm 20040923 Removed support for old DC8 up-down retrievals using OBavA, OBavB, etc.
' mjm 20040907 Saved current versions of code before introducing OPTION EXPLICIT and BASE 0 for arrays
'              Code changes are flagged by "mjmmjm"
' mjm 20030131 SetArcAvgObs, changed code to be function of LocHor, but Nelev=10 only
' mjm 19990907 Added filter (dCrms) to remove RFI based on CtsQuality for WB57
'              Hardwired: need to add parameters and define for ER2 and DC2
' mjm 19990730 Added look ahead at A-line to calculate average pALT and OATnav
' mjm 19990727 Removed MainER and MainDC, changed all gains to arrays (1 to Channels)
'

Public ShowZp As Boolean               'If TRUE display ATP in pressure altitude, not geometric
Public ShowDiagnostics As Boolean      'If TRUE write diagnostic information
Public RetrieveSingleScan As Boolean   'If TRUE retrieve a single scan
Public Forward As Boolean              'If TRUE step forward, not backward
Dim BINfileRead As Boolean             'Never set!
Dim ShowNDP As Boolean                 'If TRUE, show Number Density profile
Dim SingleStep As Boolean              'If TRUE, single step through data analysis
Dim DoRetrieval As Boolean             'If TRUE, perform retrievals (normally TRUE)
Dim ShowRAW As Boolean                 'Show RAW data on ABCDE tab
Dim ShowATP As Boolean                 'Show Altitude Temperature Profile form
Dim ShowTBs As Boolean                 'Show Brightness Temperature form
Dim Takeoff As Boolean                 'TRUE if aircraft has taken off
Dim Landing As Boolean                 'TRUE if aircraft has landed
Dim FirstAline As Boolean              'TRUE if first Aline has been read

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
Dim ThetaStart!             'Starting IAC level
Dim ThetaEnd!               'Ending IAC level
Dim ThetaStep!              'IAC level increment
Dim ThetaLevels%            'Number of IAC levels

' Variables used to track mouse in picture boxes
Dim FirstMouseLat As Long
Dim FirstMouseLon As Long
Dim FirstMouseX As Long
Dim LastMouseX As Long
Dim LastMouseXSave As Long
Dim FirstMouseXSave As Long
Dim FirstMouseY As Long
Dim LastMouseY As Long
Dim LastMouseYSave As Long
Dim FirstMouseYSave As Long
Dim DrawingBox As Boolean
Dim EnableCXY As Boolean
Dim EnableMXY As Boolean
Dim YC0save%, YC1save%, YC2save%, YC3save%  'CTS
Dim YG0save%, YG1save%, YG2save%, YG3save%  'GAIN
Dim YM0save%, YM1save%, YM2save%, YM3save%  'Mux
Dim YT0save%, YT1save%, YT2save%, YT3save%  'TB
Dim YP0save%, YP1save%, YP2save%, YP3save%  'PT

Dim PTWfile$                        'PTW file name
Dim RT!(1 To 33)                    'Current ATP temperature (K)
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
Dim flag%()                         'Moving Average (MA) counts quality flag
Dim cTs%()                          'Counts MA array
Dim MLC!()                          'Tmxr MA array
Dim TLC!()                          'Ttgt MA array
Dim WLC!()                          'Twin MA array
Dim OLC!()                          'Tnav MA array

Dim BadNr%                          'Number of bad scans
Dim pALTtakeoff!                    'Altitude at take off
Dim Scanlength!                     'Scan Length

' API Call to start another program
' Used to open HTML help file
Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" _
              (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, _
               ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
                
Private Const SW_SHOW = 1

' API Call to start a non-modal Message Box
Private Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (ByVal hwnd As Long, ByVal lpText As String, ByVal lpCaption As String, ByVal wType As Long) As Long

Sub CalculateArrayMAfast()
' There are two box car averaging windows: FAST and SLOW
' This routine uses the FAST window on sky counts
' CalculateArrayMAslow uses the SLOW window on Base, Target and ND counts, and Tifa and Ttgt temperatues
  Dim i%, j%, k%, L%, n2%, n3%, r%, Nr%, Good As Boolean, GoodCount%, Cycle%, BadCyclesLast2%, x!
  Dim save%, More As Boolean, M%, MoreCount%, Diff!, c1%, c2%, DiffMax!(1 To 3), DiffMaxLast!
  Static TtCNT%
' Hardwire CMAcycles on first pass to be sure to capture "tentpoles" If this was not done,
' the fast cycle CMAcycles2 value might be too small
  CMAcycles2 = CMAcycles2   '13    ' Number of FAST cycles to boxcar average fast on first pass
'  RFIthreshold = 200 ' Threshold for first edit pass (get the really bad guys)
'  frmFLTINFO.txtRFIthreshold.Text = RFIthreshold
  MUXthreshold = Val(frmFLTINFO.txtMUXthreshold.Text)
  RFIthreshold = Val(frmFLTINFO.txtRFIthreshold.Text) 'xxxmjm20110127
  
  ReDim flag%(1 To CMAcycles), cTs%(1 To Channels, 1 To 14, 1 To 31), mA!(1 To Channels, 1 To 14)
  ReDim MLC!(1 To 31), TLC!(1 To 31), WLC!(1 To 31), OLC!(1 To 31)
  txtFileRecords.Text = TotalCycles

' Fast Averaging (Sky Counts only)
  RawCycle = 0
  Cycle = 0
  n2 = Int(CMAcycles2 / 2)  'eg. CMA=31, N2=15, N3=16; CMA must be ODD
  n3 = n2 + 1
  c1 = 1: c2 = 10         'Sky Counts

' Fill CTS with CMAcycles of records
  For r = 1 To CMAcycles2       'First CMAcycles Cycles
    For i = 1 To Channels
      For j = c1 To c2
        cTs(i, j, r) = CV(i, j, r)
      Next j
    Next i
  Next r
  Nr = 2 * (r - 1) + 1
  For r = 1 To n3
    For i = 1 To Channels
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To Nr
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / Nr + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
  Next r
  'Debug.Print R; cTs(1, 1, 1); cTs(1, 1, 2); cTs(1, 1, 3)

  TtCNT = 0:  Badcycles2 = 0: x = 0
  For r = n3 + 1 To TotalCycles - n2 'Remaining TotalCycles-CMAcycles
' Shift Moving Average stack
    For k = 1 To CMAcycles2
      For i = 1 To Channels
        For j = c1 To c2
          If k = CMAcycles2 Then
            cTs(i, j, k) = CV(i, j, r + n2)    'mjm added N2
          Else
            cTs(i, j, k) = cTs(i, j, k + 1)
          End If
        Next j
      Next i
    Next k
   'Debug.Print R; cTs(1, 1, 1); cTs(1, 1, 2); cTs(1, 1, 3)
    CQV(r) = True      'Now do Counts Data
    DiffMax(1) = 0: DiffMax(2) = 0: DiffMax(3) = 0
    For i = 1 To Channels
      CQM(i, r) = 0
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To CMAcycles2
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        EV(i, j, r) = Int(1# * mA(i, j) / CMAcycles2 + 0.5)  'Average
        Diff = CV(i, j, r) - EV(i, j, r)
        If r > 627 And r < 633 And j = 6 And i = 3 Then
        'Debug.Print r; CV(i, j, r); EV(i, j, r); Diff
        k = k
        
        End If
        
        If chkAllowNegativeRFI.Value = 0 Then Diff = Abs(Diff)
        
        If Diff > RFIthreshold Then
          CQV(r) = False
          CQM(i, r) = CQM(i, r) Or 2 ^ (j - 1)
          If Diff > DiffMax(3) Then
            DiffMax(3) = Diff
            DiffMax(1) = i
            DiffMax(2) = j
          End If
        End If
        If Mission$ = "CAMEX4" Then    ' Fix Rapid T change in Humberto's Eye
          If r > 1123 And r - 2 < 1133 And YYYYMMDD$ = "20010923" Then CQVS(r) = True
        End If
      Next j
    Next i
'    If DiffMax(3) > Threshold Then
'      Debug.Print r; DiffMax(1); DiffMax(2); DiffMax(3); CQV(r)
'      x = x + 1
'    End If
    DiffMaxLast = DiffMax(3)
    If Not CQV(r) Then Badcycles2 = Badcycles2 + 1: txtBadCounts.Text = Str(Badcycles2)
    txtRecord.Text = r
    ProgressBar1.Value = r
    DoEvents
  Next r

  For r = TotalCycles - n2 + 1 To TotalCycles
    M = n2
    For i = 1 To Channels
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To 2 * (M - 1) + 1
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / (2 * (M - 1) + 1) + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
  Next r

' Now do a pass with only good scans. Fill CTS with CMAcycles of records
  CMAcycles = Val(frmFLTINFO.txtCMAcycles.Text)        'Number of SLOW cycles to boxcar average slow
  CMAcycles2 = Val(frmFLTINFO.txtCMAcycles2.Text)      'Number of FAST cycles to boxcar average fast
  If CMAcycles2 > CMAcycles Then
    CMAcycles2 = CMAcycles
    frmFLTINFO.txtCMAcycles2.Text = frmFLTINFO.txtCMAcycles.Text
    MsgBox "Fast Cycle MA Count must be <= Slow Cycle Count!", vbOKOnly
  End If
  RFIthreshold = Val(frmFLTINFO.txtRFIthreshold.Text)  'Threshold for first edit pass
  MUXthreshold = Val(frmFLTINFO.txtMUXthreshold.Text)
  RFIiterations2 = 0
  n2 = Int(CMAcycles2 / 2)  'eg. CMA=31, N2=15, N3=16; CMA must be ODD
  n3 = n2 + 1
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Repeat2:
  CMAcycles2 = Val(frmFLTINFO.txtCMAcycles2.Text)      'Number of FAST cycles to boxcar average fast
  RFIiterations2 = RFIiterations2 + 1
  Badcycles2 = 0
  For r = 1 To CMAcycles2       'First CMAcycles Cycles
    For i = 1 To Channels
      For j = c1 To c2
        cTs(i, j, r) = CV(i, j, r)
      Next j
    Next i
  Next r
  Nr = 2 * (r - 1) + 1
  For r = 1 To n3
    For i = 1 To Channels
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To Nr
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / Nr + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
  Next r

  For r = n3 + 1 To TotalCycles - n2 'Remaining TotalCycles-CMAcycles
' Shift Moving Average stack
    For k = 1 To CMAcycles2
      For i = 1 To Channels
        For j = c1 To c2
          If k = CMAcycles2 Then
            cTs(i, j, k) = CV(i, j, r + n2)
            If CQV(r + n2) Then flag(k) = 1 Else flag(k) = 0     '0=Bad 1=Good
          Else
            cTs(i, j, k) = cTs(i, j, k + 1)
            flag(k) = flag(k + 1)
          End If
'  If (Not CQV(r)) And r > 671 And k = CMAcycles2 And r < 674 Then 'And i = 1 And j = 9 And k = 3 Then
'  Debug.Print r; i; j; CQV(r); cTs(i, j, 1); cTs(i, j, 2); cTs(i, j, 3); flag(1); flag(2); flag(3)
'  x = x
'  End If
        Next j
      Next i
    Next k

'   Now calculate Moving Average
    CQV(r) = True
    For i = 1 To Channels
      CQM(i, r) = 0
      For j = c1 To c2
        mA(i, j) = 0: GoodCount = 0
        If r > 671 Then
        x = x
        End If
        For k = 1 To CMAcycles2
          If flag(k) = 1 Then
            mA(i, j) = mA(i, j) + cTs(i, j, k)
            GoodCount = GoodCount + 1
          End If
        Next k
        If GoodCount > 0 Then
          EV(i, j, r) = Int(1# * mA(i, j) / GoodCount + 0.5)
        Else
          EV(i, j, r) = EV(i, j, r - 1)
        End If
        
        Diff = CV(i, j, r) - EV(i, j, r)
        If chkAllowNegativeRFI.Value = 0 Then Diff = Abs(Diff)
      
        If Diff > RFIthreshold Then 'Check again
          CQV(r) = False
          CQM(i, r) = CQM(i, r) Or 2 ^ (j - 1)
          If Mission$ = "CAMEX4" Then    ' Fix Rapid T change in Humberto's Eye
            If r > 1123 And r - 2 < 1133 And YYYYMMDD$ = "20010923" Then CQVS(r) = True
          End If
        End If
      Next j
    Next i

  'Debug.Print CV(1, 12, R) - EV(1, 12, R)
    If Not CQV(r) Then Badcycles2 = Badcycles2 + 1
    ProgressBar1.Value = r
    DoEvents
  Next r

  For r = TotalCycles - n2 + 1 To TotalCycles
    M = n2
    For i = 1 To Channels
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To 2 * (M - 1) + 1
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / (2 * (M - 1) + 1) + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
        'If r = 389 And Not CQV(389) Then
         ' x = x
        'End If
  Next r

  txtBadCounts.Text = Str(Badcycles2)
  With frmFLTINFO
    .txtRFIiterations2 = RFIiterations2
    .txtBadCycles2 = Badcycles2
  End With
  
  If Badcycles2 > BadCyclesLast2 Then
    BadCyclesLast2 = Badcycles2
    GoTo Repeat2   'Loop again
  End If

'---------------End of Fast Cycle------------------------------------

End Sub

Sub CalculateTTOstatistics(C%, m1%, m2%, AVG!, RMS!)
Dim i%, M%, dTsum1!, dTsum2!, GoodCounts%, Tmtp!, Tref!, T!(1 To 3)
Dim dT!

    dTsum1 = 0#: dTsum2 = 0#: GoodCounts = 0
    For M = m1 To m2
      If CQV(M) Then
        GoodCounts = GoodCounts + 1
        Call CalculateGain(M, g())     'This calls ReadArrayLeadLag which calculates Tnav, Tifa, Tnd etc.
      
        Tmtp = 0#
        Select Case C
        Case 0         'Use weighted sum of three channels
          For i = 1 To Channels
            Tref = Ttgt + cTo
            T(i) = fTB(Tref, CS(i, LocHor), CB(i), g(i))
            Tmtp = Tmtp + T(i) * ChInfo(i)
          Next i
          Tmtp = Tmtp / ChInfoSum
          'Debug.Print M; NAVoffset; OATV(1, M); OAT; Tmtp
        Case Else      'Do each channel separately
          Tref = Ttgt + cTo
          Tmtp = fTB(Tref, CS(C, LocHor), CB(C), g(C))
        End Select
        dT = Tmtp - OAT                'V(1, M + NAVoffset) + OATnavCOR
        dTsum1 = dTsum1 + dT         'Sum time difference
        dTsum2 = dTsum2 + dT ^ 2     'Sum square of time difference
      End If
    Next M
' Write statistics: Channel 0 is Tmtp, weighted average of other channels
    AVG = dTsum1 / GoodCounts       'Calculate average
    If (dTsum2 - dTsum1 ^ 2 / GoodCounts) >= 0 Then RMS = Sqr((dTsum2 - dTsum1 ^ 2 / GoodCounts) / (GoodCounts - 1)) Else RMS = 0# 'Calculate RMS

End Sub

Sub CalculateTTOstatisticsE(C%, m1%, m2%, FL1!, FL2!, AVG!, RMS!, Tavg!, n%)
Dim i%, M%, dTsum1!, dTsum2!, GoodCounts%, Tmtp!, Tref!, T!(1 To 3)
Dim dT!

    dTsum1 = 0#: dTsum2 = 0#: GoodCounts = 0: Tavg = 0#
    For M = m1 To m2
      If CQV(M) And FL1 <> FL2 And ALTV(1, M) > FL1 And ALTV(1, M) < FL2 Then
        GoodCounts = GoodCounts + 1
        Call CalculateGain(M, g())     'This calls ReadArrayLeadLag which calculates Tnav, Tifa, Tnd etc.
      
        Tmtp = 0#
        Select Case C
        Case 0         'Use weighted sum of three channels
          For i = 1 To Channels
            Tref = Ttgt + cTo
            T(i) = fTB(Tref, CS(i, LocHor), CB(i), g(i))
            Tmtp = Tmtp + T(i) * ChInfo(i)
          Next i
          Tmtp = Tmtp / ChInfoSum
        Case Else      'Do each channel separately
          Tref = Ttgt + cTo
          Tmtp = fTB(Tref, CS(C, LocHor), CB(C), g(C))
        End Select
        Tavg = Tavg + OAT
        dT = Tmtp - OAT                'V(1, M + NAVoffset) + OATnavCOR
        dTsum1 = dTsum1 + dT         'Sum time difference
        dTsum2 = dTsum2 + dT ^ 2     'Sum square of time difference
      End If
    Next M
' Write statistics: Channel 0 is Tmtp, weighted average of other channels
    n = GoodCounts
    If n > 2 Then
      Tavg = Tavg / n
      AVG = dTsum1 / n       'Calculate average
      If (dTsum2 - dTsum1 ^ 2 / n) >= 0 Then RMS = Sqr((dTsum2 - dTsum1 ^ 2 / n) / (n - 1)) Else RMS = 0# 'Calculate RMS
    Else
      n = 0
    End If

End Sub
Function fATPcompare(ATP!(), jmax%)
' Compare RAOB on flight track to AA T profile used to calculate RCs
' Return with -1 if RMS or TE thresholds are not met
' Return with RC set index if either threshold is satisfied
Dim i%, j%, x!, TAavg!, TArms!, TAte!, Sum1!, Sum2!, RMSmin!, iRMS%, TEmin!, iTE%
Dim RMSthreshold!, TEthreshold!

  RMSthreshold = 2#                    'RMS Threshold
  TEthreshold = 3.5                    'Total Error Threshold
  RMSmin = 999#: TEmin = 999#
  For i = 0 To NRC - 1
    Sum1 = 0#: Sum2 = 0#
    For j = 1 To jmax
      x = (ATP(j) - RTav(i, 1, j))     'Compare to highest flight level = 1
      Sum1 = Sum1 + x                  'Weighted average
      Sum2 = Sum2 + x ^ 2              'Sum of squares
    Next j
    TAavg = Sum1 / jmax                                 'Calculate average
    TArms = Sqr((Sum2 - jmax * TAavg ^ 2) / (jmax - 1)) 'Calculate RMS
    TAte = Sqr(TAavg ^ 2 + TArms ^ 2)
    'Debug.Print i; TAavg; TArms; TAte
    If TArms < RMSmin Then RMSmin = TArms: iRMS = i
    If TAte < TEmin Then TEmin = TAte: iTE = i
  Next i
  
  fATPcompare = -1   ' Default is <0 if neither threshold is satisfied
  If RMSmin <= RMSthreshold Then
    fATPcompare = iRMS
    If TEmin < TEthreshold Then 'Use TE min if less than threshold
      fATPcompare = iTE
    End If
  Else  'Check on TE if RMS not satisfied
    If TEmin < TEthreshold Then 'Use TE min if less than threshold
      fATPcompare = iTE
    End If
  End If
  
End Function

Function fGetExpT(Record%, C%)
' Interpolate NAV exponentially weighted temperature
' C=0 ... avg of all channels
' C=1, 2, 3 ... use individual channel offsets
Dim n%, OAT1!, OAT2!, OATexp!
  
  n = Int(aNAVoffset(C))
  If aNAVoffset(C) - n <> 0 Then  'Interpolate
    OAT1 = Te(fRecordCheck(Record, n), C)
    OAT2 = Te(fRecordCheck(Record, n + 1), C)
    OATexp = OAT1 + (OAT2 - OAT1) * (aNAVoffset(C) - n)
  Else
    OATexp = Te(fRecordCheck(Record, CSng(n)), C)
  End If
  fGetExpT = OATexp '+ OATnavCOR
End Function

Function fGetNavT(Record%, C%)
' Interpolate NAV temperature
' C=0 ......... avg of all channels
' C=1, 2, 3 ... use individual channel offsets
Dim n%, OAT1!, OAT2!, OAT!
  
  n = Int(aNAVoffset(C))
  If aNAVoffset(C) - n <> 0 Then  'Interpolate
    OAT1 = OATV(1, fRecordCheck(Record, n))
    OAT2 = OATV(1, fRecordCheck(Record, n + 1))
    OAT = OAT1 + (OAT2 - OAT1) * (aNAVoffset(C) - n)
    'Debug.Print Record; aNAVoffset(C); n; OAT1; OAT2; OAT
  Else
    OAT = OATV(1, fRecordCheck(Record, CSng(n)))
  End If
  fGetNavT = OAT + OATnavCOR

End Function

Function fLOstuck(RawCycle) As Boolean
Dim i%, NDdiff%(1 To 3)
Static NDlast%(1 To 3), LOstuck As Boolean, NDavg!(1 To 3)
  'Debug.Print RawCycle; CN(1); CN(2); CN(3); LOstuck
  If RawCycle > 1 Then
    For i = 1 To Channels
      NDdiff(i) = Abs(CN(i) - NDlast(i))
    Next i
    'Debug.Print "LO Stuck"; Rawcycle; NDdiff(1); NDdiff(2); NDdiff(3)
    If NDdiff(1) < 20 And NDdiff(2) < 20 And NDdiff(3) < 20 Then
      fLOstuck = False
      LOstuck = False
      For i = 1 To Channels: NDlast(i) = CN(i):      Next i
    Else
      If Not LOstuck Then MsgBox "LO is stuck on one frequency", vbOKOnly, "LO Stuck"
      fLOstuck = True
      LOstuck = True
    End If
  Else
    For i = 1 To Channels: NDlast(i) = CN(i): Next i
  End If

End Function

Function fTB(Tref!, Csky%, Cref%, g!)
'  T=(Ttgt-(cb-cs)/G-(TWIN+273.15)*EMISSIVITY-(TIFA+273.15)*REFLECTIVITY)/(1-EMISSIVITY-REFLECTIVITY)
  If g > 0 Then
    fTB = (Tref + (Csky - Cref) / g - (Twin + cTo) * Emissivity - (Tifa + cTo) * Reflectivity) / (1 - Emissivity - Reflectivity)
  Else
    fTB = 200
  End If
End Function

Function fTwt(T!())
Dim i%, Twt!
' Calculate weighted average temperature

  Twt! = 0#
  For i = 1 To Channels
    Twt = Twt + ChInfo(i) * T(i)
  Next i
  fTwt = Twt / ChInfoSum
  
End Function

Sub IntegrateHorizonTB(j&, Texp!(), iMax%)
Dim i%, k%, eTime!, xSum(1 To 3), x!(1 To 3, 0 To 200)
' Exponentially weight Nav T's to get effective horizon T
' j   ... is index to current a/c position
' Texp .. are the exponentially weighted Ts for CH1, 2 and 3
' iMax .. is number of seconds to integrate from a/c position to 5 e-fold distances
'         I tried 10 e-folding distances as well, but it made no difference in result
    
    If j > iPTZ Then Exit Sub
    Texp(0) = 0#
    For k = 1 To 3
      Texp(k) = 0#
      xSum(k) = 0#
      eTime = (1# / fAbsVsZp(k, Zptz(j))) / 0.2   'efolding distance divided by a/c speed (200 m/s)
      If k = 1 Then iMax = Int(3 * eTime)         '5 efolding times for CH1
      For i = 0 To iMax
        x(k, i) = Exp(-i / eTime)
        xSum(k) = xSum(k) + x(k, i)
        Texp(k) = Texp(k) + Tptz(i + j) * x(k, i)
      Next i
      Texp(k) = Texp(k) / xSum(k) + OATnavCOR
      Texp(0) = Texp(0) + ChInfo(k) * Texp(k)
    Next k
    Texp(0) = Texp(0) / ChInfoSum

End Sub

Public Sub Navigate(ByVal NavTo As String)
  Dim hBrowse As Long
  hBrowse = ShellExecute(0&, "open", NavTo, "", "", SW_SHOW)
End Sub
Sub CalculateATPlevels(RA!(), RT!(), Zt!(), Tstart!, Tstep!, n%)
Dim i%, j%, it%, ib%, Theta!(1 To 33), zTheta!(1 To 33), T!
' Enter with ATP and return with interpolated IAC levels in zPT()
' Code is not working
  For j = 1 To n
    T = Tstart + (j - 1) * Tstep
'    zT(j) = fLinterp(RT(), RA(), Nlev, T)
  i = 0
  Do
    i = i + 1
  Loop Until RT(i) < T Or i = Nlev
  
  If RT(i) = T Then Zt(j) = RT(i)
  If i = Nlev Then Zt(j) = RT(n)
  it = i
  ib = i - 1
  If ib = 0 Then Exit Sub
  If RT(it) = RT(ib) Then
    Zt(j) = RA(ib)
  Else
    Zt(j) = RA(ib) + (RA(it) - RA(ib)) * (T - RT(ib)) / (RT(it) - RT(ib))
  End If
  Next j
' End of PT code
End Sub

Sub CalculateAvgRMS(A!(), ch%, Loc%, r1%, r2%, AVG!, RMS!)
Dim Sum1!, Sum2!, x!, Rec%, r%, j%, i%
  
' Scan to Scan RMS
  For r = r1 + 1 To r2
    x = A(i, j, r) - A(i, j, r - 1)
    Sum1 = Sum1 + x   'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next r
  Rec = r2 - r1
  AVG = Sum1 / Rec         'Calculate average
  If Sum2 - AVG ^ 2 / Rec > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * Rec) / (Rec - 1)) 'Calculate RMS
  
End Sub

Function fScanLength(n1%, n2%) As Single
Dim i%, n%, dUTsec%, UTlast&, ScanTime!
' Calculate average scan length for all records between n1 and n2
' that are less than 25 seconds long.
' CAN instruments have ~20 s scan length
' All other three frequency instruments ~15 s, so 25 s is >1 scan

  i = 0
  UTlast = UV(1, n1)
  ScanTime = 0#
  For n = n1 + 1 To n2
    dUTsec = UV(1, n) - UTlast
    If dUTsec < 25 Then ScanTime = ScanTime + dUTsec: i = i + 1
    UTlast = UV(1, n)
  Next n
  fScanLength = ScanTime / i

End Function
Sub CalculateGain(r%, g!())
Dim i%, j%
  
  Call ReadArrayLeadLag(r)       'get Ttgt, Tmxr, Tnav with lead/lag
                                 'OAT corresponds to aNAVoffset(0)
  Call GainGE(Geqn(), r)         'GE need Tmxr with lag
  Select Case GainScale
  Case 1: For j = 1 To Channels: g(j) = Geqn(j): Next j          'Geqn
  Case 2: Call GainND(g())                                       'ND
  Case 3: Call GainOAT(Geqn(), g(), False)                       'OAT
  End Select
' T=(Ttgt-(cb-cs)/G-(TWIN+273.15)*EMISSIVITY-(TIFA+273.15)*REFLECTIVITY)/(1-EMISSIVITY-REFLECTIVITY)
'  If REFsource = 1 Then Tref = Ttgt + cTo Else Tref = OAT: CB(1) = CS(1): CB(2) = CS(2): CB(3) = CS(3)
'  Tmtp = (Tref + (CS(i) - CB(i)) / g(i) - (Twin + cTo) * Emissivity - (Tifa + cTo) * Reflectivity) / Denom

End Sub

Sub CalculateIAClevels(RA!(), RT!(), zPT!(), ThetaStart!, ThetaStep!, n%)
Dim i%, j%, Theta!(1 To 33), zTheta!(1 To 33), pt!
' Enter with ATP and return with interpolated IAC levels in zPT()
' Code to insert PT levels
' Convert T profile to PT profile
    For j = 1 To Nlev
      Theta(j) = fTheta(RT(j), fZtoP(RA(j)))
    Next j
  ' Do spline fit to theta profile
    Call SPLINE(Theta(), RA(), TD1(), TD2(), 1, Nlev)
'
    For j = 1 To n
      pt = ThetaStart + (j - 1) * ThetaStep
      zPT(j) = SPLINT(Theta!(), RA(), TD2(), 1, Nlev, pt, 1)
    Next j
' End of PT code

End Sub

Sub Custom_Patches()
' Use for testing ideas
' CalculateArrayMA
'   If R - N2 > 1123 And R - N2 < 1133 And Mission$ = "CAMEX4" And yyyymmdd$ = "20010923" Then
'     CQV(R - N2) = True 'Fix Rapid T change in Humberto's Eye
'   End If
'
'
End Sub

Sub ERFwrite()
Dim dZt!, i%, L%, UT1&, UT2&, z!(1 To 33), T!(1 To 33), TemperatureFlag As Boolean
Dim Tzt1!, Tzt2!, OATzt1!, OATzt2!, pALTlast!, UTsecLast&
Dim RetAltMaxP!, RetAltMinP!, pALTrecent!, pALTtrend!
Dim NrCyGap%, Wt!, ALTdiscrep!, UTks!, Ntot%, Zmin!, Zmax!, Zn!, Zx!, x!, f!
Dim Zac!, FileLength&

  GoSub ERFwriteInit

  Do
    UTsecLast = UTsec
    EditWord = 0
    GoSub ReadCycle                                      'Do RetAlt Edit here
    GoSub EditTrops
    
EndCycle:
    Call REFwrite(ERFlu%, Cycle%)
    If frmFLTINFO.chkGoodScansOnly.Value = 1 And GoodScan Then
      Call CheckLimits(Cycle%) ' Collect statistics based on good records.
    Else
      Call CheckLimits(Cycle%) ' Collect statistics for all scans
    End If
    ProgressBar1.Value = Cycle%
    UpdatePC (Cycle%)
    If Cycle > 1150 Then
    EditWord = EditWord
    End If
  Loop Until Cycle% = TotalCycles - 2
  Close

  UpdateForm
  DoEvents
  
  REFwriteHEADER (ERFfile$)
  REFwriteFLTINFO "C:\MTP\Setup\FLTINFO.REF"
Exit Sub
'-------------- SUB-ROUTINES --------------------------------
ReadCycle:
  Cycle% = Cycle% + 1
  If Cycle >= 1329 Then
  x = x
  End If
  Call REFread(REFlu%, Cycle%)
  
  If pALTlast = 0# Then pALTlast = pALT
  ' MTPret uses UTnav with correction to scan horizon position
  ' Only correct if utMTPcor>0
  If utMTPcor > 0 Then
    If Abs(UTsec - UTsecMTP) < 1000 Then
      UTsec = UTsecMTP + utMTPcor + 0.5
    Else
      UTsec = UTsecMTP + utMTPcor + 86400.5
    End If
  End If
  If Cycle% Mod 10 = 0 Then DoEvents 'Allow for user intervention
  'Debug.Print pALT
' Scram if no data
  If Nlev = 0 Then GoodScan = False: GoTo EndCycle

' Read-in 33 levels & decode & edit for retrieved altitude range
' 981008 mjm leave zzz and TTT in same locations, not doing screws up
' SE calculation and you loose original data
  
  If GoodScan Then
  Nlev1 = 0
  TemperatureFlag = True 'Set FALSE when RetAlt out of range
  If frmFLTINFO.chkUseRelativeRA.Value = 0 Then 'Not Implemented Yet
    RetAltMaxP = RetAltMax
    RetAltMinP = RetAltMin
    For L% = 1 To 33  '
    ' Check that A/C is within range of RCs
      z(L) = sZZZ(L%)
      T(L) = sTTT(L%)
      Select Case z(L)
      Case Is > RetAltMaxP       ' Retrieved points too high
      Case Is > RetAltMinP       ' These are the good scans
        If Nlev1 = 0 Then
          Nlev1 = L%
        Else
          Nlev2 = L%
        End If
      Case Else                 ' Retrieved points too low
      End Select
'Debug.Print L; X; Nlev; Nlev1; Nlev2; pALT
' Now check for temperatures out of range once per cycle
      If T(L) < TemperatureMin Or T(L) > TemperatureMax Then
        If TemperatureFlag Then
          TemperatureFlag = False
          EditWord = EditWord Or 2
          INC TemperaturePC
          'Debug.Print L; TemperaturePC, T(L); GoodScan
        End If
      End If
      DoEvents
    Next L
    
  Else  'Use offset relative to flight level
    RetAltMaxP = pALT + Val(frmFLTINFO.txtAbove.Text)
    RetAltMinP = pALT + Val(frmFLTINFO.txtBelow.Text)
    If RetAltMinP < 0 Then RetAltMinP = 0.001
    For L% = 1 To 33  '
  ' Check that A/C is within range of RCs
      z(L) = sZZZ(L%)
      T(L) = sTTT(L%)
      Select Case z(L)
      Case Is > RetAltMaxP       ' Retrieved points too high
      Case Is > RetAltMinP       ' These are the good scans
        If Nlev1 = 0 Then
          Nlev1 = L%
        Else
          Nlev2 = L%
        End If
      Case Else                 ' Retrieved points too low
      End Select
'Debug.Print L; X; Nlev; Nlev1; Nlev2; pALT
' Now check for temperatures out of range once per cycle
      If T(L) < TemperatureMin Or T(L) > TemperatureMax Then
        If TemperatureFlag Then
          TemperatureFlag = False
          EditWord = EditWord Or 2
          INC TemperaturePC
        End If
      End If
      DoEvents
    Next L
  End If
  End If
  If Nlev1 <= 0 Then Nlev1 = 1
  Nlev% = Nlev2 - Nlev1 + 1

  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
    EditWord = EditWord Or 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
    NlevPC = NlevPC + 1
  End If
  If pALT < z(Nlev1) Then EditWord = EditWord Or 1: INC RetAltPC: GoodScan = False

  If pALT < RetAltMin Then GoodScan = False: GoTo EndCycle 'Was 0.1 km
  
  If Abs(pALT - pALTlast) > Val(frmFLTINFO.txtRateMax.Text / 1000) Then
    GoodScan = False
    EditWord = EditWord Or 256
    RatePC = RatePC + 1
  End If
  
  pALTlast = pALT
  If Not GoodScan Then
    GoodTrop = False
    Zt1 = 99.9: Th1 = 999.9
    Zt2 = 99.9: Th2 = 999.9
    LRac = 99.9
  End If
  
  Return
''''''''''''''''''''''''''
' xxxx Check this code out
  If pALTrecent = 0 Then pALTrecent = pALT: pALTtrend = 0.01: pALTlast = pALT

  NrCyGap = Int((UTsec - UTsecLast) / 13) 'Nr cycles in data gap
  If NrCyGap = 0 And UTsec > UTsecLast Then NrCyGap = 1

  If pALT > 10 Then
    Wt = 1
    ALTdiscrep = pALT - (pALTrecent + (Wt + 1) * pALTtrend)
    If ALTdiscrep > 5 Then: Beep: pALT = pALTrecent
  End If

  If pALT < 23 Then
    pALTrecent = (pALTrecent + pALT) / 2
    pALTtrend = (pALTtrend + (pALT - pALTlast) / NrCyGap) / 2
  End If

  pALTlast = pALT

Return

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
EditTrops:
' Empirical Trop Alt Correction
  UTks = UTsec / 1000
  If EditOATtrop Then
    Select Case UTks
    Case Is < OATks20  'Ascent
      If UTks > OATks10 And (OATzt10 > 0 Or OATzt20 > 0) Then
        OATzt1 = OATzt10
        OATzt2 = OATzt20
        Tzt1 = Tzt10
        Tzt2 = Tzt20
      Else
        GoTo NoOATtrop
      End If
      
    Case Is < OATks22  'Dip 1
      If UTks > OATks12 And (OATzt12 > 0 Or OATzt22 > 0) Then
        OATzt1 = OATzt12
        OATzt2 = OATzt22
        Tzt1 = Tzt12
        Tzt2 = Tzt22
      Else
        GoTo NoOATtrop
      End If
      
    Case Is < OATks23  'Dip 2
      If UTks > OATks13 And (OATzt13 > 0 Or OATzt23 > 0) Then
        OATzt1 = OATzt13
        OATzt2 = OATzt23
        Tzt1 = Tzt13
        Tzt2 = Tzt23
      Else
        GoTo NoOATtrop
      End If
      
    Case Is > OATks11  'Descent
      If (OATzt11 > 0 Or OATzt21 > 0) Then
        OATzt1 = OATzt11
        OATzt2 = OATzt21
        Tzt1 = Tzt11
        Tzt2 = Tzt21
      Else
        GoTo NoOATtrop
      End If
      
    Case Else
      GoTo NoOATtrop
    End Select
' Substitute OAT Tropopause for Zt1 and Zt2
  EditWord = EditWord Or 4
  OATtropPC = OATtropPC + 1
  If OATzt1 > 0 Then
    Zt1 = OATzt1
    Th1 = fTheta(Tzt1, fZtoP(Zt1))
    If OATzt2 > 0 Then
      Zt2 = OATzt2
      Th2 = fTheta(Tzt2, fZtoP(Zt2))
    Else
      Zt2 = 99.9
      Th2 = 999.9
    End If
  Else
    If OATzt2 > 0 Then
      Zt1 = OATzt2
      Th1 = fTheta(Tzt2, fZtoP(Zt1))
    Else
      Zt1 = 99.9
      Th1 = 999.9
    End If
    
  End If
  
  SEp1 = 99.9
  
  dZt = Zt1 - pALT
  If EditZtOff And (dZt < ZtOffA Or dZt > ZtOffB) Then EditWord = EditWord Or 8: ZtOffPC = ZtOffPC + 1
  If EditTropAlt And (Zt1 < TropAltMin Or Zt1 > TropAltMax) Then EditWord = EditWord Or 16: TropAltPC = TropAltPC + 1
  If EditPitch And (Pitch < ePitchMin Or Pitch > ePitchMax) Then EditWord = EditWord Or 32: PitchPC = PitchPC + 1
  If EditRoll And (Roll < eRollMin Or Roll > eRollMax) Then EditWord = EditWord Or 64: RollPC = RollPC + 1

Return
' EditWord Bitmap
' RetAlt        1*
' Temperature   2
' OATtrop       4
' ZtOff         8*
' TropAlt      16*
' Pitch        32*
' Roll         64*
' Nlev<8      128*

Else
NoOATtrop:
  If Zt1 < 99 Then    ' Bad trop from REFmake, no solution found
    dZt = Zt1 - pALT
    If EditZtOff And (dZt < ZtOffA Or dZt > ZtOffB) Then
      EditWord = EditWord Or 8
      ZtOffPC = ZtOffPC + 1
      GoodTrop = False
    End If
  
    If EditTropAlt And (Zt1 < TropAltMin Or Zt1 > TropAltMax) Then
      EditWord = EditWord Or 16
      TropAltPC = TropAltPC + 1
      GoodTrop = False
    End If
  End If
  
  If EditPitch And (Pitch < ePitchMin Or Pitch > ePitchMax) Then
    EditWord = EditWord Or 32  'H0010
    PitchPC = PitchPC + 1
    'Debug.Print Pitch
'    GoodTrop = False   'xxx mjm 20000116  'too many trops lost on ACCENT
  End If
  
  If EditRoll And (Roll < eRollMin Or Roll > eRollMax) Then
    'Debug.Print UTks; Roll
    EditWord = EditWord Or 64  'H0020
    RollPC = RollPC + 1
    GoodTrop = False
    GoodScan = False
  End If
  
'  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
'    EditWord = EditWord + 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
'    NlevPC = NlevPC + 1
'  End If
'  If pALT < zzz(Nlev2) / 100 Then EditWord = EditWord + 1: INC RetAltPC: GoodScan = False
  
End If

' Make sure there are at least 8 acceptable levels
  Ntot = 0: Zmin = 0.1: Zmax = 30
  For L = 1 To 33
    x = sZZZ(L) '/ 100
    If x > Zmin And x < Zmax Then Ntot = Ntot + 1
  Next L
' If Ntot < 8 Then GoodScan = False: GoTo EndCycle    'skip out if not valid data cycle
  f = (40 - Abs(Roll)) / 40: If f < 0 Then f = 0
  If Abs(Roll) > 3 Then
    Zn = Zmin: Zx = Zmax: Zac = pALT
    Zmax = Zac + (Zx - Zac) * f
    Zmin = Zac + (Zn - Zac) * f
  End If

  If frmFLTINFO.chkSubstitute.Value = 1 And GoodTrop = False Then 'Replace all undefined trops
    If OATzt14 > 0 Then
      Zt1 = OATzt14
      Th1 = fTheta(Tzt14, fZtoP(Zt1))
      If OATzt24 > 0 Then
        Zt2 = OATzt24
        Th2 = fTheta(Tzt24, fZtoP(Zt2))
      Else
        Zt2 = 99.9
        Th2 = 999.9
      End If
    End If
    EditWord = EditWord Or 4
    OATtropPC = OATtropPC + 1
    GoodTrop = True
  End If
Return

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  
ERFwriteInit:
  On Error GoTo OpenERF
  FileLength = FileLen(ERFfile$)
  If FileLength > 0 And Not DoAll Then
    Msg = ERFfile$ + " already exists!" + vbCrLf + vbCr + "Do you want to delete it?"  ' Define message.
    Style = vbYesNo                             ' Define buttons.
    Title = "WARNING!"                 ' Define title.
  ' Help = "DEMO.HLP" ' Define Help file.
  ' Ctxt = 1000 ' Define topic  context.
    'response = MsgBox(Msg, Style, Title, Help, Ctxt)
    'If response = vbYes Then Kill Path$ + ERFfile$
    Kill ERFfile$
  Else
    Kill ERFfile$
  End If
    
OpenERF:
  ERFopen ERFfile$
' Open REF-file; don't allow re-edit of ERF file
' Use RAF file instead of REF file is it exists
  If Dir(RAFfile$) <> "" Then
    REFopen RAFfile$
  Else
    REFopen REFfile$
  End If
  ProgressBar1.Min = 0
  FileFormatIn = fREFreadFileFormat(REFfile$)
  Select Case FileFormatIn
  Case 32
    FileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
  Case 33
    FileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
  End Select
  ProgressBar1.Max = TotalCycles    'FileRecords
  
  Call CheckLimitsInit   'Reset Limit Information
  
  TropAltPC = 0:  RetAltPC = 0:  ZtOffPC = 0: OATtropPC = 0
  TemperaturePC = 0:  PitchPC = 0:  RollPC = 0: NlevPC = 0: RatePC = 0
  
' Check for OAT trops
'  If chkOATtrop2.value = 1 Then EditOATtrop = True Else EditOATtrop = False
  With frmFLTINFO
    OATzt10 = Val(.txtOATzt1(0).Text)
    Tzt10 = Val(.txtTzt1(0).Text)
    OATzt20 = Val(.txtOATzt2(0).Text)
    Tzt20 = Val(.txtTzt2(0).Text)
    OATks10 = Val(.txtOATks1(0).Text)
    OATks20 = Val(.txtOATks2(0).Text)

    OATzt11 = Val(.txtOATzt1(1).Text)
    Tzt11 = Val(.txtTzt1(1).Text)
    OATzt21 = Val(.txtOATzt2(1).Text)
    Tzt21 = Val(.txtTzt2(1).Text)
    OATks11 = Val(.txtOATks1(1).Text)
    OATks21 = Val(.txtOATks2(1).Text)

    OATzt12 = Val(.txtOATzt1(2).Text)
    Tzt12 = Val(.txtTzt1(2).Text)
    OATzt22 = Val(.txtOATzt2(2).Text)
    Tzt22 = Val(.txtTzt2(2).Text)
    OATks12 = Val(.txtOATks1(2).Text)
    OATks22 = Val(.txtOATks2(2).Text)

    OATzt13 = Val(.txtOATzt1(3).Text)
    Tzt13 = Val(.txtTzt1(3).Text)
    OATzt23 = Val(.txtOATzt2(3).Text)
    Tzt23 = Val(.txtTzt2(3).Text)
    OATks13 = Val(.txtOATks1(3).Text)
    OATks23 = Val(.txtOATks2(3).Text)
  End With
  
  Cycle% = 0                        ' skip Limit info - update
  If frmFLTINFO.chkUTminmax.Value = 1 Then
    UT1 = Val(frmFLTINFO.txtUTtakeoff.Text)
    UT2 = Val(frmFLTINFO.txtUTlanding.Text)
    frmFLTINFO.txtUTksmin = UT1
    frmFLTINFO.txtUTksmax = UT2
    UTksmin = UT1
    UTksmax = UT2
  End If
  If frmFLTINFO.txtUTksmax.Text = "" Or frmFLTINFO.txtUTksmax.Text = "0" Then UT2 = Val(frmFLTINFO.txtUTmax.Text)
  If frmFLTINFO.txtUTksmin.Text = "" Or frmFLTINFO.txtUTksmin.Text = "0" Then UT1 = Val(frmFLTINFO.txtUTmin.Text)

Return

End Sub

Function fMJcolor&(n%)
If n > 25 Then n = n Mod 25
Select Case n
Case 1: fMJcolor = &H80&      'dark red
Case 2: fMJcolor = &HFF&      'red
Case 3: fMJcolor = &HC0C0FF   'light red
Case 4: fMJcolor = &H4080&
Case 5: fMJcolor = &H80FF&    'orange
Case 6: fMJcolor = &HC0E0FF
Case 7: fMJcolor = &H8080&
Case 8:
fMJcolor = &HFFFF&    'yellow
Case 9: fMJcolor = &HC0FFFF
Case 10: fMJcolor = &H8000&
Case 11: fMJcolor = &HFF00&   'green
Case 12: fMJcolor = &HC0FFC0
Case 13: fMJcolor = &H808000
Case 14: fMJcolor = &HFFFF00  'light blue
Case 15: fMJcolor = &HFFFFC0
Case 16: fMJcolor = &H800000
Case 17: fMJcolor = &HFF0000  'blue
Case 18: fMJcolor = &HFFC0C0
Case 19: fMJcolor = &H800080
Case 20: fMJcolor = &HFF00FF  'purple
Case 21: fMJcolor = &HFFC0FF
Case 22: fMJcolor = &H404040
Case 23: fMJcolor = &HC0C0C0  'grey
Case 24: fMJcolor = &HFFFFFF  'white
Case 25: fMJcolor = &H0&      'black
End Select

End Function

Function fPI$()
Dim A$, HDRfile$, lu%

  lu% = FreeFile
  HDRfile$ = MNpath$ + Mission$ + ".HDR"
  Open HDRfile$ For Input As lu%
  Line Input #lu%, A$              'Skip first line
  Line Input #lu%, A$              'Get PI$
  Close (lu%)
  fPI$ = A$

End Function
Function fRecordCheck(Record%, offset%)
Dim r%
' Make sure (Record% + offset) is >=1 or <= TotalCycles
  r = Record + offset
  Select Case r
  Case Is < 1: r = 1
  Case Is > TotalCycles: r = TotalCycles
  End Select
  fRecordCheck = r
End Function

Sub GainGE(Geqn!(), r%)
Dim i%, j%, FP!(1 To 5)

  For i = 1 To Nfit   ' Get appropriate fit parameters, FP(j)
    Select Case NP$(i)
    Case "Offset": FP(i) = 1#
    Case "Tifa": FP(i) = Tifa - GOF(i)
    Case "Tlo1": FP(i) = Tlo1
    Case "Tlo2": FP(i) = Tlo2
    Case "Tnav": FP(i) = OAT - cTo      'OAT was Tnav, which was undefined in ReadArrayLeadLag
    Case "Time": FP(i) = GOF(i)
    End Select
  Next i
  
' Note that in the following Go was redefined after PEMTB and WAM
  Select Case Mission$
  Case "PEMTB", "WAM"             ' G = Go + slope*(Tifa-Tref)  pre "SOLVE", "ACCENT", "TexAQS", "CAMEX4"
    For i = 1 To Channels
      Geqn(i) = 0#
      For j = 1 To Nfit
        If j = 5 Then
          Geqn(i) = Geqn(i) + FP(j) * Exp(-UTsec / GEC(i, j))
        Else
          Geqn(i) = Geqn(i) + FP(j) * GEC(i, j)
        End If
      Next j
    Next
  Case Else                        'G = Go*(1+slope*(Tifa-Tref))
    For i = 1 To Channels
      Geqn(i) = GEC(i, 1)
      For j = 2 To Nfit
        If NP$(j) = "Time" Then
          Geqn(i) = Geqn(i) + FP(j) * Exp(-(UV(1, r) - UV(1, 1)) / 1000# / GEC(i, j))
        Else
          Geqn(i) = Geqn(i) + FP(j) * GEC(i, j)
        End If
      Next j
    Next
  End Select
    
End Sub
Sub GainND(Gnd!())
Dim i%, dT!
' This code used to have hardwired Noise Diode radiation temperatures
' before auto fitting procedure was developed

  dT = Tnd - TrefND
  For i = 1 To Channels
    ' Note that the ND deflection (dND) is moving average if UseMAforCN is TRUE
    ' This is determined in ReadArrayLeadLag
    ' Cnd0(i) are zero the first time this routine is read
    If Cnd0(i) > 0 Then Gnd(i) = dND(i) / (Cnd0(i) * (1# + Cnd1(i) * dT + Cnd2(i) * dT ^ 2))
  Next i

End Sub





Function fGetNavZp(Record%)
Dim ALT!, ALT1!, ALT2!, n%
  
  If NAVoffset - Int(NAVoffset) <> 0 Then  'Interpolate
    n = Int(NAVoffset)
    ALT1 = ALTV(1, fRecordCheck(Record, n))
    ALT2 = ALTV(1, fRecordCheck(Record, n + 1))
    ALT = ALT1 + (ALT2 - ALT1) * (NAVoffset - n)
  Else
    ALT = ALTV(1, fRecordCheck(Record, CSng(NAVoffset)))
  End If
  fGetNavZp = ALT
End Function

Sub ReadCURRENTrecord(RAWlu%)
Dim i%, j%, A$, b$, UTks!, x$, UT&, Th!, U!, V!, W!, c1$, Lt$
Dim GoToNext As Boolean, dALT!, CycleTakeoff%, PTfile$
Dim mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, NextAline$, FirstAline As Boolean
Static UTsecLast&, Day, Change As Boolean, pALTftLast&, lu%, Poff!, pALTlast!, Status_Packet$
Dim ALTlast!, OUTlu%
' This routine is only used during realtime data processing

  If RawCycle = 0 Then UTsecLast = 0  'Make sure reset in case of multiple passes
  If Left$(SU$, 3) = "CAN" Then
    GoSub ReadGVscan
  Else
    GoSub ReadABCDElines               'Read five or six lines of input data
  End If

' Note: Cycle only counts good cycles, but ML and TL use all cycles.
  RawCycle% = RawCycle% + 1            'Increment total raw cycle count
  Cycle = RawCycle
  If NavQual Then NQV(RawCycle) = True Else NQV(RawCycle) = False
  Aline = 0                            'Reset for next pass
  
  WriteArray (RawCycle)
  CheckLimits (RawCycle) 'Lat Lon check after fix in WriteArray
  LastMouseX = RawCycle

' Construct MTPstatus.txt using Ku data in case Iridium status packets are missing
' MTP,20111029T071925.000,259,10499,12549,14511,9209,10895,13001,7279,8565,11083,54.0098,-60.8687,45.8913,9,4011
'  c(i,j) has counts
  Status_Packet = "MTP," + YYYYMMDD$ + "T" + fSecToTstring(UTsec, False) + ".000,259,"
  Status_Packet = Status_Packet + Format(C(1, 6), "00000") + "," + Format(C(1, 12), "00000") + "," + Format(C(1, 11), "00000") + ","
  Status_Packet = Status_Packet + Format(C(2, 6), "00000") + "," + Format(C(2, 12), "00000") + "," + Format(C(2, 11), "00000") + ","
  Status_Packet = Status_Packet + Format(C(3, 6), "00000") + "," + Format(C(3, 12), "00000") + "," + Format(C(3, 11), "00000") + ","
  Status_Packet = Status_Packet + Format(Ttgt, "##0.000") + "," + Format(Twin, "##0.000") + "," + Format(Tnd, "##0.000") + ","
  Status_Packet = Status_Packet + Format(dACC, "###0") + "," + Format(RawCycle, "0000")
  OUTlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Output As OUTlu
  Print #OUTlu, Status_Packet
  Close OUTlu
EndOfSub:
'  Close RAWlu
Exit Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
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
  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  DoEvents
  If Len(A$) < 5 Then GoTo TryAgainGV
    
  Select Case Mid$(A$, 2, 1)
  Case " ", "W", "0", "t"                          'Valid second characters
  Case Else: GoTo TryAgainGV
  End Select
  
ProcessNextAlineGV:
  'If all scan lines have not been read,
  If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then  '
  ' Loop through here until all the necessary lines have been read for a scan
    GoSub ProcessInputLineGV
    If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then GoTo TryAgainGV '
  End If
Return

ProcessInputLineGV:
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" Then Return
    'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
    'Un-comment a$ line above to simulate bad DADS data
    NavQual = True

    Call DecodeGVline(A$, Not FirstAline)            'This routine decodes all lines
    
    pALT = ZpAvg                     'Use average Zp for scan
'   Time in A-line is MTP clock time synchronized to NTP -- program uses this
'   Time in IWG-line is time from IWG server, which has some latency
    LineCurr(1) = A$
    Aline% = 1
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
    Call DecodeGVline(A$, Not FirstAline)
    
    If Left$(A$, 3) = "IWG" Then
'     Look for Takeoff
      dALT = pALT - ALTlast
      If pALT > 0.006 And dALT > 0.015 And Not Takeoff And Cycle > 0 Then
        CycleTakeoff = Cycle
        Takeoff = True
        Landing = False
        UTtakeoff = UTsecLast
        pALTtakeoff = pALT
        hhmmssTO$ = hhmmssNavLast$
      End If
      
      If Takeoff And Not Landing Then
        If pALT < 2 And Cycle > 1000 And dALT < -0.015 Then
          Landing = True
          UTlanding = UTsec
          If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
        End If
      End If

      'If UTsecLast > UTsec Then UTsec = UTsec + 86400
'     Apply modelled correction to Znav if necessary
      If Pcorr <> 99 Then
        Select Case AC$
        Case "NG"
        ' Code to do pressure corrections
        End Select
      End If
      pALTft = pALT * cft_km
      ALTkmft = pALTft
     
'     At this point OATn and pALT are NAV values
      OAT = cTo + OATn + OATnavCOR
      If OAT > 999.9 Then OAT = 999.9
      Tac = Int(10 * OAT) / 10#          'Temperature at aircraft
      ALTkmLast = pALT
    End If
    
  End Select

  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)

  If ShowRAW Then
    Select Case Lt$
    Case "A":  txtLine(0).Text = A$
    Case "B":  txtLine(1).Text = A$
    Case "C":  txtLine(2).Text = A$
    Case "D":  txtLine(3).Text = A$
    Case "E":  txtLine(4).Text = A$
    End Select
  End If
  UTsecLast = UTsec
  ALTlast = pALT
  UTsecMTPlast = UTsec             'Save in case UT time correction needed
Return


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Read non-GV line

ReadABCDElines:
  If Cycle > 0 Then A$ = NextAline$: GoTo ProcessNextAline
TryAgain:
  If EOF(RAWlu) Then
    If chkZgZp.Value = 1 Then Close lu
    If Not RealTime Then Quit = True
    GoTo EndOfSub
  End If
' Get an input line from RAW File
  Input #RAWlu%, A$
  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
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
' Reset line% flags for the next go around
' Now have A-E lines
Return

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ProcessInputLine:
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" And Not RealTime Then Return
    'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
    'Un-comment a$ line above to simulate bad DADS/ICATS data
    If Len(A$) < 70 Then
      If Mid$(A$, 9, 4) = "2001" Then  'Deal with ROOF data: A-line=A 06-27-2001 01:15:16
        UTsec = fTstringToSec&(Mid$(A$, 14, 8), True)
        pALT = 0#: pALTft = 0#: OATn = 30#
      End If
      'Beep:         GoTo EndOfSub 'GoTo TerminateProgram
    Else
      NavQual = True
      pALTlast = pALT
      Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file

      TTO = fTTO(pALT)       'Time Tag Offset
      UTsec = Int(UTsec + TTO + 0.5)

      dALT = pALT - pALTlast   'Look for Takeoff
      'Debug.Print Cycle; UTsec; pALT; dALT
      If pALT > 0.006 And dALT > 0.015 And Not Takeoff And Cycle > 0 Then
        CycleTakeoff = Cycle
        Takeoff = True
        Landing = False
        UTtakeoff = UTsecLast
        pALTtakeoff = pALT
        hhmmssTO$ = hhmmssNavLast$
      End If
      If Takeoff And Not Landing Then
          If pALT < 2 * pALTtakeoff And dALT < -0.015 Then
            Landing = True
            UTlanding = UTsec
            If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
          End If
      End If
End If
  
    If UTsecLast > UTsec Then
      UTsec = UTsec + 86400
    End If
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
' Note: PTfile$ not defined
' This is where OATmms is defined. With the new procedure of using substituted RAW files, OATnav=OATmms
' and this code is not needed.
    If cboPTfile.ListIndex > 0 Then
''' change to use GetNAVvalues!!!!!!!!!
      Select Case Trim(cboPTfile.Text)
      Case "MM": Call GetMMSvalues_n(UTsec, UT, mALT, OATmms, Th, U, V, W, PTfile$, Quit)
      Case "PT": Call GetPTWvalues(UTsec, UT, mALT, OATmms, PTfile$)
      Case "FT": Call GetFTvalues(UTsec, UT, mALT, OATmms, mLatitude, mLongitude, mZg!, mZp!, mWspd!, mWdir!, PTfile$)
      End Select
      If Quit Then GoTo EndOfSub  'Quit if EOF
      mALTft = mALT * cft_km
    End If
               
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
    frmATP.Tac = Tac
    ALTkmLast = pALT
    UTsecLast = UTsec
  
    If YYYYMMDD$ <> "20020716" Then 'MTP Clocks off by 12 hours
      If (Abs(UTsecNav - UTsecMTP) < 1000 And ElCorUsed = 1) Then NavQual = True Else NavQual = False
    End If
    Aline% = True
    LineCurr(1) = A$
  
'   UpdateCommLine
    Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0

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
          MsgBox "Length of " + c1$ + "-line <61 and this is not an fEc error!", vbOKOnly
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
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'  txtBadCounts.Text = Str(BadCycle)
'  txtFileRecords.Text = Str(Cycle)
  UTsecLast = UTsec

  If ShowRAW Then
    Select Case Lt$
    Case "A":  txtLine(0).Text = A$
    Case "B":  txtLine(1).Text = A$
    Case "C":  txtLine(2).Text = A$
    Case "d":  txtLine(3).Text = A$
    Case "D":  txtLine(4).Text = A$
    Case "E":  txtLine(5).Text = A$
    End Select
  End If
Return

End Sub


Sub RealtimeHTML(FQFN$)
Dim lu%, STAlu%
Dim Status_Packet$, A$, s$(0 To 20), Status$, n%, y!, Twin$             'GH Status Packet

  lu = FreeFile
  Open FQFN$ For Output As lu
  STAlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Input As #STAlu
  Line Input #STAlu, Status_Packet$
  Close STAlu

  Print #lu, "<html>"
  Print #lu, "<head>"
  Print #lu, "<meta content=""Text/html; Charset=ISO-8859-1"""
  Print #lu, "http-equiv=""content-type"">"
  Print #lu, "<title>MTP Realtime</title>"
  Print #lu, "<meta http-equiv=Refresh content=5>"
  Print #lu, "</head>"
  Print #lu, "<body style=""background-image: url(Emily_Luna.jpeg);"">"
  Print #lu, "<h1 style=""color: rgb(255, 255, 255);"">JPL MTP Realtime Temperature Profile</h1>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Universal Time: " + fSecToTstring(UTsec, True) + "</span>&nbsp; "
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Latitude: " + Format(Latitude, "#0.###") + "</span>&nbsp;"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Longitude: " + Format(Longitude, "##0.###") + "</span><Br>"
  Print #lu, "<img alt=""MTPprofile.PNG"" src=""MTPprofile.PNG"" style=""width: 800px; height: 600px;""><br>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">Status Packet: </span><Br>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">"
  Print #lu, Status_Packet$
  Print #lu, "</span><Br>"
  
  A$ = Status_Packet$
  Call ParseCSV(A$, s$(), n%)
  Print #lu, "<pre>"
  Print #lu, "<span style=""font-weight: bold;color: rgb(255, 255, 255);"">"
  Status$ = "Status:  " + s(3) + " -  " + s(2) + "  -  Packet no: " + Format(Val(s(17)), "0000")
  Print #lu, Status$
  Print #lu, "         CH1    CH2    CH3"
  
  Status$ = "TGT+ND:  " + Format(s(4), "00000") + "  " + Format(s(5), "00000") + "  " + Format(s(6), "00000")
  Status$ = Status$ + "  TTGT: +" + Format(Val(s(13)), "00.0")
  Print #lu, Status$
'  Print #lu, "</span>"
  
  Status$ = "TGT:     " + Format(s(7), "00000") + "  " + Format(s(8), "00000") + "  " + Format(s(9), "00000")
  y = Val(s(14))
  If y < 0# Then Twin$ = Format(y, "#00.0") Else Twin$ = Format(y, "00.0")
  Status$ = Status$ + "  TWIN: " + Twin$
  
  Print #lu, Status$
  
  Status$ = "Horizon: " + Format(s(10), "00000") + "  " + Format(s(11), "00000") + "  " + Format(s(12), "00000")
  Status$ = Status$ + "  TND:  +" + Format(Val(s(15)), "00.0")
  Print #lu, Status$
  Print #lu, "</pre>"
  Print #lu, "</body>"
  Print #lu, "</html>"

  Close lu
End Sub
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


Sub SetAttitudeOK(aPitchMin!, aPitchMax!, aRollMin!, aRollMax!)
Dim i%, P!, r!, fEcOK As Boolean, PitchOK As Boolean, RollOK As Boolean

  For i = 1 To TotalCycles
    P = NAVV(1, i)
    r = NAVV(2, i)
    If P > aPitchMin And P < aPitchMax Then PitchOK = True Else PitchOK = False
    If r > aRollMin And r < aRollMax Then RollOK = True Else RollOK = False
    If fEmax(P, r, EmaxDegrees) >= EmaxDegrees Then fEcOK = True Else fEcOK = False
    If PitchOK And RollOK And fEcOK Then AttitudeOK(i) = True Else AttitudeOK(i) = False
    'Debug.Print i; P; r; fEmax(P, r, EmaxDegrees)
  Next i

End Sub



Sub UpdateGAINrange()
Dim i%, j%, M%, m1%, m2%, x1!, x2!, x3!, LineC&, y!
Static GVtop!, GVleft!, GVwidth!, GVheight!, LastRawcycle%
' Called by the UPDATE button on the Gain tab

  If Not ArrayRead And Not RealTime Then Exit Sub
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  
' Set Scales
  With picGain
  .Cls
  .ScaleMode = 0
  .ScaleTop = Val(cboGAIN2.Text)
  .ScaleLeft = m1
  .ScaleHeight = Val(cboGAIN1.Text) - Val(cboGAIN2.Text)
  .ScaleWidth = Val(m2 - m1)
  If m1 = 1 Then m1 = 2

  GVtop = .ScaleTop
  GVleft = .ScaleLeft
  GVheight = .ScaleHeight
  GVwidth = .ScaleWidth
  End With
' Show position
  If LastRawcycle = 0 Then
    picGain.PSet (RawCycle, GVtop)
    picGain.Line -(RawCycle, GVtop + GVheight), QBColor(Black)
    LastRawcycle = RawCycle
  Else
    picGain.PSet (LastRawcycle, GVtop)
    picGain.Line -(LastRawcycle, GVtop + GVheight), picGain.BackColor
    picGain.PSet (RawCycle, GVtop)
    picGain.Line -(RawCycle, GVtop + GVheight), QBColor(Black)
    LastRawcycle = RawCycle
  End If
' Draw X tic marks
  x1 = m1
  x2 = m2
 Select Case cboUTunit(1).Text
  Case "Rec No"
    x3 = Val(cboX3(1).Text)
  Case "UTks"
    x3 = Val(cboX3(1).Text) * GVwidth / (Val(txtX2(1).Text) - Val(txtX1(1).Text))
  Case "Hour"
    x3 = Val(cboX3(1).Text) * GVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  Case "hh:mm:ss"
    x3 = Val(cboX3(1).Text) * GVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  End Select
  
  If chkXgrid.Value = 1 Then
    For i = x1 To x2 Step x3
      picGain.PSet (i, GVtop)
      picGain.Line -(i, GVtop + GVheight), QBColor(Black)
    Next i
  Else
    For i = x1 To x2 Step x3
      picGain.PSet (i, GVtop)
      picGain.Line -(i, GVtop + 0.05 * GVheight), QBColor(Black)
      picGain.PSet (i, GVtop + GVheight)
      picGain.Line -(i, GVtop + 0.95 * GVheight), QBColor(Black)
    Next i
  End If
' Draw Y tic marks
  If chkYgrid.Value = 1 Then
    For i = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step Val(cboGAIN3.Text)
      picGain.PSet (GVleft, i)
      picGain.Line -(GVleft + GVwidth, i), QBColor(Black)
    Next i
  Else
    For i = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step Val(cboGAIN3.Text)
      picGain.PSet (GVleft, i)
      picGain.Line -(GVleft + 0.02 * GVwidth, i), QBColor(Black)
      picGain.PSet (GVleft + GVwidth, i)
      picGain.Line -(GVleft + 0.98 * GVwidth, i), QBColor(Black)
    Next i
  End If
  DoEvents

  For i = 0 To GVchannels
    For j = 1 To Channels
      Select Case j
      Case 1: LineC = &HFF&        'red
      Case 2: LineC = &HFFFFFF     'white
      Case 3: LineC = &HFF0000     'blue
      End Select

      If chkGain(i).Value = 1 Then
        Select Case i
        Case Is < 5
          picGain.PSet (1, GV(i, j, m1))
          For M = m1 + 1 To m2
            If CQV(M) Then
            picGain.Line -(M, GV(i, j, M)), LineC  'And continue from there
            End If
          Next M
        Case 5         'Zp
          y = fFixP(i, GV(i, j, m1))
          LineC = &HFFFF00   'Cyan
          picGain.PSet (m1, y)
          For M = m1 + 1 To m2
            y = fFixP(i, GV(i, j, M))
            picGain.Line -(M, y), LineC  'And continue from there
          Next M
        Case 6         'OAT
          y = fFixT(i, GV(i, j, m1))
          LineC = &HFFFF&     'Yellow
          picGain.PSet (m1, y)
          For M = m1 + 1 To m2
            y = fFixT(i, GV(i, j, M))
            picGain.Line -(M, y), LineC  'And continue from there
          Next M
        End Select
      End If
    Next j
    DoEvents
  Next i
End Sub

Sub UpdateIWGframe(IWGlu%, IWGrecord&)
Dim IWG_yyyymmdd$, IWG_hhmmss$, IWG_UTsec&
    
    Call IWGread(IWGlu, IWGrecord)
    
    Frame(7).Caption = "NAV Parameters"
    Call DateTtoStrings(IWG_DandT, IWG_yyyymmdd, IWG_hhmmss, IWG_UTsec, True)
    label(154) = " 1. Date: " + IWG_yyyymmdd$ + "  Time: " + IWG_hhmmss$ + "  =  " + Format(IWG_UTsec / 1000, "#00.000") + " ks"

    label(155) = " 2. Latitude = " + Format(IWG_Parameter(2), "#00.000")
    label(156) = " 3. Longitude = " + Format(IWG_Parameter(3), "#000.000")
    label(157) = " 4. Geometric Altitude = " + Format(IWG_Parameter(4), "#0000") + " m"
    label(158) = " 5. GPS Altitude = " + Format(IWG_Parameter(5), "#0000") + " m"
    label(159) = " 6. Pressure Altitude = " + Format(IWG_Parameter(6), "#0000") + " ft"
    label(160) = " 7. Radar Altitude = " + Format(IWG_Parameter(7), "#0000") + " ft"
    label(161) = " 8. Ground Spd = " + Format(IWG_Parameter(8), "#00") + " m/s"
    label(162) = " 9. True Air Spd = " + Format(IWG_Parameter(9), "#00") + " m/s"
    label(163) = "10. Indicated Air Spd = " + Format(IWG_Parameter(10), "#00") + " kts"
    label(164) = "11. Mach Number = " + Format(IWG_Parameter(11), "0.000")
    label(165) = "12. Vertical Air Speed = " + Format(IWG_Parameter(12), "#0.00") + " m/s"
    label(166) = "13. Heading = " + Format(IWG_Parameter(13), "#00.0") + " deg"
    label(167) = "14. Track Angle = " + Format(IWG_Parameter(14), "##0.0") + " deg"
    label(168) = "15. Drift Angle = " + Format(IWG_Parameter(15), "##0.0") + " deg"
    label(169) = "16. Pitch = " + Format(IWG_Parameter(16), "#0.0") + " deg"
    label(170) = "17. Roll = " + Format(IWG_Parameter(17), "#0.0") + " deg"
    label(171) = "18. Side Slip Angle = " + Format(IWG_Parameter(18), "#0.0") + " deg"
    label(172) = "19. Attack Angle = " + Format(IWG_Parameter(19), "#0.0") + " deg"
    If chkTinK.Value = 1 Then
      label(173) = "20. OAT = " + Format(IWG_Parameter(20) + cTo, "##0.00") + " K"
    Else
      label(173) = "20. OAT = " + Format(IWG_Parameter(20), "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(174) = "21. Dew Point = " + Format(IWG_Parameter(21) + cTo, "##0.0") + " K"
    Else
      label(174) = "21. Dew Point = " + Format(IWG_Parameter(21), "##0.0") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(175) = "22. Total T = " + Format(IWG_Parameter(22) + cTo, "##0.0") + " K"
    Else
      label(175) = "22. Total T = " + Format(IWG_Parameter(22), "##0.0") + " C"
    End If
    label(176) = "23. Static Pressure = " + Format(IWG_Parameter(23), "###0.0") + " mb"
    label(177) = "24. Dynamic Pressure = " + Format(IWG_Parameter(24), "#0.0") + " mb"
    label(178) = "25. Cabin Pressure = " + Format(IWG_Parameter(25), "##0.0") + " mb"
    label(179) = "26. Wind Speed = " + Format(IWG_Parameter(26), "##0.0") + " m/s"
    label(180) = "27. Wind Direction = " + Format(IWG_Parameter(27), "##0.0") + " deg"
    label(181) = "28. Vertical Wind Speed = " + Format(IWG_Parameter(28), "#0.0") + " m/s"
    label(182) = "29. Solar Zenith Angle = " + Format(IWG_Parameter(29), "##0.0") + " deg"
    label(183) = "30. Sun Elevation A/C = " + Format(IWG_Parameter(30), "##0.0") + " deg"
    label(184) = "31. Sun Elevation Gnd = " + Format(IWG_Parameter(31), "##0.0") + " deg"
    label(185) = "32. Sun Azimuth A/C = " + Format(IWG_Parameter(32), "##0.0") + " deg"

End Sub

Sub UpdateNAVframe(NAVlu%, NAVrecord&)
Dim NAV_yyyymmdd$, NAV_hhmmss$, NAV_UTsec&
    
    Call NAVread(NAVlu, NAVrecord)
    
    Frame(7).Caption = "NAV Parameters"
    Call DateTtoStrings(NAV_DandT, NAV_yyyymmdd, NAV_hhmmss, NAV_UTsec, True)
    label(154) = " 1. Date: " + NAV_yyyymmdd$ + "  Time: " + NAV_hhmmss$ + "  =  " + Format(NAV_UTsec / 1000, "#00.000") + " ks"

    label(155) = " 2. Vertical Acceleration = " + Format(NAV_Parameter(40), "#0.000") + " m/s2"
    label(156) = " 3. IRS Alitude = " + Format(NAV_Parameter(9), "#0.000") + " km"
    If chkTinK.Value = 1 Then
      label(157) = " 4. OAT (AT_A) = " + Format(NAV_Parameter(16), "#000.00") + " K"
    Else
      label(157) = " 4. OAT (AT_A) = " + Format(NAV_Parameter(16) - cTo, "#000.00") + " C"
    End If
    label(158) = " 5. GPS Altitude = " + Format(NAV_Parameter(8), "#0.000") + " km"
    
    label(159) = " 6. Latitude = " + Format(NAV_Parameter(5), "#00.000") + " deg"
    label(160) = " 7. Longitude = " + Format(NAV_Parameter(6), "#000.000") + " deg"
    label(161) = " 8. Ground Spd = " + Format(NAV_Parameter(30), "#00") + " m/s"
    label(162) = " 9. GSF Spd = " + Format(NAV_Parameter(30), "#00") + " m/s"
    label(163) = "10. Mach Number = " + Format(NAV_Parameter(22), "0.000")
    label(164) = "11. PALT_A = " + Format(NAV_Parameter(9), "#00.000") + " km"
    label(165) = "12. Pitch = " + Format(NAV_Parameter(3), "#0.0") + " deg"
    
    label(166) = "13. Pstatic = " + Format(NAV_Parameter(1), "#000.0") + " hPa"
    label(167) = "14. Ptotal = " + Format(NAV_Parameter(28), "#000.0") + " hPa"
    label(168) = "15. QC_A = " + Format(NAV_Parameter(41), "##0.0") + " deg"
    label(169) = "16. Roll = " + Format(NAV_Parameter(4), "#0.0") + " deg"
    label(170) = "17. TAS = " + Format(NAV_Parameter(42), "#000") + " kts"
    label(171) = "18. Heading = " + Format(NAV_Parameter(7), "#0.0") + " deg"
    If chkTinK.Value = 1 Then
      label(172) = "19. TOATn1 = " + Format(NAV_Parameter(34), "#0.00") + " K"
    Else
      label(172) = "19. TOATn1 = " + Format(NAV_Parameter(34) - cTo, "#0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(173) = "20. TOATn2 = " + Format(NAV_Parameter(35), "##0.00") + " K"
    Else
      label(173) = "20. TOATn2 = " + Format(NAV_Parameter(35) - cTo, "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(174) = "21. TOATn3 = " + Format(NAV_Parameter(36), "##0.00") + " K"
    Else
      label(174) = "21. TOATn3 = " + Format(NAV_Parameter(36) - cTo, "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(175) = "22. TOATn4 = " + Format(NAV_Parameter(37), "##0.00") + " K"
    Else
      label(175) = "22. TOATn4 = " + Format(NAV_Parameter(37) - cTo, "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(176) = "23. TOATn0 = " + Format(NAV_Parameter(33), "##0.00") + " K"
    Else
      label(176) = "23. TOATn0 = " + Format(NAV_Parameter(33) - cTo, "##0.00") + " C"
    End If
    
    label(177) = "24. Vzac = " + Format(NAV_Parameter(32), "##0.00") + " m/s"
    If chkTinK.Value = 1 Then
      label(178) = "25. OATn1 = " + Format(NAV_Parameter(17), "#0.00") + " K"
    Else
      label(178) = "25. OATn1 = " + Format(NAV_Parameter(17) - cTo, "#0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(179) = "26. OATn2 = " + Format(NAV_Parameter(18), "##0.00") + " K"
    Else
      label(179) = "26. OATn2 = " + Format(NAV_Parameter(18) - cTo, "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(180) = "27. OATn3 = " + Format(NAV_Parameter(19), "##0.00") + " K"
    Else
      label(180) = "27. OATn3 = " + Format(NAV_Parameter(19) - cTo, "##0.00") + " C"
    End If
    If chkTinK.Value = 1 Then
      label(181) = "28. OATn4 = " + Format(NAV_Parameter(20), "##0.00") + " K"
    Else
      label(181) = "28. OATn4 = " + Format(NAV_Parameter(20) - cTo, "##0.00") + " C"
    End If

    label(182) = "29. Pressure Altitude = " + Format(NAV_Parameter(9), "##0.000") + " km"
    label(183) = "30. Side Slip = " + Format(NAV_Parameter(46), "##0.0") + " deg"
    label(184) = "31. TASF = " + Format(NAV_Parameter(42), "##0.0") + " deg"
    label(185) = "32. Wind Spd = " + Format(NAV_Parameter(10), "##0.0") + " m/s"
    label(186) = "33. Wind Dir = " + Format(NAV_Parameter(11), "##0.0") + " deg"
    label(187) = "34. Mach^2 = " + Format(NAV_Parameter(23), "0.000")

End Sub


Sub UpdateTBrange()
' Update TB Fit Window
Dim i%, ii%, j&, j1&, j2&, M%, m1%, m2%, x!, x1!, x2!, x3!, LineC&, T!(1 To 3), Tmtp!, y!, iMax%
Dim dT!, DTavg!, DTrms!, GoodCounts%, Tref!, PitchOK As Boolean, RollOK As Boolean, Tscale!
Dim TcSum1!(0 To 7), TcSum2!(0 To 7), aPitchMin!, aPitchMax!, aRollMin!, aRollMax!
Dim NewPset As Boolean, PsetM%, IgnoreBadThings As Boolean, WCT!(1 To 3), T1!, T2!, T3!
Dim OAT!, n%, Denom!, T1Sum!, T2Sum!, fEcOK As Boolean, UT&, ZpMin!, Texp!(0 To 3)
Dim Go!(1 To 3), Gr!(1 To 3), gnda!(1 To 3)
Static lu%, TBtop!, TBleft!, TBwidth!, TBheight!, LastRawcycle%, g!(1 To 3), Init As Boolean
    
  If Not ArrayRead And Not RealTime Then Exit Sub
  
  If Not Init Then
    lu = FreeFile
    Open "C:\MTP\TEMP.txt" For Output As lu
    Init = True
  End If
  If chkShowChannels(9).Value = 1 Then IgnoreBadThings = True Else IgnoreBadThings = False
  
  With frmFLTINFO
    aPitchMin = Val(.txtPitchMin.Text)
    aPitchMax = Val(.txtPitchMax.Text)
    aRollMin = Val(.txtRollMin.Text)
    aRollMax = Val(.txtRollMax.Text)
  End With
  
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  Call SetAttitudeOK(aPitchMin!, aPitchMax!, aRollMin!, aRollMax!)
  
' Get first scan where CQV, Pitch and Roll are all OK
' This code is disabled so TBfit sees beginning of flight with excessive pitch or roll
'  i = m1 - 1
'  Do: i = i + 1
'    If i > m2 Then
'      Call MsgBox("There was no good attitude found in this range", vbOKOnly)
'      m1 = 1
'      Exit Sub
'    End If
'  Loop Until CQV(i) And AttitudeOK(i)  'PitchOK And RollOK And fEcOK
'  m1 = i
' Get the rest within window
  i = m1
  Do: i = i + 1
    If CQV(i) And AttitudeOK(i) Then j = i
  Loop Until i = m2
  m2 = j '- NAVoffset  'commented out because if < zero can cause m2 to exceed number of records.
    
' Set Scales
  With picTB
  .Cls
  .ScaleMode = 0
  .ScaleTop = Val(cboTB2.Text)
  .ScaleLeft = m1
  If (Val(cboTB1.Text) - Val(cboTB2.Text)) = 0 Then Exit Sub
  .ScaleHeight = Val(cboTB1.Text) - Val(cboTB2.Text)
  .ScaleWidth = Val(m2 - m1)
  'If m1 = 1 Then m1 = 2

  TBtop = .ScaleTop
  TBleft = .ScaleLeft
  TBheight = .ScaleHeight
  TBwidth = .ScaleWidth
  End With
' Draw X tic marks
  x1 = m1
  x2 = m2
 Select Case cboUTunit(2).Text
  Case "Rec No"
    x3 = Val(cboX3(2).Text)
  Case "UTks"
    x3 = Val(cboX3(2).Text) * TBwidth / (Val(txtX2(2).Text) - Val(txtX1(2).Text))
  Case "Hour"
    x3 = Val(cboX3(2).Text) * TBwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  Case "hh:mm:ss"
    x3 = Val(cboX3(2).Text) * TBwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  End Select
  
  For i = x1 To x2 Step x3
    picTB.PSet (i, TBtop)
    picTB.Line -(i, TBtop + 0.05 * TBheight), QBColor(Black)
    picTB.PSet (i, TBtop + TBheight)
    picTB.Line -(i, TBtop + 0.95 * TBheight), QBColor(Black)
  Next i

' Draw Y tic marks
  For i = Val(cboTB1.Text) To Val(cboTB2.Text) Step Val(cboTB3.Text)
    picTB.PSet (TBleft, i)
    picTB.Line -(TBleft + 0.02 * TBwidth, i), QBColor(Black)
    picTB.PSet (TBleft + TBwidth, i)
    picTB.Line -(TBleft + 0.98 * TBwidth, i), QBColor(Black)
  Next i

  DoEvents
  For M = 1 To TotalCycles   'Calculate gain in case it's changed
    Call CalculateGain(M, g!())
    MV(36, M) = g(1)
    MV(37, M) = g(2)
    MV(38, M) = g(3)
  Next M

' Plot OATnav
'  OATnav = OATV(1, R)
'  OATmms = OATV(2, R)
'  OATmtp = OATV(3, R)
  LineC = &H0&    'black
  If chkShowChannels(4).Value = 1 Then  'Tnav
    picTB.PSet (m1, fGetNavT(m1, 0))
    For M = m1 + 1 To m2
      If M + NAVoffset > TotalCycles Then Exit For
      picTB.Line -(M, fGetNavT(M, 0)), LineC  'And continue from there
    Next M
    DoEvents
  End If
  
' Plot 1 Hz T data
  If chkShowChannels(7).Value = 1 Then
    UTsec = UV(1, m1)
    If UTptz > UTsec Then  ' UTptz etc read during frmFLTinfo load event
      MsgBox "UTptz > UTm1!", vbOKOnly
      Exit Sub
    End If
    LineC = &HC000&            'green
    picTB.PSet (m1, fGetNavT(m1, 0))
    For M = m1 + 1 To m2
      If M + aNAVoffset(0) > TotalCycles Then Exit For
      j1 = UV(1, M) - UTptz '- 1
      j2 = UV(1, M + 1) - UTptz '- 1
      For j = j1 To j2 - 1
        x = M + (j - j1) / (j2 - j1) - aNAVoffset(0)
        OAT = Tptz(j) + OATnavCOR    'NB OAT is declared in this sub
        'Debug.Print x; M; Tptz(j)
        picTB.Line -(x, OAT), LineC  'And continue from there
      Next j
    Next M
    DoEvents
  End If

' Plot Pitch and Roll Violations
  picTB.DrawWidth = 2
  If chkShowChannels(5).Value = 1 Then
    y = Val(cboTB1.Text) + Val(cboTB3.Text)
    picTB.PSet (m1, y)
    For M = m1 + 1 To m2
      If AttitudeOK(M) Then
        LineC = &HC000&      'green
      Else
        LineC = &HFF&        'red
      End If
      picTB.Line -(M, y), LineC  'And continue from there
    Next M
    DoEvents
  End If
  picTB.DrawWidth = 1

' Plot Pressure altitude
  If chkShowChannels(8).Value = 1 Then
    LineC = &HC00000     'd blue
    Tscale = Abs(picTB.ScaleHeight) / 22#
    y = Val(cboTB1.Text) + Tscale * ALTV(1, m1)
    picTB.PSet (m1, y)
    For M = m1 + 1 To m2
      y = Val(cboTB1.Text) + Tscale * ALTV(1, M)
      picTB.Line -(M, y), LineC  'And continue from there
    Next M
    DoEvents
  End If

' Calculate GE TB
' OATsource: 1=MTP, 2=A/C, 3=MMS
' GainScale: 1=EQN, 2=ND,  3=OAT
' REFsource: 1=TGT, 2=OAT
  Denom = 1 - Emissivity - Reflectivity
  NAVgainCount = 0
  LineC = QBColor(Red)
  Forward = True
  
' Plot individual channel TBs if enabled
  For i = 1 To Channels
    If chkShowChannels(i).Value = 1 Then
      Select Case i
      Case 1: LineC = &HFF&        'red
      Case 2: LineC = &HFFFFFF     'white
      Case 3: LineC = &HFF0000     'blue
      End Select
      For M = m1 To m2
        If (CQV(M) And AttitudeOK(M)) Or IgnoreBadThings Then
          GoodCounts = GoodCounts + 1
          Call CalculateGain(M, g())   'Calls ReadArrayLeadLag
          If REFsource = 1 Then        'Target is Reference
            Tref = Ttgt + cTo          '= Tbase from ReadArrayLeadLag
            If chkShowChannels(12).Value = 1 Then   'Instead of OAT use Cal Target at Loc 6 at 90 C
              Tmtp = cTo + Val(txtTtgt.Text)
            Else
              Tmtp = fTB(Tref, CS(i, LocHor), CB(i), g(i))
            End If
          Else                         'OAT is Reference
            Tref = fGetNavT(M, i)      'OAT = Tnav interpolated to TnavOffset
            If UseMAforCS Then Tmtp = Tref + (CSMA(i, LocHor) - CS(i, LocHor)) / g(i) Else Tmtp = Tref
          End If
          
          OATbest = (OATbest + Tmtp / 2) / 1.5
          If M = m1 Then
            picTB.PSet (m1, Tmtp)
          Else
            picTB.Line -(M, Tmtp), LineC 'And continue from there
            'Debug.Print Tmtp
          End If
          If 1 = 0 Then
            Call ReadArrayLeadLag(M)       'get Ttgt, Tmxr, Tnav with lead/lag
            'If chkshowchannel(11).Value = 1 Then
            
            'End If
            Call GainGE(Geqn(), M)         'GE need Tmxr with lag
            Call GainND(g())               'ND
            Call GainOAT(Geqn(), Go(), False)   'OAT
            For ii = 1 To 3
              Gr(ii) = (CSMA(ii, LocHor) - CB(ii)) / (MV(20, M) - Ttgt)
              Select Case ii
              Case 1
                Print #lu, M, Gr(ii), EV(ii, 14, M), CB(ii),
              Case 2
                Print #lu, Gr(ii), EV(ii, 14, M), CB(ii),
              Case 3
                Print #lu, Gr(ii), EV(ii, 14, M), CB(ii), MV(20, M), Ttgt
              End Select
            Next ii
            'Print #lu, M, Geqn(1), Geqn(2), Geqn(3), g(1), g(2), g(3), Go(1), Go(2), Go(3), Gr(1), Gr(2), Gr(3)
            
          End If

        End If
      Next M
    End If
  Next i
  
' Use NAV to integrate what MTP would see
  If chkShowChannels(6).Value = 1 Or chkShowChannels(10).Value = 1 Then
    UTsec = UV(1, m1)
    If UTptz > UTsec Then  'Tptz etc read during frmFLTinfo form Load event
      If chkShowChannels(10).Value = 1 Then
        MsgBox "Te Fit has been disabled because UTptz>UTm1!, vbokonly"
        chkShowChannels(10).Value = 0
      Else
        MsgBox "UTptz > UTm1!", vbOKOnly
      End If
      Exit Sub
    End If
    
    j = UV(1, M) - UTptz + 1
    Call IntegrateHorizonTB(j&, Texp!(), iMax)
    For i = 1 To 3
      Te(m1, i) = Texp(i)
    Next i
    Te(m1, 0) = Texp(3)
    For M = m1 + 1 To m2
      If M + NAVoffset > TotalCycles Then Exit For
      j = UV(1, M) - UTptz + 1
      If j > iPTZ Then Exit For
      Call IntegrateHorizonTB(j&, Texp!(), iMax)
      OAT = 0#
      For i = 1 To Channels
        Te(M, i) = Texp(i)
      Next i
      Te(M, 0) = Texp(0)
    Next M
    
    If chkShowChannels(6).Value = 1 Then
      LineC = &HFFFF&        'yellow
      picTB.PSet (m1, fGetNavT(m1, 0))
      For M = m1 + 1 To m2
        picTB.Line -(M, Te(M, 0)), LineC 'And continue from there
      Next M
    End If
    DoEvents
  End If
  
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Plot weighted average and do statistics
  T1Sum = 0#: T2Sum = 0#: GoodCounts = 0
  For i = 0 To 6: TcSum1(i) = 0#: TcSum2(i) = 0#: Next i
  OATbest = 0#: LineC = &HFF00FF   'pink
  NewPset = True
  n = 0
  For M = m1 To m2
    If (CQV(M) And AttitudeOK(M)) Or IgnoreBadThings Then
      If NewPset Then PsetM = M
      GoodCounts = GoodCounts + 1
      Call CalculateGain(M, g())
      ' Calculate MTP temperatures, T(i)
      For i = 1 To Channels
      ' If g(i) = 0 Then g(i) = 16
        If REFsource = 1 Then  'TGT
          Tref = Ttgt + cTo
          T(i) = fTB(Tref, CS(i, LocHor), CB(i), g(i))
        Else                   'OAT = 0
          Tref = fGetNavT(M, i)
          If UseMAforCS Then
            T(i) = Tref + (CSMA(i, LocHor) - CS(i, LocHor)) / g(i)
          Else
            T(i) = Tref
          End If
        End If
      Next i
      Tmtp = fTwt(T())
      If 1 = 0 Then
        Tref = fGetNavT(M, 1)
        Print #lu, M, Tmtp, Tref, Tmtp - Tref, ALTV(1, M), MV(2, M)
      End If
      If GainScale = 2 Then 'ND
        n = n + 1
        gnda(1) = gnda(1) + g(1)
        gnda(2) = gnda(2) + g(2)
        gnda(3) = gnda(3) + g(3)
      End If
      ' Calculate channel 1-3 statistics compared to Tnav or Texp
      For i = 1 To Channels
        If i = 2 And Abs(T(1) - T(2)) > 1.5 And chkDisableCh2.Value = 1 Then
          'x = x
          'Debug.Print M; T(1) - T(2)
'        Else
'          Tmtp = Tmtp + T(i) * ChInfo(i)
        End If
        If chkShowChannels(10).Value = 1 Then
          OAT = Te(M, i)  'fGetExpT(M, i) this function shifts Te series!
        Else
          OAT = fGetNavT(M, i)
        End If
        dT = OAT - T(i)
        TcSum1(i) = TcSum1(i) + dT
        TcSum2(i) = TcSum2(i) + dT ^ 2
      Next i
      
      TcSum1(7) = TcSum1(7) + Tmtp  'OAT
      TcSum2(7) = TcSum2(7) + Tmtp ^ 2 'OAT ^ 2
      
      If chkShowChannels(10).Value = 1 Then
        OAT = Te(M, 0)  'fGetExpT(M, 0)
      Else
        OAT = fGetNavT(M, 0)
      End If
      OATbest = (OATbest + Tmtp / 2) / 1.5
      dT = OAT - Tmtp
      Tmtp_Tnav(M) = -dT
      TcSum1(0) = TcSum1(0) + dT         'Sum time difference
      TcSum2(0) = TcSum2(0) + dT ^ 2     'Sum square of time differenc
      If Channels = 2 Then  ' Calculate RMS between MTP channels
        dT = T(1) - T(2)
        TcSum1(4) = TcSum1(4) + dT         'Sum time difference
        TcSum2(4) = TcSum2(4) + dT ^ 2     'Sum square of time differenc
      Else
        dT = T(1) - T(2)
        TcSum1(4) = TcSum1(4) + dT         'Sum time difference
        TcSum2(4) = TcSum2(4) + dT ^ 2     'Sum square of time differenc
        dT = T(1) - T(3)
        TcSum1(5) = TcSum1(5) + dT         'Sum time difference
        TcSum2(5) = TcSum2(5) + dT ^ 2     'Sum square of time differenc
        dT = T(2) - T(3)
        TcSum1(6) = TcSum1(6) + dT         'Sum time difference
        TcSum2(6) = TcSum2(6) + dT ^ 2     'Sum square of time difference
      End If
      If chkShowChannels(0).Value = 1 Then   'MTP weighted average T
        If NewPset Then  'M = m1 Then
          picTB.PSet (PsetM, Tmtp)
          NewPset = False
        Else
          picTB.Line -(M, Tmtp), LineC 'And continue from there
        End If
      End If
    Else
      NewPset = True
    End If
  Next M
  If n > 0 Then
    gnda(1) = gnda(1) / n: frmFLTINFO.txtGnd(0).Text = Format(gnda(1), "00.00")
    gnda(2) = gnda(2) / n: frmFLTINFO.txtGnd(1).Text = Format(gnda(2), "00.00")
    gnda(3) = gnda(3) / n: frmFLTINFO.txtGnd(2).Text = Format(gnda(3), "00.00")
  End If

' Write statistics: Channel 0 is Tmtp, weighted average of other channels
  If GoodCounts > 2 Then
    For i = 0 To 7
      DTavg = TcSum1(i) / GoodCounts          'Calculate average
      If (TcSum2(i) - TcSum1(i) ^ 2 / GoodCounts) >= 0 Then DTrms = Sqr((TcSum2(i) - TcSum1(i) ^ 2 / GoodCounts) / (GoodCounts - 1)) Else DTrms = 0# 'Calculate RMS
      TBfitRMS(i) = DTrms
      Select Case i
      Case Is < 5
        txtDTa(i).Text = Format(DTavg, "#0.00")
        txtDTb(i).Text = Format(DTrms, "#0.00")
        txtDTc(i).Text = Format(Sqr(DTavg ^ 2 + DTrms ^ 2), "#0.00")
      Case Else
        If Channels = 2 Then
          txtDTa(i).Text = ""
          txtDTb(i).Text = ""
          txtDTc(i).Text = ""
        Else
          txtDTa(i).Text = Format(DTavg, "#0.00")
          txtDTb(i).Text = Format(DTrms, "#0.00")
          txtDTc(i).Text = Format(Sqr(DTavg ^ 2 + DTrms ^ 2), "#0.00")
        End If
      End Select
    Next i
  Else
    MsgBox "There was only " + Format(GoodCounts, "###0") + " good scans!", vbOKOnly
  End If
  DoEvents
    
End Sub
Sub GainOAT(Geqn!(), Gnav!(), UseGE As Boolean)
Dim Qgnav%(1 To 3), Qgmms%(1 To 3), GoodNav%, OATg!, i%
Dim dTAi_OK As Boolean, dG!(1 To 3), NDo!(1 To 3), TGTmOATi!, dTGTmOATi!
Static TGTmOATlast!, dTGTmOAT!
      
  TGTmOATi = (Ttgt + cTo) - OAT

  If OATbest = 0 Then  'First time thru
    If OAT > 180 Then OATbest = OAT
    TGTmOATlast = TGTmOATi
    dTGTmOAT = 0
  End If

  dTGTmOATi = TGTmOATi - TGTmOATlast
  dTGTmOAT = (dTGTmOAT + 0.2 * dTGTmOATi) / 1.2
     
  If Abs(OATbest - OAT) < 15 Then OATg = OAT Else OATg = OATbest
  For i = 1 To Channels
    If UseMAforCSgain Then
      Gnav(i) = (CB(i) - CSMA(i, LocHor)) / (Tbase - (1 - Emissivity - Reflectivity) * OATg - Emissivity * (Twin + cTo) - Reflectivity * (Tifa + cTo))
    Else
      Gnav(i) = (CB(i) - CS(i, LocHor)) / (Tbase - (1 - Emissivity - Reflectivity) * OATg - Emissivity * (Twin + cTo) - Reflectivity * (Tifa + cTo))
    End If
    GnavGood(i) = True
'    If Gnav(i) < GnavMin(i) Or Gnav(i) > GnavMax(i) Then Gnav(i) = 99.99: GnavGood(1) = False  'was 5/9
  Next i
  
  For i = 1 To Channels
    If (Abs(Gnav(i) - Geqn(i))) / Geqn(i) < 0.3 Then Qgnav(i) = 1 Else Qgnav(i) = 0
  Next i
  If Abs(dTGTmOAT) > 0.25 Then Qgnav(1) = 0: Qgnav(2) = 0: Qgnav(3) = 0
  If TGTmOATi < 10 Then Qgnav(1) = 0: Qgnav(2) = 0: Qgnav(3) = 0
      
  GoodNav = 1
  For i = 1 To Channels: GoodNav = GoodNav * Qgnav(i): Next i
  If GoodNav Then
    For i = 1 To Channels: 'Gi(i) = Gnav(i):
      Goat(i) = Gnav(i)
    Next i
    NAVgainCount = NAVgainCount + 1
  Else
'    If UseGE Then
      For i = 1 To Channels  ' Gi(i) = Geqn(i):
        Goat(i) = 0#
      Next i 'Use gain eqn if DADS no good
      GEgainCount = GEgainCount + 1
'    End If
  End If
  TGTmOATlast = TGTmOATi

End Sub

Sub UpdatePC(Cycle%)
Dim PC%(0 To 8), i%

txtRecord.Text = Cycle%
'frmFLTINFO.txtLimitsUT(0) = UTsec
'frmFLTINFO.txtLimitsUT(1) = fSecToTstring$(UTsec, True)
  PC(0) = RetAltPC       'H0001 Retrieved Altitude
  PC(1) = TemperaturePC  'H0002 Temperature
  PC(2) = ZtOffPC        'H0004 (Trop-pALT) Offset
  PC(3) = TropAltPC      'H0008 Tropopause Altitude
  PC(4) = PitchPC        'H0010 Pitch Range
  PC(5) = RollPC         'H0020 Roll Range
  PC(6) = NlevPC         'H0040 Nlev
  PC(7) = RatePC         'H0080 DeltaP
  PC(8) = OATtropPC      'H0100 Number of scans which have substituted trops

If frmFLTINFO.ShowPC Then
  For i% = 0 To 8
    PC(i%) = Int(1000 * PC(i%) / Cycle%) / 10
    frmFLTINFO.txtPC(i%).Text = Format$(PC(i%), "##0.0#")
  Next i%
Else
  For i% = 0 To 8
    frmFLTINFO.txtPC(i%).Text = Format$(PC(i%), "###0")
  Next i%
End If

End Sub
Sub UpdateLimitSummary()
 Dim seconds&, hms$
 With frmSummary
  .Text1.Text = UTmin
  .Text2.Text = UTmax
  If UTmax < UTmin Then UTmax = UTmax + 86400
  seconds = UTmax - UTmin
  hms$ = fSecToTstring(seconds, True)
  .txtElapsedHHMMSS.Text = hms$
  .txtElapseds.Text = seconds
  .Text3.Text = Format(LATmin, "##0.000")
  .Text4.Text = Format(LATmax, "##0.000")
  .Text5.Text = Format(LONmin, "###0.000")
  .Text6.Text = Format(LONmax, "###0.000")
  .Text7.Text = Format(Pitchmin, "#00.0")
  .Text8.Text = Format(PitchMax, "#00.0")
  .Text9.Text = Format(Rollmin, "#00.0")
  .Text10.Text = Format(Rollmin, "#00.0")
  .Text11.Text = Format(pALTmin, "#0.00")
  .Text12.Text = Format(pALTmax, "#0.00")
  .Text13.Text = Format(Tacmin, "000.0")
  .Text14.Text = Format(Tacmax, "000.0")
  .Text15.Text = Format(Zt1min, "#0.00")
  .Text16.Text = Format(Zt1max, "#0.00")
  .Text17.Text = Format(PT1min, "000.0")
  .Text18.Text = Format(PT1max, "000.0")
  .Text19.Text = Format$(T1min, "###.#")
  .Text20.Text = Format$(T1max, "###.#")
  .Text21.Text = Format(Zt2min, "#0.00")
  .Text22.Text = Format(Zt2max, "#0.00")
  .Text23.Text = Format(PT2min, "000.0")
  .Text24.Text = Format(PT2max, "000.0")
  .Text25.Text = Format$(T2min, "###.#")
  .Text26.Text = Format$(T2max, "###.#")
  .Text27.Text = Format(LRmin, "##0.00")
  .Text28.Text = Format(LRmax, "##0.00")
  .Text29.Text = Format(Zmin, "#0.00")
  .Text30.Text = Format(Zmax, "#0.00")
  .Text31.Text = Format(Tmin, "000.0")
  .Text32.Text = Format(Tmax, "000.0")
  .Text33.Text = Format(zTmin, "#0.00")
  .Text34.Text = Format(ZtMax, "#0.00")
  .Text35.Text = Format$(PTmin, "###.#")
  .Text36.Text = Format$(PTmax, "###.#")
  .Text37.Text = Format$(MRImin, "#0.00")
  .Text38.Text = Format$(MRImax, "#0.00")
  .Text47.Text = fSecToTstring$(UTtakeoff, True)
  .Text39.Text = UTtakeoff
  If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
  .Text48.Text = fSecToTstring$(UTlanding, True)
  .Text40.Text = UTlanding
  .Text41.Text = fSecToTstring$(UTmin, True)
  .Text42.Text = fSecToTstring$(UTmax, True)
  .Text43.Text = Format(gALTmin, "#0.00")
  .Text44.Text = Format(gALTmax, "#0.00")
  seconds = UTlanding - UTtakeoff
  .Text45.Text = fSecToTstring(seconds, True)
  .Text46.Text = seconds
'  .Text50.Text = Progress
  DoEvents
 End With

End Sub
Sub CalculateArrayMAslow()
Dim save%, More As Boolean, MoreCount%, Diff!, c1%, c2%
Dim Cycle%, BadCyclesLast%, DiffMax!(1 To 3), DiffMaxLast!
Dim i%, j%, k%, L%, n2%, n3%, M%, r%, Nr%, Good As Boolean, GoodCount%, lu%
Static TtCNT%

  RawCycle = 0
  Cycle = 0
  RFIthreshold = 200       ' Threshold for second edit pass
  CMAcycles = CMAcycles  '15           ' Hardwire on first pass to get "tentpoles"
  n2 = Int(CMAcycles / 2)  ' eg. CMA=31, N2=15, N3=16; CMA must be ODD
  n3 = n2 + 1
  c1 = 11: c2 = 14         ' Non-sky counts (Base+dND, Base, dND, and Horizon)
  
' Fill CTS with CMAcycles of records
  For r = 1 To CMAcycles       'Get first CMAcycles Cycles
    If Left$(SU$, 3) = "CAN" Then
      MLC(r) = MV(21, r)         'Tmxr
      WLC(r) = MV(20, r)         'Twin
    Else
      MLC(r) = MV(4, r)         'Tmxr
      WLC(r) = MV(8, r)         'Twin
    End If
    TLC(r) = MV(0, r)         'Ttgt
    OLC(r) = OATV(1, r) - cTo 'Tnav
    For i = 1 To Channels
      For j = c1 To c2
        cTs(i, j, r) = CV(i, j, r)
      Next j
    Next i
  ' Debug.Print R; EV(1, 12, R)
  Next r

  For r = 1 To n3       'Average First n3 Cycles
    TTMA = 0#: TMMA = 0#: TWMA = 0#: TOMA = 0#
    For k = 1 To r
      TTMA = TTMA + TLC(k)
      TMMA = TMMA + MLC(k)
      TWMA = TWMA + WLC(k)
      TOMA = TOMA + OLC(k)
    Next k
    MV(49, r) = TTMA / r
    MV(50, r) = TMMA / r
    MV(51, r) = TWMA / r
    MV(25, r) = TOMA / r
    
    Nr = 2 * (r - 1) + 1
    For i = 1 To Channels
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To Nr
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / Nr + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
    CQVS(r) = True
  Next r

  TtCNT = 0:  BadCyclesLast = 0
  DiffMax(1) = 0: DiffMax(2) = 0: DiffMax(3) = 0
  For r = n3 + 1 To TotalCycles - n2 'Remaining TotalCycles-CMAcycles
' Shift Moving Average stack
    For k = 1 To CMAcycles
      If k = CMAcycles Then
        If Left$(SU$, 3) = "CAN" Then
          MLC(k) = MV(21, r + n2)          'Tmxr
          WLC(k) = MV(20, r + n2)          'Twin
        Else
          MLC(k) = MV(4, r + n2)          'Tmxr
          WLC(k) = MV(8, r + n2)          'Twin
        End If
        TLC(k) = MV(0, r + n2)          'Ttgt
        OLC(k) = OATV(1, r + n2) - cTo  'Tnav
      Else
        TLC(k) = TLC(k + 1)
        MLC(k) = MLC(k + 1)
        WLC(k) = WLC(k + 1)
        OLC(k) = OLC(k + 1)
      End If
    
      For i = 1 To Channels
        For j = c1 To c2
          If k = CMAcycles Then
            cTs(i, j, k) = CV(i, j, r + n2)
          Else
            cTs(i, j, k) = cTs(i, j, k + 1)
          End If
        Next j
      Next i
    Next k
  
'   Now calculate Moving Average
    TTMA = 0#: TMMA = 0#: TWMA = 0#: TOMA = 0#
    For k = 1 To CMAcycles
      TTMA = TTMA + TLC(k)
      TMMA = TMMA + MLC(k)
      TWMA = TWMA + WLC(k)
      TOMA = TOMA + OLC(k)
    Next k
    TTMA = TTMA / CMAcycles
    TMMA = TMMA / CMAcycles
    TWMA = TWMA / CMAcycles
    TOMA = TOMA / CMAcycles

'   Replace bad values with moving average
    If TLC(n3) - TTMA > MUXthreshold Then TLC(n3) = TTMA: TtCNT = TtCNT + 1
    If MLC(n3) - TMMA > MUXthreshold Then MLC(n3) = TMMA
    If WLC(n3) - TWMA > MUXthreshold Then WLC(n3) = TWMA
'    If OLC(n3) - TOMA > MUXthreshold Then OLC(n3) = TOMA
    MV(49, r) = TTMA  'Ttgt
    MV(50, r) = TMMA  'Tmxr
    MV(51, r) = TWMA  'Twin
    MV(25, r) = TOMA  'Tnav
    
    CQVS(r) = True     'Now do Base+dND, Base, dND, and Horizon
    For i = 1 To Channels
      CQM(i, r) = 0
      For j = c1 To c2
        mA(i, j) = 0
        For k = 1 To CMAcycles
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        EV(i, j, r) = Int(1# * mA(i, j) / CMAcycles + 0.5)  'Average
      
        Diff = CV(i, j, r) - EV(i, j, r)
        If chkAllowNegativeRFI.Value = 0 Then Diff = Abs(Diff)

        If Diff > RFIthreshold Then
          CQVS(r) = False                   '
          CQM(i, r) = CQM(i, r) Or 2 ^ (j - 1)
          If Diff > DiffMax(3) Then
            DiffMax(3) = Diff
            DiffMax(1) = i
            DiffMax(2) = j
          End If
        End If
        If Mission$ = "CAMEX4" Then    ' Fix Rapid T change in Humberto's Eye
          If r > 1123 And r - 2 < 1133 And YYYYMMDD$ = "20010923" Then CQVS(r) = True
        End If
      Next j
    Next i
'   Set CQV bad scan flag for (FAST) Sky Counts BAD when SLOW counts are BAD
'   Historically this is the way we have always operated. However, this means that ND spikes
'   make a scan bad when, if a MA is used, the calibration would be good since the ND MA would
'   be fine. Similarly, noisey Twin would not flag Sky Counts as bad.
    If chkUseCQVS.Value = 1 Then
      If Not CQVS(r) Then CQV(r) = False
    End If
    DiffMaxLast = DiffMax(3)
    If Not CQVS(r) Then BadCyclesLast = BadCyclesLast + 1: txtBadCounts.Text = Str(BadCyclesLast)
    txtRecord.Text = r
    ProgressBar1.Value = r
    DoEvents
  Next r

' Now do a pass with only good scans. Fill CTS with CMAcycles of records
  RFIiterations = 0
  CMAcycles = Val(frmFLTINFO.txtCMAcycles.Text)
  RFIthreshold = Val(frmFLTINFO.txtRFIthreshold.Text)  'Threshold for first edit pass
  MUXthreshold = Val(frmFLTINFO.txtMUXthreshold.Text)
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Repeat:
  RFIiterations = RFIiterations + 1
  BadCycles = 0
  For r = 1 To CMAcycles
    For i = 1 To Channels
      For j = c1 To c2
        cTs(i, j, r) = CV(i, j, r)
        mA(i, j) = 0
        For k = 1 To r
          mA(i, j) = mA(i, j) + cTs(i, j, k)
        Next k
        mA(i, j) = Int(1# * mA(i, j) / r + 0.5)
        EV(i, j, r) = mA(i, j)
      Next j
    Next i
  'Debug.Print R; EV(1, 12, R)
    If CQVS(r) Then flag(r) = 1 Else flag(r) = 0
  Next r

  For r = CMAcycles + 1 To TotalCycles - n2
'   Shift Moving Average stack
    For k = 1 To CMAcycles
      For i = 1 To Channels
        For j = c1 To c2
          If k = CMAcycles Then
            cTs(i, j, k) = CV(i, j, r + n2)
            If CQVS(r + n2) Then flag(k) = 1 Else flag(k) = 0
          Else
            cTs(i, j, k) = cTs(i, j, k + 1)
            flag(k) = flag(k + 1)
          End If
        Next j
      Next i
    Next k

'   Now calculate Moving Average
    CQVS(r) = True
    For i = 1 To Channels
    ''CQM(i, R) = 0
      For j = c1 To c2
        mA(i, j) = 0: GoodCount = 0
        For k = 1 To CMAcycles
          If flag(k) = 1 Then
            mA(i, j) = mA(i, j) + cTs(i, j, k)
            GoodCount = GoodCount + 1
          End If
        Next k
        If GoodCount > 0 Then
          EV(i, j, r) = Int(1# * mA(i, j) / GoodCount + 0.5)
        Else
          EV(i, j, r) = EV(i, j, r - 1)
        End If
        
        Diff = CV(i, j, r) - EV(i, j, r)
        If chkAllowNegativeRFI.Value = 0 Then Diff = Abs(Diff)
      
        If Diff > RFIthreshold Then 'Check again
          CQVS(r) = False
          CQM(i, r) = CQM(i, r) Or 2 ^ (j - 1)
          If Mission$ = "CAMEX4" Then    ' Fix Rapid T change in Humberto's Eye
            If r > 1123 And r - 2 < 1133 And YYYYMMDD$ = "20010923" Then CQVS(r) = True
          End If
        End If
      Next j
    Next i
'   Set CQV bad scan flag for (FAST) Sky Counts BAD when SLOW counts are BAD
'   Historically this is the way we have always operated. However, this means that ND spikes
'   make a scan bad when, if a MA is used, the calibration would be good since the ND MA would
'   be fine. Similarly, noisey Twin would not flag Sky Counts as bad.
    If chkUseCQVS.Value = 1 Then
      If Not CQVS(r) Then CQV(r) = False
    End If
  'Debug.Print CV(1, 12, R) - EV(1, 12, R)
    If Not CQVS(r) Then BadCycles = BadCycles + 1
    ProgressBar1.Value = r
    DoEvents
  Next r
  txtBadCounts.Text = Str(BadCycles)
  With frmFLTINFO
    .txtRFIiterations = RFIiterations
    .txtBadCycles = BadCycles
  End With
  If BadCycles > BadCyclesLast Then
    BadCyclesLast = BadCycles
    GoTo Repeat   'Loop again
  End If

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Fill in last N2 cycles with last value
  For r = TotalCycles - n2 + 1 To TotalCycles
    For i = 1 To Channels
      For j = c1 To c2
        EV(i, j, r) = EV(i, j, TotalCycles)
      Next j
    Next i
    CQVS(r) = True
  Next r

  frmFLTINFO.UpdateFLTINFO
  frmMTPbin.Caption = "MTP Data Analysis for " + AC$ + "/" + Mission$ + "/" + YYYYMMDD$
  frmFLTINFO.cmdWriteDefault_Click
  Close OUTlu

  BINopen BINfile$
  Forward = True
  For i = 1 To TotalCycles
    Call ReadArray(i - 1)  'i-1 because ReadArray INC/DECs record count depending on FORWARD
    If CQV(i) Then GoodScan = True Else GoodScan = False
  'Debug.Print i; GoodScan
    If NQV(i) Then MakeWord = 0 Else MakeWord = 1
'    Call BINwrite(BINlu, i)
  'If i < 100 Then Debug.Print i; MakeWord; GoodScan
  Next i
  GoTo NextCode
  lu = FreeFile
  Open "C:\MTP\Target.txt" For Output As lu
  For i = 1 To TotalCycles
    Call ReadArray(i - 1)  'i-1 because ReadArray INC/DECs record count depending on FORWARD
    Print #lu, i; Ttgtlo; Ttgthi
  Next i
  Close lu

NextCode:
  Close BINlu
  Call REFwriteHEADER(BINfile$)
  Call REFwriteHEADER("C:\MTP\Setup\FLTINFO.REF")

End Sub

Sub DoTropStatusBox()

If (frmATP.WindowState = 2 Or RealTime) And chkShowStatus.Value = 1 Then 'Check if Maximized
  With frmStatus
    If .optZx(0).Value = True Then  'Display in pressure altitude units
      ShowZp = True
      
'      .Label(0).Caption = "  Zp         Zp      Temp  Theta"
      .Visible = True
      .SetFocus
      .txtALTkft(0).Text = Format(pALT * ckft_km, "00.0")
      If Zt1 <> 99.9 Then
      .txtALTkft(1).Text = Format(Zt1 * ckft_km, "00.0")
      Else
      .txtALTkft(1).Text = "99.9"
      End If
      If Zt2 <> 99.9 Then
      .txtALTkft(2).Text = Format(Zt2 * ckft_km, "00.0")
      Else
      .txtALTkft(2).Text = "99.9"
      End If
      If Zmin5 <> 99.9 Then
      .txtALTkft(3).Text = Format(Zmin5 * ckft_km, "00.0")
      Else
      .txtALTkft(3).Text = "99.9"
      End If

      .txtALTkft(4).Text = Format(dZg * ckft_km, "0.00")
    
      .txtALTkm(0).Text = Format(pALT, "00.0")
      .txtALTkm(1).Text = Format(Zt1, "00.0")
      .txtALTkm(2).Text = Format(Zt2, "00.0")
      .txtALTkm(3).Text = Format(Zmin5, "00.0")
      .txtALTkm(4).Text = Format(dZg, "0.00")
    
      .txtTemp(0).Text = Format(OATnav, "000.0")
      .txtTemp(1).Text = Format(TT1, "000.0")
      .txtTemp(2).Text = Format(TT2, "000.0")
      .txtTemp(3).Text = Format(Tmin5, "000.0")
      .txtTheta(0).Text = Format(fTheta(OATnav, fZtoP(pALT)), "000.0")
      If Zt1 <> 99.9 Then
      .txtTheta(1).Text = Format(fTheta(TT1, fZtoP(Zt1)), "000.0")
      Else
      .txtTheta(1).Text = "999.9"
      End If

      If Zt2 <> 99.9 Then
      .txtTheta(2).Text = Format(fTheta(TT2, fZtoP(Zt2)), "000.0")
      Else
      .txtTheta(2).Text = "999.9"
      End If
    
      If Zmin5 <> 99.9 Then
      .txtTheta(3).Text = Format(fTheta(Tmin5, fZtoP(Zmin5)), "000.0")
      Else
      .txtTheta(3).Text = "999.9"
      End If
   
      .txtFraction = Format(RCwt, "0.00")
  
    Else
      ShowZp = False
      .label(0).Caption = "  Zg         Zg      Temp  Theta"
      .Visible = True
      .SetFocus
      .txtALTkft(0).Text = Format(gALT * ckft_km, "00.0")
      If Zt1 <> 99.9 Then
      .txtALTkft(1).Text = Format((Zt1 + dZg) * ckft_km, "00.0")
      Else
      .txtALTkft(1).Text = "99.9"
      End If
      If Zt2 <> 99.9 Then
      .txtALTkft(2).Text = Format((Zt2 + dZg) * ckft_km, "00.0")
      Else
      .txtALTkft(2).Text = "99.9"
      End If
      If Zmin5 <> 99.9 Then
      .txtALTkft(3).Text = Format((Zmin5 + dZg) * ckft_km, "00.0")
      Else
      .txtALTkft(3).Text = "99.9"
      End If

      .txtALTkft(4).Text = Format(dZg * ckft_km, "0.00")
    
      .txtALTkm(0).Text = Format(gALT, "00.0")
      .txtALTkm(1).Text = Format(Zt1 + dZg, "00.0")
      .txtALTkm(2).Text = Format(Zt2 + dZg, "00.0")
      .txtALTkm(3).Text = Format(Zmin5 + dZg, "00.0")
      .txtALTkm(4).Text = Format(dZg, "0.00")
    
      .txtTemp(0).Text = Format(OATnav, "000.0")
      .txtTemp(1).Text = Format(TT1, "000.0")
      .txtTemp(2).Text = Format(TT2, "000.0")
      .txtTemp(3).Text = Format(Tmin5, "000.0")
      .txtTheta(0).Text = Format(fTheta(OATnav, fZtoP(pALT)), "000.0")
      If Zt1 <> 99.9 Then
      .txtTheta(1).Text = Format(fTheta(TT1, fZtoP(Zt1)), "000.0")
      Else
      .txtTheta(1).Text = "999.9"
      End If

      If Zt2 <> 99.9 Then
      .txtTheta(2).Text = Format(fTheta(TT2, fZtoP(Zt2)), "000.0")
      Else
      .txtTheta(2).Text = "999.9"
      End If
    
      If Zmin5 <> 99.9 Then
      .txtTheta(3).Text = Format(fTheta(Tmin5, fZtoP(Zmin5)), "000.0")
      Else
      .txtTheta(3).Text = "999.9"
      End If
   
      .txtFraction = Format(RCwt, "0.00")
    End If
  End With

Else
  frmStatus.Visible = False
End If

End Sub


Sub ReadRAWrecord(Quit As Boolean)
Dim i%, j%, A$, b$, UTks!, x$, UT&, c1$, c2$
Dim GoToNext As Boolean, CycleTakeoff%
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, Lt$
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Th!, dALT!
Static UTsecLast&, Day, Change As Boolean, pALTftLast&, lu%, Poff!, UTsecMTPlast&, dtMTP_NAV&, ALTlast!
  
  If RawCycle = 0 Then UTsecLast = 0: UTsec = 0     'Make sure reset in case of multiple passes
  If Left$(SU$, 3) = "CAN" Then
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
  If chkEditTtgt.Value = 1 Then   'xxx mjm 20001023 Was AND not OR
  ' 20020515 mjm Ttgt corrupted after scan 30 on flight 20020514 on WB57
    Ttgt = fTtgtFix(EditTtgt, TtgtFix, Ttgt, RawCycle%)
    If TtgtFix Then TtgtPC = TtgtPC + 1: frmFLTINFO.txtPC(14) = TtgtPC
    Muxs(0) = Ttgt
  End If
  Aline = False
  If chkZgZp.Value = 1 Then
    If RawCycle = 1 Then
      lu = FreeFile
      Open Drive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\ZgZp" + YYYYMMDD$ + ".txt" For Output As lu
      Print #lu, " UTks Zp  Zg  Zg-Zp"
    Else
      Print #lu, UTsec / 1000; pALT; gALT; (gALT - pALT) * 1000
    End If
  End If

' Write current cycle to memory
  If UTsecLast > UTsec Then       'This happens when the day changes
    UTsec = UTsec + 86400
  End If
  UTsecLast = UTsec
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
  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  DoEvents
  If Len(A$) < 5 Then GoTo TryAgainGV
    
  Select Case Mid$(A$, 2, 1):
  Case " ", "n", "W", "0", "t"                  'Valid second characters
  Case Else: GoTo TryAgainGV
  End Select
  
ProcessNextAlineGV:
  'If all scan lines have not been read,
  If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then '
  ' Loop through here until all the necessary lines have been read for a scan
    GoSub ProcessInputLineGV
    If Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 0 Then GoTo TryAgainGV   '
  End If
Return

ProcessInputLineGV:
  Select Case Left$(A$, 1)
  Case "A"    'Process A-line using NEW data format.
    If Mid$(A$, 3, 2) = "99" Then Return
    'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
    'Un-comment a$ line above to simulate bad DADS data
    NavQual = True
    UTsecMTPlast = UTsecMTP          'Save in case UT time correction needed

    Call DecodeGVline(A$, Not FirstAline)           'Get UTsec from RAW file
    TTO = fTTO(pALT)       'Time Tag Offset
    UTsec = Int(UTsec + TTO + 0.5)
    pALT = ZpAvg                     'Use ZpAvg rather than IWG pressure altitude (Note: MapGVtoOther sets pALT=pALTI)
If UTsec > 86300 Then
'Debug.Print Cycle; UTsec; UTsecLast; A$
End If
    dALT = pALT - ALTlast            'Look for Takeoff
    If pALT > 0.006 And dALT > 0.015 And Not Takeoff And Cycle > 0 Then
      CycleTakeoff = Cycle
      Takeoff = True
      Landing = False
      UTtakeoff = UTsecLast
      pALTtakeoff = pALT
      hhmmssTO$ = hhmmssNavLast$
    End If
      
    If Takeoff And Not Landing Then
      If dALT < -0.015 Then     'pALT < 2 And Cycle > 1000 And
        'Landing = True
        UTlanding = UTsec
        If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
      End If
    End If
    ALTlast = pALT                   'So when IWG line is read pALT ends up equalling  pALTI
    
' Apply modelled correction to Znav if necessary
    If Pcorr <> 99.99 Then
      Select Case AC$
      Case "GV"
      End Select
    End If
    ALTkmft = pALT * cft_km
    pALTft = pALT * cft_km

      
'   At this point OATn and pALT are NAV values
    OAT = cTo + OATn + OATnavCOR
    If OAT > 999.9 Then OAT = 999.9
    Tac = Int(10 * OAT) / 10#          'Temperature at aircraft
    ALTkmLast = pALT
'    UTsecLast = UTsec
    
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
    Call DecodeGVline(A$, Not FirstAline)
  
  End Select

  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)

  If ShowRAW Then
    Select Case Lt$
    Case "A":  txtLine(0).Text = A$
    Case "B":  txtLine(1).Text = A$
    Case "C":  txtLine(2).Text = A$
    Case "D":  txtLine(3).Text = A$
    Case "E":  txtLine(4).Text = A$
    End Select
  End If
Return


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Process non-GV data
ReadABCDElines:
  If Cycle > 0 Then GoTo ProcessNextAline
TryAgain:
  If EOF(RAWlu) Then
    If chkZgZp.Value = 1 Then Close lu
    If Not RealTime Then Quit = True
    Exit Sub
  End If
' Get an input line from RAW File
  Input #RAWlu%, A$
  'Debug.Print A$
  
  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
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
      TTO = fTTO(pALT)       'Time Tag Offset
      UTsec = Int(UTsec + TTO + 0.5)

      dALT = pALT - ALTlast   'Look for Takeoff
      'Debug.Print Cycle; UTsec; pALT; dALT
      If pALT > 0.006 And dALT > 0.015 And Not Takeoff And Cycle > 0 Then
        CycleTakeoff = Cycle
        Takeoff = True
        Landing = False
        UTtakeoff = UTsecLast
        pALTtakeoff = pALT
        hhmmssTO$ = hhmmssNavLast$
      End If
      
      If Takeoff And Not Landing Then
          If pALT < 2 And Cycle > 1000 And dALT < -0.015 Then
            Landing = True
            UTlanding = UTsec
            If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
          End If
      End If
    End If
  
    If UTsecLast > UTsec Then
      UTsec = UTsec + 86400
    End If
' Apply modelled correction to Znav if necessary
    If Pcorr <> 99.99 Then
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
    If cboPTfile.ListIndex > 0 And chkUseSubstitutedRAWfile.Value = 0 Then
      Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, OATmms, param!(), PTWfile$, Quit)
      If Quit Then GoTo EndOfSub  'Quit if EOF
      mALT = fPtoZ(P)
      mALTft = mALT * cft_km
    End If
               
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
    
    If YYYYMMDD$ <> "20020716" And YYYYMMDD$ <> "20030115" Then 'MTP Clocks off by 12 hours
      If (Abs(UTsecNav - UTsecMTP) < 1000 And ElCorUsed = 1) Then NavQual = True Else NavQual = False
    End If
    LineCurr(1) = A$
    Aline% = True
'    Debug.Print NavQual; UTsecMTP
'   UpdateCommLine
    Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0

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
  If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'  txtBadCounts.Text = Str(BadCycle)
'  txtFileRecords.Text = Str(Cycle)
  UTsecLast = UTsec

  If ShowRAW Then
    Select Case Lt$
    Case "A":  txtLine(0).Text = A$
    Case "B":  txtLine(1).Text = A$
    Case "C":  txtLine(2).Text = A$
    Case "D":  txtLine(3).Text = A$
    Case "E":  txtLine(4).Text = A$
    End Select
  End If
Return

End Sub

Sub GainCalculation(r%)
Dim Qgnav%(1 To 3), Qgmms%(1 To 3), GoodNav%, GoodMMS%, i%
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3), TGTmOATi!, dTGTmOATi!
Static FP!(1 To 4), TGTmOATlast!, dTGTmOAT!, Wtg!, OATg!, BadGain%

' Do gain calculation for GAIN EQU, ND and DADS and then pick one to use
' Normally OAT:  GainScale%  '1=EQN, 2=ND,  3=OAT  (replaces CalSource$)
' Normally NAV:  OATsource%  '1=MTP, 2=NAV, 3=MMS  (OAT source if OAT calsource)
' Normally TGT:  REFsource%  '1=TGT, 2=OAT         (Reference source if OAT calsource)
' Gains from Multi-linear Regression on DC19990410
' G1          G2          G3
' -0.134255685  -0.085272429  0.040783428 14.73671718   -0.279103355  0.320803662 0.018203484 4.380520217   -0.249018496  0.34056574  0.011131299 2.41735786
' 0.073390013 0.15662167  0.04386726  4.066519699   0.073071627 0.155942202 0.043676951 4.048878023   0.070931822 0.151375644 0.04239793  3.930312053
' 0.241634829 0.269839731 #N/A        #N/A          0.200599467 0.268669092 #N/A        #N/A          0.123386934 0.260801478 #N/A        #N/A
' 86.34763455 813         #N/A        #N/A          68.00402707 813         #N/A        #N/A          38.14437693 813         #N/A        #N/A
' 18.86181539 59.19735957 #N/A        #N/A          14.72622062 58.68484498 #N/A        #N/A          7.78344527  55.29815501 #N/A        #N/A
'
' Cifa   Clo2  Clo1  Coffset  .........
' SEifa  SElo2 SElo1 SEoffset .........
' r2     SEy ..........................
' F      df ...........................
' ssreg  ssresid ......................
'
' OATsource: 1=MTP, 2=A/C, 3=MMS
' GainScale: 1=EQN, 2=ND,  3=OAT
' REFsource: 1=TGT, 2=OAT
'
' GainScale = 1 (Gain Equation) DEFAULT if others are bad
  
  If NewRetrieval Then BadGain = 0
    
    Call GainGE(Geqn!(), r)
    For i = 1 To Channels
      GeqnGood(i) = True
      If Geqn(i) < GeqnMin(i) And pALT > 5 Then
        Geqn(i) = GeqnMin(i): GeqnGood(i) = False: BadGain = BadGain + 1
        If Not RealTime And BadGain > 0 And BadGain < 4 Then MsgBox "WARNING: Gain Equation gain minimum was exceeded!", vbOKOnly
      End If
      If Geqn(i) > GeqnMax(i) And pALT > 5 Then
        Geqn(i) = GeqnMax(i): GeqnGood(i) = False: BadGain = BadGain + 1
        If Not RealTime And BadGain > 0 And BadGain < 4 Then MsgBox "WARNING: Gain Equation gain maximum was exceeded!", vbOKOnly
      End If
      If GeqnGood(i) Then Gi(i) = Geqn(i)
    Next i
    
'   First time thru; give running avg gains: g(1), g(2), g(3) gain equation value
    If g(1) = 0 Then g(1) = Geqn(1): g(2) = Geqn(2): g(3) = Geqn(3)
  
'   Gainscale = 2 (Noise Diode)
    Call GainND(Gnd())
    For i = 1 To Channels
      GndGood(i) = True
      If Gnd(i) < GndMin(i) And pALT > 5 Then
        Gnd(i) = GndMin(i): GndGood(i) = False: BadGain = BadGain + 1
        If Not RealTime And BadGain > 0 And BadGain < 4 Then MsgBox "WARNING: Noise Diode gain minimum was exceeded!", vbOKOnly
      End If
      If Gnd(i) > GndMax(i) And pALT > 5 Then
        Gnd(i) = GndMax(i): GndGood(i) = False: BadGain = BadGain + 1
        If Not RealTime And BadGain > 0 And BadGain < 4 Then MsgBox "WARNING: Noise Diode gain maximum was exceeded!", vbOKOnly
      End If
      If GndGood(i) And GainScale = 2 Then Gi(i) = Gnd(i)
    Next i

' GainScale = 3 (Outside Air Temperature)

  Select Case OATsource  'MTP, NAV (eg DADS) Or MMS (eg PTW or MMS)
  Case 1                 'MTP - based on archival data since ND T appears stable
                         'OATmtp = Ttgt + (Csky - Ctgt)/GainND
                         
  Case 2                 'NAV
    If CtsQual Then
      Call GainOAT(Geqn(), Gnav(), True)
      NAVgainCount = NAVgainCount + 1
    Else
      For i = 1 To Channels: Gnav(i) = Geqn(i): Goat(i) = 0#: Next i 'Use gain eqn if DADS no good
      GEgainCount = GEgainCount + 1
    End If

  Case 3 'MMS-derived OAT (from old ER2 code using MMS and PTW)
        
    If OATbest = 0 Then OATbest = OATmms
    If Abs(OATbest - OATmms) < 15 Then OATg = OATmms Else OATg = OATbest

    For i = 1 To Channels
      Gmms(i) = (CS(i, LocHor) - CB(i)) / (OATg - Ttgt) ' Ttgt was TB mjmmjm
      If Gmms(i) < GnavMin(i) Or Gmms(i) > GnavMax(i) Then Gmms(i) = 99.99
      Qgmms(i) = 0
      If Abs(Gmms(i) - Geqn(i)) < 2 * Geqn(i) Then Qgmms(i) = 1
    Next i
   
    GoodMMS = 1
    For i = 1 To Channels: GoodMMS = GoodMMS * Qgmms(i): Next i
  
    If GoodMMS Then
'      For i = 1 To Channels: Gi(i) = Gmms(i): Goat(i) = Gmms(i): GmmsGood(i) = True: Next i
    Else
      For i = 1 To Channels: Gmms(i) = Geqn(i): Goat(i) = 0#: GeqnGood(i) = True: Next i
    End If
  End Select

'''''''''''''''''''''''''
' Calculate Weighted-Average Gains
' g1i, g2i, g3i have instantaneous gain for GAIN EQN, DADS or ND
' g1,  g2,  g3  are weighted-average values used in calibrating data

'  If Abs(Altkmtrend) > 0.2 Then Wtg = 1 Else Wtg = 0.5
  Wtg = 0.1 'Problem with AltKmTrend if not starting from ground
  '19990727 BLG originally used weights of 1 and 1/5!
  'Only use OAT gains until gain eqn is available for flight, then
  'don't use gain averaging!
  'Debug.Print Altkmtrend
  Select Case GainScale
  Case 1  'No running average if using Gain Equation
    For i = 1 To Channels: g(i) = Geqn(i): dG(i) = 0#: Next i
  Case 2
    For i = 1 To Channels: g(i) = (g(i) + Gnd(i) * Wtg) / (1 + Wtg): dG(i) = g(i) - Geqn(i): Next i
  Case 3
    For i = 1 To Channels: g(i) = (g(i) + Gnav(i) * Wtg) / (1 + Wtg): dG(i) = g(i) - Geqn(i): Next i
  End Select
  
  If PrintDiagnostics And UTsec = Val(txtPrintUTsec) Then
    Print #TSTlu, "GAINS:"
    Print #TSTlu, "Gain Equation:"
    For i = 1 To Channels: Print #TSTlu, i; Geqn(i),: Next i
    Print #TSTlu, ""
    Print #TSTlu, "Gi:"
    For i = 1 To Channels: Print #TSTlu, i; Gi(i),: Next i
    Print #TSTlu, ""
  End If

End Sub

Sub ReadArray(r%)
Dim i%, j%, x!, n%
' Read a record stored in arrays. Used in MTP retrieval code

  n = Int(Val(cboAverageNcycles.Text) / 2)
' Average to minimize effect of mesoscale T fluctuations on profile shifting
' n=0     no averaging
' n=1     average 3 cycles
' n=2     average 5 cycles
'  If EOF(BINlu%) Then Exit Sub
  If Forward Then INC r Else DEC r
  If r <= 0 Then r = 1
  If r > TotalCycles Then r = TotalCycles
  
  CtsQual = CQV(r)
  NavQual = NQV(r)
  If CtsQual And NavQual Then GoodScan = True Else GoodScan = False
  Cycle = r
  If n > 0 And r > n And r < TotalCycles - n Then
    Select Case n
    Case 1  'Average 3 samples
      GoodScan = (CQV(r - 1) And NQV(r - 1) And GoodScan And CQV(r + 1) And NQV(r + 1)) / 3#
    Case 2  'Average 5 samples
      GoodScan = (CQV(r - 2) And NQV(r - 2) And CQV(r - 1) And NQV(r - 1) And GoodScan And CQV(r + 1) And NQV(r + 1) And CQV(r + 2) And NQV(r + 2)) / 5#
    End Select
  End If
  pALT = ALTV(1, r)
  gALT = ALTV(2, r)
  rALT = ALTV(3, r)
  mALT = ALTV(4, r)
  If n > 0 And r > n And r < TotalCycles - n Then
    Select Case n
    Case 1  'Average 3 samples
      pALT = (ALTV(1, r - 1) + pALT + ALTV(1, r + 1)) / 3#
      pALT = (ALTV(2, r - 1) + pALT + ALTV(2, r + 1)) / 3#
    Case 2  'Average 5 samples
      pALT = (ALTV(1, r - 2) + ALTV(1, r - 1) + pALT + ALTV(1, r + 1) + ALTV(1, r + 2)) / 5#
      gALT = (ALTV(2, r - 2) + ALTV(2, r - 1) + gALT + ALTV(2, r + 1) + ALTV(2, r + 2)) / 5#
    End Select
  End If
  
  TTO = 0 'fTTO(pALT)       'Time Tag Offset
  TTOV(0, r) = TTO
  TTOV(1, r) = TTO1
  TTOV(2, r) = TTO2
  TTOV(3, r) = TTO3
  UTsec = UV(1, r)
  UTsec = Int(UTsec + TTO + 0.5)

  UTsecMTP = UV(2, r)
  UTsecNav = UV(3, r)
  
  Pitch = NAVV(1, r)
  Roll = NAVV(2, r)
  Latitude = NAVV(3, r)
  Longitude = NAVV(4, r)
  Heading = NAVV(5, r)
  TAS = NAVV(6, r)
  Wspd = NAVV(7, r)
  Wdir = NAVV(8, r)
  Elcor = NAVV(9, r)
  ElCorUsed = NAVV(10, r)
  
  OATnav = OATV(1, r)
  OATmms = OATV(2, r)
  OATmtp = OATV(3, r)
  If n > 0 And r > n And r < TotalCycles - n Then
    Select Case n
    Case 1  'Average 3 samples
      OATnav = (OATV(1, r - 1) + OATnav + OATV(1, r + 1)) / 3#
      OATmms = (OATV(2, r - 1) + OATmms + OATV(2, r + 1)) / 3#
      OATmtp = (OATV(3, r - 1) + OATmtp + OATV(3, r + 1)) / 3#
    Case 2  'Average 5 samples
      OATnav = (OATV(1, r - 2) + OATV(1, r - 1) + OATnav + OATV(1, r + 1) + OATV(1, r + 2)) / 5#
      OATmms = (OATV(2, r - 2) + OATV(2, r - 1) + OATmms + OATV(2, r + 1) + OATV(2, r + 2)) / 5#
      OATmtp = (OATV(3, r - 2) + OATV(3, r - 1) + OATmtp + OATV(3, r + 1) + OATV(3, r + 2)) / 5#
      If OATmtp = 0 Then OATmtp = OATnav + OATnavCOR
      'xxx mjm 20060309
    End Select
  End If
  
  TTMA = MV(49, r)
  TMMA = MV(50, r)
  TWMA = MV(51, r)
  TOMA = MV(52, r)
  
  For i% = 1 To Channels
    RFImask(i) = CQM(i, r)
    For j% = 1 To 13
      C(i%, j%) = CV(i%, j%, r)
      CMA(i, j) = EV(i, j, r)
    Next j%
    If n > 0 And r > n And r < TotalCycles - n Then
      Select Case n
      Case 1  'Average 3 samples
        For j% = 1 To 13
          C(i%, j%) = (C(i, j) * 1# + CV(i%, j%, r - 1) + CV(i%, j%, r + 1)) / 3#
          CMA(i, j) = (CMA(i, j) * 1# + EV(i%, j%, r - 1) + EV(i%, j%, r + 1)) / 3#
        Next j%
      Case 2  'Average 5 samples
        For j% = 1 To 13
          C(i%, j%) = (CV(i%, j%, r - 2) * 1# + CV(i%, j%, r - 1) * 1# + C(i, j) * 1# + CV(i%, j%, r + 1) + CV(i%, j%, r + 2)) / 5#
          CMA(i, j) = (EV(i%, j%, r - 2) * 1# + EV(i%, j%, r - 1) * 1# + CMA(i, j) * 1# + EV(i%, j%, r + 1) + EV(i%, j%, r + 2)) / 5#
        Next j
      End Select
    End If
  Next i%
  
  Muxs(0) = MV(0, r)
  For i% = 1 To 24
    Muxs(i%) = MV(i, r)
    Mux(i%) = MUXV(i, r)
  Next i%
  If Left$(SU$, 3) = "CAN" Then
    Ttgt = MV(0, r)           'Either tgt lo or tgt hi
    Vm08 = MV(1, r)
    Vvid = MV(2, r)
    Vp08 = MV(3, r)
    Vmtr = MV(4, r)
    Vsyn = MV(5, r)
    Vp15 = MV(6, r)
    Vp05 = MV(7, r)
    Vm15 = MV(8, r)
    ACCp = MV(9, r)
    Tdat = MV(10, r)
    Tmtr = MV(11, r)
    Tair = MV(12, r)   '11 on DC8
    Tsmp = MV(13, r)   '12 on DC8
    Tpsp = MV(14, r)
    Tnc = MV(15, r)
    Tsyn = MV(16, r)
    R350 = MV(17, r)
    Ttg1 = MV(18, r)
    Ttg2 = MV(19, r)
    Twin = MV(20, r)
    Tmix = MV(21, r)
    Tamp = MV(22, r)
    Tnd = MV(23, r)
    R600 = MV(24, r)
  
  Else
    Ttgt = MV(0, r)           'Either tgt lo or tgt hi
    Tnd = MV(1, r)
    Tlo1 = MV(2, r)
    Tlo2 = MV(3, r)
    Tifa = MV(4, r)
    Ttgtlo = MV(5, r)
    Ttgthi = MV(6, r)
    ACCp = MV(7, r)
    Twin = MV(8, r)
    Tmtr = MV(9, r)
    'asterisk = MV(10)
    Vref = MV(11, r)
    Tdc1 = MV(12, r)   '11 on DC8
    Tdc2 = MV(13, r)   '12 on DC8
    Vps5 = MV(14, r)
    Vps12 = MV(15, r)
    ACCm = MV(16, r)
  End If
  dACC = ACCp - ACCm             'Peak positive acceleration minus peak negative acceleration
  Tac = OATmtp
  
  UpdateCommLine
  UpdateRT
  'txtTTO.Text = Format(TTO, "00.0")
  If frmEditBits.Visible Then
    MakeEditWord = Ew(r) * 2 ^ 8 + MW(r)
    For i% = 0 To 16
      If MakeEditWord And 2 ^ i% Then frmEditBits.chkWords(i%).Value = 1 Else frmEditBits.chkWords(i%).Value = 0
    Next i%
  End If
  
'  Select Case frmMTPbin.cboUT.Text
'  Case "s": frmMTPbin.txtUT(2).Text = Format(UTsec, "#####0")
'  Case "ks": frmMTPbin.txtUT(2).Text = Format(UTsec / 1000, "##0.000")
'  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UTsec, True)
'  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UTsec / 3600#, "#0.0000")
'  End Select
  txtRecord.Text = Str(RawCycle)
  txtFileRecords.Text = Str(TotalCycles)
  If TotalCycles > 0 Then ProgressBar1.Value = r
  UpdateGoodscanBox (GoodScan)
  DoEvents
End Sub

Sub GetBINcycle(Cycle%)
Dim Zt!, Tt!, Th!

'Read new profile
GetaCycle:
  If EOF(BINlu%) Then Exit Sub 'XXXX
  If Forward Then INC Cycle% Else DEC Cycle%
  'Debug.Print Forward; Record%
  
  If Cycle% < 1 Then
    Cycle% = 1
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdDirection(1).Caption = "&Continue"
  End If
  
  If Cycle% > TotalCycles Then
    Cycle% = TotalCycles
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdDirection(1).Caption = "&Continue"
  End If
  Call BINread(BINlu%, Cycle%)
  
  If Cycle% > TotalCycles Then
    EndRecord% = Cycle%            'Track largest record
    If Not GoodScan Then INC BadNr%      'Increment Bad record count if new
  End If
  'Debug.Print GoodScan; GoodTrop
  
'  If SkipBadScan And Not GoodScan Then
'    If Not Forward Then Cycle% = Cycle% - 2
'    txtRecord.Text = Str$(Cycle%)
'    GoTo GetaCycle
'  End If
  
'  If SkipBadTrop And Not GoodTrop Then
'    If Not Forward Then Cycle% = Cycle% - 2
'    txtRecord.Text = Str$(Cycle%)
'    GoTo GetaCycle
'  End If
  
'  If (Not GoodScan And StopOnBadScan) Or (Not GoodTrop And StopOnBadTrop) Then
'    tmrGetCycle.Enabled = False
'    cmdStop.Caption = "&Continue"
'  End If
  
  UpdateForm
  
End Sub

Sub ReadRAWfile()
Dim Mux%(0 To 16), TXTlu%, txtFile$, TTTlu%, TTTfile$
Dim i%, j%, A$, UTks!, x$
Dim PTfile$, Navg%, n2%, n3%, GoToNext As Boolean, Zgh!, UT&
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3), ByteCount1&
Dim f As Form, DefCalfile$, ans As Variant, Emax!, fn$
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Th!, INIlu%, Ext$

Static FP!(1 To 4)

  GoSub Initialize
  GoSub SyncToExternalData
  Takeoff = False
  fEcCount = 0
  For i = 1 To 6: LineLast(i) = LineCurr(i): Next i
' Start reading RAW records
  Do
    ReadRAWrecord (Quit)  'Quit is TRUE if EOF and NOT Realtime
  ' Calculate maximum accessible elevation angle and check if it is exceeded
    Emax = fEmax(Pitch, Roll, EmaxDegrees)
    If Emax < EmaxDegrees Then
      fEcCount = fEcCount + 1            'Count number elevation angle exceedences
     ' Debug.Print Pitch; Roll; Emax
    End If
  Loop Until EOF(RAWlu) And Not RealTime

EndOfSub:
  Close RAWlu  'Close RAW file
  
  With frmFLTINFO
    .txtUTmin.Text = Str(UTmin)
    .txtUTmax.Text = Str(UTmax)
    .txtUTtakeoff.Text = Str(UTtakeoff)
    .txtUTlanding.Text = Str(UTlanding)
    .txtUTksmin = ""  'clear UT range used
    .txtUTksmax = ""
  End With
  RAWstamp = Mid$(yymmdd$, 3, 2) + "/" + Right$(yymmdd$, 2) + "/" + Left$(yymmdd$, 2)
  frmFLTINFO.UpdateForm
  UpdateFLTINFO
  REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"  'Header written to default and BIN file
  TotalCycles = RawCycle
  frmFLTINFO.txtScans(0).Text = TotalCycles
  frmFLTINFO.txtScans(1).Text = TotalCycles
  frmFLTINFO.txtTotalCycles = TotalCycles
  ProgressBar1.Min = 0
  If TotalCycles = 0 Then TotalCycles = 1000
  ProgressBar1.Max = TotalCycles
  ScanTime = fScanLength(1, TotalCycles)    '(UTmax - UTmin) / TotalCycles
  If chkZgZp.Value = 1 Then Exit Sub
  
  If chkFixTO.Value = 0 Then  'Fix Take Off?
    CalculateArrayMAfast
    CalculateArrayMAslow
  Else
    
  End If
Exit Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
SyncToExternalData:
' Open external PT data file if needed
  PText$ = fACext(AC$)  'just in case it's needed somewhere
  If cboPTfile.Text <> "NO" And chkUseSubstitutedRAWfile.Value = 0 Then
    PTfile$ = fFullFileName(Trim(cboPTfile.Text))
'   Synchronize all files (assume that RAW file begins before PT files
    Do
NextAline:
      Line Input #RAWlu, A$
'     Debug.Print a$
      If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then
        Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
        TTO = fTTO(pALT)
        UTsec = Int(UTsec + TTO + 0.5)
'       Print #TXTlu, UTsec, vbTab, Latitude, vbTab, Longitude, vbTab, pALT, vbTab, gALT, vbTab, gALT - pALT, vbTab, OATnav
        Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
'       If PTfile$ = "" Then GoTo EndOfSub  'Set to NULL string if EOF
        If UT > UTsec Then  'MMS/PTW ahead of RAW
          Do
            Line Input #RAWlu, A$
            If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then
              Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
              TTO = fTTO(pALT)
              UTsec = Int(UTsec + TTO + 0.5)
'             Print #TXTlu, UTsec / 1000, vbTab, Latitude, vbTab, Longitude, vbTab, pALT, vbTab, gALT, vbTab, gALT - pALT, vbTab, OATnav
            End If
          Loop Until UTsec >= UT
          If UTsec > UT Then  'Make equal
            Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
          End If
        End If
      Else
        GoTo NextAline
      End If
    Loop Until UT <= UTsec
  End If
  Return

Initialize:
  TtgtPC = 0
  RawCycle = 0
  GoodCycle = 0
  FirstAline = False
  Cycle = 0
  UTsec = 0
  UTsecLast = 0
  Navg = Val(txtTavg.Text)      'Number of temperatures to average
' Handle aircraft dependent parameters
  frmATP.FooterType = 0
  Ceiling = 24#             'ER2 Service ceiling
  CycleTime = 15            'Time for one MTP cycle (s)
  Select Case AC$
  Case "ER", "WB"
  Case "NG", "HA"
    CycleTime = 18
  Case "DC"
    Ceiling = 15              'DC8 service ceiling
    Remote = True             'Allow remote control commands
    CAC = True                'Chassis Analysis Computer in use?
    If Val(YYYYMMDD$) < 19960325 Then RHS = False: LocHor% = 5
  Case "M5"
    LocHor = 5
  End Select
  
  DoEvents
  Set f = frmFLTINFO
  
  Call DirCheck(MNpath$, Trim(f.cboFltDates.Text))
  
  If chkUseSubstitutedRAWfile.Value = 0 Then RAWextension$ = "RAW" Else RAWextension$ = "R" + Trim(cboPTfile.Text)
  If RAWextension$ = "RAW" Then
    RAWfile$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".RAW" ' + RAWextension$
  Else
    RAWfile$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + "." + RAWextension$
  End If
  
' Get total number of cycles in RAW file and redimension arrays
  TotalCycles = fRAWscans(RAWfile$)
  ReDim MV!(0 To MVchannels, 1 To TotalCycles), CV%(1 To Channels, 1 To 14, 1 To TotalCycles)
  ReDim EV%(1 To Channels, 1 To 14, 1 To TotalCycles)
  ReDim GV!(0 To GVchannels, 1 To Channels, 1 To TotalCycles)
  ReDim UV&(1 To 4, 1 To TotalCycles), NAVV!(1 To 10, 1 To TotalCycles), OATV!(1 To 10, 1 To TotalCycles)
  ReDim ALTV!(1 To 4, 1 To TotalCycles), MUXV%(1 To 24, 1 To TotalCycles)
  ReDim CQV(1 To TotalCycles) As Boolean, CQM%(1 To Channels, 1 To TotalCycles)
  ReDim NQV(1 To TotalCycles) As Boolean, CQVS(1 To TotalCycles) As Boolean
  ReDim Tmtp_Tnav(1 To TotalCycles), TTOV!(0 To 3, 1 To TotalCycles)
  ReDim AttitudeOK(1 To TotalCycles) As Boolean

' Open RAW or RXX file
  RAWlu = FreeFile
  Open RAWfile$ For Input As RAWlu
  LOFraw = LOF(RAWlu%)
  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
  Record = 0
  FirstAline = True

' Deal with Calfile
  DefCalfile$ = MNpath$ + "Setup\" + Mission$ + "_CAL.txt"
'  If Source$ = "F" Then
    If RealTime Then
      CALfile$ = DefCalfile$
    Else
      CALfile$ = Path$ + AC$ + YYYYMMDD$ + ".CAL"
    End If
'  Else
'    CALfile$ = DefCalfile$
'  End If
  
  Call CheckLimitsInit   'Reset Limit Information
  
  If Left$(SU$, 3) = "CAN" Then Ext$ = ".MTPH" Else Ext$ = ".SUI"
  fn$ = Dir(Path$ + AC$ + YYYYMMDD$ + Ext$, vbNormal)
  If fn$ = "" Then
    MTPyaw = 0
    MTPpitch = 0
    MTProll = 0
    MsgBox "Warning: " + Ext$ + " file does not exist for this campaign!", vbOKOnly
  Else
    If Left$(SU$, 3) = "CAN" Then
      ReadMTPH Path$ + fn$
    Else
      INIlu = FreeFile
      Open Path$ + fn$ For Input As INIlu
      Do
        Line Input #INIlu, A$
      Loop Until Left$(A$, 12) = "Frequencies:"
      'f(1)= 56.655
      '123456789012
      Line Input #INIlu, A$
      LOSUI(1) = Mid$(A$, 6, 7)
      Line Input #INIlu, A$
      LOSUI(2) = Mid$(A$, 6, 7)
      Line Input #INIlu, A$
      LOSUI(3) = Mid$(A$, 6, 7)
      If LO(1) <> LOSUI(1) Or LO(2) <> LOSUI(2) Or LO(3) <> LOSUI(3) Then
        MsgBox "RC LO frequencies are different from measurement frequencies in SUI file!", vbOKOnly
      End If
  
      Do
        Line Input #INIlu, A$
      Loop Until Left$(A$, 16) = "' getMAM offsets"
      Line Input #INIlu, A$
      i = InStr(1, A$, ",")
      MTPyaw = Val(Left$(A$, i - 1))
      j = InStr(i + 1, A$, ",")
      MTPpitch = Val(Mid$(A$, i + 1, j - i - 1))
      MTProll = Val(Mid$(A$, j + 1, Len(A$) - j - 1))
    
      Do
        Line Input #INIlu, A$
      Loop Until Left$(A$, 12) = "ElAngle  0 ="
      MTPfiduciary = Val(Mid$(A$, 13, 8))
      EmaxDegrees = 0#
      For i = 1 To 10
        Line Input #INIlu, A$
        ElSUI(i) = Val(Mid$(A$, 13, 8))
        If Abs(ElSUI(i)) > EmaxDegrees Then EmaxDegrees = ElSUI(i)
      Next i
      Close INIlu
    End If
  End If

' Calculate MTP Attitude Matrix with respect to INS platform
  Call Get_MTP_Attitude_Matrix(MAM!(), MTPyaw, MTPpitch, MTProll)
  Return
  
End Sub

Sub OpenRAWfile(RAWfile$)
Dim A$, GoToNext As Boolean, dALT!, ALTlast!
  
  dALT = 0#
  RAWlu% = FreeFile
  Open RAWfile$ For Input As RAWlu%
  
  If AC$ = "ER" Then
    Do
NextAline:
      Line Input #RAWlu, A$
      If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then
        ALTlast = pALT
        Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
        dALT = pALT - ALTlast
        If pALT > 0.006 And dALT > 0.015 And Not Takeoff Then
          Takeoff = True
          UTtakeoff = UTsec
          pALTtakeoff = pALT
        End If
        If Takeoff Then
          If pALT < 2 * pALTtakeoff And dALT > -0.015 Then UTlanding = UTsec
        End If
            
'       Select Case Trim(cboPTfile.Text)
'       Case "MM": Call GetMMSvalues(UTsec, UT, P, T, Th, U, v, W, PTfile$, Quit)
'       Case "PT": Call GetPTWvalues(UTsec&, UT, P!, T!, PTfile$)
'       Case "FT": Call GetFTvalues(UTsec&, UT, P!, T!,mLatitude, mLongitude, mZg!, mZp!, mWspd!, mWdir!, PTfile$)
'       Case "IN": Call GetINUvalues(UTsec&, UT&, Pinu, PA1, Pim, PA2, PFfile$)
'       End Select
'       If PTfile$ = "" Then GoTo EndOfSub  'Set to NULL string if EOF
      Else
        GoTo NextAline
      End If
    Loop Until EOF(RAWlu)
  End If

End Sub


Sub ProgramConstants()
  
  DTavg = 0#
  DTrms = 0#
  GoodCycle = 0
  BadCycle = 0
  Ccycle = 0                           'Counts Quality Count

' Initialize Logical Units (lu)
  INPlu% = 0: OUTlu% = 0:  MMSlu% = 0: REFlu% = 0
 
' Then Keyboard Commands and System Flags                      Change Command
                             'Auto-step thru data                      Ctrl+A
                             'Single-step thru data                    Ctrl+S
                             'Fast step thru data without display      Ctrl+F
  Risky = True               'Character (FALSE) or Line (TRUE)         Ctrl+X
  Risky = False              '960511 change/removed 970926 mjm
                             'Error on Line input halt QBs
                             'if full line not recv'd
  TtgtFix = False            'Fix spikes in Ttgt
  
  Err53count% = 0            'File Not Found
  Err57count% = 0            'Device I/O error count
  Errxxcount% = 0            'All other errors

  Tzgm = 999.9               'Global (for experiment) minimum temperature
  Ztgm = 99.9                'Altitude of Tzgm
  Tmin = 999.9               'Current scan minimum
  Zmin = 99.9                'Altitude of Tmin
  
  If NRC = 0 Then NRC = 3
  'xxx mjm
  'RAWstamp = Mid$(yymmdd$, 3, 2) + "/" + Right$(yymmdd$, 2) + "/" + Left$(yymmdd$, 2)
  If CalSource$ = "MMS" Then MMSstamp = FileDateTime(MMSfile$)
  Nret = 33                            'Number of retrieved levels
  Nel = 10                             'Number of elevation angles
  Nobs = Channels * (Nel - 1) + 1      'Number of observables
  'InitializeGainLimits                 'Min and Max Gain values
  vWB$ = "20040928.0"                  'Current version of program
  RollThreshold = 5                     'Roll warning criterion
  RollThresholdFlag = False                  '
  frmFLTINFO.UpdateFLTINFO

End Sub



Sub TBcalculation()
Dim dTAi_OK As Boolean, dTAi!(1 To 3)
Dim xx!, i%, j%, k%, num!, Denom!, Sum!
Static dTAav!(1 To 3)

' Calculate TBs using radiometric OATs and DADS
' Map for Indexing observables for DC8
' Scan  Ch  Cij RHS=6
'          TAij  OB OBa OBb
' 01     1   11  10   6              'Zenith if LocHor=6 since SUCCESS
'        2   21  20  12
'        3   31  30  18
' 02     1   12   9   5
'        2   22  19  11
'        3   32  29  17
' 03     1   13   8   4
'        2   23  18  10
'        3   33  28  16
' 04     1   14   7   3
'        2   24  17   9
'        3   34  27  15
' 05     1   15   6   2   5
'        2   25  16   8  10
'        3   35  26  14  15
' 06     1   16   5                  'Horizon LocHor=6 since SUCCESS
'        2   26  15
'        3   36  25
' 07     1   17   4   1   4
'        2   27  14   7   9
'        3   37  24  13  14
' 08     1   18   3       3
'        2   28  13       8
'        3   38  23      13
' 09     1   19   2       2
'        2   29  12       7
'        3   39  22      12
' 10     1   17   1       1
'        2   27  11       6
'        3   37  21      11
'                31              'pALT
'                32  19  16    'OATbest= f(OATmtp , OAT)

  xx = 1 - Emissivity - Reflectivity
' OATsource: 1=MTP, 2=A/C, 3=MMS
' GainScale: 1=EQN, 2=ND,  3=OAT
' REFsource: 1=TGT, 2=OAT
' NB OAT is calculated in ReadArrayLeadLag - this is where OATsource is applied
'    It uses aNAVoffset instead of TTO
'    OATmtp is calculated below using ChInfo
  If REFsource = 2 Then  'Use corrected OAT as Reference
    For j = 1 To 10
      For i = 1 To Channels
        TA(i, j) = OAT + (C(i, j) - C(i, LocHor)) / g(i)
      Next i
    Next j
  Else                   'Use TGT as Reference and MTP, Eqn or ND for gain
    For j = 1 To 10
      For i = 1 To Channels
'        If j = LocHor And i = 1 Then
'          TA(i, j) = OAT
'        Else
          TA(i, j) = fTB(Tbase, C(i, j), CB(i), g(i))
'        End If
      Next i
    Next j               'TA1&2 set, [K]
  End If
      
  If PrintDiagnostics And UTsec = Val(txtPrintUTsec.Text) Then
    Print #TSTlu, "Brightness Temperatures:"
    For j = 1 To 10
      Print #TSTlu, j,
      For i = 1 To Channels: Print #TSTlu, TA(i, j),: Next i
      Print #TSTlu, ""
    Next j              'TA1&2 set, [K]
  End If
  
' Apply Window Corrections to Measured TBs
  If frmFLTINFO.chkEnableWCT.Value = 1 Then
    
    If frmFLTINFO.chkUseZpDependence.Value = 1 Then
      For i = 1 To Channels
        For j = 1 To 10
          TA(i, j) = TA(i, j) + WINcor(i, j) * Exp(-(17.7 - pALT) / 2) 'see \AVE\TBelev_70.xls
        Next j
      Next i
    Else
      For i = 1 To Channels
        For j = 1 To 10
          TA(i, j) = TA(i, j) + WINcor(i, j)   'ADD window correction table
        Next j
      Next i
    
    End If
  
  End If
  
' If Counts Quality is GOOD
  If CtsQual Then
    num = 0#: Denom = 0#  'did have LocHor instead of 6. Assume sorted so 6=sky position
    For i = 1 To Channels
      num = num + ChInfo(i) * TA(i, LocHor)
      Denom = Denom + ChInfo(i)
    Next i
    OATmtp = num / Denom
    If chkCh2Only.Value = 1 Then OATmtp = TA(2, LocHor)
    Tac = OATmtp
    'Debug.Print "TBcalculation: OAT="; OAT; " OATmtp="; OATmtp; " TB(1,6)="; TA(1, LocHor); " TB(2,6)="; TA(2, LocHor); " TB(3,6)="; TA(3, LocHor)
    If PrintDiagnostics And UTsec = Val(txtPrintUTsec.Text) Then
      Print #TSTlu, "Window Corrections:"
      For j = 1 To 10
        Print #TSTlu, j,
        For i = 1 To Channels: Print #TSTlu, WINcor(i, j),: Next i
        Print #TSTlu, ""
      Next j              'TA1&2 set, [K]
    End If

' Apply Transparency Correction - Deleted this code
' Was used for applicable altitude retrieval, not needed for statistical retrieval!

' Inter-channel cal at horizon only - should consider removing this code
    For i = 1 To Channels
      dTAi(i) = OATmtp - TA(i, LocHor)  'OATmtp is ChInfo weighted horizon TB
      dTAav(i) = (2 * dTAav(i) + dTAi(i)) / 3
    Next i
  
    dTAi_OK = True
    For i = 1 To Channels
      If Abs(dTAi(i)) < 10 Then dTAi_OK = dTAi_OK * True Else dTAi_OK = dTAi_OK * False
    Next i
  
    If dTAi_OK Then   ' force inter-channel calibration
      For j = 1 To 10
        For i = 1 To Channels: TA(i, j) = TA(i, j) + dTAav(i): Next i
      Next j
    End If
  
  End If

' Check TA Quality
  Sum = 0
  For j = 2 To 9
    For i = 1 To Channels
      Sum = Sum + Abs(TA(i, j) - (TA(i, j - 1) + TA(i, j + 1)) / 2)
    Next i
  Next j
  If Sum > 75 Then TAqual = False Else TAqual = True '   950510, 950515
  
' Check OAT quality
  If OATbest = 0 And TAqual And CtsQual Then OATbest = OATmtp Else OATbest = OAT
  If TAqual And CtsQual Then OATbest = (OATbest + OATmtp / 2) / 1.5

' The following routine was developed in MTPsim to map TAs to OBs (verified for 3 channels, LocHor=6)
'  Call MapTAtoOB(TA!(), ob!(), Channels%, Nel%, LocHor%, ChInfo!())  'Used in Sim code
'  ob(0) = pALT    'pALT
'
'  If ShowDiagnostics Then UpdateTBwindow

End Sub

Sub UpdateCommLine()
Dim VV!(1 To 12), x$
' ####0 ##.0 #0.0 S###.### S###.### S###.### S###.# S###.# S###### S###### S###.# S###.# S###.# "
' UTsec pALT gALT Lat_deg Lon_deg Hdg Pitch  Roll OAT_K
  VV(1) = UTsec: VV(2) = pALT: VV(3) = gALT: VV(4) = Latitude: VV(5) = Longitude: VV(6) = Heading
  VV(7) = Pitch: VV(8) = Roll: VV(9) = OATnav
'                           HHMMSS UTsec pALT gALT    Lat     Lon Hdg Pitch  Roll OAT_K
  x$ = fUsing("#####0 #0.0 #0.0 ##0.00 ###0.00 ##0 ##0.0 ##0.0 #00.0", VV())
  txtCommLine.Text = x$
End Sub

Sub UpdateGoodscanBox(GoodScan As Boolean)
  
  If GoodScan Then
    GoodCycle = GoodCycle + 1
    txtGoodScan.BackColor = &H8000&
  Else
    txtGoodScan.BackColor = &HFF&
    BadCycle = BadCycle + 1
  End If

  If GoodTrop Then
    txtGoodTrop.BackColor = &H8000&
  Else
    txtGoodTrop.BackColor = &HFF&
  End If

End Sub

Sub UpdateMUXrange()
Dim i%, j%, M%, m1%, m2%, x1!, x2!, x3!, LineC&, x!, dT!(1 To 10000), Tavg!, Trms!, n%
Static MVtop!, MVleft!, MVwidth!, MVheight!, LastRawcycle%, LastWasNonZero As Boolean
  
  If Not ArrayRead And Not RealTime Then Exit Sub
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  
' Set Scales
  With picMUX
  .Cls
  .ScaleMode = 0
  .ScaleTop = Val(cboMUX2.Text)
  .ScaleLeft = m1
  If cboMUX1.Text = cboMUX2.Text Then Exit Sub
  .ScaleHeight = Val(cboMUX1.Text) - Val(cboMUX2.Text)
  .ScaleWidth = Val(m2 - m1)
  If m1 = 1 Then m1 = 2

  MVtop = .ScaleTop
  MVleft = .ScaleLeft
  MVheight = .ScaleHeight
  MVwidth = .ScaleWidth
  End With
' Show position
  If LastRawcycle = 0 Then
    picMUX.PSet (RawCycle, MVtop)
    picMUX.Line -(RawCycle, MVtop + MVheight), QBColor(Black)
    LastRawcycle = RawCycle
  Else
    picMUX.PSet (LastRawcycle, MVtop)
    picMUX.Line -(LastRawcycle, MVtop + MVheight), picMUX.BackColor
    picMUX.PSet (RawCycle, MVtop)
    picMUX.Line -(RawCycle, MVtop + MVheight), QBColor(Black)
    LastRawcycle = RawCycle
  End If
' Draw X tic marks
  x1 = m1
  x2 = m2
 Select Case cboUTunit(1).Text
  Case "Rec No"
    x3 = Val(cboX3(1).Text)
  Case "UTks"
    x3 = Val(cboX3(1).Text) * MVwidth / (Val(txtX2(1).Text) - Val(txtX1(1).Text))
  Case "Hour"
    x3 = Val(cboX3(1).Text) * MVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  Case "hh:mm:ss"
    x3 = Val(cboX3(1).Text) * MVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  End Select
  
  For i = x1 To x2 Step x3
    picMUX.PSet (i, MVtop)
    picMUX.Line -(i, MVtop + 0.05 * MVheight), QBColor(Black)
    picMUX.PSet (i, MVtop + MVheight)
    picMUX.Line -(i, MVtop + 0.95 * MVheight), QBColor(Black)
  Next i

' Draw Y tic marks
  For i = Val(cboMUX1.Text) To Val(cboMUX2.Text) Step Val(cboMUX3.Text)
    picMUX.PSet (MVleft, i)
    picMUX.Line -(MVleft + 0.02 * MVwidth, i), QBColor(Black)
    picMUX.PSet (MVleft + MVwidth, i)
    picMUX.Line -(MVleft + 0.98 * MVwidth, i), QBColor(Black)
  Next i

  DoEvents
' Recalculate gains in case gain scale changed
  For M = 1 To TotalCycles
    Call CalculateGain(M, g!())
    MV(56, M) = g(1)  '36
    MV(57, M) = g(2)  '37
    MV(58, M) = g(3)  '38
'    If M < 200 And M <> 1 Then Debug.Print M; MV(37, M); MV(37, M) - MV(37, M - 1)
  Next M

  For i = 0 To MVchannels
    j = i
    If chkMux(i).Value = 1 Then
      LineC = fMJcolor(j)
      chkMux(i).ForeColor = LineC
      picMUX.DrawWidth = 1
      If i > 64 And i < 73 Then '39 to 46 are status bits now 65-72
        LastWasNonZero = False
        For M = m1 To m2
          x = 5 * (73 - i) * MV(i, M) / 2 ^ (72 - i)
          If x <> 0 Then
            If Not LastWasNonZero Then picMUX.PSet (M, x)
            picMUX.Line -(M, x), LineC   'And continue from there
          End If
          If x <> 0 Then LastWasNonZero = True Else LastWasNonZero = False
          Next M
      Else
        picMUX.PSet (m1, MV(i, 1))
        For M = m1 To m2
          picMUX.Line -(M, MV(i, M)), LineC   'And continue from there
        Next M
      End If
    Else
      chkMux(i).ForeColor = QBColor(Black)
    End If
    DoEvents
  Next i
' Calculate Tnav - ToMA RMS
  For M = m1 To m2
    dT(M - m1 + 1) = OATV(1, M) - cTo - MV(25, M)    '20 now 25=To
  Next M
  n = m2 - m1 + 1
  Call AvgRMS(dT(), n, Tavg, Trms)
  txtTnavRMS.Text = Format(Trms, "0.00")
  txtTnavAVG.Text = Format(Tavg, "0.00")
  
End Sub

Sub UpdateCTSrange()
Dim i%, j%, M%, m1%, m2%, x1!, x2!, x3!, LineC&, r%, g%, b%, flag%, ChColor&, y%, LastGoodOne!, LastM%
Static CVtop!, CVleft!, CVwidth!, CVheight!, LastRawcycle%, x!
  
  If Not ArrayRead And Not RealTime Then Exit Sub
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If TotalCycles < 20 Then Exit Sub
  
' Set Scales
  With picCTS
    .Cls
    .ScaleMode = 0
    .ScaleTop = Val(cboCTS2.Text)
    .ScaleLeft = m1
    If cboCTS1.Text = cboCTS2.Text Then Exit Sub
    .ScaleHeight = Val(cboCTS1.Text) - Val(cboCTS2.Text)
    .ScaleWidth = Val(m2 - m1)
    If m1 = 1 Then m1 = 2

    CVtop = .ScaleTop
    CVleft = .ScaleLeft
    CVheight = .ScaleHeight
    CVwidth = .ScaleWidth
    If chkPlotBadEls.Value = 0 Then
      .BackColor = &H808080
      .DrawWidth = 1
    Else
      .BackColor = &H808080
      .DrawWidth = 3
    End If
  End With
' Show position
  If LastRawcycle = 0 Then
    picCTS.PSet (RawCycle, CVtop)
    picCTS.Line -(RawCycle, CVtop + CVheight), QBColor(Black)
    LastRawcycle = RawCycle
  Else
    picCTS.PSet (LastRawcycle, CVtop)
    picCTS.Line -(LastRawcycle, CVtop + CVheight), picCTS.BackColor
    picCTS.PSet (RawCycle, CVtop)
    picCTS.Line -(RawCycle, CVtop + CVheight), QBColor(Black)
    LastRawcycle = RawCycle
  End If
' Draw X tic marks
  x1 = m1
  x2 = m2
 Select Case cboUTunit(0).Text
  Case "Rec No"
    x3 = Val(cboX3(0).Text)
  Case "UTks"
    x3 = Val(cboX3(0).Text) * CVwidth / (Val(txtX2(0).Text) - Val(txtX1(0).Text))
  Case "Hour"
    x3 = Val(cboX3(0).Text) * CVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  Case "hh:mm:ss"
    x3 = Val(cboX3(0).Text) * CVwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  End Select
  For i = x1 To x2 Step x3
    picCTS.PSet (i, CVtop)
    picCTS.Line -(i, CVtop + 0.05 * CVheight), QBColor(Black)
    picCTS.PSet (i, CVtop + CVheight)
    picCTS.Line -(i, CVtop + 0.95 * CVheight), QBColor(Black)
  Next i

' Draw Y tic marks
  For i = Val(cboCTS1.Text) To Val(cboCTS2.Text) Step Val(cboCTS3.Text)
    picCTS.PSet (CVleft, i)
    picCTS.Line -(CVleft + 0.02 * CVwidth, i), QBColor(Black)
    picCTS.PSet (CVleft + CVwidth, i)
    picCTS.Line -(CVleft + 0.98 * CVwidth, i), QBColor(Black)
  Next i

  DoEvents
If chkPlotBadEls.Value = 0 Then
  For j = 0 To 25  '0-11 are counts, 12 is dND, 13-25 are corresponding Moving Averages
    If chkCTS(j).Value = 1 Then
      For i = 1 To Channels
        If chkCH(i - 1).Value = 1 Then
        LineC = fMJcolor(j)  'RGB(r, g, b) 'QBColor(12)  'LT RED
        chkCTS(j).ForeColor = LineC
        If j < 13 Then
          'Debug.Print j; LineC
          picCTS.PSet (1, CV(i, j + 1, 1))
          LastGoodOne = CV(i, j + 1, 1)
          LastM = 1
          For M = m1 To m2
            If chkPlotBadScans.Value = 1 Then 'Plot everything
              If j < 11 Then
                If Not CQV(M) Then LineC = &HFF0000 Else LineC = fMJcolor(j)  'RGB(r, g, b)
              Else
                If Not CQVS(M) Then LineC = &HFF0000 Else LineC = fMJcolor(j)  'RGB(r, g, b)
              End If
              picCTS.Line -(M, CV(i, j + 1, M)), LineC 'And continue from there
            Else                              'Plot only GOOD scans
'             If M > 1226 Then
'               Debug.Print M; CV(i, j + 1, M); CQM(i, M); CQV(M)
'               X = X
'             End If
              LineC = fMJcolor(j)
              If CQV(M) Then                  'Plot good scans if Cts Quality good
                picCTS.PSet (LastM, LastGoodOne)
                picCTS.Line -(M, CV(i, j + 1, M)), LineC 'And continue from there
                LastGoodOne = CV(i, j + 1, M)
                LastM = M
              Else                            'BAD scan, but dND might be OK, check
                If j = 12 And chkPlotND.Value = 1 Then  '0-11 are 12 Counts, and 12 is dND
                  ' Skip this ND measurement if either CB+ND or CB are interfered with
                  If (CQM(i, M) And 2 ^ 10) Or (CQM(i, M) And 2 ^ 11) Then
                    x = x  'If either Base Counts or Base Counts + ND bad then ignore
                  Else
                    If chkUseCQVS.Value = 1 Then  'Let bad dND affect Sky counts
                      picCTS.PSet (LastM, LastGoodOne)
                      picCTS.Line -(M, CV(i, j + 1, M)), LineC 'And continue from there
                      LastGoodOne = CV(i, j + 1, M)
                      LastM = M
                    Else                          'Use MA dND to avoid bad dND
                      picCTS.PSet (LastM, LastGoodOne)
                      picCTS.Line -(M, EV(i, j + 1, M)), LineC 'And continue from there
                      LastGoodOne = EV(i, j + 1, M)
                      LastM = M
                    End If
                  End If
                End If
              End If
            End If
          Next M
          DoEvents
        
        Else                'j>12
          LineC = QBColor(Black)      'RGB(r, g, b) '  'LT RED
          picCTS.PSet (1, EV(i, j - 12, 1))
          For M = m1 To m2
            picCTS.Line -(M, EV(i, j - 12, M)), LineC 'And continue from there
          Next M
          DoEvents
        End If
        End If
      Next i
    Else
      chkCTS(j).ForeColor = QBColor(Black)
    End If
  Next j

Else  'Plot Bad Els

    For i = 1 To Channels
      Select Case i
      Case 1: ChColor = &HFF&              'RED
      Case 2: ChColor = &HFFFFFF       'White
      Case 3: ChColor = &HFF0000        'Blue
      Case Else
      End Select
      
      For j = 1 To 12
        picCTS.PSet (m1, i + 5 * (j - 1))
        For M = m1 To m2
          'y = i + 5 * (j - 1)
          flag = CQM(i, M) And 2 ^ (j - 1)
          If flag Then LineC = ChColor Else LineC = picCTS.BackColor  'RGB(r, g, b)
          picCTS.Line -(M, i + 5 * (j - 1)), LineC 'And continue from there
        Next M
        DoEvents
      Next j
    Next i
End If
End Sub

Sub CalculateAverageProfile(RA!(), RT!(), RAavg!(), RTavg!())
Dim i%, j%, L%, RAsave!(1 To 4, 1 To 33), RTsave!(1 To 4, 1 To 33), q%
Dim Wtot!, W!(1 To 4), Wg!
Static Tg!(1 To 33), Zg!(1 To 33), Tcorr!
  
  txtOATmtpCor.Text = Format(OATmtpCOR, "#0.00")
  If chkOATmtpCOR.Value = 1 Then Tcorr = OATmtpCOR Else Tcorr = 0#
  For j = 1 To 33
    If RA(j) < FLA(NFL) / 100# Then RAavg(j) = 0# Else RAavg(j) = RA(j)
    RTavg(j) = RT(j) + Tcorr
    If chkWriteATP.Value = 1 Then Debug.Print j; RTavg(j); RAavg(j)
  Next j
  'Debug.Print OATnav; OATnavCOR; OAT; ob(6); ob(16); ob(26); TA(1, 6); TA(2, 6); TA(3, 6); RT(16)

Exit Sub
  
' Averaging corrupts data mjm 19990226
  If Abs(Roll) > 30 And Roll <> 999.9 Then Exit Sub
  
' Check for valid A/C Roll before updating profile and derived parameters
' Save last 4 RA() & RT(), and derive weighted avg:  RAavg() & RTavg()
  For i = 3 To 3     'FOR i = 1 TO 3 to 3 TO 3 does 2 cy; 1 to 3 does 4 cy
    For j = 1 To 33:
      RAsave(i, j) = RAsave(i + 1, j): RTsave(i, j) = RTsave(i + 1, j):
    Next j
  Next i

'Check quality of retrieved temperature, RT()
  q% = 1
  If RT(17) < 170 Or RT(17) > 310 Then q% = 0
  If CtsQual = BAD Or TAqual = BAD Then q% = 0
  'more quality checks of current profile go here

If q% = 1 Then
  For j = 1 To 33: RAsave(4, j) = RA(j): RTsave(4, j) = RT(j): Next j
    '  RAsave() & RTsave() are now set; 1 = 4 cy ago, 4 = most recent cy

  W(1) = 0.8: W(2) = 1: W(3) = 1.3: W(4) = 1.6
  For i = 3 To 4     'FOR i = 1 TO 4  '3 to 4 does 2 cy, 1 to 4 does 4 cy
    If RTsave(i, 17) = 0 Then W(i) = 0
  Next i
  Wtot = 0: For i = 3 To 4: Wtot = Wtot + W(i): Next i
  If Wtot = 0 Then Stop

  For j = 1 To 33: RAavg(j) = 0: RTavg(j) = 0: Next j
  For i = 3 To 4      '3 to 4 does 2 cy; 1 to 4 does 4 cy
    For j = 1 To 33
      RAavg(j) = RAavg(j) + W(i) * RAsave(i, j)
      RTavg(j) = RTavg(j) + W(i) * RTsave(i, j)
    Next j
  Next i

  For j = 1 To 33:
    RAavg(j) = RAavg(j) / Wtot: RTavg(j) = RTavg(j) / Wtot
  Next j

End If

' Update ghost profile
  If Tg(17) = 0 And q% = 1 Then
    If RTavg(17) > 170 And RTavg(17) < 310 Then  'chk quality of OAT
      For L = 1 To 33: Zg(L) = RAavg(L): Tg(L) = RTavg(L): Next L
    End If
  End If
  Wg = 0.06
  If Cycle% > 1 Then
    For L = 1 To 33
      Zg(L) = (Zg(L) + Wg * RAavg(L)) / (1 + Wg)
      Tg(L) = (Tg(L) + Wg * RTavg(L)) / (1 + Wg)
    Next L
  End If
End Sub

Function fCheckCountsQuality(Cycle%, ALLflag As Boolean) As Boolean
Dim i%, j%, k%, dCBcts%(1 To 3), dCScts%(1 To 3), dCNcts%(1 To 3), CBmax%, CSmax%, CNmax%
Static CBlast!(1 To 3), CBgood!(1 To 3)
Static CSlast!(1 To 3), CSgood!(1 To 3)
Static CNlast!(1 To 3), CNgood!(1 To 3)
Static Init!, DC!(1 To 3), Ccycle%
Dim dCavg!(1 To 3), dCrms!(1 To 3), Asum!(1 To 3), Rsum!(1 To 3), x$, y$
Dim CBbad%(1 To 3), CSbad%(1 To 3), CRbad%(1 To 3), CNbad%(1 To 3), ECB%, ECS%, ECN%, ECR%

'If chkCtsQuality.value = 0 Then Exit Function  ' Skip quality check

If Init = 0 Then
  Ccycle = 0
  For i = 1 To Channels
    CBgood(i) = CB(i)
    CSgood(i) = CS(i, LocHor)
    CNgood(i) = dND(i)
  Next i
  Init = 1
  If ALLflag Then
    Print #ALLlu, "Average" + vbTab + "=average(b7:b2000)"
    Print #ALLlu, "Std Dev" + vbTab + "=stdev(b7:b2000)"
    Print #ALLlu, "Min" + vbTab + "=min(b7:b2000)"
    Print #ALLlu, "Max" + vbTab + "=max(b7:b2000)"
    Print #ALLlu, " "
    x$ = " UT(ks)" + vbTab + " H:M:S" + vbTab + "Scan" + vbTab + "pALT" + vbTab + "X"
    For i = 1 To Channels
      Select Case i
      Case 1
        x$ = x$ + vbTab + "B1" + vbTab + "S1" + vbTab + "N1" + vbTab + "R1" + vbTab + "dB1" + vbTab + "dS1" + vbTab + "dN1" + vbTab + "dR1" + vbTab + " CB1 " + vbTab + " CS1 " + vbTab + " ND1 "
      Case 2
        x$ = x$ + vbTab + "B2" + vbTab + "S2" + vbTab + "N2" + vbTab + "R2" + vbTab + "dB2" + vbTab + "dS2" + vbTab + "dN2" + vbTab + "dR2" + vbTab + " CB2 " + vbTab + " CS2 " + vbTab + " ND2 "
      Case 3
        x$ = x$ + vbTab + "B3" + vbTab + "S3" + vbTab + "N3" + vbTab + "R3" + vbTab + "dB3" + vbTab + "dS3" + vbTab + "dN3" + vbTab + "dR3" + vbTab + " CB3 " + vbTab + " CS3 " + vbTab + " ND3 "
      End Select
    Next i
    Print #ALLlu, x$
  End If
End If
' End Initialization

' Clear text boxes and initialize flags
  For i = 1 To Channels
    txtCB(i - 1) = "": txtCS(i - 1) = "": txtCN(i - 1) = "": txtScanRMS(i - 1) = ""
    CBbad(i) = 0: CSbad(i) = 0: CNbad(i) = 0: CRbad(i) = 0
  Next i
  Ccycle = Ccycle + 1
  fCheckCountsQuality = Good

' First check base counts as it is faster.
    CSmax = 20
    CBmax = 20
    CNmax = 20
If Cycle > 1 Then
  For i = 1 To Channels
    dCBcts(i) = CB(i) - CBgood(i)
    txtCB(i - 1).Text = Str(dCBcts(i))
    If Abs(dCBcts(i)) > CBmax Then   'ER2 had RMS in both channels of 6.3 counts
      CBbad(i) = 1
      fCheckCountsQuality = BAD
      If chkWriteBadCts.Value = 1 Then BadCounts = BadCounts + 1
    Else 'Make sure two bads in a row aren't present
      CBgood(i) = (CBgood(i) + 0.5 * CB(i)) / 1.5
    End If
    
'Check Sky Counts
    dCScts(i) = CS(i, LocHor) - CSgood(i)
    txtCS(i - 1).Text = Str(dCScts(i))
    If Abs(dCScts(i)) > CSmax Then   'ER2 had RMS in both channels of 6.3 counts
      fCheckCountsQuality = BAD
      CSbad(i) = 1
      If chkWriteBadCts.Value = 1 Then BadCounts = BadCounts + 1
    Else
      CSgood(i) = (CSgood(i) + 0.5 * CS(i, LocHor)) / 1.5
    End If
   'Debug.Print UTsec; "CS"; Ccycle; i; CSlast(i); CSgood(i); CS(i)
  
'Check Noise Diode Counts
    dCNcts(i) = dND(i) - CNgood(i)
    txtCN(i - 1).Text = Str(dCNcts(i))
    If Abs(dCNcts(i)) > CNmax Then   'ER2 had RMS in both channels of 6.3 counts
      fCheckCountsQuality = BAD
      CNbad(i) = 1
      If chkWriteBadCts.Value = 1 Then BadCounts = BadCounts + 1
    Else
      CNgood(i) = (CNgood(i) + 0.5 * dND(i)) / 1.5
    End If
  
  Next i

Else
  For i = 1 To Channels: CBgood(i) = CB(i): CSgood(i) = CS(i, LocHor): CNgood(i) = dND(i): Next i
End If
' Save current CB CS values
  For i = 1 To Channels: CBlast(i) = CB(i): CSlast(i) = CS(i, LocHor): CNlast(i) = dND(i): Next i

' Calculate scan RMS
  For i = 1 To Channels: Asum(i) = 0: Rsum(i) = 0: Next i
  For i = 1 To Channels
    For j = 1 To 9
      Asum(i) = Asum(i) + C(i, j) - C(i, j + 1)
      Rsum(i) = Rsum(i) + (C(i, j) - C(i, j + 1)) ^ 2
    Next j
    dCavg(i) = Asum(i) / 9                       'Calculate average
    dCrms(i) = Sqr((Rsum(i) - dCavg(i) ^ 2) / 8) 'Calculate RMS
    txtScanRMS(i - 1).Text = Format(dCrms(i), "00.0")
  Next i
'
  For i = 1 To Channels
    If dCrms(i) > DC(i) Then
      fCheckCountsQuality = BAD
      CRbad(i) = 1
      If chkWriteBadCts.Value = 1 Then BadCounts = BadCounts + 1
    End If
  Next i

If ALLflag Then
'  Exit Function  'xxx mjm 20000119 should this be here
  ECB = 4 * CBbad(3) + 2 * CBbad(2) + CBbad(1)
  ECS = 4 * CSbad(3) + 2 * CSbad(2) + CSbad(1)
  ECN = 4 * CNbad(3) + 2 * CNbad(2) + CNbad(1)
  ECR = 4 * CRbad(3) + 2 * CRbad(2) + CRbad(1)
  
  If ECB + ECS + ECN + ECR > 0 Then y$ = "1" Else y$ = "0"
  x$ = Format(UTsec / 1000, "000.000") + vbTab + hhmmssNAV$ + vbTab + Format(Cycle, "0000") + vbTab + Format(pALT, "00.00") + vbTab + y$
  k = 1
  For i = 1 To Channels
    k = 4 * (i - 1) + 1 '1 5 9
    x$ = x$ + vbTab + formats(k * CBbad(i), "0") + vbTab + formats((k + 1) * CSbad(i), "0") + vbTab + formats((k + 2) * CNbad(i), "0") + vbTab + formats((k + 3) * CRbad(i), "0") + vbTab + formats(dCBcts(i), "000") + vbTab + formats(dCScts(i), "000") + vbTab + formats(dCNcts(i), "000") + vbTab + formats(dCrms(i), "000.0") + vbTab + formats(CB(i), "00000") + vbTab + formats(CS(i, LocHor), "00000") + vbTab + formats(dND(i), "00000")
  Next i
  Print #ALLlu, x$
End If

elsewhere:
' PEM TB DC8 data
'                C15-C16      C25-C26       C35-C36
' avg       -21.16393443   -8.163934426  -1.918032787  Restricted to good data
' rms         6.856092978   5.879853535   5.54164561   Restricted to good data
' 5*rms      34.28046489   29.39926768   27.70822805
' avg all   -11.90687766  -10.0127815     0.802191114
' stdev all 185.5563297    33.47576006  190.1839463
' ER2 SOLVE 20000307
'23.22058099 11.49187592 Average
'8.609056399 7.195360299 Stdev

End Function

Sub Delay(dT!)
' Delay for dT seconds
Dim time1!
  
  time1 = Timer
  DoEvents
  Do: Loop While Timer - time1 < dT

End Sub

Function fFixT!(i%, y!)
If i = 6 Then
  fFixT = picGain.ScaleTop + (300 - y) * picGain.ScaleHeight / 120
Else
  fFixT = y
End If

End Function

Sub Initialize()
Dim A$, ReadLine As Boolean, xx#, RetVal As Variant, DefCalfile$, MyDate As Variant
Dim UT&, U!, V!, W!, P!, T!, Th!, GoToNext As Boolean, FileLength&, fiMMS$, eFormat$, E$
  
  If RealTime Then RawCycle = 1
  ProgramConstants
' Handle aircraft dependent parameters
  Ceiling = 24#             'ER2 Service ceiling
  CycleTime = 15            'Time for one MTP cycle (s)
  frmATP.FooterType = 0
  LocHor = 6
  Select Case AC$
  Case "ER", "WB", "GH"
  Case "DC"
    Ceiling = 13              'DC8 service ceiling
    Remote = True             'Allow remote control commands
    CAC = True                'Chassis Analysis Computer in use?
    If Val(YYYYMMDD$) < 19960325 Then RHS = False: LocHor% = 5
  Case "NG", "HA"
    Ceiling = 16
    CycleTime = 18
  Case "M5"
    LocHor = 5
  End Select
  MRIrec% = 0
  MRI1sum = 0#
  MRI2sum = 0#
  ScanTime = fScanLength(1, TotalCycles)
  DoEvents
  
'  If Not Substitute Then 'Don't delete REF file on cmdSubstitute
'    Select Case Source$
'    Case "F"
   If Not RealTime Then
      On Error GoTo CreateREFfile
      FileLength = FileLen(REFfile$)
      If FileLength > 0 Then Kill REFfile$
'        If DoAll Then  ' Don't bother with message if DoAll is TRUE, just delete file
'          Kill REFfile$
'        Else
'          Msg = REFfile$ + " already exists!" + vbCrLf + vbCr + "Do you want to delete it?" ' Define message.
'          Style = vbYesNo                    ' Define buttons.
'          Title = "WARNING!"                 ' Define title.
'          response = MsgBox(Msg, Style, Title, Help, Ctxt)
'          If response = vbYes Then Kill REFfile$
'        End If
        frmFLTINFO.UpdateFLTINFO         'Update FLTinfo
        Call REFwriteHEADER(REFfile$)    'And write new header
'      End If
  
CreateREFfile:
      REFopen REFfile$
      On Error GoTo TryWND
      If useMMS Then
        MMSlu% = FreeFile             'line below BLG(980411)
        Open MMSfile$ For Input As #MMSlu%
      End If
      GoTo SetHeaders

TryWND:
      Mid$(fiMMS$, 12, 3) = "WND"
      Close #MMSlu%
      MMSlu% = FreeFile                      'line below BLG(980411)
      Open Path$ + MMSfile$ For Input As MMSlu%

SetHeaders:
      On Error GoTo 0
  End If
'    Case "D"
'      On Error GoTo Error_Handler  'Set up to handle Run Time errors (in D-mode only)

'    Case "T"
  
'    End Select
'  End If
  
  ProgressBar1.Min = 0
  ProgressBar1.Max = TotalCycles
  DoEvents
Exit Sub

Error_Handler:
  eFormat$ = "Error ### occurred on line ####. "

  Select Case Err
  Case 53                              'File not Found
    E$ = eFormat$ + "File Not Found" + Space$(32)
'    Print Using; e$; Err; Erl
    INC Err53count%
  Case 57                              'Device I/O Error
    E$ = eFormat$ + "Device I/O Error" + Space$(31)
'    Print Using; e$; Err; Erl
    INC Err57count%
    Resume Next
  Case Else
    INC Errxxcount%
    Resume Next
  End Select

End Sub

Function fFindDrive(g As Control, DriveName$) As String
Dim i%

' Search for DriveName$ in Drive List and return with Logical Unit (letter) without colon
  For i = 0 To g.ListCount - 1
    If InStr(1, g.List(i), DriveName$, 1) > 0 Then
      fFindDrive = Left$(g.List(i), 1)
      Exit Function
    End If
  Next i
  fFindDrive = ""
End Function

Sub Main()
' Mother-of-All-Programs MAIN Program
' This sub is called from tmrGetCycle after a record has been read by ReadArray
Dim i%, j%, k%, L%, n%, xx!, A$, jt1%, jt2%, Zi!, Ti!, Tice!, Tnat!, dTi!
Dim x$, ZtMax1!, ZtMax2!, ZtMax!, TropTheta1!, TropTheta2!
Dim Zt1Qual As Boolean, Zt2Qual As Boolean, Zt1g!, Zt2g!, zacg!, Tzt1g!, Tzt2g!
Dim Pz!(1 To 33), RHZ!(1 To 33), Zx!(1 To 33), Thmin5!, LOstuck As Boolean

Static Znd!(1 To 33), Vnd!(1 To 33)
Static NDavgCount%, lu%, pALTndp!, LowTrops%, TMPlu%, RollLast!
Static C1last%, C2last%, C3last%, TBtwo As Boolean, TAsave!(1 To 3, 1 To 10), nTB%

ReadLine:                            'Loop here from End-of-Program
  If Not RealTime Then
    Call ReadArrayLeadLag(RawCycle)
  Else
    For i = 1 To Channels
      dND(i) = CV(i, 11, RawCycle) - CV(i, 12, RawCycle)
    Next i
  End If
  
  If chkLOchanging.Value = 1 Then LOstuck = fLOstuck(RawCycle) Else LOstuck = False
  
  If chkNextAline.Value = 1 Then Call ReadArrayNextAline(RawCycle)   'Normally TRUE if not REALTIME
  
  Call GainCalculation(RawCycle)     'Do Gain calculation
  
  If Not RealTime Then Call UpdateGainWindow(False)
  
  Call TBcalculation                 'And calculate observable vector
'------------------------------------------------------------------------------------------------------
' Code below here modified to average TBs for ATTREX
  
  If chkAverageTBs.Value = 1 Then
    If TBtwo Then
      For i = 1 To 3
        For j = 1 To 10
          TA(i, j) = (TA(i, j) + TAsave(i, j)) / Val(cboAverageTBs.Text)
        Next j
      Next i
      For i = 1 To 3: For j = 1 To 10: TAsave(i, j) = 0#: Next j: Next i
      TBtwo = False
      nTB = 0
    Else
      For i = 1 To 3
        For j = 1 To 10
          TAsave(i, j) = TAsave(i, j) + TA(i, j)
        Next j
      Next i
      nTB = nTB + 1
      If nTB = Val(cboAverageTBs.Text) - 1 Then TBtwo = True
      Exit Sub
    End If

  End If
' The following routine was developed in MTPsim to map TAs to OBs (verified for 3 channels, LocHor=6)
  Call MapTAtoOB(TA!(), ob!(), Channels%, Nel%, LocHor%, ChInfo!())  'Used in Sim code
  ob(0) = pALT    'pALT
  
  If ShowDiagnostics Then UpdateTBwindow
  
' Code above here modified to average TBs for ATTREX
'-------------------------------------------------------------------------------------------------------
  
  Call DetermineRCregion(ob(), frmMTPbin, frmStatus, frmFLTINFO)
  
  Algorithm = cboAlgorithm.ListIndex
  Call Do_Retrieval(Algorithm, Latitude, ob(), RT(), RA(), frmMTPbin)

  Call CalculateAverageProfile(RA(), RT(), RAavg(), RTavg())  ' Update avg profile if data is OK
        
  GoSub DetermineTropParameters      'Do Cubic Spline fit for Trop stuff
   
  GoSub TropRejection                'Reject trop based on many criteria
  
  GoSub WriteToOutFiles              'Write to Output files
  
  UpdateRT
  
  REFwriteLIMITS REFfile$
  
EndOfSub:
  
  DoEvents

  Exit Sub

'****************************  SUB ROUTINES  *******************************'
'
DetermineTropParameters:
  If Abs(Roll) > 30 And Roll <> 999.9 Then Return
  'Check for valid A/C Roll before updating profile and derived parameters
' Start overhead for SPLINE fit, First ...

' Figure out number of non-zero retrieved levels
  jt1% = 0: Do: jt1% = jt1% + 1: Loop Until RAavg(jt1%) >= 0 Or jt1% = 33
  jt2% = 34: Do: jt2% = jt2% - 1: Loop Until RAavg(jt2%) > 0 Or jt2% = 1
  If jt1% = 33 Then GoTo EndOfSub     ' No retrieved level, start all over
             

' Finally, do the SPLINE fit to retrieved ATP
  Call SPLINE(RAavg(), RTavg(), T1(), T2(), jt1%, jt2%)

' Calculate temperature at AC altitude
  'Tac = SPLINT(RAavg(), RTavg(), t2(), jt1%,jt2%, pALT,1)
  'Tac = OAT     'Do this way for now, SPLINT leads to strange T's on gnd!
                 ' and sometimes in the air
' Figure out value of z for which lapse rate is -2.
  Call WMOTropLoc(RAavg(), RTavg(), T1(), T2(), jt1%, jt2%, Trop1!, TTrop1!, Trop2!, Ttrop2!)

' Find cold point Z and T
  Call FindXmin(jt1, jt2, RTavg(), RAavg(), 6#, 19#, Tcp, Zcp)
  
  If Latitude > 50# And 1 = 2 Then
    ' Based on examination of radiosondes for SOLVE2 there a linear fit to Ttrop1 vs Trop1
    ' with an Std Deviation of 4.9 K. Exclude all trops with deviations from fit of >10 K
    ' Valid trops were seen between 5.6 and 14.3 km (see Coldest2D.xls in DC8/SOLVE2/ folder)
    ' If Abs(250.89 - 4.005 * Trop1 - TTrop1) > 10 Then Trop1 = 99.9: Trop2 = 99.9
    If Trop1 <> 99.9 Then
      If 280 - 5.5 * Trop1 < TTrop1 Then Trop1 = 99.9
    End If
  End If
  
  If Trop1 < RAavg(1) Then
    Trop1 = 99.9
    Trop2 = 99.9
    LowTrops = LowTrops + 1
    Debug.Print "Low Trop Count="; LowTrops
  End If
  If chkEnableDT.Value = 0 Then
  Trop2 = 99.9
  End If
  If Trop1 <> 99.9 Then TropTheta1 = fTheta(TTrop1, fZtoP(Trop1)) Else TropTheta1 = 999.9
    
  If Trop2 <> 99.9 Then TropTheta2 = fTheta(Ttrop2, fZtoP(Trop2)) Else TropTheta2 = 999.9
' Define new parameters for data structures
  Zt1 = Trop1: Zt2 = Trop2
  TT1 = TTrop1: TT2 = Ttrop2
  Th1 = TropTheta1: Th2 = TropTheta2

' Calculate position of current and gobal minima
  Call T_minimum(pALT, Cycle, RAavg(), RTavg(), T1(), T2(), jt1%, jt2%, Zmin5, Tmin5)
  Thmin5 = fTheta(Tmin5, fZtoP(Zmin5))
  If Tmin5 < Tzgm And Tmin5 > 175 Then
    Tzgm = Tmin5: Ztgm = Zmin5: Thgm = Thmin5: UTgm = UTsec
  End If

  Call TropLoc(RAavg(), RTavg(), zt0)   ' BLG TropLoc Calculation
                                              ' zt0=TropAvg, Tzt0=Ttrop
' Calculate maximum ice saturation temperature
  Zice = 999.9: dTice = 999.9
  For j = 1 To 33
    Zi = RAavg(j): Ti = RTavg(j)       '[km] & [Kelvin]
    Tice = 188.5 - 0.89 * (Zi - 20)    'ice saturation, assuming 4.5 ppmv
    Tnat = 213.58 - 0.8648 * Zi        'ice sat'n, 10 ppbv HNO3 & 5 ppmv H2O
    Tnat = 195.5 - 0.8626 * (Zi - 20)  'fit for 8 ppbv HNO3 & 4.5 ppmv H2O
    If DoNAT Then Tice = Tnat         'use Tnat instead of Tice
    dTi = Ti - Tice
    If Zi > Zt1 + 1 And dTi < dTice Then dTice = dTi: Zice = Zi
  Next j
' dTice = -dTice         '(+)-values to represent supersaturation
' IF dTice < -10 THEN Zice = 999.9   'no chance of NAT or ICE, so don't plot

' Calculate Lapse Rate at A/C and its average
  LRac = 999.9
  LRac2 = 999.9
  LRac3 = 999.9
  If pALT < 0.15 Then
    If RA(21) <> RA(16) Then LRac = (RT(21) - RT(16)) / (RA(21) - RA(16)) '1 km
    If RA(24) <> RA(16) Then LRac2 = (RT(24) - RT(16)) / (RA(24) - RA(16)) ' 2 km
    If RA(18) <> RA(16) Then LRac3 = (RT(18) - RT(16)) / (RA(18) - RA(16)) ' 300 m
  Else
    If pALT < 0.3 Then
      If RA(20) <> RA(15) Then LRac = (RT(20) - RT(15)) / (RA(20) - RA(15)) '850 m
      If RA(24) <> RA(15) Then LRac2 = (RT(24) - RT(15)) / (RA(24) - RA(15)) ' 2.15 km
      If RA(17) <> RA(15) Then LRac3 = (RT(17) - RT(15)) / (RA(17) - RA(15)) ' 300 m
    Else
      If pALT < 1# Then
        If RA(19) <> RA(14) Then LRac = (RT(19) - RT(14)) / (RA(19) - RA(14)) ' 1 km
        If RA(23) <> RA(14) Then LRac2 = (RT(23) - RT(14)) / (RA(23) - RA(14)) ' 1.9 km
        If RA(17) <> RA(15) Then LRac3 = (RT(17) - RT(15)) / (RA(17) - RA(15)) ' 300 m
      Else
        If RA(19) <> RA(13) Then LRac = (RT(19) - RT(13)) / (RA(19) - RA(13)) ' 1 km
        If RA(21) <> RA(11) Then LRac2 = (RT(21) - RT(11)) / (RA(21) - RA(11)) ' 2 km
        If RA(17) <> RA(15) Then LRac3 = (RT(17) - RT(15)) / (RA(17) - RA(15)) ' 300 m
      End If
    End If
  End If
  Select Case LRac
  Case 0, Is < -20, Is > 40
    LRac = 999.9
  End Select
  'If RealTime Then RawCycle = RawCycle + 1
  MV(33, RawCycle) = LRac
  MV(34, RawCycle) = LRac2
  MV(35, RawCycle) = LRac3

'If TMPlu = 0 Then
'  TMPlu = FreeFile
'  Open "C:\MTP\LRs.txt" For Output As TMPlu
'Else
'    Print #TMPlu, UTsec / 1000; LRac; LRac2; LRac3; pALT
'End If
  
Return

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
TropRejection:
  Zt1Qual = Good: Zt2Qual = Good
' Determine useability of tropopause solutions Zt1 & Zt2 for RET recording
  'IF Source$ = "D" THEN return     'Don't bother if "D" mode

  If CtsQual = BAD Then Return

  If Abs(Roll) > 15 Then Zt1Qual = BAD: Zt2Qual = BAD: Return
  If Abs(Roll) < 45 Then
    If Abs(Roll - RollLast) > 7 Then Zt1Qual = BAD: Zt2Qual = BAD
    RollLast = Roll
  End If
  If Zt1Qual = BAD Then Return

  'IF ABS(ALTkmTREND) > 17 * 5 THEN Zt1Qual% = BAD: Zt2Qual% = BAD
  'MJM 971011
  If Zt1Qual = BAD Then Return

  'Now chk for Zt being outside permissible altitude limits
  ZtMax1 = pALT + 7.5
  ZtMax2 = 2.8 * pALT - 9
  ZtMax = ZtMax1: If ZtMax2 < ZtMax Then ZtMax = ZtMax2
  zTmin = 2.5 + pALT / 2
  If Zt1 > ZtMax Or Zt1 < zTmin Then Zt1Qual = BAD
  If Zt2 > ZtMax Or Zt2 < zTmin Then Zt2Qual = BAD
Return

'''''''''''''''''''''''''''''''''''''''''''''''''''F''''''''''''''''''''''''''
WriteToOutFiles:
' I don't think this is needed because "D" mode no longer used, even in real time
'  If Source$ = "D" Then  'Update to allow remote access
'    Close OUTlu
'    Open OUTfile$ For Append As OUTlu
'  End If
  
  If (NavQual And CtsQual And TAqual) Then CycleQual = True Else CycleQual = False
  If (OATmtp < 175 Or OATmtp > 320) Then OATmtp = 999.9: CycleQual = False
  'xxx pALT for DC8 is 14
  If (pALT > 25 Or Abs(Roll) > 30) Then CycleQual = False
 
' Resume after skipping RET-file output (due to poor data quality)

  GoodScan = CycleQual
  GoodTrop = True
  If DoRetrieval Then
'   Deal with geometric altitude if necessary
    dZg = gALT - pALT  'gALT offset wrt pALT at flight level
'  1 is lowest level
' 16 is A/C level
' 33 is highest level
' Nlev1 is first good level starting from 1
' Nlev2 is last good level starting from Nlev1
' Nlev is total number of good levels
    
    Nlev = 0
    For i = 1 To 33
      If RAavg(i) >= FLA(NFL) / 100# And RAavg(i) > 0 Then 'xxx
        Nlev = Nlev + 1  'Keep track of levels count for plotting
        If Nlev = 1 Then Nlev1 = i   'First level above ground
      End If
    Next i
    Nlev2 = Nlev + Nlev1 - 1
    
    Call IntegrateHE(Nlev, RAavg(), RTavg(), dZgeo(), NoD())

    For i = 1 To 33
      Pz(i) = fZtoP(RAavg(i))
      RHZ(i) = 50#
    Next i
  
'    Call CalculateZarray(Latitude, 0#, Nlev1%, Nlev2%, Pz!(), RTavg(), RHZ!(), 1, Zx!())
    ' dZgeo() are offsets wrt fight level, NoD() is number density profile
    For i = 33 - Nlev + 1 To 33
      If Abs(gALT + dZgeo(i)) > 30 Then
      dZgeo(i) = 30
      End If
      sZg(i) = gALT + dZgeo(i)
      Zgeo(i) = Int(100 * sZg(i))
      Zx(i) = Zx(i) - RTavg(16) + dZg
      If Abs(NoD(i)) > 3E+25 Then NoD(i) = 3E+25       '#/m3

      ND(i) = Int(NoD(i) / 1E+21)
      sND(i) = ND(i)
      'Debug.Print i; Zgeo(i); Zx(i)'
NextLevel:
    Next i
'    NDac = ND(16)          mjmmjm
    
    Nlev = 0
    For i = 1 To 33
    ' Prepare arrays for REFwrite
      If RTavg(i) < 0 Then RTavg(i) = 0#
      sZZZ(i) = RAavg(i)
      sTTT(i) = RTavg(i)
      zzz(i) = Int(100# * RAavg(i))
'      If RTavg(i) > 32767 Then TTT(i) = 32767 Else TTT(i) = Int(100# * RTavg(i))
      If RAavg(i) > 0# Then
        Nlev = Nlev + 1  'Keep track of levels count for plotting
        If Nlev = 1 Then Nlev1 = i   'First level above ground
      ' Prepare pALT or gALT arrays dimensioned 1 to Nlev
        If ShowZp Then
          RAp(Nlev) = RAavg(i)  'array from 1 to Nlev
        Else
          RAp(Nlev) = gALT + dZgeo(i)
        End If
        If i = 16 Then
          'Debug.Print "Main: RAavg(16)="; RAavg(16); " NLEV="; Nlev; " i="; i
        End If
        
        RTp(Nlev) = RTavg(i)
        RTp2(Nlev) = TAAi(i)
        RTp3(Nlev) = TAAi2(i)
      Else
      x = x
      End If
    Next i
    Nlev2 = Nlev1 + Nlev - 1

    If ShowZp Then  'Plot Pressure Altitude
      With frmATP
        .ALTkm = pALT
        .Zt1 = Zt1
        .Zt2 = Zt2
        .dZg = dZg
        If RealTime Then .ShowAAprofile = False
      End With

    Else            'Plot Geometric Altitude
    ' Finally, do the SPLINE fit to retrieved ATP
      Call SPLINE(RAp(), RTp(), T1(), T2(), 1, Nlev)
      Call WMOTropLoc(RAp(), RTp(), T1(), T2(), 1, Nlev, Zt1g, Tzt1g, Zt2g, Tzt2g)

      With frmATP
        .ALTkm = gALT
        .Zt1 = Zt1g
        .Zt2 = Zt2g
        .dZg = dZg
      End With
    End If
    
    txtUT(0).Text = UTsec
    txtUT(1).Text = fSecToTstring$(UTsec, True)
    txtRecord.Text = Cycle%
    txtMRI1.Text = Format$(MRI, "##0.00")
    txtMRI1.BackColor = &HC0FFC0
    txtMRI2.Text = Format$(MRI2, "##0.00")
    If RCindex2 >= 0 Then
      If (Sgn(TBavg(RCindex2)) <> Sgn(TBavg(RCindex1))) Then TwoAAprofiles = True Else TwoAAprofiles = False
      If Algorithm = 7 Then TwoAAprofiles = True
    End If
    If RCindex1 >= 0 Then txtOBavg(0) = OBbias(RCindex1) Else txtOBavg(0) = ""
    If RCindex2 >= 0 Then txtOBavg(1) = OBbias(RCindex2) Else txtOBavg(1) = ""
    If RCindex1 >= 0 Then label(196).Caption = frmFLTINFO.cboReg(RCindex1)
    If RCindex2 >= 0 Then label(197).Caption = frmFLTINFO.cboReg(RCindex2) Else label(197).Caption = ""
    If TwoAAprofiles Then txtMRI2.BackColor = &HC0FFC0 Else txtMRI2.BackColor = &HC0C0FF
    
    Call UpdateTBwindow
    
    If ShowTBs Then
      If (Not GoodScan And chkDisplayBadScans.Value = 1) Or GoodScan Then
        With frmTB
          .TBupdate
        End With
      End If
    End If
    
    If ShowATP Then
      If (Not GoodScan And chkDisplayBadScans.Value = 1) Or GoodScan Then
        With frmATP
          If chkEnableFit.Value = 1 Then .EnableFit = True Else .EnableFit = False
          .Tac = Tac
          .TwoAAprofiles = TwoAAprofiles
          .ATPupdate
          If ShowZp Then .ShowPressureAltitude = True Else .ShowPressureAltitude = False
        End With
      End If
    End If
  End If
  'Debug.Print "Main: OATmtp="; OATmtp; " Tac="; Tac; " RTp(16)="; RTp(16 - Nlev1 + 1); " RAp(16)="; RAp(16 - Nlev1 + 1); " ALTkm="; frmATP.ALTkm; "pALT="; pALT
' Update Realtime Status Box
  DoTropStatusBox
  
' Display NDP if required
  If ShowNDP And DoRetrieval Then
    frmNDP.Visible = True
    For i = 1 To Nlev
      RAp(i) = pALT + dZg + dZgeo(i + Nlev1 - 1)
      RTp(i) = NoD(i + Nlev1 - 1)
    Next i
    frmNDP.NDPupdate
  Else
    frmNDP.Visible = False
  End If
  
' Check if average ND profile is needed and whether to FTP
  If chkWriteND.Value = 1 Then
    If NDavgCount = 0 Then
      NDavgCount = Val(txtNDavgCount.Text)
      For i = 1 To 33
        Znd(i) = 0#: Vnd(i) = 0#
      Next i
      pALTndp = pALT      'Save initial value
    End If
    
    If Abs(pALT - pALTndp) > 0.03 Then   'restart if pALT changes by >30 meters
      NDavgCount = Val(txtNDavgCount.Text)
      For i = 1 To 33
        Znd(i) = 0#: Vnd(i) = 0#
      Next i
      pALTndp = pALT      'Save initial value
    End If
    
    NDavgCount = NDavgCount - 1
'    Debug.Print NDavgCount
    
    Select Case NDavgCount
    Case Is > 0
      For i = 1 To Nlev
        Znd(i) = Znd(i) + pALT + dZg + dZgeo(i + Nlev1 - 1)
        Vnd(i) = Vnd(i) + NoD(i + Nlev1 - 1)
      Next i
    Case Is = 0
      For i = 1 To Nlev
        Znd(i) = Znd(i) + pALT + dZg + dZgeo(i + Nlev1 - 1)
        Vnd(i) = Vnd(i) + NoD(i + Nlev1 - 1)
      Next i

      lu = FreeFile
      Open "C:\MTP\Setup\Density\NDprofile.txt" For Output As lu
      ' Calculate average
      For j = 1 To Nlev
        Znd(j) = Znd(j) / Val(txtNDavgCount.Text)
        Vnd(j) = Vnd(j) / Val(txtNDavgCount.Text)
        Print #lu, Znd(j), Vnd(j)
      Next j
      Close lu
      chkWriteND.Value = 0
      'FTP Data to DIAL
      If chkFTPtoDIAL.Value = 1 Then

      '  Inet1.URL = "ftp://137.79.98.116/home/mahoney/"
      '  Inet1.Password = "martin3"
      '  Inet1.UserName = "mahoney"
      '  Inet1.Execute , "SEND C:\MTP\Density\NDprofile.txt NDprofile.txt"

        Inet1.URL = txtURL.Text
        Inet1.Password = txtPassword.Text
        Inet1.UserName = txtUsername.Text
        Inet1.Execute , "SEND C:\MTP\Setup\Density\NDprofile.txt /pub/mtp/NDprofile.txt"
      End If
    End Select
  End If
  
  
  
  
'  Debug.Print Nlev; Nlev1; Nlev2
' MAKEword Bitmap
' NavQual          1
' TAqual           2
' CtsQual          4b
' CycleQual        8
' TtgtFix         16
' RAWbad          32

  MakeWord = 0
  EditWord = 0
'  GoodScan = True

  If Not NavQual Then MakeWord = 1: NavQualPC = NavQualPC + 1
  If Not TAqual Then MakeWord = MakeWord Or 2: TAqualPC = TAqualPC + 1
  If Not CtsQual Then
    MakeWord = MakeWord Or 4: CtsQualPC = CtsQualPC + 1
  End If
  If Not CycleQual Then
    MakeWord = MakeWord Or 8: CycleQualPC = CycleQualPC + 1: GoodScan = False
  End If
  If TtgtFix Then MakeWord = MakeWord Or 16: TtgtPC = TtgtPC + 1
  If Trop1 >= 30 Then MakeWord = MakeWord Or 32: RAWbadPC = RAWbadPC + 1: GoodTrop = False
  
  With frmFLTINFO
    .txtPC(9).Text = Str$(NavQualPC)
    .txtPC(10).Text = Str$(TAqualPC)
    .txtPC(11).Text = Str$(CtsQualPC)
    .txtPC(12).Text = Str$(CycleQualPC)
'   .txtPC(12).Text = Str$(RAWbadPC)
  End With
  DoEvents
  If REFlu > 0 Then  'And Nlev1 <> Nlev2 Then
  
    'Tom additions
    Dim tblu%
    tblu% = FreeFile
    Open "C:\MTP\TBal.txt" For Append As tblu%
    Print #tblu%, UTsec, TimeString(UTsec), Format(pALT, "#0.000"), Latitude, Longitude, CV(1, 6, RawCycle), CV(2, 6, RawCycle), CV(3, 6, RawCycle), GoodScan
    Close (tblu%)
    
    Call REFwrite(REFlu%, Cycle%)
'  Else
'    Cycle = Cycle - 1
  End If
Return

ErrorHandler:
Stop

End Sub

Sub ReadArrayNextAline(Record%)
' Read Next A line from BIN file

Dim A As REFrecord, i%, j%, x!, ALTcor!

  If Record = TotalCycles Then Exit Sub
'  UTsec = UV(1, Record + 1)   'enabling this line screws up UTsec
  TTO = TTOV(0, Record)
  TTO1 = TTOV(1, Record)
  TTO2 = TTOV(2, Record)
  TTO3 = TTOV(3, Record)
  
  NpALT = ALTV(1, Record + 1)                      'pALTnav
  nOATn = OATV(1, Record + 1) - cTo                'OATnav (C)
' Following code note needed with substituted files because OATnav is set equal to OATmms
'  If chkUseSubstitutedRAWfile.Value = 1 Then
'    If chkUseT.Value = 1 Then nOATn = OATV(2, Record + 1) - cTo   'OATmms
'    If chkUseP.Value = 1 Then NpALT = ALTV(4, Record + 1)         'pALTmms
'  End If
  
  If AC$ = "ER" And (YYYYMMDD$ = "20000109" Or YYYYMMDD$ = "20000203") Then
    NpALT = fPtoZ(fZtoP(NpALT) - 3.51)  'Correct Nav pressure to agree with MMS which is missing
  End If
  NpALTft = NpALT * cft_km
  nOATnav = nOATn + cTo         'NB nOATn changes if met file is present (see below)
  
  If nOATn < -130# Or nOATn > 100# Then nOATn = 999.9
  ' Check that OATn is available, else assume on ground and use Twin
  ' Need this for DADStgt gain calibration 971020 mjm
  ' Set Twin to 15 C for first cycle since MUX data is decoded later.
  If nOATn > 90 Then
    If Cycle% > 0 Then nOATn = Twin Else nOATn = 15
  End If
  nOAT = cTo + nOATn + OATnavCOR
  If nOAT > 999.9 Then nOAT = 999.9
' Changed from OATnav = (OATnav + nOAT) / 2, to
'  OAT = nOAT
'  OATnav = nOATnav  ' Because MTP OAT is ~14 second ahead of Nav OAT, see tto above
' mjm 20040528 - removed above two lines because this is taken into account by NAVoffset
  
  If NpALT > Ceiling Then NpALT = 99.9
  If NpALT < 0# Then NpALT = 0#: NpALTft = 0#

' Fix pALT if necessary
  If NpALT = 99.9 Then
    NpALTft = pALT   'was pALTlast
    NALTft = NpALTft
    NALTkm = NpALTft / cft_km
  Else
    If NpALTft < 0 Then NpALTft = 1
    NALTkm = NpALTft / cft_km
    NALTkmu = NALTkm
    'ALTcor = .046 - .000437 * ALTkmu ^ 2 + 3.315E-05 * (ALTkmu ^ 3)
    NALTkm = NALTkmu + ALTfujCONST * ALTcor
    NALTft = Int(NALTkm * cft_km + 0.5)
  End If
  NpALT = NALTkm         'Have some sort of valid altitude at this point
'  Debug.Print pALT - NpALT; OATnav - NOAT; DPmax; DTmax
'  WB57: DPmax=0.45 km, DTmax=4.0 K
  ' Calculate average values at horizon position
  ' NB 1 sec on long scans and at target, and 13 s at 10 elevation angles
  
' pALT = (pALT + NpALT) / 2 '(1 + 6 * (16 - 3) / 10) / 16=0.55
' Previous line averaged before and after pressure altitudes. Now just get pALT at NAVoffset
  pALT = (ALTV(1, Record) + ALTV(1, Record + 1)) / 2# 'fGetNavZp(Record)   'Use NAV pressure altitude at NAVoffset
'  Tac = OATV(1, Record + 1)        'K  MTP T

End Sub

Sub ReadArrayLeadLag(Record%)
Dim A As REFrecord, i%, j%, x!, r%, n%, OAT1!, OAT2!, CS1!, CS2!, num!, Denom!
' Record 1 is FI record
' Record 2 is Limits record
' Record 3, 4 and 5 are spares
' Now deal with Ttgt and Tmxr lead and lags. NB These arrays use ALL raw cycles
' MV() Map for NG vs other instruments
'  Other                NGV
'  MV(17, R) = TTMA = 49
'  MV(18, R) = TMMA = 50
'  MV(19, R) = TWMA = 51
'  MV(0, R)  = Ttgt = 0
'  MV(1, R)  = Tnd  = 23
'  MV(4, R)  = Tifa = 21
'  MV(8, R)  = Twin = 20


'If Left$(SU$, 3) = "CAN" Then
  If UseMAforTifa Then
    Tifa = MV(50, fRecordCheck(Record, MXRoffset))
  Else
    Tifa = MV(21, fRecordCheck(Record, MXRoffset))
  End If
  Muxs(21) = Tifa
  
  If UseMAforTtgt Then
    Ttgt = MV(49, fRecordCheck(Record, TGToffset))
  Else
    Ttgt = MV(0, fRecordCheck(Record, TGToffset))
  End If
  Muxs(0) = Ttgt
  Tbase = cTo + Ttgt                        'Convert to Kelvin
  
  Twin = MV(20, Record)
  Muxs(20) = Twin                            'Save averaged Twin

  Tnd = MV(23, fRecordCheck(Record, NDoffset))

' Calculate OAT to use
' OATsource: 1=MTP, 2=A/C, 3=MMS
  Select Case OATsource
  Case 1   'The MTP OAT is normally based on archival data assuming a stable ND source
    Call GainND(g())
    num = 0#: Denom = 0#
    For i = 1 To Channels
      num = num + ChInfo(i) * fTB(Tbase, C(i, LocHor), CB(i), g(i))
      Denom = Denom + ChInfo(i)
    Next i
    OAT = num / Denom + OATnavCOR
    'Debug.Print Record; "Tmtp-Tnav= "; OAT - fGetNavT(Record, 0)
  Case 2
    OAT = fGetNavT(Record, 0)               'OATnav + OATnavCOR interpolated by aNAVoffset
  Case 3
    ' Not needed anymore because MMS (or other PI T and P) are substituted into RAW file
  End Select
  
' Set base (reference target) counts
  If UseMAforCB Then                        'Smoothed Base counts
    For i = 1 To Channels
      CB(i) = EV(i, 12, Record)             'Smoothed base counts
      If Record > 1 Then
        CBL(i) = EV(i, 12, Record - 1)      'Base counts for last record
      Else
        CBL(i) = EV(i, 12, 1)
      End If
    Next i
    'Debug.Print Record; CB(1); CB(2); CB(3)
  Else                                      'Actual base counts
    For i = 1 To Channels
      CB(i) = CV(i, 12, Record)
      If Record > 1 Then
        CBL(i) = CV(i, 12, Record - 1)
      Else
        CBL(i) = CV(i, 12, 1)
      End If
    Next i
  End If

' Set Noise Diode Deflection counts (dND)
  If UseMAforCN Then
    For i = 1 To Channels
      dND(i) = EV(i, 11, Record) - EV(i, 12, Record)
      CN(i) = dND(i)
      If Record > 1 Then
        CNL(i) = EV(i, 11, Record - 1) - EV(i, 12, Record - 1)
      Else
        CNL(i) = EV(i, 11, 1) - EV(i, 12, 1)
      End If
    Next i
  Else
    For i = 1 To Channels
      dND(i) = CV(i, 11, Record) - CV(i, 12, Record)
      CN(i) = dND(i)
      If Record > 1 Then
        CNL(i) = CV(i, 11, Record - 1) - CV(i, 12, Record - 1)
      Else
        CNL(i) = CV(i, 11, 1) - CV(i, 12, 1)
      End If
    Next i
  End If
  

      For i = 1 To Channels
        For j = 1 To 10
          CSMA(i, j) = EV(i, j, Record)
          If Record > 1 Then
            CSMAL(i, j) = EV(i, j, Record - 1)
          Else
            CSMAL(i, j) = EV(i, j, Record)
          End If
        Next j
      Next i
      For i = 1 To Channels
        For j = 1 To 10
          CS(i, j) = CV(i, j, Record)
          If Record > 1 Then
            CSL(i, j) = CV(i, j, Record - 1)
          Else
            CSL(i, j) = CV(i, j, Record)
          End If
        Next j
      Next i
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Exit Sub
'Else
  If UseMAforTifa Then
    Tifa = MV(18, fRecordCheck(Record, MXRoffset))
  Else
    Tifa = MV(4, fRecordCheck(Record, MXRoffset))
  End If
  Muxs(4) = Tifa
  
  If UseMAforTtgt Then
    Ttgt = MV(17, fRecordCheck(Record, TGToffset))
  Else
    Ttgt = MV(0, fRecordCheck(Record, TGToffset))
  End If
  Muxs(0) = Ttgt
  Tbase = cTo + Ttgt                        'Convert to Kelvin
  
  Twin = MV(8, Record)
  Muxs(8) = Twin                            'Save averaged Twin

  Tnd = MV(1, fRecordCheck(Record, NDoffset))

' Calculate OAT to use
' OATsource: 1=MTP, 2=A/C, 3=MMS
  Select Case OATsource
  Case 1   'The MTP OAT is normally based on archival data assuming a stable ND source
    Call GainND(g())
    num = 0#: Denom = 0#
    For i = 1 To Channels
      num = num + ChInfo(i) * fTB(Tbase, C(i, LocHor), CB(i), g(i))
      Denom = Denom + ChInfo(i)
    Next i
    OAT = num / Denom + OATnavCOR
    'Debug.Print Record; "Tmtp-Tnav= "; OAT - fGetNavT(Record, 0)
  Case 2
    OAT = fGetNavT(Record, 0)               'OATnav + OATnavCOR interpolated by aNAVoffset
  Case 3
    ' Not needed anymore because MMS (or other PI T and P) are substituted into RAW file
  End Select
  
' Set base (reference target) counts
  If UseMAforCB Then                        'Smoothed Base counts
    For i = 1 To Channels
      CB(i) = EV(i, 12, Record)             'Smoothed base counts
      If Record > 1 Then
        CBL(i) = EV(i, 12, Record - 1)      'Base counts for last record
      Else
        CBL(i) = EV(i, 12, 1)
      End If
    Next i
    'Debug.Print Record; CB(1); CB(2); CB(3)
  Else                                      'Actual base counts
    For i = 1 To Channels
      CB(i) = CV(i, 12, Record)
      If Record > 1 Then
        CBL(i) = CV(i, 12, Record - 1)
      Else
        CBL(i) = CV(i, 12, 1)
      End If
    Next i
  End If

' Set Noise Diode Deflection counts (dND)
  If UseMAforCN Then
    For i = 1 To Channels
      dND(i) = EV(i, 11, Record) - EV(i, 12, Record)
      CN(i) = dND(i)
      If Record > 1 Then
        CNL(i) = EV(i, 11, Record - 1) - EV(i, 12, Record - 1)
      Else
        CNL(i) = EV(i, 11, 1) - EV(i, 12, 1)
      End If
    Next i
  Else
    For i = 1 To Channels
      dND(i) = CV(i, 11, Record) - CV(i, 12, Record)
      CN(i) = dND(i)
      If Record > 1 Then
        CNL(i) = CV(i, 11, Record - 1) - CV(i, 12, Record - 1)
      Else
        CNL(i) = CV(i, 11, 1) - CV(i, 12, 1)
      End If
    Next i
  End If
  
'  If NAVoffset - Int(NAVoffset) <> 0 Then  'Interpolate Sky Counts for NAVoffset
'    n = Int(NAVoffset)
'    If UseMAforCS Then                        'Sky Counts
'      For i = 1 To Channels
'        CS1 = EV(i, LocHor, fRecordCheck(Record, n))
'        CS2 = EV(i, LocHor, fRecordCheck(Record, n + 1))
'        CS(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'        CS1 = EV(i, LocHor + 1, fRecordCheck(Record, n))
'        CS2 = EV(i, LocHor + 1, fRecordCheck(Record, n + 1))
'        CSP(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'        CS1 = EV(i, LocHor - 1, fRecordCheck(Record, n))
'        CS2 = EV(i, LocHor - 1, fRecordCheck(Record, n + 1))
'        CSM(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'      Next i
'    Else
'      For i = 1 To Channels
'        CS1 = CV(i, LocHor, fRecordCheck(Record, n))
'        CS2 = CV(i, LocHor, fRecordCheck(Record, n + 1))
'        CS(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'        CS1 = CV(i, LocHor + 1, fRecordCheck(Record, n))
'        CS2 = CV(i, LocHor + 1, fRecordCheck(Record, n + 1))
'        CSP(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'        CS1 = CV(i, LocHor - 1, fRecordCheck(Record, n))
'        CS2 = CV(i, LocHor - 1, fRecordCheck(Record, n + 1))
'        CSM(i) = CS1 + (CS2 - CS1) * (NAVoffset - n)
'      Next i
'    End If
'  Else
'    If UseMAforCS Then                        'Sky Counts
      For i = 1 To Channels
        For j = 1 To 10
          CSMA(i, j) = EV(i, j, Record)
          If Record > 1 Then
            CSMAL(i, j) = EV(i, j, Record - 1)
          Else
            CSMAL(i, j) = EV(i, j, Record)
          End If
'         CSP(i) = EV(i, LocHor + 1, Record)
'         CSM(i) = EV(i, LocHor - 1, Record)
        Next j
      Next i
'    Else
      For i = 1 To Channels
        For j = 1 To 10
          CS(i, j) = CV(i, j, Record)
          If Record > 1 Then
            CSL(i, j) = CV(i, j, Record - 1)
          Else
            CSL(i, j) = CV(i, j, Record)
          End If
'         CSP(i) = CV(i, LocHor + 1, Record)
'         CSM(i) = CV(i, LocHor - 1, Record)
        Next j
      Next i
'    End If
'  End If
'End If

End Sub


Sub NoMoreInput()
Dim ProgramDrive$, INPfile$, OUTfile$
  TotalCycles% = Cycle%
  Tstamp = Date + Time
  REFstamp = Date + Time
  frmFLTINFO.UpdateForm
  frmFLTINFO.UpdateFLTINFO
  Close         'Close all open files
  lblProgress.Caption = "REF file created"
  DoEvents
  
  If DoAll Then
    Unload frmATP
    Unload frmFormatATP
    Unload frmStatus
    Unload frmNDP
    'Reset DoAll in every program; Can only be set TRUE in MTPman in default FLTINFO
    If Not ShowDiagnostics Then cmdREF_Click (6) 'mnuWriteFLTINFO_Click  'DoAll set FALSE in mnuWriteFLTINFO_Click
    Unload Me
  Else
    If Not ShowDiagnostics Then cmdREF_Click (6) 'mnuWriteFLTINFO_Click  'DoAll set FALSE in mnuWriteFLTINFO_Click
  End If
End Sub

Sub UpdateGainWindow(Refresh As Boolean)
Static GVtop!, GVleft!, GVwidth!, GVheight!, GVshift!
Dim y!, GV1!, GV2!, GV3!, i%, j%, k%, LineC&
' Only called from Main in data processing

If Not ShowDiagnostics Then Exit Sub
If Refresh And GVindex > 0 Then GoTo Redraw

' Save Engineering and Other data for real time diagnostics
If GVindex = 0 Then 'Save initial state
  Refresh = True
  picGain.Cls
  picGain.ScaleMode = 0
  picGain.ScaleTop = Val(cboGAIN2.Text)
  picGain.ScaleLeft = 1
  picGain.ScaleHeight = Val(cboGAIN1.Text) - Val(cboGAIN2.Text)
  picGain.ScaleWidth = Val(TotalCycles)
  GVshift = picGain.ScaleWidth / 5
  GVtop = picGain.ScaleTop
  GVleft = picGain.ScaleLeft
  GVheight = picGain.ScaleHeight
  GVwidth = picGain.ScaleWidth
' Draw tic marks
  For i = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step 1
    picGain.PSet (GVleft, i)
    picGain.Line -(GVleft + 0.02 * GVwidth, i), QBColor(15)
    picGain.PSet (GVleft + GVwidth, i)
    picGain.Line -(GVleft + 0.98 * GVwidth, i), QBColor(15)
  Next i
End If
  
'If Refresh Then GoTo Redraw
  
' Reset everything if anything changed
  picGain.ScaleTop = Val(cboGAIN2.Text)
  picGain.ScaleLeft = 1
  picGain.ScaleHeight = Val(cboGAIN1.Text) - Val(cboGAIN2.Text)
  picGain.ScaleWidth = Val(TotalCycles)

  If picGain.ScaleTop <> GVtop Or picGain.ScaleLeft <> GVleft Or picGain.ScaleHeight <> GVheight Or picGain.ScaleWidth <> GVwidth Then
    picGain.Cls
    GVtop = picGain.ScaleTop
    GVleft = picGain.ScaleLeft
    GVheight = picGain.ScaleHeight
    GVwidth = picGain.ScaleWidth
    GVshift = picGain.ScaleWidth / 5
    ' Draw tic marks
    For i = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step 1
      picGain.PSet (GVleft, i)
      picGain.Line -(GVleft + 0.02 * GVwidth, i), QBColor(15)
      picGain.PSet (GVleft + GVwidth, i)
      picGain.Line -(GVleft + 0.98 * GVwidth, i), QBColor(15)
    Next i
  ' Redraw entire screen
    GoTo Redraw
  End If

' Save new data and shift if necessary
  GVindex = GVindex + 1
  
  If GVindex > TotalCycles Then 'Array limit exceeded, so shift data
    For i = 0 To 6
      Select Case i
      Case 0: GV1 = g(1): GV2 = g(2): GV3 = g(3)              'Gain Used
      Case 1: GV1 = Geqn(1): GV2 = Geqn(2): GV3 = Geqn(3)     'Gain Equation
      Case 2: GV1 = Gnd(1): GV2 = Gnd(2): GV3 = Gnd(3)        'Noise Diode
      Case 3: GV1 = Gnav(1): GV2 = Gnav(2): GV3 = Gnav(3)     'MMS
      Case 4: GV1 = Gnav(1): GV2 = Gnav(2): GV3 = Gnav(3)     'NAV
      Case 5: 'Spare
      Case 6: GV1 = pALT: GV2 = OAT
'        GV1 = picGain.ScaleTop - picGain.ScaleHeight + pALT * picGain.ScaleHeight / 25
'        GV2 = picGain.ScaleTop - picGain.ScaleHeight + OAT * picGain.ScaleHeight / 150
'        GV3 = 0#
      End Select
      
      For j = 1 To TotalCycles - GVshift
        GV(i, 1, j) = GV(i, 1, j + GVshift)
        GV(i, 2, j) = GV(i, 2, j + GVshift)
        If Channels > 2 Then GV(i, 3, j) = GV(i, 3, j + GVshift)
      Next j
      GV(i, 1, TotalCycles - GVshift + 1) = GV1
      GV(i, 2, TotalCycles - GVshift + 1) = GV2
      If Channels > 2 Then GV(i, 3, TotalCycles - GVshift + 1) = GV3
    Next i
    GVindex = TotalCycles - GVshift + 1

' Redraw beginning of shifted plot
Redraw:
    picGain.Cls
    For i = 0 To 6
      If chkGain(i).Value = 1 Then
        Select Case i
        Case 0: LineC = QBColor(White)
        Case Else: LineC = QBColor(i) 'RED
        End Select
        chkGain(i).ForeColor = LineC
          
        y = fFixP(i, GV(i, 1, GVleft))
        If i = 6 Then LineC = &HFFFF00   'Cyan
        picGain.PSet (GVleft, y)
        For j = GVleft + 1 To GVindex
          y = fFixP(i, GV(i, 1, j))
          picGain.Line -(j, y), LineC  'And continue from there
        Next j
          
        y = fFixT(i, GV(i, 2, GVleft))
        If i = 6 Then LineC = &HFFFF&     'Yellow
        picGain.PSet (GVleft, y)
        For j = GVleft + 1 To GVindex
          y = fFixT(i, GV(i, 2, j))
          picGain.Line -(j, y), LineC  'And continue from there
        Next j
        If Channels = 3 Then
          picGain.PSet (GVleft, GV(i, 3, GVleft))
          For j = GVleft + 1 To GVindex
            picGain.Line -(j, GV(i, 3, j)), LineC  'And continue from there
          Next j
        End If
      End If
    Next i
  ' Draw tic marks
    For k = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step 1
      picGain.PSet (GVleft, k)
      picGain.Line -(GVleft + 0.02 * GVwidth, k), QBColor(15)
    Next k
    For k = Val(cboGAIN1.Text) To Val(cboGAIN2.Text) Step (Val(cboGAIN2.Text) - Val(cboGAIN1.Text)) / 4
      picGain.PSet (GVleft + GVwidth, k)
      picGain.Line -(GVleft + 0.98 * GVwidth, k), QBColor(15)
    Next k
    Exit Sub
    
  Else   'Otherwise just plot latest point
    For i = 0 To 6
      Select Case i
      Case 0: GV1 = g(1): GV2 = g(2): GV3 = g(3)              'Gain Used
      Case 1: GV1 = Geqn(1): GV2 = Geqn(2): GV3 = Geqn(3)     'Gain Equation
      Case 2: GV1 = Gnd(1): GV2 = Gnd(2): GV3 = Gnd(3)        'Noise Diode
      Case 3: GV1 = Gnav(1): GV2 = Gnav(2): GV3 = Gnav(3)     'Nav
      Case 4: GV1 = Gnav(1): GV2 = Gnav(2): GV3 = Gnav(3)     'Spare
      Case 5: 'Spare
      Case 6: GV1 = pALT: GV2 = OAT
'              GV1 = picGain.ScaleTop - picGain.ScaleHeight + pALT * picGain.ScaleHeight / 25
'        GV2 = picGain.ScaleTop - picGain.ScaleHeight + OAT * picGain.ScaleHeight / 150
'        GV3 = 0#
      End Select
      
      GV(i, 1, GVindex) = GV1
      GV(i, 2, GVindex) = GV2
      If Channels = 3 Then GV(i, 3, GVindex) = GV3
    
      If chkGain(i).Value = 1 Then
        Select Case i
        Case 0: LineC = QBColor(White)
        Case Else: LineC = QBColor(i) 'RED
        End Select
        chkGain(i).ForeColor = LineC

        If Refresh Then  'Draw entire line
          y = fFixP(i, GV(i, 1, 1))
          If i = 6 Then LineC = &HFFFF00   'Cyan
          picGain.PSet (GVleft, y)
          For j = GVleft + 1 To GVindex
            y = fFixP(i, GV(i, 1, j))
            picGain.Line -(j, y), LineC  'And continue from there
          Next j
          
          y = fFixT(i, GV(i, 2, 1))
          If i = 6 Then LineC = &HFFFF&     'Yellow
          picGain.PSet (GVleft, y)
          For j = GVleft + 1 To GVindex
            y = fFixT(i, GV(i, 2, j))
            picGain.Line -(j, y), LineC  'And continue from there
          Next j
          
          If Channels = 3 Then
            picGain.PSet (GVleft, GV(i, 3, 1))
            For j = GVleft + 1 To GVindex
              picGain.Line -(j, GV(i, 3, j)), LineC  'And continue from there
            Next j
          End If
        Else
          For j = 1 To Channels
            If i = 6 Then
              Select Case j
              Case 1: y = fFixP(i, GV(i, j, GVindex - 1)): LineC = &HFFFF00
              Case 2: y = fFixT(i, GV(i, j, GVindex - 1)): LineC = &HFFFF&     'Yellow
              Case 3: y = GV(i, j, GVindex - 1)
              End Select
            Else
              y = GV(i, j, GVindex - 1)
            End If
            picGain.PSet (GVindex - 1, y)
            picGain.Line -(GVindex, y), LineC  'And continue from there
          Next j
        End If
      Else
        chkGain(i).ForeColor = QBColor(Black)
      End If
      DoEvents
    Next i
  End If

End Sub
Function fFixP!(i%, y!)
If i = 5 Then
  fFixP = picGain.ScaleTop + (24 - y) * picGain.ScaleHeight / 24
Else
  fFixP = y
End If
End Function
Sub UpdateMUXwindow(Refresh As Boolean)
Dim i%, j%, k%, LineC&
Static MVtop!, MVleft!, MVwidth!, MVheight!, MVshift!

If Refresh Then GoTo Redraw

' Save Engineering and Other data for real time diagnostics
If MVindex = 0 Then 'Save initial state
  Refresh = True
  picMUX.ScaleMode = 0
  picMUX.ScaleTop = Val(cboMUX2.Text)
  picMUX.ScaleLeft = 1
  picMUX.ScaleHeight = Val(cboMUX1.Text) - Val(cboMUX2.Text)
  picMUX.ScaleWidth = Val(TotalCycles)
  MVshift = picMUX.ScaleWidth / 5
  MVtop = picMUX.ScaleTop
  MVleft = picMUX.ScaleLeft
  MVheight = picMUX.ScaleHeight
  MVwidth = picMUX.ScaleWidth
' Draw tic marks
  For i = Val(cboMUX1.Text) To Val(cboMUX2.Text) Step 10
    picMUX.PSet (MVleft, i)
    picMUX.Line -(MVleft + 0.02 * MVwidth, i), QBColor(15)
    picMUX.PSet (MVleft + MVwidth, i)
    picMUX.Line -(MVleft + 0.98 * MVwidth, i), QBColor(15)
  Next i
End If
  
' Reset everything if anything changed
  picMUX.ScaleTop = Val(cboMUX2.Text)
  picMUX.ScaleLeft = 1
  picMUX.ScaleHeight = Val(cboMUX1.Text) - Val(cboMUX2.Text)
  picMUX.ScaleWidth = TotalCycles 'Val(txtSamples.Text)

  If picMUX.ScaleTop <> MVtop Or picMUX.ScaleLeft <> MVleft Or picMUX.ScaleHeight <> MVheight Or picMUX.ScaleWidth <> MVwidth Then
    picMUX.Cls
    MVtop = picMUX.ScaleTop
    MVleft = picMUX.ScaleLeft
    MVheight = picMUX.ScaleHeight
    MVwidth = picMUX.ScaleWidth
    MVshift = picMUX.ScaleWidth / 5
    ' Draw tic marks
    For i = Val(cboMUX1.Text) To Val(cboMUX2.Text) Step 10
      picMUX.PSet (MVleft, i)
      picMUX.Line -(MVleft + 0.02 * MVwidth, i), QBColor(15)
      picMUX.PSet (MVleft + MVwidth, i)
      picMUX.Line -(MVleft + 0.98 * MVwidth, i), QBColor(15)
    Next i
  ' Redraw entire screen
    GoTo Redraw
  End If

' Save new data and shift if necessary
  MVindex = MVindex + 1
  
'  Debug.Print MVindex
  If MVindex > TotalCycles Then 'Array limit exceeded, so shift data
    For i = 0 To MVchannels
      For j = 1 To TotalCycles - MVshift
        MV(i, j) = MV(i, j + MVshift)
      Next j
      MV(i, TotalCycles - MVshift + 1) = Muxs(i)
    Next i
    MVindex = TotalCycles - MVshift + 1
    ' Redraw beginning of shifted plot
Redraw:
    picMUX.Cls
    For i = 0 To MVchannels
      If chkMux(i).Value = 1 Then
        Select Case i
        Case 0: LineC = QBColor(15)  'Ttgt editted
        Case 5: LineC = QBColor(12)  'Tlo
        Case 6: LineC = QBColor(9)   'Thi
        Case Is < 16: LineC = QBColor(i) 'RED
        Case 16: LineC = QBColor(13) 'RED
        Case 17: LineC = QBColor(14) 'OAT
        Case 18: LineC = QBColor(15) 'Palt [km]
        End Select
        chkMux(i).ForeColor = LineC
        picMUX.PSet (MVleft, MV(i, MVleft))
        For j = MVleft + 1 To MVindex
          picMUX.Line -(j, MV(i, j)), LineC   'And continue from there
        Next j
      End If
    Next i
  ' Draw tic marks
    For i = Val(cboMUX1.Text) To Val(cboMUX2.Text) Step 10
      picMUX.PSet (MVleft, i)
      picMUX.Line -(MVleft + 0.02 * MVwidth, i), QBColor(15)
      picMUX.PSet (MVleft + MVwidth, i)
      picMUX.Line -(MVleft + 0.98 * MVwidth, i), QBColor(15)
    Next i
    Exit Sub
    
  Else   'Otherwise just plot latest point
    For i = 0 To MVchannels
      MV(i, MVindex) = Muxs(i)
      If chkMux(i).Value = 1 Then
        Select Case i
        Case 0: LineC = QBColor(15)  'Ttgt editted
        Case 5: LineC = QBColor(12)  'Tlo
        Case 6: LineC = QBColor(9)   'Thi
        Case Is < 16: LineC = QBColor(i) 'RED
        Case 16: LineC = QBColor(13) 'RED
        Case 17: LineC = QBColor(14) 'OAT
        Case 18: LineC = QBColor(15) 'Palt [km]
        End Select
        chkMux(i).ForeColor = LineC

        If Refresh Then  'Draw entire line
          picMUX.PSet (MVleft, MV(i, 1))
          For j = MVleft + 1 To MVindex
            picMUX.Line -(j, MV(i, j)), LineC   'And continue from there
          Next j
        Else
          picMUX.PSet (MVindex - 1, MV(i, MVindex - 1))
          picMUX.Line -(MVindex, MV(i, MVindex)), LineC   'And continue from there
        End If
      Else
        chkMux(i).ForeColor = QBColor(Black)
      End If
      DoEvents
    Next i
  End If
End Sub




Sub UpdateRT()
Dim i%, j%

  txtDTavg.Text = DTavg
  txtDTrms.Text = DTrms
  txtMRIavg.Text = MRIavg
  txtMRIrms.Text = MRIrms
  
  Call UpdateGoodscanBox(GoodScan)

  If Not ShowDiagnostics Or RawCycle = 0 Then Exit Sub

' Write Engineering Temperatures
  For i% = 0 To MVchannels
    If i > 65 And i < 73 Then
      If MV(i, RawCycle) > 0 Then txtMux(i%).Text = 1 Else txtMux(i%).Text = 0
    Else
      txtMux(i%).Text = Format$(MV(i%, RawCycle), "##0.0")
    End If
  Next i%
' Write RAW counts
  For j% = 1 To 12
    txtCounts1(j% - 1).Text = C(1, j%)
    If CQM(1, RawCycle) And 2 ^ (j - 1) Then txtCounts1(j - 1).BackColor = &HFF& Else txtCounts1(j - 1).BackColor = &HFFFFFF
  Next j%
  
  For j% = 1 To 12
    txtCounts2(j% - 1).Text = C(2, j%)
    If CQM(2, RawCycle) And 2 ^ (j - 1) Then txtCounts2(j - 1).BackColor = &HFF& Else txtCounts2(j - 1).BackColor = &HFFFFFF
  Next j%
  If Channels = 3 Then
    For j% = 1 To 12
      txtCounts3(j% - 1).Text = C(3, j%)
      If CQM(3, RawCycle) And 2 ^ (j - 1) Then txtCounts3(j - 1).BackColor = &HFF& Else txtCounts3(j - 1).BackColor = &HFFFFFF
    Next j%
  End If

' Write Gain Information
  Call ReadArrayLeadLag(RawCycle)       'get Ttgt, Tmxr, Tnav with lead/lag
  Call GainGE(Geqn(), RawCycle)         'GE need Tmxr with lag
  Call GainND(Gnd())                    'ND
  Call GainOAT(Geqn(), Goat(), False)   'OAT
      
'      GV(5, i, r) = ALTV(1, r)
'      GV(6, i, r) = OATV(1, r)
  txtGain(18).Text = Format$(ALTV(1, RawCycle), "#0.00")
  txtGain(19).Text = Format$(OATV(1, RawCycle), "#00.0")
  txtGain(20).Text = Format$(RawCycle, "###0")

  txtGain(0).Text = Format$(g(1), "##0.0")
  txtGain(1).Text = Format$(g(2), "##0.0")
  txtGain(2).Text = Format$(g(3), "##0.0")
  
  If GeqnGood(1) Then txtGain(3).ForeColor = &H8000& Else txtGain(3).ForeColor = &H0&
  txtGain(3).Text = Format$(Geqn(1), "##0.0")
  If GeqnGood(2) Then txtGain(4).ForeColor = &H8000& Else txtGain(4).ForeColor = &H0&
  txtGain(4).Text = Format$(Geqn(2), "##0.0")
  If GeqnGood(3) Then txtGain(5).ForeColor = &H8000& Else txtGain(5).ForeColor = &H0&
  txtGain(5).Text = Format$(Geqn(3), "##0.0")
  
  If GndGood(1) Then txtGain(6).ForeColor = &H8000& Else txtGain(6).ForeColor = &H0&
  txtGain(6).Text = Format$(Gnd(1), "##0.0")
  If GndGood(2) Then txtGain(7).ForeColor = &H8000& Else txtGain(7).ForeColor = &H0&
  txtGain(7).Text = Format$(Gnd(2), "##0.0")
  If GndGood(3) Then txtGain(8).ForeColor = &H8000& Else txtGain(8).ForeColor = &H0&
  txtGain(8).Text = Format$(Gnd(3), "##0.0")
  
  If GmmsGood(1) Then txtGain(9).ForeColor = &H8000& Else txtGain(9).ForeColor = &H0&
  txtGain(9).Text = Format$(Gmms(1), "##0.0")
  If GmmsGood(2) Then txtGain(10).ForeColor = &H8000& Else txtGain(10).ForeColor = &H0&
  txtGain(10).Text = Format$(Gmms(2), "##0.0")
  If GmmsGood(3) Then txtGain(11).ForeColor = &H8000& Else txtGain(11).ForeColor = &H0&
  txtGain(11).Text = Format$(Gmms(3), "##0.0")
  
  If GnavGood(1) Then txtGain(12).ForeColor = &H8000& Else txtGain(12).ForeColor = &H0&
  txtGain(12).Text = Format$(Gnav(1), "##0.0")
  If GnavGood(2) Then txtGain(13).ForeColor = &H8000& Else txtGain(13).ForeColor = &H0&
  txtGain(13).Text = Format$(Gnav(2), "##0.0")
  If GnavGood(3) Then txtGain(14).ForeColor = &H8000& Else txtGain(14).ForeColor = &H0&
  txtGain(14).Text = Format$(Gnav(3), "##0.0")
  ForeColor = &H0&
    
  For j% = 1 To 10: txtTA1(j% - 1).Text = TA(1, j%): Next j%
  For j% = 1 To 10: txtTA2(j% - 1).Text = TA(2, j%): Next j%
  If Channels = 3 Then
    For j% = 1 To 10: txtTA3(j% - 1).Text = TA(3, j%): Next j%
  End If
    DoEvents

End Sub

Sub UpdateFlightTrack()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!
Dim LAmin!, LAmax!, LOmin!, LOmax!, r%, iStep%
'  Latitude = NAVV(3, r)
'  Longitude = NAVV(4, r)

'Plot Flight Track in picture window
  picTrack.Cls
  picTrack.ScaleMode = 0
  picTrack.ScaleTop = 0
  picTrack.ScaleHeight = 11

' Set temperature display scale
  If chkAutoscaleTrack Then
    LOmin = Int(LONmin - 1)
    LOmax = Int(LONmax + 1)
    picTrack.ScaleWidth = LOmax - LOmin
    picTrack.ScaleLeft = LOmin
    txtFTX1.Text = Format(LOmin, "##0")
    txtFTX2.Text = Format(LOmax, "##0")

    LAmin = Int(LATmin - 1)
    LAmax = Int(LATmax + 1)
    picTrack.ScaleHeight = LAmin - LAmax
    picTrack.ScaleTop = LAmax
    txtFTY1.Text = Format(LAmin, "##0")
    txtFTY2.Text = Format(LAmax, "##0")
  Else
    LOmin = Val(txtFTX1.Text)
    picTrack.ScaleLeft = LOmin
    picTrack.ScaleWidth = Val(txtFTX2.Text) - Val(txtFTX1.Text)
    LOmax = picTrack.ScaleLeft + picTrack.ScaleWidth
    
    LAmax = Val(txtFTY2.Text)
    picTrack.ScaleTop = LAmax
    picTrack.ScaleHeight = Val(txtFTY1.Text) - Val(txtFTY2.Text)
    LAmin = picTrack.ScaleTop + picTrack.ScaleHeight
  
  End If
  
' Draw Latitude-tic marks
  If LAmax - LAmin > 15 Then iStep = 2 Else iStep = 1
  txtFTY3.Text = Str(iStep)
  For i = LAmin To LAmax Step iStep
    picTrack.PSet (LOmin, i)
    picTrack.Line -(LOmax, i), QBColor(5)
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
  If LOmax - LOmin > 20 Then iStep = 2 Else iStep = 1
  txtFTX3.Text = Str(iStep)
  For i = LOmin To LOmax Step iStep
    picTrack.PSet (i, LAmin)
    picTrack.Line -(i, LAmax), QBColor(5)
  Next i

' Draw flight track
  picTrack.PSet (NAVV(4, 1), NAVV(3, 1))
  If chkHiliteSelection.Value = 1 Then
    For r = 2 To TotalCycles
      If r < FirstMouseX Or r > LastMouseX Then LineC = QBColor(9) Else LineC = QBColor(12)
      picTrack.Line -(NAVV(4, r), NAVV(3, r)), LineC  'And continue from there
    Next r
  Else
    For r = 2 To TotalCycles
      picTrack.Line -(NAVV(4, r), NAVV(3, r)), QBColor(9)  'And continue from there
    Next r
  End If
End Sub

Sub UpdateTPTrange()
Dim i%, j%, M%, m1%, m2%, n%, x1!, x2!, x3!, LineC&
Dim PA!, yScale!
Static TPTtop!, TPTleft!, TPTwidth!, TPTheight!, LastRawcycle%
  
  If Not ArrayRead And Not RealTime Then Exit Sub
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  
' Set Scales
  With picTPT
  .Cls
  .ScaleMode = 0
  .ScaleTop = Val(cboTPT2.Text)
  .ScaleLeft = m1
  .ScaleHeight = Val(cboTPT1.Text) - Val(cboTPT2.Text)
  .ScaleWidth = Val(m2 - m1)
  'If m1 = 1 Then m1 = 2

  TPTtop = .ScaleTop
  TPTleft = .ScaleLeft
  TPTheight = .ScaleHeight
  TPTwidth = .ScaleWidth
  End With
' Show position
'  If LastRawcycle = 0 Then
'    picTPT.PSet (RawCycle, TPTtop)
'    picTPT.Line -(RawCycle, TPTtop + TPTheight), QBColor(Black)
'    LastRawcycle = RawCycle
'  Else
'    picTPT.PSet (LastRawcycle, TPTtop)
'    picTPT.Line -(LastRawcycle, TPTtop + TPTheight), picTPT.BackColor
'    picTPT.PSet (RawCycle, TPTtop)
'    picTPT.Line -(RawCycle, TPTtop + TPTheight), QBColor(Black)
'    LastRawcycle = RawCycle
'  End If
' Draw X tic marks
  x1 = m1
  x2 = m2
  Select Case cboUTunit(4).Text
  Case "Rec No"
    x3 = Val(cboX3(4).Text)
  Case "UTks"
    x3 = Val(cboX3(4).Text) * TPTwidth / (Val(txtX2(4).Text) - Val(txtX1(4).Text))
  Case "Hour"
    x3 = Val(cboX3(4).Text) * TPTwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  Case "hh:mm:ss"
    x3 = Val(cboX3(4).Text) * TPTwidth / ((Val(UV(1, m2)) - Val(UV(1, m1))) / 3600)
  End Select
  
  For i = x1 To x2 Step x3
    picTPT.PSet (i, TPTtop)
    picTPT.Line -(i, TPTtop + 0.05 * TPTheight), QBColor(Black)
    picTPT.PSet (i, TPTtop + TPTheight)
    picTPT.Line -(i, TPTtop + 0.95 * TPTheight), QBColor(Black)
  Next i

' Draw Y tic marks
  For PA = Val(cboTPT1.Text) To Val(cboTPT2.Text) Step Val(cboTPT3.Text)
    picTPT.PSet (TPTleft, PA)
    picTPT.Line -(TPTleft + 0.02 * TPTwidth, PA), QBColor(Black)
    picTPT.PSet (TPTleft + TPTwidth, PA)
    picTPT.Line -(TPTleft + 0.98 * TPTwidth, PA), QBColor(Black)
  Next PA
  
  ThetaStart = Trim(cboParms(0).Text)
  ThetaEnd = Trim(cboParms(1).Text)
  ThetaStep = Trim(cboParms(2).Text)
  ThetaLevels = Int((ThetaEnd - ThetaStart)) / ThetaStep + 1
  
  If cboTPT4.Text = "km" Then yScale = 1 Else yScale = kft_km
  For j = 1 To ThetaLevels
    LineC = fMJcolor(j)
    picTPT.PSet (m1, Tptw(j, m1) * yScale)
    For M = m1 + 1 To m2
      If CQV(M) Then
        If Abs(PA) < 1000 Then picTPT.Line -(M, Tptw(j, M) * yScale), LineC 'And continue from there
      End If
    Next M
  Next j
    
End Sub

Sub UpdateX1X2()
Dim i%

  For i = 0 To 4
    Select Case cboUTunit(i).Text
    Case "Rec No"
      txtX1(i).Text = FirstMouseX
      txtX2(i).Text = LastMouseX
    Case "UTks"
      txtX1(i).Text = UV(1, FirstMouseX) / 1000
      txtX2(i).Text = UV(1, LastMouseX) / 1000
    Case "Hour"
      txtX1(i).Text = Format(UV(1, FirstMouseX) / 3600, "00.00")
      txtX2(i).Text = Format(UV(1, LastMouseX) / 3600, "00.00")
    Case "hh:mm:ss"
      txtX1(i).Text = fSecToTstring$(Int(UV(1, FirstMouseX)), False)
      txtX2(i).Text = fSecToTstring$(Int(UV(1, LastMouseX)), False)
    End Select
  Next i

End Sub

Sub WriteArray(r%)
' Put raw data/parameters into arrays in memory
Dim i%, j%, x%

  CQV(r) = True    'RAW data has not been editted yet by CalculateArrayMA
  NQV(r) = NavQual
  
  Cycle = r
  UV(1, r) = UTsec
  UV(2, r) = UTsecMTP
  UV(3, r) = UTsecNav
  
  TTOV(0, r) = TTO
  TTOV(1, r) = TTO1
  TTOV(2, r) = TTO2
  TTOV(3, r) = TTO3
  
  ALTV(1, r) = pALT
  ALTV(2, r) = gALT
  ALTV(3, r) = rALT
  ALTV(4, r) = mALT
  
  NAVV(1, r) = Pitch
  NAVV(2, r) = Roll
  NAVV(3, r) = Latitude
  NAVV(4, r) = Longitude
  
  If r > 1 Then
    If Abs(NAVV(3, r)) > 85 Then NAVV(4, r) = NAVV(4, r - 1): Longitude = NAVV(4, r)
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
  If chkShowChannels(12).Value = 1 Then OATV(1, r) = cTo + Val(txtTtgt.Text)
  OATV(2, r) = OATmms             'for CAN instruments this is OATnav (from substituted RAW file, eg .RNG)
  OATV(3, r) = OATnav + OATnavCOR 'initial OATmtp because only OATnav is known
  
  For i% = 1 To Channels
    CQM(i, r) = RFImask(i)
    For j% = 1 To 12
      CV(i%, j%, r) = C(i%, j%)
'      EV(i, j, r) = MA(i, j)
    Next j%
    CV(i, 13, r) = CV(i%, 11, r) - CV(i%, 12, r)
    CV(i, 14, r) = C(i, LocHor)    'Sky counts for SLOW averaging because normally
    If CV(i, 11, r) < 10000 Then   'sky counts are only given FAST averaging
                                   'Needed to calculate Goat
    End If
  Next i%
  
' Mux tab
  MV(0, r) = Muxs(0)
  If Left$(SU$, 3) = "CAN" Then j = 24 Else j = 16
  For i% = 1 To j
    MV(i, r) = Muxs(i%)
    MUXV(i, r) = Mux(i%)
  Next i%
  
' Nav tab
'  MV(7, R) = (Muxs(7) - Muxs(16)) / 52
  MV(25, r) = TOMA
  MV(26, r) = Zt1
  If OATnav > 170 Then
    MV(27, r) = OATnav - cTo
  Else
    MV(27, r) = OATnav
  End If
  MV(28, r) = OATmms
  MV(29, r) = OATmtp
  MV(30, r) = pALT
  MV(31, r) = gALT
  MV(32, r) = rALT
  MV(33, r) = Pitch
  MV(34, r) = Roll
  MV(35, r) = Wspd
  MV(36, r) = Wdir
  MV(37, r) = VWS
  MV(38, r) = GroundSpeed
  MV(39, r) = TAS
  MV(40, r) = Heading
  MV(41, r) = Mach
  MV(42, r) = Vzac
  MV(43, r) = Tdew
  MV(44, r) = Ttotal
  MV(45, r) = Pstatic
  MV(46, r) = Pdynamic
  MV(47, r) = SZA
  MV(48, r) = SAAC
  
' Misc tab
  MV(49, r) = TTMA
  MV(50, r) = TMMA
  MV(51, r) = TWMA
  MV(52, r) = Zt1
  MV(53, r) = LRac
  MV(54, r) = LRac2
  MV(55, r) = LRac3
  MV(56, r) = g(1)
  MV(57, r) = g(2)
  MV(58, r) = g(3)
  MV(59, r) = 0
  MV(60, r) = 0
  MV(61, r) = 0
  MV(62, r) = 0
  MV(63, r) = 0
  MV(64, r) = 0
  
' Status tab
  MV(65, r) = StatusBits And 128
  MV(66, r) = StatusBits And 64
  MV(67, r) = StatusBits And 32
  MV(68, r) = StatusBits And 16
  MV(69, r) = StatusBits And 8
  MV(70, r) = StatusBits And 4
  MV(71, r) = StatusBits And 2
  MV(72, r) = StatusBits And 1
  
End Sub

Private Sub cboAlgorithm_Click()
  
Dim i%

Algorithm = cboAlgorithm.ListIndex

'0 "Undefined"
'1 "MRI"
'2 "LAT Blend"
'3 "LON Blend"
'4 "UT Blend"
'5 "Retrievable Index"
'6 "Observable Index"
'7 "Separate Up Dn OBs"

Select Case Algorithm
Case 0
Case 1    'MRI
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = False
    frmFLTINFO.txtLAT(i).Enabled = False
  Next i
  LAT1 = 18#   'xxx mjm 19991130 These need to be set whenever MRI retrieval is used!!
  LAT2 = 28#
  LAT3 = 45#
  LAT4 = 53#
  frmFLTINFO.txtLAT(0).Text = Str$(LAT1): frmFLTINFO.txtLAT(1).Text = Str$(LAT2)
  frmFLTINFO.txtLAT(2).Text = Str$(LAT3): frmFLTINFO.txtLAT(3).Text = Str$(LAT4)

Case 2   'Lat Blend
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = True
    frmFLTINFO.txtLAT(i).Enabled = True
    frmFLTINFO.lblLAT(i).Caption = "LAT" + Format(i + 1, "0")
  Next i
  
Case 3   'LON Blend
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = True
    frmFLTINFO.txtLAT(i).Enabled = True
    frmFLTINFO.lblLAT(i).Caption = "LON" + Format(i + 1, "0")
  Next i

Case 4
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = True
    frmFLTINFO.txtLAT(i).Enabled = True
    frmFLTINFO.lblLAT(i).Caption = "UT" + Format(i + 1, "0")
  Next i

Case 5    'Retrievable Blend
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = False
    frmFLTINFO.txtLAT(i).Enabled = False
    frmFLTINFO.lblLAT(i).Caption = ""
  Next i

Case 6    'Observable Blend
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = False
    frmFLTINFO.txtLAT(i).Enabled = False
    frmFLTINFO.lblLAT(i).Caption = ""
  Next i

Case 7    'Observable Blend with Separate Up Dn"
  For i = 0 To 3
    frmFLTINFO.lblLAT(i).Enabled = False
    frmFLTINFO.txtLAT(i).Enabled = False
    frmFLTINFO.lblLAT(i).Caption = ""
  Next i

End Select

End Sub


Private Sub cboCTS1_Click()
UpdateCTSrange
End Sub

Private Sub cboCTS2_Change()
UpdateCTSrange
End Sub

Private Sub cboCTS3_Change()
UpdateCTSrange
End Sub

Private Sub cboMUX1_Click()
UpdateMUXrange
End Sub

Private Sub cboMUX2_Click()
UpdateMUXrange
End Sub

Private Sub cboMUX3_Click()
UpdateMUXrange
End Sub

Private Sub cboUT_Click()
Dim UT&

If Not BINfileRead Then Exit Sub
  
Select Case frmMTPbin.cboUT.ListIndex
Case 0: UT = Val(cboUT.Text)
Case 1: UT = Val(cboUT.Text) * 1000
Case 2: UT = fTstringToSec(cboUT.Text, True)
End Select

Select Case frmMTPbin.cboUT.Text
Case "s": frmMTPbin.txtUT(2).Text = Format(UT, "#####0")
Case "ks": frmMTPbin.txtUT(2).Text = Format(UT / 1000, "##0.000")
Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UT, True)
Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UT / 3600#, "#0.0000")
End Select

End Sub



Private Sub cboUTunit_Click(Index As Integer)
Dim i


For i = 0 To 4
cboUTunit(i) = cboUTunit(Index).Text
cboX3(i).Clear
Select Case cboUTunit(Index).Text
Case "Rec No"
  txtX1(i).Text = FirstMouseX
  txtX2(i).Text = LastMouseX
  With cboX3(i)
    .AddItem "10"
    .AddItem "20"
    .AddItem "50"
    .AddItem "100"
    .AddItem "200"
    .ListIndex = 4
  End With
  
Case "UTks"
  txtX1(i).Text = UV(1, FirstMouseX) / 1000
  txtX2(i).Text = UV(1, LastMouseX) / 1000
  With cboX3(i)
    .AddItem "0.1"
    .AddItem "0.25"
    .AddItem "0.5"
    .AddItem "1"
    .AddItem "2"
    .AddItem "4"
    .ListIndex = 4
  End With

Case "Hour"
  txtX1(i).Text = Format(UV(1, FirstMouseX) / 3600, "#0.00")
  txtX2(i).Text = Format(UV(1, LastMouseX) / 3600, "#0.00")
  With cboX3(i)
    .AddItem "0.05"
    .AddItem "0.1"
    .AddItem "0.25"
    .AddItem "0.5"
    .AddItem "1"
    .AddItem "2"
    .ListIndex = 4
  End With

Case "hh:mm:ss"
  txtX1(i).Text = fSecToTstring$(Int(UV(1, FirstMouseX)), True)
  txtX2(i).Text = fSecToTstring$(Int(UV(1, LastMouseX)), True)
  With cboX3(i)
    .AddItem "0.05"
    .AddItem "0.1"
    .AddItem "0.25"
    .AddItem "0.5"
    .AddItem "1"
    .AddItem "2"
    .ListIndex = 4
  End With
End Select
Next i
UpdateCTSrange
UpdateMUXrange
End Sub


Private Sub chkAutoscaleTrack_Click()
  UpdateFlightTrack
End Sub






Private Sub chkBiasMode_Click()
  If chkBiasMode.Value = 1 Then
    BiasMode = 2
  Else
    BiasMode = 1
  End If

End Sub

Private Sub chkHiliteSelection_Click()
  UpdateFlightTrack
End Sub



Private Sub chkNewCalFormat_Click()

  If chkNewCalFormat.Value = 1 Then
    CalFileFormat = 1
  Else
    CalFileFormat = 0
  End If
End Sub


Private Sub chkPlotBadEls_Click()
Static CTS1save%, CTS2save%, CTS3save%
Dim CTS1%, CTS2%, CTS3%

If chkPlotBadEls.Value = 1 Then
  CTS1save = Val(cboCTS1.Text)
  CTS2save = Val(cboCTS2.Text)
  CTS3save = Val(cboCTS3.Text)
  CTS1 = 0
  CTS2 = (Channels + 3) * (Nel + 2) + 1
  CTS3 = (Channels + 3)
  cboCTS1.Text = Str(CTS1)
  cboCTS2.Text = Str(CTS2)
  cboCTS3.Text = Str(CTS3)
Else
  cboCTS1.Text = Str(CTS1save)
  cboCTS2.Text = Str(CTS2save)
  cboCTS3.Text = Str(CTS3save)
End If
End Sub


Private Sub chkRemoveOBbias_Click()

  If chkRemoveOBbias.Value = 1 Then
    RemoveOBbias = True
  Else
    RemoveOBbias = False
  End If
  
End Sub





Private Sub cboGOTO_Change()
UpdateForm
End Sub

Private Sub cboSource_Click()
Dim Filename$
' Source$ dates back to QuickBasic! and was used to control program flow when the QuickBasic program also
' did real time retrievals on the DC8 and WB57 using data directly from a serial port. Now it is always
' assumed that real time data is put in a file C:\mtp\rta\raw\MTPcurrent.raw
' So COM port data no longer reaches this program, and real time simulation got put into MTPsetup
' to avoid timing issues. There are really only two analysis modes now: real time and offline
' the latter being the normal post mission analysis mode. In real time mode the data will normally come from
' MTPcurrent.RAW, however, that file might not be in the C:\mtp\rta\raw\ folder (e.g., it might be on an FTP
' server somewhere)

  Select Case cboSource.ListIndex
  Case 0
    Source$ = "F"  'Normal data processing
  Case 1
    Source$ = "D"  'Data Unit COM port
  Case 2
    Source$ = "T"  'Test file
  Case 3
    Source$ = "S"  'Realtime Simulation file
    RawCycle = 1
  End Select

End Sub






Private Sub chkUseSmatrix_Click()

  If chkUseSmatrix.Value = 1 Then
    UseSmatrix = True
  Else
    UseSmatrix = False
  End If

End Sub

Private Sub chkUseSubstitutedRAWfile_Click()
If chkUseSubstitutedRAWfile.Value = 1 Then
  cmdREF(0).Caption = "&Open R" + Trim(cboPTfile.Text)
  frmFLTINFO.txtRAWextension.Text = "R" + Trim(cboPTfile.Text)
Else
  cmdREF(0).Caption = "&Open RAW"
  frmFLTINFO.txtRAWextension.Text = "RAW"
End If
End Sub

Private Sub cmdAssorted_Click(Index As Integer)

Select Case Index
Case 0
Dim RAOBlu%, i%, ans As Variant, OutRecord&, HHMMSS$, W$ ' As Variant
  
'  Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
'  Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
'  Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
'  Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
'  Call FindXmin(jt1, jt2, RTavg(), RAavg(), 6#, 19#, Tcp, Zcp)
'  If RAOBgood Then RAOBedit = &H1
'  If TropGood Then RAOBedit = RAOBedit + &H2
'  If EditLevel Then RAOBedit = RAOBedit + &H4
'  If EditTrop Then RAOBedit = RAOBedit + &H8
'  If UseEditLevel Then RAOBedit = RAOBedit + &H10
'  If UseEditTrop Then RAOBedit = RAOBedit + &H20
'  RAOBedit = 3
  RAOBgood = True
  TropGood = True
  EditLevel = False
  EditTrop = False
  UseEditLevel = False
  UseEditTrop = False
  WMOnumber = 99999
  WMO4l = "XXXX"
  HHMMSS$ = fSecToTstring(UTsec, False)
  'DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  Iyear = Val(Left$(YYYYMMDD$, 4))
  Imonth = Val(Mid$(YYYYMMDD$, 5, 2))
  Iday = Val(Right$(YYYYMMDD$, 2))
  Ihour = Val(Left$(HHMMSS$, 2))
  Imin = Val(Mid$(HHMMSS$, 3, 2))
  Isec = Val(Right$(HHMMSS$, 2))
  
  Nlevr = Nlev
  Nedit = 0
  dTMRI = 0
  ijkl = 0
  OATraob = OATmtp
  OATraobPP = 0
  WMOlatitude = Latitude
  WMOlongitude = Longitude
  WMOaltitude = 0#
  zMRI1 = 0
  zMRI2 = 0
  Ztrop = Zt1
  Ttrop = TT1
  For i = 1 To Nlev
    If RAp(i) > Zt1 Then Exit For
  Next i
  Ltrop = i - 1
  Ztrop2 = Zt2
  Ttrop2 = TT2
  Ltrop2 = 0
  IntRhoV = 0#
  Tcoldest = Tcp
  Zcoldest = Zcp
  ZtropE = 0
  TtropE = 0
  LtropE = 0
  nRAOBlevels = 150
'  GZr2(1 To 153) As Integer       'Geometric Altitude [m]
'  PZr2(1 To 153) As Integer       'Pressure altitude  [0.1mb]
'  TZr2(1 To 153) As Integer       'Temperatures [10 mK]
'  RZr2(1 To 153) As Integer       'Relative Humidity [0.01%]
'  LZr2(1 To 3) As Integer         'Editted level number

  For i% = 1 To Nlevr: ZGraob(i%) = RAp(i): Next i
  For i% = 1 To Nlevr: ZPraob(i%) = RAp(i): Next i
  For i% = 1 To Nlevr: PZraob(i%) = fZtoP(RAp(i)): Next i
  For i% = 1 To Nlevr: TZraob(i%) = RTp(i): Next i
  For i% = 1 To Nlevr: RZraob(i%) = 0#: Next i
  
  OutRecord = 1
  If NewStart Then
    W$ = Dir(txtATP_RAOB2.Text, vbNormal)
    If W$ <> "" Then
      ans = MsgBox(txtATP_RAOB2.Text + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
      If ans = vbYes Then
        Kill (txtATP_RAOB2.Text)
        RAOBlu = FreeFile
        Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
        OutRecord = 1                           ' Overwrite file if it exists
      Else
        RAOBlu = FreeFile
        Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
        OutRecord = LOF(RAOBlu%) \ Len(RAOB2) + 1 ' Append to existing file
      End If
    End If
    NewStart = False
  Else
    RAOBlu = FreeFile
    Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
    OutRecord = LOF(RAOBlu%) \ Len(RAOB2) + 1 ' Append to existing file
  End If
  
  Call RAOBwriteSL(RAOBlu%, OutRecord&)
  Close RAOBlu
  
Case 1
  UpdateFlightTrack
  
Case 2
'Load Form1
Dim lu%, A As String * 15, b As Byte, C As String * 82, D As String * 2
Dim testfile$, Fileformat As Byte
  testfile$ = "C:\WB57\AVE\20040121\WB20040121.REF"
  Doy = 51
  FltNumber = 20040001
  Mission = "AVE"
  Fileformat = 1
  Pi = "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
  Yeer = 2004
  lu = FreeFile
  Open testfile$ For Random Access Read Write As lu Len = Len(TST)
  TST.Doy = Doy
  TST.FltNumber = FltNumber
  TST.Mission = Mission$
  TST.Fileformat = Fileformat
  TST.Pi = Pi$
  TST.Yeer = Yeer
'  Put #lu%, 1, TST
'  Put #lu%, 2, TST
'  Put #lu%, 3, TST
'  Close lu
  Doy = 0
  FltNumber = 0
  Mission = ""
  Pi = ""
  Yeer = 0
  lu = FreeFile
'  Open testfile$ For Input As lu
  Open testfile$ For Random Access Read Write As lu Len = Len(FIR2)
  Get #lu, 1, FIR2
'  Input #lu, A
  Doy = FIR2.Doy
  FltNumber = FIR2.FltNumber
  Mission = FIR2.Mission$
  Fileformat = FIR2.Fileformat
  Pi = FIR2.Pi
  Yeer = FIR2.Yeer
  Close lu

  Debug.Print Fileformat

Case 3
Dim OUTlu%, RAWlu%, RPTlu%, RMMlu%, Tnav!, Tptw!, Tmms!, Znav!, Zptw!, Zmms!
Dim FltDate$, UTks!, UTnav!, UTptw!, UTmms!, Path$, Threshold!, Zgps!

' Use NoteTab to edit all files to start at the same UT
'17:36:58
Threshold = 7         'gALT     17# * 3600 + 36 * 60 + 57

FltDate = YYYYMMDD$
Path$ = "C:\MTP\Data\" + Platform$ + "\" + Mission$ + "\"
RAWlu = FreeFile
Open Path$ + FltDate + "\WB" + FltDate + ".RAW" For Input As RAWlu
RPTlu = FreeFile
Open Path$ + FltDate + "\WB" + FltDate + ".RPT" For Input As RPTlu
RMMlu = FreeFile
Open Path$ + FltDate + "\WB" + FltDate + ".RMM" For Input As RMMlu
OUTlu = FreeFile
Open Path$ + FltDate + "\T" + FltDate$ + ".txt" For Output As OUTlu
Ceiling = 25
Do
  
  Do
    If EOF(RPTlu) Then GoTo ExitPoint
    Input #RPTlu, A$
    If Left$(A$, 1) = "A" Then Call DecodeAline(A$, Quit)
  Loop Until gALT > Threshold And Left$(A$, 1) = "A"
  UTptw = UTsec / 1000: Tptw = OATnav: Zptw = pALT
  
  Do
    If EOF(RMMlu) Then GoTo ExitPoint
    Input #RMMlu, A$
    If Left$(A$, 1) = "A" Then Call DecodeAline(A$, Quit)
  Loop Until gALT > Threshold And Left$(A$, 1) = "A"
  UTmms = UTsec / 1000: Tmms = OATnav: Zmms = pALT
  If Quit Then GoTo ExitPoint
  
'  Do
  Do
    If EOF(RAWlu) Then GoTo ExitPoint
    Input #RAWlu, A$
    If Left$(A$, 1) = "A" Then Call DecodeAline(A$, Quit)
  Loop Until gALT > Threshold And Left$(A$, 1) = "A"
  UTnav = UTsec / 1000#: Tnav = OATnav: Znav = pALT: Zgps = gALT
'  Loop Until UTnav = UTptw
  
'  If pALT > 5 Then
    If UTnav = UTptw And UTnav = UTmms Then UTks = UTnav
    Print #OUTlu, UTnav; UTptw; UTmms; Tnav; Tptw; Tmms; Znav; Zptw; Zmms; Zgps; fSecToTstring(UTsec, True)
    Debug.Print UTnav; UTptw; UTmms; Tnav; Tptw; Tmms; Znav; Zptw; Zmms; Zgps
'  End If
  
Loop Until EOF(RAWlu) Or EOF(RPTlu) Or EOF(RMMlu)

ExitPoint:
  Close
  
Case 4
Dim REFlu%, Record%, z!(1 To 33), T!(1 To 33), pt!(1 To 33), j%, k%, zo!, PTi!(1 To 25)
Dim UThr!, UT1!, UT2!, Tnav1!, Tnav2!, Zp1!, Zp2!, PTiac!(304 To 314, 1 To 500)
Dim Tt!(0 To 3700), ZZ!(0 To 3700), iMax%, Tavg!, Tmtp!, UTh!(0 To 3700), UTn&, Mode%
Dim OAT!(1 To 20), ALT!(1 To 20), Zavg!, Trms!, Zrms!, Tmin!, Tmax!, Count%
Dim Z2!(1 To 4000, 1 To 33), T2!(1 To 4000, 1 To 33), PT2!(1 To 4000, 1 To 33), PT3!(1 To 4000, 1 To 33)
Dim UThr2!(1 To 4000), UTks2!(1 To 4000), Palt2!(1 To 4000), OATnav2!(1 To 4000), n%
Dim V!(1 To 4000)

  Mode = 2
  REFfile$ = txtREFfile.Text
  FileFormatIn = fREFreadFileFormat(REFfile$)
  
  Select Case Mode
  Case 0   'PT @ specific z
    REFopen REFfile$
    OUTlu = FreeFile
    Open "C:\DC8\TexAQS\TvsZ.txt" For Output As OUTlu
    Record = 1
    Do
      Record% = Record% + 1
      Call REFread(REFlu%, Record%)
      UThr = UTsec / 3600#
      If UThr >= 18 And UThr <= 19# And Not EOF(REFlu) Then
        If GoodScan Then
          For i = 1 To Nlev
            z(i) = sZZZ(Nlev1 + i - 1)
            T(i) = sTTT(Nlev1 + i - 1)
            pt(i) = fTheta(T(i), fZtoP(z(i)))
          Next i
          A$ = Format(UThr, "00.000") + vbTab + Format(pALT, "00.000") + vbTab + Format(OATnav, "000.00") + vbTab + Format(fTheta(OATnav, fZtoP(pALT)), "000.00")
          'Debug.Print A$;
          Print #OUTlu, A$;
          For j = 2 To 25 Step 1
            zo = j / 10#
'            PTi(j) = fLinterp(Z(), PT(), Nlev, zo)
            PTi(j) = fLinterp(z(), T(), Nlev, zo)
            Debug.Print vbTab + Format(PTi(j), "000.00");
            Print #OUTlu, vbTab + Format(PTi(j), "000.00");
          Next j
          Debug.Print ""
          Print #OUTlu, ""
        
        End If
      End If
      DoEvents
    Loop Until EOF(REFlu%)
    Close OUTlu, REFlu
  
  Case 1
    lu = FreeFile  '1 second data
    Open "C:\DC8\TexAQS\TPnav0828.txt" For Input As lu
    For i = 0 To 3600
      Input #lu, UTn, Tt!(i), ZZ!(i)
      UTh(i) = UTn
    Next i
    REFopen REFfile$
    OUTlu = FreeFile
    Open "C:\DC8\TexAQS\AvgRms.txt" For Output As OUTlu
    Record = 1: UT1 = 0: UT2 = 0
    Call REFread(REFlu%, 1)
    UT1 = UTsec: Zp1 = pALT: Tnav1 = OATnav
    j = 0
    Do
      Record% = Record% + 1
      Call REFread(REFlu%, Record%)
      UT2 = UTsec: Zp2 = pALT: Tnav2 = OATnav
      UThr = UTsec / 3600#
      UTks = UTsec / 1000#
      If UThr >= 18 And UThr <= 19# And Not EOF(REFlu) Then
        If GoodScan Then
          Do
            j = j + 1
          Loop Until UTh(j) + 18 * 3600# >= UT1
          i = 0
          Do
            j = j + 1
            i = i + 1
            OAT(i) = Tt(j)
            ALT(i) = ZZ(j)
          Loop Until UTh(j) + 18 * 3600# >= UT2
          Call AvgRmsMinMax(OAT(), i, Tavg!, Trms!, Tmin!, Tmax!)
          Call AvgRmsMinMax(ALT(), i, Zavg!, Zrms!, Zmin!, Zmax!)
          A$ = Format(UTks, "00.000") + vbTab + Format(UThr, "00.0000") + vbTab
          A$ = A$ + Format((Zp1 + Zp2) / 2#, "0.000") + vbTab + Format(Tnav2, "000.00") + vbTab
          A$ = A$ + Format(Tavg, "000.00") + vbTab + Format(Trms, "0.000") + vbTab
          A$ = A$ + Format(Zavg, "0.000") + vbTab + Format(Zrms, "0.000") + vbTab
          A$ = A$ + Format(Tmin, "000.00") + vbTab + Format(Tmax, "000.00") + vbTab
          A$ = A$ + Format(Zmin, "0.000") + vbTab + Format(Zmax, "0.000") + vbTab
          A$ = A$ + Format(Tmax - Tmin, "#0.00") + vbTab + Format(Zmax - Zmin, "0.000")
          'Debug.Print A$
          Print #OUTlu, A$
        End If
      End If
      UT1 = UT2: Zp1 = Zp2: Tnav1 = Tnav2
      DoEvents
    Loop Until EOF(REFlu%)
    Close OUTlu, REFlu
  
  Case 2    'Calculate IACs
    REFopen REFfile$
    OUTlu = FreeFile
    Open "C:\DC8\TexAQS\ZvsPTavg.txt" For Output As OUTlu
    Record = 1
    k = 0
    Count = 0
    Do
      Record% = Record% + 1
      Call REFread(REFlu%, Record%)
      UThr = UTsec / 3600#
      If UThr >= 18 And UThr <= 19# And Not EOF(REFlu) Then
        If GoodScan Then
          Count = Count + 1
          UThr2(Count) = UThr
          Palt2(Count) = pALT
          OATnav2(Count) = OATnav
          UTks2(Count) = UTsec / 1000#
          ' Get a profile
          For i = 1 To Nlev
            z(i) = sZZZ(Nlev1 + i - 1)
            T(i) = sTTT(Nlev1 + i - 1)
          Next i
          ' And interpolate to fixed altitudes to avoid Zp change effects
          For j = 1 To 33
            Z2(Count, j) = 0.1 * j
            T2(Count, j) = fLinterp(z(), T(), Nlev, Z2(Count, j))
          Next j
        End If
      End If
    Loop Until EOF(REFlu)
    Close REFlu
    ' Now do running averages
    n = 9
    For j = 1 To 33
      For i = 1 To Count: V(i) = T2(i, j): Next i
      Call RBCaverage(n%, Count%, V!())
'      If j = 28 Then
'        For i = 1 To Count
'          Print #OUTlu, i, vbTab, V(i), vbTab, T2(i, j)
'        Next i
'      End If
      For i = 1 To Count: T2(i, j) = V(i): Next i
      For i = 1 To Count
        PT2(i, j) = fTheta(T2(i, j), fZtoP(Z2(i, j)))
      Next i
    Next j
'    Exit Sub
    ' Now calculate IACs using smoothed temperatures
    For i = 1 To Count
      A$ = Format(UTks2(i), "00.000") + vbTab + Format(UThr2(i), "00.000") + vbTab + Format(Palt2(i), "00.000") + vbTab + Format(OATnav2(i), "000.00") + vbTab + Format(fTheta(OATnav2(i), fZtoP(Palt2(i))), "000.00")
      'Debug.Print A$;
      Print #OUTlu, A$;
      For j = 1 To 33: pt(j) = PT2(i, j): z(j) = Z2(i, j): T(j) = T2(i, j): Next j
      k = k + 1
      For j = 628 To 608 Step -1
        zo = j / 2#
        PTi(1) = fPTinterp(pt(), z(), 33, zo)
        If PTi(1) = -1 Then
          Debug.Print vbTab + "";
          Print #OUTlu, vbTab + "";
        Else
          Debug.Print vbTab + Format(PTi(1), "00.00");
          Print #OUTlu, vbTab + Format(PTi(1), "00.00");
        End If
      Next j
'      For j = 1 To 25 Step 1
'        zo = j / 10#
'        PTi(1) = fLinterp(z(), T(), 25, zo)
'        Debug.Print vbTab + Format(PTi(1), "00.00");
'        Print #OUTlu, vbTab + Format(PTi(1), "00.00");
'      Next j
      Debug.Print ""
      Print #OUTlu, ""
      
      DoEvents
    Next i
    Close OUTlu
  
  End Select

Case 5
Dim TptS!, zPT!, PptS!, PptT!, PsNav!, NDRlu%
Dim PTWfile$, UT&
Dim Ts!, Ps!, Mach!, ptt!, ttx!

FltDate = "20050619"   'yyyymmdd$
RAWlu = FreeFile
Open "C:\MTP\Data\WB57\HAVE2\" + FltDate + "\WB" + FltDate + ".RAW" For Input As RAWlu
NDRlu = FreeFile
Open "C:\MTP\Data\WB57\HAVE2\NDR\" + FltDate + "\" + FltDate + ".ASC" For Input As NDRlu
Input #NDRlu, A$
Input #NDRlu, A$
Input #NDRlu, A$
Input #NDRlu, A$
Input #NDRlu, A$
PTWfile$ = "C:\MTP\Data\WB57\HAVE2\PT\" + "\PT" + FltDate + ".WB57"
OUTlu = FreeFile
Open "C:\MTP\Data\WB57\HAVE2\PT" + FltDate$ + ".txt" For Output As OUTlu

Ceiling = 25
Do
  Do
    If EOF(RAWlu) Then GoTo ExitPoint
    Input #RAWlu, A$
    If Left$(A$, 1) = "A" Then Call DecodeAline(A$, Quit)
  Loop Until pALT > 12 And Left$(A$, 1) = "A"
  UTnav = UTsec / 1000#: Tnav = OATnav: Znav = pALT: PsNav = fZtoP(Znav)
  Call GetPTWvalues_All(UTsec&, UT&, PptS!, PptT!, TptS!, PTWfile$)
  zPT = fPtoZ(PptS)
  
  Do
    If EOF(NDRlu) Then GoTo ExitPoint2
    Input #NDRlu, A$
'000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111122222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222223333333333333333333333333333333333
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999000000000011111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990000000000111111111122222222223333
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123
' 1 2..........2 3.......3 4........4 5....5 6......6 7......7 8....8 9....9 1010 11.11 12..12 13..13 14..14 15.15 16.16 17.17 18..18 19...19 20.....20 21......21 22....22 23....23 24,,24 25..25 26..26 27...27 28.28 29..29 3030 31.31 32..32 33..33 34..34 35..35 36..36 37..37 38..38 39..39 40..40 41..41 42..42 43..43 44..44 45..45 46
'   day:hh:mm:ss iLatitude iLongitude tHead. Pitch... Roll.... gSpeed TrackA iWsp iWdir bLongA bLatlA bNormA TArat pRate rRate iVsped gAlt... gLatitude gLongitude sPress.. tPress.. dPress tTemp. sTemp. pAlt... Mach. pTAS.. pWsp pWdir sElev. sAzim. DAV-1. DAV-2. DAV-3. DAV-4. DAV-5. DAV-6. DAV-7. DAV-8. DAV-9. DAV-10 DAV-11 DAV-12 rn
'G 170:18:46:13 N29.61811 W 95.16495 65.98  -0.4999  0.0330   3.22   67.75  3.0  248.7 0.001  0.018  0.027  3.4   0.1   -0.1  0.05   5.2     N29.61811 W 95.16495 344.648  1006.018 0.139  -0.02  -0.04  60.4    0.014 4.65   1.6  56.2  81.79  222.48 -0.005 -0.003 0.663  4.998  4.998  4.998  4.998  4.998  4.998  4.998  4.998  4.998
'G 170:18:46:14 N29.61811 W 95.16495 67.91  -0.4285  0.0165   3.09   69.88  2.8  253.0 -0.003 0.019  -0.017 4.2   0.1   0.2   0.01   5.2     N29.61811 W 95.16495 344.648  1006.167 0.095  -0.02  -0.03  59.1    0.012 3.84   0.8  47.9  81.79  222.50 -0.006 -0.002 0.663  4.998  4.998  4.998  4.998  4.998  4.998  4.998  4.998  4.998
    UTnav = fTstringToSec(Mid$(A$, 8, 8), True)
  Loop Until UTnav = UTsec
  Ps = Mid$(A$, 172, 8)
  ptt = Ps + Mid$(A$, 181, 6)
  ttx = Mid$(A$, 188, 6) + 273.15
  Ts = Mid$(A$, 195, 6) + 273.15
  Mach = Mid$(A$, 210, 5)
 
'  If UTnav = UTptw And UTnav = UTmms Then UTks = UTnav
  Print #OUTlu, UTnav; UT / 1000; Tnav; PsNav; TptS; PptS; PptT; Znav; zPT; Ps; ptt; Ts; ttx; Mach
  Debug.Print UTnav; UT / 1000; Tnav; PsNav; TptS; PptS; PptT; Znav; zPT; Ps; ptt; Ts; ttx; Mach
  
Loop Until EOF(RAWlu)

ExitPoint2:
  Close
  
Case 6

Case 7
Dim INPlu%, IWGlu%, Line$, IWG$, FirstAline As Boolean
'Instrument on 23:21:18 12-05-2007
'A 23:21:40 12-05-2007
'IWG1,20010920T151645,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
'B 009653 013837 014684 009729 013910 014784 010241 014390 015307 010302 014467 015363 010306 014442 015380 010310 014441 015373 010310 014453 015369 010299 014442 015366 010292 014442 015369 009755 013976 014663
'M01: 2943 2109 2884 3097 3086 2954 2479 2936
'M02: 2101 1247 1429 4095 1531 1191 4095 1060
'Pt: 2436 14145 14143 16383 14187 14163 14028 14768
'E 011467 015347 016344 009836 013969 014870

  INPlu = FreeFile
  Open "C:\MTP\Data\NGV\NG_Test\20071205\NG20071205.RAW" For Input As INPlu
  
  IWGlu = FreeFile
  Open "C:\MTP\Data\NGV\TREX\IWG1.txt" For Input As IWGlu
  
  OUTlu = FreeFile
  Open "C:\MTP\Data\NGV\NG_Test\20071218\NG20071218.RAW" For Output As OUTlu
'A 23:21:40 12-05-2007
'IWG1,20010920T151645,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
'123456789012345678901234567890123456789012345678901234567890
  FirstAline = False
  Do
    Line Input #INPlu, Line$
    If Left$(Line$, 1) = "A" Then
      A$ = "A " + Mid$(Line$, 18, 4) + "-" + Mid$(Line$, 12, 2) + "-" + "18"
      A$ = A$ + " " + Mid$(Line$, 3, 2) + ":" + Mid$(Line$, 6, 2) + ":" + Mid$(Line$, 9, 2)
      A$ = A$ + " 002.30 00.05 001.11 00.10 01.30 0.01 267.00 0.20"
      Print #OUTlu, A$
      A$ = "IWG1," + Mid$(Line$, 18, 4) + Mid$(Line$, 12, 2) + "18"
      A$ = A$ + "T" + Mid$(Line$, 3, 2) + Mid$(Line$, 6, 2) + Mid$(Line$, 9, 2)
      Line Input #IWGlu, IWG$
      IWG$ = A$ + Mid$(IWG$, 21, Len(IWG) - 24)
      Print #OUTlu, IWG$
    Else
      Print #OUTlu, Line$
    End If
    DecodeGVline Line$, Not FirstAline
    FirstAline = True
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu, IWGlu

End Select
End Sub

Private Sub cmdAutoScaleTA_Click()

AutoScaleTA = Not AutoScaleTA
If AutoScaleTA Then
  cmdAutoScaleTA.Caption = "M"
  txtT1.Enabled = False
  txtT2.Enabled = False
  txtTC.Enabled = False
Else
  cmdAutoScaleTA.Caption = "A"
  txtT1.Enabled = True
  txtT2.Enabled = True
  txtTC.Enabled = True
End If

End Sub



Private Sub cmdCheckPointing_Click()
Dim Ext$, MPheader$, V!(1 To 25), LocalOpen As Boolean, fn$, Scanlength!
Dim Filename$, TSfile$, MPlu%, TSlu%, HDRfile$, L%, x!, A$, dZp!, dZg!, Discard As Boolean, dALT!
Dim M!(1 To 3), header$, f As Form, TA15!, TA25!, TA35!, TA17!, TA27!, TA37!, i%
Set f = frmMTPbin

  ERFfile$ = ERFfile$
  REFfile$ = REFfile$
  fn$ = Dir(ERFfile$, vbNormal)  'Check if ERF exits
  If fn$ = "" Then               'No ERF file, check for REF
    fn$ = Dir(REFfile$, vbNormal)
    If fn$ = "" Then             'Neither ERF or REF
      Call MsgBox("Neither requested ERF or REF file exist", vbOKOnly)
      Exit Sub
    Else
      fn$ = REFfile$
    End If
  Else
    fn$ = ERFfile$
  End If

  If MPlu <> 0 Then Close MPlu
  MPlu% = FreeFile
  Open fn$ For Random Access Read Write As MPlu% Len = Len(REF2)
  txtREFfile.Text = REFfile$
'  FileRecords = LOF(MPlu%) \ Len(REF) - HiddenRecords
  ProgressBar1.Min = 0
  ProgressBar1.Max = TotalCycles
  txtFileRecords.Text = TotalCycles
  REFread MPlu, TotalCycles
  Scanlength = UTsec
  REFread MPlu, 1
  If UTsec > Scanlength Then Scanlength = Scanlength + 86400
  Scanlength = (Scanlength - UTsec) / (TotalCycles - 1)  'Needed for GTE format
  


  If REFlu% = 0 Then
    Filename$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
    REFlu% = FreeFile
    Open Filename$ For Random Access Read As REFlu% Len = Len(REF2)
    LocalOpen = True
    'txtFileRecords = LOF(REFlu%) / Len(REF) - HiddenRecords
  End If
  
  If Channels = 3 Then
    MPheader$ = " ##0.000 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 #0.000 #0.000 #0.000 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 ##0.00 #0.00"
    header$ = " UT(ks)  Tmtp  Tnav  TA15   TA25   TA35  TA17   TA27   TA37   m1     m2     m3  TA15-TA25  TA17-TA27  TA15-TA35  TA17-TA37  TA25-TA35  TA27-TA37  LRac Pitch Roll Zp"
  Else
    MPheader$ = " ##0.000 ##0.00 ##0.00 ##0.00 ##0.00 #0.000 #0.000"
    header$ = " UT(ks)  Tmtp  Tnav  TA15   TA25  TA17   TA27   m1     m2"
  End If
'  If f.chkDiscard.value = 1 Then Discard = True Else Discard = False
'  dALT = Val(f.txtDiscard.Text)
     
  Ext$ = fACext(AC$)
  If Mission$ = "TexAQS" Then Ext$ = ".ELEC"
  TSfile$ = "PE" + YYYYMMDD$ + Ext$
  
' Will read & modify header after first assigning MP output file
  TSlu% = FreeFile
  Open Drive$ + Rdir2$ + Mission$ + "\PE\" + TSfile$ For Output As #TSlu%
  Print #TSlu, header$
' Write Limits Record
  Record% = 0
  Do
    Record% = Record% + 1
    If Record > f.ProgressBar1.Max Then Exit Do
    f.txtRecord.Text = Str(Record)
    f.ProgressBar1.Value = Record
    Call REFread(REFlu%, Record%)
    If Not EOF(REFlu) Then
'      If Not GoodScansOnly Or GoodScan Or WriteBadHeaders Then GoSub WriteCycle
'      If GoodScan Or WriteBadHeaders Then GoSub WriteCycle
      If GoodScan Then GoSub WriteCycle
    End If
    DoEvents
  Loop Until EOF(REFlu%)
  
  Close (TSlu%)
  If LocalOpen Then Close (REFlu%)
  
  Exit Sub

WriteCycle:
  If UTsec / 1000# < Val(txtUTpoint(0).Text) Or UTsec / 1000# > Val(txtUTpoint(1).Text) Then Return
' NB only come here on GoodScan = True  (get rid of checks for goodscan)
  If EditNlev And (Nlev < NlevMin Or Nlev > NlevMax) Then
    EditWord = EditWord Or 128: GoodScan = False ' Determine total # of acceptable levels; throw out if <7 levels
    NlevPC = NlevPC + 1
  End If
  If Nlev1 = 0 Then Nlev1 = 1
  If pALT < zzz(Nlev1) / 100 Then EditWord = EditWord Or 1: RetAltPC = RetAltPC + 1: GoodScan = False
  
  TA15 = TA(1, LocHor - 1): TA25 = TA(2, LocHor - 1): TA35 = TA(3, LocHor - 1)
  TA17 = TA(1, LocHor + 1): TA27 = TA(2, LocHor + 1): TA37 = TA(3, LocHor + 1)
'  TA15 = TA(1, 5) - WINcor(1, 5): TA25 = TA(2, 5) - WINcor(2, 5): TA35 = TA(3, 5) - WINcor(3, 5)
 ' TA17 = TA(1, 7) - WINcor(1, 7): TA27 = TA(2, 7) - WINcor(2, 7): TA37 = TA(3, 7) - WINcor(3, 7)
    M(1) = (TA15 - TA25) / (TA17 - TA27)
    M(2) = (TA15 - TA35) / (TA17 - TA37)
    M(3) = (TA25 - TA35) / (TA27 - TA37)

' Update the number of valid levels
  If Channels = 3 Then
    V(1) = UTsec / 1000#: V(2) = OATmtp: V(3) = OATnav: V(4) = TA15: V(5) = TA25: V(6) = TA35:
    V(7) = TA17: V(8) = TA27: V(9) = TA37: V(10) = M(1): V(11) = M(2): V(12) = M(3):
    V(13) = TA15 - TA25: V(14) = TA17 - TA27: V(15) = TA15 - TA35: V(16) = TA17 - TA37:
    V(17) = TA25 - TA35: V(18) = TA27 - TA37: V(19) = LRac: V(20) = Pitch: V(21) = Roll: V(22) = pALT
  Else
    V(1) = UTsec / 1000#: V(2) = OATmtp: V(3) = OATnav: V(4) = TA15: V(5) = TA25: V(6) = TA17: V(7) = TA27: V(6) = M(1): V(7) = M(2)
  End If
  
  'If GoodScan Or WriteBadHeaders Or (Discard And pALT < dALT) Then Print #TSlu%, fUsing$(MPheader$, v!())
  For i = 1 To 22
    Print #TSlu, Format(V(i), "##0.000"),
  Next i
  Print #TSlu, ""
Return

End Sub



Private Sub cmdCopyRaw_Click()
Dim RAWfile$, RAWfileDest$, INIfile$, INIfileDest$, BASE$, RF$(1 To 10), File$
Dim CALfileDest$, CALfile$, ans As Variant, i%, j%, lu%, OUTlu%, x$
Dim f As Form, jOffset%
  
' Copy RAW, INI and CAL files
' RAW ... is raw data in Mission RAW folder
' INI ... is Sensor Unit Configuration File (with pointing offsets) in Mission RAW folder
' CAL ... is default Calfile in Mission Setup folder
  
  Set f = frmFLTINFO
' Set Initial directory
  CommonDialog1.InitDir = MNpath$ + "RAW\"
' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  RAWfile$ = CommonDialog1.Filename
  If RAWfile$ = "" Then Exit Sub
  
  If Left$(SU$, 3) = "CAN" Then jOffset = 9 Else jOffset = 6
  i = 0
  j = Len(MNpath$) + 4
  BASE$ = Left$(RAWfile$, j + jOffset)
  File$ = Dir(BASE$ + "??.??")
  Do
    If Len(File$) > 0 Then
      i = i + 1
      RF$(i) = File$
      File$ = Dir()
    Else
      Exit Do
    End If
  Loop
  
  
  
  
  If Left$(SU$, 3) = "CAN" Then
    INIfile$ = Left$(RAWfile$, Len(RAWfile$) - 2) + "mtph"
    INIfileDest$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".MTPH"
  Else
    INIfile$ = Left$(RAWfile$, Len(RAWfile$) - 2) + "INI"
    INIfileDest$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".SUI"
  End If
  If Dir(INIfile$) = "" Then
    MsgBox INIfile$ + " does not exist!" + vbCrLf + " Copy it to RAW file folder and try again.", vbOKOnly
    Exit Sub
  End If
  
  CALfile$ = MNpath$ + "Setup\" + Mission$ + "_CAL.txt"
  If Dir(CALfile$) = "" Then
    MsgBox CALfile$ + " does not exist!" + vbCrLf + " Copy it to RAW file folder and try again.", vbOKOnly
    Exit Sub
  End If

' Make sure destination path exists
  Call DirCheck(MNpath$, Trim(f.cboFltDates.Text))
  
  RAWfileDest$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".RAW"
  If Dir(RAWfileDest$) <> "" Then
    ans = MsgBox(RAWfileDest$ + " already exists!" + vbCrLf + "Do you want to overwrite it?", vbYesNo)
    If ans = vbYes Then
      'FileCopy RAWfile$, RAWfileDest$
      If i > 1 Then
        ans = MsgBox("There are " + Str(i) + " files for this day!" + vbCrLf + "Do you want to concatenate them?", vbYesNo)
        If ans = vbYes Then
          OUTlu = FreeFile
          Open RAWfileDest$ For Output As OUTlu
          For j = 1 To i
            lu = FreeFile
            Open RF(j) For Input As lu
            Do
              Line Input #lu, x$
              Print #OUTlu, x$
            Loop Until EOF(lu)
            Close lu
          Next j
          Close OUTlu
        Else
          FileCopy RAWfile$, RAWfileDest$
        End If
    Else
      FileCopy RAWfile$, RAWfileDest$
    End If

    End If
  Else
    If i > 1 Then
      ans = MsgBox("There are " + Str(i) + " files for this day!" + vbCrLf + "Do you want to concatenate them?", vbYesNo)
      If ans = vbYes Then
        OUTlu = FreeFile
        Open RAWfileDest$ For Output As OUTlu
        For j = 1 To i
          lu = FreeFile
          Open RF(j) For Input As lu
          Do
            Line Input #lu, x$
            Print #OUTlu, x$
          Loop Until EOF(lu)
          Close lu
        Next j
        Close OUTlu
      Else
        FileCopy RAWfile$, RAWfileDest$
      End If
    Else
      FileCopy RAWfile$, RAWfileDest$
    End If
  End If
  
' Copy INI file (Need pointing information) to Sensor Unit Initialization (SUI) file
  FileCopy INIfile$, INIfileDest$
  
' Copy default CAL file
  CALfileDest$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".CAL"
  FileCopy CALfile$, CALfileDest$
  
  
End Sub


Private Sub cmdCTS_Click(Index As Integer)
Dim Sum1!, Sum2!, Rec%, i%, j%, r%, g!(1 To 3), x!, AVG!, RMS!, RMSpath$
Dim M%, m1%, m2%, L5%, L6%, CtsMin%, CtsMax%, Gnd!(1 To 3), lu%, T1!, T2!, y!

Select Case Index
Case 0 'Clear
  If chkRAW.Value = 1 Then For i = 0 To 12: chkCTS(i).Value = 0: Next i
  If chkEdit.Value = 1 Then For i = 13 To 25: chkCTS(i).Value = 0: Next i
  
Case 1 'Reset X Scale
  FirstMouseX = 1
  If Not RealTime Then LastMouseX = TotalCycles
  FirstMouseXSave = 1
  If Not RealTime Then LastMouseXSave = TotalCycles
  UpdateX1X2
  Call UpdateCTSrange
  
Case 2 'Set Selected Column
  If chkRAW.Value = 1 Then For i = 0 To 12: chkCTS(i).Value = 1: Next i
  If chkEdit.Value = 1 Then For i = 13 To 25: chkCTS(i).Value = 1: Next i

Case 3 'Update
  frmFLTINFO.cmdWriteDefault_Click
  Call UpdateCTSrange

Case 4, 7, 9, 10 'Scan to Scan AVG or RMS
  SSTab1.Tab = 3
  frmFLTINFO.UpdateFLTINFO
' Calculate mesoscale temperature rms
  T1 = OATV(1, FirstMouseX)
  Rec = 1
  For r = FirstMouseX + 1 To LastMouseX
    T2 = OATV(1, r)
    If T2 > 180 And T2 < 320 Then
      x = T2 - T1
      Sum1 = Sum1 + x
      Sum2 = Sum2 + x ^ 2
      Rec = Rec + 1
      T1 = T2
    End If
  Next r
  AVG = Sum1 / Rec         'Calculate average
  If Sum2 - AVG ^ 2 / Rec > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * Rec) / (Rec - 1)) 'Calculate RMS
  txtTrms.Text = Format(RMS, "#0.00")
  For i = 1 To Channels
    For j = 1 To 13
      Sum1 = 0#: Sum2 = 0#: Rec = 0
      For r = FirstMouseX + 1 To LastMouseX
        Call CalculateGain(r%, g!())
        If chkDisplayKelvin.Value = 0 Then g(i) = 1
        If chkFlagBad.Value = 0 Then
          If CQV(r) And CQV(r - 1) Then
            Select Case j
            Case Is < 11
              If UseMAforCS Then
                x = (CSMA(i, j) - CSMAL(i, j)) / g(i)
              Else
                x = (CS(i, j) - CSL(i, j)) / g(i)
              End If
            Case 11
              x = (CN(i) - CNL(i)) / g(i)
            Case 12
              x = (CB(i) - CBL(i)) / g(i)
            Case 13
              x = (CN(i) - CNL(i)) / g(i)
            End Select
            Sum1 = Sum1 + x
            Sum2 = Sum2 + x ^ 2
            Rec = Rec + 1
          End If
        Else
          Select Case j
          Case Is < 11
            If UseMAforCS Then
              x = (CSMA(i, j) - CSMAL(i, j)) / g(i)
            Else
              x = (CS(i, j) - CSL(i, j)) / g(i)
            End If
          Case 11
             x = (CN(i) - CNL(i)) / g(i)
          Case 12
             x = (CB(i) - CBL(i)) / g(i)
          Case 13
            x = (CN(i) - CNL(i)) / g(i)
          End Select
          Sum1 = Sum1 + x
          Sum2 = Sum2 + x ^ 2
          Rec = Rec + 1
        End If
      If i = 2 And j = 11 Then
      'Debug.Print r; X; Sum1; Sum2; Rec
      End If
      Next r
       
      AVG = Sum1 / Rec         'Calculate average
      If (Sum2 - AVG ^ 2 * Rec) >= 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * Rec) / (Rec - 1)) Else RMS = 99# 'Calculate RMS
      Select Case i
      Case 1: If Index = 4 Or Index = 9 Then txtCounts1(j - 1).Text = Format(AVG, "##0.00") Else txtCounts1(j - 1).Text = Format(RMS, "##0.00")
      Case 2: If Index = 4 Or Index = 9 Then txtCounts2(j - 1).Text = Format(AVG, "##0.00") Else txtCounts2(j - 1).Text = Format(RMS, "##0.00")
      Case 3: If Index = 4 Or Index = 9 Then txtCounts3(j - 1).Text = Format(AVG, "##0.00") Else txtCounts3(j - 1).Text = Format(RMS, "##0.00")
      End Select
    Next j
  Next i
  
  If chkAutoRemoveTrms.Value = 1 And (Index = 7 Or Index = 10) Then 'Remove mesoscale T variations from population RMS to get radiometer noise
    y = Val(txtTrms.Text)
    For i = 0 To 9
      x = (Val(txtCounts1(i).Text) ^ 2 - y ^ 2)
      If x < 0.0225 Then x = 0.0225               'RMS<0.14 K
      txtCounts1(i).Text = Format(Sqr(x), "#0.00")
      x = (Val(txtCounts2(i).Text) ^ 2 - y ^ 2)
      If x < 0.0225 Then x = 0.0225
      txtCounts2(i).Text = Format(Sqr(x), "#0.00")
      x = (Val(txtCounts3(i).Text) ^ 2 - y ^ 2)
      If x < 0.0225 Then x = 0.0225
      txtCounts3(i).Text = Format(Sqr(x), "#0.00")
    Next i
  End If

Case 5, 8 'Loc to Loc AVG and RMS
  SSTab1.Tab = 3
  frmFLTINFO.UpdateFLTINFO 'Make sure gain scale hasn't changed
  L5 = Trim(cboLoc5.Text)
  L6 = Trim(cboLoc6.Text)
  For i = 1 To Channels
    For j = 1 To 13
      Select Case i
      Case 1: txtCounts1(j - 1).Text = ""
      Case 2: txtCounts2(j - 1).Text = ""
      Case 3: txtCounts3(j - 1).Text = ""
      End Select
    Next j
      Sum1 = 0#: Sum2 = 0#: Rec = 0
      For r = FirstMouseX To LastMouseX
        If chkDisplayKelvin.Value = 0 Then g(i) = 1 Else Call CalculateGain(r%, g!())
        If chkFlagBad.Value = 0 Then
          If CQV(r) Then
            x = (CV(i, L5, r) - CV(i, L6, r)) / g(i)
            Sum1 = Sum1 + x
            Sum2 = Sum2 + x ^ 2
            Rec = Rec + 1
          End If
        Else
          x = (CV(i, L5, r) - CV(i, L6, r)) / g(i)
          Sum1 = Sum1 + x
          Sum2 = Sum2 + x ^ 2
          Rec = Rec + 1
        End If
      Next r
    AVG = Sum1 / Rec         'Calculate average
    If Sum2 - AVG ^ 2 / Rec > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * Rec) / (Rec - 1)) 'Calculate RMS
    Select Case i
    Case 1: txtCounts1(0).Text = AVG: txtCounts1(1).Text = RMS
    Case 2: txtCounts2(0).Text = AVG: txtCounts2(1).Text = RMS
    Case 3: txtCounts3(0).Text = AVG: txtCounts3(1).Text = RMS
    End Select
  Next i


Case 6 'Reset Y Scale
  YC0save = 0
  YG0save = 0
  YM0save = 0
  YT0save = 0
  YP0save = 0
  cboCTS1.Text = Str(YC1save)
  cboCTS2.Text = Str(YC2save)
  cboCTS3.Text = Str(YC3save)
  cboGAIN1.Text = Str(YG1save)
  cboGAIN2.Text = Str(YG2save)
  cboGAIN3.Text = Str(YG3save)
  cboMUX1.Text = Str(YM1save)
  cboMUX2.Text = Str(YM2save)
  cboMUX3.Text = Str(YM3save)
  cboTB1.Text = Str(YT1save)
  cboTB2.Text = Str(YT2save)
  cboTB3.Text = Str(YT3save)
  cboTPT1.Text = Str(YP1save)
  cboTPT2.Text = Str(YP2save)
  cboTPT3.Text = Str(YP3save)
'  cmdCTS_Click (5)  'disable XY
  cmdCTS_Click (1)  'Use full X scale and Update display

Case 11  'Auto scale
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 Then m1 = 1
  If m2 = 0 Then m2 = TotalCycles
  CtsMin = 10000
  CtsMax = -10000
  For j = 0 To 25
    If chkCTS(j).Value = 1 Then
      For i = 1 To Channels
        If chkCH(i - 1).Value = 1 Then
          For M = m1 To m2
            If j < 13 Then
              If CV(i, j + 1, M) < CtsMin Then CtsMin = CV(i, j + 1, M)
              If CV(i, j + 1, M) > CtsMax Then CtsMax = CV(i, j + 1, M)
              If CtsMin < -80 Then
                x = x
              End If
            Else
              If EV(i, j - 12, M) < CtsMin Then CtsMin = EV(i, j - 12, M)
              If EV(i, j - 12, M) > CtsMax Then CtsMax = EV(i, j - 12, M)
              If CtsMin < -80 Then
                x = x
              End If
            End If
            
          Next M
          DoEvents
        End If
      Next i
    End If
  Next j
  CtsMax = Int(CtsMax) + 1
  CtsMin = Int(CtsMin)
  If CtsMax - CtsMin > 10000 Then
    cboCTS3.Text = "1000"
    cboCTS1.Text = 1000 * Int(CtsMin / 1000)
    cboCTS2.Text = 1000 * Int(CtsMax / 1000) + 1000
  Else
    cboCTS3.Text = "500"
    cboCTS1.Text = 1000 * Int(CtsMin / 1000)
    cboCTS2.Text = 1000 * Int(CtsMax / 1000) + 1000
  End If
  Call UpdateCTSrange

Case 12  'Write RMS Counts for current flight to file for observable errors
  RMSpath$ = RSpath$ + "RMSerror\"
  CreateDirectoryTree RMSpath$, True

  lu = FreeFile
  Open MNpath$ + "RAOB\RMSerror\RMS" + YYYYMMDD$ + ".txt" For Output As lu
  For i = 0 To 9
    Print #lu, txtCounts1(i)
  Next i
    For i = 0 To 9
    Print #lu, txtCounts2(i)
  Next i
    For i = 0 To 9
    Print #lu, txtCounts3(i)
  Next i
  Close lu
  
Case Else
End Select
End Sub



Private Sub cmdDirection_Click(Index As Integer)

  Select Case Index
  Case 0 'Back
    Forward = False
    tmrGetCycle_Timer
  
  Case 1 'Stop
    If tmrGetCycle.Enabled Then
      tmrGetCycle.Enabled = False
      cmdDirection(1).Caption = "&Continue"
    Else
      tmrGetCycle.Enabled = True
      cmdDirection(1).Caption = "&Stop"
    End If
  
  Case 2 'Forward
    Forward = True
    tmrGetCycle_Timer
    
  Case 3 'Repeat
    If Forward Then
      RawCycle = RawCycle - 1
    Else
      RawCycle = RawCycle + 1
    End If
    tmrGetCycle_Timer
    
  End Select
  
End Sub







Private Sub cmdGAIN_Click(Index As Integer)
Dim i%, j%, Gmin!, Gmax!, r%

Select Case Index
Case 0
  For i = 0 To 6: chkGain(i).Value = 0: Next i
  
Case 1
  FirstMouseX = 1
  If Not RealTime Then LastMouseX = TotalCycles
  FirstMouseXSave = 1
  If Not RealTime Then LastMouseXSave = TotalCycles
  UpdateX1X2
  Call UpdateGAINrange
  
Case 2
  For i = 0 To 6: chkGain(i).Value = 1: Next i

Case 3  'UPDATE button
  For r = 1 To TotalCycles
    Call ReadArrayLeadLag(r)       'get Ttgt, Tmxr, Tnav with lead/lag
    Call GainGE(Geqn(), r)         'GE need Tmxr with lag
    Call GainND(Gnd())       'ND
    Call GainOAT(Geqn(), Goat(), False)   'OAT
    For i = 1 To Channels
      GV(0, i, r) = g(i)
      GV(1, i, r) = Geqn(i)
      GV(2, i, r) = Gnd(i)
      GV(4, i, r) = Goat(i)
      GV(5, i, r) = ALTV(1, r)
      GV(6, i, r) = OATV(1, r)
    Next i
  Next r
  Call UpdateGAINrange

Case 4

Case 5 'Scale to gain range
  Gmax = 0: Gmin = 100
  For i = 1 To Channels
    For j = 1 To TotalCycles
      If GV(0, i, j) < Gmin Then Gmin = GV(0, i, j)
      If GV(0, i, j) > Gmax Then Gmax = GV(0, i, j)
    Next j
  Next i
  Gmax = Int(Gmax) + 1
  Gmin = Int(Gmin)
  cboGAIN1.Text = Gmin
  cboGAIN2.Text = Gmax
  cboGAIN3.Text = "1"
  Call UpdateGAINrange
  
Case 6
  YC0save = 0
  YG0save = 0
  YM0save = 0
  YT0save = 0
  YP0save = 0
  cboCTS1.Text = Str(YC1save)
  cboCTS2.Text = Str(YC2save)
  cboCTS3.Text = Str(YC3save)
  cboGAIN1.Text = Str(YG1save)
  cboGAIN2.Text = Str(YG2save)
  cboGAIN3.Text = Str(YG3save)
  cboMUX1.Text = Str(YM1save)
  cboMUX2.Text = Str(YM2save)
  cboMUX3.Text = Str(YM3save)
  cboTB1.Text = Str(YT1save)
  cboTB2.Text = Str(YT2save)
  cboTB3.Text = Str(YT3save)
  cboTPT1.Text = Str(YP1save)
  cboTPT2.Text = Str(YP2save)
  cboTPT3.Text = Str(YP3save)
  cmdGAIN_Click (1)             'Use full X scale and Update display
  
Case Else

End Select

End Sub

Private Sub cmdGO_Click()
Dim RecordFound As Boolean, tks&, LoopCount%, RecInc!
  
On Error GoTo InvalidRequest

Select Case cboGOTO.Text
Case "Rec No"
  Record = Val(txtGOTO) - 1
Case "ks"
  Record = (Val(txtGOTO) * 1000 - UTmin) / Scanlength
Case "seconds"
  Record = (Val(txtGOTO) - UTmin) / Scanlength
Case "hhmmss"
  Record = (fTstringToSec&(txtGOTO, False) - UTmin) / Scanlength
Case "hh:mm:ss"
  Record = (fTstringToSec&(txtGOTO, True) - UTmin) / Scanlength
End Select
' Check limits
  Select Case Record
  Case Is < 0: Record = 0
  Case Is > TotalCycles: Record = TotalCycles - 1
  End Select
  
  RawCycle = Record
  RecordFound = False
  Forward = True
  LoopCount% = 0
  Do
    LoopCount% = LoopCount% + 1
    tmrGetCycle_Timer
    Select Case cboGOTO.Text
    Case "Rec No"
      RecordFound = True
    Case "ks"
      RecInc = (Val(1000 * txtGOTO.Text - UTsec)) / Scanlength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "seconds"
      RecInc = (Val(txtGOTO.Text - UTsec)) / Scanlength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "hhmmss"
      tks& = fTstringToSec&(txtGOTO.Text, False)
      RecInc = (tks& - UTsec) / Scanlength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "hh:mm:ss"
      tks& = fTstringToSec&(txtGOTO.Text, True)
      RecInc = (tks& - UTsec) / Scanlength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    End Select
    Cycle = Cycle + RecInc
  Loop Until RecordFound Or LoopCount% > 5
  UpdateForm
  If chkRetrieveSingleScan.Value = 1 Then
    Forward = True
    tmrGetCycle_Timer
  End If
Exit Sub

InvalidRequest:
  Msg = "Invalid GOTO request" ' Define message.
  Style = vbOK                 ' Define buttons.
  Title = "WARNING!"           ' Define title.
  Help = ""                    ' Define Help file.
  Ctxt = 0                     ' Define topic context.
' Display message.
  response = MsgBox(Msg, Style, Title, Help, Ctxt)

End Sub

Private Sub cmdMissionSummary_Click()
Dim LI%, OUTlu%, out$, seconds&, FltCount%, TotalTime&, i%, hms$

  OUTlu = FreeFile
  Open MNpath$ + "MissionSummary.txt" For Output As #OUTlu
  Print #OUTlu, Path$ + AC$
  For i = 0 To frmFLTINFO.cboFltDates.ListCount - 1
    frmFLTINFO.cboFltDates.Text = frmFLTINFO.cboFltDates.List(i)
    REFfile$ = Path$ + AC$ + Trim(frmFLTINFO.cboFltDates.List(i)) + ".REF"
    If chkFixTO.Value = 1 Then
      ReadRAWfile
      REFfile$ = "C:\MTP\Setup\FLTINFO.REF"
    End If
    REFreadHEADER (REFfile$)
    DoEvents
    'Debug.Print frmFLTINFO.cboFltDates.List(i); UTtakeoff; UTlanding
    If UTmax < UTmin Then UTmax = UTmax + 86400
    seconds = UTmax - UTmin
    hms$ = fSecToTstring(seconds, True)
    out$ = Trim(frmFLTINFO.cboFltDates.List(i)) + "  " + Format(UTmin, "000000") + "  " + Format(UTmax, "000000") + "  " + Format(seconds, "000000") + "  " + hms$ + "  "
    If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
    seconds = UTlanding - UTtakeoff
    TotalTime = TotalTime + seconds
    hms$ = fSecToTstring(seconds, True)
    out$ = out$ + Format(UTtakeoff, "000000") + " " + fSecToTstring(UTtakeoff, True) + "  " + Format(UTlanding, "000000") + " " + fSecToTstring(UTlanding, True) + "  " + Format(seconds, "000000") + "  " + hms$ + "  "
  
    Print #OUTlu, out$
  Next i
  Print #OUTlu, "Flt Count= " + Str(frmFLTINFO.cboFltDates.ListCount)
  Print #OUTlu, "Elapsed Time (s)= " + Str(TotalTime)
  Print #OUTlu, "Elapsed Time (h)= " + fSecToTstring(TotalTime, True)
  Close OUTlu%
    
    

End Sub

Private Sub cmdMerge_Click()
Dim MPeditlu%, MPeditFile$, MPyyyymmdd$, MPch%, UTsec&, UTstart&, UTend&, MPfilename$, CHxlu0%, CHXlu%, CH123lu%
Dim MPfilenameOut$, Mergelu%, A$, b$, out$, i%

  MPeditFile$ = MNpath$ + "MP\PREDICT_MPedit.txt"
  MPeditlu = FreeFile
  Open MPeditFile$ For Input As MPeditlu
  Do
'20100901 3 54717 57297
'12345678901234567890
    Line Input #MPeditlu, A$
    MPyyyymmdd$ = Left$(A$, 8)
    MPch = Mid$(A$, 10, 1)
    UTstart = Mid$(A$, 12, 5)
    UTend = Mid$(A$, 17, 6)
    MPfilename$ = "MP" + MPyyyymmdd$ + ".NGV"
    MPfilenameOut$ = "MP" + MPyyyymmdd$ + ".NGVO"
    
    CH123lu = FreeFile
    Open MNpath$ + "MP\123\" + MPfilename$ For Input As CH123lu
    
    CHXlu = FreeFile
    Open MNpath$ + "MP\" + Format(MPch, "0") + "\" + MPfilename For Input As CHXlu
    
    OUTlu = FreeFile
    Open MNpath$ + "MP\123\" + MPfilenameOut$ For Output As OUTlu
    
    For i = 1 To 64
      Line Input #CH123lu, b$
      Print #OUTlu, b$
      Line Input #CHXlu, A$
    Next i
    Do  'Go to UTstart in MP123 file
      Line Input #CH123lu, b$
      If Len(b$) > 30 Then UTsec = Left$(b$, 7)
      Print #OUTlu, b$
    Loop Until UTsec >= UTstart
    Line Input #CH123lu, b$
    While Len(b$) < 30
      Print #OUTlu, b$
      Line Input #CH123lu, b$
    Wend
'    Print #OUTlu, B$
    Do  'CHXlu catchup
      Line Input #CHXlu, b$
      If Len(b$) > 30 Then UTsec = Left$(b$, 7)
    Loop Until UTsec >= UTstart
    Line Input #CHXlu, b$
    While Len(b$) < 30
'      Print #OUTlu, B$
      Line Input #CHXlu, b$
    Wend
    Print #OUTlu, b$
    Do 'Substittute CHx from UTstart to UTend
      Line Input #CHXlu, b$
      If Len(b$) > 30 Then UTsec = Left$(b$, 7)
      Print #OUTlu, b$
    Loop Until UTsec >= UTend
'    Print #OUTlu, B$
    Line Input #CHXlu, b$
    While Len(b$) < 30
      Print #OUTlu, b$
      Line Input #CHXlu, b$
      Debug.Print b$
    Wend
    UTsec = -1
    Do  'Go to UTend in MP123 file
      Line Input #CH123lu, b$
      Debug.Print b$
      If Len(b$) > 30 Then UTsec = Left$(b$, 7)
    Loop Until UTsec >= UTend
    Line Input #CH123lu, A$
    While Len(A$) < 30
      Line Input #CH123lu, A$
      Debug.Print A$
    Wend
    Print #OUTlu, A$

    Do  'Copy rest of MP123 file
      Line Input #CH123lu, b$
      Print #OUTlu, b$
    Loop Until EOF(CH123lu)
    Close CH123lu, OUTlu, CHXlu
    Kill MNpath$ + "MP\123\" + MPfilename$
    FileCopy MNpath$ + "MP\123\" + MPfilenameOut$, MNpath$ + "MP\123\" + MPfilename$
    Kill MNpath$ + "MP\123\" + MPfilenameOut$
  Loop Until EOF(MPeditlu)
  Close MPeditlu

End Sub

Private Sub cmdMUX_Click(Index As Integer)
Dim i%, j%, M%, m1%, m2%, MUXmin!, MUXmax!, x!

Select Case Index
Case 0
  For i = 0 To MVchannels: chkMux(i).Value = 0: Next i
  
Case 1
  FirstMouseX = 1
  If Not RealTime Then LastMouseX = TotalCycles
  FirstMouseXSave = 1
  If Not RealTime Then LastMouseXSave = TotalCycles
  UpdateX1X2
  Call UpdateMUXrange
  
Case 2
  For i = 0 To MVchannels: chkMux(i).Value = 1: Next i

Case 3
  Call UpdateMUXrange

Case 4 'Clear SStab2 window
  Select Case SSTab2.Tab
  Case 0:  For i = 0 To 16: chkMux(i).Value = 0: Next i
  Case 1:  For i = 20 To 31: chkMux(i).Value = 0: Next i
  Case 2:
    For i = 32 To 38: chkMux(i).Value = 0: Next i
    For i = 17 To 19: chkMux(i).Value = 0: Next i
  Case 3: For i = 39 To MVchannels: chkMux(i).Value = 0: Next i
  End Select

Case 5 'Set SStab2 window
  Select Case SSTab2.Tab
  Case 0
    If Left$(SU$, 3) = "CAN" Then
      For i = 0 To 24: chkMux(i).Value = 1: Next i
    Else
      For i = 0 To 16: chkMux(i).Value = 1: Next i
    End If
  Case 1:  For i = 20 To 31: chkMux(i).Value = 1: Next i
  Case 2:
    For i = 32 To 38: chkMux(i).Value = 1: Next i
    For i = 17 To 19: chkMux(i).Value = 1: Next i
  Case 3: For i = 39 To MVchannels: chkMux(i).Value = 1: Next i
  End Select

Case 6
  YC0save = 0
  YG0save = 0
  YM0save = 0
  YT0save = 0
  YP0save = 0
  cboCTS1.Text = Str(YC1save)
  cboCTS2.Text = Str(YC2save)
  cboCTS3.Text = Str(YC3save)
  cboGAIN1.Text = Str(YG1save)
  cboGAIN2.Text = Str(YG2save)
  cboGAIN3.Text = Str(YG3save)
  cboMUX1.Text = Str(YM1save)
  cboMUX2.Text = Str(YM2save)
  cboMUX3.Text = Str(YM3save)
  cboTB1.Text = Str(YT1save)
  cboTB2.Text = Str(YT2save)
  cboTB3.Text = Str(YT3save)
  cboTPT1.Text = Str(YP1save)
  cboTPT2.Text = Str(YP2save)
  cboTPT3.Text = Str(YP3save)

'  cmdMUX_Click (5)  'disable XY
  cmdMUX_Click (1)  'Use full X scale and Update display
  
Case 7
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  MUXmin = 10000
  MUXmax = -10000
  For j = 0 To MVchannels
    If chkMux(j).Value = 1 And j <> 7 And j <> 16 Then
      For M = m1 To m2
        If j > 38 And j < 47 Then x = 5 * (47 - j) * MV(j, M) / 2 ^ (46 - j) Else x = MV(j, M)
        If x < MUXmin Then MUXmin = x
        If x > MUXmax Then MUXmax = x
      Next M
      DoEvents
    End If
  Next j
  MUXmax = Int(MUXmax) + 1
  MUXmin = Int(MUXmin)
  Select Case MUXmax - MUXmin
  Case Is > 500: cboMUX3.Text = "50"
  Case Is > 400: cboMUX3.Text = "40"
  Case Is > 300: cboMUX3.Text = "30"
  Case Is > 200: cboMUX3.Text = "20"
  Case Is > 100: cboMUX3.Text = "10"
  Case Is > 50: cboMUX3.Text = "5"
  Case Is > 20: cboMUX3.Text = "2"
  Case Else: cboMUX3.Text = "1"
  End Select
  cboMUX1.Text = 10 * Int(MUXmin / 10)
  cboMUX2.Text = 10 * Int(MUXmax / 10) + 10
  Call UpdateMUXrange

Case Else
End Select

End Sub



Public Sub cmdREF_Click(Index As Integer)
Dim i%, j%, M%, r%, jj&, Ext$, ZpMin!, A$
Dim fn$, NUMlu%, Objective$, n%, FltDate&, MPlu%, Filename$, MPpath$, OUTlu%, Filename2$
Dim SaveCaption$, lu%

' If in REALTIME mode and not Exit command
  If RealTime And Index <> 3 Then
    MsgBox "Not valid command when in REALTIME mode" + vbCrLf + "Go to FLTinfo Ad Hoc tab to get out!", vbOK
    Exit Sub
  End If
  
' If not View REF command save default FLTINFO file
  If Index <> 2 And Not RealTime Then frmFLTINFO.cmdWriteDefault_Click 'Make sure any changes are saved
  
' Process REF command
  Select Case Index
  Case 0 'Read RAW file into memory and calculate moving averages
    SaveCaption = cmdREF(0).Caption
    cmdREF(0).Caption = "Processing"
    ReadRAWfile
    REFwriteHEADER (REFfile$)
    ArrayRead = True
    If RAWextension <> "RAW" Then  'Load Tptz, Pptz and Zptz arrays, UTptz, iPTZ
      ZpMin = 6#                   'Must be above 5 km (below freezing T) for some probes to work!
      ReadPTZtoArray (ZpMin!)      '999.99 data are ignored.
    End If

    cmdCTS_Click (1)      'UpdateCTSrange called from here
    Call UpdateMUXrange
    cmdCTS_Click (11)     'Scale
    cmdMUX_Click (7)      'Scale
    If DoAll Then frmTools.tmrBatch1.Enabled = True
    cmdREF(0).Caption = SaveCaption
    txtATP_RAOB2.Text = RSpath$ + Mission$ + "_ATP.RAOB2"
    frmFLTINFO.txtTTO.Text = Format(NAVoffset * ScanTime, "#0.0")
  
  Case 1 'Plot CTC
    With frmCurtain
    .Show
    .lblMTP_Click
    End With
    If DoAll Then frmTools.tmrBatch1.Enabled = True

  Case 2 'View REF
    With frmREFview
    .txtREFfile.Text = frmFLTINFO.txtSourceFile.Text
    .FileRecords = Val(txtFileRecords.Text)
    .Nparms = 38
    .Show
    End With

  Case 3 'Exit
    Close
    Quit = True
'    frmFLTINFO.cmdWriteDefault_Click
    Unload Me
    End

  Case 4 'Write OAT time series
    fn$ = Dir(ERFfile$, vbNormal)  'Check if ERF exits
    If fn$ = "" Then               'No ERF file, check for REF
      fn$ = Dir(REFfile$, vbNormal)
      If fn$ = "" Then             'Neither ERF or REF
        Call MsgBox("Neither requested ERF or REF file exist", vbOKOnly)
        Exit Sub
      Else
        fn$ = REFfile$
      End If
    Else
      fn$ = ERFfile$
    End If

    If MPlu <> 0 Then Close MPlu
    MPlu% = FreeFile
    Open fn$ For Random Access Read Write As MPlu% Len = Len(REF)
    txtREFfile.Text = REFfile$
'  FileRecords = LOF(MPlu%) \ Len(REF) - HiddenRecords
    ProgressBar1.Min = 0
    ProgressBar1.Max = TotalCycles
    txtFileRecords.Text = TotalCycles
    REFread MPlu, TotalCycles
    Scanlength = UTsec
    REFread MPlu, 1
    If UTsec > Scanlength Then Scanlength = Scanlength + 86400
    Scanlength = (Scanlength - UTsec) / (TotalCycles - 1)  'Needed for GTE format
  
    If chkGoodScansOnly.Value = 1 Then
      If chkWriteBadHeaders.Value = 1 Then
        Call TSwrite(MPlu, True, True, frmMTPbin)
      Else
        Call TSwrite(MPlu, True, False, frmMTPbin)
      End If
    Else
      If chkWriteBadHeaders.Value = 1 Then
        Call TSwrite(MPlu, False, True, frmMTPbin)
      Else
        Call TSwrite(MPlu, False, False, frmMTPbin)
      End If
    End If

  Case 5   'Start serial IO
    tmrSerialStart.Enabled = True
    Load frmSerialIO
    frmSerialIO.Visible = True
    SerialIO = True
  
  Case 6 'Write FLTINFO
    With frmFLTINFO
    .cmdWritePath_Click
    .cmdWriteDefault_Click
    End With

  Case 7  'Edit REF data to ERF file
  ' Write FLTINFO.INI to capture USE5$
    SaveCaption = cmdREF(7).Caption
    cmdREF(7).Caption = "Processing"
    ERFstamp = Date + Time
    frmFLTINFO.UpdateForm
    frmFLTINFO.UpdateFLTINFO
    WriteFLTINFO (PathDot$ + "INI")
    REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"
    frmFLTINFO.SSTab1.Tab = 2
  
    ERFwrite
  
' Write FLTINFO.INI again to capture Edit statistics
    UpdateFLTINFO
    WriteFLTINFO (PathDot$ + "INI")
    REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"
    REFwriteHEADER REFfile$
    REFwriteHEADER ERFfile$
    If Len(Dir(RAFfile$)) > 0 Then REFwriteHEADER RAFfile$
    ' xxxmjm 20110124 If DoAll Then frmTools.tmrBatch1.Enabled = True
    cmdREF(7).Caption = SaveCaption

  Case 9 'Write MP file
    fn$ = Dir(ERFfile$, vbNormal)  'Check if ERF exits
    If fn$ = "" Then               'No ERF file, check for REF
     fn$ = Dir(REFfile$, vbNormal)
      If fn$ = "" Then             'Neither ERF or REF
        Call MsgBox("Neither requested ERF or REF file exist", vbOKOnly)
        Exit Sub
      Else
        fn$ = REFfile$
      End If
    Else
      fn$ = ERFfile$
    End If

'    If MPlu <> 0 Then Close MPlu
    Call ERFopen(ERFfile$)
'    MPlu% = FreeFile
'    Open fn$ For Random Access Read Write As MPlu% Len = Len(REF)
    txtREFfile.Text = REFfile$
'   FileRecords = LOF(MPlu%) \ Len(REF) - HiddenRecords
    ProgressBar1.Min = 0
    ProgressBar1.Max = TotalCycles
    txtFileRecords.Text = TotalCycles
'    REFread ERFlu, TotalCycles
'    Scanlength = UTsec
'    REFread ERFlu, 1
'    If UTsec > Scanlength Then Scanlength = Scanlength + 86400
'    Scanlength = (Scanlength - UTsec) / (TotalCycles - 1)  'Needed for GTE format
    If ScanTime = 0 Then cmdREF_Click (0)
    Scanlength = ScanTime
    If chkGoodScansOnly.Value = 1 Then
      If chkWriteBadHeaders.Value = 1 Then
        If chkWriteICT.Value = 1 Then
          Call MTPwrite(ERFlu, True, True, frmMTPbin)
        Else
          Call MPwrite(ERFlu, True, True, frmMTPbin)
        End If
      Else
        If chkWriteICT.Value = 1 Then
          Call MTPwrite(ERFlu, True, False, frmMTPbin)
        Else
          Call MPwrite(ERFlu, True, False, frmMTPbin)
        End If
      End If
    Else
      If chkWriteBadHeaders.Value = 1 Then
        If chkWriteICT.Value = 1 Then
          Call MTPwrite(ERFlu, False, True, frmMTPbin)
        Else
          Call MPwrite(ERFlu, False, True, frmMTPbin)
        End If
      Else
        If chkWriteICT.Value = 1 Then
          Call MTPwrite(ERFlu, False, False, frmMTPbin)
        Else
          Call MPwrite(ERFlu, False, False, frmMTPbin)
        End If
      End If
    End If
    If DoAll Then frmTools.tmrBatch1.Enabled = True
  
  Case 10   'Send UDP IWG1 1 Hz data
'    If cmdREF(10).Caption = "Send UDP" Then
'      cmdREF(10).Caption = "Stop UDP"
'      frmUDP.Show
'    Else
'      cmdREF(10).Caption = "Send UDP"
'      frmUDP.Timer1.Enabled = False
'      frmUDP.Hide
'    End If

  Case 11  'Do Retrieval
  
'Tom additions
Dim tblu%
tblu% = FreeFile
Open "C:\MTP\TBal.txt" For Append As tblu%
Print #tblu%, Mission, YYYYMMDD, HHMMSS
Print #tblu%, "Sec", "HHMMSS", "PALT", "Lat", "Lon", "CV(1,6)", "CV(2,6)", "CV(3,6)", "Good"
Close (tblu%)

    If Not ArrayRead Then  'Make sure a file is OPEN
      MsgBox "Open a file by depressing Open Raw button directly above!", vbOKOnly
      Exit Sub
    End If
    
    NewStart = True
    NewRetrieval = True
    DoRetrieval = True
    UTsecIlast = -1
    txtREFfile.Text = REFfile$
    REFreadHEADER (BINfile$)          'Put here after reading rawfile
    REFwriteHEADER (REFfile$)
    UpdateFLTINFO
    frmFLTINFO.UpdateFLTINFO
    WriteFLTINFO (PathDot$ + "INI")
    
    Initialize
    Initialize_RCs_Calfile   ' Do this stuff only if Retrievals are needed
  ' Must read RCs first (above call) to know whether Sensitivity matrix was found
    If chkRemoveOBbias.Value = 1 Then RemoveOBbias = True Else RemoveOBbias = False
    If chkBiasMode.Value = 1 Then BiasMode = 2 Else BiasMode = 1
    If Not FoundSmatrix Then chkUseSmatrix.Value = 0  'Didn't find S matrix
    If chkUseSmatrix.Value = 1 Then UseSmatrix = True Else UseSmatrix = False

    REFwriteHEADER REFfile$

'   Get timer going and set interval depending on Mode
    If chkRetrieveSingleScan.Value = 0 Then 'Retrieve all scans
      cmdDirection(1).Caption = "&Stop"
      Cycle = 0
      RawCycle = 0
      tmrGetCycle.Enabled = True
    End If
    For i = 0 To NRCbin - 1: RCuse(i) = 0: txtRCuse(i).Text = "": frmFLTINFO.txtRCuse(i).Text = "": Next i
    Forward = True
    tmrGetCycle.Interval = 50
    
  Case 12  'Fix blank MP file lines
    i = 0
    lu = FreeFile
    Open "C:\MTP\Data\ER2\TC4\MP\MP20070805.ER2" For Input As lu
    OUTlu = FreeFile
    Open "C:\MTP\Data\ER2\TC4\MP\MP20070805x.ER2" For Output As OUTlu
    Do
      Line Input #lu, A$
      i = i + 1
      If Len(A$) > 0 Then Print #OUTlu, A$
    Loop Until EOF(lu)
    Close lu, OUTlu
  
  Case 13 'Open Next Flight
    With frmFLTINFO
      If .cboFltDates.ListIndex < .cboFltDates.ListCount - 1 Then
        .cboFltDates.ListIndex = .cboFltDates.ListIndex + 1
      Else
        MsgBox "There are no more flights to process!", vbOKOnly
        Exit Sub
      End If
      .cmdFlight_Click (5) 'INfile_Click
      .cmdWriteDefault_Click
    End With
    cmdREF_Click (0)
    
  Case 14      'Open Tools Dialog Box
'    If cmdREF(14).Caption = "Open &Tools" Then
'      cmdREF(14).Caption = "Close &Tools"
      frmTools.Show
'    Else
'      cmdREF(14).Caption = "Open &Tools"
'      frmTools.Hide
'    End If

  Case 15   ' Remove blank lines from all MP files
    Filename$ = Dir(MPpath$ + "MP*" + fACext(AC$))
    Filename2$ = Filename$
    Do
      MPlu = FreeFile
      Open MPpath$ + Filename$ For Input As MPlu
      Input #MPlu, i, j        'get number of header lines
      Close MPlu
    
      MPlu = FreeFile
      Open MPpath$ + Filename$ For Input As MPlu
      OUTlu = FreeFile
      Open MPpath$ + fReplaceExtension(Filename$, "NEW") For Output As OUTlu
      jj = 0: n = 0
      Do
        Line Input #MPlu, A$
        jj = jj + 1
        If jj > i Then
          If Len(A$) > 5 Then
            Print #OUTlu, A$
            'Debug.Print a$
          Else
            n = n + 1
          End If
        Else
          Print #OUTlu, A$
        End If
      Loop Until EOF(MPlu)
      Close MPlu, OUTlu
      Kill MPpath$ + Filename$
      Name MPpath$ + fReplaceExtension(Filename$, "NEW") As MPpath$ + Filename$
      Filename$ = Dir()
    Loop Until Filename$ = Filename2$ 'Because Dir adds on renamed files

  Case Else
  End Select
  DoEvents
  
End Sub

Private Sub cmdRemoveFECerr_Click()
Dim RAWlu%, OUTlu%, A$, b$, C$, c1$
Dim AA$, Bb$, cC$, DD$, EE$, First As Boolean, FECerr As Boolean
Dim Aline As Boolean, Bline As Boolean, Cline As Boolean, Dline As Boolean, Eline As Boolean
    
  RAWlu = FreeFile
  Open RAWfile$ For Input As RAWlu
  OUTlu = FreeFile
  Open OUTfile$ For Output As OUTlu
  First = True
  Do
    Line Input #RAWlu, A$
  Loop Until Left$(A$, 1) = "A"
  Aline = False: Bline = False: Cline = False: Dline = False: Eline = False
  Do  'Remove fEc Errors
    If First Then First = False Else Line Input #RAWlu, A$
    'Check for FECerr messages in CRYSTAL-FACE test flight data
    c1$ = Left$(A$, 1)
    Select Case c1$
    Case "A"
      AA$ = A$: Aline = True: Bline = False: Cline = False: Dline = False: Eline = False
'      AA$ = Left$(AA$, 5) + ":" + Right$(AA$, Len(AA$) - 5)
    Case "B"
      If Len(A$) < 61 Then  'Skip FECerr scans
        Aline = False: Bline = False: Cline = False: Dline = False: Eline = False
      Else
        Bb$ = A$: Bline = True
      End If
    Case "C"
      If Len(A$) < 61 Then  'Skip FECerr scans
        Aline = False: Bline = False: Cline = False: Dline = False: Eline = False
      Else
        cC$ = A$: Cline = True
      End If
    Case "D"
      DD$ = A$: Dline = True
    Case "E"
      EE$ = A$: Eline = True
    Case Else
    End Select
    
    If Aline And Bline And Cline And Dline And Eline Then
      Print #OUTlu, AA$
      Print #OUTlu, Bb$
      Print #OUTlu, cC$
      Print #OUTlu, DD$
      Print #OUTlu, EE$
      Print #OUTlu, ""
      Aline = False: Bline = False: Cline = False: Dline = False: Eline = False
    End If
  Loop Until EOF(RAWlu)

  Close RAWlu, OUTlu
  
  Kill RAWfile$
  Name OUTfile$ As RAWfile$

End Sub

Private Sub cmdRTcatchup_Click()
Dim DestFilename$, CALfile$, DirPath$, ans, Filename$, RAWlu%, i%

  tmrRealTime.Enabled = False

' Set Initial directory
  CommonDialog1.InitDir = Drive$
' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  If Filename$ = "" Then Exit Sub
  CurrentStamp = FileDateTime(Filename$)   ' Returns "2/12/93 4:35:47 PM".
  RAWlu = FreeFile
  Open Filename$ For Input As RAWlu
  Do
    For i = 1 To 6: LineLast(i) = LineCurr(i): Next i
    Call ReadCURRENTrecord(RAWlu)
    CtsQual = True 'Normally obtained from ReadArray
    Main
    LastStamp = CurrentStamp
  Loop Until EOF(RAWlu)
  Close RAWlu
  tmrRealTime.Enabled = True  'Wait for file to update



End Sub









Private Sub cmdSaveATPasRAOB2_Click()
Dim RAOBlu%, i%, A$, ans As Variant, OutRecord&, HHMMSS$
  
'  Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
'  Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
'  Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
'  Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
'  Call FindXmin(jt1, jt2, RTavg(), RAavg(), 6#, 19#, Tcp, Zcp)
'  If RAOBgood Then RAOBedit = &H1
'  If TropGood Then RAOBedit = RAOBedit + &H2
'  If EditLevel Then RAOBedit = RAOBedit + &H4
'  If EditTrop Then RAOBedit = RAOBedit + &H8
'  If UseEditLevel Then RAOBedit = RAOBedit + &H10
'  If UseEditTrop Then RAOBedit = RAOBedit + &H20
'  RAOBedit = 3
  RAOBgood = True
  TropGood = True
  EditLevel = False
  EditTrop = False
  UseEditLevel = False
  UseEditTrop = False
  WMOnumber = 99999
  WMO4l = "XXXX"
  HHMMSS$ = fSecToTstring(UTsec, False)
  'DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  Iyear = Val(Left$(YYYYMMDD$, 4))
  Imonth = Val(Mid$(YYYYMMDD$, 5, 2))
  Iday = Val(Right$(YYYYMMDD$, 2))
  Ihour = Val(Left$(HHMMSS$, 2))
  Imin = Val(Mid$(HHMMSS$, 3, 2))
  Isec = Val(Right$(HHMMSS$, 2))
  
  Nlevr = Nlev
  Nedit = 0
  dTMRI = 0
  ijkl = 0
  OATraob = OATmtp
  OATraobPP = 0
  WMOlatitude = Latitude
  WMOlongitude = Longitude
  WMOaltitude = 0#
  zMRI1 = 0
  zMRI2 = 0
  Ztrop = Zt1
  Ttrop = TT1
  For i = 1 To Nlev
    If RAp(i) > Zt1 Then Exit For
  Next i
  Ltrop = i - 1
  Ztrop2 = Zt2
  Ttrop2 = TT2
  Ltrop2 = 0
  IntRhoV = 0#
  Tcoldest = Tcp
  Zcoldest = Zcp
  ZtropE = 0
  TtropE = 0
  LtropE = 0
  nRAOBlevels = 150
'  GZr2(1 To 153) As Integer       'Geometric Altitude [m]
'  PZr2(1 To 153) As Integer       'Pressure altitude  [0.1mb]
'  TZr2(1 To 153) As Integer       'Temperatures [10 mK]
'  RZr2(1 To 153) As Integer       'Relative Humidity [0.01%]
'  LZr2(1 To 3) As Integer         'Editted level number

  For i% = 1 To Nlevr: ZGraob(i%) = RAp(i): Next i
  For i% = 1 To Nlevr: ZPraob(i%) = RAp(i): Next i
  For i% = 1 To Nlevr: PZraob(i%) = fZtoP(RAp(i)): Next i
  For i% = 1 To Nlevr: TZraob(i%) = RTp(i): Next i
  For i% = 1 To Nlevr: RZraob(i%) = 0#: Next i
  
  OutRecord = 1
  If NewStart Then
    A$ = Dir(txtATP_RAOB2.Text, vbNormal)
    If A$ <> "" Then
      ans = MsgBox(txtATP_RAOB2.Text + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
      If ans = vbYes Then
        Kill (txtATP_RAOB2.Text)
        RAOBlu = FreeFile
        Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
        OutRecord = 1                           ' Overwrite file if it exists
      Else
        RAOBlu = FreeFile
        Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
        OutRecord = LOF(RAOBlu%) \ Len(RAOB2) + 1 ' Append to existing file
      End If
    End If
    NewStart = False
  Else
    RAOBlu = FreeFile
    Open Trim(txtATP_RAOB2.Text) For Random Access Read Write As RAOBlu Len = Len(RAOB2)
    OutRecord = LOF(RAOBlu%) \ Len(RAOB2) + 1 ' Append to existing file
  End If
  
  Call RAOBwriteSL(RAOBlu%, OutRecord&)
  Close RAOBlu

End Sub


Private Sub cmdStrip_Click()
Dim RAWfile$, INPlu%, OUTlu%, OUTfile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim L%, T$

  RAWfile$ = Path$ + AC$ + YYYYMMDD$ + ".RAW"
  OUTfile$ = Path$ + AC$ + YYYYMMDD$ + ".TXT"
  INPlu% = FreeFile
  Open RAWfile$ For Input As INPlu%
  OUTlu% = FreeFile
  Open OUTfile$ For Output As OUTlu%
  LOFraw = LOF(INPlu%)
  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
Do
  Line Input #INPlu, A$
  Select Case Left$(A$, 1)
  Case "A"
'AE999 18:12:39  41.168 -104.812 999   1.9 - 0.3 98118 10747  14.3 0008   0287   10.3 1 000811 181236
'1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
    If Mid$(A$, 3, 3) = "999" Then
      T$ = Right$(A$, 13)
      y% = Val(Left$(T$, 2))
      If y% < 80 Then y% = 2000 + y% Else y% = 1900 + y%
      M% = Val(Mid$(T$, 3, 2))
      D% = Val(Mid$(T$, 5, 2))
      Doy = fYMDtoDOY(y, M, D)
      Mid$(A$, 3, 3) = Format(Doy, "000")
    End If
    If Mid$(A$, 33, 1) = "9" Then  'Fix for first TexAQS flight
      L = Len(A$)
      A$ = Left$(A$, 32) + " " + Right$(A$, L - 32)  'Shift everything to right
    End If
    Print #OUTlu, A$
  
  Case "I", "B", "C", "D", "E"
    Print #OUTlu, A$
    If Left$(A$, 1) = "E" Then Print #OUTlu, ""
  Case Else
  End Select
  ByteCount = Loc(INPlu%) * 128
  If ByteCount < LOFraw Then frmMTPbin.ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
  DoEvents
Loop Until EOF(INPlu)
Close INPlu, OUTlu
Kill RAWfile$
Name OUTfile$ As RAWfile$
End Sub

Public Sub cmdTB_Click(Index As Integer)
Dim i%, TBmin!, TBmax!, Gsensitivity!(1 To 3), Psave!(1 To 3), AVGnew!(1 To 3), AVGsave!(1 To 3)
Dim M%, j%, n%, TmtpTnavThreshold!, Tmtp!, Tref!, m1%, m2%, g!(1 To 3), T!(1 To 3)
Dim dTsum1!, dTsum2!, RMS!, AVG!, RMSmin!(0 To 3), offset!, GoodCounts%, ScanTime!, n1%(0 To 3), dT!
Dim C%, FL1!, FL2!, FL!, dFL!, nGood%, iFL%, Tavg!, offsetE!(0 To 3)

Select Case Index
Case 0 'Do Automatic Fit to TB data
  OATnavCOR = Val(frmFLTINFO.txtOATnavCOR.Text)
  frmFLTINFO.cboREFsource.ListIndex = 0 ' "TGT"
  REFsource = 1
  cmdTB_Click (4)  'Restore Fit Region
  Call cmdTB_Click(3)
  Select Case frmFLTINFO.cboGscale.Text
  Case "ND"
    ' First determine CH sensitivity
    For i = 1 To Channels
      Psave(i) = Val(frmFLTINFO.txtCnd0(i - 1).Text)
      AVGsave(i) = Val(txtDTa(i).Text)
    Next i
    ' Add 1K to ND Temperature
    For i = 1 To Channels: frmFLTINFO.txtCnd0(i - 1).Text = Str(Psave(i) + 1): Next i
    DoEvents
    Call cmdTB_Click(3)  'Populates text boxes with results
    For i = 1 To Channels
      AVGnew(i) = Val(txtDTa(i).Text)
      Gsensitivity(i) = AVGnew(i) - AVGsave(i)
      If Gsensitivity(i) <> 0# Then
        frmFLTINFO.txtCnd0(i - 1).Text = Format(Psave(i) - AVGsave(i) / Gsensitivity(i), "##0.000")
      Else
        frmFLTINFO.txtCnd0(i - 1).Text = Val(Psave(i))
      End If
    Next i
    
  Case "EQN"
    ' First determine CH sensitivity
    For i = 1 To Channels
      Psave(i) = Val(frmFLTINFO.txtGEval((i - 1) * 5).Text)
      AVGsave(i) = txtDTa(i).Text
    Next i
    ' Add 0.5 counts/K to gains
    For i = 1 To Channels: frmFLTINFO.txtGEval((i - 1) * 5).Text = Str(Psave(i) + 0.5): Next i
    Call cmdTB_Click(3)
    For i = 1 To Channels
      AVGnew(i) = txtDTa(i).Text
      Gsensitivity(i) = 2 * (AVGnew(i) - AVGsave(i))
      frmFLTINFO.txtGEval((i - 1) * 5).Text = Format(Psave(i) - AVGsave(i) / Gsensitivity(i), "#0.000")
    Next i
  
  Case Else
  End Select
  
  cmdTB_Click (3)
  If fGetSelectedOption(frmFLTINFO.optChInfo) = 2 Then
    For i = 0 To 2
      frmFLTINFO.txtChInfo(i) = Format(TBfitRMS(i), "0.00")
    Next i
  End If
  frmFLTINFO.cboREFsource.ListIndex = 1 ' "OAT"
  REFsource = 2
  frmFLTINFO.cmdWriteDefault_Click
  frmFLTINFO.cmdWritePath_Click
  
Case 1
  FirstMouseX = 1
  If Not RealTime Then LastMouseX = TotalCycles
  FirstMouseXSave = 1
  If Not RealTime Then LastMouseXSave = TotalCycles
  UpdateX1X2
  Call UpdateTBrange
  
Case 2  'Save Fit Region
  With frmFLTINFO
  .txtXfit(0).Text = txtX1(2).Text
  .txtXfit(1).Text = txtX2(2).Text
  .txtYfit(0).Text = cboTB1.Text
  .txtYfit(1).Text = cboTB2.Text
  End With
  
Case 3
  frmFLTINFO.cmdWriteDefault_Click
  Call UpdateTBrange

Case 4   'Restore Fit Region
  With frmFLTINFO
  txtX1(2).Text = .txtXfit(0).Text
  txtX2(2).Text = .txtXfit(1).Text
  cboTB1.Text = .txtYfit(0).Text
  cboTB2.Text = .txtYfit(1).Text
  End With
  FirstMouseX = Val(txtX1(2).Text)
  LastMouseX = Val(txtX2(2).Text)
  Call UpdateTBrange

Case 5 'Scale to TB range
  TBmax = 0: TBmin = 400
  For i = 1 To TotalCycles
      If OATV(1, i) < TBmin Then TBmin = OATV(1, i)
      If OATV(1, i) > TBmax Then TBmax = OATV(1, i)
  Next i

  TBmax = Int(TBmax) + 1
  TBmin = Int(TBmin)
  cboTB1.Text = TBmin
  cboTB2.Text = TBmax
  If TBmax - TBmin > 15 Then cboTB3.Text = "2" Else cboTB3.Text = "1"
  Call UpdateTBrange

Case 6
  YC0save = 0
  YG0save = 0
  YM0save = 0
  YT0save = 0
  YP0save = 0
  cboCTS1.Text = Str(YC1save)
  cboCTS2.Text = Str(YC2save)
  cboCTS3.Text = Str(YC3save)
  cboGAIN1.Text = Str(YG1save)
  cboGAIN2.Text = Str(YG2save)
  cboGAIN3.Text = Str(YG3save)
  cboMUX1.Text = Str(YM1save)
  cboMUX2.Text = Str(YM2save)
  cboMUX3.Text = Str(YM3save)
  cboTB1.Text = Str(YT1save)
  cboTB2.Text = Str(YT2save)
  cboTB3.Text = Str(YT3save)
  cboTPT1.Text = Str(YP1save)
  cboTPT2.Text = Str(YP2save)
  cboTPT3.Text = Str(YP3save)
  cmdTB_Click (1)                 'Use full X scale and Update display
  
Case 7 ' Tmtp - Tnav filter for bad scans due to stepper motor sticking
' Check for Tmtp-Tnav rejection
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  TmtpTnavThreshold = Val(frmFLTINFO.txtTmtpTnav.Text)
  If TmtpTnavThreshold > 0 Then
    For M = m1 To m2
      Call CalculateGain(M, g())
      
      Tmtp = 0#
'      ChInfoSum = 0#
      For i = 1 To Channels
        If g(i) = 0 Then g(i) = 16
        Tref = Ttgt + cTo
        T(i) = fTB(Tref, CS(i, LocHor), CB(i), g(i))
'        ChInfoSum = ChInfoSum + ChInfo(i)
        Tmtp = Tmtp + T(i) * ChInfo(i)
      Next i
      Tmtp = Tmtp / ChInfoSum
      dT = Tmtp - OAT                'V(1, M + NAVoffset) + OATnavCOR
      If Abs(dT) > TmtpTnavThreshold Then
        GoodScan = False
        CQV(M) = False
      End If
    Next M
  Else
    MsgBox "WARNING: Tmtp - Tnav threshold is zero!" + vbCrLf + "Set on frmFLTINFO GAIN tab.", vbOKOnly
  End If
  
Case 8  'Determine TTO
  cmdTB(8).Caption = "BUSY"
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  frmFLTINFO.UpdateFLTINFO
  ScanTime = fScanLength(m1, m2)
    
  For C = 0 To Channels
    RMSmin(C) = 100#
  ' Do first pass with 0.1 scan resolution +/- two scans
  ' Following For loop had gone from 0 to 30. This was a problem for the GV
  ' because the longer scan time meant that the MTP horizon temperature actually lagged the in situ temperature measurement!!
    For n = -20 To 20  '-/+ two scans
      aNAVoffset(0) = n / 10#
      NAVoffset = aNAVoffset(0)
      Call CalculateTTOstatistics(C%, m1, m2, AVG!, RMS!)
      If RMS < RMSmin(C) Then
        RMSmin(C) = RMS: offset = n / 10#: n1(C) = n * 10
      End If
    Next n
    'Debug.Print C; n1(C) / 10; RMSmin(C); offset; offset * ScanTime
  Next C
' Do second pass with 0.01 scan resolution
  For C = 0 To Channels
    RMSmin(C) = 100#
    For n = n1(C) - 10 To n1(C) + 10
      aNAVoffset(0) = n / 100#
      NAVoffset = aNAVoffset(0)
      Call CalculateTTOstatistics(C%, m1, m2, AVG!, RMS!)
      If RMS < RMSmin(C) Then
        RMSmin(C) = RMS: offset = n / 100#: n1(C) = n * 10
      End If
    Next n
    'Debug.Print C; Format(n1(C) / 100, "#0.0"); "  "; "  "; Format(RMSmin(C), "#0.00"); "  "; Format(offset, "#0.00"); "  "; Format(offset * ScanTime, "#0.00"); "  "; nGood; FL; dFL
    frmFLTINFO.txtNAVoffset(C).Text = Format(offset, "0.00")
  Next C
  NAVoffset = Val(frmFLTINFO.txtNAVoffset(0).Text)
  cmdTB(8).Caption = "TTO"
  frmFLTINFO.txtTTO.Text = Format(NAVoffset * ScanTime, "#0.0")
  cmdTB_Click (0)  'do fit automatically after TTO
  
Case 9   'TTO-Enhanced Does TTO calculation at multiple flight levels
  cmdTB(8).Caption = "BUSY"
  m1 = Val(FirstMouseX)
  m2 = Val(LastMouseX)
  If m1 = 0 And m2 = 0 Then Exit Sub
  frmFLTINFO.UpdateFLTINFO
  ScanTime = fScanLength(m1, m2)
  dFL = 0.5   ' Distance (km) above and below flight level
  
  Debug.Print Mission$ + "  "; YYYYMMDD; m1; m2; UV(1, m1); UV(1, m2); Format(ScanTime, "00.000"); "  "; dFL; Format(LO(1), "00.000"); "  "; Format(LO(2), "00.000"); "  "; Format(LO(3), "00.000"); "  "
  Debug.Print "Zp    N    Tavg   All    CH1    CH2    CH3"
  For iFL = 1 To 15  'Flight levels at which to do averaging
    FL = iFL
    FL1 = FL - dFL: FL2 = FL + dFL   '+/- dFL = 500 meters
  
    For C = 0 To Channels
      RMSmin(C) = 100#
      ' Do first pass with 0.1 scan resolution +/- two scans
      ' Following For loop had gone from 0 to 30. This was a problem for the GV
      ' because the longer scan time meant that the MTP horizon temperature actually lagged the in situ temperature measurement!!
      For n = -20 To 20
        aNAVoffset(0) = n / 10#
        NAVoffset = aNAVoffset(0)
        Call CalculateTTOstatisticsE(C%, m1, m2, FL1!, FL2!, AVG!, RMS!, Tavg!, nGood)
        If RMS < RMSmin(C) Then RMSmin(C) = RMS: offsetE(C) = n / 10#: n1(C) = n * 10
      Next n
      'Debug.Print C; n1(C) / 10; RMSmin(C); offset; offset * ScanTime; nGood; FL; dFL
    Next C
'   Do second pass with 0.01 scan resolution
    For C = 0 To Channels
      RMSmin(C) = 100#
      For n = n1(C) - 10 To n1(C) + 10
        aNAVoffset(0) = n / 100#
        NAVoffset = aNAVoffset(0)
        Call CalculateTTOstatisticsE(C%, m1, m2, FL1!, FL2!, AVG!, RMS!, Tavg!, nGood)
        If RMS < RMSmin(C) Then RMSmin(C) = RMS: offsetE(C) = n / 100#: n1(C) = n * 10
      Next n
      frmFLTINFO.txtNAVoffset(C).Text = Format(offset, "0.00")
    Next C
    NAVoffset = Val(frmFLTINFO.txtNAVoffset(0).Text)
    cmdTB(8).Caption = "TTO"
    frmFLTINFO.txtTTO.Text = Format(NAVoffset * ScanTime, "#0.0")
    If nGood > 0 Then
      Debug.Print Format(FL, "00.0"); "  "; Format(nGood, "000"); "  "; Format(Tavg, "000.0"); "  "; Format(offsetE(0), "#0.00"); "  "; Format(offsetE(1), "#0.00"); "  "; Format(offsetE(2), "#0.00"); "  "; Format(offsetE(3), "#0.00")
    End If
  Next iFL

Case Else

End Select

End Sub


Private Sub cmdTestIWG_Click(Index As Integer)
Dim i%, IWG_yyyymmdd$, IWG_hhmmss$, IWG_UTsec&, NAV_yyyymmdd$, NAV_hhmmss$, NAV_UTsec&
Static IWGlu%, UTstart&, IWGrecordMax&

  Select Case Index
  Case 0
    If cmdTestIWG(0).Caption = "Open IWG" Then
      IWGlu = FreeFile
      Open MNpath + "IWG\IWG1_" + YYYYMMDD$ + ".RIF" For Random Access Read Write As IWGlu Len = Len(IWG)
      IWGrecordMax = LOF(IWGlu%) \ Len(NAV)
      IWGrecord = 1
      Call IWGread(IWGlu%, IWGrecord&)
      Call DateTtoStrings(IWG_DandT, IWG_yyyymmdd, IWG_hhmmss, IWG_UTsec, True)
      UTstart = IWG_UTsec
      cmdTestIWG(0).Caption = "Close IWG"
      IWGforward = True
      tmrGoIWG.Enabled = True
      tmrGoIWG.Interval = 1000
      cmdTestIWG(4).Caption = "Stop"
    Else
      tmrGoIWG.Enabled = False
      Close IWGlu
      cmdTestIWG(0).Caption = "Open IWG"
      Exit Sub
    End If
    
  Case 1  'Forward
    If Not EOF(IWGlu) Then IWGrecord = IWGrecord + 1
    'If IWGrecord > IWGrecordMax Then IWGrecord = IWGrecordMax
    IWGforward = True
  
  Case 2  'GOTO
    Select Case cboGoIWG.Text
    Case "Rec No"
      IWGrecord = Val(txtGoIWG)
    Case "ks"
      IWGrecord = (Val(txtGoIWG) * 1000 - UTstart) + 1
    Case "seconds"
      IWGrecord = (Val(txtGoIWG) - UTstart) + 1
    Case "hhmmss"
      IWGrecord = (fTstringToSec&(txtGoIWG, False) - UTstart) + 1
    Case "hh:mm:ss"
      IWGrecord = (fTstringToSec&(txtGoIWG, True) - UTstart) + 1
    End Select
    
  Case 3
    IWGrecord = IWGrecord - 1
    If IWGrecord < 1 Then IWGrecord = 1
    IWGforward = False
  
  Case 4  'Continue/Stop
    If cmdTestIWG(4).Caption = "Continue" Then
      cmdTestIWG(4).Caption = "Stop"
      tmrGoIWG.Interval = 1000
      tmrGoIWG.Enabled = True
    Else
      cmdTestIWG(4).Caption = "Continue"
      tmrGoIWG.Enabled = False
    End If
    Exit Sub
  
  Case 5
    If cmdTestIWG(5).Caption = "Open NG" Then
      IWGlu = FreeFile
      Open MNpath + "NG\NG" + YYYYMMDD$ + ".RNF" For Random Access Read Write As IWGlu Len = Len(NAV)
      IWGrecordMax = LOF(IWGlu%) \ Len(NAV)
      IWGrecord = 1
      Call NAVread(IWGlu%, IWGrecord&)
      Call DateTtoStrings(NAV_DandT, NAV_yyyymmdd, NAV_hhmmss, NAV_UTsec, True)
      UTstart = NAV_UTsec
      cmdTestIWG(5).Caption = "Close NG"
      IWGforward = True
      tmrGoIWG.Enabled = True
      tmrGoIWG.Interval = 1000
      cmdTestIWG(4).Caption = "Stop"
    Else
      tmrGoIWG.Enabled = False
      Close IWGlu
      cmdTestIWG(5).Caption = "Open NG"
      Exit Sub
    End If

  End Select
  txtIWGrecord.Text = IWGrecord
  If cmdTestIWG(0).Caption = "Open IWG" Then
    UpdateNAVframe IWGlu, IWGrecord
  Else
    UpdateIWGframe IWGlu, IWGrecord
  End If

End Sub

Private Sub cmdTPT_Click(Index As Integer)
Dim i%, r%
Dim z!(1 To 33), T!(1 To 33)

Select Case Index
Case 0
  For i = 0 To 4: chkShowChannels(i).Value = 0: Next i
  
Case 1
  FirstMouseX = 1
  LastMouseX = TotalCycles
  FirstMouseXSave = 1
  LastMouseXSave = TotalCycles
  UpdateX1X2
  Call UpdateTBrange
  
Case 2
  For i = 0 To 4: chkShowChannels(i).Value = 1: Next i

Case 3
  frmFLTINFO.cmdWriteDefault_Click
  Call UpdateTPTrange

Case 4

Case 5 'Calculate isotherms/isentropes
  ThetaStart = Trim(cboParms(0).Text)
  ThetaEnd = Trim(cboParms(1).Text)
  ThetaStep = Trim(cboParms(2).Text)
  ThetaLevels = Int((ThetaEnd - ThetaStart)) / ThetaStep + 1
  ReDim zPT!(1 To ThetaLevels), Tptw!(1 To ThetaLevels, 1 To TotalCycles)
  
  For r = 1 To TotalCycles
    If CQV(r) Then
      Nlev = NW(0, r)
      For i = 1 To Nlev: z(i) = ZW(i, r): T(i) = Tw(i, r): Next i
      If optParms(1).Value = True Then
        Call CalculateIAClevels(z(), T(), zPT!(), ThetaStart!, ThetaStep!, ThetaLevels%)
      Else
        Call CalculateATPlevels(z(), T(), zPT!(), ThetaStart!, ThetaStep!, ThetaLevels%)
      End If
      For i = 1 To ThetaLevels:
        Tptw(i, r) = zPT(i)
      Next i
    End If
  Next r
  
Case 6
  YC0save = 0
  YG0save = 0
  YM0save = 0
  YT0save = 0
  YP0save = 0
  cboCTS1.Text = Str(YC1save)
  cboCTS2.Text = Str(YC2save)
  cboCTS3.Text = Str(YC3save)
  cboGAIN1.Text = Str(YG1save)
  cboGAIN2.Text = Str(YG2save)
  cboGAIN3.Text = Str(YG3save)
  cboMUX1.Text = Str(YM1save)
  cboMUX2.Text = Str(YM2save)
  cboMUX3.Text = Str(YM3save)
  cboTB1.Text = Str(YT1save)
  cboTB2.Text = Str(YT2save)
  cboTB3.Text = Str(YT3save)
  cboTPT1.Text = Str(YP1save)
  cboTPT2.Text = Str(YP2save)
  cboTPT3.Text = Str(YP3save)
  cmdTPT_Click (1)                 'Use full X scale and Update display
  
Case Else

End Select
End Sub









Private Sub Combo1_Change()

End Sub













Private Sub Command10_Click()

End Sub


Private Sub Command8_Click()
Dim Filename$, Filename2$, INPlu%, OUTlu%, cyclein%, cycleout%, UTsecLast&

  Filename$ = "c:/mtp/data/ngv/start08/20080504/NG20080504sav.REF"
  Filename2$ = "c:/mtp/data/ngv/start08/20080504/NG20080504.REF"
  INPlu = FreeFile
  Open Filename$ For Random Access Read Write As #INPlu Len = Len(REF2)
  REFreadHEADER (Filename$)
  
  OUTlu = FreeFile
  Open Filename2$ For Random Access Read Write As #OUTlu Len = Len(REF2)
  REFwriteHEADER (Filename2$)
  
  cyclein = 1
  cycleout = 0
  Call REFread33(INPlu%, cyclein%)
  UTsecLast = UTsec
  
  Do
    cyclein = cyclein + 1
    Call REFread33(INPlu%, cyclein%)
    If UTsec - UTsecLast < 21 Then
      If GoodScan And GoodTrop And Pitch > -2 And Pitch < 4 And Roll > -2 And Roll < 2 Then
        If UTsec > 60000 Then
          UTsec = UTsec
        End If
        cycleout = cycleout + 1
        Call REFwrite33(OUTlu%, cycleout%)
        
      End If
    End If
     Debug.Print cyclein; UTsec; UTsec - UTsecLast
     If UTsec > 57300 Then
      UTsec = UTsec
     End If
    UTsecLast = UTsec
    
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
End Sub

Private Sub Command9_Click()
Dim FQFN1$, FQFN2$, FilenameHTML$, FilenamePNG$

  FQFN1$ = "C:\www\missions\realtime\"
  FilenameHTML$ = "index.html"
  
  FQFN2$ = FQFN2$
'    CurrentStamp = FileDateTime(PathRAW + FilenameRAW)
        ' Send URL
'        Inet1.URL = "ftp://ftp.eol.ucar.edu"
        Inet1.Protocol = icFTP
        Debug.Print "2" & icError
        Inet1.URL = "ftp://130.134.190.11"
        Debug.Print "1" & icError
        GoSub InetStatus
          
'        Inet1.UserName = "anonymous"
        Inet1.UserName = "mtp"
        GoSub InetStatus
          
'        Inet1.Password = "Michael.J.Mahoney@jpl.nasa,gov"
        Inet1.Password = "emptypea"
        GoSub InetStatus
          
'        Inet1.Execute , "CD " + "/pub/temp/users/haggerty"
'        Inet1.Execute , "CD " + "/../mtp/public_html/"
'        GoSub InetStatus
        
'        Inet1.Execute , "DIR "
'        GoSub InetStatus
        
'        Inet1.Execute , "GET " & FQFN1$ & " " & FilenameHTML     'HTML code
        Inet1.Execute , "PUT " & FQFN1$ & " " & FilenameHTML     'HTML code
        GoSub InetStatus
        
'        Inet1.Execute , "PUT " & FQFN2$ & " " & FilenamePNG      'MTPprofile.PNG
'        GoSub InetStatus
          
      
'    LastStamp = CurrentStamp
  
'  CommandStamp = FileDateTime("C:\MTP\RTA\Command.TXT")
'  If CommandStamp > LastCommandStamp Then
  

Exit Sub

InetStatus:
  Do While Inet1.StillExecuting
  DoEvents
  Loop
  'Debug.Print icError; Inet1.ResponseInfo
  Return
  
End Sub

Private Sub Form_DblClick()
  
  CaptureFormMJ Me, "", 3, True
  
End Sub

Private Sub Form_Load()
Dim A$, ReadLine As Boolean, xx#, RetVal As Variant, DefCalfile$, MyDate As Variant
Dim UT&, GoToNext As Boolean
Dim f As Form, lu%, Line$, Ext$, BadCycle%, UTsecLast&, Navg%, DirPath$, FirstAline As Boolean
Dim FileLength&, eFormat$, E$, i%, x$, RTyyyymmdd$

  Set f = frmFLTINFO
  Set CallingForm = frmMTPbin
  
  With frmMTPbin
  .Top = 0
  .Left = 0
  .Height = 5835
  .SSTab1.Tab = 0
  .Show
  End With
  
  GoSub LoadComboBoxes
  
' Make sure that size of REF record is valid!
  If Len(REF) <> 1000 Or Len(REF2) <> 2000 Then
    MsgBox "WARNING: REFrecord does not have the correct length!", vbOKOnly
    Exit Sub
  End If
   
  EnableMXY = True
  
' FLTinfo form options
  Load f
  With f
  .Top = frmMTPbin.Height
  .Left = frmMTPbin.Left + frmMTPbin.Width - frmFLTINFO.Width
  .Height = 5265
  .Show
  .SSTab1.Tab = 0     ' Select FLTINFO Tab
  End With
  If RealTime Or UseDefaultRCs Then f.cmdRCfiles_Click (0) 'Load whatever RCs are in the Mission\RC\ folder
  
 ' ATP form options
  With frmATP
  .Top = frmMTPbin.Height
  .Left = 0
  .Width = frmMTPbin.Width - f.Width
  .Height = f.Height '- 18
  .PlotSingleScan = True
  .BorderStyle = 2                    'Set to 0 for small plot
  .Visible = True
  '.ATP_Initialize
  End With
  ShowATP = True
  
' Hide all the picture box shape objects
  shpCTS.Visible = False
  shpMUX.Visible = False
  shpGAIN.Visible = False
  shpTB.Visible = False
  shpTPT.Visible = False
  
  Load frmSummary
  With frmSummary
  .Hide
  .Top = (f.Height + f.Top - frmSummary.Height)
  .Left = (f.Left - frmSummary.Width)
  End With
  
  Load frmTools
  With frmTools
  .Hide
  .Top = (768 * Screen.TwipsPerPixelY - frmTools.Height) / 2
  .Left = (1024 * Screen.TwipsPerPixelX - frmTools.Width) / 2
  End With
  
'  Load frmUDP
'  With frmUDP
'  .Hide
'  .Top = (768 * Screen.TwipsPerPixelY - frmUDP.Height) / 2
'  .Left = (1024 * Screen.TwipsPerPixelX - frmUDP.Width) / 2
'  End With
  
  PgmDrive$ = ProgramDrive$
  Drive$ = DataDrive$
  MAKEversion = FileDateTime(PgmDrive$ + "\MTP\VB6\VBP\Main\MOAP\MTPbin.vbp")
'  EXEversion = FileDateTime(PgmDrive$ + "\MTP\VB6\EXE\MTPbin.exe")
  
  RCs$ = Left$(AC$, 1) + "RC"
  
  Load frmStatus
  ShowZp = True

  Select Case RTimageSize   'Screen.Width
  Case 800
    With frmStatus
    .Top = 765
    .Left = 6375 + 120  '120 is shift to frmATP to get on RT display
    .Show
    .optZx(0).Value = True
    .Visible = False
    End With
  Case 1024
    With frmStatus
    .Top = 830
    .Left = 8750 + 120
    .Show
    .optZx(0).Value = True
    .Visible = False
    End With
  Case 1280
    With frmStatus
    .Top = 800
    .Left = 8850 + 120
    .Show
    .optZx(0).Value = True
    .Visible = False
    End With
  
  End Select

' If DoAll Then cmdREF_Click (1)
  
  With txtCOM1
  .BackColor = &HFF&
  .ForeColor = &HFFFFFF
  .Text = "No COM1:"
  End With
  
' Default retrieval transition latitudes
  defLAT1 = 18: defLAT2 = 28: defLAT3 = 45: defLAT4 = 53
  
  If EditNav Then f.chkEditNav.Value = 1 Else f.chkEditNav.Value = 0
  If EditTA Then f.chkEditTA.Value = 1 Else f.chkEditTA.Value = 0
  If EditCts Then f.chkEditCts.Value = 1 Else f.chkEditCts.Value = 0
  If EditCycle Then f.chkEditCycle.Value = 1 Else f.chkEditCycle.Value = 0
  If EditRAWbad Then f.chkEditRAWbad.Value = 1 Else f.chkEditRAWbad.Value = 0
  If EditTtgt Then f.chkEditTtgt.Value = 1 Else f.chkEditTtgt.Value = 0

  chkShowTBs.Value = 0
  chkNewCalFormat.Value = 1
  chkEnableFit.Value = 1
  chkLOchanging.Value = 0
  
  ShowDiagnostics = True
  mnuViewData.Checked = True
  mnuViewATP.Checked = True
  ShowNDP = False

  If Mission$ = "TEST" Then Call ReadMissionRCS(Mission$, "", True)
  MVindex = 0
  TotalCycles = 3000
  GVindex = 0
  ArrayRead = False
  
  frmNDP.PlotSingleScan = True
  AutoScaleTA = False
  cmdAutoScaleTA_Click
  tmrSerialStart = False
  DoRetrieval = False
  
  'UpdateForm
  
  chkGain(0).Value = 1
  chkGain(1).Value = 1
  chkGain(4).Value = 1
  With frmFLTINFO
  .UpdateFLTINFO
  .txtUTmin.Text = Str(UTmin)
  .txtUTmax.Text = Str(UTmax)
  .txtUTtakeoff.Text = Str(UTtakeoff)
  .txtUTlanding.Text = Str(UTlanding)
  End With
  
  For i = 1 To Channels
    chkCH(i - 1).Value = 1
  Next i
  
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Check for REALTIME operation
' Flag set when MTPsetup.INI is read and can be changed using MTPsetup.exe
' MTPsetup.INI is read on the frmFLTINFO load event which precedes this code
  If RealTime Then
    chkNextAline.Value = 0
    GoSub Initialize         ' Only if REALTIME
    Initialize_RCs_Calfile   ' Do this stuff only if Retrievals are needed
    LastStamp = FileDateTime("C:\MTP\RTA\RAW\MTPcurrent.RAW")
    LastCommandStamp = FileDateTime("C:\MTP\RTA\Command.TXT")
    tmrRealTime.Interval = 2000
    tmrRealTime.Enabled = True
    If chkRemoveOBbias.Value = 1 Then RemoveOBbias = True Else RemoveOBbias = False
    If chkBiasMode.Value = 1 Then BiasMode = 2 Else BiasMode = 1
    mnuViewATP.Checked = True
    ShowATP = True
    With frmATP
    .Show
    .ScaleMode = 1 'twips
'    .WindowState = 2
    .Top = 0
    .Left = 120
    .Width = 800 * 15     'frmMTPbin.Width - f.Width
    .Height = 600 * 15    'f.Height '- 18
    End With
    ShowATP = True

    chkDisplayBadScans.Value = 1
    frmStatus.Visible = False
    
    TotalCycles = 4000   '5/min*60min/hr*10hr
    ReDim MV!(0 To MVchannels, 1 To TotalCycles), CV%(1 To Channels, 1 To 14, 1 To TotalCycles)
    ReDim EV%(1 To Channels, 1 To 14, 1 To TotalCycles)
    ReDim GV!(0 To GVchannels, 1 To Channels, 1 To TotalCycles)
    ReDim UV&(1 To 4, 1 To TotalCycles), NAVV!(1 To 10, 1 To TotalCycles), OATV!(1 To 10, 1 To TotalCycles)
    ReDim ALTV!(1 To 4, 1 To TotalCycles), MUXV%(1 To 24, 1 To TotalCycles)
    ReDim CQV(1 To TotalCycles) As Boolean, CQM%(1 To Channels, 1 To TotalCycles)
    ReDim NQV(1 To TotalCycles) As Boolean, CQVS(1 To TotalCycles) As Boolean
    ReDim Tmtp_Tnav(1 To TotalCycles), TTOV!(0 To 3, 1 To TotalCycles)
    FirstMouseX = 1
    RawCycle = 1
    UpdateForm
'    Call Winsock1_Connect
  End If
  
  For i = 1 To 3: Beep: Next i

Exit Sub
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
LoadComboBoxes:
  
  With cboRevComment
  .AddItem "Preliminary Data"
  .AddItem "Final Data"
  .ListIndex = 0
  End With

  With cboICTrev
  .AddItem "0"
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .ListIndex = 0
  End With
  
  With cboPTfile
  .AddItem "NO"
  .AddItem "IW"
  .AddItem "NG"
  .AddItem "MM"
  .AddItem "PT"
  .AddItem "UC"
  .AddItem "TD"
  .AddItem "NC"
  .AddItem "IC"
  .AddItem "IN"
  .AddItem "FT"
  .ListIndex = 0
  End With
  
  With cboAlgorithm
  .AddItem "User Selected"          '0
  .AddItem "MRI"                    '1
  .AddItem "Latitude Blend"         '2
  .AddItem "Longitude Blend"        '3
  .AddItem "UT Blend"               '4
  .AddItem "Retrievable Index"      '5
  .AddItem "Observable Index"       '6
  .AddItem "Separate Up Dn OBs"     '7
  .ListIndex = 6  'Algorithm            '
  End With

  With cboAverageNcycles
  .AddItem "1"
  .AddItem "3"
  .AddItem "5"
  .AddItem "7"
  .ListIndex = 0
  End With
  
  
  For i = 0 To 4
    With cboUTunit(i)
    .AddItem "Rec No"
    .AddItem "UTks"
    .AddItem "Hour"
    .AddItem "hh:mm:ss"
    .ListIndex = 0
    End With
  Next i

  With cboCTS1
  .AddItem "0000"
  .AddItem "1000"
  .AddItem "2000"
  .AddItem "3000"
  .AddItem "4000"
  .AddItem "5000"
  .AddItem "6000"
  .AddItem "7000"
  .AddItem "8000"
  .AddItem "9000"
  .AddItem "10000"
  .AddItem "11000"
  .AddItem "12000"
  .AddItem "13000"
  .AddItem "14000"
  .AddItem "15000"
  Select Case AC$
  Case "DC": .ListIndex = 13
  Case Else:  .ListIndex = 8
  End Select
  End With

  With cboCTS2
  .AddItem "20000"
  .AddItem "19000"
  .AddItem "18000"
  .AddItem "17000"
  .AddItem "16000"
  .AddItem "15000"
  .AddItem "14000"
  .AddItem "13000"
  .AddItem "12000"
  .AddItem "11000"
  .AddItem "10000"
  .AddItem "9000"
  .AddItem "8000"
  .AddItem "7000"
  .AddItem "6000"
  .AddItem "5000"
  .AddItem "4000"
  .AddItem "3000"
  .AddItem "2000"
  .AddItem "1000"
  Select Case AC$
  Case "DC": .ListIndex = 2
  Case Else: .ListIndex = 5
  End Select
  End With
  
  With cboCTS3
  .AddItem "100"
  .AddItem "200"
  .AddItem "500"
  .AddItem "1000"
  .AddItem "2000"
  .ListIndex = 2
  End With

  With cboMUX1
  .AddItem "-90"
  .AddItem "-80"
  .AddItem "-70"
  .AddItem "-60"
  .AddItem "-50"
  .AddItem "-40"
  .AddItem "-30"
  .AddItem "-20"
  .AddItem "-10"
  .AddItem "00"
  .AddItem "10"
  .AddItem "20"
  .AddItem "30"
  .AddItem "40"
  .AddItem "50"
  .AddItem "60"
  .ListIndex = 1
  End With

  With cboMUX2
  .AddItem "70"
  .AddItem "60"
  .AddItem "50"
  .AddItem "40"
  .AddItem "30"
  .AddItem "20"
  .AddItem "10"
  .AddItem "0"
  .AddItem "-10"
  .AddItem "-20"
  .AddItem "-30"
  .AddItem "-40"
  .ListIndex = 1
  End With
  
  With cboMUX3
  .AddItem "1"
  .AddItem "2"
  .AddItem "5"
  .AddItem "10"
  .AddItem "20"
  .ListIndex = 3
  End With

  With cboTB1
  .AddItem "180"
  .AddItem "190"
  .AddItem "200"
  .AddItem "205"
  .AddItem "210"
  .AddItem "220"
  .AddItem "230"
  .AddItem "240"
  .AddItem "250"
  .AddItem "260"
  .AddItem "270"
  .AddItem "280"
  .AddItem "290"
  .AddItem "300"
  .ListIndex = 0
  End With

  With cboTB2
  .AddItem "310"
  .AddItem "300"
  .AddItem "290"
  .AddItem "280"
  .AddItem "270"
  .AddItem "260"
  .AddItem "250"
  .AddItem "240"
  .AddItem "230"
  .AddItem "225"
  .AddItem "220"
  .AddItem "215"
  .AddItem "210"
  .ListIndex = 0
  End With

  With cboTB3
  .AddItem "1"
  .AddItem "2"
  .AddItem "5"
  .AddItem "10"
  .AddItem "20"
  .ListIndex = 2
  End With

  With cboGAIN1
  .AddItem "0"
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .AddItem "6"
  .AddItem "7"
  .AddItem "8"
  .AddItem "9"
  .AddItem "10"
  .AddItem "12"
  .AddItem "14"
  .AddItem "16"
  .ListIndex = 8
  End With

  With cboGAIN2
  .AddItem "10"
  .AddItem "11"
  .AddItem "12"
  .AddItem "13"
  .AddItem "14"
  .AddItem "15"
  .AddItem "16"
  .AddItem "17"
  .AddItem "18"
  .AddItem "19"
  .AddItem "20"
  .AddItem "21"
  .AddItem "22"
  .AddItem "24"
  .AddItem "26"
  .AddItem "28"
    .ListIndex = 5
  End With

  With cboGAIN3
  .AddItem "0.5"
  .AddItem "1"
  .AddItem "2"
  .AddItem "5"
  .ListIndex = 1
  End With

  With cboTPT1
    For i = 0 To 16
    .AddItem Format(i, "#0")
    Next i
  .ListIndex = 0
  End With

  With cboTPT2
  .AddItem "10"
  .AddItem "11"
  .AddItem "12"
  .AddItem "13"
  .AddItem "14"
  .AddItem "15"
  .AddItem "16"
  .AddItem "17"
  .AddItem "18"
  .AddItem "19"
  .AddItem "20"
  .AddItem "21"
  .AddItem "22"
  .AddItem "24"
  .AddItem "26"
  .AddItem "28"
  .ListIndex = 15
  End With

  With cboTPT3
  .AddItem "0.5"
  .AddItem "1"
  .AddItem "2"
  .AddItem "5"
  .ListIndex = 2
  End With
  
  With cboTPT4
  .AddItem "km"
  .AddItem "kft"
  .ListIndex = 0
  End With

'  With cboSource
'  .AddItem "Offline: Existing File"              'F
'  .AddItem "Offline: Test File"                  'T
'  .AddItem "Realtime: Data Unit"                 'D
'  .AddItem "Realtime: MTPcurrent.RAW"            'S
'  .AddItem "Realtime: REVEAL"                    'R
'  .ListIndex = 0
'  End With

  With cboUT
  .AddItem "hh:mm:ss"
  .AddItem "hh.hhhh"
  .AddItem "ks"
  .AddItem "s"
  .ListIndex = 2
  End With
  
  With cboTrack
  .AddItem "Rec No"
  .AddItem "ks"
  .AddItem "seconds"
  .AddItem "hhmmss"
  .AddItem "hh:mm:ss"
  .ListIndex = 2
  End With
  
  With cboGOTO
  .AddItem "Rec No"
  .AddItem "ks"
  .AddItem "seconds"
  .AddItem "hhmmss"
  .AddItem "hh:mm:ss"
  .ListIndex = 0
  End With
  
  With cboGoIWG
  .AddItem "Rec No"
  .AddItem "ks"
  .AddItem "seconds"
  .AddItem "hhmmss"
  .AddItem "hh:mm:ss"
  .ListIndex = 2
  End With

  With cboParms(0)
  .AddItem "300"
  .AddItem "310"
  .AddItem "320"
  .AddItem "330"
  .AddItem "340"
  .AddItem "350"
  .AddItem "360"
  .AddItem "370"
  .AddItem "380"
  .AddItem "390"
  .ListIndex = 9
  End With
  
  With cboParms(1)
  .AddItem "360"
  .AddItem "370"
  .AddItem "380"
  .AddItem "390"
  .AddItem "400"
  .AddItem "410"
  .AddItem "420"
  .AddItem "440"
  .AddItem "460"
  .AddItem "480"
  .ListIndex = 7
  End With
  
  With cboParms(2)
  .AddItem "5"
  .AddItem "10"
  .AddItem "20"
  .AddItem "40"
  .ListIndex = 2
  End With
  
  With cboParms(3)
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .ListIndex = 0
  End With
'
  With cboLoc5
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .AddItem "6"
  .AddItem "7"
  .AddItem "8"
  .AddItem "9"
  .AddItem "10"
  .AddItem "11"
  .AddItem "12"
  .ListIndex = 4
  End With
 
  With cboLoc6
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .AddItem "6"
  .AddItem "7"
  .AddItem "8"
  .AddItem "9"
  .AddItem "10"
  .AddItem "11"
  .AddItem "12"
  .ListIndex = 5
  End With
 
  With frmFormatATP
  .chkAircraft.Value = 1
  .chkTropopause.Value = 1
  .chkAircraft.Value = 1
  .cmbYunitl.AddItem "km"
  .cmbYunitl.AddItem "kft"
  .cmbYunitl.AddItem "mb"
  .cmbYunitl.AddItem "hPa"
  .cmbYunitl.Text = "km"

  .cmbYunitr.AddItem "km"
  .cmbYunitr.AddItem "kft"
  .cmbYunitr.AddItem "mb"
  .cmbYunitr.AddItem "hPa"
  .cmbYunitr.Text = "kft"
  .optYcoordl(0).Value = 1
  .optYcoordr(0).Value = 1
  .Hide
  End With

  With cboSpeed
  .AddItem "1X"
  .AddItem "5X"
  .AddItem "10X"
  .AddItem "50X"
  .AddItem "100X"
  .ListIndex = 0
  End With

  With cboAverageTBs
  .AddItem "1"
  .AddItem "2"
  .AddItem "4"
  .ListIndex = 1
  End With

  GVchannels = 6
  MVchannels = 72
  ReDim Tx$(0 To MVchannels)

Return

' This Initialize routine is only called for Realtime data processing
Initialize:
  ProgramConstants
  DoRetrieval = True
'  frmATP.WindowState = 2
  ShowZp = True
  TtgtPC = 0
  RawCycle = 0
  GoodCycle = 0
  BadCycle = 0
  Cycle = 0
  UTsec = 0
  UTsecLast = 0
  Navg = Val(txtTavg.Text)      'Number of temperatures to average
' Initialize constants used in retrieval code
  Ccycle = 0                           'Counts Quality Count
  MVindex = 0
  GVindex = 0
' Initialize flags that detect lines read (0=FALSE)
  Aline% = 0: Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0: Fline% = 0
' Bad Bit Counts
  NavQualPC = 0
  TAqualPC = 0
  CtsQualPC = 0
  CycleQualPC = 0
  TtgtPC = 0
  RAWbadPC = 0
' Set limits for acceptable gains and counters to track
  NAVgainCount = 0
  GEgainCount = 0
  NDgainCount = 0
'  LocHor = Val(txtLocHor.Text)  'Initialization routines have not been run yet!
' Handle aircraft dependent parameters
  RHS = True                'Assume SU on RHS after 960325
  Remote = True             'Allow remote control commands
  CAC = True                'Chassis Analysis Computer in use?
  CycleTime = 15            'Time for one MTP cycle (s)
  frmATP.FooterType = 0
  LocHor = 6
  Select Case AC$
  Case "ER", "WB", "GH"
    Ceiling = 24#             'ER2 service ceiling
  Case "DC"
    Ceiling = 13             'DC8 service ceiling
    Remote = True             'Allow remote control commands
    CAC = True                'Chassis Analysis Computer in use?
    If Val(YYYYMMDD$) < 19960325 Then RHS = False: LocHor% = 5
  Case "NG"
    Ceiling = 16
  Case "M5"
    LocHor = 5
  End Select
  DoEvents
  DirPath$ = "C:\MTP\RTA\REALTIME\"
  Set f = frmFLTINFO
  
  TotalCycles = 10 'fRAWscans(RAWfile$)
  ReDim MV!(0 To MVchannels, 1 To TotalCycles), CV%(1 To Channels, 1 To 14, 1 To TotalCycles)
  ReDim EV%(1 To Channels, 1 To 14, 1 To TotalCycles), TTOV!(0 To 3, 1 To TotalCycles)
  ReDim GV!(0 To GVchannels, 1 To Channels, 1 To TotalCycles), TTOV!(0 To 3, 1 To TotalCycles)
  ReDim UV&(1 To 4, 1 To TotalCycles), NAVV!(1 To 10, 1 To TotalCycles), OATV!(1 To 10, 1 To TotalCycles)
  ReDim ALTV!(1 To 4, 1 To TotalCycles), MUXV%(1 To 24, 1 To TotalCycles)
  ReDim CQV(1 To TotalCycles) As Boolean, CQM%(1 To Channels, 1 To TotalCycles)
  ReDim NQV(1 To TotalCycles) As Boolean, CQVS(1 To TotalCycles) As Boolean
  
  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
  CheckLimitsInit

  Record = 0
  FirstAline = True
  PText$ = "." + AC$ + "8"
  
' Deal with Calfile
  CALfile$ = DirPath$ + "REALTIME.CAL"
  
  Call CheckLimitsInit   'Reset Limit Information
  InitializeGainLimits
  If Not RealTime Then
'  Select Case Source$
'  Case "F", "S"
    'FileLength = FileLen(REFfile$)
    'If FileLength > 0 Then Kill REFfile$
  
      If RealTime Then
        RTyyyymmdd$ = AC$ + Right$(Date, 4) + Format(Month(Date), "00") + Format(Day(Date), "00")
        x$ = "C:\MTP\RTA\REF\"
        If Len(Dir(x$, vbDirectory)) = 0 Then MkDir x$  'Check if Directory exists, if not create
        REFfile$ = x$ + RTyyyymmdd$ + ".REF"
      End If
      REFlu% = FreeFile
      Open REFfile$ For Random Access Read Write As REFlu% Len = Len(REF2)

'  Case "D"
'    On Error GoTo Error_Handler  'Set up to handle Run Time errors (in D-mode only)
'  Case "T"
'  End Select
  End If
  ProgressBar1.Min = 0
  ProgressBar1.Max = TotalCycles
  DoEvents
  
  chkEnableFit.Value = 1
  mnuViewATP.Checked = True
  mnuViewATP_Click
  Return

Error_Handler:
  eFormat$ = "Error ### occurred on line ####. "

  Select Case Err
  Case 53                              'File not Found
    E$ = eFormat$ + "File Not Found" + Space$(32)
'    Print Using; e$; Err; Erl
    INC Err53count%
  Case 57                              'Device I/O Error
    E$ = eFormat$ + "Device I/O Error" + Space$(31)
'    Print Using; e$; Err; Erl
    INC Err57count%
    Resume Next
  Case Else
    INC Errxxcount%
    Resume Next
  End Select

End Sub

Sub Initialize_RCs_Calfile()
Dim DefCalfile$, yy$, mm$, DD$, RAOBdate As Date, RAOB$, Cycle&
Dim Status As Boolean, lu%, DTevent As Date, Template As Boolean
Dim ATP!(1 To 33), RMSmin!, iRMS%, RAOBlu%, jmax%, UArecord&, iRAOB%

Static GA!(1 To 300), PA!(1 To 300), RH!(1 To 300), Tt!(1 To 300), ZA!(1 To 300)
Static GAa!(1 To 300), PAa!(1 To 300), RHa!(1 To 300), TTa!(1 To 300), ZAa!(1 To 300)
Static GAb!(1 To 300), PAb!(1 To 300), RHb!(1 To 300), TTb!(1 To 300), ZAb!(1 To 300)

Dim File$, Nevents%, Rec1&, Rec2&, T1!, T2!, Te!, Prefix$, Wt1!, Wt2!, Navg%
Dim i%, j%, ii%, jj%, M%, Tmin!, Tmax!, Traob!, Tmtp!, TZ1!(1 To 300), TZ2!(1 To 300), Zp1!(1 To 300), Zp2!(1 To 300), nTZ1%, nTZ2%
Static Nrla%, Nrlb%, Nrl%, Isame%, OUTlu%, RAOB1$, RAOB2x$, AvgTdiff!, RmsTdiff!
Dim Ln1!, Ln2!, Ln3!, LT1!, Lt2!, Lt3!, D1!, D2!, Distance!, Line$
Dim A$, b$, C$, iD%, iM%, iY%, UT0!, UT1!, UT2!, DT1 As Date, DT2 As Date, Filename$, T As Date
Dim LR11!, LR12!, Zb1!, LR21!, LR22!, Zb2!, z!, iNum%, dT As Date
Dim yyyymmdd1$, yyyymmdd2$

Static aZp!, aTnav!, aLat!, aLon!, aRoll!, aPitch!, aZt!, aTt!
Static Site1Lat!, Site1Lon!, Site1Distance!, Site2Lat!, Site2Lon!, Site2Distance!
Static Found1 As Boolean, Found2 As Boolean


  If Not DoRetrieval Then Exit Sub
  
' Initialize constants used in retrieval code
  Ccycle = 0                           'Counts Quality Count
  MVindex = 0
  GVindex = 0

' Initialize flags that detect lines read (0=FALSE)
  Aline% = 0: Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0: Fline% = 0

' Bad Bit Counts
  NavQualPC = 0
  TAqualPC = 0
  CtsQualPC = 0
  CycleQualPC = 0
  TtgtPC = 0
  RAWbadPC = 0

' Set limits for acceptable gains and counters to track
  NAVgainCount = 0
  GEgainCount = 0
  NDgainCount = 0

' Initialize limit summary parameters
'  CheckLimitsInit  'do in raw file

' Establish CALFILE name
  DefCalfile$ = Drive$ + Rdir2$ + Mission$ + "\Setup\" + Mission$ + "_CAL.txt"
  If Not RealTime Then    'If Source$ = "F" Then
    If RealTime Then CALfile$ = DefCalfile$ Else CALfile$ = Path$ + AC$ + YYYYMMDD$ + ".CAL"
  Else
    CALfile$ = DefCalfile$
  End If

' Read Retrieval Coefficients
  For i = 0 To NRCbin - 1
    RCuse(i) = 0: txtRCuse(i).Text = ""
    txtWavg(i).Text = ""
    txtWrms(i).Text = ""
    txtTBavg(i).Text = ""
    txtTBrms(i).Text = ""
    txtlnP(i).Text = ""
  Next i
  For i = 0 To NRCmax - 1: RCuse(i) = 0: frmFLTINFO.txtRCuse(i).Text = "": Next i

    
' Read in RCs
  If Not RealTime Then   'If Source$ = "F" Then
    For i = 0 To NRC - 1
      Call ReadInRC(RCpath$ + RCs$ + Reg$(i), i)
    Next i
  Else     ' Set defaults for D-mode
    LAT1 = defLAT1: LAT2 = defLAT2: LAT3 = defLAT3: LAT4 = defLAT4
    For i = 0 To NRC - 1
      Call ReadInRC(RCpath$ + RCs$ + Reg$(i), i)
    Next i
  End If
' Now check to see if RAOB Reanalysis is needed
' ENAS__2003012612.RAOB2
' 1234567890123456789012
  If RAOBreanalysis Then   'Set when RCconfig file is read. Make sure new format supports
  
  If chkSkipTemplateRAOBs Then Exit Sub
'   Check for flights which have RAOBs that look like RAOB templates
    RAOBlu = FreeFile
'    Open RSpath$ + Mission$ + "_Template_RAOBs.RAOB2" For Random Access Read Write As RAOBlu Len = Len(RAOB2)
    Open RSpath$ + Mission$ + "_RAOBused.RAOB2" For Random Access Read Write As RAOBlu Len = Len(RAOB2)
    iRAOB = LOF(RAOBlu) / Len(RAOB2)
    dT = fDate(YYYYMMDD$)
    DT1 = DateSerial(Year(dT), Month(dT), Day(dT) - 1) 'Convert string to Date variable
    DT2 = DateSerial(Year(dT), Month(dT), Day(dT) + 1)  'Convert string to Date variable
    nRAOBtemplates = 0
    For j = 0 To NRC - 1   'For each NRC look for template RAOB (Note: may not be in order)
      Cycle = 0
      For i = 1 To iRAOB
        Cycle = Cycle + 1
        Call RAOBread(RAOBlu, Cycle)
        'Debug.Print Cycle; fRMSfilename$(WMO4l, WMOnumber, Iyear, Imonth, Iday, Ihour)
        dT = DateSerial(Iyear, Imonth, Iday)  'Date variables for year, month, and day
        If dT >= DT1 And dT <= DT2 Then           'Templates within 1 day of yyyymmdd$
          RMSfilename$ = fRMSfilename$(WMO4l, WMOnumber, Iyear, Imonth, Iday, Ihour)
          ' Make sure RCF file names match before adding to RAOB template list
'          If frmFLTINFO.cboReg(j).Text = frmFLTINFO.cboReg(0).List(Cycle - 1) Then
          If RMSfilename$ = fGetFilename(frmFLTINFO.cboReg(j).ToolTipText) Then
            Debug.Print Str(j) + "  " + RMSfilename$ + "  " + fGetFilename(frmFLTINFO.cboReg(j).ToolTipText)
            nRAOBtemplates = nRAOBtemplates + 1
            RCset(nRAOBtemplates) = j
            rLatitude(nRAOBtemplates) = WMOlatitude
            rLongitude(nRAOBtemplates) = WMOlongitude
            Exit For  'Found template RAOB for this NRC, move on to next one
          End If
        End If
      Next i
    Next j
    Close RAOBlu
  
Exit Sub
' The code below is how RAOB templates were handled before 20080131 when the
' Mission_Template_RAOBs.RAOB2 was created. There is a button named Template_RAOBs
' on the RAOBman Export tab which reads all the RCF files in the Mission/RC/ folder
' and extracts them into this RAOB2 file.
'   Check for flights which have RAOBs that look like RAOB templates
    RAOBlu = FreeFile
    Open MNpath$ + Mission$ + "_RAOBs.RAOB2" For Random Access Read Write As RAOBlu Len = Len(RAOB2)
   
    lu = FreeFile
    Open MNpath$ + Mission$ + "_RAOBrange.txt" For Input As lu    'eg SOLVE2_RAOBrange.txt
    Input #lu, A$
        
    nRAOBtemplates = 0
    Do      'Read list of RAOBs within 160 km of Flight Track
      Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
      i = InStr(1, A$, vbTab)
      RAOB1$ = Left$(A$, i - 1)
      ii = InStr(i + 1, A$, vbTab)
      RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
      DTevent = DateSerial(iY, iM, iD) '+ TimeValue(fSecToTstring(UT0 * 1000, True))
      ' Check
      If DTevent = FlightDate Then  ' Need only RAOBs on flight date
        ' Get RAOBs launch before and after closest approach
'        If UT0 <= 43.2 Then UT1 = 0: UT2 = 12 Else UT1 = 12: UT2 = 24
'        yyyymmdd$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
        UT1 = Int(UT0 / 3.6)
        Do
          UT1 = UT1 + 1
        Loop Until UT1 Mod 3 = 0  ' Find first sounding after UT0 at 3,6,9,12,15,18,21,24 hours
        
        ' Before in 3 hour steps
        DT1 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0)
        Do
          DT1 = DT1 - TimeValue("03:00:00")
          'Debug.Print "1 "; RAOB1$; " "; DT1
          Call GetRAOB(RAOBlu, DT1, RAOB1$, Found1, Rec1)
          DoEvents
        Loop Until Found1
        Site1Lat = WMOlatitude
        Site1Lon = WMOlongitude
        Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)
        Call CopyArray(ZPraob(), Zp1(), Nlevr)
        Call CopyArray(TZraob(), TZ1(), Nlevr)
        nTZ1 = Nlevr
        ' After in 3 hour steps
        DT2 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0) - TimeValue("3:00:00")
        Do
          DT2 = DT2 + TimeValue("3:00:00")
          'Debug.Print "2 "; RAOB1$; " "; DT2
          Call GetRAOB(RAOBlu, DT2, RAOB1$, Found2, Rec2)
          DoEvents
        Loop Until Found2
        'Debug.Print DT1; DT2
        ' Calculate weighted average of before and after soundings
        Wt1 = (UT2 - UT0 / 3.6) / (UT2 - UT1)
        Call BlendROABtime(RAOBlu, Rec1, Rec2, Wt1, GAa(), PAa(), TTa(), RHa(), Nrla, AvgTdiff!, RmsTdiff!)
        For i = 1 To Nrla: ZAa(i) = fPtoZ(PAa(i)): Next i
        ' Interpolate RAOB to retrieval levels
        For j = 1 To Nret
          z = RAav(0, 1, j)
          If z > ZAa(Nrla) Then Exit For
          ATP(j) = fLinterp(ZAa(), TTa(), Nrla, z)
        Next j
        jmax = j - 1
        ' Compare interpolated sounding to AA T profile
        iRMS = fATPcompare(ATP!(), jmax%)
        If iRMS >= 0 Then  'TRUE if AA T-profile like RAOB
          'Debug.Print iRMS; fGetFilename(RCtemplate$(iRMS))
        ' Get Site Latitudes and Longitudes for RAOB near flight track
          Status = fGetUAsite(RAOB1$, UArecord&)
          If Status Then
            nRAOBtemplates = nRAOBtemplates + 1
            RCset(nRAOBtemplates) = iRMS
            rLatitude(nRAOBtemplates) = GPlatitude
            rLongitude(nRAOBtemplates) = GPlongitude
          Else
            MsgBox "RAOB: " + RAOB1$ + " was not found!", vbOKOnly
          End If
        End If
      End If
    Loop Until EOF(lu)
    Close lu, RAOBlu
  End If
NewStuff:

End Sub
Sub CopyArray(A!(), b!(), n%)
' Copy array A() to array B()
Dim i%

  For i = 1 To n
    b(i) = A(i)
  Next i

End Sub


Sub BlendROABtime(RAOBlu%, Rec1&, Rec2&, Wt1!, GA!(), PA!(), Tt!(), RH!(), Nrl%, AvgTdiff!, RmsTdiff!)
Dim i%, j%, k%, M%, n%, jj%, ii%, Nrl1%, Nrl2%, Test As Boolean, Sum1!, Sum2!, Zp!

Dim GA1!(1 To 300), PA1!(1 To 300), RH1!(1 To 300), TT1!(1 To 300)
Dim GA2!(1 To 300), PA2!(1 To 300), RH2!(1 To 300), TT2!(1 To 300)
Dim Isame%, Tdiff!(1 To 300)
Dim Pint!, Gint!, Tint!, Rint!

RAOBread RAOBlu, Rec1

If UseEditLevel And EditLevel Then   'Edit levels
  For i = 1 To Nedit
    TZraob(LZraob(i%)) = TZraob(Nlevr + i)
  Next i%
End If

For i = 1 To Nlevr
  GA1(i) = ZGraob(i)
  PA1(i) = PZraob(i)
  TT1(i) = TZraob(i)
  RH1(i) = RZraob(i)
Next i
Nrl1 = Nlevr
'If optSite(0).value Then
'  OAT11 = fTinterp(pALT, ZPraob(), TZraob(), Nlevr) 'xxx mjm
'Else
'  OAT21 = fTinterp(pALT, ZPraob(), TZraob(), Nlevr)
'End If

RAOBread RAOBlu, Rec2
'First fix bad levels
If UseEditLevel And EditLevel Then   'Edit levels
  For i = 1 To Nedit
    TZraob(LZraob(i%)) = TZraob(Nlevr + i)
  Next i%
End If

For i = 1 To Nlevr
  GA2(i) = ZGraob(i)
  PA2(i) = PZraob(i)
  TT2(i) = TZraob(i)
  RH2(i) = RZraob(i)
Next i
Nrl2 = Nlevr
'If optSite(0).value Then
'  OAT12 = fTinterp(pALT, ZPraob(), TZraob(), Nlevr)   'xxx mjm
'Else
'  OAT22 = fTinterp(pALT, ZPraob(), TZraob(), Nlevr)
'End If

'Interpolate so both RAOBs have same levels
Nrl = 1: i = 1: j = 1: k = 1: ii = Nrl1: jj = Nrl2: Isame = 0
Do

'  For iii = 1 To 20: Debug.Print iii; GA1(iii); GA2(iii): Next iii


  Select Case GA1(i)
  Case Is < GA2(j)        'Add a level to GA2
    Pint = fPAinterp(GA2(), PA2(), j, GA1(i))
    Gint = GA1(i)      'Set geometric altitudes equal
    Tint = fXAinterp(GA2(), TT2(), j, GA1(i))
    Rint = fXAinterp(GA2(), RH2(), j, GA1(i))
    For M = jj To j Step -1
      PA2(M + 1) = PA2(M)
      GA2(M + 1) = GA2(M)
      TT2(M + 1) = TT2(M)
      RH2(M + 1) = RH2(M)
    Next M
    PA2(j) = Pint
    GA2(j) = Gint
    TT2(j) = Tint
    RH2(j) = Rint
    jj = jj + 1
    
  Case Is = GA2(j) 'Equal
    Isame = Isame + 1
    
  Case Is > GA2(j) 'Interpolate PA1
    Pint = fPAinterp(GA1(), PA1(), i, GA2(j))
    Gint = GA2(i)      'Set geometric altitudes equal
    Tint = fXAinterp(GA1(), TT1(), i, GA2(j))
    Rint = fXAinterp(GA1(), RH1(), i, GA2(j))
    For M = ii To i Step -1  'Make room for a new level
      PA1(M + 1) = PA1(M)
      GA1(M + 1) = GA1(M)
      TT1(M + 1) = TT1(M)
      RH1(M + 1) = RH1(M)
    Next M
    PA1(i) = Pint
    GA1(i) = Gint
    TT1(i) = Tint
    RH1(i) = Rint
    ii = ii + 1       'Add a GA1 level
    
  End Select
  j = j + 1
  i = i + 1
  Nrl = Nrl + 1
  'Debug.Print i; j; ii; jj; GA1(i); GA2(j)
Loop Until i > ii Or j > jj
' Add code to deal with extending RAOB
If j > jj Then  'extend series 2
  For M = j To ii
    PA2(M) = fPAinterp(GA2(), PA2(), jj, GA1(M))
    GA2(M) = GA1(M)      'Set geometric altitudes equal
    TT2(M) = fXAinterp(GA2(), TT2(), jj, GA1(M))
    RH2(M) = fXAinterp(GA2(), RH2(), jj, GA1(M))
  Next M
Else
  For M = i To jj
    PA1(M) = fPAinterp(GA1(), PA1(), ii, GA2(M))
    GA1(M) = GA2(j)   'Set geometric altitudes equal
    TT1(M) = fXAinterp(GA1(), TT1(), ii, GA2(M))
    RH1(M) = fXAinterp(GA1(), RH1(), ii, GA2(M))
  Next M
End If

Nrl = Nrl1 + Nrl2 - Isame
  
' Test plot of interpolated profiles
Test = False
If Test Then
  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA1(i%))
    Tplot(i%) = TT1(i%)
  Next i
  'UpdateDisplay

  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA2(i%))
    Tplot(i%) = TT2(i%)
  Next i
  'UpdateDisplay
End If

' Now interpolate levels in time
  j = 0
  For i = 1 To Nrl    'Write out as increasing pressure altitude [m]
    GA(i) = GA1(i) * Wt1 + GA2(i) * (1 - Wt1)
    PA(i) = PA1(i) * Wt1 + PA2(i) * (1 - Wt1)
    Tt(i) = TT1(i) * Wt1 + TT2(i) * (1 - Wt1)
    RH(i) = RH1(i) * Wt1 + RH2(i) * (1 - Wt1)
    Zp = fPtoZ(PA(i))
    If Zp < 4 Or Zp > 20 Then 'Use 12 km +/- 8 km to focus on MTP accessability
    ' Ignore this data
    Else
      j = j + 1
      Tdiff(j) = TT1(i) - TT2(i)
    End If
  Next i
' Calculate Avg and RMS difference
  Call AvgRMS(Tdiff(), j, AvgTdiff, RmsTdiff)
End Sub

Sub GetRAOB(RAOBlu%, dT As Date, RAOB$, Found As Boolean, Cycle&)
Dim i%, x$
  If EOF(RAOBlu) Then Call RAOBreadSL(RAOBlu, 1)
  Found = False
  Cycle = 0
  Do
    Cycle = Cycle + 1
    Call RAOBread(RAOBlu, Cycle)
    If Not fNumber(Left$(RAOB$, 1)) Then
      If DandT = dT And Trim(WMO4l) = RAOB$ Then
        Found = True
        Exit Do
      End If
    Else
      If DandT = dT And WMOnumber = Val(RAOB$) Then
        Found = True
        Exit Do
      End If
    End If
  Loop Until EOF(RAOBlu)

End Sub

Function fNumber(L$) As Boolean

Select Case Asc(L$)
Case Is < 58: fNumber = True
Case Else: fNumber = False
End Select
End Function



Private Sub Inet1_StateChanged(ByVal State As Integer)
    On Error Resume Next
    
    Select Case State
           Case icNone
           Case icResolvingHost:
               Me.lblResponse.Caption = "Resolving Host"
           Case icHostResolved:
               Me.lblResponse.Caption = "Host Resolved"
           Case icConnecting:
                Me.lblResponse.Caption = "Connecting..."
           Case icConnected:
               Me.lblResponse.Caption = "Connected"
           Case icResponseReceived:
                Me.lblResponse.Caption = "Transferring File..."
           Case icDisconnecting:
               Me.lblResponse.Caption = "Disconnecting..."
           Case icDisconnected:
                  Me.lblResponse.Caption = "Disconnected"
           Case icError:
               MsgBox "Error:" & Inet1.ResponseCode & " " & Inet1.ResponseInfo
           Case icResponseCompleted:
              Me.lblResponse.Caption = "Process Complete."
     End Select
      
         Me.lblResponse.Refresh
       
      Err.Clear
End Sub


Private Sub mnuFileExit_Click()
  Close
  Quit = True
  Unload Me
  End
End Sub


Private Sub mnuFormatATP_Click()
  frmFormatATP.Show
End Sub

Private Sub mnuFormatCTC_Click()
frmCurtain.Show
End Sub

Private Sub mnuHelpAbout_Click()
Dim DandT$

  DandT$ = "yyyy mm dd hh:mm:ss"
' Set properties
  frmAbout.Application = "MTP Data Analysis Program"
  frmAbout.Heading = "Release " + Format$(MAKEversion, DandT$)
  frmAbout.Copyright = "1998-2011 MJ Mahoney"
' Call a method
  frmAbout.Display

End Sub

Private Sub mnuHelpUsersGuide_Click()

  Navigate "C:\www\notes\software\MTP Software Notes.html"

End Sub

Private Sub mnuViewATP_Click()
  
  If mnuViewATP.Checked Then
    mnuViewATP.Checked = False
    ShowATP = False
    frmATP.Hide
  Else
    mnuViewATP.Checked = True
    ShowATP = True
    frmATP.Show
  End If
  
End Sub

Private Sub OBatch_Click()
'  If mnuViewBatch.Checked Then
'    mnuViewBatch.Checked = False
'    frmFLTINFO.Hide
'  Else
'    mnuViewFltInfo.Checked = True
'    frmFLTINFO.Show
'    frmFLTINFO.Refresh
'  End If

End Sub


Private Sub mnuViewData_Click()
  
  ShowDiagnostics = Not ShowDiagnostics
  If ShowDiagnostics Then
    mnuViewData.Checked = True
  Else
    mnuViewData.Checked = False
  End If

End Sub

Private Sub mnuViewEditBits_Click()
  
  If mnuViewEditBits.Checked Then
    mnuViewEditBits.Checked = False
    frmEditBits.Hide
  Else
    mnuViewEditBits.Checked = True
    frmEditBits.Show
  End If

End Sub

Private Sub mnuViewFltInfo_Click()
  
  If mnuViewFltInfo.Checked Then
    mnuViewFltInfo.Checked = False
    frmFLTINFO.Hide
  Else
    mnuViewFltInfo.Checked = True
    frmFLTINFO.Show
    frmFLTINFO.Refresh
  End If

End Sub

Private Sub mnuViewLimits_Click()
  
  If Not mnuViewLimits.Checked Then
    mnuViewLimits.Checked = True
    REFreadLIMITS (txtREFfile.Text)
    UpdateLimitSummary
    With frmSummary
      .Show
      .txtREF.Text = txtREFfile.Text
    End With
    frmSummary.Refresh
  Else
    mnuViewLimits.Checked = False
    frmSummary.Hide
  End If

End Sub





Private Sub mnuViewNDP_Click()
  If mnuViewNDP.Checked Then
    mnuViewNDP.Checked = False
    ShowNDP = False
    frmNDP.Hide
  Else
    mnuViewNDP.Checked = True
    ShowNDP = True
    frmNDP.Show
  End If

End Sub

Private Sub mnuViewTBs_Click()
  
  If mnuViewTBs.Checked Then
    mnuViewTBs.Checked = False
    ShowTBs = False
    frmTB.Hide
  Else
    mnuViewTBs.Checked = True
    ShowTBs = True
    frmTB.Show
    frmTB.AutoScaleTA = True
  End If

End Sub

Private Sub picCTS_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim BoxWidth%, BoxHeight%

  If Button = 1 Then
    DrawingBox = True
    FirstMouseX = x: FirstMouseY = y
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpCTS
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
      .BorderStyle = 3
      .Visible = True
    End With
    DoEvents
  End If

End Sub


Private Sub picCTS_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, k1%, k2%, iCh%, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!
Dim BoxWidth%, BoxHeight%, b$
Static x1!, x2!, y1!, y2!

  If Not ArrayRead Then Exit Sub
  If Button = 1 And DrawingBox = True Then
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpCTS
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
    End With
  End If

iCh = 0: jLoc = 0
iX = Int(x): iY = Int(y)
Xthreshold = 20 * (Val(txtX2(0).Text) - Val(txtX1(0).Text)) / picCTS.Width
Ythreshold = 20 * (Val(cboCTS2.Text) - Val(cboCTS1.Text)) / picCTS.Height
If chkPlotBadEls.Value = 0 Then
For i = 1 To Channels
  For j = 1 To 26
    k1 = iX - Xthreshold
    k2 = iX + Xthreshold
    If k1 < 1 Then k1 = 1
    If k2 > TotalCycles Then k2 = TotalCycles
    For k = k1 To k2
      If chkCTS(j - 1).Value = 1 Then
        If j < 14 Then
          If Abs(CV(i, j, k) - y) < Ythreshold Then iCh = i: jLoc = j: Exit For
        Else
          If Abs(EV(i, j - 13, k) - y) < Ythreshold Then iCh = i: jLoc = j: Exit For
        End If
      End If
    Next k
  Next j
  If iCh <> 0 Then Exit For
Next i
b$ = ""
If chkFlagBad.Value = 1 And Not CQV(x) Then b$ = "BAD: " Else b$ = "GOOD:  "

If iCh <> 0 Then
  picCTS.ToolTipText = b$ + "CH" + Str(iCh) + " Loc" + Str(jLoc) + " X=" + Format(x, "###0") + "," + "Y=" + Format(y, "####0")
Else
  picCTS.ToolTipText = "X=" + Format(x, "###0") + "," + "Y=" + Format(y, "####0")
End If

Else
For i = 1 To Channels
  For j = 1 To 12
    k1 = iX - Xthreshold
    k2 = iX + Xthreshold
    If k1 < 1 Then k1 = 1
    If k2 > TotalCycles Then k2 = TotalCycles
    For k = k1 To k2
      If CQM(i, k) And 2 ^ (j - 1) Then
          If Abs((i + 5 * (j - 1)) - y) < Ythreshold Then iCh = i: jLoc = j: Exit For
        End If
      
    Next k
  Next j
  If iCh <> 0 Then Exit For
Next i
b$ = ""
If iCh <> 0 Then b$ = "BAD: " Else b$ = "GOOD:  "

If iCh <> 0 Then
  picCTS.ToolTipText = b$ + "CH" + Str(iCh) + " Loc" + Str(jLoc) + " X=" + Format(x, "###0") + "," + "Y=" + Format(y, "####0")
Else
  picCTS.ToolTipText = "X=" + Format(x, "###0") + "," + "Y=" + Format(y, "####0")
End If


End If

  Select Case frmMTPbin.cboUT.Text
  Case "s": frmMTPbin.txtUT(2).Text = Format(UV(1, x), "#####0")
  Case "ks": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 1000, "##0.000")
  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UV(1, x), True)
  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 3600, "#0.0000")
  End Select

End Sub


Private Sub picCTS_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%

LastMouseX = Int(x)
LastMouseY = Int(y)
DrawingBox = False
shpCTS.Visible = False

If FirstMouseX = LastMouseX Then 'GOTO this scan on cursor
  FirstMouseX = FirstMouseXSave
  LastMouseX = LastMouseXSave
  txtGOTO.Text = Str(Int(x))
  cmdGO_Click
  
Else
  FirstMouseXSave = FirstMouseX
  LastMouseXSave = LastMouseX
  
  If YC0save = 0 Then
    YC0save = 1
    YC1save = cboCTS1.Text
    YC2save = cboCTS2.Text
    YC3save = cboCTS3.Text
    YM0save = 1
    YM1save = cboMUX1.Text
    YM2save = cboMUX2.Text
    YM3save = cboMUX3.Text
    YG0save = 1
    YG1save = cboGAIN1.Text
    YG2save = cboGAIN2.Text
    YG3save = cboGAIN3.Text
    YT0save = 1
    YT1save = cboTB1.Text
    YT2save = cboTB2.Text
    YT3save = cboTB3.Text
    YP0save = 1
    YP1save = cboTPT1.Text
    YP2save = cboTPT2.Text
    YP3save = cboTPT3.Text
  End If

  UpdateX1X2
  
  If FirstMouseY > LastMouseY Then
    cboCTS2.Text = Int(FirstMouseY)
    cboCTS1.Text = Int(LastMouseY)
  Else
    cboCTS1.Text = Int(FirstMouseY)
    cboCTS2.Text = Int(LastMouseY)
  End If
  cboCTS3.Text = Int((Val(cboCTS2.Text) - Val(cboCTS1.Text)) / 10)
  If cboCTS3.Text = "0" Then cboCTS3.Text = "1"
End If

Call UpdateCTSrange

End Sub


Private Sub picGain_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim BoxWidth%, BoxHeight%

  If Button = 1 Then
    DrawingBox = True
    FirstMouseX = x: FirstMouseY = y
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpGAIN
      .Left = FirstMouseX
      .Width = BoxWidth
      .Height = BoxHeight
      .Top = FirstMouseY
      .BorderStyle = 3
      .Visible = True
    End With
    DoEvents
  End If

End Sub

Private Sub picGain_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, iCh%, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!
Dim BoxWidth%, BoxHeight%, k1%, k2%
Static x1!, x2!, y1!, y2!

  If Not ArrayRead Then Exit Sub
  
  If Button = 1 And DrawingBox = True Then
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpGAIN
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
    End With
  End If
  
iCh = 0: jLoc = 0
iX = Int(x): iY = Int(y)
Xthreshold = 20 * (Val(txtX2(3).Text) - Val(txtX1(3).Text)) / picGain.Width
Ythreshold = 20 * (Val(cboTB2.Text) - Val(cboTB1.Text)) / picGain.Height

k1 = Int(iX - Xthreshold)
k2 = Int(iX + Xthreshold)
If k1 < 1 Then k1 = 1
If k2 > TotalCycles Then k2 = TotalCycles
For k = k1 To k2
  If Abs(OATV(1, k) + OATnavCOR - y) < Ythreshold Then jLoc = k: Exit For
Next k

If jLoc <> 0 Then
  picGain.ToolTipText = "Tnav:" + "  X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
Else
  picGain.ToolTipText = "X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
End If
On Error GoTo ExitSub
  Select Case frmMTPbin.cboUT.Text
  Case "s": frmMTPbin.txtUT(2).Text = Format(UV(1, x), "#####0")
  Case "ks": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 1000, "##0.000")
  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UV(1, x), True)
  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 3600, "#0.0000")
  End Select
ExitSub:
End Sub


Private Sub picGain_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%

LastMouseX = Int(x)
LastMouseY = Int(y)
DrawingBox = False
shpGAIN.Visible = False

If Abs(FirstMouseX - LastMouseX) < 2 Then 'GOTO this scan on cursor
  FirstMouseX = FirstMouseXSave
  LastMouseX = LastMouseXSave
  txtGOTO.Text = Str(Int(x))
  cmdGO_Click
  
Else
  FirstMouseXSave = FirstMouseX
  LastMouseXSave = LastMouseX
  If YM0save = 0 Then
    YC0save = 1
    YC1save = cboCTS1.Text
    YC2save = cboCTS2.Text
    YC3save = cboCTS3.Text
    YM0save = 1
    YM1save = cboMUX1.Text
    YM2save = cboMUX2.Text
    YM3save = cboMUX3.Text
    YG0save = 1
    YG1save = cboGAIN1.Text
    YG2save = cboGAIN2.Text
    YG3save = cboGAIN3.Text
    YT0save = 1
    YT1save = cboTB1.Text
    YT2save = cboTB2.Text
    YT3save = cboTB3.Text
    YP0save = 1
    YP1save = cboTPT1.Text
    YP2save = cboTPT2.Text
    YP3save = cboTPT3.Text
  End If
    
  UpdateX1X2
    
    If FirstMouseY > LastMouseY Then
      cboGAIN2.Text = Int(FirstMouseY)
      cboGAIN1.Text = Int(LastMouseY)
    Else
      cboGAIN1.Text = Int(FirstMouseY)
      cboGAIN2.Text = Int(LastMouseY)
    End If
    cboGAIN3.Text = Int((Val(cboGAIN2.Text) - Val(cboGAIN1.Text)) / 10)
    If cboGAIN3.Text = 0 Then cboGAIN3.Text = 1
    
End If
Call UpdateRT
Call UpdateGAINrange
End Sub


Private Sub picMUX_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim BoxWidth%, BoxHeight%
  
  If Button = 1 Then
    DrawingBox = True
    FirstMouseX = x: FirstMouseY = y
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpMUX
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
      .BorderStyle = 3
      .Visible = True
    End With
    DoEvents
  End If
  
End Sub

Private Sub picMUX_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, iCh%, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, BoxWidth%, BoxHeight%
Static x1!, x2!, y1!, y2!, s!, k1%, k2%

  If Not ArrayRead Then Exit Sub
  
  If Button = 1 And DrawingBox = True Then
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpMUX
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
    End With
  End If

iCh = 0: jLoc = 0
iX = Int(x): iY = Int(y)
Xthreshold = 10 * (Val(txtX2(1).Text) - Val(txtX1(1).Text)) / picMUX.Width
Ythreshold = 20 * (Val(cboMUX2.Text) - Val(cboMUX1.Text)) / picMUX.Height

  For j = 0 To MVchannels
    k1 = iX - Xthreshold
    k2 = iX + Xthreshold
    If k1 < 1 Then k1 = 1
    If k2 > TotalCycles Then k2 = TotalCycles
    For k = k1 To k2
      If chkMux(j).Value = 1 Then
        If j > 38 And j < 47 Then s = 5 * (47 - j) * MV(j, k) / 2 ^ (46 - j) Else s = MV(j, k)
        If Abs(s - y) < Ythreshold Then jLoc = j: Exit For
      End If
    Next k
    If jLoc <> 0 Then Exit For
  Next j

If jLoc <> 0 Then
  picMUX.ToolTipText = Tx(jLoc) + "  X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
Else
  picMUX.ToolTipText = "X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
End If
  Select Case frmMTPbin.cboUT.Text
  Case "s": frmMTPbin.txtUT(2).Text = Format(UV(1, x), "#####0")
  Case "ks": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 1000, "##0.000")
  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UV(1, x), True)
  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 3600, "#0.0000")
  End Select
'Debug.Print iX, iY, MV(jLoc, iX), Xthreshold, Ythreshold
End Sub


Private Sub picMUX_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%

LastMouseX = Int(x)
LastMouseY = Int(y)
DrawingBox = False
shpMUX.Visible = False

If FirstMouseX = LastMouseX Then 'GOTO this scan on cursor
  FirstMouseX = FirstMouseXSave
  LastMouseX = LastMouseXSave
  txtGOTO.Text = Str(Int(x))
  cmdGO_Click
  
Else
  FirstMouseXSave = FirstMouseX
  LastMouseXSave = LastMouseX
  If YM0save = 0 Then
    YC0save = 1
    YC1save = cboCTS1.Text
    YC2save = cboCTS2.Text
    YC3save = cboCTS3.Text
    YM0save = 1
    YM1save = cboMUX1.Text
    YM2save = cboMUX2.Text
    YM3save = cboMUX3.Text
    YG0save = 1
    YG1save = cboGAIN1.Text
    YG2save = cboGAIN2.Text
    YG3save = cboGAIN3.Text
    YT0save = 1
    YT1save = cboTB1.Text
    YT2save = cboTB2.Text
    YT3save = cboTB3.Text
    YP0save = 1
    YP1save = cboTPT1.Text
    YP2save = cboTPT2.Text
    YP3save = cboTPT3.Text
  End If
    
  UpdateX1X2
    
    If FirstMouseY > LastMouseY Then
      cboMUX2.Text = Int(FirstMouseY)
      cboMUX1.Text = Int(LastMouseY)
    Else
      cboMUX1.Text = Int(FirstMouseY)
      cboMUX2.Text = Int(LastMouseY)
    End If
    cboMUX3.Text = Int((Val(cboMUX2.Text) - Val(cboMUX1.Text)) / 10)
    If cboMUX3.Text = 0 Then cboMUX3.Text = 1
    
End If
    
Call UpdateMUXrange

End Sub

Private Sub picTA_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)

  picTA.ToolTipText = "TB=" + Format(x, "##0.0") + ", Loc=" + Format(y, "##0.0")

End Sub


Private Sub picTB_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim BoxWidth%, BoxHeight%
  
  If Button = 1 Then
    DrawingBox = True
    FirstMouseX = x: FirstMouseY = y
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpTB
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
      .BorderStyle = 3
      .Visible = True
    End With
    DoEvents
  End If

End Sub


Private Sub picTB_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, iCh%, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, BoxWidth%, BoxHeight%, k1%, k2%, UTks!
Static x1!, x2!, y1!, y2!, Tscale!, Tbottom!

  If Not ArrayRead Then Exit Sub
  If Button = 1 And DrawingBox = True Then
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpTB
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
    End With
  End If
  
iCh = 0: jLoc = 0
iX = Int(x): iY = Int(y)
Xthreshold = 20 * (Val(txtX2(2).Text) - Val(txtX1(2).Text)) / picTB.Width
Ythreshold = 20 * (Val(cboTB2.Text) - Val(cboTB1.Text)) / picTB.Height
'Debug.Print Xthreshold; Ythreshold

k1 = Int(iX - Xthreshold)
k2 = Int(iX + Xthreshold)
If k1 < 1 Then k1 = 1
If k2 > TotalCycles Then k2 = TotalCycles
'0 Tmtp 1 CH1 2 CH2 3 CH3 4 Tnav 5 P&R 6 Texp 7 1Hz 8 Zp
Tbottom = Val(cboTB1.Text)
Tscale = (Val(cboTB2.Text) - Tbottom) / 22#
For k = k1 To k2
  For i = 0 To 8
    If chkShowChannels(i).Value = 1 Then
      Select Case i
      Case 0
        'Debug.Print OATV(1, k) + OATnavCOR - y
        'If Abs(OATV(1, k) + OATnavCOR - y) < Ythreshold Then jLoc = k: Exit For
      Case 4   'Tnav
        'Debug.Print OATV(1, k) + OATnavCOR - y
        If Abs(OATV(1, k) + OATnavCOR - y) < Ythreshold Then jLoc = k: Exit For
      Case 8   'Zp
        'Debug.Print Tbottom; Tscale; ALTV(1, k); y; Tbottom + Tscale * ALTV(1, k) - y; Ythreshold
        If Abs(Tbottom + Tscale * ALTV(1, k) - y) < Ythreshold Then jLoc = k: Exit For
      End Select
    End If
  Next i
Next k

If jLoc <> 0 Then
  Select Case i
  Case 4: picTB.ToolTipText = "Tnav:" + "  X=" + Format(x, "###0.00") + "," + "Y=" + Format(OATV(1, k - 1) + OATnavCOR, "##0.00")
  Case 8:
    If Tscale > 0 Then picTB.ToolTipText = "Zp:" + "  X=" + Format(x, "###0.00") + "," + "Y=" + Format((y - Tbottom) / Tscale, "#0.00") + " km"
  End Select
Else
  picTB.ToolTipText = "X=" + Format(x, "###0.00") + "," + "Y=" + Format(y, "##0.00")
End If
On Error GoTo ExitSub
  UTks = (UV(1, iX) + (x - iX) * (UV(1, iX + 1) - UV(1, iX))) / 1000#
  Select Case frmMTPbin.cboUT.Text
  Case "s": frmMTPbin.txtUT(2).Text = Format(UTks * 1000#, "#####0.0")
  Case "ks": frmMTPbin.txtUT(2).Text = Format(UTks, "##0.0000")
  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(Int(UTks * 1000#), True)
  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 3600, "#0.0000")
  End Select
ExitSub:
'Debug.Print iX, iY, MV(jLoc, iX), Xthreshold, Ythreshold
End Sub


Private Sub picTB_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%

LastMouseX = Int(x)
LastMouseY = Int(y)
DrawingBox = False
shpTB.Visible = False

If FirstMouseX = LastMouseX Then 'GOTO this scan on cursor
  FirstMouseX = FirstMouseXSave
  LastMouseX = LastMouseXSave
  txtGOTO.Text = Str(Int(x))
  cmdGO_Click
  
Else
  FirstMouseXSave = FirstMouseX
  LastMouseXSave = LastMouseX
  If YT0save = 0 Then
    YC0save = 1
    YC1save = cboCTS1.Text
    YC2save = cboCTS2.Text
    YC3save = cboCTS3.Text
    YM0save = 1
    YM1save = cboMUX1.Text
    YM2save = cboMUX2.Text
    YM3save = cboMUX3.Text
    YG0save = 1
    YG1save = cboGAIN1.Text
    YG2save = cboGAIN2.Text
    YG3save = cboGAIN3.Text
    YT0save = 1
    YT1save = cboTB1.Text
    YT2save = cboTB2.Text
    YT3save = cboTB3.Text
    YP0save = 1
    YP1save = cboTPT1.Text
    YP2save = cboTPT2.Text
    YP3save = cboTPT3.Text
  End If
    
  UpdateX1X2
    
    If FirstMouseY > LastMouseY Then
      cboTB2.Text = Int(FirstMouseY)
      cboTB1.Text = Int(LastMouseY)
    Else
      cboTB1.Text = Int(FirstMouseY)
      cboTB2.Text = Int(LastMouseY)
    End If
    cboTB3.Text = Int((Val(cboTB2.Text) - Val(cboTB1.Text)) / 10)
    If cboTB3.Text = 0 Then cboTB3.Text = 1
    
End If
UpdateTBrange
cmdTB_Click 2
'cmdTB_Click (0)  'Clear Check boxes
End Sub

Private Sub picTPT_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim BoxWidth%, BoxHeight%
    
  If Button = 1 Then
    DrawingBox = True
    FirstMouseX = x: FirstMouseY = y
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpTPT
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
      .BorderStyle = 3
      .Visible = True
    End With
    DoEvents
  End If

End Sub


Private Sub picTPT_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, iCh%, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, Theta!, Tt$, yScale!
Dim BoxWidth%, BoxHeight%, k1%, k2%
Static x1!, x2!, y1!, y2!

  If Not ArrayRead Then Exit Sub
  If Button = 1 And DrawingBox = True Then
    LastMouseX = x: LastMouseY = y
    BoxWidth = Abs(LastMouseX - FirstMouseX) + 1
    BoxHeight = Abs(LastMouseY - FirstMouseY) + 1
    With shpTPT
      .Left = FirstMouseX
      .Width = BoxWidth
      .Top = FirstMouseY
      .Height = BoxHeight
    End With
  End If
  
iCh = 0: jLoc = 0
iX = Int(x): iY = Int(y)
Xthreshold = 20 * (Val(txtX2(4).Text) - Val(txtX1(4).Text)) / picTPT.Width
Ythreshold = 20 * (Val(cboTPT2.Text) - Val(cboTPT1.Text)) / picTPT.Height
If cboTPT4.Text = "km" Then yScale = 1 Else yScale = kft_km

k1 = Int(iX - Xthreshold)
k2 = Int(iX + Xthreshold)
If k1 < 1 Then k1 = 1
If k2 > TotalCycles Then k2 = TotalCycles
For k = k1 To k2
  For i = 1 To ThetaLevels
    If Abs(Tptw(i, k) * yScale - y) < Ythreshold Then jLoc = i: Exit For
  Next i
  If jLoc <> 0 Then Exit For
Next k

If jLoc <> 0 Then
  If optParms(0).Value = True Then Tt$ = "T=" Else Tt$ = "Theta="
  picTPT.ToolTipText = Tt$ + Format(ThetaStart + (i - 1) * ThetaStep, "000") + " K: " + "  X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
Else
  picTPT.ToolTipText = "X=" + Format(x, "###0") + "," + "Y=" + Format(y, "##0.0")
End If
On Error GoTo ExitSub
  Select Case frmMTPbin.cboUT.Text
  Case "s": frmMTPbin.txtUT(2).Text = Format(UV(1, x), "#####0")
  Case "ks": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 1000, "##0.000")
  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UV(1, x), True)
  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UV(1, x) / 3600, "#0.0000")
  End Select
ExitSub:
'Debug.Print iX, iY, MV(jLoc, iX), Xthreshold, Ythreshold
End Sub


Private Sub picTPT_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%

LastMouseX = Int(x)
LastMouseY = Int(y)
DrawingBox = False
shpTPT.Visible = False

If FirstMouseX = LastMouseX Then 'GOTO this scan on cursor
  FirstMouseX = FirstMouseXSave
  LastMouseX = LastMouseXSave
  txtGOTO.Text = Str(Int(x))
  cmdGO_Click
  
Else
  FirstMouseXSave = FirstMouseX
  LastMouseXSave = LastMouseX
  If YT0save = 0 Then
    YC0save = 1
    YC1save = cboCTS1.Text
    YC2save = cboCTS2.Text
    YC3save = cboCTS3.Text
    YM0save = 1
    YM1save = cboMUX1.Text
    YM2save = cboMUX2.Text
    YM3save = cboMUX3.Text
    YG0save = 1
    YG1save = cboGAIN1.Text
    YG2save = cboGAIN2.Text
    YG3save = cboGAIN3.Text
    YT0save = 1
    YT1save = cboTB1.Text
    YT2save = cboTB2.Text
    YT3save = cboTB3.Text
    YP0save = 1
    YP1save = cboTPT1.Text
    YP2save = cboTPT2.Text
    YP3save = cboTPT3.Text
  End If
    
  UpdateX1X2
    
    If FirstMouseY > LastMouseY Then
      cboTPT2.Text = Int(FirstMouseY)
      cboTPT1.Text = Int(LastMouseY)
    Else
      cboTPT1.Text = Int(FirstMouseY)
      cboTPT2.Text = Int(LastMouseY)
    End If
    cboTPT3.Text = Int((Val(cboTPT2.Text) - Val(cboTPT1.Text)) / 10)
    If cboTPT3.Text = 0 Then cboTPT3.Text = 1
    
End If
    
cmdTPT_Click (0)
End Sub

Private Sub pictrack_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
FirstMouseLon = Int(x)
FirstMouseLat = Int(y)
End Sub

Private Sub picTrack_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim i%, j%, k%, k1%, k2%, iUT&, jLoc%, iX%, iY%, Xthreshold!, Ythreshold!, iRec%, Track$, r%
  If TotalCycles <= 20 Then Exit Sub
  If Not ArrayRead Then Exit Sub
  iX = Int(x): iY = Int(y): iRec = 0: iUT = 0
  Xthreshold = 10 * (Val(txtFTX2.Text) - Val(txtFTX1.Text)) / picTrack.Width
  Ythreshold = 10 * (Val(txtFTY2.Text) - Val(txtFTY1.Text)) / picTrack.Height
  For r = 1 To TotalCycles
    If Abs(NAVV(3, r) - y) < Ythreshold And Abs(NAVV(4, r) - x) < Xthreshold Then iUT = UV(1, r): iRec = r: Exit For
  Next r

  If iUT <> 0 Or iRec <> 0 Then
    Select Case cboTrack.Text
    Case "Rec No.": Track$ = Str(iRec)
    Case "seconds": Track$ = Format(iUT, "#####0")
    Case "ks": Track$ = Format(iUT / 1000, "##0.000")
    Case "hh:mm:ss": Track$ = fSecToTstring$(iUT, True)
    End Select
    picTrack.ToolTipText = "Lon=" + Format(x, "##0.0") + "," + "Lat=" + Format(y, "##0.0") + "  UT(s)=" + Track$
  Else
    picTrack.ToolTipText = "Lon=" + Format(x, "##0.0") + "," + "Lat=" + Format(y, "##0.0")
  End If

End Sub



Private Sub SSTab1_Click(PreviousTab As Integer)
If LastMouseXSave > 0 Then
  Select Case SSTab1.Tab
  Case 3
    UpdateFlightTrack
  Case 4
    frmFLTINFO.SSTab1.Tab = 6
    UpdateCTSrange
  Case 5
    UpdateMUXrange
  Case 8
    frmFLTINFO.SSTab1.Tab = 7
  End Select
End If
End Sub

Private Sub SSTab1_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub




Sub UpdateREFfile()
'  cboFltDates.ListIndex = Lindex
'  cboFltNos.ListIndex = Lindex
'  cboObjectives.ListIndex = Lindex
'  yyyymmdd$ = Trim(cboFltDates.Text)
'  yymmdd$ = Right$(yyyymmdd$, 6)
'  Path$ = Drive$ + Rdir2$ + Mission$ + "\" + yyyymmdd$ + "\"
'  If Not RealTime Then txtREFfile.Text = Path$ + AC$ + yyyymmdd$ + ".REF"
End Sub
Sub UpdateForm()
' Update information of MTPbin form
Dim LineC&, j%, DandT$, f As Form, i%
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3)
Static FP!(1 To 4), LastRawcycle%
  
  Set f = frmFLTINFO
  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$
  
  For i = 1 To NRC
    If i <= NRCbin Then
      lblRCname(i - 1).Caption = f.cboReg(i - 1).Text
      lblRCname(i - 1).ToolTipText = f.cboReg(i - 1).ToolTipText
    End If
  Next i
  If NRC < NRCbin Then
    For i = NRC + 1 To NRCbin
      lblRCname(i - 1).Caption = ""
      txtlnP(i - 1).BackColor = &HFFFFFF
      txtRCuse(i - 1) = ""
      RCuse(i - 1) = 0
    Next i
  End If
  
  MPfile$ = MPpath$ + "MP" + YYYYMMDD$ + fACext(AC$)
  MTP_MPfile$ = MPpath$ + "MTP-Tprofile_GHawk_" + YYYYMMDD$ + "_R" + Format(cboICTrev.Text, "0") + ".ict"
  ERFfile$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
  If Not RealTime Then REFfile$ = Path$ + AC$ + YYYYMMDD$ + ".REF"
  
  If Not RealTime Then txtREFfile = Path$ + AC$ + YYYYMMDD$ + ".REF"
'  txtmri1 = Format$(MRI, "##0.00")
'  txtMRI2 = Format$(MRI2, "##0.00")

  Call TimeStats
  txtDTavg.Text = DTavg
  txtDTrms.Text = DTrms
  txtMRIavg.Text = MRIavg
  txtMRIrms.Text = MRIrms

 
  If chkShowTBs.Value = 1 Then
    Tbase = cTo + Ttgt                 'Convert to Kelvin for REF=Tbase
    Call GainCalculation(RawCycle)            'Lots to gain here!
    Call TBcalculation              'And calculate observable vector
  End If
  
  UpdateRT
    
  UpdateCommLine
'  txtTTO.Text = Format(TTO, "00.0")
'  Select Case frmMTPbin.cboUT.Text
'  Case "s": frmMTPbin.txtUT(2).Text = Format(UTsec, "#####0")
'  Case "ks": frmMTPbin.txtUT(2).Text = Format(UTsec / 1000, "##0.000")
'  Case "hh:mm:ss": frmMTPbin.txtUT(2).Text = fSecToTstring$(UTsec, True)
'  Case "hh.hhhh": frmMTPbin.txtUT(2).Text = Format(UTsec / 3600#, "#0.0000")
'  End Select
  txtRecord.Text = Str(RawCycle)
  txtFileRecords.Text = Str(TotalCycles)
  If TotalCycles > 0 Then
    ProgressBar1.Min = 0
    ProgressBar1.Max = TotalCycles
'    ProgressBar1.value = RawCycle
  End If
' Mux data (0 to 24)
  If Left$(SU$, 3) = "CAN" Then
    Tx(0) = "Ttgt": Tx(1) = "V-8": Tx(2) = "Vvid":  Tx(3) = "V+8": Tx(4) = "Vmtr"
    Tx(5) = "Vsyn":  Tx(6) = "V+15": Tx(7) = "V+5": Tx(8) = "V-15"
    Tx(9) = "ACC": Tx(10) = "Tdat": Tx(11) = "Tmtr":  Tx(12) = "Tair"
    Tx(13) = "Tsm": Tx(14) = "Tps":  Tx(15) = "Tnc": Tx(16) = "Tsyn"
    Tx(17) = "R350":  Tx(18) = "Ttg1": Tx(19) = "Ttg2": Tx(20) = "Twin"
    Tx(21) = "Tmix": Tx(22) = "Tamp": Tx(23) = "Tnd":  Tx(24) = "R600"
  Else
    Tx(0) = "Ttgt": Tx(1) = "Tnd": Tx(2) = "Tlo1":  Tx(3) = "Tlo2"
    Tx(4) = "Tifa": Tx(5) = "Tlo":  Tx(6) = "Thi"
    Tx(7) = "0.1g": Tx(8) = "Twin":  Tx(9) = "Tmtr"
    Tx(10) = "Spr": Tx(11) = "Vref":  Tx(12) = "DC1"
    Tx(13) = "DC2": Tx(14) = "5V":  Tx(15) = "12V"
    Tx(16) = "A(-)": Tx(17) = "":  Tx(18) = ""
    Tx(19) = "": Tx(20) = "":  Tx(21) = ""
    Tx(22) = "": Tx(23) = "":  Tx(24) = ""
  End If

' Nav data (25 to 48) was 20-31
  Tx(25) = "To":  Tx(26) = "Zt1": Tx(27) = "Tnav": Tx(28) = "Tmms"
  Tx(29) = "Tmtp": Tx(30) = "pALT": Tx(31) = "gALT":  Tx(32) = "rALT"
  Tx(33) = "Pitch": Tx(34) = "Roll":  Tx(35) = "Wspd": Tx(36) = "Wdir"
  Tx(37) = "VWS":  Tx(38) = "Gspd": Tx(39) = "TAS": Tx(40) = "Hdg"
  Tx(41) = "Mach":  Tx(42) = "Vzac": Tx(43) = "Tdw": Tx(44) = "Ttot"
  Tx(45) = "Psta":  Tx(46) = "Pdy": Tx(47) = "SZA": Tx(48) = "SAAC"

' Misc data (49 to 64) was 17-19 and 32-38
  Tx(49) = "TtMA": Tx(50) = "TmMA": Tx(51) = "TwMA": Tx(52) = "Zt1"
  Tx(53) = "LRac": Tx(54) = "LRac2": Tx(55) = "LRac3":  Tx(56) = "Gain1"
  Tx(57) = "Gain2": Tx(58) = "Gain3":  Tx(59) = "": Tx(60) = ""
  Tx(61) = "": Tx(62) = "":  Tx(63) = "": Tx(64) = ""

' Status data (65 to 72) was 39-46
  Tx(65) = "Gs": Tx(66) = "DU":  Tx(67) = "SU":  Tx(68) = "SP1"
  Tx(69) = "SP2": Tx(70) = "SP3":  Tx(71) = "EC": Tx(72) = "PLO"
' Write labels on MUX tab
  For i = 1 To MVchannels
    chkMux(i).Caption = Tx(i)
  Next i
  
  txtTA1(LocHor - 1).BackColor = &HFF00&
  txtTA2(LocHor - 1).BackColor = &HFF00&
  txtTA3(LocHor - 1).BackColor = &HFF00&

  DoEvents
End Sub

Sub TimeStats()
Dim utMTPcorf!, T1Sum!, T2Sum!
Static Rec%

  utMTPcorf = UTsecNav - UTsecMTP   'Only update correction on valid nav sec
  If Abs(utMTPcorf) > 6000 Then
    If utMTPcorf < 0 Then
      utMTPcorf = utMTPcorf + 86400
    Else
      utMTPcorf = utMTPcorf - 86400
    End If
  End If
' If utMTPcorf > 100 Then utMTPcorf = utMTPcor 'Use MTP if NAV is 100 s off
  INC Rec%                          'Count number of good nav cycles
  T1Sum = T1Sum + utMTPcorf         'Sum time difference
  T2Sum = T2Sum + utMTPcorf ^ 2     'Sum square of time differences
  If Rec% > 10 Then
    DTavg = T1Sum / Rec%            'Calculate average
    If T2Sum - DTavg ^ 2 / Rec > 0 Then DTrms = Sqr((T2Sum - DTavg ^ 2 * Rec) / (Rec% - 1)) 'Calculate RMS
  End If

End Sub



Sub UpdateTBwindow()
' Update TB display window
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, FL!, iRC1%, iRC2%, TAAp!(1 To 20), AAbias!
  
  If chkShowOnlyGoodscans.Value = 1 And Not GoodScan Then Exit Sub

' Plot Antenna Tempertures in picture window
  With picTA
  .Cls
  .ScaleMode = 0
  .ScaleTop = 0
  .ScaleHeight = 11
  End With
'If chkShowArcAvg.Value = 1 Then
'' Calculate weighted average values of archive average observables
'    Call Get_iBot_iTop(RCindex1, pALT, iBot, iTop, WtB, WtT)
'    If iBot = 1 Then iBot = 2: iTop = 1
'    For j = 1 To Nobs
'      OBavgWt(RCindex1, j) = OBav(RCindex1, iBot, j) * WtB + OBav(RCindex1, iTop, j) * WtT
'    Next j
'    SetArcAvgObs RCindex1 'Map AA Avg Observable to TAA array for UpdateTBwindow
'    If RCindex2 >= 0 Then 'Need to plot second set of AA observables.
'      Call Get_iBot_iTop(RCindex2, pALT, iBot, iTop, WtB, WtT)
'      If iBot = 1 Then iBot = 2: iTop = 1
'      For j = 1 To Nobs
'        OBavgWt(RCindex2, j) = OBav(RCindex2, iBot, j) * WtB + OBav(RCindex2, iTop, j) * WtT
'      Next j
'      SetArcAvgObs RCindex2 'Map AA Avg Observable to TAA array for UpdateTBwindow
'    End If
'  End If
' Set temperature display scale
  If AutoScaleTA Then
    Tmin = 500
    Tmax = 100
    For i = 1 To 10
      T = TA(1, i)
      If T > Tmax Then Tmax = T
      If T < Tmin Then Tmin = T
    Next i
    
    If chkShowArcAvg.Value = 1 Then
      For j = 1 To 10
        T = TAA(RCindex1, 1, j)
        If T > Tmax Then Tmax = T
        If T < Tmin Then Tmin = T
      Next j
      If RCindex2 >= 0 Then 'Need to plot second set of AA observables.
        For j = 1 To 10
          T = TAA(RCindex2, 1, j)
          If T > Tmax Then Tmax = T
          If T < Tmin Then Tmin = T
        Next j
      End If
    End If
    
    Tmin = ((Tmin - 5) \ 5) * 5
    Tmax = ((Tmax + 5) \ 5) * 5
    picTA.ScaleWidth = Tmax - Tmin
    If picTA.ScaleWidth < 20 Then
      Tmax = Tmin + 20
      picTA.ScaleWidth = 20
    End If
    picTA.ScaleLeft = Tmin
    txtT1.Text = Format(Tmin, "000")
    txtT2.Text = Format(Tmax, "000")
    txtTC.Text = Format(picTA.ScaleWidth, "000")
  Else
    Tmin = Val(txtT1.Text)
    picTA.ScaleLeft = Tmin
    picTA.ScaleWidth = Val(txtTC.Text)
    Tmax = picTA.ScaleLeft + picTA.ScaleWidth
    txtT2.Text = Format(Tmax, "000")
  End If
  
  For i = 1 To Channels
    Select Case i
    Case 1: LineC = QBColor(12)  'LT RED
    Case 2: LineC = QBColor(15)  'Bright WHITE
    Case 3: LineC = QBColor(9)   'LT BLUE
    End Select
    picTA.PSet (TA(i, 1), 1)
    For j = 2 To 10
      picTA.Line -(TA(i, j), j), LineC    'And continue from there
    Next j
  Next i
  
' Draw archive average TAs
  If chkShowArcAvg.Value = 1 Then
    Select Case Algorithm
    Case 1, 2, 3, 4, 5, 6
      iRC1 = aRC(IlnPmin1)
      iRC2 = aRC(IlnPmin2)
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = 1 To 10: TAAp(j) = TAA(iRC1, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC1, 2)
          For j = 1 To 10: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(1), 1)
        For j = 2 To 10
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
      If Sgn(TBavg(iRC2)) <> Sgn(TBavg(iRC1)) Then
        For i = 1 To Channels
          Select Case i
          Case 1: LineC = QBColor(4)  'RED
          Case 2: LineC = QBColor(7)  'WHITE
          Case 3: LineC = QBColor(1)   'BLUE
          End Select
          For j = 1 To 10: TAAp(j) = TAA(iRC2, i, j): Next j
          If chkRemoveAAbias.Value = 1 Then
            AAbias = fCalcBiasWrtAA(ob(), iRC2, 2)
            For j = 1 To 10: TAAp(j) = TAAp(j) + AAbias: Next j
          End If
          picTA.PSet (TAAp(1), 1)
          For j = 2 To 10
            picTA.Line -(TAAp(j), j), LineC    'And continue from there
          Next j
        Next i
      End If
      
    Case 7  'Separate Up and Dn retrievals
      iRC1 = aRC(IlnPmin1)
      iRC2 = aRC(IlnPmin2)
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = 1 To LocHor: TAAp(j) = TAA(iRC1, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC1, 2)
          For j = 1 To LocHor: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(1), 1)
        For j = 2 To LocHor
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = LocHor To 10: TAAp(j) = TAA(iRC2, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC2, 2)
          For j = LocHor To 10: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(LocHor), LocHor)
        For j = LocHor + 1 To 10
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
    End Select
  End If

' Draw tic marks
  For i = 1 To 10
    If i = LocHor Then 'A/C Level
      picTA.PSet (Tmin, i)
      picTA.Line -(Tmin + picTA.ScaleWidth, i), QBColor(15)
    Else
      picTA.PSet (Tmin, i)
      picTA.Line -(Tmin + 0.04 * picTA.ScaleWidth, i), QBColor(15)
      picTA.PSet (Tmin + picTA.ScaleWidth, i)
      picTA.Line -(Tmin + 0.96 * picTA.ScaleWidth, i), QBColor(15)
    End If
  Next i
' Draw a/c altitude
  If chkShowFL.Value = 1 Then
    FL = 11 - pALT / 2
    picTA.PSet (Tmin, FL)
    picTA.Line -(Tmin + picTA.ScaleWidth, FL), QBColor(13)  'Pink
'    picTA.Line -(Tmin + 0.04 * picTA.ScaleWidth, FL), QBColor(13) 'Pink
'    picTA.PSet (Tmin + picTA.ScaleWidth, FL)
'    picTA.Line -(Tmin + 0.96 * picTA.ScaleWidth, FL), QBColor(13)
  End If
End Sub
Sub UpdateFLTINFO()
Dim D$, f As Form

'DoAll = False            'Always make DoAll FALSE except in MTPman
Set f = frmFLTINFO
Drive$ = DataDrive$
PgmDrive$ = ProgramDrive$

'UTmin = Val(frmFLTINFO.txtUTmin.Text)
'UTmax = Val(frmFLTINFO.txtUTmax.Text)

If Algorithm = 2 Then UserLATs = True Else UserLATs = False

' Time and MRI Statistics
DTavg = Val(txtDTavg.Text)
DTrms = Val(txtDTrms.Text)
MRIavg = Val(txtMRIavg.Text)
MRIrms = Val(txtMRIrms.Text)

' Counts Thresholds
'CMAcycles = Val(txtCMAcycles.Text)
'RFIthreshold1 = Val(txtRFIthreshold1.Text)
'RFIthreshold2 = Val(txtRFIthreshold2.Text)
'MUXthreshold = Val(txtMUXthreshold.Text)

End Sub


Private Sub TBs_Click()
  If mnuViewATP.Checked Then
    mnuViewATP.Checked = False
    ShowATP = False
    frmATP.Hide
  Else
    mnuViewATP.Checked = True
    ShowATP = True
    frmATP.Show
  End If

End Sub

Private Sub tmrGetCycle_Timer()
'  tmrGetCycle.Interval = Val(txtTimerSpeed.Text) 'Auto Step cycle time in ms
  If DoRetrieval Then
    If RawCycle > TotalCycles - 1 Then
      tmrGetCycle.Enabled = False   ' STOP when last cycle is reached
      REFstamp = Date + Time
      frmFLTINFO.UpdateForm
      frmFLTINFO.UpdateFLTINFO
      REFwriteHEADER REFfile$
      REFwriteHEADER BINfile$
      REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"
      WriteCAL CALfile$
      Close
      If DoAll Then frmTools.tmrBatch1.Enabled = True
      Exit Sub
    Else
      If RetrieveSingleScan Then
        tmrGetCycle.Enabled = False   ' STOP after one cycle
        Call ReadArray(RawCycle%)
        Main
      Else
        Call ReadArray(RawCycle%)
        Main
      End If
    End If
  Else
    Call ReadArray(RawCycle%)    'Call GetBINcycle(RawCycle%)
  End If
  NewRetrieval = False
  DoEvents
End Sub

Private Sub tmrGoIWG_Timer()

  tmrGoIWG.Enabled = False
  Select Case cboSpeed.Text
  Case "1X": tmrGoIWG.Interval = 1000
  Case "5X": tmrGoIWG.Interval = 200
  Case "10X": tmrGoIWG.Interval = 100
  Case "50X": tmrGoIWG.Interval = 20
  Case "100X": tmrGoIWG.Interval = 10
  End Select
  If IWGforward Then
    cmdTestIWG_Click (1)
  Else
    cmdTestIWG_Click (3)
  End If
  txtIWGrecord.Text = IWGrecord

  tmrGoIWG.Enabled = True
  
End Sub

Private Sub tmrNDP_Timer()

If FTPcount = 0 Then
  chkWriteND.Value = 1  'Start Timer
Else
  FTPcount = FTPcount - 1
End If

End Sub



Private Sub tmrRealTime_Timer()
Dim i%, CMDlu%, cMd$, Status As Boolean, PathRAW$, PathLocal$, PathLocal2$, PathRemote$, PathRemote2$
Dim FQFN0$, FQFN1$, FQFN2$, FQFN5$, FQFN6$, DTstamp$, UDP_Status$, FQFN3$
Dim FilenameRAW$, FilenamePNG$, FilenameHTML$, OUTlu%, FilenameATP$

  tmrRealTime.Enabled = False
  On Error GoTo ErrorHandler
  
' Define filenames and paths here
  FilenameRAW = "MTPcurrent.RAW"
  FilenamePNG = "MTPprofile.PNG"
  FilenameHTML = "index.html"
  FilenameATP = "ATP.txt"
  PathRAW = "C:\MTP\RTA\RAW\"   'frmFLTINFO.cboDataSource.Text    '
  PathLocal = "C:\MTP\WWW\missions\realtime\"
  PathLocal2 = "C:\MTP\WWW\missions\realtime\"
  PathRemote = frmFLTINFO.cboDestination.Text   'cCD(DataDestinationMode)
  PathRemote2 = frmFLTINFO.cboDestination2.Text   'cCD2(DataDestinationMode2)
' HTML and PNG sent to local C-drive web page
  FQFN0$ = PathRAW + FilenameRAW
  FQFN1$ = PathLocal + FilenameHTML
  FQFN2$ = PathLocal + FilenamePNG
  FQFN3$ = PathLocal + FilenameATP
  
' Check whether MTPcurrent.RAW is from FTP or local HD
  If Left$(PathRAW, 3) = "ftp" Then
    Inet1.URL = cFTP0(DataSourceMode)
    GoSub InetStatus
    Inet1.UserName = cUsername0(DataSourceMode)
    GoSub InetStatus
    Inet1.Password = cPassword0(DataSourceMode)
    GoSub InetStatus
    Inet1.Execute , "CD " + cCD0(DataSourceMode)
    GoSub InetStatus
    Inet1.Execute "GET MTPcurrent.RAW C:\MTP\RTA\RAW\MTPcurrent.RAW"      'MTPcurrent.RAW
    GoSub InetStatus
  End If
  CurrentStamp = FileDateTime(PathRAW + FilenameRAW) ' Returns "2/12/93 4:35:47 PM".
          
' Check whether MTPcurrent.RAW time stamp has changed. If so, process the new file.
  If CurrentStamp > LastStamp Then
'   Clear raw data lines
    For i = 1 To 6: LineLast(i) = LineCurr(i): Next i
    
'   Open REALTIME MTPcurrent.RAW file and read it
    RAWlu = FreeFile
    Open FQFN0$ For Input As RAWlu
    Call ReadCURRENTrecord(RAWlu)
    Close RAWlu
    CtsQual = True           'Normally obtained from ReadArray if not REALTIME
    
    If LineCurr(1) = "" Then 'Check for valid A-line
      NavData = False
      frmStatus.txtStatus.Text = "No NAV"
    Else
      NavData = True
      frmStatus.txtStatus.Text = ""

'     Do retrieval
      Main
'     Write RA() amd RT() values to txt file
      OUTlu = FreeFile
      Open PathLocal + "ATP.txt" For Output As OUTlu
      For i = 1 To Nlev
        Print #OUTlu, Format(i, "00") + "  "; Format(RAp(i), "00.00") + "  "; Format(RTp(i), "000.00")
      Next i
      Close OUTlu
      
'     Write frmATP image to C:\WWW\missions\realtime\MTPprofile.PNG
      Call CaptureFormMJ(frmATP, PathLocal2 + FilenamePNG, 1, False)
'      FileCopy PathLocal + FilenamePNG, PathLocal2 + FilenamePNG
      
'     Write dynamic HTML to C:\www\missions\realtime\index.html
      RealtimeHTML (PathLocal2 + FilenameHTML)
'      FileCopy PathLocal + FilenameHTML, PathLocal2 + FilenameHTML

'     Check if MTPprofile.PNG and index.html need to be FTPed somewhere
      If UCase$(Left$(PathRemote, 3)) = "FTP" Then             'FTP
        ' Send URL
        Inet1.URL = cFTP(DataDestinationMode)
        Debug.Print icError
        GoSub InetStatus
          
        Inet1.UserName = cUsername(DataDestinationMode)
        GoSub InetStatus
          
        Inet1.Password = cPassword(DataDestinationMode)
        GoSub InetStatus
          
'        Inet1.Execute , "CD " + cCD(DataDestinationMode)
        GoSub InetStatus
        
        Inet1.Execute , "PUT " & FQFN1$ & " " & FilenameHTML     'HTML code
        GoSub InetStatus
        
'        Inet1.Execute , "PUT " & FQFN2$ & " " & FilenamePNG      'MTPprofile.PNG
'        GoSub InetStatus
      End If
      
      If UCase$(Left$(PathRemote2, 3)) = "FTP" Then             'FTP
'       Send files to ground
'       Change to downlink folder and send PNG and TXT files
        ' Send URL
        Inet1.URL = cFTP2(DataDestinationMode2)

        GoSub InetStatus
        
        Inet1.UserName = cUsername2(DataDestinationMode2)
        GoSub InetStatus
          
        Inet1.Password = cPassword2(DataDestinationMode2)
        GoSub InetStatus

        Inet1.Execute "", "CD " + cCD2(DataDestinationMode2)
        GoSub InetStatus
        
'       DTstamp = fTstamp
'       Inet1.Execute , "PUT " & FQFN2 & " " & "MTPprofile_" & fTstamp & ".PNG"      'MTPprofile_yyyymmdd_hhmmss.PNG
'       GoSub InetStatus

        Inet1.Execute "", "PUT " & FQFN1$ & " " & FilenameHTML     'HTML code
        GoSub InetStatus
        
        Inet1.Execute "", "PUT " & FQFN2$ & " " & FilenamePNG
        GoSub InetStatus
        
        Inet1.Execute "", "PUT " & FQFN3$ & " " & FilenameATP
        GoSub InetStatus
        
        Inet1.Execute "", "QUIT"
        GoSub InetStatus
        
'       Send UDP Status to Status Page
        If Left$(SU$, 3) = "CAN" Then
          'UDP_Status = "<?xml version=""1.0""?><group><name>MTP</name><clock>"
          '   UDP_Status = UDP_Status & fTstamp() & "</clock></group>"
          'Winsock1.SendData UDP_Status
        End If
      Else
'        FileCopy FQFN1$, PathRemote + FilenameHTML    'HTML cod
'        FileCopy FQFN2$, PathRemote + FilenamePNG     'MTPprofile.PNG
      End If
    End If
    LastStamp = CurrentStamp
  End If
  
  CommandStamp = FileDateTime("C:\MTP\RTA\Command.TXT")
  If CommandStamp > LastCommandStamp Then
    CMDlu = FreeFile
    Open "C:\MTP\RTA\Command.TXT" For Input As CMDlu
    Input #CMDlu, cMd$
    Close CMDlu
    Select Case cMd$
    Case "NONE"
    Case "Exit"
      cmdREF_Click (3)
    End Select
    
  End If
  tmrRealTime.Enabled = True  'Wait for file to update
Exit Sub

InetStatus:
  Do While Inet1.StillExecuting
  DoEvents
  Loop
  'Debug.Print icError; Inet1.ResponseInfo
  Return


ErrorHandler:
' Re-enable timer so data is still processed even if not FTPed
  tmrRealTime.Enabled = True  'Wait for file to update
  Exit Sub

'---------------------------------------------------------------
'       FTP test code
      ' Send URL
        Inet1.Protocol = icFTP
        Inet1.RequestTimeout = 60
        Inet1.RemotePort = 21
        Inet1.AccessType = icDirect
        Inet1.URL = "ftp://ghoc.dfrc.nasa.gov"
        GoSub InetStatus
        
        Inet1.UserName = "mtp"
        GoSub InetStatus
          
        Inet1.Password = "emptypea"
        GoSub InetStatus

        Inet1.Execute "", "CD public_html"
        GoSub InetStatus
        
        Inet1.Execute "", "PUT" & Space(1) & "C:\MTP\WWW\missions\realtime\index.html" & " " & "index.html"     'HTML code
        GoSub InetStatus
        Inet1.Execute "", "PUT" & Space(1) & "C:\MTP\WWW\missions\realtime\MTPprofile.PNG" & " " & "MTPprofile.PNG"
        GoSub InetStatus
        Inet1.Execute "", "QUIT"
        GoSub InetStatus
        
Exit Sub

End Sub

Private Sub tmrSerialStart_Timer()
  If frmSerialIO.cmdStart.Caption = "&End Input" Then
    tmrSerialStart.Enabled = False
    'put in timer to wait for Serial io to start
    
    RAWlu = FreeFile
    Open frmSerialIO.txtFilename For Input As RAWlu
    
    BINlu = FreeFile
    Open fExtension(frmSerialIO.txtFilename) + ".BIN" For Random Access Read Write As BINlu Len = Len(REF)
  End If

End Sub

Private Sub txtX1_Change(Index As Integer)
'If Not EnableCXY And Not EnableMXY Then
'FirstMouseX = Val(txtX1(Index).Text)
End Sub

Private Sub txtX2_Change(Index As Integer)
'LastMouseX = Val(txtX2(Index).Text)
End Sub


Sub Winsock1_Close()
  
  Do Until Winsock1.State = sckClosed
    Winsock1.Close
    DoEvents
  Loop

End Sub

Sub Winsock1_Connect()
On Error GoTo errhandler

  With Winsock1
' Set the remotehost property
  .RemoteHost = "239.0.0.20"
' Set the remoteport property.
' This should be equal to the localhost property of the remote machine.
  .RemotePort = "30001"
' The localport property cannot be changed,so check if it has already been set
    If .LocalPort = Empty Then
      .LocalPort = "30000"
      .Bind .LocalPort
    End If
  End With
  
  Exit Sub

errhandler:
  MsgBox "Winsock failed to establish connection with remote server", vbCritical

End Sub

Public Function TimeString(seconds As Long, Optional Verbose _
As Boolean = False) As String

'if verbose = false, returns
'something like
'02:22.08
'if true, returns
'2 hours, 22 minutes, and 8 seconds

Dim lHrs As Long
Dim lMinutes As Long
Dim lSeconds As Long

lSeconds = seconds

lHrs = Int(lSeconds / 3600)
lMinutes = (Int(lSeconds / 60)) - (lHrs * 60)
lSeconds = Int(lSeconds Mod 60)

Dim sAns As String


If lSeconds = 60 Then
    lMinutes = lMinutes + 1
    lSeconds = 0
End If

If lMinutes = 60 Then
    lMinutes = 0
    lHrs = lHrs + 1
End If

sAns = Format(CStr(lHrs), "#####0") & ":" & _
  Format(CStr(lMinutes), "00") & "." & _
  Format(CStr(lSeconds), "00")

If Verbose Then sAns = TimeStringtoEnglish(sAns)
TimeString = sAns

End Function

Private Function TimeStringtoEnglish(sTimeString As String) _
As String

Dim sAns As String
Dim sHour, sMin As String, sSec As String
Dim itemp As Integer, sTemp As String
Dim iPos As Integer
iPos = InStr(sTimeString, ":") - 1

sHour = Left$(sTimeString, iPos)
If CLng(sHour) <> 0 Then
    sAns = CLng(sHour) & " hour"
    If CLng(sHour) > 1 Then sAns = sAns & "s"
    sAns = sAns & ", "
End If

sMin = Mid$(sTimeString, iPos + 2, 2)

itemp = sMin

If sMin = "00" Then
   sAns = IIf(Len(sAns), sAns & "0 minutes, and ", "")
Else
   sTemp = IIf(itemp = 1, " minute", " minutes")
   sTemp = IIf(Len(sAns), sTemp & ", and ", sTemp & " and ")
   sAns = sAns & Format$(itemp, "##") & sTemp
End If

itemp = Val(Right$(sTimeString, 2))
sSec = Format$(itemp, "#0")
sAns = sAns & sSec & " second"
If itemp <> 1 Then sAns = sAns & "s"

TimeStringtoEnglish = sAns

End Function
