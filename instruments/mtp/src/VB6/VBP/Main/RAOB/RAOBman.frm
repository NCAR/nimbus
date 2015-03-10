VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Object = "{FE0065C0-1B7B-11CF-9D53-00AA003C9CB6}#1.1#0"; "COMCT232.OCX"
Begin VB.Form frmRAOBman 
   Caption         =   "RAOB Manager"
   ClientHeight    =   6960
   ClientLeft      =   60
   ClientTop       =   630
   ClientWidth     =   7140
   Icon            =   "RAOBman.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   6960
   ScaleWidth      =   7140
   Begin VB.CommandButton cmdToggleAC 
      Caption         =   "&A/C"
      Height          =   375
      Left            =   3780
      TabIndex        =   14
      Top             =   5970
      Width           =   555
   End
   Begin VB.CommandButton cmdToggleTrop 
      Caption         =   "&Trop"
      Height          =   375
      Left            =   3240
      TabIndex        =   13
      Top             =   5970
      Width           =   555
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   435
      Left            =   6480
      TabIndex        =   12
      Top             =   6465
      Width           =   615
   End
   Begin VB.TextBox txtFileRec 
      Height          =   285
      Left            =   5085
      TabIndex        =   11
      ToolTipText     =   "Number of records in open RAOB file"
      Top             =   5985
      Width           =   615
   End
   Begin VB.Frame Frame6 
      Caption         =   "RAOB GoTo"
      Height          =   615
      Left            =   0
      TabIndex        =   7
      Top             =   5805
      Width           =   3135
      Begin VB.TextBox txtRAOBgo 
         Height          =   285
         Left            =   60
         TabIndex        =   10
         Top             =   240
         Width           =   855
      End
      Begin VB.ComboBox cboRAOBgo 
         Height          =   315
         Left            =   900
         TabIndex        =   9
         Top             =   240
         Width           =   1575
      End
      Begin VB.CommandButton cmdRAOBgo 
         Caption         =   "Go"
         Height          =   315
         Left            =   2520
         TabIndex        =   8
         ToolTipText     =   "Go directly to selected location"
         Top             =   240
         Width           =   555
      End
   End
   Begin VB.TextBox txtProgress 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   6480
      MultiLine       =   -1  'True
      TabIndex        =   5
      ToolTipText     =   "Current record number"
      Top             =   5985
      Width           =   615
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "&Open RAOB"
      Height          =   435
      Left            =   3240
      TabIndex        =   4
      ToolTipText     =   "Display/Close RAOB file"
      Top             =   6465
      Width           =   1095
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   2070
      Top             =   6420
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Timer tmrGetCycle 
      Left            =   2610
      Top             =   6480
   End
   Begin VB.CommandButton cmdStop 
      Caption         =   "&Stop"
      Height          =   435
      Left            =   4980
      TabIndex        =   3
      ToolTipText     =   "Auto/Single step thru RAOB file"
      Top             =   6465
      Width           =   855
   End
   Begin VB.CommandButton cmdFwd 
      Caption         =   "&Fwd"
      Height          =   435
      Left            =   5835
      TabIndex        =   2
      ToolTipText     =   "Go Forward in RAOB file"
      Top             =   6465
      Width           =   615
   End
   Begin VB.CommandButton cmdBack 
      Caption         =   "&Back"
      Height          =   435
      Left            =   4365
      TabIndex        =   1
      ToolTipText     =   "Go Backward in RAOB file"
      Top             =   6465
      Width           =   615
   End
   Begin VB.TextBox txtRAOBfile 
      Height          =   345
      Left            =   0
      TabIndex        =   0
      Top             =   5415
      Width           =   7095
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   5385
      Left            =   30
      TabIndex        =   15
      Top             =   0
      Width           =   7095
      _ExtentX        =   12515
      _ExtentY        =   9499
      _Version        =   393216
      Tabs            =   8
      Tab             =   6
      TabsPerRow      =   8
      TabHeight       =   520
      TabCaption(0)   =   "&Filters"
      TabPicture(0)   =   "RAOBman.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Label(23)"
      Tab(0).Control(1)=   "Frame(15)"
      Tab(0).Control(2)=   "Frame(12)"
      Tab(0).Control(3)=   "chkNegateFilter"
      Tab(0).Control(4)=   "Frame(11)"
      Tab(0).Control(5)=   "Frame(1)"
      Tab(0).Control(6)=   "txtFilter(9)"
      Tab(0).Control(7)=   "Frame(6)"
      Tab(0).Control(8)=   "Frame(4)"
      Tab(0).Control(9)=   "Frame(7)"
      Tab(0).Control(10)=   "Frame(8)"
      Tab(0).Control(11)=   "Frame(5)"
      Tab(0).Control(12)=   "Frame(9)"
      Tab(0).Control(13)=   "Frame(3)"
      Tab(0).Control(14)=   "Frame(10)"
      Tab(0).Control(15)=   "Frame(2)"
      Tab(0).ControlCount=   16
      TabCaption(1)   =   "I/O"
      TabPicture(1)   =   "RAOBman.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "cmdStartConversion"
      Tab(1).Control(1)=   "Frame2"
      Tab(1).Control(2)=   "Frame4"
      Tab(1).Control(3)=   "Drive1"
      Tab(1).Control(4)=   "chkFindTrop"
      Tab(1).Control(5)=   "chkGeoPotHeight(0)"
      Tab(1).Control(6)=   "cboRAOBtype"
      Tab(1).Control(7)=   "cboRAOBtypeOUT"
      Tab(1).Control(8)=   "cmdGAL(0)"
      Tab(1).Control(9)=   "cmdGAL(1)"
      Tab(1).Control(10)=   "CommonDialog2"
      Tab(1).Control(11)=   "cdConvertIn"
      Tab(1).Control(12)=   "lblMakky"
      Tab(1).Control(13)=   "Label(3)"
      Tab(1).Control(14)=   "Label(72)"
      Tab(1).Control(15)=   "Label(73)"
      Tab(1).ControlCount=   16
      TabCaption(2)   =   "&Details"
      TabPicture(2)   =   "RAOBman.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "lblLoc"
      Tab(2).Control(1)=   "Label(76)"
      Tab(2).Control(2)=   "Label(51)"
      Tab(2).Control(3)=   "Label(50)"
      Tab(2).Control(4)=   "Label(42)"
      Tab(2).Control(5)=   "Label(41)"
      Tab(2).Control(6)=   "Label(40)"
      Tab(2).Control(7)=   "Label(39)"
      Tab(2).Control(8)=   "Label19"
      Tab(2).Control(9)=   "Label3(1)"
      Tab(2).Control(10)=   "Label3(0)"
      Tab(2).Control(11)=   "lblTrop"
      Tab(2).Control(12)=   "lblTime"
      Tab(2).Control(13)=   "lblSite"
      Tab(2).Control(14)=   "Label5"
      Tab(2).Control(15)=   "UpDown1"
      Tab(2).Control(16)=   "txtList(6)"
      Tab(2).Control(17)=   "cboWindow"
      Tab(2).Control(18)=   "txtDTacMraob"
      Tab(2).Control(19)=   "txtTac"
      Tab(2).Control(20)=   "txtTinterpolated"
      Tab(2).Control(21)=   "txtZp"
      Tab(2).Control(22)=   "cmdList(7)"
      Tab(2).Control(23)=   "txtLiquid2"
      Tab(2).Control(24)=   "txtLiquid1"
      Tab(2).Control(25)=   "txtBI2"
      Tab(2).Control(26)=   "txtIntRhoV2"
      Tab(2).Control(27)=   "cmdList(3)"
      Tab(2).Control(28)=   "cmdList(6)"
      Tab(2).Control(29)=   "cmdList(1)"
      Tab(2).Control(30)=   "cmdList(5)"
      Tab(2).Control(31)=   "cmdList(4)"
      Tab(2).Control(32)=   "txtMod(1)"
      Tab(2).Control(33)=   "txtMod(0)"
      Tab(2).Control(34)=   "txtUse(1)"
      Tab(2).Control(35)=   "txtUse(0)"
      Tab(2).Control(36)=   "txtGood(1)"
      Tab(2).Control(37)=   "txtGood(0)"
      Tab(2).Control(38)=   "cmdList(2)"
      Tab(2).Control(39)=   "cmdList(0)"
      Tab(2).Control(40)=   "txtList(5)"
      Tab(2).Control(41)=   "txtList(4)"
      Tab(2).Control(42)=   "txtList(3)"
      Tab(2).Control(43)=   "txtList(2)"
      Tab(2).Control(44)=   "txtList(1)"
      Tab(2).Control(45)=   "txtList(0)"
      Tab(2).Control(46)=   "lstListRAOB"
      Tab(2).ControlCount=   47
      TabCaption(3)   =   "&Select"
      TabPicture(3)   =   "RAOBman.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "lstSelect"
      Tab(3).Control(1)=   "cmdSelect(0)"
      Tab(3).Control(2)=   "cmdSelect(1)"
      Tab(3).Control(3)=   "cmdSelect(2)"
      Tab(3).Control(4)=   "cmdSelect(3)"
      Tab(3).Control(5)=   "cmdSelect(4)"
      Tab(3).Control(6)=   "chkAutoView"
      Tab(3).Control(7)=   "chkMultipleScans"
      Tab(3).Control(8)=   "txtTotalRec"
      Tab(3).Control(9)=   "cmdSelect(5)"
      Tab(3).Control(10)=   "cmdSelect(6)"
      Tab(3).Control(11)=   "cmdSelect(7)"
      Tab(3).Control(12)=   "cmdSelect(8)"
      Tab(3).Control(13)=   "cmdSelect(9)"
      Tab(3).Control(14)=   "txtColor(0)"
      Tab(3).Control(15)=   "txtThickness(0)"
      Tab(3).Control(16)=   "cmdSelect(10)"
      Tab(3).Control(17)=   "cmdSelect(11)"
      Tab(3).Control(18)=   "chkAvgWrite"
      Tab(3).Control(19)=   "cmdColdest"
      Tab(3).Control(20)=   "txtColdestAlt"
      Tab(3).Control(21)=   "txtColdestT"
      Tab(3).Control(22)=   "txtColdestRAOB"
      Tab(3).Control(23)=   "chkEnableAlt"
      Tab(3).Control(24)=   "chkNDP"
      Tab(3).Control(25)=   "chkEnableRH"
      Tab(3).Control(26)=   "cmdSelect(12)"
      Tab(3).Control(27)=   "cmdSelect(13)"
      Tab(3).Control(28)=   "cboTincrement"
      Tab(3).Control(29)=   "chkAutoSort"
      Tab(3).Control(30)=   "cmdSelect(14)"
      Tab(3).Control(31)=   "txtRMS"
      Tab(3).Control(32)=   "txtAVG"
      Tab(3).Control(33)=   "cmdSelect(15)"
      Tab(3).Control(34)=   "chkKeepSelection"
      Tab(3).Control(35)=   "chkSort"
      Tab(3).Control(36)=   "cmdSelect(16)"
      Tab(3).Control(37)=   "cmdSelect(17)"
      Tab(3).Control(38)=   "cmdSelect(18)"
      Tab(3).Control(39)=   "cmdSelect(19)"
      Tab(3).Control(40)=   "chkPlotT(2)"
      Tab(3).Control(41)=   "cmdSelect(20)"
      Tab(3).Control(42)=   "chkPlotT(3)"
      Tab(3).Control(43)=   "chkPlotT(0)"
      Tab(3).Control(44)=   "chkPlotT(1)"
      Tab(3).Control(45)=   "chkShowDots"
      Tab(3).Control(46)=   "chkUseBias"
      Tab(3).Control(47)=   "cmdSelect(21)"
      Tab(3).Control(48)=   "chkShowRMSprofile"
      Tab(3).Control(49)=   "cmdSelect(22)"
      Tab(3).Control(50)=   "cmdSelect(23)"
      Tab(3).Control(51)=   "cmdSelect(24)"
      Tab(3).Control(52)=   "cmdSelect(25)"
      Tab(3).Control(53)=   "chkDoubleTrop"
      Tab(3).Control(54)=   "cmdBias"
      Tab(3).Control(55)=   "cmdRMS"
      Tab(3).Control(56)=   "cmdSelect(26)"
      Tab(3).Control(57)=   "Frame(14)"
      Tab(3).Control(58)=   "Frame3"
      Tab(3).Control(59)=   "chkAddBias"
      Tab(3).Control(60)=   "UpDown3(0)"
      Tab(3).Control(61)=   "UpDown2(0)"
      Tab(3).Control(62)=   "Label6"
      Tab(3).Control(63)=   "Label(0)"
      Tab(3).Control(64)=   "Label(18)"
      Tab(3).Control(65)=   "Label(19)"
      Tab(3).Control(66)=   "Label(45)"
      Tab(3).Control(67)=   "Label(46)"
      Tab(3).Control(68)=   "Label(57)"
      Tab(3).Control(69)=   "Label(58)"
      Tab(3).ControlCount=   70
      TabCaption(4)   =   "&Export"
      TabPicture(4)   =   "RAOBman.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Label(56)"
      Tab(4).Control(1)=   "Label(48)"
      Tab(4).Control(2)=   "Label(4)"
      Tab(4).Control(3)=   "cmdRAOBrangeUsed"
      Tab(4).Control(4)=   "cmdTemplates"
      Tab(4).Control(5)=   "cmdRMSwrtFL"
      Tab(4).Control(6)=   "cmdRAOBrange"
      Tab(4).Control(7)=   "cmdNlevrMax"
      Tab(4).Control(8)=   "cmdRAOBdetails"
      Tab(4).Control(9)=   "cboMap"
      Tab(4).Control(10)=   "cmdColdestT"
      Tab(4).Control(11)=   "txtMTPpath"
      Tab(4).Control(12)=   "cmdRAOBcompare"
      Tab(4).Control(13)=   "Frame19"
      Tab(4).Control(14)=   "cmdTropList"
      Tab(4).Control(15)=   "cmdT1017"
      Tab(4).Control(16)=   "txtExportPath"
      Tab(4).Control(17)=   "cmdExport"
      Tab(4).Control(18)=   "Frame1"
      Tab(4).Control(19)=   "Frame8"
      Tab(4).Control(20)=   "Frame7"
      Tab(4).ControlCount=   21
      TabCaption(5)   =   "&Spare"
      TabPicture(5)   =   "RAOBman.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).ControlCount=   0
      TabCaption(6)   =   "&Blend"
      TabPicture(6)   =   "RAOBman.frx":04EA
      Tab(6).ControlEnabled=   -1  'True
      Tab(6).Control(0)=   "Label(83)"
      Tab(6).Control(0).Enabled=   0   'False
      Tab(6).Control(1)=   "Label(82)"
      Tab(6).Control(1).Enabled=   0   'False
      Tab(6).Control(2)=   "Label(81)"
      Tab(6).Control(2).Enabled=   0   'False
      Tab(6).Control(3)=   "Label(80)"
      Tab(6).Control(3).Enabled=   0   'False
      Tab(6).Control(4)=   "Label(79)"
      Tab(6).Control(4).Enabled=   0   'False
      Tab(6).Control(5)=   "Label(75)"
      Tab(6).Control(5).Enabled=   0   'False
      Tab(6).Control(6)=   "Label(67)"
      Tab(6).Control(6).Enabled=   0   'False
      Tab(6).Control(7)=   "Label(16)"
      Tab(6).Control(7).Enabled=   0   'False
      Tab(6).Control(8)=   "Label(20)"
      Tab(6).Control(8).Enabled=   0   'False
      Tab(6).Control(9)=   "Label(17)"
      Tab(6).Control(9).Enabled=   0   'False
      Tab(6).Control(10)=   "Label(15)"
      Tab(6).Control(10).Enabled=   0   'False
      Tab(6).Control(11)=   "Label(14)"
      Tab(6).Control(11).Enabled=   0   'False
      Tab(6).Control(12)=   "Label(12)"
      Tab(6).Control(12).Enabled=   0   'False
      Tab(6).Control(13)=   "Label(13)"
      Tab(6).Control(13).Enabled=   0   'False
      Tab(6).Control(14)=   "Label4"
      Tab(6).Control(14).Enabled=   0   'False
      Tab(6).Control(15)=   "Label(11)"
      Tab(6).Control(15).Enabled=   0   'False
      Tab(6).Control(16)=   "chkTcalCorr"
      Tab(6).Control(16).Enabled=   0   'False
      Tab(6).Control(17)=   "cmdBlend(10)"
      Tab(6).Control(17).Enabled=   0   'False
      Tab(6).Control(18)=   "txtWait"
      Tab(6).Control(18).Enabled=   0   'False
      Tab(6).Control(19)=   "cmdBlend(9)"
      Tab(6).Control(19).Enabled=   0   'False
      Tab(6).Control(20)=   "cmdBlend(8)"
      Tab(6).Control(20).Enabled=   0   'False
      Tab(6).Control(21)=   "txtAbsRoll"
      Tab(6).Control(21).Enabled=   0   'False
      Tab(6).Control(22)=   "txtMinZp"
      Tab(6).Control(22).Enabled=   0   'False
      Tab(6).Control(23)=   "txtDeltaZp"
      Tab(6).Control(23).Enabled=   0   'False
      Tab(6).Control(24)=   "txtLayerThickness"
      Tab(6).Control(24).Enabled=   0   'False
      Tab(6).Control(25)=   "txtMaxRange"
      Tab(6).Control(25).Enabled=   0   'False
      Tab(6).Control(26)=   "txtNavg"
      Tab(6).Control(26).Enabled=   0   'False
      Tab(6).Control(27)=   "txtPrefix"
      Tab(6).Control(27).Enabled=   0   'False
      Tab(6).Control(28)=   "chkWait"
      Tab(6).Control(28).Enabled=   0   'False
      Tab(6).Control(29)=   "chkWritePNGs"
      Tab(6).Control(29).Enabled=   0   'False
      Tab(6).Control(30)=   "Frame(13)"
      Tab(6).Control(30).Enabled=   0   'False
      Tab(6).Control(31)=   "txtDate2(0)"
      Tab(6).Control(31).Enabled=   0   'False
      Tab(6).Control(32)=   "txtDate2(1)"
      Tab(6).Control(32).Enabled=   0   'False
      Tab(6).Control(33)=   "optSite(0)"
      Tab(6).Control(33).Enabled=   0   'False
      Tab(6).Control(34)=   "optSite(1)"
      Tab(6).Control(34).Enabled=   0   'False
      Tab(6).Control(35)=   "txtDate1(0)"
      Tab(6).Control(35).Enabled=   0   'False
      Tab(6).Control(36)=   "txtDate1(1)"
      Tab(6).Control(36).Enabled=   0   'False
      Tab(6).Control(37)=   "cmdBlend(6)"
      Tab(6).Control(37).Enabled=   0   'False
      Tab(6).Control(38)=   "txtOATpp"
      Tab(6).Control(38).Enabled=   0   'False
      Tab(6).Control(39)=   "txtOATraob"
      Tab(6).Control(39).Enabled=   0   'False
      Tab(6).Control(40)=   "txtWeight(1)"
      Tab(6).Control(40).Enabled=   0   'False
      Tab(6).Control(41)=   "txtALTkm"
      Tab(6).Control(41).Enabled=   0   'False
      Tab(6).Control(42)=   "txtDistance(2)"
      Tab(6).Control(42).Enabled=   0   'False
      Tab(6).Control(43)=   "txtDistance(1)"
      Tab(6).Control(43).Enabled=   0   'False
      Tab(6).Control(44)=   "txtDistance(0)"
      Tab(6).Control(44).Enabled=   0   'False
      Tab(6).Control(45)=   "txtEventNo"
      Tab(6).Control(45).Enabled=   0   'False
      Tab(6).Control(46)=   "cmdBlend(5)"
      Tab(6).Control(46).Enabled=   0   'False
      Tab(6).Control(47)=   "txtLATn(2)"
      Tab(6).Control(47).Enabled=   0   'False
      Tab(6).Control(48)=   "txtLONn(2)"
      Tab(6).Control(48).Enabled=   0   'False
      Tab(6).Control(49)=   "txtLONn(1)"
      Tab(6).Control(49).Enabled=   0   'False
      Tab(6).Control(50)=   "txtLONn(0)"
      Tab(6).Control(50).Enabled=   0   'False
      Tab(6).Control(51)=   "txtLATn(1)"
      Tab(6).Control(51).Enabled=   0   'False
      Tab(6).Control(52)=   "txtLATn(0)"
      Tab(6).Control(52).Enabled=   0   'False
      Tab(6).Control(53)=   "cmdBlend(4)"
      Tab(6).Control(53).Enabled=   0   'False
      Tab(6).Control(54)=   "cmdBlend(3)"
      Tab(6).Control(54).Enabled=   0   'False
      Tab(6).Control(55)=   "txtWeight(0)"
      Tab(6).Control(55).Enabled=   0   'False
      Tab(6).Control(56)=   "cmdBlend(2)"
      Tab(6).Control(56).Enabled=   0   'False
      Tab(6).Control(57)=   "cmdBlend(1)"
      Tab(6).Control(57).Enabled=   0   'False
      Tab(6).Control(58)=   "txtFraction(1)"
      Tab(6).Control(58).Enabled=   0   'False
      Tab(6).Control(59)=   "txtRec2(1)"
      Tab(6).Control(59).Enabled=   0   'False
      Tab(6).Control(60)=   "txtRec1(1)"
      Tab(6).Control(60).Enabled=   0   'False
      Tab(6).Control(61)=   "txtEvent"
      Tab(6).Control(61).Enabled=   0   'False
      Tab(6).Control(62)=   "txtFraction(0)"
      Tab(6).Control(62).Enabled=   0   'False
      Tab(6).Control(63)=   "txtRec2(0)"
      Tab(6).Control(63).Enabled=   0   'False
      Tab(6).Control(64)=   "txtRec1(0)"
      Tab(6).Control(64).Enabled=   0   'False
      Tab(6).Control(65)=   "txtSite(1)"
      Tab(6).Control(65).Enabled=   0   'False
      Tab(6).Control(66)=   "txtSite(0)"
      Tab(6).Control(66).Enabled=   0   'False
      Tab(6).Control(67)=   "cmdBlend(0)"
      Tab(6).Control(67).Enabled=   0   'False
      Tab(6).Control(68)=   "lstEvent"
      Tab(6).Control(68).Enabled=   0   'False
      Tab(6).Control(69)=   "cmdBlend(11)"
      Tab(6).Control(69).Enabled=   0   'False
      Tab(6).Control(70)=   "chkRFF"
      Tab(6).Control(70).Enabled=   0   'False
      Tab(6).Control(71)=   "cmdBlend(12)"
      Tab(6).Control(71).Enabled=   0   'False
      Tab(6).Control(72)=   "cmdBlend(7)"
      Tab(6).Control(72).Enabled=   0   'False
      Tab(6).ControlCount=   73
      TabCaption(7)   =   "&ATP"
      TabPicture(7)   =   "RAOBman.frx":0506
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "Label(63)"
      Tab(7).Control(1)=   "Label(62)"
      Tab(7).Control(2)=   "Label(47)"
      Tab(7).Control(3)=   "Label(31)"
      Tab(7).Control(4)=   "Label(30)"
      Tab(7).Control(5)=   "Label(29)"
      Tab(7).Control(6)=   "Label(28)"
      Tab(7).Control(7)=   "UpDown2(1)"
      Tab(7).Control(8)=   "UpDown3(1)"
      Tab(7).Control(9)=   "ProgressBar2"
      Tab(7).Control(10)=   "cmdTES"
      Tab(7).Control(11)=   "cmdMLS"
      Tab(7).Control(12)=   "txtRCindex2"
      Tab(7).Control(13)=   "txtRCindex1"
      Tab(7).Control(14)=   "cmdDescription"
      Tab(7).Control(15)=   "txtDescription"
      Tab(7).Control(16)=   "chkSkipBadScans"
      Tab(7).Control(17)=   "chkTexAQS"
      Tab(7).Control(18)=   "txtCommLine"
      Tab(7).Control(19)=   "cmdGetXS"
      Tab(7).Control(20)=   "chkEnableFit"
      Tab(7).Control(21)=   "chkShowTropopause"
      Tab(7).Control(22)=   "cmdATP(2)"
      Tab(7).Control(23)=   "chkGeoPotHeight(1)"
      Tab(7).Control(24)=   "txtASCIIout"
      Tab(7).Control(25)=   "cmdATP(1)"
      Tab(7).Control(26)=   "txtColor(1)"
      Tab(7).Control(27)=   "txtThickness(1)"
      Tab(7).Control(28)=   "chkPlotSingleATPscan"
      Tab(7).Control(29)=   "cmdATP(0)"
      Tab(7).Control(30)=   "txtREFrecords"
      Tab(7).Control(31)=   "txtREFrecord"
      Tab(7).Control(32)=   "chkShowATP"
      Tab(7).Control(33)=   "tmrGetCycle2"
      Tab(7).Control(34)=   "cmdREFloc(1)"
      Tab(7).Control(35)=   "cmdREFloc(2)"
      Tab(7).Control(36)=   "cmdREFloc(0)"
      Tab(7).Control(37)=   "txtREFfile"
      Tab(7).Control(38)=   "cmdOpenREFfile"
      Tab(7).Control(39)=   "Frame10"
      Tab(7).ControlCount=   40
      Begin VB.CommandButton cmdBlend 
         Caption         =   "ATP Tcal"
         Height          =   405
         Index           =   7
         Left            =   5715
         TabIndex        =   433
         ToolTipText     =   "Write Averaged RAOB File as LRS file"
         Top             =   3015
         Width           =   1275
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "IWG FL Tcal"
         Height          =   375
         Index           =   12
         Left            =   5730
         TabIndex        =   432
         ToolTipText     =   "Calibrate NGV Tres and Tavi against RAOBs"
         Top             =   4140
         Width           =   1275
      End
      Begin VB.CheckBox chkRFF 
         Caption         =   "Use RFF File"
         Height          =   195
         Left            =   3780
         TabIndex        =   431
         Top             =   4650
         Width           =   1755
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Open &Tools"
         Height          =   375
         Index           =   11
         Left            =   5715
         TabIndex        =   430
         ToolTipText     =   "Calibrate NGV Tres and Tavi against RAOBs"
         Top             =   4545
         Width           =   1305
      End
      Begin VB.ListBox lstListRAOB 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1500
         ItemData        =   "RAOBman.frx":0522
         Left            =   -74880
         List            =   "RAOBman.frx":0524
         TabIndex        =   373
         Top             =   660
         Width           =   5715
      End
      Begin VB.CommandButton cmdStartConversion 
         Caption         =   "Start Conversion"
         Height          =   375
         Left            =   -69840
         TabIndex        =   372
         Top             =   4305
         Width           =   1815
      End
      Begin VB.Frame Frame 
         Caption         =   "Time"
         Height          =   1335
         Index           =   2
         Left            =   -74880
         TabIndex        =   357
         ToolTipText     =   "Select only RAOBs in time window"
         Top             =   360
         Width           =   3495
         Begin VB.ComboBox cmbEnd 
            Height          =   315
            Index           =   3
            Left            =   2760
            TabIndex        =   368
            Text            =   "13"
            Top             =   960
            Width           =   615
         End
         Begin VB.ComboBox cmbEnd 
            Height          =   315
            Index           =   2
            Left            =   2040
            TabIndex        =   367
            Text            =   "30"
            Top             =   960
            Width           =   615
         End
         Begin VB.ComboBox cmbEnd 
            Height          =   315
            Index           =   1
            Left            =   1320
            TabIndex        =   366
            Text            =   "11"
            Top             =   960
            Width           =   615
         End
         Begin VB.ComboBox cmbEnd 
            Height          =   315
            Index           =   0
            Left            =   480
            TabIndex        =   365
            Text            =   "1998"
            Top             =   960
            Width           =   735
         End
         Begin VB.ComboBox cmbStart 
            Height          =   315
            Index           =   3
            Left            =   2760
            TabIndex        =   364
            Text            =   "12"
            Top             =   600
            Width           =   615
         End
         Begin VB.ComboBox cmbStart 
            Height          =   315
            Index           =   2
            Left            =   2040
            TabIndex        =   363
            Text            =   "30"
            Top             =   600
            Width           =   615
         End
         Begin VB.ComboBox cmbStart 
            Height          =   315
            Index           =   1
            ItemData        =   "RAOBman.frx":0526
            Left            =   1320
            List            =   "RAOBman.frx":0528
            TabIndex        =   362
            Text            =   "10"
            Top             =   600
            Width           =   615
         End
         Begin VB.ComboBox cmbStart 
            Height          =   315
            Index           =   0
            Left            =   480
            TabIndex        =   361
            Text            =   "1998"
            Top             =   600
            Width           =   735
         End
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   0
            Left            =   120
            TabIndex        =   360
            Top             =   240
            Width           =   255
         End
         Begin VB.CheckBox chkMonthOnly 
            Caption         =   "Use Month Only"
            Height          =   255
            Left            =   1920
            TabIndex        =   359
            Top             =   120
            Value           =   2  'Grayed
            Width           =   1455
         End
         Begin VB.CheckBox chkHourOnly 
            Caption         =   "Use Hour Only"
            Height          =   255
            Left            =   600
            TabIndex        =   358
            Top             =   120
            Width           =   1335
         End
         Begin VB.Label Label16 
            Caption         =   "Start"
            Height          =   255
            Left            =   120
            TabIndex        =   371
            Top             =   600
            Width           =   375
         End
         Begin VB.Label Label17 
            Caption         =   "End"
            Height          =   255
            Left            =   120
            TabIndex        =   370
            Top             =   960
            Width           =   375
         End
         Begin VB.Label Label18 
            Caption         =   "    Year          Month       Day         Hour"
            Height          =   255
            Left            =   360
            TabIndex        =   369
            Top             =   360
            Width           =   2895
         End
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   0
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   356
         Top             =   2220
         Width           =   615
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   1
         Left            =   -74280
         MultiLine       =   -1  'True
         TabIndex        =   355
         Top             =   2220
         Width           =   855
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   2
         Left            =   -73440
         MultiLine       =   -1  'True
         TabIndex        =   354
         Top             =   2220
         Width           =   855
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   3
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   353
         Top             =   2220
         Width           =   855
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   4
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   352
         Top             =   2220
         Width           =   855
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   5
         Left            =   -70920
         MultiLine       =   -1  'True
         TabIndex        =   351
         Top             =   2220
         Width           =   735
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Refresh"
         Height          =   315
         Index           =   0
         Left            =   -68970
         TabIndex        =   350
         ToolTipText     =   "Refresh ATP"
         Top             =   1830
         Width           =   975
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "LR Line"
         Height          =   315
         Index           =   2
         Left            =   -68970
         TabIndex        =   349
         ToolTipText     =   "Show -2K/km line at selected level"
         Top             =   1470
         Width           =   975
      End
      Begin VB.Frame Frame7 
         Caption         =   "ASCII Out Mode "
         Height          =   525
         Left            =   -74880
         TabIndex        =   345
         Top             =   420
         Width           =   3615
         Begin VB.OptionButton optOutMode 
            Caption         =   "Individual Files"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   347
            Top             =   240
            Width           =   1335
         End
         Begin VB.OptionButton optOutMode 
            Caption         =   "Combined File"
            Height          =   255
            Index           =   1
            Left            =   1560
            TabIndex        =   346
            Top             =   240
            Width           =   1335
         End
      End
      Begin VB.Frame Frame8 
         Caption         =   "Out Format"
         Height          =   525
         Left            =   -71220
         TabIndex        =   340
         Top             =   420
         Width           =   3195
         Begin VB.OptionButton optOutFormat 
            Caption         =   "BLG"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   344
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton optOutFormat 
            Caption         =   "SJK"
            Height          =   255
            Index           =   1
            Left            =   780
            TabIndex        =   343
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton optOutFormat 
            Caption         =   "VB6"
            Height          =   255
            Index           =   2
            Left            =   1380
            TabIndex        =   342
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton optOutFormat 
            Caption         =   "Excel"
            Height          =   255
            Index           =   3
            Left            =   2040
            TabIndex        =   341
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Site"
         Height          =   2145
         Index           =   10
         Left            =   -74880
         TabIndex        =   332
         ToolTipText     =   "Site list"
         Top             =   2370
         Width           =   1815
         Begin VB.CommandButton cmdSite 
            Caption         =   "Add"
            Height          =   315
            Index           =   0
            Left            =   120
            TabIndex        =   339
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRSsite 
            CausesValidation=   0   'False
            Height          =   285
            HideSelection   =   0   'False
            Left            =   840
            TabIndex        =   338
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstSite 
            Height          =   1425
            ItemData        =   "RAOBman.frx":052A
            Left            =   840
            List            =   "RAOBman.frx":052C
            TabIndex        =   337
            Top             =   480
            Width           =   855
         End
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   1
            Left            =   120
            TabIndex        =   336
            Top             =   240
            Width           =   255
         End
         Begin VB.CommandButton cmdSite 
            Caption         =   "Sub"
            Height          =   315
            Index           =   1
            Left            =   120
            TabIndex        =   335
            Top             =   960
            Width           =   615
         End
         Begin VB.CommandButton cmdSite 
            Caption         =   "Clear"
            Height          =   315
            Index           =   2
            Left            =   120
            TabIndex        =   334
            Top             =   1320
            Width           =   615
         End
         Begin VB.CommandButton cmdSite 
            Caption         =   "RSL"
            Height          =   315
            Index           =   3
            Left            =   120
            TabIndex        =   333
            Top             =   1680
            Width           =   615
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "Input Mode"
         Height          =   615
         Left            =   -74880
         TabIndex        =   329
         Top             =   1080
         Width           =   3615
         Begin VB.OptionButton optMode 
            Caption         =   "Random (*.RAOB)"
            Height          =   255
            Index           =   1
            Left            =   1425
            TabIndex        =   331
            Top             =   240
            Width           =   1695
         End
         Begin VB.OptionButton optMode 
            Caption         =   "ASCII (*.SJK)"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   330
            Top             =   240
            Width           =   1335
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Region"
         Height          =   1095
         Index           =   3
         Left            =   -71340
         TabIndex        =   320
         ToolTipText     =   "Select only RAOBs in Lat/Lon area"
         Top             =   360
         Width           =   2325
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1620
            MultiLine       =   -1  'True
            TabIndex        =   325
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   324
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1620
            MultiLine       =   -1  'True
            TabIndex        =   323
            Top             =   390
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   322
            Top             =   390
            Width           =   615
         End
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   2
            Left            =   120
            TabIndex        =   321
            Top             =   180
            Width           =   255
         End
         Begin VB.Label Label13 
            Caption         =   "Longitude"
            Height          =   195
            Left            =   120
            TabIndex        =   328
            Top             =   780
            Width           =   705
         End
         Begin VB.Label Label1 
            Caption         =   "Latitude"
            Height          =   195
            Left            =   120
            TabIndex        =   327
            Top             =   450
            Width           =   585
         End
         Begin VB.Label Label20 
            Caption         =   "   From         To"
            Height          =   165
            Left            =   900
            TabIndex        =   326
            Top             =   150
            Width           =   1335
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "ASCII RAOB Input"
         Height          =   1335
         Left            =   -74880
         TabIndex        =   308
         Top             =   420
         Width           =   6855
         Begin VB.ComboBox cmbSrcFormat 
            Height          =   315
            ItemData        =   "RAOBman.frx":052E
            Left            =   720
            List            =   "RAOBman.frx":0530
            TabIndex        =   314
            Text            =   "Wyoming"
            Top             =   240
            Width           =   1095
         End
         Begin VB.TextBox txtFileMask 
            Height          =   285
            Left            =   5190
            TabIndex        =   313
            Text            =   "R*.*"
            Top             =   600
            Width           =   1575
         End
         Begin VB.TextBox txtPath 
            Height          =   285
            Left            =   720
            TabIndex        =   312
            Text            =   "C:\MTP\Data\NGV\TREX\RAOB\TXT\"
            Top             =   600
            Width           =   3345
         End
         Begin VB.CommandButton cmdConvertIn 
            Caption         =   "Input File"
            Height          =   255
            Left            =   120
            TabIndex        =   311
            ToolTipText     =   "Use to convert single file"
            Top             =   960
            Width           =   975
         End
         Begin VB.TextBox txtConvertIn 
            Height          =   285
            Left            =   1230
            TabIndex        =   310
            Top             =   960
            Width           =   5535
         End
         Begin VB.ComboBox cmbInputMode 
            Height          =   315
            Left            =   3885
            TabIndex        =   309
            Text            =   "Single File with many RAOBs"
            Top             =   240
            Width           =   2895
         End
         Begin VB.Label lblLabel 
            Caption         =   "Format"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   319
            Top             =   240
            Width           =   615
         End
         Begin VB.Label lblSource 
            Caption         =   "File Mask"
            Height          =   225
            Index           =   1
            Left            =   4155
            TabIndex        =   318
            Top             =   615
            Width           =   750
         End
         Begin VB.Label lblSource 
            Caption         =   "Path"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   317
            Top             =   600
            Width           =   495
         End
         Begin VB.Label lblLabel 
            Caption         =   "Mode"
            Height          =   255
            Index           =   1
            Left            =   3375
            TabIndex        =   316
            Top             =   255
            Width           =   495
         End
         Begin VB.Label Label2 
            Caption         =   "Label2"
            Height          =   135
            Left            =   480
            TabIndex        =   315
            Top             =   360
            Width           =   15
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "Random RAOB Output"
         Height          =   855
         Left            =   -74880
         TabIndex        =   302
         Top             =   3120
         Width           =   6855
         Begin VB.CommandButton cmdConvertOut 
            Caption         =   "Output File"
            Height          =   255
            Left            =   120
            TabIndex        =   306
            ToolTipText     =   "Select random output file"
            Top             =   480
            Width           =   975
         End
         Begin VB.TextBox txtConvertOut 
            Height          =   285
            Left            =   1245
            TabIndex        =   305
            Text            =   "C:\MTP\Data\NGV\TREX\RAOB\TREX.RAOB2"
            Top             =   480
            Width           =   5535
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "New File"
            Height          =   255
            Index           =   0
            Left            =   1200
            TabIndex        =   304
            Top             =   240
            Width           =   975
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "Append"
            Height          =   255
            Index           =   1
            Left            =   2160
            TabIndex        =   303
            Top             =   240
            Width           =   975
         End
         Begin VB.Label lblLabel 
            Caption         =   "Mode"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   307
            Top             =   240
            Width           =   495
         End
      End
      Begin VB.TextBox txtGood 
         BackColor       =   &H00008000&
         Height          =   285
         Index           =   0
         Left            =   -68250
         TabIndex        =   301
         Top             =   3120
         Width           =   255
      End
      Begin VB.TextBox txtGood 
         BackColor       =   &H000000FF&
         Height          =   285
         Index           =   1
         Left            =   -68250
         TabIndex        =   300
         Top             =   3480
         Width           =   255
      End
      Begin VB.TextBox txtUse 
         BackColor       =   &H00FFFFFF&
         Height          =   285
         Index           =   0
         Left            =   -68580
         TabIndex        =   299
         Top             =   3120
         Width           =   255
      End
      Begin VB.TextBox txtUse 
         Height          =   285
         Index           =   1
         Left            =   -68580
         TabIndex        =   298
         Top             =   3480
         Width           =   255
      End
      Begin VB.TextBox txtMod 
         ForeColor       =   &H00FFFFFF&
         Height          =   285
         Index           =   0
         Left            =   -68970
         TabIndex        =   297
         Top             =   3120
         Width           =   255
      End
      Begin VB.TextBox txtMod 
         Height          =   285
         Index           =   1
         Left            =   -68970
         TabIndex        =   296
         Top             =   3480
         Width           =   255
      End
      Begin VB.ListBox lstSelect 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2460
         ItemData        =   "RAOBman.frx":0532
         Left            =   -74880
         List            =   "RAOBman.frx":0534
         MultiSelect     =   2  'Extended
         TabIndex        =   295
         Top             =   660
         Width           =   3735
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Search"
         Height          =   375
         Index           =   0
         Left            =   -71055
         TabIndex        =   294
         ToolTipText     =   "Search Open RAOB file subject to Filters"
         Top             =   3540
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Write"
         Height          =   375
         Index           =   1
         Left            =   -71040
         TabIndex        =   293
         ToolTipText     =   "Write all list files to RAOB out file in I/O tab or Avg File using Export Tab if Write Avg Check Box set"
         Top             =   3150
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Remove"
         Height          =   375
         Index           =   2
         Left            =   -69990
         TabIndex        =   292
         ToolTipText     =   "Remove selected RAOB from list"
         Top             =   2370
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&View"
         Height          =   375
         Index           =   3
         Left            =   -69990
         TabIndex        =   291
         ToolTipText     =   "View RAOBs on list separated by T increment above"
         Top             =   2760
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Clear &List"
         Height          =   375
         Index           =   4
         Left            =   -71040
         TabIndex        =   290
         ToolTipText     =   "Clear list window"
         Top             =   2760
         Width           =   975
      End
      Begin VB.CheckBox chkAutoView 
         Caption         =   "Auto View on Selection"
         Height          =   195
         Left            =   -71040
         TabIndex        =   289
         ToolTipText     =   "Plot selected RAOB from list"
         Top             =   420
         Width           =   1995
      End
      Begin VB.CheckBox chkMultipleScans 
         Caption         =   "Plot Multiple Scans"
         Height          =   255
         Left            =   -71040
         TabIndex        =   288
         ToolTipText     =   "Plot successive selections on same plot"
         Top             =   660
         Width           =   2055
      End
      Begin VB.Frame Frame 
         Caption         =   "Valid RAOB Data Needed"
         Height          =   645
         Index           =   9
         Left            =   -74880
         TabIndex        =   279
         ToolTipText     =   "Minimum acceptable burst altitude"
         Top             =   1740
         Width           =   3495
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   3
            Left            =   2100
            TabIndex        =   285
            Top             =   240
            Width           =   255
         End
         Begin VB.ComboBox cmbAltMin 
            Height          =   315
            Left            =   2820
            TabIndex        =   284
            Text            =   "mb"
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtAltMin 
            Height          =   285
            Left            =   2340
            TabIndex        =   283
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtAltMax 
            Height          =   285
            Left            =   720
            TabIndex        =   282
            Top             =   240
            Width           =   495
         End
         Begin VB.ComboBox cmbAltMax 
            Height          =   315
            Left            =   1200
            TabIndex        =   281
            Text            =   "mb"
            Top             =   240
            Width           =   615
         End
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   14
            Left            =   480
            TabIndex        =   280
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "From"
            Height          =   165
            Index           =   77
            Left            =   60
            TabIndex        =   287
            Top             =   300
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "To"
            Height          =   165
            Index           =   78
            Left            =   1830
            TabIndex        =   286
            Top             =   300
            Width           =   195
         End
      End
      Begin VB.TextBox txtTotalRec 
         BackColor       =   &H0000FF00&
         Height          =   285
         Left            =   -71760
         TabIndex        =   278
         ToolTipText     =   "Total number of RAOBs in list"
         Top             =   3360
         Width           =   615
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Clear &Plot"
         Height          =   375
         Index           =   5
         Left            =   -71040
         TabIndex        =   277
         ToolTipText     =   "Clear plot window"
         Top             =   2370
         Width           =   975
      End
      Begin VB.Frame Frame 
         Caption         =   "T1017 [K]"
         Height          =   945
         Index           =   5
         Left            =   -71340
         TabIndex        =   267
         ToolTipText     =   "Temperature difference at 10 and 17 km"
         Top             =   1470
         Width           =   2325
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   4
            Left            =   120
            TabIndex        =   272
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtT1017 
            Height          =   285
            Index           =   0
            Left            =   840
            TabIndex        =   271
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtT1017 
            Height          =   285
            Index           =   1
            Left            =   1680
            TabIndex        =   270
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtT10 
            Height          =   285
            Left            =   840
            TabIndex        =   269
            Text            =   "10.0"
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtT17 
            Height          =   285
            Left            =   1680
            TabIndex        =   268
            Text            =   "17.0"
            Top             =   600
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "From"
            Height          =   165
            Index           =   1
            Left            =   420
            TabIndex        =   276
            Top             =   300
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "To"
            Height          =   165
            Index           =   2
            Left            =   1380
            TabIndex        =   275
            Top             =   330
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "Alts [km]"
            Height          =   195
            Index           =   7
            Left            =   180
            TabIndex        =   274
            Top             =   630
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "and"
            Height          =   195
            Index           =   8
            Left            =   1350
            TabIndex        =   273
            Top             =   690
            Width           =   345
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "IJKL"
         Height          =   585
         Index           =   8
         Left            =   -68970
         TabIndex        =   265
         ToolTipText     =   "Write if ijkl set"
         Top             =   450
         Width           =   915
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   5
            Left            =   120
            TabIndex        =   266
            Top             =   240
            Width           =   255
         End
      End
      Begin VB.CommandButton cmdExport 
         Caption         =   "Export RAOBs"
         Height          =   405
         Left            =   -69720
         TabIndex        =   264
         ToolTipText     =   "Export in BLG or SJK format"
         Top             =   2610
         Width           =   1695
      End
      Begin VB.DriveListBox Drive1 
         Height          =   315
         Left            =   -73650
         TabIndex        =   263
         Top             =   1830
         Width           =   3105
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Templates"
         Height          =   375
         Index           =   6
         Left            =   -71040
         TabIndex        =   262
         ToolTipText     =   "Clear plot window"
         Top             =   1980
         Width           =   975
      End
      Begin VB.TextBox txtExportPath 
         Height          =   285
         Left            =   -74070
         TabIndex        =   261
         Text            =   "C:\MTP\Data\NGV\TREX\RAOB\"
         Top             =   3420
         Width           =   3375
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Find Trop"
         Height          =   315
         Index           =   4
         Left            =   -68970
         TabIndex        =   260
         ToolTipText     =   "Show -2K/km line at selected level"
         Top             =   1110
         Width           =   975
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB"
         Height          =   585
         Index           =   7
         Left            =   -68970
         TabIndex        =   257
         Top             =   1140
         Width           =   945
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   6
            Left            =   120
            TabIndex        =   258
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "OK"
            Height          =   255
            Index           =   5
            Left            =   360
            TabIndex        =   259
            Top             =   240
            Width           =   255
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Trop"
         Height          =   1185
         Index           =   4
         Left            =   -71160
         TabIndex        =   250
         Top             =   2730
         Width           =   645
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   7
            Left            =   120
            TabIndex        =   253
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtTropMin 
            Height          =   285
            Left            =   210
            TabIndex        =   252
            Text            =   "0.0"
            Top             =   600
            Width           =   375
         End
         Begin VB.TextBox txtTropMax 
            Height          =   285
            Left            =   210
            TabIndex        =   251
            Text            =   "99."
            Top             =   840
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "OK"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   256
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   ">"
            Height          =   255
            Index           =   21
            Left            =   90
            TabIndex        =   255
            Top             =   600
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "<"
            Height          =   255
            Index           =   22
            Left            =   90
            TabIndex        =   254
            Top             =   840
            Width           =   135
         End
      End
      Begin VB.CheckBox chkFindTrop 
         Caption         =   "Find Tropopause"
         Height          =   255
         Left            =   -69600
         TabIndex        =   249
         Top             =   1830
         Value           =   1  'Checked
         Width           =   1575
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Auto T Fix"
         Height          =   315
         Index           =   5
         Left            =   -68970
         TabIndex        =   248
         ToolTipText     =   "Show -2K/km line at selected level"
         Top             =   2190
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Auto T Fix"
         Height          =   375
         Index           =   7
         Left            =   -69990
         TabIndex        =   247
         ToolTipText     =   "Clear plot window"
         Top             =   1980
         Width           =   975
      End
      Begin VB.CommandButton cmdT1017 
         Caption         =   "Export T1017 File"
         Height          =   375
         Left            =   -69720
         TabIndex        =   246
         Top             =   1380
         Width           =   1695
      End
      Begin VB.Frame Frame 
         Caption         =   "MRI"
         Height          =   585
         Index           =   6
         Left            =   -72960
         TabIndex        =   240
         Top             =   3930
         Width           =   2445
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   8
            Left            =   120
            TabIndex        =   243
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtMRI 
            Height          =   285
            Index           =   0
            Left            =   900
            TabIndex        =   242
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtMRI 
            Height          =   285
            Index           =   1
            Left            =   1650
            TabIndex        =   241
            Top             =   240
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "From"
            Height          =   165
            Index           =   9
            Left            =   480
            TabIndex        =   245
            Top             =   300
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "To"
            Height          =   165
            Index           =   10
            Left            =   1410
            TabIndex        =   244
            Top             =   300
            Width           =   195
         End
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "T1"
         Height          =   375
         Index           =   8
         Left            =   -71025
         TabIndex        =   239
         ToolTipText     =   "Select First RAOB"
         Top             =   1560
         Width           =   465
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "T2"
         Height          =   375
         Index           =   9
         Left            =   -70530
         TabIndex        =   238
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   465
      End
      Begin VB.ListBox lstEvent 
         Height          =   1035
         ItemData        =   "RAOBman.frx":0536
         Left            =   120
         List            =   "RAOBman.frx":0538
         TabIndex        =   237
         Top             =   2040
         Width           =   1965
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   " &Import Events"
         Height          =   375
         Index           =   0
         Left            =   120
         TabIndex        =   236
         ToolTipText     =   "Read ELL File from MTPview"
         Top             =   3120
         Width           =   1965
      End
      Begin VB.TextBox txtSite 
         Height          =   285
         Index           =   0
         Left            =   330
         MultiLine       =   -1  'True
         TabIndex        =   235
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtSite 
         Height          =   285
         Index           =   1
         Left            =   330
         MultiLine       =   -1  'True
         TabIndex        =   234
         Top             =   1080
         Width           =   615
      End
      Begin VB.TextBox txtRec1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   4140
         MultiLine       =   -1  'True
         TabIndex        =   233
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtRec2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   4650
         MultiLine       =   -1  'True
         TabIndex        =   232
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtFraction 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   5190
         MultiLine       =   -1  'True
         TabIndex        =   231
         Top             =   720
         Width           =   465
      End
      Begin VB.TextBox txtEvent 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   120
         TabIndex        =   230
         Top             =   1680
         Width           =   1965
      End
      Begin VB.TextBox txtRec1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   4140
         MultiLine       =   -1  'True
         TabIndex        =   229
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtRec2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   4650
         MultiLine       =   -1  'True
         TabIndex        =   228
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtFraction 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   5190
         MultiLine       =   -1  'True
         TabIndex        =   227
         Top             =   1080
         Width           =   465
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Temporal Avg"
         Height          =   375
         Index           =   1
         Left            =   5730
         TabIndex        =   226
         ToolTipText     =   "Temporally Average Station 1 RAOBS"
         Top             =   630
         Width           =   1275
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Temporal Avg"
         Height          =   375
         Index           =   2
         Left            =   5730
         TabIndex        =   225
         ToolTipText     =   "Temporally Average Station 2 RAOBs"
         Top             =   1020
         Width           =   1275
      End
      Begin VB.TextBox txtWeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   5190
         MultiLine       =   -1  'True
         TabIndex        =   224
         Top             =   1680
         Width           =   465
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "&Spatial Avg"
         Height          =   375
         Index           =   3
         Left            =   5730
         TabIndex        =   223
         ToolTipText     =   "Perform Spatial Averaging"
         Top             =   1800
         Width           =   1275
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Write RAOB"
         Height          =   405
         Index           =   4
         Left            =   5730
         TabIndex        =   222
         ToolTipText     =   "Write Averaged RAOB File as LRS file"
         Top             =   2190
         Width           =   1275
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   2970
         MultiLine       =   -1  'True
         TabIndex        =   221
         Top             =   1680
         Width           =   735
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   2970
         MultiLine       =   -1  'True
         TabIndex        =   220
         Top             =   1965
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   3810
         MultiLine       =   -1  'True
         TabIndex        =   219
         Top             =   1680
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   3810
         MultiLine       =   -1  'True
         TabIndex        =   218
         Top             =   1965
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   3810
         MultiLine       =   -1  'True
         TabIndex        =   217
         Top             =   2250
         Width           =   735
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   2970
         MultiLine       =   -1  'True
         TabIndex        =   216
         Top             =   2250
         Width           =   735
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Calc Weights"
         Height          =   375
         Index           =   5
         Left            =   5730
         TabIndex        =   215
         ToolTipText     =   "Figure out spatial weighting factor"
         Top             =   1410
         Width           =   1275
      End
      Begin VB.TextBox txtEventNo 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5040
         MultiLine       =   -1  'True
         TabIndex        =   214
         ToolTipText     =   "Record Number to be Written"
         Top             =   2820
         Width           =   585
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   4650
         MultiLine       =   -1  'True
         TabIndex        =   213
         Top             =   1680
         Width           =   495
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   4650
         MultiLine       =   -1  'True
         TabIndex        =   212
         Top             =   1965
         Width           =   495
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   4650
         MultiLine       =   -1  'True
         TabIndex        =   211
         Top             =   2250
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Height          =   285
         Index           =   0
         Left            =   -70680
         TabIndex        =   210
         Text            =   "13"
         Top             =   960
         Width           =   375
      End
      Begin VB.TextBox txtThickness 
         Height          =   285
         Index           =   0
         Left            =   -69480
         TabIndex        =   209
         Text            =   "0"
         Top             =   960
         Width           =   255
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "BG"
         Height          =   375
         Index           =   10
         Left            =   -69990
         TabIndex        =   206
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   375
      End
      Begin VB.TextBox txtALTkm 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2970
         MultiLine       =   -1  'True
         TabIndex        =   205
         Top             =   2535
         Width           =   735
      End
      Begin VB.TextBox txtWeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   5190
         MultiLine       =   -1  'True
         TabIndex        =   204
         Top             =   1965
         Width           =   465
      End
      Begin VB.TextBox txtOATraob 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2970
         MultiLine       =   -1  'True
         TabIndex        =   203
         Top             =   2820
         Width           =   735
      End
      Begin VB.TextBox txtOATpp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3810
         MultiLine       =   -1  'True
         TabIndex        =   202
         Top             =   2820
         Width           =   405
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Save Edit"
         Height          =   315
         Index           =   1
         Left            =   -68970
         TabIndex        =   201
         ToolTipText     =   "Refresh ATP"
         Top             =   390
         Width           =   975
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Undo Edit"
         Height          =   315
         Index           =   6
         Left            =   -68970
         TabIndex        =   200
         ToolTipText     =   "Show -2K/km line at selected level"
         Top             =   2550
         Width           =   975
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "Save Trop"
         Height          =   315
         Index           =   3
         Left            =   -68970
         TabIndex        =   199
         ToolTipText     =   "Refresh ATP"
         Top             =   750
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Avg"
         Height          =   375
         Index           =   11
         Left            =   -69510
         TabIndex        =   198
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   495
      End
      Begin VB.CheckBox chkAvgWrite 
         Caption         =   "WriteAvg"
         Height          =   255
         Left            =   -69960
         TabIndex        =   197
         Top             =   1320
         Width           =   975
      End
      Begin VB.CommandButton cmdTropList 
         Caption         =   "Export Select List"
         Height          =   405
         Left            =   -69720
         TabIndex        =   196
         ToolTipText     =   "Export Site, Lat, Lon & Trops"
         Top             =   1770
         Width           =   1695
      End
      Begin VB.CheckBox chkGeoPotHeight 
         Caption         =   "Use GP Height"
         Height          =   255
         Index           =   0
         Left            =   -69600
         TabIndex        =   195
         Top             =   2130
         Value           =   1  'Checked
         Width           =   1455
      End
      Begin VB.Frame Frame10 
         Caption         =   "ATP GoTo"
         Height          =   615
         Left            =   -74880
         TabIndex        =   191
         Top             =   3120
         Width           =   2775
         Begin VB.TextBox txtATPgo 
            Height          =   285
            Left            =   120
            TabIndex        =   194
            Top             =   240
            Width           =   855
         End
         Begin VB.ComboBox cboATPgo 
            Height          =   315
            Left            =   945
            TabIndex        =   193
            Text            =   "Rec No"
            Top             =   240
            Width           =   1335
         End
         Begin VB.CommandButton cmdATPgo 
            Caption         =   "&Go"
            Height          =   495
            Left            =   2295
            TabIndex        =   192
            Top             =   90
            Width           =   495
         End
      End
      Begin VB.CommandButton cmdOpenREFfile 
         Caption         =   "Open REF"
         Height          =   345
         Left            =   -72060
         TabIndex        =   190
         ToolTipText     =   "Open REF file"
         Top             =   3120
         Width           =   1215
      End
      Begin VB.TextBox txtREFfile 
         Height          =   285
         Left            =   -74880
         TabIndex        =   189
         Top             =   2760
         Width           =   4455
      End
      Begin VB.CommandButton cmdREFloc 
         Caption         =   "&Back"
         Height          =   615
         Index           =   0
         Left            =   -70080
         TabIndex        =   187
         Top             =   3180
         Width           =   615
      End
      Begin VB.CommandButton cmdREFloc 
         Caption         =   "&Fwd"
         Height          =   615
         Index           =   2
         Left            =   -68640
         TabIndex        =   186
         Top             =   3180
         Width           =   615
      End
      Begin VB.CommandButton cmdREFloc 
         Caption         =   "&Stop"
         Height          =   615
         Index           =   1
         Left            =   -69480
         TabIndex        =   185
         Top             =   3180
         Width           =   855
      End
      Begin VB.Timer tmrGetCycle2 
         Left            =   -71760
         Top             =   1560
      End
      Begin VB.CheckBox chkShowATP 
         Caption         =   "Show ATP"
         Height          =   255
         Left            =   -74880
         TabIndex        =   184
         Top             =   1680
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.TextBox txtREFrecord 
         Height          =   285
         Left            =   -69720
         TabIndex        =   183
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtREFrecords 
         Height          =   285
         Left            =   -68640
         TabIndex        =   182
         Top             =   2760
         Width           =   615
      End
      Begin VB.CommandButton cmdATP 
         Caption         =   "Transfer ATP"
         Height          =   375
         Index           =   0
         Left            =   -72060
         TabIndex        =   181
         ToolTipText     =   "Transfer ATP to RAOB plot"
         Top             =   3480
         Width           =   1215
      End
      Begin VB.CheckBox chkPlotSingleATPscan 
         Caption         =   "Plot Single ATP scan"
         Height          =   255
         Left            =   -73680
         TabIndex        =   180
         Top             =   1680
         Width           =   1815
      End
      Begin VB.TextBox txtThickness 
         Height          =   285
         Index           =   1
         Left            =   -68520
         TabIndex        =   179
         Text            =   "0"
         Top             =   480
         Width           =   255
      End
      Begin VB.TextBox txtColor 
         Height          =   315
         Index           =   1
         Left            =   -70080
         TabIndex        =   178
         Text            =   "13"
         Top             =   480
         Width           =   375
      End
      Begin VB.CommandButton cmdATP 
         Caption         =   "Write Selected List to ASCII File"
         Height          =   375
         Index           =   1
         Left            =   -70440
         TabIndex        =   177
         Top             =   1320
         Width           =   2415
      End
      Begin VB.TextBox txtASCIIout 
         Height          =   285
         Left            =   -74880
         TabIndex        =   176
         Top             =   1320
         Width           =   4095
      End
      Begin VB.CheckBox chkGeoPotHeight 
         Caption         =   "Use Geopotential Height"
         Height          =   255
         Index           =   1
         Left            =   -74880
         TabIndex        =   175
         Top             =   480
         Value           =   1  'Checked
         Width           =   2055
      End
      Begin VB.Frame Frame19 
         Caption         =   "Write Average Options"
         Height          =   1575
         Left            =   -74880
         TabIndex        =   160
         Top             =   1800
         Width           =   4215
         Begin VB.TextBox txtAvgOpt 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   170
            Text            =   "RAOBman.frx":053A
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtAvgOpt 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   169
            Text            =   "RAOBman.frx":053F
            Top             =   480
            Width           =   495
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "T [K]"
            Height          =   255
            Index           =   0
            Left            =   1680
            TabIndex        =   168
            Top             =   240
            Width           =   735
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "gALT"
            Height          =   255
            Index           =   1
            Left            =   1680
            TabIndex        =   167
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "pALT"
            Height          =   255
            Index           =   2
            Left            =   2520
            TabIndex        =   166
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "Trms"
            Height          =   255
            Index           =   3
            Left            =   3240
            TabIndex        =   165
            Top             =   480
            Width           =   735
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "ND"
            Height          =   255
            Index           =   4
            Left            =   2520
            TabIndex        =   164
            Top             =   240
            Width           =   615
         End
         Begin VB.CheckBox chkAvgOpt 
            Caption         =   "dND%"
            Height          =   255
            Index           =   5
            Left            =   3240
            TabIndex        =   163
            Top             =   240
            Width           =   855
         End
         Begin VB.ComboBox cboAvgType 
            Height          =   315
            Left            =   1080
            TabIndex        =   162
            Top             =   1200
            Width           =   1335
         End
         Begin VB.TextBox txtAvgOpt 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   161
            Text            =   "RAOBman.frx":0545
            Top             =   720
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "Max Alt [km]"
            Height          =   255
            Index           =   32
            Left            =   120
            TabIndex        =   174
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Alt Step [km]"
            Height          =   255
            Index           =   33
            Left            =   120
            TabIndex        =   173
            Top             =   480
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Selection"
            Height          =   255
            Index           =   34
            Left            =   120
            TabIndex        =   172
            Top             =   1200
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Min Alt [km]"
            Height          =   255
            Index           =   61
            Left            =   120
            TabIndex        =   171
            Top             =   720
            Width           =   975
         End
      End
      Begin VB.CommandButton cmdATP 
         Caption         =   "Write Sampled ATP"
         Height          =   375
         Index           =   2
         Left            =   -70440
         TabIndex        =   159
         Top             =   960
         Width           =   2415
      End
      Begin VB.CheckBox chkShowTropopause 
         Caption         =   "Show Tropopause"
         Height          =   255
         Left            =   -74880
         TabIndex        =   158
         Top             =   720
         Value           =   1  'Checked
         Width           =   2295
      End
      Begin VB.CheckBox chkEnableFit 
         Caption         =   "Do Spline Fit"
         Height          =   255
         Left            =   -74880
         TabIndex        =   157
         Top             =   960
         Value           =   1  'Checked
         Width           =   2295
      End
      Begin VB.TextBox txtFilter 
         BackColor       =   &H000000FF&
         Height          =   285
         Index           =   9
         Left            =   -70440
         TabIndex        =   156
         Top             =   4170
         Width           =   255
      End
      Begin VB.CommandButton cmdGetXS 
         Caption         =   "Plot XS Profile"
         Height          =   375
         Left            =   -70455
         TabIndex        =   155
         Top             =   1665
         Width           =   2415
      End
      Begin VB.TextBox txtIntRhoV2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73680
         MultiLine       =   -1  'True
         TabIndex        =   154
         Top             =   4095
         Width           =   495
      End
      Begin VB.TextBox txtBI2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   153
         Top             =   4095
         Width           =   495
      End
      Begin VB.TextBox txtLiquid1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73680
         MultiLine       =   -1  'True
         TabIndex        =   152
         Top             =   4395
         Width           =   495
      End
      Begin VB.TextBox txtLiquid2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72600
         MultiLine       =   -1  'True
         TabIndex        =   151
         Top             =   4395
         Width           =   495
      End
      Begin VB.CommandButton cmdColdest 
         Caption         =   "Tcoldest"
         Height          =   375
         Left            =   -74880
         TabIndex        =   150
         ToolTipText     =   "Find Coldest T in entire list. Default is Tcoldest for selected RAOB"
         Top             =   4320
         Width           =   795
      End
      Begin VB.TextBox txtColdestAlt 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74040
         MultiLine       =   -1  'True
         TabIndex        =   149
         Top             =   4350
         Width           =   435
      End
      Begin VB.TextBox txtColdestT 
         Alignment       =   1  'Right Justify
         Enabled         =   0   'False
         Height          =   285
         Left            =   -73320
         MultiLine       =   -1  'True
         TabIndex        =   148
         Top             =   4350
         Width           =   585
      End
      Begin VB.TextBox txtColdestRAOB 
         Height          =   720
         Left            =   -71040
         TabIndex        =   147
         Top             =   3960
         Width           =   3045
      End
      Begin VB.CheckBox chkEnableAlt 
         Caption         =   "Enable Alt"
         Height          =   195
         Left            =   -74130
         TabIndex        =   146
         Top             =   3690
         Value           =   2  'Grayed
         Width           =   1095
      End
      Begin VB.CheckBox chkNDP 
         Caption         =   "NDP"
         Height          =   195
         Left            =   -74880
         TabIndex        =   145
         ToolTipText     =   "Show Number Density Profile"
         Top             =   3900
         Width           =   675
      End
      Begin VB.Frame Frame 
         Caption         =   "UT Window"
         Height          =   615
         Index           =   1
         Left            =   -72990
         TabIndex        =   141
         Top             =   2400
         Width           =   1815
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   10
            Left            =   120
            TabIndex        =   144
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtUT1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   143
            Text            =   "RAOBman.frx":054B
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtUT2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   142
            Text            =   "RAOBman.frx":054F
            Top             =   240
            Width           =   495
         End
      End
      Begin VB.TextBox txtCommLine 
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
         Left            =   -74880
         TabIndex        =   140
         Top             =   4080
         Width           =   6855
      End
      Begin VB.CheckBox chkTexAQS 
         Caption         =   "Use TexAQS Scales"
         Height          =   255
         Left            =   -72480
         TabIndex        =   139
         Top             =   720
         Width           =   2055
      End
      Begin VB.CheckBox chkSkipBadScans 
         Caption         =   "Skip Bad Scans"
         Height          =   255
         Left            =   -72480
         TabIndex        =   138
         Top             =   480
         Width           =   1455
      End
      Begin VB.CommandButton cmdRAOBcompare 
         Caption         =   "RAOB Comparison"
         Height          =   390
         Left            =   -69720
         TabIndex        =   137
         Top             =   3810
         Width           =   1695
      End
      Begin VB.TextBox txtMTPpath 
         Height          =   285
         Left            =   -74040
         TabIndex        =   136
         Text            =   "C:\DC8\CAMEX4\"
         Top             =   4020
         Width           =   2175
      End
      Begin VB.Frame Frame 
         Caption         =   "Low Level Inversion"
         Height          =   585
         Index           =   11
         Left            =   -72960
         TabIndex        =   133
         Top             =   3330
         Width           =   1785
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   11
            Left            =   120
            TabIndex        =   134
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "Maximum Altitude"
            Height          =   195
            Index           =   49
            Left            =   480
            TabIndex        =   135
            Top             =   270
            Width           =   1245
         End
      End
      Begin VB.CheckBox chkNegateFilter 
         Caption         =   "Negate Filter"
         Height          =   255
         Left            =   -69180
         TabIndex        =   132
         ToolTipText     =   "Note that this works with only 1 filter selected"
         Top             =   4230
         Width           =   1215
      End
      Begin VB.CommandButton cmdList 
         Caption         =   "T @ Zp"
         Height          =   315
         Index           =   7
         Left            =   -72960
         TabIndex        =   131
         ToolTipText     =   "Interpolate to Zp altitude in Edit Box"
         Top             =   3690
         Width           =   855
      End
      Begin VB.TextBox txtZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74280
         MultiLine       =   -1  'True
         TabIndex        =   130
         Text            =   "RAOBman.frx":0553
         Top             =   3690
         Width           =   615
      End
      Begin VB.TextBox txtTinterpolated 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72120
         MultiLine       =   -1  'True
         TabIndex        =   129
         Top             =   3690
         Width           =   615
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Compare ATP"
         Height          =   405
         Index           =   6
         Left            =   5715
         TabIndex        =   128
         ToolTipText     =   "Write Averaged RAOB File as LRS file"
         Top             =   2610
         Width           =   1275
      End
      Begin VB.CheckBox chkEnableRH 
         Caption         =   "Enable RH"
         Height          =   195
         Left            =   -74130
         TabIndex        =   127
         Top             =   3900
         Width           =   1095
      End
      Begin VB.Frame Frame 
         Caption         =   "Coldest T"
         Height          =   975
         Index           =   12
         Left            =   -70440
         TabIndex        =   118
         Top             =   2940
         Width           =   2415
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   12
            Left            =   120
            TabIndex        =   122
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtColdThreshold 
            Height          =   285
            Left            =   840
            TabIndex        =   121
            Text            =   "200"
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtZcoldest2 
            Height          =   285
            Left            =   1800
            TabIndex        =   120
            Text            =   "17.0"
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtZcoldest1 
            Height          =   285
            Left            =   840
            TabIndex        =   119
            Text            =   "15.0"
            Top             =   600
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "T <"
            Height          =   255
            Index           =   52
            Left            =   480
            TabIndex        =   126
            Top             =   240
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "K"
            Height          =   255
            Index           =   53
            Left            =   1440
            TabIndex        =   125
            Top             =   240
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "and"
            Height          =   255
            Index           =   54
            Left            =   1440
            TabIndex        =   124
            Top             =   600
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "Alts [km]"
            Height          =   255
            Index           =   55
            Left            =   120
            TabIndex        =   123
            Top             =   600
            Width           =   615
         End
      End
      Begin VB.CommandButton cmdColdestT 
         Caption         =   "Add Coldest T"
         Height          =   405
         Left            =   -69720
         TabIndex        =   117
         ToolTipText     =   "Recalculate Zcoldest, Tcoldest, dTMRI"
         Top             =   2190
         Width           =   1695
      End
      Begin VB.ComboBox cboMap 
         Height          =   315
         Left            =   -73320
         TabIndex        =   116
         Text            =   "North America"
         Top             =   4380
         Width           =   1455
      End
      Begin VB.CommandButton cmdRAOBdetails 
         Caption         =   "Show RAOB Details"
         Height          =   480
         Left            =   -71745
         TabIndex        =   115
         Top             =   4200
         Width           =   1830
      End
      Begin VB.TextBox txtTac 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   114
         Top             =   3690
         Width           =   615
      End
      Begin VB.TextBox txtDTacMraob 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70500
         MultiLine       =   -1  'True
         TabIndex        =   113
         Top             =   3690
         Width           =   495
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Sort"
         Height          =   375
         Index           =   12
         Left            =   -68940
         TabIndex        =   112
         ToolTipText     =   "Sort selected RAOBs from earliest to latest"
         Top             =   1980
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Plot &All"
         Height          =   375
         Index           =   13
         Left            =   -68940
         TabIndex        =   111
         ToolTipText     =   "Plot all the selected with T offset given above"
         Top             =   2370
         Width           =   975
      End
      Begin VB.ComboBox cboTincrement 
         Height          =   315
         Left            =   -68940
         TabIndex        =   110
         Text            =   "1"
         Top             =   1020
         Width           =   690
      End
      Begin VB.CheckBox chkAutoSort 
         Caption         =   "Auto Sort"
         Height          =   225
         Left            =   -69000
         TabIndex        =   109
         Top             =   360
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "RMS "
         Height          =   375
         Index           =   14
         Left            =   -68940
         TabIndex        =   108
         ToolTipText     =   "Select all RAOBs from list which are have an RMS less than criteria above"
         Top             =   3150
         Width           =   975
      End
      Begin VB.TextBox txtRMS 
         Height          =   285
         Left            =   -68940
         TabIndex        =   107
         Text            =   "3.0"
         ToolTipText     =   "RMS (K) for selecting RAOBs"
         Top             =   1680
         Width           =   405
      End
      Begin VB.TextBox txtAVG 
         Height          =   285
         Left            =   -68940
         TabIndex        =   106
         Text            =   "2.0"
         Top             =   1350
         Width           =   405
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "D to D"
         Height          =   375
         Index           =   15
         Left            =   -69990
         TabIndex        =   105
         ToolTipText     =   "Calculate Day-to-Day Statistics"
         Top             =   3150
         Width           =   975
      End
      Begin VB.CheckBox chkKeepSelection 
         Caption         =   "Keep Sel"
         Height          =   195
         Left            =   -71040
         TabIndex        =   104
         ToolTipText     =   "Keep selected soundings and change Remove to Keep"
         Top             =   1320
         Width           =   975
      End
      Begin VB.TextBox txtDescription 
         Height          =   285
         Left            =   -73320
         TabIndex        =   103
         Text            =   "Description"
         Top             =   2040
         Width           =   5295
      End
      Begin VB.CommandButton cmdDescription 
         Caption         =   "Plot Description"
         Height          =   315
         Left            =   -74880
         TabIndex        =   102
         Top             =   2040
         Width           =   1455
      End
      Begin VB.TextBox txtRCindex1 
         Height          =   285
         Left            =   -69720
         TabIndex        =   101
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtRCindex2 
         Height          =   285
         Left            =   -68640
         TabIndex        =   100
         Text            =   "SP012"
         Top             =   2400
         Width           =   615
      End
      Begin VB.CheckBox chkSort 
         Caption         =   "Sort?"
         Height          =   195
         Left            =   -74880
         TabIndex        =   99
         Top             =   3660
         Width           =   735
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Document"
         Height          =   375
         Index           =   16
         Left            =   -68940
         TabIndex        =   98
         ToolTipText     =   "Put RAOB Name and Time on plot"
         Top             =   3540
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Replicate"
         Height          =   375
         Index           =   17
         Left            =   -69990
         TabIndex        =   97
         ToolTipText     =   "Make 100 Copies of selected and write to file"
         Top             =   3540
         Width           =   975
      End
      Begin VB.TextBox txtDate1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   1020
         MultiLine       =   -1  'True
         TabIndex        =   96
         Top             =   1080
         Width           =   1515
      End
      Begin VB.TextBox txtDate1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   1020
         MultiLine       =   -1  'True
         TabIndex        =   95
         Top             =   720
         Width           =   1515
      End
      Begin VB.OptionButton optSite 
         Height          =   195
         Index           =   1
         Left            =   90
         TabIndex        =   94
         Top             =   1080
         Width           =   225
      End
      Begin VB.OptionButton optSite 
         Height          =   195
         Index           =   0
         Left            =   90
         TabIndex        =   93
         Top             =   705
         Width           =   225
      End
      Begin VB.TextBox txtDate2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   2580
         MultiLine       =   -1  'True
         TabIndex        =   92
         Top             =   1080
         Width           =   1515
      End
      Begin VB.TextBox txtDate2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   2580
         MultiLine       =   -1  'True
         TabIndex        =   91
         Top             =   720
         Width           =   1515
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB     Avg      RMS"
         Height          =   1005
         Index           =   13
         Left            =   120
         TabIndex        =   84
         Top             =   3645
         Width           =   1935
         Begin VB.TextBox txtSavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   630
            MultiLine       =   -1  'True
            TabIndex        =   88
            Top             =   270
            Width           =   555
         End
         Begin VB.TextBox txtSrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   87
            Top             =   270
            Width           =   555
         End
         Begin VB.TextBox txtSavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   630
            MultiLine       =   -1  'True
            TabIndex        =   86
            Top             =   630
            Width           =   555
         End
         Begin VB.TextBox txtSrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   85
            Top             =   630
            Width           =   555
         End
         Begin VB.Label Label 
            Caption         =   "Site 1"
            Height          =   225
            Index           =   65
            Left            =   90
            TabIndex        =   90
            Top             =   270
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "Site 2"
            Height          =   225
            Index           =   66
            Left            =   90
            TabIndex        =   89
            Top             =   660
            Width           =   435
         End
      End
      Begin VB.CheckBox chkWritePNGs 
         Caption         =   "Save Images"
         Height          =   285
         Left            =   3780
         TabIndex        =   83
         Top             =   4095
         Width           =   1305
      End
      Begin VB.CheckBox chkWait 
         Caption         =   "Wait (s)"
         Height          =   225
         Left            =   2280
         TabIndex        =   82
         Top             =   4125
         Width           =   855
      End
      Begin VB.TextBox txtPrefix 
         Height          =   285
         Left            =   3750
         TabIndex        =   81
         Text            =   "C:\MTP\Data\DC8\PAVE\"
         Top             =   4995
         Width           =   3255
      End
      Begin VB.ComboBox cboRAOBtype 
         Height          =   315
         Left            =   -73650
         TabIndex        =   80
         Text            =   "110"
         Top             =   2220
         Width           =   765
      End
      Begin VB.CommandButton cmdNlevrMax 
         Caption         =   "Nlevr Max"
         Height          =   375
         Left            =   -69720
         TabIndex        =   79
         Top             =   990
         Width           =   1695
      End
      Begin VB.ComboBox cboRAOBtypeOUT 
         Height          =   315
         Left            =   -71310
         TabIndex        =   78
         Text            =   "110"
         Top             =   2190
         Width           =   765
      End
      Begin VB.Frame Frame 
         Caption         =   "Nlevr"
         Height          =   645
         Index           =   15
         Left            =   -68970
         TabIndex        =   74
         Top             =   1770
         Width           =   945
         Begin VB.TextBox txtFilter 
            BackColor       =   &H000000FF&
            Height          =   285
            Index           =   13
            Left            =   60
            TabIndex        =   76
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtNlevr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   510
            MultiLine       =   -1  'True
            TabIndex        =   75
            Text            =   "RAOBman.frx":055C
            Top             =   240
            Width           =   345
         End
         Begin VB.Label Label 
            Caption         =   ">"
            Height          =   165
            Index           =   74
            Left            =   390
            TabIndex        =   77
            Top             =   270
            Width           =   75
         End
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Save Ref"
         Height          =   375
         Index           =   18
         Left            =   -68940
         TabIndex        =   73
         ToolTipText     =   "Save selected RAOB as reference for RMS calculation"
         Top             =   2760
         Width           =   975
      End
      Begin VB.TextBox txtNavg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3765
         MultiLine       =   -1  'True
         TabIndex        =   72
         Text            =   "RAOBman.frx":0561
         Top             =   3480
         Width           =   495
      End
      Begin VB.CommandButton cmdRAOBrange 
         Caption         =   "Get RAOB Range"
         Height          =   375
         Left            =   -69720
         TabIndex        =   71
         Top             =   3030
         Width           =   1695
      End
      Begin VB.CommandButton cmdRMSwrtFL 
         Caption         =   "RMS wrt FL"
         Height          =   405
         Left            =   -69720
         TabIndex        =   70
         Top             =   3420
         Width           =   1695
      End
      Begin VB.ComboBox cboWindow 
         Height          =   315
         Left            =   -69750
         TabIndex        =   69
         Text            =   "1"
         ToolTipText     =   "Offset wrt selected level to be used to determine slope"
         Top             =   2820
         Width           =   615
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "G"
         Height          =   285
         Index           =   19
         Left            =   -74610
         TabIndex        =   68
         ToolTipText     =   "Set RAOBgood flag for all RAOBs in list"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CheckBox chkPlotT 
         Caption         =   "Plot PT"
         Height          =   225
         Index           =   2
         Left            =   -72990
         TabIndex        =   67
         Top             =   3690
         Width           =   855
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "X"
         Height          =   285
         Index           =   20
         Left            =   -74865
         TabIndex        =   66
         ToolTipText     =   "Replace XXXX with proper ICAO filename"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CheckBox chkPlotT 
         Caption         =   "Plot EPT"
         Height          =   225
         Index           =   3
         Left            =   -72990
         TabIndex        =   65
         Top             =   3900
         Width           =   945
      End
      Begin VB.CheckBox chkPlotT 
         Caption         =   "Plot T"
         Height          =   195
         Index           =   0
         Left            =   -72990
         TabIndex        =   64
         Top             =   4110
         Value           =   1  'Checked
         Width           =   795
      End
      Begin VB.CheckBox chkPlotT 
         Caption         =   "Plot ET"
         Height          =   195
         Index           =   1
         Left            =   -72030
         TabIndex        =   63
         Top             =   3900
         Width           =   825
      End
      Begin VB.CheckBox chkShowDots 
         Caption         =   "Dots"
         Height          =   195
         Left            =   -74880
         TabIndex        =   62
         Top             =   4110
         Width           =   705
      End
      Begin VB.TextBox txtList 
         Alignment       =   1  'Right Justify
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   6
         Left            =   -70170
         MultiLine       =   -1  'True
         TabIndex        =   61
         Top             =   2220
         Width           =   705
      End
      Begin VB.CheckBox chkUseBias 
         Caption         =   "Use Bias"
         Height          =   195
         Left            =   -69000
         TabIndex        =   60
         ToolTipText     =   "Include Bias in RMS search; otherwise, remove it"
         Top             =   570
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "E"
         Height          =   285
         Index           =   21
         Left            =   -74310
         TabIndex        =   59
         ToolTipText     =   "Extend RAOB to 50 km"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CheckBox chkShowRMSprofile 
         Caption         =   "RMS T"
         Height          =   195
         Left            =   -72030
         TabIndex        =   58
         Top             =   3690
         Width           =   885
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "M"
         Height          =   285
         Index           =   22
         Left            =   -73740
         TabIndex        =   57
         ToolTipText     =   "Show MLS Profile"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "W"
         Height          =   285
         Index           =   23
         Left            =   -74010
         TabIndex        =   56
         ToolTipText     =   "Write Selected Soundings to MISSION_RAOBs.RAOB2  File"
         Top             =   3300
         Width           =   285
      End
      Begin VB.TextBox txtMaxRange 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3750
         MultiLine       =   -1  'True
         TabIndex        =   55
         Text            =   "RAOBman.frx":0565
         Top             =   3780
         Width           =   465
      End
      Begin VB.TextBox txtLayerThickness 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3750
         MultiLine       =   -1  'True
         TabIndex        =   54
         Text            =   "RAOBman.frx":056B
         Top             =   3180
         Width           =   465
      End
      Begin VB.TextBox txtDeltaZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5220
         MultiLine       =   -1  'True
         TabIndex        =   53
         Text            =   "RAOBman.frx":0572
         Top             =   3180
         Width           =   405
      End
      Begin VB.TextBox txtMinZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5220
         MultiLine       =   -1  'True
         TabIndex        =   52
         Text            =   "RAOBman.frx":0578
         Top             =   3480
         Width           =   405
      End
      Begin VB.TextBox txtAbsRoll 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5220
         MultiLine       =   -1  'True
         TabIndex        =   51
         Text            =   "RAOBman.frx":057E
         Top             =   3780
         Width           =   405
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "R"
         Height          =   285
         Index           =   24
         Left            =   -73440
         TabIndex        =   50
         ToolTipText     =   "Write Selected Soundings to MISSION_RAOBs.RAOB2  File"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CommandButton cmdMLS 
         Caption         =   "MLS"
         Height          =   375
         Left            =   -70800
         TabIndex        =   49
         Top             =   3120
         Width           =   555
      End
      Begin VB.CommandButton cmdTES 
         Caption         =   "TES"
         Height          =   375
         Left            =   -70800
         TabIndex        =   48
         Top             =   3480
         Width           =   555
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "L"
         Height          =   285
         Index           =   25
         Left            =   -72900
         TabIndex        =   47
         ToolTipText     =   "Show MLS Profile"
         Top             =   3300
         Width           =   285
      End
      Begin VB.CheckBox chkDoubleTrop 
         Caption         =   "Dbl Trop"
         Height          =   195
         Left            =   -74130
         TabIndex        =   46
         Top             =   4110
         Width           =   915
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "NGV FL Tcal"
         Height          =   375
         Index           =   8
         Left            =   5730
         TabIndex        =   45
         ToolTipText     =   "Calibrate NGV Tres and Tavi against RAOBs"
         Top             =   3390
         Width           =   1275
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Filter Output"
         Height          =   285
         Index           =   9
         Left            =   5730
         TabIndex        =   44
         ToolTipText     =   "Filter NGV temperature comparisons into different altitude ranges"
         Top             =   330
         Width           =   1275
      End
      Begin VB.TextBox txtWait 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3135
         MultiLine       =   -1  'True
         TabIndex        =   43
         Text            =   "RAOBman.frx":0583
         Top             =   4080
         Width           =   390
      End
      Begin VB.CommandButton cmdGAL 
         Caption         =   "SEST"
         Height          =   300
         Index           =   0
         Left            =   -71475
         TabIndex        =   42
         Top             =   2775
         Width           =   915
      End
      Begin VB.CommandButton cmdGAL 
         Caption         =   "MROC"
         Height          =   300
         Index           =   1
         Left            =   -70455
         TabIndex        =   41
         Top             =   2775
         Width           =   915
      End
      Begin VB.CommandButton cmdTemplates 
         Caption         =   "Template RAOBs"
         Height          =   420
         Left            =   -69720
         TabIndex        =   40
         ToolTipText     =   "Read all RCF files and write template RAOBs into Mission_Template_RAOBs.RAOB2"
         Top             =   4230
         Width           =   1695
      End
      Begin VB.CommandButton cmdBias 
         Caption         =   "Bias"
         Height          =   345
         Left            =   -68520
         TabIndex        =   39
         ToolTipText     =   "Default Bias"
         Top             =   1305
         Width           =   555
      End
      Begin VB.CommandButton cmdRMS 
         Caption         =   "RMS"
         Height          =   345
         Left            =   -68520
         TabIndex        =   38
         ToolTipText     =   "Defaut RMS"
         Top             =   1635
         Width           =   555
      End
      Begin VB.CommandButton cmdRAOBrangeUsed 
         Caption         =   "RAOBrangeUsed"
         Height          =   450
         Left            =   -71745
         TabIndex        =   37
         Top             =   3735
         Width           =   1830
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "T "
         Height          =   285
         Index           =   26
         Left            =   -73140
         TabIndex        =   36
         ToolTipText     =   "Select all RAOBs from list which are have an RMS less than criteria above"
         Top             =   3300
         Width           =   255
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "COSMIC Tcal"
         Height          =   375
         Index           =   10
         Left            =   5730
         TabIndex        =   35
         ToolTipText     =   "Calibrate NGV Tres and Tavi against RAOBs"
         Top             =   3780
         Width           =   1275
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB Extension"
         Height          =   645
         Index           =   14
         Left            =   -74910
         TabIndex        =   25
         Top             =   4710
         Width           =   3765
         Begin VB.CheckBox chkEnableRAOBextension 
            Height          =   255
            Left            =   105
            TabIndex        =   30
            Top             =   240
            Width           =   285
         End
         Begin VB.TextBox txtExtensionNoise 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   3240
            MultiLine       =   -1  'True
            TabIndex        =   29
            Text            =   "RAOBman.frx":0585
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtLR2 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   2310
            MultiLine       =   -1  'True
            TabIndex        =   28
            Text            =   "RAOBman.frx":058B
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtZb 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   1500
            MultiLine       =   -1  'True
            TabIndex        =   27
            Text            =   "RAOBman.frx":0591
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtLR1 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   780
            MultiLine       =   -1  'True
            TabIndex        =   26
            Text            =   "RAOBman.frx":0596
            Top             =   240
            Width           =   435
         End
         Begin VB.Line Line1 
            X1              =   390
            X2              =   390
            Y1              =   240
            Y2              =   540
         End
         Begin VB.Label Label 
            Caption         =   "Noise"
            Height          =   255
            Index           =   71
            Left            =   2790
            TabIndex        =   34
            Top             =   270
            Width           =   465
         End
         Begin VB.Label Label 
            Caption         =   "LR2"
            Height          =   255
            Index           =   70
            Left            =   1980
            TabIndex        =   33
            Top             =   270
            Width           =   315
         End
         Begin VB.Label Label 
            Caption         =   "Zb"
            Height          =   255
            Index           =   69
            Left            =   1260
            TabIndex        =   32
            Top             =   300
            Width           =   285
         End
         Begin VB.Label Label 
            Caption         =   "LR1"
            Height          =   255
            Index           =   68
            Left            =   450
            TabIndex        =   31
            Top             =   270
            Width           =   285
         End
      End
      Begin VB.CheckBox chkTcalCorr 
         Caption         =   "Tcal Corr"
         Height          =   225
         Left            =   3780
         TabIndex        =   24
         Top             =   4380
         Width           =   1395
      End
      Begin VB.Frame Frame3 
         Caption         =   "RMS Zp Range (km)"
         Height          =   645
         Left            =   -71070
         TabIndex        =   17
         Top             =   4710
         Width           =   3075
         Begin VB.TextBox txtRMSrange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   510
            MultiLine       =   -1  'True
            TabIndex        =   20
            Text            =   "RAOBman.frx":059B
            Top             =   300
            Width           =   405
         End
         Begin VB.TextBox txtRMSrange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   19
            Text            =   "RAOBman.frx":059D
            Top             =   300
            Width           =   405
         End
         Begin VB.TextBox txtRMSrange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   2580
            MultiLine       =   -1  'True
            TabIndex        =   18
            Text            =   "RAOBman.frx":05A3
            Top             =   270
            Width           =   405
         End
         Begin VB.Label Label 
            Caption         =   "Min"
            Height          =   255
            Index           =   24
            Left            =   180
            TabIndex        =   23
            Top             =   300
            Width           =   315
         End
         Begin VB.Label Label 
            Caption         =   "Step"
            Height          =   255
            Index           =   25
            Left            =   1110
            TabIndex        =   22
            Top             =   300
            Width           =   405
         End
         Begin VB.Label Label 
            Caption         =   "Max"
            Height          =   255
            Index           =   26
            Left            =   2160
            TabIndex        =   21
            Top             =   300
            Width           =   315
         End
      End
      Begin VB.CheckBox chkAddBias 
         Caption         =   "Add Bias"
         Height          =   195
         Left            =   -72030
         TabIndex        =   16
         ToolTipText     =   "Add a temperature bias when Writing soundings"
         Top             =   4140
         Width           =   945
      End
      Begin MSComDlg.CommonDialog CommonDialog2 
         Left            =   -70800
         Top             =   4185
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin ComctlLib.ProgressBar ProgressBar2 
         Height          =   255
         Left            =   -74040
         TabIndex        =   188
         Top             =   2400
         Width           =   3615
         _ExtentX        =   6376
         _ExtentY        =   450
         _Version        =   327682
         Appearance      =   1
      End
      Begin ComCtl2.UpDown UpDown3 
         Height          =   255
         Index           =   0
         Left            =   -70320
         TabIndex        =   207
         Top             =   960
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   450
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin ComCtl2.UpDown UpDown2 
         Height          =   255
         Index           =   0
         Left            =   -69240
         TabIndex        =   208
         Top             =   960
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   450
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin ComCtl2.UpDown UpDown1 
         Height          =   375
         Left            =   -69420
         TabIndex        =   348
         Top             =   2220
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   661
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin MSComDlg.CommonDialog cdConvertIn 
         Left            =   -70320
         Top             =   4185
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin ComCtl2.UpDown UpDown3 
         Height          =   375
         Index           =   1
         Left            =   -69720
         TabIndex        =   374
         Top             =   480
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   661
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin ComCtl2.UpDown UpDown2 
         Height          =   375
         Index           =   1
         Left            =   -68280
         TabIndex        =   375
         Top             =   480
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   661
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin VB.Label Label5 
         Caption         =   "      Zg[km] Zp[km] Zp[mb]   T[K] RH[%] PT[K]"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   429
         Top             =   420
         Width           =   5775
      End
      Begin VB.Label lblSite 
         Caption         =   "Site:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   428
         Top             =   2580
         Width           =   5145
      End
      Begin VB.Label lblTime 
         Caption         =   "Time:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   427
         Top             =   2790
         Width           =   5115
      End
      Begin VB.Label lblTrop 
         Caption         =   "Trop:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   426
         Top             =   3000
         Width           =   5115
      End
      Begin VB.Label lblMakky 
         Caption         =   "Process Status"
         Height          =   255
         Left            =   -74880
         TabIndex        =   425
         ToolTipText     =   "Status of conversion"
         Top             =   4080
         Width           =   4215
      End
      Begin VB.Label Label3 
         Caption         =   "RAOB"
         Height          =   255
         Index           =   0
         Left            =   -69570
         TabIndex        =   424
         Top             =   3180
         Width           =   495
      End
      Begin VB.Label Label3 
         Caption         =   "Trop"
         Height          =   255
         Index           =   1
         Left            =   -69570
         TabIndex        =   423
         Top             =   3480
         Width           =   375
      End
      Begin VB.Label Label19 
         Caption         =   "Mod   Use  OK"
         Height          =   255
         Left            =   -69090
         TabIndex        =   422
         Top             =   2880
         Width           =   1095
      End
      Begin VB.Label Label6 
         Caption         =   "Site WMOno yyyy mm dd hhmm i"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   -74760
         TabIndex        =   421
         Top             =   420
         Width           =   3975
      End
      Begin VB.Label Label 
         Caption         =   "Records"
         Height          =   255
         Index           =   0
         Left            =   -71760
         TabIndex        =   420
         Top             =   3150
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Default Drive"
         Height          =   255
         Index           =   3
         Left            =   -74880
         TabIndex        =   419
         Top             =   1860
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Export Path"
         Height          =   255
         Index           =   4
         Left            =   -74880
         TabIndex        =   418
         Top             =   3420
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "UTsec    Alt [km]"
         Height          =   255
         Index           =   11
         Left            =   120
         TabIndex        =   417
         Top             =   1440
         Width           =   1215
      End
      Begin VB.Label Label4 
         Caption         =   " Site            Date1       Time 1       Date2       Time 2  Rec 1  Rec 2    Wt"
         Height          =   255
         Left            =   450
         TabIndex        =   416
         Top             =   480
         Width           =   5235
      End
      Begin VB.Label Label 
         Caption         =   "Latitude      Longitude   D [km]"
         Height          =   255
         Index           =   13
         Left            =   2955
         TabIndex        =   415
         Top             =   1425
         Width           =   2295
      End
      Begin VB.Label Label 
         Caption         =   "Site 1"
         Height          =   165
         Index           =   12
         Left            =   2250
         TabIndex        =   414
         Top             =   1740
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Site 2"
         Height          =   165
         Index           =   14
         Left            =   2250
         TabIndex        =   413
         Top             =   2025
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Event"
         Height          =   195
         Index           =   15
         Left            =   2250
         TabIndex        =   412
         Top             =   2310
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Record"
         Height          =   165
         Index           =   17
         Left            =   4380
         TabIndex        =   411
         Top             =   2880
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Color"
         Height          =   255
         Index           =   18
         Left            =   -71040
         TabIndex        =   410
         Top             =   960
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Thick"
         Height          =   255
         Index           =   19
         Left            =   -69960
         TabIndex        =   409
         Top             =   960
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Alt [km]"
         Height          =   255
         Index           =   20
         Left            =   2250
         TabIndex        =   408
         Top             =   2535
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "OAT [K]"
         Height          =   195
         Index           =   16
         Left            =   2250
         TabIndex        =   407
         Top             =   2880
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Location"
         Height          =   255
         Index           =   28
         Left            =   -74880
         TabIndex        =   406
         Top             =   2400
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "of"
         Height          =   255
         Index           =   29
         Left            =   -69000
         TabIndex        =   405
         Top             =   2760
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Thick"
         Height          =   255
         Index           =   30
         Left            =   -69000
         TabIndex        =   404
         Top             =   480
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Color"
         Height          =   255
         Index           =   31
         Left            =   -70560
         TabIndex        =   403
         Top             =   480
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Special filter"
         Height          =   195
         Index           =   23
         Left            =   -70140
         TabIndex        =   402
         Top             =   4230
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Int Vapor (cm)"
         Height          =   255
         Index           =   39
         Left            =   -74880
         TabIndex        =   401
         Top             =   4095
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "BI (m)"
         Height          =   255
         Index           =   40
         Left            =   -73080
         TabIndex        =   400
         Top             =   4095
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Liquid Layer (m)"
         Height          =   255
         Index           =   41
         Left            =   -74880
         TabIndex        =   399
         Top             =   4395
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   " to "
         Height          =   255
         Index           =   42
         Left            =   -73080
         TabIndex        =   398
         Top             =   4395
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "km"
         Height          =   255
         Index           =   45
         Left            =   -73560
         TabIndex        =   397
         Top             =   4380
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "K"
         Height          =   255
         Index           =   46
         Left            =   -72720
         TabIndex        =   396
         Top             =   4410
         Width           =   105
      End
      Begin VB.Label Label 
         Caption         =   " HHMMSS UTsec pALT gALT    Lat     Lon Hdg Pitch  Roll OAT_K"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   47
         Left            =   -74880
         TabIndex        =   395
         Top             =   3840
         Width           =   6495
      End
      Begin VB.Label Label 
         Caption         =   "MTP Path"
         Height          =   255
         Index           =   48
         Left            =   -74880
         TabIndex        =   394
         Top             =   4020
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Zp (km)"
         Height          =   255
         Index           =   50
         Left            =   -74880
         TabIndex        =   393
         Top             =   3690
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "K, Tac-Traob"
         Height          =   255
         Index           =   51
         Left            =   -71490
         TabIndex        =   392
         Top             =   3690
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Location Map"
         Height          =   255
         Index           =   56
         Left            =   -74880
         TabIndex        =   391
         Top             =   4380
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "K"
         Height          =   165
         Index           =   57
         Left            =   -68160
         TabIndex        =   390
         Top             =   1065
         Width           =   120
      End
      Begin VB.Label Label 
         Caption         =   "T Increment"
         Height          =   255
         Index           =   58
         Left            =   -68955
         TabIndex        =   389
         Top             =   810
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "RCs"
         Height          =   195
         Index           =   62
         Left            =   -70320
         TabIndex        =   388
         Top             =   2460
         Width           =   315
      End
      Begin VB.Label Label 
         Caption         =   "Record"
         Height          =   195
         Index           =   63
         Left            =   -70320
         TabIndex        =   387
         Top             =   2820
         Width           =   555
      End
      Begin VB.Label Label 
         Caption         =   "Out File Path"
         Height          =   195
         Index           =   67
         Left            =   2280
         TabIndex        =   386
         Top             =   5055
         Width           =   915
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Levels IN"
         Height          =   255
         Index           =   72
         Left            =   -74880
         TabIndex        =   385
         Top             =   2310
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Levels OUT"
         Height          =   255
         Index           =   73
         Left            =   -72750
         TabIndex        =   384
         Top             =   2280
         Width           =   1395
      End
      Begin VB.Label Label 
         Caption         =   "Cycles Averaged"
         Height          =   195
         Index           =   75
         Left            =   2280
         TabIndex        =   383
         Top             =   3540
         Width           =   1275
      End
      Begin VB.Label Label 
         Caption         =   "Offset"
         Height          =   255
         Index           =   76
         Left            =   -69750
         TabIndex        =   382
         Top             =   2640
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Max Range (km)"
         Height          =   195
         Index           =   79
         Left            =   2280
         TabIndex        =   381
         Top             =   3870
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Layer Thickness (m)"
         Height          =   195
         Index           =   80
         Left            =   2280
         TabIndex        =   380
         Top             =   3210
         Width           =   1485
      End
      Begin VB.Label Label 
         Caption         =   "Delta Zp [m]"
         Height          =   195
         Index           =   81
         Left            =   4290
         TabIndex        =   379
         Top             =   3240
         Width           =   885
      End
      Begin VB.Label Label 
         Caption         =   "Min Zp [km]"
         Height          =   195
         Index           =   82
         Left            =   4290
         TabIndex        =   378
         Top             =   3540
         Width           =   915
      End
      Begin VB.Label Label 
         Caption         =   "AbsRoll [o]"
         Height          =   195
         Index           =   83
         Left            =   4290
         TabIndex        =   377
         Top             =   3840
         Width           =   795
      End
      Begin VB.Label lblLoc 
         Caption         =   "Location:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   -74880
         TabIndex        =   376
         Top             =   3240
         Width           =   5145
      End
   End
   Begin VB.Label lblProgress 
      Caption         =   "Record"
      Height          =   255
      Left            =   5820
      TabIndex        =   6
      Top             =   5985
      Width           =   735
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpenAscii 
         Caption         =   "Open &Ascii RAOB File"
      End
      Begin VB.Menu mnuFileOpenBinary 
         Caption         =   "Open &Binary RAOB File"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuOption 
      Caption         =   "&Option"
      Begin VB.Menu mnuOptionFLTinfo 
         Caption         =   "Show FLTinfo"
      End
      Begin VB.Menu mnuOptionFormatRAOB 
         Caption         =   "Format &RAOB"
      End
      Begin VB.Menu mnuOptionFormatATP 
         Caption         =   "Format &ATP"
      End
      Begin VB.Menu mnuOptionDetails 
         Caption         =   "&RAOB Details"
      End
      Begin VB.Menu mnuOptionConvert 
         Caption         =   "&Convert .txt to .WMO"
      End
      Begin VB.Menu mnuOptionFlatfile 
         Caption         =   "Convert Flatfile to WMO"
      End
      Begin VB.Menu munOptionMakky 
         Caption         =   "Convert Even Makky"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpUsersGuide 
         Caption         =   "&User's Guide"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmRAOBman"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' 19990907 mjm  Changed code so that RAOB always save trops in pressure altitude,
'               to be consistent with ATP.
'
Dim RAOBlu%, ASCIIinput As Boolean
Dim Idum%, i%, Cycle&, Cycle2%
Dim ConvertInFile$, GOTOcommand As Boolean
Dim EnableFilter(0 To 14) As Boolean, InputMode%
Dim LAT1!, LAT2!, LON1!, LON2!, DandTstart As Date, DandTend As Date
Dim AutoView As Boolean, BurstAlt!, StartAlt!
Dim ExportMode%, ExportFormat%
Dim Site1 As Boolean, TimeSec&, Ltn!, Lnn!, EventNumber&, EventTime&
Dim IyearE%, IdoyE%, ImonthE%, IdayE%
Dim DefaultBG As Boolean
Dim EventList$(1 To 20), OAT11!, OAT21!, OAT12!, OAT22!
Dim CurrentRecord&, REFfileRecords%, REFfileRecord%, Update As Boolean, ScanLength!
Dim EnableAbs(1 To 4) As Boolean, RhoLstart!
Dim Documentation$
Dim CMD14 As Boolean
Dim ShowFLTinfo As Boolean
Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" _
              (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, _
               ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
                
Private Const SW_SHOW = 1


Private Sub cmdStart_Click()
Dim n%, A$, INPlu%, OUTlu%, InFile$

  InFile = txtPath.Text + "RAOB\TREX_RAOBrangeAll.txt"
  INPlu = FreeFile
  Open InFile For Input As INPlu

  OUTlu = FreeFile
  OUTfile = txtPath.Text + "RAOB\TREX_RAOBrangeUsed.txt"
  Open OUTfile For Output As OUTlu
  Input #INPlu, A$
  Print #OUTlu, A$

  Do
    Input #INPlu, A$
    n = Left$(A$, 3)
    If Len(Dir(txtPath.Text + "PNG\R" + Format(n, "000") + "*.PNG")) > 0 Then
      Print #OUTlu, A$
    End If
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu

End Sub

Sub AverageNcycles(n%, aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, Nada As Boolean)
Dim X!, i%, j%, pALT1!, Nlev11%, Nlev21%, n2%, aRA(1 To 33), aRT(1 To 33), dNlev%
Dim DeltaZp!, MinZp!, AbsRoll!
Dim LoopCount%
' Exit it no good data after looping 10 times

  For i = 1 To 33: RAp(i) = 0: RTp(i) = 0: aRA(i) = 0: aRT(i) = 0: Next i
  aZp = 0#: aTnav! = 0#: aTmtp! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
  i = 0: Nlev11 = 0: Nlev21 = 0: n2 = Int(n / 2) + 1
  Nada = False
  DeltaZp = Val(txtDeltaZp.Text) / 1000# 'Convert to km
  MinZp = Val(txtMinZp.Text)
  AbsRoll = Val(txtAbsRoll.Text)
  LoopCount = 0
  Cycle2 = Cycle2 - 1  'xxxmjm 20101229 GetREFcycle (below) automatically increments match point by 1 cycle
  Do
    i = i + 1
NextCycle:
    If LoopCount > 20 Then Nada = True: Exit Sub
    Cycle2 = Cycle2 + i - n2  'Center on time of closest approach
    Call GetREFcycle(Cycle2, False)
    If pALT < MinZp Then Nada = True: Exit Sub
    If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
    If Abs(pALT - pALT1) > DeltaZp Or Nlev11 <> Nlev1 Or Nlev21 <> Nlev2 Or Not GoodScan Or Abs(Roll) > AbsRoll Then
      i = 1
      Cycle2 = Cycle2 + 1  'xxxmjm 20101229 was +2
      aZp = 0#: aTnav! = 0#: aTmtp! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
      For j = 1 To 33: RAp(i) = 0: RTp(i) = 0: aRA(j) = 0#: aRT(j) = 0#: Next j
      LoopCount = LoopCount + 1
      GoTo NextCycle         'Start over if climbing or descending
    End If
    If i = n2 Then txtATPgo.Text = Format(UTsec / 1000#, "##0.000")
    aZp = aZp + pALT
    'Debug.Print i; pALT
    aZt = aZt + Zt1
    aTnav = aTnav + OATnav
    aTmtp = aTmtp + sTTT(16)  'RTp(16)   'OATmtp is not same as RTp(16) and gives different Tmtp-Traob
  'Debug.Print OATnav; OATmtp;
    aTt = aTt + TT1
    aLat = aLat + Latitude
    aLon = aLon + Longitude
    aRoll = aRoll + Roll
    aPitch = aPitch + Pitch
    For j = 1 To 33
      aRA(j) = aRA(j) + sZZZ(j)    'RAp(j)
      aRT(j) = aRT(j) + sTTT(j)    'RTp(j)
    Next j
  ' Debug.Print RAp(16); RTp(16); RTp(16) - OATmtp
    ProgressBar2.Value = Cycle2
    txtREFrecord.Text = Str(Cycle2)
  Loop Until i = n
  
' Calculate aveage values
  aZp = aZp / n
  aZt = aZt / n
  aTnav = aTnav / n
  aTmtp = aTmtp / n
  aTt = aTt / n
  aLat = aLat / n
  aLon = aLon / n
  aRoll = aRoll / n
  aPitch = aPitch / n
  For i = Nlev1 To Nlev2
    RAp(i - Nlev1 + 1) = aRA(i) / n
    RTp(i - Nlev1 + 1) = aRT(i) / n
  Next i
' Remove any small offset between aZp and RAp("16")
' Note that RAp may not have 33 levels, in which case level 16 is not FL
  If Nlev1 = 1 Then dNlev = 0 Else dNlev = 33 - Nlev
  X = aZp - sZZZ(16) 'RAp(16 - dNlev)
  For i = 1 To Nlev
    RAp(i) = RAp(i) + X
  Next i
  X = aTmtp - sTTT(16)     'RTp(16 - dNlev)
  For i = 1 To Nlev
    RTp(i) = RTp(i) + X
  Next i
 
' If Nlev = 33 Then Debug.Print RAp(16); aZp; ; aTmtp; RTp(16); RTp(16) - aTmtp; fLinterp(RAp(), RTp(), Nlev, aZp)
  frmATP.EnableFit = False
  If Nlev > 0 And Nlev <= 33 Then frmATP.ATPupdate

End Sub


Sub AverageNcyclesNGV(NGVlu, UTsecLast&, n%, aZp!, aTna!, aTnr!, aTnr2!, aTnr3!, aTnr4!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2!, Nada As Boolean)
Dim X!, i%, j%, pALT1!, Nlev11%, Nlev21%, n2%, aRA(1 To 33), aRT(1 To 33), dNlev%
Dim DeltaZp!, MinZp!, AbsRoll!, T!, Tc!, Tav!, Tre!, Tre2!, Tre3!, Tre4!, P!, mZg!, C!, Mach2!
Dim LoopCount%
' For HIPPO-1: aTna=AT_A, aTnr=ATFR, aTnr2=ATHR1, aTnr3=ATHR2
' Exit if no good data after looping 20 times

  aZp = 0#: aTna! = 0#: aTnr! = 0#: aTnr2! = 0#: aTnr3! = 0#: aTnr4 = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#: aMach2 = 0#
  i = 0: Nlev11 = 0: Nlev21 = 0: n2 = Int(n / 2) + 1
  Zt1 = 12#: TT1 = 200#
  Nada = False
  DeltaZp = Val(txtDeltaZp.Text) / 1000# 'Convert to km
  MinZp = Val(txtMinZp.Text)
  AbsRoll = Val(txtAbsRoll.Text)
  LoopCount = 0
  Do
    i = i + 1
NextCycle:
    If LoopCount > 400 Then Nada = True: Exit Sub
    Call GetNGline(NGVlu, UTsec&, P!, Tav, Tre, Tre2, Tre3, Tre4!, Latitude!, Longitude!, mZg!, pALT, Pitch, Roll, Mach2)
    If chkTcalCorr.Value = 1 Then
      Select Case Mission
      Case "START-08"
      Case "HIPPO"
        Tre = fTc(Tre, pALT, Mach2!)
      Case "HIPPO-2", "HIPPO-3", "PREDICT", "PREDICT2", "HIPPO-5", "TORERO"
        Tav = fTc(Tav, aZp!, Mach2!)
      Case Else
        MsgBox "fTc() not found for this mission", vbOK
      End Select
    End If
    
    If UTsec < UTsecLast Then UTsec = UTsec + 86400#
    ALTkm = pALT
    If pALT < MinZp Then Nada = True: Exit Sub
    If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
    If Abs(pALT - pALT1) > DeltaZp Or Abs(Roll) > AbsRoll Then
      i = 1
      'Cycle2 = Cycle2 + 2
      aZp = 0#: aTna! = 0#: aTnr! = 0#: aTnr2! = 0#: aTnr3! = 0#: aTnr4! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
      LoopCount = LoopCount + 1
      GoTo NextCycle         'Start over if climbing or descending
    End If
    aZp = aZp + pALT
    aZt = aZt + Zt1
    aTna = aTna + Tav
    aTnr = aTnr + Tre
    aTnr2 = aTnr2 + Tre2
    aTnr3 = aTnr3 + Tre3
    aTnr4 = aTnr4 + Tre4
    aTt = aTt + TT1
    aLat = aLat + Latitude
    aLon = aLon + Longitude
    aRoll = aRoll + Roll
    aPitch = aPitch + Pitch
    aMach2 = aMach2 + Mach2
  Loop Until i = n
  
' Calculate average values
  aZp = aZp / n
  aZt = aZt / n
  aTna = aTna / n
  aTnr = aTnr / n
  aTnr2 = aTnr2 / n
  aTnr3 = aTnr3 / n
  aTnr4 = aTnr4 / n
  aTt = aTt / n
  aLat = aLat / n
  aLon = aLon / n
  aRoll = aRoll / n
  aPitch = aPitch / n
  aMach2 = aMach2 / n

End Sub

Sub AverageNcyclesIWG(IWGlu%, IWGrecord&, UTsecLast&, n%, aZp!, aTna!, aTnr!, aTnr2!, aTnr3!, aTnr4!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2!, Nada As Boolean)
Dim X!, i%, j%, pALT1!, Nlev11%, Nlev21%, n2%, aRA(1 To 33), aRT(1 To 33), dNlev%
Dim DeltaZp!, MinZp!, AbsRoll!, T!, Tc!, Tav!, Tre!, Tre2!, Tre3!, Tre4!, P!, mZg!, C!, Mach2!
Dim LoopCount%
' Exit if no good data after looping 20 times

  aZp = 0#: aTna! = 0#: aTnr! = 0#: aTnr2! = 0#: aTnr3! = 0#: aTnr4 = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#: aMach2 = 0#
  i = 0: Nlev11 = 0: Nlev21 = 0: n2 = Int(n / 2) + 1
  Zt1 = 12#: TT1 = 200#
  Nada = False
  DeltaZp = Val(txtDeltaZp.Text) / 1000# 'Convert to km
  MinZp = Val(txtMinZp.Text)
  AbsRoll = Val(txtAbsRoll.Text)
  LoopCount = 0
  Do
    i = i + 1
NextCycle:
    If LoopCount > 400 Then Nada = True: Exit Sub
    Call IWGreadAVG(IWGlu, IWGrecord, UTsec&, P!, Tav, Tre, Tre2, Tre3, Tre4!, Latitude!, Longitude!, mZg!, pALT, Pitch, Roll, Mach2)
    IWGrecord = IWGrecord + 1
    If chkTcalCorr.Value = 1 Then  'Apply the Tcal correction to the IWG file which has the preliminary MMS data
      Select Case Mission
      Case "GloPac", "ATTREX"
        Tav = fTc(Tav + cTo, aZp!, Mach2!) - cTo
      Case Else
        MsgBox "fTc() not found for this mission", vbOK
      End Select
    End If
    
    If UTsec < UTsecLast Then UTsec = UTsec + 86400#
    ALTkm = pALT
    If pALT < MinZp Then Nada = True: Exit Sub
    If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
    If Abs(pALT - pALT1) > DeltaZp Or Abs(Roll) > AbsRoll Then
      i = 1
      'Cycle2 = Cycle2 + 2
      aZp = 0#: aTna! = 0#: aTnr! = 0#: aTnr2! = 0#: aTnr3! = 0#: aTnr4! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
      LoopCount = LoopCount + 1
      GoTo NextCycle         'Start over if climbing or descending
    End If
    aZp = aZp + pALT
    aZt = aZt + Zt1
    aTna = aTna + Tav
    aTnr = aTnr + Tre
    aTnr2 = aTnr2 + Tre2
    aTnr3 = aTnr3 + Tre3
    aTnr4 = aTnr4 + Tre4
    aTt = aTt + TT1
    aLat = aLat + Latitude
    aLon = aLon + Longitude
    aRoll = aRoll + Roll
    aPitch = aPitch + Pitch
    aMach2 = aMach2 + Mach2
  Loop Until i = n
  
' Calculate average values
  aZp = aZp / n
  aZt = aZt / n
  aTna = aTna / n + cTo
  aTnr = aTnr / n + cTo
  aTnr2 = aTnr2 / n + cTo
  aTnr3 = aTnr3 / n + cTo
  aTnr4 = aTnr4 / n + cTo
  aTt = aTt / n + cTo
  aLat = aLat / n
  aLon = aLon / n
  aRoll = aRoll / n
  aPitch = aPitch / n
  aMach2 = aMach2 / n
End Sub
Sub AverageNcyclesNGV_Trex(NGVlu, n%, aZp!, aTna!, aTnr!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2!, Nada As Boolean)
Dim X!, i%, j%, pALT1!, Nlev11%, Nlev21%, n2%, aRA(1 To 33), aRT(1 To 33), dNlev%
Dim DeltaZp!, MinZp!, AbsRoll!, Tav!, Tre!, Tre2!, Tre3!, Tre4!, P!, mZg!
Dim LoopCount%
' Exit it no good data after looping 10 times

  aZp = 0#: aTna! = 0#: aTnr! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
  i = 0: Nlev11 = 0: Nlev21 = 0: n2 = Int(n / 2) + 1
  Zt1 = 12#: TT1 = 200#
  Nada = False
  DeltaZp = Val(txtDeltaZp.Text) / 1000# 'Convert to km
  MinZp = Val(txtMinZp.Text)
  AbsRoll = Val(txtAbsRoll.Text)
  LoopCount = 0
  Do
    i = i + 1
NextCycle:
    If LoopCount > 20 Then Nada = True: Exit Sub
    Call GetNGline(NGVlu, UTsec&, P!, Tav, Tre, Tre2, Tre3, Tre4, Latitude!, Longitude!, mZg!, pALT, Pitch, Roll, aMach2)
    ALTkm = pALT
    If pALT < MinZp Then Nada = True: Exit Sub
    If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
    If Abs(pALT - pALT1) > DeltaZp Or Abs(Roll) > AbsRoll Then
      i = 1
      'Cycle2 = Cycle2 + 2
      aZp = 0#: aTna! = 0#: aTnr! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
      LoopCount = LoopCount + 1
      GoTo NextCycle         'Start over if climbing or descending
    End If
    aZp = aZp + pALT
    aZt = aZt + Zt1
    aTna = aTna + Tav
    aTnr = aTnr + Tre
    aTt = aTt + TT1
    aLat = aLat + Latitude
    aLon = aLon + Longitude
    aRoll = aRoll + Roll
    aPitch = aPitch + Pitch
  Loop Until i = n
  
' Calculate aveage values
  aZp = aZp / n
  aZt = aZt / n
  aTna = aTna / n
  aTnr = aTnr / n
  aTt = aTt / n
  aLat = aLat / n
  aLon = aLon / n
  aRoll = aRoll / n
  aPitch = aPitch / n


End Sub

Function fNumber(L$) As Boolean

Select Case Asc(L$)
Case Is < 58: fNumber = True
Case Else: fNumber = False
End Select
End Function


Sub GetAuraRange(AuraLat!, AuraLon!, Rmax!, ALTkmMin!, Cycle%(), UTks!(), Rsave!(), Count%)
Dim X!, i%, j%, RSLlu%, Status As Boolean, UArecord&
Const Ncompare = 300
Static RAOB$(1 To Ncompare)
Dim Record%, Rcurrent!, Path$
Dim Range!(1 To Ncompare), Found(1 To Ncompare) As Boolean
Dim UTksLast, RangeLast!(1 To Ncompare), ZpSave!(1 To Ncompare), ZpLast!(1 To Ncompare)
  
  For i = 1 To Ncompare: Range(i) = -1#: Next i
  Record = 0: j = 0
  Do
NextRecord:
    i = 1
    Record = Record + 1
    If EOF(REFlu) Then Exit Do
    Call REFread(REFlu, Record)
    If pALT < ALTkmMin Or Not GoodScan Then
      GoTo NextRecord
    End If
      Rcurrent = fGCD(AuraLat, AuraLon, Latitude, Longitude)
      'Debug.Print Record; Rcurrent
      If Rcurrent <= Rmax Then
        If Range(i) < 0 Then             'First time through
          Range(i) = 9999#
          RangeLast(i) = Rcurrent
          ZpLast(i) = ALTkm
        Else                             'Check if closer range
          If Rcurrent < Range(i) And Rcurrent < RangeLast(i) Then  'Yes
            Range(i) = Rcurrent
            ZpLast(i) = ALTkm
            UTksLast = UTsec / 1000#
            Found(i) = False
          Else                           'No, Save closest approach
            If Not Found(i) Then
              j = j + 1
              'r$(j) = RAOB$(i)
              UTks(j) = UTksLast
              Rsave(j) = RangeLast(i)
              ZpSave(j) = ZpLast(i)
              Range(i) = -1            'Start over with this site
              Found(i) = True
              Cycle(j) = Record
            End If
          End If
          RangeLast(i) = Rcurrent
        End If
        'Debug.Print RAOB$(i); Rcurrent; UTsec
      End If
    DoEvents
  Loop Until EOF(REFlu) Or j = 1
  Count = j
  
' Print out closest approach list
  For i = 1 To j
    If ZpSave(i) >= Val(ALTkmMin) Then
'      Debug.Print Format(Cycle(i), "0000") + vbTab + Format(UTks(i), "00.00") + vbTab + r$(i) + vbTab + r$(i) + vbTab + Format(Rsave(i), "000.0") + vbTab + Format(ZpSave(i), "000.0")
    End If
  Next i

End Sub

Sub GetRAOB(Index%, dT As Date, RAOB$, Found As Boolean)
Dim i%, X$
  Found = False
  For i = 0 To lstSelect.ListCount - 1
    X$ = lstSelect.List(i)
    Cycle = Right$(X$, 5)
    Call RAOBread(RAOBlu, Cycle)
    If Not fNumber(Left$(RAOB$, 1)) Then
      If DandT = dT And Trim(WMO4l) = RAOB$ Then
        lstSelect.ListIndex = i      'this draws RAOB on frmRAOB
'        lstSelect.Selected(i) = True
        cmdSelect_Click (Index)
        Found = True
        Exit For
      End If
    Else
      If DandT = dT And WMOnumber = Val(RAOB$) Then
        lstSelect.ListIndex = i
        cmdSelect_Click (Index)
        Found = True
        Exit For
      End If
    End If
  Next i
  
End Sub
Sub GetRAOBs(dT As Date, RAOB$, Beforecycle&, AfterCycle&)
Dim i%, X$, L%, Number As Boolean, iBefore%, FoundRAOBsite As Boolean

  Beforecycle = 0
  AfterCycle = 0
  L = Len(RAOB$)
  If fNumber(Left$(RAOB$, 1)) Then Number = True Else Number = False

  i = 0
  Do
    ' Read RAOB select list until correct site is found
    FoundRAOBsite = False
    Do
      X$ = lstSelect.List(i)
      Cycle = Right$(X$, 5)
      Call RAOBread(RAOBlu, Cycle)
      If Number Then
        If WMOnumber = Val(RAOB$) Then FoundRAOBsite = True
      Else
        If Trim(WMO4l) = RAOB$ Then FoundRAOBsite = True
      End If
 '     Debug.Print i + 1; Trim(WMO4l)
      If Trim(WMO4l) = "HK02" Then
      i = i
      End If
      i = i + 1
    Loop Until FoundRAOBsite Or i = lstSelect.ListCount
    If i = lstSelect.ListCount Then Exit Do   'Site not found, Exit Sub
    ' Then check if RAOB time stamp is after wanted time
    If Left$(RAOB$, 2) = "HK" Or Left$(RAOB$, 4) = "MROC" Then
      If DatePart("yyyy", DandT) = DatePart("yyyy", dT) And DatePart("m", DandT) = DatePart("m", dT) And DatePart("d", DandT) = DatePart("d", dT) Then
        AfterCycle = Cycle
'       frmRAOB.LineColor = Yellow
'       lstSelect.ListIndex = Cycle  'this draws before RAOB on frmRAOB
'       cmdSelect_Click (8)            'this transfers before RAOB data to Blend tab
        frmRAOB.LineColor = LCyan
        lstSelect.ListIndex = Cycle - 1  'this draws RAOB on frmRAOB
        cmdSelect_Click (9)            'this transfers after RAOB data to Blend tab
        lstSelect_Click
        DoEvents
        Exit Do
      End If
      iBefore = i - 1
      Beforecycle = Cycle              'save last correct site
      AfterCycle = Cycle
    Else
      If DandT >= dT Then            'transfer RAOBs to frmRAOB
        AfterCycle = Cycle
        frmRAOB.LineColor = Yellow
        lstSelect.ListIndex = iBefore  'this draws before RAOB on frmRAOB
        cmdSelect_Click (8)            'this transfers before RAOB data to Blend tab
        frmRAOB.LineColor = LCyan
        lstSelect.ListIndex = i - 1    'this draws RAOB on frmRAOB
        cmdSelect_Click (9)            'this transfers after RAOB data to Blend tab
        Exit Do
      End If
      iBefore = i - 1
      Beforecycle = Cycle              'save last correct site
    End If
  Loop
  
End Sub
Sub GetRAOB2(RAOBlu%, dT As Date, RAOB$, Found As Boolean, Cycle&)
Dim i%, X$
  If EOF(RAOBlu) Then Call RAOBreadSL(RAOBlu, 1)
  Found = False
  Cycle = 0
  Do
    Cycle = Cycle + 1
    Call RAOBread(RAOBlu, Cycle)
    If Not fNumber(Left$(RAOB$, 1)) Then
      If DandT = dT And WMO4l = RAOB$ Then
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
Sub GetRAOBfromList(Index%, dT As Date, RAOB$, Found As Boolean, ByRef iLast&)
'1234567890123456789012345678901234567890
'ABR  72659 2005  6  1  000 0   00001
'ICAO WMOno YYYY MM DD HHMM ijk Cycle

Dim i%, X$, DandT As Date, j%, iStart&, r$
Dim Iyear%, Imonth%, Iday%, Ihour%, Imin%, Isec%
  
  Found = False
  iStart = iLast
  For i = iStart To lstSelect.ListCount - 1
    X$ = lstSelect.List(i)
    j = InStr(1, X$, " ")
    r$ = Left$(X$, j - 1)
    Iyear = Mid$(X$, 12, 4)
    Imonth = Mid$(X$, 17, 2)
    Iday = Mid$(X$, 20, 2)
    Ihour = Mid$(X$, 23, 2)
    Imin = Mid$(X$, 25, 2)
    DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, 0)
    'Debug.Print x$, DandT
    Cycle = Right$(X$, 5)
'    Call RAOBread(RAOBlu, Cycle)
    If Not fNumber(Left$(RAOB$, 1)) Then
      If DandT = dT And r$ = RAOB$ Then
        lstSelect.ListIndex = i
'        lstSelect.Selected(i) = True
'        cmdSelect_Click (Index)
        Found = True
        iLast = i
        Exit For
      End If
    Else
      If DandT = dT And r$ = RAOB$ Then
        lstSelect.ListIndex = i
'        cmdSelect_Click (Index)
        Found = True
        iLast = i
        Exit For
      End If
    End If
  Next i
End Sub

Public Sub Navigate(ByVal NavTo As String)
  Dim hBrowse As Long
  hBrowse = ShellExecute(0&, "open", NavTo, "", "", SW_SHOW)
End Sub

Sub GetREFcycle(Cycle%, ByVal PlotUpdateFlag As Boolean)
Dim Zt!, Tt!, Th!, Lt!(1 To 33), LT1%, Lt2%, f!

'Read new profile
GetaCycle:
  If EOF(REFlu%) Then Exit Sub 'XXXX
  If Forward2 Then Cycle = Cycle + 1 Else Cycle = Cycle - 1
  
  If Cycle% < 1 Then
    Cycle% = 1
    If tmrGetCycle2.Enabled Then tmrGetCycle2.Enabled = False: cmdREFloc(1).Caption = "&Continue"
  End If
  
  If Cycle% > REFfileRecords Then
    Cycle% = REFfileRecords
    If tmrGetCycle2.Enabled Then tmrGetCycle2.Enabled = False: cmdREFloc(1).Caption = "&Continue"
  End If
  
  Call REFread(REFlu%, Cycle%)
  
' Select only Cycles that have good trops
  If Nlev1 > 0 Then
    LT1 = 1: Lt2 = 1
    For i = Nlev1 To Nlev2
      If chkGeoPotHeight(0).Value = 1 Then
        RAp(i - Nlev1 + 1) = sZg(i) '/ 100
        Lt(i - Nlev1 + 1) = sZZZ(i) '/ 100
        If Lt(i - Nlev1 + 1) > Zt1 And LT1 = 1 And Zt1 < 90# Then LT1 = i - Nlev1
        If Lt(i - Nlev1 + 1) > Zt2 And Lt2 = 1 And Zt2 < 90# Then Lt2 = i - Nlev1
      Else
        RAp(i - Nlev1 + 1) = sZZZ(i) '/ 100
      End If
      RTp(i - Nlev1 + 1) = sTTT(i) '/ 10
    Next i
  End If
  ' Calculate trops in terms of geopotential height if necessary
  If chkGeoPotHeight(0).Value = 1 Then
    If LT1 > 1 Then
      f = (Zt1 - Lt(LT1)) / (Lt(LT1 + 1) - Lt(LT1))
      Zt1 = RAp(LT1) + f * (RAp(LT1 + 1) - RAp(LT1))
    End If
    If Lt2 > 1 Then
      f = (Zt2 - Lt(Lt2)) / (Lt(Lt2 + 1) - Lt(Lt2))
      Zt2 = RAp(Lt2) + f * (RAp(Lt2 + 1) - RAp(Lt2))
    End If
  End If
  With frmATP
    .Zt1 = Zt1
    .Zt2 = Zt2
    .ALTkm = pALT
    If chkEnableFit.Value = 1 Then
      .EnableFit = True
    Else
      .EnableFit = False
    End If
    If chkShowTropopause.Value = 1 Then
      .ShowTropopause = True
    Else
      .ShowTropopause = False
    End If
  End With
  'UpdateForm
  'txtEvent.Text = DateValue(Left$(yyyymmdd$, 4) + "/" + Mid$(yyyymmdd$, 5, 2) + "/" + Right$(yyyymmdd$, 2)) + TimeValue(fSecToTstring(UTsec, True))
  Tac = OATmtp  'For PlotUpdate
  If PlotUpdateFlag Then frmATP.ATPupdate

End Sub

Sub BlendROABtime(RAOBlu%, Rec1&, Rec2&, Wt1!, GA!(), PA!(), Tt!(), RH!(), Nrl%, AvgTdiff!, RmsTdiff!)
Dim i%, j%, k%, M%, n%, jj%, ii%, Nrl1%, Nrl2%, Test As Boolean, Sum1!, Sum2!, Zp!

Dim GA1!(1 To 500), PA1!(1 To 500), RH1!(1 To 500), TT1!(1 To 500)
Dim GA2!(1 To 500), PA2!(1 To 500), RH2!(1 To 500), TT2!(1 To 500)
Dim Isame%, Tdiff!(1 To 500)
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
If optSite(0).Value Then
  OAT11 = fTinterp(ALTkm, ZPraob(), TZraob(), Nlevr)
Else
  OAT21 = fTinterp(ALTkm, ZPraob(), TZraob(), Nlevr)
End If

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
If optSite(0).Value Then
  OAT12 = fTinterp(ALTkm, ZPraob(), TZraob(), Nlevr)
Else
  OAT22 = fTinterp(ALTkm, ZPraob(), TZraob(), Nlevr)
End If

'Interpolate so both RAOBs have same levels
'A RAOB levels are retained, but to each is added the interpolated level from the other.
Nrl = 1: i = 1: j = 1: k = 1: ii = Nrl1: jj = Nrl2: Isame = 0
Do

'  For iii = 1 To 20: Debug.Print iii; GA1(iii); GA2(iii): Next iii

If GA1(i) > 14 Then
j = j
End If
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
    jj = jj + 1           'Number of RAOB 2 levels (old plus added)
    
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
    ii = ii + 1       'Add a RAOB 1 level
    
  End Select
  j = j + 1
  i = i + 1
  Nrl = Nrl + 1
  'Debug.Print i; j; ii; jj; GA1(i); GA2(j)
Loop Until i > ii Or j > jj
' Add code to deal with extending RAOB
'If j > jj Then  'extend series 2
'  For M = j To ii
'    PA2(M) = fPAinterp(GA2(), PA2(), jj, GA1(M))
'    GA2(M) = GA1(M)      'Set geometric altitudes equal
'    Tt2(M) = fXAinterp(GA2(), Tt2(), jj, GA1(M))
'    RH2(M) = fXAinterp(GA2(), RH2(), jj, GA1(M))
'  Next M
'Else
'  For M = i To jj
'    PA1(M) = fPAinterp(GA1(), PA1(), ii, GA2(M))
'    GA1(M) = GA2(j)   'Set geometric altitudes equal
'    Tt1(M) = fXAinterp(GA1(), Tt1(), ii, GA2(M))
'    RH1(M) = fXAinterp(GA1(), RH1(), ii, GA2(M))
'  Next M
'End If
If j > jj Then Nrl = jj Else Nrl = ii
'Nrl = Nrl1 + Nrl2 - Isame
  
' Test plot of interpolated profiles
Test = False
If Test Then
  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA1(i%))
    Tplot(i%) = TT1(i%)
  Next i
  UpdateDisplay

  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA2(i%))
    Tplot(i%) = TT2(i%)
  Next i
  UpdateDisplay
End If

' Now interpolate levels in time
  j = 0
  For i = 1 To Nrl    'Write out as increasing pressure altitude [m]
    GA(i) = GA1(i) * Wt1 + GA2(i) * (1 - Wt1)
    PA(i) = PA1(i) * Wt1 + PA2(i) * (1 - Wt1)
    Tt(i) = TT1(i) * Wt1 + TT2(i) * (1 - Wt1)
    RH(i) = RH1(i) * Wt1 + RH2(i) * (1 - Wt1)
    Zp = fPtoZ(PA(i))
    If Zp > 14.5 Then
    j = j
    End If
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
Sub BlendROABspace(RAOBlu%, Rec1&, Rec2&, Wt1!, GA!(), PA!(), Tt!(), RH!(), Nrl%)

End Sub


Sub dTMRItable()
Dim V!(1 To 10), Filename$, lu%, SaveCycle&, TotalRec%, T1116!, T1017!, X$
  
  Filename$ = Left$(Drive1.Drive, 2) + Rdir$ + Mission$ + "\RAOB\T1017.txt"
  lu = FreeFile
  Open Filename$ For Output As #lu
  
  SaveCycle = Cycle
  Cycle = 0
  TotalRec = 0
  Do
    Cycle = Cycle + 1

    Call RAOBread(RAOBlu, Cycle)
    
    If fFilter Then   'Make sure filter criteria are satisfied
      TotalRec = TotalRec + 1
      T1116 = fDeltaT(11#, 16#, ZPraob!(), TZraob!(), Nlevr%)
      T1017 = fDeltaT(10#, 17#, ZPraob!(), TZraob!(), Nlevr%)
      V(1) = WMOnumber: V(2) = WMOlatitude: V(3) = WMOlongitude
      V(4) = Iyear: V(5) = Iday: V(6) = T1017: V(7) = T1116
'                                12345678901234567890123456789
      'If Val(WMO4l) = 0 Then WMO4l = "    "
      X$ = WMO4l + " " + fUsing("#0000 ###0.000 ###0.000 ###0 ##0 ##0.00 ##0.00", V())
      Print #lu, X$
      'Debug.Print x$
      txtTotalRec.Text = Str$(TotalRec)
      DoEvents
    End If
  Loop Until EOF(RAOBlu) Or Cycle = FileRecords
  Cycle = SaveCycle

Close lu
End Sub

Function fMRI!(dTMRI!)
  If dTMRI > 100 Then fMRI = -1: Exit Function
  Select Case dTMRI
  Case Is >= 38: fMRI = 0
  Case Is >= 24: fMRI = (38 - dTMRI) / 14
  Case Is >= 5: fMRI = 1
  Case Is >= 1: fMRI = (5 - dTMRI) / 4
  Case Else: fMRI = 2
  End Select
End Function

Sub Doy()
'Julian Date: 1997 245 0448.5   1998 245 0813.5  1999  245 1178.5

'                   Oct             1997                 Oct
' GPS WK Sun Mon Tue Wed Thu Fri Sat   GPS WK Sun Mon Tue Wed Thu Fri Sat
'   925                1   2   3   4     925              274 275 276 277
'   926    5   6   7   8   9  10  11     926  278 279 280 281 282 283 284
'   927   12  13  14  15  16  17  18     927  285 286 287 288 289 290 291
'   928   19  20  21  22  23  24  25     928  292 293 294 295 296 297 298
'   929   26  27  28  29  30  31         929  299 300 301 302 303 304
''
'
'                   Nov             1997                 Nov
' GPS WK Sun Mon Tue Wed Thu Fri Sat   GPS WK Sun Mon Tue Wed Thu Fri Sat
'   929                            1     929                          305
'   930    2   3   4   5   6   7   8     930  306 307 308 309 310 311 312
'   931    9  10  11  12  13  14  15     931  313 314 315 316 317 318 319
'   932   16  17  18  19  20  21  22     932  320 321 322 323 324 325 326
'   933   23  24  25  26  27  28  29     933  327 328 329 330 331 332 333
'   934   30                             934  334
End Sub


Function fFilter() As Boolean
Dim f(0 To 14) As Boolean, X!, Z1!, Z2!, Z3!, Tr1!, Tr2!, Tr3!, T1!, T2!, T3!
Dim UT!, DT0!, DT1!, DT2!, dT3!
' Set local flags FALSE
For i = 0 To 14: f(i) = False: Next i
fFilter = False

If EnableFilter(0) Then  'Time window
  If chkMonthOnly.Value = 1 Then
    If Imonth >= Val(cmbStart(1).Text) And Imonth <= Val(cmbEnd(1).Text) Then f(0) = True Else Exit Function
  Else
    If chkHourOnly.Value = 1 Then
      If Ihour >= Val(cmbStart(3).Text) And Ihour <= Val(cmbEnd(3).Text) Then f(0) = True Else Exit Function
    Else
      If DandT >= DandTstart And DandT <= DandTend Then f(0) = True Else Exit Function
    End If
  End If
Else
  f(0) = True
End If

If EnableFilter(1) Then  'Site Selection
  f(1) = False
  For i = 0 To lstSite.ListCount - 1
    If Len(lstSite.List(i)) < 5 Then
      If UCase$(Trim(WMO4l)) = UCase$(lstSite.List(i)) Then f(1) = True: Exit For
    Else
      If WMOnumber = lstSite.List(i) Then f(1) = True: Exit For
    End If
  Next i
Else
  f(1) = True
End If

If EnableFilter(2) Then  'Region window
    If WMOlatitude > LAT1 And WMOlatitude < LAT2 Then
      If WMOlongitude > LON1 And WMOlongitude < LON2 Then f(2) = True Else Exit Function
    End If
Else
  f(2) = True
End If

If EnableFilter(3) Then  'Minimum acceptable burst altitude
  If Nlevr > 0 Then
    If ZPraob(Nlevr) >= BurstAlt Then f(3) = True Else Exit Function
  End If
Else
  f(3) = True
End If

If EnableFilter(4) Then  'Range for dTMRI
  dTMRI = fdTMRI(ZPraob(), TZraob(), Nlevr, Val(txtT10.Text), Val(txtT17.Text))
  If dTMRI >= txtT1017(0).Text And dTMRI <= txtT1017(1).Text Then f(4) = True Else Exit Function
Else
  f(4) = True
End If

If EnableFilter(5) Then  'Write only if ijkl flag <> 0
  If ijkl <> 0 Then f(5) = True Else Exit Function
Else
  f(5) = True
End If

If EnableFilter(6) Then  'Write only if GOOD scan
  If RAOBgood Then f(6) = True Else Exit Function
Else
  f(6) = True
End If

If EnableFilter(7) Then  'Write only if GOOD trop
  If TropGood Then
    If Ztrop > Val(txtTropMin.Text) And Ztrop < Val(txtTropMax.Text) Then f(7) = True Else Exit Function
  End If
Else
  f(7) = True
End If

If EnableFilter(8) Then  'Write only if MRI is in range
  X = fMRI(dTMRI)
  If X >= Val(txtMRI(0).Text) And X <= Val(txtMRI(1).Text) Then f(8) = True Else Exit Function
Else
  f(8) = True
End If

If EnableFilter(9) Then  'Write only if MRI is in range
' Arctic threshold: T1=210 at 10 km and T2=200 at 20 km
 Z1 = 2 / kft_km: Z2 = 2 / kft_km
'Coastal
'T1 = 290: T2 = 296  '>25 km, SOLVE8.RAOB
'T1 = 296: T2 = 297.5 '>25 km, SOLVE8.RAOB
'T1 = 297.5: T2 = 299 '>25 km, SOLVE8.RAOB
'T1 = 299: T2 = 330 '>25 km, SOLVE8.RAOB
'FWD, SHV
'T1 = 290: T2 = 302  '>25 km, SOLVE8.RAOB
T1 = 297: T2 = 330 '>25 km, SOLVE8.RAOB

' z1 = 20: z2 = 20
'T1 = 170: T2 = 193  '>25 km, SOLVE8.RAOB
'T1 = 170: T2 = 196
'T1 = 196: T2 = 202
'T1 = 202: T2 = 210
'T1 = 210: T2 = 240: T3 = 220
'North America
'  z1 = 8: z2 = 26
' T1 = 231: T2 = 215.5
  If Nlevr > 2 Then
  Tr1 = fTinterp(Z1, ZPraob(), TZraob(), Nlevr)
  Tr2 = fTinterp(Z2, ZPraob(), TZraob(), Nlevr)
  Else
  X = X
  End If
'  Tr3 = fTinterp(z3, ZPraob(), TZraob(), Nlevr)
'  If Tr1 >= T1 And Tr1 < T2 Then f(9) = True Else Exit Function
'  If Tr1 <= T1 And Tr2 < T2 Then f(9) = True Else Exit Function
'  If Tr1 >= T1 And Tr2 > T2 Then f(9) = True Else Exit Function
'  If Tr1 <= T1 And Tr2 < T2 Then f(9) = True Else Exit Function
'  If Tr1 <= T1 And Tr2 > T2 Then f(9) = True Else Exit Function
  If Tr1 >= T1 Then f(9) = True Else Exit Function

Else
  f(9) = True
End If

If EnableFilter(10) Then  'UT window
  If Val(txtUT1.Text) < Val(txtUT2.Text) Then
    If Ihhmm / 100 > Val(txtUT1.Text) And Ihhmm / 100 < Val(txtUT2.Text) Then
      f(10) = True
    Else
      Exit Function
    End If
  Else
    UT = Ihhmm / 100
    If UT < Val(txtUT1.Text) Then UT = UT + 24
    If UT > Val(txtUT1.Text) And UT < Val(txtUT2.Text) + 24 Then
      f(10) = True
    Else
      Exit Function
    End If
  End If
Else
  f(10) = True
End If

If EnableFilter(11) Then  'Write only if low level inversion exists
  i = 3
  DT0 = TZraob(1) - TZraob(2)
  Do
    i = i + 1
    DT1 = TZraob(i - 1) - TZraob(i)
    DT2 = TZraob(i - 2) - TZraob(i)
    dT3 = TZraob(i - 3) - TZraob(i)
    If (DT0 < -1 Or dT3 < -1 Or DT1 < -0.5) And ZPraob(i) > 0# / kft_km Then
      Exit Do
    End If
  Loop Until ZPraob(i) > 5 / kft_km
    If (DT0 < -1 Or dT3 < -1 Or DT1 < -0.5) Then f(11) = True Else Exit Function
  
Else
  f(11) = True
End If

If EnableFilter(12) Then  'Write only if coldest T < given value
  If Zcoldest > Val(txtZcoldest1.Text) And Zcoldest < Val(txtZcoldest2.Text) Then
    If Tcoldest < Val(txtColdThreshold.Text) Then f(12) = True Else Exit Function
  End If
Else
  f(12) = True
End If

If EnableFilter(13) Then  'Write only if Nlevr > given value
  If Nlevr > Val(txtNlevr.Text) Then f(13) = True Else Exit Function
Else
  f(13) = True
End If

If EnableFilter(14) Then  'Minimum acceptable starting RAOB altitude
  If Nlevr > 0 Then
    If ZPraob(1) <= StartAlt Then f(14) = True Else Exit Function
  End If
Else
  f(14) = True
End If

' Check to make sure that all filters are satisfied
If f(0) And f(1) And f(2) And f(3) And f(4) And f(5) And f(6) And f(7) And f(8) And f(9) And f(10) And f(11) And f(12) And f(13) Then fFilter = True Else fFilter = False

End Function

Sub FindRAOBtropBLG(z!(), T!(), Nlev%, Zt1!, TT1!, Lt%)
Dim i%, j%, Ztop!, Zlo!, Zhi!, Tlo!, Thi!, ThicknessCrit!, LRi!, LRavg!
Dim Nsteps%, Zstep!, Z1!, T1!, Z2!, T2!

  ThicknessCrit = 2#
  Nsteps = 40                '50 meter steps
  Zstep = ThicknessCrit / Nsteps
  Zt1 = 0#: TT1 = 0#

' Roe uses 1.0 km, but my retrievals still have artifact inflections at low end
  j = 0: Ztop = z(Nlev)
' Find first level above 500 mb = 5.6 km
  Do
    j = j + 1
    If j > Nlev Then Exit Sub
    Zlo = z(j)
  Loop While Zlo < 5.6

' Check for LR>-2 K/km on any RAOB segment
TryAnotherZ:
  Do
    Zlo = z(j):  Tlo = T(j)
    If j + 1 > Nlev Then Exit Sub
    Zhi = z(j + 1)
    Thi = T(j + 1)
    LRi = (Thi - Tlo) / (Zhi - Zlo)
    j = j + 1
  ' Debug.Print Zlo; Zhi; Tlo; Thi; LRi
  Loop Until LRi >= -2
  
' OK, now check that AVERAGE LR over next 2 km is >-2 K/km
  Zhi = Zlo + ThicknessCrit
  Thi = fTinterp(Zhi, z(), T(), Nlev)
  If Thi = 0# Then Exit Sub  'Ran out of RAOB
  
  LRavg = 0#:  i = 0
  For Z1 = Zlo To Zhi - Zstep / 2 Step Zstep
    i = i + 1
    Z2 = Z1 + Zstep
    T1 = fTinterp(Z1, z(), T(), Nlev)
    T2 = fTinterp(Z2, z(), T(), Nlev)
    LRi = (T2 - T1) / (Z2 - Z1)
    LRavg = LRavg + LRi
    'Debug.Print i; Z1; Z2; LRi; LRavg / i
  Next Z1
  LRavg = LRavg / Nsteps
  If LRavg <= -2 Then GoTo TryAnotherZ

  Zt1 = Zlo
  TT1 = Tlo
  Lt = j - 1
End Sub

Sub Main()
Dim SScale%, zc%, zs%, Tc%, Ts%, X%, y%, TL%, Zb%, Zt%
' Initialize plot window center position, size and borders
  SScale% = 1                 'Default Plot scale (1, 2, or 3)           1,2,3
  zc% = 12: zs% = 24: Tc% = 250: Ts% = 120
  X% = Ts% / 2 ^ (SScale% - 1) / 2: TL% = Tc% - X%: 'Tr% = Tc% + x%
  y% = zs% / 2 ^ (SScale% - 1) / 2: Zb% = zc% - y%: Zt% = zc% + y%

  tmrGetCycle.Interval = 100 'Auto Step cycle time in ms
  tmrGetCycle.Enabled = False
  cmdStop.Caption = "&Continue"
  Cycle = 0

End Sub

Sub GetRAOBcycle(lu%, Cycle&, ASCIIinput As Boolean)
Dim i%

If lu% = 0 Then Exit Sub
If ASCIIinput Then  'Assume SJK
  ConvertSJK (lu%)  'Read one complete record.
Else  'Read Binary records
'Read new profile
GetaCycle:
  If lu% = 0 Then Exit Sub 'XXXX
  If Forward Then Cycle = Cycle + 1 Else Cycle = Cycle - 1
'  'Debug.Print Forward; Record%
'
  If Cycle < 1 Then
    Cycle = 1
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
'
  If Cycle > FileRecords Then
    Cycle = FileRecords
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
'
  Call RAOBread(lu%, Cycle)
   
  For i = 0 To 5:  txtList(i).Text = "": Next i  'Clear edit line

  If Not GOTOcommand Then Call UpdateRAOBlisting
End If
  txtProgress.Text = Str$(Cycle)
End Sub
Sub DetermineROABtoShow()
' Always start with original RAOB, then add requested editting
  If chkGeoPotHeight(0).Value = 1 Then
    For i% = 1 To Nlevr
      Zplot(i%) = ZGraob(i%)
      Tplot(i%) = TZraob(i%)
      Rplot(i%) = RZraob(i%)
    Next i
    frmRAOB.ShowPressureAltitude = False
    If frmRAOB.PlotT0 = True Then
      frmRAOB.Caption = "RAOB - Geopotential Height vs Temperature"
    Else
      frmRAOB.Caption = "RAOB - Geopotential Height vs Potential Temperature"
    End If
  Else
    For i% = 1 To Nlevr
      Zplot(i%) = ZPraob(i%)
      Tplot(i%) = TZraob(i%)
      Rplot(i%) = RZraob(i%)
    Next i
    frmRAOB.ShowPressureAltitude = True
    If frmRAOB.PlotT0 = True Then
      frmRAOB.Caption = "RAOB - Pressure Altitude vs Temperature"
    Else
      frmRAOB.Caption = "RAOB - Pressure Altitude vs Potential Temperature"
    End If
  End If
' Note that trops are always saved in terms of pressure altitude
' If geopotential height is being used, then use ZGraob(Ltrop) to
' get geopotential height of the tropopause
  With frmRAOB
    If UseEditTrop And EditTrop Then  'Edit tropopause
    .Zt1 = ZtropE
    .TT1 = TtropE
    LT1 = LtropE
    .Zt2 = Ztrop2
    .TT2 = Ttrop2
    Lt2 = Ltrop2
    Else
    .Zt1 = Ztrop
    .TT1 = Ttrop
    If Ltrop > 0 Then LT1 = Ltrop Else LT1 = 1
    .Zt2 = Ztrop2
    .TT2 = Ttrop2
    Lt2 = 1         'Double trops not provided by any RAOB sites
    End If
  End With
  If chkDoubleTrop.Value = 0 Then frmRAOB.Zt2 = 99.9
' Check if trop should be in geopotential height
  If chkGeoPotHeight(0).Value = 1 Then
    frmRAOB.Zt1 = ZGraob(LT1)
    If Lt2 > 0 Then frmRAOB.Zt2 = ZGraob(Lt2) Else Zt2 = 99.9: Lt2 = 1
  End If
  
  If UseEditLevel And EditLevel Then   'Edit levels
    For i% = 1 To Nedit
      Tplot(LZraob(i%)) = TZraob(Nlevr + i)
    Next i%
  End If

End Sub


Sub OpenREFfile(Filename$)
Dim i%, n%, dUTsec%, UTlast&, ScanTime!

If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
  Exit Sub
Else
'  On Error GoTo errhandler
  If REFlu <> 0 Then Close REFlu
  REFlu% = FreeFile
  FileFormatIn = fREFreadFileFormat(Filename$)

  Select Case FileFormatIn
  Case 32
    Open Filename$ For Random Access Read Write As REFlu Len = Len(REF)
    REFfileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
  Case 33
    Open Filename$ For Random Access Read Write As REFlu Len = Len(REF2)
    REFfileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
  End Select
  txtREFfile.Text = Filename$
  ProgressBar2.Min = 0
  ProgressBar2.Max = REFfileRecords
  txtREFrecords.Text = REFfileRecords
'  REFread REFlu, REFfileRecords  'Get Last Record
'  ScanLength = UTsec
  REFread REFlu, 1               'Get First Record
'  If UTsec > ScanLength Then ScanLength = ScanLength + 86400
'  ScanLength = (ScanLength - UTsec) / (REFfileRecords - 1)
' Calculate average scan length for all records between n1 and n2
' that are less than 25 seconds long.
' CAN instruments have ~20 s scan length
' All other three frequency instruments ~15 s, so 25 s is >1 scan

  i = 0
  UTlast = UTsec
  ScanTime = 0#
  For n = 2 To REFfileRecords - 1
    REFread REFlu, n
    dUTsec = UTsec - UTlast
    If dUTsec < 25 Then ScanTime = ScanTime + dUTsec: i = i + 1
    UTlast = UTsec
  Next n
  ScanLength = ScanTime / i
  
  
  cmdOpenREFfile.Caption = "Close REF"

End If
  
  REFreadHEADER Filename$
  
' Initialize
  tmrGetCycle2.Interval = 100 'Auto Step cycle time in ms
  tmrGetCycle2.Enabled = False
  cmdREFloc(1).Caption = "&Continue"
  Cycle2 = 0
  txtREFrecord.Text = Str(Cycle2)
  frmATP.Visible = True
End Sub

Sub CopyArray(A!(), b!(), n%)
Dim i%
For i = 1 To n
  b(i) = A(i)
Next i
End Sub



Sub ShowProfile(T!(), z!(), Nlevr%)
    For i% = 1 To Nlevr
      Zplot(i%) = z(i) 'RAp = Zgeo or Zp depending on chkGeoPotHeight.value
      Tplot(i%) = T(i)
    Next i
    UpdateDisplay
End Sub

Function Test()
Dim h1$, h2$, h3$, A$, b$, V!(1 To 10), Zg!, TA!, Td!, Zp!
'Odd
'      IDN  DAY[SYD] TIME[HMS]      Z[M]  LEV      T[K]     TD[K]  SPD[MPS]  DIR[DEG]
'--------- --------- --------- --------- ---- --------- --------- --------- ---------
'     3953     97291         0        14 SFC     287.76    286.76       1.0       100
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234
'Even
'      IDN  DAY[SYD] TIME[HMS] TYPE        P3        P2        P1
'--------- --------- --------- ---- --------- --------- ---------
'     3953     97291         0 SIGT     10040     28676     28776
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234
h1$ = "      IDN  DAY[SYD] TIME[HMS]      Z[M]  LEV      T[K]     TD[K]  SPD[MPS]  DIR[DEG]"
h2$ = "--------- --------- --------- --------- ---- --------- --------- --------- ---------"
h3$ = "########0 ###0.0 ###0.00 #####0.00 ######0.0 ########0"
Debug.Print h1$      'write header
Debug.Print h2$
Open "d:\dc8\sonex\Papers\rs\makky202.crlf" For Input As 1
Line Input #1, A$
'Debug.Print a$

Line Input #1, A$
'Debug.Print a$

Do
b$ = ""
Line Input #1, A$
If Left$(A$, 6) = "Number" Then Close 1: Exit Function
'a$ = "     3953     97291         0 SIGT     10040     28676     28776"
'Debug.Print a$
b$ = Left$(A$, 30)
V(1) = -1#
If Len(Mid$(A$, 36, 9)) > 0 Then
  V(2) = Val(Mid$(A$, 36, 9) / 10#)
Else
  V(2) = 0#
End If
If Len(A$) > 55 And Mid$(A$, 46, 9) <> "         " Then
  V(4) = Val(Mid$(A$, 46, 9) / 100#)
Else
  V(4) = 0#
End If
If Len(Mid$(A$, 56, 9)) > 0 Then
  V(3) = Val(Mid$(A$, 56, 9) / 100#)
Else
  V(3) = 0#
End If
V(5) = 0#
V(6) = 0#
If V(2) > 0 Then Debug.Print b$ + fUsing$(h3$, V!())
Loop Until EOF(1)
Close 1
End Function

Sub UniqueRecords(ByRef OutCount&, ByRef RU&(), ByVal Record&)
Dim i&
' RU array has all unique record numbers
  If OutCount = 0 Then
    OutCount = 1
    RU(OutCount) = Record
  Else
    For i = 1 To OutCount
      If RU(i) = Record Then Exit Sub
    Next i
    OutCount = OutCount + 1
    RU(OutCount) = Record
  End If

End Sub

Private Sub cboMap_Click()
Dim Path$, Map$

Path$ = "C:\MTP\Maps\RAOB\"
Select Case cboMap.ListIndex
Case 0: Map$ = Path$ + "naconf.jpg"
Case 1: Map$ = Path$ + "samer.jpg"
Case 2: Map$ = Path$ + "pac.jpg"
Case 3: Map$ = Path$ + "nz.jpg"
Case 4: Map$ = Path$ + "ant.jpg"
Case 5: Map$ = Path$ + "np.jpg"
Case 6: Map$ = Path$ + "europe.jpg"
Case 7: Map$ = Path$ + "africa.jpg"
Case 8: Map$ = Path$ + "seasia.jpg"
End Select

With frmMap
.Caption = "Map of " + Trim(cboMap.Text)
.picMap.Picture = LoadPicture(Map$)
.Width = .picMap.Width + 100
.Height = .picMap.Height + 300
.Left = (1024 * Screen.TwipsPerPixelX - .Width) / 2
.Top = (768 * Screen.TwipsPerPixelY - .Height) / 2
.Show
End With

End Sub


Private Sub cboRAOBtype_Click()
' 0 =  150 levels to accomodate latest Wyoming soundings with more than sig/man levels
' 1 =   70 levels to accomodate original significant/manadatory levels
' 2 = 2000 levels to accomodate very high resolution soundings
' 3 =  500 levels to accomodate COSMIC soundings
RAOBtype = cboRAOBtype.ListIndex
nRAOBlevels = Val(cboRAOBtype.Text)
End Sub


Private Sub cboRAOBtypeOUT_Click()
' 0 =  150 levels
' 1 =   70 levels
' 2 = 2000 levels
RAOBtypeOUT = cboRAOBtypeOUT.ListIndex
nRAOBlevelsOUT = Val(cboRAOBtypeOUT.Text)

End Sub


Private Sub chkAutoView_Click()
If chkAutoView.Value = 1 Then
  AutoView = True
Else
  AutoView = False
End If
End Sub

Private Sub chkConvertLiquid_Click()
'If chkConvertLiquid.Value = 1 Then
'  RhoLstart = Val(txtRhoL.Text)
'  RhoLvapor = 0#
'Else
'  RhoLvapor = 0#
'End If
End Sub

Private Sub chkEnableRH_Click()
If chkEnableRH.Value = 1 Then
  frmRAOB.ShowRH = True
Else
  frmRAOB.ShowRH = False
End If
End Sub

Private Sub chkFindTrop_Click()
' Force the use of Geopotential Height if Trop is to be found
If chkFindTrop.Value = 1 Then chkGeoPotHeight(0).Value = 1
End Sub


Private Sub chkGeoPotHeight_Click(Index As Integer)
' Synchronize the two check boxes
Select Case Index
Case 0
  If chkGeoPotHeight(0).Value = 0 Then chkGeoPotHeight(1).Value = 0 Else chkGeoPotHeight(1).Value = 1
Case 1
  If chkGeoPotHeight(1).Value = 0 Then chkGeoPotHeight(0).Value = 0 Else chkGeoPotHeight(0).Value = 1
End Select

Select Case chkGeoPotHeight(Index).Value
Case 0 'Pressure Altitude
  With frmATP
    .Caption = "JPL MTP - Pressure Altitude vs Temperature"
    .ShowPressureAltitude = True
  End With
  With frmRAOB
    .Caption = "RAOB - Pressure Altitude vs Temperature"
    .ShowPressureAltitude = True
  End With
Case 1 'Geopotential Height
  With frmATP
    .Caption = "JPL MTP - Geopotential Height vs Temperature"
    .ShowPressureAltitude = False
  End With
  With frmRAOB
    .Caption = "RAOB - Geopotential Height vs Temperature"
    .ShowPressureAltitude = False
  End With
End Select

If RAOBlu > 0 Then
  If Forward Then Cycle = Cycle - 1 Else Cycle = Cycle + 1
  tmrGetCycle_Timer ' Reread cycle to get new ordinate values
  frmRAOB.RAOBupdate
End If
If REFlu > 0 Then
  If Forward2 Then Cycle2 = Cycle2 - 1 Else Cycle2 = Cycle2 + 1
  tmrGetCycle2_Timer ' Reread cycle to get new ordinate values
  frmATP.ATPupdate
End If
End Sub


Private Sub chkKeepSelection_Click()

If chkKeepSelection.Value = 1 Then
  cmdSelect(2).Caption = "&Keep"
Else
  cmdSelect(2).Caption = "&Remove"
End If

End Sub

Private Sub chkMultipleScans_Click()
  If chkMultipleScans.Value = 1 Then frmRAOB.PlotSingleScan = False Else frmRAOB.PlotSingleScan = True
  With frmRAOB
   .EnableFit = False
   .SetDefaultScales
   .RAOB_Plot_Set
   .RAOB_Plot_Axes_Labels
   .RAOB_Plot_Grid_Marks
   .RAOB_Plot_Tic_Marks
  End With
  Documentation$ = ""
End Sub

Private Sub chkPlotSingleATPscan_Click()
If chkPlotSingleATPscan.Value = 1 Then frmATP.PlotSingleScan = True Else frmATP.PlotSingleScan = False
End Sub

Private Sub chkPlotT_Click(Index As Integer)

Select Case Index
Case 0
  If chkPlotT(Index).Value = 1 Then
    frmRAOB.PlotT0 = True
  Else
    frmRAOB.PlotT0 = False
  End If

Case 1
  If chkPlotT(Index).Value = 1 Then
    frmRAOB.PlotT1 = True
  Else
    frmRAOB.PlotT1 = False
  End If

Case 2
  If chkPlotT(Index).Value = 1 Then
    frmRAOB.PlotT2 = True
  Else
    frmRAOB.PlotT2 = False
  End If

Case 3
  If chkPlotT(Index).Value = 1 Then
    frmRAOB.PlotT3 = True
  Else
    frmRAOB.PlotT3 = False
  End If

End Select

End Sub

Private Sub chkShowATP_Click()
If chkShowATP.Value = 1 Then frmATP.Visible = True Else frmATP.Visible = False
End Sub

Private Sub chkShowDots_Click()

If chkShowDots.Value = 1 Then
  frmRAOB.ShowDots = True
Else
  frmRAOB.ShowDots = False
End If
End Sub

Private Sub chkTexAQS_Click()
If chkTexAQS.Value = 1 Then
  frmRAOB.UseTexAQS = True
Else
  frmRAOB.UseTexAQS = False
End If
frmRAOB.RAOB_Initialize
With frmRAOB
  .ShowAircraft = False
  .ShowTropopause = False
  .ShowDocument = False
  .ShowSelectLevel = False
  .ShowPressureAltitude = False
End With

frmRAOB.RAOBupdate
End Sub

Private Sub cmbEnd_Click(Index As Integer)
DandTend = DateValue(cmbEnd(0).Text + " " + cmbEnd(1).Text + " " + cmbEnd(2).Text)
DandTend = DandTend + TimeValue(cmbEnd(3).Text + ":00:00")

End Sub


Private Sub cmbInputMode_Click()

InputMode = cmbInputMode.ListIndex

Select Case InputMode
Case 0
  txtConvertIn.Enabled = False
  cmdConvertIn.Enabled = False
  lblSource(0).Enabled = True
  lblSource(1).Enabled = True
  txtPath.Enabled = True
  txtFileMask.Enabled = True
Case 1, 2
  txtConvertIn.Enabled = True
  cmdConvertIn.Enabled = True
  lblSource(0).Enabled = False
  lblSource(1).Enabled = False
  txtPath.Enabled = False
  txtFileMask.Enabled = False
End Select

End Sub


Private Sub cmbSrcFormat_Click()
Dim X$

SourceFormat = cmbSrcFormat.ListIndex
Select Case SourceFormat
Case 0 'Forecast Systems Laboratory (FSL)
  txtFileMask.Text = "*.fsl": cboRAOBtype.ListIndex = 0
Case 1 'Wyoming
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 0
Case 2 'BLG
  txtFileMask.Text = "r*.*":  cboRAOBtype.ListIndex = 0
Case 3 'SJK
  txtFileMask.Text = "*.sjk":  cboRAOBtype.ListIndex = 0
Case 4 'Pfister
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 0
Case 5 'EAFB
  txtFileMask.Text = "edw*.txt":  cboRAOBtype.ListIndex = 0
Case 6 'UCSD
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 0
Case 7 'Makky
  txtFileMask.Text = "RS*.*":  cboRAOBtype.ListIndex = 0
Case 8 'PNNL
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 2
Case 9 'Andros Island (CAMEX-4)
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 2
Case 10 'CLS5 Dropsondes (CAMEX-4)
  txtFileMask.Text = "*.QCCLS":  cboRAOBtype.ListIndex = 2
Case 11 'TAMU Fake Dropsondes (TexAQS 2000)
  txtFileMask.Text = "*.txt":  cboRAOBtype.ListIndex = 2
End Select

X$ = Left$(Drive1.Drive, 2) + "\MTP\Data" + Rdir$ + Mission$ + "\RAOB\" + Mission$
Select Case cboRAOBtype.ListIndex
Case 0:  txtConvertOut.Text = X$ + ".RAOB2"
Case 1:  txtConvertOut.Text = X$ + ".RAOB"
Case 2:  txtConvertOut.Text = X$ + ".LRF"
Case 3:  txtConvertOut.Text = X$ + ".RAOB3"
End Select

End Sub
Private Sub cmbStart_Click(Index As Integer)
DandTstart = DateValue(cmbStart(0).Text + " " + cmbStart(1).Text + " " + cmbStart(2).Text)
DandTstart = DandTstart + TimeValue(cmbStart(3).Text + ":00:00")
cmbEnd(Index).Text = cmbStart(Index).Text
DandTend = DateValue(cmbEnd(0).Text + " " + cmbEnd(1).Text + " " + cmbEnd(2).Text)
DandTend = DandTend + TimeValue(cmbEnd(3).Text + ":00:00")
' Need to fix this so that when the date is say 31 and the month
' is changed to Feb you don't get an error.
End Sub


Private Sub cmdATP_Click(Index As Integer)
Dim V!(1 To 20), CurrentColor%, CurrentThickness%, X$, y$, i%, j%
Dim RAg!(1 To 33), Tint!, Zint!, Gint!, n%, n1%, n2%, NlevrSave%

  Select Case Index
  Case 0 'Transfer ATP to RAOB plot
    If REFlu = 0 Then
      MsgBox "You need to OPEN a REF file before using this command!", vbOKOnly
      Exit Sub
    End If
    Nlev = Nlev2 - Nlev1 + 1
    For i% = 1 To Nlev
      Zplot(i%) = RAp(i) 'RAp = Zgeo or Zp depending on chkGeoPotHeight.value
      Tplot(i%) = RTp(i)
      ''Debug.Print i; RAp(i); RTp(i)
    Next i
    NlevrSave = Nlevr
    Nlevr = Nlev
    With frmRAOB
      .Tac = OATnav
      .ALTkm = pALT
      .ShowAircraft = True
      .EnableFit = True
      .PlotSingleScan = False
      chkMultipleScans.Value = 1
    ' Save RAOB display parameters
      CurrentColor = .LineColor
      CurrentThickness = .LineThickness
    ' Set new display parameters
      .LineColor = frmATP.LineColor
      .LineThickness = frmATP.LineThickness
      .RAOBupdate
      .EnableFit = False     'So RAOB does not try to fit!
    ' Restore RAOB display parameters
      .LineColor = CurrentColor
      .LineThickness = CurrentThickness
    End With
    Nlevr = NlevrSave
    
  Case 1 'Write RAOBs in select list to a single ASCII file
    OUTlu = FreeFile
    Open txtASCIIout.Text For Output As OUTlu
    y$ = "#00.0" + vbTab + "#000.0" + vbTab + "#0.00" + vbTab + "#0.00" + vbTab + "#00.0"
    
    For i = 0 To lstSelect.ListCount - 1
      X$ = lstSelect.List(i)
      Cycle = Right$(X$, 5)
      Call RAOBread(RAOBlu, Cycle)
      ' Write header line
      X$ = Format(WMOnumber, "00000") + vbTab + Trim(WMO4l)
      Print #OUTlu, X$
      X$ = Mid$(Format(Iyear, "0000"), 3, 2) + vbTab + Format(Imonth, "00") + vbTab + Format(Iday, "00") + vbTab + Format(Ihour, "00")
      Print #OUTlu, X$
      X$ = "Trop=" + vbTab + Format(Ztrop, "#0.00")
      Print #OUTlu, X$
      X$ = "T[K]" + vbTab + "P[mb]" + vbTab + "Zp[km]" + vbTab + "Zg[km]" + vbTab + "RH[%]"
      Print #OUTlu, X$
      For j = 1 To Nlev
        X$ = Format(TZraob(j), "#00.0") + vbTab + Format(PZraob(j), "#000.0") + vbTab + Format(ZPraob(j), "#0.00")
        X$ = X$ + vbTab + Format(ZGraob(j), "#0.00") + vbTab + Format(RZraob(j), "#00")
        Print #OUTlu, X$
      Next j
      Print #OUTlu, " "
    Next i
    Close OUTlu
    
  Case 2 'Write Sampled ATP to an ASCII file
    OUTlu = FreeFile
    Open txtASCIIout.Text For Output As OUTlu
    Print #OUTlu, "ATP Profile and Number Density Using:"
    Print #OUTlu, "Platform" + vbTab + "Mission" + vbTab + "YYYYMMDD" + vbTab + "UTks" + vbTab + "Trop[km]"
    
    'Call REFread(REFlu, Cycle2)
    X$ = Platform$ + vbTab + Mission$ + vbTab
    X$ = X$ + YYYYMMDD$ + vbTab + Format(UTsec / 1000#, "#00.000") + vbTab
    X$ = X$ + Format(Zt1, "#0.00")
    Print #OUTlu, X$
    Nlev = (Val(txtAvgOpt(0).Text) - Val(txtAvgOpt(2).Text)) / Val(txtAvgOpt(1).Text) + 1
    n1 = Val(txtAvgOpt(2).Text) / Val(txtAvgOpt(1).Text)
    n2 = Val(txtAvgOpt(0).Text) / Val(txtAvgOpt(1).Text)
    Print #OUTlu, "T[K]" + vbTab + "pALT[km]" + vbTab + "gALT[km]" + vbTab + "Trms[K]" + vbTab + "ND[#/cm3 E21]" + vbTab + "dND[%]"
    Imonth = Val(Mid$(YYYYMMDD$, 5, 2))
    ' Do the SPLINE fit to retrieved ATP----------------------------<<<<<
    For i = Nlev1 To Nlev2
      RAp(i - Nlev1 + 1) = zzz(i) / 100#
      RAg(i - Nlev1 + 1) = Zgeo(i) / 100#
      RTp(i - Nlev1 + 1) = TTT(i) / 10#
    Next i
    Call SPLINE(RAp!(), RTp!(), TD1#(), TD2#(), 1, Nlev2 - Nlev1 + 1)

    ' And calculate the interpolated values for the spline fit display
    For i = n1 To n2
      If i = 0 Then Zint = 0.1 Else Zint = i * Val(txtAvgOpt(1).Text)
      If Zint >= RAp(1) And Zint < RAp(Nlev2 - Nlev1 + 1) Then
        Tint = SPLINT(RAp!(), RTp!(), TD2#(), 1, Nlev2 - Nlev1 + 1, Zint, True)
        'Gint = RAg(n) + (Zint - RAp(n)) * (RAg(n + 1) - RAg(n)) / (RAp(n + 1) - RAp(n))
        X$ = Format(Tint, "#00.0") + vbTab + Format(Zint, "#0.00") + vbTab
'        x$ = x$ + Format(Gint, "#0.00") + vbTab + Format(0#, "#0.0") + vbTab
'        NDstd = fStandardDensity(Imonth, Gint, Latitude) * cAo / cMd / 1E+21
'        NDmj = fNDmj(0#, fZtoP(Zint), Tint) / 1E+21 '#/cm3 /E21
'        x$ = x$ + Format(NDmj, "#0000") + vbTab + Format(100 * (NDmj - NDstd) / NDstd, "#0.0")
        Print #OUTlu, X$
      End If
    Next i
    Close OUTlu
    
  End Select

End Sub

Private Sub cmdATPgo_Click()
Dim RecordFound As Boolean, tks&, LoopCount%, RecInc%
  
On Error GoTo InvalidRequest
Forward2 = True
Select Case cboATPgo.Text
Case "Rec No"
  Record = Val(txtATPgo) - 1
Case "ks"
  Record = (Val(txtATPgo) * 1000 - UTmin) / ScanLength
Case "seconds"
  Record = (Val(txtATPgo) - UTmin) / ScanLength
Case "hhmmss"
  Record = (fTstringToSec&(txtATPgo, False) - UTmin) / ScanLength
Case "hh:mm:ss"
  Record = (fTstringToSec&(txtATPgo, True) - UTmin) / ScanLength
End Select
' Check limits
  Select Case Record
  Case Is < 0: Record = 0
  Case Is > REFfileRecords: Record = REFfileRecords - 1
  End Select
  
  Cycle2 = Record
  RecordFound = False
  Forward = True
  LoopCount% = 0
  Do
    LoopCount% = LoopCount% + 1
    tmrGetCycle2_Timer
    Select Case cboATPgo.Text
    Case "Rec No"
      RecordFound = True
    Case "ks"
      RecInc = (Val(1000 * txtATPgo.Text - UTsec)) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "seconds"
      RecInc = (Val(txtATPgo.Text - UTsec)) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "hhmmss"
      tks& = fTstringToSec&(txtATPgo.Text, False)
      RecInc = (tks& - UTsec) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    Case "hh:mm:ss"
      tks& = fTstringToSec&(txtATPgo.Text, True)
      RecInc = (tks& - UTsec) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Record = Record + RecInc
    End Select
    Cycle2 = Cycle2 + RecInc
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

Private Sub cmdBack_Click()
Forward = False
tmrGetCycle_Timer

End Sub



Private Sub cmdBias_Click()
txtAVG.Text = "2.0"
End Sub

Private Sub cmdBlend_Click(Index As Integer)

Dim lu%, lu2%, File$, Nevents%, Rec1&, Rec2&, Tre!, Tav!, Te!, Prefix$, PrefixR$, Wt1!, Wt2!, Navg%, X!
Dim n%, i%, j%, ii%, jj%, M%, Tmin!, Tmax!, Traob!, Tmtp!, TZ1!(1 To 503), TZ2!(1 To 503), Zp1!(1 To 503), Zp2!(1 To 503), nTZ1%, nTZ2%
Dim Ln1!, Ln2!, Ln3!, LT1!, Lt2!, Lt3!, D1!, D2!, Distance!, Line$, PNGname$, OutCount&, iL&
Dim A$, iD%, iM%, iY%, UT0!, UT1!, UT2!, DT1 As Date, DT2 As Date, Filename$, T As Date, FileName2$, RAOBlu2%
Dim Pint!, Gint!, Rint!, Tint!, WtA!, jmax%, DTevent As Date, Thickness!, Tn_Tr!
Dim LR11!, LR12!, Zb1!, LR21!, LR22!, Zb2!, Ra1!, UTsave!, Nada As Boolean, NGVlu%, IWGlu%, Irecord&
Dim iNums%, iYs%, iMs%, iDs%, UTs!, LR11s!, LR12s!, Zb1s!, LR21s!, LR22s!, Zb2s!, RAOB1s$, C$
Dim Tnr!, Tnr2!, Tnr3!, Tnr4!, Tna!, Trwt!, aTnr!, aTnr2!, aTnr3!, aTnr4!, aTna!, Tna_Trwt, Tnr_Trwt, Tnr2_Trwt, Tnr3_Trwt, aMach2!
Dim hms0$, DT0 As Date, P!, DTflight As Date
Dim out$, dZtrop!, dTtrop!, header$, OutCounter%
Static aTnavi!(1 To 500), aTraob!(1 To 500), aTres!(1 To 500), aRange!(1 To 500)
Dim Threshold!, Tthreshold!(1 To 10), Rthreshold!(1 To 10), nSamples%(1 To 10), aAlt!
Dim AVG!, RMS!, TPthreshold!(1 To 10), RecordsUsed&(1 To 500), Ztrop1!, InFile$, OUTfile$, start As Boolean
Static Zac!(1 To 500), Ztp!(1 To 500), iNum%, iDlast%, SameFlight As Boolean

Static GA!(1 To 300), PA!(1 To 300), RH!(1 To 300), Tt!(1 To 300), ZA!(1 To 300)
Static GAa!(1 To 300), PAa!(1 To 300), RHa!(1 To 300), TTa!(1 To 300), ZAa!(1 To 300)
Static GAb!(1 To 300), PAb!(1 To 300), RHb!(1 To 300), TTb!(1 To 300), ZAb!(1 To 300), zoffset!
Static aZp!, aTnav!, aTmtp!, aLat!, aLon!, aRoll!, aPitch!, aZt!, aTt!, Rcounter%, Tlayer!
Static Site1Lat!, Site1Lon!, Site1Distance!, Site2Lat!, Site2Lon!, Site2Distance!, j1%, j2%
Static Nrla%, Nrlb%, Nrl%, Isame%, OUTlu%, RAOB1$, RAOB2x$, AvgTdiff!, RmsTdiff!, LevelCount%(1 To 200)
Static Found1 As Boolean, Found2 As Boolean, Status As Boolean, z!, jt1%, jt2%, k%, UTsecLast&
Static Beforecycle&, AfterCycle&

Select Case Index
Case 0 'Import Event file
'  UTks   pALT Latitud Longitud
'4
' 66770 12.077  36.125 -117.061
' 68517 10.862  36.100 -116.894
' 77012 15.076  36.694 -117.725
' 78680 19.502  36.096 -117.296
  
  If Dir(Drive$ + Rdir$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".ELL", vbNormal) = "" Then
  ' Set Initial directory
    CommonDialog1.InitDir = Drive$ + Rdir$ + Mission$ + "\" + AC$ + YYYYMMDD$ + "\"
  ' Set Size of FileName buffer
    CommonDialog1.MaxFileSize = 1024
  ' Set filters.
    CommonDialog1.Filter = "ELL Files (*.ELL)|*.ELL|All Files (*.*)|*.*"
  ' Specify default filter.
    CommonDialog1.FilterIndex = 1
  ' Display the File Open dialog box.
    CommonDialog1.ShowOpen
    File$ = CommonDialog1.Filename
  Else
    File$ = Drive$ + Rdir$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".ELL"
  End If
  If File$ = "" Then Exit Sub
  
  lu = FreeFile
  Open File$ For Input As lu
  Input #lu, Nevents
  For i = 0 To Nevents - 1
    Line Input #lu, Line$
    lstEvent.List(i) = Left$(Line$, 13)
    EventList$(i + 1) = Line$
  Next i
  Close lu
    
Case 1 ' Time average pair of site 1 raobs

  Rec1 = Val(txtRec1(0).Text)
  Rec2 = Val(txtRec2(0).Text)
  Wt1 = Val(txtFraction(0).Text)
  Thickness = Val(txtLayerThickness.Text) / 1000# 'Convert to km
  If Left$(RAOB1$, 2) = "HK" Or Left$(RAOB1$, 4) = "MROC" Then
    Rec1 = Beforecycle
    Rec2 = AfterCycle
    Wt1 = 1
  End If
  
  Call BlendROABtime(RAOBlu, Rec1, Rec2, Wt1, GAa(), PAa(), TTa(), RHa(), Nrla, AvgTdiff!, RmsTdiff!)
  
  txtSavg(0).Text = Format(AvgTdiff, "00.00")
  txtSrms(0).Text = Format(RmsTdiff, "00.00")
  For i% = 1 To Nrla
    Zplot(i%) = fPtoZ(PAa(i%))
    Tplot(i%) = TTa(i%)
  Next i
  Tlayer = fCalculate_Layer_Avg_T(Zplot!(), Tplot!(), Nrla%, pALT, Thickness!)
  Nlevr = Nrla:  jt1% = 1: jt2% = Nlevr
  frmRAOB.LineThickness = 2
  frmRAOB.LineColor = White
  frmRAOB.RAOB_Plot_Curve Zplot!(), Tplot!(), Nlevr, White
  'optSite(1).value = True
  
Case 2 ' Time average pair of site 2 raobs
  Rec1 = Val(txtRec1(1).Text)
  Rec2 = Val(txtRec2(1).Text)
  Wt2 = Val(txtFraction(1).Text)
  
  Call BlendROABtime(RAOBlu, Rec1, Rec2, Wt2, GAb(), PAb(), TTb(), RHb(), Nrlb, AvgTdiff!, RmsTdiff!)
  
  txtSavg(1).Text = Format(AvgTdiff, "00.00")
  txtSrms(1).Text = Format(RmsTdiff, "00.00")
  For i% = 1 To Nrlb
    Zplot(i%) = fPtoZ(PAb(i%))
    Tplot(i%) = TTb(i%)
  Next i
  Nlevr = Nrlb:  jt1% = 1: jt2% = Nlevr
  frmRAOB.DrawWidth = 1
  frmRAOB.RAOB_Plot_Curve Zplot!(), Tplot!(), Nlevr, White

Case 3  'Do spatial interpolations
'Interpolate so both RAOBs have same levels
Nrl = 1: i = 1: j = 1: k = 1: ii = Nrla: jj = Nrlb: Isame = 0
Do
  Select Case GAa(i)
  Case Is < GAb(j)        'Add a level to GA2
    Pint = fPAinterp(GAb(), PAb(), j, GAa(i))
    Gint = GAa(i)      'Set geometric altitudes equal
    Tint = fXAinterp(GAb(), TTb(), j, GAa(i))
    Rint = fXAinterp(GAb(), RHb(), j, GAa(i))
    For M = jj To j Step -1
      PAb(M + 1) = PAb(M)
      GAb(M + 1) = GAb(M)
      TTb(M + 1) = TTb(M)
      RHb(M + 1) = RHb(M)
    Next M
    PAb(j) = Pint
    GAb(j) = Gint
    TTb(j) = Tint
    RHb(j) = Rint
    jj = jj + 1
    
  Case Is = GAb(j) 'Equal
    Isame = Isame + 1
    
  Case Is > GAb(j) 'Interpolate PA1
    Pint = fPAinterp(GAa(), PAa(), i, GAb(j))
    Gint = GAb(i)      'Set geometric altitudes equal
    Tint = fXAinterp(GAa(), TTa(), i, GAb(j))
    Rint = fXAinterp(GAa(), RHa(), i, GAb(j))
    For M = ii To i Step -1  'Make room for a new level
      PAa(M + 1) = PAa(M)
      GAa(M + 1) = GAa(M)
      TTa(M + 1) = TTa(M)
      RHa(M + 1) = RHa(M)
    Next M
    PAa(i) = Pint
    GAa(i) = Gint
    TTa(i) = Tint
    RHa(i) = Rint
    ii = ii + 1       'Add a GA1 level
    
  End Select
  j = j + 1
  i = i + 1
  Nrl = Nrl + 1
Loop Until i > ii Or j > jj
'
If j > jj Then  'extend series 2
  For M = j To ii
    PAb(M) = fPAinterp(GAb(), PAb(), jj, GAa(M))
    GAb(M) = GAa(M)      'Set geometric altitudes equal
    TTb(M) = fXAinterp(GAb(), TTb(), jj, GAa(M))
    RHb(M) = fXAinterp(GAb(), RHb(), jj, GAa(M))
  Next M
Else
  For M = i To jj
    PAa(M) = fPAinterp(GAa(), PAa(), ii, GAb(M))
    GAa(M) = GAb(j)   'Set geometric altitudes equal
    TTa(M) = fXAinterp(GAa(), TTa(), ii, GAb(M))
    RHa(M) = fXAinterp(GAa(), RHa(), ii, GAb(M))
  Next M
End If

' Now interpolate levels in time
  Nrl = Nrla + Nrlb - Isame
  WtA = Val(txtWeight(0).Text)
  For i = 1 To Nrl    'Write out as increasing pressure altitude [m]
    GA(i) = GAa(i) * WtA + GAb(i) * (1 - WtA)
    PA(i) = PAa(i) * WtA + PAb(i) * (1 - WtA)
    Tt(i) = TTa(i) * WtA + TTb(i) * (1 - WtA)
    RH(i) = RHa(i) * WtA + RHb(i) * (1 - WtA)
  Next i

'
  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA(i%))
    Tplot(i%) = Tt(i%)
  Next i
  OATraob = fTinterp(ALTkm, Zplot(), Tplot(), Nrl)
  Tmin = 999#
  If OAT11 < Tmin Then Tmin = OAT11
  If OAT12 < Tmin Then Tmin = OAT12
  If OAT21 < Tmin Then Tmin = OAT21
  If OAT22 < Tmin Then Tmin = OAT22
  Tmax = 0#
  If OAT11 > Tmax Then Tmax = OAT11
  If OAT12 > Tmax Then Tmax = OAT12
  If OAT21 > Tmax Then Tmax = OAT21
  If OAT22 > Tmax Then Tmax = OAT22
  OATraobPP = Tmax - Tmin
  txtOATraob.Text = Format(OATraob, "000.0")
  txtOATpp.Text = Format(OATraobPP, "#0.0")
  
  Nlevr = Nrl:  jt1% = 1: jt2% = Nlevr
  frmRAOB.DrawWidth = 2
  frmRAOB.RAOB_Plot_Curve Zplot!(), Tplot!(), Nlevr, Yellow
'
Case 4  'Write Interpolated RAOB
'  72387   FUJ   981120   98 NOV 20 12Z    36.62   -116.02
'
'     954.6      502   999999     -0.1
'     938.7      637   999999     -2.2
'     887.9     1100     1200     -3.3
  For i = 1 To Nrl    'Write out as increasing pressure altitude [m]
    ZGraob(i) = GA(i)
    PZraob(i) = PA(i)
    TZraob(i) = Tt(i)
    RZraob(i) = RH(i)
  Next i

  EventNumber = Val(txtEventNo.Text)
  OUTlu = FreeFile
  Open Path$ + AC$ + YYYYMMDD$ + ".LRF" For Random Access Read Write As OUTlu Len = Len(LRF)
  WMO4l = "MTP"
  WMOnumber = EventTime
  Iyear = IyearE
  Idoy = IdoyE
  Imonth = ImonthE
  Iday = IdayE
  Ihour = Int(EventTime / 3600#)
  Imin = Int((EventTime - 3600# * Ihour) / 60#)
  Isec = EventTime - 3600# * Ihour - 60# * Imin
  Nlevr = Nrl
  Call RAOBwriteL(OUTlu, EventNumber)
  Close OUTlu
  
Case 5  'Calculate Weights for spatial interpolation
  LT1 = Val(txtLATn(0).Text)
  Ln1 = Val(txtLONn(0).Text)
  Lt2 = Val(txtLATn(1).Text)
  Ln2 = Val(txtLONn(1).Text)
  Lt3 = Val(txtLATn(2).Text)
  Ln3 = Val(txtLONn(2).Text)
  
  Distance = Sqr((LT1 - Lt2) ^ 2 + (Cos(cPI * (LT1 + Lt2) / 360) * (Ln1 - Ln2)) ^ 2)
  D1 = Sqr((LT1 - Lt3) ^ 2 + (Cos(cPI * (LT1 + Lt2) / 360) * (Ln1 - Ln3)) ^ 2)
  D2 = Distance - D1
  If Distance <> 0 Then
    txtWeight(0).Text = Val(D1 / Distance)
    txtWeight(1).Text = Val(D2 / Distance)
  Else
    txtWeight(0).Text = 1#
    txtWeight(1).Text = 0#
  End If

  txtDistance(0).Text = Format(D1 * 60 * 1.85, "000")
  txtDistance(1).Text = Format(D2 * 60 * 1.85, "000")
  txtDistance(2).Text = Format(Distance * 60 * 1.85, "000")
  
Case 6

  For i = 1 To Nrla
    ZAa(i) = fPtoZ(PAa(i))
  Next i
  Traob = fLinterp(ZAa(), TTa(), Nrla, aZp)
  A$ = Format(iNum, "##0") + vbTab + RAOB1$ + vbTab + YYYYMMDD$ + vbTab + Format(Val(txtATPgo.Text), "00.000") + vbTab + Format(aZp, "00.000")
  A$ = A$ + vbTab + Format(Site1Distance, "##0.0")
  A$ = A$ + vbTab + txtSavg(0).Text + vbTab + txtSrms(0).Text + vbTab + Format(Val(txtFraction(0).Text), "0.000")
  A$ = A$ + vbTab + Format(aZt, "00.000") + vbTab + Format(aTnav, "000.00") + vbTab + Format(aTmtp, "000.00")
  A$ = A$ + vbTab + Format(Traob, "000.00") + vbTab + Format(Tlayer, "000.00") + vbTab + Format(aTt, "000.0")
  A$ = A$ + vbTab + Format(aTnav - Traob, "##0.00") + vbTab + Format(aTmtp - Traob, "##0.00")
  A$ = A$ + vbTab + Format(aTnav - Tlayer, "##0.00") + vbTab + Format(aTmtp - Tlayer, "##0.00")
  'Debug.Print A$,
  
'  Select Case AC$
'  Case "ER", "M5", "WB": j1 = 16: j2 = 64   ' 8-32 km
'  Case "DC": j1 = 1: j2 = 50                ' 0.5-25 km
'  End Select
' This code does not take into account different flight levels
'  For j = j1 To j2
'    z = 0.5 * j
'    If z > ZAa(Nrla) Then Exit For
'    LevelCount(j) = LevelCount(j) + 1
'    Traob = fLinterp(ZAa(), TTa(), Nrla, z)
'    Tmtp = fLinterp(RAp(), RTp(), Nlev, z)
'    Print #OUTlu, Format(Tmtp - Traob, "00.00"),
'  Next j
' Calculate Tmtp - Traob +/- 10 km wrt flight level using interpolated RAOB (ZAa, TTa)
' And average temperature profile (RAp, RTp) for N scans
  For j = -20 To 20
    z = aZp + 0.5 * j
    If z > ZAa(Nrla) Then Exit For    'exit if above top of RAOB
    If z > 2 Then   'RAp(1) Then  'xxx mjm RAp sometimes <0
      LevelCount(j + 21) = LevelCount(j + 21) + 1
 ' Average over 1 km thick layer. Better to use raw sounding
      zoffset = z - aZp
'      Traob = (fLinterp(ZAa(), TTa(), Nrla, aZp + zoffset * 0.8) + fLinterp(ZAa(), TTa(), Nrla, aZp + zoffset * 1.3)) / 2
      Traob = fLinterp(ZAa(), TTa(), Nrla, z)
'      Traob = (fLinterp(ZAa(), TTa(), Nrla, z - 0.5) + fLinterp(ZAa(), TTa(), Nrla, z + 0.5)) / 2
      Tmtp = fLinterp(RAp(), RTp(), Nlev, z)  'RAp and RTp are average of n profiles
      A$ = A$ + vbTab + Format(Tmtp - Traob, "00.00")
    Else
      A$ = A$ + vbTab
    End If
 Next j
  jmax = j - 1
  Print #OUTlu, A$
  Debug.Print A$
  Debug.Print " "

  DoEvents

Case 7      'Import RAOBs - Do comparison (compares full profile cf. Case 8 which flight level only)
' Open following files that are all located in the /Mission/RAOB/ folder
' Open RAOB2 File with time-sorte RAOBs:     Mission_RAOBs.RAOB2
' Open output RAOB2 file with those used:    Mission_RAOBused.RAOB2
' Open file with list of closest approaches: Mission_RAOBrangeUsed.txt (from MTPbin/Tools)
' Open output file with RAOB comparisons:    RAOBcomparison.txt
  
  Prefix$ = txtPrefix.Text
  PrefixR$ = Prefix$ + "RAOB\"
  RAOBtype = 0
  Rcounter = 0
  cmdBlend(7).Caption = "Busy!"
  
  RAOBlu% = FreeFile
  Filename$ = PrefixR$ + Mission$ + "_RAOBs.RAOB2"
  Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
  
  RAOBlu2 = FreeFile
  FileName2$ = PrefixR$ + Mission$ + "_RAOBused.RAOB2"   'Write out RAOBs actually used for use elsewhere
  Open FileName2$ For Random Access Read Write As RAOBlu2% Len = Len(RAOB2)
  FileRecords = LOF(RAOBlu%) \ Len(RAOB2)
  txtFileRec = Str$(FileRecords)
  DoEvents
  optMode(1).Value = True
  Cycle = 0
  cmdOpen.Caption = "&Close RAOB"
  txtASCIIout.Text = fReplaceExtension(Filename$, "txt")
' And fill Search List
  Call cmdSelect_Click(4)  'Clear list box
  Call cmdSelect_Click(0)  'And refill it
'N Day Month Year  RAOBs UT0 UT1 UT2
'1 9 1 2003  BGDH  30.2  0 12
'2 9 1 2003  BGTL  25.0  0 12
'3 12  1 2003  ENJA  32.9  0 12
'12345678901234567890
  Navg = Val(txtNavg.Text)
  AC$ = Mid$(Prefix$, 13, 2)
  OUTlu = FreeFile
  Open PrefixR$ + "RAOBcomparison.txt" For Output As OUTlu
  
'  lu2 = FreeFile
'  Open PrefixR$ + Mission$ + "_RAOBrangeUsed.txt" For Output As lu2
  
  lu = FreeFile
  Open PrefixR$ + Mission$ + "_RAOBrangeUsed.txt" For Input As lu
  
  Input #lu, A$
  chkMultipleScans.Value = 1
  OutCount = 0
  Do
GetNext:
    Rcounter = Rcounter + 1
    If EOF(lu) Then Exit Do
    Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
    'If iNum = 26 Then
    Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; A$
    'End If
    DT0 = DateSerial(iY, iM, iD)
    If iNum = 26 Then
    X = X
    End If
    UTsave = UT0
    hms0$ = fSecToTstring(Int(UT0 * 1000), False)
    DT0 = DT0 + TimeSerial(Left$(hms0$, 2), Mid$(hms0$, 3, 2), Right$(hms0$, 2))
        
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    ii = InStr(i + 1, A$, vbTab)
    RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
    i = ii
    ii = InStr(i + 1, A$, vbTab)
    Ra1 = Val(Mid$(A$, i + 1, ii - i - 1))  'Get Range to first site
    
    If Ra1 < Val(Trim(txtMaxRange.Text)) Then  'Check that we are within minimum acceptable range
      YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      Filename$ = Prefix$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$
      If chkRFF.Value = 1 Then Filename$ = Filename$ + ".RFF" Else Filename$ = Filename$ + ".REF"
      PNGname$ = Prefix$ + "PNG\R" + Format(iNum, "000") + "_" + YYYYMMDD$ + "_" + RAOB1$ + ".PNG"
      Call RollOver(iY, iM, iD, UT0)
      DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring(UT0 * 1000, True))
      txtEvent.Text = DTevent
      Documentation$ = Format(iNum, "000") + " " + Format(UT0, "000.000") + "_ks " + Format(Ra1, "000") + "_km "
      
      If RAOB2x$ = RAOB1$ Then 'Only one site involved (no spatial interpolation)
'        If UT0 <= 43.2 Then UT1 = 0: UT2 = 12 Else UT1 = 12: UT2 = 24
        Call OpenREFfile(Filename$)
        txtATPgo.Text = Str(UTsave)
        cmdATPgo_Click
        Call AverageNcycles(Navg, aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, Nada)
        Close REFlu
        If Nada Then GoTo GetNext
        
        With frmRAOB
        .ALTkm = aZp
        .Tac = aTnav
        .LineColor = Yellow
        .LineThickness = 1
        End With
        
'       Now get the before and after radiosondes
        Call GetRAOBs(DT0, RAOB1$, Beforecycle&, AfterCycle&)
        If AfterCycle = 0 Or ZPraob(Nlevr) < aZp Then GoTo GetNext
        
        Call RAOBread(RAOBlu, Beforecycle)
        OutCount = OutCount + 1
        Call RAOBwrite(RAOBlu2, OutCount)
        DT1 = DandT
        nTZ1 = Nlevr
        OAT11 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        
        Site1Lat = WMOlatitude
        Site1Lon = WMOlongitude
        Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)
        
        Call CopyArray(ZPraob(), Zp1(), Nlevr)
        Call CopyArray(TZraob(), TZ1(), Nlevr)
        
        Call RAOBread(RAOBlu, AfterCycle)
        OutCount = OutCount + 1
        Call RAOBwrite(RAOBlu2, OutCount)
        DT2 = DandT
        OAT12 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)

        Call cmdBlend_Click(1)   'Temporal interpolation
        Call cmdBlend_Click(6)   'Write results
        Call cmdATP_Click(0)     'Transfer ATP (in arrays aRA() and aRT() from AverageNcycles)
        Call cmdSelect_Click(16) 'Write Documentation on image
        OATmtp = aTnav           'Or aTmtp
        pALT = aZp
        cmdToggleAC_Click        'Draw aircraft altitude pALT and OATmtp on frmRAOB
        DoEvents
        If chkWritePNGs.Value = 1 Then
          frmRAOB.ScaleMode = 1
          CaptureFormMJ frmRAOB, PNGname$, 1, False
          'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        End If
        If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
        Call cmdSelect_Click(5)  'Clear plot
      
      Else  'Need to do spatial interpolation
    
      End If
    
    End If
  Loop Until EOF(lu)
  Close lu
  
  Select Case AC$
  Case "ER", "M5", "WB", "GH": j1 = 16: j2 = 64        ' 8-32 km
  Case "DC", "NG": j1 = 1: j2 = 50                     ' 0.5-25 km
  End Select

  Print #OUTlu, ""
  For j = 1 To 19: Print #OUTlu, "x" + vbTab,: Next j
  For j = j1 To j2: Print #OUTlu, Format(LevelCount(j), "##0") + vbTab,: Next j
  Close OUTlu, lu, RAOBlu2
  cmdBlend(7).Caption = "ATP Tcal"

  
Case 8  ' NGV T cal (flight level comparison only)
' Open following files that are all located in the /Mission/RAOB/ folder
' Open RAOB2 File with time-sorted RAOBs:    Mission_RAOBs.RAOB2
' Open output RAOB2 file with those used:    Mission_RAOBused.RAOB2
' Open file with list of closest approaches: Mission_RAOBrangeUsed.txt (from MTPbin/Tools)
' Open output file with RAOB comparisons:    RAOBcomparison.txt

  cmdBlend(8).Caption = "BUSY!"
  Prefix$ = txtPrefix.Text
  PrefixR$ = Prefix$ + "RAOB\"
  RAOBtype = 0
  Rcounter = 0
  
  RAOBlu% = FreeFile
  Filename$ = PrefixR$ + Mission$ + "_RAOBs.RAOB2"
  Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
  txtRAOBfile.Text = Filename$
  
  FileRecords = LOF(RAOBlu%) \ Len(RAOB2)
  txtFileRec = Str$(FileRecords)
  DoEvents
  optMode(1).Value = True
  Cycle = 0
  cmdOpen.Caption = "&Close RAOB"
  txtASCIIout.Text = fReplaceExtension(Filename$, "txt")
' And fill Search List
  Call cmdSelect_Click(4)  'Clear list box
  Call cmdSelect_Click(0)  'And refill it
  Navg = Val(txtNavg.Text)
  AC$ = Mid$(Prefix$, 13, 2)
  OUTlu = FreeFile
  Open PrefixR$ + "RAOBcomparison.txt" For Output As OUTlu
  Select Case Mission$
  Case "HIPPO"
    header$ = "Num" + vbTab + "RAOB" + vbTab + "YMD" + vbTab + "UT" + vbTab + "Range" + vbTab + "Zt1" + vbTab + "Tt1" + vbTab
    header$ = header$ + "Zt2" + vbTab + "Tt2" + vbTab + "Zt1-Zt2" + vbTab + "Tt1-Tt2" + vbTab + "Zp" + vbTab + "ATFR" + vbTab + "ATHR1" + vbTab + "ATHR2" + vbTab + "AT_A" + vbTab
    header$ = header$ + "AT_A-ATFR" + vbTab + "ATHR1" + vbTab + "ATHR2" + vbTab + "ATHR2-ATHR1" + vbTab + "Tra" + vbTab + "AT_A-Tra" + vbTab + "ATFR-Tra" + vbTab + "ATHR1-Tra" + vbTab + "ATHR2-Tra" + vbTab + "XMACH2"
  Case Else     '"HIPPO-2", "HIPPO-3", "PREDICT"
    header$ = "Num" + vbTab + "RAOB" + vbTab + "YMD" + vbTab + "UT" + vbTab + "Zp" + vbTab + "Range" + vbTab + "Zt1" + vbTab + "Tt1" + vbTab
    header$ = header$ + "Zt2" + vbTab + "Tt2" + vbTab + "Zt1-Zt2" + vbTab + "Tt1-Tt2" + vbTab + "ATHL1" + vbTab + "ATHL2" + vbTab + "ATHR1" + vbTab + "ATHR2" + vbTab + "AT_A" + vbTab
    header$ = header$ + "Trab" + vbTab + "Traa" + vbTab + "Traa-Trab" + vbTab + "Trawt" + vbTab + "AT_A-Tra" + vbTab + "ATHL1-Tra" + vbTab + "ATHL2-Tra" + vbTab + "ATHR1-Tra" + vbTab + "XMACH2"
  End Select
  Print #OUTlu, header$

  lu = FreeFile
  Open PrefixR$ + Mission$ + "_RAOBrangeUsed.txt" For Input As lu
  
  Input #lu, A$
  chkMultipleScans.Value = 1
  OutCount = 0
  OutCounter = 0
  Do
GetNextNGV:
    Rcounter = Rcounter + 1
    If EOF(lu) Then Exit Do
    Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
    Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; A$
    DT0 = DateSerial(iY, iM, iD)
    UTsave = UT0
    hms0$ = fSecToTstring(Int(UT0 * 1000), False)
    DT0 = DT0 + TimeSerial(Left$(hms0$, 2), Mid$(hms0$, 3, 2), Right$(hms0$, 2))
    
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    ii = InStr(i + 1, A$, vbTab)
    RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
    i = ii
    ii = InStr(i + 1, A$, vbTab)
    Ra1 = Val(Mid$(A$, i + 1, ii - i - 1))  'Get Range to first site
    cmdSelect_Click (5)                     'clear RAOB display
    
    ' Check that we are within minimum acceptable range
    If Ra1 < Val(Trim(txtMaxRange.Text)) Then
      YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      Filename$ = Prefix$ + "NG\NG" + YYYYMMDD$ + ".asc"
      PNGname$ = Prefix$ + "PNG\R" + Format(iNum, "000") + "_" + YYYYMMDD$ + RAOB1$ + ".PNG"
      Call RollOver(iY, iM, iD, UT0)
      DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring(UT0 * 1000, True))
      txtEvent.Text = DTevent
      Documentation$ = Format(iNum, "000") + " " + Format(UT0, "000.000") + "_ks " + Format(Ra1, "000") + "_km "
      
'      If RAOB2x$ = RAOB1$ And Not (RAOB1$ = "INCA") Then 'Only one site involved
      
      If RAOB2x$ = RAOB1$ Then 'Only one site involved
        NGVlu = FreeFile
        Open Filename$ For Input As NGVlu
        Line Input #NGVlu, A$   'skip header line
        ' Go to first record in aircraft data that is beyond time of closest approach
        Do
          Call GetNGline(NGVlu, UTsec&, P, Tna, Tnr, Tnr2, Tnr3, Tnr4!, Latitude!, Longitude!, gALT!, pALT!, Pitch!, Roll!, aMach2)
        Loop Until UTsec / 1000# > UT0 Or EOF(NGVlu) = True
        If EOF(NGVlu) Then Exit Do
        
        ' Get averaged OAT from aircraft data
        Call AverageNcyclesNGV(NGVlu, UTsecLast&, Navg, aZp!, aTna!, aTnr!, aTnr2!, aTnr3!, aTnr4!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2, Nada)
        Close NGVlu
         If Nada Then GoTo GetNextNGV
        
        With frmRAOB
        .ALTkm = aZp
        .Tac = aTna     'xxxmjm 20090122
        .LineThickness = 1
        End With
        
'       Now get the before and after radiosondes
        
        Call GetRAOBs(DT0, RAOB1$, Beforecycle&, AfterCycle&)
        If Left$(RAOB1$, 2) = "HK" Or Left$(RAOB1$, 4) = "MROC" Then
          If AfterCycle = 0 Then
          AfterCycle = Beforecycle
          Else
          Beforecycle = AfterCycle
          End If
        End If
        
        If AfterCycle = 0 Or ZPraob(Nlevr) < aZp Then  'Make sure AfterCycle and after RAOB above aZp
          GoTo GetNextNGV
        End If
        OutCounter = OutCounter + 1
        Call UniqueRecords(OutCount&, RecordsUsed&(), Beforecycle)
        Call UniqueRecords(OutCount&, RecordsUsed&(), AfterCycle)
        
'       Get Before RAOB
        Call RAOBread(RAOBlu, Beforecycle)
        If ZPraob(Nlevr) < aZp Then GoTo GetNextNGV    'Make sure before RAOB above aZp
        DT1 = DandT
        nTZ1 = Nlevr
        OAT11 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        
        Site1Lat = WMOlatitude
        Site1Lon = WMOlongitude
        Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)

        out$ = Format(iNum, "000") + vbTab + RAOB1$ + vbTab + YYYYMMDD$ + vbTab + Format(UT0, "##0.000") + vbTab
        out$ = out$ + Format(aZp, "#0.00") + vbTab + Format(Site1Distance, "##0.0") + vbTab
        Ztrop1 = Ztrop
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        dZtrop = Ztrop
        dTtrop = Ttrop
        Call CopyArray(ZPraob(), Zp1(), Nlevr)
        Call CopyArray(TZraob(), TZ1(), Nlevr)

'       Get After RAOB
        Call RAOBread(RAOBlu, AfterCycle)
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        If dZtrop > 0 And Ztrop > 0 Then
          dZtrop = dZtrop - Ztrop
          dTtrop = dTtrop - Ttrop
          out$ = out$ + Format(dZtrop, "#00.00") + vbTab + Format(dTtrop, "#0.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        DT2 = DandT
        If Beforecycle = AfterCycle Then DT2 = DandT + "03:00:00"
        OAT12 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        
        Call cmdBlend_Click(1)   'Temporal interpolation
        Call cmdSelect_Click(16) 'Write Documentation on image
        
        Trwt = OAT11 + ((OAT12 - OAT11) / (DT2 - DT1)) * (DT0 - DT1) 'Temperature RAOB time weighted
        Tna_Trwt = aTna - Trwt
        Tnr_Trwt = aTnr - Trwt
        Tnr2_Trwt = aTnr2 - Trwt
        Tnr3_Trwt = aTnr3 - Trwt
'        Debug.Print aTnr; aTna; aTna - aTnr; OAT11; OAT12; OAT12 - OAT11; Trwt; Tna_Trwt; Tnr_Trwt
' Tre  Tav  Tav_Tre  Tr1  Tr2  Tr2_Tr1  Trwt  aTna_Trwt  aTnr_Trwt
        'HIPPO-3 aTnr=ATHL1, aTnr2=ATHL2, aTnr3=ATHR1, aTna=AT_A
        out$ = out$ + Format(aTnr, "000.00") + vbTab
        out$ = out$ + Format(aTnr2, "000.00") + vbTab + Format(aTnr3, "000.00") + vbTab
        out$ = out$ + Format(aTnr4, "000.00") + vbTab + Format(aTna, "000.00") + vbTab
        out$ = out$ + Format(OAT11, "000.00") + vbTab + Format(OAT12, "000.00") + vbTab
        out$ = out$ + Format(OAT12 - OAT11, "#0.0000") + vbTab + Format(Trwt, "000.0000") + vbTab
        out$ = out$ + Format(Tna_Trwt, "##0.0000") + vbTab + Format(Tnr_Trwt, "##0.0000") + vbTab
        out$ = out$ + Format(Tnr2_Trwt, "##0.0000") + vbTab + Format(Tnr3_Trwt, "##0.0000") + vbTab + Format(Mach2, "##0.0000")
        Print #OUTlu, out$
        aTnavi(OutCounter) = aTna
        aTres(OutCounter) = aTnr
        aTraob(OutCounter) = Trwt
        aRange(OutCounter) = Site1Distance
        Zac(OutCounter) = aZp
        Ztp(OutCounter) = Ztrop + dZtrop / 2#
        
        OATmtp = aTna      'Use the avionics T on the comparison plot for a/c T at pALT
        pALT = aZp
        cmdToggleAC_Click  'Plots AC pALT at OATmtp
        DoEvents
        If chkWritePNGs.Value = 1 Then
          ' Create PNG folder under Mission$ if necessary on first pass
          If Rcounter = 1 Then CreateDirectoryTree fGetPath(PNGname$), False
          frmRAOB.ScaleMode = 1
          CaptureFormMJ frmRAOB, PNGname$, 1, False
          'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        End If
        If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
        Call cmdSelect_Click(5)  'Clear plot
      
      Else  'spatial interpolation needs to be programmed
      
      End If
    End If
  
  Loop Until EOF(lu) ' End of Mission$_RAOBs.RAOB2 file
  Close lu

' Write out all the unique RAOBs used to Mission$_RAOBused.RAOB2
  RAOBlu2 = FreeFile
  FileName2$ = PrefixR$ + Mission$ + "_RAOBused.RAOB2"   'Write out RAOBs actually used for use elsewhere
  If Dir$(FileName2$) <> "" Then Kill FileName2$
  Open FileName2$ For Random Access Read Write As RAOBlu2% Len = Len(RAOB2)
  Call BubbleSortL(RecordsUsed(), OutCount)
  For iL = 1 To OutCount
    Call RAOBread(RAOBlu, RecordsUsed(iL))
    Call RAOBwrite(RAOBlu2, iL)
  Next iL
  Close RAOBlu, RAOBlu2  'Leave OUTlu open
  
' Now do Range and Temperature Threshold statistics
' Write T threshold data for avionics
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for avionics"
  For i = 1 To 8
    Call AvgRMSdiffT(aTnavi(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write T threshold data for research
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for research"
  For i = 1 To 8
    Call AvgRMSdiffT(aTres(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Range threshold data for avionics
  Rthreshold(1) = 300#:  Rthreshold(2) = 250#:  Rthreshold(3) = 200#:  Rthreshold(4) = 150#
  Rthreshold(5) = 100#:   Rthreshold(6) = 75#:   Rthreshold(7) = 50#:  Rthreshold(8) = 25#
  Rthreshold(9) = 10#:   Rthreshold(10) = 5#
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for avionics"
  For i = 1 To 10
    Call AvgRMSdiffR(aTnavi(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i

' Write Range threshold data for research
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for research"
  For i = 1 To 10
    Call AvgRMSdiffR(aTres(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Tropopause offset threshold data
  TPthreshold(1) = 5#:  TPthreshold(2) = 4#:  TPthreshold(3) = 3#:  TPthreshold(4) = 2.5
  TPthreshold(5) = 2#:   TPthreshold(6) = 1.5:   TPthreshold(7) = 1#:  TPthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Tropopause offset threshold statistics"
  For i = 1 To 8
    Call AvgRMSdiffTP(Zac(), Ztp(), OutCounter, TPthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out
    End If
  Next i
  Close OUTlu
  cmdBlend(8).Caption = "NGV Tcal"

  
Case 9   'Filter output
'Pause and Continue
'  If cmdBlend(9).Caption = "Pause" Then
'    cmdBlend(9).Caption = "Continue"
'    GoSleep Val(txtWait.Text)
'    cmdBlend(9).Caption = "Pause"
'  End If

  InFile = MNpath$ + "RAOB\TREX_RAOBrangeAll.txt"
  INPlu = FreeFile
  Open InFile For Input As INPlu

  OUTlu = FreeFile
  OUTfile = MNpath$ + "RAOB\TREX_RAOBrangeUsedAVIchk.txt"
  Open OUTfile For Output As OUTlu
  Input #INPlu, A$
  Print #OUTlu, A$

  Do
    Input #INPlu, A$
    n = Left$(A$, 3)
    aAlt = Right$(A$, 5)
'    If aAlt >= 11.5 And aAlt <= 13.5 Then
'    If InStr(A$, "INCA") > 0 Then
      If Len(Dir(MNpath$ + "PNG\R" + Format(n, "000") + "*.PNG")) > 0 Then
        Print #OUTlu, A$
      End If
'    End If
'    End If
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu

Case 10  ' COSMIC T cal (flight level comparison only)
' Open following files that are all located in the /Mission/RAOB/ folder
' Open RAOB2 File with time-sorted RAOBs:    Mission_COSMICs.RAOB2
' Open output RAOB2 file with those used:    Mission_COSMICused.RAOB2
' Open file with list of closest approaches: Mission_COSMICrangeUsed.txt (from MTPbin/Tools)
' Open output file with COSMIC comparisons:  COSMICcomparison.txt

  Prefix$ = txtPrefix.Text
  PrefixR$ = Prefix$ + "RAOB\"
  RAOBtype = 3
  Rcounter = 0
  
  RAOBlu% = FreeFile
  Filename$ = PrefixR$ + Mission$ + "_COSMICs.RAOB3"
  Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB3)
  txtRAOBfile.Text = Filename$
  
  FileRecords = LOF(RAOBlu%) \ Len(RAOB3)
  txtFileRec = Str$(FileRecords)
  DoEvents
  optMode(1).Value = True
  Cycle = 0
  cmdOpen.Caption = "&Close RAOB"
  txtASCIIout.Text = fReplaceExtension(Filename$, "txt")
' And fill Search List
  Call cmdSelect_Click(4)  'Clear list box
  Call cmdSelect_Click(0)  'And refill it
  Navg = Val(txtNavg.Text)
  AC$ = Mid$(Prefix$, 13, 2)
  OUTlu = FreeFile
  Open PrefixR$ + "COSMICcomparison.txt" For Output As OUTlu
  header$ = "Num" + vbTab + "RAOB" + vbTab + "YMD" + vbTab + "UT" + vbTab + "Range" + vbTab + "Zt1" + vbTab + "Tt1" + vbTab
  header$ = header$ + "Zt2" + vbTab + "Tt2" + vbTab + "Zt1-Zt2" + vbTab + "Tt1-Tt2" + vbTab + "Zp" + vbTab + "Tre" + vbTab + "Tre2" + vbTab + "Tre3" + vbTab + "Tav" + vbTab
  header$ = header$ + "Tav_Tre" + vbTab + "Tr1" + vbTab + "Tr2" + vbTab + "Tr2_Tr1" + vbTab + "Tra" + vbTab + "Tav_Tra" + vbTab + "Tre_Tra"
  Print #OUTlu, header$

  lu = FreeFile
  Open PrefixR$ + Mission$ + "_COSMICrangeUsed.txt" For Input As lu
  
  Input #lu, A$
  chkMultipleScans.Value = 1
  OutCount = 0
  OutCounter = 0
  Do
GetNextCOSMIC:
    Rcounter = Rcounter + 1
    If EOF(lu) Then Exit Do
    Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
'   Debug.Print iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; a$
    DT0 = DateSerial(iY, iM, iD)
    UTsave = UT0
    hms0$ = fSecToTstring(Int(UT0 * 1000), False)
    DT0 = DT0 + TimeSerial(Left$(hms0$, 2), Mid$(hms0$, 3, 2), Right$(hms0$, 2))
    
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    ii = InStr(i + 1, A$, vbTab)
    RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
    i = ii
    ii = InStr(i + 1, A$, vbTab)
    Ra1 = Val(Mid$(A$, i + 1, ii - i - 1))  'Get Range to first site
    cmdSelect_Click (5)                     'clear RAOB display
    
    ' Check that we are within minimum acceptable range
    If Ra1 < Val(Trim(txtMaxRange.Text)) Then
      YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      Filename$ = Prefix$ + "NG\NG" + YYYYMMDD$ + ".asc"
      PNGname$ = Prefix$ + "PNG\R" + Format(iNum, "000") + "_" + YYYYMMDD$ + RAOB1$ + ".PNG"
      Call RollOver(iY, iM, iD, UT0)
      DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring(UT0 * 1000, True))
      txtEvent.Text = DTevent
      Documentation$ = Format(iNum, "000") + " " + Format(UT0, "000.00") + "_ks " + Format(Ra1, "000") + "_km "
      
'      If RAOB2x$ = RAOB1$ And Not (RAOB1$ = "INCA") Then 'Only one site involved
      
      If RAOB2x$ = RAOB1$ Then 'Only one site involved
        start = True
        NGVlu = FreeFile
        Open Filename$ For Input As NGVlu
        Line Input #NGVlu, A$   'skip header line
        ' Go to first record in aircraft data that is beyond time of closest approach
        Do
          Call GetNGline(NGVlu, UTsec&, P, Tna, Tnr, Tnr2, Tnr3, Tnr4, Latitude!, Longitude!, gALT!, pALT!, Pitch!, Roll!, aMach2!)
          If start Then
            start = False
          Else
            If UTsec < UTsecLast Then UTsec = UTsec + 86400#
          End If
          UTsecLast = UTsec
        Loop Until UTsec / 1000# > UTsave Or EOF(NGVlu) = True
        If EOF(NGVlu) Then Exit Do
        
        ' Get averaged OAT from aircraft data
        Call AverageNcyclesNGV(NGVlu, UTsecLast, Navg, aZp!, aTna!, aTnr!, aTnr2!, aTnr3!, aTnr4, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2!, Nada)
        Close NGVlu
        If Nada Then GoTo GetNextCOSMIC
        
        With frmRAOB
        .ALTkm = aZp
        .Tac = aTnr3     'xxxmjm 20090122
        .LineThickness = 1
        End With
        
'       Get COSMIC sounding
        OutCounter = OutCounter + 1
        Beforecycle = Right$(RAOB1$, 4)
        Call RAOBread(RAOBlu, Beforecycle)
        If ZPraob(Nlevr) < aZp Then GoTo GetNextCOSMIC    'Make sure before RAOB above aZp
        DT1 = DandT
        nTZ1 = Nlevr
        OAT11 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        OAT12 = OAT11
        
        Site1Lat = WMOlatitude
        Site1Lon = WMOlongitude
        Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)
        If Site1Distance > Ra1 + 10 Then
          i = i
        End If

        out$ = Format(iNum, "000") + vbTab + RAOB1$ + vbTab + YYYYMMDD$ + vbTab + Format(UTsave, "##0.00") + vbTab
        out$ = out$ + Format(Site1Distance, "##0.0") + vbTab
        Ztrop1 = Ztrop
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        dZtrop = Ztrop
        dTtrop = Ttrop
        Call CopyArray(ZPraob(), Zp1(), Nlevr)
        Call CopyArray(TZraob(), TZ1(), Nlevr)
        
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        If dZtrop > 0 And Ztrop > 0 Then
          dZtrop = dZtrop - Ztrop
          dTtrop = dTtrop - Ttrop
          out$ = out$ + Format(dZtrop, "#00.00") + vbTab + Format(dTtrop, "#0.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        DT2 = DandT
        
        
        
        Call cmdSelect_Click(16) 'Write Documentation on image
        
        Trwt = OAT11  'Temperature RAOB time weighted
        Tna_Trwt = aTna - Trwt
        Tnr_Trwt = aTnr - Trwt
        Debug.Print aTnr; aTna; aTna - aTnr; OAT11; OAT12; OAT12 - OAT11; Trwt; Tna_Trwt; Tnr_Trwt
' Tre  Tav  Tav_Tre  Tr1  Tr2  Tr2_Tr1  Trwt  aTna_Trwt  aTnr_Trwt
        out$ = out$ + Format(aZp, "#0.00") + vbTab + Format(aTnr, "000.00") + vbTab
        out$ = out$ + Format(aTnr2, "000.00") + vbTab + Format(aTnr3, "000.00") + vbTab
        out$ = out$ + Format(aTna, "000.00") + vbTab + Format(aTna - aTnr, "##0.00") + vbTab
        out$ = out$ + Format(OAT11, "000.00") + vbTab + Format(OAT12, "000.00") + vbTab
        out$ = out$ + Format(OAT12 - OAT11, "#0.0000") + vbTab + Format(Trwt, "000.0000") + vbTab
        out$ = out$ + Format(Tna_Trwt, "##0.0000") + vbTab + Format(Tnr_Trwt, "##0.0000")
        Print #OUTlu, out$
        aTnavi(OutCounter) = aTna
        aTres(OutCounter) = aTnr
        aTraob(OutCounter) = Trwt
        aRange(OutCounter) = Site1Distance
        Zac(OutCounter) = aZp
        Ztp(OutCounter) = Ztrop + dZtrop / 2#
        
        OATmtp = aTna      'Use the avionics T on the comparison plot for a/c T at pALT
        pALT = aZp
        cmdToggleAC_Click  'Plots AC pALT at OATmtp
        DoEvents
        If chkWritePNGs.Value = 1 Then
          ' Create PNG folder under Mission$ if necessary on first pass
          If Rcounter = 1 Then CreateDirectoryTree fGetPath(PNGname$), False
          frmRAOB.ScaleMode = 1
          CaptureFormMJ frmRAOB, PNGname$, 1, False
          'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        End If
        If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
        Call cmdSelect_Click(5)  'Clear plot
      
      Else  'spatial interpolation needs to be programmed
      
      End If
    End If
  
  Loop Until EOF(lu) ' End of Mission$_RAOBs.RAOB2 file
  Close lu

' Write out all the unique RAOBs used to Mission$_RAOBused.RAOB2
  RAOBlu2 = FreeFile
  FileName2$ = PrefixR$ + Mission$ + "_RAOBused.RAOB2"   'Write out RAOBs actually used for use elsewhere
  If Dir$(FileName2$) <> "" Then Kill FileName2$
  Open FileName2$ For Random Access Read Write As RAOBlu2% Len = Len(RAOB2)
  Call BubbleSortL(RecordsUsed(), OutCount)
  For iL = 1 To OutCount
    Call RAOBread(RAOBlu, RecordsUsed(iL))
    Call RAOBwrite(RAOBlu2, iL)
  Next iL
  Close RAOBlu, RAOBlu2  'Leave OUTlu open
  
' Now do Range and Temperature Threshold statistics
' Write T threshold data for avionics
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for avionics"
  For i = 1 To 8
    Call AvgRMSdiffT(aTnavi(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write T threshold data for research
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for research"
  For i = 1 To 8
    Call AvgRMSdiffT(aTres(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Range threshold data for avionics
  Rthreshold(1) = 300#:  Rthreshold(2) = 250#:  Rthreshold(3) = 200#:  Rthreshold(4) = 150#
  Rthreshold(5) = 100#:   Rthreshold(6) = 75#:   Rthreshold(7) = 50#:  Rthreshold(8) = 25#
  Rthreshold(9) = 10#:   Rthreshold(10) = 5#
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for avionics"
  For i = 1 To 10
    Call AvgRMSdiffR(aTnavi(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i

' Write Range threshold data for research
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for research"
  For i = 1 To 10
    Call AvgRMSdiffR(aTres(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Tropopause offset threshold data
  TPthreshold(1) = 5#:  TPthreshold(2) = 4#:  TPthreshold(3) = 3#:  TPthreshold(4) = 2.5
  TPthreshold(5) = 2#:   TPthreshold(6) = 1.5:   TPthreshold(7) = 1#:  TPthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Tropopause offset threshold statistics"
  For i = 1 To 8
    Call AvgRMSdiffTP(Zac(), Ztp(), OutCounter, TPthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out
    End If
  Next i
  Close OUTlu

  Case 11      'Open Tools Dialog Box
'    If cmdBlend(11).Caption = "Open &Tools" Then
'      cmdBlend(11).Caption = "Close &Tools"
      frmTools.Show
'    Else
'      cmdBlend(11).Caption = "Open &Tools"
'      frmTools.Hide
'    End If


  Case 12   ' Flight level Tcal for all instruments other than NGV
' Open following files that are all located in the /Mission/RAOB/ folder
' Open RAOB2 File with time-sorted RAOBs:    Mission_RAOBs.RAOB2
' Open output RAOB2 file with those used:    Mission_RAOBused.RAOB2
' Open file with list of closest approaches: Mission_RAOBrangeUsed.txt (from MTPbin/Tools)
' Open output file with RAOB comparisons:    RAOBcomparison.txt

  cmdBlend(12).Caption = "BUSY!"
  Prefix$ = txtPrefix.Text
  PrefixR$ = Prefix$ + "RAOB\"
  RAOBtype = 0
  Rcounter = 0
  iDlast = 0
  SameFlight = False
  
  RAOBlu% = FreeFile
  Filename$ = PrefixR$ + Mission$ + "_RAOBs.RAOB2"
  Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
  txtRAOBfile.Text = Filename$
  
  FileRecords = LOF(RAOBlu%) \ Len(RAOB2)
  txtFileRec = Str$(FileRecords)
  DoEvents
  optMode(1).Value = True
  Cycle = 0
  cmdOpen.Caption = "&Close RAOB"
  txtASCIIout.Text = fReplaceExtension(Filename$, "txt")
' And fill Search List
  Call cmdSelect_Click(4)  'Clear list box
  Call cmdSelect_Click(0)  'And refill it
  Navg = Val(txtNavg.Text)
  AC$ = Mid$(Prefix$, 13, 2)
  OUTlu = FreeFile
  Open PrefixR$ + "RAOBcomparison.txt" For Output As OUTlu
  Select Case Mission$
  Case "GloPac", "ATTREX"
    header$ = "Num" + vbTab + "RAOB" + vbTab + "YMD" + vbTab + "UT" + vbTab + "Zp" + vbTab + "Range" + vbTab + "Zt1" + vbTab + "Tt1" + vbTab
    header$ = header$ + "Zt2" + vbTab + "Tt2" + vbTab + "Zt1-Zt2" + vbTab + "Tt1-Tt2" + vbTab + "ATHL1" + vbTab + "ATHL2" + vbTab + "ATHR1" + vbTab + "ATHR2" + vbTab + "AT_A" + vbTab
    header$ = header$ + "Trab" + vbTab + "Traa" + vbTab + "Traa-Trab" + vbTab + "Trawt" + vbTab + "AT_A-Tra" + vbTab + "ATHL1-Tra" + vbTab + "ATHL2-Tra" + vbTab + "ATHR1-Tra" + vbTab + "XMACH2"
  Case Else     '
    header$ = "Num" + vbTab + "RAOB" + vbTab + "YMD" + vbTab + "UT" + vbTab + "Zp" + vbTab + "Range" + vbTab + "Zt1" + vbTab + "Tt1" + vbTab
    header$ = header$ + "Zt2" + vbTab + "Tt2" + vbTab + "Zt1-Zt2" + vbTab + "Tt1-Tt2" + vbTab + "ATHL1" + vbTab + "ATHL2" + vbTab + "ATHR1" + vbTab + "ATHR2" + vbTab + "AT_A" + vbTab
    header$ = header$ + "Trab" + vbTab + "Traa" + vbTab + "Traa-Trab" + vbTab + "Trawt" + vbTab + "AT_A-Tra" + vbTab + "ATHL1-Tra" + vbTab + "ATHL2-Tra" + vbTab + "ATHR1-Tra" + vbTab + "XMACH2"
  End Select
  Print #OUTlu, header$

  lu = FreeFile
  Open PrefixR$ + Mission$ + "_RAOBrangeUsed.txt" For Input As lu
  
  Input #lu, A$
  chkMultipleScans.Value = 1
  OutCount = 0
  OutCounter = 0
  Do
GetNextIWG:
    Rcounter = Rcounter + 1
    If EOF(lu) Then Exit Do
    Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
    Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; A$
    If iNum > 44 Then
      X = X
    End If
    If iD = iDlast Then SameFlight = True Else SameFlight = False
    iDlast = iD
    DTflight = DateSerial(iY, iM, iD)
    UTsave = UT0
    hms0$ = fSecToTstring(Int(UT0 * 1000), False)
    DT0 = DTflight + TimeSerial(Left$(hms0$, 2), Mid$(hms0$, 3, 2), Right$(hms0$, 2))
    
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    ii = InStr(i + 1, A$, vbTab)
    RAOB2x$ = Mid$(A$, i + 1, ii - i - 1)
    i = ii
    ii = InStr(i + 1, A$, vbTab)
    Ra1 = Val(Mid$(A$, i + 1, ii - i - 1))  'Get Range to first site
    cmdSelect_Click (5)                     'clear RAOB display
    
    ' Check that we are within minimum acceptable range
    If Ra1 < Val(Trim(txtMaxRange.Text)) Then
      YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      Filename$ = Prefix$ + "IWG\IWG1_" + YYYYMMDD$ + ".RIF"
      PNGname$ = Prefix$ + "PNG\R" + Format(iNum, "000") + "_" + YYYYMMDD$ + RAOB1$ + ".PNG"
      Call RollOver(iY, iM, iD, UT0)
      DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring(UT0 * 1000, True))
      txtEvent.Text = DTevent
      Documentation$ = Format(iNum, "000") + " " + Format(UTsave, "000.000") + "_ks " + Format(Ra1, "000") + "_km "
      If RAOB2x$ = RAOB1$ Then 'Only one site involved
        IWGlu = FreeFile
        Open Filename$ For Random Access Read Write As IWGlu Len = Len(IWG)
        If Not SameFlight Then Irecord = 0
        ' Go to first record in aircraft data that is beyond time of closest approach
        Do
          Irecord = Irecord + 1
          Call IWGread(IWGlu%, Irecord&)
          Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsec&, False)
          If iNum >= 57 Then
          X = X
          End If
          If DandT - DTflight >= 1 Then UTsec = UTsec + 86400
        Loop Until UTsec / 1000# >= UTsave Or EOF(IWGlu) = True
        If EOF(IWGlu) Then Exit Do
        
        ' Get averaged OAT from aircraft data; note that any fTc correction is made here
        Call AverageNcyclesIWG(IWGlu, Irecord, UTsecLast&, Navg, aZp!, aTna!, aTnr!, aTnr2!, aTnr3!, aTnr4!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aMach2, Nada)
        Close IWGlu
         If Nada Then GoTo GetNextIWG
        
        With frmRAOB
        .ALTkm = aZp
        .Tac = aTna     'xxxmjm 20090122
        .LineThickness = 1
        End With
        
'       Now get the before and after radiosondes
        Call GetRAOBs(DT0, RAOB1$, Beforecycle&, AfterCycle&)
        If AfterCycle = 0 Or ZPraob(Nlevr) < aZp Then  'Make sure AfterCycle and after RAOB above aZp
          GoTo GetNextIWG
        End If
        OutCounter = OutCounter + 1
        Call UniqueRecords(OutCount&, RecordsUsed&(), Beforecycle)
        Call UniqueRecords(OutCount&, RecordsUsed&(), AfterCycle)
        
'       Get Before RAOB
        Call RAOBread(RAOBlu, Beforecycle)
        If ZPraob(Nlevr) < aZp Then GoTo GetNextIWG    'Make sure before RAOB above aZp
        DT1 = DandT
        nTZ1 = Nlevr
        OAT11 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        
        Site1Lat = WMOlatitude
        Site1Lon = WMOlongitude
        Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)

        out$ = Format(iNum, "000") + vbTab + RAOB1$ + vbTab + YYYYMMDD$ + vbTab + Format(UT0, "##0.000") + vbTab
        out$ = out$ + Format(aZp, "#0.00") + vbTab + Format(Site1Distance, "##0.0") + vbTab
        Ztrop1 = Ztrop
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        dZtrop = Ztrop
        dTtrop = Ttrop
        Call CopyArray(ZPraob(), Zp1(), Nlevr)
        Call CopyArray(TZraob(), TZ1(), Nlevr)

'       Get After RAOB
        Call RAOBread(RAOBlu, AfterCycle)
        If Ztrop > 0 Then
          out$ = out$ + Format(Ztrop, "#0.00") + vbTab + Format(Ttrop, "000.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        If dZtrop > 0 And Ztrop > 0 Then
          dZtrop = dZtrop - Ztrop
          dTtrop = dTtrop - Ttrop
          out$ = out$ + Format(dZtrop, "#00.00") + vbTab + Format(dTtrop, "#0.00") + vbTab
        Else
          out$ = out$ + " " + vbTab + " " + vbTab
        End If
        DT2 = DandT
        OAT12 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
        
        Call cmdBlend_Click(1)   'Temporal interpolation
        Call cmdSelect_Click(16) 'Write Documentation on image
        
        Trwt = OAT11 + ((OAT12 - OAT11) / (DT2 - DT1)) * (DT0 - DT1) 'Temperature RAOB time weighted
        Tna_Trwt = aTna - Trwt
        Tnr_Trwt = aTnr - Trwt
        Tnr2_Trwt = aTnr2 - Trwt
        Tnr3_Trwt = aTnr3 - Trwt
'        Debug.Print aTnr; aTna; aTna - aTnr; OAT11; OAT12; OAT12 - OAT11; Trwt; Tna_Trwt; Tnr_Trwt
' Tre  Tav  Tav_Tre  Tr1  Tr2  Tr2_Tr1  Trwt  aTna_Trwt  aTnr_Trwt
        'HIPPO-3 aTnr=ATHL1, aTnr2=ATHL2, aTnr3=ATHR1, aTna=AT_A
        out$ = out$ + Format(aTnr, "000.00") + vbTab
        out$ = out$ + Format(aTnr2, "000.00") + vbTab + Format(aTnr3, "000.00") + vbTab
        out$ = out$ + Format(aTnr4, "000.00") + vbTab + Format(aTna, "000.00") + vbTab
        out$ = out$ + Format(OAT11, "000.00") + vbTab + Format(OAT12, "000.00") + vbTab
        out$ = out$ + Format(OAT12 - OAT11, "#0.0000") + vbTab + Format(Trwt, "000.0000") + vbTab
        out$ = out$ + Format(Tna_Trwt, "##0.0000") + vbTab + Format(Tnr_Trwt, "##0.0000") + vbTab
        out$ = out$ + Format(Tnr2_Trwt, "##0.0000") + vbTab + Format(Tnr3_Trwt, "##0.0000") + vbTab + Format(aMach2, "##0.0000")
        Print #OUTlu, out$
        aTnavi(OutCounter) = aTna
        aTres(OutCounter) = aTnr
        aTraob(OutCounter) = Trwt
        aRange(OutCounter) = Site1Distance
        Zac(OutCounter) = aZp
        Ztp(OutCounter) = Ztrop + dZtrop / 2#
        
        OATmtp = aTna      'Use the avionics T on the comparison plot for a/c T at pALT
        pALT = aZp
        cmdToggleAC_Click  'Plots AC pALT at OATmtp
        DoEvents
        If chkWritePNGs.Value = 1 Then
          ' Create PNG folder under Mission$ if necessary on first pass
          If Rcounter = 1 Then CreateDirectoryTree fGetPath(PNGname$), False
          frmRAOB.ScaleMode = 1
          CaptureFormMJ frmRAOB, PNGname$, 1, False
          'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        End If
        If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
        Call cmdSelect_Click(5)  'Clear plot
      
      Else  'spatial interpolation needs to be programmed
      
      End If
    End If
  
  Loop Until EOF(lu) ' End of Mission$_RAOBs.RAOB2 file
  Close lu

' Write out all the unique RAOBs used to Mission$_RAOBused.RAOB2
  RAOBlu2 = FreeFile
  FileName2$ = PrefixR$ + Mission$ + "_RAOBused.RAOB2"   'Write out RAOBs actually used for use elsewhere
  If Dir$(FileName2$) <> "" Then Kill FileName2$
  Open FileName2$ For Random Access Read Write As RAOBlu2% Len = Len(RAOB2)
  Call BubbleSortL(RecordsUsed(), OutCount)
  For iL = 1 To OutCount
    Call RAOBread(RAOBlu, RecordsUsed(iL))
    Call RAOBwrite(RAOBlu2, iL)
  Next iL
  Close RAOBlu, RAOBlu2  'Leave OUTlu open
  
' Now do Range and Temperature Threshold statistics
' Write T threshold data for avionics
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for avionics"
  For i = 1 To 8
    Call AvgRMSdiffT(aTnavi(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write T threshold data for research
  Tthreshold(1) = 5#:  Tthreshold(2) = 4#:  Tthreshold(3) = 3#:  Tthreshold(4) = 2.5
  Tthreshold(5) = 2#:   Tthreshold(6) = 1.5:   Tthreshold(7) = 1#:  Tthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Temperature threshold statistics for research"
  For i = 1 To 8
    Call AvgRMSdiffT(aTres(), aTraob(), OutCounter, Tthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Range threshold data for avionics
  Rthreshold(1) = 300#:  Rthreshold(2) = 250#:  Rthreshold(3) = 200#:  Rthreshold(4) = 150#
  Rthreshold(5) = 100#:   Rthreshold(6) = 75#:   Rthreshold(7) = 50#:  Rthreshold(8) = 25#
  Rthreshold(9) = 10#:   Rthreshold(10) = 5#
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for avionics"
  For i = 1 To 10
    Call AvgRMSdiffR(aTnavi(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i

' Write Range threshold data for research
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Range threshold statistics for research"
  For i = 1 To 10
    Call AvgRMSdiffR(aTres(), aTraob(), aRange(), OutCounter, Rthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Rthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "00.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out$
    End If
  Next i
  
' Write Tropopause offset threshold data
  TPthreshold(1) = 5#:  TPthreshold(2) = 4#:  TPthreshold(3) = 3#:  TPthreshold(4) = 2.5
  TPthreshold(5) = 2#:   TPthreshold(6) = 1.5:   TPthreshold(7) = 1#:  TPthreshold(8) = 0.5
  out$ = ""
  Print #OUTlu, out$
  Print #OUTlu, "Tropopause offset threshold statistics"
  For i = 1 To 8
    Call AvgRMSdiffTP(Zac(), Ztp(), OutCounter, TPthreshold(i), M%, AVG!, RMS!)
    If M > 1 Then
      out$ = Format(Tthreshold(i), "#0.0") + vbTab + Format(M, "000") + vbTab
      out$ = out$ + Format(1# / Sqr(M - 1), "0.000") + vbTab + Format(AVG, "#0.000") + vbTab + Format(RMS / Sqr(M - 1), "#0.000")
      Print #OUTlu, out
    End If
  Next i
  Close OUTlu
  cmdBlend(12).Caption = "IWG FL Tcal"
  
End Select

End Sub



Private Sub cmdColdest_Click()
Dim i%, j%, k%, M%, n%, Line$, Ct!(1 To 50), CZ!(1 To 50), X$
Dim Tmin!, Zmin!, ColdestAlt!, RAOBcount&
Dim StartTime As Date, EndTime As Date, PrintOut As Boolean, OUTlu%
  
  StartTime = cmbStart(0).Text + "/" + Format$(cmbStart(1).Text, "00") + "/" + Format(Val(cmbStart(2).Text), "00") + " " + Format(Val(cmbStart(3).Text), "00") + ":00"
  EndTime = cmbEnd(0).Text + "/" + Format$(cmbEnd(1).Text, "00") + "/" + Format(Val(cmbEnd(2).Text), "00") + " " + Format(Val(cmbEnd(3).Text), "00") + ":00"
  '  For GetDate = StartTime To EndTime Step Val(cmbHourStep.Text) / 24# '12 Hour steps
  PrintOut = True
  If PrintOut = True Then
    OUTlu = FreeFile
    Open MNpath$ + "ColdestT.txt" For Output As OUTlu
  End If
  
  Zmin = Val(txtColdestAlt.Text)
  ColdestT = 400#
  For M = 1 To 50
    Ct(M) = 400#
  Next M

  n = Val(lstSelect.ListCount)
  If n = 0 Then Exit Sub
  
  For i = 0 To n - 1
    X$ = lstSelect.List(i)
    lblProgress.Caption = "Record"
    'If AutoView Then cmdSelect_Click (3)
    Cycle = Val(Right$(X$, 5))
    txtProgress.Text = Str$(Cycle)
    Call RAOBread(RAOBlu, Cycle)

'    For k = 8 To 28 Step 4
      Zmin = k
'     Find RAOB minimum temperature, or temperature at a particular altitude
      Tmin = 400#
      If chkEnableAlt.Value = 1 Then
        ' Make sure RAOB has data below Zmin
        If ZPraob(1) < Zmin And ZPraob(Nlevr) > Zmin Then Tmin = fLinterp(ZPraob(), TZraob(), Nlevr, Zmin)
      Else  'Find Tmin at any alt
        For j = 1 To Nlevr    'averaging pressure altitudes
          If TZraob(j) < Tmin And j < Nlevr Then
            Tmin = TZraob(j)
            Zmin = ZPraob(j)
          End If
        Next j
      End If
    '500 mb = 5.5 km
      If PrintOut And Tmin < 400 And Zmin > 5 And ZPraob(Nlevr) > 15 And ZtropE < 19 And ZtropE > 5 Then
        If WMO4l = "XXXX" Then Line$ = "WMO" + Str(WMOnumber) Else Line$ = WMO4l
        Line$ = Line$ + vbTab + Format(Iday, "00") + vbTab + Format(Ihour, "00") + vbTab + Format(Imin, "0000") + vbTab + Format(Tmin, "###.##") + vbTab + Format(Zmin, "##.##") + vbTab + Format(TtropE, "###.##") + vbTab + Format(ZtropE, "##.##")
        Print #OUTlu, Line$
      End If
      DoEvents
'    Next k
'   Sort by Tmin (lowest temperature first)
    If chkSort.Value = 1 Then
      n = 0
      Do: n = n + 1
      Loop Until Tmin <= Ct(n) Or n = 50
    
      If n < 50 Then
        For M = 50 To n + 1 Step -1
          Ct(M) = Ct(M - 1)
          CZ(M) = CZ(M - 1)
        Next M
        Ct(n) = Tmin
        CZ(n) = Zmin
        If n = 1 Then
          ColdestT = Ct(1)
          ColdestZ = CZ(1)
          txtColdestT.Text = Format(ColdestT, "###.##")
          txtColdestAlt.Text = Format(ColdestZ, "##.##")
          Line$ = "Station: WMO " + Format$(WMOnumber, "00000")
          If WMO4l <> "" Then Line$ = Line$ + "   " + WMO4l + "  N=" + Format$(Nlevr, "00")
          Line$ = Line$ + "Time: " + Format$(Iyear, "0000") + " " + Format$(Imonth, "00") + " "
          Line$ = Line$ + Format$(Iday, "00") + " " + Format$(Ihhmm, "0000") + "Z  DOY:" + Format$(Idoy, "000")
          txtColdestRAOB.Text = Line$
          DoEvents
        End If
      End If
    End If
  Next i

  
  For i = 1 To 50
   'Debug.Print i; CT(i); CZ(i)
  Next i
  
  If PrintOut = True Then Close OUTlu
  
  RAOBcount = Val(lstSelect.ListCount)

End Sub

Private Sub cmdColdestT_Click()
Dim j%, Z1!, Z2!, TotalRec%

  If RAOBlu = 0 Then
    MsgBox "You must open a RAOB file before using this command!", vbOKOnly
    Exit Sub
  End If
  DoEvents
  Cycle = 0
  TotalRec = 0
  Do
    Cycle = Cycle + 1

    Call RAOBread(RAOBlu, Cycle)
    Tcoldest = 400#
    For j = 1 To Nlevr    'averaging pressure altitudes
      If TZraob(j) < Tcoldest And j < Nlevr Then
        Tcoldest = TZraob(j)
        Zcoldest = ZPraob(j)
      End If
    Next j
'   Calculate dTMRI for Good RAOBs
'   NB Use pALT for consistency with trop solutions, calc using GeoPot Height if necessary
    Z1 = Val(txtT10.Text): Z2 = Val(txtT17.Text)
    dTMRI = fdTMRI(ZPraob(), TZraob(), Nlevr, Z1, Z2)
    Call RAOBwrite(RAOBlu, Cycle)
    txtProgress.Text = Str(Cycle)
    DoEvents
  Loop Until EOF(RAOBlu) Or Cycle = FileRecords

End Sub

Private Sub cmdConvertIn_Click()
' Set Initial directory
cdConvertIn.InitDir = Drive$ ' + Rdir$ + Mission$
' Set Size of FileName buffer
cdConvertIn.MaxFileSize = 1024
' Set filters.
cdConvertIn.Filter = "Mask Files|" + txtFileMask.Text + "|All Files (*.*)|*.*"
' Specify default filter.
cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
cdConvertIn.ShowOpen

ConvertInFile$ = cdConvertIn.Filename

If ConvertInFile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  INPlu% = FreeFile
  Open ConvertInFile$ For Input As INPlu%
  txtConvertIn.Text = ConvertInFile$
  'FileRecords = LOF(RAOBlu%)
  'ProgressBar2.Min = 0
 ' ProgressBar2.Max = FileRecords
  'frmREFview.txtFileRecords.Text = FileRecords
End If
  
End Sub

Private Sub cmdConvertOut_Click()
Dim ConvertOutFile$

' Set Initial directory
cdConvertIn.InitDir = Drive$ + Rdir$ + Mission$
' Set Size of FileName buffer
cdConvertIn.MaxFileSize = 1024
' Set filters.
cdConvertIn.Filter = "RAOB Files (*.sjk, *.RAOB)|*.sjk|All Files (*.*)|*.*"
' Specify default filter.
cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
cdConvertIn.ShowOpen

ConvertOutFile$ = cdConvertIn.Filename
txtConvertOut.Text = ConvertOutFile$

If ConvertOutFile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler

  
  'ProgressBar2.Min = 0
 ' ProgressBar2.Max = FileRecords
  'frmREFview.txtFileRecords.Text = FileRecords
End If

End Sub



Private Sub UpdateRAOBlisting()
Dim Line$, RAOBfmt$, i%, V!(1 To 20), iMax%, Tmax!, Tmin!, iTmax%, iTmin%
Dim IntRhoVcm!, iBI%, RHthreshold!, BI!, j%

Call DetermineROABtoShow

Line$ = "Station: WMO " + Format$(WMOnumber, "00000")
If WMO4l <> "" Then Line$ = Line$ + "   " + WMO4l + "  N=" + Format$(Nlevr, "00")
lblLoc.Caption = "Location: Lat: " + Format(WMOlatitude, "#00.00") + " Lon: " + Format(WMOlongitude, "#000.00")
lblSite.Caption = Line$
Line$ = "Time: " + Format$(Iyear, "0000") + " " + Format$(Imonth, "00") + " "
Line$ = Line$ + Format$(Iday, "00") + " " + Format$(Ihhmm, "0000") + "Z  DOY:" + Format$(Idoy, "000")
lblTime.Caption = Line$
Line$ = "Trop: " + Format(frmRAOB.Zt1, "00.000") + " km, " + Format$(frmRAOB.TT1, "000.00") + " K"
lblTrop.Caption = Line$
  Tmin = 400#
  For j = 1 To Nlevr    'averaging pressure altitudes
    If TZraob(j) < Tmin And j < Nlevr Then
      Tmin = TZraob(j)
      Zmin = ZPraob(j)
    End If
  'Debug.Print j; TZraob(j); ZPraob(j)
  Next j
  txtColdestT.Text = Format(Tmin, "###.##")
  txtColdestAlt.Text = Format(Zmin, "##.##")


'If Ihhmm = 1800 Then Debug.Print Format(frmRAOB.Tt1, "000.00") + " " + Format$(frmRAOB.Zt1, "00.00") + " " + Format(Tmin, "###.##") + " " + Format(Zmin, "##.##")
'Set flags
If RAOBgood Then
  txtGood(0).BackColor = &H8000&
Else
  txtGood(0).BackColor = &HFF&
End If

If TropGood Then
  txtGood(1).BackColor = &H8000&
Else
  txtGood(1).BackColor = &HFF&
End If

If EditLevel Then
  txtMod(0).Text = Str$(Nedit)
  txtMod(0).BackColor = &H8000&
Else
  txtMod(0).Text = ""
  txtMod(0).BackColor = &HFF&
End If
If EditTrop Then txtMod(1).BackColor = &H8000& Else txtMod(1).BackColor = &HFF&

If UseEditLevel Then txtUse(0).BackColor = &H8000& Else txtUse(0).BackColor = &HFF&
If UseEditTrop Then txtUse(1).BackColor = &H8000& Else txtUse(1).BackColor = &HFF&

RAOBfmt$ = "#000 #0.000 #0.000 #000.0 #00.00 #00.0 #00.0"
lstListRAOB.Clear
For i = Nlevr To 1 Step -1
  V(1) = i: V(2) = ZGraob(i): V(3) = ZPraob(i): V(4) = PZraob(i): V(5) = TZraob(i): V(6) = RZraob(i): V(7) = fTheta(TZraob(i), PZraob(i))
  Line$ = fUsing(RAOBfmt$, V())
  lstListRAOB.AddItem Line$
Next i

If LT1 < Nlevr Then lstListRAOB.ListIndex = Nlevr - LT1

IntRhoVcm = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)
frmFog.txtIntRhoV.Text = Format(IntRhoVcm, "#0.00")
txtIntRhoV2.Text = Format(IntRhoVcm, "#0.00")
RHthreshold = Val(frmFog.txtRHthreshold.Text)

i = 0
Do: i = i + 1: Loop Until ZGraob(i) >= 2
iMax = i - 1

Tmin = 400
Tmax = 0
For i = 1 To iMax
  If TZraob(i) < Tmin Then Tmin = TZraob(i): iTmin = i
  If TZraob(i) > Tmax Then Tmax = TZraob(i): iTmax = i
Next i

If Tmax - Tmin > 5 And iTmax - iTmin > 0 Then
  If iTmin = 1 Then iBI = iTmin Else iBI = iTmin - 1
  Do
    iBI = iBI + 1
    BI = ZGraob(iBI) * 1000        'Convert to meters
  Loop Until TZraob(iBI) - TZraob(iTmin) > 3 And RZraob(iBI) < RHthreshold Or iBI > 299
  iBI = iBI - 1
End If
frmFog.txtBI.Text = Format(BI, "#000")
txtBI2.Text = Format(BI, "#000")

' Determine levels for which RH >RH threshold
iLiquid1 = 0
iLiquid2 = 0
For i = 1 To iMax
  If RZraob(i) >= RHthreshold And iLiquid1 = 0 Then iLiquid1 = i
  If RZraob(i) < RHthreshold And iLiquid1 <> 0 And iLiquid2 = 0 Then iLiquid2 = i
Next i
If iLiquid2 = 0 And iLiquid1 <> 0 Then iLiquid2 = iTmin
If iLiquid1 * iLiquid2 <> 0 Then
  txtLiquid1.Text = Format(1000 * ZGraob(iLiquid1), "#000")
  'txtLiquid2.Text = Format(1000 * ZGraob(iLiquid2 - 1), "#000")
  frmFog.txtLiquid12.Text = Format(1000 * ZGraob(iLiquid1), "#000")
  'txtLiquid22.Text = Format(1000 * ZGraob(iLiquid2 - 1), "#000")
Else
  txtLiquid1.Text = ""
  txtLiquid2.Text = ""
  frmFog.txtLiquid12.Text = ""
  frmFog.txtLiquid22.Text = ""
End If
If iLiquid2 > iTmin Then
  BI = ZGraob(iLiquid2 - 1) * 1000
  frmFog.txtBI.Text = Format(BI, "#000")
  txtBI2.Text = Format(BI, "#000")
End If

End Sub



Private Sub cmdDescription_Click()
frmRAOB.RAOB_Plot_Description (txtDescription.Text)
End Sub

Private Sub cmdExit_Click()
mnuFileExit_Click
End Sub

Private Sub cmdExport_Click()
Dim X$, OUTfile$, OUTlu%, V!(1 To 6), i%, SJKfmt$, Cycle&, BLGfmt$

' Assume that *.RAOB file is used for input
'123456789012345678901234567890
' 72469   DEN   971112  12Z   P[mb], Zp[m], Zg[m], T[C], DPD[C]
'
'"#####0.0 #####0 #####0 ###0.0"
''   711.0   2889  99999   -4.5
''   700.0   3012   2940   -4.7
Select Case ExportMode
Case 0  'Single ASCII Files
  Select Case ExportFormat
  Case 0 'BLG
    BLGfmt$ = "#####0.0 #####0 #####0 #00.0 #00.0"
    Cycle = 0
    Do
      Cycle = Cycle + 1
      Call RAOBread(RAOBlu, Cycle)
      OUTfile$ = "R" + Format(Iyear, "0000") + Hex$(Imonth) + Format(Iday, "00") + "_" + Format(Ihhmm, "0000")
      Select Case ijkl
      Case 1: X$ = "i"
      Case 2: X$ = "j"
      Case 3: X$ = "k"
      Case 4: X$ = "l"
      Case Else: X$ = "_"
      End Select
      OUTfile$ = OUTfile$ + X$ + "." + Trim$(WMO4l)
      OUTlu = FreeFile
      Open txtExportPath.Text + OUTfile$ For Output As OUTlu
      X$ = " " + Format(WMOnumber, "00000") + "   " + Trim(WMO4l) + "   "
      X$ = X$ + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
      X$ = X$ + "   " + Format(Ihhmm, "0000") + "Z LAT= " + Format(WMOlatitude, "#00.000") + " LON= " + Format(WMOlongitude, "#00.000")
      Print #OUTlu, X$
'          "#####0.0 #####0 #####0 ###0.0"
      X$ = "   P[mb]  Zp[m]  Zg[m]    T[K]  RH[%]"
      Print #OUTlu, X$
      For i = 1 To Nlevr
        V(1) = PZraob(i): V(2) = ZPraob(i) * 1000#: V(3) = ZGraob(i) * 1000#
        V(4) = TZraob(i): V(5) = RZraob(i)
        Print #OUTlu, fUsing(BLGfmt$, V())
      Next i
      Close OUTlu
    Loop Until Cycle = FileRecords
  Case 1 'SJK
  
  Case 2 'VB6
      OUTfile$ = "c:\MTP\RAOB\VB6_RAOB.txt"
      OUTlu = FreeFile
      Open OUTfile$ For Output As OUTlu
      Print #OUTlu, Nlevr
      X$ = Format(WMOnumber, "00000") + "   " + Trim(WMO4l) + "   "
      X$ = X$ + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
      X$ = X$ + "   " + Format(Ihhmm, "0000") + "Z LAT= " + Format(WMOlatitude, "#00.000") + " LON= " + Format(WMOlongitude, "#00.000")
      Print #OUTlu, X$
      X$ = "   P[mb]  Zp[m]  Zg[m]    T[K]  RH[%]"
      Print #OUTlu, X$
      For i = 1 To Nlevr
        X$ = Format(PZraob(i), "###0.000") + vbTab
        X$ = X$ + Format(ZPraob(i), "#0.0000") + vbTab
        X$ = X$ + Format(ZGraob(i), "#0.0000") + vbTab
        X$ = X$ + Format(TZraob(i), "000.000") + vbTab
        X$ = X$ + Format(RZraob(i), "00.0")
'        X$ = "PZraob(" + Format(i, "##0") + ") = " + Format(PZraob(i), "###0.0") + ": "
'        X$ = X$ + "ZPraob(" + Format(i, "##0") + ") = " + Format(ZPraob(i), "#0.00") + ": "
'        X$ = X$ + "ZGraob(" + Format(i, "##0") + ") = " + Format(ZGraob(i), "#0.00") + ": "
'        X$ = X$ + "TZraob(" + Format(i, "##0") + ") = " + Format(TZraob(i), "000.0") + ": "
'        X$ = X$ + "RZraob(" + Format(i, "##0") + ") = " + Format(RZraob(i), "00.0")
        Print #OUTlu, X$
      Next i
      Close OUTlu
  End Select
  
Case 1  'One Combined ASCII file
  Select Case ExportFormat
  Case 0
    BLGfmt$ = "#####0.0 #####0 #####0 ###0.0"
    i = InStr(1, txtRAOBfile.Text, ".")
    OUTfile$ = Left$(txtRAOBfile.Text, i) + "txt"
    OUTlu = FreeFile
    Open OUTfile$ For Output As OUTlu
    Cycle = 0
    Do
      Cycle = Cycle + 1
      Call RAOBread(RAOBlu, Cycle)
      X$ = " " + Format(WMOnumber, "00000") + "   " + Trim(WMO4l) + "   "
      X$ = X$ + Mid$(Format(Iyear, "0000"), 3, 2) + Format(Imonth, "00") + Format(Iday, "00")
      X$ = X$ + "   " + Format(Ihour, "00") + "Z"
      Print #OUTlu, X$
'          "#####0.0 #####0 #####0 ###0.0"
      X$ = "   P[mb]  Zp[m]  Zg[m]    T[C]  DPD[C]"
      Print #OUTlu, X$
      For i = 1 To Nlevr
        V(1) = PZraob(i): V(2) = ZPraob(i) * 1000#: V(3) = ZGraob(i) * 1000#
        V(4) = TZraob(i) - 273.15
        Print #OUTlu, fUsing(BLGfmt$, V())
      Next i
      Print #OUTlu, "EndOfRAOB"
    Loop Until Cycle = FileRecords
    Close OUTlu
    
  Case 1 'SJK
    SJKfmt$ = "###0.000 ####0.0 #000.00 ##0.00"
    i = InStr(1, txtRAOBfile.Text, ".")
    OUTfile$ = Left$(txtRAOBfile.Text, i) + "sjk"
    OUTlu = FreeFile
    Open OUTfile$ For Output As OUTlu
    Cycle = 0
    Do
      Cycle = Cycle + 1
      Call RAOBread(RAOBlu, Cycle)
'recno yy mm dd hh Nlevr
'####0 #0 #0 #0 #0 #0"
'    2 80  2 26 12 23
'###0.000 ####0.0 #000.00 ##0.00"
'   0.000  1014.0  299.56  83.51
'   0.122  1000.0  297.96  83.76
      V(1) = Cycle: V(2) = Val(Right$(Str$(Iyear), 2)): V(3) = Imonth
      V(4) = Iday: V(5) = Ihour: V(6) = Nlevr
      Print #OUTlu, fUsing("####0 #0 #0 #0 #0 #0", V())
      For i = 1 To Nlevr
        V(1) = ZGraob(i): V(2) = PZraob(i): V(3) = TZraob(i): V(4) = RZraob(i)
        Print #OUTlu%, fUsing(SJKfmt$, V())
      Next i
    Loop Until EOF(RAOBlu)
    Close OUTlu
  
  End Select
  
End Select

End Sub

Private Sub cmdGAL_Click(Index As Integer)
Dim lu%
' Program to fix missing names in FH files.
  Select Case Index
  Case 0  'SEST
    WMO4l = "SEST"
    WMOnumber = 84008
  Case 1  'MROC
    WMO4l = "MROC"
    WMOnumber = 78762
  End Select
  lu = FreeFile
  Open "C:\mtp\data\ER2\TC4\raob\fh\FHfix.RAOB2" For Random Access Read Write As lu% Len = Len(RAOB2)
  Call RAOBwriteSL(lu%, Cycle&)
  Close lu
End Sub

Private Sub cmdGetXS_Click()
Dim PV!(1 To 17), T!(1 To 17), Zg!(1 To 17), Zp!(1 To 17), Wz!(1 To 17), Wm!(1 To 17)
Dim Quit As Boolean, UT&, XSfile$, Levels%

'  Forward2 = True
'  tmrGetCycle2_Timer

  XSfile$ = Root$ + Mission$ + "\XS\XS" + YYYYMMDD$ + ".DC8"
  Call GetXSvalues(1, UTsec&, UT&, Latitude!, Longitude!, PV(), T(), Zg(), Zp(), Wz(), Wm(), Levels%, XSfile$, Quit)
' Select only Cycles that have good trops
  If Nlev1 > 0 Then
    LT1 = 1: Lt2 = 1
    Nlev1 = 1: Nlev2 = Levels: Nlev = Levels
    For i = Nlev1 To Nlev2
      If chkGeoPotHeight(0).Value = 1 Then
        RAp(i - Nlev1 + 1) = Zg(i)
'        Lt(i - Nlev1 + 1) = Zp(i)
'        If Lt(i - Nlev1 + 1) > Zt1 And Lt1 = 1 And Zt1 < 90# Then Lt1 = i - Nlev1
'        If Lt(i - Nlev1 + 1) > Zt2 And Lt2 = 1 And Zt2 < 90# Then Lt2 = i - Nlev1
      Else
        RAp(i - Nlev1 + 1) = Zp(i)
      End If
      RTp(i - Nlev1 + 1) = T(i)
    Next i
  End If
  
  With frmATP
    If chkEnableFit.Value = 1 Then
      .EnableFit = True
    Else
      .EnableFit = False
    End If
    If chkShowTropopause.Value = 1 Then
      .ShowTropopause = True
    Else
      .ShowTropopause = False
    End If
  End With
  'UpdateForm
  
frmATP.ATPupdate

End Sub

Private Sub cmdHKM_Click()
Dim i%

For i = 1 To 10
  Cycle = i
  RAOBread RAOBlu, Cycle
  WMO4l = Trim("HK" + Format(i, "00"))
  RAOBwrite RAOBlu, Cycle
Next i

End Sub

Private Sub cmdList_Click(Index As Integer)
Dim Line$, i%, M!, n%

Select Case Index
Case 0  'Refresh
  If Not frmRAOB.PlotSingleScan Then
    frmRAOB.SetDefaultScales
    frmRAOB.RAOB_Plot_Set
    frmRAOB.RAOB_Plot_Axes_Labels
    frmRAOB.RAOB_Plot_Grid_Marks
    frmRAOB.RAOB_Plot_Tic_Marks
  End If

Case 1  'Save changes in edit window
  Nedit = Nedit + 1
  txtMod(0).Text = Str$(Nedit)
  i = Nlevr + Nedit
  If Nedit < 4 Then
    ZGraob(i) = Val(txtList(1).Text)
    ZPraob(i) = Val(txtList(2).Text)
    PZraob(i) = fZtoP(ZPraob(i))
    TZraob(i) = Val(txtList(4).Text)
    RZraob(i) = Val(txtList(5).Text)
    LZraob(Nedit) = SelectLevel
    frmRAOB.RAOBupdate           'for now, redraw entire plot
    EditLevel = True
    UseEditLevel = True
    Cycle = Val(Right$(lstSelect.List(lstSelect.ListIndex), 5))
    Call RAOBwrite(RAOBlu, Cycle)
  End If
  
Case 2  'Show Trop LR lines
  frmRAOB.RAOB_Draw_LR_Lines Val(txtList(2).Text), Val(txtList(4).Text), 99.9, 999.9
  Exit Sub
  
Case 3  'Save Trop
  ZtropE = ZPraob(SelectLevel)
  TtropE = TZraob(SelectLevel)
  LtropE = SelectLevel
  Zt1 = ZtropE: TT1 = TtropE
  frmRAOB.RAOBupdate           'for now, redraw entire plot
  EditTrop = True
  UseEditTrop = True
  Line$ = "Trop: " + Format(ZtropE, "00.000") + " km, " + Format$(TtropE, "000.00") + " K"
  lblTrop.Caption = Line$
  Call RAOBwrite(RAOBlu, CurrentRecord)
  
Case 4                         'Find Trop
  DetermineROABtoShow
  Call FindRAOBtrop(Zplot(), Tplot(), Nlevr, ZtropE, TtropE, LtropE, Ztrop2, Ttrop2, Ltrop2)
  If ZtropE > 0 Then
    Line$ = "Trop: " + Format(ZtropE, "00.000") + " km, " + Format$(TtropE, "000.00") + " K"
    lblTrop.Caption = Line$
    frmRAOB.RAOB_Draw_LR_Lines ZtropE, TtropE, Ztrop2, Ttrop2
    EditTrop = True
    UseEditTrop = True
    Call RAOBwrite(RAOBlu, CurrentRecord)
  End If
  
Case 5  'Edit T
  Nedit = Nedit + 1
  txtMod(0).Text = Str$(Nedit)
  i = Nlevr + Nedit
  n = Val(Trim(cboWindow.Text))
  If Nedit < 4 Then
    ZGraob(i) = Val(txtList(1).Text)
    ZPraob(i) = Val(txtList(2).Text)
    PZraob(i) = fZtoP(ZPraob(i))
    M = (TZraob(SelectLevel + n) - TZraob(SelectLevel - n)) / (ZPraob(SelectLevel + n) - ZPraob(SelectLevel - n))
    TZraob(i) = TZraob(SelectLevel - n) + M * (ZPraob(SelectLevel) - ZPraob(SelectLevel - n))
    RZraob(i) = Val(txtList(5).Text)
    LZraob(Nedit) = SelectLevel
    frmRAOB.RAOBupdate           'for now, redraw entire plot
    EditLevel = True
    UseEditLevel = True
    CurrentRecord = CLng(Val(txtProgress.Text))
    'Call RAOBwrite(RAOBlu, Cycle)
    cmdList_Click (4)
  End If

Case 6  'Undo any edits
  Nedit = 0
  EditLevel = False
  UseEditLevel = False

Case 7  'Interpolate to Zp value in Edit Box
  txtTinterpolated.Text = fLinterp(ZPraob(), TZraob(), Nlevr, Val(txtZp.Text))
  txtDTacMraob.Text = Format(Val(txtTac.Text) - Val(txtTinterpolated.Text), "#0.00")
  Debug.Print UTsec; pALT; Tac; Val(txtTinterpolated.Text); Val(txtDTacMraob.Text)
End Select

UpdateRAOBlisting
frmRAOB.RAOBupdate           'for now, redraw entire plot
frmRAOB.RAOB_SelectLevel (SelectLevel)

End Sub


Private Sub cmdMLS_Click()
Dim i%, j%, MLSlu%, QualMLS!, MTPlu%, PNGname$, Status As Boolean
Dim Tmls!(1 To 37), Zmls!(1 To 37), Amls!(1 To 37), Zmls2!(0 To 73), Tmls2!(1 To 37)
Dim LatMLS!, LonMLS!, UTmls!, A$, Nmls%, Zmls1!(1 To 37), NlevrSave%
Dim CurrentColor%, CurrentThickness%, StaMLS%, iSample%, ymd$, UTks!(1 To 10)
Dim Tmtp!(1 To 33), Zmtp!(1 To 33), REFfilename$, Cycle%(1 To 10), Rsave!(1 To 10), Count%
Dim iMTP%, iMLS%, Sum1!(1 To 37), Sum2!(1 To 37), X!, iLvl%(1 To 37), iStart%
Dim AVG(1 To 37), RMS(1 To 37)

  cmdOpenREFfile_Click
  ymd$ = YYYYMMDD$
  Call Calculate_MLS_Levels(Zmls2())
  For i = 1 To 37
    Zmls1(i) = Zmls2(2 * (i - 1))
    iLvl(i) = 0
    Sum1(i) = 0
    Sum2(i) = 0
  Next i
  
  Nmls = 37
  MLSlu = FreeFile
  Open "C:\MTP\Data\WB57\HAVE2\MLS\MLS" + ymd$ + ".txt" For Input As MLSlu
  
  'REFfilename$ = "C:\MTP\Data\WB57\HAVE2\WB" + ymd$ + ".REF"
  'REFopen REFfilename$

'123456789012345678901234567890
' 13-Jun-2005 19:50:47.158 UT
' lat 14.84  lon -94.79 Quality 1.752 Status 0
'Temperature -CorePlusR2A
'  300.029663  282.247681  265.274689  247.019485  224.349609  204.377594  195.143433  201.985535  210.505707  215.452438  223.356781  226.979965  232.304153  239.841064  247.596420  254.263962  260.060608  264.258301  267.758118  265.468536  258.335083  251.702179  246.242279  237.207260  223.941299  204.333282  195.840881  190.924942  183.778381  177.516098  178.079300  186.208603  192.297745  210.597336  266.591827  359.761719  359.761719
'
'
'Precision:
'   -5.000000   -6.250000   -7.500000    1.819545    1.006031    0.927071    0.814555    0.630475    0.506581    0.462597    0.475521    0.507861    0.525618    0.563792    0.606960    0.656223    0.701408    0.764138    0.827031    0.870282    0.924468    0.993841    1.050621    1.077566    1.031147    1.053075    1.130575    1.315924    1.504262    1.431637    2.153991  -20.000000  -20.000000  -20.000000  -20.000000  -20.000000  -20.000000
'**********************************************************************************
  iSample = 0
  Do
    Line Input #MLSlu, A$
    If Mid$(A$, 5, 3) = "Jun" Then
      iSample = iSample + 1
      UTmls = fTstringToSec(Mid$(A$, 14, 12), True) / 1000#
      Line Input #MLSlu, A$
      LatMLS = Mid$(A$, 5, 6)
      LonMLS = Mid$(A$, 16, 7)
     
      If Abs(LonMLS) < 100 Then
        QualMLS = Mid$(A$, 31, 6)
      Else
        QualMLS = Mid$(A$, 32, 6)
      End If
      Line Input #MLSlu, A$
      For i = 1 To 37
        Input #MLSlu, Tmls2(i)
      Next i
      Line Input #MLSlu, A$
      Line Input #MLSlu, A$
      Line Input #MLSlu, A$
      For i = 1 To 37
        Input #MLSlu, Amls(i)  'Precision
      Next i
      
    If iSample > 9 Then

      If QualMLS >= 1 And StaMLS Mod 2 = 0 Then
      Call GetAuraRange(LatMLS!, LonMLS!, 160#, 10#, Cycle(), UTks(), Rsave(), Count)
      
      For j = 1 To Count
      Call cmdSelect_Click(5)  'Clear plot
      Debug.Print Format(iSample, "00") + vbTab + Format(Cycle(j), "0000") + vbTab + Format(UTks(j), "00.00") + vbTab + Format(Rsave(j), "000.0")
      i = 0
      Nmls = 0
      Do
        i = i + 1
        If Amls(i) > 0 Then
          Nmls = Nmls + 1
          Zmls(Nmls) = Zmls1(i)
          Tmls(Nmls) = Tmls2(i)
        End If
      Loop Until Zmls1(i) > 40
      Nlev = Nmls
      For i% = 1 To Nmls
        Zplot(i%) = Zmls(i) 'RAp = Zgeo or Zp depending on chkGeoPotHeight.value
        Tplot(i%) = Tmls(i)
      ''Debug.Print i; RAp(i); RTp(i)
      Next i
      NlevrSave = Nlevr
      Nlevr = Nlev
      With frmRAOB
      .ScaleMode = 0
      .EnableFit = True
      .PlotSingleScan = False
      chkMultipleScans.Value = 1
      ' Save RAOB display parameters
      CurrentColor = .LineColor
      CurrentThickness = .LineThickness
      ' Set new display parameters
      .LineColor = frmATP.LineColor
      .LineThickness = frmATP.LineThickness
      .RAOBupdate
      .EnableFit = False     'So RAOB does not try to fit!
      ' Restore RAOB display parameters
      .LineColor = CurrentColor
      .LineThickness = CurrentThickness
      End With
      Nlevr = NlevrSave
      
        Debug.Print iSample; UTmls; Format(Cycle(j), "0000") + vbTab + Format(UTks(j), "00.00") + vbTab + Format(Rsave(j), "000.0")
        ' Interpolate MTP profile to MLS levels
        Call REFread(REFlu%, Cycle(j))
        Call Calculate_MLS_Profile(sZZZ(), sTTT(), Nlev, Zplot(), Tplot(), Nmls)
        NlevrSave = Nlevr
        Nlevr = Nmls
        frmRAOB.ShowDots = True
        frmRAOB.EnableFit = False
        frmRAOB.Tac = Tac
        frmRAOB.ALTkm = pALT
        frmRAOB.ShowAircraft = True
        frmRAOB.RAOBupdate
        DoEvents
        frmRAOB.Line (Tac - 5, pALT)-(Tac + 5, pALT), QBColor(White)
        Nlevr = NlevrSave
  'UpdateRAOBlisting
        frmRAOB.ShowDots = False
        frmRAOB.ScaleMode = 1
        PNGname$ = "C:\MTP\Data\WB57\HAVE2\MLS\MLS" + YYYYMMDD$ + "_" + Format(iSample, "00") + "_" + Format(j, "0") + ".PNG"
        'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        CaptureFormMJ frmRAOB, PNGname$, 1, False
        ' Calculate statistics
        i = 0
        Do:     i = i + 1
        Loop Until Zmls1(i) = Zplot(1)  'First MTP level
        Select Case i
        Case 2:
        X = X
        iMTP = 2: iStart = 4
        Case 3: iMTP = 1: iMLS = 0: iStart = 4
        Case 4: iMTP = 0: iMLS = 0: iStart = 4
        Case 5: iMTP = -1: iMLS = 1: iStart = 5
        
        Case 6:
        X = X
        End Select
        
        For i = iStart To 12
          If Zplot(i) = 0# Then Exit For
          iLvl(i) = iLvl(i) + 1
          X = Tmls(i - 3) - Tplot(i - 3 + iMTP)
          Debug.Print i; Zmls(i - 3); Zplot(i - 3 + iMTP)
          Sum1(i) = Sum1(i) + X         'Sum time difference
          Sum2(i) = Sum2(i) + X ^ 2     'Sum square of time differences
        Next i
      Next j
      
    End If
    End If
    End If
    Close MTPlu

  Loop Until EOF(MLSlu)
  Close MLSlu, REFlu
      
  For i = 4 To 11
    If iLvl(i) > 2 Then
      AVG(i) = Sum1(i) / iLvl(i)            'Calculate average
      If Sum2(i) - AVG(i) ^ 2 * iLvl(i) > 0 Then RMS(i) = Sqr((Sum2(i) - AVG(i) ^ 2 * iLvl(i)) / (iLvl(i) - 1)) Else RMS(i) = 0# 'Calculate RMS
    End If
    Debug.Print i; Zmls1(i); AVG(i); RMS(i); iLvl(i)
  Next i
  
End Sub

Private Sub cmdNlevrMax_Click()
Dim NlevMax%, TotalRec%

  NlevMax = 0
  txtTotalRec.BackColor = &HFF&       'Red while searching
  DoEvents
  Cycle = 0
  TotalRec = 0
  Do
    Cycle = Cycle + 1
    Call RAOBread(RAOBlu, Cycle)
    If Nlevr > NlevMax Then NlevMax = Nlevr
    txtProgress.Text = Cycle
    DoEvents
  Loop Until EOF(RAOBlu) Or Cycle = FileRecords
  cmdNlevrMax.Caption = "NlevMax = " + Format(NlevMax, "000")
End Sub

Private Sub cmdOpen_Click()
If cmdOpen.Caption = "&Open RAOB" Then
  mnuFileOpenBinary_Click
Else
  Close RAOBlu
  RAOBlu = 0
  Call cmdSelect_Click(4)
  cmdOpen.Caption = "&Open RAOB"
  txtProgress.Text = ""
End If
End Sub

Private Sub cmdOpenREFfile_Click()
Dim xx$, Filename$
  
If cmdOpenREFfile.Caption = "Open REF" Then
  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"
' Set Initial directory
  CommonDialog2.InitDir = Drive$ + Rdir$ + Mission$
' Set Size of FileName buffer
  CommonDialog2.MaxFileSize = 1024
' Set filters.
  CommonDialog2.Filter = "REF Data Structure Files (*.REF, *.ERF)|*.??F|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog2.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog2.ShowOpen

  Filename$ = CommonDialog2.Filename

  If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
    Exit Sub
  Else
'  On Error GoTo errhandler
    If REFlu <> 0 Then Close REFlu
  'REFlu% = FreeFile
    REFopen Filename$ 'For Random Access Read Write As REFlu% Len = Len(REF)
    txtREFfile.Text = Filename$
    Select Case FileFormatIn
    Case 32:  REFfileRecords = LOF(REFlu%) \ Len(REF) - HiddenRecords
    Case 33:  REFfileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
    End Select
  
    ProgressBar2.Min = 0
    ProgressBar2.Max = REFfileRecords
    txtREFrecords.Text = REFfileRecords
    REFread REFlu, REFfileRecords  'Get Last Record
    ScanLength = UTsec
    REFread REFlu, 1               'Get First Record
    If UTsec > ScanLength Then ScanLength = ScanLength + 86400
    ScanLength = (ScanLength - UTsec) / (REFfileRecords - 1)
    cmdOpenREFfile.Caption = "Close REF"

  End If
  
  REFreadHEADER Filename$
  'REFreadFLTINFO Filename$

'  Initialize
  tmrGetCycle2.Interval = 100 'Auto Step cycle time in ms
  tmrGetCycle2.Enabled = False
  cmdREFloc(1).Caption = "&Continue"
  Cycle2 = 0
  txtREFrecord.Text = Str(Cycle2)
  frmATP.Visible = True
Else
  cmdOpenREFfile.Caption = "Open REF"
  Close (REFlu)
End If
End Sub

Private Sub cmdRAOBcompare_Click()
Dim MTPpath$, Filename$, YYYYMMDD$, UTsecRAOB&, Record%, i%, j%, jmax%
Dim NlevRAOB%, OUTlu%, X$, z!, Traob!, Tmtp!

  MTPpath$ = txtMTPpath.Text
  OUTlu = FreeFile
  Open "C:\MTP\RAOBcomparison_" + Mission$ + ".txt" For Output As OUTlu

For i = 0 To lstSelect.ListCount - 1
  X$ = lstSelect.List(i)
  Cycle = Right$(X$, 5)
  Call RAOBread(RAOBlu, Cycle)
  NlevRAOB = Nlevr
  If Ihour < 2 Then Iday = Iday - 1: Ihour = Ihour + 24
  YYYYMMDD$ = Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
  UTsecRAOB& = 3600# * Ihour + 60# * Imin + Isec
  Filename$ = MTPpath$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
  REFlu = FreeFile
  Open Filename$ For Random Access Read Write As REFlu Len = Len(REF)
  Record = 0
  
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
  Loop Until (UTsec > UTsecRAOB And GoodScan) Or EOF(REFlu)
  Close REFlu
  Debug.Print X$, Format(pALT, "00.00"),
  Print #OUTlu, X$, Format(pALT, "00.00"),
  For j = Nlev1 To Nlev2
    RAp(j - Nlev1 + 1) = zzz(j) / 100
    RTp(j - Nlev1 + 1) = TTT(j) / 10
  Next j

  For j = 1 To 21      '24 To 44
    z = 0.5 * j
    If z > ZPraob(NlevRAOB) Then Exit For
    Traob = fLinterp(ZPraob(), TZraob(), NlevRAOB, z)
    Tmtp = fLinterp(RAp(), RTp(), Nlev, z)
    'Debug.Print Format(Tmtp - Traob, "00.0"),
    Print #OUTlu, Format(Tmtp - Traob, "00.0"),
  Next j
  jmax = j - 1
  Print #OUTlu, " "
  Debug.Print " "
  DoEvents
Next i
Close OUTlu

End Sub

Private Sub cmdRAOBdetails_Click()
mnuOptionDetails_Click
End Sub

Private Sub cmdRAOBrangeUsed_Click()
Dim n%, A$, INPlu%, OUTlu%, InFile$

  InFile = MNpath$ + "RAOB\" + Mission$ + "_RAOBrangeAll.txt"
  INPlu = FreeFile
  Open InFile For Input As INPlu

  OUTlu = FreeFile
  OUTfile = MNpath$ + "RAOB\" + Mission$ + "_RAOBrangeUsed.txt"
  Open OUTfile For Output As OUTlu
  Input #INPlu, A$
  Print #OUTlu, A$

  Do
    Input #INPlu, A$
    n = InStr(A$, vbTab)
    n = Val(Left$(A$, n - 1))
    If Len(Dir(MNpath$ + "PNG\R" + Format(n, "000") + "*.PNG")) > 0 Then
      Print #OUTlu, A$
    End If
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
End Sub

Private Sub cmdREFloc_Click(Index As Integer)
Select Case Index
Case 0 'Back
  Forward2 = False
  tmrGetCycle2_Timer
Case 1 'Stop
If tmrGetCycle2.Enabled Then
  tmrGetCycle2.Enabled = False
  cmdREFloc(1).Caption = "&Continue"
Else
  tmrGetCycle2.Enabled = True
  cmdREFloc(1).Caption = "&Stop"
End If
Case 2 'Forward
  Forward2 = True
  tmrGetCycle2_Timer
End Select
End Sub

Private Sub cmdRMS_Click()
txtRMS.Text = "3.0"
End Sub

Private Sub cmdRMSwrtFL_Click()
Dim Tsum1!(1 To 60), Tsum2!(1 To 60), n%(1 To 60), i%, j%, CurrentRecord&, i1%, i2%, i0%, zFL!
Dim Tavg!(1 To 60), Trms!(1 To 60), z!(1 To 60), Tr!, Tc!, X$, dT!

  i0 = 37     '18.5 km Flight level
  i1 = 16     ' 8.0 km
  i2 = 58     '29.0 km
  zFL = 18.5  'FL
  For j = 1 To 60
    Tsum1(j) = 0#: Tsum2(j) = 0#: n(j) = 0: z(j) = 0.5 * j
  Next j
  
  For i = 1 To lstSelect.ListCount  'First time thru calculate avg T profile
    lstSelect.ListIndex = i - 1
    X$ = lstSelect.List(lstSelect.ListIndex)
    CurrentRecord = Val(Right$(X$, 5))
    Call RAOBread(RAOBlu, CurrentRecord)
    
    Tr = fLinterp(ZPraob(), TZraob(), Nlevr, zFL)
    For j = i1 To i2
      If z(j) > ZPraob(Nlevr) Then Exit For
      Tc = fLinterp(ZPraob(), TZraob(), Nlevr, z(j))
      n(j) = n(j) + 1
      Tsum1(j) = Tsum1(j) + Tc
    Next j
  Next i
  For j = i1 To i2
   Tavg(j) = Tsum1(j) / n(j)             'Calculate average profile
  Next j
  For j = 1 To 60
    Tsum1(j) = 0#: Tsum2(j) = 0#: n(j) = 0: z(j) = 0.5 * j
  Next j
  
  For i = 1 To lstSelect.ListCount
    lstSelect.ListIndex = i - 1
    X$ = lstSelect.List(lstSelect.ListIndex)
    CurrentRecord = Val(Right$(X$, 5))
    lblProgress.Caption = "Record"
    txtProgress.Text = Str$(CurrentRecord)
    Call RAOBread(RAOBlu, CurrentRecord)
    
    Tr = fLinterp(ZPraob(), TZraob(), Nlevr, zFL) - Tavg(i0) 'Offset at FL
    For j = i1 To i2      '24 To 44
      If z(j) > ZPraob(Nlevr) Then Exit For
      Tc = fLinterp(ZPraob(), TZraob(), Nlevr, z(j))
      dT = Tc - Tr - Tavg(j)
      n(j) = n(j) + 1
      Tsum1(j) = Tsum1(j) + dT
      Tsum2(j) = Tsum2(j) + dT ^ 2
    Next j
  Next i
  
  For j = i1 To i2
   Tavg(j) = Tsum1(j) / n(j)             'Calculate average
   Trms(j) = Sqr((Tsum2(j) - Tavg(j) ^ 2) / (n(j) - 1))     'Calculate RMS
   Debug.Print j; z(j); Tavg(j); Trms(j); n(j)
  Next j
  
End Sub

Private Sub cmdSelect_Click(Index As Integer)
Dim X$, V!(1 To 8), OutRecord&, OUTlu%, ans, SaveCycle&, i&, j%, k%, jj%, jjj%, lu%, lu2%
Dim TotalRec&, A$, hh$, mm$, xx!, Cycle&, Zp!, Ti!(1 To 300), T1Sum!(1 To 300), T2Sum!(1 To 300)
Dim RAOBsite$, n%, z!, NDstd!, Arg!, CurrentColor%, CurrentThickness%, Nmls%
Dim RecordList&(), Tr!, Tc!, Tsum1!, Tsum2!, Tavg!, Trms!, NlevrSave%, DTevent As Date
Dim RMS!, AVG!, kmax&, i1%, i2%, Noise!, Nz%(1 To 300), Status As Boolean, Remove As Boolean
Dim LR1!, LR2!, Zb!, ExtNoise!, Filename$, RAOBfilename$, RAOBfilename1$, RAOBfilename2$
Dim iNum%, iY%, iM%, iD%, UT0!, UTraob!, LR11!, LR12!, Zb1!, LR21!, LR22!, Zb2!, Record&, kSave%, dT!, alpha!
Dim FlightRAOBfile$, FRlu%, FRrecord&, Rlu%, UT1!, UT2!, DT1 As Date, DT2 As Date, RAOB1$
Dim Rec1&, Rec2&, DTtemp As Date, WMOnumberTemp&, iCycle&, Ext$, Tbias!

Static Ri!(1 To 300), RAOBcount%, ZPplot!(1 To 300), ZGplot!(1 To 300), RZplot!(1 To 300)
Static SortCycle&(), SortDandT() As Date, SortCount%, EventDT As Date, NDmj!, y$, NL%, f!
Static PlotAll As Boolean, DnT1 As Date, DnT2 As Date, Nref%, Zref!(1 To 300), Tref!(1 To 300)
Static FRinitialized As Boolean, CMD06 As Boolean
Static Found1 As Boolean, Found2 As Boolean


Select Case Index
Case 0  'Search
  If lstSelect.ListCount > 0 Then
    MsgBox "Clear List Box before performing a search!", vbOKOnly
  Else
  If RAOBlu = 0 Then
    MsgBox "You must open a RAOB file before using this command!", vbOKOnly
    Exit Sub
  End If
  SaveCycle = Cycle
  txtTotalRec.BackColor = &HFF&       'Red while searching
  DoEvents
  Cycle = 0
  TotalRec = 0
  ReDim SortCycle&(1 To Val(txtFileRec.Text)), SortDandT(1 To Val(txtFileRec.Text))
  Do
    Cycle = Cycle + 1

    Call RAOBread(RAOBlu, Cycle)
    
    If (fFilter And chkNegateFilter.Value = 0) Or (Not fFilter And chkNegateFilter.Value = 1) Then 'Make sure filter criteria are satisfied
      'Debug.Print WMOnumber, WMO4l
      TotalRec = TotalRec + 1
      V(1) = WMOnumber: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihhmm
'                                12345678901234567890123456789
      
      If Val(WMO4l) <> 0 Then X$ = "    " Else X$ = WMO4l
      X$ = X$ + " " + fUsing("#0000 #000 #0 #0 #000", V())
      X$ = X$ + " " + Format$(ijkl, "0") + "   " + Format$(Cycle, "00000")
      lstSelect.AddItem X$
      'Debug.Print x$
      i = lstSelect.ListCount
      SortDandT(i) = DandT
      SortCycle(i) = Cycle
      'Debug.Print x$ + " " + Format(3.6 * Ihour + 0.06 * Imin, "00.000")
      txtTotalRec.Text = Str$(TotalRec)
      DoEvents
    End If
  Loop Until EOF(RAOBlu) Or Cycle = FileRecords
  Cycle = SaveCycle
  RAOBsite$ = Str(WMOnumber)  'save last search site
  txtTotalRec.BackColor = &HFF00&           'Green when finished searching
  If chkAutoSort.Value = 1 Then cmdSelect_Click (12)
  End If
  
  Case 1  'Write
    cmdSelect(1).Caption = "Busy!"
    Tbias = Val(txtAVG.Text)
    If lstSelect.ListCount > 0 Then
    If chkAvgWrite.Value = 0 Then
      ' First check if new file, and delete it if it exists
      If optOutput(0).Value Then  'Append or New file
        A$ = Dir(txtConvertOut.Text, vbNormal)
        If A$ <> "" Then
          ans = MsgBox(txtConvertOut.Text + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
          If ans = vbYes Then Kill (txtConvertOut.Text) Else Exit Sub
          'Exit Sub
        End If
      End If

    ' Assign output RAOB file
      'RAOBtype = 0
      OUTlu = FreeFile
      Ext$ = fExtension(txtConvertOut.Text)
      Select Case Ext$
      Case "RAOB2"
        Open txtConvertOut.Text For Random Access Read Write As OUTlu Len = Len(RAOB2)
        If optOutput(1).Value Then    ' Append to existing file
          OutRecord = LOF(OUTlu%) \ Len(RAOB2)
        Else                          ' Overwrite file if it exists
          OutRecord = 0
        End If
      Case "RAOB3"
        Open txtConvertOut.Text For Random Access Read Write As OUTlu Len = Len(RAOB3)
        If optOutput(1).Value Then    ' Append to existing file
          OutRecord = LOF(OUTlu%) \ Len(RAOB3)
        Else                          ' Overwrite file if it exists
          OutRecord = 0
        End If
      End Select
      For i = 0 To lstSelect.ListCount - 1
        X$ = lstSelect.List(i)
        Cycle = Right$(X$, 5)
    
        Call RAOBread(RAOBlu, Cycle)
'        If chkEnableRAOBextension.Value = 1 Then
'          LR1 = Val(txtLR1.Text) + fSDT(Val(txtExtensionNoise.Text))
'          LR2 = Val(txtLR2.Text) + fSDT(Val(txtExtensionNoise.Text))
'          Zb = Val(txtZb.Text) + fSDT(Val(txtExtensionNoise.Text))
'          Call RAOBextension(TZraob(), ZPraob(), Nlevr, LR1, Zb, LR2, RAOBtop)
'        End If
        If chkAddBias.Value = 1 Then   'Add a temperature bias to all soundings being written
          For j = 1 To Nlevr
            TZraob(j) = TZraob(j) + Tbias
          Next j
        End If
        ijkl = Mid$(X$, 28, 1)  'Only parameter changed
        OutRecord = OutRecord + 1
        Call RAOBwrite(OUTlu, OutRecord)
      Next i
      X = X
    Else   'Write avgerage RAOB
'      If Len(Dir(txtExport.Text)) = 0 Then
'        MsgBox "You need to specify an export filename!" + vbCr + "See EXPORT Tab!", vbOKOnly
'        Exit Sub
'      End If
      OUTlu = FreeFile
      Open txtExportPath.Text For Output As OUTlu
      Select Case cboAvgType.Text
      Case "Trop Stats"
        Print #OUTlu, Trim(WMO4l) + vbTab + Trim(Str(WMOnumber))
        Print #OUTlu, Str(WMOlatitude) + vbTab + Str(WMOlongitude)
        Print #OUTlu, Str(RAOBcount) + vbTab + txtTropMin.Text + vbTab + txtTropMax.Text
        Print #OUTlu, "pALT" + vbTab + "T(z)" + vbTab + "Trms(z)"
        For i = 1 To Nlevr
          Print #OUTlu, Format(Zplot(i), "00.0") + vbTab + Format(Tplot(i), "000.0") + vbTab + Format(Ri(i), "#0.0")
        Next i
      Case "ND Profile"
        Print #OUTlu, "Avg RAOB Profile and Number Density Using:"
        Print #OUTlu, "ICAO" + vbTab + "WMOnumber" + vbTab + "Year" + vbTab + "Month" + vbTab + "Day" + vbTab + "UThr" + vbTab + "Trop[km]"
        For i = 0 To lstSelect.ListCount - 1
          X$ = lstSelect.List(i)
          Cycle = Right$(X$, 5)
          Call RAOBread(RAOBlu, Cycle)
          X$ = Trim(WMO4l) + vbTab + Trim(Str(WMOnumber)) + vbTab
          X$ = X$ + Format(Iyear, "0000") + vbTab + Format(Imonth, "00") + vbTab + Format(Iday, "00") + vbTab + Format(Ihour, "00")
          X$ = X$ + vbTab + Format(Ztrop, "#0.00")
          Print #OUTlu, X$
        Next i
        Nlevr = Val(txtAvgOpt(0).Text) / Val(txtAvgOpt(1).Text) + 1
        Print #OUTlu, "T[K]" + vbTab + "pALT[km]" + vbTab + "gALT[km]" + vbTab + "Trms[K]" + vbTab + "ND[#/cm3 E21]" + vbTab + "dND[%]"
        For i = 1 To Nlevr
          X$ = Format(Tplot(i), "#00.0") + vbTab + Format(ZPplot(i), "#0.00") + vbTab
          X$ = X$ + Format(ZGplot(i), "#0.00") + vbTab + Format(Ri(i), "#0.0") + vbTab
          NDstd = fStandardDensity(Imonth, ZGplot(i), WMOlatitude) * cAo / cMd / 1E+21
          NDmj = fNDmj(RZplot(i), fZtoP(Zplot(i)), Tplot(i)) / 1E+21 '#/cm3 /E21
          X$ = X$ + Format(NDmj, "#0000") + vbTab + Format(100 * (NDmj - NDstd) / NDstd, "#0.0")
          Print #OUTlu, X$
        Next i
        X = X
        
      Case "MP for DIAL"
        Print #OUTlu, "Avg RAOB Profile and Number Density Using:"
        Print #OUTlu, "ICAO WMOnum  Year Month Day UT Trop[km]"
                       X$ = " 0000    00  00 0000    #0.00"
        For i = 0 To 1  'lstSelect.ListCount - 1
          y$ = lstSelect.List(i)
          Cycle = Right$(y$, 5)
          Call RAOBread(RAOBlu, Cycle)
          X$ = Trim(WMO4l) + "  " + Str(WMOnumber) + " " + Str(Iyear) + "    " + Format(Imonth, "00") + "  " + Format(Iday, "00") + " " + Format(Ihour, "00") + " " + Format(Ztrop, "#0.00")
          Print #OUTlu, X$
        Next i
        Print #OUTlu, "T [K], pALT [km], Trms [K], gALT [km], RH[%]"
        Nlevr = Val(txtAvgOpt(0).Text) / Val(txtAvgOpt(1).Text) + 1
        Print #OUTlu, "    T  pALT Trms  gALT    ND  dND  RH"
                 X$ = "#00.00  #0.00  #0.0  #0.00 ##0.0"   '#0000 #000"
        For i = 1 To Nlevr
          'Debug.Print i
          V(2) = ZPplot(i): V(1) = Tplot(i)
          V(3) = Ri(i): V(4) = ZGplot(i): V(5) = RZplot(i)
'          NDstd = fStandardDensity(Imonth, ZGplot(i), WMOlatitude) * cAo / cMd / 1E+21
'          NDmj = fNDmj(RZplot(i), fZtoP(Zplot(i)), Tplot(i)) / 1E+21 '#/cm3 /E21
'          v(5) = Int(NDmj): v(6) = Int(1000 * (NDmj - NDstd) / NDstd)
          Print #OUTlu, fUsing(X$, V())
        Next i
 
      End Select
      
    End If
    Close OUTlu
    Else
      MsgBox "Nothing was Written" + vbCrLf + "Because the selection list is empty!", vbOKOnly
    End If
    cmdSelect(1).Caption = "&Write"

Case 2  'Remove or Keep
  If cmdSelect(2).Caption = "&Remove" Then Remove = True Else Remove = False
  Call ListBoxRemoveItem(lstSelect, Remove)
  
'  Select Case lstSelect.SelCount
'  Case Is < 1                 'Empty list
'  Case 1                      'Remove single item
'    i = lstSelect.ListIndex
'    lstSelect.RemoveItem (lstSelect.ListIndex)
'    If i >= 0 And i < lstSelect.ListCount - 1 Then
'      lstSelect.Selected(i) = True
'    Else
'      If lstSelect.ListCount > 0 Then lstSelect.Selected(lstSelect.ListCount - 1) = True
'    End If
'  Case Is > 1                                          'Keep or Remove multiple items
'    i = -1
'    If cmdSelect(2).Caption = "&Remove" Then
'      Do
'        i = i + 1
'        If lstSelect.Selected(i) Then lstSelect.RemoveItem (i): i = i - 1
'      Loop Until i = lstSelect.ListCount - 1
'    Else
'      Do
'        i = i + 1
'        If Not lstSelect.Selected(i) Then lstSelect.RemoveItem (i): i = i - 1
'      Loop Until i = lstSelect.ListCount - 1
'    End If
'  End Select
  TotalRec = Val(lstSelect.ListCount)
  txtTotalRec.Text = Str(TotalRec)
  lstSelect.SetFocus
'  lstSelect_Click
  
Case 3  'View
  If Not CMD14 Then 'Don't read or view if doing RMS statistics
    If RAOBlu = 0 Then
      MsgBox "You must open a RAOB file before using this command!", vbOKOnly
      Exit Sub
    End If

    X$ = lstSelect.List(lstSelect.ListIndex)
    Cycle = Val(Right$(X$, 5))

    Call RAOBread(RAOBlu, CurrentRecord)
  
    DetermineROABtoShow  'Load Zplot and Tplot
    If PlotAll Then
      For i = 1 To Nlevr
        Tplot(i) = TZraob(i) + Val(cboTincrement.Text) * lstSelect.ListIndex
      Next i
    End If
    frmRAOB.EnableFit = False
    UpdateDisplay
    UpdateRAOBlisting
  End If
  
Case 4  'Clear List
  lstSelect.Clear
  txtTotalRec.Text = ""
  
Case 5  'Clear Plot
  With frmRAOB
    .EnableFit = False
    .SetDefaultScales
    .RAOB_Plot_Set
    .RAOB_Plot_Axes_Labels
    .RAOB_Plot_Grid_Marks
    .RAOB_Plot_Tic_Marks
  End With
  Documentation$ = ""
  
Case 6  'Automatically create template RAOB files for all RAOBs in Mission_RAOBcomparisonUnique.txt file
  lu = FreeFile
  Open RSpath$ + Mission$ + "_RAOBrangeUnique.txt" For Input As lu
  Input #lu, A$     'Read header record
  CMD06 = True
  Do
    Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
'      Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; A$
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    cmdSelect_Click (4)                               'Clear Select list with Clear List button
    cmdSite_Click (2)                                 'Clear Site list box
    txtRSsite.Text = RAOB1$
    cmdSite_Click (0)                                 'Add sounding to Site list
    If Not EnableFilter(1) Then txtFilter_Click (1)   'Turn on Site filter
    cmdSelect_Click (0)                               'Reload Select list with Search button
' Now have list for this site, now find needed RAOB
    For i = 1 To lstSelect.ListCount
      lstSelect.ListIndex = i - 1
      X$ = lstSelect.List(lstSelect.ListIndex)
      CurrentRecord = Val(Right$(X$, 5))
      lblProgress.Caption = "Record"
      txtProgress.Text = Str$(CurrentRecord)
      Call RAOBread(RAOBlu, CurrentRecord)
      If Iyear = iY And Imonth = iM And iD = Iday Then
        UTraob = (Ihour * 3600# + Imin * 60#) / 1000#
        Select Case UT0
        Case Is < 43.2
          If UTraob < 43.2 Then Exit For
        Case Else
          If UTraob >= 43.2 Then Exit For
        End Select
      End If
    Next i
    lstSelect.ListIndex = i - 1
    cmdSelect_Click (18)

  Loop Until EOF(lu)
  Close lu
  CMD06 = False
'Clear IJKL flags
'  If RAOBlu = 0 Then
'    MsgBox "You must open a RAOB file before using this command!", vbOKOnly
'    Exit Sub
'  End If'
'
'  For i = 0 To lstSelect.ListCount - 1
'    x$ = lstSelect.List(i)
'    Cycle = Right$(x$, 5)
'    Mid$(x$, 28, 1) = "0"
'    lstSelect.List(i) = x$
'    Call RAOBread(RAOBlu, Cycle)
'    ijkl = 0                    'Only parameter changed
'    Call RAOBwrite(RAOBlu, Cycle)
'  Next i
  
Case 7   'Auto T Edit and Fix (including new trop search)
  Call cmdList_Click(5)
  
Case 8   'Time 1
'      v(1) = WMOnumber: v(2) = Iyear: v(3) = Imonth: v(4) = Iday
'      v(5) = Ihhmm
'                            12345678901234567890123456789
'      x$ = WMO4l + " " + fUsing("#0000 #000 #0 #0 #000", v())
'      x$ = x$ + " " + Format$(ijkl, "0") + "   " + Format$(Cycle, "00000")
'DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)
  X$ = lstSelect.List(lstSelect.ListIndex)
  hh$ = Trim(Mid$(X$, 23, 2)): mm$ = Mid$(X$, 25, 2)
  If Len(hh$) = 1 Then hh$ = "0" + hh$
  TimeSec = 3600# * Val(hh$) + 60# * Val(mm$)
  With frmRAOBsites
    .optShow(0).Value = True
    .txtShow(0).Text = WMO4l
'    .cmdDetails_Click (0)
  End With
'  txtStation(1).Text = WMO4l
'  cmdDetails_Click (1)
  
  If Site1 Then
    txtSite(0).Text = WMO4l
    txtDate1(0).Text = DateSerial(Mid$(X$, 12, 4), Mid$(X$, 17, 2), Mid$(X$, 20, 2))
    txtDate1(0).Text = txtDate1(0).Text + " " + hh$ + ":" + mm$
    txtRec1(0).Text = Val(Right$(X$, 5))
    txtLATn(0).Text = Format$(WMOlatitude, "##0.000")
    txtLONn(0).Text = Format$(WMOlongitude, "##0.000")
    DnT1 = txtDate1(0).Text
  Else
    txtSite(1).Text = WMO4l
    txtDate1(1).Text = DateSerial(Mid$(X$, 12, 4), Mid$(X$, 17, 2), Mid$(X$, 20, 2))
    txtDate1(1).Text = txtDate1(1).Text + " " + hh$ + ":" + mm$
    txtRec1(1).Text = Val(Right$(X$, 5))
    txtLATn(1).Text = Format$(WMOlatitude, "##0.000")
    txtLONn(1).Text = Format$(WMOlongitude, "##0.000")
    DnT1 = txtDate1(1).Text
  End If
  ' Save yyyy MM DD of Event as date of first RAOB
  ' Add code later in case Event is >86400 sec
  IyearE = Iyear
  IdoyE = Idoy
  ImonthE = Imonth
  IdayE = Iday
  
Case 9   'Time 2
  X$ = lstSelect.List(lstSelect.ListIndex)
  hh$ = Trim(Mid$(X$, 23, 2)): mm$ = Mid$(X$, 25, 2)
  If Len(hh$) = 1 Then hh$ = "0" + hh$
  TimeSec = 3600# * Val(hh$) + 60# * Val(mm$)
  If Site1 Then
    txtDate2(0).Text = DateSerial(Mid$(X$, 12, 4), Mid$(X$, 17, 2), Mid$(X$, 20, 2))
    txtDate2(0).Text = txtDate2(0).Text + " " + hh$ + ":" + mm$
    DnT2 = txtDate2(0).Text
    EventDT = txtEvent.Text
    If DnT2 - DnT1 <> 0 Then
      xx = 1 - (EventDT - DnT1) / (DnT2 - DnT1)
    Else
      xx = 1#
    End If
    txtFraction(0).Text = Format(xx, "0.00")
    txtRec2(0).Text = Val(Right$(X$, 5))
  Else
    txtDate2(1).Text = DateSerial(Mid$(X$, 12, 4), Mid$(X$, 17, 2), Mid$(X$, 20, 2))
    txtDate2(1).Text = txtDate2(1).Text + " " + hh$ + ":" + mm$
    DnT2 = txtDate2(1).Text
    EventDT = txtEvent.Text
    If DnT2 - DnT1 <> 0 Then
      xx = 1 - (EventDT - DnT1) / (DnT2 - DnT1)
    Else
      xx = 1#
    End If
    txtFraction(1).Text = Format(xx, "0.00")
    txtRec2(1).Text = Val(Right$(X$, 5))
  End If

Case 10
  DefaultBG = Not DefaultBG
  If DefaultBG Then
' And assign to some attributes to facilitate changing
  With frmRAOB
    .LRL = White               'Lapse Rate lines
    .LRP = LGray               'Lapse Rate pointers
    .ACP = White               'Aircraft pointer
    .ACS = White               'Aircraft symbol
    .PBG = Blue                'Plot Background color
    .PBC = Black               'Plot border color
    .PFG = Yellow              'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Yellow              'Plot tic marks and border
    .Pgm = Yellow              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
  End With
  Else
  With frmRAOB
    .LRL = White               'Lapse Rate lines
    .LRP = Black 'BGray               'Lapse Rate pointers
    .ACP = LBlue               'Aircraft pointer
    .ACS = LBlue               'Aircraft symbol
    .PBG = LGray 'White                'Plot Background color
    .PBC = bGray 'White               'Plot border color
    .PFG = White              'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Black              'Plot tic marks and border
    .Pgm = Black 'LGray              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
  End With
  End If

Case 11 'Average selected RAOBs
  n = Val(lstSelect.ListCount)
  If n = 0 Then Exit Sub
  For j = 1 To 300
    T1Sum(j) = 0#: T2Sum(j) = 0#
    ZGplot(j) = 0#: RZplot(j) = 0#: ZPplot(j) = 0#
  Next j
  For i = 0 To n - 1
    X$ = lstSelect.List(i)
    lblProgress.Caption = "Record"
    'If AutoView Then cmdSelect_Click (3)
    Cycle = Val(Right$(X$, 5))
    txtProgress.Text = Str$(Cycle)
    Call RAOBread(RAOBlu, Cycle)
    k = 2
    NL = Val(txtAvgOpt(0).Text) / Val(txtAvgOpt(1).Text)
    For j = 1 To NL    'averaging pressure altitudes
      If j = 1 Then z = 0.1 Else z = (j - 1#) * Val(txtAvgOpt(1).Text)
      ZPplot(j) = z
      If ZPraob(Nlevr) > z Then
        Nz(j) = Nz(j) + 1
        If ZPraob(k) < z Then Do: k = k + 1: Loop Until ZPraob(k) > z
        Ti(j) = fTinterpP(k, z)
        ' Now calculate geopotential altitude at this pALT
        If (ZPraob(k) - ZPraob(k - 1)) <> 0 Then
          f = (z - ZPraob(k - 1)) / (ZPraob(k) - ZPraob(k - 1))
        Else
          f = 0#
        End If
        ZGplot(j) = ZGplot(j) + ZGraob(k - 1) + f * (ZGraob(k) - ZGraob(k - 1))
        RZplot(j) = RZplot(j) + RZraob(k - 1) + f * (RZraob(k) - RZraob(k - 1))
        T1Sum(j) = T1Sum(j) + Ti(j)
        T2Sum(j) = T2Sum(j) + Ti(j) ^ 2     'Sum square of temperature differences
      Else
      X = X
      End If
    Next j
'    Debug.Print i + 1; TI(3); T1sum(3); T2sum(3); T1sum(3) / (i + 1)
'    If i > 1 Then
'      Debug.Print Sqr(((i + 1) * T2sum(3) - T1sum(3) ^ 2) / ((i + 1) * i))
'    End If
  Next i
' Take average and calculate T statistics
  For j = 1 To NL
    If Nz(j) > 0 Then
      ZGplot(j) = ZGplot(j) / Nz(j)
      RZplot(j) = RZplot(j) / Nz(j)
      Tplot(j) = T1Sum(j) / Nz(j)   'Calculate average
      Arg = T2Sum(j) - Nz(j) * Tplot(j) ^ 2
      If Arg >= 0# And Nz(j) > 1 Then
        Ri(j) = Sqr(Arg / (Nz(j) - 1))  'Calculate RMS
      Else
        Ri(j) = 0#
      End If
      Debug.Print j; ZPplot(j); Tplot(j); Ri(j); Nz(j)
      If chkGeoPotHeight(0).Value = 1 Then Zplot(j) = ZGplot(j) Else Zplot(j) = ZPplot(j)
    Else
      NL = NL - 1
    End If
  Next j
  Nlevr = NL
  With frmRAOB
    CurrentColor = .LineColor
    CurrentThickness = .LineThickness
    .LineColor = White
    .LineThickness = 2
    .RAOBupdate
    .LineColor = CurrentColor
    .LineThickness = CurrentThickness
  End With
  
  RAOBcount = Val(lstSelect.ListCount)
  'UpdateRAOBlisting
Case 12
  'Do Bubble sort
  For j = 2 To lstSelect.ListCount
    DandT = SortDandT(j)
    Cycle = SortCycle(j)
    For i = j - 1 To 1 Step -1
      If SortDandT(i) <= DandT Then GoTo Insert
      SortDandT(i + 1) = SortDandT(i)
      SortCycle(i + 1) = SortCycle(i)
    Next i
    i = 0
Insert:
   SortDandT(i + 1) = DandT
   SortCycle(i + 1) = Cycle
  Next j
  SortCount = lstSelect.ListCount
  lstSelect.Clear
  For i = 1 To SortCount
    Cycle = SortCycle(i)
    Call RAOBread(RAOBlu, Cycle)
    V(1) = WMOnumber: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihhmm
    If Val(WMO4l) <> 0 Then X$ = "    " Else X$ = WMO4l
    X$ = X$ + " " + fUsing("#0000 #000 #0 #0 #000", V())
    X$ = X$ + " " + Format$(ijkl, "0") + "   " + Format$(Cycle, "00000")
    lstSelect.AddItem X$
  Next i

Case 13  'Plot all selected soundings
  cmdSelect_Click (5)
  'chkMultipleScans.value = 1
  PlotAll = True
  For i = 1 To lstSelect.ListCount
    lstSelect.ListIndex = i - 1
    X$ = lstSelect.List(lstSelect.ListIndex)
    CurrentRecord = Val(Right$(X$, 5))
    lblProgress.Caption = "Record"
    txtProgress.Text = Str$(CurrentRecord)
    If AutoView Then cmdSelect_Click (3)
  Next i
  PlotAll = False
  
Case 14  'Select RAOBs from Select List that differ from select RAOB by <than specified RMS
  ' Save reference RAOB (the last selected)
  If Nref = 0 Then
    MsgBox "You must save the reference profile first!"
    Exit Sub
  End If
  CMD14 = True
  i2 = Int(Val(txtRMSrange(2).Text) / Val(txtRMSrange(1).Text))
  i1 = Int(Val(txtRMSrange(0).Text) / Val(txtRMSrange(1).Text))
  Filename$ = MNpath$ + "RAOB\Templates\" + RMSfilename$         'RMSfilename$ set in lstSelect_Click() when Ref Profile is selected
  
  If chkEnableRAOBextension.Value = 1 Then
    LR1 = Val(txtLR1.Text)
    LR2 = Val(txtLR2.Text)
    Zb = Val(txtZb.Text)
    Call RAOBextension(Tref(), Zref(), Nref, LR1, Zb, LR2, RAOBtop)
  Else
    MsgBox "You must enable RAOB extension for this function to work properly!", vbOKOnly
    Exit Sub
  End If
  
  If CMD06 Then
    txtAVG.Text = "2"
    txtRMS.Text = "5"
    TotalRec = Val(txtTotalRec.Text)
    Do
      AVG = Val(txtAVG.Text)
      RMS = Val(txtRMS.Text)
      GoSub Calculate_RMS
      Select Case TotalRec
      Case Is < 250: txtRMS.Text = Format(RMS - 0.05, "0.00")
      Case Is < 300: txtRMS.Text = Format(RMS - 0.1, "0.00")
      Case Is < 400: txtRMS.Text = Format(RMS - 0.2, "0.00")
      Case Is < 500: txtRMS.Text = Format(RMS - 0.3, "0.00")
      Case Else:     txtRMS.Text = Format(RMS - 0.5, "0.00")
      End Select
    Loop Until TotalRec < 200
  Else
    AVG = Val(txtAVG.Text)
    RMS = Val(txtRMS.Text)
    GoSub Calculate_RMS
  End If
  CMD14 = False
  
Case 15  'Calculate Day-to-Day statistics
  For i = 1 To lstSelect.ListCount
    lstSelect.ListIndex = i - 1
    X$ = lstSelect.List(lstSelect.ListIndex)
    CurrentRecord = Val(Right$(X$, 5))
    lblProgress.Caption = "Record"
    txtProgress.Text = Str$(CurrentRecord)
    Call RAOBread(RAOBlu, CurrentRecord)
    Tsum1 = 0#: Tsum2 = 0#: n = 0
    i1 = Int(Val(txtAvgOpt(2).Text) / Val(txtAvgOpt(1).Text))
    i2 = Int(Val(txtAvgOpt(0).Text) / Val(txtAvgOpt(1).Text))
    For j = i1 To i2      '24 To 44
      z = 0.5 * j
      If z > Zref(Nref) Then Exit For
      Tr = fLinterp(Zref(), Tref(), Nref, z)
      Tc = fLinterp(ZPraob(), TZraob(), Nlevr, z)
      X = Tc - Tr
      n = n + 1
      Tsum1 = Tsum1 + X
      Tsum2 = Tsum2 + X ^ 2
'      Debug.Print Format(Tmtp - Traob, "00.0"),
    Next j
    Tavg = Tsum1 / n                              'Calculate average
    Trms = Sqr((Tsum2 - Tavg ^ 2) / (n - 1))      'Calculate RMS
    Debug.Print i; Tavg; Trms
    If Trms < RMS And Abs(Tavg) < AVG Then
      kmax = kmax + 1
      RecordList(kmax) = CurrentRecord
    End If
  Next i

Case 16
  frmRAOB.Documentation = Documentation$
  frmRAOB.RAOB_Plot_Documentation2

Case 17  ' Use selected sounding to generate 150 additional soundings with specified Noise parameters
  X$ = lstSelect.List(lstSelect.ListIndex)
  Cycle = Val(Right$(X$, 5))
  lstSelect.Clear
  Call RAOBread(RAOBlu, Cycle)
  For j = 1 To Nlevr    'averaging pressure altitudes
    Tref(j) = TZraob(j)
    Zref(j) = ZPraob(j)
  Next j
  Noise = 1.5
  RAOBtype = 0
  OUTlu = FreeFile
  Open RSpath$ + "RAOBrandom.RAOB2" For Random Access Read Write As OUTlu Len = Len(RAOB2)
  Call RAOBwrite(OUTlu, 1)  'Write template
  LR1 = Val(txtLR1.Text) + fSDT(Val(txtExtensionNoise.Text))
  LR2 = Val(txtLR2.Text) + fSDT(Val(txtExtensionNoise.Text))
  Zb = Val(txtZb.Text) + fSDT(Val(txtExtensionNoise.Text))
  For i = 2 To 150
    Cycle = i
    Isec = Isec + 1
    If Isec > 59 Then Isec = 0: Imin = Imin + 1
    dT = fSDT(Noise)
    alpha = 0# 'fSDT(0.1)
    jjj = 0  'Int(fSDT(2))
    For j = 1 To Nlevr    'averaging pressure altitudes
      jj = j + jjj
      If jj < 1 Then jj = 1
      If jj > Nlevr Then jj = Nlevr
      TZraob(j) = Tref(jj) + fSDT(Noise)     'dT + alpha * Zref(j)
    Next j
    Call RAOBwrite(OUTlu, Cycle)
    DoEvents
  Next i

Case 18  'Save Reference Profile
  For j = 1 To Nlevr    'averaging pressure altitudes
    Tref(j) = TZraob(j)
    Zref(j) = ZPraob(j)
  Next j
  Nref = Nlevr
' Open MissionRAOBrange.txt file and set LR extension parameters

    If Not fGetLRextension(RMSfilename$, LR11!, LR12!, Zb1!) Then
      MsgBox "The selected sounding is not in the " + Mission$ + "_RAOBrangeUnique.txt file." + vbCrLf + "You will need to manually enter LR information.", vbOKOnly
    Else
      chkEnableRAOBextension.Value = 1  'Force lapse rate extension
      txtLR1.Text = Str(LR11)
      txtLR2.Text = Str(LR12)
      txtZb.Text = Str(Zb1)
    End If
    ' Since a filter is normally used to select a particular site for the Ref sounding, deselect the filter and reload all soundings
    If EnableFilter(1) Then txtFilter_Click (1)  'Turn off Site filter
    cmdSite_Click (2)                            'Clear Site list box
    cmdSelect_Click (4)                          'Clear Select list with Clear List button
    cmdSelect_Click (0)                          'Reload Select list with Search button
    If CMD06 Then cmdSelect_Click (14)
    
Case 19   'Set RAOBgood flag for all soundings in file
  Cycle = 0
  Do
    Cycle = Cycle + 1
    txtProgress.Text = Str$(Cycle)
    Call RAOBread(RAOBlu, CurrentRecord)
    RAOBgood = True
    Call RAOBwrite(RAOBlu, CurrentRecord)
  Loop Until Cycle = FileRecords

Case 20   'Check and fix ICAO name if it is XXXX
  Cycle = 0
  Do
    Cycle = Cycle + 1
    txtProgress.Text = Str$(Cycle)
    Call RAOBread(RAOBlu, Cycle)
    Status = fGetUAsite(Str(WMOnumber), Record)
    If Status And Left$(GPicao, 4) <> "    " Then WMO4l = Trim$(Left$(GPicao, 4)) Else WMO4l = "XXXX"
    Call RAOBwrite(RAOBlu, Cycle)
  Loop Until Cycle = FileRecords

Case 21    'Extend last RAOB to 50 km using info on Details tab
  LR1 = Val(txtLR1.Text) + fSDT(Val(txtExtensionNoise.Text))
  LR2 = Val(txtLR2.Text) + fSDT(Val(txtExtensionNoise.Text))
  Zb = Val(txtZb.Text) + fSDT(Val(txtExtensionNoise.Text))
  Call RAOBextension(TZraob(), ZPraob(), Nlevr, LR1, Zb, LR2, 50#)
  For i = 1 To Nlevr
    Tplot(i) = TZraob(i)
    Zplot(i) = ZPraob(i)
  Next i
  frmRAOB.EnableFit = False
  UpdateDisplay
  UpdateRAOBlisting
  
Case 22    'Show MLS profile for current RAOB
  Call Calculate_MLS_Profile(ZPraob(), TZraob(), Nlevr, Zplot(), Tplot(), Nmls)
  NlevrSave = Nlevr
  Nlevr = Nmls
  frmRAOB.ShowDots = True
  frmRAOB.EnableFit = False
  UpdateDisplay
  Nlevr = NlevrSave
  'UpdateRAOBlisting
  frmRAOB.ShowDots = False
  
Case 23    'Write selected soundings to template file C:\MTP\Data\AC\Mission\Mission_RAOBs.RAOB2
  FlightRAOBfile$ = MNpath$ + Mission$ + "_RAOBs.RAOB2"
  If Not FRinitialized Then
    If Dir(FlightRAOBfile$) <> "" Then
      ans = MsgBox(FlightRAOBfile$ + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
      If ans = vbYes Then Kill (FlightRAOBfile$)
      FRlu = FreeFile
      Close FRlu
    End If
    FRinitialized = True
  End If
  FRlu = FreeFile
  Open FlightRAOBfile$ For Random Access Read Write As FRlu Len = Len(RAOB2)
  FRrecord = LOF(FRlu%) \ Len(RAOB2)
  For i = 0 To lstSelect.ListCount - 1
    If lstSelect.Selected(i) Then
      X$ = lstSelect.List(i)
      Cycle = Right$(X$, 5)
      Call RAOBread(RAOBlu, Cycle)
      FRrecord = FRrecord + 1
      Call RAOBwrite(FRlu, FRrecord)
    End If
  Next i
  Close FRlu
  
Case 24   'Read Mission_RAOBrangeAll.txt and find before and after RAOBs and write to
'          Mission_RAOBs.RAOB2 file
'          Search list should contain the RAOB file with soundings to be searched
    Rlu = FreeFile  'File that RAOBs are written to
    Open MNpath$ + "\RAOB\" + Mission$ + "_RAOBs.RAOB2" For Random Access Read Write As Rlu Len = Len(RAOB2)
   
    lu2 = FreeFile
    Open MNpath$ + "\RAOB\" + Mission$ + "_RAOBrangeAll.txt" For Input As lu2 'eg SOLVE2_RAOBrange.txt
    Input #lu2, A$
    nRAOBtemplates = 0
    Cycle = 0
    Do      'Read list of RAOBs within 160 km of Flight Track
      Rec1 = 0   'Rec1 - 200
      If Rec1 < 0 Then Rec1 = 0
      Input #lu2, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
      i = InStr(1, A$, vbTab)
      RAOB1$ = Left$(A$, i - 1)
      DTevent = DateSerial(iY, iM, iD) '+ TimeValue(fSecToTstring(UT0 * 1000, True))
      ' Check
      'If DTevent = FlightDate Then  ' Need only RAOBs on flight date
        ' Get RAOBs launch before and after closest approach
        UT1 = Int(UT0 / 3.6)
        Do
          UT1 = UT1 + 1
        Loop Until UT1 Mod 3 = 0  ' Find first sounding after UT0 at 3,6,9,12,15,18,21,24 hours
        'yyyymmdd$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
        ' Before in 3 hour steps
        DT1 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0) '+ TimeValue("3:00:00")
        Do
          DT1 = DT1 - TimeValue("3:00:00")
          Debug.Print "1 "; RAOB1$; " "; DT1
          Call GetRAOBfromList(RAOBlu, DT1, RAOB1$, Found1, Rec1)
          DoEvents
        Loop Until Found1
        Call RAOBread(RAOBlu, Rec1 + 1)
        WMOnumberTemp = WMOnumber
        If Cycle = 0 Then WMOnumberTemp = 0
        ' Check if already in file
        For iCycle = 1 To Cycle
          Call RAOBread(Rlu, iCycle)
          If DandT = DT1 And WMOnumber = WMOnumberTemp Then
            Exit For
          End If
        Next iCycle
        
        If DandT <> DT1 Or WMOnumber <> WMOnumberTemp Then
          Call RAOBread(RAOBlu, Rec1 + 1)
          Cycle = Cycle + 1
          Call RAOBwrite(Rlu, Cycle)
        End If
        ' After in 3 hour steps
        DT2 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0) - TimeValue("3:00:00")
        Do
          DT2 = DT2 + TimeValue("3:00:00")
          Debug.Print "2 "; RAOB1$; " "; DT2
          Call GetRAOBfromList(Rlu, DT2, RAOB1$, Found2, Rec1)
          DoEvents
        Loop Until Found2
        Call RAOBread(RAOBlu, Rec1 + 1)
        WMOnumberTemp = WMOnumber
        For iCycle = 1 To Cycle
          Call RAOBread(Rlu, iCycle)
          If DandT = DT2 And WMOnumber = WMOnumberTemp Then
            Exit For
          End If
        Next iCycle
        If DandT <> DT2 Or WMOnumber <> WMOnumberTemp Then
          Call RAOBread(RAOBlu, Rec1 + 1)
          Cycle = Cycle + 1
          Call RAOBwrite(Rlu, Cycle)
        End If
      'End If
    Loop Until EOF(lu2)
    Close lu2, RAOBlu, Rlu
    
Case 25   'Load RC AA temperature profiles
  For i = 1 To NRC
    'If optRCindex.value = 1 Then
    '  Call ReadInRC(RCpathToDot$, i)
    'End If
  Next i
  
End Select
Exit Sub

Calculate_RMS:
  ReDim RecordList(1 To lstSelect.ListCount)
  kmax = 0
  
  If chkEnableRAOBextension.Value = 1 Then
    A$ = Dir(Filename$, vbNormal)
    If A$ <> "" Then
      If Not CMD06 Then
        ans = MsgBox(Filename$ + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
        If ans = vbYes Then Kill (Filename$) Else Exit Sub
      Else
        Kill (Filename$)
      End If
    End If
    OUTlu = FreeFile
    Open Filename$ For Random Access Read Write As OUTlu Len = Len(RAOB2)
  End If
  
  For i = 1 To lstSelect.ListCount
    lstSelect.ListIndex = i - 1
    X$ = lstSelect.List(lstSelect.ListIndex)
    CurrentRecord = Val(Right$(X$, 5))
    lblProgress.Caption = "Record"
    txtProgress.Text = Str$(CurrentRecord)
    Call RAOBread(RAOBlu, CurrentRecord)
    
    If chkShowRMSprofile.Value = 1 Then Call ShowProfile(TZraob(), ZPraob(), Nlevr)
    
    kSave = kmax
    Tsum1 = 0#: Tsum2 = 0#: n = 0
    For j = i1 To i2      '24 To 44
      z = 0.5 * j
      If z > Zref(Nref) Or z > ZPraob(Nlevr) Then Exit For
      Tr = fLinterp(Zref(), Tref(), Nref, z)
      Tc = fLinterp(ZPraob(), TZraob(), Nlevr, z)
      X = Tc - Tr
      n = n + 1
      Tsum1 = Tsum1 + X
      Tsum2 = Tsum2 + X ^ 2
'      Debug.Print Format(Tmtp - Traob, "00.0"),
    Next j
    j = j - 1
    Tavg = Tsum1 / n             'Calculate average
    Trms = Sqr((Tsum2 - Tavg ^ 2) / (n - 1))     'Calculate RMS
    'Debug.Print i; Tavg; Trms
    If Trms < RMS Then
      If chkUseBias.Value = 1 Then
        If Abs(Tavg) < AVG Then
          kmax = kmax + 1
          RecordList(kmax) = CurrentRecord
          If chkShowRMSprofile.Value = 1 Then Call ShowProfile(TZraob(), ZPraob(), Nlevr)
          If chkEnableRAOBextension.Value = 1 Then
            Call RAOBwrite(OUTlu, kmax)  'Write template
          Else
            MsgBox vbOKOnly, "Template file was not written!" + vbCrLf + "RAOB extension must be specified on RAOB Extension frame."
          End If
        End If
      Else
          kmax = kmax + 1
          RecordList(kmax) = CurrentRecord
'         x = Tc - Tr
          For j = 1 To Nlevr
            TZraob(j) = TZraob(j) - Tavg
          Next j
          If chkShowRMSprofile.Value = 1 Then Call ShowProfile(TZraob(), ZPraob(), Nlevr)
          If chkEnableRAOBextension.Value = 1 Then Call RAOBwrite(OUTlu, kmax)  'Write template
      End If
    End If
    DoEvents
  Next i
  Close OUTlu
  ' Clear list and insert RAOBs that satisfy AVG and RMS criteria
  lstSelect.Clear
  TotalRec = 0
  For k = 1 To kmax
    Call RAOBread(RAOBlu, RecordList(k))
    'Debug.Print WMOnumber, WMO4l
    TotalRec = TotalRec + 1
    V(1) = WMOnumber: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihhmm
    If Val(WMO4l) <> 0 Then X$ = "    " Else X$ = WMO4l
    X$ = X$ + " " + fUsing("#0000 #000 #0 #0 #000", V())
    X$ = X$ + " " + Format$(ijkl, "0") + "   " + Format$(RecordList(k), "00000")
    lstSelect.AddItem X$
    i = lstSelect.ListCount
    SortDandT(i) = DandT
    SortCycle(i) = Cycle
    txtTotalRec.Text = Str$(TotalRec)
    DoEvents
  Next k
  Return

End Sub


Private Sub cmdSite_Click(Index As Integer)
Dim A$

Select Case Index
Case 0  'Add
  lstSite.AddItem UCase$(txtRSsite.Text)
Case 1  'Remove
  lstSite.RemoveItem (lstSite.ListIndex)
Case 2  'Clear
  lstSite.Clear
Case 3  'Open RSL File
' Set Initial directory
  cdConvertIn.InitDir = "C:\MTP\RAOB\"
' Set Size of FileName buffer
  cdConvertIn.MaxFileSize = 1024
' Set filters.
  cdConvertIn.Filter = "RSL Files|*.RSL|All Files (*.*)|*.*"
' Specify default filter.
  cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
  cdConvertIn.ShowOpen

ConvertInFile$ = cdConvertIn.Filename

If ConvertInFile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  INPlu% = FreeFile
  Open ConvertInFile$ For Input As INPlu%
  Do
    Input #INPlu, A$
    txtRSsite.Text = A$
    cmdSite_Click (0)
  Loop Until EOF(INPlu)
  Close INPlu
End If

End Select
txtRSsite.Text = ""
End Sub

Private Sub cmdStartConversion_Click()
Dim OutFileRecords&, OutFileRecord&, i%, ans, A$, Path$
If InputMode <> 3 Then  'Source Mask
' Make sure a source format has been selected
  If SourceFormat < 0 Then
    Call MsgBox("You must select a Source Format option!", vbOKOnly, "Warning")
    Exit Sub
  End If
' Check if path already exist, and create if it doesn't
  Path$ = fGetPath(txtConvertOut.Text)
  If Len(Dir(Path$)) = 0 Then
    Call CreateDirectoryTree(Path$, True)
  End If

' First check if new file, and delete it if it exists
  If optOutput(0).Value Then
    A$ = Dir(txtConvertOut.Text, vbNormal)
    If A$ <> "" Then
      ans = MsgBox(txtConvertOut.Text + " already exists!" + vbCrLf + "Do you want to delete it?", vbYesNo)
      If ans = vbYes Then Kill (txtConvertOut.Text)
    End If
  End If
' Assign output RAOB file
  OUTlu% = FreeFile
  Select Case cboRAOBtype.ListIndex
  Case 0
    Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(RAOB2)
    OutFileRecords = LOF(OUTlu%) \ Len(RAOB2)
  Case 1
    Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(RAOB)
    OutFileRecords = LOF(OUTlu%) \ Len(RAOB)
  Case 2
    Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(LRF)
    OutFileRecords = LOF(OUTlu%) \ Len(LRF)
  Case 3
    Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(RAOB3)
    OutFileRecords = LOF(OUTlu%) \ Len(RAOB3)
  End Select
  If optOutput(1).Value Then    ' Append to existing file
    OutFileRecord = OutFileRecords
  Else                          ' Overwrite file if it exists
    OutFileRecord = 0
  End If
End If

Select Case InputMode
Case 0   ' Do entire directory
'  RAOBtype = 1
  ConvertInFile$ = Dir(txtPath.Text + txtFileMask.Text, vbNormal)
  Do
    txtConvertIn.Text = txtPath.Text + ConvertInFile$
    DoEvents
    OutFileRecord = OutFileRecord + 1
    INPlu = FreeFile
    Open txtPath.Text + ConvertInFile$ For Input As INPlu
    If LOF(INPlu) > 1000 Then   '1000 Then
      Call ConvertRAOB(INPlu, SourceFormat, frmRAOBman)
      ' Now check FILTERS
      If fFilter Then RAOBgood = True Else RAOBgood = False
      Call RAOBwrite(OUTlu, OutFileRecord)
    Else
      OutFileRecord = OutFileRecord - 1
    End If
    Close INPlu
    ConvertInFile$ = Dir
    'Debug.Print ConvertInFile$
  Loop Until ConvertInFile$ = ""
  
Case 1                       ' Do single file
  OutFileRecord = OutFileRecord + 1
  INPlu = FreeFile
  Open txtConvertIn.Text For Input As INPlu
  Call ConvertRAOB(INPlu, SourceFormat, frmRAOBman)
  ' Now check FILTERS
  If fFilter Then RAOBgood = True Else RAOBgood = False

  Close INPlu
  
  If RAOBtype <> 1 Then
    Close OUTlu
  ' Assign output RAOB file
    OUTlu% = FreeFile
    Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(LRF)
    OutFileRecords = LOF(OUTlu%) \ Len(LRF)

    If optOutput(1).Value Then    ' Append to existing file
      OutFileRecord = OutFileRecords + 1
    Else                          ' Overwrite file if it exists
      OutFileRecord = 1
    End If
  End If
  
  Call RAOBwrite(OUTlu, OutFileRecord)

Case 2                       ' Single file with many RAOBs
'  RAOBtype = 1
  INPlu = FreeFile
  Open txtConvertIn.Text For Input As INPlu
  Do
    Call ConvertRAOB(INPlu, SourceFormat, frmRAOBman)
    ' Now check FILTERS
    If fFilter Then RAOBgood = True Else RAOBgood = False

'    If Not EOF(INPlu) Then
      OutFileRecord = OutFileRecord + 1
      Call RAOBwrite(OUTlu, OutFileRecord)
      txtProgress.Text = Str$(OutFileRecord)
      DoEvents
'    End If
  Loop Until EOF(INPlu)
  Close INPlu
Case 3  ' Sequential Makky Conversion
    Call ConvertRAOB(INPlu, SourceFormat, frmRAOBman)
    ' Now check FILTERS
    If fFilter Then RAOBgood = True Else RAOBgood = False

End Select
Close OUTlu

End Sub
Private Sub cmdT1017_Click()
Dim V!(1 To 10), fmt$, WMOnumberLast&, TotalRec%, T1116!, T1017!, X$

  OUTfile$ = txtExportPath.Text
  OUTlu = FreeFile
  Open txtExportPath.Text For Output As OUTlu
  Print #OUTlu, "RAOB Source File: " + txtRAOBfile.Text
  Print #OUTlu, "dTMRI Altitudes: ", txtT10.Text + "  " + txtT17.Text
  fmt$ = "####0 ##0.000 ###0.000 ##0.00"
  Cycle = 0
  RAOBtype = 0
  Do
    Cycle = Cycle + 1
    Call RAOBread(RAOBlu, Cycle)
    DetermineROABtoShow
    If fFilter Then   'Make sure filter criteria are satisfied
      TotalRec = TotalRec + 1
      T1116 = fDeltaT(11#, 16#, ZPraob!(), TZraob!(), Nlevr%)
      T1017 = fDeltaT(10#, 17#, ZPraob!(), TZraob!(), Nlevr%)
      V(1) = WMOnumber: V(2) = WMOlatitude: V(3) = WMOlongitude
      V(4) = Iyear: V(5) = Iday: V(6) = T1017: V(7) = T1116: V(8) = Zt1
'                                12345678901234567890123456789
      'If Val(WMO4l) = 0 Then WMO4l = "    "
      X$ = WMO4l + " " + fUsing("#0000 ###0.000 ###0.000 ###0 ##0 ##0.00 ##0.00 #0.##", V())
      If T1017 < 999# And T1116 < 999# Then
      Print #OUTlu, X$
      'Debug.Print x$
      End If
      txtTotalRec.Text = Str$(TotalRec)
      DoEvents
    End If
    
'    If fFilter And dTMRI < 200# Then      'Make sure filter criteria are satisfied
'      If WMOnumber <> WMOnumberLast Then  'Save time, look for repeat
'        txtStation(0).Text = Str$(WMOnumber)
'        cmdWMOstation_Click (0)
'      End If
'      v(1) = WMOnumber: v(2) = WMOlatitude: v(3) = WMOlongitude
'      v(4) = dTMRI
'      Print #OUTlu, fUsing(fmt$, v())
'    End If
  Loop Until Cycle = FileRecords
  Close OUTlu
End Sub



Private Sub cmdTexAQS_Click()

With frmRAOB
  
End With
End Sub

Private Sub cmdTemplates_Click()
Dim FilePath$, FileMask$, InFile$, FileArray$(0 To 64)
Dim i%, j%, NRC%, RAOBlu%, TemplateFile$, OUTlu%, cycleIn&, CycleOut&, Ext$

' Get all RAOB template files and load into combo box (if there are any)
  cmdTemplates.Caption = "Processing ..."
  FilePath$ = RCpath$
  FileMask$ = "*.RCF"
  InFile$ = Dir(FilePath$ + FileMask, vbNormal)
' Check that at least one file exists
  If InFile$ = "" Then
    MsgBox "No files were found in path: " + FilePath$ + vbCrLf + "Satisfying the mask: " + FileMask + vbCrLf + "This is not essential to calculate RCs!"
    Exit Sub
  End If
' One does, so now check the rest of the directory
  i = -1
  Do
    i = i + 1
    FileArray$(i) = InFile$
    InFile$ = Dir
  Loop Until InFile$ = ""
  
  If txtRAOBfile.Text = "" Then Ext$ = "RAOB2" Else Ext$ = fGetExtension(txtRAOBfile.Text) 'Make output file extension the same as the input file extension
  TemplateFile$ = RSpath$ + Mission$ + "_Template_RAOBs" + "." + Ext$
  OUTlu = FreeFile
  If Ext$ = "RAOB2" Then  'RAOB2 and RAOB3 are the only supported options
    Open TemplateFile$ For Random Access Read Write As OUTlu% Len = Len(RAOB2)
  Else
    Open TemplateFile$ For Random Access Read Write As OUTlu% Len = Len(RAOB3)
  End If
  NRC = i + 1
  CycleOut = 0
  For i = 0 To NRC - 1
    Call ReadInRC(fGetPathToDot(RCpath$ + FileArray$(i)), i)
    RAOBlu = FreeFile
    Open RAOBfilename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB2)
    cycleIn = 0
    InFile$ = fGetFilename(RAOBfilename$)
    Do
      cycleIn = cycleIn + 1
      RAOBread RAOBlu, cycleIn
      RMSfilename$ = fRMSfilename$(WMO4l, WMOnumber, Iyear, Imonth, Iday, Ihour) + "." + Ext$
    Loop Until RMSfilename$ = InFile$ Or EOF(RAOBlu)
    If Not EOF(RAOBlu) Then
      CycleOut = CycleOut + 1
      RAOBwrite OUTlu%, CycleOut
    End If
    Close RAOBlu
  Next i
  Close OUTlu
  cmdTemplates.Caption = "Template RAOBs"
End Sub

Private Sub cmdTES_Click()
Dim i%, j%, TESlu%, QualTES!, MTPlu%, PNGname$, Status As Boolean
Dim Ttes!(1 To 87), Ztes!(1 To 87), Ates!(1 To 87), Ztes2!(0 To 175), Ttes2!(1 To 87)
Dim LatTES!, LonTES!, UTtes!, A$, Ntes%, Ztes1!(1 To 87), NlevrSave%
Dim CurrentColor%, CurrentThickness%, StaTES%, iSample%, ymd$, UTks!(1 To 10)
Dim Tmtp!(1 To 33), Zmtp!(1 To 33), REFfilename$, Cycle%(1 To 10), Rsave!(1 To 10), Count%
Dim iMTP%, iTES%, Sum1!(1 To 87), Sum2!(1 To 87), X!, iLvl%(1 To 87), iStart%
Dim AVG(1 To 87), RMS(1 To 87), UThr!, Tm!, UT&, Range!(1 To 100), ir%

  cmdOpenREFfile_Click
  ymd$ = YYYYMMDD$
  For i = 1 To 87
    iLvl(i) = 0
    Sum1(i) = 0
    Sum2(i) = 0
  Next i
  ir = 0
  Ntes = 87
  TESlu = FreeFile
  Open "C:\MTP\Data\DC8\PAVE\TES\TES" + ymd$ + ".txt" For Input As TESlu
  ' skip header
  For i = 1 To 8
  Line Input #TESlu, A$
  Next i
  For i = 1 To 87
    Line Input #TESlu, A$
    Ztes(i) = fPtoZ(Val(Mid$(A$, 7, 7)))
    Ttes(i) = Mid$(A$, 19, 8)
  Next i
  ' Calculate intermediate levels
  Ztes2(0) = Ztes(1) - (Ztes(2) - Ztes(1)) / 2
  For i = 1 To 86
    Ztes2(2 * i - 1) = Ztes(i)
    Ztes2(2 * i) = Ztes(i) + (Ztes(i + 1) - Ztes(i)) / 2
    Debug.Print i; Ztes(i); Ztes2(2 * i - 1); Ztes2(2 * i)
  Next i
  Close TESlu
' Reopen file
  TESlu = FreeFile
  Open "C:\MTP\Data\DC8\PAVE\TES\TES" + ymd$ + ".txt" For Input As TESlu
  ' skip header
  For i = 1 To 7
  Line Input #TESlu, A$
  Next i
'TES Temperature retrievals from 3 Feb 2005 PAVE flight.
'File format is one row of: UT, LAT, LON
'followed by 87 rows of PRES, TEMP.
'The pressures are standard pressure levels.  Missing data
'is represented by -999.000.
'Created by R. Herman from TES_L2_RUN2614_V04.0.SAV file
'on 14 Sept 2005.  Quality flag is "OK" for all these data.
'      18.6207      35.1130     -79.9380
'      1211.53     -999.000
'      1100.70     -999.000
'      1000.00      268.600
'      908.514      271.590
'1234567890123456789012345678901234567890'**********************************************************************************
  iSample = 0
  Do
    iSample = iSample + 1
    Input #TESlu, UThr, LatTES, LonTES
    UTtes = UThr * 3.6
    UT = Int(1000 * UTtes)
    For i = 1 To 87
      Line Input #TESlu, A$
      Ztes(i) = fPtoZ(Val(Mid$(A$, 7, 7)))
      Ttes(i) = Val(Mid$(A$, 19, 8))
    Next i
    
    Call GetAuraRange(LatTES!, LonTES!, 300#, 5#, Cycle(), UTks(), Rsave(), Count)
    
    If Count > 1 Then
    X = X
    End If
    If Range(iSample) < 30 Then
    For j = 1 To Count
      Call cmdSelect_Click(5)  'Clear plot
      Debug.Print Format(iSample, "00") + vbTab + Format(Cycle(j), "0000") + vbTab + Format(UTks(j), "00.00") + vbTab + Format(Rsave(j), "000.0")
      i = 0
      Ntes = 0
      Do
        i = i + 1
        If Ttes(i) > 0 Then
          Ntes = Ntes + 1
          Ztes(Ntes) = Ztes(i)
          Ttes(Ntes) = Ttes(i)
        End If
      Loop Until Ztes(i) > 28
      Nlev = Ntes
      For i% = 1 To Ntes
        Zplot(i%) = Ztes(i) 'RAp = Zgeo or Zp depending on chkGeoPotHeight.value
        Tplot(i%) = Ttes(i)
      ''Debug.Print i; RAp(i); RTp(i)
      Next i
      NlevrSave = Nlevr
      Nlevr = Nlev
      With frmRAOB
      .ScaleMode = 0
      .EnableFit = True
      .PlotSingleScan = False
      chkMultipleScans.Value = 1
      ' Save RAOB display parameters
      CurrentColor = .LineColor
      CurrentThickness = .LineThickness
      ' Set new display parameters
      .LineColor = frmATP.LineColor
      .LineThickness = frmATP.LineThickness
      .RAOBupdate
      .EnableFit = False     'So RAOB does not try to fit!
      ' Restore RAOB display parameters
      .LineColor = CurrentColor
      .LineThickness = CurrentThickness
      End With
      Nlevr = NlevrSave
      
        Debug.Print iSample; UTtes; Format(Cycle(j), "0000") + vbTab + Format(UTks(j), "00.00") + vbTab + Format(Rsave(j), "000.0")
        ' Interpolate MTP profile to TES levels
        Call REFread(REFlu%, Cycle(j))
        For i = 1 To Nlev
          Zplot(i) = sZZZ(i)
          Tplot(i) = sTTT(i)
        Next i
        'Call Calculate_MLS_Profile(sZZZ(), sTTT(), Nlev, Zplot(), Tplot(), Ntes)
        NlevrSave = Nlevr
        Nlevr = Nlev
        frmRAOB.ShowDots = True
        frmRAOB.EnableFit = False
        frmRAOB.Tac = Tac
        frmRAOB.ALTkm = pALT
        frmRAOB.ShowAircraft = True
        frmRAOB.RAOBupdate
        Range(iSample) = Rsave(1)
        Documentation$ = " UT: " + fSecToTstring(UT, True) + " (" + Format(UTtes, "#00.000") + " ks) Lat: " + Format(LatTES, "#00.000") + " Lon: " + Format(LonTES, "#000.000") + " Range: " + Format(Range, "000")
        cmdSelect_Click (16)
        DoEvents
        frmRAOB.Line (Tac - 5, pALT)-(Tac + 5, pALT), QBColor(White)
        Nlevr = NlevrSave
  'UpdateRAOBlisting
        frmRAOB.ShowDots = False
        frmRAOB.ScaleMode = 1
        PNGname$ = "C:\MTP\Data\DC8\PAVE\TES\TES" + YYYYMMDD$ + "_" + Format(iSample, "00") + "_" + Format(j, "0") + ".PNG"
        'Status = SaveFormAsPNG(frmRAOB, PNGname$)
        CaptureFormMJ frmRAOB, PNGname$, 1, False
        
        ' Calculate statistics
        iStart = 0
        Do: iStart = iStart + 1
        Loop Until Zplot(iStart) > Ztes(iStart) 'Find first MTP level
        For i = iStart To Ntes
          If Zplot(i) = 0# Then Exit For
          iLvl(i) = iLvl(i) + 1
          Tm = fLinterp(Zplot(), Tplot(), Nlev, Ztes(i))
          X = Ttes(i) - Tm
          'Debug.Print i; Ztes(i); Zplot(i); Ttes(i); Tplot(i); x
          Sum1(i) = Sum1(i) + X         'Sum time difference
          Sum2(i) = Sum2(i) + X ^ 2     'Sum square of time differences
        Next i
      Next j
      End If
      Close MTPlu
  Loop Until EOF(TESlu)
  Close TESlu, REFlu
      
  For i = iStart To Ntes
    If iLvl(i) > 2 Then
      AVG(i) = Sum1(i) / iLvl(i)            'Calculate average
      If Sum2(i) - AVG(i) ^ 2 * iLvl(i) > 0 Then RMS(i) = Sqr((Sum2(i) - AVG(i) ^ 2 * iLvl(i)) / (iLvl(i) - 1)) Else RMS(i) = 0# 'Calculate RMS
    End If
    Debug.Print i; Ztes(i); AVG(i); RMS(i); iLvl(i); Range(iSample)
  Next i
  
  End Sub

Private Sub cmdToggleAC_Click()
Dim DrawWidthSave%
'If frmRAOB.ShowAircraft = True Then frmRAOB.ShowAircraft = False Else frmRAOB.ShowAircraft = True
'UpdateDisplay
  ' Draw pointers at A/C Altitude. Do last to put on top
    Call frmRAOB.RAOB_Pointer_Pair(pALT, White, -1)
    DrawWidthSave = frmRAOB.DrawWidth
    frmRAOB.DrawWidth = 2
    'If PT_flag Then y = fTheta(OATnav, fZtoP(pALT)) Else y = OATnav
    frmRAOB.Line (OATmtp - 5, pALT)-(OATmtp + 5, pALT), QBColor(White)

    frmRAOB.DrawWidth = DrawWidthSave

End Sub

Private Sub cmdToggleTrop_Click()
If frmFormatRAOB.chkTropopause.Value = 1 Then frmFormatRAOB.chkTropopause.Value = 0 Else frmFormatRAOB.chkTropopause.Value = 1
If frmFormatRAOB.chkTropopause.Value = 1 Then
  frmRAOB.ShowTropopause = True
Else
  frmRAOB.ShowTropopause = False
End If
UpdateDisplay

End Sub

Private Sub cmdTropList_Click()
Dim OUTlu%, X$, OUTfile$, UArecord&, Status As Boolean

  X$ = lstSelect.List(0)
  Cycle = Right$(X$, 5)
  Call RAOBread(RAOBlu, Cycle)

  OUTfile$ = "H:\StudyOut\S" + Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00") + Format(Ihhmm, "0000") + ".txt"
  OUTlu = FreeFile
  Open OUTfile$ For Output As OUTlu
  Print #OUTlu, "ICAOsite" + vbTab + "WMOnumber" + vbTab + "Long" + vbTab + "Lat" + vbTab + "Tt1" + vbTab + "Zt1" + vbTab + "Tt2" + vbTab + "Zt2"

  For i = 0 To lstSelect.ListCount - 1
    X$ = lstSelect.List(i)
    Cycle = Right$(X$, 5)
    Call RAOBread(RAOBlu, Cycle)
    Status = fGetUAsite(Str(WMOnumber), UArecord)
    If Status Then
      Print #OUTlu, ICAOsite, vbTab, WMOnumber, vbTab, GPlongitude, vbTab, GPlatitude, vbTab, TtropE, vbTab, ZtropE, vbTab, Ttrop2, vbTab, Ztrop2
    End If
  Next i
  Close OUTlu
End Sub


Private Sub Command1_Click()
txtRMS.Text = "3.0"
End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
'Select Case KeyCode
'Case 38  'Up Arrow
'  frmRAOBman.lstSelect.ListIndex = frmMJMviewer.filList.ListIndex - 1
'  If frmRAOBman.lstSelect.ListIndex < 0 Then frmRAOBman.lstSelect.ListIndex = 0
'Case 40  'Down Arrow
'  frmRAOBman.lstSelect.ListIndex = frmMJMviewer.filList.ListIndex + 1
'  If frmRAOBman.lstSelect.ListIndex > frmRAOBman.lstSelect.ListCount - 1 Then frmRAOBman.lstSelect.ListIndex = frmRAOBman.lstSelect.ListCount - 1
'End Select
End Sub


Private Sub lstEvent_Click()
Dim Line$
txtEvent.Text = lstEvent.List(lstEvent.ListIndex)
' 66770 12.079  36.125 -117.061
'1234567890123456789012345678901
Line$ = EventList$(lstEvent.ListIndex + 1)
EventTime = Val(Left$(Line$, 6))
' Add some code to check if event is in next day ... mjm
ALTkm = Val(Mid$(Line$, 7, 7))
Ltn = Val(Mid$(Line$, 14, 8))
Lnn = Val(Mid$(Line$, 22, 9))
txtLATn(2).Text = Val(Ltn)
txtLONn(2).Text = Val(Lnn)
EventNumber = Val(lstEvent.ListIndex + 1)
txtEventNo.Text = Str$(EventNumber)
txtALTkm.Text = Val(ALTkm)
optSite(0).Value = True

End Sub

Private Sub lstListRAOB_Click()
Dim A$, cval0%, cval1!, cval2!, cval3!, cval4!, cval5!, cval6!
'  txtPalt.Text = lstPalt.List(lstPalt.ListIndex)
'  Alt$ = Format$(txtPalt.Text, "00")
'              1234567890123456789012345678901234567
'  RAOBfmt$ = "## #0.000 #0.000 #000.0 #00.00 #00.00"
'  v(1) = Nlevr - i + 1: v(2) = ZGraob(i): v(3) = ZPraob(i): v(4) = PZraob(i): v(5) = TZraob(i): v(6) = RZraob(i)
  A$ = lstListRAOB.List(lstListRAOB.ListIndex)
  cval0 = Val(Left$(A$, 4))           'Nlevr
  cval1 = Val(Mid$(A$, 5, 6))         'ZGraob
  cval2 = Val(Mid$(A$, 12, 6))        'ZPraob
  cval3 = Val(Mid$(A$, 19, 6))        'PZraob
  cval4 = Val(Mid$(A$, 26, 6))        'TZraob
  cval5 = Val(Mid$(A$, 33, 6))        'RZraob
  cval6 = Val(Mid$(A$, 40, 6))        'Theta
  txtList(0).Text = Format(cval0, "#00")
  txtList(1).Text = Format(cval1, "0.000")
  txtList(2).Text = Format(cval2, "0.000")
  txtList(3).Text = Format(cval3, "#0.0")
  txtList(4).Text = Format(cval4, "000.0")
  txtList(5).Text = Format(cval5, "#00.0")
  txtList(6).Text = Format(cval6, "000.0")
  
  frmRAOB.RAOB_SelectLevel (cval0)  'draw current level
  SelectLevel = cval0
End Sub

Private Sub lstSelect_Click()
Dim X$, Tmin!, Zmin!, i%, j%, temp$, Status As Boolean, Record&

  If CMD14 Then Exit Sub
  X$ = lstSelect.List(lstSelect.ListIndex)
  CurrentRecord = Val(Right$(X$, 5))
  lblProgress.Caption = "Record"
  txtProgress.Text = Str$(CurrentRecord)
  If AutoView Then cmdSelect_Click (3)
  Tmin = 400#
  For j = 1 To Nlevr    'averaging pressure altitudes
    If TZraob(j) < Tmin And j < Nlevr Then
      Tmin = TZraob(j)
      Zmin = ZPraob(j)
    End If
  'Debug.Print j; TZraob(j); ZPraob(j)
  Next j
  txtColdestT.Text = Format(Tmin, "###.##")
  txtColdestAlt.Text = Format(Zmin, "##.##")
  'Debug.Print Format(Tmin, "###.##"); " "; Format(Zmin, "##.##")
  If WMO4l = "XXXX" Then temp$ = Format$(WMOnumber, "00000") Else temp$ = WMO4l
'  Documentation$ = ""
  Documentation$ = Documentation$ + temp$ + " " + Format$(Iyear, "##")
  Documentation$ = Documentation$ + Format$(Imonth, "00") + Format$(Iday, "00") + " " + Format$(Ihour, "00") + Format$(Imin, "00") + ", "
  ' Generate default filename for RAOB template file of soundings in case RMS command is used
  RMSfilename$ = fRMSfilename$(WMO4l, WMOnumber, Iyear, Imonth, Iday, Ihour) + ".RAOB2" '+ fGetExtension(txtRAOBfile.Text)
'  Debug.Print Documentation$ + " " + Str(WMOlatitude) + " " + Str(WMOlongitude) + " " + Str(WMOaltitude)
  
End Sub

Private Sub lstSelect_KeyDown(KeyCode As Integer, Shift As Integer)
' Need to modify so that it is only used when lstSelect does not have focus.
Select Case KeyCode
Case 13
'  if txtrssite.sle
  
Case 38  'Up Arrow
  If frmRAOBman.lstSelect.ListIndex > 1 Then
'    frmRAOBman.lstSelect.ListIndex = frmRAOBman.lstSelect.ListIndex - 1
  End If
Case 40  'Down Arrow
  If frmRAOBman.lstSelect.ListIndex < frmRAOBman.lstSelect.ListCount - 1 Then
'    frmRAOBman.lstSelect.ListIndex = frmRAOBman.lstSelect.ListIndex + 1
  End If
End Select

End Sub


Private Sub mnuFileExit_Click()
  Unload frmAbout
  Unload frmATP
  Unload frmFormatATP
  Unload frmRAOB
  Unload frmFormatRAOB
  Unload frmMap
  Unload frmNDP
  Unload frmRAOBsites
  Unload Me
  'Close All
  End
End Sub

Private Sub mnuFileOpenAscii_Click()
Dim RAOBfile$

' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir$ + Mission$
' Set Size of FileName buffer
CommonDialog1.MaxFileSize = 1024
' Set filters.
CommonDialog1.Filter = "RAOB Files (*.sjk, *.RAOB)|*.sjk|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

RAOBfile$ = CommonDialog1.Filename

If RAOBfile$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
  Exit Sub
Else
'  On Error GoTo errhandler
  RAOBlu% = FreeFile
  Open RAOBfile$ For Input As RAOBlu%
  txtRAOBfile.Text = RAOBfile$
  FileRecords = LOF(RAOBlu%)
  'ProgressBar2.Min = 0
 ' ProgressBar2.Max = FileRecords
  'frmREFview.txtFileRecords.Text = FileRecords
  End If
optMode(0).Value = True

End Sub

Private Sub mnuFileOpenBinary_Click()
Dim Filename$
' Set Initial directory
CommonDialog1.InitDir = Left$(Drive1.Drive, 2) + "\RAOB\" + Mission$
' Set Size of FileName buffer
CommonDialog1.MaxFileSize = 1024
' Set filters.
'CommonDialog1.Filter = "REF Data Structure Files (*.REF, *.ERF)|*.??F|All Files (*.*)|*.*"
CommonDialog1.Filter = "Binary RAOB Files (*.RAOB,*.LRF)|*.*R*|Large RAOB file (*.LRF)|*.LRF|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

Filename$ = CommonDialog1.Filename

If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
  Call MsgBox("You haven't opened a RAOB file!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  txtRAOBfile.Text = UCase$(Filename$)
  Select Case fExtension(UCase$(Filename$))
  Case "RAOB2": RAOBtype = 0: nRAOBlevels = 150
  Case "RAOB": RAOBtype = 1: nRAOBlevels = 70
  Case "LRF": RAOBtype = 2: nRAOBlevels = 2000
  Case "RAOB3": RAOBtype = 3: nRAOBlevels = 500
  End Select
  
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
  Case 3
    Open Filename$ For Random Access Read Write As RAOBlu% Len = Len(RAOB3)
    FileRecords = LOF(RAOBlu%) \ Len(RAOB3)
  End Select
  
  txtFileRec = Str$(FileRecords)
  DoEvents
  'ProgressBar2.Min = 0
 ' ProgressBar2.Max = FileRecords
  'frmREFview.txtFileRecords.Text = FileRecords
End If
  optMode(1).Value = True
  Cycle = 0
  Main
  cmdOpen.Caption = "&Close RAOB"
  cmdFwd_Click
  txtASCIIout.Text = fReplaceExtension(Filename$, "txt")
End Sub

Private Sub mnuHelpAbout_Click()
  Load frmAbout
  'Set defaults
  With frmAbout
    .lblApplication.Caption = "RAOBman"
    .lblHeading.Caption = "MTP Software"
    .lblVersion.Caption = "Version: " + Str(FileDateTime("C:\MTP\VB6\EXE\RAOBman.exe"))
    .Show
  End With

End Sub

Private Sub mnuHelpIndex_Click()
Navigate "C:\www\ref\ref\mtp_sw.html"

End Sub

Private Sub mnuHelpUsersGuide_Click()
  Navigate "C:\Documents and Settings\mj\My Documents\VB6\RAOB\RAOBman.HTML"
End Sub


Private Sub mnuOptionConvert_Click()
  WMOwrite
End Sub

Private Sub mnuOptionDetails_Click()

Load frmRAOBsites
With frmRAOBsites
  .Top = frmRAOBman.Height
  .Left = frmRAOBman.Left
  .Visible = True
End With

End Sub

Private Sub mnuOptionFlatfile_Click()
Dim INPlu%, OUTlu%, Record&, loc, Deg!, Min!, ByteCount&, iD%, bytes%
Dim A As WMOrecord, out$, RadioSondeSite As Boolean, C$
' this routine is only used to convert .txt listing of WMO sites
' to a random access version of the same thing.
' MJM 19981224

  INPlu = FreeFile
  Open Drive$ + "\RAOB\pub9vola981214.flatfile" For Input As INPlu
  ByteCount = LOF(INPlu)
  lblProgress.Caption = "Progress [%]"
  OUTlu = FreeFile
  Open Drive$ + "\RAOB\wmosites.WMO" For Random Access Read Write As OUTlu Len = Len(WMO)
' Skip first two lines
  Do: C$ = Input(1, INPlu): Loop Until C$ = vbLf
  Do: C$ = Input(1, INPlu): Loop Until C$ = vbLf
  Record = 0
  Do
  ' 1.RegionId > 2.RegionName > 3.CountryArea > 4.CountryCode > 5.StationId > 6.IndexNbr > 7.IndexSubNbr
  ' 8.StationName > 9.Lat > 10.Long > 11.Hp > 12.HpFlag > 13.Hha > 14.HhaFlag > 15.PressureDefId >
  '16.SO-1 > 17.SO-2 > 18.SO-3 > 19.SO-4 > 20.SO-5 > 21.SO-6 > 22.SO-7 > 23.SO-8 > 24.ObsHs > 25.UA-1
  '26.UA-2 > 27.UA-3 > 28.UA-4 > 29.ObsRems
    iD = 0  'Index of line entry
    out$ = ""
    RadioSondeSite = False
    Do   'This loop reads each line of Flatfile
      C$ = Input(1, INPlu)
      bytes = bytes + 1
      Select Case C$
      Case vbLf  'Last Field = ObsRems
      Case vbTab
        iD = iD + 1
        'Debug.Print ID; out$
        Select Case iD
        Case 1  'RegionID
          WMOregionID = Val(out$)
        Case 2  'RegionName
          'WMOregion = Val(out$)
        Case 3  'CountryArea
         'WMOstate = Mid$(Line$, 22, 2)
          If InStr(1, out$, "/", 1) = 0 Then
            WMOcountry = out$
          Else
            WMOcountry = Left$(out$, InStr(1, out$, "/", 1) - 1)
          End If
        Case 4  'CountryCode
        Case 5
        Case 6  'StationId
          WMOnumber = Val(out$)
        Case 7  'IndexNbr
        'Case 8  'IndexSubNbr
        Case 8  'StationName
          WMOstation = out$
        Case 9  'Lat eg 36 50N
          loc = InStr(1, out$, " ", 1)
          Deg = Val(Left$(out$, loc - 1))
          Min = Val(Mid$(out$, loc + 1, 2))
          WMOlatitude = Deg + Min / 60#  'deg
          If Mid$(out$, loc + 3) = "S" Then WMOlatitude = -WMOlatitude
        Case 10 'Long eg 05 47E
          loc = InStr(1, out$, " ", 1)
          Deg = Val(Left$(out$, loc - 1))
          Min = Val(Mid$(out$, loc + 1, 2))
          WMOlongitude = Deg + Min / 60#  'deg
          If Mid$(out$, loc + 3) = "W" Then WMOlongitude = -WMOlongitude
        Case 11 'Hp
          WMOaltitude = Val(out$)  'meters
        Case 12 'HpFlag
        Case 13 'Hha
        Case 14 'HhaFlag
        Case 15 'PressureDefId
        Case 16 'SO-1
        Case 17 'SO-2
        Case 18 'SO-3
        Case 19 'SO-4
        Case 20 'SO-5
        Case 21 'SO-6
        Case 22 'SO-7
        Case 23 'SO-8
        Case 24 'ObsHs
        Case 25, 26, 27, 28 'UA-1 -2 -3 -4  'look for "R"=radiosonde
          If InStr(1, out$, "R", 1) > 0 Then RadioSondeSite = True
        End Select
        out$ = ""            'Clear for next field
      Case Else
        out$ = out$ + C$     'Collect field data
      End Select
    Loop Until C$ = vbLf     'End of line
    If bytes Mod 100 = 0 Then
      txtProgress.Text = Str$(Int(bytes * 100# / ByteCount + 0.5))
      DoEvents
    End If
    If RadioSondeSite Then
      Record = Record + 1
      A.WMOnumber = WMOnumber
      A.WMOregionID = WMOregionID
      WMOnumber = Get4l(WMOnumber)
      A.WMO4l = WMO4l
      A.WMOstation = WMOstation
      A.WMOstate = WMOstate
      A.WMOcountry = WMOcountry
      A.WMOlatitude = WMOlatitude
      A.WMOlongitude = WMOlongitude
      A.WMOaltitude = WMOaltitude

      Put #OUTlu%, Record, A
    End If
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu

End Sub

Private Sub mnuOptionFLTinfo_Click()
   
  ShowFLTinfo = Not ShowFLTinfo
  If ShowFLTinfo Then
    frmFLTINFO.Visible = True
  Else
    frmFLTINFO.Visible = False
  End If
  
End Sub

Private Sub mnuOptionFormatATP_Click()
  frmFormatATP.Top = frmATP.Top + (frmATP.Height - frmFormatATP.Height) / 2
  frmFormatATP.Left = frmATP.Left + (frmATP.Width - frmFormatATP.Width) / 2
  frmFormatATP.Visible = True
End Sub

Private Sub mnuOptionFormatRAOB_Click()
  frmFormatRAOB.Top = frmRAOB.Top + (frmRAOB.Height - frmFormatRAOB.Height) / 2
  frmFormatRAOB.Left = frmRAOB.Left + (frmRAOB.Width - frmFormatRAOB.Width) / 2
  frmFormatRAOB.Visible = True

End Sub


Private Sub munOptionMakky_Click()
Dim h1$, h2$, A$, b$, V!(1 To 10), INlu%, OUTlu%
Dim InFile$, OUTfile$

' Set Initial directory
cdConvertIn.InitDir = txtPath.Text
' Set Size of FileName buffer
cdConvertIn.MaxFileSize = 1024
' Set filters.
cdConvertIn.Filter = "RAOB Files (*.*)|*.*|All Files (*.*)|*.*"
' Specify default filter.
cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
cdConvertIn.ShowOpen

InFile$ = cdConvertIn.Filename

If InFile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  OUTfile$ = Left$(InFile$, Len(InFile$) - 5) + ".CONV"
End If
'Odd
'      IDN  DAY[SYD] TIME[HMS]      Z[M]  LEV      T[K]     TD[K]  SPD[MPS]  DIR[DEG]
'--------- --------- --------- --------- ---- --------- --------- --------- ---------
'     3953     97291         0        14 SFC     287.76    286.76       1.0       100
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234
'Even
'      IDN  DAY[SYD] TIME[HMS] TYPE        P3        P2        P1
'--------- --------- --------- ---- --------- --------- ---------
'     3953     97291         0 SIGT     10040     28676     28776
'000000000111111111122222222223333333333444444444455555555556666666666777777777788888
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234

Call ConvertEvenMakky(InFile$, OUTfile$)

End Sub




Private Sub optMode_Click(Index As Integer)

Select Case Index
Case 0
  ASCIIinput = True
Case 1
  ASCIIinput = False
End Select

End Sub



Private Sub optSite_Click(Index As Integer)

If Index = 0 Then Site1 = True Else Site1 = False

End Sub

Private Sub optOutFormat_Click(Index As Integer)
ExportFormat = Index
End Sub

Private Sub optOutMode_Click(Index As Integer)
ExportMode = Index
End Sub


Private Sub tmrGetCycle_Timer()
  Call GetRAOBcycle(RAOBlu, Cycle, ASCIIinput)
  CurrentRecord = Cycle
  frmRAOB.EnableFit = False
  If Not EOF(RAOBlu) And chkAutoView Then UpdateDisplay
  If chkNDP.Value = 1 Then
    With frmNDP
    .Left = frmRAOBman.Width
    .Top = frmRAOB.Height
    .Width = frmRAOB.Width
    .Height = 768 * Screen.TwipsPerPixelY - frmRAOB.Height - 400
    .Show
    .Visible = True
    .PlotSingleScan = True
    .Nlev = Nlevr
    .NDPmonth = Imonth
    .ShowAircraft = False
    .PCstart = -10
    End With
    Latitude = WMOlatitude
    
    For i = 1 To Nlevr
      RAp(i) = ZPraob(i)
      RTp(i) = fNDmj(RZraob(i), PZraob(i), TZraob(i))
    Next i
    frmNDP.NDPupdate
  Else
    frmNDP.Visible = False
  End If
  DoEvents
End Sub
Sub UpdateDisplay()
 
  frmRAOB.RAOBupdate

End Sub


Private Sub cmdFwd_Click()
Forward = True
tmrGetCycle_Timer

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

Private Sub Form_Load()
Dim lu%, File$, DriveList$, i%

  Show
  ReadSETUP "C:\MTP\MTPsetup.INI"
  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"
  Set CallingForm = frmRAOBman
    
  Drive$ = DataDrive$
  If Len(Drive$) = 0 Then Drive$ = "C:"
  txtPrefix.Text = Drive$ + "\MTP\Data\" + Platform$ + "\" + Mission$ + "\"
' Synch Drive List Box to default drive
  DriveList$ = Left$(Drive1.Drive, 2)
  If UCase$(DriveList$) <> UCase$(Drive$) Then
    For i = 0 To Drive1.ListCount - 1
      If UCase$(Left$(Drive1.List(i), 1)) = UCase$(Left$(Drive$, 1)) Then
        Drive1.Drive = Drive1.List(i)
        Exit For
      End If
    Next i
  End If
  
  txtPath.Text = Left$(Drive1.Drive, 2) + "\MTP\Data\" + Platform$ + "\" + Mission$ + "\RAOB\"
  txtExportPath.Text = txtPath.Text
  txtMTPpath.Text = Left$(Drive1.Drive, 2) + "\MTP\Data\" + Platform$ + "\" + Mission$ + "\"
  
  With cboRAOBtype
  .AddItem "150"
  .AddItem "70"
  .AddItem "2000"
  .AddItem "500"
  .ListIndex = 0
  End With
  RAOBtype = 0

'  Select Case RAOBtype
'  Case 0:  txtConvertOut.Text = Left$(Drive1.Drive, 2) + "\RAOB\Missions\" + Mission$ + "\" + Mission$ + ".RAOB2"
'  Case 1:  txtConvertOut.Text = Left$(Drive1.Drive, 2) + "\RAOB\Missions\" + Mission$ + "\" + Mission$ + ".RAOB"
'  Case 2:  txtConvertOut.Text = Left$(Drive1.Drive, 2) + "\RAOB\Missions\" + Mission$ + "\" + Mission$ + ".LRF"
'  End Select

  With cboRAOBtypeOUT
  .AddItem "150"
  .AddItem "70"
  .AddItem "2000"
  .AddItem "500"
  .ListIndex = 0
  End With
  RAOBtypeOUT = 0

' Set up RAOB Options Form
  cboRAOBgo.AddItem "Record"
  cboRAOBgo.AddItem "3-Letter"
  cboRAOBgo.AddItem "WMO Station"
  cboRAOBgo.ListIndex = 1

  With cboATPgo
  .AddItem "Rec No"
  .AddItem "ks"
  .AddItem "seconds"
  .AddItem "hhmmss"
  .AddItem "hh:mm:ss"
  .ListIndex = 1
  End With

  With frmRAOB
  .RAOB_Initialize
  .Top = 0
  .Left = Width
  .Width = 1024 * Screen.TwipsPerPixelX - Width
  .Height = Height
  .PlotSingleScan = True
  .PlotT0 = True
  .PlotT1 = False
  .PlotT2 = False
  .PlotT3 = False
  .ShowDots = False
  End With
  
  Load frmTools
  With frmTools
  .Hide
  .Top = (frmRAOBman.Height - frmTools.Height) / 2
  .Left = frmRAOBman.Width + (frmRAOB.Width - frmTools.Width) / 2
  End With
  Set frmRAOB.ExtForm = frmRAOBman

  Load frmFormatRAOB                  'Load but make invisible
  frmFormatRAOB.Visible = False
  frmFormatRAOB.chkAircraft.Value = 0
  frmFormatRAOB.chkTropopause.Value = 0
  frmFormatRAOB.chkAircraft.Value = 0
  frmFormatRAOB.chkSelectLevel.Value = 0

  With frmATP
  .ATP_Initialize
  .Top = frmRAOB.Height
  .Left = frmRAOB.Left
  .Width = frmRAOB.Width
  .Height = frmRAOB.Height * (3 / 4)
  .Visible = False
  End With

  With frmFormatRAOB
  .optYcoordl(0).Value = 1
  .optYcoordr(0).Value = 1
  .Hide
  End With

  With cmbSrcFormat
  .AddItem "FSL"
  .AddItem "Wyoming"
  .AddItem "BLG"
  .AddItem "SJK"
  .AddItem "Pfister"
  .AddItem "EDW"
  .AddItem "TTY"
  .AddItem "Makky"
  .AddItem "PNNL"
  .AddItem "Andros"
  .AddItem "CLS5"
  .AddItem "TAMU"
  .ListIndex = 1
  End With

  With cmbInputMode
  .AddItem "All files satisfying mask"
  .AddItem "Single file with one RAOB"
  .AddItem "Single file with many RAOBs"
  .AddItem "Makky Files Sequential"
  .ListIndex = 0
  End With

  optOutput(1).Value = True


  For i = 0 To 3
    EnableFilter(i) = True
    txtFilter_Click (i)   'Set False and  label as Disabled
  Next i

  For i = 1995 To Val(Right$(Date, 4))
    cmbStart(0).AddItem Str$(i): cmbEnd(0).AddItem Str$(i)
  Next i
  cmbStart(0).Text = cmbStart(0).List(cmbStart(0).ListCount - 1)
  cmbEnd(0).Text = cmbEnd(0).List(cmbEnd(0).ListCount - 1)

  For i = 1 To 12
    cmbStart(1).AddItem Str$(i): cmbEnd(1).AddItem Str$(i)
  Next i
  cmbStart(1).Text = "3": cmbEnd(1).Text = "3"

  For i = 1 To 31
    cmbStart(2).AddItem Str$(i): cmbEnd(2).AddItem Str$(i)
  Next i
  cmbStart(2).Text = "1": cmbEnd(2).Text = "1"

  For i = 0 To 21 Step 3
    cmbStart(3).AddItem Format(i, "00"): cmbEnd(3).AddItem Format(i, "00")
  Next i
  cmbStart(3).Text = "00": cmbEnd(3).Text = "12"

  With cmbAltMin     'Min burst altitude
  .AddItem "km"
  .AddItem "mb"
  .AddItem "kft"
  .ListIndex = 0
  End With

  With cmbAltMax     'Min sounding altitude
  .AddItem "km"
  .AddItem "mb"
  .AddItem "kft"
  .ListIndex = 0
  End With


  With cboAvgType
  .AddItem "Trop Stats"
  .AddItem "ND Profile"
  .AddItem "MP for DIAL"
  .ListIndex = 2
  End With

  With cboTincrement
  .AddItem "0.0"
  .AddItem "0.5"
  .AddItem "1.0"
  .AddItem "2.0"
  .AddItem "5.0"
  .AddItem "10.0"
  .ListIndex = 0
  End With

  With cboMap
  .AddItem "North America"
  .AddItem "South America"
  .AddItem "South Pacific"
  .AddItem "New Zealand"
  .AddItem "Antarctica"
  .AddItem "Arctic"
  .AddItem "Europe"
  .AddItem "Africa"
  .AddItem "Southeast Asia"
  End With

  With cboWindow
  .AddItem "1"
  .AddItem "2"
  .AddItem "3"
  .AddItem "4"
  .AddItem "5"
  .ListIndex = 0
  End With

' Set Export Options
  optOutFormat(0).Value = True
  optOutMode(0).Value = True

  chkGeoPotHeight(0).Value = 0
  chkGeoPotHeight_Click (0)

  chkAutoView.Value = 1
  optSite(0).Value = True
  frmATP.PlotSingleScan = True
  DefaultBG = True
  frmRAOB.LineColor = 13
  UpDown3_UpClick (0)   'Make 14 = Yellow
  frmATP.LineColor = 12 'Make 13 = Pink
  UpDown3_UpClick (1)
  UpDown2_UpClick (0)   'RAOB thickness  = 1
  UpDown2_UpClick (1)   'ATP thickness  = 2
  ShowFLTinfo = False
  Main

End Sub

Private Sub tmrGetCycle2_Timer()
Dim i%, j%, V!(1 To 9), X$
NextCycle:
  Call GetREFcycle(Cycle2, False)
  If chkSkipBadScans.Value = 1 And Not GoodScan Then GoTo NextCycle
  ProgressBar2.Value = Cycle2
  txtREFrecord.Text = Str(Cycle2)
  If Nlev > 0 And Nlev <= 33 Then frmATP.ATPupdate
  V(1) = UTsec: V(2) = pALT: V(3) = gALT: V(4) = Latitude: V(5) = Longitude: V(6) = Heading
  V(7) = Pitch: V(8) = Roll: V(9) = Tac
'                           HHMMSS UTsec pALT gALT    Lat     Lon Hdg Pitch  Roll OAT_K
  X$ = fSecToTstring$(UTsec, False) + fUsing(" #####0 #0.0 #0.0 ##0.00 ###0.00 ##0 ##0.0 ##0.0 #00.0", V())
  txtCommLine.Text = X$
  txtZp.Text = Format(pALT, "00.000")
  txtTac.Text = Format(Tac, "000.00")
  txtRCindex1.Text = Reg(RCindex1)
  If RCindex2 > 0 Then txtRCindex2.Text = Reg(RCindex2) Else txtRCindex2 = ""
  
  DoEvents

End Sub

Private Sub txtColor_Click(Index As Integer)
Dim f As Form
If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
  
If f.LineColor > 0 Then f.LineColor = -1 Else f.LineColor = 13
txtColor(Index).Text = Val(f.LineColor)

If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate

End Sub


Private Sub txtFilter_Click(Index As Integer)
  
  EnableFilter(Index) = Not EnableFilter(Index)
  If EnableFilter(Index) Then
    txtFilter(Index).BackColor = &H8000&
  Else
    txtFilter(Index).BackColor = &HFF&
  End If
  
  Select Case Index
  Case 3 'Convert BurstAlt to km
    Select Case cmbAltMin.Text
    Case "km": BurstAlt = Val(txtAltMin.Text)
    Case "mb": BurstAlt = fPtoZ(Val(txtAltMin.Text))
    Case "kft": BurstAlt = Val(txtAltMin.Text) / 3.2808
    End Select
  Case 14 'Convert Minimum RAOB Altitude to km
    Select Case cmbAltMax.Text
    Case "km": StartAlt = Val(txtAltMin.Text)
    Case "mb": StartAlt = fPtoZ(Val(txtAltMin.Text))
    Case "kft": StartAlt = Val(txtAltMin.Text) / 3.2808
    End Select
  End Select
End Sub


Private Sub txtGood_Click(Index As Integer)
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop
' &H0001 ... Good RAOB
' &H0002 ... Good Tropopause
' &H0004 ... Editted levels
' &H0008 ... Editted Trop


Select Case Index
Case 0
  RAOBgood = Not RAOBgood
  If RAOBgood Then
    txtGood(0).BackColor = &H8000&
  Else
    txtGood(0).BackColor = &HFF&
    TropGood = False                  'Force Trop bad if RAOB is bad
  End If

Case 1
  TropGood = Not TropGood
  If TropGood Then
    txtGood(1).BackColor = &H8000&
  Else
    txtGood(1).BackColor = &HFF&
  End If
End Select

Call RAOBwrite(RAOBlu%, Cycle)
UpdateRAOBlisting

End Sub


Private Sub txtMod_Click(Index As Integer)
Dim ans
' Don't do anything if there has been no editting
If txtMod(Index).BackColor = &HFF& Then Exit Sub

Select Case Index
Case 0
  ans = MsgBox("Are you sure you want to clear editted level information?", vbYesNoCancel)
  If ans = vbYes Then
    Nedit = 0
    EditLevel = False
    txtMod(0).BackColor = &HFF&
    txtMod(0).Text = ""
    txtMod(0).Text = Str$(Nedit)
    txtUse(0).BackColor = &HFF&
  End If
  
Case 1
  ans = MsgBox("Are you sure you want to clear editted trop information?", vbYesNoCancel)
  If ans = vbYes Then
    ZtropE = Ztrop
    TtropE = Ttrop
    LtropE = Ltrop
    EditTrop = False
    txtMod(1).BackColor = &HFF&
    UseEditTrop = False
    txtUse(1).BackColor = &HFF&
  End If
End Select

Call RAOBwrite(RAOBlu%, Cycle)
UpdateRAOBlisting

End Sub

Private Sub txtRange_Change(Index As Integer)
  
Select Case Index
Case 0:  LAT1 = Val(txtRange(0).Text)
Case 1:  LAT2 = Val(txtRange(1).Text)
Case 2:  LON1 = Val(txtRange(2).Text)
Case 3:  LON2 = Val(txtRange(3).Text)
End Select

End Sub



Private Sub txtRSsite_Change()
Dim X!

X = X
End Sub

Private Sub txtRSsite_KeyPress(KeyAscii As Integer)

  If KeyAscii = 13 Then
   cmdSite_Click (0) 'Add to list if CR detected
  End If

End Sub


Private Sub txtT1017_Change(Index As Integer)
'    T1017 = T10km - T17km
'    Select Case T1017             ' Calculate Region Number based on Delta T
'    Case Is >= 38: RegNr1 = 0
'    Case Is >= 24: RegNr1 = (38 - T1017) / 14
'    Case Is >= 5: RegNr1 = 1
'    Case Is >= 1: RegNr1 = 1 + (5 - T1017) / 4
'    Case Else: RegNr1 = 2
'    End Select
'    Tropical                  > 38
'    Transitional   38 > T1017 > 24
'    Mid Lat                   > 5
'    Transitional    5 > t1017 > 1
'    Polar                     <1
End Sub


Private Sub txtThickness_Click(Index As Integer)
Dim f As Form

If Index = 0 Then Set f = frmRAOB Else Set f = frmRAOB
If f.LineThickness = 0 Then f.LineThickness = 1 Else f.LineThickness = 0
txtThickness(Index).Text = Val(f.LineThickness)
If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate
End Sub


Private Sub txtUse_Click(Index As Integer)
Select Case Index
Case 0
  If EditLevel Then UseEditLevel = Not UseEditLevel Else Exit Sub
  If UseEditLevel Then
    txtUse(0).BackColor = &H8000&
  Else
    txtUse(0).BackColor = &HFF&
  End If

Case 1
  If EditTrop Then UseEditTrop = Not UseEditTrop Else Exit Sub
  If UseEditTrop Then
    txtUse(1).BackColor = &H8000&
  Else
    txtUse(1).BackColor = &HFF&
  End If
End Select

Call RAOBwrite(RAOBlu%, Cycle)
UpdateRAOBlisting

End Sub


Private Sub UpDown1_DownClick()
' Up/Down is in atmosphere NOT in list box
If lstListRAOB.ListIndex = lstListRAOB.ListCount - 1 Then
  Exit Sub
Else
  lstListRAOB.ListIndex = lstListRAOB.ListIndex + 1
End If
lstListRAOB_Click
End Sub


Private Sub UpDown1_UpClick()
' Up/Down is in atmosphere NOT in list box
If lstListRAOB.ListIndex = 0 Then
  Exit Sub
Else
  lstListRAOB.ListIndex = lstListRAOB.ListIndex - 1
  lstListRAOB_Click
End If
End Sub


Private Sub UpDown2_DownClick(Index As Integer)
Dim f As Form
If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
f.LineThickness = f.LineThickness - 1
If f.LineThickness < 0 Then f.LineThickness = 0
txtThickness(Index).Text = Val(f.LineThickness)
If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate
End Sub


Private Sub UpDown2_UpClick(Index As Integer)
Dim f As Form

If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
f.LineThickness = f.LineThickness + 1
If f.LineThickness > 5 Then f.LineThickness = 5
txtThickness(Index).Text = Val(f.LineThickness)
If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate
End Sub


Private Sub UpDown3_DownClick(Index As Integer)
Dim f As Form
If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
f.LineColor = (f.LineColor - 1)
If f.LineColor < -1 Then f.LineColor = 15
txtColor(Index).Text = Val(f.LineColor)
If f.LineColor > 0 Then txtColor(Index).BackColor = QBColor(f.LineColor)
If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate

End Sub


Private Sub UpDown3_UpClick(Index As Integer)
Dim f As Form

If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
f.LineColor = (f.LineColor + 1) Mod 16
txtColor(Index).Text = Val(f.LineColor)
txtColor(Index).BackColor = QBColor(f.LineColor)

If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate

End Sub


