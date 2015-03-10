VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmREFview 
   Caption         =   "REF File Viewer"
   ClientHeight    =   5205
   ClientLeft      =   1020
   ClientTop       =   1710
   ClientWidth     =   4800
   ForeColor       =   &H00C0C0C0&
   Icon            =   "REFview.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   5205
   ScaleWidth      =   4800
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   3960
      TabIndex        =   475
      Top             =   4815
      Width           =   735
   End
   Begin VB.Timer tmrGetCycle 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   2205
      Top             =   3645
   End
   Begin VB.TextBox txtGoodTrop 
      Alignment       =   2  'Center
      BackColor       =   &H00008000&
      ForeColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   3240
      MultiLine       =   -1  'True
      TabIndex        =   16
      Top             =   4485
      Width           =   1455
   End
   Begin VB.TextBox txtGoodScan 
      Alignment       =   2  'Center
      BackColor       =   &H00008000&
      ForeColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   3240
      MultiLine       =   -1  'True
      TabIndex        =   15
      Top             =   4185
      Width           =   1455
   End
   Begin VB.CommandButton cmdBack 
      Caption         =   "&Back"
      Height          =   375
      Left            =   1275
      TabIndex        =   11
      Top             =   4815
      Width           =   735
   End
   Begin VB.CommandButton cmdFwd 
      Caption         =   "&Fwd"
      Height          =   375
      Left            =   3135
      TabIndex        =   10
      Top             =   4815
      Width           =   735
   End
   Begin VB.Frame Frame4 
      Caption         =   "GOTO"
      Height          =   615
      Left            =   120
      TabIndex        =   4
      Top             =   4125
      Width           =   3015
      Begin VB.CommandButton cmdGO 
         Caption         =   "&GO"
         Height          =   345
         Left            =   2130
         TabIndex        =   7
         Top             =   150
         Width           =   735
      End
      Begin VB.ComboBox cmbGOTO 
         Height          =   315
         Left            =   960
         TabIndex        =   6
         Text            =   "Rec No"
         Top             =   210
         Width           =   1095
      End
      Begin VB.TextBox txtGOTO 
         Height          =   285
         Left            =   120
         TabIndex        =   5
         Top             =   210
         Width           =   855
      End
   End
   Begin VB.CommandButton cmdStop 
      Caption         =   "&Stop"
      Height          =   375
      Left            =   2085
      TabIndex        =   3
      Top             =   4815
      Width           =   975
   End
   Begin VB.Frame Frame1 
      Caption         =   "Universal Time"
      Height          =   615
      Left            =   120
      TabIndex        =   0
      Top             =   3480
      Width           =   4575
      Begin VB.TextBox txtFileRecords 
         Enabled         =   0   'False
         Height          =   285
         Left            =   3960
         TabIndex        =   14
         Top             =   240
         Width           =   495
      End
      Begin VB.TextBox txtRecord 
         Height          =   285
         Left            =   3120
         TabIndex        =   9
         Top             =   240
         Width           =   495
      End
      Begin VB.ComboBox cmbUT 
         Height          =   315
         Left            =   960
         TabIndex        =   2
         Text            =   "hh:mm:ss"
         Top             =   240
         Width           =   1095
      End
      Begin VB.TextBox txtUT 
         Height          =   285
         Left            =   120
         TabIndex        =   1
         Top             =   240
         Width           =   855
      End
      Begin VB.Label Label50 
         Caption         =   "of"
         Height          =   255
         Left            =   3720
         TabIndex        =   13
         Top             =   240
         Width           =   255
      End
      Begin VB.Label Label11 
         Caption         =   "Record"
         Height          =   255
         Left            =   2400
         TabIndex        =   8
         Top             =   240
         Width           =   615
      End
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   3450
      Left            =   75
      TabIndex        =   18
      Top             =   0
      Width           =   4695
      _ExtentX        =   8281
      _ExtentY        =   6085
      _Version        =   393216
      Tabs            =   12
      Tab             =   8
      TabsPerRow      =   6
      TabHeight       =   520
      BackColor       =   -2147483648
      TabCaption(0)   =   "&General"
      TabPicture(0)   =   "REFview.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "cboAlgorithm"
      Tab(0).Control(1)=   "chkPlotZg"
      Tab(0).Control(2)=   "Frame(0)"
      Tab(0).Control(3)=   "Frame2"
      Tab(0).Control(4)=   "Frame3"
      Tab(0).Control(5)=   "txtREFfile"
      Tab(0).Control(6)=   "chkOK"
      Tab(0).Control(7)=   "ProgressBar2"
      Tab(0).Control(8)=   "Label47"
      Tab(0).Control(9)=   "Label48"
      Tab(0).Control(10)=   "Label49"
      Tab(0).ControlCount=   11
      TabCaption(1)   =   "&Temps"
      TabPicture(1)   =   "REFview.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame5"
      Tab(1).Control(1)=   "Frame13"
      Tab(1).ControlCount=   2
      TabCaption(2)   =   "&Gains"
      TabPicture(2)   =   "REFview.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "txtMux(24)"
      Tab(2).Control(1)=   "txtRux(23)"
      Tab(2).Control(2)=   "txtRux(22)"
      Tab(2).Control(3)=   "txtRux(21)"
      Tab(2).Control(4)=   "txtRux(20)"
      Tab(2).Control(5)=   "txtRux(19)"
      Tab(2).Control(6)=   "txtRux(18)"
      Tab(2).Control(7)=   "txtRux(17)"
      Tab(2).Control(8)=   "txtRux(16)"
      Tab(2).Control(9)=   "txtMux(23)"
      Tab(2).Control(10)=   "txtMux(22)"
      Tab(2).Control(11)=   "txtMux(21)"
      Tab(2).Control(12)=   "txtMux(20)"
      Tab(2).Control(13)=   "txtMux(19)"
      Tab(2).Control(14)=   "txtMux(18)"
      Tab(2).Control(15)=   "txtMux(17)"
      Tab(2).Control(16)=   "Frame7"
      Tab(2).ControlCount=   17
      TabCaption(3)   =   "&Flags"
      TabPicture(3)   =   "REFview.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "Frame12"
      Tab(3).Control(1)=   "Frame10"
      Tab(3).Control(2)=   "Frame11"
      Tab(3).Control(3)=   "chkSelectOnlyGood"
      Tab(3).Control(4)=   "chkEnableFit"
      Tab(3).ControlCount=   5
      TabCaption(4)   =   "Trops"
      TabPicture(4)   =   "REFview.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Label9"
      Tab(4).Control(1)=   "Label10"
      Tab(4).Control(2)=   "Label19"
      Tab(4).Control(3)=   "Label18"
      Tab(4).Control(4)=   "Label17"
      Tab(4).Control(5)=   "Label16"
      Tab(4).Control(6)=   "Label15"
      Tab(4).Control(7)=   "Label14"
      Tab(4).Control(8)=   "Label13"
      Tab(4).Control(9)=   "Label58"
      Tab(4).Control(10)=   "Label59"
      Tab(4).Control(11)=   "Label60"
      Tab(4).Control(12)=   "Label(4)"
      Tab(4).Control(13)=   "Label(5)"
      Tab(4).Control(14)=   "Label(6)"
      Tab(4).Control(15)=   "txtMRI"
      Tab(4).Control(16)=   "txtLR"
      Tab(4).Control(17)=   "cmbTh"
      Tab(4).Control(18)=   "txtTh2"
      Tab(4).Control(19)=   "txtTh1"
      Tab(4).Control(20)=   "txtTt2"
      Tab(4).Control(21)=   "txtTt1"
      Tab(4).Control(22)=   "cmbTt"
      Tab(4).Control(23)=   "cmbZt"
      Tab(4).Control(24)=   "txtZt2"
      Tab(4).Control(25)=   "txtZt1"
      Tab(4).Control(26)=   "txtNlev"
      Tab(4).Control(27)=   "txtNlev1"
      Tab(4).Control(28)=   "txtNlev2"
      Tab(4).Control(29)=   "txtALTac"
      Tab(4).Control(30)=   "txtTac"
      Tab(4).Control(31)=   "txtThAC"
      Tab(4).Control(32)=   "txtOATnav"
      Tab(4).Control(33)=   "txtOATmms"
      Tab(4).Control(34)=   "txtThcp"
      Tab(4).Control(35)=   "txtTcp"
      Tab(4).Control(36)=   "txtZcp"
      Tab(4).ControlCount=   37
      TabCaption(5)   =   "E&xtract"
      TabPicture(5)   =   "REFview.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "cmdWriteParms(0)"
      Tab(5).Control(1)=   "Frame8"
      Tab(5).Control(2)=   "cmdWriteParms(1)"
      Tab(5).ControlCount=   3
      TabCaption(6)   =   "&Counts"
      TabPicture(6)   =   "REFview.frx":04EA
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "Label20"
      Tab(6).Control(1)=   "Label46"
      Tab(6).Control(2)=   "Label45"
      Tab(6).Control(3)=   "txtCounts(11)"
      Tab(6).Control(4)=   "txtCounts(10)"
      Tab(6).Control(5)=   "txtCounts(9)"
      Tab(6).Control(6)=   "txtCounts(8)"
      Tab(6).Control(7)=   "txtCounts(7)"
      Tab(6).Control(8)=   "txtCounts(6)"
      Tab(6).Control(9)=   "txtCounts(5)"
      Tab(6).Control(10)=   "txtCounts(4)"
      Tab(6).Control(11)=   "txtCounts(3)"
      Tab(6).Control(12)=   "txtCounts(2)"
      Tab(6).Control(13)=   "txtCounts(1)"
      Tab(6).Control(14)=   "txtCounts(0)"
      Tab(6).Control(15)=   "txtCounts(12)"
      Tab(6).Control(16)=   "txtCounts(13)"
      Tab(6).Control(17)=   "txtCounts(14)"
      Tab(6).Control(18)=   "txtCounts(15)"
      Tab(6).Control(19)=   "txtCounts(16)"
      Tab(6).Control(20)=   "txtCounts(17)"
      Tab(6).Control(21)=   "txtCounts(18)"
      Tab(6).Control(22)=   "txtCounts(19)"
      Tab(6).Control(23)=   "txtCounts(20)"
      Tab(6).Control(24)=   "txtCounts(21)"
      Tab(6).Control(25)=   "txtCounts(22)"
      Tab(6).Control(26)=   "txtCounts(23)"
      Tab(6).Control(27)=   "txtCounts(24)"
      Tab(6).Control(28)=   "txtCounts(25)"
      Tab(6).Control(29)=   "txtCounts(26)"
      Tab(6).Control(30)=   "txtCounts(27)"
      Tab(6).Control(31)=   "txtCounts(28)"
      Tab(6).Control(32)=   "txtCounts(29)"
      Tab(6).Control(33)=   "txtCounts(30)"
      Tab(6).Control(34)=   "txtCounts(31)"
      Tab(6).Control(35)=   "txtCounts(32)"
      Tab(6).Control(36)=   "txtCounts(33)"
      Tab(6).Control(37)=   "txtCounts(34)"
      Tab(6).Control(38)=   "txtCounts(35)"
      Tab(6).ControlCount=   39
      TabCaption(7)   =   "&Words"
      TabPicture(7)   =   "REFview.frx":0506
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "Frame9"
      Tab(7).Control(1)=   "Frame6"
      Tab(7).ControlCount=   2
      TabCaption(8)   =   "TBs"
      TabPicture(8)   =   "REFview.frx":0522
      Tab(8).ControlEnabled=   -1  'True
      Tab(8).Control(0)=   "Label55"
      Tab(8).Control(0).Enabled=   0   'False
      Tab(8).Control(1)=   "Label54"
      Tab(8).Control(1).Enabled=   0   'False
      Tab(8).Control(2)=   "Label53"
      Tab(8).Control(2).Enabled=   0   'False
      Tab(8).Control(3)=   "lblAlgorithm"
      Tab(8).Control(3).Enabled=   0   'False
      Tab(8).Control(4)=   "txtTA(10)"
      Tab(8).Control(4).Enabled=   0   'False
      Tab(8).Control(5)=   "txtTA(11)"
      Tab(8).Control(5).Enabled=   0   'False
      Tab(8).Control(6)=   "txtTA(12)"
      Tab(8).Control(6).Enabled=   0   'False
      Tab(8).Control(7)=   "txtTA(13)"
      Tab(8).Control(7).Enabled=   0   'False
      Tab(8).Control(8)=   "txtTA(14)"
      Tab(8).Control(8).Enabled=   0   'False
      Tab(8).Control(9)=   "txtTA(15)"
      Tab(8).Control(9).Enabled=   0   'False
      Tab(8).Control(10)=   "txtTA(16)"
      Tab(8).Control(10).Enabled=   0   'False
      Tab(8).Control(11)=   "txtTA(17)"
      Tab(8).Control(11).Enabled=   0   'False
      Tab(8).Control(12)=   "txtTA(18)"
      Tab(8).Control(12).Enabled=   0   'False
      Tab(8).Control(13)=   "txtTA(19)"
      Tab(8).Control(13).Enabled=   0   'False
      Tab(8).Control(14)=   "txtTA(20)"
      Tab(8).Control(14).Enabled=   0   'False
      Tab(8).Control(15)=   "txtTA(21)"
      Tab(8).Control(15).Enabled=   0   'False
      Tab(8).Control(16)=   "txtTA(22)"
      Tab(8).Control(16).Enabled=   0   'False
      Tab(8).Control(17)=   "txtTA(23)"
      Tab(8).Control(17).Enabled=   0   'False
      Tab(8).Control(18)=   "txtTA(24)"
      Tab(8).Control(18).Enabled=   0   'False
      Tab(8).Control(19)=   "txtTA(25)"
      Tab(8).Control(19).Enabled=   0   'False
      Tab(8).Control(20)=   "txtTA(26)"
      Tab(8).Control(20).Enabled=   0   'False
      Tab(8).Control(21)=   "txtTA(27)"
      Tab(8).Control(21).Enabled=   0   'False
      Tab(8).Control(22)=   "txtTA(28)"
      Tab(8).Control(22).Enabled=   0   'False
      Tab(8).Control(23)=   "txtTA(29)"
      Tab(8).Control(23).Enabled=   0   'False
      Tab(8).Control(24)=   "picTA"
      Tab(8).Control(24).Enabled=   0   'False
      Tab(8).Control(25)=   "txtTC"
      Tab(8).Control(25).Enabled=   0   'False
      Tab(8).Control(26)=   "txtT1"
      Tab(8).Control(26).Enabled=   0   'False
      Tab(8).Control(27)=   "txtT2"
      Tab(8).Control(27).Enabled=   0   'False
      Tab(8).Control(28)=   "cmdAutoScaleTA"
      Tab(8).Control(28).Enabled=   0   'False
      Tab(8).Control(29)=   "txtTA(0)"
      Tab(8).Control(29).Enabled=   0   'False
      Tab(8).Control(30)=   "txtTA(1)"
      Tab(8).Control(30).Enabled=   0   'False
      Tab(8).Control(31)=   "txtTA(2)"
      Tab(8).Control(31).Enabled=   0   'False
      Tab(8).Control(32)=   "txtTA(3)"
      Tab(8).Control(32).Enabled=   0   'False
      Tab(8).Control(33)=   "txtTA(4)"
      Tab(8).Control(33).Enabled=   0   'False
      Tab(8).Control(34)=   "txtTA(5)"
      Tab(8).Control(34).Enabled=   0   'False
      Tab(8).Control(35)=   "txtTA(6)"
      Tab(8).Control(35).Enabled=   0   'False
      Tab(8).Control(36)=   "txtTA(7)"
      Tab(8).Control(36).Enabled=   0   'False
      Tab(8).Control(37)=   "txtTA(8)"
      Tab(8).Control(37).Enabled=   0   'False
      Tab(8).Control(38)=   "txtTA(9)"
      Tab(8).Control(38).Enabled=   0   'False
      Tab(8).Control(39)=   "chkShowAAObs"
      Tab(8).Control(39).Enabled=   0   'False
      Tab(8).Control(40)=   "txtRCused2(1)"
      Tab(8).Control(40).Enabled=   0   'False
      Tab(8).Control(41)=   "txtRCused1(1)"
      Tab(8).Control(41).Enabled=   0   'False
      Tab(8).ControlCount=   42
      TabCaption(9)   =   "&ATP"
      TabPicture(9)   =   "REFview.frx":053E
      Tab(9).ControlEnabled=   0   'False
      Tab(9).Control(0)=   "lblLvl(7)"
      Tab(9).Control(1)=   "lblLvl(6)"
      Tab(9).Control(2)=   "lblLvl(4)"
      Tab(9).Control(3)=   "lblLvl(3)"
      Tab(9).Control(4)=   "lblLvl(2)"
      Tab(9).Control(5)=   "lblLvl(1)"
      Tab(9).Control(6)=   "lblLvl(0)"
      Tab(9).Control(7)=   "lblLvl(9)"
      Tab(9).Control(8)=   "lblLvl(10)"
      Tab(9).Control(9)=   "lblLvl(11)"
      Tab(9).Control(10)=   "lblLvl(12)"
      Tab(9).Control(11)=   "lblLvl(13)"
      Tab(9).Control(12)=   "lblLvl(14)"
      Tab(9).Control(13)=   "lblLvl(15)"
      Tab(9).Control(14)=   "lblLvl(16)"
      Tab(9).Control(15)=   "lblLvl(17)"
      Tab(9).Control(16)=   "lblLvl(18)"
      Tab(9).Control(17)=   "lblLvl(19)"
      Tab(9).Control(18)=   "lblLvl(20)"
      Tab(9).Control(19)=   "lblLvl(21)"
      Tab(9).Control(20)=   "lblLvl(22)"
      Tab(9).Control(21)=   "lblLvl(23)"
      Tab(9).Control(22)=   "lblLvl(24)"
      Tab(9).Control(23)=   "lblLvl(25)"
      Tab(9).Control(24)=   "lblLvl(26)"
      Tab(9).Control(25)=   "lblLvl(27)"
      Tab(9).Control(26)=   "lblLvl(28)"
      Tab(9).Control(27)=   "lblLvl(29)"
      Tab(9).Control(28)=   "lblLvl(30)"
      Tab(9).Control(29)=   "lblLvl(31)"
      Tab(9).Control(30)=   "lblLvl(32)"
      Tab(9).Control(31)=   "lblLvl(33)"
      Tab(9).Control(32)=   "lblLvl(34)"
      Tab(9).Control(33)=   "txtZZZ(0)"
      Tab(9).Control(34)=   "txtTTT(0)"
      Tab(9).Control(35)=   "txtTTT(11)"
      Tab(9).Control(36)=   "txtZZZ(11)"
      Tab(9).Control(37)=   "txtTTT(12)"
      Tab(9).Control(38)=   "txtZZZ(12)"
      Tab(9).Control(39)=   "txtTTT(13)"
      Tab(9).Control(40)=   "txtZZZ(13)"
      Tab(9).Control(41)=   "txtTTT(14)"
      Tab(9).Control(42)=   "txtZZZ(14)"
      Tab(9).Control(43)=   "txtTTT(15)"
      Tab(9).Control(44)=   "txtZZZ(15)"
      Tab(9).Control(45)=   "txtTTT(16)"
      Tab(9).Control(46)=   "txtZZZ(16)"
      Tab(9).Control(47)=   "txtTTT(17)"
      Tab(9).Control(48)=   "txtZZZ(17)"
      Tab(9).Control(49)=   "txtTTT(18)"
      Tab(9).Control(50)=   "txtZZZ(18)"
      Tab(9).Control(51)=   "txtTTT(19)"
      Tab(9).Control(52)=   "txtZZZ(19)"
      Tab(9).Control(53)=   "txtTTT(20)"
      Tab(9).Control(54)=   "txtZZZ(20)"
      Tab(9).Control(55)=   "txtTTT(21)"
      Tab(9).Control(56)=   "txtZZZ(21)"
      Tab(9).Control(57)=   "txtTTT(22)"
      Tab(9).Control(58)=   "txtZZZ(22)"
      Tab(9).Control(59)=   "txtTTT(23)"
      Tab(9).Control(60)=   "txtZZZ(23)"
      Tab(9).Control(61)=   "txtTTT(24)"
      Tab(9).Control(62)=   "txtZZZ(24)"
      Tab(9).Control(63)=   "txtTTT(25)"
      Tab(9).Control(64)=   "txtZZZ(25)"
      Tab(9).Control(65)=   "txtTTT(26)"
      Tab(9).Control(66)=   "txtZZZ(26)"
      Tab(9).Control(67)=   "txtTTT(27)"
      Tab(9).Control(68)=   "txtZZZ(27)"
      Tab(9).Control(69)=   "txtTTT(28)"
      Tab(9).Control(70)=   "txtZZZ(28)"
      Tab(9).Control(71)=   "txtTTT(29)"
      Tab(9).Control(72)=   "txtZZZ(29)"
      Tab(9).Control(73)=   "txtTTT(30)"
      Tab(9).Control(74)=   "txtZZZ(30)"
      Tab(9).Control(75)=   "txtTTT(31)"
      Tab(9).Control(76)=   "txtZZZ(31)"
      Tab(9).Control(77)=   "txtTTT(32)"
      Tab(9).Control(78)=   "txtZZZ(32)"
      Tab(9).Control(79)=   "txtTTT(1)"
      Tab(9).Control(80)=   "txtZZZ(1)"
      Tab(9).Control(81)=   "txtTTT(2)"
      Tab(9).Control(82)=   "txtZZZ(2)"
      Tab(9).Control(83)=   "txtTTT(3)"
      Tab(9).Control(84)=   "txtZZZ(3)"
      Tab(9).Control(85)=   "txtTTT(4)"
      Tab(9).Control(86)=   "txtZZZ(4)"
      Tab(9).Control(87)=   "txtTTT(5)"
      Tab(9).Control(88)=   "txtZZZ(5)"
      Tab(9).Control(89)=   "txtTTT(6)"
      Tab(9).Control(90)=   "txtZZZ(6)"
      Tab(9).Control(91)=   "txtTTT(7)"
      Tab(9).Control(92)=   "txtZZZ(7)"
      Tab(9).Control(93)=   "txtTTT(8)"
      Tab(9).Control(94)=   "txtZZZ(8)"
      Tab(9).Control(95)=   "txtTTT(9)"
      Tab(9).Control(96)=   "txtZZZ(9)"
      Tab(9).Control(97)=   "txtTTT(10)"
      Tab(9).Control(98)=   "txtZZZ(10)"
      Tab(9).ControlCount=   99
      TabCaption(10)  =   "A&vg"
      TabPicture(10)  =   "REFview.frx":055A
      Tab(10).ControlEnabled=   0   'False
      Tab(10).Control(0)=   "Label(0)"
      Tab(10).Control(1)=   "Label(1)"
      Tab(10).Control(2)=   "Label(2)"
      Tab(10).Control(3)=   "Label(3)"
      Tab(10).Control(4)=   "txtAvgCycles"
      Tab(10).Control(5)=   "Frame(1)"
      Tab(10).Control(6)=   "txtOutFilename"
      Tab(10).Control(7)=   "lstAvg"
      Tab(10).Control(8)=   "cmdAvg(3)"
      Tab(10).Control(9)=   "txtAvg"
      Tab(10).Control(10)=   "cmdAvg(0)"
      Tab(10).Control(11)=   "cmdAvg(1)"
      Tab(10).Control(12)=   "cmdAvg(2)"
      Tab(10).Control(13)=   "cmdAvg(4)"
      Tab(10).Control(14)=   "cmdAvg(5)"
      Tab(10).Control(15)=   "cmbAvg"
      Tab(10).Control(16)=   "chkMultipleScans"
      Tab(10).Control(17)=   "cmdAvg(6)"
      Tab(10).ControlCount=   18
      TabCaption(11)  =   "&Spare"
      TabPicture(11)  =   "REFview.frx":0576
      Tab(11).ControlEnabled=   0   'False
      Tab(11).ControlCount=   0
      Begin VB.ComboBox cboAlgorithm 
         Height          =   315
         Left            =   -73290
         TabIndex        =   501
         Top             =   2565
         Width           =   1620
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -74925
         MultiLine       =   -1  'True
         TabIndex        =   498
         Top             =   2850
         Width           =   495
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -74370
         MultiLine       =   -1  'True
         TabIndex        =   497
         Top             =   2850
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -74385
         MultiLine       =   -1  'True
         TabIndex        =   496
         Top             =   2535
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -74385
         MultiLine       =   -1  'True
         TabIndex        =   495
         Top             =   2235
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -74385
         MultiLine       =   -1  'True
         TabIndex        =   494
         Top             =   1935
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74370
         MultiLine       =   -1  'True
         TabIndex        =   493
         Top             =   1620
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74370
         MultiLine       =   -1  'True
         TabIndex        =   492
         Top             =   1305
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -74370
         MultiLine       =   -1  'True
         TabIndex        =   491
         Top             =   1005
         Width           =   615
      End
      Begin VB.TextBox txtRux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -74355
         MultiLine       =   -1  'True
         TabIndex        =   490
         Top             =   690
         Width           =   615
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -74940
         MultiLine       =   -1  'True
         TabIndex        =   489
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -74925
         MultiLine       =   -1  'True
         TabIndex        =   488
         Top             =   2250
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -74925
         MultiLine       =   -1  'True
         TabIndex        =   487
         Top             =   1950
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -74910
         MultiLine       =   -1  'True
         TabIndex        =   486
         Top             =   1620
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74910
         MultiLine       =   -1  'True
         TabIndex        =   485
         Top             =   1305
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74910
         MultiLine       =   -1  'True
         TabIndex        =   484
         Top             =   975
         Width           =   495
      End
      Begin VB.TextBox txtMux 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -74895
         MultiLine       =   -1  'True
         TabIndex        =   483
         Top             =   675
         Width           =   495
      End
      Begin VB.TextBox txtZcp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   481
         Top             =   1860
         Width           =   735
      End
      Begin VB.TextBox txtTcp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   480
         Top             =   1860
         Width           =   615
      End
      Begin VB.TextBox txtThcp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71280
         MultiLine       =   -1  'True
         TabIndex        =   479
         Top             =   1860
         Width           =   615
      End
      Begin VB.TextBox txtRCused1 
         Height          =   285
         Index           =   1
         Left            =   3075
         TabIndex        =   478
         Text            =   "SA012"
         Top             =   3120
         Width           =   690
      End
      Begin VB.TextBox txtRCused2 
         Height          =   285
         Index           =   1
         Left            =   3810
         TabIndex        =   477
         Text            =   "SA012"
         Top             =   3120
         Width           =   675
      End
      Begin VB.CheckBox chkShowAAObs 
         Caption         =   "AA Obs"
         Height          =   195
         Left            =   2190
         TabIndex        =   476
         Top             =   3150
         Value           =   1  'Checked
         Width           =   840
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   474
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   473
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   472
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   471
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   470
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   469
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   468
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   467
         Top             =   1440
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   466
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   90
         MultiLine       =   -1  'True
         TabIndex        =   465
         Top             =   960
         Width           =   615
      End
      Begin VB.CommandButton cmdAutoScaleTA 
         Caption         =   "M"
         Height          =   315
         Left            =   2655
         TabIndex        =   464
         ToolTipText     =   "Auto (A) or Manual (M) Scale"
         Top             =   2745
         Width           =   270
      End
      Begin VB.TextBox txtT2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   4110
         MultiLine       =   -1  'True
         TabIndex        =   463
         Top             =   2760
         Width           =   375
      End
      Begin VB.TextBox txtT1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2190
         MultiLine       =   -1  'True
         TabIndex        =   462
         Top             =   2730
         Width           =   375
      End
      Begin VB.TextBox txtTC 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   2940
         MultiLine       =   -1  'True
         TabIndex        =   461
         Top             =   2745
         Width           =   375
      End
      Begin VB.PictureBox picTA 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00808080&
         Height          =   2055
         Left            =   2190
         ScaleHeight     =   1995
         ScaleWidth      =   2235
         TabIndex        =   460
         Top             =   690
         Width           =   2295
      End
      Begin VB.CheckBox chkPlotZg 
         BackColor       =   &H80000000&
         Caption         =   "Plot Zg"
         Height          =   285
         Left            =   -74160
         TabIndex        =   459
         Top             =   2580
         Width           =   840
      End
      Begin VB.Frame Frame 
         Caption         =   "RCs Used"
         Height          =   885
         Index           =   0
         Left            =   -71610
         TabIndex        =   453
         Top             =   1920
         Width           =   1035
         Begin VB.TextBox txtRCused2 
            Height          =   285
            Index           =   0
            Left            =   150
            TabIndex        =   455
            Text            =   "SA012"
            Top             =   510
            Width           =   735
         End
         Begin VB.TextBox txtRCused1 
            Height          =   285
            Index           =   0
            Left            =   150
            TabIndex        =   454
            Text            =   "SA012"
            Top             =   210
            Width           =   735
         End
      End
      Begin VB.TextBox txtOATmms 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   452
         Top             =   2820
         Width           =   615
      End
      Begin VB.TextBox txtOATnav 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   451
         Top             =   2505
         Width           =   615
      End
      Begin VB.CheckBox chkEnableFit 
         Caption         =   "Enable Spline Fit"
         Height          =   255
         Left            =   -74760
         TabIndex        =   448
         Top             =   2640
         Value           =   1  'Checked
         Width           =   1935
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   442
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   441
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   440
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   439
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   438
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   437
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   436
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   435
         Top             =   1440
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   434
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   1440
         MultiLine       =   -1  'True
         TabIndex        =   433
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   432
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   431
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   430
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   429
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   428
         Top             =   2160
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   427
         Top             =   1920
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   426
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   425
         Top             =   1440
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   424
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtTA 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   780
         MultiLine       =   -1  'True
         TabIndex        =   423
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   35
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   422
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   34
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   421
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   33
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   420
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   419
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   418
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   417
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   416
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   415
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   414
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   413
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   412
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   411
         Top             =   2640
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   410
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   409
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   408
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   407
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   406
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   405
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   404
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   403
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   402
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   401
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   400
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   399
         Top             =   1800
         Width           =   615
      End
      Begin VB.CommandButton cmdWriteParms 
         Caption         =   "Write &Default Parms"
         Height          =   375
         Index           =   1
         Left            =   -74880
         TabIndex        =   398
         Top             =   3000
         Width           =   2055
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Clear Plot"
         Height          =   375
         Index           =   6
         Left            =   -73080
         TabIndex        =   397
         Top             =   1200
         Width           =   855
      End
      Begin VB.CheckBox chkMultipleScans 
         Caption         =   "Plot Multiple Scans"
         Height          =   255
         Left            =   -74880
         TabIndex        =   396
         Top             =   1200
         Width           =   1695
      End
      Begin VB.ComboBox cmbAvg 
         Height          =   315
         Left            =   -73320
         TabIndex        =   393
         Text            =   "seconds"
         Top             =   2640
         Width           =   1095
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Write TA and ATP"
         Height          =   375
         Index           =   5
         Left            =   -72120
         TabIndex        =   392
         Top             =   2640
         Width           =   1575
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Save"
         Height          =   375
         Index           =   4
         Left            =   -72120
         TabIndex        =   391
         ToolTipText     =   "Write List Items to Mission.events"
         Top             =   2160
         Width           =   735
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Clear"
         Height          =   375
         Index           =   2
         Left            =   -72120
         TabIndex        =   390
         Top             =   720
         Width           =   735
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Remove"
         Height          =   375
         Index           =   1
         Left            =   -72120
         TabIndex        =   389
         Top             =   1680
         Width           =   735
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Add"
         Height          =   375
         Index           =   0
         Left            =   -72120
         TabIndex        =   388
         Top             =   1200
         Width           =   735
      End
      Begin VB.TextBox txtAvg 
         Height          =   285
         Left            =   -71280
         TabIndex        =   387
         Top             =   1200
         Width           =   735
      End
      Begin VB.CommandButton cmdAvg 
         Caption         =   "&Import"
         Height          =   375
         Index           =   3
         Left            =   -71280
         TabIndex        =   386
         ToolTipText     =   "Import List Items"
         Top             =   720
         Width           =   735
      End
      Begin VB.ListBox lstAvg 
         Height          =   1035
         Left            =   -71280
         TabIndex        =   385
         Top             =   1560
         Width           =   735
      End
      Begin VB.TextBox txtOutFilename 
         Height          =   285
         Left            =   -73800
         TabIndex        =   384
         Top             =   3120
         Width           =   3255
      End
      Begin VB.Frame Frame 
         Caption         =   "Output Format"
         Height          =   615
         Index           =   1
         Left            =   -74880
         TabIndex        =   380
         Top             =   2400
         Width           =   1455
         Begin VB.OptionButton optOutFormat 
            Caption         =   "REF"
            Height          =   255
            Index           =   2
            Left            =   720
            TabIndex        =   382
            Top             =   240
            Width           =   615
         End
         Begin VB.OptionButton optOutFormat 
            Caption         =   "Text"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   381
            Top             =   240
            Width           =   615
         End
      End
      Begin VB.TextBox txtAvgCycles 
         Alignment       =   1  'Right Justify
         BackColor       =   &H00FFFFFF&
         Height          =   285
         Left            =   -74160
         MultiLine       =   -1  'True
         TabIndex        =   379
         ToolTipText     =   "Number of Cycles to Average from Current Position"
         Top             =   720
         Width           =   375
      End
      Begin VB.TextBox txtThAC 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71280
         MultiLine       =   -1  'True
         TabIndex        =   366
         Top             =   2175
         Width           =   615
      End
      Begin VB.TextBox txtTac 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   365
         Top             =   2175
         Width           =   615
      End
      Begin VB.TextBox txtALTac 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   364
         Top             =   2175
         Width           =   735
      End
      Begin VB.TextBox txtNlev2 
         Height          =   285
         Left            =   -74400
         TabIndex        =   362
         Top             =   2820
         Width           =   375
      End
      Begin VB.TextBox txtNlev1 
         Height          =   285
         Left            =   -74400
         TabIndex        =   360
         Top             =   2505
         Width           =   375
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   358
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   357
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   356
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   355
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   354
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   353
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   352
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   351
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   350
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   349
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   348
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   347
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   346
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   345
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   344
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   343
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   342
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   341
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   340
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   339
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   338
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   32
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   337
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   336
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   31
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   335
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   334
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   30
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   333
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   332
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   331
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   330
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   329
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   328
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   327
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   326
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   325
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   324
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   323
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   322
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   321
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   320
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   319
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -71640
         MultiLine       =   -1  'True
         TabIndex        =   318
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -71040
         MultiLine       =   -1  'True
         TabIndex        =   317
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   316
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   315
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   314
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   313
         Top             =   2880
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   312
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   311
         Top             =   2640
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   310
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   309
         Top             =   2400
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   308
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   307
         Top             =   2160
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   306
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   305
         Top             =   1920
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   304
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   303
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   302
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   301
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   300
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   299
         Top             =   1200
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   298
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   297
         Top             =   960
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -73080
         MultiLine       =   -1  'True
         TabIndex        =   296
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   295
         Top             =   720
         Width           =   495
      End
      Begin VB.Frame Frame2 
         Caption         =   "Location Info"
         Height          =   1215
         Left            =   -74880
         TabIndex        =   224
         Top             =   690
         Width           =   4335
         Begin VB.TextBox txtALT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   235
            Top             =   480
            Width           =   615
         End
         Begin VB.ComboBox cmbALT 
            Height          =   315
            Left            =   3600
            TabIndex        =   234
            Text            =   "km"
            Top             =   480
            Width           =   615
         End
         Begin VB.TextBox txtLatitude 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   233
            Top             =   240
            Width           =   855
         End
         Begin VB.TextBox txtLongitude 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   232
            Top             =   480
            Width           =   855
         End
         Begin VB.TextBox txtOAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   231
            Top             =   240
            Width           =   615
         End
         Begin VB.ComboBox cmbOAT 
            Height          =   315
            Left            =   3600
            TabIndex        =   230
            Text            =   "K"
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtPitch 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   229
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtRoll 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3720
            MultiLine       =   -1  'True
            TabIndex        =   228
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtHdg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   227
            Top             =   840
            Width           =   495
         End
         Begin VB.ComboBox cmbLat 
            Height          =   315
            Left            =   1680
            TabIndex        =   226
            Text            =   "deg"
            Top             =   240
            Width           =   735
         End
         Begin VB.ComboBox cmbLon 
            Height          =   315
            Left            =   1680
            TabIndex        =   225
            Text            =   "deg"
            Top             =   480
            Width           =   735
         End
         Begin VB.Label Label1 
            Caption         =   "pALT"
            Height          =   255
            Left            =   2520
            TabIndex        =   242
            Top             =   600
            Width           =   375
         End
         Begin VB.Label label2 
            Caption         =   "Latitude"
            Height          =   255
            Left            =   120
            TabIndex        =   241
            Top             =   240
            Width           =   615
         End
         Begin VB.Label Label3 
            Caption         =   "Longitude"
            Height          =   255
            Left            =   120
            TabIndex        =   240
            Top             =   480
            Width           =   735
         End
         Begin VB.Label Label4 
            Caption         =   "OAT"
            Height          =   255
            Left            =   2520
            TabIndex        =   239
            Top             =   240
            Width           =   495
         End
         Begin VB.Label Label5 
            Caption         =   "Pitch [d]"
            Height          =   255
            Left            =   1680
            TabIndex        =   238
            Top             =   840
            Width           =   615
         End
         Begin VB.Label Label6 
            Caption         =   "Roll [d]"
            Height          =   255
            Left            =   3120
            TabIndex        =   237
            Top             =   840
            Width           =   735
         End
         Begin VB.Label Label12 
            Caption         =   "Heading [d]"
            Height          =   255
            Left            =   120
            TabIndex        =   236
            Top             =   840
            Width           =   855
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "Environment Info"
         Height          =   615
         Left            =   -74880
         TabIndex        =   218
         Top             =   1920
         Width           =   3225
         Begin VB.TextBox txtWdir 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   221
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtWspd 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   220
            Top             =   240
            Width           =   495
         End
         Begin VB.ComboBox cmbSpeed 
            Height          =   315
            Left            =   2280
            TabIndex        =   219
            Text            =   "knots"
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label7 
            Caption         =   "Wind from"
            Height          =   255
            Left            =   120
            TabIndex        =   223
            Top             =   240
            Width           =   735
         End
         Begin VB.Label Label8 
            Caption         =   "at"
            Height          =   255
            Left            =   1560
            TabIndex        =   222
            Top             =   240
            Width           =   255
         End
      End
      Begin VB.TextBox txtNlev 
         Height          =   285
         Left            =   -74400
         TabIndex        =   217
         Top             =   3120
         Width           =   375
      End
      Begin VB.TextBox txtZt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   216
         Top             =   1230
         Width           =   735
      End
      Begin VB.TextBox txtZt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   215
         Top             =   1545
         Width           =   735
      End
      Begin VB.ComboBox cmbZt 
         Height          =   315
         Left            =   -73320
         TabIndex        =   214
         Top             =   885
         Width           =   735
      End
      Begin VB.ComboBox cmbTt 
         Height          =   315
         Left            =   -72240
         TabIndex        =   213
         Top             =   885
         Width           =   615
      End
      Begin VB.TextBox txtTt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   212
         Top             =   1230
         Width           =   615
      End
      Begin VB.TextBox txtTt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72240
         MultiLine       =   -1  'True
         TabIndex        =   211
         Top             =   1545
         Width           =   615
      End
      Begin VB.TextBox txtTh1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71280
         MultiLine       =   -1  'True
         TabIndex        =   210
         Top             =   1230
         Width           =   615
      End
      Begin VB.TextBox txtTh2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71280
         MultiLine       =   -1  'True
         TabIndex        =   209
         Top             =   1545
         Width           =   615
      End
      Begin VB.ComboBox cmbTh 
         Height          =   315
         Left            =   -71280
         TabIndex        =   208
         Top             =   885
         Width           =   615
      End
      Begin VB.Frame Frame5 
         Caption         =   "Temperatures"
         Height          =   2655
         Left            =   -73080
         TabIndex        =   173
         Top             =   720
         Width           =   2535
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   190
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   189
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   188
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   187
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   186
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   185
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   184
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   183
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   182
            Top             =   2280
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   181
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   180
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   179
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   178
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   177
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   176
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   175
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   174
            Top             =   2280
            Width           =   495
         End
         Begin VB.Label Label21 
            Caption         =   "Target"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   207
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label22 
            Caption         =   "LO1"
            Height          =   255
            Left            =   120
            TabIndex        =   206
            Top             =   840
            Width           =   495
         End
         Begin VB.Label Label23 
            Caption         =   "LO2"
            Height          =   255
            Left            =   120
            TabIndex        =   205
            Top             =   1080
            Width           =   495
         End
         Begin VB.Label Label24 
            Caption         =   "IF Amp"
            Height          =   255
            Left            =   120
            TabIndex        =   204
            Top             =   1320
            Width           =   615
         End
         Begin VB.Label Label25 
            Caption         =   "Tgt Lo"
            Height          =   255
            Left            =   120
            TabIndex        =   203
            Top             =   1560
            Width           =   495
         End
         Begin VB.Label Label26 
            Caption         =   "Diode"
            Height          =   255
            Left            =   120
            TabIndex        =   202
            Top             =   600
            Width           =   615
         End
         Begin VB.Label Label27 
            Caption         =   "Tgt Hi"
            Height          =   255
            Left            =   120
            TabIndex        =   201
            Top             =   1800
            Width           =   615
         End
         Begin VB.Label Label28 
            Caption         =   "Acc(+)"
            Height          =   255
            Left            =   120
            TabIndex        =   200
            Top             =   2040
            Width           =   615
         End
         Begin VB.Label Label29 
            Caption         =   "Window"
            Height          =   255
            Left            =   120
            TabIndex        =   199
            Top             =   2280
            Width           =   615
         End
         Begin VB.Label Label30 
            Caption         =   "Motor"
            Height          =   255
            Left            =   1320
            TabIndex        =   198
            Top             =   600
            Width           =   615
         End
         Begin VB.Label Label31 
            BackColor       =   &H00C0C0C0&
            Caption         =   "Spare"
            Height          =   255
            Left            =   1320
            TabIndex        =   197
            Top             =   840
            Width           =   615
         End
         Begin VB.Label Label32 
            Caption         =   "Vref"
            Height          =   255
            Left            =   1320
            TabIndex        =   196
            Top             =   1080
            Width           =   615
         End
         Begin VB.Label Label33 
            Caption         =   "DC 1"
            Height          =   255
            Left            =   1320
            TabIndex        =   195
            Top             =   1320
            Width           =   615
         End
         Begin VB.Label Label34 
            Caption         =   "DC 2"
            Height          =   255
            Left            =   1320
            TabIndex        =   194
            Top             =   1560
            Width           =   615
         End
         Begin VB.Label Label35 
            Caption         =   "PS 5 v"
            Height          =   255
            Left            =   1320
            TabIndex        =   193
            Top             =   1800
            Width           =   495
         End
         Begin VB.Label Label36 
            Caption         =   "PS 12 v"
            Height          =   255
            Left            =   1320
            TabIndex        =   192
            Top             =   2040
            Width           =   615
         End
         Begin VB.Label Label37 
            Caption         =   "Acc (-)"
            Height          =   255
            Left            =   1320
            TabIndex        =   191
            Top             =   2280
            Width           =   615
         End
      End
      Begin VB.Frame Frame7 
         Caption         =   "Gains"
         Height          =   2655
         Left            =   -73560
         TabIndex        =   144
         Top             =   720
         Width           =   3015
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   165
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   164
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   163
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   162
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   161
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   160
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   159
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   158
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   157
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   156
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   155
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   154
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   153
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   152
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   151
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   150
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   149
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   148
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   18
            Left            =   2400
            MultiLine       =   -1  'True
            TabIndex        =   147
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   19
            Left            =   1800
            MultiLine       =   -1  'True
            TabIndex        =   146
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtGain 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   20
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   145
            Top             =   2040
            Width           =   495
         End
         Begin VB.Label Label38 
            Caption         =   "Ch 1"
            Height          =   255
            Left            =   1200
            TabIndex        =   172
            Top             =   120
            Width           =   375
         End
         Begin VB.Label Label39 
            Caption         =   "Ch 2"
            Height          =   255
            Left            =   1800
            TabIndex        =   171
            Top             =   120
            Width           =   495
         End
         Begin VB.Label Label40 
            Caption         =   "Ch 3"
            Height          =   255
            Left            =   2400
            TabIndex        =   170
            Top             =   120
            Width           =   495
         End
         Begin VB.Label Label41 
            Caption         =   "Used"
            Height          =   255
            Left            =   120
            TabIndex        =   169
            Top             =   360
            Width           =   975
         End
         Begin VB.Label Label42 
            Caption         =   "Gain Eqn"
            Height          =   255
            Left            =   120
            TabIndex        =   168
            Top             =   840
            Width           =   975
         End
         Begin VB.Label Label43 
            Caption         =   "Noise Diode"
            Height          =   255
            Left            =   120
            TabIndex        =   167
            Top             =   1080
            Width           =   975
         End
         Begin VB.Label Label44 
            Caption         =   "OAT"
            Height          =   255
            Left            =   120
            TabIndex        =   166
            Top             =   1320
            Width           =   975
         End
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   143
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   142
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   141
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   140
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   139
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   138
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -74760
         MultiLine       =   -1  'True
         TabIndex        =   137
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   136
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   135
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   134
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -71880
         MultiLine       =   -1  'True
         TabIndex        =   133
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtCounts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -71160
         MultiLine       =   -1  'True
         TabIndex        =   132
         Top             =   1200
         Width           =   615
      End
      Begin VB.TextBox txtLR 
         Height          =   285
         Left            =   -71160
         TabIndex        =   131
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtMRI 
         Height          =   285
         Left            =   -73320
         TabIndex        =   130
         Top             =   3120
         Width           =   495
      End
      Begin VB.TextBox txtREFfile 
         Height          =   285
         Left            =   -74160
         TabIndex        =   129
         Top             =   2880
         Width           =   3615
      End
      Begin VB.CheckBox chkOK 
         Alignment       =   1  'Right Justify
         BackColor       =   &H80000000&
         Caption         =   "OK"
         Height          =   375
         Left            =   -74850
         TabIndex        =   128
         Top             =   2520
         Width           =   555
      End
      Begin VB.CheckBox chkSelectOnlyGood 
         Caption         =   "Export Only Good Trops"
         Height          =   255
         Left            =   -74760
         TabIndex        =   127
         Top             =   2880
         Width           =   2055
      End
      Begin VB.Frame Frame8 
         Caption         =   "Select Parameters"
         Height          =   2175
         Left            =   -74880
         TabIndex        =   105
         Top             =   720
         Width           =   4335
         Begin VB.CheckBox chkParm 
            Caption         =   "Tcp"
            Height          =   195
            Index           =   37
            Left            =   3360
            TabIndex        =   500
            Top             =   1605
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Zcp"
            Height          =   195
            Index           =   36
            Left            =   3360
            TabIndex        =   499
            Top             =   1395
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Tmtp3"
            Height          =   195
            Index           =   16
            Left            =   3360
            TabIndex        =   447
            Top             =   1170
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Tmtp2"
            Height          =   195
            Index           =   15
            Left            =   3360
            TabIndex        =   446
            Top             =   960
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Tmtp1"
            Height          =   195
            Index           =   14
            Left            =   3360
            TabIndex        =   445
            Top             =   720
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Th2"
            Height          =   195
            Index           =   13
            Left            =   2520
            TabIndex        =   444
            Top             =   720
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Th1"
            Height          =   195
            Index           =   12
            Left            =   1680
            TabIndex        =   443
            Top             =   720
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Mux"
            Height          =   195
            Index           =   35
            Left            =   1680
            TabIndex        =   377
            Top             =   1920
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CH3"
            Height          =   195
            Index           =   34
            Left            =   960
            TabIndex        =   376
            Top             =   1920
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CH2"
            Height          =   195
            Index           =   33
            Left            =   120
            TabIndex        =   375
            Top             =   1920
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CH1"
            Height          =   195
            Index           =   32
            Left            =   2520
            TabIndex        =   374
            Top             =   1680
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C36"
            Height          =   195
            Index           =   31
            Left            =   1680
            TabIndex        =   373
            Top             =   1680
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C26"
            Height          =   195
            Index           =   30
            Left            =   960
            TabIndex        =   372
            Top             =   1680
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C16"
            Height          =   195
            Index           =   29
            Left            =   120
            TabIndex        =   371
            Top             =   1680
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C35"
            Height          =   195
            Index           =   28
            Left            =   2520
            TabIndex        =   370
            Top             =   1440
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C25"
            Height          =   195
            Index           =   27
            Left            =   1680
            TabIndex        =   369
            Top             =   1440
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "C15"
            Height          =   195
            Index           =   26
            Left            =   960
            TabIndex        =   368
            Top             =   1440
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "ND3"
            Height          =   195
            Index           =   25
            Left            =   120
            TabIndex        =   367
            Top             =   1440
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "UTks"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   126
            Top             =   240
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "pALT"
            Height          =   195
            Index           =   1
            Left            =   960
            TabIndex        =   125
            Top             =   240
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Tmtp"
            Height          =   195
            Index           =   2
            Left            =   1680
            TabIndex        =   124
            Top             =   240
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Tnav"
            Height          =   195
            Index           =   9
            Left            =   3360
            TabIndex        =   123
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Zt1"
            Height          =   195
            Index           =   10
            Left            =   120
            TabIndex        =   122
            Top             =   720
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Lat"
            Height          =   195
            Index           =   3
            Left            =   2520
            TabIndex        =   121
            Top             =   240
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Lon"
            Height          =   195
            Index           =   4
            Left            =   3360
            TabIndex        =   120
            Top             =   240
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Pitch"
            Height          =   195
            Index           =   5
            Left            =   120
            TabIndex        =   119
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Roll"
            Height          =   195
            Index           =   6
            Left            =   960
            TabIndex        =   118
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Hdg"
            Height          =   195
            Index           =   7
            Left            =   1680
            TabIndex        =   117
            Top             =   480
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "LRac"
            Height          =   195
            Index           =   8
            Left            =   2520
            TabIndex        =   116
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Zt2"
            Height          =   195
            Index           =   11
            Left            =   960
            TabIndex        =   115
            Top             =   720
            Width           =   615
         End
         Begin VB.CommandButton cmdParmsAll 
            Caption         =   "Set All"
            Height          =   255
            Left            =   3345
            TabIndex        =   114
            Top             =   1875
            Width           =   855
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Gain1"
            Height          =   195
            Index           =   17
            Left            =   120
            TabIndex        =   113
            Top             =   960
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CB3"
            Height          =   195
            Index           =   22
            Left            =   960
            TabIndex        =   112
            Top             =   1200
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "ND1"
            Height          =   195
            Index           =   23
            Left            =   1680
            TabIndex        =   111
            Top             =   1200
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "ND2"
            Height          =   195
            Index           =   24
            Left            =   2520
            TabIndex        =   110
            Top             =   1200
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Gain2"
            Height          =   195
            Index           =   18
            Left            =   960
            TabIndex        =   109
            Top             =   960
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "Gain3"
            Height          =   195
            Index           =   19
            Left            =   1680
            TabIndex        =   108
            Top             =   960
            Width           =   735
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CB1"
            Height          =   195
            Index           =   20
            Left            =   2520
            TabIndex        =   107
            Top             =   960
            Width           =   615
         End
         Begin VB.CheckBox chkParm 
            Caption         =   "CB2"
            Height          =   195
            Index           =   21
            Left            =   120
            TabIndex        =   106
            Top             =   1200
            Width           =   735
         End
      End
      Begin VB.CommandButton cmdWriteParms 
         Caption         =   "Write &Selected Parms"
         Height          =   375
         Index           =   0
         Left            =   -72720
         TabIndex        =   104
         Top             =   3000
         Width           =   2175
      End
      Begin VB.Frame Frame6 
         Caption         =   "EditWord"
         Height          =   2685
         Left            =   -72720
         TabIndex        =   78
         Top             =   720
         Width           =   2175
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   16
            Left            =   600
            TabIndex        =   458
            Top             =   2400
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   16
            Left            =   240
            TabIndex        =   457
            Top             =   2400
            Width           =   255
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Rate (m/s)"
            Height          =   255
            Index           =   16
            Left            =   960
            TabIndex        =   456
            Top             =   2400
            Width           =   1155
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Nlev "
            Height          =   255
            Index           =   15
            Left            =   960
            TabIndex        =   102
            Top             =   2160
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Roll"
            Height          =   255
            Index           =   14
            Left            =   960
            TabIndex        =   101
            Top             =   1920
            Width           =   615
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Pitch"
            Height          =   255
            Index           =   13
            Left            =   960
            TabIndex        =   100
            Top             =   1680
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Trop Alt"
            Height          =   255
            Index           =   12
            Left            =   960
            TabIndex        =   99
            Top             =   1440
            Width           =   855
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "ZtOff"
            Height          =   255
            Index           =   11
            Left            =   960
            TabIndex        =   98
            Top             =   1200
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "OATtrop"
            Height          =   255
            Index           =   10
            Left            =   960
            TabIndex        =   97
            Top             =   960
            Width           =   975
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Temps"
            Height          =   255
            Index           =   9
            Left            =   960
            TabIndex        =   96
            Top             =   720
            Width           =   855
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "RetAlt"
            Height          =   255
            Index           =   8
            Left            =   960
            TabIndex        =   95
            Top             =   480
            Width           =   855
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   8
            Left            =   240
            TabIndex        =   94
            Top             =   480
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   9
            Left            =   240
            TabIndex        =   93
            Top             =   720
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   10
            Left            =   240
            TabIndex        =   92
            Top             =   960
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   11
            Left            =   240
            TabIndex        =   91
            Top             =   1200
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   12
            Left            =   240
            TabIndex        =   90
            Top             =   1440
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   13
            Left            =   240
            TabIndex        =   89
            Top             =   1680
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   14
            Left            =   240
            TabIndex        =   88
            Top             =   1920
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   15
            Left            =   240
            TabIndex        =   87
            Top             =   2160
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   8
            Left            =   600
            TabIndex        =   86
            Top             =   480
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   9
            Left            =   600
            TabIndex        =   85
            Top             =   720
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   10
            Left            =   600
            TabIndex        =   84
            Top             =   960
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   11
            Left            =   600
            TabIndex        =   83
            Top             =   1200
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   12
            Left            =   600
            TabIndex        =   82
            Top             =   1440
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   13
            Left            =   600
            TabIndex        =   81
            Top             =   1680
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   14
            Left            =   600
            TabIndex        =   80
            Top             =   1920
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   15
            Left            =   600
            TabIndex        =   79
            Top             =   2160
            Width           =   255
         End
         Begin VB.Label Label57 
            Caption         =   "Scan Trop   Actual"
            Height          =   255
            Left            =   120
            TabIndex        =   103
            Top             =   240
            Width           =   1335
         End
      End
      Begin VB.Frame Frame9 
         Caption         =   "MakeWord"
         Height          =   2685
         Left            =   -74880
         TabIndex        =   52
         Top             =   720
         Width           =   2055
         Begin VB.CheckBox chkWords 
            Caption         =   "Cts"
            Height          =   255
            Index           =   2
            Left            =   960
            TabIndex        =   76
            Top             =   960
            Width           =   855
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "TAs"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   75
            Top             =   720
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Nav"
            Height          =   255
            Index           =   0
            Left            =   960
            TabIndex        =   74
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Ttgt"
            Height          =   255
            Index           =   4
            Left            =   960
            TabIndex        =   73
            Top             =   1440
            Width           =   735
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "RAWbad"
            Height          =   255
            Index           =   5
            Left            =   960
            TabIndex        =   72
            Top             =   1680
            Width           =   975
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Cycle"
            Height          =   255
            Index           =   3
            Left            =   960
            TabIndex        =   71
            Top             =   1200
            Width           =   855
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   0
            Left            =   240
            TabIndex        =   70
            Top             =   480
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   1
            Left            =   240
            TabIndex        =   69
            Top             =   720
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   2
            Left            =   240
            TabIndex        =   68
            Top             =   960
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   3
            Left            =   240
            TabIndex        =   67
            Top             =   1200
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   4
            Left            =   240
            TabIndex        =   66
            Top             =   1440
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   5
            Left            =   240
            TabIndex        =   65
            Top             =   1680
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   0
            Left            =   600
            TabIndex        =   64
            Top             =   480
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   1
            Left            =   600
            TabIndex        =   63
            Top             =   720
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   2
            Left            =   600
            TabIndex        =   62
            Top             =   960
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   3
            Left            =   600
            TabIndex        =   61
            Top             =   1200
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   4
            Left            =   600
            TabIndex        =   60
            Top             =   1440
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   5
            Left            =   600
            TabIndex        =   59
            Top             =   1680
            Width           =   255
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Check8"
            Height          =   255
            Index           =   6
            Left            =   600
            TabIndex        =   58
            Top             =   1920
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Check8"
            Height          =   255
            Index           =   6
            Left            =   240
            TabIndex        =   57
            Top             =   1920
            Width           =   255
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Spare 1"
            Height          =   255
            Index           =   6
            Left            =   960
            TabIndex        =   56
            Top             =   1920
            Width           =   855
         End
         Begin VB.CheckBox chkBadTrop 
            Caption         =   "Spare 2"
            Height          =   255
            Index           =   7
            Left            =   600
            TabIndex        =   55
            Top             =   2160
            Width           =   255
         End
         Begin VB.CheckBox chkBadScan 
            Caption         =   "Spare 2"
            Height          =   255
            Index           =   7
            Left            =   240
            TabIndex        =   54
            Top             =   2160
            Width           =   255
         End
         Begin VB.CheckBox chkWords 
            Caption         =   "Spare 2"
            Height          =   255
            Index           =   7
            Left            =   960
            TabIndex        =   53
            Top             =   2160
            Width           =   855
         End
         Begin VB.Label Label56 
            Caption         =   "Scan Trop  Actual"
            Height          =   255
            Left            =   120
            TabIndex        =   77
            Top             =   240
            Width           =   1575
         End
      End
      Begin VB.Frame Frame11 
         Caption         =   "Enable Updates"
         Height          =   615
         Left            =   -74880
         TabIndex        =   46
         Top             =   1920
         Width           =   4335
         Begin VB.CheckBox chkEnableATP 
            Caption         =   "ATP"
            Height          =   255
            Left            =   2760
            TabIndex        =   294
            Top             =   240
            Width           =   615
         End
         Begin VB.CheckBox chkEnableTA 
            Caption         =   "TAs"
            Height          =   255
            Left            =   2160
            TabIndex        =   51
            Top             =   240
            Width           =   615
         End
         Begin VB.CheckBox chkEnableGains 
            Caption         =   "Gains"
            Height          =   255
            Left            =   840
            TabIndex        =   50
            Top             =   240
            Width           =   735
         End
         Begin VB.CheckBox chkEnableEng 
            Caption         =   "Temp"
            Height          =   255
            Left            =   120
            TabIndex        =   49
            Top             =   240
            Width           =   855
         End
         Begin VB.CheckBox chkEnableCounts 
            Caption         =   "Cts"
            Height          =   255
            Left            =   1560
            TabIndex        =   48
            Top             =   240
            Width           =   615
         End
         Begin VB.CommandButton cmdEnableUpdates 
            Caption         =   "Set All"
            Height          =   255
            Left            =   3480
            TabIndex        =   47
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.Frame Frame10 
         Caption         =   "Scan Control"
         Height          =   615
         Left            =   -74880
         TabIndex        =   42
         Top             =   720
         Width           =   4335
         Begin VB.CheckBox chkSkipBadScan 
            Caption         =   "Skip Bad"
            Height          =   255
            Left            =   120
            TabIndex        =   45
            Top             =   240
            Width           =   975
         End
         Begin VB.CheckBox chkStopOnBadScan 
            Caption         =   "Stop on Bad"
            Height          =   255
            Left            =   1320
            TabIndex        =   44
            Top             =   240
            Width           =   1215
         End
         Begin VB.CheckBox chkAutoScan 
            Caption         =   "Auto Change"
            Height          =   255
            Left            =   2880
            TabIndex        =   43
            Top             =   240
            Width           =   1335
         End
      End
      Begin VB.Frame Frame12 
         Caption         =   "Trop Control"
         Height          =   615
         Left            =   -74880
         TabIndex        =   38
         Top             =   1320
         Width           =   4335
         Begin VB.CheckBox chkSkipBadTrop 
            Caption         =   "Skip Bad"
            Height          =   255
            Left            =   120
            TabIndex        =   41
            Top             =   240
            Width           =   975
         End
         Begin VB.CheckBox chkStopOnBadTrop 
            Caption         =   "Stop on Bad"
            Height          =   255
            Left            =   1320
            TabIndex        =   40
            Top             =   240
            Width           =   1215
         End
         Begin VB.CheckBox chkAutoTrop 
            Caption         =   "Auto Change"
            Height          =   255
            Left            =   2880
            TabIndex        =   39
            Top             =   240
            Width           =   1335
         End
      End
      Begin VB.Frame Frame13 
         Caption         =   "Raw Mux"
         Height          =   2655
         Left            =   -74880
         TabIndex        =   21
         Top             =   720
         Width           =   1695
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   37
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   36
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   35
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   34
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   33
            Top             =   1560
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   32
            Top             =   1800
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   31
            Top             =   2040
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   30
            Top             =   2280
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   975
            MultiLine       =   -1  'True
            TabIndex        =   29
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   28
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   27
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   26
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   25
            Top             =   1560
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   24
            Top             =   1800
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   23
            Top             =   2040
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   22
            Top             =   2280
            Width           =   615
         End
      End
      Begin VB.TextBox txtTTT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -73920
         MultiLine       =   -1  'True
         TabIndex        =   20
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtZZZ 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74520
         MultiLine       =   -1  'True
         TabIndex        =   19
         Top             =   720
         Width           =   495
      End
      Begin ComctlLib.ProgressBar ProgressBar2 
         Height          =   135
         Left            =   -74160
         TabIndex        =   254
         Top             =   3240
         Width           =   3615
         _ExtentX        =   6376
         _ExtentY        =   238
         _Version        =   327682
         Appearance      =   1
      End
      Begin VB.Label lblAlgorithm 
         Caption         =   "Algo=6"
         Height          =   240
         Left            =   3465
         TabIndex        =   503
         Top             =   2790
         Width           =   510
      End
      Begin VB.Label Label47 
         Caption         =   "Algorithm"
         Height          =   285
         Left            =   -73605
         TabIndex        =   502
         Top             =   3465
         Width           =   690
      End
      Begin VB.Label Label 
         Caption         =   "Cold Point"
         Height          =   255
         Index           =   6
         Left            =   -74865
         TabIndex        =   482
         Top             =   1860
         Width           =   1185
      End
      Begin VB.Label Label 
         Caption         =   "OATmms"
         Height          =   255
         Index           =   5
         Left            =   -73320
         TabIndex        =   450
         Top             =   2820
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "OATnav"
         Height          =   255
         Index           =   4
         Left            =   -73320
         TabIndex        =   449
         Top             =   2505
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "cycles"
         Height          =   255
         Index           =   3
         Left            =   -73680
         TabIndex        =   395
         Top             =   720
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "List Parameter"
         Height          =   255
         Index           =   2
         Left            =   -73320
         TabIndex        =   394
         Top             =   2400
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Out Filename"
         Height          =   255
         Index           =   1
         Left            =   -74880
         TabIndex        =   383
         Top             =   3120
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Average"
         Height          =   255
         Index           =   0
         Left            =   -74880
         TabIndex        =   378
         Top             =   720
         Width           =   615
      End
      Begin VB.Label Label60 
         Caption         =   "Aircraft"
         Height          =   255
         Left            =   -74865
         TabIndex        =   363
         Top             =   2175
         Width           =   615
      End
      Begin VB.Label Label59 
         Caption         =   "Nlev2"
         Height          =   255
         Left            =   -74880
         TabIndex        =   361
         Top             =   2820
         Width           =   495
      End
      Begin VB.Label Label58 
         Caption         =   "Nlev1"
         Height          =   255
         Left            =   -74880
         TabIndex        =   359
         Top             =   2505
         Width           =   495
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "33"
         Height          =   255
         Index           =   34
         Left            =   -72000
         TabIndex        =   293
         Top             =   3120
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "32"
         Height          =   255
         Index           =   33
         Left            =   -72000
         TabIndex        =   292
         Top             =   2880
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "31"
         Height          =   255
         Index           =   32
         Left            =   -72000
         TabIndex        =   291
         Top             =   2640
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "30"
         Height          =   255
         Index           =   31
         Left            =   -72000
         TabIndex        =   290
         Top             =   2400
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "23"
         Height          =   255
         Index           =   30
         Left            =   -72000
         TabIndex        =   289
         Top             =   720
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "24"
         Height          =   255
         Index           =   29
         Left            =   -72000
         TabIndex        =   288
         Top             =   960
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "25"
         Height          =   255
         Index           =   28
         Left            =   -72000
         TabIndex        =   287
         Top             =   1200
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "26"
         Height          =   255
         Index           =   27
         Left            =   -72000
         TabIndex        =   286
         Top             =   1440
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "27"
         Height          =   255
         Index           =   26
         Left            =   -72000
         TabIndex        =   285
         Top             =   1680
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "28"
         Height          =   255
         Index           =   25
         Left            =   -72000
         TabIndex        =   284
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "29"
         Height          =   255
         Index           =   24
         Left            =   -72000
         TabIndex        =   283
         Top             =   2160
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "22"
         Height          =   255
         Index           =   23
         Left            =   -73440
         TabIndex        =   282
         Top             =   3120
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "21"
         Height          =   255
         Index           =   22
         Left            =   -73440
         TabIndex        =   281
         Top             =   2880
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "20"
         Height          =   255
         Index           =   21
         Left            =   -73440
         TabIndex        =   280
         Top             =   2640
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "19"
         Height          =   255
         Index           =   20
         Left            =   -73440
         TabIndex        =   279
         Top             =   2400
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "12"
         Height          =   255
         Index           =   19
         Left            =   -73440
         TabIndex        =   278
         Top             =   720
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "13"
         Height          =   255
         Index           =   18
         Left            =   -73440
         TabIndex        =   277
         Top             =   960
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "14"
         Height          =   255
         Index           =   17
         Left            =   -73440
         TabIndex        =   276
         Top             =   1200
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "15"
         Height          =   255
         Index           =   16
         Left            =   -73440
         TabIndex        =   275
         Top             =   1440
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "16"
         Height          =   255
         Index           =   15
         Left            =   -73440
         TabIndex        =   274
         Top             =   1680
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "17"
         Height          =   255
         Index           =   14
         Left            =   -73440
         TabIndex        =   273
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "18"
         Height          =   255
         Index           =   13
         Left            =   -73440
         TabIndex        =   272
         Top             =   2160
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "11"
         Height          =   255
         Index           =   12
         Left            =   -74880
         TabIndex        =   271
         Top             =   3120
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "10"
         Height          =   255
         Index           =   11
         Left            =   -74880
         TabIndex        =   270
         Top             =   2880
         Width           =   255
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "9"
         Height          =   255
         Index           =   10
         Left            =   -74760
         TabIndex        =   269
         Top             =   2640
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "8"
         Height          =   255
         Index           =   9
         Left            =   -74760
         TabIndex        =   268
         Top             =   2400
         Width           =   135
      End
      Begin VB.Label Label13 
         Caption         =   "Label13"
         Height          =   375
         Left            =   -74880
         TabIndex        =   267
         Top             =   720
         Width           =   15
      End
      Begin VB.Label Label14 
         Caption         =   "Tropopause 1"
         Height          =   255
         Left            =   -74880
         TabIndex        =   266
         Top             =   1230
         Width           =   1095
      End
      Begin VB.Label Label15 
         Caption         =   "Tropopause 2"
         Height          =   255
         Left            =   -74880
         TabIndex        =   265
         Top             =   1545
         Width           =   1095
      End
      Begin VB.Label Label16 
         Caption         =   "Nlev"
         Height          =   255
         Left            =   -74880
         TabIndex        =   264
         Top             =   3120
         Width           =   375
      End
      Begin VB.Label Label17 
         Caption         =   "Altitude"
         Height          =   255
         Left            =   -73200
         TabIndex        =   263
         Top             =   645
         Width           =   615
      End
      Begin VB.Label Label18 
         Caption         =   "Temperature"
         Height          =   255
         Left            =   -72360
         TabIndex        =   262
         Top             =   645
         Width           =   975
      End
      Begin VB.Label Label19 
         Caption         =   "Theta"
         Height          =   255
         Left            =   -71160
         TabIndex        =   261
         Top             =   645
         Width           =   495
      End
      Begin VB.Label Label45 
         Caption         =   "Channel 1"
         Height          =   255
         Left            =   -74880
         TabIndex        =   260
         Top             =   720
         Width           =   855
      End
      Begin VB.Label Label46 
         Caption         =   "Channel 2"
         Height          =   255
         Left            =   -74880
         TabIndex        =   259
         Top             =   1560
         Width           =   855
      End
      Begin VB.Label Label10 
         Caption         =   "Lapse Rate [K/km]"
         Height          =   255
         Left            =   -72600
         TabIndex        =   258
         Top             =   3120
         Width           =   1455
      End
      Begin VB.Label Label9 
         Caption         =   "MRI"
         Height          =   255
         Left            =   -73800
         TabIndex        =   257
         Top             =   3120
         Width           =   375
      End
      Begin VB.Label Label48 
         Caption         =   "Viewing"
         Height          =   255
         Left            =   -74880
         TabIndex        =   256
         Top             =   2880
         Width           =   615
      End
      Begin VB.Label Label49 
         Caption         =   "Location"
         Height          =   255
         Left            =   -74880
         TabIndex        =   255
         Top             =   3120
         Width           =   735
      End
      Begin VB.Label Label20 
         Caption         =   "Channel 3"
         Height          =   255
         Left            =   -74880
         TabIndex        =   253
         Top             =   2400
         Width           =   855
      End
      Begin VB.Label Label53 
         Caption         =   "Ch 1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Left            =   210
         TabIndex        =   252
         Top             =   690
         Width           =   435
      End
      Begin VB.Label Label54 
         Caption         =   "Ch 2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   870
         TabIndex        =   251
         Top             =   690
         Width           =   435
      End
      Begin VB.Label Label55 
         Caption         =   "Ch 3"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Left            =   1530
         TabIndex        =   250
         Top             =   690
         Width           =   435
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "1"
         Height          =   255
         Index           =   0
         Left            =   -74760
         TabIndex        =   249
         Top             =   720
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "2"
         Height          =   255
         Index           =   1
         Left            =   -74760
         TabIndex        =   248
         Top             =   960
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "3"
         Height          =   255
         Index           =   2
         Left            =   -74760
         TabIndex        =   247
         Top             =   1200
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "4"
         Height          =   255
         Index           =   3
         Left            =   -74760
         TabIndex        =   246
         Top             =   1440
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "5"
         Height          =   255
         Index           =   4
         Left            =   -74760
         TabIndex        =   245
         Top             =   1680
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "6"
         Height          =   255
         Index           =   6
         Left            =   -74760
         TabIndex        =   244
         Top             =   1920
         Width           =   135
      End
      Begin VB.Label lblLvl 
         Alignment       =   1  'Right Justify
         Caption         =   "7"
         Height          =   255
         Index           =   7
         Left            =   -74760
         TabIndex        =   243
         Top             =   2160
         Width           =   135
      End
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   1680
      Top             =   -240
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Label lblLvl 
      Alignment       =   1  'Right Justify
      Caption         =   "22"
      Height          =   255
      Index           =   5
      Left            =   120
      TabIndex        =   17
      Top             =   1920
      Width           =   255
   End
   Begin VB.Label lblMTP 
      Alignment       =   2  'Center
      BackColor       =   &H80000000&
      Caption         =   "MTP"
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   24
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00800000&
      Height          =   495
      Left            =   120
      TabIndex        =   12
      Top             =   4695
      Width           =   975
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open REF"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close REF"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuFormat 
      Caption         =   "&Format"
      Begin VB.Menu mnuHelpFormat 
         Caption         =   "&Format ATP"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmREFview"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public Speed%, FileRecords%, Forward As Boolean
