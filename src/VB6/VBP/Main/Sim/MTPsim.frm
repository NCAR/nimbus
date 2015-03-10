VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{FE0065C0-1B7B-11CF-9D53-00AA003C9CB6}#1.1#0"; "COMCT232.OCX"
Begin VB.Form frmMTPsim 
   Caption         =   "MTP Retrieval Simulator"
   ClientHeight    =   5580
   ClientLeft      =   1080
   ClientTop       =   2730
   ClientWidth     =   10170
   ClipControls    =   0   'False
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   5580
   ScaleWidth      =   10170
   Begin VB.CheckBox chkTotalError 
      Caption         =   "Total Error MRI"
      Height          =   195
      Left            =   8640
      TabIndex        =   617
      Top             =   2280
      Width           =   1485
   End
   Begin VB.ComboBox cboFilter 
      Height          =   315
      Left            =   9120
      TabIndex        =   607
      Text            =   "CH1"
      Top             =   990
      Width           =   1020
   End
   Begin VB.CheckBox chkTdependence 
      Caption         =   "Check T Depen"
      Height          =   195
      Left            =   8640
      TabIndex        =   581
      Top             =   1830
      Width           =   1485
   End
   Begin VB.CheckBox chkRemoveOBbias 
      Caption         =   "Remove OB Bias"
      Height          =   195
      Left            =   8640
      TabIndex        =   580
      Top             =   2055
      Width           =   1515
   End
   Begin VB.CheckBox chkUseSmatrix 
      Caption         =   "Apply S Matrix"
      Height          =   225
      Left            =   8640
      TabIndex        =   579
      Top             =   1380
      Width           =   1485
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "Test"
      Height          =   375
      Left            =   8670
      TabIndex        =   578
      Top             =   510
      Width           =   945
   End
   Begin VB.CheckBox chkOriginalMRI 
      Caption         =   "Original MRI"
      Height          =   195
      Left            =   8640
      TabIndex        =   577
      Top             =   1605
      Width           =   1200
   End
   Begin VB.TextBox txtATPbias 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   9600
      MultiLine       =   -1  'True
      TabIndex        =   575
      Text            =   "MTPsim.frx":0000
      Top             =   2580
      Width           =   525
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Statistics"
      Height          =   375
      Index           =   4
      Left            =   8640
      TabIndex        =   375
      Top             =   90
      Width           =   975
   End
   Begin VB.CommandButton cmdOpenRAOBfile 
      Caption         =   "Open RAOB File"
      Height          =   375
      Left            =   7620
      TabIndex        =   374
      Top             =   3450
      Width           =   2535
   End
   Begin VB.CommandButton cmdBG 
      Caption         =   "BG Color"
      Height          =   375
      Left            =   7620
      TabIndex        =   253
      Top             =   90
      Width           =   975
   End
   Begin VB.CommandButton cmdRepeat 
      Caption         =   "Repeat"
      Height          =   465
      Left            =   7620
      TabIndex        =   250
      Top             =   5100
      Width           =   1245
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "Arc Avg"
      Height          =   375
      Index           =   6
      Left            =   7620
      TabIndex        =   157
      Top             =   1350
      Width           =   975
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Plot ATP"
      Height          =   375
      Index           =   5
      Left            =   7620
      TabIndex        =   137
      Top             =   2610
      Width           =   975
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Calc TAs"
      Height          =   375
      Index           =   2
      Left            =   7620
      TabIndex        =   134
      Top             =   1770
      Width           =   975
   End
   Begin VB.CommandButton cmdShowTrop 
      Caption         =   "Show Trop"
      Height          =   375
      Left            =   7620
      TabIndex        =   122
      Top             =   930
      Width           =   975
   End
   Begin VB.CommandButton cmdShowAC 
      Caption         =   "Show A/C"
      Height          =   375
      Left            =   7620
      TabIndex        =   121
      Top             =   510
      Width           =   975
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "E&xit"
      Height          =   465
      Index           =   1
      Left            =   8940
      TabIndex        =   100
      Top             =   5100
      Width           =   1215
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Clear Plot"
      Height          =   375
      Index           =   0
      Left            =   7620
      TabIndex        =   68
      Top             =   3030
      Width           =   975
   End
   Begin VB.CommandButton cmdBack 
      Caption         =   "&Back"
      Height          =   495
      Left            =   7620
      TabIndex        =   43
      ToolTipText     =   "Go Backward in RAOB file"
      Top             =   4560
      Width           =   675
   End
   Begin VB.CommandButton cmdFwd 
      Caption         =   "&Fwd"
      Height          =   495
      Left            =   9480
      TabIndex        =   42
      ToolTipText     =   "Go Forward in RAOB file"
      Top             =   4560
      Width           =   675
   End
   Begin VB.CommandButton cmdStop 
      Caption         =   "&Continue"
      Height          =   495
      Left            =   8400
      TabIndex        =   41
      ToolTipText     =   "Auto/Single step thru RAOB file"
      Top             =   4560
      Width           =   1035
   End
   Begin VB.TextBox txtProgress 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   9480
      MultiLine       =   -1  'True
      TabIndex        =   40
      ToolTipText     =   "Current record number"
      Top             =   3090
      Width           =   645
   End
   Begin VB.Frame Frame6 
      Caption         =   "RAOB GoTo"
      Height          =   615
      Left            =   7620
      TabIndex        =   37
      Top             =   3870
      Width           =   2535
      Begin VB.ComboBox cboGoTo 
         Height          =   315
         Left            =   810
         TabIndex        =   616
         Top             =   225
         Width           =   1095
      End
      Begin VB.CommandButton cmdGoTo 
         Caption         =   "Go"
         Height          =   495
         Left            =   2040
         TabIndex        =   39
         ToolTipText     =   "Go directly to selected location"
         Top             =   120
         Width           =   495
      End
      Begin VB.TextBox txtGoTo 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   38
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.TextBox txtFileRec 
      Height          =   285
      Left            =   8670
      TabIndex        =   36
      ToolTipText     =   "Number of records in open RAOB file"
      Top             =   3090
      Width           =   615
   End
   Begin VB.Timer tmrGetCycle 
      Left            =   9600
      Top             =   630
   End
   Begin VB.CommandButton cmdREF 
      Caption         =   "&Retrieve"
      Height          =   375
      Index           =   3
      Left            =   7620
      TabIndex        =   0
      Top             =   2190
      Width           =   975
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   9570
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   5565
      Left            =   0
      TabIndex        =   1
      Top             =   30
      Width           =   7575
      _ExtentX        =   13361
      _ExtentY        =   9816
      _Version        =   393216
      Tabs            =   8
      Tab             =   6
      TabsPerRow      =   8
      TabHeight       =   520
      BackColor       =   12632256
      TabCaption(0)   =   "&Setup"
      TabPicture(0)   =   "MTPsim.frx":0004
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Label(13)"
      Tab(0).Control(1)=   "Label(43)"
      Tab(0).Control(2)=   "Label(23)"
      Tab(0).Control(3)=   "txtCommLine"
      Tab(0).Control(4)=   "txtREFfile"
      Tab(0).Control(5)=   "txtRAOBfile"
      Tab(0).Control(6)=   "cmdOpen"
      Tab(0).Control(7)=   "chkGeoPotHeight"
      Tab(0).Control(8)=   "chkAutoClear"
      Tab(0).Control(9)=   "chkAutoRetrieve"
      Tab(0).Control(10)=   "Frame7"
      Tab(0).Control(11)=   "Frame(1)"
      Tab(0).Control(12)=   "chkWriteResults"
      Tab(0).Control(13)=   "chkProcessFile"
      Tab(0).Control(14)=   "chkTexAQS"
      Tab(0).Control(15)=   "chkDisableBlending"
      Tab(0).Control(16)=   "txtExcessGndT"
      Tab(0).Control(17)=   "chkExtendRAOB"
      Tab(0).Control(18)=   "txtZtop"
      Tab(0).Control(19)=   "Command1"
      Tab(0).Control(20)=   "chkFixChnRF(0)"
      Tab(0).Control(21)=   "txtCHnLSB(0)"
      Tab(0).Control(22)=   "chkFixChnRF(1)"
      Tab(0).Control(23)=   "chkFixChnRF(2)"
      Tab(0).Control(24)=   "txtCHnLSB(1)"
      Tab(0).Control(25)=   "txtCHnLSB(2)"
      Tab(0).ControlCount=   26
      TabCaption(1)   =   "&Cal"
      TabPicture(1)   =   "MTPsim.frx":0020
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame(2)"
      Tab(1).Control(1)=   "txtRCpath"
      Tab(1).Control(2)=   "cmdRCpath"
      Tab(1).Control(3)=   "txtNRC"
      Tab(1).Control(4)=   "txtNret"
      Tab(1).Control(5)=   "txtNobs"
      Tab(1).Control(6)=   "txtlatitude"
      Tab(1).Control(7)=   "Frame1"
      Tab(1).Control(8)=   "Frame3"
      Tab(1).Control(9)=   "chkOldDC8RC"
      Tab(1).Control(10)=   "cboUSE"
      Tab(1).Control(11)=   "fraLAT"
      Tab(1).Control(12)=   "Label(21)"
      Tab(1).Control(13)=   "Label(25)"
      Tab(1).Control(14)=   "Label(16)"
      Tab(1).Control(15)=   "Label(0)"
      Tab(1).Control(16)=   "Label(24)"
      Tab(1).Control(17)=   "Label(20)"
      Tab(1).ControlCount=   18
      TabCaption(2)   =   "&TAs"
      TabPicture(2)   =   "MTPsim.frx":003C
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Label(31)"
      Tab(2).Control(1)=   "Label(30)"
      Tab(2).Control(2)=   "Label(29)"
      Tab(2).Control(3)=   "Label(28)"
      Tab(2).Control(4)=   "Label(32)"
      Tab(2).Control(5)=   "Label(33)"
      Tab(2).Control(6)=   "Label(34)"
      Tab(2).Control(7)=   "Label(35)"
      Tab(2).Control(8)=   "Label(36)"
      Tab(2).Control(9)=   "Label(37)"
      Tab(2).Control(10)=   "Label(38)"
      Tab(2).Control(11)=   "Label(39)"
      Tab(2).Control(12)=   "Label(40)"
      Tab(2).Control(13)=   "Label(41)"
      Tab(2).Control(14)=   "Label(42)"
      Tab(2).Control(15)=   "Label(67)"
      Tab(2).Control(16)=   "Label(52)"
      Tab(2).Control(17)=   "Label(53)"
      Tab(2).Control(18)=   "Label(54)"
      Tab(2).Control(19)=   "txtTA1(0)"
      Tab(2).Control(20)=   "txtTA1(1)"
      Tab(2).Control(21)=   "txtTA1(2)"
      Tab(2).Control(22)=   "txtTA1(3)"
      Tab(2).Control(23)=   "txtTA1(4)"
      Tab(2).Control(24)=   "txtTA1(5)"
      Tab(2).Control(25)=   "txtTA1(6)"
      Tab(2).Control(26)=   "txtTA1(7)"
      Tab(2).Control(27)=   "txtTA1(8)"
      Tab(2).Control(28)=   "txtTA1(9)"
      Tab(2).Control(29)=   "txtTA2(0)"
      Tab(2).Control(30)=   "txtTA2(1)"
      Tab(2).Control(31)=   "txtTA2(2)"
      Tab(2).Control(32)=   "txtTA2(3)"
      Tab(2).Control(33)=   "txtTA2(4)"
      Tab(2).Control(34)=   "txtTA2(5)"
      Tab(2).Control(35)=   "txtTA2(6)"
      Tab(2).Control(36)=   "txtTA2(7)"
      Tab(2).Control(37)=   "txtTA2(8)"
      Tab(2).Control(38)=   "txtTA2(9)"
      Tab(2).Control(39)=   "txtTA3(0)"
      Tab(2).Control(40)=   "txtTA3(1)"
      Tab(2).Control(41)=   "txtTA3(2)"
      Tab(2).Control(42)=   "txtTA3(3)"
      Tab(2).Control(43)=   "txtTA3(4)"
      Tab(2).Control(44)=   "txtTA3(5)"
      Tab(2).Control(45)=   "txtTA3(6)"
      Tab(2).Control(46)=   "txtTA3(7)"
      Tab(2).Control(47)=   "txtTA3(8)"
      Tab(2).Control(48)=   "txtTA3(9)"
      Tab(2).Control(49)=   "txtTA1(10)"
      Tab(2).Control(50)=   "txtTA1(11)"
      Tab(2).Control(51)=   "txtTA1(12)"
      Tab(2).Control(52)=   "txtTA1(13)"
      Tab(2).Control(53)=   "txtTA1(14)"
      Tab(2).Control(54)=   "txtTA1(15)"
      Tab(2).Control(55)=   "txtTA1(16)"
      Tab(2).Control(56)=   "txtTA1(17)"
      Tab(2).Control(57)=   "txtTA1(18)"
      Tab(2).Control(58)=   "txtTA1(19)"
      Tab(2).Control(59)=   "txtTA2(10)"
      Tab(2).Control(60)=   "txtTA2(11)"
      Tab(2).Control(61)=   "txtTA2(12)"
      Tab(2).Control(62)=   "txtTA2(13)"
      Tab(2).Control(63)=   "txtTA2(14)"
      Tab(2).Control(64)=   "txtTA2(15)"
      Tab(2).Control(65)=   "txtTA2(16)"
      Tab(2).Control(66)=   "txtTA2(17)"
      Tab(2).Control(67)=   "txtTA2(18)"
      Tab(2).Control(68)=   "txtTA2(19)"
      Tab(2).Control(69)=   "txtTA3(10)"
      Tab(2).Control(70)=   "txtTA3(11)"
      Tab(2).Control(71)=   "txtTA3(12)"
      Tab(2).Control(72)=   "txtTA3(13)"
      Tab(2).Control(73)=   "txtTA3(14)"
      Tab(2).Control(74)=   "txtTA3(15)"
      Tab(2).Control(75)=   "txtTA3(16)"
      Tab(2).Control(76)=   "txtTA3(17)"
      Tab(2).Control(77)=   "txtTA3(18)"
      Tab(2).Control(78)=   "txtTA3(19)"
      Tab(2).Control(79)=   "txtTA1(20)"
      Tab(2).Control(80)=   "txtTA1(21)"
      Tab(2).Control(81)=   "txtTA1(22)"
      Tab(2).Control(82)=   "txtTA1(23)"
      Tab(2).Control(83)=   "txtTA1(24)"
      Tab(2).Control(84)=   "txtTA1(25)"
      Tab(2).Control(85)=   "txtTA1(26)"
      Tab(2).Control(86)=   "txtTA1(27)"
      Tab(2).Control(87)=   "txtTA1(28)"
      Tab(2).Control(88)=   "txtTA1(29)"
      Tab(2).Control(89)=   "txtTA2(20)"
      Tab(2).Control(90)=   "txtTA2(21)"
      Tab(2).Control(91)=   "txtTA2(22)"
      Tab(2).Control(92)=   "txtTA2(23)"
      Tab(2).Control(93)=   "txtTA2(24)"
      Tab(2).Control(94)=   "txtTA2(25)"
      Tab(2).Control(95)=   "txtTA2(26)"
      Tab(2).Control(96)=   "txtTA2(27)"
      Tab(2).Control(97)=   "txtTA2(28)"
      Tab(2).Control(98)=   "txtTA2(29)"
      Tab(2).Control(99)=   "txtCyclesAveraged"
      Tab(2).Control(100)=   "txtOATnavCor"
      Tab(2).Control(101)=   "cmdWCT(0)"
      Tab(2).Control(102)=   "txtAverageZp"
      Tab(2).Control(103)=   "txtTA3(20)"
      Tab(2).Control(104)=   "txtTA3(21)"
      Tab(2).Control(105)=   "txtTA3(22)"
      Tab(2).Control(106)=   "txtTA3(23)"
      Tab(2).Control(107)=   "txtTA3(24)"
      Tab(2).Control(108)=   "txtTA3(25)"
      Tab(2).Control(109)=   "txtTA3(26)"
      Tab(2).Control(110)=   "txtTA3(27)"
      Tab(2).Control(111)=   "txtTA3(28)"
      Tab(2).Control(112)=   "txtTA3(29)"
      Tab(2).Control(113)=   "txtPrefix"
      Tab(2).Control(114)=   "tmrGetCycle2"
      Tab(2).Control(115)=   "txtAverageZt"
      Tab(2).Control(116)=   "txtAverageTt"
      Tab(2).Control(117)=   "txtAverageTn"
      Tab(2).ControlCount=   118
      TabCaption(3)   =   "&RAOB"
      TabPicture(3)   =   "MTPsim.frx":0058
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "Label(19)"
      Tab(3).Control(1)=   "Label(18)"
      Tab(3).Control(2)=   "Label(2)"
      Tab(3).Control(3)=   "Label6"
      Tab(3).Control(4)=   "lblTrop"
      Tab(3).Control(5)=   "lblTime"
      Tab(3).Control(6)=   "lblSite"
      Tab(3).Control(7)=   "cmdSelect(11)"
      Tab(3).Control(8)=   "cmdSelect(10)"
      Tab(3).Control(9)=   "txtColor(0)"
      Tab(3).Control(10)=   "cmdSelect(5)"
      Tab(3).Control(11)=   "txtTotalRec"
      Tab(3).Control(12)=   "chkMultipleScans"
      Tab(3).Control(13)=   "chkAutoView"
      Tab(3).Control(14)=   "cmdSelect(4)"
      Tab(3).Control(15)=   "cmdSelect(3)"
      Tab(3).Control(16)=   "cmdSelect(2)"
      Tab(3).Control(17)=   "cmdSelect(1)"
      Tab(3).Control(18)=   "cmdSelect(0)"
      Tab(3).Control(19)=   "lstSelect"
      Tab(3).Control(20)=   "txtThickness"
      Tab(3).Control(21)=   "UpDown1"
      Tab(3).Control(22)=   "Frame10"
      Tab(3).Control(23)=   "cmdSelect(9)"
      Tab(3).Control(24)=   "cmdSelect(8)"
      Tab(3).Control(25)=   "cmdATP(0)"
      Tab(3).Control(26)=   "Frame11"
      Tab(3).Control(27)=   "txtASCIIout"
      Tab(3).Control(28)=   "txtAVGopt(0)"
      Tab(3).Control(29)=   "txtAVGopt(1)"
      Tab(3).Control(30)=   "picGain"
      Tab(3).Control(31)=   "txtAVGopt(2)"
      Tab(3).ControlCount=   32
      TabCaption(4)   =   "&Retrieval"
      TabPicture(4)   =   "MTPsim.frx":0074
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame5"
      Tab(4).Control(1)=   "Frame4"
      Tab(4).Control(2)=   "Frame2"
      Tab(4).ControlCount=   3
      TabCaption(5)   =   "&Profile"
      TabPicture(5)   =   "MTPsim.frx":0090
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "chksjkPextension"
      Tab(5).Control(1)=   "Frame(14)"
      Tab(5).Control(2)=   "chkSinElCorrection"
      Tab(5).Control(3)=   "chkSingleFrequency"
      Tab(5).Control(4)=   "chkUSSA"
      Tab(5).Control(5)=   "txtTB(3)"
      Tab(5).Control(6)=   "txtTB(2)"
      Tab(5).Control(7)=   "txtTB(1)"
      Tab(5).Control(8)=   "txtTB(0)"
      Tab(5).Control(9)=   "chkFlag(3)"
      Tab(5).Control(10)=   "chkFlag(2)"
      Tab(5).Control(11)=   "chkFlag(1)"
      Tab(5).Control(12)=   "chkFlag(0)"
      Tab(5).Control(13)=   "txtFreq"
      Tab(5).Control(14)=   "cmdCalculateTB"
      Tab(5).Control(15)=   "txtZp"
      Tab(5).Control(16)=   "txtElev"
      Tab(5).Control(17)=   "txtSaveAs"
      Tab(5).Control(18)=   "Frame9(0)"
      Tab(5).Control(19)=   "txtRH"
      Tab(5).Control(20)=   "Label(57)"
      Tab(5).Control(21)=   "Label(56)"
      Tab(5).Control(22)=   "Label(55)"
      Tab(5).Control(23)=   "Label(17)"
      Tab(5).Control(24)=   "Label(15)"
      Tab(5).Control(25)=   "Label(14)"
      Tab(5).Control(26)=   "Label(12)"
      Tab(5).Control(27)=   "Label(27)"
      Tab(5).ControlCount=   28
      TabCaption(6)   =   "&Metrics"
      TabPicture(6)   =   "MTPsim.frx":00AC
      Tab(6).ControlEnabled=   -1  'True
      Tab(6).Control(0)=   "Label(1)"
      Tab(6).Control(0).Enabled=   0   'False
      Tab(6).Control(1)=   "Label(3)"
      Tab(6).Control(1).Enabled=   0   'False
      Tab(6).Control(2)=   "Label(4)"
      Tab(6).Control(2).Enabled=   0   'False
      Tab(6).Control(3)=   "Label(5)"
      Tab(6).Control(3).Enabled=   0   'False
      Tab(6).Control(4)=   "Label(8)"
      Tab(6).Control(4).Enabled=   0   'False
      Tab(6).Control(5)=   "lblRCname(0)"
      Tab(6).Control(5).Enabled=   0   'False
      Tab(6).Control(6)=   "lblRCname(1)"
      Tab(6).Control(6).Enabled=   0   'False
      Tab(6).Control(7)=   "lblRCname(2)"
      Tab(6).Control(7).Enabled=   0   'False
      Tab(6).Control(8)=   "lblRCname(3)"
      Tab(6).Control(8).Enabled=   0   'False
      Tab(6).Control(9)=   "lblRCname(4)"
      Tab(6).Control(9).Enabled=   0   'False
      Tab(6).Control(10)=   "lblRCname(5)"
      Tab(6).Control(10).Enabled=   0   'False
      Tab(6).Control(11)=   "lblRCname(6)"
      Tab(6).Control(11).Enabled=   0   'False
      Tab(6).Control(12)=   "lblRCname(7)"
      Tab(6).Control(12).Enabled=   0   'False
      Tab(6).Control(13)=   "lblRCname(8)"
      Tab(6).Control(13).Enabled=   0   'False
      Tab(6).Control(14)=   "lblRCname(9)"
      Tab(6).Control(14).Enabled=   0   'False
      Tab(6).Control(15)=   "lblRCname(10)"
      Tab(6).Control(15).Enabled=   0   'False
      Tab(6).Control(16)=   "lblRCname(11)"
      Tab(6).Control(16).Enabled=   0   'False
      Tab(6).Control(17)=   "lblRCname(12)"
      Tab(6).Control(17).Enabled=   0   'False
      Tab(6).Control(18)=   "lblRCname(13)"
      Tab(6).Control(18).Enabled=   0   'False
      Tab(6).Control(19)=   "Label(11)"
      Tab(6).Control(19).Enabled=   0   'False
      Tab(6).Control(20)=   "Label(26)"
      Tab(6).Control(20).Enabled=   0   'False
      Tab(6).Control(21)=   "lblRCname(14)"
      Tab(6).Control(21).Enabled=   0   'False
      Tab(6).Control(22)=   "lblRCname(15)"
      Tab(6).Control(22).Enabled=   0   'False
      Tab(6).Control(23)=   "Frame8"
      Tab(6).Control(23).Enabled=   0   'False
      Tab(6).Control(24)=   "txtMRI"
      Tab(6).Control(24).Enabled=   0   'False
      Tab(6).Control(25)=   "Frame(0)"
      Tab(6).Control(25).Enabled=   0   'False
      Tab(6).Control(26)=   "chkShowAllRetrievals"
      Tab(6).Control(26).Enabled=   0   'False
      Tab(6).Control(27)=   "chkShowArcAvg"
      Tab(6).Control(27).Enabled=   0   'False
      Tab(6).Control(28)=   "cmdAutoScaleTA"
      Tab(6).Control(28).Enabled=   0   'False
      Tab(6).Control(29)=   "picTA"
      Tab(6).Control(29).Enabled=   0   'False
      Tab(6).Control(30)=   "txtTC"
      Tab(6).Control(30).Enabled=   0   'False
      Tab(6).Control(31)=   "txtT1"
      Tab(6).Control(31).Enabled=   0   'False
      Tab(6).Control(32)=   "txtT2"
      Tab(6).Control(32).Enabled=   0   'False
      Tab(6).Control(33)=   "chkInterpolateATPs"
      Tab(6).Control(33).Enabled=   0   'False
      Tab(6).Control(34)=   "txtATPrange"
      Tab(6).Control(34).Enabled=   0   'False
      Tab(6).Control(35)=   "chkShowAAATP"
      Tab(6).Control(35).Enabled=   0   'False
      Tab(6).Control(36)=   "cmdSelectedObs"
      Tab(6).Control(36).Enabled=   0   'False
      Tab(6).Control(37)=   "txtTBnoise"
      Tab(6).Control(37).Enabled=   0   'False
      Tab(6).Control(38)=   "chkShowTotalError"
      Tab(6).Control(38).Enabled=   0   'False
      Tab(6).Control(39)=   "Frame(3)"
      Tab(6).Control(39).Enabled=   0   'False
      Tab(6).Control(40)=   "chkDefaultNoise"
      Tab(6).Control(40).Enabled=   0   'False
      Tab(6).Control(41)=   "chkDown"
      Tab(6).Control(41).Enabled=   0   'False
      Tab(6).Control(42)=   "chkRemoveAAbias"
      Tab(6).Control(42).Enabled=   0   'False
      Tab(6).ControlCount=   43
      TabCaption(7)   =   "&Blend"
      TabPicture(7)   =   "MTPsim.frx":00C8
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "Label4"
      Tab(7).Control(1)=   "Label(44)"
      Tab(7).Control(2)=   "Label(45)"
      Tab(7).Control(3)=   "Label(46)"
      Tab(7).Control(4)=   "Label(47)"
      Tab(7).Control(5)=   "Label(48)"
      Tab(7).Control(6)=   "Label(49)"
      Tab(7).Control(7)=   "Label(50)"
      Tab(7).Control(8)=   "Label(51)"
      Tab(7).Control(9)=   "txtDate2(0)"
      Tab(7).Control(10)=   "txtDate2(1)"
      Tab(7).Control(11)=   "optSite(0)"
      Tab(7).Control(12)=   "optSite(1)"
      Tab(7).Control(13)=   "txtDate1(0)"
      Tab(7).Control(14)=   "txtDate1(1)"
      Tab(7).Control(15)=   "cmdBlend(2)"
      Tab(7).Control(16)=   "cmdBlend(1)"
      Tab(7).Control(17)=   "txtFraction(1)"
      Tab(7).Control(18)=   "txtRec2(1)"
      Tab(7).Control(19)=   "txtRec1(1)"
      Tab(7).Control(20)=   "txtFraction(0)"
      Tab(7).Control(21)=   "txtRec2(0)"
      Tab(7).Control(22)=   "txtRec1(0)"
      Tab(7).Control(23)=   "txtSite(1)"
      Tab(7).Control(24)=   "txtSite(0)"
      Tab(7).Control(25)=   "Frame(13)"
      Tab(7).Control(26)=   "txtWait"
      Tab(7).Control(27)=   "chkWait"
      Tab(7).Control(28)=   "chkWriteBMPs"
      Tab(7).Control(29)=   "txtOATpp"
      Tab(7).Control(30)=   "txtOATraob"
      Tab(7).Control(31)=   "txtWeight(1)"
      Tab(7).Control(32)=   "txtALTkm"
      Tab(7).Control(33)=   "txtDistance(2)"
      Tab(7).Control(34)=   "txtDistance(1)"
      Tab(7).Control(35)=   "txtDistance(0)"
      Tab(7).Control(36)=   "txtEventNo"
      Tab(7).Control(37)=   "txtLATn(2)"
      Tab(7).Control(38)=   "txtLONn(2)"
      Tab(7).Control(39)=   "txtLONn(1)"
      Tab(7).Control(40)=   "txtLONn(0)"
      Tab(7).Control(41)=   "txtLATn(1)"
      Tab(7).Control(42)=   "txtLATn(0)"
      Tab(7).Control(43)=   "txtWeight(0)"
      Tab(7).Control(44)=   "txtEvent"
      Tab(7).Control(45)=   "cmdBlend(0)"
      Tab(7).Control(46)=   "lstEvent"
      Tab(7).Control(47)=   "chkUseSubstitutedRAWfile"
      Tab(7).Control(48)=   "cboPTfile"
      Tab(7).ControlCount=   49
      Begin VB.CheckBox chkRemoveAAbias 
         Caption         =   "AA Bias"
         Height          =   195
         Left            =   6435
         TabIndex        =   621
         Top             =   4365
         Width           =   960
      End
      Begin VB.CheckBox chkDown 
         Caption         =   "Down"
         Height          =   240
         Left            =   6435
         TabIndex        =   620
         Top             =   4590
         Width           =   870
      End
      Begin VB.ComboBox cboPTfile 
         Height          =   315
         Left            =   -68130
         TabIndex        =   619
         Text            =   "Combo1"
         Top             =   3000
         Visible         =   0   'False
         Width           =   525
      End
      Begin VB.CheckBox chkUseSubstitutedRAWfile 
         Caption         =   "Use Subs"
         Height          =   195
         Left            =   -68100
         TabIndex        =   618
         Top             =   2700
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.TextBox txtCHnLSB 
         Height          =   285
         Index           =   2
         Left            =   -70620
         MultiLine       =   -1  'True
         TabIndex        =   615
         Text            =   "MTPsim.frx":00E4
         Top             =   3450
         Width           =   375
      End
      Begin VB.TextBox txtCHnLSB 
         Height          =   285
         Index           =   1
         Left            =   -70620
         MultiLine       =   -1  'True
         TabIndex        =   614
         Text            =   "MTPsim.frx":00EA
         Top             =   3180
         Width           =   375
      End
      Begin VB.CheckBox chkFixChnRF 
         Caption         =   "Fix CH3 LSB %"
         Height          =   225
         Index           =   2
         Left            =   -72000
         TabIndex        =   613
         Top             =   3450
         Width           =   1365
      End
      Begin VB.CheckBox chkFixChnRF 
         Caption         =   "Fix CH2 LSB %"
         Height          =   195
         Index           =   1
         Left            =   -72000
         TabIndex        =   612
         Top             =   3210
         Width           =   1365
      End
      Begin VB.TextBox txtCHnLSB 
         Height          =   285
         Index           =   0
         Left            =   -70620
         MultiLine       =   -1  'True
         TabIndex        =   611
         Text            =   "MTPsim.frx":00F0
         Top             =   2910
         Width           =   375
      End
      Begin VB.CheckBox chkFixChnRF 
         Caption         =   "Fix CH1 LSB %"
         Height          =   225
         Index           =   0
         Left            =   -72000
         TabIndex        =   610
         Top             =   2970
         Width           =   1365
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Fix RC IFs"
         Height          =   435
         Left            =   -69600
         TabIndex        =   609
         Top             =   4980
         Width           =   2085
      End
      Begin VB.TextBox txtAVGopt 
         Height          =   285
         Index           =   2
         Left            =   -73560
         TabIndex        =   606
         Text            =   "Text1"
         Top             =   4020
         Width           =   585
      End
      Begin VB.PictureBox picGain 
         Height          =   285
         Left            =   -72930
         ScaleHeight     =   225
         ScaleWidth      =   315
         TabIndex        =   605
         Top             =   4020
         Width           =   375
      End
      Begin VB.TextBox txtAVGopt 
         Height          =   285
         Index           =   1
         Left            =   -74190
         TabIndex        =   604
         Text            =   "Text1"
         Top             =   4020
         Width           =   585
      End
      Begin VB.TextBox txtAVGopt 
         Height          =   285
         Index           =   0
         Left            =   -74820
         TabIndex        =   603
         Text            =   "Text1"
         Top             =   4020
         Width           =   585
      End
      Begin VB.TextBox txtASCIIout 
         Height          =   285
         Left            =   -71670
         TabIndex        =   602
         Text            =   "Text1"
         Top             =   3630
         Width           =   2625
      End
      Begin VB.CheckBox chksjkPextension 
         Caption         =   "Use SJK T Extension"
         Height          =   195
         Left            =   -69570
         TabIndex        =   601
         ToolTipText     =   "Use Incorrect SJK Temperature extension"
         Top             =   2160
         Width           =   1845
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB Extension"
         Height          =   645
         Index           =   14
         Left            =   -71760
         TabIndex        =   591
         Top             =   4830
         Width           =   4245
         Begin VB.TextBox txtLR1 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   1020
            MultiLine       =   -1  'True
            TabIndex        =   596
            Text            =   "MTPsim.frx":00F6
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtZb 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   1770
            MultiLine       =   -1  'True
            TabIndex        =   595
            Text            =   "MTPsim.frx":00FB
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtLR2 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   2610
            MultiLine       =   -1  'True
            TabIndex        =   594
            Text            =   "MTPsim.frx":0100
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtExtensionNoise 
            Alignment       =   1  'Right Justify
            Height          =   315
            Left            =   3570
            MultiLine       =   -1  'True
            TabIndex        =   593
            Text            =   "MTPsim.frx":0106
            Top             =   240
            Width           =   435
         End
         Begin VB.CheckBox chkEnableRAOBextension 
            Caption         =   "On"
            Height          =   255
            Left            =   90
            TabIndex        =   592
            ToolTipText     =   "Enable RAOB extension"
            Top             =   240
            Width           =   525
         End
         Begin VB.Label Label 
            Caption         =   "LR1"
            Height          =   255
            Index           =   68
            Left            =   690
            TabIndex        =   600
            Top             =   270
            Width           =   285
         End
         Begin VB.Label Label 
            Caption         =   "Zb"
            Height          =   255
            Index           =   69
            Left            =   1530
            TabIndex        =   599
            Top             =   270
            Width           =   285
         End
         Begin VB.Label Label 
            Caption         =   "LR2"
            Height          =   255
            Index           =   70
            Left            =   2280
            TabIndex        =   598
            Top             =   270
            Width           =   315
         End
         Begin VB.Label Label 
            Caption         =   "Noise"
            Height          =   255
            Index           =   71
            Left            =   3090
            TabIndex        =   597
            Top             =   270
            Width           =   465
         End
         Begin VB.Line Line1 
            X1              =   630
            X2              =   630
            Y1              =   240
            Y2              =   540
         End
      End
      Begin VB.CheckBox chkSinElCorrection 
         Caption         =   "Use Sin(El) Correction"
         Height          =   195
         Left            =   -69570
         TabIndex        =   590
         Top             =   1860
         Value           =   1  'Checked
         Width           =   1905
      End
      Begin VB.CheckBox chkSingleFrequency 
         Caption         =   "Single Frequency"
         Height          =   285
         Left            =   -69570
         TabIndex        =   589
         ToolTipText     =   "Do not use full IF passband"
         Top             =   2400
         Value           =   1  'Checked
         Width           =   1575
      End
      Begin VB.CheckBox chkUSSA 
         Caption         =   "Use US Std Atmosphere"
         Height          =   285
         Left            =   -69570
         TabIndex        =   588
         ToolTipText     =   "Do not use default RAOB profile"
         Top             =   2700
         Width           =   2025
      End
      Begin VB.TextBox txtTB 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   3
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   586
         Text            =   "MTPsim.frx":010C
         Top             =   4410
         Width           =   615
      End
      Begin VB.TextBox txtTB 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   2
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   584
         Text            =   "MTPsim.frx":0114
         Top             =   4050
         Width           =   615
      End
      Begin VB.TextBox txtTB 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   1
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   582
         Text            =   "MTPsim.frx":011A
         Top             =   3720
         Width           =   615
      End
      Begin VB.TextBox txtAverageTn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   573
         Top             =   4890
         Width           =   615
      End
      Begin VB.TextBox txtAverageTt 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   572
         Top             =   4890
         Width           =   615
      End
      Begin VB.TextBox txtAverageZt 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   570
         Top             =   5190
         Width           =   615
      End
      Begin VB.TextBox txtZtop 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72630
         MultiLine       =   -1  'True
         TabIndex        =   568
         Text            =   "MTPsim.frx":0120
         Top             =   3120
         Width           =   435
      End
      Begin VB.CheckBox chkExtendRAOB 
         Caption         =   "Extend RAOB to Ztop (km)"
         Height          =   225
         Left            =   -74880
         TabIndex        =   567
         Top             =   3150
         Value           =   1  'Checked
         Width           =   2205
      End
      Begin VB.Timer tmrGetCycle2 
         Left            =   -67980
         Top             =   4020
      End
      Begin VB.Frame Frame11 
         Caption         =   "Random RAOB Output"
         Height          =   855
         Left            =   -74910
         TabIndex        =   561
         Top             =   4590
         Width           =   6855
         Begin VB.CommandButton cmdConvertOut 
            Caption         =   "Output File"
            Height          =   255
            Left            =   120
            TabIndex        =   565
            ToolTipText     =   "Select random output file"
            Top             =   480
            Width           =   975
         End
         Begin VB.TextBox txtConvertOut 
            Height          =   285
            Left            =   1200
            TabIndex        =   564
            Text            =   "f:\RAOB\SOLVE\SOLVE1999.RAOB"
            Top             =   480
            Width           =   5535
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "New File"
            Height          =   255
            Index           =   0
            Left            =   1200
            TabIndex        =   563
            Top             =   240
            Width           =   975
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "Append"
            Height          =   255
            Index           =   1
            Left            =   2160
            TabIndex        =   562
            Top             =   240
            Width           =   975
         End
         Begin VB.Label lblLabel 
            Caption         =   "Mode"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   566
            Top             =   240
            Width           =   495
         End
      End
      Begin VB.CommandButton cmdATP 
         Caption         =   "Transfer       ATP"
         Height          =   615
         Index           =   0
         Left            =   -68880
         TabIndex        =   560
         ToolTipText     =   "Transfer ATP to RAOB plot"
         Top             =   3990
         Width           =   795
      End
      Begin VB.ListBox lstEvent 
         Height          =   1035
         ItemData        =   "MTPsim.frx":0123
         Left            =   -74880
         List            =   "MTPsim.frx":0125
         TabIndex        =   551
         Top             =   1950
         Width           =   1965
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   " &Import Events"
         Height          =   375
         Index           =   0
         Left            =   -74880
         TabIndex        =   550
         ToolTipText     =   "Read ELL File from MTPview"
         Top             =   3030
         Width           =   1455
      End
      Begin VB.TextBox txtEvent 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   -74880
         TabIndex        =   549
         Top             =   1590
         Width           =   1965
      End
      Begin VB.TextBox txtWeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69810
         MultiLine       =   -1  'True
         TabIndex        =   548
         Top             =   1590
         Width           =   465
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   547
         Top             =   1590
         Width           =   735
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   546
         Top             =   1950
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -71190
         MultiLine       =   -1  'True
         TabIndex        =   545
         Top             =   1590
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -71190
         MultiLine       =   -1  'True
         TabIndex        =   544
         Top             =   1950
         Width           =   735
      End
      Begin VB.TextBox txtLONn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -71190
         MultiLine       =   -1  'True
         TabIndex        =   543
         Top             =   2310
         Width           =   735
      End
      Begin VB.TextBox txtLATn 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   542
         Top             =   2310
         Width           =   735
      End
      Begin VB.TextBox txtEventNo 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -69960
         MultiLine       =   -1  'True
         TabIndex        =   541
         ToolTipText     =   "Record Number to be Written"
         Top             =   3060
         Width           =   585
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70350
         MultiLine       =   -1  'True
         TabIndex        =   540
         Top             =   1590
         Width           =   495
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70350
         MultiLine       =   -1  'True
         TabIndex        =   539
         Top             =   1950
         Width           =   495
      End
      Begin VB.TextBox txtDistance 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -70350
         MultiLine       =   -1  'True
         TabIndex        =   538
         Top             =   2310
         Width           =   495
      End
      Begin VB.TextBox txtALTkm 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   537
         Top             =   2670
         Width           =   735
      End
      Begin VB.TextBox txtWeight 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69810
         MultiLine       =   -1  'True
         TabIndex        =   536
         Top             =   1950
         Width           =   465
      End
      Begin VB.TextBox txtOATraob 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   535
         Top             =   3030
         Width           =   735
      End
      Begin VB.TextBox txtOATpp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71190
         MultiLine       =   -1  'True
         TabIndex        =   534
         Top             =   3030
         Width           =   495
      End
      Begin VB.CheckBox chkWriteBMPs 
         Caption         =   "Write Bit Maps"
         Height          =   285
         Left            =   -70680
         TabIndex        =   533
         Top             =   3450
         Width           =   1365
      End
      Begin VB.CheckBox chkWait 
         Caption         =   "Wait (s)"
         Height          =   225
         Left            =   -70680
         TabIndex        =   532
         Top             =   3780
         Width           =   855
      End
      Begin VB.TextBox txtWait 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -69690
         MultiLine       =   -1  'True
         TabIndex        =   531
         Text            =   "MTPsim.frx":0127
         Top             =   3720
         Width           =   345
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB     Avg      RMS"
         Height          =   1005
         Index           =   13
         Left            =   -74880
         TabIndex        =   524
         Top             =   4440
         Width           =   1935
         Begin VB.TextBox txtSrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   528
            Top             =   630
            Width           =   555
         End
         Begin VB.TextBox txtSavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   630
            MultiLine       =   -1  'True
            TabIndex        =   527
            Top             =   630
            Width           =   555
         End
         Begin VB.TextBox txtSrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   526
            Top             =   270
            Width           =   555
         End
         Begin VB.TextBox txtSavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   630
            MultiLine       =   -1  'True
            TabIndex        =   525
            Top             =   270
            Width           =   555
         End
         Begin VB.Label Label 
            Caption         =   "Site 2"
            Height          =   225
            Index           =   66
            Left            =   90
            TabIndex        =   530
            Top             =   660
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "Site 1"
            Height          =   225
            Index           =   65
            Left            =   90
            TabIndex        =   529
            Top             =   270
            Width           =   435
         End
      End
      Begin VB.TextBox txtSite 
         Height          =   285
         Index           =   0
         Left            =   -74700
         MultiLine       =   -1  'True
         TabIndex        =   523
         Top             =   630
         Width           =   615
      End
      Begin VB.TextBox txtSite 
         Height          =   285
         Index           =   1
         Left            =   -74700
         MultiLine       =   -1  'True
         TabIndex        =   521
         Top             =   990
         Width           =   615
      End
      Begin VB.TextBox txtRec1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70890
         MultiLine       =   -1  'True
         TabIndex        =   520
         Top             =   630
         Width           =   495
      End
      Begin VB.TextBox txtRec2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70380
         MultiLine       =   -1  'True
         TabIndex        =   519
         Top             =   630
         Width           =   495
      End
      Begin VB.TextBox txtFraction 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69840
         MultiLine       =   -1  'True
         TabIndex        =   518
         Top             =   630
         Width           =   465
      End
      Begin VB.TextBox txtRec1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70890
         MultiLine       =   -1  'True
         TabIndex        =   517
         Top             =   990
         Width           =   495
      End
      Begin VB.TextBox txtRec2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70380
         MultiLine       =   -1  'True
         TabIndex        =   516
         Top             =   990
         Width           =   495
      End
      Begin VB.TextBox txtFraction 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69840
         MultiLine       =   -1  'True
         TabIndex        =   515
         Top             =   990
         Width           =   465
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Temporal Avg"
         Height          =   375
         Index           =   1
         Left            =   -69300
         TabIndex        =   514
         ToolTipText     =   "Temporally Average Station 1 RAOBS"
         Top             =   510
         Width           =   1275
      End
      Begin VB.CommandButton cmdBlend 
         Caption         =   "Temporal Avg"
         Height          =   375
         Index           =   2
         Left            =   -69300
         TabIndex        =   513
         ToolTipText     =   "Temporally Average Station 2 RAOBs"
         Top             =   990
         Width           =   1275
      End
      Begin VB.TextBox txtDate1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -74010
         MultiLine       =   -1  'True
         TabIndex        =   512
         Top             =   990
         Width           =   1515
      End
      Begin VB.TextBox txtDate1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -74010
         MultiLine       =   -1  'True
         TabIndex        =   511
         Top             =   630
         Width           =   1515
      End
      Begin VB.OptionButton optSite 
         Height          =   195
         Index           =   1
         Left            =   -74940
         TabIndex        =   510
         Top             =   990
         Width           =   225
      End
      Begin VB.OptionButton optSite 
         Height          =   195
         Index           =   0
         Left            =   -74940
         TabIndex        =   509
         Top             =   630
         Width           =   225
      End
      Begin VB.TextBox txtDate2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -72450
         MultiLine       =   -1  'True
         TabIndex        =   508
         Top             =   990
         Width           =   1515
      End
      Begin VB.TextBox txtDate2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -72450
         MultiLine       =   -1  'True
         TabIndex        =   507
         Top             =   630
         Width           =   1515
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "T1"
         Height          =   375
         Index           =   8
         Left            =   -68850
         TabIndex        =   506
         ToolTipText     =   "Select First RAOB"
         Top             =   1560
         Width           =   465
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "T2"
         Height          =   375
         Index           =   9
         Left            =   -68340
         TabIndex        =   505
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   465
      End
      Begin VB.TextBox txtPrefix 
         Height          =   285
         Left            =   -69990
         TabIndex        =   503
         Text            =   "C:\M55\EUPLEX\"
         Top             =   4770
         Width           =   2445
      End
      Begin VB.Frame Frame10 
         Caption         =   "ATP GoTo"
         Height          =   615
         Left            =   -71730
         TabIndex        =   499
         Top             =   3960
         Width           =   2775
         Begin VB.TextBox txtATPgo 
            Height          =   285
            Left            =   120
            TabIndex        =   502
            Top             =   240
            Width           =   855
         End
         Begin VB.ComboBox cboATPgo 
            Height          =   315
            Left            =   960
            TabIndex        =   501
            Text            =   "Rec No"
            Top             =   240
            Width           =   1335
         End
         Begin VB.CommandButton cmdATPgo 
            Caption         =   "&Go"
            Height          =   495
            Left            =   2280
            TabIndex        =   500
            Top             =   120
            Width           =   495
         End
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   498
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   497
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   496
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   495
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   494
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   493
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   492
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   491
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   490
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -68670
         MultiLine       =   -1  'True
         TabIndex        =   489
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtAverageZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   488
         Top             =   5190
         Width           =   615
      End
      Begin VB.CommandButton cmdWCT 
         Caption         =   "&Calculate WCT"
         Height          =   435
         Index           =   0
         Left            =   -69990
         TabIndex        =   486
         ToolTipText     =   "Get MTP Observables from FLTinfo REF File"
         Top             =   5070
         Width           =   2445
      End
      Begin VB.TextBox txtOATnavCor 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   485
         Top             =   4590
         Width           =   615
      End
      Begin VB.TextBox txtCyclesAveraged 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   483
         Text            =   "MTPsim.frx":0129
         Top             =   4590
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   481
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   480
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   479
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   478
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   477
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   476
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   475
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   474
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   473
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -69330
         MultiLine       =   -1  'True
         TabIndex        =   472
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   29
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   471
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   28
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   470
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   27
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   469
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   26
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   468
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   25
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   467
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   24
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   466
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   23
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   465
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   22
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   464
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   21
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   463
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   20
         Left            =   -69990
         MultiLine       =   -1  'True
         TabIndex        =   462
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   455
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   454
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   453
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   452
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   451
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   450
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   449
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   448
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   447
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -73560
         MultiLine       =   -1  'True
         TabIndex        =   446
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   443
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   442
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   441
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   440
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   439
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   438
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   437
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   436
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   435
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -74220
         MultiLine       =   -1  'True
         TabIndex        =   434
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   19
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   433
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   18
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   432
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   17
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   431
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   16
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   430
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   15
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   429
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   14
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   428
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   13
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   427
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   12
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   426
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   11
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   425
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   10
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   424
         Top             =   960
         Width           =   615
      End
      Begin VB.CheckBox chkDefaultNoise 
         Caption         =   "Default Noise"
         Height          =   225
         Left            =   4860
         TabIndex        =   422
         Top             =   4620
         Width           =   1305
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB A/R"
         Height          =   5085
         Index           =   3
         Left            =   690
         TabIndex        =   389
         Top             =   390
         Width           =   1125
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   31
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   421
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   31
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   420
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   30
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   419
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   30
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   418
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   29
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   417
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   29
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   416
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   28
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   415
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   28
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   414
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   27
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   413
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   27
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   412
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   26
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   411
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   26
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   410
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   25
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   409
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   25
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   408
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   24
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   407
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   24
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   406
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   23
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   405
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   23
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   404
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   22
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   403
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   22
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   402
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   21
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   401
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   21
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   400
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   20
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   399
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   20
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   398
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   19
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   397
            Top             =   1140
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   19
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   396
            Top             =   1140
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   18
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   395
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   18
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   394
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   393
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   392
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   391
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   390
            Top             =   240
            Width           =   435
         End
      End
      Begin VB.TextBox txtTB 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   0
         Left            =   -70200
         MultiLine       =   -1  'True
         TabIndex        =   387
         Text            =   "MTPsim.frx":012B
         Top             =   3360
         Width           =   615
      End
      Begin VB.CheckBox chkFlag 
         Caption         =   "Include Liquid absorption"
         Height          =   315
         Index           =   3
         Left            =   -71790
         TabIndex        =   386
         Top             =   2700
         Width           =   2085
      End
      Begin VB.CheckBox chkFlag 
         Caption         =   "Include Vapor absorption"
         Height          =   315
         Index           =   2
         Left            =   -71790
         TabIndex        =   385
         Top             =   2400
         Value           =   1  'Checked
         Width           =   2085
      End
      Begin VB.CheckBox chkFlag 
         Caption         =   "Include N2 absorption"
         Height          =   315
         Index           =   1
         Left            =   -71790
         TabIndex        =   384
         Top             =   2100
         Value           =   1  'Checked
         Width           =   2085
      End
      Begin VB.CheckBox chkFlag 
         Caption         =   "Include O2 absorption"
         Height          =   315
         Index           =   0
         Left            =   -71790
         TabIndex        =   383
         Top             =   1800
         Value           =   1  'Checked
         Width           =   2085
      End
      Begin VB.TextBox txtFreq 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70230
         MultiLine       =   -1  'True
         TabIndex        =   382
         Text            =   "MTPsim.frx":0131
         Top             =   1140
         Width           =   585
      End
      Begin VB.CommandButton cmdCalculateTB 
         Caption         =   "Calculate TB"
         Height          =   435
         Left            =   -69090
         TabIndex        =   380
         Top             =   750
         Width           =   1185
      End
      Begin VB.TextBox txtZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70230
         MultiLine       =   -1  'True
         TabIndex        =   379
         Text            =   "MTPsim.frx":0139
         Top             =   810
         Width           =   585
      End
      Begin VB.TextBox txtElev 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -70230
         MultiLine       =   -1  'True
         TabIndex        =   376
         Text            =   "MTPsim.frx":013E
         Top             =   480
         Width           =   585
      End
      Begin VB.CheckBox chkShowTotalError 
         Caption         =   "Show Total Error"
         Height          =   225
         Left            =   4860
         TabIndex        =   373
         Top             =   4380
         Width           =   1515
      End
      Begin VB.TextBox txtTBnoise 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   6180
         MultiLine       =   -1  'True
         TabIndex        =   349
         Text            =   "MTPsim.frx":0143
         Top             =   4920
         Width           =   435
      End
      Begin VB.CommandButton cmdSelectedObs 
         Caption         =   "Sel Obs"
         Height          =   315
         Left            =   6720
         TabIndex        =   328
         ToolTipText     =   "Show Selected Observables in TA window"
         Top             =   5160
         Width           =   765
      End
      Begin VB.TextBox txtExcessGndT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -67980
         MultiLine       =   -1  'True
         TabIndex        =   326
         Text            =   "MTPsim.frx":0147
         Top             =   4590
         Width           =   435
      End
      Begin VB.CheckBox chkShowAAATP 
         Height          =   195
         Left            =   4860
         TabIndex        =   325
         Top             =   3660
         Width           =   195
      End
      Begin VB.TextBox txtATPrange 
         Height          =   285
         Left            =   6180
         TabIndex        =   324
         Text            =   "20.0"
         Top             =   5190
         Width           =   435
      End
      Begin VB.CheckBox chkInterpolateATPs 
         Caption         =   "Interpolate ATPs"
         Height          =   225
         Left            =   4860
         TabIndex        =   322
         Top             =   4140
         Width           =   1515
      End
      Begin ComCtl2.UpDown UpDown1 
         Height          =   285
         Left            =   -69390
         TabIndex        =   252
         Top             =   2760
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   503
         _Version        =   327681
         AutoBuddy       =   -1  'True
         BuddyControl    =   "txtThickness"
         BuddyDispid     =   196717
         OrigLeft        =   5760
         OrigTop         =   2880
         OrigRight       =   6000
         OrigBottom      =   3135
         SyncBuddy       =   -1  'True
         BuddyProperty   =   0
         Enabled         =   -1  'True
      End
      Begin VB.TextBox txtThickness 
         Height          =   285
         Left            =   -69630
         TabIndex        =   251
         Text            =   "1"
         Top             =   2760
         Width           =   255
      End
      Begin VB.CheckBox chkDisableBlending 
         Caption         =   "Disable Blending"
         Height          =   255
         Left            =   -72000
         TabIndex        =   249
         Top             =   2220
         Width           =   1815
      End
      Begin VB.CheckBox chkTexAQS 
         Caption         =   "Use TexAQS Scales"
         Height          =   255
         Left            =   -72000
         TabIndex        =   242
         Top             =   2460
         Width           =   1815
      End
      Begin VB.CheckBox chkProcessFile 
         Caption         =   "Process RAOB File"
         Height          =   255
         Left            =   -72000
         TabIndex        =   236
         Top             =   2700
         Width           =   1815
      End
      Begin VB.CheckBox chkWriteResults 
         Caption         =   "Write Results"
         Height          =   255
         Left            =   -74880
         TabIndex        =   235
         Top             =   3390
         Width           =   1575
      End
      Begin VB.TextBox txtT2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   7080
         MultiLine       =   -1  'True
         TabIndex        =   230
         Top             =   2790
         Width           =   375
      End
      Begin VB.TextBox txtT1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5160
         MultiLine       =   -1  'True
         TabIndex        =   229
         Top             =   2790
         Width           =   375
      End
      Begin VB.TextBox txtTC 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   6120
         MultiLine       =   -1  'True
         TabIndex        =   228
         Top             =   2910
         Width           =   375
      End
      Begin VB.PictureBox picTA 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00808080&
         Height          =   2175
         Left            =   5160
         ScaleHeight     =   2115
         ScaleWidth      =   2235
         TabIndex        =   227
         Top             =   630
         Width           =   2295
      End
      Begin VB.CommandButton cmdAutoScaleTA 
         Caption         =   " Manual    Scale"
         Height          =   375
         Left            =   5700
         TabIndex        =   226
         Top             =   3210
         Width           =   1335
      End
      Begin VB.CheckBox chkShowArcAvg 
         Caption         =   "Show Archive Average"
         Height          =   255
         Left            =   5160
         TabIndex        =   225
         Top             =   3630
         Value           =   1  'Checked
         Width           =   1935
      End
      Begin VB.CheckBox chkShowAllRetrievals 
         Caption         =   "Show All Retrievals"
         Height          =   255
         Left            =   4860
         TabIndex        =   224
         Top             =   3870
         Width           =   1935
      End
      Begin VB.Frame Frame 
         Caption         =   "ArcAvg A/R"
         Height          =   5085
         Index           =   0
         Left            =   1890
         TabIndex        =   211
         Top             =   390
         Width           =   1125
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   354
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   353
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   352
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   351
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   313
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   312
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   311
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   310
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   301
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   300
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   299
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   298
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   285
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   284
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   283
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   282
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   281
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   280
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   279
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   278
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   223
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   222
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   221
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   220
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   219
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   218
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   217
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   216
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   215
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   214
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtWavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   213
            Top             =   1140
            Width           =   435
         End
         Begin VB.TextBox txtWrms 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   212
            Top             =   1140
            Width           =   435
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "RC Files"
         Height          =   5085
         Index           =   2
         Left            =   -72480
         TabIndex        =   179
         Top             =   390
         Width           =   2895
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   15
            Left            =   720
            TabIndex        =   371
            Top             =   4710
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   15
            Left            =   1680
            TabIndex        =   370
            Top             =   4710
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   15
            Left            =   2040
            TabIndex        =   369
            Top             =   4710
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   15
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   368
            Top             =   4710
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   14
            Left            =   720
            TabIndex        =   366
            Top             =   4380
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   14
            Left            =   1680
            TabIndex        =   365
            Top             =   4380
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   14
            Left            =   2040
            TabIndex        =   364
            Top             =   4380
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   14
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   363
            Top             =   4380
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   13
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   347
            Top             =   4050
            Width           =   495
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   13
            Left            =   2040
            TabIndex        =   346
            Top             =   4050
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   13
            Left            =   1680
            TabIndex        =   345
            Top             =   4050
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   13
            Left            =   720
            TabIndex        =   344
            Top             =   4050
            Width           =   975
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   12
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   342
            Top             =   3750
            Width           =   495
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   12
            Left            =   2040
            TabIndex        =   341
            Top             =   3750
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   12
            Left            =   1680
            TabIndex        =   340
            Top             =   3750
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   12
            Left            =   720
            TabIndex        =   339
            Top             =   3750
            Width           =   975
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   11
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   337
            Top             =   3450
            Width           =   495
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   11
            Left            =   2040
            TabIndex        =   336
            Top             =   3450
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   11
            Left            =   1680
            TabIndex        =   335
            Top             =   3450
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   11
            Left            =   720
            TabIndex        =   334
            Top             =   3450
            Width           =   975
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   10
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   332
            Top             =   3150
            Width           =   495
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   10
            Left            =   2040
            TabIndex        =   331
            Top             =   3150
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   10
            Left            =   1680
            TabIndex        =   330
            Top             =   3150
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   10
            Left            =   720
            TabIndex        =   329
            Top             =   3150
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   9
            Left            =   720
            TabIndex        =   272
            Top             =   2850
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   9
            Left            =   1680
            TabIndex        =   271
            Top             =   2850
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   9
            Left            =   2040
            TabIndex        =   270
            Top             =   2850
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   9
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   269
            Top             =   2850
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   8
            Left            =   720
            TabIndex        =   267
            Top             =   2550
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   8
            Left            =   1680
            TabIndex        =   266
            Top             =   2550
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   8
            Left            =   2040
            TabIndex        =   265
            Top             =   2550
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   8
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   264
            Top             =   2550
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   7
            Left            =   720
            TabIndex        =   262
            Top             =   2250
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   7
            Left            =   1680
            TabIndex        =   261
            Top             =   2250
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   7
            Left            =   2040
            TabIndex        =   260
            Top             =   2250
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   7
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   259
            Top             =   2250
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   6
            Left            =   720
            TabIndex        =   257
            Top             =   1950
            Width           =   975
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   6
            Left            =   1680
            TabIndex        =   256
            Top             =   1950
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   6
            Left            =   2040
            TabIndex        =   255
            Top             =   1950
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   6
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   254
            Top             =   1950
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   0
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   210
            Top             =   150
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   1
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   209
            Top             =   450
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   2
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   208
            Top             =   750
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   3
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   207
            Top             =   1050
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   4
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   206
            Top             =   1350
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   5
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   205
            Top             =   1650
            Width           =   495
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   5
            Left            =   2040
            TabIndex        =   204
            Top             =   1650
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   4
            Left            =   2040
            TabIndex        =   203
            Top             =   1350
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   3
            Left            =   2040
            TabIndex        =   202
            Top             =   1050
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   2
            Left            =   2040
            TabIndex        =   201
            Top             =   750
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   1
            Left            =   2040
            TabIndex        =   200
            Top             =   450
            Width           =   255
         End
         Begin VB.OptionButton optRC 
            Caption         =   "Option1"
            Height          =   255
            Index           =   0
            Left            =   2040
            TabIndex        =   199
            Top             =   180
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   5
            Left            =   1680
            TabIndex        =   198
            Top             =   1650
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   4
            Left            =   1680
            TabIndex        =   197
            Top             =   1350
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   3
            Left            =   1680
            TabIndex        =   196
            Top             =   1050
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   2
            Left            =   1680
            TabIndex        =   195
            Top             =   810
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   1
            Left            =   1680
            TabIndex        =   194
            Top             =   450
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   0
            Left            =   1680
            TabIndex        =   193
            Top             =   150
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   5
            Left            =   720
            TabIndex        =   185
            Top             =   1650
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   4
            Left            =   720
            TabIndex        =   184
            Top             =   1350
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   3
            Left            =   720
            TabIndex        =   183
            Top             =   1050
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   2
            Left            =   720
            TabIndex        =   182
            Top             =   750
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   1
            Left            =   720
            TabIndex        =   181
            Top             =   450
            Width           =   975
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   0
            Left            =   720
            TabIndex        =   180
            Top             =   150
            Width           =   975
         End
         Begin VB.Label lblRC 
            Caption         =   "15"
            Height          =   165
            Index           =   15
            Left            =   120
            TabIndex        =   372
            Top             =   4710
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "14"
            Height          =   165
            Index           =   14
            Left            =   120
            TabIndex        =   367
            Top             =   4350
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "13"
            Height          =   255
            Index           =   13
            Left            =   120
            TabIndex        =   348
            Top             =   4020
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "12"
            Height          =   255
            Index           =   12
            Left            =   120
            TabIndex        =   343
            Top             =   3720
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "11"
            Height          =   255
            Index           =   11
            Left            =   120
            TabIndex        =   338
            Top             =   3420
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "10"
            Height          =   255
            Index           =   10
            Left            =   90
            TabIndex        =   333
            Top             =   3120
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "9"
            Height          =   255
            Index           =   9
            Left            =   120
            TabIndex        =   273
            Top             =   2820
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "8"
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   268
            Top             =   2550
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "7"
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   263
            Top             =   2250
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "6"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   258
            Top             =   1950
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "5"
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   191
            Top             =   1650
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "4"
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   190
            Top             =   1350
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "3"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   189
            Top             =   1050
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "2"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   188
            Top             =   750
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "1"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   187
            Top             =   510
            Width           =   615
         End
         Begin VB.Label lblRC 
            Caption         =   "0"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   186
            Top             =   180
            Width           =   615
         End
      End
      Begin VB.TextBox txtMRI 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5160
         MultiLine       =   -1  'True
         TabIndex        =   171
         Top             =   3210
         Width           =   375
      End
      Begin VB.Frame Frame8 
         Caption         =   "Obs Avg, RMS, lnP"
         Height          =   5085
         Left            =   3060
         TabIndex        =   158
         Top             =   390
         Width           =   1725
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   15
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   360
            Top             =   4740
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   15
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   359
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   358
            Top             =   4740
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   14
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   357
            Top             =   4440
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   14
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   356
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   355
            Top             =   4440
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   321
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   13
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   320
            Top             =   4140
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   13
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   319
            Top             =   4140
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   318
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   12
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   317
            Top             =   3840
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   12
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   316
            Top             =   3840
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   307
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   11
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   306
            Top             =   3540
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   11
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   305
            Top             =   3540
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   304
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   10
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   303
            Top             =   3240
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   10
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   302
            Top             =   3240
            Width           =   585
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   9
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   297
            Top             =   2940
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   9
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   296
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   295
            Top             =   2940
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   8
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   294
            Top             =   2640
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   8
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   293
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   292
            Top             =   2640
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   7
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   291
            Top             =   2340
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   7
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   290
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   289
            Top             =   2340
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   6
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   288
            Top             =   2040
            Width           =   585
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   6
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   287
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   286
            Top             =   2040
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   178
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   5
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   177
            Top             =   1740
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   5
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   176
            Top             =   1740
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   175
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   4
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   174
            Top             =   1440
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   4
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   173
            Top             =   1440
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   170
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   169
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   168
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   0
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   167
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   1
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   166
            Top             =   540
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   2
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   165
            Top             =   840
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   0
            Left            =   1035
            MultiLine       =   -1  'True
            TabIndex        =   164
            Text            =   "MTPsim.frx":014D
            Top             =   240
            Width           =   585
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   1
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   163
            Top             =   540
            Width           =   585
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   2
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   162
            Top             =   840
            Width           =   585
         End
         Begin VB.TextBox txtTBavg 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   90
            MultiLine       =   -1  'True
            TabIndex        =   161
            Top             =   1140
            Width           =   435
         End
         Begin VB.TextBox txtTBrms 
            Height          =   285
            Index           =   3
            Left            =   570
            MultiLine       =   -1  'True
            TabIndex        =   160
            Top             =   1140
            Width           =   435
         End
         Begin VB.TextBox txtlnP 
            Height          =   285
            Index           =   3
            Left            =   1050
            MultiLine       =   -1  'True
            TabIndex        =   159
            Top             =   1140
            Width           =   585
         End
      End
      Begin VB.TextBox txtRCpath 
         Height          =   375
         Left            =   -73800
         TabIndex        =   156
         Top             =   4290
         Width           =   1245
      End
      Begin VB.CommandButton cmdRCpath 
         Caption         =   "RC Path"
         Height          =   375
         Left            =   -74880
         TabIndex        =   155
         Top             =   4290
         Width           =   975
      End
      Begin VB.TextBox txtSaveAs 
         Height          =   285
         Left            =   -74880
         TabIndex        =   154
         Text            =   "C:\MTP\RAOB\ATP.TXT"
         Top             =   4590
         Width           =   3015
      End
      Begin VB.Frame Frame9 
         Caption         =   "Frame9"
         Height          =   3855
         Index           =   0
         Left            =   -74880
         TabIndex        =   144
         Top             =   660
         Width           =   2535
         Begin VB.CommandButton cmdZT 
            Caption         =   "Save As"
            Height          =   375
            Index           =   4
            Left            =   1560
            TabIndex        =   153
            Top             =   2280
            Width           =   855
         End
         Begin VB.CommandButton cmdZT 
            Caption         =   "Open"
            Height          =   375
            Index           =   3
            Left            =   1560
            TabIndex        =   152
            Top             =   1800
            Width           =   855
         End
         Begin VB.CommandButton cmdZT 
            Caption         =   "Clear"
            Height          =   375
            Index           =   2
            Left            =   1560
            TabIndex        =   151
            Top             =   1320
            Width           =   855
         End
         Begin VB.CommandButton cmdZT 
            Caption         =   "Sub"
            Height          =   375
            Index           =   1
            Left            =   1560
            TabIndex        =   150
            Top             =   840
            Width           =   855
         End
         Begin VB.CommandButton cmdZT 
            Caption         =   "Add"
            Height          =   375
            Index           =   0
            Left            =   1560
            TabIndex        =   149
            Top             =   360
            Width           =   855
         End
         Begin VB.ListBox lstT 
            Height          =   2985
            ItemData        =   "MTPsim.frx":0155
            Left            =   840
            List            =   "MTPsim.frx":0157
            TabIndex        =   148
            Top             =   720
            Width           =   615
         End
         Begin VB.ListBox lstZ 
            Height          =   2985
            ItemData        =   "MTPsim.frx":0159
            Left            =   120
            List            =   "MTPsim.frx":015B
            TabIndex        =   147
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   146
            Top             =   360
            Width           =   615
         End
         Begin VB.TextBox txtZ 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   145
            Top             =   360
            Width           =   615
         End
      End
      Begin VB.TextBox txtRH 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73440
         MultiLine       =   -1  'True
         TabIndex        =   143
         Top             =   4950
         Width           =   495
      End
      Begin VB.Frame Frame 
         Caption         =   "T-profile Source"
         Height          =   615
         Index           =   1
         Left            =   -74850
         TabIndex        =   138
         Top             =   3840
         Width           =   4935
         Begin VB.OptionButton optTsource 
            Caption         =   "User List"
            Height          =   255
            Index           =   2
            Left            =   3000
            TabIndex        =   141
            Top             =   240
            Width           =   975
         End
         Begin VB.OptionButton optTsource 
            Caption         =   "Std Atmosphere"
            Height          =   255
            Index           =   1
            Left            =   1320
            TabIndex        =   140
            Top             =   240
            Width           =   1455
         End
         Begin VB.OptionButton optTsource 
            Caption         =   "RAOB File"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   139
            Top             =   240
            Width           =   1215
         End
      End
      Begin VB.TextBox txtNRC 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74400
         MultiLine       =   -1  'True
         TabIndex        =   136
         Top             =   2730
         Width           =   495
      End
      Begin VB.TextBox txtNret 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74400
         MultiLine       =   -1  'True
         TabIndex        =   133
         Top             =   3450
         Width           =   495
      End
      Begin VB.TextBox txtNobs 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74400
         MultiLine       =   -1  'True
         TabIndex        =   132
         Top             =   3090
         Width           =   495
      End
      Begin VB.Frame Frame7 
         Caption         =   "pALTs to Retrieve"
         Height          =   2175
         Left            =   -69480
         TabIndex        =   123
         Top             =   2340
         Width           =   1935
         Begin VB.ComboBox cboPaltUnit 
            Height          =   315
            Left            =   960
            TabIndex        =   240
            Text            =   "km"
            Top             =   240
            Width           =   855
         End
         Begin VB.CommandButton cmdFL 
            Caption         =   "Default"
            Height          =   255
            Index           =   3
            Left            =   960
            TabIndex        =   129
            Top             =   1320
            Width           =   855
         End
         Begin VB.CommandButton cmdFL 
            Caption         =   "Clear"
            Height          =   255
            Index           =   2
            Left            =   960
            TabIndex        =   128
            Top             =   1080
            Width           =   855
         End
         Begin VB.CommandButton cmdFL 
            Caption         =   "Sub"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   127
            Top             =   840
            Width           =   855
         End
         Begin VB.CommandButton cmdFL 
            Caption         =   "Add"
            Height          =   255
            Index           =   0
            Left            =   960
            TabIndex        =   126
            Top             =   600
            Width           =   855
         End
         Begin VB.TextBox txtFL 
            Height          =   285
            Left            =   120
            TabIndex        =   125
            Top             =   240
            Width           =   735
         End
         Begin VB.ListBox lstFL 
            Height          =   1425
            ItemData        =   "MTPsim.frx":015D
            Left            =   120
            List            =   "MTPsim.frx":015F
            TabIndex        =   124
            Top             =   600
            Width           =   735
         End
         Begin VB.Label Label 
            Caption         =   "List Items are in km"
            Height          =   375
            Index           =   22
            Left            =   960
            TabIndex        =   241
            Top             =   1680
            Width           =   855
         End
      End
      Begin VB.TextBox txtlatitude 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73200
         MultiLine       =   -1  'True
         TabIndex        =   119
         Text            =   "MTPsim.frx":0161
         Top             =   2670
         Width           =   495
      End
      Begin VB.Frame Frame1 
         Caption         =   "Observable Order"
         Height          =   615
         Left            =   -74880
         TabIndex        =   115
         Top             =   1950
         Width           =   2295
         Begin VB.OptionButton optOBorder 
            Caption         =   "SJK"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   118
            Top             =   240
            Width           =   615
         End
         Begin VB.OptionButton optOBorder 
            Caption         =   "Int"
            Height          =   255
            Index           =   1
            Left            =   840
            TabIndex        =   117
            Top             =   240
            Width           =   615
         End
         Begin VB.OptionButton optOBorder 
            Caption         =   "MJM"
            Height          =   255
            Index           =   2
            Left            =   1440
            TabIndex        =   116
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "RC pALTs [km]"
         Height          =   3135
         Left            =   -70200
         TabIndex        =   106
         Top             =   780
         Width           =   2175
         Begin VB.TextBox txtNFL 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1560
            MultiLine       =   -1  'True
            TabIndex        =   113
            Top             =   2760
            Width           =   375
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Default"
            Height          =   375
            Index           =   3
            Left            =   1200
            TabIndex        =   112
            Top             =   1320
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Clear"
            Height          =   375
            Index           =   2
            Left            =   1200
            TabIndex        =   111
            Top             =   960
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Remove"
            Height          =   375
            Index           =   1
            Left            =   1200
            TabIndex        =   110
            Top             =   600
            Width           =   855
         End
         Begin VB.CommandButton cmdPalt 
            Caption         =   "&Add"
            Height          =   375
            Index           =   0
            Left            =   1200
            TabIndex        =   109
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstPalt 
            Height          =   2400
            ItemData        =   "MTPsim.frx":0164
            Left            =   120
            List            =   "MTPsim.frx":0166
            TabIndex        =   108
            Top             =   600
            Width           =   975
         End
         Begin VB.TextBox txtPalt 
            Height          =   285
            Left            =   120
            TabIndex        =   107
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "NFL"
            Height          =   255
            Index           =   6
            Left            =   1200
            TabIndex        =   114
            Top             =   2760
            Width           =   375
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "IF Offsets and Weights"
         Height          =   3135
         Left            =   -69480
         TabIndex        =   104
         Top             =   390
         Width           =   1935
         Begin VB.ListBox lstSU 
            Height          =   2595
            ItemData        =   "MTPsim.frx":0168
            Left            =   120
            List            =   "MTPsim.frx":016A
            TabIndex        =   105
            Top             =   360
            Width           =   1695
         End
      End
      Begin VB.CheckBox chkOldDC8RC 
         Caption         =   "Use Old DC8 RC Format"
         Height          =   255
         Left            =   -74880
         TabIndex        =   102
         Top             =   3990
         Width           =   2055
      End
      Begin VB.CheckBox chkAutoRetrieve 
         Caption         =   "Do Retrieval on Fwd/Back"
         Height          =   255
         Left            =   -74880
         TabIndex        =   101
         Top             =   2880
         Value           =   1  'Checked
         Width           =   2295
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   99
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   98
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   97
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   96
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   95
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   94
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   93
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   92
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   91
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -71100
         MultiLine       =   -1  'True
         TabIndex        =   90
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   89
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   88
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   87
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   86
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   85
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   84
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   83
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   82
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   81
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -71760
         MultiLine       =   -1  'True
         TabIndex        =   80
         Top             =   960
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   9
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   79
         Top             =   4200
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   8
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   78
         Top             =   3840
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   7
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   77
         Top             =   3480
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   6
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   76
         Top             =   3120
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   5
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   75
         Top             =   2760
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   74
         Top             =   2400
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   73
         Top             =   2040
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   72
         Top             =   1680
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   71
         Top             =   1320
         Width           =   615
      End
      Begin VB.TextBox txtTA1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -72420
         MultiLine       =   -1  'True
         TabIndex        =   70
         Top             =   960
         Width           =   615
      End
      Begin VB.CheckBox chkAutoClear 
         Caption         =   "Auto Clear Plot"
         Height          =   255
         Left            =   -74880
         TabIndex        =   67
         Top             =   2640
         Value           =   1  'Checked
         Width           =   2175
      End
      Begin VB.Frame Frame4 
         Caption         =   "EL Angles"
         Height          =   3135
         Left            =   -74880
         TabIndex        =   55
         Top             =   780
         Width           =   1935
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Default"
            Height          =   375
            Index           =   3
            Left            =   960
            TabIndex        =   62
            Top             =   1320
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Clear"
            Height          =   375
            Index           =   2
            Left            =   960
            TabIndex        =   61
            Top             =   960
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Remove"
            Height          =   375
            Index           =   1
            Left            =   960
            TabIndex        =   60
            Top             =   600
            Width           =   855
         End
         Begin VB.CommandButton cmdEL 
            Caption         =   "&Add"
            Height          =   375
            Index           =   0
            Left            =   960
            TabIndex        =   59
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstEL 
            Height          =   2400
            ItemData        =   "MTPsim.frx":016C
            Left            =   120
            List            =   "MTPsim.frx":016E
            TabIndex        =   58
            Top             =   600
            Width           =   735
         End
         Begin VB.TextBox txtEL 
            Height          =   285
            Left            =   120
            TabIndex        =   57
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox txtNel 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   56
            Top             =   2760
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "Nel"
            Height          =   255
            Index           =   9
            Left            =   1440
            TabIndex        =   63
            Top             =   2760
            Width           =   375
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "LO Frequencies"
         Height          =   3135
         Left            =   -72600
         TabIndex        =   47
         Top             =   780
         Width           =   2175
         Begin VB.TextBox txtLO 
            Height          =   285
            Left            =   120
            TabIndex        =   103
            Top             =   240
            Width           =   975
         End
         Begin VB.TextBox txtNlo 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   53
            Top             =   2760
            Width           =   255
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Default"
            Height          =   375
            Index           =   3
            Left            =   1200
            TabIndex        =   52
            Top             =   1320
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Clear"
            Height          =   375
            Index           =   2
            Left            =   1200
            TabIndex        =   51
            Top             =   960
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Remove"
            Height          =   375
            Index           =   1
            Left            =   1200
            TabIndex        =   50
            Top             =   600
            Width           =   855
         End
         Begin VB.CommandButton cmdLO 
            Caption         =   "Add"
            Height          =   375
            Index           =   0
            Left            =   1200
            TabIndex        =   49
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstLO 
            Height          =   2400
            ItemData        =   "MTPsim.frx":0170
            Left            =   120
            List            =   "MTPsim.frx":0172
            TabIndex        =   48
            Top             =   600
            Width           =   975
         End
         Begin VB.Label Label 
            Caption         =   "Nlo"
            Height          =   255
            Index           =   7
            Left            =   1560
            TabIndex        =   54
            Top             =   2760
            Width           =   375
         End
      End
      Begin VB.CheckBox chkGeoPotHeight 
         Caption         =   "Plot Geopotential Height"
         Height          =   255
         Left            =   -74880
         TabIndex        =   46
         Top             =   2400
         Width           =   2295
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "&Open RAOB"
         Height          =   345
         Left            =   -74880
         TabIndex        =   45
         Top             =   4500
         Width           =   1215
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
         ItemData        =   "MTPsim.frx":0174
         Left            =   -74880
         List            =   "MTPsim.frx":0176
         TabIndex        =   31
         Top             =   600
         Width           =   3735
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Search"
         Height          =   375
         Index           =   0
         Left            =   -71040
         TabIndex        =   30
         ToolTipText     =   "Search Open RAOB file subject to Filters"
         Top             =   1560
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Write"
         Height          =   375
         Index           =   1
         Left            =   -68880
         TabIndex        =   29
         ToolTipText     =   "Write all list files to RAOB out file in I/O tab"
         Top             =   600
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Remove"
         Height          =   375
         Index           =   2
         Left            =   -69960
         TabIndex        =   28
         ToolTipText     =   "Remove selected RAOB from list"
         Top             =   600
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&View"
         Height          =   375
         Index           =   3
         Left            =   -69960
         TabIndex        =   27
         ToolTipText     =   "Plot RAOB (see Auto Plot)"
         Top             =   1080
         Width           =   975
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Clear &List"
         Height          =   375
         Index           =   4
         Left            =   -71040
         TabIndex        =   26
         ToolTipText     =   "Clear list window"
         Top             =   1080
         Width           =   975
      End
      Begin VB.CheckBox chkAutoView 
         Caption         =   "Auto View on Selection"
         Height          =   195
         Left            =   -71040
         TabIndex        =   25
         ToolTipText     =   "Plot selected RAOB from list"
         Top             =   2040
         Width           =   2175
      End
      Begin VB.CheckBox chkMultipleScans 
         Caption         =   "Plot Multiple Scans"
         Height          =   255
         Left            =   -71040
         TabIndex        =   24
         ToolTipText     =   "Plot successive selections on same plot"
         Top             =   2280
         Width           =   2055
      End
      Begin VB.TextBox txtTotalRec 
         Height          =   285
         Left            =   -71040
         TabIndex        =   23
         ToolTipText     =   "Total number of RAOBs in list"
         Top             =   2760
         Width           =   735
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Clear &Plot"
         Height          =   375
         Index           =   5
         Left            =   -71040
         TabIndex        =   22
         ToolTipText     =   "Clear plot window"
         Top             =   600
         Width           =   975
      End
      Begin VB.TextBox txtColor 
         Height          =   285
         Index           =   0
         Left            =   -70080
         TabIndex        =   21
         Text            =   "13"
         Top             =   2760
         Width           =   375
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "BG"
         Height          =   375
         Index           =   10
         Left            =   -69960
         TabIndex        =   20
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   375
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Avg"
         Height          =   375
         Index           =   11
         Left            =   -69480
         TabIndex        =   19
         ToolTipText     =   "Select Second RAOB for Time Interpolation"
         Top             =   1560
         Width           =   495
      End
      Begin VB.TextBox txtRAOBfile 
         Height          =   315
         Left            =   -73560
         TabIndex        =   18
         Top             =   4500
         Width           =   3615
      End
      Begin VB.TextBox txtREFfile 
         Enabled         =   0   'False
         Height          =   315
         Left            =   -73560
         TabIndex        =   14
         Top             =   4860
         Width           =   3615
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
         TabIndex        =   13
         Top             =   1920
         Width           =   7335
      End
      Begin VB.ComboBox cboUSE 
         Height          =   315
         ItemData        =   "MTPsim.frx":0178
         Left            =   -73740
         List            =   "MTPsim.frx":017A
         TabIndex        =   12
         Top             =   3330
         Width           =   975
      End
      Begin VB.Frame fraLAT 
         Caption         =   "Algorithm"
         Height          =   1335
         Left            =   -74880
         TabIndex        =   2
         Top             =   390
         Width           =   2295
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   7
            Top             =   960
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   6
            Top             =   960
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   5
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   4
            Top             =   600
            Width           =   495
         End
         Begin VB.ComboBox cboAlgorithm 
            Height          =   315
            Left            =   120
            TabIndex        =   3
            Top             =   240
            Width           =   2055
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT4"
            Height          =   255
            Index           =   3
            Left            =   1200
            TabIndex        =   11
            Top             =   960
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT3"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   10
            Top             =   960
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT2"
            Height          =   255
            Index           =   1
            Left            =   1200
            TabIndex        =   9
            Top             =   600
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT1"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   8
            Top             =   600
            Width           =   495
         End
      End
      Begin VB.Label Label 
         Caption         =   "TB - (TB+ + TB-)/2"
         Height          =   345
         Index           =   57
         Left            =   -71790
         TabIndex        =   587
         Top             =   4410
         Width           =   1635
      End
      Begin VB.Label Label 
         Caption         =   "TB (-HPBW/2)"
         Height          =   345
         Index           =   56
         Left            =   -71790
         TabIndex        =   585
         Top             =   4050
         Width           =   1635
      End
      Begin VB.Label Label 
         Caption         =   "Brightness Temp (K)"
         Height          =   345
         Index           =   55
         Left            =   -71790
         TabIndex        =   583
         Top             =   3720
         Width           =   1635
      End
      Begin VB.Label Label 
         Caption         =   "Average Tn (K)"
         Height          =   285
         Index           =   54
         Left            =   -74880
         TabIndex        =   574
         Top             =   4920
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "Average Tt (K)"
         Height          =   285
         Index           =   53
         Left            =   -72420
         TabIndex        =   571
         Top             =   4920
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "Average Zt (km)"
         Height          =   285
         Index           =   52
         Left            =   -72420
         TabIndex        =   569
         Top             =   5190
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "UTsec    Alt [km]"
         Height          =   255
         Index           =   51
         Left            =   -74880
         TabIndex        =   559
         Top             =   1350
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Latitude      Longitude   D [km]"
         Height          =   255
         Index           =   50
         Left            =   -72030
         TabIndex        =   558
         Top             =   1350
         Width           =   2295
      End
      Begin VB.Label Label 
         Caption         =   "Site 1"
         Height          =   255
         Index           =   49
         Left            =   -72630
         TabIndex        =   557
         Top             =   1590
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Site 2"
         Height          =   255
         Index           =   48
         Left            =   -72630
         TabIndex        =   556
         Top             =   1950
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Event"
         Height          =   255
         Index           =   47
         Left            =   -72630
         TabIndex        =   555
         Top             =   2310
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Record"
         Height          =   255
         Index           =   46
         Left            =   -70620
         TabIndex        =   554
         Top             =   3060
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Alt [km]"
         Height          =   255
         Index           =   45
         Left            =   -72630
         TabIndex        =   553
         Top             =   2670
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "OAT [K]"
         Height          =   255
         Index           =   44
         Left            =   -72630
         TabIndex        =   552
         Top             =   3030
         Width           =   615
      End
      Begin VB.Label Label4 
         Caption         =   " Site            Date1       Time 1       Date2       Time 2  Rec 1  Rec 2    Wt"
         Height          =   255
         Left            =   -74580
         TabIndex        =   522
         Top             =   390
         Width           =   5235
      End
      Begin VB.Label Label 
         Caption         =   "REF File Path"
         Height          =   285
         Index           =   67
         Left            =   -69990
         TabIndex        =   504
         Top             =   4560
         Width           =   1065
      End
      Begin VB.Label Label 
         Caption         =   "Average Zp (km)"
         Height          =   285
         Index           =   42
         Left            =   -74880
         TabIndex        =   487
         Top             =   5220
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "OATnavCor (K)"
         Height          =   285
         Index           =   41
         Left            =   -72420
         TabIndex        =   484
         Top             =   4650
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Cycles Averaged"
         Height          =   285
         Index           =   40
         Left            =   -74910
         TabIndex        =   482
         Top             =   4620
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "WCT = Predicted - Measured"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Index           =   39
         Left            =   -70200
         TabIndex        =   461
         Top             =   390
         Width           =   2535
      End
      Begin VB.Label Label 
         Caption         =   "CH 1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   38
         Left            =   -69870
         TabIndex        =   460
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   37
         Left            =   -69180
         TabIndex        =   459
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 3"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Index           =   36
         Left            =   -68520
         TabIndex        =   458
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Measured Observables"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Index           =   35
         Left            =   -74880
         TabIndex        =   457
         Top             =   390
         Width           =   1965
      End
      Begin VB.Label Label 
         Caption         =   "Predicted Observables"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Index           =   34
         Left            =   -72420
         TabIndex        =   456
         Top             =   390
         Width           =   1965
      End
      Begin VB.Label Label 
         Caption         =   "CH 2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   33
         Left            =   -74100
         TabIndex        =   445
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 3"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Index           =   32
         Left            =   -73440
         TabIndex        =   444
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   28
         Left            =   -74730
         TabIndex        =   423
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "TB (+HPBW/2)"
         Height          =   345
         Index           =   17
         Left            =   -71790
         TabIndex        =   388
         Top             =   3360
         Width           =   1635
      End
      Begin VB.Label Label 
         Caption         =   "Frequency (GHz)"
         Height          =   225
         Index           =   15
         Left            =   -71820
         TabIndex        =   381
         Top             =   1200
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "Pressure Altitude (km)"
         Height          =   225
         Index           =   14
         Left            =   -71820
         TabIndex        =   378
         Top             =   870
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "Elevation Angle (deg)"
         Height          =   225
         Index           =   12
         Left            =   -71820
         TabIndex        =   377
         Top             =   510
         Width           =   1575
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC16"
         Height          =   195
         Index           =   15
         Left            =   120
         TabIndex        =   362
         Top             =   5130
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC15"
         Height          =   195
         Index           =   14
         Left            =   120
         TabIndex        =   361
         Top             =   4830
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Extra TB Noise (K)"
         Height          =   195
         Index           =   26
         Left            =   4860
         TabIndex        =   350
         Top             =   4950
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Excess Gnd Temp (C)"
         Height          =   195
         Index           =   23
         Left            =   -69660
         TabIndex        =   327
         Top             =   4620
         Width           =   1635
      End
      Begin VB.Label Label 
         Caption         =   "ATP Range (km)"
         Height          =   255
         Index           =   11
         Left            =   4860
         TabIndex        =   323
         Top             =   5220
         Width           =   1275
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC14"
         Height          =   315
         Index           =   13
         Left            =   120
         TabIndex        =   315
         Top             =   4560
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC13"
         Height          =   255
         Index           =   12
         Left            =   120
         TabIndex        =   314
         Top             =   4260
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC12"
         Height          =   255
         Index           =   11
         Left            =   120
         TabIndex        =   309
         Top             =   3990
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC11"
         Height          =   255
         Index           =   10
         Left            =   120
         TabIndex        =   308
         Top             =   3690
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC10"
         Height          =   255
         Index           =   9
         Left            =   120
         TabIndex        =   277
         Top             =   3390
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC9"
         Height          =   255
         Index           =   8
         Left            =   120
         TabIndex        =   276
         Top             =   3090
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC8"
         Height          =   255
         Index           =   7
         Left            =   120
         TabIndex        =   275
         Top             =   2760
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC7"
         Height          =   255
         Index           =   6
         Left            =   120
         TabIndex        =   274
         Top             =   2490
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC6"
         Height          =   255
         Index           =   5
         Left            =   120
         TabIndex        =   248
         Top             =   2190
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC5"
         Height          =   255
         Index           =   4
         Left            =   120
         TabIndex        =   247
         Top             =   1890
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC4"
         Height          =   255
         Index           =   3
         Left            =   120
         TabIndex        =   246
         Top             =   1590
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC3"
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   245
         Top             =   1290
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC2"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   244
         Top             =   990
         Width           =   615
      End
      Begin VB.Label lblRCname 
         Caption         =   "RC1"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   243
         Top             =   690
         Width           =   615
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
         TabIndex        =   239
         Top             =   3180
         Width           =   3735
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
         TabIndex        =   238
         Top             =   3420
         Width           =   3735
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
         Left            =   -74850
         TabIndex        =   237
         Top             =   3720
         Width           =   7215
      End
      Begin VB.Label Label 
         Caption         =   "Scan Angle vs TB"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   8
         Left            =   5520
         TabIndex        =   234
         Top             =   390
         Width           =   1575
      End
      Begin VB.Label Label 
         Caption         =   "10"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   5
         Left            =   4920
         TabIndex        =   233
         Top             =   2670
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "5"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   4
         Left            =   4920
         TabIndex        =   232
         Top             =   1590
         Width           =   135
      End
      Begin VB.Label Label 
         Caption         =   "1"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   3
         Left            =   4920
         TabIndex        =   231
         Top             =   510
         Width           =   135
      End
      Begin VB.Label Label 
         Caption         =   "Change Observable Order, and RC format before selecting new RC type"
         Height          =   615
         Index           =   21
         Left            =   -69480
         TabIndex        =   192
         Top             =   3990
         Width           =   1935
      End
      Begin VB.Label Label 
         Caption         =   "MRI"
         Height          =   255
         Index           =   1
         Left            =   4830
         TabIndex        =   172
         Top             =   3240
         Width           =   345
      End
      Begin VB.Label Label 
         Caption         =   "Relative Humidity"
         Height          =   255
         Index           =   27
         Left            =   -74880
         TabIndex        =   142
         Top             =   4950
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "NRC"
         Height          =   255
         Index           =   25
         Left            =   -74880
         TabIndex        =   135
         Top             =   2730
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Nret"
         Height          =   255
         Index           =   16
         Left            =   -74880
         TabIndex        =   131
         Top             =   3450
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Nobs"
         Height          =   255
         Index           =   0
         Left            =   -74880
         TabIndex        =   130
         Top             =   3090
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Lat"
         Height          =   255
         Index           =   24
         Left            =   -73680
         TabIndex        =   120
         Top             =   2670
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 3"
         ForeColor       =   &H00FF0000&
         Height          =   255
         Index           =   29
         Left            =   -70950
         TabIndex        =   66
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 2"
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Index           =   30
         Left            =   -71610
         TabIndex        =   65
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "CH 1"
         ForeColor       =   &H000000FF&
         Height          =   255
         Index           =   31
         Left            =   -72300
         TabIndex        =   64
         Top             =   720
         Width           =   375
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
         TabIndex        =   35
         Top             =   360
         Width           =   3975
      End
      Begin VB.Label Label 
         Caption         =   "Total Rec"
         Height          =   255
         Index           =   2
         Left            =   -71040
         TabIndex        =   34
         Top             =   2520
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Color"
         Height          =   255
         Index           =   18
         Left            =   -70080
         TabIndex        =   33
         Top             =   2520
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Thick"
         Height          =   255
         Index           =   19
         Left            =   -69600
         TabIndex        =   32
         Top             =   2520
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Output REF File"
         Height          =   225
         Index           =   43
         Left            =   -74880
         TabIndex        =   17
         Top             =   4860
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "USE File"
         Height          =   255
         Index           =   20
         Left            =   -73740
         TabIndex        =   16
         Top             =   3090
         Width           =   615
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
         Index           =   13
         Left            =   -74880
         TabIndex        =   15
         Top             =   1680
         Width           =   6495
      End
   End
   Begin VB.Label Label 
      Caption         =   "Filter"
      Height          =   255
      Index           =   58
      Left            =   8670
      TabIndex        =   608
      Top             =   1050
      Width           =   405
   End
   Begin VB.Label Label1 
      Caption         =   "ATP Bias (K)"
      Height          =   285
      Left            =   8670
      TabIndex        =   576
      Top             =   2610
      Width           =   915
   End
   Begin VB.Label Label 
      Caption         =   "Current"
      Height          =   255
      Index           =   10
      Left            =   9510
      TabIndex        =   69
      Top             =   2850
      Width           =   615
   End
   Begin VB.Label lblProgress 
      Caption         =   "Records"
      Height          =   255
      Left            =   8670
      TabIndex        =   44
      Top             =   2850
      Width           =   855
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpenBinary 
         Caption         =   "&Open RAOB File"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
         Shortcut        =   {F12}
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsFormatRAOB 
         Caption         =   "&Format RAOB"
      End
      Begin VB.Menu mnuOptionsShowNDP 
         Caption         =   "Show &NDP"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmMTPsim"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' UID yyyymmdd Comment
