VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmTools 
   Caption         =   "MTP Tools"
   ClientHeight    =   5955
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7995
   Icon            =   "frmTools.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5955
   ScaleWidth      =   7995
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   495
      Left            =   6285
      TabIndex        =   221
      Top             =   5445
      Width           =   1665
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   5385
      Left            =   30
      TabIndex        =   0
      Top             =   0
      Width           =   7920
      _ExtentX        =   13970
      _ExtentY        =   9499
      _Version        =   393216
      Tabs            =   8
      Tab             =   3
      TabsPerRow      =   8
      TabHeight       =   520
      TabCaption(0)   =   "XS File"
      TabPicture(0)   =   "frmTools.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "cmdReadXS(3)"
      Tab(0).Control(1)=   "chkUseUTwindow"
      Tab(0).Control(2)=   "chkUseZg"
      Tab(0).Control(3)=   "txtNfiles"
      Tab(0).Control(4)=   "cmdReadXS(2)"
      Tab(0).Control(5)=   "cndTimeTest"
      Tab(0).Control(6)=   "cmdTestGH"
      Tab(0).Control(7)=   "cboFileSrc"
      Tab(0).Control(8)=   "txtUTstart"
      Tab(0).Control(9)=   "txtZtMax"
      Tab(0).Control(10)=   "txtZpMin"
      Tab(0).Control(11)=   "txtZtOffset"
      Tab(0).Control(12)=   "chkZtOffset"
      Tab(0).Control(13)=   "cmdReadXS(1)"
      Tab(0).Control(14)=   "cmdReadXS(0)"
      Tab(0).Control(15)=   "Label(5)"
      Tab(0).Control(16)=   "Label(4)"
      Tab(0).Control(17)=   "Label(3)"
      Tab(0).Control(18)=   "Label(2)"
      Tab(0).Control(19)=   "Label(1)"
      Tab(0).Control(20)=   "Label(0)"
      Tab(0).ControlCount=   21
      TabCaption(1)   =   "&Extract"
      TabPicture(1)   =   "frmTools.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "chkUse_ks"
      Tab(1).Control(1)=   "cmdXSavg"
      Tab(1).Control(2)=   "cmdKojima"
      Tab(1).Control(3)=   "Command2"
      Tab(1).Control(4)=   "txtColor(23)"
      Tab(1).Control(5)=   "txtColor(22)"
      Tab(1).Control(6)=   "txtColor(21)"
      Tab(1).Control(7)=   "txtColor(20)"
      Tab(1).Control(8)=   "txtColor(19)"
      Tab(1).Control(9)=   "txtColor(18)"
      Tab(1).Control(10)=   "txtColor(17)"
      Tab(1).Control(11)=   "txtColor(16)"
      Tab(1).Control(12)=   "txtColor(15)"
      Tab(1).Control(13)=   "txtColor(14)"
      Tab(1).Control(14)=   "txtColor(13)"
      Tab(1).Control(15)=   "txtColor(12)"
      Tab(1).Control(16)=   "txtColor(11)"
      Tab(1).Control(17)=   "txtColor(10)"
      Tab(1).Control(18)=   "txtColor(9)"
      Tab(1).Control(19)=   "txtColor(8)"
      Tab(1).Control(20)=   "txtColor(7)"
      Tab(1).Control(21)=   "txtColor(6)"
      Tab(1).Control(22)=   "txtColor(5)"
      Tab(1).Control(23)=   "txtColor(4)"
      Tab(1).Control(24)=   "txtColor(3)"
      Tab(1).Control(25)=   "txtColor(2)"
      Tab(1).Control(26)=   "txtColor(1)"
      Tab(1).Control(27)=   "txtColor(0)"
      Tab(1).Control(28)=   "cmdCheckPointing"
      Tab(1).Control(29)=   "cmdExecute(8)"
      Tab(1).Control(30)=   "cmdExecute(7)"
      Tab(1).Control(31)=   "cmdExecute(6)"
      Tab(1).Control(32)=   "cmdExecute(5)"
      Tab(1).Control(33)=   "cmdExecute(4)"
      Tab(1).Control(34)=   "cmdExecute(3)"
      Tab(1).Control(35)=   "cmdExecute(2)"
      Tab(1).Control(36)=   "cmdExecute(1)"
      Tab(1).Control(37)=   "cmdExecute(0)"
      Tab(1).Control(38)=   "txtTotalRecords"
      Tab(1).Control(39)=   "txtLocHor"
      Tab(1).Control(40)=   "cmdOpenSource"
      Tab(1).Control(41)=   "txtRAWfile"
      Tab(1).Control(42)=   "chkUseThisSource"
      Tab(1).Control(43)=   "txtRecord"
      Tab(1).Control(44)=   "Frame(6)"
      Tab(1).Control(45)=   "chkSkipBadScan"
      Tab(1).Control(46)=   "cmdWriteParms(0)"
      Tab(1).Control(47)=   "cmdWriteParms(1)"
      Tab(1).Control(48)=   "Label(28)"
      Tab(1).Control(49)=   "Label(27)"
      Tab(1).Control(50)=   "Label(22)"
      Tab(1).ControlCount=   51
      TabCaption(2)   =   "Run Pgm"
      TabPicture(2)   =   "frmTools.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "txtRevFP"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "txtRevMM"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "chkReadICTfile"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "chkMMS"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "cmdBatchProcessor(20)"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).Control(5)=   "cmdBatchProcessor(19)"
      Tab(2).Control(5).Enabled=   0   'False
      Tab(2).Control(6)=   "cmdBatchProcessor(18)"
      Tab(2).Control(6).Enabled=   0   'False
      Tab(2).Control(7)=   "cmdBatchProcessor(17)"
      Tab(2).Control(7).Enabled=   0   'False
      Tab(2).Control(8)=   "cmdBatchProcessor(16)"
      Tab(2).Control(8).Enabled=   0   'False
      Tab(2).Control(9)=   "cmdBatchProcessor(15)"
      Tab(2).Control(9).Enabled=   0   'False
      Tab(2).Control(10)=   "cmdBatchProcessor(14)"
      Tab(2).Control(10).Enabled=   0   'False
      Tab(2).Control(11)=   "cmdBatchProcessor(13)"
      Tab(2).Control(11).Enabled=   0   'False
      Tab(2).Control(12)=   "cmdBatchProcessor(11)"
      Tab(2).Control(12).Enabled=   0   'False
      Tab(2).Control(13)=   "cmdBatchProcessor(4)"
      Tab(2).Control(13).Enabled=   0   'False
      Tab(2).Control(14)=   "cmdBatchProcessor(3)"
      Tab(2).Control(14).Enabled=   0   'False
      Tab(2).Control(15)=   "cmdBatchProcessor(8)"
      Tab(2).Control(15).Enabled=   0   'False
      Tab(2).Control(16)=   "cmdBatchProcessor(7)"
      Tab(2).Control(16).Enabled=   0   'False
      Tab(2).Control(17)=   "cmdBatchProcessor(6)"
      Tab(2).Control(17).Enabled=   0   'False
      Tab(2).Control(18)=   "cmdBatchProcessor(5)"
      Tab(2).Control(18).Enabled=   0   'False
      Tab(2).Control(19)=   "cmdBatchProcessor(0)"
      Tab(2).Control(19).Enabled=   0   'False
      Tab(2).Control(20)=   "Frame(4)"
      Tab(2).Control(20).Enabled=   0   'False
      Tab(2).Control(21)=   "Frame(3)"
      Tab(2).Control(21).Enabled=   0   'False
      Tab(2).Control(22)=   "Frame(5)"
      Tab(2).Control(22).Enabled=   0   'False
      Tab(2).ControlCount=   23
      TabCaption(3)   =   "&RAW"
      TabPicture(3)   =   "frmTools.frx":0496
      Tab(3).ControlEnabled=   -1  'True
      Tab(3).Control(0)=   "lblProgress"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "Frame(11)"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "ProgressBar1"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "Command1"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).Control(4)=   "Command3"
      Tab(3).Control(4).Enabled=   0   'False
      Tab(3).Control(5)=   "cmdReadMS"
      Tab(3).Control(5).Enabled=   0   'False
      Tab(3).Control(6)=   "cmdUCSE"
      Tab(3).Control(6).Enabled=   0   'False
      Tab(3).Control(7)=   "cmdUCSE_TDC"
      Tab(3).Control(7).Enabled=   0   'False
      Tab(3).Control(8)=   "cmdReadMP"
      Tab(3).Control(8).Enabled=   0   'False
      Tab(3).ControlCount=   9
      TabCaption(4)   =   "&Batch"
      TabPicture(4)   =   "frmTools.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "txtCurrentProcess"
      Tab(4).Control(0).Enabled=   0   'False
      Tab(4).Control(1)=   "chkProcesses(0)"
      Tab(4).Control(1).Enabled=   0   'False
      Tab(4).Control(2)=   "Frame(12)"
      Tab(4).Control(2).Enabled=   0   'False
      Tab(4).Control(3)=   "Frame5"
      Tab(4).Control(3).Enabled=   0   'False
      Tab(4).Control(4)=   "cmdBatchProcessor(12)"
      Tab(4).Control(4).Enabled=   0   'False
      Tab(4).Control(5)=   "tmrBatch1"
      Tab(4).Control(5).Enabled=   0   'False
      Tab(4).Control(6)=   "tmrBatch0"
      Tab(4).Control(6).Enabled=   0   'False
      Tab(4).Control(7)=   "Label(31)"
      Tab(4).Control(7).Enabled=   0   'False
      Tab(4).ControlCount=   8
      TabCaption(5)   =   "Pt Fit"
      TabPicture(5)   =   "frmTools.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "Frame(8)"
      Tab(5).Control(0).Enabled=   0   'False
      Tab(5).Control(1)=   "Frame(7)"
      Tab(5).Control(1).Enabled=   0   'False
      Tab(5).Control(2)=   "CommonDialog1"
      Tab(5).Control(2).Enabled=   0   'False
      Tab(5).Control(3)=   "txtTest"
      Tab(5).Control(3).Enabled=   0   'False
      Tab(5).Control(4)=   "Frame(2)"
      Tab(5).Control(4).Enabled=   0   'False
      Tab(5).Control(5)=   "Frame(1)"
      Tab(5).Control(5).Enabled=   0   'False
      Tab(5).Control(6)=   "Frame(0)"
      Tab(5).Control(6).Enabled=   0   'False
      Tab(5).ControlCount=   7
      TabCaption(6)   =   "Text"
      TabPicture(6)   =   "frmTools.frx":04EA
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "txtTextBox"
      Tab(6).Control(1)=   "Frame1(0)"
      Tab(6).Control(2)=   "cmdText"
      Tab(6).Control(3)=   "lstTextBox"
      Tab(6).Control(4)=   "Label(20)"
      Tab(6).Control(5)=   "Label(21)"
      Tab(6).ControlCount=   6
      TabCaption(7)   =   "RAF"
      TabPicture(7)   =   "frmTools.frx":0506
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "cmdGenerate(4)"
      Tab(7).Control(1)=   "cmdGenerate(3)"
      Tab(7).Control(2)=   "cmdGenerate(2)"
      Tab(7).Control(3)=   "cmdGenerate(1)"
      Tab(7).Control(4)=   "txtFit"
      Tab(7).Control(5)=   "cmdGenerate(0)"
      Tab(7).Control(6)=   "Label(33)"
      Tab(7).ControlCount=   7
      Begin VB.CommandButton cmdReadMP 
         Caption         =   "Read MP"
         Height          =   465
         Left            =   3915
         TabIndex        =   254
         Top             =   2070
         Width           =   1140
      End
      Begin VB.CommandButton cmdUCSE_TDC 
         Caption         =   "  Compare UCSE, TDC"
         Height          =   525
         Left            =   3915
         TabIndex        =   253
         Top             =   1485
         Width           =   1095
      End
      Begin VB.CommandButton cmdUCSE 
         Caption         =   " Write UCSE        P + R"
         Height          =   525
         Left            =   3915
         TabIndex        =   252
         Top             =   945
         Width           =   1095
      End
      Begin VB.CommandButton cmdReadMS 
         Caption         =   "Read MS"
         Height          =   390
         Left            =   3915
         TabIndex        =   251
         Top             =   495
         Width           =   1065
      End
      Begin VB.TextBox txtRevFP 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71670
         MultiLine       =   -1  'True
         TabIndex        =   249
         Text            =   "frmTools.frx":0522
         Top             =   4635
         Width           =   285
      End
      Begin VB.TextBox txtRevMM 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71985
         MultiLine       =   -1  'True
         TabIndex        =   248
         Text            =   "frmTools.frx":0524
         Top             =   4635
         Width           =   285
      End
      Begin VB.CheckBox chkReadICTfile 
         Caption         =   "Use ICT Rev"
         Height          =   195
         Left            =   -73245
         TabIndex        =   247
         Top             =   4680
         Width           =   1230
      End
      Begin VB.CheckBox chkMMS 
         Caption         =   "MMS Mach No"
         Height          =   255
         Left            =   -73230
         TabIndex        =   241
         Top             =   4425
         Width           =   1545
      End
      Begin VB.CommandButton Command3 
         Caption         =   "RAOBrangeAll Sample"
         Height          =   495
         Left            =   6150
         TabIndex        =   240
         Top             =   960
         Width           =   1245
      End
      Begin VB.CommandButton Command1 
         Caption         =   "WCT Sample"
         Height          =   345
         Left            =   6150
         TabIndex        =   239
         Top             =   570
         Width           =   1245
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Spare"
         Height          =   405
         Index           =   20
         Left            =   -73290
         TabIndex        =   238
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   1965
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Spare"
         Height          =   375
         Index           =   19
         Left            =   -73290
         TabIndex        =   237
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   1575
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Spare"
         Height          =   375
         Index           =   18
         Left            =   -73290
         TabIndex        =   236
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   1185
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "RIF => BND"
         Height          =   375
         Index           =   17
         Left            =   -73290
         TabIndex        =   235
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   795
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "NG => RNF"
         Height          =   375
         Index           =   16
         Left            =   -73290
         TabIndex        =   234
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   405
         Width           =   1575
      End
      Begin VB.CommandButton cmdGenerate 
         Caption         =   "Spare"
         Height          =   405
         Index           =   4
         Left            =   -68940
         TabIndex        =   233
         Top             =   4485
         Width           =   1455
      End
      Begin VB.CommandButton cmdGenerate 
         Caption         =   "IF Bandpass"
         Height          =   405
         Index           =   3
         Left            =   -70440
         TabIndex        =   232
         Top             =   4485
         Width           =   1455
      End
      Begin VB.CommandButton cmdGenerate 
         Caption         =   "A Priori Errors"
         Height          =   405
         Index           =   2
         Left            =   -71940
         TabIndex        =   231
         Top             =   4485
         Width           =   1455
      End
      Begin VB.CommandButton cmdGenerate 
         Caption         =   "WCT"
         Height          =   405
         Index           =   1
         Left            =   -73410
         TabIndex        =   230
         Top             =   4485
         Width           =   1455
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "IWG1 => RIF"
         Height          =   375
         Index           =   15
         Left            =   -74865
         TabIndex        =   228
         ToolTipText     =   "Convert IWG1 file from ASCII to BIN"
         Top             =   4455
         Width           =   1575
      End
      Begin VB.TextBox txtFit 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3840
         Left            =   -74910
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   227
         Top             =   570
         Width           =   7425
      End
      Begin VB.CommandButton cmdGenerate 
         Caption         =   "RAF Coefficients"
         Height          =   405
         Index           =   0
         Left            =   -74910
         TabIndex        =   226
         Top             =   4485
         Width           =   1455
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Mission Summary"
         Height          =   405
         Index           =   14
         Left            =   -74880
         TabIndex        =   225
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   4020
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Disable WCT"
         Height          =   405
         Index           =   13
         Left            =   -74880
         TabIndex        =   223
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   3600
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Load/Enable WCT"
         Height          =   405
         Index           =   11
         Left            =   -74880
         TabIndex        =   222
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   3180
         Width           =   1575
      End
      Begin VB.TextBox txtCurrentProcess 
         Height          =   285
         Left            =   -72420
         TabIndex        =   220
         Top             =   2460
         Width           =   2025
      End
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Open Raw"
         Height          =   255
         Index           =   0
         Left            =   -72420
         TabIndex        =   219
         Top             =   2790
         Visible         =   0   'False
         Width           =   1335
      End
      Begin VB.Frame Frame 
         Caption         =   "Change Retrieval Parameters"
         Height          =   4470
         Index           =   12
         Left            =   -74880
         TabIndex        =   212
         Top             =   420
         Width           =   2385
         Begin VB.CheckBox chkChangeParameter 
            Caption         =   "OATnavCOR"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   218
            Top             =   300
            Width           =   1335
         End
         Begin VB.TextBox txtNewParameter 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   217
            Text            =   "frmTools.frx":0526
            Top             =   270
            Width           =   615
         End
         Begin VB.CheckBox chkChangeParameter 
            Caption         =   "Read New WCT"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   216
            Top             =   630
            Width           =   1515
         End
         Begin VB.TextBox txtNewParameter 
            Height          =   285
            Index           =   1
            Left            =   1680
            TabIndex        =   215
            Text            =   "TRUE"
            Top             =   600
            Width           =   615
         End
         Begin VB.CheckBox chkChangeParameter 
            Caption         =   "Enable WCT"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   214
            Top             =   960
            Width           =   1485
         End
         Begin VB.TextBox txtNewParameter 
            Height          =   285
            Index           =   2
            Left            =   1680
            TabIndex        =   213
            Text            =   "TRUE"
            Top             =   930
            Width           =   615
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "Process Selection"
         Height          =   1875
         Left            =   -72390
         TabIndex        =   207
         Top             =   420
         Width           =   1995
         Begin VB.CheckBox chkProcesses 
            Caption         =   "Retrieval"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   211
            Top             =   240
            Width           =   1335
         End
         Begin VB.CheckBox chkProcesses 
            Caption         =   "Editting"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   210
            Top             =   480
            Width           =   1335
         End
         Begin VB.CheckBox chkProcesses 
            Caption         =   "Plot CTC"
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   209
            Top             =   960
            Width           =   1335
         End
         Begin VB.CheckBox chkProcesses 
            Caption         =   "Export MP-File"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   208
            Top             =   720
            Width           =   1455
         End
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Start Batch Process"
         Height          =   465
         Index           =   12
         Left            =   -72420
         TabIndex        =   206
         Top             =   4425
         Width           =   1995
      End
      Begin VB.Timer tmrBatch1 
         Left            =   -69870
         Top             =   510
      End
      Begin VB.Timer tmrBatch0 
         Left            =   -70350
         Top             =   510
      End
      Begin VB.TextBox txtTextBox 
         Height          =   2775
         Left            =   -74910
         MultiLine       =   -1  'True
         TabIndex        =   203
         Top             =   1200
         Width           =   2805
      End
      Begin VB.Frame Frame1 
         Caption         =   "Output File Type"
         Height          =   495
         Index           =   0
         Left            =   -74910
         TabIndex        =   196
         Top             =   390
         Width           =   4125
         Begin VB.OptionButton optTextBox 
            Caption         =   "FLA"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   202
            Top             =   240
            Width           =   615
         End
         Begin VB.OptionButton optTextBox 
            Caption         =   "RMS"
            Height          =   195
            Index           =   1
            Left            =   780
            TabIndex        =   201
            Top             =   240
            Width           =   675
         End
         Begin VB.OptionButton optTextBox 
            Caption         =   "WCT"
            Height          =   195
            Index           =   2
            Left            =   1470
            TabIndex        =   200
            Top             =   240
            Width           =   675
         End
         Begin VB.OptionButton optTextBox 
            Caption         =   "NBR"
            Height          =   195
            Index           =   3
            Left            =   2160
            TabIndex        =   199
            Top             =   240
            Width           =   645
         End
         Begin VB.OptionButton optTextBox 
            Caption         =   "XXX"
            Height          =   195
            Index           =   4
            Left            =   2820
            TabIndex        =   198
            Top             =   240
            Width           =   615
         End
         Begin VB.OptionButton optTextBox 
            Caption         =   "XXX"
            Height          =   195
            Index           =   5
            Left            =   3450
            TabIndex        =   197
            Top             =   240
            Width           =   615
         End
      End
      Begin VB.CommandButton cmdText 
         Caption         =   "&Save"
         Height          =   495
         Left            =   -70680
         TabIndex        =   195
         Top             =   390
         Width           =   765
      End
      Begin VB.ListBox lstTextBox 
         Height          =   2790
         ItemData        =   "frmTools.frx":052A
         Left            =   -71940
         List            =   "frmTools.frx":052C
         TabIndex        =   194
         Top             =   1200
         Width           =   2715
      End
      Begin VB.CheckBox chkUse_ks 
         Caption         =   "Use ks"
         Height          =   285
         Left            =   -70470
         TabIndex        =   193
         Top             =   3510
         Width           =   1095
      End
      Begin VB.CommandButton cmdXSavg 
         Caption         =   "Avg"
         Height          =   465
         Left            =   -69300
         TabIndex        =   192
         Top             =   3810
         Width           =   1065
      End
      Begin VB.CommandButton cmdKojima 
         Caption         =   "Kojima List"
         Height          =   465
         Left            =   -70470
         TabIndex        =   191
         Top             =   3810
         Width           =   1065
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Color Selection"
         Height          =   375
         Left            =   -72090
         TabIndex        =   190
         Top             =   3390
         Width           =   1455
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   23
         Left            =   -68760
         TabIndex        =   189
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   22
         Left            =   -68760
         TabIndex        =   188
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   21
         Left            =   -68760
         TabIndex        =   187
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   20
         Left            =   -69240
         TabIndex        =   186
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   19
         Left            =   -69240
         TabIndex        =   185
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   18
         Left            =   -69240
         TabIndex        =   184
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   17
         Left            =   -69720
         TabIndex        =   183
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   16
         Left            =   -69720
         TabIndex        =   182
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   15
         Left            =   -69720
         TabIndex        =   181
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   14
         Left            =   -70200
         TabIndex        =   180
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   13
         Left            =   -70200
         TabIndex        =   179
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   12
         Left            =   -70200
         TabIndex        =   178
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   11
         Left            =   -70680
         TabIndex        =   177
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   10
         Left            =   -70680
         TabIndex        =   176
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   9
         Left            =   -70680
         TabIndex        =   175
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   8
         Left            =   -71160
         TabIndex        =   174
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   7
         Left            =   -71160
         TabIndex        =   173
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   6
         Left            =   -71160
         TabIndex        =   172
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   5
         Left            =   -71640
         TabIndex        =   171
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   4
         Left            =   -71640
         TabIndex        =   170
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   3
         Left            =   -71640
         TabIndex        =   169
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   2
         Left            =   -72120
         TabIndex        =   168
         Top             =   2940
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   1
         Left            =   -72120
         TabIndex        =   167
         Top             =   2580
         Width           =   495
      End
      Begin VB.TextBox txtColor 
         Alignment       =   2  'Center
         Height          =   405
         Index           =   0
         Left            =   -72120
         TabIndex        =   166
         Top             =   2220
         Width           =   495
      End
      Begin VB.CommandButton cmdCheckPointing 
         Caption         =   "Check Pointing"
         Height          =   435
         Left            =   -72090
         TabIndex        =   165
         Top             =   3840
         Width           =   1455
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Radiometer Noise"
         Height          =   375
         Index           =   4
         Left            =   -74880
         TabIndex        =   164
         ToolTipText     =   "Calculate RadiometerNoise.txt file from RMSyyyymmdd.txt files"
         Top             =   2400
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Print ND Temps"
         Height          =   375
         Index           =   3
         Left            =   -74880
         TabIndex        =   163
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   2790
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Create RAF Files"
         Height          =   375
         Index           =   8
         Left            =   -74880
         TabIndex        =   160
         ToolTipText     =   "Put flight track lat and lon in LLYYYYMMDD.txt"
         Top             =   1590
         Width           =   1575
      End
      Begin ComctlLib.ProgressBar ProgressBar1 
         Height          =   255
         Left            =   780
         TabIndex        =   159
         Top             =   4635
         Width           =   6705
         _ExtentX        =   11827
         _ExtentY        =   450
         _Version        =   327682
         Appearance      =   1
      End
      Begin VB.Frame Frame 
         Caption         =   "RAW File Options"
         Height          =   3585
         Index           =   11
         Left            =   90
         TabIndex        =   140
         Top             =   390
         Width           =   3615
         Begin VB.CheckBox chkUseRIF 
            Caption         =   "Use RIF file for P,T,M"
            Height          =   225
            Left            =   1560
            TabIndex        =   242
            Top             =   1380
            Value           =   1  'Checked
            Width           =   1905
         End
         Begin VB.CheckBox chkSubstituteAll 
            Caption         =   "Substitute in All Files"
            Height          =   255
            Left            =   1560
            TabIndex        =   157
            Top             =   1110
            Width           =   1845
         End
         Begin VB.ComboBox cboPTfile 
            Height          =   315
            Left            =   720
            TabIndex        =   152
            Text            =   "NO"
            Top             =   240
            Width           =   615
         End
         Begin VB.CheckBox chkDisablePcorr 
            Caption         =   "Disable P Corrections"
            Height          =   255
            Left            =   120
            TabIndex        =   151
            ToolTipText     =   "Normally disabled. Enable to do CAMEX-4 ER-2 pressure corrections."
            Top             =   2520
            Width           =   1815
         End
         Begin VB.CommandButton cmdEditPTfile 
            Caption         =   "Edit Source File"
            Height          =   375
            Left            =   1530
            TabIndex        =   150
            ToolTipText     =   "Edit Source File to remove Ts which differ from last by some threshold (in code)"
            Top             =   180
            Width           =   1695
         End
         Begin VB.CheckBox chkRemoveBadTemps 
            Caption         =   "Remove Bad Temps"
            Height          =   255
            Left            =   1560
            TabIndex        =   149
            ToolTipText     =   "If Source T is bad, remove it from file"
            Top             =   600
            Width           =   1815
         End
         Begin VB.CheckBox chkRenameFile 
            Caption         =   "Rename New File"
            Height          =   255
            Left            =   1560
            TabIndex        =   148
            Top             =   840
            Value           =   1  'Checked
            Width           =   1815
         End
         Begin VB.CommandButton cmdSubstitute 
            Caption         =   "Substitute P and T Data into "
            Height          =   375
            Left            =   90
            TabIndex        =   147
            Top             =   1785
            Width           =   2415
         End
         Begin VB.CommandButton cmdUTnavCor 
            Caption         =   "Correct Nav Clock By"
            Height          =   375
            Left            =   120
            TabIndex        =   146
            Top             =   3150
            Width           =   1815
         End
         Begin VB.TextBox txtUTnavCor 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2040
            MultiLine       =   -1  'True
            TabIndex        =   145
            Text            =   "frmTools.frx":052E
            Top             =   3150
            Width           =   615
         End
         Begin VB.CheckBox chkWritePTtxt 
            Alignment       =   1  'Right Justify
            Caption         =   "Write PT txt"
            Height          =   255
            Left            =   120
            TabIndex        =   144
            ToolTipText     =   "Write Ps, Ts to text file"
            Top             =   1380
            Width           =   1215
         End
         Begin VB.TextBox txtRAWsource 
            Height          =   285
            Left            =   2520
            TabIndex        =   143
            Text            =   "RAW"
            ToolTipText     =   "Extension of File into which P, T should be substituted"
            Top             =   1815
            Width           =   495
         End
         Begin VB.CheckBox chkShowChannels 
            Caption         =   "Lab Cal"
            Height          =   195
            Index           =   11
            Left            =   120
            TabIndex        =   142
            Top             =   2790
            Width           =   855
         End
         Begin VB.TextBox txtTargetT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   141
            Text            =   "frmTools.frx":0530
            Top             =   2730
            Width           =   345
         End
         Begin VB.Label Label1 
            Caption         =   "File"
            Height          =   225
            Left            =   3090
            TabIndex        =   158
            Top             =   1875
            Width           =   405
         End
         Begin VB.Label Label 
            Caption         =   "Source"
            Height          =   255
            Index           =   30
            Left            =   120
            TabIndex        =   155
            ToolTipText     =   "Source of alternate P and T: NO=default=Nav, MM=MMS, PT=PTW"
            Top             =   240
            Width           =   615
         End
         Begin VB.Line Line1 
            BorderColor     =   &H00000000&
            X1              =   120
            X2              =   3480
            Y1              =   1695
            Y2              =   1695
         End
         Begin VB.Label Label 
            Caption         =   "Seconds"
            Height          =   165
            Index           =   40
            Left            =   2760
            TabIndex        =   154
            Top             =   3240
            Width           =   735
         End
         Begin VB.Label Label2 
            Caption         =   "C"
            Height          =   195
            Left            =   1500
            TabIndex        =   153
            Top             =   2790
            Width           =   255
         End
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Lat and Lon"
         Height          =   375
         Index           =   7
         Left            =   -74880
         TabIndex        =   139
         ToolTipText     =   "Put flight track lat and lon in LLYYYYMMDD.txt"
         Top             =   1200
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Strat or Trop"
         Height          =   375
         Index           =   6
         Left            =   -74880
         TabIndex        =   138
         ToolTipText     =   "Put strat or trop flag in STYYYYMMDD.txt"
         Top             =   810
         Width           =   1575
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Gain Parameters"
         Height          =   375
         Index           =   5
         Left            =   -74880
         TabIndex        =   137
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   420
         Width           =   1575
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "Note Tab"
         Height          =   375
         Index           =   8
         Left            =   -73710
         TabIndex        =   136
         Top             =   2460
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "Explorer"
         Height          =   375
         Index           =   7
         Left            =   -73710
         TabIndex        =   135
         Top             =   1980
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "Excel"
         Height          =   375
         Index           =   6
         Left            =   -73710
         TabIndex        =   134
         Top             =   1500
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "Calculator"
         Height          =   375
         Index           =   5
         Left            =   -74880
         TabIndex        =   133
         Top             =   3405
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "MTPcalc"
         Height          =   375
         Index           =   4
         Left            =   -73710
         TabIndex        =   132
         Top             =   2940
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "CRLF"
         Height          =   375
         Index           =   3
         Left            =   -74880
         TabIndex        =   131
         Top             =   2940
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "RAOBget"
         Height          =   375
         Index           =   2
         Left            =   -74880
         TabIndex        =   130
         Top             =   2460
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "RAOBman"
         Height          =   375
         Index           =   1
         Left            =   -74880
         TabIndex        =   129
         Top             =   1980
         Width           =   1125
      End
      Begin VB.CommandButton cmdExecute 
         Caption         =   "MAPget"
         Height          =   375
         Index           =   0
         Left            =   -74850
         TabIndex        =   128
         Top             =   1500
         Width           =   1125
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Avg/Rms Trop CP"
         Height          =   405
         Index           =   0
         Left            =   -74880
         TabIndex        =   125
         ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
         Top             =   1980
         Width           =   1575
      End
      Begin VB.TextBox txtTotalRecords 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68865
         MultiLine       =   -1  'True
         TabIndex        =   121
         Top             =   1830
         Width           =   555
      End
      Begin VB.TextBox txtLocHor 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68865
         MultiLine       =   -1  'True
         TabIndex        =   119
         Text            =   "frmTools.frx":0533
         Top             =   1500
         Width           =   555
      End
      Begin VB.CommandButton cmdOpenSource 
         Caption         =   "Open Source"
         Height          =   390
         Left            =   -74925
         TabIndex        =   118
         Top             =   360
         Width           =   1215
      End
      Begin VB.TextBox txtRAWfile 
         Height          =   360
         Left            =   -73680
         TabIndex        =   117
         Top             =   375
         Width           =   5475
      End
      Begin VB.CheckBox chkUseThisSource 
         Caption         =   "Use This Source"
         Height          =   300
         Left            =   -74850
         TabIndex        =   116
         Top             =   4020
         Width           =   1500
      End
      Begin VB.Frame Frame 
         Caption         =   "Fit"
         Height          =   1335
         Index           =   8
         Left            =   -74940
         TabIndex        =   90
         Top             =   3060
         Width           =   3675
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Write"
            Height          =   375
            Index           =   7
            Left            =   2760
            TabIndex        =   100
            Top             =   330
            Width           =   825
         End
         Begin VB.TextBox txtCtoT 
            Height          =   315
            Index           =   3
            Left            =   2670
            TabIndex        =   99
            Top             =   930
            Width           =   945
         End
         Begin VB.TextBox txtC0 
            Height          =   315
            Left            =   60
            TabIndex        =   97
            Top             =   390
            Width           =   855
         End
         Begin VB.TextBox txtC1 
            Height          =   315
            Left            =   960
            TabIndex        =   96
            Top             =   390
            Width           =   825
         End
         Begin VB.CommandButton cmdPtFit 
            Caption         =   "Do Fit"
            Height          =   375
            Left            =   1920
            TabIndex        =   95
            Top             =   330
            Width           =   765
         End
         Begin VB.TextBox txtCtoT 
            Height          =   315
            Index           =   0
            Left            =   60
            TabIndex        =   93
            Top             =   930
            Width           =   855
         End
         Begin VB.TextBox txtCtoT 
            Height          =   315
            Index           =   1
            Left            =   960
            TabIndex        =   92
            Top             =   930
            Width           =   825
         End
         Begin VB.TextBox txtCtoT 
            Height          =   315
            Index           =   2
            Left            =   1800
            TabIndex        =   91
            Top             =   930
            Width           =   825
         End
         Begin VB.Label Label 
            Caption         =   "R(Counts) coeffs"
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   98
            Top             =   180
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "T(Counts) coeffs c(0), c(1), c(2), c(3)"
            Height          =   225
            Index           =   14
            Left            =   90
            TabIndex        =   94
            Top             =   720
            Width           =   2655
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Calculate T Given R"
         Height          =   615
         Index           =   7
         Left            =   -70500
         TabIndex        =   86
         Top             =   3750
         Width           =   2295
         Begin VB.CommandButton cmdTr 
            Caption         =   "T(R)?"
            Height          =   375
            Left            =   1680
            TabIndex        =   89
            Top             =   150
            Width           =   555
         End
         Begin VB.TextBox txtT 
            Height          =   315
            Left            =   930
            TabIndex        =   88
            Top             =   240
            Width           =   675
         End
         Begin VB.TextBox txtR 
            Height          =   315
            Left            =   90
            TabIndex        =   87
            Top             =   240
            Width           =   765
         End
      End
      Begin VB.TextBox txtRecord 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68865
         MultiLine       =   -1  'True
         TabIndex        =   83
         Top             =   1155
         Width           =   555
      End
      Begin VB.Frame Frame 
         Caption         =   "Source Type"
         Height          =   585
         Index           =   6
         Left            =   -74925
         TabIndex        =   79
         Top             =   810
         Width           =   3045
         Begin VB.ComboBox cboRAW 
            Height          =   315
            Left            =   330
            TabIndex        =   85
            Text            =   "RAW"
            Top             =   210
            Width           =   765
         End
         Begin VB.OptionButton optSource 
            Caption         =   "MP Archive"
            Height          =   195
            Index           =   2
            Left            =   1860
            TabIndex        =   82
            Top             =   270
            Width           =   1125
         End
         Begin VB.OptionButton optSource 
            Caption         =   "REF"
            Height          =   195
            Index           =   1
            Left            =   1170
            TabIndex        =   81
            Top             =   270
            Value           =   -1  'True
            Width           =   675
         End
         Begin VB.OptionButton optSource 
            Height          =   225
            Index           =   0
            Left            =   90
            TabIndex        =   80
            Top             =   255
            Width           =   285
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Level Flight Legs"
         Height          =   3075
         Index           =   4
         Left            =   -71325
         TabIndex        =   67
         Top             =   360
         Width           =   1845
         Begin VB.TextBox txtZpMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   102
            Text            =   "frmTools.frx":0537
            Top             =   210
            Width           =   435
         End
         Begin VB.TextBox txtZpBin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   77
            Text            =   "frmTools.frx":053C
            ToolTipText     =   "Histogram Zp bin size"
            Top             =   525
            Width           =   435
         End
         Begin VB.TextBox txtPitchMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   76
            Text            =   "frmTools.frx":0542
            Top             =   1380
            Width           =   435
         End
         Begin VB.TextBox txtdUTmin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   74
            Text            =   "frmTools.frx":0548
            Top             =   1680
            Width           =   435
         End
         Begin VB.TextBox txtdPitchMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   73
            Text            =   "frmTools.frx":054E
            Top             =   1080
            Width           =   435
         End
         Begin VB.TextBox txtdZpMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   72
            Text            =   "frmTools.frx":0552
            Top             =   810
            Width           =   435
         End
         Begin VB.CommandButton cmdBatchProcessor 
            Caption         =   "Get"
            Height          =   375
            Index           =   2
            Left            =   945
            TabIndex        =   68
            ToolTipText     =   "Put level flight legs in Mission_LevelFlight.txt"
            Top             =   2625
            Width           =   825
         End
         Begin VB.Label Label 
            Caption         =   "ZpMax (km)"
            Height          =   225
            Index           =   23
            Left            =   90
            TabIndex        =   101
            Top             =   270
            Width           =   1005
         End
         Begin VB.Label Label 
            Caption         =   "Zp Bin (m)"
            Height          =   225
            Index           =   19
            Left            =   90
            TabIndex        =   78
            Top             =   540
            Width           =   735
         End
         Begin VB.Label Label 
            Caption         =   "ABS(Pitch) Max"
            Height          =   225
            Index           =   18
            Left            =   60
            TabIndex        =   75
            Top             =   1410
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "dUTmin (sec)"
            Height          =   225
            Index           =   17
            Left            =   60
            TabIndex        =   71
            Top             =   1680
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "dPitchMax (deg)"
            Height          =   225
            Index           =   16
            Left            =   60
            TabIndex        =   70
            Top             =   1140
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "dZpMax (m)"
            Height          =   225
            Index           =   9
            Left            =   60
            TabIndex        =   69
            Top             =   840
            Width           =   945
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB Range"
         Height          =   1860
         Index           =   3
         Left            =   -71325
         TabIndex        =   63
         Top             =   3435
         Width           =   4170
         Begin VB.TextBox txtTimeStep 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1710
            MultiLine       =   -1  'True
            TabIndex        =   246
            Text            =   "frmTools.frx":0558
            Top             =   1035
            Width           =   465
         End
         Begin VB.TextBox txtZpStep 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2565
            MultiLine       =   -1  'True
            TabIndex        =   244
            Text            =   "frmTools.frx":055C
            Top             =   540
            Width           =   375
         End
         Begin VB.CommandButton cmdBatchProcessor 
            Caption         =   "RAOBrangeUnique"
            Height          =   405
            Index           =   10
            Left            =   135
            TabIndex        =   162
            ToolTipText     =   "Put unique soundings in Mission_RAOBrange.RSL file within this range into /RAOB/Mission_RAOBrangeUnique.txt"
            Top             =   1395
            Width           =   1545
         End
         Begin VB.CommandButton cmdBatchProcessor 
            Caption         =   "RAOBrangeNR"
            Height          =   405
            Index           =   9
            Left            =   120
            TabIndex        =   161
            ToolTipText     =   $"frmTools.frx":0560
            Top             =   945
            Width           =   1560
         End
         Begin VB.CheckBox chkZpLevels 
            Caption         =   "   Get Zp Levels"
            Height          =   195
            Left            =   2595
            TabIndex        =   127
            Top             =   255
            Width           =   1515
         End
         Begin VB.CheckBox chkCOSMIC 
            Caption         =   "COSMIC Sounding"
            Height          =   285
            Left            =   2475
            TabIndex        =   126
            Top             =   1530
            Width           =   1650
         End
         Begin VB.TextBox txtALTkmMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1725
            MultiLine       =   -1  'True
            TabIndex        =   104
            Text            =   "frmTools.frx":05F0
            Top             =   570
            Width           =   450
         End
         Begin VB.CommandButton cmdBatchProcessor 
            Caption         =   "Get RAOBs within"
            Height          =   405
            Index           =   1
            Left            =   120
            TabIndex        =   65
            ToolTipText     =   "Put soundings in Mission_RAOBrange.RSL file within this range into /RAOB/Mission_RAOBrangeAll.txt"
            Top             =   210
            Width           =   1560
         End
         Begin VB.TextBox txtRmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1725
            MultiLine       =   -1  'True
            TabIndex        =   64
            Text            =   "frmTools.frx":05F6
            Top             =   240
            Width           =   450
         End
         Begin VB.Label Label 
            Caption         =   "Minimum Time Step (sec)"
            Height          =   225
            Index           =   34
            Left            =   2205
            TabIndex        =   245
            Top             =   1080
            Width           =   1815
         End
         Begin VB.Label Label 
            Caption         =   "Zp Step (km)"
            Height          =   225
            Index           =   32
            Left            =   3015
            TabIndex        =   243
            Top             =   630
            Width           =   960
         End
         Begin VB.Label Label 
            Caption         =   "km"
            Height          =   225
            Index           =   25
            Left            =   2220
            TabIndex        =   105
            Top             =   600
            Width           =   225
         End
         Begin VB.Label Label 
            Caption         =   "Min Pressure Altitude"
            Height          =   165
            Index           =   24
            Left            =   135
            TabIndex        =   103
            Top             =   660
            Width           =   1530
         End
         Begin VB.Label Label 
            Caption         =   "km"
            Height          =   225
            Index           =   124
            Left            =   2220
            TabIndex        =   66
            Top             =   270
            Width           =   225
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Flight Dates to Process"
         Height          =   3090
         Index           =   5
         Left            =   -69435
         TabIndex        =   54
         Top             =   360
         Width           =   2265
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Select"
            Height          =   315
            Index           =   6
            Left            =   1305
            TabIndex        =   250
            ToolTipText     =   "Remove Selected Flighht Date from List"
            Top             =   900
            Width           =   915
         End
         Begin VB.TextBox txtFltCount 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1875
            MultiLine       =   -1  'True
            TabIndex        =   123
            Top             =   2730
            Width           =   330
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Save"
            Height          =   315
            Index           =   5
            Left            =   1290
            TabIndex        =   62
            ToolTipText     =   "Save Flight Date List to MISSION_NUM_batch.txt"
            Top             =   2355
            Width           =   915
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Default"
            Height          =   315
            Index           =   4
            Left            =   1290
            TabIndex        =   61
            ToolTipText     =   "Load List with MISSION_NUM_batch.txt"
            Top             =   1995
            Width           =   915
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Default All"
            Height          =   315
            Index           =   3
            Left            =   1290
            TabIndex        =   60
            ToolTipText     =   "Load List with FLTinfo Flight Dates"
            Top             =   1635
            Width           =   915
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Clear"
            Height          =   315
            Index           =   2
            Left            =   1290
            TabIndex        =   59
            ToolTipText     =   "Clear Flight Date List"
            Top             =   1275
            Width           =   915
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Remove"
            Height          =   315
            Index           =   1
            Left            =   1290
            TabIndex        =   58
            ToolTipText     =   "Remove Selected Flighht Date from List"
            Top             =   555
            Width           =   915
         End
         Begin VB.CommandButton cmdFltDates 
            Caption         =   "&Add"
            Height          =   270
            Index           =   0
            Left            =   1290
            TabIndex        =   57
            ToolTipText     =   "Add this Flight Date to List"
            Top             =   240
            Width           =   915
         End
         Begin VB.ListBox lstFltDates 
            Height          =   2400
            ItemData        =   "frmTools.frx":05FC
            Left            =   90
            List            =   "frmTools.frx":0603
            MultiSelect     =   1  'Simple
            TabIndex        =   56
            ToolTipText     =   "Flight Dates to be Batch Processed"
            Top             =   570
            Width           =   1125
         End
         Begin VB.TextBox txtFltDates 
            Height          =   285
            Left            =   90
            TabIndex        =   55
            Top             =   240
            Width           =   1125
         End
         Begin VB.Label Label 
            Caption         =   "Entries"
            Height          =   240
            Index           =   29
            Left            =   1290
            TabIndex        =   124
            Top             =   2760
            Width           =   510
         End
      End
      Begin VB.CheckBox chkSkipBadScan 
         Caption         =   "Skip Bad Scans"
         Height          =   255
         Left            =   -69750
         TabIndex        =   53
         Top             =   870
         Width           =   1545
      End
      Begin MSComDlg.CommonDialog CommonDialog1 
         Left            =   -71250
         Top             =   3510
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.TextBox txtTest 
         Height          =   315
         Left            =   -71250
         TabIndex        =   48
         Top             =   4020
         Width           =   705
      End
      Begin VB.Frame Frame 
         Caption         =   "File Name"
         Height          =   2145
         Index           =   2
         Left            =   -72180
         TabIndex        =   42
         Top             =   360
         Width           =   3975
         Begin VB.Frame Frame 
            Caption         =   "RTD"
            Height          =   735
            Index           =   10
            Left            =   90
            TabIndex        =   109
            Top             =   660
            Width           =   2085
            Begin VB.OptionButton optTtgt 
               Caption         =   "Low"
               Height          =   195
               Index           =   1
               Left            =   150
               TabIndex        =   113
               Top             =   450
               Value           =   -1  'True
               Width           =   795
            End
            Begin VB.OptionButton optTtgt 
               Caption         =   "High"
               Height          =   195
               Index           =   2
               Left            =   1020
               TabIndex        =   112
               Top             =   450
               Width           =   855
            End
            Begin VB.OptionButton optTtgt 
               Caption         =   "None"
               Height          =   195
               Index           =   0
               Left            =   150
               TabIndex        =   111
               Top             =   210
               Width           =   705
            End
            Begin VB.OptionButton optTtgt 
               Caption         =   "Window"
               Height          =   195
               Index           =   3
               Left            =   1020
               TabIndex        =   110
               Top             =   210
               Width           =   945
            End
         End
         Begin VB.Frame Frame 
            Caption         =   "Before/After"
            Height          =   735
            Index           =   9
            Left            =   2220
            TabIndex        =   106
            Top             =   660
            Width           =   1665
            Begin VB.OptionButton optBA 
               Caption         =   "Before"
               Height          =   195
               Index           =   0
               Left            =   120
               TabIndex        =   108
               Top             =   270
               Value           =   -1  'True
               Width           =   765
            End
            Begin VB.OptionButton optBA 
               Caption         =   "After"
               Height          =   195
               Index           =   1
               Left            =   120
               TabIndex        =   107
               Top             =   480
               Width           =   645
            End
         End
         Begin VB.TextBox txtPtFilename 
            Height          =   285
            Left            =   90
            TabIndex        =   47
            Text            =   "c:\MTP\Setup\Pt\Pt_ERhigh.txt"
            Top             =   1740
            Width           =   3825
         End
         Begin VB.ComboBox cboPt 
            Height          =   315
            Left            =   480
            TabIndex        =   43
            Text            =   "ER2"
            Top             =   270
            Width           =   1785
         End
         Begin VB.Label Label 
            Caption         =   "Default Filename"
            Height          =   225
            Index           =   15
            Left            =   90
            TabIndex        =   46
            Top             =   1530
            Width           =   1245
         End
         Begin VB.Label Label 
            Caption         =   "SU"
            Height          =   285
            Index           =   13
            Left            =   90
            TabIndex        =   44
            Top             =   300
            Width           =   435
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Calculate T Given Counts"
         Height          =   885
         Index           =   1
         Left            =   -70500
         TabIndex        =   33
         Top             =   2880
         Width           =   2295
         Begin VB.CheckBox chkCThex 
            Caption         =   "H"
            Height          =   225
            Left            =   330
            TabIndex        =   114
            ToolTipText     =   "Text Box contains a hexidecimal number if checked"
            Top             =   240
            Width           =   405
         End
         Begin VB.CheckBox chkTunits 
            Caption         =   "K"
            Height          =   225
            Left            =   1140
            TabIndex        =   39
            ToolTipText     =   "Answer in Kelvin (not Celcius) when checked"
            Top             =   240
            Width           =   405
         End
         Begin VB.CommandButton cmdCalcTc 
            Caption         =   "T(C)?"
            Height          =   375
            Left            =   1680
            TabIndex        =   38
            Top             =   420
            Width           =   555
         End
         Begin VB.TextBox txtTcelcius 
            Enabled         =   0   'False
            Height          =   315
            Left            =   930
            TabIndex        =   36
            Top             =   480
            Width           =   705
         End
         Begin VB.TextBox txtCts 
            Height          =   315
            Left            =   90
            TabIndex        =   35
            Text            =   "1000"
            Top             =   480
            Width           =   765
         End
         Begin VB.Label Label 
            Caption         =   ")"
            Height          =   225
            Index           =   26
            Left            =   750
            TabIndex        =   115
            Top             =   240
            Width           =   105
         End
         Begin VB.Label Label 
            Caption         =   ")"
            Height          =   225
            Index           =   12
            Left            =   1530
            TabIndex        =   40
            Top             =   240
            Width           =   105
         End
         Begin VB.Label Label 
            Caption         =   "T("
            Height          =   255
            Index           =   11
            Left            =   930
            TabIndex        =   37
            Top             =   240
            Width           =   165
         End
         Begin VB.Label Label 
            Caption         =   "C ("
            Height          =   255
            Index           =   10
            Left            =   90
            TabIndex        =   34
            Top             =   240
            Width           =   195
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Pt Fit Input"
         Height          =   2685
         Index           =   0
         Left            =   -74940
         TabIndex        =   24
         Top             =   360
         Width           =   2715
         Begin VB.ListBox lstOhms 
            Height          =   1230
            ItemData        =   "frmTools.frx":0611
            Left            =   60
            List            =   "frmTools.frx":0624
            TabIndex        =   52
            Top             =   810
            Width           =   855
         End
         Begin VB.TextBox txtOhms 
            Height          =   315
            Left            =   60
            TabIndex        =   51
            Top             =   480
            Width           =   855
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&H to D"
            Height          =   375
            Index           =   6
            Left            =   930
            TabIndex        =   50
            Top             =   2130
            Width           =   825
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Import"
            Height          =   375
            Index           =   5
            Left            =   60
            TabIndex        =   49
            Top             =   2130
            Width           =   825
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Save"
            Height          =   375
            Index           =   4
            Left            =   1860
            TabIndex        =   45
            Top             =   2130
            Width           =   765
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Default"
            Height          =   375
            Index           =   3
            Left            =   1860
            TabIndex        =   41
            Top             =   1710
            Width           =   765
         End
         Begin VB.CheckBox chkHex 
            Caption         =   "Hex"
            Height          =   255
            Left            =   1650
            TabIndex        =   30
            Top             =   210
            Width           =   585
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Clear"
            Height          =   375
            Index           =   2
            Left            =   1860
            TabIndex        =   29
            Top             =   1290
            Width           =   765
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Sub"
            Height          =   375
            Index           =   1
            Left            =   1860
            TabIndex        =   28
            Top             =   870
            Width           =   765
         End
         Begin VB.CommandButton cmdPt 
            Caption         =   "&Add"
            Height          =   345
            Index           =   0
            Left            =   1860
            TabIndex        =   27
            Top             =   480
            Width           =   765
         End
         Begin VB.TextBox txtCounts 
            Height          =   315
            Left            =   960
            TabIndex        =   26
            Top             =   480
            Width           =   825
         End
         Begin VB.ListBox lstCounts 
            Height          =   1230
            ItemData        =   "frmTools.frx":0645
            Left            =   960
            List            =   "frmTools.frx":0658
            TabIndex        =   25
            Top             =   810
            Width           =   825
         End
         Begin VB.Label Label 
            Caption         =   ")"
            Height          =   225
            Index           =   7
            Left            =   2250
            TabIndex        =   32
            Top             =   210
            Width           =   105
         End
         Begin VB.Label Label 
            Caption         =   "Resitance    Counts  ("
            Height          =   285
            Index           =   6
            Left            =   90
            TabIndex        =   31
            Top             =   210
            Width           =   1635
         End
      End
      Begin VB.CommandButton cmdReadXS 
         Caption         =   "Compare MTP and XS at Different Altiudes"
         Height          =   885
         Index           =   3
         Left            =   -74820
         TabIndex        =   23
         Top             =   2190
         Width           =   1365
      End
      Begin VB.CheckBox chkUseUTwindow 
         Caption         =   "Use UT Window"
         Height          =   345
         Left            =   -72990
         TabIndex        =   22
         Top             =   1590
         Value           =   1  'Checked
         Width           =   1545
      End
      Begin VB.CheckBox chkUseZg 
         Caption         =   "Use Geometric Altitude"
         Height          =   345
         Left            =   -72990
         TabIndex        =   21
         Top             =   1920
         Value           =   1  'Checked
         Width           =   2085
      End
      Begin VB.TextBox txtNfiles 
         Alignment       =   1  'Right Justify
         Height          =   345
         Left            =   -71010
         MultiLine       =   -1  'True
         TabIndex        =   20
         Text            =   "frmTools.frx":0679
         Top             =   2640
         Width           =   345
      End
      Begin VB.CommandButton cmdReadXS 
         Caption         =   "Read Many XS Files With Temporal Interpolation"
         Height          =   885
         Index           =   2
         Left            =   -72000
         TabIndex        =   18
         Top             =   3120
         Width           =   1365
      End
      Begin VB.CommandButton cndTimeTest 
         Caption         =   "Time Test"
         Height          =   435
         Left            =   -71670
         TabIndex        =   17
         Top             =   750
         Width           =   1095
      End
      Begin VB.CommandButton cmdWriteParms 
         Caption         =   "Write &Selected Parms"
         Height          =   375
         Index           =   0
         Left            =   -71760
         TabIndex        =   16
         Top             =   1290
         Width           =   1800
      End
      Begin VB.CommandButton cmdWriteParms 
         Caption         =   "Write &Default Parms"
         Height          =   375
         Index           =   1
         Left            =   -71760
         TabIndex        =   15
         Top             =   900
         Width           =   1800
      End
      Begin VB.CommandButton cmdTestGH 
         Caption         =   "Test GH Format"
         Height          =   435
         Left            =   -74790
         TabIndex        =   14
         Top             =   1440
         Width           =   1425
      End
      Begin VB.ComboBox cboFileSrc 
         Height          =   315
         Left            =   -71940
         TabIndex        =   13
         Text            =   "DAO"
         Top             =   2250
         Width           =   915
      End
      Begin VB.TextBox txtUTstart 
         Alignment       =   1  'Right Justify
         Height          =   345
         Left            =   -71970
         MultiLine       =   -1  'True
         TabIndex        =   11
         Text            =   "frmTools.frx":067D
         Top             =   2640
         Width           =   345
      End
      Begin VB.TextBox txtZtMax 
         Height          =   315
         Left            =   -72210
         TabIndex        =   9
         Text            =   "18.0"
         Top             =   1140
         Width           =   435
      End
      Begin VB.TextBox txtZpMin 
         Height          =   315
         Left            =   -72210
         TabIndex        =   8
         Text            =   "10.0"
         Top             =   810
         Width           =   435
      End
      Begin VB.TextBox txtZtOffset 
         Height          =   315
         Left            =   -73200
         TabIndex        =   4
         Text            =   "1.00"
         Top             =   510
         Width           =   435
      End
      Begin VB.CheckBox chkZtOffset 
         Caption         =   "Only Use Data >"
         Height          =   285
         Left            =   -74820
         TabIndex        =   3
         Top             =   510
         Width           =   1485
      End
      Begin VB.CommandButton cmdReadXS 
         Caption         =   "Read 1 XS File With Temporal Interpolation"
         Height          =   885
         Index           =   1
         Left            =   -73380
         TabIndex        =   2
         Top             =   3120
         Width           =   1305
      End
      Begin VB.CommandButton cmdReadXS 
         Caption         =   "Read XS File"
         Height          =   885
         Index           =   0
         Left            =   -74820
         TabIndex        =   1
         Top             =   3120
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "CodeText Box"
         Height          =   255
         Index           =   33
         Left            =   -74880
         TabIndex        =   229
         Top             =   360
         Width           =   1185
      End
      Begin VB.Label Label 
         Caption         =   "Use this command only when the Tools form is being used in MTPbin"
         Height          =   585
         Index           =   31
         Left            =   -72360
         TabIndex        =   224
         ToolTipText     =   "Source of alternate P and T: NO=default=Nav, MM=MMS, PT=PTW"
         Top             =   3795
         Width           =   1965
      End
      Begin VB.Label Label 
         Caption         =   "Enter text below"
         Height          =   255
         Index           =   20
         Left            =   -74910
         TabIndex        =   205
         Top             =   930
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "List Box of Items"
         Height          =   255
         Index           =   21
         Left            =   -71940
         TabIndex        =   204
         Top             =   960
         Width           =   1215
      End
      Begin VB.Label lblProgress 
         Caption         =   "Position"
         Height          =   255
         Left            =   120
         TabIndex        =   156
         Top             =   4635
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Total Recs"
         Height          =   195
         Index           =   28
         Left            =   -69750
         TabIndex        =   122
         Top             =   1905
         Width           =   930
      End
      Begin VB.Label Label 
         Caption         =   "LocHor"
         Height          =   195
         Index           =   27
         Left            =   -69750
         TabIndex        =   120
         Top             =   1560
         Width           =   555
      End
      Begin VB.Label Label 
         Caption         =   "Record"
         Height          =   195
         Index           =   22
         Left            =   -69750
         TabIndex        =   84
         Top             =   1200
         Width           =   555
      End
      Begin VB.Label Label 
         Caption         =   "Number of FIles"
         Height          =   405
         Index           =   5
         Left            =   -71610
         TabIndex        =   19
         Top             =   2610
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "XS File Src"
         Height          =   315
         Index           =   4
         Left            =   -72900
         TabIndex        =   12
         Top             =   2310
         Width           =   915
      End
      Begin VB.Label Label 
         Caption         =   "UT xx Hours of First File"
         Height          =   405
         Index           =   3
         Left            =   -72900
         TabIndex        =   10
         Top             =   2610
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Maximum Tropopause Altitude [km]"
         Height          =   285
         Index           =   2
         Left            =   -74790
         TabIndex        =   7
         Top             =   1200
         Width           =   2535
      End
      Begin VB.Label Label 
         Caption         =   "Minimum Aircraft Altitude [km]"
         Height          =   285
         Index           =   1
         Left            =   -74790
         TabIndex        =   6
         Top             =   900
         Width           =   2115
      End
      Begin VB.Label Label 
         Caption         =   "km from tropopause"
         Height          =   315
         Index           =   0
         Left            =   -72690
         TabIndex        =   5
         Top             =   540
         Width           =   1485
      End
   End