Public PlotDone As Boolean, LRerase As Boolean
Public P_flag As Boolean, G_flag As Boolean
Public L_flag As Boolean, S_flag As Boolean, T_flag As Boolean, Skip
Public GM_flag As Boolean, PT_flag As Boolean
Public Quit As Boolean, BadNr%, EndRecord%, Minus As Boolean
Public SScale%, zc%, zs%, Tc%, Ts%, TL%, Tr%, Zb%, Zt%, dTdot
Public Tzgm, Zgm, Nparms%
Public zt1o, zt2o, Tzt1o, Tzt2o, ALTkmo
Public Taco!, jt1%, jt2%, jOut%
Public FastFwd As Boolean, FastBwd As Boolean, AutoStep As Boolean
Public X%, yymmdd$
Public EnableEng As Boolean, EnableGains As Boolean, EnableCounts As Boolean
Public EnableTA As Boolean, EnableAllUpdates As Boolean
Public EnableATP As Boolean
Public SkipBadTrop As Boolean, SkipBadScan As Boolean
Public StopOnBadTrop As Boolean, StopOnBadScan As Boolean, SelectOnlyGood As Boolean

Public QProAll As Boolean, AllParms As Boolean, Filename$
Public AutoGoodTrop As Boolean, AutoGoodScan As Boolean
Public mskBadScan&, mskBadTrop&, Scanlength!