' mjm 19990730 Added look ahead at A-line to calculate average pALT and OATnav
' mjm 19990727 Removed MainER and MainDC, changed all gains to arrays (1 to Channels)
'
Public yymmdd$, Progress!, ShowZp As Boolean, ShowDiagnostics As Boolean
Public SelectedRCset%, SelectedFL%, Cycle2% ', Cycle%
Dim Head$(1 To 6), t1_Sum!, t2_Sum!, RCs$
Dim MV!(0 To 18, 1 To 200), MVindex%, MVindexMax%
Dim GV!(0 To 6, 1 To 3, 1 To 200), GVindex%, GVindexMax%
Dim rtt!(1 To 33), ob!(0 To 32)    'Raw observables ob 0-19 for ER2 0-32 for DC8
Dim RT1!(1 To 33), RT2!(1 To 33), Pause As Boolean, ShowNDP As Boolean
Dim SaveOutput As Boolean, DoRetrieval As Boolean, Lindex%
Dim Rinterp!(1 To 5)
Dim EventList$(1 To 20), iTAstart%
Dim Site1 As Boolean, TimeSec&, Ltn!, Lnn!, EventNumber&, EventTime&
Dim LR11!, LR12!, Zb1!, LR21!, LR22!, Zb2!, Ztop!
Dim DefaultBG As Boolean
Dim AutoView As Boolean
Dim DTevent As Date
Dim TropMin!, TropMax!