End
Attribute VB_Name = "frmTools"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim AllParms As Boolean, Nparms%
Dim OpenSource As Boolean              'TRUE if Open Source on Tools tab is being used
Dim FirstAline As Boolean
Dim Takeoff As Boolean
Dim Landing As Boolean

Dim defLAT1!                        'Default latitude for start of tropical to mid-latitude transition
Dim defLAT2!                        'Default latitude for end of tropical to mid-latitude transition
Dim defLAT3!                        'Default latitude for start of mid-latitude to polar transition
Dim defLAT4!                        'Default latitude for end of mid-latitude to polar transition

Dim NAVgainCount%                   'Number of good NAV gains
Dim GEgainCount%                    'Number of good Gain Equation gains
Dim NDgainCount%                    'Number of good Noise Diode gains
Dim FTPcount%                       'Number of FTP transmissions
Dim DoRetrieval As Boolean          'If TRUE, perform retrievals (normally TRUE)
Dim vWB$                            'Current version of program
Dim Ccycle%                         'Count Quality Count
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
Dim Substitute As Boolean   'Substitute P and T values into RAW file
Dim BadCounts&              'Number of bad counts

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

Sub NAVinit()
Dim V!

  V = 99999.99
  Pstatic = V: OATn = V: Pitch = V: Roll = V: Latitude = V: Longitude = V: Heading = V: gALT = V: pALT = V: Wspd = V
  Wdir = V: UWS = V: VWS = V: WWS = V: ThetaAC = V: OATn0 = V: OATn1 = V: OATn0 = V: OATn2 = V: OATn3 = V: OATn4 = V
  OATn5 = V: Mach = V: Mach2 = V: wALT = V: rALT = V: hALT = V: iALT = V: Ptotal = V: Pdynamic = V: GroundSpeed = V
  PSFC = V: Vzac = V: TOATn0 = V: TOATn1 = V: TOATn2 = V: TOATn3 = V: TOATn4 = V: TOATn5 = V: ACINS = V
  QC_A = V: TAS = V: IAS = V: TrackAngle = V: DriftAngle = V: SideSlipAngle = V: AttackAngle = V: Tdew = V: Pcabin = V: SZA = V
  SEAC = V: SAG = V: SAAC = V