Public AutoScaleTA As Boolean
Sub UpdateTBwindow()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, X!, iRC%, iBot%, iTop%
Dim WtB!, WtT!
'If Not ShowDiagnostics Then Exit Sub
  If Not GoodScan Then
  X = X
  End If
'  If chkShowOnlyGoodscans.value = 1 And Not GoodScan Then Exit Sub
'Plot Antenna Tempertures in picture window
  picTA.Cls
  picTA.ScaleMode = 0
  picTA.ScaleTop = 0
  picTA.ScaleHeight = 11
  
  If chkShowAAObs.Value = 1 Then
' Calculate weighted average values of archive average observables
    Call Get_iBot_iTop(RCindex1, pALT, iBot, iTop, WtB, WtT)
    If iBot = 1 Then iBot = 2: iTop = 1
    For j = 1 To Nobs
      OBavgWt(RCindex1, j) = OBav(RCindex1, iBot, j) * WtB + OBav(RCindex1, iTop, j) * WtT
    Next j
    If TwoAAprofiles Then 'Need to plot second set of AA observables.
      Call Get_iBot_iTop(RCindex2, pALT, iBot, iTop, WtB, WtT)
      If iBot = 1 Then iBot = 2: iTop = 1
      For j = 1 To Nobs
        OBavgWt(RCindex2, j) = OBav(RCindex2, iBot, j) * WtB + OBav(RCindex2, iTop, j) * WtT
      Next j
    End If
  End If