Dim T1#(1 To 33), T2#(1 To 33), RTavg!(1 To 33), RAavg!(1 To 33), RTsave!(1 To 4, 1 To 33), RAsave!(1 To 4, 1 To 33)
Dim zs!(1 To 150), Ts!(1 To 150), zIN!(1 To 30), TIs!(1 To 30), TNs!(1 To 30)
Dim GlineFormat$(1 To 80), OAT!(1 To 4), Tt!(1 To 20)
Dim NoD!(1 To 33), dZgeo!(1 To 33)
Dim Trop1!, TTrop1!, Trop2!, Ttrop2!
Dim W!(1 To 4), Tmin!, zTmin!, Zmin!
Dim defLAT1!, defLAT2!, defLAT3!, defLAT4!
Dim PC!(0 To 5), i%, EditSelect As Boolean, ChInfo!(1 To 4)
'''''''''''''''''''''''''''''''''''''''''''''''''''''
Dim PTflag As Boolean, Ccycle&, ATPautoStepInterval&
Dim Ceiling!, CtsRejThreshold%, dTAi!(1 To 3), dTAav!(1 To 3)

' Remove these when new MAIN routinge is working
Dim OAT1!, OAT2!, OAT3!
Dim Rcycle&
' DC8 ONLY stuff
Dim CL%(1 To 3, 1 To 12)
Dim RT!(1 To 33), RA!(1 To 33)
Dim RegNr1%, RegNr2%
Dim zt0!, Tzt0!
Dim RollWarnCrit!, RollWarnFlg As Boolean, ShowATP As Boolean

Dim GeqnMax!(1 To 3), GeqnMin!(1 To 3)
Dim GnavMax!(1 To 3), GnavMin!(1 To 3)
Dim GndMax!(1 To 3), GndMin!(1 To 3)
Dim GeqnGood(1 To 3) As Boolean, GnavGood(1 To 3) As Boolean
Dim GndGood(1 To 3) As Boolean, GmmsGood(1 To 3) As Boolean
Dim RAOBlu%
Dim CurrentRecord&, REFlu%, REFfileRecords%, REFfileRecord%, ScanLength!
Dim AutoScaleTA As Boolean

Sub GetRAOBs(dT As Date, RAOB$, Beforecycle&, AfterCycle&)
Dim i%, X$, L%, Number As Boolean, iBefore%, FoundRAOBsite As Boolean, Cycle&

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
      i = i + 1
    Loop Until FoundRAOBsite Or i = lstSelect.ListCount
    If i = lstSelect.ListCount Then Exit Do   'Site not found, Exit Sub
    ' Then check if RAOB time stamp is after wanted time
    If DandT >= dT Then              'transfer RAOBs to frmRAOB
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
  Loop
  
End Sub

Sub DoRetObsCalculationTBerr(z0!, TA!())
Dim OUTfile$, StartTime$, EndTime$, fmt$, jj%
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, M%, iL%, X%
Dim TRANGD!, Cycle&, Fif!(1 To 24), Plus As Boolean, RecordStep%, ans As Boolean
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

Dim TBvsEl!(29), Sum!, Dfreq!, ROlu%
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
'  ans = fReadFile_XXX("IFB", "DC8")
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
    Call CalcZgLevels(El(1), False)    'Depends only on sign of Elevation Angle
    ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
    Call TPRatZgLevels(Nlevr%)         'Needs HI() from CalcZgLevels
    ' Calculate Absorption at each frequency in bandpass
    Call CalcBandTau(Fif())      'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
    ' Assume elevation angles go from + to -
    Plus = True
    For L = 1 To Nel
      If El(L) < 0 And Plus Then  'Elevation angle sign change! Need new HI() levels, etc
        Plus = False              'Only go thru here on elevation angle sign change
      ' Calculate NH geometric integration levels HI(i) wrt a/c
        Call CalcZgLevels(El(L), False)    'Depends only on sign of Elevation Angle
      ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
        Call TPRatZgLevels(Nlevr%)         'Needs HI() from CalcZgLevels
      ' Calculate Absorption at each frequency in bandpass
        Call CalcBandTau(Fif())  'Needs NH, PI(), TI(), RI()
      End If
      
      If El(L) = 0 Then
        TA(M, L) = Ti(NH)   'Use OAT
        'If M = 1 Then Debug.Print "OAT="; TI(1)
      Else
      ' Calculate TBs along line of sight at each IF frequency and for each elevation angle
        Call CalcBandTBs(El(L), TB())
      ' Calculate Bandpass weighted TBs at each elevation.
        Sum = 0#
        For i = 1 To 2 * Nif
          Sum = Sum + BandWt(M, i) * TB(i)
        Next i
        TA(M, L) = Sum / BandSum(M)
      End If
      DoEvents
    Next L
    
  Next M
End Sub



Sub AverageNcycles(n%, aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!)
Dim aRA(1 To 33), aRT(1 To 33)
Dim X!, i%, j%, pALT1!, Nlev11%, Nlev21%

aZp = 0#: aTnav! = 0#: aTmtp! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
i = 0: Nlev11 = 0: Nlev21 = 0
Do
  i = i + 1
NextCycle:
  Cycle2 = Cycle2 + i - 1
  Call GetREFcycle(Cycle2, False)
  If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
  If Abs(pALT - pALT1) > 0.2 Or Nlev11 <> Nlev1 Or Nlev21 <> Nlev2 Then
    i = 1
    Cycle2 = Cycle2 + 1
    aZp = 0#: aTnav! = 0#: aTmtp! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
    For j = 1 To 33: aRA(j) = 0#: aRT(j) = 0#: Next j
    GoTo NextCycle         'Start over if climbing or descending
  End If
  If Not GoodScan Then GoTo NextCycle
  If Abs(Roll) > 3 Then GoTo NextCycle
  aZp = aZp + pALT
  aZt = aZt + Zt1
  aTnav = aTnav + OATnav
  aTmtp = aTmtp + OATmtp
  Debug.Print OATnav; OATmtp;
  aTt = aTt + TT1
  aLat = aLat + Latitude
  aLon = aLon + Longitude
  aRoll = aRoll + Roll
  aPitch = aPitch + Pitch
  For j = 1 To 33
    aRA(j) = aRA(j) + RAp(j)
    aRT(j) = aRT(j) + RTp(j)
  Next j
  Debug.Print RAp(16); RTp(16); RTp(16) - OATmtp
'  ProgressBar2.value = Cycle2                   'These two are not available yet in MTPsim
'  txtREFrecord.Text = Str(Cycle2)               ' xxx mjm
Loop Until i = n
aZp = aZp / n
aZt = aZt / n
aTnav = aTnav / n
aTmtp = aTmtp / n
Debug.Print aTnav; aTmtp
aTt = aTt / n
aLat = aLat / n
aLon = aLon / n
aRoll = aRoll / n
aPitch = aPitch / n
For i = 1 To 33
  RAp(i) = aRA(i) / n
  RTp(i) = aRT(i) / n
Next i
If aZp <> RAp(16) Then
  X = aZp - RAp(16)
  For i = 1 To 33
    RAp(i) = RAp(i) + X
  Next i
End If

If Nlev = 33 Then Debug.Print RAp(16); aZp; ; aTmtp; RTp(16); RTp(16) - aTmtp; fLinterp(RAp(), RTp(), Nlev, aZp)

  'If Nlev > 0 And Nlev <= 33 Then frmATP.ATPupdate

End Sub

Sub AverageNcyclesTA(n%, aTA!(), aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!)
Dim X!, i%, j%, k%, iStall%
Dim pALT1!, Nlev11%, Nlev21%

'N%, aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!
aZp = 0#: aZt = 0#: aTnav! = 0#: aTt! = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
For i% = 1 To Channels
  For j% = 1 To 10: aTA(i%, j%) = 0:  Next j%
Next i%
i = 0
iStall = 0
Do
  i = i + 1
NextCycle:
  If iStall > 20 Then  'Set flag if looping too many times
    aZp = 0#
    Exit Sub
  End If
  iStall = iStall + 1
  Cycle2 = Cycle2 + i - 1
  Call GetREFcycle(Cycle2, False)
  If i = 1 Then pALT1 = pALT: Nlev11 = Nlev1: Nlev21 = Nlev2
  If Abs(pALT - pALT1) > 0.2 Or Nlev11 <> Nlev1 Or Nlev21 <> Nlev2 Then
    i = 1
    Cycle2 = Cycle2 + 1
    aZp = 0#: aTnav! = 0#: aTmtp! = 0#: aZt = 0#: aTt = 0#: aLat! = 0#: aLon = 0#: aPitch = 0#: aRoll = 0#
    For k% = 1 To Channels
      For j% = 1 To 10: aTA(k%, j%) = 0:  Next j%
    Next k%
    GoTo NextCycle         'Start over if climbing or descending
  End If
  If Not GoodScan Then GoTo NextCycle
  If Abs(Roll) > 3 Then GoTo NextCycle
  aZp = aZp + pALT
  aZt = aZt + Zt1
  aTnav = aTnav + OATnav
  aTmtp = aTmtp + OATmtp
  Debug.Print OATnav; OATmtp
  aTt = aTt + TT1
  aLat = aLat + Latitude
  aLon = aLon + Longitude
  aRoll = aRoll + Roll
  aPitch = aPitch + Pitch
  For j% = 1 To Channels
    For k% = 1 To 10
      aTA(j, k) = aTA(j, k) + TA(j, k)
    Next k
    Debug.Print j; TA(j, LocHor)
  Next j%
'  ProgressBar2.value = Cycle2
'  txtREFrecord.Text = Str(Cycle2)
Loop Until i = n

aZp = aZp / n
aZt = aZt / n
aTnav = aTnav / n
aTmtp = aTmtp / n
aTt = aTt / n
aLat = aLat / n
aLon = aLon / n
aRoll = aRoll / n
aPitch = aPitch / n
  For j% = 1 To Channels
    For k% = 1 To 10: aTA(j, k) = aTA(j, k) / n: Next k
  Next j%
'  If Nlev > 0 And Nlev <= 33 Then frmATP.ATPupdate
End Sub
Sub FillTAboxes(TA!(), Offset%)
Dim i%, j%
  For i = 1 To Channels
    For j = 1 To Nel
      Select Case i
      Case 1: txtTA1(j - 1 + Offset).Text = Format(TA(1, j), "#0.00")
      Case 2: txtTA2(j - 1 + Offset).Text = Format(TA(2, j), "#0.00")
      Case 3: txtTA3(j - 1 + Offset).Text = Format(TA(3, j), "#0.00")
      End Select
    Next j
  Next i

End Sub


Sub OBavgSaveRestore(ob!(), iRC%, Nobs%, Save As Boolean)
Dim i%
' If Save = TRUE,  Save ob
'         = FALSE, Restore ob
Static obSAVE!(0 To 15, 1 To 28)

If Save Then          'Save
  For i = 1 To Nobs: obSAVE(iRC, i) = ob(iRC, i): Next i
Else                  'Restore
  For i = 1 To Nobs: ob(iRC, i) = obSAVE(iRC, i): Next i
End If

End Sub

Private Sub chkEnableRAOBextension_Click()
If chkEnableRAOBextension.Value = 1 Then
  userTextension = True
Else
  userTextension = False
End If
End Sub

Private Sub chkRemoveOBbias_Click()
  If chkRemoveOBbias.Value = 1 Then
    RemoveOBbias = True
  Else
    RemoveOBbias = False
  End If
End Sub

Private Sub chksjkPextension_Click()
If chksjkPextension.Value = 1 Then
  sjkPextension = True
Else
  sjkPextension = False
End If

End Sub

Private Sub chkUseSmatrix_Click()
  If chkUseSmatrix.Value = 1 Then
    UseSmatrix = True
  Else
    UseSmatrix = False
  End If
End Sub

Private Sub cmdATP_Click(Index As Integer)
Dim V!(1 To 20), CurrentColor%, CurrentThickness%, X$, y$, i%, j%
Dim RAg!(1 To 33), Tint!, Zint!, Gint!, n%, n1%, n2%, NlevrSave%, Cycle&

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
    Nlev = (Val(txtAVGopt(0).Text) - Val(txtAVGopt(2).Text)) / Val(txtAVGopt(1).Text) + 1
    n1 = Val(txtAVGopt(2).Text) / Val(txtAVGopt(1).Text)
    n2 = Val(txtAVGopt(0).Text) / Val(txtAVGopt(1).Text)
    Print #OUTlu, "T[K]" + vbTab + "pALT[km]" + vbTab + "gALT[km]" + vbTab + "Trms[K]" + vbTab + "ND[#/cm3 E21]" + vbTab + "dND[%]"
    Imonth = Val(Mid$(YYYYMMDD$, 5, 2))
    ' Do the SPLINE fit to retrieved ATP----------------------------<<<<<
    For i = Nlev1 To Nlev2
      RAp(i - Nlev1 + 1) = zzz(i) / 100#
      RAg(i - Nlev1 + 1) = Zgeo(i) / 100#
      RTp(i - Nlev1 + 1) = TTT(i) / 10#
    Next i
    Call SPLINE(RAp!(), RTp!(), TD1!(), TD2!(), 1, Nlev2 - Nlev1 + 1)

    ' And calculate the interpolated values for the spline fit display
    For i = n1 To n2
      If i = 0 Then Zint = 0.1 Else Zint = i * Val(txtAVGopt(1).Text)
      If Zint >= RAp(1) And Zint < RAp(Nlev2 - Nlev1 + 1) Then
        Tint = SPLINT(RAp!(), RTp!(), TD2!(), 1, Nlev2 - Nlev1 + 1, Zint, True)
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

Private Sub cmdBlend_Click(Index As Integer)
Static GA!(1 To 500), PA!(1 To 500), RH!(1 To 500), Tt!(1 To 500), ZA!(1 To 500)
Static GAa!(1 To 500), PAa!(1 To 500), RHa!(1 To 500), TTa!(1 To 500), ZAa!(1 To 500)
Static GAb!(1 To 500), PAb!(1 To 500), RHb!(1 To 500), TTb!(1 To 500), ZAb!(1 To 500)

Dim lu%, File$, Nevents%, Rec1&, Rec2&, T1!, T2!, Te!, Prefix$, Wt1!, Wt2!
Dim i%, j%, ii%, jj%, M%, Tmin!, Tmax!, Traob!, Tmtp!, TZ1!(1 To 500), TZ2!(1 To 500), Zp1!(1 To 500), Zp2!(1 To 500), nTZ1%, nTZ2%
Static Nrla%, Nrlb%, Nrl%, Isame%, OUTlu%, RAOB1$, RAOB2$, AvgTdiff!, RmsTdiff!
Dim Ln1!, Ln2!, Ln3!, LT1!, Lt2!, Lt3!, D1!, D2!, Distance!, Line$, BMPname$, jt1%, jt2%
Dim A$, iD%, iM%, iY%, UT0!, UT1!, UT2!, DT1 As Date, DT2 As Date, Filename$, T As Date
Dim Pint!, Gint!, Tint!, Rint!, WtA!, OAT11!, OAT12!, OAT21!, OAT22!
Dim IyearE%, IdoyE%, IdayE%, ImonthE%, jmax%, DTevent As Date
Static aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!
Static Site1Lat!, Site1Lon!, Site1Distance!, Site2Lat!, Site2Lon!, Site2Distance!
Static Found1 As Boolean, Found2 As Boolean, z!

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
  optSite(0).Value = True
  Rec1 = Val(txtRec1(0).Text)
  Rec2 = Val(txtRec2(0).Text)
  Wt1 = Val(txtFraction(0).Text)
  
  Call BlendROABtime(RAOBlu, Rec1, Rec2, Wt1, GAa(), PAa(), TTa(), RHa(), Nrla, AvgTdiff!, RmsTdiff!)
  
  txtSavg(0).Text = Format(AvgTdiff, "00.00")
  txtSrms(0).Text = Format(RmsTdiff, "00.00")
  For i% = 1 To Nrla
    Zplot(i%) = fPtoZ(PAa(i%))
    Tplot(i%) = TTa(i%)
    PZraob(i) = PAa(i)
    ZPraob(i) = Zplot(i)
    ZGraob(i) = GAa(i)
    RZraob(i) = RHa(i)
    TZraob(i) = TTa(i)
  Next i
  Nlevr = Nrla:  jt1% = 1: jt2% = Nlevr
  
  Call cmdREF_Click(0)
  frmRAOB.DrawWidth = 1
  frmRAOB.LineColor = Yellow
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
    PZraob(i) = PAb(i)
    ZPraob(i) = Zplot(i)
    ZGraob(i) = GAb(i)
    RZraob(i) = RHb(i)
    TZraob(i) = TTb(i)
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
  Traob = fLinterp(ZAa(), TTa(), Nrla, RAp(16))
  A$ = RAOB1$ + " " + YYYYMMDD$ + " " + Format(Site1Distance, "##0.0") + " "
  A$ = A$ + txtSavg(0).Text + " " + txtSrms(0).Text + " " + Format(Val(txtFraction(0).Text), "0.000") + " "
  A$ = A$ + " " + Format(Val(txtATPgo.Text), "00.000") + " " + Format(RAp(16), "00.000")
  A$ = A$ + " " + Format(aTnav - Traob, "##0.00") + " " + Format(RTp(16) - Traob, "##0.00")
  Debug.Print A$,
  Print #OUTlu, A$,
  
  For j = 1 To 50      '24 To 44
    z = 0.5 * j
    If z > ZAa(Nrla) Then Exit For
    Traob = fLinterp(ZAa(), TTa(), Nrla, z)
    Tmtp = fLinterp(RAp(), RTp(), Nlev, z)
    Debug.Print Format(Tmtp - Traob, "00.0"),
    Print #OUTlu, Format(Tmtp - Traob, "00.0"),
  Next j
  jmax = j - 1
  Print #OUTlu, " "
  Debug.Print " "
  DoEvents

Case 7
'N Day Month Year  RAOBs UT0 UT1 UT2
'1 9 1 2003  BGDH  30.2  0 12
'2 9 1 2003  BGTL  25.0  0 12
'3 12  1 2003  ENJA  32.9  0 12
'12345678901234567890
  Prefix$ = txtPrefix.Text
  AC$ = Mid$(Prefix$, 4, 2)
  OUTlu = FreeFile
  Open Prefix$ + "RAOBcomparison.txt" For Output As OUTlu
  lu = FreeFile
  Open Prefix$ + "RAOBtimes.txt" For Input As lu
  
  Input #lu, A$
  chkMultipleScans.Value = 1
  Do
    Input #lu, iD, iM, iY, UT0, LR11, LR12, Zb1, LR21, LR22, A$
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    RAOB2$ = Right$(A$, Len(A$) - i)
    DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring(UT0 * 1000, True))
    txtEvent.Text = DTevent
    
    If RAOB2$ = RAOB1$ Then 'Only one site involved
      If UT0 <= 43.2 Then UT1 = 0: UT2 = 12 Else UT1 = 12: UT2 = 24
      YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
      Filename$ = Prefix$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
      BMPname$ = Prefix$ + "BMP\RAOB" + YYYYMMDD$ + RAOB1$ + ".BMP"
      Call OpenREFfile(Filename$)
      txtATPgo.Text = Str(UT0)
      cmdATPgo_Click
      Call AverageNcycles(5, aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!)
      Close REFlu
      DT1 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0) + TimeValue("12:00:00")
      Do
        DT1 = DT1 - TimeValue("12:00:00")
        Call GetRAOB(8, DT1, RAOB1$, Found1)
      Loop Until Found1
      Site1Lat = WMOlatitude
      Site1Lon = WMOlongitude
      Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)
      Call CopyArray(ZPraob(), Zp1(), Nlevr)
      Call CopyArray(TZraob(), TZ1(), Nlevr)
      nTZ1 = Nlevr
      DT2 = DateSerial(iY, iM, iD) + TimeSerial(UT2, 0, 0) - TimeValue("12:00:00")
      Do
        DT2 = DT2 + TimeValue("12:00:00")
        Call GetRAOB(9, DT2, RAOB1$, Found2)
      Loop Until Found2
      Call cmdBlend_Click(1)   'Temporal interpolation
      Call cmdBlend_Click(6)   'Write results
      Call cmdATP_Click(0)     'Transfer ATP
      Call cmdSelect_Click(16) 'Write Documentation
      If chkWriteBMPs.Value = 1 Then SavePicture frmRAOB.Image, BMPname$
      If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
      Call cmdSelect_Click(5)  'Clear plot
    Else  'Need to do spatial interpolation
    
    End If
  Loop Until EOF(lu)
  Close OUTlu, lu
  
End Select

End Sub

Sub CopyArray(A!(), b!(), n%)
Dim i%
For i = 1 To n
  b(i) = A(i)
Next i
End Sub

Sub BlendROABtime(RAOBlu%, Rec1&, Rec2&, Wt1!, GA!(), PA!(), Tt!(), RH!(), Nrl%, AvgTdiff!, RmsTdiff!)
Dim i%, j%, k%, M%, n%, jj%, ii%, Nrl1%, Nrl2%, Test As Boolean, Sum1!, Sum2!

Dim GA1!(1 To 500), PA1!(1 To 500), RH1!(1 To 500), TT1!(1 To 500)
Dim GA2!(1 To 500), PA2!(1 To 500), RH2!(1 To 500), TT2!(1 To 500)
Dim Isame%, Tdiff!(1 To 500)
Dim Pint!, Gint!, Tint!, Rint!, OAT11!, OAT21!, OAT12!, OAT22!

RAOBread RAOBlu, Rec1

If UseEditLevel And EditLevel Then   'Edit levels
  For i = 1 To Nedit
    TZraob(LZraob(i%)) = TZraob(Nlevr + i)
  Next i%
End If
If chkExtendRAOB.Value = 1 Then Call RAOBextension(TZraob!(), ZPraob!(), Nlevr%, LR11, Zb1, LR12, Ztop!)

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
If chkExtendRAOB.Value = 1 Then Call RAOBextension(TZraob!(), ZPraob!(), Nlevr%, LR21, Zb2, LR22, Ztop!)

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
'  UpdateDisplay

  For i% = 1 To Nrl
    Zplot(i%) = fPtoZ(PA2(i%))
    Tplot(i%) = TT2(i%)
  Next i
'  UpdateDisplay
End If

' Now interpolate levels in time
  For i = 1 To Nrl    'Write out as increasing pressure altitude [m]
    GA(i) = GA1(i) * Wt1 + GA2(i) * (1 - Wt1)
    PA(i) = PA1(i) * Wt1 + PA2(i) * (1 - Wt1)
    Tt(i) = TT1(i) * Wt1 + TT2(i) * (1 - Wt1)
    RH(i) = RH1(i) * Wt1 + RH2(i) * (1 - Wt1)
    Tdiff(i) = TT1(i) - TT2(i)
    'Debug.Print i; GA(i); Tt(i)
  Next i
' Calculate Avg and RMS difference
  Call AvgRMS(Tdiff(), Nrl, AvgTdiff, RmsTdiff)
End Sub

Sub AddNoise(TA!(), Noise!)
' Add noise of rms "Noise" to brightness temperatures
    If Noise > 0 Then
      For i = 1 To Channels
        For j = 1 To Nel
            TA(i, j) = TA(i, j) + fSDT(Noise)
        Next j
      Next i
    End If
End Sub

Sub AddOBnoise(TA!())
      k = 1
      For i = 1 To Channels
        For j = 1 To Nel
'          Call Get_iBot_iTop(j, z0, iBot, iTop, altFR)
          Select Case j + 10 * (i - 1)
          Case Is < 6: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j))
          Case Is = 6: TA(i, j) = TA(i, j) + fSDT(OBrms(k, 28))
          Case Is < 11: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j - 1))
          Case Is < 16: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j + 9 * (i - 1)))
          Case Is = 16: TA(i, j) = TA(i, j) + fSDT(OBrms(k, 28))
          Case Is < 21: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j))
          Case Is < 26: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j))
          Case Is = 26: TA(i, j) = TA(i, j) + fSDT(OBrms(k, 28))
          Case Is < 31: TA(i, j) = TA(i, j) + fSDT(OBrms(k, j + 9 * (i - 1)))
          End Select
          'TA(i, j) = TA(i, j) + WINcor(i, j)
          Select Case i
          Case 1: txtTA1(j - 1).Text = TA(1, j)
          Case 2: txtTA2(j - 1).Text = TA(2, j)
          Case 3: txtTA3(j - 1).Text = TA(3, j)
          End Select
        Next j
      Next i

End Sub

Sub CalculateRAOBstats(RA!(), RT!(), AVG!, RMS!)
Dim Sum1!, Sum2!, n%, X!
' Calculate Avg and RMS of retrieved profile compared to RAOB
' interpolated to 33 retrieval levels
' Exclude RAOB levels below RA(1) and retrieved profile levels above Zplot(Nlevr)

    Sum1 = 0#: Sum2 = 0#: n = 0
    For i = 1 To 33
      If Zplot(1) <= RA(1) And Zplot(Nlevr) >= RA(i) And Abs(RA(i) - RA(16)) < ATPrange Then
        n = n + 1
        X = RT(i) - fLinterp(Zplot(), Tplot(), Nlevr, RA(i))
        Sum1 = Sum1 + X
        Sum2 = Sum2 + X ^ 2
      End If
    Next i
    If n < 3 Then n = 2
    AVG = Sum1 / n
    RMS = Sqr((Sum2 - n * AVG ^ 2) / (n - 1))

End Sub

Sub Clear_TAs()
Dim i%, j%

  For i = 1 To Channels
    Select Case i
    Case 1
      For j = 0 To 9: txtTA1(j).Text = "": Next j
    Case 2
      For j = 0 To 9: txtTA2(j).Text = "": Next j
    Case 3
      For j = 0 To 9: txtTA3(j).Text = "": Next j
    End Select
  Next i
End Sub


Sub DetermineROABtoShow()
Dim i%
' Always start with original RAOB, then add requested editting
  If chkGeoPotHeight.Value = 1 Then
    For i% = 1 To Nlev
      Zplot(i%) = ZGraob(i%)
      Tplot(i%) = TZraob(i%)
    Next i
    frmRAOB.ShowPressureAltitude = False
    frmRAOB.Caption = "RAOB - Geopotential Height vs Temperature"
  Else
'    If chkTdependence.value = 0 Then
'      For i% = 1 To Nlev
'        Zplot(i%) = ZPraob(i%)
'        Tplot(i%) = TZraob(i%)
'      Next i
'    Else
      ATPbias = Val(Trim(txtATPbias.Text))
      For i% = 1 To Nlev
        TZraob(i%) = TZraob(i%) + ATPbias
        Zplot(i%) = ZPraob(i%)
        Tplot(i%) = TZraob(i%)
      Next i
    
'    End If
    frmRAOB.ShowPressureAltitude = True
    frmRAOB.Caption = "RAOB - Pressure Altitude vs Temperature"
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
' Check if trop should be in geopotential height
  If chkGeoPotHeight.Value = 1 Then
    Zt1 = ZGraob(LT1)
    If Lt2 > 0 Then Zt2 = ZGraob(Lt2) Else Zt2 = 99.9: Lt2 = 1
  End If
  
  If UseEditLevel And EditLevel Then   'Edit levels
    For i% = 1 To Nedit
      Tplot(LZraob(i%)) = TZraob(Nlev + i)
    Next i%
  End If

End Sub



Sub CalculateObservables(z0!)
Dim OUTfile$, RAOBfile$, StartTime$, EndTime$, fmt$, jj%, Rv!
Dim Tcal!, Ecal!, ABSox!, ABSv!, SinEL!, i%, j%, k%, L%, M%, iL%, X%
Dim TRANGD!, Cycle&, Fif!(1 To 24), Plus As Boolean, ans As Boolean
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

Dim TBvsEl!(27), Sum!, Dfreq!, ROlu%, SumLSB!, SumUSB!
Dim jp1%, jm1%, NHm1%, V!(1 To 20), argUP!, argDN!, Tavg!

Dim Temperature!, Pressure!, RhoV!, f!, alpha!, alphaC!, Tbg!, Rl!
Dim IKT&, flag(1 To 4) As Boolean

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
' Nlev   = NUMBER OF RADIOSONDE HEIGHT LEVELS.
' NH     = NUMBER OF NOMINAL HEIGHT LEVELS.
' NraobMax    = MAXIMUM NUMBER OF RADIOSONDES FOR WHICH OBSERVABLES
'          AND RETRIEVABLES ARE TO BE COMPUTED.
' NTB    = NUMBER OF BRIGHTNESS TEMPERATURE OBSERVABLES.
' NTBP2  = TOTAL NUMBER OF OBSERVABLES (INCLUDING OBSERVATION
'          HEIGHT TEMPERATURE AND PRESSURE).
'****************************************************************
  Pz = fZtoP(z0)
  TAmax = 0: TAmin = 500
'  ans = fReadFile_XXX("IFB", "DC8")
  For i = 1 To 3
    CHnLSBloss(i) = Val(txtCHnLSB(i - 1).Text)
  Next i
  CalcInit                      'Get EL angles, IF frequencies and Weights
  Pz = fZtoP(pALT)
  Rl = 0#: flag(1) = True: flag(2) = True: flag(3) = True: flag(4) = False
  Call CalcRetLevels(z0)        'Initialize retrieval pressure levels wrt A/C z0, Pret(i), i=1 - 33
  
' Interpolate RAOBs at geometric retrieval levels wrt A/C for Tret and Hret (geometric altitude)
' Enter with Pz which has a different Hz and Hret set for each RAOB
  Calc_Hret_Tret
  Rv = fRhoVsjk(Rz, TZ)
  KnuAC = ABS_TOT(TZ, Pz, Rv, Rl, 55.51, flag())
' Do calculation in order which avoids recalculating optical depths unnecessarily
' First, do calculation for each LO frequency
  For M = 1 To Channels
  ' Do calculation for each IF frequency at each LO
    For j = 1 To Nnu
      Fif(j) = LO(M) + LSBUSB(M, j)
    Next j
'   Nothing depended on elevation angle to this point
    ' Calculate NH geometric integration levels HI(i) wrt a/c
    Call CalcZgLevels(El(1), False)    'Depends only on sign of Elevation Angle
    ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
    Call TPRatZgLevels(Nlevr%)         'Needs HI() from CalcZgLevels
    ' Calculate Absorption at each frequency in bandpass
    Call CalcBandTau(Fif())      'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
    ' Assume elevation angles go from + to -
    Plus = True
    For L = 1 To Nel
      If El(L) < 0 And Plus Then  'Elevation angle sign change! Need new HI() levels, etc
        Plus = False              'Only go thru here on elevation angle sign change
      ' Calculate NH geometric integration levels HI(i) wrt a/c
        Call CalcZgLevels(El(L), False)    'Depends only on sign of Elevation Angle
      ' Establish T,P,RH vs all height levels by interpolation of values at RAOB height levels.
        Call TPRatZgLevels(Nlevr%)         'Needs HI() from CalcZgLevels
      ' Calculate Absorption at each frequency in bandpass
        Call CalcBandTau(Fif())  'Needs NH, PI(), TI(), RI()
      End If
      
      If El(L) = 0 Then
        TA(M, L) = Ti(1)   'Use OAT
        'If M = 1 Then Debug.Print "OAT="; TI(1)
      Else
       ' Calculate TBs along line of sight at each IF frequency and for each elevation angle
        Call CalcBandTBs(El(L), TB())
      ' Calculate Bandpass weighted TBs at each elevation.
        Sum = 0#
        For i = 1 To 2 * Nif
          Sum = Sum + BandWt(M, i) * TB(i)
          If i = Nif Then SumLSB = Sum
        Next i
        SumUSB = (Sum - SumLSB) / BandSumUSB(M)
        SumLSB = SumLSB / BandSumLSB(M)
        TA(M, L) = Sum / BandSum(M)
        If M = 1 And L = 1 Then
        'Debug.Print SumLSB, SumUSB, TA(M, L)
        i = i
        End If
      End If
      ' Figure out TAmax and TAmin
      If TA(M, L) < TAmin Then TAmin = Int(TA(M, L))
      If TA(M, L) > TAmax Then TAmax = Int(TA(M, L)) + 1
    Debug.Print M; L; TA(M, L)
      DoEvents
    Next L
    
  Next M
  
End Sub

Sub CalcInit()
Dim i%, j%
  
  Htop = 50#  'Highest integration level = 50 km

' Get ALL elevation angles in "natural" order
  For i = 1 To lstEL.ListCount
    El(i) = lstEL.List(i - 1)
  Next i
  
For i = 1 To Channels
' Calculate the Nif x 2 Side Band frequencies
  For j = 1 To Nif
    LSBUSB(i, j) = -IFoff(i, Nif + 1 - j)
'    LSBUSB(2 * Nif + 1 - j) = IFoff(Nif + 1 - j)
    LSBUSB(i, Nif + j) = IFoff(i, j)
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
    If chkFixChnRF(i - 1).Value = 1 Then
      'BandWt(i, j) = IFwt(i, Nif - j + 1) * (1 + CHnLSBloss(i) / 100) '(1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
      If CHnLSBloss(i) >= 0 Then
'        BandWt(i, j) = IFwt(i, Nif - j + 1) * (1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
        BandWt(i, j) = IFwt(i, Nif - j + 1) * (1 - (Nif - j + 1) * CHnLSBloss(i) / 50#)
      Else
'        BandWt(i, j) = IFwt(i, Nif - j + 1) * (-(Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
        BandWt(i, j) = IFwt(i, Nif - j + 1) * (-(Nif - j + 1) * CHnLSBloss(i) / 50#)
      End If
      'Debug.Print i; j; IFwt(i, Nif - j + 1); IFwt(i, Nif - j + 1) * (1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif); (1 - (Nif - j + 1) * CHnLSBloss(i) / 100# / Nif); (-(Nif - j + 1) * CHnLSBloss(i) / 100# / Nif)
    Else
      BandWt(i, j) = IFwt(i, Nif - j + 1)
    End If
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


Sub CalculateAverageProfile(RA!(), RT!(), RAavg!(), RTavg!())
Dim j%

  For j = 1 To Nret
    RAavg(j) = RA(j)
    RTavg(j) = RT(j)
  Next j

End Sub

Sub Delay(dT!)
Dim time1!
  time1 = Timer
  DoEvents
  Do: Loop While Timer - time1 < dT
End Sub

Sub DoTropStatusBox()

If frmRAOB.WindowState = 2 Then 'Check if Maximized
  frmStatus.Visible = True
  With frmStatus
    .txtALTkft(0).Text = Format(ALTkm * ckft_km, "00.0")
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
    .txtALTkft(3).Text = "99.9"
    .txtALTkft(4).Text = Format(dZg * ckft_km, "0.00")
    
    .txtALTkm(0).Text = Format(ALTkm, "00.0")
    .txtALTkm(1).Text = Format(Zt1, "00.0")
    .txtALTkm(2).Text = Format(Zt2, "00.0")
    .txtALTkm(3).Text = Format("99.9", "00.0")
    .txtALTkm(4).Text = Format(dZg, "0.00")
    
    .txtTemp(0).Text = OATnav
    .txtTemp(1).Text = TT1
    .txtTemp(2).Text = TT2
    .txtTemp(3).Text = "999.9"
    .txtTheta(0).Text = fTheta(OATnav, fZtoP(ALTkm))
    If Zt1 <> 99.9 Then
    .txtTheta(1).Text = fTheta(TT1, fZtoP(Zt1))
    Else
    .txtTheta(1).Text = "999.9"
    End If

    If Zt2 <> 99.9 Then
    .txtTheta(2).Text = fTheta(TT2, fZtoP(Zt2))
    Else
    .txtTheta(2).Text = "999.9"
    End If
    .txtTheta(3).Text = "999.9"
   
  End With
Else
  frmStatus.Visible = False
End If

End Sub

Function fFixT!(i%, y!)
If i = 6 Then
  fFixT = picGain.ScaleTop + (320 - y) * picGain.ScaleHeight / 150
Else
  fFixT = y
End If

End Function

Function fTB(pALT!, El!, Freq!, Filter%, flag() As Boolean, TZraob!(), ZPraob!(), RZraob!(), Nlevr%, Mode As Boolean)
Dim j%, jj%, i%, Knu!(1 To 50), Tau!(1 To 50), Knu2!(1 To 50), Fif!(1 To 24)
Static jm1%, Sum!, argUP!, argDN!, Tavg!
Dim T!, Rv!, Rl!, P!, f!, SinEL!, r!, TRANGD!
' mjm 2003.11.12
' Calculate TB for given pALT, Elevation Angle and Frequency
' If Mode=1 do at single frequency, if Mode=0 do over IF pass band
' Calls
' Calc_Hret_Tret
' CalcZgLevels (El)
' Call TPRatZgLevels(Nlevr%)
' Call CalcBandTau(Fif())
' Call CalcBandTBs(El, TB())
' fZtoP(HI(j))
' fRhoV(Ri(j), T)
' ABS_TOT(T, P, Rv, Rl, f, flag())
' The following parameters must be defined
' LSBUSB ..... upper and lower sideband frequency offsets
' Nif ........ number of IF channels
' Nnu ........ number of frequency channels = 2*Nif
' BandWt .....

  f = Freq
  For j = 1 To Nnu: Fif(j) = f + LSBUSB(Filter, j): Next j

  Pz = fZtoP(pALT)
  Rl = 0#: flag(1) = True: flag(2) = True: flag(3) = True: flag(4) = False

  r = fRhoVsjk(Rz, TZ)
  KnuAC = ABS_TOT(TZ, Pz, r, Rl, f, flag())
' Interpolate RAOBs at geometric retrieval levels wrt A/C for Tret and Hret (geometric altitude)
' Enter with Pz which has a different Hz and Hret set for each RAOB
  Calc_Hret_Tret
  If El = 0 Then fTB = TZ: Exit Function
  
  If chkSinElCorrection.Value = 1 Then
    Call CalcZgLevels(El, True)
  Else
    Call CalcZgLevels(El, False)
  End If
  Call TPRatZgLevels(Nlevr%)         'Needs HI() from CalcZgLevels
  
  If Not Mode Then                   'Do entire pass band
    Call CalcBandTau(Fif())          'Needs NH, PrI(), TI(), RI() from TPRPRFDC8
      
    ' Calculate TBs along line of sight at each IF frequency and for each elevation angle
      Call CalcBandTBs(El, TB())
    ' Calculate Bandpass weighted TBs at each elevation.
      Sum = 0#
      For i = 1 To 2 * Nif
        Sum = Sum + BandWt(Filter, i) * TB(i)
      Next i
      fTB = Sum / BandSum(Filter)
    DoEvents
  Else                               'Do a single frequency
    SinEL = (Sin(El * Atn(1) / 45#))
    For j = 1 To NH
      T = Ti(j)
      P = PrI(j)
      Rv = fRhoV(Ri(j), T)
      Rl = 0#
      Knu(j) = ABS_TOT(T, P, Rv, Rl, f, flag())
'      Knu(j) = OXLIEB93(T, P, Rv, f) + fVliebe(T, P, Rv, f)
     'Debug.Print HI(j); T; P; Rv; Knu(j)
    Next j

' Compute opacity profile in the vertical for each integration level
' Problem with SJK approach is that when SinEl<>90 degrees Knu is for wrong altitude
' Don't stop at 50 km when at 20 km. There is still some contribution at high El angles
    Tau(1) = 0#
    For j = 2 To NH
      jm1 = j - 1
      Tau(j) = Tau(jm1) + 0.5 * (Knu(j) + Knu(jm1)) * (HI(j) - HI(jm1)) / SinEL
    Next j
      
    Sum = 0#
    Select Case El
    Case Is < 0
      For j = 1 To NH - 1
        Tavg = (Ti(j) + Ti(j + 1)) / 2#
        Sum = Sum + Tavg * (Exp(Tau(NH) - Tau(j + 1)) - Exp(Tau(NH) - Tau(j)))
       'Debug.Print j; Sum; HI(j + 1)
      Next j
      TRANGD = Tau(NH)
      If (TRANGD > 20#) Then TRANGD = 20#
      fTB = Sum + (Ti(1) + ExcessGndT) * Exp(TRANGD)     'Add Ground emission
    
    Case Is > 0
      For j = 1 To NH - 1
        Tavg = (Ti(j) + Ti(j + 1)) / 2#
        Sum = Sum + Tavg * (Exp(-Tau(j)) - Exp(-Tau(j + 1)))
       Debug.Print j; Sum; HI(j); Knu(j); Tau(j); PrI(j); Ti(j); fRhoV(Ri(j), Ti(j))
      Next j
      fTB = Sum + 2.75 * Exp(-Tau(NH))                   'Add CMB
    End Select
  End If
End Function

Sub Initialize()
Dim A$, ReadLine As Boolean, xx#, RetVal As Variant, DefCalfile$, MyDate As Variant
Dim FileLength&, eFormat$, E$

  CtsQual = True

  t1_Sum = 0#: t2_Sum = 0#: DTavg = 0#: DTrms = 0#
'  Rec% = 0
  Cycle = 0
  Nret = 33
  Nel = 10
  Nobs = Channels * (Nel - 1) + 1
  txtNret.Text = Str(Nret)
  txtNobs.Text = Str(Nobs)
  txtNRC.Text = Str(NRC)
  
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
' Then Variables
  pALTlast = 0               'Needed if no DADS on the ground

  Err53count% = 0            'File Not Found
  Err57count% = 0            'Device I/O error count
  Errxxcount% = 0            'All other errors

' Initialize flags that detect lines read (0=FALSE)
  Aline% = 0: Bline% = 0: Cline% = 0: Dline% = 0: Eline% = 0

  Tzgm = 999.9               'Global (for experiment) minimum temperature
  Ztgm = 99.9                 'Altitude of Tzgm
  Tmin = 999.9               'Current scan minimum
  Zmin = 99.9                'Altitude of Tmin

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Select Case Source$
Case "F"                   'Will get data from File ONLY if "F"
  Speed% = 1
  fid$ = AC$ + YYYYMMDD$
  pid$ = Path$
  ' Define and open RAW data file and REF out file in F-mode
  If RealTime Then  'Open file on any computer (incl. networked)
  ' Set Initial directory
    CommonDialog1.InitDir = Drive$ + Rdir$ + Mission$
  ' Set filters.
    CommonDialog1.Filter = "All Files (*.*)|*.*"
  ' Specify default filter.
    CommonDialog1.FilterIndex = 0
  ' Display the File Open dialog box.
    CommonDialog1.ShowOpen
    RAWfile$ = CommonDialog1.Filename
    If RAWfile$ = "" Then Exit Sub
    REFfile$ = Drive$ + Rdir$ + "RAW\RT" + fTstamp$ + ".REF"
    RAWstamp = FileDateTime(RAWfile$)
  Else  'Open file on local computer
    RAWfile$ = Path$ + fid$ + ".RAW"
    REFfile$ = Path$ + fid$ + ".REF"  'Random Everything File
    RAWstamp = Mid$(yymmdd$, 3, 2) + "/" + Right$(yymmdd$, 2) + "/" + Left$(yymmdd$, 2)

    'RAWstamp = CDate(MyDate) 'xxx mjm 990505 FileDateTime(RAWfile$)
  End If
  
  INPlu% = FreeFile
  Open RAWfile$ For Input As INPlu%
  LOFraw = LOF(INPlu%)
  'ProgressBar1.min = 0
  'ProgressBar1.Max = 100

  MMSfile$ = Path$ + "MM" + YYYYMMDD$ + ".WND" 'MMS OAT calibration file
  If CalSource$ = "MMS" Then MMSstamp = FileDateTime(MMSfile$)

End Select
  
  On Error GoTo Error_Handler  'Set up to handle Run Time errors (in D-mode only)
'
  Speed% = 2
If Source$ <> "F" Then
' Deal with default output files in RAW directory in D or T mode
'  dT$ = Mid$(Date$, 10, 1) + Mid$(Date$, 1, 2) + Mid$(Date$, 4, 2)
'  TM$ = Left$(Time$, 2) + "." + Mid$(Time$, 4, 2)
'  OUTfile$ = Drive$ + Rdir$ + "RAW\" + Left$(AC$, 1) + dT$ + TM$ 'See initialization for pod$ defn
'  OUTlu% = FreeFile
'  Open OUTfile$ For Output As OUTlu%
End If

' Deal with Calfile
  A$ = Left$(Rdir$, Len(Rdir$) - 1) 'Strip back slashes
  A$ = Right$(A$, Len(A$) - 1)
  DefCalfile$ = Drive$ + Rdir$ + A$ + ".CAL"  'DC8.cal, ER2.cal or wb57.cal

If Source$ = "F" Then
  'Calfile$ = Drive$ + Rdir$ + Mission$ + "\" + Mission$ + ".CAL"
  If RealTime Then
    CALfile$ = DefCalfile$
  Else
    CALfile$ = Path$ + AC$ + YYYYMMDD$ + ".CAL"
  End If
Else
  CALfile$ = DefCalfile$
End If
'If chkNewCalFormat.value = 1 Then
'  ReadCalfile (Calfile$)     'new mjm format 19990728
'Else
'  Select Case AC$
'  Case "ER", "WB"
'    CtsRejThreshold = 400
'    ReadCalfileER (Calfile$) '2 channel CAL
'  Case "DC"
'    CtsRejThreshold = 200
'    ReadCalfileDC (Calfile$) '3 channel CAL
'  End Select
'End If
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

If Source$ = "F" Then
  On Error GoTo CreateREFfile
  FileLength = FileLen(REFfile$)
  If FileLength > 0 Then
    If DoAll Then  ' Don't bother with message if DoAll is TRUE, just delete file
'      Kill REFfile$
    Else
'      Msg = REFfile$ + " already exists!" + vbCrLf + vbCr + "Do you want to delete it?" ' Define message.
'      Style = vbYesNo                    ' Define buttons.
'      Title = "WARNING!"                 ' Define title.
'      Response = MsgBox(Msg, Style, Title, Help, Ctxt)
'      If Response = vbYes Then Kill REFfile$
    End If
  End If
  
CreateREFfile:
  If SaveOutput Then
    REFlu% = FreeFile
    Open REFfile$ For Random Access Read Write As REFlu% Len = Len(REF)
  End If
  
  On Error GoTo TryWND
  If useMMS Then
    MMSlu% = FreeFile             'line below BLG(980411)
    Open MMSfile$ For Input As #MMSlu%
  End If
  GoTo SetHeaders

TryWND:
'  Mid$(fiMMS$, 12, 3) = "WND"
'  Close #MMSlu%
'  MMSlu% = FreeFile                      'line below BLG(980411)
'  Open Path$ + MMSfile$ For Input As MMSlu%

SetHeaders:
   On Error GoTo 0
End If

  NavQualPC = 0
  TAqualPC = 0
  CtsQualPC = 0
  CycleQualPC = 0
  TtgtPC = 0
  RAWbadPC = 0
  
' Set limits for acceptable gains and counters to track
'  NAVgainCount = 0
'  GEgainCount = 0
'  NDgainCount = 0

' Initialize limit summary parameters
  CheckLimitsInit
  
  If SaveOutput Then
    REFwriteFLTINFO REFfile$
    REFwriteLIMITS REFfile$
  End If
  
  Cycle% = 0

  Select Case Platform$
  Case "DC8": InitializeDC          'DC8
  Case Else:  InitializeER          'WB57 and ER2
  End Select
  Source$ = "F"
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

Sub Main(RetrievalOnly As Boolean)
Dim V!(1 To 16), a10!(1 To 10), T10!(1 To 10), RegNr1!, RegNr2!
Dim i%, j%, L%, M%, n%, xx!, A$, a10Last!, T10last!, LATold!, time1#
Dim X$, Value!, wd!, Why$, dZ!, Zi!, Ti!, Tice!, Tnat!, dTi!, LRavg!
Static TGTmOATlast!, TGTmOATi!
Dim xy!(0 To 15), Doy$, T1sum!, T2sum!, vv!, UseNavUT As Boolean
Dim WorstCtsDif%, absLAT!, num!, Denom!
Dim Zt1Qual As Boolean, Zt2Qual As Boolean, zt1g!, zt2g!, zacg!, Tzt1g!, Tzt2g!
Dim Pz!(1 To 33), RHZ!(1 To 33), Zx!(1 To 33)
Dim Qgnav%(1 To 3), Qgmms%(1 To 3), GoodNav%, GoodMMS%, CBrate!(1 To 3), CBlast!(1 To 3)
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3)
Static UTsecLast&, Day, Change As Boolean, UTsec10&, ALTftLast&, Tbase!
Static NextAline$, DPmax!, DTmax!, lu%
Dim NpALT!, NpALTft!, nOATn!, NALTkm!, NALTkmu!, NALTcor!, BadCounts%, Thgmin!, UTgmin!
Dim z0!, jj%, NlevSave%, iRCset%, Noise!, Sum1!, Sum2!, AVG!, RMS!, NlevrSave%, Thmin!, Zgm!
Dim SaveT!(1 To 33), SaveZ!(1 To 33), SaveColor!, jt1%, jt2%, Rtrop!, TropTheta1!, TropTheta2!
Static OUTlu%, TAcorr!(1 To 3, 1 To 10)

ReadLine:                            'Loop here from End-of-Program
    NlevSave = Nlev
    If lstFL.ListCount = 0 Then
      MsgBox "You must specify flight levels to retieve!", vbOKOnly
      Exit Sub
    End If
    Randomize
    ChInfo(1) = 1: ChInfo(2) = 1: ChInfo(3) = 1
    
  For jj = 0 To lstFL.ListCount - 1  'Iterate through altitudes
    z0 = Val(lstFL.List(jj))
    pALT = z0
    If Not RetrievalOnly Then
      txtFL.Text = z0
      Clear_TAs
      DoEvents
      Nlev = NlevSave
      If lstFL.ListCount = 1 Then frmRAOB.LineColor = Yellow Else frmRAOB.LineColor = ((jj + 10) Mod 15)
      CalculateObservables z0
      Call FillTAboxes(TA!(), 0)
      
      'DoRetObsCalculationTBerr z0, TAcorr()   'Code from RCcalc
      If chkTdependence.Value = 1 Then
        If OUTlu = 0 Then
          OUTlu = FreeFile
          Open "C:\MTP\Tdependence.txt" For Output As OUTlu
        End If
        Debug.Print z0; vbTab; txtATPbias.Text
        Debug.Print "Lvl"; vbTab; "CH1"; vbTab; "CH2"; vbTab; "CH3"
        Print #OUTlu, z0; vbTab; txtATPbias.Text
        Print #OUTlu, "Lvl"; vbTab; "CH1"; vbTab; "CH2"; vbTab; "CH3"
        For L = 1 To 10
          Debug.Print L; vbTab; txtTA1(L + 19).Text; vbTab; txtTA2(L + 19).Text; vbTab; txtTA3(L + 19).Text
          Print #OUTlu, L; vbTab; txtTA1(L + 19).Text; vbTab; txtTA2(L + 19).Text; vbTab; txtTA3(L + 19).Text
        Next L
        'For j = 1 To 10: Debug.Print j; TAcorr(1, j); TAcorr(2, j); TAcorr(3, j): Next j
      End If
      If chkDefaultNoise.Value = 1 Then Call AddOBnoise(TA!())
      OATmtp = TA(1, LocHor)  'All freqs are the same (Not true if different altitudes are used!)
    
    Else
      k = SelectedFL
      For i = 1 To Channels
        For j = 1 To Nel
'          Call Get_iBot_iTop(j, z0, iBot, iTop, altFR)
          Select Case j + 10 * (i - 1)
          Case Is < 6: TA(i, j) = OBavgWt(SelectedRCset, j) + fSDT(OBrms(k, j))
          Case Is = 6: TA(i, j) = OBavgWt(SelectedRCset, 28) + fSDT(OBrms(k, 28))
          Case Is < 11: TA(i, j) = OBavgWt(SelectedRCset, j - 1) + fSDT(OBrms(k, j - 1))
          Case Is < 16: TA(i, j) = OBavgWt(SelectedRCset, j + 9 * (i - 1)) + fSDT(OBrms(k, j + 9 * (i - 1)))
          Case Is = 16: TA(i, j) = OBavgWt(SelectedRCset, 28) + fSDT(OBrms(k, 28))
          Case Is < 21: TA(i, j) = OBavgWt(SelectedRCset, j - 1 + 9 * (i - 1)) + fSDT(OBrms(k, j))
          Case Is < 26: TA(i, j) = OBavgWt(SelectedRCset, j + 9 * (i - 1)) + fSDT(OBrms(k, j))
          Case Is = 26: TA(i, j) = OBavgWt(SelectedRCset, 28) + fSDT(OBrms(k, 28))
          Case Is < 31: TA(i, j) = OBavgWt(SelectedRCset, j - 1 + 9 * (i - 1)) + fSDT(OBrms(k, j + 9 * (i - 1)))
          End Select
          'TA(i, j) = TA(i, j) + WINcor(i, j)
          Select Case i
          Case 1: txtTA1(j - 1).Text = TA(1, j)
          Case 2: txtTA2(j - 1).Text = TA(2, j)
          Case 3: txtTA3(j - 1).Text = TA(3, j)
          End Select
        Next j
      Next i

    End If
'   Add TB Noise
    Noise = Val(txtTBnoise.Text)
    Call AddNoise(TA(), Noise)
    Call MapTAtoOB(TA!(), ob!(), Channels%, Nel%, LocHor%, ChInfo!())
    ob(0) = pALT
    
'    Call OBavgSaveRestore(OBavg(), SelectedFL, Nobs, True)
    Call DetermineRCregion(ob(), frmMTPsim, frmStatus, frmFLTINFO)
    
    If Algorithm = 0 Then
      n = UserRCindex
      IlnPmin1 = n
      IlnPmin2 = n
    Else
      n = IlnPmin1
    End If
    UpdateTBwindow
    
  ' Do default retrieval
    Call Do_RetrievalSim(Algorithm, Latitude, ob(), RT(), RA(), frmMTPsim)
  
  ' Update avg profile if data is OK
    Call CalculateAverageProfile(RA(), RT(), RAavg(), RTavg())
  
  ' Calculate statistics for RAOB compared to retrieval
    Call CalculateRAOBstats(RA!(), RT!(), AVG!, RMS!)

  ' Do Cubic Spline fit for Trop stuff
    GoSub DetermineTropParameters
  
  ' Write to Output files
    If chkShowAllRetrievals.Value = 0 Then
      iRCset = n
      GoSub WriteToOutFiles
    
'   Restore AA observables
'    Call OBavgSaveRestore(OBavg(), SelectedFL, Nobs, False)
    Else
     'If chkUseAllRCs.value = 1 Then
      For iRCset = 0 To NRC - 1 'NRCbin - 1
        frmRAOB.LineColor = ((iRCset + 9) Mod 15)
        Call RetrieveAllRCs(iRCset)
        Call CalculateRAOBstats(RA!(), RT!(), AVG!, RMS!)
        Stats(1, iRCset + 16) = AVG
        Stats(2, iRCset + 16) = RMS
        Stats(3, iRCset + 16) = Sqr(AVG ^ 2 + RMS ^ 2)
        
        If iRCset < 16 Then
          If chkShowTotalError.Value = 0 Then
            txtWavg(iRCset + 16).Text = Format(AVG, "#0.0")
            txtWrms(iRCset + 16).Text = Format(RMS, "#0.0")
          Else
            txtWavg(iRCset + 16).Text = Format(Stats(3, iRCset + 16), "#0.0")
            txtWrms(iRCset + 16).Text = ""
          End If
        End If
        ' Update avg profile if data is OK
        Call CalculateAverageProfile(RA(), RT(), RAavg(), RTavg())
        'Debug.Print iRCset; RAavg(16); RTavg(16)
      ' Do Cubic Spline fit for Trop stuff
        GoSub DetermineTropParameters
      ' Write to Output files
        GoSub WriteToOutFiles
      Next iRCset
    End If

  Next jj  'End of iterating through altitudes
  
EndOfSub:
  If Quit Then
    NoMoreInput
  Else
    'tmrATPautoStep.Enabled = True  'Enable during processing
  End If
  
  DoEvents

  Exit Sub

'****************************  SUB ROUTINES  *******************************'


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
DetermineTropParameters:
  If Abs(Roll) > 30 And Roll <> 999.9 Then Return
  'Check for valid A/C Roll before updating profile and derived parameters
' Start overhead for SPLINE fit, First ...

' Figure out number of non-zero retrieved levels
  jt1% = 0: Do: jt1% = jt1% + 1: Loop Until RAavg(jt1%) >= 0 Or jt1% = 33
  jt2% = 34: Do: jt2% = jt2% - 1: Loop Until RAavg(jt2%) > 0 Or jt2% = 1
  If jt1% = 33 Then GoTo EndOfSub 'GoTo ReadLine      ' No retrieved level, start all over

' Finally, do the SPLINE fit to retrieved ATP
  Call SPLINE(RAavg(), RTavg(), T1(), T2(), jt1%, jt2%)
  Rtrop = Trop1  'save RAOB trop
  Call WMOTropLoc(RAavg(), RTavg(), T1(), T2(), jt1%, jt2%, Trop1!, TTrop1!, Trop2!, Ttrop2!)
  
  If Trop1 <> 99.9 Then TropTheta1 = fTheta(TTrop1, fZtoP(Trop1)) Else TropTheta1 = 999.9
    
  If Trop2 < 99.9 Then TropTheta2 = fTheta(Ttrop2, fZtoP(Trop2)) Else TropTheta2 = 999.9
' Define new parameters for data structures
  Zt1 = Trop1: Zt2 = Trop2
  TT1 = TTrop1: TT2 = Ttrop2
  Th1 = TropTheta1: Th2 = TropTheta2

' Calculate position of current and gobal minima
  Call T_minimum(pALT, Cycle, RAavg(), RTavg(), T1(), T2(), jt1%, jt2%, Zmin, Tmin)
  Thmin = fTheta(Tmin, fZtoP(Zmin))
  If Tmin < Tzgm And Tmin > 175 Then
    Tzgm = Tmin: Zgm = Zmin: Thgmin = Thmin: UTgmin = UTsec
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
  If RA(19) <> RA(13) Then
    LRac = (RT(19) - RT(13)) / (RA(19) - RA(13))
  Else
    LRac = 999.9
  End If
  Select Case LRac
  Case 0, Is < -20, Is > 40
    LRac = 999.9
  End Select
  If ALTkm < 0.1 Then LRac = 999.9
  'LRavg = (RTavg(19) - RTavg(13)) / (RAavg(19) - RAavg(13))
  If RAavg(19) <> RAavg(13) Then
    LRavg = (RTavg(19) - RTavg(13)) / (RAavg(19) - RAavg(13))
  Else
    LRavg = 999.9
  End If

Return
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
TropRejection:
  Zt1Qual = Good: Zt2Qual = Good
' Determine useability of tropopause solutions Zt1 & Zt2 for RET recording
  'IF Source$ = "D" THEN return     'Don't bother if "D" mode

  If CtsQual = BAD Then Return

'  If Abs(Roll) > 15 Then Zt1Qual = BAD: Zt2Qual = BAD: Return
'  If Abs(Roll) < 45 Then
'    If Abs(Roll - RollLast) > 7 Then Zt1Qual = BAD: Zt2Qual = BAD
'    RollLast = Roll
'  End If
'  If Zt1Qual = BAD Then Return

  'IF ABS(ALTkmTREND) > 17 * 5 THEN Zt1Qual% = BAD: Zt2Qual% = BAD
  'MJM 971011
  If Zt1Qual = BAD Then Return

  'Now chk for Zt being outside permissible altitude limits
'  ZtMax1 = ALTkm + 7.5
'  ZtMax2 = 2.8 * ALTkm - 9
'  ZtMax = ZtMax1: If ZtMax2 < ZtMax Then ZtMax = ZtMax2
'  zTmin = 2.5 + ALTkm / 2
'  If Zt1 > ZtMax Or Zt1 < zTmin Then Zt1Qual = BAD
'  If Zt2 > ZtMax Or Zt2 < zTmin Then Zt2Qual = BAD
Return

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
WriteToOutFiles:
' Don't clear if processing an entire RAOB file (so results can be seen)
  If chkProcessFile.Value = 1 Then
    If chkAutoClear.Value = 1 Then cmdREF_Click (0) 'Clear plot
  
    If Not EOF(RAOBlu) Then
      With frmRAOB
        .ShowAircraft = False
        .Zt1 = Ztrop
        .TT1 = Ttrop
        .LineColor = White                'Plot current RAOB
        .RAOBupdate                       'Uses Zplot() and Tplot()
      End With
    End If
    DoEvents
'    If chkAutoRetrieve Then cmdREF_Click (2)
  End If
' above code plots RAOB file
  GoodScan = True                          'Force all records to be good
  GoodTrop = True
  

'   Deal with geometric altitude if necessary
    dZg = 0  'gALT - pALT  'gALT offset wrt pALT at flight level
    
    Call IntegrateHE(Nlev, RAavg(), RTavg(), dZgeo(), NoD())
    
    For i = 1 To 33
      Pz(i) = fZtoP(RAavg(i))
      RHZ(i) = 50#
    Next i
   
'    Call CalculateZarray(Latitude, 0#, Nlev1%, Nlev2%, Pz!(), RTavg(), RHZ!(), 1, Zx!())
    ' dZgeo() are offsets wrt fight level, NoD() is number density profile
    For i = 1 To 33
'      Zgeo(i) = Int(100 * (gALT + dZgeo(i)))
'      Zx(i) = Zx(i) - RTavg(16) + dZg
      If Abs(NoD(i)) > 3E+25 Then NoD(i) = 3E+25
      
      ND(i) = Int(NoD(i) / 1E+21)
    Next i
'    NDac = ND(16)
    
    Nlev = 0
    For i = 1 To 33
    ' Prepare arrays for REFwrite
      zzz(i) = Int(100# * RAavg(i)): TTT(i) = Int(10# * RTavg(i))
      If RAavg(i) > 0# Then
        Nlev = Nlev + 1  'Keep track of levels count for plotting
        If Nlev = 1 Then Nlev1 = i   'First level above ground
      ' Prepare pALT or gALT arrays dimensioned 1 to Nlev
        If ShowZp Then
          RAp(Nlev) = RAavg(i)  'array from 1 to Nlev
        Else
          RAp(Nlev) = gALT + dZgeo(i)
        End If
        RTp(Nlev) = RTavg(i)
'       Debug.Print i%; RA(i%); RT(i%); Nlev; zzz(Nlev%); TTT(Nlev%)
      End If
    Next i
    Nlev2 = Nlev1 + Nlev - 1
    
    If ShowZp Then
      With frmRAOB
'        .EnableFit = True       'TD1 and TD2 are only dimensioned 33
        .ALTkm = pALT
        .Tac = OATmtp
        .Zt1 = Zt1
        .Zt2 = Zt2
        .TT1 = TT1
        .TT2 = TT2
        '.dZg = dZg
      End With
    Else
  ' Finally, do the SPLINE fit to retrieved ATP
    Call SPLINE(RAp(), RTp(), T1(), T2(), 1, Nlev)
    Call WMOTropLoc(RAp(), RTp(), T1(), T2(), 1, Nlev, zt1g, Tzt1g, zt2g, Tzt2g)
      With frmRAOB
        .ALTkm = ALTkm
        .Zt1 = zt1g
        .Zt2 = zt2g
'        .dZg = dZg
      End With
    End If
    
    If ShowATP Then  'And (iRCset = n Or chkShowAllRetrievals.Value = 1) Then
      For i = 1 To Nlev
        Zplot(i) = RAavg(Nlev1 + i - 1)
        Tplot(i) = RTavg(Nlev1 + i - 1)
      'Debug.Print i; Zplot(i); Tplot(i)
      Next i
      NlevrSave = Nlevr
      Nlevr = Nlev
      With frmRAOB
        .PlotSingleScan = False
        .PlotT0 = True
        If iRCset = n And chkShowAllRetrievals.Value = 0 Then
          .LineColor = Yellow               'Plot current RAOB
        Else
          If chkShowAllRetrievals.Value = 1 Then
            .LineColor = (iRCset + 9) Mod 15
          Else
            .LineColor = Yellow
          End If
          txtRCuse(iRCset).BackColor = (iRCset + 9) Mod 15
        End If
        If chkProcessFile.Value = 1 Then .LineColor = Yellow
        .RAOBupdate
      End With
      Nlevr = NlevrSave
    End If
    
    If chkShowAAATP And iRCset = n Then
      For i = 1 To 33
        SaveT(i) = Tplot(i)
        SaveZ(i) = Zplot(i)
        Tplot(i) = TAAi(i)        'was RTavi(i)
        Zplot(i) = RA(i)
      Next i
      SaveColor = frmRAOB.LineColor
      With frmRAOB
        .LineColor = LGreen
        .RAOBupdate
      End With
      For i = 1 To 33
        Tplot(i) = SaveT(i)
        Zplot(i) = SaveZ(i)
      Next i
      frmRAOB.LineColor = SaveColor
    End If
  
  If ShowNDP And iRCset = n Then
    For i = 1 To Nlev
      RAp(i) = pALT + dZg + dZgeo(i + Nlev1 - 1)
      RTp(i) = NoD(i + Nlev1 - 1)
    Next i
    frmNDP.NDPupdate
  End If
'
'  DoTropStatusBox
  
  MakeWord = 0
 
  GoodScan = True
  If chkWriteResults.Value = 1 Then
    If Rcycle = 1 Then
      lu = FreeFile
      Open "c:\mtp\data\er2\tc4\raob\TropComp_" + Format(pALT, "00") + "_km.txt" For Output As lu
      Print #lu, txtRAOBfile.Text
      Print #lu, ""
      X$ = "Cycle" + vbTab + "Zm" + vbTab + "Zr" + vbTab + "Zm-Zr" + vbTab + "Tm" + vbTab + "Tr" + vbTab + "Tm-Tr" + vbTab
      X$ = X$ + "Zcm" + vbTab + "Zcr" + vbTab + "Zcm-Zcr" + vbTab + "Tcm" + vbTab + "Tcr" + vbTab + "Tcm-Tr"
      Print #lu, X$
    End If
    X$ = Format(Rcycle, "0000") + vbTab
'    For i = 1 To Nlev
'      If ZPraob(1) < RAavg(i) And ZPraob(Nlevr) >= RAavg(i) Then
'        Print #lu, RTavg(i) - fLinterp(ZPraob(), TZraob(), NlevSave, RAavg(i)); vbTab;
'      Else
'        Print #lu, " "; vbTab;
'      End If
'    Next i
      X$ = X$ + Format(Zt1, "#0.00") + vbTab + Format(Ztrop, "#0.00") + vbTab + Format(Zt1 - Ztrop, "#0.00") + vbTab
      X$ = X$ + Format(TT1, "#0.00") + vbTab + Format(Ttrop, "#0.00") + vbTab + Format(TT1 - Ttrop, "#0.00") + vbTab
      X$ = X$ + Format(Zmin, "#0.00") + vbTab + Format(Zcoldest, "#0.00") + vbTab + Format(Zmin - Zcoldest, "#0.00") + vbTab
      X$ = X$ + Format(Tmin, "#0.00") + vbTab + Format(Tcoldest, "#0.00") + vbTab + Format(Tmin - Tcoldest, "#0.00") + vbTab
      Print #lu, X$
      If Rcycle = Val(txtFileRec.Text) Then
        Close lu
        cmdStop_Click
        chkWriteResults.Value = 0
        txtGoTo.Text = "1"
        cboGoTo.ListIndex = 0
        cmdGoTo_Click
      End If
  End If
Return


ErrorHandler:
Stop

End Sub



Sub NoMoreInput()
Dim ProgramDrive$, INPfile$, OUTfile$
  TotalCycles% = Cycle%
  Tstamp = Date + Time
  REFstamp = Date + Time
  
  If Not ShowDiagnostics Then mnuWriteFLTINFO_Click

  Close INPlu
  Close RAWlu
  If SaveOutput Then Close REFlu
 
  lblProgress.Caption = "REF file created"
  DoEvents
  
'  If DoAll Then
'    Close
'    Unload frmRAOB
'    Unload Me
'  End If
End Sub

Function fFixP!(i%, y!)
If i = 6 Then
  fFixP = picGain.ScaleTop + (25 - y) * picGain.ScaleHeight / 25
Else
  fFixP = y
End If
End Function
Sub RetrieveAllRCs(IlnPmin1%)
Static Rec1%, MRI1sum!, MRI2sum!, Aavg!, Arms!, LATold!
Dim w1!, w2!, X!, f As Form
Set f = frmMTPsim

  'Debug.Print "X "; IlnPmin1; ob(28)
  
  Call RetrieveATP(IlnPmin1, ob(), RT1(), RA(), Aavg, Arms)
  Stats(1, IlnPmin1) = Aavg
  Stats(2, IlnPmin1) = Arms
  Stats(3, IlnPmin1) = Sqr(Aavg ^ 2 + Arms ^ 2)
  If chkShowTotalError.Value = 0 Then
    txtWavg(IlnPmin1).Text = Format(Aavg, "#0.0")
    txtWrms(IlnPmin1).Text = Format(Arms, "#0.0")
  Else
    txtWavg(IlnPmin1).Text = Format(Stats(3, IlnPmin1), "#0.0")
  End If
  For j = 1 To 33: RT(j) = RT1(j): Next j
  
  If pALT > RetAltMin Then
    MRI1sum = MRI1sum + MRI         'Sum MRI values
    MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
    INC Rec1
    If Rec1 > 10 Then
      MRIavg = MRI1sum / Rec1                            'Calculate average
'      MRIrms = Sqr((MRI2sum - Rec1 * MRIavg ^ 2) / (Rec1 - 1)) 'Calculate RMS
    End If
    'Debug.Print MRI; MRIavg; MRIrms
  End If


End Sub

Sub UpdateFormRet()
Dim i%

'  PRmin = Val(txtRAOBmin.Text)
  
  Nlo = lstLO.ListCount
  For i = 0 To lstLO.ListCount - 1
      LO(i + 1) = Val(lstLO.List(i))
  Next i
 
'  ROfile$ = txtRCpath.Text + txtRegion.Text + "_" + ALT$ + ".DAT"
'  txtROfile.Text = ROfile$
  
  If Nobs = 0 Then Nobs = 9 Else Nobs = Nlo * (Nel - 1) + 1
  If Nlo = 0 Then Nlo = 3  'Handle startup
  If Nnu = 0 Then Nnu = 24
  NTB = Nlo * Nnu * Nobs    'Number of freq, IF, and el angle combos

End Sub

Sub UpdateRCs(i%)

'  If chkOldDC8RC.value = 1 Then RCformat(i) = 0 Else RCformat(i) = 1

'  Select Case cboSU.Text
'  Case "DC8"
'    If RCformat(i) > 0 Then
'      Call ReadInRCdcNew(frmMTPsim.txtRCpath.Text + RCs$, Reg$(i), i, OBav!(), OBrms(), rc!(), RTav!(), RMSa!(), RMSe!())
'      Call ReadInRC '(frmMTPsim.txtRCpath.Text + RCs$, Reg$(i), i, OBav!(), OBrms(), rc!(), RTav!(), RMSa!(), RMSe!())
'    Else
'      Call ReadInRCdc(frmMTPsim.txtRCpath.Text, Reg$(i), i, OBavA(), OBavB(), RCa(), RCb(), RTav())
'    End If

'  Case Else
'    Call ReadInRCerNew(frmMTPsim.txtRCpath.Text + RCs$, Reg$(i), i, OBav!(), OBrms(), rc!(), RTav!(), RMSa!(), RMSe!())
'    txtRCformat(i).Text = RCformat(i)
'  End Select
  
  For i = 1 To NRC
    lblRCname(i - 1).ForeColor = QBColor((i + 8) Mod 15)
    lblRCname(i - 1).Caption = cboReg(i - 1).Text
'    lblRC(i - 1).Caption = cboReg(i - 1).Text
  Next i

End Sub

Private Sub cboFltDates_Click()
Lindex = frmFLTINFO.cboFltDates.ListIndex
UpdateREFfile
End Sub

Private Sub cboFltNos_Click()
Lindex = frmFLTINFO.cboFltNos.ListIndex
UpdateREFfile
End Sub

Private Sub cboMissions_Click()
UpdateFltDates frmMTPsim
Mission$ = Trim(frmFLTINFO.cboMissions.Text)
UpdateREFfile

End Sub

Private Sub cboObjectives_Click()
Lindex = frmFLTINFO.cboObjectives.ListIndex
UpdateREFfile
End Sub



Private Sub cboPlatforms_Click()
Dim i%

 cmdLO_Click (3)   'Uses cboPlatform.ListIndex, Defines Nlo
 cmdEL_Click (3)   'Uses cboPlatform.ListIndex, Defines Nel, Nobs
 Rdir$ = "\" + Trim(frmFLTINFO.cboPlatforms.Text) + "\"

Select Case frmFLTINFO.cboPlatforms.ListIndex
Case 0 'DC8
  AC$ = "DC"
' Std errors on observables
  For i = 1 To Nobs: ESV(i) = 0.6: Next i
  ESV(8) = 1#: ESV(9) = 1.4: ESV(10) = 2#: ESV(11) = 1.2: ESV(12) = 1#
'  For i = 1 To Nobs
'  cmbESV.AddItem Format$(i, "00") + "  " + Format$(ESV(i), "0.0")
'  Next i
  frmFLTINFO.cboSU.ListIndex = 0
  Platform$ = "DC8"
  
Case 1   'ER2
  AC$ = "ER"
' Std errors on observables
  For i = 1 To Nobs: ESV(i) = 0.7: Next i
  ESV(1) = 1.5: ESV(2) = 1.1: ESV(3) = 0.8: ESV(6) = 1#: ESV(7) = 0.9
  ESV(8) = 0.8: ESV(11) = 0.9: ESV(12) = 0.8: ESV(15) = 0.9: ESV(16) = 0.8
'  For i = 1 To Nobs
'  cmbESV.AddItem Format$(i, "00") + "  " + Format$(ESV(i), "0.0")
'  Next i
' cboSU.ListIndex = 1
  Platform$ = "ER2"
  
 Case 2  'WB57
  AC$ = "WB"
' Std errors on observables
  For i = 1 To Nobs: ESV(i) = 0.7: Next i
  ESV(1) = 1.5: ESV(2) = 1.1: ESV(3) = 0.8: ESV(6) = 1#: ESV(7) = 0.9
  ESV(8) = 0.8: ESV(11) = 0.9: ESV(12) = 0.8: ESV(15) = 0.9: ESV(16) = 0.8
'  For i = 1 To Nobs
'  cmbESV.AddItem Format$(i, "00") + "  " + Format$(ESV(i), "0.0")
'  Next i
  'cboSU.ListIndex = 2  'xxx mjm
  Platform$ = "WB57"
  
End Select

' cmdPalt_Click (3) 'Set default P alts
' UpdateForm
End Sub


Private Sub cboReg_Click(Index As Integer)

Select Case Index
Case 0: Reg0$ = Trim(cboReg(0).Text): Reg$(0) = Reg0$  'Handle MRI case as first 3
Case 1: Reg1$ = Trim(cboReg(1).Text): Reg$(1) = Reg1$
Case 2: Reg2$ = Trim(cboReg(2).Text): Reg$(2) = Reg2$
Case 3: Reg$(3) = Trim(cboReg(3).Text)
Case 4: Reg$(4) = Trim(cboReg(4).Text)
Case 5: Reg$(5) = Trim(cboReg(5).Text)

End Select
USE5$ = cboUSE.Text

UpdateRCs (Index)
txtRCformat(Index).Text = RCformat(Index)
For i = 0 To 5
  If Trim(cboReg(i).Text) <> "" Then NRC = i + 1
Next i
txtNRC.Text = NRC

End Sub


Private Sub cboSU_Click()
Dim ans

' Change IF frequencies and weights ONLY.
Select Case frmFLTINFO.cboSU.ListIndex
Case 0 'DC8 Sensor Unit
  Nif = 11
  IFoff(1, 1) = 0.22: IFoff(1, 2) = 0.25: IFoff(1, 3) = 0.27: IFoff(1, 4) = 0.29
  IFoff(1, 5) = 0.31: IFoff(1, 6) = 0.33: IFoff(1, 7) = 0.35
  IFoff(1, 8) = 0.37: IFoff(1, 9) = 0.39: IFoff(1, 10) = 0.41: IFoff(1, 11) = 0.44
' IF Weights
  IFwt(1, 1) = 0.0114: IFwt(1, 2) = 0.0647: IFwt(1, 3) = 0.458: IFwt(1, 4) = 0.8995
  IFwt(1, 5) = 0.9838: IFwt(1, 6) = 0.9942: IFwt(1, 7) = 0.9557
  IFwt(1, 8) = 0.6528: IFwt(1, 9) = 0.1713: IFwt(1, 10) = 0.0339: IFwt(1, 11) = 0.011
' OLD BLG BP Model
' IF offsets
'  Nif = 7
'  IFoff(1,1) = 0.25: IFoff(1,2) = 0.27: IFoff(1,3) = 0.29:  IFoff(1,4) = 0.32
'  IFoff(1,5) = 0.355: IFoff(1,6) = 0.75:  IFoff(1,7) = 0.395
' IF Weights
'  IFwt(1,1) = 0.029: IFwt(1,2) = 0.229: IFwt(1,3) = 0.381
'  IFwt(1,4) = 1#: IFwt(1,5) = 0.381: IFwt(1,6) = 0.229
'  IFwt(1,7) = 0.029

Case 1 'ER2 Sensor Unit #1 - First MTP SU with Synthesizer
' Flew on ER2 during CRYSTAL-FACE
' IF offsets (Based on Spring 2002 JPL Lab measurements)
  Nif = 12
  IFoff(1, 1) = 0.2: IFoff(1, 2) = 0.23: IFoff(1, 3) = 0.25
  IFoff(1, 4) = 0.27: IFoff(1, 5) = 0.29: IFoff(1, 6) = 0.31
  IFoff(1, 7) = 0.33: IFoff(1, 8) = 0.35: IFoff(1, 9) = 0.37
  IFoff(1, 10) = 0.39: IFoff(1, 11) = 0.41: IFoff(1, 12) = 0.44
' IF Weights
  IFwt(1, 1) = 0.0015: IFwt(1, 2) = 0.0054: IFwt(1, 3) = 0.0581: IFwt(1, 4) = 0.5192
  IFwt(1, 5) = 0.9645: IFwt(1, 6) = 0.8392: IFwt(1, 7) = 0.6516: IFwt(1, 8) = 0.4745
  IFwt(1, 9) = 0.3022: IFwt(1, 10) = 0.0886: IFwt(1, 11) = 0.0138: IFwt(1, 12) = 0.0019

Case 2 'ER2 Sensor Unit #2 - Second MTP SU with Synthesizer
' Flew on WB57 during CRYSTAL-FACE
' IF offsets (Based on Spring 2002 JPL Lab measurements)
  Nif = 12
  IFoff(1, 1) = 0.2: IFoff(1, 2) = 0.23: IFoff(1, 3) = 0.25
  IFoff(1, 4) = 0.27: IFoff(1, 5) = 0.29: IFoff(1, 6) = 0.31
  IFoff(1, 7) = 0.33: IFoff(1, 8) = 0.35: IFoff(1, 9) = 0.37
  IFoff(1, 10) = 0.39: IFoff(1, 11) = 0.41: IFoff(1, 12) = 0.44
' IF Weights
  IFwt(1, 1) = 0.0056: IFwt(1, 2) = 0.0063: IFwt(1, 3) = 0.0654: IFwt(1, 4) = 0.6917
  IFwt(1, 5) = 0.964: IFwt(1, 6) = 0.9407: IFwt(1, 7) = 0.9015: IFwt(1, 8) = 0.8677
  IFwt(1, 9) = 0.7397: IFwt(1, 10) = 0.23: IFwt(1, 11) = 0.0155: IFwt(1, 12) = 0.0067

Case 3 'Gunn Diode ER2 Sensor Unit
' IF offsets
  Nif = 12
  IFoff(1, 1) = 0.18: IFoff(1, 2) = 0.25875: IFoff(1, 3) = 0.271: IFoff(1, 4) = 0.284
  IFoff(1, 5) = 0.3: IFoff(1, 6) = 0.318: IFoff(1, 7) = 0.336: IFoff(1, 8) = 0.355
  IFoff(1, 9) = 0.368: IFoff(1, 10) = 0.376: IFoff(1, 11) = 0.387: IFoff(1, 12) = 0.475
' IF Weights
  IFwt(1, 1) = 0.007: IFwt(1, 2) = 0.038: IFwt(1, 3) = 0.143
  IFwt(1, 4) = 0.464: IFwt(1, 5) = 1#: IFwt(1, 6) = 0.842
  IFwt(1, 7) = 0.488: IFwt(1, 8) = 0.586: IFwt(1, 9) = 0.184
  IFwt(1, 10) = 0.074: IFwt(1, 11) = 0.036: IFwt(1, 12) = 0.007
  
Case 4 'RKW - Rockwell MTP
  ans = MsgBox("No Bandpass currently available for RKW MTP!", vbOKOnly)
  Exit Sub

End Select
  frmFLTINFO.cboSU.Text = frmFLTINFO.cboSU.List(frmFLTINFO.cboSU.ListIndex)

  Nnu = 2 * Nif
  lstSU.Clear
  For i = 1 To Nif
  lstSU.AddItem Format$(i, "00") + "  " + Format$(IFoff(1, i), "0.000") + "  " + Format$(IFwt(1, i), "0.0000")
  Next i
  UpdateFormRet

End Sub


Private Sub chkDefNoise_Click()

End Sub

Private Sub chkOldDC8RC_Click()

'If chkOldDC8RC.value = 1 Then RCformat = 0 Else RCformat = 1

End Sub

Private Sub cboAlgorithm_Click()
Dim i%

Algorithm = cboAlgorithm.ListIndex
'0 "Undefined"
'1 "MRI"
'2 "LAT Blend"
'3 "LON Blend"
'4 "UT Blend"

Select Case Algorithm
Case 0
Case 1    'MRI
  For i = 0 To 3
    lblLAT(i).Enabled = False
    txtLAT(i).Enabled = False
  Next i

Case 2   'Lat Blend
  For i = 0 To 3
    lblLAT(i).Enabled = True
    txtLAT(i).Enabled = True
    lblLAT(i).Caption = "LAT" + Format(i + 1, "0")
  Next i
  
Case 3   'LON Blend
  For i = 0 To 3
    lblLAT(i).Enabled = True
    txtLAT(i).Enabled = True
    lblLAT(i).Caption = "LON" + Format(i + 1, "0")
  Next i

Case 4
  For i = 0 To 3
    lblLAT(i).Enabled = True
    txtLAT(i).Enabled = True
    lblLAT(i).Caption = "UT" + Format(i + 1, "0")
  Next i

Case 5    'Retrievable Blend
  For i = 0 To 3
    lblLAT(i).Enabled = False
    txtLAT(i).Enabled = False
    lblLAT(i).Caption = ""
  Next i

Case 6    'Observable Blend
  For i = 0 To 3
    lblLAT(i).Enabled = False
    txtLAT(i).Enabled = False
    lblLAT(i).Caption = ""
  Next i

End Select

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


Private Sub chkProcessFile_Click()
  frmFormatRAOB.chkTropopause.Value = 1  'Show tropopause by default
End Sub

Private Sub chkShowTotalError_Click()
Dim i%

  If chkShowTotalError.Value = 1 Then
    For i = 0 To 31
      txtWavg(i).Text = Format(Stats(3, i), "#0.0")
      txtWrms(i).Text = ""
    Next i
  Else
    For i = 0 To 31
      txtWavg(i).Text = Format(Stats(1, i), "#0.0")
      txtWrms(i).Text = Format(Stats(2, i), "#0.0")
    Next i
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

Private Sub cmdAutoScaleTA_Click()

AutoScaleTA = Not AutoScaleTA
If AutoScaleTA Then
  cmdAutoScaleTA.Caption = "Manual   Scale"
  txtT1.Enabled = False
  txtTC.Enabled = False
Else
  cmdAutoScaleTA.Caption = "  Auto     Scale"
  txtT1.Enabled = True
  txtTC.Enabled = True
End If

End Sub

Private Sub cmdBack_Click()
Forward = False
tmrGetCycle_Timer

End Sub

Private Sub cmdBG_Click()
Static DefaultBG As Boolean
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
    .PBC = bGray  'White              'Plot border color
    .PFG = White  'Black            'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Black              'Plot tic marks and border
    .Pgm = Black 'LGray              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
  End With
  End If
End Sub

Private Sub cmdCalculateTB_Click()
Dim flag(1 To 4) As Boolean, Freq!, Elev!, pALT!, TB0!, TB1!, TB2!, TB3!, TB4!, TB5!, dT!, HPBW!, i%, z!
Dim Mode As Boolean, Alpha1!, Alpha2!, Beta1!, Beta2!
'Public ZGraob!(1 To 2000)   'Geopotential Height (km)
'Public PZraob!(1 To 2000)   'Pressure (mb)
'Public TZraob!(1 To 2000)   'Temperature (K)
'Public RZraob!(1 To 2000)   'Relative Humidity (%)
'Public ZPraob!(1 To 2000)   'Pressure Altitude (km) - calculated from PZraob
'Public RVraob!(1 To 2000)   'Vapor density (gm/m3)

pALT = Val(txtZp.Text)
Elev = Val(txtElev.Text)
Freq = Val(txtFreq.Text)
chkEnableRAOBextension_Click
chksjkPextension_Click

'  If chkEnableRAOBextension.value = 1 Then
    ExtensionLR = Val(txtLR1.Text)
    ExtensionLR2 = Val(txtLR2.Text)
    LRbreakZ = Val(txtZb.Text)
'    Ztop = 40#
'    Call RAOBextension(Tref(), Zref(), Nref, LR1, zb, LR2, Ztop)
'  End If

HPBW = 7.6 'degrees
If chkFlag(0).Value = 1 Then flag(1) = True Else flag(1) = False
If chkFlag(1).Value = 1 Then flag(2) = True Else flag(2) = False
If chkFlag(2).Value = 1 Then flag(3) = True Else flag(3) = False
If chkFlag(3).Value = 1 Then flag(4) = True Else flag(4) = False

If chkUSSA.Value = 1 Then
' Use US Standard Atmosphere
  Nlevr = 51
  For i = 1 To 51
    z = i - 1
    ZPraob(i) = z
    ZGraob(i) = z
    PZraob(i) = fZtoP(z)
    TZraob(i) = fTstd(z)
    RVraob(i) = 0#
  Next i
End If

If chkSingleFrequency.Value = 1 Then Mode = True Else Mode = False
'Elev = 45#
'pALT = 20
'For i = -100 To 100 Step 2
'  Elev = i / 10
'  TB0 = fTB(pALT!, Elev! + HPBW / 2, Freq!, flag, TZraob(), ZPraob(), RVraob(), Nlevr, Mode)
'  TB1 = fTB(pALT!, Elev! + HPBW / 4, Freq!, flag, TZraob(), ZPraob(), RVraob(), Nlevr, Mode)
  TB2 = fTB(pALT!, Elev!, Freq!, flag, TZraob(), ZPraob(), RVraob(), Nlevr, Mode)
'  TB3 = fTB(pALT!, Elev! - HPBW / 4, Freq!, flag, TZraob(), ZPraob(), RVraob(), Nlevr, Mode)
'  TB4 = fTB(pALT!, Elev! - HPBW / 2, Freq!, flag, TZraob(), ZPraob(), RVraob(), Nlevr, Mode)
  txtTB(0).Text = Format(TB0, "000.00")
  txtTB(1).Text = Format(TB2, "000.00")
  txtTB(2).Text = Format(TB4, "000.00")
  Alpha1 = (TB2 - TB0) / 2#
  Alpha2 = (TB4 - TB2) / 2#
  Beta1 = 2# * ((TB2 + TB0) / 2 - TB1)
  Beta2 = 2# * ((TB2 + TB4) / 2 - TB3)
  dT = (4 / (3 * 3.14159)) * (Alpha1 - Alpha2) + 0.25 * (Beta1 - Beta2)
    
  txtTB(3).Text = Format(dT, "0.000")
  Debug.Print " "; Elev; Format(TB0, "000.00"); " "; Format(TB2, "000.00"); " "; Format(TB4, "000.00"); " "; Format(dT, "0.000"); " "; Format(Alpha1 - Alpha2, "0.000"); " "; Format(Beta1 - Beta2, "0.000")
  'Debug.Print " "; Elev; Alpha1; Alpha2; Beta1; Beta2
  'Debug.Print " "; Elev; TB0; TB1; TB2; TB3; TB4
'Next i
End Sub

Private Sub cmdEL_Click(Index As Integer)

Select Case Index
Case 0  'Add
  lstEL.AddItem txtEL.Text
  
Case 1  'Remove
  lstEL.RemoveItem (lstEL.ListIndex)
  
Case 2  'Clear
  lstEL.Clear
  
Case 3  'Default
  lstEL.Clear
  LoadListBox frmMTPsim.lstEL, "ELEV_" + Trim(frmFLTINFO.cboPlatforms.Text), 1, 1#

End Select

txtNel.Text = Str(lstEL.ListCount)
Nel = Val(txtNel.Text)
txtEL.Text = ""
Nobs = Nlo * (Nel - 1) + 1 'Assume 1 OAT level

End Sub

Private Sub cmdFL_Click(Index As Integer)
Dim ALTkm!

Select Case Index
Case 0  'Add
  ALTkm = Val(txtFL.Text)
  Select Case Trim(cboPaltUnit.Text)
  Case "km"
  Case "kft": ALTkm = ALTkm / kft_km
  Case "m": ALTkm = ALTkm / 1000#
  Case "ft": ALTkm = ALTkm / 1000 / kft_km
  End Select
  lstFL.AddItem Format(ALTkm, "#0.000")
  txtFL.Text = ""
Case 1  'Remove
  lstFL.RemoveItem (lstFL.ListIndex)
Case 2  'Clear
  lstFL.Clear
Case 3  'Default
  lstFL.Clear
'  LoadListBox frmMTPsim.lstFL, "ALT_" + Trim(cboPlatforms.Text), 3, 1#
  'For i = 1 To NFL: Pr(i) = fZtoP(Palt(i)): Next i
    
End Select
'txtNFL.Text = Str$(lstPalt.ListCount)
'NFL = Val(txtNFL.Text)
'NFL = NFL
End Sub

Public Sub cmdFwd_Click()
Dim z!, Zstd!(1 To 11)

If optTsource(0).Value = True Then
  Forward = True
  tmrGetCycle_Timer
Else
  If optTsource(1).Value = True Then
    Nlev = 11
    Zstd(1) = 0
    Zstd(2) = 11
    Zstd(3) = 20
    Zstd(4) = 24
    Zstd(5) = 28
    Zstd(6) = 30
    Zstd(7) = 32
    Zstd(8) = 47
    Zstd(9) = 51
    Zstd(10) = 71
    Zstd(11) = 84.852
    For i% = 1 To Nlevr
      z = Zstd(i)
      Zplot(i%) = z
      ZGraob(i) = z
      ZPraob(i) = z
      PZraob(i) = fZtoP(z)
      TZraob(i) = fTstd(z)
      Tplot(i) = TZraob(i)
      RZraob(i) = Val(txtRH.Text)
    Next i
    frmRAOB.ShowPressureAltitude = True
    frmRAOB.Caption = "RAOB - US Standard Atmosphere"
    cmdREF_Click (5)
  Else
    
  End If
End If
End Sub

Private Sub cmdGoTo_Click()
Dim RecordFound As Boolean, tks&, LoopCount%, RecInc%
  
On Error GoTo InvalidRequest

Select Case cboGoTo.Text
Case "Rec No"
  Rcycle = Val(txtGoTo) - 1
Case "ks"
  Rcycle = (Val(txtGoTo) * 1000 - UTmin) / ScanLength
Case "seconds"
  Rcycle = (Val(txtGoTo) - UTmin) / ScanLength
Case "hhmmss"
  Rcycle = (fTstringToSec&(txtGoTo, False) - UTmin) / ScanLength
Case "hh:mm:ss"
  Rcycle = (fTstringToSec&(txtGoTo, True) - UTmin) / ScanLength
End Select
' Check limits
  Select Case Rcycle
  Case Is < 0: Rcycle = 0
  Case Is > FileRecords: Rcycle = FileRecords - 1
  End Select
  
  RecordFound = False
  Forward = True
  LoopCount% = 0
  Do
    LoopCount% = LoopCount% + 1
    tmrGetCycle_Timer
    Select Case cboGoTo.Text
    Case "Rec No"
      RecordFound = True
    Case "ks"
      RecInc = (Val(1000 * txtGoTo.Text - UTsec)) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Rcycle = Rcycle + RecInc
    Case "seconds"
      RecInc = (Val(txtGoTo.Text - UTsec)) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Rcycle = Rcycle + RecInc
    Case "hhmmss"
      tks& = fTstringToSec&(txtGoTo.Text, False)
      RecInc = (tks& - UTsec) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Rcycle = Rcycle + RecInc
    Case "hh:mm:ss"
      tks& = fTstringToSec&(txtGoTo.Text, True)
      RecInc = (tks& - UTsec) / ScanLength
      If Abs(RecInc) < 1.5 Then RecordFound = True Else Rcycle = Rcycle + RecInc
    End Select
    Rcycle = Rcycle + RecInc
  Loop Until RecordFound Or LoopCount% > 5
  UpdateForm
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

Private Sub cmdLO_Click(Index As Integer)
Select Case Index
Case 0 'Add
'  lstLO.AddItem cboLO.Text
'  cboLO.Text = ""
  
Case 1 'Remove
  lstLO.RemoveItem lstLO.ListIndex
  
Case 2 'Clear
  lstLO.Clear
  
Case 3 'Default
  lstLO.Clear
  LoadListBox frmMTPsim.lstLO, "FREQ_" + Trim(frmFLTINFO.cboPlatforms.Text), 3, 1#

End Select
'lstLO.Sorted
txtNlo.Text = Str(lstLO.ListCount)
Nlo = Val(txtNlo.Text)
Channels = Nlo
For i = 1 To Nlo
  LO(i) = lstLO.List(i - 1)
Next i
End Sub

Private Sub cmdOpen_Click()

If cmdOpen.Caption = "&Open RAOB" Then
  mnuFileOpenBinary_Click
  cmdOpen.Caption = "&Close RAOB"
  cmdOpenRAOBfile.Caption = "&Close RAOB"
Else
  Close RAOBlu
  RAOBlu = 0
  cmdOpen.Caption = "&Open RAOB"
  cmdOpenRAOBfile.Caption = "&Open RAOB"
  txtProgress.Text = ""
End If
End Sub

Private Sub cmdOpenRAOBfile_Click()
cmdOpen_Click
End Sub


Private Sub cmdPalt_Click(Index As Integer)
Select Case Index
Case 0  'Add
  lstPalt.AddItem txtPalt.Text
  txtPalt.Text = ""
Case 1  'Remove
  lstPalt.RemoveItem (lstPalt.ListIndex)
Case 2  'Clear
  lstPalt.Clear
Case 3  'Default
  lstPalt.Clear
  LoadListBox frmMTPsim.lstPalt, "ALT_" + Trim(frmFLTINFO.cboPlatforms.Text), 3, 1#
'  lstPalt.AddItem "20.00"
'  Call ReadFLA(Platform$, Mission$)
'  For i = 1 To NFL: lstPalt.AddItem Format$(FLA(i) / 100, "#0.000"): Next i
'  'For i = 1 To NFL: Pr(i) = fZtoP(Palt(i)): Next i
    
End Select
txtNFL.Text = Str$(lstPalt.ListCount)
NFL = Val(txtNFL.Text)
NFL = NFL

End Sub



Private Sub cmdREF_Click(Index As Integer)
Dim ShowAircraft As Boolean, Sum1!(1 To 33), Sum2!(1 To 33), n%, AVG!, RMS!, X!, i%, j%, k%
Dim lu%

If Rcycle = 0 And optTsource(0).Value = True And Index <> 1 Then
  MsgBox "You have not opened a RAOB file!", vbOKOnly
  Exit Sub
End If

Select Case Index
Case 0 'Clear Plot
  frmRAOB.SetDefaultScales
  frmRAOB.RAOB_Plot_Set
  frmRAOB.RAOB_Plot_Axes_Labels
  frmRAOB.RAOB_Plot_Grid_Marks
  frmRAOB.RAOB_Plot_Tic_Marks

Case 1
  Close
  Quit = True
  Unload frmRAOB
  Unload frmFormatRAOB
  Unload frmStatus
  Unload frmNDP
  Unload Me  ' xxx This sets SaveOutput true for some reason
  End
  
Case 2, 3 'Calc TA and then do retrieval
  ExcessGndT = Val(txtExcessGndT.Text)
  Latitude = Val(txtlatitude.Text)
  For i = 0 To NRC - 1
    aRC(i) = fGetiRC(i, frmFLTINFO)
  Next i
  If Index = 2 Then
    Main (False)                      'Calc TAs and Retrieve
    If chkProcessFile.Value = 1 Then cmdFwd_Click
    DoEvents
  Else
    Main (True)                       'Retrieval Only
  End If

Case 4
  SelectedFL = 1
  SelectedRCset = 4
  n = 1000
  For j = 1 To NFL
    OBrms(j, 1) = 0.61
    OBrms(j, 2) = 0.5
    OBrms(j, 3) = 0.44
    OBrms(j, 4) = 0.37
    OBrms(j, 5) = 0.12
    OBrms(j, 6) = 0.32
    OBrms(j, 7) = 0.34
    OBrms(j, 8) = 0.37
    OBrms(j, 9) = 0.45
  
    OBrms(j, 10) = 0.37
    OBrms(j, 11) = 0.38
    OBrms(j, 12) = 0.38
    OBrms(j, 13) = 0.34
    OBrms(j, 14) = 0.33
    OBrms(j, 15) = 0.32
    OBrms(j, 16) = 0.33
    OBrms(j, 17) = 0.34
    OBrms(j, 18) = 0.36
    
    OBrms(j, 19) = 0.37
    OBrms(j, 20) = 0.38
    OBrms(j, 21) = 0.38
    OBrms(j, 22) = 0.34
    OBrms(j, 23) = 0.33
    OBrms(j, 24) = 0.32
    OBrms(j, 25) = 0.33
    OBrms(j, 26) = 0.34
    OBrms(j, 27) = 0.36
    OBrms(j, 28) = 0.3
  Next j
  For i = 1 To n
    Main (True)                      'Calc TAs and Retrieve
    If i = 1 Then
      lu = FreeFile
      Open "C:\MTP\Statistics.txt" For Output As lu
      For j = 1 To Nret - 1
        Print #lu, vbTab; RAav(SelectedRCset, SelectedFL, j);
      Next j
      Print #lu, vbTab; RAav(SelectedRCset, SelectedFL, Nret)

    End If
    For j = 1 To Nret
      X = RTavg(j) - RTav(SelectedRCset, SelectedFL, j)
      Sum1(j) = Sum1(j) + X
      Sum2(j) = Sum2(j) + X ^ 2
    Next j
  Next i
  For j = 1 To Nret - 1
    AVG = Sum1(j) / n
    Print #lu, vbTab; AVG;
  Next j
  Print #lu, vbTab; Sum1(Nret) / n
  For j = 1 To Nret - 1
    AVG = Sum1(j) / n
    If (Sum2(j) - n * AVG ^ 2) > 0 Then RMS = Sqr((Sum2(j) - n * AVG ^ 2) / (n - 1)) Else RMS = 0#
    Print #lu, vbTab; RMS;
  Next j
  AVG = Sum1(Nret) / n
  RMS = Sqr((Sum2(Nret) - n * AVG ^ 2) / (n - 1))
  Print #lu, vbTab; RMS
  Close lu
  
Case 5
  cmdREF_Click (0)   'Clear plot
  With frmRAOB
    ShowAircraft = .ShowAircraft
    .ShowAircraft = False
    .LineColor = White               'Plot current RAOB
    .RAOBupdate
    .ShowAircraft = ShowAircraft
  End With

Case 6               'Show Archive average profile
  frmRAOB.PlotSingleScan = False
  frmRAOB.LineColor = LGreen
  For i = 1 To Nlev
    Zplot(i) = ZAavg(i)
    Tplot(i) = TAavg(i)
  Next i
  frmRAOB.RAOBupdate

End Select

End Sub


Private Sub cmdRepeat_Click()

  Rcycle = Val(txtProgress) - 1

' Check limits
  Select Case Rcycle
  Case Is < 0: Rcycle = 0
  Case Is > FileRecords: Rcycle = FileRecords - 1
  End Select
  
  Forward = True
  tmrGetCycle_Timer
  UpdateForm
End Sub

Private Sub cmdSelect_Click(Index As Integer)
Dim X$, V!(1 To 8), OutRecord&, OUTlu%, ans, SaveCycle&, i%, j%, k%, jj%, jjj%
Dim TotalRec&, A$, hh$, mm$, xx!, Cycle&, Zp!, Ti!(1 To 300), T1sum!(1 To 300), T2sum!(1 To 300)
Dim RAOBsite$, lu%, n%, z!, NDstd!, Arg!, CurrentColor%, CurrentThickness%
Dim RecordList&(), Tr!, Tc!, Tsum1!, Tsum2!, Tavg!, Trms!
Dim RMS!, AVG!, kmax&, i1%, i2%, Noise!, Nz%(1 To 300), dT!, alpha!
Dim LR1!, LR2!, Zb!, ExtNoise!, Ztop!, Filename$, IyearE%, IdoyE%, IdayE%, ImonthE%
Static Ri!(1 To 300), RAOBcount%, ZPplot!(1 To 300), ZGplot!(1 To 300), RZplot!(1 To 300)
Static SortCycle%(), SortDandT() As Date, SortCount%, EventDT As Date
Static PlotAll As Boolean, DnT1 As Date, DnT2 As Date, Nref%, Zref!(1 To 300), Tref!(1 To 300)

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
  ReDim SortCycle%(1 To Val(txtFileRec.Text)), SortDandT(1 To Val(txtFileRec.Text))
  Do
    Cycle = Cycle + 1

    Call RAOBread(RAOBlu, Cycle)
    
'    If (fFilter And chkNegateFilter.value = 0) Or (Not fFilter And chkNegateFilter.value = 1) Then 'Make sure filter criteria are satisfied
      'Debug.Print WMOnumber, WMO4l
      TotalRec = TotalRec + 1
      V(1) = WMOnumber: V(2) = Iyear: V(3) = Imonth: V(4) = Iday: V(5) = Ihhmm
'                                12345678901234567890123456789
      If Val(WMO4l) <> 0 Then X$ = "    " Else X$ = WMO4l
      X$ = X$ + " " + fUsing("#0000 #000 #0 #0 #000", V())
      X$ = X$ + " " + Format$(ijkl, "0") + "   " + Format$(Cycle, "00000")
      lstSelect.AddItem X$
      i = lstSelect.ListCount
      SortDandT(i) = DandT
      SortCycle(i) = Cycle
      'Debug.Print x$ + " " + Format(3.6 * Ihour + 0.06 * Imin, "00.000")
      txtTotalRec.Text = Str$(TotalRec)
      DoEvents
'    End If
  Loop Until EOF(RAOBlu) Or Cycle = FileRecords
  Cycle = SaveCycle
  RAOBsite$ = Str(WMOnumber)  'save last search site
  txtTotalRec.BackColor = &HFF00&           'Green when finished searching
'  If chkAutoSort.value = 1 Then cmdSelect_Click (12)
  End If
  
Case 2  'Remove or Keep
  If lstSelect.SelCount = 1 Then
    lstSelect.RemoveItem (lstSelect.ListIndex)
  Else
    i = -1
    If cmdSelect(2).Caption = "&Remove" Then
      Do
        i = i + 1
        If lstSelect.Selected(i) Then lstSelect.RemoveItem (i): i = i - 1
      Loop Until i = lstSelect.ListCount - 1
    Else
      Do
        i = i + 1
        If Not lstSelect.Selected(i) Then lstSelect.RemoveItem (i): i = i - 1
      Loop Until i = lstSelect.ListCount - 1
    End If
  End If
  TotalRec = Val(lstSelect.ListCount)
  txtTotalRec.Text = Str(TotalRec)
  

  
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
'  Documentation$ = ""
  
Case 6 'Clear IJKL flags
  If RAOBlu = 0 Then
    MsgBox "You must open a RAOB file before using this command!", vbOKOnly
    Exit Sub
  End If

  For i = 0 To lstSelect.ListCount - 1
    X$ = lstSelect.List(i)
    Cycle = Right$(X$, 5)
    Mid$(X$, 28, 1) = "0"
    lstSelect.List(i) = X$
    Call RAOBread(RAOBlu, Cycle)
    ijkl = 0                    'Only parameter changed
    Call RAOBwrite(RAOBlu, Cycle)
  Next i
  
  
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
'  With frmRAOBsites
'    .optShow(0).value = True
'    .txtShow(0).Text = WMO4l
'    .cmdDetails_Click (0)
'  End With
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

Case 13
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
  


Case 16
'  frmRAOB.Documentation = Documentation$
  frmRAOB.RAOB_Plot_Documentation2

Case 17
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
  Open "C:\RAOB\SOLVE2\ENASrandom.RAOB2" For Random Access Read Write As OUTlu Len = Len(RAOB)
  Call RAOBwrite(OUTlu, 1)  'Write template
  LR1 = Val(txtLR1.Text) + fSDT(Val(txtExtensionNoise.Text))
  LR2 = Val(txtLR2.Text) + fSDT(Val(txtExtensionNoise.Text))
  Zb = Val(txtZb.Text) + fSDT(Val(txtExtensionNoise.Text))
  For i = 2 To 120
    Cycle = i
    Isec = Isec + 1
    If Isec > 59 Then Isec = 0: Imin = Imin + 1
    dT = fSDT(Noise)
    alpha = 0# 'fSDT(0.1)
    jjj = Int(fSDT(2))
    For j = 1 To Nlevr    'averaging pressure altitudes
      jj = j + jjj
      If jj < 1 Then jj = 1
      If jj > Nlevr Then jj = Nlevr
      TZraob(j) = Tref(jj) + dT + alpha * Zref(j)
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

End Select

End Sub

Private Sub cmdSelectedObs_Click()
Dim LineC%
  
  For i = 1 To Channels
    Select Case i
    Case 1: LineC = QBColor(4)  'RED
    Case 2: LineC = QBColor(7)  'WHITE
    Case 3: LineC = QBColor(1)   'BLUE
    End Select
    picTA.PSet (TAA(UserRCindex, i, 1), 1)
    For j = 2 To 10
      picTA.Line -(TAA(UserRCindex, i, j), j), LineC   'And continue from there
    Next j
  Next i

End Sub

Private Sub cmdShowAC_Click()

frmRAOB.ShowAircraft = Not frmRAOB.ShowAircraft
If frmRAOB.ShowAircraft Then
  cmdShowAC.Caption = "Hide A/C"
  frmFormatATP.chkAircraft.Value = 1
Else
  cmdShowAC.Caption = "Show A/C"
  frmFormatATP.chkAircraft.Value = 0
End If

End Sub

Private Sub cmdShowTrop_Click()
frmRAOB.ShowTropopause = Not frmRAOB.ShowTropopause
If frmRAOB.ShowTropopause Then
  cmdShowTrop.Caption = "Hide Trop"
  frmFormatRAOB.chkTropopause.Value = 1
Else
  cmdShowTrop.Caption = "Show Trop"
  frmFormatRAOB.chkTropopause.Value = 0
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

Private Sub cmdTest_Click()
'Call Test

End Sub

Private Sub cmdWCT_Click(Index As Integer)
Dim WCTlu%
Static GA!(1 To 300), PA!(1 To 300), RH!(1 To 300), Tt!(1 To 300), ZA!(1 To 300)
Static GAa!(1 To 300), PAa!(1 To 300), RHa!(1 To 300), TTa!(1 To 300), ZAa!(1 To 300)
Static GAb!(1 To 300), PAb!(1 To 300), RHb!(1 To 300), TTb!(1 To 300), ZAb!(1 To 300)

Dim Beforecycle&, AfterCycle&
Dim lu%, File$, Nevents%, Rec1&, Rec2&, T1!, T2!, Te!, Prefix$, PrefixR$, Wt1!, Wt2!
Dim i%, j%, ii%, jj%, M%, Tmin!, Tmax!, Traob!, Tmtp!, TZ1!(1 To 300), TZ2!(1 To 300), Zp1!(1 To 300), Zp2!(1 To 300), nTZ1%, nTZ2%
Static Nrla%, Nrlb%, Nrl%, Isame%, OUTlu%, RAOB1$, RAOB2$, AvgTdiff!, RmsTdiff!
Dim Ln1!, Ln2!, Ln3!, LT1!, Lt2!, Lt3!, D1!, D2!, Distance!, Line$, BMPname$, iNum%
Dim A$, iD%, iM%, iY%, UT0!, UT1!, UT2!, DT1 As Date, DT2 As Date, Filename$, T As Date
Dim aWinCor!(1 To 3, 1 To 10), aSum1!(1 To 3, 1 To 10), aSum2!(1 To 3, 1 To 10)
Static aZp!, aZt!, aTnav!, aTmtp!, aTt!, aLat!, aLon!, aRoll!, aPitch!, aTA!(1 To 3, 1 To 10)
Static Site1Lat!, Site1Lon!, Site1Distance!, Site2Lat!, Site2Lon!, Site2Distance!
Static Found1 As Boolean, Found2 As Boolean, UTsave!

' Open MISSION_RAOBSs.RAOB2 file if not already open
If RAOBlu = 0 Then mnuFileOpenBinary_Click
cmdWCT(0).Caption = "BUSY!"
Select Case Index
Case 0  'Get MTP Observables
  Prefix$ = txtPrefix.Text
  PrefixR$ = Prefix$ + "RAOB\"
'  AC$ = Mid$(Prefix$, 4, 2)
  If lstSelect.ListCount = 0 Then cmdSelect_Click (0)
  Ztop = Val(txtZtop.Text)
  OUTlu = FreeFile
  Open PrefixR$ + "WCT.txt" For Output As OUTlu
  WCTlu = FreeFile
  Open PrefixR$ + Mission$ + "_RAOBrangeWCT.txt" For Input As WCTlu
  
  Input #WCTlu, A$
  chkMultipleScans.Value = 1
  Do
GetNext:
    'N Day Month Year  RAOBs UT0 UT1 UT2
    '1 9 1 2003  BGDH  30.2  0 12
    '2 9 1 2003  BGTL  25.0  0 12
    '3 12  1 2003  ENJA  32.9  0 12
    '12345678901234567890
    Input #WCTlu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, A$
    YYYYMMDD$ = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
    UTsave = UT0
    i = InStr(1, A$, vbTab)
    RAOB1$ = Left$(A$, i - 1)
    ii = InStr(i + 1, A$, vbTab)
    RAOB2$ = Mid$(A$, i + 1, ii - i - 1)
    Call RollOver(iY, iM, iD, UT0)
    DTevent = DateSerial(iY, iM, iD) + TimeValue(fSecToTstring$(CLng(UT0 * 1000), True))
    txtEvent.Text = DTevent
    
    If RAOB2$ = RAOB1$ Then 'Only one site involved
'      If UT0 <= 43.2 Then UT1 = 0: UT2 = 12 Else UT1 = 12: UT2 = 24
      Filename$ = Prefix$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
      Call OpenREFfile(Filename$)
      txtATPgo.Text = Str(UTsave)
      cmdATPgo_Click
      'N%, aTA!(), aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!
      Call AverageNcyclesTA(Val(txtCyclesAveraged.Text), aTA(), aZp!, aTnav!, aTmtp!, aZt!, aTt!, aLat!, aLon!, aRoll!, aPitch!)
      Close REFlu
      If aZp = 0# Then GoTo NextOne
      Call FillTAboxes(aTA(), 10)
      txtAverageZp.Text = Format(aZp, "#0.00")
      txtAverageZt.Text = Format(aZt, "#0.00")
      txtAverageTt.Text = Format(aTt, "000.0")
      txtAverageTn.Text = Format(aTnav, "000.0")
      txtZp.Text = Format(aZp, "#0.00")
      txtFL.Text = Format(aZp, "#0.00")
      cmdFL_Click 2  'Clear Flight Level List Box
      cmdFL_Click 0  'Add Zp of aircraft
      Site1 = True
      UT1 = Int(UT0 / 3.6)
'      Do
'        UT1 = UT1 + 1
'      Loop Until UT1 Mod 3 = 0  ' Find first sounding after UT0 at 3,6,9,12,15,18,21,24 hours

      ' Before in 3 hour steps
'      DT1 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0)
'      Do
'        DT1 = DT1 - TimeValue("03:00:00")
'        Debug.Print "1 "; RAOB1$; " "; DT1
'        Call GetRAOB(8, DT1, RAOB1$, Found1)
'        DoEvents
'      Loop Until Found1
'      Debug.Print DT1; RAOB1$; Found1

'       Now get the before and after radiosondes
        DT1 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0)
        Call GetRAOBs(DT1, RAOB1$, Beforecycle&, AfterCycle&)
        If AfterCycle = 0 Or ZPraob(Nlevr) < aZp Then GoTo GetNext
        
        Call RAOBread(RAOBlu, Beforecycle)
'        OutCount = OutCount + 1
'        Call RAOBwrite(RAOBlu2, OutCount)
        DT1 = DandT
        nTZ1 = Nlevr
'        OAT11 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
      
      Site1Lat = WMOlatitude
      Site1Lon = WMOlongitude
      Site1Distance = fGCD(aLat, aLon, Site1Lat, Site1Lon)
      Call CopyArray(ZPraob(), Zp1(), Nlevr)
      Call CopyArray(TZraob(), TZ1(), Nlevr)
'      nTZ1 = Nlevr
'      ' After in 3 hour steps
'      DT2 = DateSerial(iY, iM, iD) + TimeSerial(UT1, 0, 0) - TimeValue("3:00:00")
'      Do
'        DT2 = DT2 + TimeValue("3:00:00")
'        Debug.Print "2 "; RAOB1$; " "; DT2
'        Call GetRAOB(9, DT2, RAOB1$, Found2)
'        DoEvents
'      Loop Until Found2
      
        Call RAOBread(RAOBlu, AfterCycle)
'        OutCount = OutCount + 1
'        Call RAOBwrite(RAOBlu2, OutCount)
        DT2 = DandT
'        OAT12 = fTinterp(aZp, ZPraob(), TZraob(), Nlevr)
      
      Call cmdBlend_Click(1)    'Temporal interpolation
      CalculateObservables aZp  'Calculate TAs for RAOB, predicted
      Call FillTAboxes(TA(), 0)
      OATnavCOR = TA(1, LocHor) - aTA(1, LocHor) 'predicted - observed
      txtOATnavCor.Text = Format(OATnavCOR, "#0.00")
      For i = 1 To Channels  'predicted - observed  - (predicted - observed)horizon (Make horizon value zero)
        For j = 1 To Nel: WINcor(i, j) = TA(i, j) - aTA(i, j) - OATnavCOR: Next j
      Next i
      Call FillTAboxes(WINcor(), 20)
      ' Print results
      Print #OUTlu, " " + RAOB1$ + YYYYMMDD$
      Print #OUTlu, Format(UT0, "00.0") + vbTab + Format(aZp, "00.00") + vbTab + Format(aZt, "00.00") + vbTab + Format(aTnav, "000.0") + vbTab + Format(aTt, "000.0")
      Print #OUTlu, " El        CH1        CH2         CH3   " + Format(OATnavCOR, "#0.00")
      For j = 1 To Nel
          Print #OUTlu, j; Format(WINcor(1, j), "#0.00"), Format(WINcor(2, j), "#0.00"), Format(WINcor(3, j), "#0.00")
      Next j
      
      If chkWait.Value = 1 Then GoSleep Val(txtWait.Text)
NextOne:
      Call cmdSelect_Click(5)  'Clear plot
    Else  'Need to do spatial interpolation
    
    End If
    DoEvents
  Loop Until EOF(WCTlu)
  Close OUTlu, WCTlu

End Select
cmdWCT(0).Caption = "Calculate WCT"

End Sub
Sub RAOBextension(TZraob!(), ZPraob!(), Nlevr%, LR1!, Zb!, LR2!, Ztop!)
Dim j%
      
  If ZPraob(Nlevr) > Zb And ZPraob(Nlevr) < Ztop And Nlevr < nRAOBlevelsOUT Then
    If ZPraob(Nlevr) < Ztop Then
      For j = 3 To 1 Step -1
        TZraob(Nlevr + j + 1) = TZraob(Nlevr + j)
        ZPraob(Nlevr + j + 1) = ZPraob(Nlevr + j)
        PZraob(Nlevr + j + 1) = PZraob(Nlevr + j)
      Next j
      TZraob(Nlevr + 1) = TZraob(Nlevr) + LR2 * (Ztop - ZPraob(Nlevr))
      ZPraob(Nlevr + 1) = Ztop
      PZraob(Nlevr + 1) = fZtoP(Ztop)
      ZGraob(Nlevr + 1) = ZGraob(Nlevr) + fHypsometricEqn(TZraob(Nlevr), PZraob(Nlevr), TZraob(Nlevr + 1), PZraob(Nlevr + 1))
      Nlevr = Nlevr + 1
    End If
  Else
    If Nlevr < nRAOBlevels - 1 Then
      For j = 3 To 1 Step -1
        TZraob(Nlevr + j + 2) = TZraob(Nlevr + j)
        ZPraob(Nlevr + j + 2) = ZPraob(Nlevr + j)
        PZraob(Nlevr + j + 2) = PZraob(Nlevr + j)
      Next j
      TZraob(Nlevr + 1) = TZraob(Nlevr) + LR1 * (Zb - ZPraob(Nlevr))
      ZPraob(Nlevr + 1) = Zb
      PZraob(Nlevr + 1) = fZtoP(Zb)
      ZGraob(Nlevr + 1) = ZGraob(Nlevr) + fHypsometricEqn(TZraob(Nlevr), PZraob(Nlevr), TZraob(Nlevr + 1), PZraob(Nlevr + 1))
      TZraob(Nlevr + 2) = TZraob(Nlevr + 1) + LR2 * (Ztop - ZPraob(Nlevr + 1))
      ZPraob(Nlevr + 2) = Ztop
      PZraob(Nlevr + 2) = fZtoP(Ztop)
      ZGraob(Nlevr + 2) = ZGraob(Nlevr + 1) + fHypsometricEqn(TZraob(Nlevr + 1), PZraob(Nlevr + 1), TZraob(Nlevr + 2), PZraob(Nlevr + 2))
      Nlevr = Nlevr + 2
    End If
  End If

End Sub


Sub GetRAOB(Index%, dT As Date, RAOB$, Found As Boolean)
Dim i%, X$, Cycle&
  Found = False
  For i = 0 To lstSelect.ListCount - 1
    X$ = lstSelect.List(i)
    Cycle = Right$(X$, 5)
    Call RAOBread(RAOBlu, Cycle)
    If Not fNumber(Left$(RAOB$, 1)) Then
      If DandT = dT And Trim(WMO4l) = RAOB$ Then
        lstSelect.ListIndex = i
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


Function fNumber(L$) As Boolean

Select Case Asc(L$)
Case Is < 58: fNumber = True
Case Else: fNumber = False
End Select
End Function

Sub OpenREFfile(Filename$)

If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
  Exit Sub
Else
'  On Error GoTo errhandler
  If REFlu <> 0 Then Close REFlu
  REFlu% = FreeFile
  Open Filename$ For Random Access Read Write As REFlu% Len = Len(REF2)
  txtREFfile.Text = Filename$
  REFfileRecords = LOF(REFlu%) \ Len(REF2) - HiddenRecords
'  ProgressBar2.min = 0
'  ProgressBar2.Max = REFfileRecords
'  txtREFrecords.Text = REFfileRecords
  REFread REFlu, REFfileRecords  'Get Last Record
  ScanLength = UTsec
  REFread REFlu, 1               'Get First Record
  If UTsec > ScanLength Then ScanLength = ScanLength + 86400
  ScanLength = (ScanLength - UTsec) / (REFfileRecords - 1)
'  cmdOpenREFfile.Caption = "Close REF"
   REFreadHEADER Filename$
'  REFreadLIMITS Filename$
'  REFreadFLTINFO Filename$

End If
  
'  REFreadLIMITS FileName$
'  REFreadFLTINFO FileName$

'  Initialize
'  tmrGetCycle2.Interval = 100 'Auto Step cycle time in ms
'  tmrGetCycle2.Enabled = False
'  cmdREFloc(1).Caption = "&Continue"
'  Cycle2 = 0
'  txtREFrecord.Text = Str(Cycle2)
'  frmATP.Visible = True
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


Private Sub Form_DblClick()
Call CaptureFormMJ(Me, "", 3, True)
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

Private Sub optSite_Click(Index As Integer)
If Index = 0 Then Site1 = True Else Site1 = False

End Sub

Private Sub tmrGetCycle2_Timer()
Dim i%, j%, V!(1 To 9), X$
NextCycle:
  Call GetREFcycle(Cycle2, False)
  If Not GoodScan Then GoTo NextCycle
'  ProgressBar2.value = Cycle2
'  txtREFrecord.Text = Str(Cycle2)
'  If Nlev > 0 And Nlev <= 33 Then frmATP.ATPupdate
  V(1) = UTsec: V(2) = pALT: V(3) = gALT: V(4) = Latitude: V(5) = Longitude: V(6) = Heading
  V(7) = Pitch: V(8) = Roll: V(9) = Tac
'                           HHMMSS UTsec pALT gALT    Lat     Lon Hdg Pitch  Roll OAT_K
  X$ = fSecToTstring$(UTsec, False) + fUsing(" #####0 #0.0 #0.0 ##0.00 ###0.00 ##0 ##0.0 ##0.0 #00.0", V())
  txtCommLine.Text = X$
  txtAverageZp.Text = Format(pALT, "00.000")
'  txtTac.Text = Format(Tac, "000.00") '
'  txtRCindex1.Text = Reg(RCindex1)
'  If RCindex2 > 0 Then txtRCindex2.Text = Reg(RCindex2) Else txtRCindex2 = ""
  
  DoEvents

End Sub
Sub GetREFcycle(Cycle%, ByVal PlotUpdateFlag As Boolean)
Dim Zt!, Tt!, Th!, Lt!(1 To 33), LT1%, Lt2

'Read new profile
GetaCycle:
  If EOF(REFlu%) Then Exit Sub 'XXXX
  If Forward2 Then Cycle = Cycle + 1 Else Cycle = Cycle - 1
  
  If Cycle% < 1 Then
    Cycle% = 1
    If tmrGetCycle2.Enabled Then tmrGetCycle2.Enabled = False ': cmdREFloc(1).Caption = "&Continue"
  End If
  
  If Cycle% > REFfileRecords Then
    Cycle% = REFfileRecords
    If tmrGetCycle2.Enabled Then tmrGetCycle2.Enabled = False ': cmdREFloc(1).Caption = "&Continue"
  End If
  Call REFread(REFlu%, Cycle)
  
' Select only Cycles that have good trops
  If Nlev1 > 0 Then
    LT1 = 1: Lt2 = 1
    For i = Nlev1 To Nlev2
'      If chkGeoPotHeight(0).value = 1 Then
'        RAp(i - Nlev1 + 1) = Zgeo(i) / 100
'        Lt(i - Nlev1 + 1) = zzz(i) / 100
'        If Lt(i - Nlev1 + 1) > Zt1 And Lt1 = 1 And Zt1 < 90# Then Lt1 = i - Nlev1
'        If Lt(i - Nlev1 + 1) > Zt2 And Lt2 = 1 And Zt2 < 90# Then Lt2 = i - Nlev1
'      Else
        RAp(i - Nlev1 + 1) = zzz(i) / 100
'      End If
      RTp(i - Nlev1 + 1) = TTT(i) / 10
    Next i
  End If
  ' Calculate trops in terms of geopotential height if necessary
'  If chkGeoPotHeight(0).value = 1 Then
'    If Lt1 > 1 Then
'      f = (Zt1 - Lt(Lt1)) / (Lt(Lt1 + 1) - Lt(Lt1))
'      Zt1 = RAp(Lt1) + f * (RAp(Lt1 + 1) - RAp(Lt1))
'    End If
'    If Lt2 > 1 Then
'      f = (Zt2 - Lt(Lt2)) / (Lt(Lt2 + 1) - Lt(Lt2))
'      Zt2 = RAp(Lt2) + f * (RAp(Lt2 + 1) - RAp(Lt2))
'    End If
'  End If
'  With frmATP
'    .Zt1 = Zt1
'    .Zt2 = Zt2
'    .ALTkm = pALT
'    If chkEnableFit.value = 1 Then
'      .EnableFit = True
'    Else
'      .EnableFit = False
'    End If
'    If chkShowTropopause.value = 1 Then
'      .ShowTropopause = True
'    Else
'      .ShowTropopause = False
'    End If
'  End With
  'UpdateForm
  'mjmjmjm 20050419 commented out following line due to DateTime overflow if >24 hours
  'txtEvent.Text = DateValue(Left$(yyyymmdd$, 4) + "/" + Mid$(yyyymmdd$, 5, 2) + "/" + Right$(yyyymmdd$, 2)) + DateTime(fSecToTstring(UTsec, True))
  Tac = OATmtp  'For PlotUpdate
'  If PlotUpdateFlag Then frmATP.ATPupdate

End Sub


Private Sub cmdZT_Click(Index As Integer)
Dim z!, T!, i%, Filename$, lu%

Select Case Index
Case 0  'Add
  lstZ.AddItem txtZ.Text
  lstT.AddItem txtT.Text
  txtZ.Text = ""
  txtZ.Text = ""

Case 1  'Remove
  i = lstT.ListIndex
  lstZ.RemoveItem (i)
  lstT.RemoveItem (i)

Case 2  'Clear
  lstZ.Clear
  lstT.Clear

Case 3  'Open File
  lstZ.Clear
  lstT.Clear
' Set Initial directory
  CommonDialog1.InitDir = "c:\MTP\RAOB\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "ATP Files (*.TXT)|*.TXT|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen
  Filename$ = CommonDialog1.Filename
  txtSaveAs.Text = Filename$
  lu = FreeFile
  Open Filename$ For Input As #lu
  i = 0
  Do
    i = i + 1
    Input #lu, z, T
      Zplot(i%) = z
      ZGraob(i) = z
      ZPraob(i) = z
      PZraob(i) = fZtoP(z)
      TZraob(i) = T
      Tplot(i%) = T
      RZraob(i) = Val(txtRH.Text)
      lstT.AddItem T
      lstZ.AddItem z
  Loop Until EOF(lu)
  Close lu
  Nlev = i
  frmRAOB.ShowPressureAltitude = True
  frmRAOB.Caption = "User File"
  cmdREF_Click (5)

Case 4 'Save As
  lu = FreeFile
  Open txtSaveAs.Text For Output As lu
  For i = 0 To lstZ.ListCount - 1
    Print #lu, lstZ.List(i); lstT.List(i)
  Next i
  Close lu
  
End Select
End Sub

Private Sub Form_Load()
Dim RetVal

  Set CallingForm = frmMTPsim
  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"
  ReadSETUP ("SYSTEM")

  PgmDrive$ = ProgramDrive$
  Drive$ = DataDrive$
  txtPrefix.Text = Drive$ + Rdir2$ + Mission$ + "\"
  nRAOBlevelsOUT = 150
  
  With cboPTfile
  .AddItem "NO"
  .AddItem "PT"
  End With
'  UserName$ = "mj"
  Show
  Select Case AC$
  Case "DC", "NG": txtFL.Text = "10.7"
  Case Else: txtFL.Text = "18.0"
  End Select
  cmdFL_Click (0)

  With cboATPgo
  .AddItem "Rec No"
  .AddItem "ks"
  .AddItem "seconds"
  .AddItem "hhmmss"
  .AddItem "hh:mm:ss"
  .ListIndex = 1
  End With

  With tmrGetCycle
  .Enabled = False
  .Interval = 50
  End With

  Load frmStatus
  With frmStatus
  .Top = 720
  .Left = Screen.Width - frmStatus.Width - 1000
  .Show
  .optZx(0).Value = True
  .Visible = False
  End With
  ShowZp = True

  With frmMTPsim
  .Top = 0
  .Left = 1024 * Screen.TwipsPerPixelX - .Width
  .SSTab1.Tab = 0
  End With

  With frmFLTINFO
  .Top = frmMTPsim.Height
  .Left = frmMTPsim.Left + frmMTPsim.Width - frmFLTINFO.Width
  .Height = 5265
  .Show
  End With

'  LoadComboBox frmMTPsim.cboAlgorithm, "ALGORITHM"
  Algorithm = 6
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
  
' Default retrieval transition latitudes
  defLAT1 = 18: defLAT2 = 28: defLAT3 = 45: defLAT4 = 53
  
  EditSelect = False  'Will toggle on click below
  
' ATP options
' Set up ATP Options Form
  With frmRAOB
  .Top = 0
  .Left = 0
  .Width = 1024 * Screen.TwipsPerPixelX - frmMTPsim.Width 'Size for 800x600
  .Height = frmMTPsim.Height
  .LineColor = White
  .PlotSingleScan = True
  .RAOB_Initialize
  End With

' If DoAll Then ShowATP = False Else
  ShowATP = True

  frmRAOB.ShowTropopause = True
  cmdShowTrop_Click
  frmRAOB.ShowAircraft = False
  cmdShowAC_Click

  With frmFormatRAOB
  .Visible = False
  .chkAircraft.Value = 1
  .chkTropopause.Value = 0
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
  
  With cboGoTo
    .AddItem "Rec No"
    .AddItem "ks"
    .AddItem "seconds"
    .AddItem "hhmmss"
    .AddItem "hh:mm:ss"
    .ListIndex = 0
  End With

  With cboPaltUnit
    .AddItem "km"
    .AddItem "kft"
    .AddItem "m"
    .AddItem "ft"
    .ListIndex = 0
  End With
  
  With cboFilter  'Doesn't appear to be used anywhere
  .AddItem "CH1"
  .AddItem "CH2"
  .AddItem "CH3"
  .AddItem "SQUARE"
  .ListIndex = 0
  End With

  ShowNDP = False

  MVindex = 0
  MVindexMax = 200
  GVindex = 0
  GVindexMax = 200

  frmRAOB.PlotSingleScan = True
  frmNDP.PlotSingleScan = True
  AutoScaleTA = False
  cmdAutoScaleTA_Click
  optOBorder(2).Value = True
  optTsource(0).Value = True
  tmrGetCycle.Enabled = False
  
  UpdateForm

  Pause = False
  
  Initialize

End Sub

Sub InitializeER()
Dim A$, lu%, i%, hdr%

  Ceiling = 24#      'ER2 Service ceiling
  'LocHor% = 6        'Horizon on Scan position 6 '''Have from FltInfo
  
  ChInfo(2) = 0#       'Ch2's OAT info
  ChInfo(3) = 0#       'Ch3's OAT info
  ChInfo(1) = 1.5      'Ch1's OAT info
  If Channels >= 2 Then ChInfo(2) = 1#  'Ch2's OAT info
  If Channels >= 3 Then ChInfo(3) = 1#  'Ch3's OAT info
  