End Sub


Sub Substitute_XXX(FltDate$)
Dim RAWfile$, INPlu%, OUTlu%, IWGlu%, TXTlu%, OUTfile$, txtFile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim i%, j%, L%, AA$, Bb$, cC$, DD$, EE$, UT&, PTWfile$, RIFfile$, UTks!
Dim GoToNext As Boolean, pALTpt!, OATpt!, pALTm!, pALTft!, PText$, b$, Tstring$, X$, One%
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, IWGrecord&
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Th!, UTsecR&, UTsecRIF&, UTsecRAW&
Static StartDOY%, UTsecLast&, DayCount%

  DayCount = 0   'Number of 24 hour changes
  Substitute = True
  RAWfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + "." + txtRAWsource.Text
  OUTfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".R" + Trim(cboPTfile.Text)
  txtFile$ = Path$ + AC$ + YYYYMMDD$ + ".txt"
  
  If chkUseRIF.Value = 0 Then
    PTWfile$ = fFullFileName(Trim(cboPTfile.Text))
' 1 means substitute values
' 0 means write P and T values to text file.
    If chkWritePTtxt.Value = 1 Then
      One = 0
      TXTlu = FreeFile
      Open txtFile$ For Output As TXTlu
    Else
      One = 1
      OUTlu% = FreeFile
      Open OUTfile$ For Output As OUTlu%
    End If
  
    BadCycle = 0
    Cycle = 1
    
    INPlu% = FreeFile
    Open RAWfile$ For Input As INPlu%
    LOFraw = LOF(INPlu%)
    ProgressBar1.Min = 0
    ProgressBar1.Max = 100
  
    Record = 0
    FirstAline = True
'   Open external PT data file if needed

'   Synchronize all files (assume that RAW file begins before PT files
    Do
NextAline:
      Line Input #INPlu, A$
      ByteCount = Loc(INPlu%) * 128
      If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
    '    Debug.Print a$
      If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then
        Call DecodeAline(A$, GoToNext)    'Get UTsec from RAW file
'       UTsecMTP = fTstringToSec(hhmmssMTP$, False)
        Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
      
        If UT > UTsec Then  'NAV ahead of RAW
          Do
            Line Input #INPlu, A$
            If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
          Loop Until UTsec >= UT
          If UTsec > UT Then  'Make equal
            Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
          End If
        End If
      Else
        GoTo NextAline
      End If
    Loop Until UT <= UTsec

' Skip one RAW cycle because current A-line is lost for MAIN processing when
' re-synched in INITIALIZE because MMS file is not closed and re-opened
    ProgressBar1.Min = 0
    ProgressBar1.Max = 100
    Do
      If EOF(INPlu) Then Exit Do
  
      Do
        If Not FirstAline Then Input #INPlu, A$
        ByteCount = Loc(INPlu%) * 128
        If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
        Select Case Left$(A$, 1)
        Case "A"
          AA$ = A$
          Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
          If UTsec < UTsecLast Then
            DayCount = DayCount + 1: UTsecLast = UTsec
          End If
          UTsec = UTsec + 86400 * DayCount
          UTks = UTsec / 1000
          If Not FirstAline Then
            Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
          Else
            FirstAline = False
          End If
          Aline = True: Bline = False: Cline = False: Dline = False: Eline = False
    
        Case "B"                           '  C(i,j)
'B  8437  7960  8476 8346  7901  8477  8244  7892  8482  8186  7899  8491  8256  7962  8538
'12345678901234567801234567890123456780123456789012345678012345678901234567801234567890123456780
'         1        2         3        4         5        6         7        8         9
          If Len(A$) < 61 Then
            Bline% = False
          Else
            Bb$ = A$
            For j = 1 To 5
              For i = 1 To Channels
                C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 1), 6))
              Next i
            Next j
            Bline% = True
          End If
    
        Case "C"                          '  C(i,j)
          If Len(A$) < 61 Then
            Bline% = False
          Else
            cC$ = A$
            For j = 6 To 10
              For i = 1 To Channels
                C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 6), 6))
              Next i
            Next j
            Cline% = True
          End If
 
        Case "D"                           '  MUX()
          DD$ = A$
          Dline = True
    
        Case "E"
          EE$ = A$
          For j = 11 To 12
            For i = 1 To Channels
              C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 11), 6))
            Next i
          Next j
  ' Define Base, Sky and Noise Diode counts (Used in a number of places)
  
          For i = 1 To Channels
            CS(i, LocHor) = C(i, LocHor): CB(i) = C(i, 12): dND(i) = C(i, 11) - CB(i) 'c(i,11) was c(1,ll) on 20030924
          Next i
'''       CtsQual = fCheckCountsQuality(Cycle, False)
          If Not CtsQual Then BadCycle = BadCycle + 1
          Cycle = Cycle + 1
          Eline% = True
        
        End Select
      Loop Until Aline * Bline * Cline * Dline * Eline = True Or EOF(INPlu)
      If EOF(INPlu) And Aline * Bline * Cline * Dline * Eline = False Then Exit Do
      Aline = False
'----------------------
' Fix Aline with P and T values
      pALTpt = fPtoZ(P)
      If pALTpt < 0 Then pALTpt = 0
      pALTm = Int(1000 * pALTpt + 0.5)
      pALTft = Int(3.2808 * pALTm + 0.5)
      If T <> 999.99 Then OATpt = T - cTo Else OATpt = 999.99      'Celcius
    
      Select Case AC$
      Case "DC"
' Sub geometric altitude for first 3 CAMEX-4 flights
'AD227 17:52:55  31.291 -103.406  100   1.6   0.8 00000 36917 -50.3 0017   0313   10.8 1 010815 175213
'123456789012345678901234567890123456789012345678901234567890
'      Mid$(AA$, 50, 5) = Format(param(8), "00000")
'      Mid$(AA$, 55, 6) = " " + Format(pAltFt, "00000")
'      Mid$(AA$, 61, 6) = " " + formats(OATpt, "00.0")
' ER2 Format
'          1         2         3         4         5         6         7         8         9         10
' 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc yymmdd hhmmss
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc Sta yymmdd hhmmss
' A!035:10:39:41 +29.616 -95.1661  38.8  -1.2    .0   -25 00023 +22.4  3.7  218.7 -178.8 1 990920 103908
' AG266:22:02:32  -3.051 -159.368  64.2    .6   3.7 20479 20178 -65.9    .9   3.2 -188.9 1 255 970923 220222
' AG091:13:58:29 +34.947 -117.810  63.5  13.9   -.1     0  2603  -1.5  +0.0 +0.00 -194.8 1 224 0.264 100402 135810

      Case "ER", "GH"
        b$ = Format(pALTm, "####0")
        Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
        b$ = formats(OATpt, "#0.00")
        Mid$(AA$, 62, 6) = Space(6 - Len(b$)) + b$
        cC$ = Right$(AA$, 14)
        AA$ = Left$(AA$, 93) + Format(Mach, "0.000") + cC$
      Case "WB"
        b$ = Format(pALTm, "####0")
        Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
        If OATpt <> 999.99 Then 'Avoid 999.9
          b$ = formats(OATpt, "#00.00")
          Mid$(AA$, 62, 6) = Space(6 - Len(b$)) + b$
        End If
        Select Case Trim(cboPTfile.Text)
        Case "FT"
          b$ = Format(param(5), "#00.000")        'Latitude
          Mid$(AA$, 15, 8) = Space(8 - Len(b$)) + b$
          b$ = Format(param(6), "#000.000")       'Longitude
          Mid$(AA$, 23, 9) = Space(9 - Len(b$)) + b$
          b$ = Format(param(8) * 1000, "#0")        'Zg
          Mid$(AA$, 50, 6) = Space(6 - Len(b$)) + b$
          b$ = Format(param(10), "##0.0")         'Wspd
          Mid$(AA$, 68, 6) = Space(6 - Len(b$)) + b$
          b$ = Format(param(11), "##0.0")         'Wdir
          Mid$(AA$, 74, 6) = Space(6 - Len(b$)) + b$
        End Select
        
      Case "M5"
        Select Case Trim(cboPTfile.Text)
        Case "TD"
          b$ = Format(pALTm, "####0")
          Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
          b$ = Format(OATpt, "#0.0")
          Mid$(AA$, 63, 5) = Space(5 - Len(b$)) + b$
        Case "UC"
'         1         2         3         4         5         6         7         8         9        10
'123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'AG...:09:27:14  67.821   20.327  58.1   2.7    .6   445   435   -.5 999.9 999.9 -191.7 1   0 030211 092647
'AG...:99:99:99  99.990  999.900 999.9  99.9  99.9 99999 99999  99.9 999.9 999.9 -193.8 0   0 030211 104607
'AG...:09:28:05  67.820  020.330 999.9  +2.7  +0.7 99999   433 -00.3 999.9 999.9 -191.7 1   0 030211 092738
          Tstring = Right$(AA$, 27)
          b$ = Right$(AA$, 13)
          y% = Val(Left$(b$, 2))
          If y% < 80 Then y% = 2000 + y% Else y% = 1900 + y%
          M% = Val(Mid$(b$, 3, 2))
          D% = Val(Mid$(b$, 5, 2))
          Doy = fYMDtoDOY(y, M, D)
'      UTsec = fTstringToSec(Right$(AA$, 6), False) + 27  'MTP is 27 seconds slow
'    param(3) = mPitch; param(4) = mRoll; param(5) = mLatitude; param(6) = mLongitude; param(7) = mHeading
'    param(8) = mZg; param(9) = mZp; param(10) = mWspd; param(11) = mWdir
          AA$ = "AG" + Format(Doy, "000") + ":" + fSecToTstring(UTsec, True)
          b$ = Format(param(5), "00.000")          'latitude
          AA$ = AA$ + Space(8 - Len(b$)) + b$
          b$ = Format(param(6), "000.000")         'longitude
          AA$ = AA$ + Space(9 - Len(b$)) + b$
          b$ = Format(param(7), "##0.0")
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = formats(param(3), "0.0")            'pitch
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = formats(param(4), "0.0")            'roll
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = Format(param(8), "####0")           'mZg
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = Format(param(9), "####0")           'mZp
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = formats(OATpt, "#0.0")
          AA$ = AA$ + " " + Space(5 - Len(b$)) + b$
          b$ = Format(param(10), "##0.0")          'Wspd
          AA$ = AA$ + Space(6 - Len(b$)) + b$
          b$ = Format(param(11), "##0.0")          'Wdir
          AA$ = AA$ + Space(6 - Len(b$)) + b$ + Tstring$
      '+ " 999.9 999.9" + Tstring$
          If Mid$(AA$, 88, 1) = "9" Or Mid$(AA$, 88, 1) = "0" Then Mid$(AA$, 88, 1) = "1"
      'Mid$(AA$, 88, 1) = "1"           'ElCorUsed
        End Select
        
      End Select
'Next line used to flag bad scans in ACCENT data
'If Not CtsQual Then Mid$(AA$, 2, 1) = "!" Else Mid$(AA$, 2, 1) = "W"

      If One = 1 Then
        Print #OUTlu, AA$
        Print #OUTlu, Bb$
        Print #OUTlu, cC$
        Print #OUTlu, DD$
        Print #OUTlu, EE$
        Print #OUTlu, ""
      Else
      'NB pALT and OATn are default NAV values, pALTpt and OATpt are substituted values
        X$ = Format(UTsec / 1000, "00.000") + vbTab + Format(pALT, "00.00") + vbTab
        X$ = X$ + Format(pALTpt, "00.00") + vbTab + Format(OATn + cTo, "#00.0") + vbTab + Format(OATpt + cTo, "#00.0")
        Print #TXTlu, X$
        'Debug.Print x$
      End If
  
      ByteCount = Loc(INPlu%) * 128
      If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
      DoEvents
    Loop Until EOF(INPlu)

    Close INPlu
    If One = 1 Then Close OUTlu Else Close TXTlu
    Substitute = False

    cboPTfile.ListIndex = 0 'Set back to "NO" = None
    If One = 1 Then
  '  Kill RAWfile$
  '  Name OUTfile$ As RAWfile$
    End If
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''



  Else   'Use RIF file
    RIFfile$ = Drive$ + Rdir2$ + Mission$ + "\IWG\IWG1_" + YYYYMMDD$ + ".RIF"
'   1 means substitute values, 0 means write P and T values to text file.
    If chkWritePTtxt.Value = 1 Then
      One = 0
      TXTlu = FreeFile
      Open txtFile$ For Output As TXTlu
    Else
      One = 1
      OUTlu% = FreeFile
      Open OUTfile$ For Output As OUTlu%
    End If
  
    BadCycle = 0
    Cycle = 1
      
    Record = 0
    FirstAline = True
'   Open external PT data file if needed

'   Synchronize all files (assume that RIF file begins before RAW files
    UTsecRIF = fGetRIFstartTime(RIFfile$)
    UTsecRAW = fGetRAWstartTime(RAWfile$)
    IWGlu = FreeFile
    IWGrecord = 0
    Open RIFfile$ For Random Access Read Write As IWGlu Len = Len(IWG)
    
    INPlu% = FreeFile
    Open RAWfile$ For Input As INPlu%
    LOFraw = LOF(INPlu%)
    ProgressBar1.Min = 0
    ProgressBar1.Max = 100
    
    If UTsecRAW > UTsecRIF Then 'RAWfile starts before RIF file, synch them
      Do
        IWGrecord = IWGrecord + 1
        Call IWGread(IWGlu%, IWGrecord&)
        Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsecRIF&, False)
      Loop Until UTsecRIF >= UTsecRAW
    Else                       'RIF file starts before RAW file, synch them
      Do
        Line Input #INPlu, A$
        If Left$(A$, 1) = "A" Then
          Call DecodeAline(A$, GoToNext)
          UTsecRAW = UTsec
        End If
      Loop Until UTsecRAW >= UTsecRIF
    End If
'   RAW file at or beyond RIF file
    Do
      IWGrecord = IWGrecord + 1
      Call IWGread(IWGlu%, IWGrecord&)
      Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsecRIF&, False)
    Loop Until UTsecRIF >= UTsecRAW
    Mach = IWG_Parameter(11)         '--
    T = IWG_Parameter(20) + cTo      'Kelvin
    If cboPTfile.Text = "IW" Then T = fTc(T, pALT, Mach ^ 2)
    P = IWG_Parameter(23)            'hPa
    IWGrecord = 0

    Do
      If EOF(INPlu) Then Exit Do
      
      Do 'Read RAW file
        If Not FirstAline Then Input #INPlu, A$
        ByteCount = Loc(INPlu%) * 128
        If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
        Select Case Left$(A$, 1)
        Case "A"
          AA$ = A$
          Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
          If UTsec < UTsecLast Then
            DayCount = DayCount + 1: UTsecLast = UTsec
          End If
          UTsec = UTsec + 86400 * DayCount
          UTks = UTsec / 1000
          If Not FirstAline Then
            Do
              IWGrecord = IWGrecord + 1
              Call IWGread(IWGlu%, IWGrecord&)
              Call DateTtoStrings(DandT, YYYYMMDD, HHMMSS, UTsecRIF&, False)
            Loop Until UTsecRIF >= UTsec
            Mach = IWG_Parameter(11)         '--
            T = IWG_Parameter(20) + cTo      'Kelvin
            If cboPTfile.Text = "IW" Then T = fTc(T, pALT, Mach ^ 2)
            P = IWG_Parameter(23)            'hPa
          
          Else
            FirstAline = False
          End If
          Aline = True: Bline = False: Cline = False: Dline = False: Eline = False
    
        Case "B"                           '  C(i,j)
'B  8437  7960  8476 8346  7901  8477  8244  7892  8482  8186  7899  8491  8256  7962  8538
'12345678901234567801234567890123456780123456789012345678012345678901234567801234567890123456780
'         1        2         3        4         5        6         7        8         9
          If Len(A$) < 61 Then
            Bline% = False
          Else
            Bb$ = A$
            For j = 1 To 5
              For i = 1 To Channels
                C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 1), 6))
              Next i
            Next j
            Bline% = True
          End If
    
        Case "C"                          '  C(i,j)
          If Len(A$) < 61 Then
            Bline% = False
          Else
            cC$ = A$
            For j = 6 To 10
              For i = 1 To Channels
                C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 6), 6))
              Next i
            Next j
            Cline% = True
          End If
 
        Case "D"                           '  MUX()
          DD$ = A$
          Dline = True
    
        Case "E"
          EE$ = A$
          For j = 11 To 12
            For i = 1 To Channels
              C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 11), 6))
            Next i
          Next j
  ' Define Base, Sky and Noise Diode counts (Used in a number of places)
  
          For i = 1 To Channels
            CS(i, LocHor) = C(i, LocHor): CB(i) = C(i, 12): dND(i) = C(i, 11) - CB(i) 'c(i,11) was c(1,ll) on 20030924
          Next i
'''       CtsQual = fCheckCountsQuality(Cycle, False)
          If Not CtsQual Then BadCycle = BadCycle + 1
          Cycle = Cycle + 1
          Eline% = True
        End Select
      Loop Until Aline * Bline * Cline * Dline * Eline = True Or EOF(INPlu)
      If EOF(INPlu) And Aline * Bline * Cline * Dline * Eline = False Then Exit Do
      Aline = False
'----------------------
' Fix Aline with P and T values
      pALTpt = fPtoZ(P)
      If pALTpt < 0 Then pALTpt = 0
      pALTm = Int(1000 * pALTpt + 0.5)
      pALTft = Int(3.2808 * pALTm + 0.5)
      If T <> 999.99 Then OATpt = T - cTo Else OATpt = 999.99      'Celcius
      
      If TAS > 0 Then
      Select Case AC$
      Case "DC"

      Case "ER", "GH"
        b$ = Format(pALTm, "####0")
        Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
        b$ = formats(OATpt, "#0.00")
        Mid$(AA$, 62, 6) = Space(6 - Len(b$)) + b$
        b$ = Right$(AA$, 14)
        AA$ = Left$(AA$, 93) + Format(Mach, "0.000") + b$
      Case "WB"
        b$ = Format(pALTm, "####0")
        Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
        If OATpt <> 999.99 Then 'Avoid 999.9
          b$ = formats(OATpt, "#00.00")
          Mid$(AA$, 62, 6) = Space(6 - Len(b$)) + b$
        End If
        Select Case Trim(cboPTfile.Text)
        Case "FT"
          b$ = Format(param(5), "#00.000")        'Latitude
          Mid$(AA$, 15, 8) = Space(8 - Len(b$)) + b$
          b$ = Format(param(6), "#000.000")       'Longitude
          Mid$(AA$, 23, 9) = Space(9 - Len(b$)) + b$
          b$ = Format(param(8) * 1000, "#0")      'Zg
          Mid$(AA$, 50, 6) = Space(6 - Len(b$)) + b$
          b$ = Format(param(10), "##0.0")         'Wspd
          Mid$(AA$, 68, 6) = Space(6 - Len(b$)) + b$
          b$ = Format(param(11), "##0.0")         'Wdir
          Mid$(AA$, 74, 6) = Space(6 - Len(b$)) + b$
        End Select
      
      End Select
    
        If One = 1 Then
          Print #OUTlu, AA$
          Print #OUTlu, Bb$
          Print #OUTlu, cC$
          Print #OUTlu, DD$
          Print #OUTlu, EE$
          Print #OUTlu, ""
        Else
        ' NB pALT and OATn are default NAV values, pALTpt and OATpt are substituted values
          X$ = Format(UTsec / 1000, "00.000") + vbTab + Format(pALT, "00.00") + vbTab
          X$ = X$ + Format(pALTpt, "00.00") + vbTab + Format(OATn + cTo, "#00.0") + vbTab + Format(OATpt + cTo, "#00.0")
          Print #TXTlu, X$
          'Debug.Print x$
        End If
      End If
        ByteCount = Loc(INPlu%) * 128
        If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
        DoEvents
      Loop Until EOF(INPlu)

      Close INPlu
      If One = 1 Then Close OUTlu Else Close TXTlu
      Substitute = False
    End If

End Sub

Sub Substitute_NGV(FltDate$)
Dim RAWfile$, INPlu%, OUTlu%, TXTlu%, OUTfile$, txtFile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim i%, j%, k%, L%, AA$, Bb$, cC$, DD$, EE$, UT&, PTWfile$, UTks!, Tc$, Zft$, TFR!
Dim GoToNext As Boolean, pALTpt!, OATpt!, pALTm!, pALTft!, PText$, b$, Tstring$, X$, One%
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Tcor!, Th!, mMach2!
Dim ALTlast!, dALT!, ValidParams As Boolean
Static DayCount%

' Substitute P and T values from a PI instrument file for the default NAV values
' RAW file has extension changed to RMM (MMS), RPT (NOAA PT), etc to avoid confusion
' Initially we read MMS or PT values when needed when the data was processed
' I found having separate files less confusing.
  
  DayCount = 0   'Number of 24 hour changes
  UTsecIlast = 0
  UTsecI = 0
' IWG line entry: 6=Pressure altitude (m), 20=ambient temperature (C)
  RAWfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + "." + txtRAWsource.Text
  OUTfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".R" + Trim(cboPTfile.Text)
  PTWfile$ = Drive$ + Rdir2$ + Mission$ + "\NG\NG" + YYYYMMDD$ + ".asc"
  txtFile$ = Path$ + AC$ + YYYYMMDD$ + ".txt"