' Set temperature display scale
  If AutoScaleTA Then
    Tmin = 500
    Tmax = 100
    For i = 1 To 10
      T = TA(1, i)
      If T > Tmax Then Tmax = T
      If T < Tmin Then Tmin = T
    Next i
    If chkShowAAObs.Value = 1 Then
      SetArcAvgObs RCindex1 'Map AA Avg Observable to TAA array for UpdateTBwindow
      For j = 1 To 10
        T = TAA(RCindex1, 1, j)
        If T > Tmax Then Tmax = T
        If T < Tmin Then Tmin = T
      Next j
      If TwoAAprofiles Then 'Need to plot second set of AA observables.
        SetArcAvgObs RCindex2 'Map AA Avg Observable to TAA array for UpdateTBwindow
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
If chkShowAAObs.Value = 1 Then
  SetArcAvgObs RCindex1 'Map AA Avg Observable to TAA array for UpdateTBwindow
' Draw archive average TAs
  For i = 1 To Channels
    Select Case i
    Case 1: LineC = QBColor(4)  'RED
    Case 2: LineC = QBColor(7)  'WHITE
    Case 3: LineC = QBColor(1)   'BLUE
    End Select
    picTA.PSet (TAA(RCindex1, i, 1), 1)
    For j = 2 To 10
      picTA.Line -(TAA(RCindex1, i, j), j), LineC   'And continue from there
    Next j
  Next i
  