'  vWB$ = "9904010.0"        'Current version of program
Select Case SU$
Case "ER2G"
' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
  GeqnMax(1) = 18: GeqnMax(2) = 18: GeqnMax(3) = 18
  GeqnMin(1) = 8: GeqnMin(2) = 8: GeqnMin(3) = 8

' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
  GnavMax(1) = 22: GnavMax(2) = 22: GnavMax(3) = 22
  GnavMin(1) = 8: GnavMin(2) = 8: GnavMin(3) = 8

' Noise Diode MINs were:     MAXs were:
  GndMax(1) = 16: GndMax(2) = 16: GndMax(3) = 16
  GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 7
'  txtGain1.Text = 5: txtGain2.Text = 22
  
Case "ER2S"
' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
  GeqnMax(1) = 45: GeqnMax(2) = 45: GeqnMax(3) = 45
  GeqnMin(1) = 15: GeqnMin(2) = 15: GeqnMin(3) = 15

' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
  GnavMax(1) = 45: GnavMax(2) = 45: GnavMax(3) = 45
  GnavMin(1) = 15: GnavMin(2) = 15: GnavMin(3) = 15

' Noise Diode MINs were:     MAXs were:
  GndMax(1) = 16: GndMax(2) = 16: GndMax(3) = 16
  GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 7
  