' 1 means substitute values, 0 means write P and T values to text file.
  If chkWritePTtxt.Value = 1 Then
    One = 0
    TXTlu = FreeFile
    Open txtFile$ For Output As TXTlu
  Else
    One = 1
    OUTlu% = FreeFile
    Open OUTfile$ For Output As OUTlu%
  End If
  
'''  Initialize   ' Was InitializeDC and ER!!!!!!!!!!!!
'''  Initialize_RCs_Calfile
  BadCycle = 0
  Cycle = 1
  Quit = False
    
  INPlu% = FreeFile
  Open RAWfile$ For Input As INPlu%
  LOFraw = LOF(INPlu%)
  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
  
  Record = 0
  FirstAline = True
' Open external PT data file if needed
' Synchronize all files (assume that RAW file begins before NG files
  Do
NextAline:
    Line Input #INPlu, A$
    ByteCount = Loc(INPlu%) * 128
    If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
    If Left$(A$, 3) = "IWG" Then
      DecodeGVline A$, FirstAline      'Get UTsec from RAW file
'      If frmMTPbin.chkShowChannels(12).Value = 1 Then yyyymmddI$ = Trim(frmFLTINFO.cboFltDates.Text)  '"20100930"

      Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
'     param(9)=PALT (NACA PA), param(18)=ATHR2

      If UT < UTsec Then  'NAV ahead of RAW
        Do  'Make equal ie advance NG file
          Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
        Loop Until UT >= UTsec
        X = X
      Else  'Advance RAW file because it starts before NG file
        Do
          Line Input #INPlu, A$
          If Left$(A$, 3) = "IWG" Then
            DecodeGVline A$, Not FirstAline       'Get UTsec from RAW file
          End If
        Loop Until UTsec >= UT
        Do  'Make equal ie advance NG file
          Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
        Loop Until UT >= UTsec

      End If
    Else
      GoTo NextAline
    End If
  Loop Until UT >= UTsec
' RAW and ASC FILES SYNCHED!!!!

  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
  Do
      Aline% = 0: Bline% = 0: M01line% = 0: M02line% = 0: Ptline = 0: Eline% = 0: IWGline% = 0
    Do
      If EOF(INPlu) Then Exit Do
      If Not FirstAline Then
        Line Input #INPlu, A$
      Else
        If Left$(A$, 1) = "A" Then Aline = 1
      End If
      FirstAline = False
      ByteCount = Loc(INPlu%) * 128
      If ByteCount < LOFraw Then ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)

      Select Case Left$(A$, 1)
      Case "A"    'Process A-line using NEW data format.
        If Mid$(A$, 3, 2) = "99" Then Return
        'a$ = "AD999 99999999 9999999 99999999 99999 99999 9999 99999 99999 99999 9999  99999 999999 1" + MID$(a$, 88, 14) '960219 101112"
        'Un-comment a$ line above to simulate bad DADS data
        NavQual = True
        Call DecodeGVline(A$, FirstAline)           'This routine decodes all lines
        pALT = ZpAvg                     'Use average Zp for scan
'       Time in A-line is MTP clock time synchronized to NTP -- program uses this
'       Time in IWG-line is time from IWG server, which has some latency
        LineCurr(1) = A$
        Aline% = 1
        Bline% = 0: M01line% = 0: M02line% = 0: Ptline = 0: Eline% = 0: IWGline% = 0

      Case Else  'Lines other than A
        Select Case Left$(A$, 1)
        Case "B": Bline = 1: LineCurr$(3) = A$
        Case "E": Eline = 1: LineCurr$(7) = A$
        End Select
    
        Select Case Left$(A$, 3)
        Case "M01": M01line = 1: LineCurr$(4) = A$
        Case "M02": M02line = 1: LineCurr$(5) = A$
        Case "Pt:": Ptline = 1: LineCurr$(6) = A$
        Case "IWG"
        'Debug.Print A$

          Call DecodeGVline(A$, FirstAline)            'This routine decodes all lines
          IWGline = 1: LineCurr$(2) = A$
          'Debug.Print A$
        End Select
      
      End Select
    
    Loop Until Aline% * IWGline% * Bline * M01line% * M02line% * Ptline * Eline% = 1  '
    If EOF(INPlu) Then Exit Do

'   Now get P and T values from ASC file in NG folder

     If UT < UTsec Then
      Do  'Make equal
        Call GetNAVvalues(Trim(cboPTfile.Text), UTsec&, UT&, P!, T!, param!(), PTWfile$, Quit)
   '    Debug.Print UTsec; UT; param(9); param(16)
      Loop Until UT >= UTsec Or Quit
    End If
   If Quit Then Exit Do
    ValidParams = True
    
'   Fix P and T values
    AA$ = LineCurr$(2)    'IWG1 line
    Bb$ = ""
    k = 0                 'k is index in IWG1 line, NOT param() index!
    j = InStr(1, AA$, ",")
    Bb$ = Left$(AA$, j)
    Do
      i = InStr(j + 1, AA$, ",")
      k = k + 1
      'Debug.Print k; Mid$(AA$, j + 1, i - j - 1); i - j - 1
      Select Case k
      Case 6
        If param(9) = -999.99 Then ValidParams = False: Exit Do   'param(9)=pALT in km
        Zft$ = Format(param(9) * 3280.8, "####0.00")              'convert ft
        Bb$ = Bb$ + Zft$ + ","
      Case 20           'ambient temperature location in IWG line
        Select Case Mission
        Case "START-08"
          If param(18) = -999.99 Then ValidParams = False: Exit Do
          Tcor = fTc(param(18), param(9), param(23))
        Case "HIPPO"
          If param(16) = -999.99 Then ValidParams = False: Exit Do
          Tcor = fTc(param(16), param(9), param(23))
        Case "HIPPO-2", "HIPPO-3", "PREDICT", "PREDICT2", "HIPPO-4", "HIPPO-5", "TORERO"
        '=Q13*(1+AF$11*AA13)+AF$10 Param(2)=AT_A, Param(23)=Mach Squared
        'Coefficients are from: HIPPO-3_RAOBcomparison­_FLonlynonredundantM.xlsx
        'Which uses Fit with M^2 instead Zp function for fit
          If param(23) < 0 Then ValidParams = False: Exit Do
          Tcor = fTc(param(2), param(9), param(23))
        Case Else
          MsgBox vbOKOnly, "Substitute_NGV does not recognize an fTc for this mission."
        End Select
        
        Tc$ = Format(Tcor - cTo, "#0.0000")  'param(18)=THR2   (17)=THR1    (16)=TFR
        Bb$ = Bb$ + Tc$ + ","
      Case Else
        Bb$ = Bb$ + Mid$(AA$, j + 1, i - j)
      End Select
      j = i
      'Debug.Print Bb$ + vbCr
    Loop Until InStr(j + 1, AA$, ",") = 0
    'Debug.Print Bb$
    If ValidParams Then
      If j < i Then Bb$ = Bb$ + Mid$(AA$, j + 1, i - j)
      LineCurr$(2) = Bb$
      For i = 1 To 7
        Print #OUTlu, LineCurr$(i)
      Next i
      Print #OUTlu, ""
    End If
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu

End Sub
Sub GetAvgRmsTropCP(FltDate$, OUTlu%)
Dim X$
Dim i%, j%, Sum1!, Sum2!
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, dpALTok As Boolean, nMax%
Dim iMax%, ZpFltMin!, Z1!(1 To 2000), Z2!(1 To 2000), T1!(1 To 2000), T2!(1 To 2000)
Dim Zt1AVG!, Zt1RMS!, Tt1AVG!, Tt1RMS!, ZcpAVG!, ZcpRMS!, TcpAVG!, TcpRMS!, Min!, Max!

  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
  nMax = 10
  ZpFltMin = 19#
  Filename$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  
  Record = 0: j = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
      
    If GoodScan And pALT > ZpFltMin And Zt1 < 19.5 And Zcp < 25 Then
      j = j + 1
      Z1(j) = Zt1
      T1(j) = TT1
      Z2(j) = Zcp
      T2(j) = Tcp
    End If
  Loop Until EOF(REFlu)
  Close REFlu
  
  X$ = Format(FltDate$, "0000") + vbTab
  Call AvgRmsMinMax(Z1(), j, Zt1AVG, Zt1RMS, Min, Max)
  X$ = X$ + Format(Zt1AVG, "#0.00") + vbTab + Format(Zt1RMS, "#0.00") + vbTab + Format(Min, "#0.00") + vbTab + Format(Max, "#0.00") + vbTab
  
  Call AvgRmsMinMax(T1(), j, Tt1AVG, Tt1RMS, Min, Max)
  X$ = X$ + Format(Tt1AVG, "#0.00") + vbTab + Format(Tt1RMS, "#0.00") + vbTab + Format(Min, "#0.00") + vbTab + Format(Max, "#0.00") + vbTab
  
  Call AvgRmsMinMax(Z2(), j, ZcpAVG, ZcpRMS, Min, Max)
  X$ = X$ + Format(ZcpAVG, "#0.00") + vbTab + Format(ZcpRMS, "#0.00") + vbTab + Format(Min, "#0.00") + vbTab + Format(Max, "#0.00") + vbTab
  
  Call AvgRmsMinMax(T2(), j, TcpAVG, TcpRMS, Min, Max)
  X$ = X$ + Format(TcpAVG, "#0.00") + vbTab + Format(TcpRMS, "#0.00") + vbTab + Format(Min, "#0.00") + vbTab + Format(Max, "#0.00") + vbTab

  Print #OUTlu, X$

End Sub

Sub GetStratTrop(FltDate$)
Dim OUTlu%, Level%, Record%, X$, Filename$

  Filename$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".ERF"
  REFopen Filename$
  
  OUTlu = FreeFile
  Open MNpath$ + "MP\ST" + FltDate$ + ".txt" For Output As OUTlu
  
  Record = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
    If Zt1 < 99 Then 'need to deal with double trop (not really since GV doesn't get above 15 km)
      If pALT <= Zt1 Then
        If Zt1 - pALT <= 1 Then Level = 2 Else Level = 1
      Else
        If pALT - Zt1 <= 1 Then Level = 3 Else Level = 4
      End If
    Else  'Undetermined
      Level = 5
    End If
    X$ = Format(UTsec, "000000") + vbTab + Format(Level, "0")
    Print #OUTlu, X$
  Loop Until EOF(REFlu)
  Close REFlu, OUTlu


End Sub

Sub GetLatLon(FltDate$)
Dim OUTlu%, Level%, Record%, X$, Filename$

  Filename$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  
  OUTlu = FreeFile
  Open MNpath$ + "LL" + FltDate$ + ".txt" For Output As OUTlu
  
  Record = 0
  Do
    Record = Record + 1
    Call REFread(REFlu, Record)
    X$ = Format(UTsec, "000000") + vbTab + Format(Latitude, "##0.000") + vbTab + Format(Longitude, "###0.000") + vbTab + Format(pALT, "#0.000")
    If UTsec <> 0 Then Print #OUTlu, X$
  Loop Until EOF(REFlu)
  Close REFlu, OUTlu


End Sub

Sub ReadRAWfile()
Dim Mux%(0 To 16), TXTlu%, txtFile$, TTTlu%, TTTfile$
Dim i%, j%, A$, UTks!, X$
Dim PTfile$, Navg%, n2%, n3%, GoToNext As Boolean, Zgh!, UT&
Dim dTAi_OK As Boolean, Gi!(1 To 3), dG!(1 To 3), NDo!(1 To 3), ByteCount1&
Dim f As Form, DefCalfile$, ans As Variant, Emax!, fn$
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Th!, INIlu%

Static FP!(1 To 4)

  GoSub Initialize
'  GoSub SyncToExternalData
  Takeoff = True   'False
'  fEcCount = 0
  For i = 1 To 6: LineLast(i) = LineCurr(i): Next i
' Start reading RAW records
  Do
    ReadRAWrecord (Quit)  'Quit is TRUE if EOF and NOT Realtime

  Loop Until EOF(RAWlu) And Not RealTime

EndOfSub:
  Close RAWlu  'Close RAW file
  txtTotalRecords.Text = RawCycle
  RAWstamp = Mid$(yymmdd$, 3, 2) + "/" + Right$(yymmdd$, 2) + "/" + Left$(yymmdd$, 2)
Exit Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
Initialize:
'  Pause = False
  TtgtPC = 0
  RawCycle = 0
  GoodCycle = 0
'  BadCycle = 0
  Cycle = 0
  UTsec = 0
  UTsecLast = 0
'  Navg = Val(txtTavg.Text)      'Number of temperatures to average
'  LocHor = Val(txtLocHor.Text)  'Initialization routines have not been run yet!
' Handle aircraft dependent parameters
  frmATP.FooterType = 0
  Ceiling = 24#               'Service ceiling
  LocHor = 6
  CycleTime = 15              'Time for one MTP cycle (s)
  Select Case AC$
  Case "ER", "WB", "NG"
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
  
'  Call DirCheck(MNpath$, Trim(f.cboFltDates.Text))
  
'  If chkUseSubstitutedRAWfile.value = 0 Then RAWextension$ = "RAW" Else RAWextension$ = "R" + Trim(cboPTfile.Text)
'  If RAWextension$ = "RAW" Then
'    RAWfile$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + ".RAW" ' + RAWextension$
'  Else
'    RAWfile$ = MNpath$ + Trim(f.cboFltDates.Text) + "\" + AC$ + Trim(f.cboFltDates.Text) + "." + RAWextension$
'  End If
  RAWfile$ = txtRAWfile.Text
  
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
  
  RAWlu = FreeFile
  Open RAWfile$ For Input As RAWlu
  LOFraw = LOF(RAWlu%)
'  ProgressBar1.Min = 0
'  ProgressBar1.Max = 100
  Record = 0
  FirstAline = True
Return
  
End Sub
Sub ReadRAWrecord(Quit As Boolean)
Dim i%, j%, A$, b$, UTks!, X$, UT&, c1$
Dim GoToNext As Boolean, CycleTakeoff%
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!, Lt$
Dim Wu!, Wv!, Ww!, param!(1 To 75), P!, T!, Th!, FirstAline As Boolean, ALTlast!, dALT!
Static Day, Change As Boolean, pALTftLast&, lu%, Poff!, UTsecMTPlast&, dtMTP_NAV&
  
  If RawCycle = 0 Then UTsecLast = 0  'Make sure reset in case of multiple passes
  GoSub ReadABCDElines               'Read five lines of input data

' Note: Cycle only counts good cycles, but ML and TL use all cycles.
  RawCycle% = RawCycle% + 1                     'Increment total raw cycle count
  Cycle = RawCycle
  If NavQual Then NQV(RawCycle) = True Else NQV(RawCycle) = False

  Aline = False

' Write current cycle to memory
'  WriteArray (RawCycle) 'Call BINwrite(BINlu, RawCycle)
'  CheckLimits (RawCycle) 'Lat Lon check after fix in WriteArray
EndOfSub:
Exit Sub

ReadABCDElines:
  If Cycle > 0 Then GoTo ProcessNextAline
TryAgain:
  If EOF(RAWlu) Then Quit = True: Exit Sub
  
' Get an input line from RAW File
  Input #RAWlu%, A$
  
  FirstAline = False
  ByteCount = Loc(RAWlu%) * 128
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
' Check if Channel 1 needs to be dropped

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
      'Beep:         GoTo EndOfSub 'GoTo TerminateProgram
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
'        pALTtakeoff = pALT
        hhmmssTO$ = hhmmssNavLast$
      End If
'      If Cycle > 1150 Then
'        Debug.Print UTseclast; Format(pALT, "00.000"); "  "; Format(dALT, "#00.000")
'      End If
      
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
    ALTkmft = pALT * cft_km
    pALTft = pALT * cft_km
               
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
    
    LineCurr(1) = A$
    Aline% = True
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

'  UTsecLast = UTsec


Return

End Sub


Function fStripAlphaNumeric(X$) As String
Dim i%, n%, A$, Keep As Boolean

A$ = ""
For i = 1 To Len(X$)
  Keep = True
  n = Asc(Mid$(X$, i, 1))
  If n > 42 Then
    If n < 58 Then
    Else
      If n < 65 Then
        Keep = False
      Else
        If n > 90 Then Keep = False
      End If
    End If
  Else
    Keep = False
  End If
  If Keep Then A$ = A$ + Mid$(X$, i, 1)
Next i
fStripAlphaNumeric = A$
' A-Z, 65-90
' 0-9, 48-57
' /    47
' .    46
' -    45
' '    44
' +    43
' _    95
' a-z, 97-122
End Function

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
  Case 8: fMJcolor = &HFFFF&    'yellow
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

Sub GetLevelFlightLegs(FltDate$, dUTmin!, PitchMax!, dZpMax!, dPitchMax!, Hist%(), HistStep!, OUTlu%)
Dim X!, i%, j%, ZpSave!(1 To 2000), ZpMax!, ZpMin, ZpAvg!, ZpRms!, Sum1!, Sum2!
Dim DD$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, dpALTok As Boolean, nMax%
Dim PitchLast!, pALTlast!(1 To 10), dPitch!, dpALT!(1 To 10), UTks1!, UTks2!, UTks!, Count%
Dim iMax%, zHistStep!
Static ZpFltMax!

  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  DD$ = Right$(FltDate$, 2)
  nMax = 10
  'ZpFltMax = 0#
  Filename$ = Path$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  REFopen Filename$
  
  Record = 1: j = 0
  Call REFread(REFlu, Record)
  For i = 1 To nMax:  pALTlast(i) = pALT: Next i
  Do
    UTks1 = 0#: UTks2 = 0#: Count = 0
    Do
      Record = Record + 1
      Call REFread(REFlu, Record)
    ' Accumulate data for Zp Histogram
    If pALT > 13 Then
    X = X
    End If
      iMax = 15 / HistStep + 1
      Do
        zHistStep = -HistStep
        For i = 1 To iMax
          zHistStep = zHistStep + HistStep
          If pALT >= zHistStep And pALT < zHistStep + HistStep Then
            Hist(i) = Hist(i) + 1
            Exit Do
          End If
        Next i
      Loop Until i >= iMax
    ' Now do remaining statistics
      If pALT > ZpFltMax Then
      ZpFltMax = pALT
      End If
      For i = 1 To nMax: dpALT(i) = pALT - pALTlast(i):  Next i
      dpALTok = True
      For i = 1 To nMax
        If Abs(dpALT(i)) > dZpMax Then dpALTok = False: Exit For
      Next i
      For i = nMax To 2 Step -1: pALTlast(i) = pALTlast(i - 1): Next i
      pALTlast(1) = pALT
      dPitch = Pitch - PitchLast
      PitchLast = Pitch
      UTks = UTsec / 1000#