' Check for second set of RCs
  If TwoAAprofiles Then 'Need to plot second set of AA observables.
    SetArcAvgObs RCindex2 'Map AA Avg Observable to TAA array for UpdateTBwindow
    For i = 1 To Channels
      Select Case i
      Case 1: LineC = QBColor(4)  'RED
      Case 2: LineC = QBColor(7)  'WHITE
      Case 3: LineC = QBColor(1)   'BLUE
      End Select
      picTA.PSet (TAA(RCindex2, i, 1), 1)
      For j = 2 To 10
        picTA.Line -(TAA(RCindex2, i, j), j), LineC   'And continue from there
      Next j
    Next i
  End If

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

End Sub

Sub Initialize()
Dim X%, y%
  
  If UTmax < UTmin Then UTmax = UTmax + 86400
  Forward = True
  Cycle% = 0
  Call GetREFcycle(Cycle%, True)
  
  BadNr% = 0                'Count number of bad cycles
  EndRecord% = 0            'Highest record read
' Then Keyboard Commands and System Flags                      Change Command
  P_flag = False            'Paint ICE and NAT regions on plot        Ctrl+P
  G_flag = False            'Flag to turn ATP ghost on and off        Ctrl+G
  L_flag = True             'Show -2K/km lapse rate lines             Ctrl+L
  S_flag = True             'Show status bar in bottom LH corner      Ctrl+I
  Skip = False              'Skip display of bad records if TRUE      Ctrl+S
  T_flag = True             'Show trop info in upper RH corner        Ctrl+T
  GM_flag = False           'Show global minimum                      Ctrl+Z
  FastFwd = False           'TRUE if fast forwarding thru data (PgUp)
  FastBwd = False           'TRUE if fast backwarding thru data (PgDn)
  AutoStep = False          'TRUE if auto-stepping thru data (INS)
  Minus = False             'Going backwards
  DoNAT = False             'NAT flag
  PlotDone = False          'Flag to indicate plot has been done
  LRerase = False           'Flag to erase LR graphics