End Select
'  frmRAOB.FooterType = 0
  CycleTime = 15.3           'Time for one MTP cycle (s)
  'rcfi$, OBav!(), OBrms!(), rc!(), RTav!(), RMSa!(), RMSe!()
  For i = 0 To NRC - 1
    'ReadInRC(RCpath$, Reg$, iRC%, OBav!(), OBrms!(), rc!(), RTav!(), RMSa!(), RMSe!())
    Call ReadInRC(txtRCpath.Text + RCs$ + Reg$(i), i)
    txtRCformat(i).Text = Str$(RCformat(i))
  Next i
' Will read final fudge of retrieved T(Z), ADD!
'  lu% = FreeFile
'  Open Drive$ + Rdir$ + Mission$ + "\" + Mission$ + ".FDT" For Input As lu%
'  Input #lu%, hdr%
'  For i = 1 To hdr%: Line Input #lu%, A$:  Next i: i = 1
'  Do: Input #lu%, FinalZ(i), FinalDT(i): i = i + 1: Loop While FinalZ(i - 1) <> 0
'  Close #lu%
'  LFinalmax% = i - 1
  
End Sub
Sub InitializeDC()
Dim A$, lu%, hdr%, Nhdr%
  
  'vWB$ = "199901310.0"        'Current version of program
  Ceiling = 13.7  'DC8 service ceiling
  
  ChInfo(2) = 0#       'Ch2's OAT info
  ChInfo(3) = 0#       'Ch3's OAT info
  ChInfo(1) = 0.9      'Ch1's OAT info
  If Channels >= 2 Then ChInfo(2) = 1#  'Ch2's OAT info
  If Channels >= 3 Then ChInfo(3) = 1#  'Ch3's OAT info
  