' Apply various limit requirements
      If dpALTok Then  'And Abs(dPitch) < dPitchMax And Abs(Pitch) < PitchMax Then
        Count = Count + 1
        ZpSave(Count) = pALT
        If UTks1 = 0# Then  'Start a flight segment
          UTks1 = UTks
        Else                'End a flight segment
          UTks2 = UTks      'Keep updating level flight end UT
        End If
      Else
        If (UTks2 > 0#) And (UTks2 - UTks1 > dUTmin) Then Exit Do    'Limit to >10 min legs
        UTks1 = 0
        UTks2 = 0
        Count = 0
      End If
      DoEvents
    Loop Until EOF(REFlu)
    If EOF(REFlu) Then
    Exit Do
    End If
      ZpMin = 999#: ZpMax = -999#: Sum1 = 0#: Sum2 = 0#
    For i = 1 To Count
      If ZpSave(i) < ZpMin Then ZpMin = ZpSave(i)
      If ZpSave(i) > ZpMax Then ZpMax = ZpSave(i)
      Sum1 = Sum1 + ZpSave(i)         'Sum time difference
      Sum2 = Sum2 + ZpSave(i) ^ 2    'Sum square of time differences
    Next i
    ZpAvg = Sum1 / Count        'Calculate average
    If Sum2 - ZpAvg ^ 2 * Count > 0 Then ZpRms = Sqr((Sum2 - ZpAvg ^ 2 * Count) / (Count - 1)) 'Calculate RMS
    'Debug.Print " "; Format(Count, "0000"); " "; Format(UTks1, "#00.000"); " "; Format(UTks2, "#00.000"); " "; Format(ZpAvg, "#0.00"); " "; Format(ZpRms, "0.00"); " "; Format(ZpMin, "#0.00"); " "; Format(ZpMax, "#0.00"); " "; Format(ZpFltMax, "#0.00")
    Print #OUTlu, " "; Format(Count, "0000"); " "; Format(UTks1, "#00.000"); " "; Format(UTks2, "#00.000"); " "; Format(ZpAvg, "#0.00"); " "; Format(ZpRms, "0.00"); " "; Format(ZpMin, "#0.00"); " "; Format(ZpMax, "#0.00"); " "; Format(ZpFltMax, "#0.00")

  Loop Until EOF(REFlu)
  Close REFlu

' Print out closest approach list
  For i = 1 To j
'    If OUTlu <> 0 Then Print #OUTlu, yy$ + vbTab + mm$ + vbTab + dd$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + R$(i) + vbTab + R$(i) + vbTab + Format(Rsave(i), "000.0")
'    Debug.Print yy$ + vbTab + mm$ + vbTab + dd$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + R$(i) + vbTab + R$(i) + vbTab + Format(Rsave(i), "000.0")
  Next i
      Debug.Print ZpFltMax

End Sub

Sub Update_Pt_Filename()
Dim Filename$, X$, AC$
  
  AC$ = Left$(Trim(cboPt.Text), 2)
  If AC$ = "DC" Then optTtgt(1).Caption = "tgt1" Else optTtgt(1).Caption = "low"
  If AC$ = "DC" Then optTtgt(2).Caption = "tgt2" Else optTtgt(2).Caption = "high"
  Filename = Filename + AC$
  Filename = SUpath$ + "Pt_" + AC$ + "_" '"C:\MTP\Setup\Pt\Pt"
  If optTtgt(0).Value = True Then X$ = "__"
  If optTtgt(1).Value = True Then X$ = optTtgt(1).Caption
  If optTtgt(2).Value = True Then X$ = optTtgt(2).Caption
  If optTtgt(3).Value = True Then X$ = "win"
  
  If optBA(0).Value Then X$ = X$ + "_B" Else X$ = X$ + "_A"
  txtPtFilename.Text = Filename + X$ + ".txt"


End Sub

Private Sub cboPt_Click()
Update_Pt_Filename
End Sub


Private Sub chkZpLevels_Click()

  If chkZpLevels.Value = 1 Then
    label(24).Visible = False
    label(25).Visible = False
    label(32).Visible = True
    txtZpStep.Visible = True
    txtZpStep.Text = 1
    txtALTkmMin.Visible = False
  Else
    label(24).Visible = True
    label(25).Visible = True
    txtALTkmMin.Visible = True
    txtALTkmMin.Text = 5#
    label(32).Visible = False
    txtZpStep.Visible = False
  End If
  
End Sub

Private Sub cmdBatchProcessor_Click(Index As Integer)
Dim Rmax!, OUTlu%, INlu%, nSites%, i%, j%, k%, L%, ZpMax!, dZpMax!, dPitchMax!, dUTmin!, PitchMax!
Dim Hist%(1 To 500), HistStep!, iMax%, X$, FQFN$, NDT01!(1 To 50), NDT02!(1 To 50), NDT03!(1 To 50)
Dim TndAvg!(1 To 3), TndRMS!(1 To 3), Apriori!(1 To 30), A!(1 To 50), y!, jLast%, temp$
Dim Filename$, REFfile$, RAFfile$, RAFlu%, YYYYMMDD$, dT!, RAOB1$, RAOB1s$, OutCount%, FilenameALL$, FilenameNR$
Dim FilenameUsed$, FilenameUnique$, FilenameCSV$, ans As Variant
Dim iNum%, iY%, iM%, iD%, UT0!, LR11!, LR12!, Zb1!, LR21!, LR22!, Zb2!, Ra1!, UTsave!, Nada As Boolean, NGVlu%, NGlu%, lu%, lu2%
Dim iNums%, iYs%, iMs%, iDs%, UTs!, LR11s!, LR12s!, Zb1s!, LR21s!, LR22s!, Zb2s!, b$, C$
Dim Zp!, Zps!, MMS As Boolean, MMfile$, FPfile$, UT&, P!, T!, Th!, Synch As Boolean
Dim LI%, out$, seconds&, FltCount%, TotalTime&, hms$, U!, V!, W!, INP$, IWGlu%, IWGrecord&, IWGvariable%, UTsecM&
Dim TimeStep!, ZpStep!, ALTkmMin!, UTksLast!

  
  Select Case Index
  Case 0   'Avg and rms trop, cold-point T and Z
    OUTlu = FreeFile
    Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_Trop_CP.txt" For Output As OUTlu
    X$ = "FltDate" + vbTab + "ZTA" + vbTab + "ZTR" + vbTab + "Min" + vbTab + "Max" + vbTab
    X$ = X$ + "TTA" + vbTab + "TTR" + vbTab + "Min" + vbTab + "Max" + vbTab
    X$ = X$ + "ZCA" + vbTab + "ZCR" + vbTab + "Min" + vbTab + "Max" + vbTab
    X$ = X$ + "TCA" + vbTab + "TCR" + vbTab + "Min" + vbTab + "Max"
    Print #OUTlu, X$
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call GetAvgRmsTropCP(FltDate$, OUTlu%)
    Next i
    Close OUTlu
  
  Case 1   'ID RAOB Range
    Rmax = Val(txtRmax.Text)     'Maximum Range for a RAOB selection
    If chkCOSMIC.Value = 1 Then temp$ = "_COSMICrangeAll.csv" Else temp$ = "_RAOBrangeAll.csv"
    FQFN$ = Drive$ + Rdir2$ + Mission$ + "\RAOB\" + Mission$ + temp$
    If Dir(FQFN$) <> "" Then
      ans = MsgBox(FQFN$ + " already exists." + vbCrLf + "You will loose your lapse rate extension information is you continue!" + vbCrLf + "Do you want to continue?", vbYesNo)
      If ans = vbNo Then Exit Sub
    End If
    OUTlu = FreeFile
    Open FQFN$ For Output As OUTlu
    
    If txtALTkmMin.Visible = True Then ALTkmMin = Val(txtALTkmMin.Text) Else ALTkmMin = 5#
    ZpStep = Val(txtZpStep.Text)
    jLast = 0
    For i = 0 To lstFltDates.ListCount - 1
      nSites = 0
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
'The commented-out code only used for Trex analysis which didn't have RAW files
'      If AC$ = "NG" Then
'        Call GetRAOBrangeNGV(FltDate$, Rmax, OUTlu, nSites%, Val(txtALTkmMin.Text))
'      Else
'        Call GetRAOBrange(FltDate$, Rmax, OUTlu, nSites%, Val(txtALTkmMin.Text))
'      End If
      If chkCOSMIC.Value = 1 Then
        If chkZpLevels.Value = 1 Then
          Call GetCOSMICRangeNGV(FltDate$, Rmax, OUTlu, nSites%, ALTkmMin, ZpStep, jLast)
        Else
          Call GetCOSMICRangeNGV(FltDate$, Rmax, OUTlu, nSites%, ALTkmMin, ZpStep, jLast)
        End If
      Else
        If chkZpLevels.Value = 1 Then
          Call GetRAOBrangeZp(FltDate$, Rmax, OUTlu, nSites%, ALTkmMin, ZpStep, jLast)
        Else
          Call GetRAOBrange(FltDate$, Rmax, OUTlu, nSites%, ALTkmMin, ZpStep, jLast)
        End If
      End If
    Next i
    Close OUTlu
    
  Case 2   'ID Level Flight Legs
    OUTlu = FreeFile
    Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_LevelFlight.txt" For Output As OUTlu
    
    ZpMax = Val(txtZpMax.Text)            'Maximum flight altitude for mission (km)
    HistStep = Val(txtZpBin.Text) / 1000# '.2 km histogram steps
    dUTmin = Val(txtdUTmin.Text) / 1000#  '.6 ks = 10 min = 600 s minimum level flight
    dZpMax = Val(txtdZpMax.Text) / 1000#  '60 meters maximum FL change per scan
    dPitchMax = Val(txtdPitchMax.Text)    ' 2 degrees maximum absolute pitch change per scan
    PitchMax = Val(txtPitchMax.Text)      'Maximum allowed pitch in level flight
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      'Debug.Print FltDate$
      Print #OUTlu, FltDate$
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call GetLevelFlightLegs(FltDate$, dUTmin!, PitchMax!, dZpMax!, dPitchMax!, Hist%(), HistStep!, OUTlu%)
    Next i
  ' Print out Histogram statistics for all flights
    Print #OUTlu, " "
    Print #OUTlu, "Zp Histogram"
    Print #OUTlu, " N Zp"
    For i = 1 To ZpMax / HistStep + 1
      Print #OUTlu, Hist(i), Format((i - 0.5) * HistStep, "00.00")
      'Debug.Print Hist(i), Format((i - 0.5) * HistStep, "00.00")
    Next i
    Close OUTlu
  
    Case 3
    OUTlu = FreeFile
    Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_ND_temperatures.txt" For Output As OUTlu
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      FQFN$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
      'Debug.Print FltDate$
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call REF2readCALFILE(FQFN$)
      NDT01(i + 1) = Cnd0(1)
      NDT02(i + 1) = Cnd0(2)
      NDT03(i + 1) = Cnd0(3)
      X$ = ""
      For j = 1 To Channels
        
        X$ = X$ + Format(Cnd0(j)) + " " + Format(Cnd1(j)) + " " + Format(Cnd2(j)) + " "
      Next j
      Print #OUTlu, FltDate$ + " " + X$
    Next i
      i = lstFltDates.ListCount
      Call AvgRMS(NDT01(), i, TndAvg(1), TndRMS(1))
      Call AvgRMS(NDT02(), i, TndAvg(2), TndRMS(2))
      Call AvgRMS(NDT03(), i, TndAvg(3), TndRMS(3))
    For i = 1 To Channels
      Print #OUTlu, "CH" + Str(i) + ": " + Format(TndAvg(i), "#00.00") + " " + Format(TndRMS(i), "#0.00")
    Next i
  ' Print out Histogram statistics for all flights
    Close OUTlu
  
  Case 4 'Radiometer Noise
    OUTlu = FreeFile
    Open MNpath$ + "RAOB\RMSerror\" + "RadiometerNoise.csv" For Output As OUTlu
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      FQFN$ = MNpath$ + "RAOB\RMSerror\RMS" + FltDate$ + ".txt"
      'Debug.Print FltDate$
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      INlu = FreeFile
      Open FQFN$ For Input As INlu
      For j = 1 To 30
        Input #INlu, y
        Apriori(j) = Apriori(j) + y
      Next j
      Close INlu
    Next i
  
  y = lstFltDates.ListCount
  For i = 1 To 10
    Print #OUTlu, Format(Apriori(i) / y, "#0.00") + ", " + Format(Apriori(i + 10) / y, "#0.00") + ", " + Format(Apriori(i + 20) / y, "#0.00")
  Next i
  Close OUTlu
  
  Case 5 'Gain tabulation
    OUTlu = FreeFile
    Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_GainParms.txt" For Output As OUTlu
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      'Debug.Print FltDate$
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      X$ = FltDate$
      Call REFreadHEADER(MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF")
      For j = 1 To Channels
        X$ = X$ + vbTab + Format(Cnd0(j), "000.00") + vbTab + Format(Cnd1(j), "#0.000") + vbTab + Format(Cnd2(j), "#0.000")
      Next j
      Print #OUTlu, X$
    Next i
    Close OUTlu
  
  Case 6 'Stratosphere of troposphere ID
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call GetStratTrop(FltDate$)
    Next i
  
  Case 7 'Print out flight track latitude and longitude
    
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call GetLatLon(FltDate$)
    Next i

  Case 8 'Create RAF Files
    For i = 0 To lstFltDates.ListCount - 1
      YYYYMMDD$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = YYYYMMDD$
      REFfile$ = MNpath$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
      REFopen REFfile$
      REFreadHEADER (REFfile$)
      
      RAFfile$ = MNpath$ + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".RAF"
      If Len(Dir(RAFfile$)) > 0 Then Kill RAFfile$
      RAFlu = FreeFile
      Open RAFfile$ For Random Access Read Write As RAFlu Len = Len(REF2)
      FileFormatOut = 33
      REFwriteHEADER (RAFfile$)
      Record = 0
      dT = fATPc(0, True)  'Initialize polynomial fit coefficients
      Do
        Record = Record + 1
        Call REFread(REFlu, Record)
        If UTsec > 80730 Then
        'Debug.Print UTsec; pALT
        End If
        For j% = 1 To 33
          'Debug.Print j; sZZZ(j); sTTT(j); fATPc(sZZZ(j%) - sZZZ(16), False); sTTT(j%) - fATPc(sZZZ(j%) - sZZZ(16), False)
          If sZZZ(j) > 0 And Abs(sZZZ(j) - sZZZ(16)) <= 8.1 Then sTTT(j%) = sTTT(j%) - fATPc(sZZZ(j%) - sZZZ(16), False)
        Next j%
        Call REFwrite(RAFlu, Record)
      Loop Until EOF(REFlu)
      Close REFlu, RAFlu
      
      DoEvents
    Next i
    DoAll = False
    
  Case 9  'Select non-redundant RAOB comparisons and write to Mission_RAOBrangeNR.txt file (and Mission_RAOBrangeUsed.txt file)
    FilenameCSV$ = RSpath$ + Mission$ + "_RAOBrangeAll.csv"
    FilenameALL$ = RSpath$ + Mission$ + "_RAOBrangeAll.txt"
    FilenameNR$ = RSpath$ + Mission$ + "_RAOBrangeNR.txt"
    FilenameUsed$ = RSpath$ + Mission$ + "_RAOBrangeUsed.txt"
    TimeStep = Val(txtTimeStep.Text) / 1000#
    
    If Dir(FilenameCSV) <> "" Then  'Copy CSV file to TXT file
      lu = FreeFile
      Open FilenameCSV For Input As lu
      lu2 = FreeFile
      Open FilenameALL For Output As lu2
      Do
        Line Input #lu, b$
        C$ = Replace(b$, ",", vbTab)
        Print #lu2, C$
      Loop Until EOF(lu)
      Close lu, lu2
    End If
    lu = FreeFile
    Open FilenameALL For Input As lu
    lu2 = FreeFile
    Open FilenameNR$ For Output As lu2
  
    Line Input #lu, b$     'Read and write header record
    Print #lu2, b$
    OutCount = 0
    iYs = 0: iMs = 0: iDs = 0: RAOB1s$ = "": Zps = 0#
    Do
      Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, b$
'      Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; b$
'Number  Year  Month Day UT0     LR11  LR12  Zb1 LR21  LR22  Zb2 RAOBs RAOBs Ra1   Ra2   Zp(km)
'1       2010  3     16  66.597  0     2.5   26  0     2.5   26  DNR   DNR   123.6 123.6 2.1
'2       2010  3     16  67.431  0     2.5   26  0     2.5   26  DNR   DNR   145.9 145.9 1.9
      i = InStr(1, b$, vbTab)
      RAOB1$ = Left$(b$, i - 1)
      i = InStrRev(b$, vbTab)
      Zp = Val(Right$(b$, Len(b$) - i))
      If iY <> iYs Or iM <> iMs Or iD <> iDs Or RAOB1$ <> RAOB1s$ Or Abs(Zp - Zps) > 0.3 Or (UT0 - UTksLast) > TimeStep Then
        OutCount = OutCount + 1
        C$ = Format(iNum, "000") + vbTab + Str(iY) + vbTab + Str(iM) + vbTab + Str(iD) + vbTab + Format(UT0, "000.000") + vbTab
        C$ = C$ + Format(LR11, "#0.0") + vbTab + Format(LR12, "#0.0") + vbTab + Format(Zb1, "#0.0") + vbTab
        C$ = C$ + Format(LR21, "#0.0") + vbTab + Format(LR22, "#0.0") + vbTab + Format(Zb2, "#0.0") + vbTab + b$
        Print #lu2, C$
        Debug.Print C$
        iYs = iY
        iMs = iM
        iDs = iD
        RAOB1s$ = RAOB1$
        Zps = Zp
        UTksLast = UT0
      End If
    Loop Until EOF(lu)
    Close lu, lu2
    If Dir(FilenameUsed$) <> "" Then Kill FilenameUsed$
    FileCopy FilenameNR$, FilenameUsed$

    
  Case 10  'Create Mission_RAOBrangeUnique.txt file to be used as unique template soundings
    FilenameUsed$ = RSpath$ + Mission$ + "_RAOBrangeUsed.txt"
    FilenameUnique$ = RSpath$ + Mission$ + "_RAOBrangeUnique.txt"
    lu = FreeFile
    Open FilenameUsed$ For Input As lu
    lu2 = FreeFile
    Open FilenameUnique$ For Output As lu2
  
    Input #lu, b$     'Read and write header record
    Print #lu2, b$
    OutCount = 0
    iYs = 0: iMs = 0: iDs = 0: RAOB1s$ = ""
    Do
      Input #lu, iNum, iY, iM, iD, UT0, LR11, LR12, Zb1, LR21, LR22, Zb2, b$
'      Debug.Print iNum; iY; iM; iD; UT0; LR11; LR12; Zb1; LR21; LR22; Zb2; b$
      i = InStr(1, b$, vbTab)
      RAOB1$ = Left$(b$, i - 1)
      If iY <> iYs Or iM <> iMs Or iD <> iDs Or RAOB1$ <> RAOB1s$ Then
        OutCount = OutCount + 1
        C$ = Format(iNum, "000") + vbTab + Str(iY) + vbTab + Str(iM) + vbTab + Str(iD) + vbTab + Format(UT0, "000.000") + vbTab
        C$ = C$ + Format(LR11, "#0.0") + vbTab + Format(LR12, "#0.0") + vbTab + Format(Zb1, "#0.0") + vbTab
        C$ = C$ + Format(LR21, "#0.0") + vbTab + Format(LR22, "#0.0") + vbTab + Format(Zb2, "#0.0") + vbTab + b$
        Print #lu2, C$
        Debug.Print C$
        iYs = iY
        iMs = iM
        iDs = iD
        RAOB1s$ = RAOB1$
      End If
    Loop Until EOF(lu)
    Close lu, lu2
    
  Case 11   'Load and Enable WCT
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      REFfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
      REF2readCALFILE (REFfile$)
      EnableWCT = True
      ReadMissionWCT
      REF2writeCALFILE (REFfile$)
    Next i
    frmFLTINFO.cmdFlight_Click (5) 'Reload default flight with new WCT enabled, FLTinfo.REF
  
  Case 12
    BatchStart = True
    tmrBatch0.Enabled = True
    
  Case 13  'Disable WCT
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      REFfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
      REF2readCALFILE (REFfile$)
      EnableWCT = False
      REF2writeCALFILE (REFfile$)
    Next i
    'frmMTPbin.cmdREF_Click (3)
    'frmFLTINFO.cmdFlight_Click (5) 'Reload default flight with new WCT enabled, FLTinfo.REF
  
  Case 14    'Mission Summary
    OUTlu = FreeFile
    Open MNpath$ + Mission$ + "_Summary.csv" For Output As #OUTlu
    out$ = "Flt Date,UTmin,UTmax,Total(s),Total(h),UTtakeoff(s),UTtakeoff(h),UTlanding(s),UTlanding(h),FltTime(s),FltTime(h)"
    Print #OUTlu, out$
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      REFfile$ = MNpath$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
'      If chkFixTO.Value = 1 Then
'        ReadRAWfile
'        REFfile$ = "C:\MTP\Setup\FLTINFO.REF"
'      End If
      REFreadHEADER (REFfile$)
      'frmMTPbin.cmdREF_Click (0)   'Open RAW or RNG file so Takeoff and Landing times can be fixed.
      DoEvents
      'Debug.Print lstFltDates.List(i); UTtakeoff; UTlanding
      If UTmax < UTmin Then UTmax = UTmax + 86400
      seconds = UTmax - UTmin
      hms$ = fSecToTstring(seconds, True)
      out$ = Trim(FltDate$) + "," + Format(UTmin, "000000") + "," + Format(UTmax, "000000") + "," + Format(seconds, "000000") + "," + hms$ + ","
      If UTlanding < UTtakeoff Then UTlanding = UTlanding + 86400
      seconds = UTlanding - UTtakeoff
      TotalTime = TotalTime + seconds
      hms$ = fSecToTstring(seconds, True)
      out$ = out$ + Format(UTtakeoff, "000000") + "," + fSecToTstring(UTtakeoff, True) + "," + Format(UTlanding, "000000") + "," + fSecToTstring(UTlanding, True) + "," + Format(seconds, "000000") + "," + hms$ + ","
  
      Print #OUTlu, out$
    Next i
    Print #OUTlu, "Flt Count= " + Str(lstFltDates.ListCount)
    Print #OUTlu, "Elapsed Time (s)= " + Str(TotalTime)
    Print #OUTlu, "Elapsed Time (h)= " + fSecToTstring(TotalTime, True)
    Close OUTlu%
    
  Case 15   'Convert IWG file from csv to bin
    cmdBatchProcessor(15).Caption = "Busy!"
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i                           'Select current flight date
      txtFltDates.Text = FltDate$
      FQFN$ = MNpath$ + "IWG\IWG1_" + FltDate$ + "*.csv"  'Make CSV file so can open easily in Excel
      FQFN$ = Dir(FQFN$)
      IWGlu = FreeFile
      Open MNpath$ + "IWG\" + FQFN$ For Input As IWGlu
      Line Input #IWGlu, INP$                             'Skip possible header line
      Line Input #IWGlu, INP$
      ' Get IWG start UT
      Call Decode_IWG_Date_Time(INP$, YYYYMMDDI$, HHMMSS$, UTsecI&, DandT, k%)
      
      OUTlu = FreeFile
      Open MNpath$ + "IWG\IWG1_" + FltDate$ + ".RIF" For Random Access Read Write As #OUTlu Len = Len(IWG)
      
      If chkMMS.Value = 1 Then MMS = True Else MMS = False
      'Assign MMS MM file (Note GetMMSvalues automatically checks for existence of FP file)
      If MMS = True Then  'Synch MM and IWG files 'MMS-MetData_GHawk_20111024_R1.ict
        If chkReadICTfile.Value = 1 Then
          MMfile$ = MNpath$ + "MMS\" + Dir(MNpath$ + "MMS\MMS-MetData_GHawk_" + FltDate$ + "_R*.ict")
          txtRevMM.Text = Left$(Right$(MMfile$, 5), 1)
          FPfile$ = MNpath$ + "MMS\" + Dir(MNpath$ + "MMS\MMS-FlightPath_GHawk_" + FltDate$ + "_R*.ict")
          txtRevFP.Text = Left$(Right$(FPfile$, 5), 1)
        Else
          MMfile$ = MNpath$ + "MM\MM" + FltDate$ + ".GH"
          FPfile$ = MNpath$ + "FP\FP" + FltDate$ + ".GH"
        End If
        UTsecM = fGetMMSstartTime(MMfile$)
        If UTsecM > UTsecI Then  'IWG file starts before MM file, synch them
          Do
            Line Input #IWGlu, INP$
            If INP$ <> "" Then Call Decode_IWG_Date_Time(INP$, YYYYMMDDI$, HHMMSS$, UTsecI&, DandT, k%)
          Loop Until UTsecI >= UTsecM
        Else                     'MM file starts before IWG file, synch them
          Call GetMMSvaluesM(UTsecI, UTsecM, P!, T!, Th!, U!, V!, W!, MMfile$, FPfile$, Quit)
        End If
      End If
      Quit = True                'Set TRUE so new MM file is opened
      IWGrecord = 0
      Do
        Do
          Line Input #IWGlu, INP$
        Loop Until Len(INP$) > 32 Or EOF(IWGlu)
        If EOF(IWGlu) Then Exit Do
        
        Call Decode_IWG_Date_Time(INP$, YYYYMMDDI$, HHMMSS$, UTsecI&, DandT, k%)
        
        IWGvariable = 1                    'variable number
'       Decode remaining comma delimited parameters
        Do
          IWGvariable = IWGvariable + 1
          j = k
          k = InStr(j + 1, INP$, ",")
          If k = 0 Then Exit Do
          V = Val(Mid$(INP$, j + 1, k - j - 1))
'         If k = j + 1 Then GotV(i) = False Else GotV(i) = True 'consecutive commas
'         If k = j + 4 Then
'           If Mid$(INP$, j + 1, k - j - 1) = "nan" Then GotV(i) = False
'         End If
          Select Case IWGvariable
          Case 2: Latitude = V
          Case 3: Longitude = V
          Case 4: gALT = V
          Case 5: wALT = V
          Case 6: pALT = V
          Case 7: rALT = V
          Case 8: GroundSpeed = V
          Case 9: TAS = V
          Case 10: IAS = V
          Case 11: Mach = V
          Case 12: Vzac = V
          Case 13: Heading = V
          Case 14: TrackAngle = V
          Case 15: DriftAngle = V
          Case 16: Pitch = V
          Case 17: Roll = V
          Case 18: SideSlipAngle = V
          Case 19: AttackAngle = V
          Case 20: OATn = V
          Case 21: Tdew = V
          Case 22: Ttotal = V
          Case 23: Pstatic = V
          Case 24: Pdynamic = V
          Case 25: Pcabin = V
          Case 26: Wspd = V
          Case 27: Wdir = V
          Case 28: VWS = V
          Case 29: SZA = V
          Case 30: SEAC = V
          Case 31: SAG = V
          Case 32: SAAC = V
          Case Else: Spares(i) = V
        End Select
      Loop
      
      If MMS Then  'Read MM and FP files for GloPac to calculate Mach No (fix for GloPac)
        Call GetMMSvaluesM(UTsecI&, UT&, P!, T!, Th!, U!, V!, W!, MMfile$, FPfile$, Quit)
        Pstatic = P
        pALT = fPtoZ(P) * cft_km
        OATn = T - cTo
        VWS = W
        TAS = TAS
        Mach = Mach
        Mach2 = Mach2
      End If

      IWGrecord = IWGrecord + 1
      Call IWGwrite(OUTlu, IWGrecord)
  
    Loop Until EOF(IWGlu)
    Close IWGlu, OUTlu
  Next i
  cmdBatchProcessor(15).Caption = "IWG1 => RIF"
  
  Case 16 'NG to RNF
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      FQFN$ = MNpath$ + "NG\NG" + FltDate$ + ".asc"
      'FQFN$ = Dir(FQFN$)
      NGlu = FreeFile
      Open FQFN$ For Input As NGlu
      OUTlu = FreeFile
      Open MNpath$ + "NG\NG" + FltDate$ + ".RNF" For Random Access Read Write As #OUTlu Len = Len(NAV)
      IWGrecord = 0
      Call NAVinit
      Do
        Line Input #NGlu, INP$  'Skip header line
        Do
          If EOF(NGlu) Then Exit Do
          Line Input #NGlu, INP$
        Loop Until Len(INP$) > 32 Or EOF(NGlu)
        If EOF(NGlu) Then Exit Do
'               12345678901234567890123456789012345678901234567890
'       INP$ = "IWG1,2007-11-27T15:13:14,+37.3456,-120.2345,12.123,"
'               Date , UTC, ACINS, ALT, AT_A, GGALT, GGLAT, GGLON, GGSPD, GSF, MACH_A,
'       INP$ = "2010-03-29,18:00:30,0.216235,360.715179,0.038023,38.184002,61.182102,
        L = Len(INP$)
        IWGvariable = 0                    'variable number
        j = 0                              'before comma index
        k = 0                              'after comma index
        j = 1                              '
        k = InStr(j, INP$, ",")
        X$ = Mid$(INP$, j, k - j)
'       Decode date and time
        YYYYMMDDI$ = Left(X$, 4) + Mid$(X$, 6, 2) + Mid$(X$, 9, 2)  'decode yyyy-mm-dd
        UTsecI = fTstringToSecX(Mid$(INP$, k + 1, 8), True)
        HHMMSS$ = fSecToTstringX$(UTsecI, False)
        Call StringsToDate(YYYYMMDDI$, HHMMSS$, DandT)
  
'       Decode remaining comma delimited parameters
        k = InStr(k + 1, INP$, ",")
        Do
          IWGvariable = IWGvariable + 1
          If k = 0 Then Exit Do Else j = k
          k = InStr(j + 1, INP$, ",")
          If k = 0 Then
            V = Val(Mid$(INP$, j + 1, Len(INP$) - j))
          Else
            V = Val(Mid$(INP$, j + 1, k - j - 1))
          End If
          Select Case IWGvariable
          Case 1: ACINS = V
          Case 2: ALT = V
          Case 3: OATn0 = V + cTo
          Case 4: gALT = V / 1000#
          Case 5: Latitude = V
          Case 6: Longitude = V
          Case 7: GroundSpeed = V
          Case 8: GSF = V
          Case 9: Mach = V
          Case 10: PALT_A = V
          Case 11: Pitch = V
          Case 12: Pstatic = V
          Case 13: Ptotal = V
          Case 14: QC_A = V
          Case 15: Roll = V
          Case 16: TAS = V
          Case 17: Heading = V
          Case 18: TOATn1 = V + cTo
          Case 19: TOATn2 = V + cTo
          Case 20: TOATn3 = V + cTo
          Case 21: TOATn4 = V + cTo
          Case 22: TOATn0 = V + cTo
          Case 23: Vzac = V
          Case 24: OATn1 = V + cTo
          Case 25: OATn2 = V + cTo
          Case 26: OATn3 = V + cTo
          Case 27: OATn4 = V + cTo
          Case 28: pALT = V / 1000#
          Case 29: PSFC = V
          Case 30: SideSlipAngle = V
          Case 31: TASF = V
          Case 32: Wdir = V
          Case 33: Wspd = V
          Case 34: Mach2 = V
          Case Else: Spares(i) = V
          End Select
        Loop
        IWGrecord = IWGrecord + 1
        Call NAVwrite(OUTlu, IWGrecord)
  
      Loop Until EOF(NGlu)
      Close NGlu, OUTlu
    Next i
  
  Case 17   'RIF to RND
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      FQFN$ = MNpath$ + "IWG\IWG1_" + FltDate$ + ".RIF"
      FQFN$ = Dir(FQFN$)
      NGlu = FreeFile
      Open MNpath$ + "IWG\" + FQFN$ For Input As NGlu
      OUTlu = FreeFile
      Open MNpath$ + "NAV\NAV" + FltDate$ + ".BND" For Random Access Read Write As #OUTlu Len = Len(NAV)
      IWGrecord = 0
      Call NAVinit   'Set all IWG parameters to 9999.99
      Do
        Call IWGread(IWGlu%, IWGrecord&)  'Read binary IWG file
        OATn0 = OATn + cTo
        WWS = VWS
        Mach2 = Mach ^ 2
        Call NAVwrite(OUTlu, IWGrecord)
      Loop Until EOF(NGlu)
      Close NGlu, OUTlu
    Next i
    
  End Select

End Sub
Private Sub cmdCalcTc_Click()
Dim k1!, k2!, Counts!

  k1 = Val(txtC0.Text)
  k2 = Val(txtC1.Text)

  If chkCThex.Value = 1 Then
    Counts = fHexToDec(Trim(txtCts.Text))
  Else
    Counts = Val(txtCts.Text)
  End If

  If chkTunits.Value = 1 Then  'Kelvin
    txtTcelcius.Text = Format(fTptC(k1!, k2!, Counts) + cTo, "000.00")
  Else                         'Celcius
    txtTcelcius.Text = Format(fTptC(k1!, k2!, Counts), "#00.00")
  End If

End Sub



Private Sub cmdEditPTfile_Click()
Dim PTWlu%, OUTlu%, PTfile$, OUTfile$, A$, Threshold!, SubPath$, PText$, i%, P!, T!
Dim Ts!, Tf!, TsLast!, TfLast!, First As Boolean, GoodSample As Boolean, HeaderCount%

  Threshold = 1#
  PText$ = fACext(AC$)
  SubPath$ = Drive$ + Rdir2$ + Mission$ + "\" + Trim(cboPTfile.Text) + "\"
  
  Select Case Trim(cboPTfile.Text)
  Case "MM": PTfile$ = SubPath$ + "MM" + YYYYMMDD$ + PText$ 'MMS OAT calibration file
  Case "PT": PTfile$ = SubPath$ + "PT" + YYYYMMDD$ + PText$ 'MMS OAT calibration file
  Case "FT": PTfile$ = SubPath$ + "FT" + YYYYMMDD$ + PText$ 'MMS OAT calibration file
  Case "IN": PTfile$ = SubPath$ + "IN" + YYYYMMDD$ + PText$ 'INS
  Case "UC": PTfile$ = SubPath$ + "UC" + YYYYMMDD$ + PText$ 'UCSE
  End Select
  OUTfile$ = fReplaceExtension(PTfile$, "OUT")
    
  PTWlu = FreeFile
  Open PTfile$ For Input As PTWlu
  Input #PTWlu, HeaderCount, A$  'Get Header line count
  Close PTWlu
  
  PTWlu = FreeFile
  Open PTfile$ For Input As PTWlu
  OUTlu = FreeFile
  Open OUTfile$ For Output As OUTlu

' Write Header
  Input #PTWlu, A$   'Skip Header Info
  Print #OUTlu, A$
  For i = 2 To HeaderCount
    Line Input #PTWlu, A$
    Print #OUTlu, A$
  Next i
  
  First = True
  Do  'Edit and Copy File
'Changing PTW formats!
'19990409
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp TAS StatPres DigPxdTemp
'  47385   23.02    0.77   -0.19   -1.40  296.55   35.73   11.41 1009.78   36.00
'
'19990912
'UtSec BoxTemp RamPres VertDifPres HorzDifPres AirTemp AnaPxdTemp System28 TAS StatPres DigPxdTemp
'  48801   36.20    1.08    0.45   -0.43  303.48   33.61   26.10   13.60 1011.89   28.00
'
'20020601
'CRYSTAL-FACE Format (NB Fast Air Temp <1 sec, but sampled at 1 sec, Slow Air Temp = 4 sec)
' UT  FastTmp SlowTmp StatP  PitotP    RamP    TAS
'60992 302.92 303.13 1017.47 1017.65   0.18   5.54
'60993 302.64 303.08 1017.45 1017.67   0.22   6.13
'12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
'         1         2         3         4         5         6         7         8
'20040116 preAVE (unchanged since C-F)
' UT  FastTmp SlowTmp StatP  PitotP    RamP    TAS
'56136 290.76 290.75 1013.73 1030.46  16.73  52.27

    GoodSample = True
    Line Input #PTWlu, A$

    Select Case Trim(cboPTfile.Text)
    Case "PT"
      Select Case Val(YYYYMMDD$)
      Case Is < 19990912             'Format changed (see above)
        P = Val(Mid$(A$, 64, 8))     'Celcius
        T = Val(Mid$(A$, 40, 8))
      Case Is < 20020601
        P = Val(Mid$(A$, 72, 8))     'Celcius
        T = Val(Mid$(A$, 40, 8))
      Case Else
        P = Val(Mid$(A$, 21, 7))
        Tf = Val(Mid$(A$, 7, 6))    'Kelvin
        Ts = Val(Mid$(A$, 14, 6))    'Kelvin
        If First Then
          TfLast = Tf
          TsLast = Ts
          First = False
        Else
          If Tf <> 999.99 Or Ts <> 999.99 Then
            If Abs(Tf - TfLast) > Threshold Then
              Mid$(A$, 7, 6) = " 999.99"  'Tfast
              Mid$(A$, 44, 6) = " 999.99"  'TAS
              GoodSample = False
            Else
              TfLast = Tf
            End If
            If Abs(Ts - TsLast) > Threshold Then
              Mid$(A$, 14, 6) = " 999.99"  'Tslow
              Mid$(A$, 44, 6) = " 999.99"  'TAS
              GoodSample = False
            Else
              TsLast = Ts
            End If
          End If
        End If
        T = Ts
      End Select
      If P < 0 Then P = 0#
' Edit RFI Temperatures
    
    Case "MM"
    
    End Select
    
    If chkRemoveBadTemps.Value = 1 Then
      If GoodSample Then Print #OUTlu, A$
    Else
      Print #OUTlu, A$
    End If
  Loop Until EOF(PTWlu)
' Debug.Print UT; P; fPtoZ(P)
  Close PTWlu, OUTlu
  
  If chkRenameFile.Value = 1 Then
    Kill PTfile$
    Name OUTfile$ As PTfile$
  End If
End Sub

Private Sub cmdExecute_Click(Index As Integer)
Dim RetVal

Select Case Index
Case 0 'MAPget    12345678901234567890123456                 789012345678901234567890
  RetVal = Shell("C:\My Documents\VB6\Misc\MAPget.EXE", 1)
Case 1 'RAOBman
  RetVal = Shell("C:\MTP\VB6\EXE\RAOBman.EXE", 1)
Case 2 'RAOBget
  RetVal = Shell("C:\MTP\VB6\EXE\RAOBget.EXE", 1)
Case 3 'CRLF
  RetVal = Shell("C:\MTP\VB6\EXE\CRLF.EXE", 1)
Case 4 'MTP Calculator
  RetVal = Shell("C:\MTP\VB6\EXE\MTPcalc.exe", 1)
Case 5 'Calculator
  RetVal = Shell("C:\WINNT\SYSTEM32\CALC.EXE", 1)
Case 6 'Excel
  RetVal = Shell("C:\Program Files\Microsoft Office\Office\EXCEL.EXE", 1)
Case 7 'Explorer
  RetVal = Shell("C:\WINNT\Explorer.EXE", 1)   ' Run Calculator.
Case 8 'Note Tab
  RetVal = Shell("C:\Program Files\NoteTab Light\NoteTab.exe", 1)   ' Run NoteTab
End Select
End Sub

Private Sub cmdExit_Click()
  Unload Me
End Sub

Private Sub cmdFltDates_Click(Index As Integer)
Dim lu%, Path$, Filename$, FltNumber&, FltDate&, i%, A$

On Error GoTo ErrorHandler1

Select Case Index
Case 0  'Add
  lstFltDates.AddItem txtFltDates.Text

Case 1  'Remove
  i = lstFltDates.ListIndex
  lstFltDates.RemoveItem (lstFltDates.ListIndex)
  If i < lstFltDates.ListCount Then lstFltDates.ListIndex = i Else lstFltDates.ListIndex = i - 1

Case 2  'Clear
  lstFltDates.Clear

Case 3  'Default All MISSION_NUM.txt
  lstFltDates.Clear
  For i = 0 To frmFLTINFO.cboFltDates.ListCount - 1
    lstFltDates.AddItem Trim(frmFLTINFO.cboFltDates.List(i))
  Next i
  Close lu

Case 4  'Load MISSION_NUM_batch.txt
  lstFltDates.Clear
  lu = FreeFile
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  Filename$ = SUpath$ + Mission$ + "_NUM_batch.txt"
  On Error GoTo ErrorHandler1
  Open Filename$ For Input As lu
  ' Read pairs of numbers, FltNumber and FltDate
  Do
    Input #lu, FltNumber, FltDate
    lstFltDates.AddItem Str(FltDate)
  Loop Until EOF(lu)
  Close lu

Case 5  'Save to MISSION_NUM_batch.txt
  lu = FreeFile
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  Filename$ = SUpath$ + Mission$ + "_NUM_batch.txt"
  Open Filename$ For Output As lu
  ' Write pairs of numbers, FltNumber and FltDate
  For i = 0 To lstFltDates.ListCount - 1
    Print #lu, i + 1, lstFltDates.List(i)
  Next i
  Close lu
  
Case 6  'Select just a single flight
  Call ListBoxRemoveItem(lstFltDates, False)

End Select
txtFltCount.Text = Str(lstFltDates.ListCount)
Exit Sub
'
ErrorHandler1:
  MsgBox "Unable to open: " + Filename$, vbOKOnly
  Exit Sub
End Sub

Private Sub cmdGenerateCode_Click()

  
End Sub

Private Sub cmdGenerate_Click(Index As Integer)
Dim Coeff!(0 To 6), y$, i%, j%, k$, n%, L%, A$, b$, lu%, num!(1 To 135), nModel%


  Select Case Index
  Case 0 'Write code to extract RAF coefficients from cAccuracy fit in MISSION_RAOBcomparisons.xls
    '    'y = -4.2588E-06x6 + 3.7811E-05x5 - 1.3640E-04x4 + 1.9086E-03x3 + 3.5238E-03x2 - 5.0725E-02x - 2.7073E-01
    '12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

    y$ = txtFit.Text
    L = Len(y$)
    i = InStr(y$, "=")
    j = InStr(i, y$, "x6")
    Coeff(6) = Val(Mid$(y$, i + 1, j - i - 1))
    i = InStr(j + 2, y$, "x5")
    Coeff(5) = Val(Mid$(y$, j + 2, i - j - 2))
    j = InStr(i + 2, y$, "x4")
    Coeff(4) = Val(Mid$(y$, i + 2, j - i - 2))
    i = InStr(j + 2, y$, "x3")
    Coeff(3) = Val(Mid$(y$, j + 2, i - j - 2))
    j = InStr(i + 2, y$, "x2")
    Coeff(2) = Val(Mid$(y$, i + 2, j - i - 2))
    i = InStr(j + 2, y$, "x")
    Coeff(1) = Val(Mid$(y$, j + 2, i - j - 2))
    Coeff(0) = Val(Right$(y$, L - i - 1))
  
    A$ = "  Case """ + Mission$ + """" + vbCrLf
    A$ = A$ + "    If Init Then" + vbCrLf
    A$ = A$ + "    ' " + y$
    A$ = A$ + "      c6 = " + Format(Coeff(6), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c5 = " + Format(Coeff(5), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c4 = " + Format(Coeff(4), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c3 = " + Format(Coeff(3), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c2 = " + Format(Coeff(2), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c1 = " + Format(Coeff(1), "#0.0000000000") + vbCrLf
    A$ = A$ + "      c0 = " + Format(Coeff(0), "#0.0000000000") + vbCrLf
    A$ = A$ + "    End If" + vbCrLf + vbCrLf

    b$ = ""
    For i = 6 To 1 Step -1
      b$ = b$ + Format(Coeff(i), "#0.0000000000") + ","
    Next i
    b$ = b$ + Format(Coeff(0), "#0.0000000000")

    txtFit.Text = A$ + "The RAF coefficients have been written into the " + Mission$ + "_RAF.csv file" + vbCrLf + b$
    lu = FreeFile
    Open RSpath$ + Mission$ + "_RAF.csv" For Output As lu
    Print #lu, b$
    Close lu
  
  Case 1 'Remove tabs form WCT and generate MISSION_WCT.txt file
    y$ = txtFit.Text
    L = Len(y$)
    k = 1
    i = 0
    A$ = ""
    For j = 1 To L 'use only spaces as separators
      If Asc(Mid$(y$, j, 1)) > 42 And Asc(Mid$(y$, j, 1)) < 58 Then
        A$ = A$ + Mid$(y$, j, 1)
      Else
        A$ = A$ + " "
      End If
    Next j
    Do
      j = InStr(k, A$, " ")
      If j > 0 Then
        i = i + 1
        num(i) = Val(Mid$(A$, k, j - k))
        Do
          j = j + 1
        Loop Until Mid$(A$, j, 1) <> " "
        k = j
      End If
    Loop Until j = 0
    '     123456789012345678901
    b$ = "Elev     CH1    CH2    CH3" + vbCrLf
    For n = 0 To 9
        b$ = b$ + formats(num(1 + 4 * n), "#00.0") + "  " + formats(num(2 + 4 * n), "#0.00") + "  " + formats(num(3 + 4 * n), "#0.00") + "  " + formats(num(4 + 4 * n), "#0.00") + vbCrLf
    Next n
    txtFit.Text = b$
    lu = FreeFile
    Open SUpath$ + Mission$ + "_WCT.txt" For Output As lu
    Print #lu, b$
    Close lu

  Case 2 'Remove tabs from a priori error data in MISSION_WCT.txt file and generate MISSION_RCS.txt file entry
    y$ = txtFit.Text
    L = Len(y$)
    k = 1
    i = 0
    A$ = ""
    For j = 1 To L 'use only spaces as separators
      If Asc(Mid$(y$, j, 1)) > 42 And Asc(Mid$(y$, j, 1)) < 58 Then
        A$ = A$ + Mid$(y$, j, 1)
      Else
        A$ = A$ + " "
      End If
    Next j
    Do
      j = InStr(k, A$, " ")
      If j > 0 Then
        i = i + 1
        num(i) = Val(Mid$(A$, k, j - k))
        Do
          j = j + 1
        Loop Until Mid$(A$, j, 1) <> " "
        k = j
      End If
    Loop Until j = 0
    '     123456789012345678901
    b$ = "[OBSERVABLE_ERRORS]" + vbCrLf
    For i = 1 To 3
      For n = 1 To 9
          b$ = b$ + formats(num(n + 10 * (i - 1)), "#0.00") + " "
      Next n
      b$ = b$ + formats(num(10 + 10 * (i - 1)), "#0.00") + vbCrLf
    Next i
    txtFit.Text = b$ + vbCrLf + vbCrLf + "Copy and paste the above observable errors into the " + Mission$ + "_RCS.txt file."
    
  Case 3  'IF Bandpass Model to be put in MISSION_RCS.txt file
    y$ = txtFit.Text
    L = Len(y$)
    k = 1
    i = 0
    n = 0
    A$ = ""
    For j = 1 To L 'use only spaces as separators
      If Asc(Mid$(y$, j, 1)) > 42 And Asc(Mid$(y$, j, 1)) < 58 Then
        A$ = A$ + Mid$(y$, j, 1)
      Else
        A$ = A$ + " "
      End If
    Next j
    
    Do
      j = InStr(k, A$, " ")
      If j > 0 Then
        i = i + 1
        num(i) = Val(Mid$(A$, k, j - k))
        Do
          j = j + 1
        Loop Until Mid$(A$, j, 1) <> " "
        k = j
      End If
    Loop Until j = 0
    nModel = i / 9
    '     123456789012345678901
    b$ = "[IF_BANDPASS]" + vbCrLf
    n = 1
    For i = 1 To 3
      For j = 1 To nModel
          b$ = b$ + Format(num(n), "00") + "  " + Format(num(n + 1), "000.000") + "  " + Format(num(n + 2), "000.0000") + vbCrLf
          n = n + 3
      Next j
    Next i
    txtFit.Text = b$ + vbCrLf + vbCrLf + "Copy and paste the above IF Bandpass Model into the " + Mission$ + "_RCS.txt file."
  
  Case 4 'Spare
  
  End Select
  
End Sub


Private Sub cmdKojima_Click()
Dim RAWfile$, INPlu%, OUTlu%, OUTfile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim UTks!, Levels%, REFlu%, Pi$, UT&, i%
Dim dTmin!, dTmax!, ZtMax!, ZtOffset!, FileSrc$, UTsec1&, UTsec2&
Dim Filename$, xsPath$, FirstPass As Boolean, XSlu%, DateMTP As Date
Dim Iyear%, Imonth%, Iday%, UTks1!, UTks2!, ZZp!(1 To 200), ZZg!(1 To 200), ZZt!(1 To 200)
Dim Ihour1%, Ihour2%, Imin1%, Imin2%, ZgMZp!, ZgMin!, ZgMax!, ZpAvg!, ZgAvg!, ZpMin!, ZpMax!
Dim ZpSum1!, ZpSum2!, ZgSum1!, ZgSum2!, ZtAvg!, ZtSum1!, ZtSum2!, Count%
FirstPass = True
Pi$ = "Ridley"

Do
  Quit = False
  If FirstPass Then
    XSlu = FreeFile
    Open "C:\WB57\CRYSTAL\" + Pi$ + "List.txt" For Input As XSlu
    xsPath$ = "C:\WB57\CRYSTAL\"
    OUTlu = FreeFile
    OUTfile$ = xsPath$ + Pi$ + ".txt"
    Open OUTfile$ For Output As #OUTlu
    FirstPass = False
  End If
  If XSlu > 0 Then
    If EOF(XSlu) Then Exit Do
  End If
  If chkUse_ks.Value = 1 Then  ' read start and end ks
    Input #XSlu, Iyear, Imonth, Iday, UTks1, UTks2
    UTsec1 = UTks1 * 1000
    UTsec2 = UTks2 * 1000
  Else  'use hours and minutes
    Input #XSlu, Iyear, Imonth, Iday, Ihour1, Imin1, Ihour2, Imin2
  'Debug.Print ymd; StartHour; nFiles
    UTsec1 = Ihour1 * 3600# + Imin1 * 60
    UTsec2 = Ihour2 * 3600# + Imin2 * 60
  End If
  DateMTP = DateSerial(Iyear, Imonth, Iday)
  YYYYMMDD$ = Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
 
  REFfile$ = xsPath + "\" + YYYYMMDD$ + "\WB" + YYYYMMDD$ + ".REF"
  REFopen REFfile$
  REFreadHEADER REFfile$

  Record = 0: Count = 0: UT = 0: UTsec = 0
  Do   'Catch up to first record
    Record = Record + 1
    Call REFread(REFlu%, Record%)
  Loop Until UTsec >= UTsec1
  
  Count = 0
  If GoodScan And Zt1 < 18 Then
    Count = 1
    ZZp(1) = pALT: ZZg(1) = gALT: ZZt(1) = Zt1
  End If
  Do ' Now accumulate required parameters
    Record = Record + 1
    Call REFread(REFlu%, Record%)
    If GoodScan And Zt1 < 18 Then
      Count = Count + 1
      ZZp(Count) = pALT: ZZg(Count) = gALT: ZZt(Count) = Zt1
    End If
  Loop Until UTsec >= UTsec2 Or EOF(REFlu)
    
  ZpMin = 999#: ZpMax = -999#: ZpSum1 = 0#: ZpSum2 = 0#: ZgMin = 999#: ZgMax = -999#:
  ZgMZp = 0#: ZgSum1 = 0#: ZgSum2 = 0#: ZtAvg! = 0#: ZtSum1 = 0#: ZtSum2 = 0#
  For i = 1 To Count
    ZgMZp = ZgMZp + ZZg(i) - ZZp(i)
    If ZZg(i) < ZgMin Then ZgMin = ZZg(i)
    If ZZp(i) < ZpMin Then ZpMin = ZZp(i)
    If ZZg(i) > ZgMax Then ZgMax = ZZg(i)
    If ZZp(i) > ZpMax Then ZpMax = ZZp(i)
    ZpSum1 = ZpSum1 + ZZp(i)          'Sum time difference
    ZpSum2 = ZpSum2 + ZZp(i) ^ 2      'Sum square of time differences
    ZgSum1 = ZgSum1 + ZZg(i)          'Sum time difference
    ZgSum2 = ZgSum2 + ZZg(i) ^ 2      'Sum square of time differences
    ZtSum1 = ZtSum1 + ZZt(i)
    ZtSum2 = ZtSum2 + ZZt(i) ^ 2
  Next i
  ZgMZp = ZgMZp / Count         ' Average Zg - Zp
  ZtAvg = ZtSum1 / Count
  ZpAvg = ZpSum1 / Count        ' Calculate average Zp
  ZgAvg = ZgSum1 / Count        ' Calculate average Zg
'    If T2sum - DTavg ^ 2 / Count > 0 Then DTrms = Sqr((T2sum - DTavg ^ 2 * Count) / (Count - 1)) 'Calculate RMS
  If chkUse_ks.Value = 1 Then
    'Debug.Print yyyymmdd$; UTks1; UTks2; Count; ZgMZp; ZpAvg; ZpMin; ZpMax; ZgAvg; ZgMin; ZgMax; ZtAvg
    Print #OUTlu, YYYYMMDD$; UTks1; UTks2; Count; ZgMZp; ZpAvg; ZpMin; ZpMax; ZgAvg; ZgMin; ZgMax; ZtAvg
  Else
    'Debug.Print yyyymmdd$; Ihour1; Imin1; Count; ZgMZp; ZpAvg; ZpMin; ZpMax; ZgAvg; ZgMin; ZgMax; ZtAvg
    Print #OUTlu, YYYYMMDD$; Ihour1; Imin1; Count; ZgMZp; ZpAvg; ZpMin; ZpMax; ZgAvg; ZgMin; ZgMax; ZtAvg
  End If
  DoEvents
  Close REFlu
        
Loop Until EOF(XSlu)
Close XSlu, OUTlu

End Sub

Private Sub cmdOpenSource_Click()
Dim xx$, Filename$
' Set Initial directory
  CommonDialog1.InitDir = MNpath$ + "RAW\"
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  Filename$ = CommonDialog1.Filename

  If Filename$ = "" Then 'frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
    Exit Sub
  End If
  txtRAWfile.Text = Filename$
  OpenSource = True
  LocHor = Val(txtLocHor.Text)
  ReadRAWfile

End Sub


Private Sub cmdPt_Click(Index As Integer)
Dim i%, Measurements%, Ohms!, Counts!, C$, lu%, Filename$, A$, b$, n%, DandT$
Dim HD$

Select Case Index
Case 0  'Add
  lstOhms.AddItem txtOhms.Text
  lstCounts.AddItem txtCounts.Text
Case 1  'Remove
  lstOhms.RemoveItem (lstOhms.ListIndex)
  lstCounts.RemoveItem (lstCounts.ListIndex)
Case 2  'Clear
  lstOhms.Clear
  lstCounts.Clear
Case 3  'Default
  lstOhms.Clear
  lstCounts.Clear
  lu% = FreeFile
  Open txtPtFilename.Text For Input As #lu%
  Input #lu, Measurements
  Input #lu, HD$
  If HD$ = "H" Then chkHex.Value = 1
  For i = 1 To Measurements
    Input #lu, Ohms!, C$
    lstOhms.AddItem Trim(Str(Ohms))
    lstCounts.AddItem Trim(C$)
    If fNonNumeric(C$) Then chkHex.Value = 1
  Next i
  Close lu
  
Case 4  'Save list
  lu = FreeFile
  Open txtPtFilename.Text For Output As #lu
  Print #lu, lstOhms.ListCount
  If chkHex.Value Then Print #lu, "H" Else Print #lu, "D"
  For i = 0 To lstOhms.ListCount - 1
    Print #lu, lstOhms.List(i); lstCounts.List(i)
  Next i
  Close lu

Case 5 'Import
  lstOhms.Clear
  lstCounts.Clear
  ' Set Initial directory
  CommonDialog1.InitDir = SUpath$       '"C:\MTP\Setup\Pt\"
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
  Input #lu, Measurements
  Input #lu, HD$
  If HD$ = "H" Then chkHex.Value = 1 Else chkHex.Value = 0
  For i = 1 To Measurements
    Line Input #lu, A$   'Ohms!, Counts!
    n = InStr(1, A$, " ")
    lstOhms.AddItem Left$(A$, n - 1)
    lstCounts.AddItem Mid$(A$, n + 1, Len(A$) - n)
  Next i
  Close lu
  If chkHex.Value = 1 Then cmdPt_Click (6)
  
Case 6  'Convert Hex to Dec
  If chkHex.Value = 1 Then
  For i = 0 To lstCounts.ListCount - 1
    C$ = lstCounts.List(i)
    If fNonNumeric(C$) Then
      lstCounts.List(i) = Str(fHexToDec(Trim(lstCounts.List(i))))
    Else
      lstCounts.List(i) = Str(fHexToDec(Trim(Str(lstCounts.List(i)))))
    End If
  Next i
  End If
  chkHex.Value = 0
  
Case 7 'Append fit coefficients to Pt file
  lu% = FreeFile
  Open txtPtFilename.Text For Append As #lu%
  Print #lu, ""
  DandT$ = Date$ + " " + Time$
  Print #lu, DandT$
  Print #lu, "R(Counts) =  c(0) + c(1)*Counts"
  Print #lu, "c(0) = " + txtC0.Text
  Print #lu, "c(1) = " + txtC1.Text
  Print #lu, "T(Counts) =  c(0) + c(1)*Counts + c(1)*Counts^2 + c(1)*Counts^3"
  Print #lu, "c(0) = " + txtCtoT(0).Text
  Print #lu, "c(1) = " + txtCtoT(1).Text
  Print #lu, "c(2) = " + txtCtoT(2).Text
  Print #lu, "c(3) = " + txtCtoT(3).Text
  Close lu
  
End Select
txtOhms.Text = ""
txtCounts.Text = ""
End Sub

Private Sub cmdPtFit_Click()
Dim nData%, A!(1 To 20), mA%, CoVar!(1 To 20, 1 To 20), ChiSq!, i%, k1!, k2!
Dim sig!(1 To 100), ListA%(1 To 20), nCVM%, mFit%, X!(1 To 20), y!(1 To 20)
Dim a1#, a2#, a3#, a4#, C#(0 To 3), V#


' Pt Resistor T calibration coefficients (revised 20041206 to 3rd order)
' see T_vs_R.xls for fit from -100 C to +45 C
'  a1 = -239.23: a2 = 0.4527: a3 = 0.00005
'  a1 = -239.439682818085
'  a2 = 0.449715517332818
'  a3 = 6.88160116346254E-05
'  a4 = -2.09702955159072E-08
  a1 = -239.529603289315
  a2 = 0.450385127204133
  a3 = 6.71883132179858E-05
  a4 = -1.96781369379622E-08
  For i = 0 To lstOhms.ListCount - 1
    X(i + 1) = lstOhms.List(i)
    If chkHex.Value = 1 Then
      y(i + 1) = Str(fHexToDec(lstCounts.List(i)))
    Else
      y(i + 1) = lstCounts.List(i)
    End If
  Next i
  nData = lstOhms.ListCount
  mA = 2
  nCVM = 2
  mFit = 2
  For i = 1 To nData: sig(i) = 1#: ListA(i) = i: Next i
' Fit Resistance as function of Counts
  Call LFIT(y!(), X!(), sig!(), nData%, A!(), mA%, ListA%(), mFit%, CoVar!(), nCVM%, ChiSq!)
'Debug.Print a(1), a(2)
  txtC0.Text = Format(A(1), "000.000")
  txtC1.Text = Format(A(2), "0.00000")

' Calculate temperature of platinum thermistor which has linear fit vs counts (v)
' R(v) = k1 + k2 * v
' For platinum thermistor, calibration curve is
' T(R) = a1 + a2*R + a3*R^2 + a4*R^3
' mjm ... 2004.03.01
' Substitute R(v) for R in expression for T(R) and collect terms
  k1 = A(1): k2 = A(2)
  C(0) = (a1 + a2 * k1 + a3 * k1 * k1 + a4 * k1 * k1 * k1)
  C(1) = (a2 * k2 + 2 * a3 * k1 * k2 + 3 * a4 * k1 * k1 * k2)
  C(2) = (a3 * k2 ^ 2 + 3 * a4 * k1 * k2 * k2)
  C(3) = (a4 * k2 * k2 * k2)
  'Debug.Print txtPtFilename.Text; " "; C(0); C(1); C(2)
' c0, c1, and c2 are terms needed in Thermistor_ER_1 and _2 routines for Thi, Tlo and Twin
  txtCtoT(0) = Format(C(0), "#000.0000")
  txtCtoT(1) = Format(C(1), "#0.000000")
  txtCtoT(2) = Format(C(2), "0.000E+0")
  txtCtoT(3) = Format(C(3), "0.000E+0")

  V = Val(txtCts.Text)
  txtTest = Format(C(0) + C(1) * V + C(2) * V ^ 2 + C(3) * V ^ 3, "#00.00")

End Sub

Private Sub cmdReadMP_Click()
Dim MPlu%, OUTlu%, Record%, i%

  MPlu = FreeFile
  Open "C:\MTP\Data\WB57\CRAVE\MP\MP20060201.WB57" For Input As MPlu
  OUTlu = FreeFile
  Open "C:\MTP\Data\WB57\CRAVE\MP\MP20060201.txt" For Output As OUTlu
  Record = 0

  Do
    Call MPreadNew(MPlu%, Record%)
    If GoodScan Then
      i = 20
      Do
        i = i - 1
      Loop Until Abs(pALT - sZZZ(i)) < 0.04
      If Record > 0 Then Print #OUTlu, UTsec / 1000#; pALT; Zt1; OATmtp; sTTT(i); sZZZ(i)
    End If
    Record = Record + 1
  Loop Until EOF(MPlu)
  Close OUTlu, MPlu

End Sub

Private Sub cmdReadMS_Click()
Dim INPly%, OUTlu%, i%, j%, CAPS!(1 To 28), UT&, UTks!, A$, Sum!
' Read MS file with CAPS info.

  INPlu = FreeFile
  Open "C:\MTP\Data\WB57\CRAVE\MS\MS20060201.WB57" For Input As INPlu
  OUTlu = FreeFile
  Open "C:\MTP\Data\WB57\CRAVE\MS\MS20060201.txt" For Output As OUTlu
  ' Skip header info
  For i = 1 To 53
    Input #INPlu, A$
    Debug.Print i; A$
  Next i
  
  Do
    Input #INPlu, UT
    UTks = UT / 1000#
    Sum = 0#
    For i = 1 To 28
      Input #INPlu, CAPS(i)
      If i < 3 Then Sum = Sum + CAPS(i)
    Next i
    Print #OUTlu, UTks; vbTab; Sum
    Debug.Print UTks; Sum
  Loop Until EOF(INPlu)
  Close INPlu
  Close OUTlu

End Sub

Private Sub cmdReadXS_Click(Index As Integer)
Dim RAWfile$, INPlu%, OUTlu%, OUTfile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim i%, j%, L%, XSfile$(1 To 4), UTks!, Levels%, REFlu%
Dim PV!(1 To 100), T!(1 To 100), Zg!(1 To 100), Zp!(1 To 100), Wz!(1 To 100), Wm!(1 To 100)
Dim PV2!(1 To 100), T2!(1 To 100), Zg2!(1 To 100), Zp2!(1 To 100), Wz2!(1 To 100), Wm2!(1 To 100)
Dim PV3!(1 To 100), T3!(1 To 100), Zg3!(1 To 100), Zp3!(1 To 100), Wz3!(1 To 100), Wm3!(1 To 100)
Dim PV4!(1 To 100), T4!(1 To 100), Zg4!(1 To 100), Zp4!(1 To 100), Wz4!(1 To 100), Wm4!(1 To 100)
Dim Txs!(0 To 10, 1 To 2000), Txs1!, Txs2!, Txs3!, Txs4!, Tnv!(0 To 10, 1 To 2000)
Dim Count%, Tlast!, Tinterp!, UTlast&, dT!
Dim dTmin!, dTmax!, ZtMax!, ZpMin!, ZtOffset!, FileSrc$, UTsec1&, UTsec2&, UTsec3&, UTsec4&
Dim Filename$, xsPath$, yyyy$, mm$, DD$, UTsave&, UT&, UTHH%, UTHH2%, UTHH3%, UTHH4%
Dim DandT(1 To 4)  As Variant, FirstPass As Boolean, DandTmtp As Date, DateMTP As Date
Dim XSlu%, StartHour%, Nfiles%, ymd&, Hinc%, z!, ZpMTP!(1 To 33), ZgMTP!(1 To 33), Tmtp!(1 To 33)
Dim Iyear%, Imonth%, Iday%, UTks1!, UTks2!, Zavg!(0 To 10), nAltitudes%, jStart%, Ext$
Dim T1Sum!, T2Sum!

  ZtOffset = Val(txtZtOffset.Text)
  ZtMax = Val(txtZtMax.Text)
  ZpMin = Val(txtZpMin.Text)
  Nfiles = Val(txtNfiles.Text)
  FileSrc$ = Trim(cboFileSrc.Text)
  StartHour = Val(txtUTstart.Text)
  
  Select Case FileSrc$
  Case "DAO", "REAN": Hinc = 6
  Case "NCEP": Hinc = 12
  Case "UKMO": Hinc = 24
  End Select
  
  FirstPass = True
  If Index < 3 Then     'Do flight level comparison
    nAltitudes = 0
    jStart = 0
    chkUseZg.Value = 0  'Force to use pressure altitude
  Else                  'Do comparison at specific altitudes
    nAltitudes = 3
    jStart = 1
    For i = 1 To nAltitudes
      Zavg(i) = 10 + (i - 1) * 1  '18 to 23 kms
    Next i
  End If
  
Do
  Quit = False
  If Index = 0 Then  'Only 1 file to process
    Iyear = Left$(YYYYMMDD$, 4)
    Imonth = Mid$(YYYYMMDD$, 5, 2)
    Iday = Right$(YYYYMMDD$, 2)
    DandT(1) = DateSerial(Iyear, Imonth, Iday)
    Nfiles = 1
  Else               'Several files with temporal interpolation
    If FirstPass Then
      If chkUseUTwindow.Value = 1 Then Ext$ = "avg.txt" Else Ext$ = ".txt"
      XSlu = FreeFile
      Open "C:\DC8\SOLVE2\Leslie\" + FileSrc$ + Ext$ For Input As XSlu
      FirstPass = False
    End If
    If XSlu > 0 Then
      If EOF(XSlu) Then Exit Do
    End If
    Input #XSlu, ymd, StartHour, Nfiles, UTks1, UTks2
    'Debug.Print ymd; StartHour; nFiles
    YYYYMMDD$ = Trim(Str(ymd))
    Iyear = Left$(YYYYMMDD$, 4)
    Imonth = Mid$(YYYYMMDD$, 5, 2)
    Iday = Right$(YYYYMMDD$, 2)
    DandT(1) = DateSerial(Iyear, Imonth, Iday) + TimeSerial(StartHour, 0, 0)
    If Nfiles > 1 Then DandT(2) = DandT(1) + TimeSerial(Hinc, 0, 0)
    If Nfiles > 2 Then DandT(3) = DandT(1) + TimeSerial(2 * Hinc, 0, 0)
    If Nfiles > 3 Then DandT(4) = DandT(1) + TimeSerial(3 * Hinc, 0, 0)
  End If
  DateMTP = DateSerial(Iyear, Imonth, Iday)
  UTHH = StartHour
  UTsec1 = UTHH * 3600#
  UTsec2 = UTsec1 + Hinc * 3600#
  UTsec3 = UTsec2 + Hinc * 3600#
  UTsec4 = UTsec3 + Hinc * 3600#
  xsPath$ = Drive$ + Rdir2$ + Mission$ + "\XS\"
  OUTlu = FreeFile
  OUTfile$ = xsPath$ + "XS" + YYYYMMDD$ + ".txt"
  Open OUTfile$ For Output As #OUTlu
  
  REFfile$ = Drive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
  REFopen REFfile$
  REFreadHEADER REFfile$

' Construct filenames
  Select Case Index
  Case 0                 ' Read one XS File without temporal interpolation
    XSfile$(1) = xsPath$ + "XS" + YYYYMMDD$ + ".DC8"
  Case 1, 2, 3             ' Read one XS File with temporal interpolation
    xsPath$ = Drive$ + Rdir2$ + Mission$ + "\Leslie\XS" + YYYYMMDD$
'   XS20030112__DAO2003-01-12T06.DC8   DAO  every 06 hours
    For i = 1 To Nfiles
      Iyear = Year(DandT(i)): Imonth = Month(DandT(i)): Iday = Day(DandT(i)): Ihour = Hour(DandT(i))
      Filename$ = "__" + FileSrc$ + Format(Iyear, "0000") + "-" + Format(Imonth, "00") + "-" + Format(Iday, "00") + "T" + Format(Ihour, "00") + ".DC8"
      XSfile$(i) = xsPath$ + Filename$
    Next i
  End Select
  
  Record = 0: Count = 0: UT = 0: UTsec = 0
  Do
CatchUp:
    Count = Count + 1
    Call GetNextXS1(0, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$(1), Quit)
    If Nfiles > 1 Then
      UTsave = UT
      Call GetNextXS2(0, UTsec&, UT&, Latitude!, Longitude!, PV2!(), T2!(), Zg2!(), Zp2!(), Wz2!(), Wm2!(), Levels%, XSfile$(2), Quit)
    End If
    If Nfiles > 2 Then
      UT = UTsave
      Call GetNextXS3(0, UTsec&, UT&, Latitude!, Longitude!, PV3!(), T3!(), Zg3!(), Zp3!(), Wz3!(), Wm3!(), Levels%, XSfile$(3), Quit)
    End If
    If Nfiles > 3 Then
      UT = UTsave
      Call GetNextXS4(0, UTsec&, UT&, Latitude!, Longitude!, PV4!(), T4!(), Zg4!(), Zp4!(), Wz4!(), Wm3!(), Levels%, XSfile$(4), Quit)
    End If
    If Quit Then Exit Do
    If chkUseUTwindow.Value = 1 Then
      If UT < UTks1 * 1000 Then
        Count = Count - 1
        GoTo CatchUp
      Else
        If UT > UTks2 * 1000 Then Exit Do
      End If
    End If
    
    If Not Quit Then
      Do
        Record = Record + 1
        Call REFread(REFlu%, Record%)
        Ihour = Int(UTsec / 3600#)
        Imin = Int((UTsec - 3600# * Ihour) / 60#)
        Isec = Int(UTsec - 3600# * Ihour - 60# * Imin)
        DandTmtp = DateMTP + TimeSerial(Ihour, Imin, Isec)
        If Record = 1 Then
        ' If XS file starts before REF file, then move ahead in XS file
        ' to UT of REF file
          If UTsec > UT Then
            UTsave = UT
            Do
              Call GetNextXS1(0, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$(1), Quit)
            Loop Until UT >= UTsec
            If Nfiles > 1 Then
              UT = UTsave
              Do
                Call GetNextXS2(0, UTsec&, UT&, Latitude!, Longitude!, PV2!(), T2!(), Zg2!(), Zp2!(), Wz2!(), Wm2!(), Levels%, XSfile$(2), Quit)
              Loop Until UT >= UTsec
            End If
            If Nfiles > 2 Then
              UT = UTsave
              Do
                Call GetNextXS3(0, UTsec&, UT&, Latitude!, Longitude!, PV3!(), T3!(), Zg3!(), Zp3!(), Wz3!(), Wm3!(), Levels%, XSfile$(3), Quit)
              Loop Until UT >= UTsec
            End If
            If Nfiles > 3 Then
              UT = UTsave
              Do
                Call GetNextXS4(0, UTsec&, UT&, Latitude!, Longitude!, PV4!(), T4!(), Zg4!(), Zp4!(), Wz4!(), Wm4!(), Levels%, XSfile$(4), Quit)
              Loop Until UT >= UTsec
            End If
          End If
        End If
      Loop Until (UTsec >= UT And GoodScan) Or EOF(REFlu)
      
      For i = 1 To Nlev
        ZpMTP(i) = sZZZ(Nlev1 + i - 1)
        ZgMTP(i) = sZg(Nlev1 + i - 1)
        Tmtp(i) = sTTT(Nlev1 + i - 1)
      Next i
       
      For j = jStart To nAltitudes
        If pALT > ZpMin And Zt1 < ZtMax Then
          If chkZtOffset.Value = 1 Then
            If Abs(pALT - Zt1) < ZtOffset Then
              Count = Count - 1: Exit For
            End If
          End If
        Else
          Count = Count - 1: Exit For
        End If
        If nAltitudes = 0 Then z = pALT: Zavg(0) = z Else z = Zavg(j)
      ' Interpolate XS to flight level
        If chkUseZg.Value = 1 Then
          Txs1 = fLinterp(Zg(), T(), Levels, z)
          If Nfiles > 1 Then Txs2 = fLinterp(Zg2(), T2(), Levels, z)
          If Nfiles > 2 Then Txs3 = fLinterp(Zg3(), T3(), Levels, z)
          If Nfiles > 3 Then Txs4 = fLinterp(Zg4(), T4(), Levels, z)
          Tnv(j, Count) = fLinterp(ZgMTP(), Tmtp(), Nlev, z)
        Else
          Txs1 = fLinterp(Zp(), T(), Levels, z)
          If Nfiles > 1 Then Txs2 = fLinterp(Zp2(), T2(), Levels, z)
          If Nfiles > 2 Then Txs3 = fLinterp(Zp3(), T3(), Levels, z)
          If Nfiles > 3 Then Txs4 = fLinterp(Zp4(), T4(), Levels, z)
          If nAltitudes = 0 Then
            Tnv(j, Count) = OATnav + OATnavCOR
          Else
            Tnv(j, Count) = fLinterp(ZpMTP(), Tmtp(), Nlev, z)
          End If
        End If
       
        If Index = 0 Then
          Txs(j, Count) = Txs1
        Else
          Select Case DandTmtp
          Case Is < DandT(2)
            Txs(j, Count) = Txs1 + (Txs2 - Txs1) * (1 - (UTsec2 - UTsec) / (UTsec2 - UTsec1))
          Case Is < DandT(3)
            Txs(j, Count) = Txs2 + (Txs3 - Txs2) * (1 - (UTsec3 - UTsec) / (UTsec3 - UTsec2))
          Case Is < DandT(4)
            Txs(j, Count) = Txs3 + (Txs4 - Txs3) * (1 - (UTsec4 - UTsec) / (UTsec4 - UTsec3))
          End Select
        End If
        dT = Txs(j, Count) - Tnv(j, Count)
        
        Print #OUTlu, Format(UTsec / 1000#, "#00.000") + vbTab + Format(Txs(j, Count), "000.0") + vbTab + Format(Tnv(j, Count), "000.0") + vbTab + Format(dT, "00.00") + vbTab + Format(z, "00.00") + vbTab + Format(Zt1, "00.00")
      Next j
      DoEvents
    End If
  Loop Until Quit Or EOF(REFlu)
  Close OUTlu, REFlu
        
  'Debug.Print FileSrc$; " "; yyyymmdd$; Count;
  For j = jStart To nAltitudes
    dTmin = 999#: dTmax = -999#: T1Sum = 0#: T2Sum = 0#
    For i = 1 To Count
      dT = Txs(j, i) - Tnv(j, i)
      If dT < dTmin Then dTmin = dT
      If dT > dTmax Then dTmax = dT
      T1Sum = T1Sum + dT         'Sum time difference
      T2Sum = T2Sum + dT ^ 2     'Sum square of time differences
    Next i
    DTavg = T1Sum / Count        'Calculate average
    If T2Sum - DTavg ^ 2 / Count > 0 Then DTrms = Sqr((T2Sum - DTavg ^ 2 * Count) / (Count - 1)) 'Calculate RMS
    'Debug.Print " "; Zavg(j); DTavg; DTrms; dTmin; dTmax;
  Next j
  Debug.Print ""

' Close XS files as EOF not reached when in UT window mode
  Call GetNextXS1(3, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$(1), Quit)
  If Nfiles > 1 Then Call GetNextXS2(3, UTsec&, UT&, Latitude!, Longitude!, PV2!(), T2!(), Zg2!(), Zp2!(), Wz2!(), Wm2!(), Levels%, XSfile$(2), Quit)
  If Nfiles > 2 Then Call GetNextXS3(3, UTsec&, UT&, Latitude!, Longitude!, PV3!(), T3!(), Zg3!(), Zp3!(), Wz3!(), Wm3!(), Levels%, XSfile$(3), Quit)
  If Nfiles > 3 Then Call GetNextXS4(3, UTsec&, UT&, Latitude!, Longitude!, PV4!(), T4!(), Zg4!(), Zp4!(), Wz4!(), Wm3!(), Levels%, XSfile$(4), Quit)
Loop Until Index < 2
  If Index = 2 Then Close XSlu

End Sub

Private Sub cmdSubstitute_Click()
Dim i%, FltDate$, FltDateSave$
' Substitute P and T values from a PI instrument file for the default NAV values
' RAW file has extension changed to RMM (MMS), RPT (NOAA PT), etc to avoid confusion
' Initially we read MMS or PT values when needed when the data was processed
' I found having separate files less confusing.
  
  If Trim(cboPTfile.Text) = "NO" Then
    MsgBox "You must select a Input Nav File other than NO", vbOKOnly
    Exit Sub
  End If
  
  If chkSubstituteAll.Value = 0 Then
    FltDate$ = YYYYMMDD$
    If cboPTfile.Text = "NG" Then
      Substitute_NGV YYYYMMDD$          'CAN instruments only
    Else
      Substitute_XXX YYYYMMDD$          'All other instruments
    End If
  Else
    FltDateSave = YYYYMMDD$
    For i = 0 To lstFltDates.ListCount - 1
      YYYYMMDD$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i         'Select current flight date
      txtFltDates.Text = YYYYMMDD$
      If cboPTfile.Text = "NG" Then
        Substitute_NGV YYYYMMDD$        'CAN instruments only
      Else
        Substitute_XXX YYYYMMDD$        'All other instruments
      End If
      DoEvents
    Next i
    YYYYMMDD$ = FltDateSave$
  End If
  cboPTfile.ListIndex = 0 'Set back to "NO" = None

  
End Sub

Private Sub cmdTestGH_Click()
Dim Record%, lu%, Prefix$

  lu = FreeFile
'  Open "C:\DC8\SOLVE2\MP\MP20030109.DC8" For Input As lu
  Open "C:\DC8\SOLVE2\XS\XS20030109.DC8" For Input As lu
  
  Record = 0
  Prefix$ = "XS"
  Call Read_Gaines_Hipskind_Format(lu%, Record%, Prefix$)

  Do
    Record = Record + 1
    Call Read_Gaines_Hipskind_Format(lu%, Record%, Prefix$)
    'Debug.Print Record; UTsec; Latitude; Longitude
  Loop Until Record = 200
  Close lu

End Sub


Private Sub cmdText_Click()
Dim A$, b$, Length%, i%, j%, iCR%, iStart%, lu#, iOption%, Ext$
Dim TArms!(1 To 3, 1 To 10), iSpace%, P$(1 To 1000), n%
Dim OBrms1!(1 To 30)

  i = -1
  Do:    i = i + 1
    If i > optTextBox.UBound Then
      MsgBox "You must select an output file type option!", vbOKOnly
      Exit Sub
    End If
  Loop Until optTextBox(i).Value = True
  
  iOption% = i
  
  Select Case iOption
  Case 0   'Save Text Box contents
    Ext$ = ".FLA"
    A$ = txtTextBox.Text
    lstTextBox.Clear
    Length = Len(A$)
    iStart = 1
    Do
      iCR = InStr(iStart, A$, vbCr, vbBinaryCompare)
      If iCR <> 0 Then
        b$ = Mid$(A$, iStart, iCR - iStart + 1)
        b$ = fStripAlphaNumeric(b$)
        lstTextBox.AddItem b$
        iStart = iCR + 1
      Else  'Check for last item in case no CR
        If iStart < Len(A$) Then
          b$ = Mid$(A$, iStart, Len(A$) - iStart + 1)
          b$ = fStripAlphaNumeric(b$)
          lstTextBox.AddItem b$
        End If
      End If
    Loop Until iCR = 0

  Case 1
    Ext$ = ".RMS"
    A$ = txtTextBox.Text
    lstTextBox.Clear
    Length = Len(A$)
    Call ParseString(A$, n%, P$())
    For i = 1 To n
      lstTextBox.AddItem P$(i)
    Next i
    j = 0
    For i = 1 To n Step Channels + 1
      j = j + 1
      TArms(1, j) = Val(P$(i + 1))
      TArms(2, j) = Val(P$(i + 2))
      If Channels > 2 Then TArms(3, j) = Val(P$(i + 3))
    Next i
    
    Call MapTArmsToOBrms(TArms!(), OBrms1!(), Channels%, Nel%, LocHor%, ChInfo!())
    lstTextBox.Clear
    For i = 1 To (Nel - 1) * Channels + 1
      lstTextBox.AddItem OBrms1(i)
    Next i
  
  Case 2    'Write WCT
    Ext$ = ".WCT"
    A$ = txtTextBox.Text
    lstTextBox.Clear
    Length = Len(A$)
    iStart = 1
    Do
      iCR = InStr(iStart, A$, vbCrLf, vbBinaryCompare)
      If iCR <> 0 Then
        b$ = Mid$(A$, iStart, iCR - iStart)
'        b$ = fStripAlphaNumeric(b$)
        lstTextBox.AddItem b$
        iStart = iCR + 2
      Else  'Check for last item in case no CR
        If iStart < Len(A$) Then
          b$ = Mid$(A$, iStart, Len(A$) - iStart + 1)
'          b$ = fStripAlphaNumeric(b$)
          lstTextBox.AddItem b$
        End If
      End If
    Loop Until iCR = 0
  
  Case 3  'Extract numbers in text box to NBR file
    Ext$ = ".NBR"
    A$ = txtTextBox.Text
    lstTextBox.Clear
    Length = Len(A$)
    Call ParseString(A$, n%, P$())
    For i = 1 To n
      lstTextBox.AddItem P$(i)
    Next i

  
  End Select
  lu = FreeFile
  Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + Ext$ For Output As #lu
   
  For i = 1 To lstTextBox.ListCount
    Print #lu, lstTextBox.List(i - 1)
  Next i
  Close lu
  
End Sub

Private Sub cmdTr_Click()
  txtT.Text = Format(fTptR(Val(txtR.Text)), "#00.00")
End Sub

Private Sub cmdUCSE_Click()
Dim RAWfile$, INPlu%, OUTlu%, OUTfile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim i%, j%, L%, AA$, Bb$, cC$, DD$, EE$, UT&, P!, T!, Th!, U!, V!, W!, PTWfile$, UTks!
Dim GoToNext As Boolean, pALTpt!, OATpt!, GA!, PText$, b$, SubPath$, ALTm!
Dim mPitch!, mRoll!, mLatitude!, mLongitude!, mZg!, mZp!, mWspd!, mWdir!, mHeading!

  If Trim(cboPTfile.Text) = "NO" Then
    MsgBox "You must select a Input Nav File other than NO", vbOKOnly
    Exit Sub
  End If
  
'  Initialize   ' Was InitializeDC and ER!!!!!!!!!!!!
'  Initialize_RCs_Calfile
  BadCycle = 0
  Cycle = 1
  RAWfile$ = Path$ + AC$ + YYYYMMDD$ + ".RAW"  '".du" '
  OUTfile$ = Path$ + AC$ + YYYYMMDD$ + ".TXT"   '"pt.TXT"
  PTWfile$ = Path$ + Trim(cboPTfile.Text) + YYYYMMDD$ + ".WB57"
  PText$ = fACext(AC$)
  
  SubPath$ = MNpath$ + Trim(cboPTfile.Text) + "\"
  
  Select Case Trim(cboPTfile.Text)
  Case "MM": PTWfile$ = SubPath$ + "MM" + YYYYMMDD$ + PText$ 'MMS OAT calibration file
  Case "PT": PTWfile$ = SubPath$ + "PT" + YYYYMMDD$ + PText$ 'PTW OAT calibration file
  Case "FT": PTWfile$ = SubPath$ + "FT" + YYYYMMDD$ + PText$ 'NAV OAT calibration file
  Case "IN": PTWfile$ = SubPath$ + "IN" + YYYYMMDD$ + PText$ 'INS
  Case "NC": PTWfile$ = SubPath$ + "NC" + YYYYMMDD$ + PText$ 'NCAR nav data
  Case "UC": PTWfile$ = SubPath$ + "UC" + YYYYMMDD$ + PText$ 'UCSE nav data
  End Select
  
  INPlu% = FreeFile
  Open RAWfile$ For Input As INPlu%
  OUTlu% = FreeFile
  Open OUTfile$ For Output As OUTlu%
  LOFraw = LOF(INPlu%)
  ProgressBar1.Min = 0
  ProgressBar1.Max = 100
  
  Record = 0
  FirstAline = True
' Open external PT data file if needed

' Synchronize all files (assume that RAW file begins before PT files
  Do
NextAline:
    Line Input #INPlu, A$
    ByteCount = Loc(INPlu%) * 128
'    If ByteCount < LOFraw Then frmMTPbin.ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'    txtBadCounts.Text = Str(BadCycle)  'Bad Cycles
'    txtFileRecords.Text = Str(Cycle)   'Total Cycles
    '    Debug.Print a$
    If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then
      Call DecodeAline(A$, GoToNext)    'Get UTsec from RAW file
'      UTsecMTP = fTstringToSec(hhmmssMTP$, False)
      
      Select Case Trim(cboPTfile.Text)
      Case "MM": Call GetMMSvalues(UTsec, UT, P, T, Th, U, V, W, PTWfile$, Quit)
      Case "PT": Call GetPTWvalues(UTsec&, UT, P!, T!, PTWfile$)
      Case "FT": Call GetFTvalues(UTsec&, UT, P!, T!, mLatitude, mLongitude, mZg!, mZp!, mWspd!, mWdir!, PTWfile$)
      Case "NC": Call GetNCvalues(UTsec&, UT, P!, T!, PTWfile$, Quit)
      Case "IC": Call GetICATSvalues(UTsec&, UT&, P!, T!, GA!, PTWfile$, Quit)
'                     GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, UCSEfile$, Quit As Boolean)
      Case "UC": Call GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, PTWfile$, Quit)
      End Select
      If UT > UTsec Then  'MMS/PTW ahead of RAW
        Do
          Line Input #INPlu, A$
          If Left$(A$, 1) = "A" And Mid$(A$, 3, 2) <> "99" Then Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
        Loop Until UTsec >= UT
        If UTsec > UT Then  'Make equal
          Select Case Trim(cboPTfile.Text)
          Case "MM": Call GetMMSvalues(UTsec, UT, P, T, Th, U, V, W, PTWfile$, Quit)
          Case "PT": Call GetPTWvalues(UTsec&, UT, P!, T!, PTWfile$)
          Case "FT": Call GetFTvalues(UTsec&, UT, P!, T!, mLatitude, mLongitude, mZg!, mZp!, mWspd!, mWdir!, PTWfile$)
          Case "NC": Call GetNCvalues(UTsec&, UT, P!, T!, PTWfile$, Quit)
          Case "IC": Call GetICATSvalues(UTsec&, UT&, P!, T!, GA!, PTWfile$, Quit)
          Case "UC": Call GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, PTWfile$, Quit)
          End Select
        End If
      End If
    Else
      GoTo NextAline
    End If
  Loop Until UT <= UTsec

'UTksStart = UTsec
' Skip one RAW cycle because current A-line is lost for MAIN processing when
' re-synched in INITIALIZE because MMS file is not closed and re-opened
'  Do  'Go to next A-line
'    Line Input #INPlu, a$
'  Loop Until Left$(a$, 1) = "A" 'And Mid$(a$, 3, 2) <> "99"
' xxx mjm 20001024, removed above as loose 1 cycle each time thru
ProgressBar1.Min = 0
ProgressBar1.Max = 100
Do
  If EOF(INPlu) Then Exit Do
  
  Do
  If Not FirstAline Then Input #INPlu, A$ Else FirstAline = False
    ByteCount = Loc(INPlu%) * 128
'    If ByteCount < LOFraw Then frmMTPbin.ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'    txtBadCounts.Text = Str(BadCycle)  'Bad Cycles
'    txtFileRecords.Text = Str(Cycle)   'Total Cycles
  Select Case Left$(A$, 1)
  Case "A"
    AA$ = A$
    Call DecodeAline(A$, GoToNext)   'Get UTsec from RAW file
    UTks = UTsec / 1000
    Select Case Trim(cboPTfile.Text)
    Case "MM": Call GetMMSvalues(UTsec, UT, P, T, Th, U, V, W, PTWfile$, Quit)
    Case "PT": Call GetPTWvalues(UTsec&, UT, P!, T!, PTWfile$)
    Case "FT": Call GetFTvalues(UTsec&, UT, P!, T!, mLatitude, mLongitude, mZg!, mZp!, mWspd!, mWdir!, PTWfile$)
    Case "NC": Call GetNCvalues(UTsec&, UT, P!, T!, PTWfile$, Quit)
    Case "IC": Call GetICATSvalues(UTsec&, UT&, P!, T!, GA!, PTWfile$, Quit)
    Case "UC": Call GetUCSEvalues(UTsec&, UT&, P!, T!, mPitch!, mRoll!, mLatitude!, mLongitude!, mHeading!, mZg!, mZp!, mWspd!, mWdir!, PTWfile$, Quit)
    End Select
    Aline = True: Bline = False: Cline = False: Dline = False: Eline = False
    Debug.Print UTks; Pitch; mPitch; Roll; mRoll
    Print #OUTlu, UTks; Pitch; mPitch; Roll; mRoll
  
  Case "B"                           '  C(i,j)
    Bb$ = A$
    For j = 1 To 5
      For i = 1 To Channels
        C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 1), 6))
      Next i
    Next j
    Bline% = True

  Case "C"                          '  C(i,j)
    cC$ = A$
    For j = 6 To 10
      For i = 1 To Channels
        C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 6), 6))
      Next i
    Next j
    Cline% = True

  Case "D"                           '  MUX()
    DD$ = A$
    Dline = True
    
  Case "E"
    EE$ = A$
    For j = 11 To 12
      For i = 1 To Channels
        C(i, j) = Val(Mid$(A$, 2 + 6 * (i - 1) + 6 * Channels * (j - 11), 6))
      Next i
    Next j
  ' Define Base, Sky and Noise Diode counts (Used in a number of places)
  
    For i = 1 To Channels
      CS(i, LocHor) = C(i, LocHor): CB(i) = C(i, 12): dND(i) = C(i, 11) - CB(i)
    Next i
'''    CtsQual = fCheckCountsQuality(Cycle, False)
    If Not CtsQual Then BadCycle = BadCycle + 1
    Cycle = Cycle + 1
    Eline% = True
  End Select
  Loop Until Aline * Bline * Cline * Dline * Eline = True Or EOF(INPlu)
  DoEvents
Loop Until EOF(INPlu)

Close INPlu, OUTlu
Exit Sub
'If EOF(INPlu) And Aline * Bline * Cline * Dline * Eline = False Then Exit Do
Aline = False
'----------------------
' Fix Aline with P and T values
    pALTpt = fPtoZ(P)
    If pALTpt < 0 Then pALTpt = 0
    ALTm = Int(1000 * pALTpt + 0.5)
    OATpt = T - cTo
    Select Case AC$
    Case "DC"
' Sub geometric altitude for first 3 CAMEX-4 flights
'AD227 17:52:55  31.291 -103.406  100   1.6   0.8 00000 36917 -50.3 0017   0313   10.8 1 010815 175213
'123456789012345678901234567890123456789012345678901234567890
      Mid$(AA$, 50, 5) = Format(GA, "00000")
' ER2 Format
'          1         2         3         4         5         6         7         8         9         10
' 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc yymmdd hhmmss
'   doy hh mm ss snn.lat snnn.lon   hdg ppp.p rr.rr .zgeo  pALT   OAT  Wspd  Wdir Scndispc Sta yymmdd hhmmss
' AG266:22:02:32  -3.051 -159.368  64.2    .6   3.7 20479 20178 -65.9    .9   3.2 -188.9 1 255 970923 220222
    Case "ER"
      b$ = Format(ALTm, "####0")
      Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
      Mid$(AA$, 62, 6) = " " + formats(OATpt, "00.0")
    Case "WB"
      b$ = Format(ALTm, "####0")
      Mid$(AA$, 56, 6) = Space(6 - Len(b$)) + b$
      Mid$(AA$, 62, 6) = " " + formats(OATpt, "00.0")
    End Select
    
'This line used to flag bad scans in ACCENT data
'If Not CtsQual Then Mid$(AA$, 2, 1) = "!" Else Mid$(AA$, 2, 1) = "W"

    Print #OUTlu, AA$
    Print #OUTlu, Bb$
    Print #OUTlu, cC$
    Print #OUTlu, DD$
   Print #OUTlu, EE$
    Print #OUTlu, ""
'X$ = Format(UTsec / 1000, "00.000") + vbTab + Format(pALT, "00.00") + vbTab
'X$ = X$ + Format(pALTpt, "00.00") + vbTab + Format(OATn + cTo, "#00.0") + vbTab + Format(OATpt + cTo, "#00.0")
'Print #OUTlu, X$
  ByteCount = Loc(INPlu%) * 128
'  If ByteCount < LOFraw Then frmMTPbin.ProgressBar1.Value = Int(ByteCount * 100 / LOFraw)
'  txtBadCounts.Text = Str(BadCycle)  'Bad Cycles
'  txtFileRecords.Text = Str(Cycle)   'Total Cycles
  DoEvents

Kill RAWfile$
Name OUTfile$ As RAWfile$

End Sub

Private Sub cmdUCSE_TDC_Click()
Dim TDClu%, UCSElu%, UCSEfile$, TDCfile$, ECMWFfile$, ECMWFlu%, Ttdc!, Tucse!, Ptdc!, Pucse!, OUTlu%
Dim param!(1 To 75), UT&, UTucse&, D$, Zu!, Zt!, Tecmwf!, A$, Pecmwf!
  
  D$ = "20051205"
  UTsec = 69199
  ECMWFfile$ = "C:\MTP\Data\M55\SCOUT-O3\EC\EC" + D$ + ".M55" 'ECMWF model data
  Call GetECMWFvalues(UTsec&, UT&, Pecmwf, Tecmwf, ECMWFfile$)

'  18970.00 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900 -9999.9900    87.3523   375.0460    88.9560   371.8893    83.9715   377.2776    85.1865   375.7736
'  18980.00    32.5575 16175.4268    -1.6006     0.1633    -0.0130    52.5569    38.7007     0.4828     0.0000     0.0000    87.3296   375.0640    88.9480   371.8841    83.9715   377.2702    85.1865   375.7667
  UCSEfile$ = "C:\MTP\Data\M55\SCOUT-O3\UC\UC" + D$ + ".M55" 'UCSE nav data
  TDCfile$ = "C:\MTP\Data\M55\SCOUT-O3\TD\TD" + D$ + ".M55"
  OUTlu% = FreeFile
  Open "C:\MTP\Data\M55\SCOUT-O3\TD\UCSE_TDC" + ".txt" For Output As OUTlu%
  Print #OUTlu, " UT(ks) Tecmwf Tucse Ttdc Zucse Ztdc Tucse-Ttdc Tecmwf-Ttdc Zucse-Ztdc"
  Do
    Do
      'UTsec = UTsec + 1
      Call GetECMWFvalues(-1, UT&, Pecmwf, Tecmwf, ECMWFfile$) 'Get next ECMWF record
      UTsec = UT
      Call GetNAVvalues("TD", UTsec&, UT&, Ptdc!, Ttdc!, param!(), TDCfile$, Quit)
    Loop Until UT >= UTsec
    If UT > UTsec Then UTsec = UT
    Call GetNAVvalues("UC", UTsec&, UTucse&, Pucse!, Tucse!, param!(), UCSEfile$, Quit)
    If UTucse = UTsec Then
      Zu = fPtoZ(Pucse)
      Zt = fPtoZ(Ptdc)
      Print #OUTlu, UTsec / 1000#; Tecmwf; Tucse; Ttdc; Zu; Zt; Tucse - Ttdc; Tecmwf - Ttdc; Zu - Zt
      'Debug.Print UTsec / 1000#; Tucse; Ttdc; fPtoZ(Pucse); fPtoZ(Ptdc)
    Else
      'Debug.Print UTsec / 1000#; Tucse; Ttdc; fPtoZ(Pucse); fPtoZ(Ptdc); "**"
    End If
    DoEvents
  Loop Until Quit
  Close  'Close all open files

End Sub

Private Sub cmdUTnavCor_Click()
Dim UTnavCor&, UTsecNav&, UTnav$, RAWfile$, OUTfile$, A$
' Correct Nav time
  RAWfile$ = Path$ + AC$ + YYYYMMDD$ + ".RAW"
  OUTfile$ = Path$ + AC$ + YYYYMMDD$ + ".TXT"
  INPlu% = FreeFile
  Open RAWfile$ For Input As INPlu%
  OUTlu% = FreeFile
  Open OUTfile$ For Output As OUTlu%
  LOFraw = LOF(INPlu%)
'  ProgressBar1.Min = 0
'  ProgressBar1.Max = 100
  UTnavCor = Val(txtUTnavCor.Text)
  Do
    Input #INPlu, A$
    If Left$(A$, 1) = "A" Then
      Doy = Val(Mid$(A$, 3, 3))
      UTsecNav = fTstringToSec(Mid$(A$, 7, 8), True)      'Nav has colons
      UTsecNav = UTsecNav + UTnavCor
      UTnav$ = fSecToTstring(UTsecNav, True)
      Mid$(A$, 7, 8) = UTnav$
    End If
    Print #OUTlu, A$
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
  Kill RAWfile$
  Name OUTfile$ As RAWfile$
End Sub

Private Sub cmdXSavg_Click()
Dim RAWfile$, INPlu%, OUTlu%, OUTfile$, A$, y%, M%, D%, Doy%, ByteCount&, LOFraw&
Dim i%, j%, k%, L%, XSfile$(1 To 4), UTks!, Levels%, REFlu%, T1Sum!, T2Sum!, dT!
Dim PV!(1 To 100), T!(1 To 100), Zg!(1 To 100), Zp!(1 To 100), Wz!(1 To 100), Wm!(1 To 100)
Dim PV2!(1 To 100), T2!(1 To 100), Zg2!(1 To 100), Zp2!(1 To 100), Wz2!(1 To 100), Wm2!(1 To 100)
Dim PV3!(1 To 100), T3!(1 To 100), Zg3!(1 To 100), Zp3!(1 To 100), Wz3!(1 To 100), Wm3!(1 To 100)
Dim PV4!(1 To 100), T4!(1 To 100), Zg4!(1 To 100), Zp4!(1 To 100), Wz4!(1 To 100), Wm4!(1 To 100)
Dim Txs!(0 To 10, 1 To 2000), Txs1!, Txs2!, Txs3!, Txs4!, Tnv!(0 To 10, 1 To 2000)
Dim Count%, Tlast!, Tinterp!, UTlast&
Dim dTmin!, dTmax!, ZtMax!, ZpMin!, ZtOffset!, FileSrc$, UTsec1&, UTsec2&, UTsec3&, UTsec4&
Dim Filename$, xsPath$, yyyy$, mm$, DD$, UTsave&, UT&, UTHH%, UTHH2%, UTHH3%, UTHH4%
Dim DandT(1 To 4)  As Variant, FirstPass As Boolean, DandTmtp As Date, DateMTP As Date
Dim XSlu%, StartHour%, Nfiles%, ymd&, Hinc%, z!, ZpMTP!(1 To 33), ZgMTP!(1 To 33), Tmtp!(1 To 33)
Dim Iyear%, Imonth%, Iday%, UTks1!, UTks2!, Zavg!(0 To 10), nAltitudes%, jStart%, Ext$
Dim dTT!(1 To 3, 1 To 10, 1 To 2000)

  ZtOffset = Val(txtZtOffset.Text)
  ZtMax = Val(txtZtMax.Text)
  ZpMin = Val(txtZpMin.Text)
  Nfiles = Val(txtNfiles.Text)
  FileSrc$ = Trim(cboFileSrc.Text)
  StartHour = Val(txtUTstart.Text)
  
  Select Case FileSrc$
  Case "DAO", "REAN": Hinc = 6
  Case "NCEP": Hinc = 12
  Case "UKMO": Hinc = 24
  End Select
  
  FirstPass = True
  nAltitudes = 3
  jStart = 1
  For i = 1 To nAltitudes
    Zavg(i) = 10 + (i - 1) * 1  '18 to 23 kms
  Next i
  
Do
  Quit = False
    If FirstPass Then
      If chkUseUTwindow.Value = 1 Then Ext$ = "avg.txt" Else Ext$ = ".txt"
      XSlu = FreeFile
      Open "C:\DC8\SOLVE2\Leslie\" + FileSrc$ + Ext$ For Input As XSlu
      FirstPass = False
    End If
    If XSlu > 0 Then
      If EOF(XSlu) Then Exit Do
    End If
    Input #XSlu, ymd, StartHour, Nfiles, UTks1, UTks2
    'Debug.Print ymd; StartHour; nFiles
    YYYYMMDD$ = Trim(Str(ymd))
    Iyear = Left$(YYYYMMDD$, 4)
    Imonth = Mid$(YYYYMMDD$, 5, 2)
    Iday = Right$(YYYYMMDD$, 2)
    DandT(1) = DateSerial(Iyear, Imonth, Iday) + TimeSerial(StartHour, 0, 0)
    If Nfiles > 1 Then DandT(2) = DandT(1) + TimeSerial(Hinc, 0, 0)
    If Nfiles > 2 Then DandT(3) = DandT(1) + TimeSerial(2 * Hinc, 0, 0)
    If Nfiles > 3 Then DandT(4) = DandT(1) + TimeSerial(3 * Hinc, 0, 0)
  
  DateMTP = DateSerial(Iyear, Imonth, Iday)
  UTHH = StartHour
  UTsec1 = UTHH * 3600#
  UTsec2 = UTsec1 + Hinc * 3600#
  UTsec3 = UTsec2 + Hinc * 3600#
  UTsec4 = UTsec3 + Hinc * 3600#
  xsPath$ = Drive$ + Rdir2$ + Mission$ + "\Leslie\"
  OUTlu = FreeFile
  OUTfile$ = xsPath$ + "XScom" + YYYYMMDD$ + ".txt"
  Open OUTfile$ For Output As #OUTlu
  
' Construct filenames
    xsPath$ = Drive$ + Rdir2$ + Mission$ + "\Leslie\XS" + YYYYMMDD$
'   XS20030112__DAO2003-01-12T06.DC8   DAO  every 06 hours
    For i = 1 To Nfiles
      Iyear = Year(DandT(i)): Imonth = Month(DandT(i)): Iday = Day(DandT(i)): Ihour = Hour(DandT(i))
      Filename$ = "__" + FileSrc$ + Format(Iyear, "0000") + "-" + Format(Imonth, "00") + "-" + Format(Iday, "00") + "T" + Format(Ihour, "00") + ".DC8"
      XSfile$(i) = xsPath$ + Filename$
    Next i
  
  Record = 0: Count = 0: UT = 0: UTsec = 0
  Do
CatchUp:
    Count = Count + 1
    Call GetNextXS1(0, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$(1), Quit)
    If Nfiles > 1 Then
      UTsave = UT
      Call GetNextXS2(0, UTsec&, UT&, Latitude!, Longitude!, PV2!(), T2!(), Zg2!(), Zp2!(), Wz2!(), Wm2!(), Levels%, XSfile$(2), Quit)
    End If
    If Nfiles > 2 Then
      UT = UTsave
      Call GetNextXS3(0, UTsec&, UT&, Latitude!, Longitude!, PV3!(), T3!(), Zg3!(), Zp3!(), Wz3!(), Wm3!(), Levels%, XSfile$(3), Quit)
    End If
    If Nfiles > 3 Then
      UT = UTsave
      Call GetNextXS4(0, UTsec&, UT&, Latitude!, Longitude!, PV4!(), T4!(), Zg4!(), Zp4!(), Wz4!(), Wm3!(), Levels%, XSfile$(4), Quit)
    End If
    If Quit Then Exit Do
    
    If Not Quit Then
       
      For j = jStart To nAltitudes
        z = Zavg(j)
      ' Interpolate XS to flight level
        If chkUseZg.Value = 1 Then
          Txs1 = fLinterp(Zg(), T(), Levels, z)
          If Nfiles > 1 Then Txs2 = fLinterp(Zg2(), T2(), Levels, z)
          If Nfiles > 2 Then Txs3 = fLinterp(Zg3(), T3(), Levels, z)
          If Nfiles > 3 Then Txs4 = fLinterp(Zg4(), T4(), Levels, z)
        Else
          Txs1 = fLinterp(Zp(), T(), Levels, z)
          If Nfiles > 1 Then Txs2 = fLinterp(Zp2(), T2(), Levels, z)
          If Nfiles > 2 Then Txs3 = fLinterp(Zp3(), T3(), Levels, z)
          If Nfiles > 3 Then Txs4 = fLinterp(Zp4(), T4(), Levels, z)
        End If
       
        dTT(1, j, Count) = Txs2 - Txs1
        If Nfiles > 2 Then dTT(2, j, Count) = Txs3 - Txs2
        If Nfiles > 3 Then dTT(3, j, Count) = Txs4 - Txs3
'        Print #OUTlu, Format(Txs(j, Count), "000.0") + vbTab + Format(Tnv(j, Count), "000.0") + vbTab + Format(dT, "00.00") + vbTab + Format(Z, "00.00") + vbTab + Format(Zt1, "00.00")
      Next j
      DoEvents
    End If
  Loop Until Quit
  Close OUTlu
        
  'Debug.Print FileSrc$; " "; yyyymmdd$; Count;
  For k = 1 To Nfiles - 1
  For j = jStart To nAltitudes
    dTmin = 999#: dTmax = -999#: T1Sum = 0#: T2Sum = 0#
    For i = 1 To Count
      dT = dTT(k, j, i)
      If dT < dTmin Then dTmin = dT
      If dT > dTmax Then dTmax = dT
      T1Sum = T1Sum + dT         'Sum time difference
      T2Sum = T2Sum + dT ^ 2     'Sum square of time differences
    Next i
    DTavg = T1Sum / Count        'Calculate average
    If T2Sum - DTavg ^ 2 / Count > 0 Then DTrms = Sqr((T2Sum - DTavg ^ 2 * Count) / (Count - 1)) 'Calculate RMS
    'Debug.Print " "; Zavg(j); DTavg; DTrms; dTmin; dTmax;
  Next j
  Next k
  'Debug.Print ""

' Close XS files as EOF not reached when in UT window mode
  Call GetNextXS1(3, UTsec&, UT&, Latitude!, Longitude!, PV!(), T!(), Zg!(), Zp!(), Wz!(), Wm!(), Levels%, XSfile$(1), Quit)
  If Nfiles > 1 Then Call GetNextXS2(3, UTsec&, UT&, Latitude!, Longitude!, PV2!(), T2!(), Zg2!(), Zp2!(), Wz2!(), Wm2!(), Levels%, XSfile$(2), Quit)
  If Nfiles > 2 Then Call GetNextXS3(3, UTsec&, UT&, Latitude!, Longitude!, PV3!(), T3!(), Zg3!(), Zp3!(), Wz3!(), Wm3!(), Levels%, XSfile$(3), Quit)
  If Nfiles > 3 Then Call GetNextXS4(3, UTsec&, UT&, Latitude!, Longitude!, PV4!(), T4!(), Zg4!(), Zp4!(), Wz4!(), Wm3!(), Levels%, XSfile$(4), Quit)
Loop Until EOF(XSlu)
   Close XSlu
End Sub

Private Sub cndTimeTest_Click()
Dim DandT(1 To 4), FileType$
Dim lu%, i%, StartHour%, Nfiles%, ymd&, Hinc%
Dim Iyear%, Imonth%, Iday%

  FileType$ = "UKMO"
  Select Case FileType$
  Case "DAO", "REAN": Hinc = 6
  Case "NCEP": Hinc = 12
  Case "UKMO": Hinc = 24
  End Select
  
  lu = FreeFile
  Open "C:\DC8\SOLVE2\Leslie\" + FileType$ + ".txt" For Input As lu
Do
  Input #lu, ymd, StartHour, Nfiles
  'Debug.Print ymd; StartHour; Nfiles
  YYYYMMDD$ = Trim(Str(ymd))
  Iyear = Left$(YYYYMMDD$, 4)
  Imonth = Mid$(YYYYMMDD$, 5, 2)
  Iday = Right$(YYYYMMDD$, 2)
  DandT(1) = DateSerial(Iyear, Imonth, Iday) + TimeSerial(StartHour, 0, 0)
  'Debug.Print DandT(1)
  If Nfiles > 1 Then DandT(2) = DandT(1) + TimeSerial(Hinc, 0, 0): Debug.Print DandT(2)
  If Nfiles > 2 Then DandT(3) = DandT(1) + TimeSerial(2 * Hinc, 0, 0): Debug.Print DandT(3)
  If Nfiles > 3 Then DandT(4) = DandT(1) + TimeSerial(3 * Hinc, Imin, Isec): Debug.Print DandT(4)
Loop Until EOF(lu)
Close lu

End Sub



Private Sub Command1_Click()
Dim INPlu%, OUTlu%, INPfile$, OUTfile$, A$

  INPfile$ = RSpath$ + Mission$ + "_RAOBrangeUsed.txt"
  OUTfile$ = RSpath$ + Mission$ + "_RAOBrangeWCT.txt"
  INPlu = FreeFile
  Open INPfile For Input As INPlu
  OUTlu = FreeFile
  Open OUTfile For Output As OUTlu
  Do
    Line Input #INPlu, A$
    Print #OUTlu, A$
    If EOF(INPlu) Then Exit Do
    Line Input #INPlu, A$
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
  
End Sub

Private Sub Command3_Click()
Dim INPlu%, OUTlu%, OUTlu2%, INPfile$, OUTfile$, OUTfile2$, A$

  INPfile$ = RSpath$ + "PREDICT2_RAObrangeAll219.txt"
  OUTfile$ = RSpath$ + "PREDICT2_RAObrangeAllCal.txt"     'RAOBs for T calibration
  OUTfile2$ = RSpath$ + "PREDICT2_RAObrangeAllTest.txt"   'RAOBs for accuracy evaluation
  INPlu = FreeFile
  Open INPfile For Input As INPlu
  OUTlu = FreeFile
  Open OUTfile For Output As OUTlu
  OUTlu2 = FreeFile
  Open OUTfile2 For Output As OUTlu2
' Write header line
  Line Input #INPlu, A$
  Print #OUTlu, A$
  Print #OUTlu2, A$
  Do
    Line Input #INPlu, A$
    Print #OUTlu, A$
    If EOF(INPlu) Then Exit Do
    Line Input #INPlu, A$
    Print #OUTlu2, A$
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu, OUTlu2

End Sub


Private Sub Command4_Click()

End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
Dim i%

  GVchannels = 6
  MVchannels = 46

  For i = 1 To 24
    txtColor(i - 1).Text = i
    txtColor(i - 1).BackColor = fMJcolor(i)
  Next i

  With cboFileSrc
  .AddItem "DAO"
  .AddItem "REAN"
  .AddItem "NCEP"
  .AddItem "UKMO"
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
  
  LoadComboBox frmTools.cboPt, "PLATFORM"
  For i = 0 To cboPt.ListCount - 1
    If AC$ = Left$(cboPt.List(i), 2) Then
      cboPt.ListIndex = i
      Exit For
    End If
  Next i

  With cboRAW
  .AddItem "RAW"
  .AddItem "RMM"
  .AddItem "RPT"
  .ListIndex = 0
  End With
  
  With tmrBatch0
  .Interval = 500
  .Enabled = False
  End With
  
  With tmrBatch1
  .Interval = 500
  .Enabled = False
  End With
  
  Call chkZpLevels_Click

  If Len(Dir$(SUpath$ + Mission$ + "_NUM_batch.txt")) > 0 Then
    cmdFltDates_Click (4)   'Subset of flights in MISSION_NUM_batch.txt
  Else
    cmdFltDates_Click (3)   'All flights in MISSION_NUM.txt
  End If
  DoAll = False
  
End Sub


Private Sub optBA_Click(Index As Integer)
Update_Pt_Filename
End Sub

Private Sub optTtgt_Click(Index As Integer)
Update_Pt_Filename
End Sub


Private Sub SSTab1_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub


Private Sub tmrBatch0_Timer()
Dim i%, j%, FltDate$, LI%
Static CurrentListIndex%, Pathprefix$
  
  tmrBatch0.Enabled = False
  If BatchStart Then
    BatchStart = False
    DoAll = True
    tmrBatch0.Interval = 500
    CurrentListIndex = 0
    ProcessCount = 0
'    Pathprefix$ = DataDrive$ + "\" + frmFLTINFO.cboPlatforms.Text + "\" + frmFLTINFO.cboMissions.Text + "\"
    Pathprefix$ = MNpath$
    For i = 1 To 4
      If chkProcesses(i).Value = 1 Then
        If i = 1 Then                             'Do Retrieval
          ProcessCount = 2
          ProcessList(0) = 0
          If chkChangeParameter(0).Value = 1 Then  'Change OATnavCOR
            ProcessList(1) = 10  'Do TB fit
            ProcessList(2) = 1
            ProcessCount = 3
          Else
            ProcessList(1) = 1
          End If
        Else
          ProcessCount = ProcessCount + 1
          ProcessList(ProcessCount - 1) = i
        End If
      End If
    Next i
    CurrentProcess = -1
    REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"
  
  Else
    CurrentListIndex = CurrentListIndex + 1
    If CurrentListIndex > lstFltDates.ListCount - 1 Then
      DoAll = False               'Clear flag to allow automatic startup
      BatchStart = False
      ProcessCount = 0
      Exit Sub
    End If
  End If

' Batch Process assumes that FLTINFO YYYYMMDD files already exist
    i = CurrentListIndex
    FltDate$ = Trim(lstFltDates.List(i))
    lstFltDates.ListIndex = i  'Select current flight date
    txtFltDates.Text = FltDate$
    j = 0
    Do
      If Trim(Str$(frmFLTINFO.cboFltDates.List(j))) = FltDate$ Then LI = j: Exit Do
      j = j + 1
    Loop
    
    With frmFLTINFO
      .cboFltDates.ListIndex = LI
      .cboFltNos.ListIndex = LI
      .cboObjectives.ListIndex = LI
    End With
    
    BatchFQFN$ = Pathprefix$ + FltDate$ + "\" + Left$(frmFLTINFO.cboPlatforms.Text, 2) + FltDate$ + ".REF"
    With frmFLTINFO
      .txtSourceFile.Text = BatchFQFN$
      .cmdFlight_Click (5)          'Sets DOALL false
      .cmdWriteDefault_Click
    End With
    DoAll = True
    tmrBatch1.Enabled = True
End Sub


Private Sub tmrBatch1_Timer()
Dim k%
Static ProcessNumber%
      
  tmrBatch1.Enabled = False
  CurrentProcess = CurrentProcess + 1
  If CurrentProcess > ProcessCount - 1 Then
    CurrentProcess = -1
    tmrBatch0.Enabled = True
    Exit Sub
  End If
  
  Select Case ProcessList(CurrentProcess)
  Case 0  'Open Raw
    For k = 0 To 2
      If chkChangeParameter(k).Value = 1 Then
        Select Case k
        Case 0 'OATnavCOR
          OATnavCOR = Val(txtNewParameter(k).Text)
          frmFLTINFO.txtOATnavCOR.Text = txtNewParameter(k)
        Case 1 'Read New WCT
          Call frmFLTINFO.cmdReadNewCAL_Click(1)
        Case 2 'Enable WCT
          If UCase(txtNewParameter(k).Text) = "TRUE" Then
            frmFLTINFO.chkEnableWCT.Value = 1
          Else
            frmFLTINFO.chkEnableWCT.Value = 0
          End If
        End Select
      End If
    Next k
    frmFLTINFO.cmdWritePath_Click
    DoEvents
    txtCurrentProcess.Text = "Reading RAW file"
    CallingForm.cmdREF_Click (0)  'Open RAW File
  
  Case 1    'Do Retrieval
    txtCurrentProcess.Text = "Performing Retrieval"
    DoEvents
    CallingForm.cmdREF_Click (11) 'And perform retrieval

  Case 2     'Edit
    txtCurrentProcess.Text = "Editting REF File"
    DoEvents
    CallingForm.cmdREF_Click (7)
    
  Case 3     'Write MP File
    txtCurrentProcess.Text = "Writing MP File"
    DoEvents
    CallingForm.cmdREF_Click (9)
  
   Case 4     'CTC Pot
    txtCurrentProcess.Text = "Plotting Temperature Curtain"
    DoEvents
    CallingForm.cmdREF_Click (0)
    
  Case 10
    txtCurrentProcess.Text = "Fitting TBs"
    DoEvents
    CallingForm.cmdTB_Click (0)           'Adjust gain to reflect new OATnavCOR

  Case Else
  
  End Select
End Sub