' Initialize plot window center position, size and borders
  SScale% = 1                 'Default Plot scale (1, 2, or 3)           1,2,3
  zc% = 12: zs% = 24: Tc% = 250: Ts% = 120
  X% = Ts% / 2 ^ (SScale% - 1) / 2: TL% = Tc% - X%: Tr% = Tc% + X%
  y% = zs% / 2 ^ (SScale% - 1) / 2: Zb% = zc% - y%: Zt% = zc% + y%

  Tzgm = 999.9               'Global (for experiment) minimum temperature
  Zgm = 99.9                 'Altitude of Tzgm
  Tmin = 999.9               'Current scan minimum
  Zmin = 99.9                'Altitude of Tmin
  'Version$ = "MJMVIEW.BAS 980413.0"
    
  tmrGetCycle.Interval = 100 'Auto Step cycle time in ms
  tmrGetCycle.Enabled = False
  cmdStop.Caption = "&Continue"
  Cycle% = 0
  Nparms = 38    'Number of extract parameters
End Sub

Sub ListMenu()
Dim C%, r%
'  r% = TIL%: c% = 41
'  LOCATE r%, c%
'  Color White%
'  Print "             MPVIEWER MENU          ";
'  INC R%: LOCATE R%, c%
'  Color LGray%
'  Print "  Up ....... Move profile upward    ";
'  INC R%: LOCATE R%, c%
'  Print "  Down ..... Move profile downward  ";
'  INC R%: LOCATE R%, c%
'  Print "  Left ..... Move profile to left   ";
'  INC R%: LOCATE R%, c%
'  Print "  Right .... Move profile to right  ";
'  INC R%: LOCATE R%, c%
'  Print "* Ins ...... Auto Step thru data    ";
'  INC R%: LOCATE R%, c%
'  Print "* Del ...... Delete current profile ";
'  INC R%: LOCATE R%, c%
'  Print "* PgUp ..... Fast forward thru data ";
'  INC R%: LOCATE R%, c%
'  Print "* PgDn ..... Fast backward thru data";
'  INC R%: LOCATE R%, c%
'  Print "* Home ..... Toggle Menu            ";
'  INC R%: LOCATE R%, c%
'  Print "  + ........ Single step forward    ";
'  INC R%: LOCATE R%, c%
'  Print "  - ........ Single step backward   ";
'  INC R%: LOCATE R%, c%
'  Print "  1,2,3 .... Three display scales   ";
'  INC R%: LOCATE R%, c%
'  Print "* Ctrl+G ... Toggle 'ghost' profile ";
'  INC R%: LOCATE R%, c%
'  Print "  Ctrl+R ... Reset Display          ";
'  INC R%: LOCATE R%, c%
'  Print "* Ctrl+S ... Skip bad records       ";
'  INC R%: LOCATE R%, c%
'  Print "* Ctrl+P ... Paint Ice/NAT region   ";
'  INC R%: LOCATE R%, c%
'  Print "  Esc ...... Quit Program           ";
'  INC R%: LOCATE R%, c%
'  Print "* .......... These commands toggle  ";
End Sub



Sub UpdateDisplay()
Dim Zt1g!, Zt2g!, Tzt1g!, Tzt2g!

    If chkPlotZg.Value = 0 Then 'Plot Pressure Altitude
      With frmATP
        .ALTkm = pALT
        .Zt1 = Zt1
        .Zt2 = Zt2
        .dZg = pALT - gALT
        .ShowPressureAltitude = True
      End With

    Else            'Plot Geometric Altitude
    ' Finally, do the SPLINE fit to retrieved ATP
      Call SPLINE(RAp(), RTp(), T1(), T2(), 1, Nlev)
      Call WMOTropLoc(RAp(), RTp(), T1(), T2(), 1, Nlev, Zt1g, Tzt1g, Zt2g, Tzt2g)

      With frmATP
        .ALTkm = gALT
        .Zt1 = Zt1g
        .Zt2 = Zt2g
        .dZg = pALT - gALT
        .ShowPressureAltitude = False
      End With
    End If
 
  frmATP.Tac = Tac

  frmATP.ATPupdate

End Sub


Sub UpdateMasks()
Dim i%

mskBadScan = 0
mskBadTrop = 0
For i% = 0 To 15
  If chkBadScan(i%).Value = 1 Then mskBadScan = mskBadScan + 2 ^ i%
  If chkBadTrop(i%).Value = 1 Then mskBadTrop = mskBadTrop + 2 ^ i%
Next i%
End Sub


Private Sub chkAutoScan_Click()
  If chkAutoScan.Value = 1 Then AutoGoodScan = True Else AutoGoodScan = False
End Sub

Private Sub chkAutoTrop_Click()
  If chkAutoTrop.Value = 1 Then AutoGoodTrop = True Else AutoGoodTrop = False
End Sub


Private Sub chkBadScan_Click(Index As Integer)
UpdateMasks
End Sub

Private Sub chkBadTrop_Click(Index As Integer)
UpdateMasks
End Sub


Private Sub chkEnableATP_Click()
  If chkEnableATP.Value = 1 Then EnableATP = True Else EnableATP = False
  UpdateForm

End Sub

Private Sub chkEnableCounts_Click()
  If chkEnableCounts.Value = 1 Then EnableCounts = True Else EnableCounts = False
  UpdateForm
End Sub

Private Sub chkEnableEng_Click()
  If chkEnableEng Then EnableEng = True Else EnableEng = False
  UpdateForm
End Sub

Private Sub chkEnableFit_Click()
If chkEnableFit.Value = 1 Then
  frmATP.EnableFit = True
Else
  frmATP.EnableFit = False
End If

End Sub

Private Sub chkEnableGains_Click()
  If chkEnableGains Then EnableGains = True Else EnableGains = False
  UpdateForm
End Sub


Private Sub chkEnableTA_Click()
  If chkEnableTA.Value = 1 Then EnableTA = True Else EnableTA = False
  UpdateForm
End Sub

Private Sub chkMultipleScans_Click()
Static SaveTropFlag As Boolean
  If chkMultipleScans.Value = 1 Then
    frmATP.PlotSingleScan = False
    SaveTropFlag = frmATP.ShowTropopause
    frmATP.ShowTropopause = False
  Else
    frmATP.PlotSingleScan = True
    frmATP.ShowTropopause = SaveTropFlag
  End If
  frmATP.SetDefaultScales
  frmATP.ATP_Plot_Set
  frmATP.ATP_Plot_Axes_Labels
  frmATP.ATP_Plot_Grid_Marks
  frmATP.ATP_Plot_Tic_Marks
End Sub

Private Sub chkOK_Click()
If chkOK.Value = 1 Then OK = True Else OK = False
End Sub



Private Sub chkSelectOnlyGood_Click()
If chkSelectOnlyGood.Value = 1 Then SelectOnlyGood = True Else SelectOnlyGood = False
End Sub

Private Sub chkSkipBadScan_Click()
If chkSkipBadScan.Value = 1 Then SkipBadScan = True Else SkipBadScan = False
End Sub

Private Sub chkSkipBadTrop_Click()
If chkSkipBadTrop.Value = 1 Then SkipBadTrop = True Else SkipBadTrop = False
End Sub

Private Sub chkStopOnBadScan_Click()
  If chkStopOnBadScan.Value = 1 Then StopOnBadScan = True Else StopOnBadScan = False
End Sub

Private Sub chkStopOnBadTrop_Click()
If chkStopOnBadTrop.Value = 1 Then StopOnBadTrop = True Else StopOnBadTrop = False
End Sub

Private Sub cmbALT_Click()
UpdateForm
End Sub

Private Sub cmbAvg_Click()
cmbGOTO.Text = cmbAvg.Text
End Sub


Private Sub cmbGOTO_Click()
UpdateForm
End Sub

Private Sub cmbLat_Click()
UpdateForm
End Sub


Private Sub cmbLon_Click()
UpdateForm
End Sub


Private Sub cmbOAT_Click()
UpdateForm
End Sub

Private Sub cmbSpeed_Click()
UpdateForm
End Sub

Private Sub cmbTh_Click()
  UpdateForm
End Sub

Private Sub cmbTt_Click()
UpdateForm
End Sub

Private Sub cmbUT_Click()
UpdateForm
End Sub

Private Sub cmbZt_Click()
UpdateForm
End Sub

Private Sub cmdATP_Click()
  
  
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

Private Sub cmdAvg_Click(Index As Integer)
Dim Navg%, AnEvent!, lu%, i%, j%, k%, L%, Count%, OutCycle%, fmt$
Dim Tavg!(1 To 33), Zavg!(1 To 33), TAavg!(1 To 3, 1 To 10)
Dim ELLlu%, V!(1 To 4), AltSum!, OATsumMTP!, OATsumNAV!, OATsumMMS!
Dim Ztes!(0 To 37), Tout!(0 To 37)

Select Case Index
Case 0  'Add
  lstAvg.AddItem txtAvg.Text
Case 1  'Remove
  lstAvg.RemoveItem (lstAvg.ListIndex)
Case 2  'Clear
  lstAvg.Clear
Case 3  'Import
  lstAvg.Clear
  ' Set Initial directory
  CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
  ' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
  ' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
  ' Specify default filter.
  CommonDialog1.FilterIndex = 1
  ' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  Filename$ = CommonDialog1.Filename

  If Filename$ = "" Then Exit Sub
  lu% = FreeFile
  Open Filename$ For Input As #lu%
  Input #lu, Navg
  For i = 1 To Navg
    Input #lu, AnEvent
    Select Case cmbAvg.Text
    Case "Rec No", "seconds"
      lstAvg.AddItem Format$(AnEvent, "000000")
    Case "ks"
      lstAvg.AddItem Format$(AnEvent, "000.000")
    Case Else
    End Select
  Next i
  Close lu
  
Case 4  'Save list
  lu = FreeFile
  Open Path$ + YYYYMMDD$ + ".events" For Output As #lu
  Print #lu, lstAvg.ListCount
  For i = 0 To lstAvg.ListCount - 1
    Print #lu, lstAvg.List(i)
  Next i
  Close lu
  
Case 5  'Average Cycles and write out
  If REFlu = 0 Then
    Call MsgBox("You must open a REF file before this command!", vbOKOnly)
    Exit Sub
  End If
' First open file for output
  OUTlu = FreeFile
  Open Path$ + "Events.REF" For Random Access Read Write As OUTlu Len = Len(REF)
  
  ELLlu = FreeFile
  Open Path$ + AC$ + YYYYMMDD$ + ".ELL" For Output As ELLlu
  Print #ELLlu, lstAvg.ListCount
  
  Count = Val(txtAvgCycles.Text)
  OutCycle = 0

  For i = 0 To lstAvg.ListCount - 1
    For k = 1 To Channels
      For L = 1 To 10: TAavg(k, L) = 0#: Next L
    Next k
    For k = 1 To 33
      Zavg(k) = 0#
      Tavg(k) = 0#
    Next k

    OutCycle = OutCycle + 1
    txtGOTO.Text = Str$(lstAvg.List(i))
    cmdGO_Click   'Find the required time/cycle
    AltSum = 0#
    OATsumMTP = 0#
    OATsumNAV = 0#
    OATsumMMS = 0#
    
    For j = Cycle + Count - 1 To Cycle Step -1
      REFread REFlu, j
      AltSum = AltSum + ALTkm
      OATsumMTP = OATsumMTP + OATmtp
      OATsumNAV = OATsumNAV + OATnav
      OATsumMMS = OATsumMMS + OATmms
      For k = 1 To Channels
        For L = 1 To 10: TAavg(k, L) = TAavg(k, L) + TA(k, L): Next L
      Next k
      For k = 1 To 33
        Zavg(k) = Zavg(k) + sZZZ(k)
        Tavg(k) = Tavg(k) + sTTT(k)
      Next k
    Next j
    ' Now calculate averages for this event
    ALTkm = AltSum / Count
    OATmtp = OATsumMTP / Count
    OATnav = OATsumNAV / Count
    OATmms = OATsumMMS / Count
    For k = 1 To Channels
      For L = 1 To 10: TA(k, L) = TAavg(k, L) / Count: Next L
    Next k
    For k = 1 To 33
      sZZZ(k) = Zavg(k) / Count
      sTTT(k) = Tavg(k) / Count
      zzz(k) = sZZZ(k) * 100#
      TTT(k) = sTTT(k) * 10#
    Next k
    ' And write to Out File
    Cycle = OutCycle
    FileFormatOut = FileFormatIn
    REFwrite OUTlu, OutCycle
    ' And write event info to ELL file
    fmt$ = "#####0 #0.000 ##0.000 ###0.000"
    V(1) = Val(lstAvg.List(i)): V(2) = ALTkm: V(3) = Latitude: V(4) = Longitude
    'Debug.Print V(1); V(2); V(3); V(4)
    Print #ELLlu, fUsing(fmt$, V())
  Next i
  REFwriteFLTINFO (OUTlu)
  NV = 2
  REFwriteLIMITS (OUTlu)
  Close OUTlu
  Close ELLlu
  Call Calculate_TES_Levels(Ztes!())
  j = -1
  Do
  j = j + 1
  Loop Until sZZZ(1) < Ztes(j)
  For i = 0 To j - 1
    Tout(i) = -999#
  Next i
  For i = j To 37
    Tout(i) = fLinterp(sZZZ(), sTTT(), Nlev, Ztes(i))
  Next i
  For i = j To 37
    'Debug.Print Tout(i); Ztes(i)
  Next i
  
Case 6
  With frmATP
    .SetDefaultScales
    .ATP_Plot_Set
    .ATP_Plot_Axes_Labels
    .ATP_Plot_Grid_Marks
    .ATP_Plot_Tic_Marks
  End With
End Select
txtAvg.Text = ""

End Sub

Private Sub cmdBack_Click()
Forward = False
tmrGetCycle_Timer
End Sub




Private Sub cmdEnableUpdates_Click()
  EnableAllUpdates = Not EnableAllUpdates
  If EnableAllUpdates Then
    cmdEnableUpdates.Caption = "&Clear All"
    chkEnableEng.Value = 1
    chkEnableGains.Value = 1
    chkEnableCounts.Value = 1
    chkEnableTA.Value = 1
    chkEnableATP.Value = 1
  Else
    cmdEnableUpdates.Caption = "&Set All"
    chkEnableEng.Value = 0
    chkEnableGains.Value = 0
    chkEnableCounts.Value = 0
    chkEnableTA.Value = 0
    chkEnableATP.Value = 0
  End If
End Sub

Private Sub cmdExit_Click()
  
  Unload frmATP
  Unload frmFormatATP
  Unload Me

End Sub

Private Sub cmdFwd_Click()
Forward = True
tmrGetCycle_Timer
End Sub

Private Sub cmdGO_Click()
Dim RecordFound As Boolean, tks&, LoopCount%, RecInc!
  
On Error GoTo InvalidRequest

Select Case cmbGOTO.Text
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
  Case Is > FileRecords: Record = FileRecords - 1
  End Select
  
  Cycle = Record
  RecordFound = False
  Forward = True
  LoopCount% = 0
  Do
    LoopCount% = LoopCount% + 1
    tmrGetCycle_Timer
    Select Case cmbGOTO.Text
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
Exit Sub