' Gain equation coefficients
  Cifa(1) = -0.134255685: Clo2(1) = -0.085272429: Clo1(1) = 0.040783428: Coff(1) = 14.73671718
  Cifa(2) = -0.279103355: Clo2(2) = 0.320803662: Clo1(2) = 0.018203484: Coff(2) = 4.380520217
  Cifa(3) = -0.249018496: Clo2(3) = 0.34056574: Clo1(3) = 0.011131299: Coff(3) = 2.41735786

' Gain Equation MINs were 6.0/7.5/8.5 MAXs were 9.0/10.5/12.0
  GeqnMax(1) = 9: GeqnMax(2) = 10: GeqnMax(3) = 10
  GeqnMin(1) = 5: GeqnMin(2) = 6: GeqnMin(3) = 6

' Nav/DADS MINs were:  5/7/7  MAXs were: 9/12/12
  GnavMax(1) = 9: GnavMax(2) = 10: GnavMax(3) = 10
  GnavMin(1) = 5: GnavMin(2) = 6: GnavMin(3) = 6

' Noise Diode MINs were:     MAXs were:
  GndMax(1) = 8: GndMax(2) = 13: GndMax(3) = 14
  GndMin(1) = 5: GndMin(2) = 7: GndMin(3) = 9
'  txtGain1.Text = 5: txtGain2.Text = 10

  'frmATP.FooterType = 1
  vD2$ = "D2MJ.BAS 980601.0"
  RollWarnCrit = 5          'Roll warning criterion
  RollWarnFlg = False       '
  RHS = True                'Assume SU on RHS after 960325
  Remote = True             'Allow remote control commands
  CAC = True                'Chassis Analysis Computer in use?
  LocHor% = 6               'Horizon on Scan position 6 not 5
  If Val(YYYYMMDD$) < 19960325 Then RHS = False: LocHor% = 5