InvalidRequest:
  Msg = "Invalid GOTO request" ' Define message.
  Style = vbOK      ' Define buttons.
  Title = "WARNING!"  ' Define title.
  Help = "" ' Define Help file.
  Ctxt = 0 ' Define topic context.
  ' Display message.
  response = MsgBox(Msg, Style, Title, Help, Ctxt)

End Sub

Private Sub cmdParmsAll_Click()
Dim i%

AllParms = Not AllParms
If AllParms Then
  cmdParmsAll.Caption = "Clear All"
  For i = 0 To Nparms - 1
    chkParm(i).Value = 1
  Next i
'  If Channels = 2 Then
'    chkParm(14).value = 0
'    chkParm(17).value = 0
'    chkParm(20).value = 0
'    chkParm(23).value = 0
'    chkParm(26).value = 0
'  End If
Else
  cmdParmsAll.Caption = "Set All"
  For i = 0 To Nparms - 1
    chkParm(i).Value = 0
  Next i
End If

End Sub

Private Sub cmdStop_Click()
If tmrGetCycle.Enabled Then
  tmrGetCycle.Enabled = False
  cmdStop.Caption = "&Continue"
Else
  tmrGetCycle.Enabled = True
  cmdStop.Caption = "&Stop"
End If

End Sub



Private Sub cmdWriteParms_Click(Index As Integer)

Select Case Index
Case 0  'Write selected parameters
Dim i%, j%, V(0 To 37) As String, H(0 To 37) As String, Free!, out$, Gain1!, Gain2!
Dim Efile$, INPfile$, OUTfile$
' REF file should be open and assigned to REFlu
' Eventually set start and end times!
' Open out file

OUTlu% = FreeFile
Efile$ = Left$(txtREFfile.Text, Len(txtREFfile.Text) - 4) + ".txt"
Open Efile$ For Output As OUTlu%
Free = 0#
' Print Formatting information
  Print #OUTlu, "Flight" + vbTab + YYYYMMDD$
  Print #OUTlu, "UT Range" + vbTab + Format$(UTmin / 1000#, "#00.000") + vbTab + Format$(UTmax / 1000#, "#00.000")
  Print #OUTlu, "LAT Range" + vbTab + Format$(LATmin, "#00.000") + vbTab + Format$(LATmax, "#00.000")
  Print #OUTlu, "LON Range" + vbTab + Format$(LONmin, "#000.000") + vbTab + Format$(LONmax, "#000.000")
  Print #OUTlu, "ALT Range" + vbTab + Format$(pALTmin, "#00.000") + vbTab + Format$(pALTmax, "#00.000")
  Print #OUTlu, "T Range" + vbTab + Format$(Tacmin, "#00.000") + vbTab + Format$(Tacmax, "#00.000")
 
  
  H$(0) = "UTks": H$(1) = "pALT": H$(2) = "OATmtp": H$(3) = "Lat": H$(4) = "Long"
  H$(5) = "Pitch": H$(6) = "Roll": H$(7) = "Hdg": H$(8) = "LRac": H$(9) = "OATnav"
  H$(10) = "Zt1": H$(11) = "Zt2": H$(12) = "Th1": H$(13) = "Th2"
  H$(14) = "Tmtp1": H$(15) = "Tmtp2": H$(16) = "Tmtp3"
  
  H$(17) = "Gain1": H$(18) = "Gain2": H$(19) = "Gain3": H$(20) = "CB1"
  H$(21) = "CB2": H$(22) = "CB3": H$(23) = "dND1": H$(24) = "dND2"
  H$(25) = "dND3": H$(26) = "C15": H$(27) = "C25": H$(28) = "C35"
  H$(29) = "C16": H$(30) = "C26": H$(31) = "C36"
  H$(32) = "C101" + vbTab + "C102" + vbTab + "C103" + vbTab + "C104" + vbTab + "C105" + vbTab + "C106" + vbTab + "C107" + vbTab + "C108" + vbTab + "C109" + vbTab + "C110" + vbTab + "C111" + vbTab + "C112"
  H$(33) = "C201" + vbTab + "C202" + vbTab + "C203" + vbTab + "C204" + vbTab + "C205" + vbTab + "C206" + vbTab + "C207" + vbTab + "C208" + vbTab + "C209" + vbTab + "C210" + vbTab + "C211" + vbTab + "C212"
  H$(34) = "C301" + vbTab + "C302" + vbTab + "C303" + vbTab + "C304" + vbTab + "C305" + vbTab + "C306" + vbTab + "C307" + vbTab + "C308" + vbTab + "C309" + vbTab + "C310" + vbTab + "C311" + vbTab + "C312"
  H$(35) = "Ttgt" + vbTab + "Tnd" + vbTab + "Tlo1" + vbTab + "Tlo2" + vbTab + "Tifa" + vbTab + "Tlo" + vbTab + "Thi" + vbTab + "Acc+" + vbTab + "Twin" + vbTab
  H$(35) = H$(35) + "Tmtr" + vbTab + "Spare" + vbTab + "Vref" + vbTab + "DC1" + vbTab + "DC2" + vbTab + "PS5" + vbTab + "PS12" + vbTab + "Acc-"
  H$(36) = "Zcp": H$(37) = "Tcp"
out$ = ""
For j = 0 To Nparms - 1
  If chkParm(j).Value = 1 Then
    If out$ = "" Then out$ = H$(j) Else out$ = out$ + vbTab + H$(j)
  End If
Next j
Print #OUTlu%, out$  'Write header string

For i = 1 To FileRecords
  Call REFread(REFlu%, i)
  If SkipBadScan And Not GoodScan Then GoTo NextRecord
  If SkipBadTrop And Not GoodTrop Then GoTo NextRecord
  