' Then Variables
  CycleTime = 16.67          'Time for one MTP cycle (s)
  Ccycle = 0                 'Counts Quality Count
  
'  On Error GoTo Error_Handler  'Set up to handle Run Time errors (in D-mode only)
  Source$ = "F"
' Read Retrieval Coefficients
    lblProgress.Caption = "Reading RCs"
    DoEvents
'    If RCformat(i) > 0 Then
    ' rc$ + Reg0$, OBav0!(), OBrms(), RC0!(), RTav0!(), RMSa0!(), RMSe0!()
'      For i = 0 To NRC - 1
'        Call ReadInRCdcNew(txtRCpath.Text + RCs$, Reg$(i), i, OBav!(), OBrms(), rc!(), RTav!(), RMSa!(), RMSe!())
'      Next i
'    Else
'      For i = 0 To NRC - 1
'        Call ReadInRCdc(txtRCpath.Text, Reg$(i), i, OBavA(), OBavB(), RCa(), RCb(), RTav())
'      Next i
'    End If
  For i = 0 To NRC - 1
'    Call ReadInRCdcNew(txtRCpath.Text + RCs$, Reg$(i), i, OBav!(), OBrms(), rc!(), RTav!(), RMSa!(), RMSe!())
    Call ReadInRC(txtRCpath.Text + RCs$ + Reg$(i), i)
    txtRCformat(i).Text = Str$(RCformat(i))
  Next i

' Will read final fudge of retrieved T(Z), ADD!
'  lu% = FreeFile
'  Open Drive$ + Rdir$ + Mission$ + "\" + Mission$ + ".FDT" For Input As lu%
'  Input #lu%, hdr%
'  For i = 1 To hdr%: Line Input #lu%, A$:  Next i: i = 1
'  Do: Input #lu%, FinalZ(i), FinalDT(i): i = i + 1: Loop While FinalZ(i - 1) <> 0
'  Close #lu%
'  LFinalmax% = i - 1
  
' Code below removed as wcc not used anywhere xxx mjm
' Just one more fudge initialization: wcc(), to shift WCT for TB1 above horizon
'  lu% = FreeFile
'  Open CALfile$ For Input As lu%
'  Input #lu%, Nhdr: For i = 1 To Nhdr + 15: Line Input #lu%, A$: Next i
'  For i = 1 To 5: Input #lu%, wcc(i): Next i
'  Close #lu%
'
End Sub

Private Sub lstSelect_Click()
Dim X$

X$ = lstSelect.List(lstSelect.ListIndex)
CurrentRecord = Val(Right$(X$, 5))
lblProgress.Caption = "Record"
txtProgress.Text = Str$(CurrentRecord)

End Sub

Private Sub mnuFileOpenBinary_Click()
Dim Filename$

' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir$ + Mission$ + "\"
' Set Size of FileName buffer
CommonDialog1.MaxFileSize = 1024
' Set filters.
CommonDialog1.Filter = "New RAOB Files (*.RAOB2)|*.RAOB2|Old RAOB Files (*.RAOB)|*.RAOB|Large RAOB file (*.LRF)|*.LRF|All Files (*.*)|*.*"
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
  Case "RAOB2": RAOBtype = 0
  Case "RAOB": RAOBtype = 1
  Case "LRF": RAOBtype = 2
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
  End Select
  txtFileRec = Str$(FileRecords)
  DoEvents
  'ProgressBar2.Min = 0
 ' ProgressBar2.Max = FileRecords
  'frmREFview.txtFileRecords.Text = FileRecords
End If
'  optMode(1).value = True
  Rcycle = 0
  'Main
  cmdOpen.Caption = "&Close RAOB"
  cmdFwd_Click
'  txtASCIIout.Text = fReplaceExtension(filename$, "txt")

End Sub

Private Sub mnuFileReadFLTINFO_Click()
Dim lu%, Filename$

' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir$ + Mission$
' Set filters.
CommonDialog1.Filter = "REF Files (*.REF)|*.REF|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

Filename$ = CommonDialog1.Filename

If Filename$ = "" Then Exit Sub

Select Case UCase$(fExtension(Filename$))
Case "INI"
  ReadFLTINFO (Filename$)
Case "REF"
  REFreadHEADER (Filename$)
End Select

UpdateForm

End Sub

Private Sub mnuFileExit_Click()
  Close
  Quit = True
  Unload frmRAOB
  Unload frmFormatRAOB
  Unload frmStatus
  Unload frmNDP
  Unload Me  ' xxx This sets SaveOutput true for some reason
End Sub

Private Sub mnuHelpAbout_Click()
'Set properties
frmAbout.Application = "MTP Simulation Program"
frmAbout.Heading = "Release April 10, 2003"
frmAbout.Copyright = "2003 MJ Mahoney"
'Call a method
frmAbout.Display

End Sub

Private Sub mnuOptionsFormatRAOB_Click()
    
  With frmFormatRAOB
    .Top = frmMTPsim.Top + frmMTPsim.Height
    .Left = frmMTPsim.Left
    '.Width = frmRAOB.Width
    '.Height = frmRAOB.Height
  End With

  Load frmFormatRAOB
  frmFormatRAOB.Visible = True

End Sub

Private Sub mnuOptionsFormatATP_Click()

End Sub


Private Sub mnuOptionsShowATP_Click()

End Sub

Private Sub mnuOptionsShowNDP_Click()
  ShowNDP = Not ShowNDP
  If ShowNDP Then
    mnuOptionsShowNDP.Caption = "Hide &NDP"
    frmNDP.Visible = True

    With frmNDP
      .NDPmonth = Val(Mid$(YYYYMMDD$, 5, 2))
      .Top = frmATP.Top + frmATP.Height
      .Left = frmATP.Left
      .Width = frmATP.Width
      .Height = frmATP.Height
     '.NDP_Initialize
    End With
  Else
    mnuOptionsShowNDP.Caption = "Show &NDP"
    frmNDP.Visible = False

  End If

End Sub



Private Sub mnuWriteFLTINFO_Click()
Dim lu%
' Write ASCII FLTINFO
  UpdateFLTINFO
  
  WriteFLTINFO ("C:\MTP\Setup\FLTINFO.INI")
  WriteFLTINFO (Path$ + AC$ + YYYYMMDD$ + ".INI")

' Write Binary REF file records and C:\MTP\FLTINFO.REF
  If SaveOutput Then REFwriteHEADER REFfile$

  REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"

End Sub








Sub UpdateREFfile()
  frmFLTINFO.cboFltDates.ListIndex = Lindex
  frmFLTINFO.cboFltNos.ListIndex = Lindex
  frmFLTINFO.cboObjectives.ListIndex = Lindex
  YYYYMMDD$ = Trim(frmFLTINFO.cboFltDates.Text)
  yymmdd$ = Right$(YYYYMMDD$, 6)
  Path$ = MNpath$ + "\" + YYYYMMDD$ + "\"
  txtREFfile = Path$ + AC$ + YYYYMMDD$ + ".REF"
End Sub
Sub UpdateForm()
Dim LineC&, j%, ans As Variant

  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$
  cboPlatforms_Click
  cboSU_Click
  For i = 1 To NRCbin
    lblRCname(i - 1).ForeColor = QBColor((i + 8) Mod 15)
    lblRCname(i - 1).Caption = frmFLTINFO.cboReg(i - 1).Text
'    lblRC(i - 1).Caption = cboReg(i - 1).Text
  Next i
  ATPrange = Val(txtATPrange.Text)
'  ans = fReadFile_XXX("IFB", frmFLTINFO.cboSU.Text)
'  If Not ans Then
'    MsgBox "Unable to read IF bandpass file!", vbOKOnly
'  End If
  AC$ = Mid$(Rdir$, 2, 2)
  Root$ = Drive$ + Rdir$
  yymmdd$ = Right$(YYYYMMDD$, 6)
  Path$ = Root$ + Mission$ + "\" + YYYYMMDD$ + "\"
  REFfile$ = Path$ + AC$ + YYYYMMDD$ + ".REF"
  RCs$ = Left$(AC$, 1) + "RC"
  RCpath$ = MNpath$ + "\RC\"
  txtRCpath.Text = RCpath$

  If Channels = 0 Then Channels = 2
  txtREFfile = Path$ + AC$ + YYYYMMDD$ + ".REF"
'  txtTropMin = TropMin: txtTropMax = TropMax
'  txtRetAltMin = RetAltMin: txtRetAltMax = RetAltMax
'  txtZtOffA = ZtOffA: txtZtOffB = ZtOffB
'  txtOATmin = OATmin: txtOATmax = OATmax
'  txtMRI = Format$(MRI, "##0.00")


txtLAT(0) = Str$(LAT1): txtLAT(1) = Str$(LAT2)
txtLAT(2) = Str$(LAT3): txtLAT(3) = Str$(LAT4)

'UpdateTaWindow
  For i = 0 To NRC - 1
    frmFLTINFO.UpdateRCs (i)
  Next i

End Sub

Sub UpdateTBwindow()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, Tstep!
'If Not ShowDiagnostics Then Exit Sub

'Plot Antenna Tempertures in picture window
  With picTA
    .Cls
    .ScaleMode = 0
    .ScaleTop = 1
    .ScaleHeight = 9
  End With
' Set temperature display scale
  If AutoScaleTA Then
    Tmin = 500
    Tmax = 100
    For i = 1 To 10
      T = TA(1, i)
      If T > Tmax Then Tmax = T
      If T < Tmin Then Tmin = T
    Next i
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
    For j = 1 To 10
      picTA.Line -(TA(i, j), j), LineC    'And continue from there
    Next j
  Next i
If chkShowArcAvg.Value = 1 Then
' Draw archive average TAs
  For i = 1 To Channels
    Select Case i
    Case 1: LineC = QBColor(4)  'RED
    Case 2: LineC = QBColor(7)  'WHITE
    Case 3: LineC = QBColor(1)   'BLUE
    End Select
    picTA.PSet (TAA(aRC(IlnPmin1), i, 1), 1)
    For j = 2 To 10
      picTA.Line -(TAA(aRC(IlnPmin1), i, j), j), LineC   'And continue from there
    Next j
  Next i
  If Sgn(TBavg(IlnPmin2)) <> Sgn(TBavg(IlnPmin1)) Then
    For i = 1 To Channels
      Select Case i
      Case 1: LineC = QBColor(4)  'RED
      Case 2: LineC = QBColor(7)  'WHITE
      Case 3: LineC = QBColor(1)   'BLUE
      End Select
      picTA.PSet (TAA(aRC(IlnPmin2), i, 1), 1)
      For j = 2 To 10
        picTA.Line -(TAA(aRC(IlnPmin2), i, j), j), LineC   'And continue from there
      Next j
    Next i
  End If
End If
' Draw Y tic marks
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
' Draw X tic marks
  If Tmax - Tmin > 15 Then Tstep = 2 Else Tstep = 1
  For i = Tmin To Tmax Step Tstep
    picTA.PSet (i, 1)
    picTA.Line -(i, 1 + 0.04 * picTA.ScaleHeight), QBColor(15)
    picTA.PSet (i, 1 + picTA.ScaleHeight)
    picTA.Line -(i, 1 + 0.96 * picTA.ScaleHeight), QBColor(15)
  Next i
End Sub

Sub UpdateFLTINFO()
' PATH information
  Drive$ = DataDrive$
  Rdir$ = "\" + Trim(frmFLTINFO.cboPlatforms.Text) + "\"
  AC$ = Mid$(Rdir$, 2, 2)
  Root$ = Drive$ + Rdir$
  Mission$ = Trim(frmFLTINFO.cboMissions.Text)
  YYYYMMDD$ = Trim(frmFLTINFO.cboFltDates.Text)
  Path$ = Root$ + Mission$ + "\" + YYYYMMDD$ + "\"
  
  FltNumber = Trim(frmFLTINFO.cboFltNos.Text)
  
' CALIBRATION information
  Reg0$ = cboReg(0).Text
  Reg1$ = cboReg(1).Text
  Reg2$ = cboReg(2).Text
  USE5$ = cboUSE.Text
  SU$ = frmFLTINFO.cboSU.Text
  
  If cboAlgorithm.ListIndex = 0 Then
    Call MsgBox("You have not selected a retrieval algorithm!" + vbCrLf + "Will use MRI.", vbOKOnly)
    Algorithm = 1
    cboAlgorithm.ListIndex = 1
  Else
    Algorithm = cboAlgorithm.ListIndex
  End If
  
  Channels = Val(txtNlo.Text)
  
'  If chkMMSpALT Then UseMMSpALT = True Else UseMMSpALT = False
  
  LAT1 = Val(txtLAT(0).Text)
  LAT2 = Val(txtLAT(1).Text)
  LAT3 = Val(txtLAT(2).Text)
  LAT4 = Val(txtLAT(3).Text)
  

End Sub

Private Sub optRC_Click(Index As Integer)
UserRCindex = Index
End Sub

Private Sub tmrGetCycle_Timer()
Dim ShowAircraft As Boolean, Line$

  Call GetRAOBcycle(RAOBlu, Rcycle, False)
  'Debug.Print Rcycle; Ztrop; Ttrop
  CurrentRecord = Rcycle
  Nlev = Nlevr
  Call DetermineROABtoShow

  Line$ = "Station: WMO " + Format$(WMOnumber, "00000")
  If WMO4l <> "" Then Line$ = Line$ + "   " + WMO4l + "  N=" + Format$(Nlev, "00")
  lblSite.Caption = Line$
  Line$ = "Time: " + Format$(Iyear, "0000") + " " + Format$(Imonth, "00") + " "
  Line$ = Line$ + Format$(Iday, "00") + " " + Format$(Ihhmm, "0000") + "Z  DOY:" + Format$(Idoy, "000")
  lblTime.Caption = Line$
  Line$ = "Trop: " + Format(frmRAOB.Zt1, "00.000") + " km, " + Format$(frmRAOB.TT1, "000.00") + " K"
  lblTrop.Caption = Line$
' Don't clear if processing an entire RAOB file (so results can be seen)
  If chkProcessFile.Value = 0 Then
    If chkAutoClear.Value = 1 Then cmdREF_Click (0) 'Clear plot
  
    If Not EOF(RAOBlu) Then
      With frmRAOB
        ShowAircraft = .ShowAircraft
'        ShowDocument = True
        .ShowAircraft = False
        .LineColor = White                'Plot current RAOB
        .RAOBupdate
        .ShowAircraft = ShowAircraft
      End With
    End If
    DoEvents
  End If
    If chkAutoRetrieve Then cmdREF_Click (2)

End Sub

Sub GetRAOBcycle(lu%, Cycle&, ASCIIinput As Boolean)
Dim i%

If lu% = 0 Then
  MsgBox "OPEN a RAOB file!", vbOKOnly
End If

If EOF(lu) Then Exit Sub
If ASCIIinput Then  'Assume SJK
  ConvertSJK (lu%)  'Read one complete record.
Else  'Read Binary records
'Read new profile
GetaCycle:
  If EOF(lu%) Then Exit Sub 'XXXX
  If Forward Then Cycle = Cycle + 1 Else Cycle = Cycle - 1
'  'Debug.Print Forward; Record%
'
  If Cycle < 1 Then
    Cycle = 1
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
'
  If Cycle > FileRecords Then
    If chkProcessFile.Value = 1 Then
       chkProcessFile.Value = 0
    End If

    Cycle = FileRecords
    If tmrGetCycle.Enabled Then tmrGetCycle.Enabled = False: cmdStop.Caption = "&Continue"
  End If
'
  Call RAOBread(lu%, Cycle)
   
  'For i = 0 To 5:  txtList(i).Text = "": Next i  'Clear edit line

'  If Not GOTOcommand Then Call UpdateRAOBlisting
End If
  txtProgress.Text = Str$(Cycle)
End Sub

Private Sub txtRAOBfile_Change()
'    Open FileName$ For Random Access Read Write As RAOBlu% Len = Len(LRF)

End Sub

Private Sub UpDown1_DownClick()
Dim f As Form
'If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
Set f = frmRAOB
f.LineThickness = f.LineThickness - 1
If f.LineThickness < 1 Then f.LineThickness = 1
'txtThickness(Index).Text = Val(f.LineThickness)
txtThickness.Text = Val(f.LineThickness)
'If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate
End Sub


Private Sub UpDown1_UpClick()
Dim f As Form
'If Index = 0 Then Set f = frmRAOB Else Set f = frmATP
Set f = frmRAOB
f.LineThickness = f.LineThickness + 1
If f.LineThickness > 5 Then f.LineThickness = 5
'txtThickness(Index).Text = Val(f.LineThickness)
txtThickness.Text = Val(f.LineThickness)
'If Index = 0 Then frmRAOB.RAOBupdate Else frmATP.ATPupdate
End Sub