' Map parameters to an array to facilitate formatting
  V(0) = Format$(UTsec / 1000#, "##0.000"): V(1) = Format$(pALT, "#0.000")
  V(2) = Format$(OATmtp, "##0.00"): V(3) = Format$(Latitude, "##0.000")
  V(4) = Format$(Longitude, "###0.000")
  V(5) = Format$(Pitch, "##0.0"):  V(6) = Format$(Roll, "#00.0"): V(7) = Format$(Heading, "#000.0")
  V(8) = Format$(LRac, "##0.0"): V(9) = Format$(OATnav, "#00.00")

  V(10) = Format$(Zt1, "#0.00"): V(11) = Format$(Zt2, "#0.00")
  V(12) = Format$(Th1, "000.0"):  V(13) = Format$(Th2, "000.0")
  V(36) = Format$(Zcp, "00.00"): V(37) = Format$(Tcp, "000.0")
  'Gain1 = (c(1, 5) - CB1) / (90 - Ttgt)
  'Gain2 = (c(2, 5) - CB2) / (90 - Ttgt)
  'Gain3 = (c(3, 5) - CB3) / (90 - Ttgt)
  V(14) = Format$(TA(1, 6), "##0.00"): V(15) = Format$(TA(2, 6), "##0.00"): V(16) = Format$(TA(3, 6), "##0.00")
  
  V(17) = Format$(g(1), "##0.00"): V(18) = Format$(g(2), "##0.00"): V(19) = Format$(g(3), "##0.00")
  V(20) = Format$(CB(1), "##0.00"): V(21) = Format$(CB(2), "##0.00"): V(22) = Format$(CB(3), "##0.00")
  V(23) = Format$(dND(1), "##0.00"): V(24) = Format$(dND(2), "##0.00"): V(25) = Format$(dND(3), "##0.00")
  V(26) = Format$(C(1, 5), "##0.00"): V(27) = Format$(C(2, 5), "##0.00"): V(28) = Format$(C(3, 5), "##0.00")
  V(29) = Format$(C(1, 6), "##0.00"): V(30) = Format$(C(2, 6), "##0.00"): V(31) = Format$(C(3, 6), "##0.00")
  
  V(32) = Format$(C(1, 1), "####0")
  For j = 2 To 12
    V(32) = V(32) + vbTab + Format$(C(1, j), "###0")
  Next j
  
  V(33) = Format$(C(2, 1), "####0")
  For j = 2 To 12
    V(33) = V(33) + vbTab + Format$(C(2, j), "###0")
  Next j
  
  V(34) = Format$(C(3, 1), "####0")
  For j = 2 To 12
    V(34) = V(34) + vbTab + Format$(C(3, j), "###0")
  Next j

  
  V(35) = Format$(Muxs(0), "##0.00")
  For j = 1 To 16
    V(35) = V(35) + vbTab + Format$(Muxs(j), "##0.00")
  Next j
'
  out$ = ""
  For j = 0 To Nparms - 1
    If chkParm(j).Value = 1 Then
      If out$ = "" Then out$ = V(j) Else out$ = out$ + vbTab + V(j)
    End If
  Next j
  If Latitude < 90 Then Print #OUTlu%, out$
  txtRecord.Text = Str$(i)
  DoEvents
NextRecord:
Next i
Close OUTlu%

Case 1 'Write default parameters
  INPfile$ = txtREFfile.Text
  OUTfile$ = fReplaceExtension(txtREFfile.Text, "txt")
  Call ExportTXTfile(INPfile$, OUTfile$)

End Select
End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
Dim f As Form, lu%, Line$, i%
Set f = frmFLTINFO
  
  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"

  cmdEnableUpdates_Click         'Enable all display flags
  AutoScaleTA = True
  
  With cmbALT
    .AddItem "km"
    .AddItem "kft"
    .ListIndex = 0
  End With

  With cmbLat
    .AddItem "deg"
    .AddItem "dms"
    .ListIndex = 0
  End With

  With cmbLon
    .AddItem "deg"
    .AddItem "dms"
    .ListIndex = 0
  End With

  With cmbZt
    .AddItem "km"
    .AddItem "kft"
    .ListIndex = 0
  End With

  With cmbTt
    .AddItem "K"
    .AddItem "C"
    .AddItem "F"
    .ListIndex = 0
  End With

  With cmbTh
    .AddItem "K"
    .AddItem "C"
    .AddItem "F"
    .ListIndex = 0
  End With

  With cmbOAT
    .AddItem "K"
    .AddItem "C"
    .AddItem "F"
    .ListIndex = 0
  End With

  With cmbSpeed
    .AddItem "knots"
    .AddItem "m/s"
    .AddItem "mph"
    .AddItem "km/hr"
    .ListIndex = 0
  End With

  With cmbUT
    .AddItem "hh:mm:ss"
    .AddItem "s"
    .ListIndex = 1
  End With

  With cmbGOTO
    .AddItem "Rec No"
    .AddItem "ks"
    .AddItem "seconds"
    .AddItem "hhmmss"
    .AddItem "hh:mm:ss"
    .ListIndex = 1
  End With

' Set up averaging combo box to be same as GOTO combo
  With cmbAvg
    .AddItem "Rec No"
    .AddItem "ks"
    .AddItem "seconds"
    .AddItem "hhmmss"
    .AddItem "hh:mm:ss"
    .ListIndex = 1
  End With

' Set up ATP Options Form

  With frmATP
    .Top = frmMTPbin.Height
    .Left = 0
    .Width = 1024 * Screen.TwipsPerPixelX - f.Width
    .Height = f.Height
    .ATP_Initialize
    .Visible = False
    .PlotSingleScan = True
    .Visible = False
    .ShowAAprofile = True
  End With

  With frmFormatATP
    .Visible = False
    .chkAircraft.Value = 1
    .chkTropopause.Value = 1
    .chkAircraft.Value = 1
  End With
  
  With frmFormatATP.cmbYunitl
    .AddItem "km"
    .AddItem "kft"
    .AddItem "mb"
    .AddItem "hPa"
    .Text = "km"
  End With

  With frmFormatATP.cmbYunitr
    .AddItem "km"
    .AddItem "kft"
    .AddItem "mb"
    .AddItem "hPa"
    .Text = "kft"
  End With

  With frmFormatATP
    .optYcoordl(0).Value = 1
    .optYcoordr(0).Value = 1
    .Hide
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
  End With


  GoodTrop = True
  txtGoodTrop.BackColor = &H8000&
  txtGoodTrop.Text = "Good Tropopause"
  GoodScan = True
  txtGoodScan.BackColor = &H8000&
  txtGoodScan.Text = "Scan"
  txtGoodTrop.BackColor = &H8000&
  txtGoodTrop.Text = "Trop"

  SkipBadScan = False
  chkSkipBadScan.Value = 0
  SkipBadTrop = False
  chkSkipBadTrop.Value = 0

  StopOnBadScan = False
  chkStopOnBadScan.Value = 0
  StopOnBadTrop = False
  chkStopOnBadTrop.Value = 0

  SelectOnlyGood = False
  chkSelectOnlyGood.Value = 0

  chkEnableFit.Value = 1
  frmATP.EnableFit = True

' Set Default Word Edit Masks
  chkBadScan(3).Value = 1    ' Cycle
  chkBadScan(8).Value = 1    ' RetAlt
  chkBadScan(15).Value = 1   ' Nlev

  chkBadTrop(5).Value = 1    ' RAWbad
  chkBadTrop(11).Value = 1   ' ZtOff
  chkBadTrop(12).Value = 1   ' TropAlt
  chkBadTrop(13).Value = 1   ' Pitch
  chkBadTrop(14).Value = 1   ' Roll

  UpdateMasks

  With frmREFview
    .Top = 0
    .Left = 0
    .Height = 5865
    .Width = 4920
  End With
  Show
  SetFocus
  
  For i = 0 To NRC - 1
    Call ReadInRC(RCpath$ + RCs$ + Reg$(i), i)
  Next i

'  frmMTPbin.mnuViewATP.Checked = True
'  frmMTPbin.ShowATP = True
  frmATP.Show
  

End Sub


Private Sub lblMTP_Click()
Dim fn$, fn1$, fn2$, Filename$

' Quick Start
  fn$ = Path$ + AC$ + YYYYMMDD$
  fn1$ = fn$ + ".ERF"
  fn2$ = fn$ + ".REF"
  fn$ = Dir(fn2$, vbNormal)      'Check if REF exits
  If fn$ = "" Then               'No ERF file, check for REF
    fn$ = Dir(fn1$, vbNormal)
    If fn$ = "" Then             'Neither ERF or REF
      Call MsgBox("Neither requested ERF or REF file exist", vbOKOnly)
      Exit Sub
    End If
  End If
  
  Filename$ = Path$ + fn$
  REFopen Filename$
  frmREFview.txtREFfile.Text = Filename$
  FileFormatIn = fREFreadFileFormat(REFfile$)
  Select Case FileFormatIn
  Case 32
    FileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
  Case 33
    FileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
  End Select
  'FileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
  ProgressBar2.Min = 0
  ProgressBar2.Max = FileRecords
  frmREFview.txtFileRecords.Text = FileRecords
  REFread REFlu, FileRecords
  Scanlength = UTsec
  REFread REFlu, 1
  If UTsec > Scanlength Then Scanlength = Scanlength + 86400
  Scanlength = (Scanlength - UTsec) / (FileRecords - 1)

  REFreadHEADER Filename$
  
  Initialize
  MainProgram
Exit Sub
ErrorMessage:
  Stop

ErrorHandler:
  
End Sub

Private Sub mnuFileClose_Click()
Close REFlu
End Sub

Private Sub mnuFileExit_Click()
  Unload frmATP
  Unload frmFormatATP
  Unload Me
End Sub

Private Sub mnuFileOpen_Click()
Dim xx$
' Set Initial directory
  CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "REF Data Structure Files (*.REF, *.ERF)|*.??F|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  Filename$ = CommonDialog1.Filename

  If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
    Exit Sub
  Else
'  On Error GoTo errhandler
    If REFlu <> 0 Then Close REFlu
    REFopen Filename$
    frmREFview.txtREFfile.Text = Filename$
'   FileFormatIn = fREFreadFileFormat(REFfile$)
    Select Case FileFormatIn
    Case 32
      FileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
    Case 33
      FileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
    End Select
    'FileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
    ProgressBar2.Min = 0
    ProgressBar2.Max = FileRecords
    frmREFview.txtFileRecords.Text = FileRecords
    REFread REFlu, FileRecords
    Scanlength = UTsec
    REFread REFlu, 1
    If UTsec > Scanlength Then Scanlength = Scanlength + 86400
    Scanlength = (Scanlength - UTsec) / (FileRecords - 1)
  End If

  REFreadHEADER Filename$

  Initialize
  MainProgram

End Sub

Sub MainProgram()

  Call GetREFcycle(Cycle%, False)
  If Nlev > 0 And Nlev <= 33 Then UpdateDisplay
  DoEvents

End Sub    'End of Main Program

Sub GetREFcycle(Cycle%, ByVal PlotUpdateFlag As Boolean)
Dim Zt!, Tt!, Th!, i%

'Read new profile
GetaCycle:
  If EOF(REFlu%) Then Exit Sub 'XXXX
  If Forward Then INC Cycle% Else DEC Cycle%
  'Debug.Print Forward; Record%
  
  If Cycle% < 1 Then
    Cycle% = 1
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
  
  If Cycle% > FileRecords Then
    Cycle% = FileRecords
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
  
  Call REFread(REFlu%, Cycle%)
  
'  If Cycle% > EndRecords% Then
'    EndRecord% = Cycle%            'Track largest record
'    If Not GoodScan Then INC BadNr%      'Increment Bad record count if new
'  End If
  'Debug.Print GoodScan; GoodTrop
  
  If SkipBadScan And Not GoodScan Then
    If Not Forward Then Cycle% = Cycle% - 2
    txtRecord.Text = Str$(Cycle%)
    GoTo GetaCycle
  End If
  
  If SkipBadTrop And Not GoodTrop Then
    If Not Forward Then Cycle% = Cycle% - 2
    txtRecord.Text = Str$(Cycle%)
    GoTo GetaCycle
  End If
  
  If (Not GoodScan And StopOnBadScan) Or (Not GoodTrop And StopOnBadTrop) Then
    tmrGetCycle.Enabled = False
    cmdStop.Caption = "&Continue"
  End If
  
  
' Select only Cycles that have good trops
  If SelectOnlyGood And Not GoodTrop Then
    GoodScan = False
    FileFormatOut = FileFormatIn   'Make sure outfile is same format!
    Call REFwrite(REFlu%, Cycle%)
  End If
  If Nlev1 > 0 Then
    For i = Nlev1 To Nlev2
      If chkPlotZg.Value = 1 Then
        RAp(i - Nlev1 + 1) = sZg(i)
      Else
        RAp(i - Nlev1 + 1) = sZZZ(i)
      End If
      RTp(i - Nlev1 + 1) = sTTT(i)
    Next i
  End If
  UpdateForm
  
  Tac = OATmtp  'For PlotUpdate
  frmATP.TwoAAprofiles = TwoAAprofiles
  If PlotUpdateFlag Then frmATP.ATPupdate

End Sub



Private Sub mnuHelpAbout_Click()
'Set properties
frmAbout.Application = "MTP REF-file Viewer"
frmAbout.Heading = "Release September 30,1998"
frmAbout.Copyright = "1998 MJ Mahoney"
'Call a method
frmAbout.Display
End Sub




Private Sub ProgressBar1_MouseDown(Button As Integer, Shift As Integer, X As Single, y As Single)

End Sub

Private Sub mnuHelpFormat_Click()
  
  Load frmFormatATP
  frmFormatATP.Visible = True

End Sub

Private Sub picTA_MouseMove(Button As Integer, Shift As Integer, X As Single, y As Single)
  
  picTA.ToolTipText = "TB=" + Format(X, "##0.0") + ", Loc=" + Format(y, "##0.0")

End Sub


Private Sub txtGoodScan_Click()
  GoodScan = Not GoodScan
  If GoodScan Then
    txtGoodScan.BackColor = &H8000&
    txtGoodScan.Text = "Good Scan"
  Else
    txtGoodScan.BackColor = &HFF&
    txtGoodScan.Text = "Bad Scan"
  End If
  FileFormatOut = FileFormatIn
  Call REFwrite(REFlu%, Cycle%)

End Sub


Private Sub txtGoodTrop_Click()
  GoodTrop = Not GoodTrop
  If GoodTrop Then
    txtGoodTrop.BackColor = &H8000&
    txtGoodTrop.Text = "Good Tropopause"
  Else
    txtGoodTrop.BackColor = &HFF&
    txtGoodTrop.Text = "Bad Tropopause"
  End If
  FileFormatOut = FileFormatIn
  Call REFwrite(REFlu%, Cycle%)

End Sub


Private Sub tmrGetCycle_Timer()
  Call GetREFcycle(Cycle%, False)
  If Nlev > 0 And Nlev <= 33 Then UpdateDisplay
  DoEvents

End Sub


Sub UpdateForm()
Dim Zt!, Tt!, Th!, di&, i%, j%, ThAC!, Thcp!
    
  If REFlu = 0 Then Exit Sub
  Select Case GoodScan
  Case True
    txtGoodScan.BackColor = &H8000&
    txtGoodScan.Text = "Good Scan"
  Case False
    txtGoodScan.BackColor = &HFF&
    txtGoodScan.Text = "Bad Scan"
  End Select
    
  Select Case GoodTrop
  Case True
    txtGoodTrop.BackColor = &H8000&
    txtGoodTrop.Text = "Good Tropopause"
  Case False
    txtGoodTrop.BackColor = &HFF&
    txtGoodTrop.Text = "Bad Tropopause"
  End Select

  If frmREFview.cmbUT.Text = "s" Then frmREFview.txtUT.Text = Str$(UTsec) Else frmREFview.txtUT.Text = fSecToTstring$(UTsec, True)
  If frmREFview.cmbALT.Text = "km" Then
    frmREFview.txtALT.Text = Format$(ALTkm, "#0.000")
  Else
    frmREFview.txtALT.Text = Format$(ALTkm * kft_km, "#0.000")
  End If
  Select Case cmbOAT
  Case "K": frmREFview.txtOAT.Text = Tac
  Case "C": frmREFview.txtOAT.Text = Tac - 273.15
  Case "F": frmREFview.txtOAT.Text = (Tac - 273.15) * 9 / 5 + 32
  End Select
  
  Select Case cmbLat.Text
  Case "deg"
    txtLatitude.Text = Format$(Latitude, "##0.000")
  Case "dms"
    txtLatitude.Text = fDegToDMS$(Latitude)
  End Select
  
  Select Case cmbLon.Text
  Case "deg"
    txtLongitude.Text = Format$(Longitude, "###0.000")
  Case "dms"
    txtLongitude.Text = fDegToDMS$(Longitude)
  End Select
  
  Select Case cmbSpeed
  Case "m/s"
    txtWspd.Text = 0.5144445 * Wspd
  Case "km/hr"
    txtWspd.Text = 1.852 * Wspd
  Case "knots"
    txtWspd.Text = Wspd
  Case "mph"
    txtWspd.Text = 1.150758 * Wspd
  End Select
  
  With frmREFview
    .txtRecord.Text = Cycle%
    .txtNlev.Text = Nlev%
    .txtNlev1.Text = Nlev1%
    .txtNlev2.Text = Nlev2%
    .txtPitch.Text = Pitch
    .txtRoll.Text = Roll
    .txtHdg.Text = Heading
    .txtWdir.Text = Wdir
    .txtMRI.Text = Format$(MRI, "#0.00")
    .txtLR = LRac
  End With
    
'First Tropopause
If Zt1 < 99 Then
  If frmREFview.cmbZt.Text = "km" Then Zt = Zt1 Else Zt = Zt1 * kft_km
  frmREFview.txtZt1.Text = Format$(Zt, "##0.0")
  
  Select Case cmbTt
  Case "K": Tt = TT1
  Case "C": Tt = TT1 - 273.15
  Case "F": Tt = (TT1 - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTt1.Text = Format$(Tt, "##0.0")
    
  Select Case cmbTh
  Case "K": Tt = Th1
  Case "C": Tt = Th1 - 273.15
  Case "F": Tt = (Th1 - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTh1.Text = Format$(Tt, "##0.0")
  
  If frmREFview.cmbZt.Text = "km" Then Zt = Zt2 Else Zt = Zt2 * kft_km
  frmREFview.txtZt2.Text = Format$(Zt, "##0.0")
Else
  frmREFview.txtZt1.Text = "99.9": frmREFview.txtTt1.Text = "999.9": frmREFview.txtTh1.Text = "999.9"
End If
    
' Second Tropopause
If Zt2 < 99 Then
  If frmREFview.cmbZt.Text = "km" Then Zt = Zt2 Else Zt = Zt2 * kft_km
  frmREFview.txtZt2.Text = Format$(Zt, "##0.0")
  
  Select Case cmbTt
  Case "K": Tt = TT2
  Case "C": Tt = TT2 - 273.15
  Case "F": Tt = (TT2 - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTt2.Text = Format$(Tt, "##0.0")
    
  Select Case cmbTh
  Case "K": Tt = Th2
  Case "C": Tt = Th2 - 273.15
  Case "F": Tt = (Th2 - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTh2.Text = Format$(Tt, "##0.0")
Else
  frmREFview.txtZt2.Text = "": frmREFview.txtTt2.Text = "": frmREFview.txtTh2.Text = ""
End If

' Cold Point
If Zcp < 99 Then
  If frmREFview.cmbZt.Text = "km" Then Zt = Zcp Else Zt = Zt1 * kft_km
  frmREFview.txtZcp.Text = Format$(Zt, "##0.0")
  
  Select Case cmbTt
  Case "K": Tt = Tcp
  Case "C": Tt = Tcp - 273.15
  Case "F": Tt = (Tcp - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTcp.Text = Format$(Tt, "##0.0")
    
  Thcp = fTheta(Tcp, fZtoP(Zcp))
  Select Case cmbTh
  Case "K": Tt = Thcp
  Case "C": Tt = Thcp - 273.15
  Case "F": Tt = (Thcp - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtThcp.Text = Format$(Tt, "##0.0")
  
  If frmREFview.cmbZt.Text = "km" Then Zt = Zcp Else Zt = Zcp * kft_km
  frmREFview.txtZcp.Text = Format$(Zt, "##0.0")
Else
  frmREFview.txtZcp.Text = "": frmREFview.txtTcp.Text = "": frmREFview.txtThcp.Text = ""
End If


If pALT < 99 Then
  If frmREFview.cmbZt.Text = "km" Then Zt = pALT Else Zt = pALT * kft_km
  frmREFview.txtALTac.Text = Format$(Zt, "##0.0")
  
  Select Case cmbTt
  Case "K": Tt = OATmtp
  Case "C": Tt = OATmtp - 273.15
  Case "F": Tt = (OATmtp - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtTac.Text = Format$(Tt, "##0.0")
  frmREFview.txtOATnav.Text = Format$(OATnav, "##0.0")
  frmREFview.txtOATmms.Text = Format$(OATmms, "##0.0")
    
  ThAC = fTheta(OATmtp, fZtoP(pALT))
  Select Case cmbTh
  Case "K": Tt = ThAC
  Case "C": Tt = ThAC - 273.15
  Case "F": Tt = (ThAC - 273.15) * 9 / 5 + 32
  End Select
  frmREFview.txtThAC.Text = Format$(Tt, "##0.0")
Else
  frmREFview.txtALTac.Text = "": frmREFview.txtTac.Text = "": frmREFview.txtThAC.Text = ""
End If

' Write Engineering Temperatures
If EnableEng Then
    txtMux(0).Text = Format$(Muxs(0), "##0.0")
  For i% = 1 To 24
    txtMux(i%).Text = Format$(Muxs(i%), "##0.0")
    txtRux(i% - 1).Text = Format$(Mux(i%), "####0")
  Next i%
End If

' Write Gain Information
If EnableGains Then
  For i = 0 To Channels - 1: txtGain(i).Text = Format$(g(i + 1), "##0.0"): Next i
  For i = 3 To 2 + Channels: txtGain(i).Text = Format$(Geqn(i - 2), "##0.0"): Next i
  For i = 6 To 5 + Channels: txtGain(i).Text = Format$(Gnd(i - 5), "##0.0"): Next i
  For i = 9 To 8 + Channels: txtGain(i).Text = Format$(Goat(i - 8), "##0.0"): Next i
End If

If EnableCounts Then
  For i = 1 To Channels
    For j = 1 To 12: txtCounts(12 * (i - 1) + j - 1).Text = C(i, j): Next j
  Next i
End If
  
If EnableTA Then
  For i = 1 To Channels
    For j = 1 To 10: txtTA(10 * (i - 1) + j - 1).Text = TA(i, j): Next j
  Next i
  UpdateTBwindow
End If
  
If EnableATP Then
  For j% = 1 To 33
      txtZZZ(j% - 1).Text = Format$(sZZZ(j%), "#0.00")
      txtTTT(j% - 1).Text = Format$(sTTT(j%), "###.0")
      'Debug.Print Format$(sTTT(j%), "###.0") + " " + Format$(sZZZ(j%), "#0.00")
  Select Case j%
    Case Is < Nlev1
      txtZZZ(j% - 1).Enabled = False
      txtTTT(j% - 1).Enabled = False
    Case Is <= Nlev2
      If j% = 16 Then
        txtZZZ(j% - 1).BackColor = &HFFFFC0
        txtTTT(j% - 1).BackColor = &HFFFFC0
      Else
        txtZZZ(j% - 1).BackColor = &HFFFFFF
        txtTTT(j% - 1).BackColor = &HFFFFFF
      End If
   Case Else
      txtZZZ(j% - 1).Enabled = False
      txtTTT(j% - 1).Enabled = False
    End Select
  Next j%

End If

'  ProgressBar2.value = Loc(REFlu) - HiddenRecords - 1
' MAKEword Bitmap
' NavQual          1
' TAqual           2
' CtsQual          4
' CycleQual        8
' TtgtFix         16
' RAWbad          32
' Spare 1         64
' Spare 2        128
'
' EDITword Bitmap
' RetAlt           1
' Temperature      2
' OATtrop          4
' ZtOff            8
' TropAlt         16
' Pitch           32
' Roll            64
' Nlev < 8       128
'
MakeEditWord = EditWord * 2 ^ 8 + MakeWord

For i% = 0 To 16
  If MakeEditWord And 2 ^ i% Then chkWords(i%).Value = 1 Else chkWords(i%).Value = 0
Next i%
' Check AutoGoodScan and AutoGoodTrop

'  If chkAutoScan Then
    If MakeEditWord And mskBadScan Then AutoGoodScan = False Else AutoGoodScan = True
'  End If
'  If chkAutoTrop Then
    If MakeEditWord And mskBadTrop Then
      If MakeEditWord And &H400 Then    'Check for OAT trop
        AutoGoodTrop = True
      Else
        AutoGoodTrop = False
      End If
    Else
      AutoGoodTrop = True
    End If
'  End If
'Debug.Print Hex(MakeEditWord); " "; AutoGoodScan; GoodScan; AutoGoodTrop; GoodTrop
  If AutoGoodScan = GoodScan And AutoGoodTrop = GoodTrop Then
    lblMTP.ForeColor = &H800000
   Else
    lblMTP.ForeColor = &HFFFF&         'Turn amber if autochange
  End If
  
  For i = 0 To 1
    txtRCused1(i).Text = Reg(RCindex1)
    If TwoAAprofiles Then txtRCused2(i).Text = Reg(RCindex2) Else txtRCused2(i).Text = " "
  Next i
  DoEvents
  
'  cboAlgorithm.ListIndex = Algorithm
  lblAlgorithm = "Algo=" + Format(Algorithm, "0")
  
End Sub

