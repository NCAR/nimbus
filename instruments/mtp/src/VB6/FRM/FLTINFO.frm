VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmFLTINFO 
   Caption         =   "FLTINFO Summary"
   ClientHeight    =   4575
   ClientLeft      =   1350
   ClientTop       =   2370
   ClientWidth     =   8220
   Icon            =   "FLTINFO.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   4575
   ScaleWidth      =   8220
   Begin TabDlg.SSTab SSTab1 
      Height          =   4605
      Left            =   0
      TabIndex        =   0
      ToolTipText     =   "Distance from FL to be used Wavg/Wrms calculation"
      Top             =   0
      Width           =   8205
      _ExtentX        =   14473
      _ExtentY        =   8123
      _Version        =   393216
      Tabs            =   9
      Tab             =   7
      TabsPerRow      =   9
      TabHeight       =   520
      TabCaption(0)   =   "&Flight"
      TabPicture(0)   =   "FLTINFO.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "cmdFlight(1)"
      Tab(0).Control(1)=   "txtNewMission"
      Tab(0).Control(2)=   "cmdNewMission"
      Tab(0).Control(3)=   "Frame(11)"
      Tab(0).Control(4)=   "Frame(10)"
      Tab(0).Control(5)=   "cmdShowFltTrack"
      Tab(0).Control(6)=   "Frame8"
      Tab(0).Control(7)=   "Frame1"
      Tab(0).Control(8)=   "cmdAsciiCalfile(1)"
      Tab(0).Control(9)=   "cmdAsciiCalfile(0)"
      Tab(0).Control(10)=   "chkAutoLoad"
      Tab(0).Control(11)=   "cboSU"
      Tab(0).Control(12)=   "cmdFlight(5)"
      Tab(0).Control(13)=   "cmdFlight(0)"
      Tab(0).Control(14)=   "cboObjectives"
      Tab(0).Control(15)=   "cboFltNos"
      Tab(0).Control(16)=   "cboFltDates"
      Tab(0).Control(17)=   "cboMissions"
      Tab(0).Control(18)=   "cboPlatforms"
      Tab(0).Control(19)=   "txtTotalCycles"
      Tab(0).Control(20)=   "txtSourceFile"
      Tab(0).Control(21)=   "txtPI"
      Tab(0).Control(22)=   "txtDestinationFile"
      Tab(0).Control(23)=   "Label4"
      Tab(0).Control(24)=   "Label3"
      Tab(0).Control(25)=   "Label(119)"
      Tab(0).Control(26)=   "Label(116)"
      Tab(0).Control(27)=   "Label(115)"
      Tab(0).Control(28)=   "Label(114)"
      Tab(0).Control(29)=   "Label(113)"
      Tab(0).Control(30)=   "Label(112)"
      Tab(0).Control(31)=   "Label(111)"
      Tab(0).Control(32)=   "Label(110)"
      Tab(0).ControlCount=   33
      TabCaption(1)   =   "&View"
      TabPicture(1)   =   "FLTINFO.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "cmdViewRC(0)"
      Tab(1).Control(1)=   "cmdViewRC(1)"
      Tab(1).Control(2)=   "Frame5"
      Tab(1).Control(3)=   "txtRCpath"
      Tab(1).Control(4)=   "lstRCfile"
      Tab(1).Control(5)=   "Label(145)"
      Tab(1).ControlCount=   6
      TabCaption(2)   =   "&Words"
      TabPicture(2)   =   "FLTINFO.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Frame4"
      Tab(2).Control(1)=   "Frame(9)"
      Tab(2).ControlCount=   2
      TabCaption(3)   =   "&Trops"
      TabPicture(3)   =   "FLTINFO.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "chkSubstitute"
      Tab(3).Control(1)=   "txtTzt2(4)"
      Tab(3).Control(2)=   "txtTzt1(4)"
      Tab(3).Control(3)=   "txtOATzt2(4)"
      Tab(3).Control(4)=   "txtOATzt1(4)"
      Tab(3).Control(5)=   "txtOATks2(3)"
      Tab(3).Control(6)=   "txtOATks1(3)"
      Tab(3).Control(7)=   "txtOATzt1(3)"
      Tab(3).Control(8)=   "txtOATks2(2)"
      Tab(3).Control(9)=   "txtOATks1(2)"
      Tab(3).Control(10)=   "txtOATzt1(2)"
      Tab(3).Control(11)=   "txtOATks1(1)"
      Tab(3).Control(12)=   "txtOATks2(0)"
      Tab(3).Control(13)=   "txtOATzt1(1)"
      Tab(3).Control(14)=   "txtOATzt1(0)"
      Tab(3).Control(15)=   "txtOATzt2(0)"
      Tab(3).Control(16)=   "txtOATzt2(1)"
      Tab(3).Control(17)=   "txtOATzt2(2)"
      Tab(3).Control(18)=   "txtOATzt2(3)"
      Tab(3).Control(19)=   "chkOATtrop"
      Tab(3).Control(20)=   "txtOATks1(0)"
      Tab(3).Control(21)=   "txtOATks2(1)"
      Tab(3).Control(22)=   "txtTzt1(0)"
      Tab(3).Control(23)=   "txtTzt1(1)"
      Tab(3).Control(24)=   "txtTzt1(2)"
      Tab(3).Control(25)=   "txtTzt1(3)"
      Tab(3).Control(26)=   "txtTzt2(0)"
      Tab(3).Control(27)=   "txtTzt2(1)"
      Tab(3).Control(28)=   "txtTzt2(2)"
      Tab(3).Control(29)=   "txtTzt2(3)"
      Tab(3).Control(30)=   "Label(127)"
      Tab(3).Control(31)=   "Label(126)"
      Tab(3).Control(32)=   "Label(125)"
      Tab(3).Control(33)=   "Label(124)"
      Tab(3).Control(34)=   "Label(123)"
      Tab(3).Control(35)=   "Label(122)"
      Tab(3).Control(36)=   "Label25"
      Tab(3).Control(37)=   "Label21"
      Tab(3).Control(38)=   "Label11"
      Tab(3).Control(39)=   "Label10"
      Tab(3).ControlCount=   40
      TabCaption(4)   =   "&History"
      TabPicture(4)   =   "FLTINFO.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "txtScans(0)"
      Tab(4).Control(1)=   "Frame(6)"
      Tab(4).Control(2)=   "Frame6"
      Tab(4).Control(3)=   "Frame7"
      Tab(4).Control(4)=   "Label(87)"
      Tab(4).ControlCount=   5
      TabCaption(5)   =   "&RCs"
      TabPicture(5)   =   "FLTINFO.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "Frame(0)"
      Tab(5).ControlCount=   1
      TabCaption(6)   =   "&Ad Hoc"
      TabPicture(6)   =   "FLTINFO.frx":04EA
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "Label(106)"
      Tab(6).Control(1)=   "Label(160)"
      Tab(6).Control(2)=   "Label(162)"
      Tab(6).Control(3)=   "Label(121)"
      Tab(6).Control(4)=   "Label(131)"
      Tab(6).Control(5)=   "Label(132)"
      Tab(6).Control(6)=   "Label(133)"
      Tab(6).Control(7)=   "Label(159)"
      Tab(6).Control(8)=   "Label(143)"
      Tab(6).Control(9)=   "Label(167)"
      Tab(6).Control(10)=   "Frame3"
      Tab(6).Control(11)=   "txtMTPcor"
      Tab(6).Control(12)=   "txtNcts"
      Tab(6).Control(13)=   "txtATPrange"
      Tab(6).Control(14)=   "Frame2"
      Tab(6).Control(15)=   "cboUSEx"
      Tab(6).Control(16)=   "Frame11"
      Tab(6).Control(17)=   "cboDestination"
      Tab(6).Control(18)=   "cboDataSource"
      Tab(6).Control(19)=   "cboDestination2"
      Tab(6).Control(20)=   "cmdTemplateMap"
      Tab(6).Control(21)=   "txtRAWextension"
      Tab(6).ControlCount=   22
      TabCaption(7)   =   "&Gain"
      TabPicture(7)   =   "FLTINFO.frx":0506
      Tab(7).ControlEnabled=   -1  'True
      Tab(7).Control(0)=   "Label(134)"
      Tab(7).Control(0).Enabled=   0   'False
      Tab(7).Control(1)=   "Label(135)"
      Tab(7).Control(1).Enabled=   0   'False
      Tab(7).Control(2)=   "Label(136)"
      Tab(7).Control(2).Enabled=   0   'False
      Tab(7).Control(3)=   "Label(137)"
      Tab(7).Control(3).Enabled=   0   'False
      Tab(7).Control(4)=   "Label(138)"
      Tab(7).Control(4).Enabled=   0   'False
      Tab(7).Control(5)=   "Label(139)"
      Tab(7).Control(5).Enabled=   0   'False
      Tab(7).Control(6)=   "Line2"
      Tab(7).Control(6).Enabled=   0   'False
      Tab(7).Control(7)=   "Label(150)"
      Tab(7).Control(7).Enabled=   0   'False
      Tab(7).Control(8)=   "Label(141)"
      Tab(7).Control(8).Enabled=   0   'False
      Tab(7).Control(9)=   "Label(105)"
      Tab(7).Control(9).Enabled=   0   'False
      Tab(7).Control(10)=   "Label(104)"
      Tab(7).Control(10).Enabled=   0   'False
      Tab(7).Control(11)=   "Label(151)"
      Tab(7).Control(11).Enabled=   0   'False
      Tab(7).Control(12)=   "Label2"
      Tab(7).Control(12).Enabled=   0   'False
      Tab(7).Control(13)=   "txtEmissivity"
      Tab(7).Control(13).Enabled=   0   'False
      Tab(7).Control(14)=   "txtReflectivity"
      Tab(7).Control(14).Enabled=   0   'False
      Tab(7).Control(15)=   "Frame10(0)"
      Tab(7).Control(15).Enabled=   0   'False
      Tab(7).Control(16)=   "txtOATnavCOR"
      Tab(7).Control(16).Enabled=   0   'False
      Tab(7).Control(17)=   "cboREFsource"
      Tab(7).Control(17).Enabled=   0   'False
      Tab(7).Control(18)=   "cboOATsource"
      Tab(7).Control(18).Enabled=   0   'False
      Tab(7).Control(19)=   "cboGscale"
      Tab(7).Control(19).Enabled=   0   'False
      Tab(7).Control(20)=   "Frame(3)"
      Tab(7).Control(20).Enabled=   0   'False
      Tab(7).Control(21)=   "Frame(4)"
      Tab(7).Control(21).Enabled=   0   'False
      Tab(7).Control(22)=   "cboGE(0)"
      Tab(7).Control(22).Enabled=   0   'False
      Tab(7).Control(23)=   "Frame(5)"
      Tab(7).Control(23).Enabled=   0   'False
      Tab(7).Control(24)=   "Frame(7)"
      Tab(7).Control(24).Enabled=   0   'False
      Tab(7).Control(25)=   "Frame(8)"
      Tab(7).Control(25).Enabled=   0   'False
      Tab(7).Control(26)=   "Frame(2)"
      Tab(7).Control(26).Enabled=   0   'False
      Tab(7).Control(27)=   "chkLHS"
      Tab(7).Control(27).Enabled=   0   'False
      Tab(7).Control(28)=   "txtLocHor"
      Tab(7).Control(28).Enabled=   0   'False
      Tab(7).Control(29)=   "txtChannels"
      Tab(7).Control(29).Enabled=   0   'False
      Tab(7).Control(30)=   "txtNel"
      Tab(7).Control(30).Enabled=   0   'False
      Tab(7).Control(31)=   "txtTmtpTnav"
      Tab(7).Control(31).Enabled=   0   'False
      Tab(7).Control(32)=   "Frame(12)"
      Tab(7).Control(32).Enabled=   0   'False
      Tab(7).Control(33)=   "txtPcorr"
      Tab(7).Control(33).Enabled=   0   'False
      Tab(7).Control(34)=   "cmdManualUpdate"
      Tab(7).Control(34).Enabled=   0   'False
      Tab(7).ControlCount=   35
      TabCaption(8)   =   "WCT"
      TabPicture(8)   =   "FLTINFO.frx":0522
      Tab(8).ControlEnabled=   0   'False
      Tab(8).Control(0)=   "Label(140)"
      Tab(8).Control(1)=   "Label(144)"
      Tab(8).Control(2)=   "Label(158)"
      Tab(8).Control(3)=   "Frame9"
      Tab(8).Control(4)=   "txtGenDate"
      Tab(8).Control(5)=   "lstWCTdates"
      Tab(8).Control(6)=   "txtDeltaTmin"
      Tab(8).Control(7)=   "txtWCTdates"
      Tab(8).Control(8)=   "cmdWCT(0)"
      Tab(8).Control(9)=   "cmdWCT(1)"
      Tab(8).Control(10)=   "Frame(1)"
      Tab(8).Control(11)=   "cmdReadNewCAL(1)"
      Tab(8).Control(12)=   "cmdReadNewCAL(0)"
      Tab(8).Control(13)=   "chkUseDefaultCalfile"
      Tab(8).Control(14)=   "cmdReadNewCAL(2)"
      Tab(8).Control(15)=   "txtReferenceTargets"
      Tab(8).Control(16)=   "fraLAT"
      Tab(8).ControlCount=   17
      Begin VB.Frame fraLAT 
         Caption         =   "Retrieval Algorithm"
         Height          =   975
         Left            =   -72525
         TabIndex        =   794
         Top             =   3465
         Width           =   2295
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   798
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   797
            Top             =   240
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   796
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtLAT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   795
            Top             =   600
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT1"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   802
            Top             =   240
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT2"
            Height          =   255
            Index           =   1
            Left            =   1200
            TabIndex        =   801
            Top             =   240
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT3"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   800
            Top             =   600
            Width           =   495
         End
         Begin VB.Label lblLAT 
            Caption         =   "LAT4"
            Height          =   255
            Index           =   3
            Left            =   1200
            TabIndex        =   799
            Top             =   600
            Width           =   495
         End
      End
      Begin VB.CommandButton cmdFlight 
         Caption         =   "&Use Batch"
         Height          =   375
         Index           =   1
         Left            =   -69000
         TabIndex        =   791
         Top             =   840
         Width           =   1035
      End
      Begin VB.TextBox txtRAWextension 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -67860
         MultiLine       =   -1  'True
         TabIndex        =   777
         Text            =   "FLTINFO.frx":053E
         Top             =   2760
         Width           =   705
      End
      Begin VB.CommandButton cmdTemplateMap 
         Caption         =   "Template Map"
         Height          =   330
         Left            =   -71310
         TabIndex        =   773
         Top             =   2550
         Width           =   1425
      End
      Begin VB.ComboBox cboDestination2 
         Height          =   315
         Left            =   -71235
         TabIndex        =   771
         Text            =   "C:\MTP\RTA\RAW\"
         Top             =   4200
         Width           =   4350
      End
      Begin VB.ComboBox cboDataSource 
         Height          =   315
         Left            =   -71235
         TabIndex        =   768
         Text            =   "C:\MTP\RTA\RAW\"
         Top             =   3525
         Width           =   4350
      End
      Begin VB.ComboBox cboDestination 
         Height          =   315
         Left            =   -71235
         TabIndex        =   767
         Text            =   "C:\MTP\RTA\RAW\"
         Top             =   3855
         Width           =   4350
      End
      Begin VB.CommandButton cmdManualUpdate 
         Caption         =   "Manual Update"
         Height          =   330
         Left            =   2835
         TabIndex        =   765
         ToolTipText     =   "Manual Update should only be used when trouble shooting!"
         Top             =   3780
         Width           =   1485
      End
      Begin VB.TextBox txtPcorr 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   7260
         MultiLine       =   -1  'True
         TabIndex        =   763
         Top             =   3600
         Width           =   555
      End
      Begin VB.Frame Frame11 
         Caption         =   "RC Files (cont.)"
         Height          =   1980
         Left            =   -74925
         TabIndex        =   633
         Top             =   405
         Width           =   8040
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   64
            Left            =   7500
            MultiLine       =   -1  'True
            TabIndex        =   756
            Top             =   1575
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   64
            Left            =   6300
            TabIndex        =   755
            Top             =   1560
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   64
            Left            =   7200
            TabIndex        =   754
            Top             =   1575
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   63
            Left            =   7500
            MultiLine       =   -1  'True
            TabIndex        =   752
            Top             =   1245
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   63
            Left            =   6300
            TabIndex        =   751
            Top             =   1230
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   63
            Left            =   7200
            TabIndex        =   750
            Top             =   1245
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   62
            Left            =   7500
            MultiLine       =   -1  'True
            TabIndex        =   748
            Top             =   915
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   62
            Left            =   6300
            TabIndex        =   747
            Top             =   900
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   62
            Left            =   7200
            TabIndex        =   746
            Top             =   915
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   61
            Left            =   7500
            MultiLine       =   -1  'True
            TabIndex        =   744
            Top             =   585
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   61
            Left            =   6300
            TabIndex        =   743
            Top             =   570
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   61
            Left            =   7200
            TabIndex        =   742
            Top             =   585
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   60
            Left            =   7500
            MultiLine       =   -1  'True
            TabIndex        =   740
            Top             =   255
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   60
            Left            =   6300
            TabIndex        =   739
            Top             =   240
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   60
            Left            =   7200
            TabIndex        =   738
            Top             =   255
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   59
            Left            =   5490
            MultiLine       =   -1  'True
            TabIndex        =   736
            Top             =   1560
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   59
            Left            =   4290
            TabIndex        =   735
            Top             =   1545
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   59
            Left            =   5190
            TabIndex        =   734
            Top             =   1560
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   58
            Left            =   5490
            MultiLine       =   -1  'True
            TabIndex        =   732
            Top             =   1230
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   58
            Left            =   4290
            TabIndex        =   731
            Top             =   1215
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   58
            Left            =   5190
            TabIndex        =   730
            Top             =   1230
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   57
            Left            =   5490
            MultiLine       =   -1  'True
            TabIndex        =   728
            Top             =   900
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   57
            Left            =   4290
            TabIndex        =   727
            Top             =   885
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   57
            Left            =   5190
            TabIndex        =   726
            Top             =   900
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   56
            Left            =   5490
            MultiLine       =   -1  'True
            TabIndex        =   724
            Top             =   570
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   56
            Left            =   4290
            TabIndex        =   723
            Top             =   555
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   56
            Left            =   5190
            TabIndex        =   722
            Top             =   570
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   55
            Left            =   5490
            MultiLine       =   -1  'True
            TabIndex        =   720
            Top             =   255
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   55
            Left            =   4290
            TabIndex        =   719
            Top             =   240
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   55
            Left            =   5190
            TabIndex        =   718
            Top             =   255
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   54
            Left            =   3480
            MultiLine       =   -1  'True
            TabIndex        =   716
            Top             =   1545
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   54
            Left            =   2280
            TabIndex        =   715
            Top             =   1530
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   54
            Left            =   3180
            TabIndex        =   714
            Top             =   1545
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   53
            Left            =   3480
            MultiLine       =   -1  'True
            TabIndex        =   712
            Top             =   1215
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   53
            Left            =   2280
            TabIndex        =   711
            Top             =   1200
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   53
            Left            =   3180
            TabIndex        =   710
            Top             =   1215
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   52
            Left            =   3480
            MultiLine       =   -1  'True
            TabIndex        =   708
            Top             =   885
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   52
            Left            =   2280
            TabIndex        =   707
            Top             =   870
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   52
            Left            =   3180
            TabIndex        =   706
            Top             =   885
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   51
            Left            =   3480
            MultiLine       =   -1  'True
            TabIndex        =   704
            Top             =   570
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   51
            Left            =   2280
            TabIndex        =   703
            Top             =   555
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   51
            Left            =   3180
            TabIndex        =   702
            Top             =   570
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   50
            Left            =   3480
            MultiLine       =   -1  'True
            TabIndex        =   700
            Top             =   240
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   50
            Left            =   2280
            TabIndex        =   699
            Top             =   225
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   50
            Left            =   3180
            TabIndex        =   698
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   49
            Left            =   1485
            MultiLine       =   -1  'True
            TabIndex        =   696
            Top             =   1560
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   49
            Left            =   285
            TabIndex        =   695
            Top             =   1545
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   49
            Left            =   1185
            TabIndex        =   694
            Top             =   1560
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   48
            Left            =   1485
            MultiLine       =   -1  'True
            TabIndex        =   692
            Top             =   1245
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   48
            Left            =   285
            TabIndex        =   691
            Top             =   1230
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   48
            Left            =   1185
            TabIndex        =   690
            Top             =   1245
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   47
            Left            =   1485
            MultiLine       =   -1  'True
            TabIndex        =   688
            Top             =   915
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   47
            Left            =   285
            TabIndex        =   687
            Top             =   900
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   47
            Left            =   1185
            TabIndex        =   686
            Top             =   915
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   46
            Left            =   1485
            MultiLine       =   -1  'True
            TabIndex        =   684
            Top             =   585
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   46
            Left            =   285
            TabIndex        =   683
            Top             =   570
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   46
            Left            =   1185
            TabIndex        =   682
            Top             =   585
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   45
            Left            =   1485
            MultiLine       =   -1  'True
            TabIndex        =   680
            Top             =   270
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   45
            Left            =   285
            TabIndex        =   679
            Top             =   255
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   45
            Left            =   1185
            TabIndex        =   678
            Top             =   270
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "64"
            Height          =   255
            Index           =   64
            Left            =   6060
            TabIndex        =   757
            Top             =   1620
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "63"
            Height          =   255
            Index           =   63
            Left            =   6060
            TabIndex        =   753
            Top             =   1290
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "62"
            Height          =   255
            Index           =   62
            Left            =   6060
            TabIndex        =   749
            Top             =   960
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "61"
            Height          =   255
            Index           =   61
            Left            =   6060
            TabIndex        =   745
            Top             =   630
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "60"
            Height          =   255
            Index           =   60
            Left            =   6060
            TabIndex        =   741
            Top             =   300
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "59"
            Height          =   255
            Index           =   59
            Left            =   4050
            TabIndex        =   737
            Top             =   1605
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "58"
            Height          =   255
            Index           =   58
            Left            =   4050
            TabIndex        =   733
            Top             =   1275
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "57"
            Height          =   255
            Index           =   57
            Left            =   4035
            TabIndex        =   729
            Top             =   945
            Width           =   255
         End
         Begin VB.Line Line5 
            X1              =   1995
            X2              =   1995
            Y1              =   210
            Y2              =   1815
         End
         Begin VB.Label lblRC 
            Caption         =   "56"
            Height          =   255
            Index           =   56
            Left            =   4050
            TabIndex        =   725
            Top             =   615
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "55"
            Height          =   255
            Index           =   55
            Left            =   4050
            TabIndex        =   721
            Top             =   300
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "54"
            Height          =   255
            Index           =   54
            Left            =   2040
            TabIndex        =   717
            Top             =   1590
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "53"
            Height          =   255
            Index           =   53
            Left            =   2040
            TabIndex        =   713
            Top             =   1260
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "52"
            Height          =   255
            Index           =   52
            Left            =   2040
            TabIndex        =   709
            Top             =   930
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "51"
            Height          =   255
            Index           =   51
            Left            =   2040
            TabIndex        =   705
            Top             =   615
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "50"
            Height          =   255
            Index           =   50
            Left            =   2040
            TabIndex        =   701
            Top             =   285
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "49"
            Height          =   255
            Index           =   49
            Left            =   45
            TabIndex        =   697
            Top             =   1605
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "48"
            Height          =   255
            Index           =   48
            Left            =   45
            TabIndex        =   693
            Top             =   1290
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "47"
            Height          =   255
            Index           =   47
            Left            =   45
            TabIndex        =   689
            Top             =   960
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "46"
            Height          =   255
            Index           =   46
            Left            =   45
            TabIndex        =   685
            Top             =   630
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "45"
            Height          =   255
            Index           =   45
            Left            =   45
            TabIndex        =   681
            Top             =   315
            Width           =   255
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "Edit Word"
         Height          =   4125
         Left            =   -74880
         TabIndex        =   589
         Top             =   420
         Width           =   4395
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   7
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   632
            Top             =   2970
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   4
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   631
            Top             =   2070
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   2
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   630
            Top             =   1455
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   3
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   629
            Top             =   1755
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   1
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   628
            Top             =   1155
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   5
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   627
            Top             =   2370
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   6
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   626
            Top             =   2670
            Width           =   615
         End
         Begin VB.TextBox txtNlevMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   625
            Top             =   2670
            Width           =   615
         End
         Begin VB.TextBox txtNlevMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   624
            Top             =   2670
            Width           =   615
         End
         Begin VB.CheckBox chkNlev 
            Caption         =   "Nlev"
            Height          =   210
            Left            =   120
            TabIndex        =   623
            Top             =   2670
            Width           =   750
         End
         Begin VB.TextBox txtTropAltMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   622
            Top             =   1770
            Width           =   615
         End
         Begin VB.TextBox txtTropAltMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   621
            Top             =   1770
            Width           =   615
         End
         Begin VB.TextBox txtZtOffA 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   620
            Top             =   1470
            Width           =   615
         End
         Begin VB.TextBox txtZtOffB 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   619
            Top             =   1470
            Width           =   615
         End
         Begin VB.TextBox txtTemperatureMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   618
            Top             =   1170
            Width           =   615
         End
         Begin VB.TextBox txtTemperatureMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   617
            Top             =   1170
            Width           =   615
         End
         Begin VB.TextBox txtPitchMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   616
            Top             =   2070
            Width           =   615
         End
         Begin VB.TextBox txtPitchMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   615
            Top             =   2070
            Width           =   615
         End
         Begin VB.TextBox txtRollMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   614
            Top             =   2370
            Width           =   615
         End
         Begin VB.TextBox txtRollMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   613
            Top             =   2370
            Width           =   615
         End
         Begin VB.CheckBox chkTropAlt 
            Caption         =   "Tropopause Altitude [km]"
            Height          =   195
            Left            =   120
            TabIndex        =   612
            Top             =   1770
            Width           =   2070
         End
         Begin VB.CheckBox chkZtOff 
            Caption         =   "(Trop-pALT) Offset [km]"
            Height          =   195
            Left            =   120
            TabIndex        =   611
            Top             =   1470
            Width           =   1995
         End
         Begin VB.CheckBox chkTemperature 
            Caption         =   "Temperature [K]"
            Height          =   195
            Left            =   120
            TabIndex        =   610
            Top             =   1170
            Width           =   1485
         End
         Begin VB.CheckBox chkPitch 
            Caption         =   "Pitch Range [deg]"
            Height          =   195
            Left            =   120
            TabIndex        =   609
            Top             =   2070
            Width           =   1635
         End
         Begin VB.CheckBox chkRoll 
            Caption         =   "Roll Range [deg]"
            Height          =   195
            Left            =   120
            TabIndex        =   608
            Top             =   2370
            Width           =   1665
         End
         Begin VB.CheckBox chkRate 
            Caption         =   "Delta P [m/scan]"
            Height          =   195
            Left            =   120
            TabIndex        =   607
            Top             =   2970
            Width           =   1545
         End
         Begin VB.TextBox txtRateMin 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   606
            Top             =   2970
            Width           =   615
         End
         Begin VB.TextBox txtRateMax 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   605
            Top             =   2970
            Width           =   615
         End
         Begin VB.TextBox txtAbove 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   600
            Text            =   "FLTINFO.frx":0542
            Top             =   870
            Width           =   615
         End
         Begin VB.TextBox txtBelow 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   599
            Text            =   "FLTINFO.frx":0546
            Top             =   870
            Width           =   615
         End
         Begin VB.CheckBox chkUseRelativeRA 
            Caption         =   "Use Relative Offsets"
            Height          =   195
            Left            =   390
            TabIndex        =   598
            Top             =   855
            Value           =   1  'Checked
            Width           =   1755
         End
         Begin VB.CheckBox chkGoodScansOnly 
            Caption         =   "Use Only Good Scans for Limits"
            Height          =   195
            Left            =   120
            TabIndex        =   597
            Top             =   3840
            Width           =   2595
         End
         Begin VB.CheckBox chkSelectAll 
            Caption         =   "Select All"
            Height          =   195
            Left            =   120
            TabIndex        =   596
            Top             =   240
            Width           =   1035
         End
         Begin VB.CheckBox chkRetAlt 
            Caption         =   "Retrieved Altitude [km]"
            Height          =   195
            Left            =   120
            TabIndex        =   595
            Top             =   570
            Width           =   1950
         End
         Begin VB.TextBox txtRetAltMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   594
            Top             =   570
            Width           =   615
         End
         Begin VB.TextBox txtRetAltMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2280
            MultiLine       =   -1  'True
            TabIndex        =   593
            Top             =   570
            Width           =   615
         End
         Begin VB.CommandButton cmdPC 
            Caption         =   "Show %"
            Height          =   375
            Index           =   0
            Left            =   3270
            TabIndex        =   592
            Top             =   3690
            Width           =   975
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   0
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   591
            Top             =   570
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   8
            Left            =   3690
            MultiLine       =   -1  'True
            TabIndex        =   590
            Top             =   3270
            Width           =   615
         End
         Begin VB.Label lblPC 
            Caption         =   "% Edited"
            Height          =   255
            Index           =   0
            Left            =   3675
            TabIndex        =   604
            Top             =   330
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "Maximum"
            Height          =   255
            Index           =   22
            Left            =   2970
            TabIndex        =   603
            Top             =   330
            Width           =   735
         End
         Begin VB.Label Label 
            Caption         =   "Minimum"
            Height          =   255
            Index           =   86
            Left            =   2250
            TabIndex        =   602
            Top             =   330
            Width           =   735
         End
         Begin VB.Label Label 
            Caption         =   "No. of Substituted OAT Trops"
            Height          =   210
            Index           =   88
            Left            =   120
            TabIndex        =   601
            Top             =   3345
            Width           =   2145
         End
      End
      Begin VB.CommandButton cmdViewRC 
         Caption         =   "View CFG"
         Height          =   375
         Index           =   0
         Left            =   -69750
         TabIndex        =   588
         Top             =   4110
         Width           =   885
      End
      Begin VB.CommandButton cmdViewRC 
         Caption         =   "View RC"
         Height          =   375
         Index           =   1
         Left            =   -68820
         TabIndex        =   587
         Top             =   4110
         Width           =   885
      End
      Begin VB.Frame Frame5 
         Caption         =   "pALT[km]"
         Height          =   4155
         Left            =   -67860
         TabIndex        =   578
         Top             =   360
         Width           =   1005
         Begin VB.CommandButton cmdFLA 
            Caption         =   "&Setup"
            Height          =   315
            Index           =   4
            Left            =   90
            TabIndex        =   761
            Top             =   3780
            Width           =   855
         End
         Begin VB.TextBox txtFLA 
            Height          =   285
            Left            =   60
            TabIndex        =   585
            Top             =   240
            Width           =   855
         End
         Begin VB.ListBox lstFLA 
            Height          =   1620
            ItemData        =   "FLTINFO.frx":054B
            Left            =   45
            List            =   "FLTINFO.frx":054D
            TabIndex        =   584
            Top             =   540
            Width           =   855
         End
         Begin VB.CommandButton cmdFLA 
            Caption         =   "&Add"
            Height          =   315
            Index           =   0
            Left            =   90
            TabIndex        =   583
            Top             =   2475
            Width           =   855
         End
         Begin VB.CommandButton cmdFLA 
            Caption         =   "&Sub"
            Height          =   315
            Index           =   1
            Left            =   90
            TabIndex        =   582
            Top             =   2805
            Width           =   855
         End
         Begin VB.CommandButton cmdFLA 
            Caption         =   "&Clear"
            Height          =   315
            Index           =   2
            Left            =   90
            TabIndex        =   581
            Top             =   3135
            Width           =   855
         End
         Begin VB.CommandButton cmdFLA 
            Caption         =   "&Default"
            Height          =   315
            Index           =   3
            Left            =   90
            TabIndex        =   580
            Top             =   3465
            Width           =   855
         End
         Begin VB.TextBox txtNFL 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   579
            Top             =   2175
            Width           =   315
         End
         Begin VB.Label Label 
            Caption         =   "NFL"
            Height          =   180
            Index           =   7
            Left            =   90
            TabIndex        =   586
            Top             =   2220
            Width           =   375
         End
      End
      Begin VB.TextBox txtRCpath 
         Height          =   285
         Left            =   -74115
         TabIndex        =   576
         Top             =   4230
         Width           =   3015
      End
      Begin VB.ListBox lstRCfile 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3630
         Left            =   -74940
         TabIndex        =   575
         Top             =   420
         Width           =   7035
      End
      Begin VB.ComboBox cboUSEx 
         Height          =   315
         ItemData        =   "FLTINFO.frx":054F
         Left            =   -67860
         List            =   "FLTINFO.frx":0551
         TabIndex        =   573
         Top             =   2430
         Width           =   975
      End
      Begin VB.Frame Frame2 
         Caption         =   "ALTfudge?"
         Height          =   1095
         Left            =   -73065
         TabIndex        =   567
         Top             =   2415
         Width           =   1515
         Begin VB.CheckBox chkMMSpALT 
            Alignment       =   1  'Right Justify
            Height          =   255
            Left            =   1200
            TabIndex        =   570
            Top             =   120
            Width           =   225
         End
         Begin VB.TextBox txtALTfujCONST 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   569
            Top             =   360
            Width           =   465
         End
         Begin VB.TextBox txtALTfujSLOPE 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   568
            Top             =   690
            Width           =   465
         End
         Begin VB.Label lblALTfujCONST 
            Caption         =   "Bias"
            Height          =   195
            Left            =   120
            TabIndex        =   572
            Top             =   420
            Width           =   525
         End
         Begin VB.Label lblALTfujSLOPE 
            Caption         =   "Slope"
            Height          =   225
            Left            =   120
            TabIndex        =   571
            Top             =   720
            Width           =   495
         End
      End
      Begin VB.TextBox txtATPrange 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73665
         MultiLine       =   -1  'True
         TabIndex        =   565
         Text            =   "FLTINFO.frx":0553
         Top             =   3915
         Width           =   495
      End
      Begin VB.TextBox txtNewMission 
         Height          =   345
         Left            =   -73260
         TabIndex        =   563
         Top             =   3210
         Width           =   1035
      End
      Begin VB.CommandButton cmdNewMission 
         Caption         =   "Add New Mission"
         Height          =   345
         Left            =   -74910
         TabIndex        =   562
         ToolTipText     =   "Sets up default folders for a new mission. First, navigate to last mission for this instrument/aircraft."
         Top             =   3210
         Width           =   1635
      End
      Begin VB.TextBox txtNcts 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73650
         MultiLine       =   -1  'True
         TabIndex        =   561
         Text            =   "FLTINFO.frx":0558
         Top             =   4230
         Width           =   495
      End
      Begin VB.TextBox txtReferenceTargets 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71130
         MultiLine       =   -1  'True
         TabIndex        =   559
         Text            =   "FLTINFO.frx":055D
         Top             =   2520
         Width           =   465
      End
      Begin VB.Frame Frame 
         Caption         =   "Moving Average Info"
         Height          =   1365
         Index           =   12
         Left            =   3390
         TabIndex        =   546
         ToolTipText     =   "Changes to any of these parameters require the RAW file to be re-read!"
         Top             =   2310
         Width           =   2505
         Begin VB.TextBox txtCMAcycles2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1860
            MultiLine       =   -1  'True
            TabIndex        =   552
            Text            =   "FLTINFO.frx":0561
            Top             =   420
            Width           =   495
         End
         Begin VB.TextBox txtBadCycles 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   551
            Text            =   "FLTINFO.frx":0565
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtRFIiterations 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   550
            Text            =   "FLTINFO.frx":0569
            Top             =   1020
            Width           =   495
         End
         Begin VB.TextBox txtCMAcycles 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1320
            MultiLine       =   -1  'True
            TabIndex        =   549
            Text            =   "FLTINFO.frx":056D
            Top             =   420
            Width           =   495
         End
         Begin VB.TextBox txtBadCycles2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1860
            MultiLine       =   -1  'True
            TabIndex        =   548
            Text            =   "FLTINFO.frx":0571
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtRFIiterations2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1860
            MultiLine       =   -1  'True
            TabIndex        =   547
            Text            =   "FLTINFO.frx":0575
            Top             =   1020
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "Fast"
            Height          =   195
            Index           =   142
            Left            =   1920
            TabIndex        =   557
            Top             =   240
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "RFI Iterations"
            Height          =   195
            Index           =   108
            Left            =   60
            TabIndex        =   556
            Top             =   1020
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "Slow"
            Height          =   255
            Index           =   107
            Left            =   1350
            TabIndex        =   555
            Top             =   240
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "Cycles Averaged"
            Height          =   255
            Index           =   152
            Left            =   60
            TabIndex        =   554
            Top             =   450
            Width           =   1275
         End
         Begin VB.Label Label 
            Caption         =   "Bad Cycles"
            Height          =   255
            Index           =   153
            Left            =   60
            TabIndex        =   553
            Top             =   720
            Width           =   1215
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Restore Format"
         Height          =   675
         Index           =   11
         Left            =   -68730
         TabIndex        =   528
         Top             =   3840
         Width           =   1755
         Begin VB.CommandButton cmdFileFormatRestore 
            Caption         =   "&Restore"
            Height          =   375
            Left            =   750
            TabIndex        =   530
            Top             =   210
            Width           =   885
         End
         Begin VB.ComboBox cboFileFormatRestore 
            Height          =   315
            Left            =   90
            TabIndex        =   529
            Text            =   "32"
            Top             =   270
            Width           =   585
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "REF File Format"
         Height          =   675
         Index           =   10
         Left            =   -70740
         TabIndex        =   523
         Top             =   3840
         Width           =   1935
         Begin VB.ComboBox cboFileFormatOut 
            Height          =   315
            Left            =   1290
            TabIndex        =   525
            Text            =   "33"
            Top             =   270
            Width           =   585
         End
         Begin VB.ComboBox cboFileFormatIn 
            Height          =   315
            Left            =   390
            TabIndex        =   524
            Text            =   "32"
            Top             =   270
            Width           =   585
         End
         Begin VB.Label Label 
            Caption         =   "In"
            Height          =   285
            Index           =   154
            Left            =   150
            TabIndex        =   527
            Top             =   330
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "Out"
            Height          =   285
            Index           =   109
            Left            =   990
            TabIndex        =   526
            Top             =   330
            Width           =   255
         End
      End
      Begin VB.CommandButton cmdShowFltTrack 
         Caption         =   "Show Flight Track"
         Height          =   405
         Left            =   -69000
         TabIndex        =   521
         Top             =   2400
         Width           =   2055
      End
      Begin VB.TextBox txtTmtpTnav 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   3735
         MultiLine       =   -1  'True
         TabIndex        =   520
         Text            =   "FLTINFO.frx":0579
         Top             =   4230
         Width           =   555
      End
      Begin VB.CommandButton cmdReadNewCAL 
         Caption         =   "Read Mission.WCT"
         Height          =   375
         Index           =   2
         Left            =   -68655
         TabIndex        =   518
         Top             =   4080
         Width           =   1695
      End
      Begin VB.TextBox txtNel 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5385
         MultiLine       =   -1  'True
         TabIndex        =   512
         Text            =   "FLTINFO.frx":057F
         Top             =   4230
         Width           =   495
      End
      Begin VB.TextBox txtChannels 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5385
         MultiLine       =   -1  'True
         TabIndex        =   511
         Text            =   "FLTINFO.frx":0584
         Top             =   3960
         Width           =   495
      End
      Begin VB.TextBox txtLocHor 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5385
         MultiLine       =   -1  'True
         TabIndex        =   509
         Text            =   "FLTINFO.frx":0586
         Top             =   3690
         Width           =   495
      End
      Begin VB.CheckBox chkLHS 
         Caption         =   "SU on LHS"
         Height          =   195
         Left            =   4320
         TabIndex        =   508
         ToolTipText     =   "Checked if Sensor Unit is on RHS of aircraft"
         Top             =   510
         Width           =   1185
      End
      Begin VB.Frame Frame 
         Caption         =   "Thresholds"
         Height          =   1155
         Index           =   2
         Left            =   2340
         TabIndex        =   503
         Top             =   2520
         Width           =   1035
         Begin VB.TextBox txtRFIthreshold 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   505
            Text            =   "FLTINFO.frx":0588
            Top             =   390
            Width           =   435
         End
         Begin VB.TextBox txtMUXthreshold 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   504
            Text            =   "FLTINFO.frx":058E
            Top             =   720
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "RFI"
            Height          =   255
            Index           =   82
            Left            =   60
            TabIndex        =   507
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "MUX"
            Height          =   255
            Index           =   84
            Left            =   60
            TabIndex        =   506
            Top             =   720
            Width           =   375
         End
      End
      Begin VB.Frame Frame8 
         Caption         =   "Destination File to Put"
         Height          =   675
         Left            =   -72870
         TabIndex        =   482
         Top             =   3840
         Width           =   2085
         Begin VB.CommandButton cmdWritePath 
            Caption         =   "&Dest Path"
            Height          =   375
            Left            =   120
            TabIndex        =   484
            Top             =   240
            Width           =   855
         End
         Begin VB.CommandButton cmdWriteDefault 
            Caption         =   "Default"
            Height          =   375
            Left            =   1080
            TabIndex        =   483
            Top             =   240
            Width           =   855
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "Source File to Get"
         Height          =   675
         Left            =   -74940
         TabIndex        =   479
         Top             =   3840
         Width           =   2025
         Begin VB.CommandButton cmdReadFLTINFO 
            Caption         =   "&Src Path"
            Height          =   375
            Index           =   0
            Left            =   120
            TabIndex        =   481
            Top             =   240
            Width           =   855
         End
         Begin VB.CommandButton cmdReadFLTINFO 
            Caption         =   "Default"
            Height          =   375
            Index           =   1
            Left            =   1080
            TabIndex        =   480
            Top             =   240
            Width           =   855
         End
      End
      Begin VB.Frame Frame 
         BackColor       =   &H80000004&
         Caption         =   "RC Files"
         Height          =   4215
         Index           =   0
         Left            =   -74970
         TabIndex        =   358
         Top             =   330
         Width           =   8085
         Begin VB.ComboBox cboPackThreshold 
            Height          =   315
            Left            =   6465
            TabIndex        =   766
            Text            =   "100"
            Top             =   3855
            Width           =   690
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   44
            Left            =   7485
            MultiLine       =   -1  'True
            TabIndex        =   676
            Top             =   2850
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   44
            Left            =   6285
            TabIndex        =   675
            Top             =   2835
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   44
            Left            =   7185
            TabIndex        =   674
            Top             =   2850
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   43
            Left            =   7485
            MultiLine       =   -1  'True
            TabIndex        =   672
            Top             =   2520
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   43
            Left            =   6285
            TabIndex        =   671
            Top             =   2505
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   43
            Left            =   7185
            TabIndex        =   670
            Top             =   2520
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   42
            Left            =   7485
            MultiLine       =   -1  'True
            TabIndex        =   668
            Top             =   2190
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   42
            Left            =   6285
            TabIndex        =   667
            Top             =   2175
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   42
            Left            =   7185
            TabIndex        =   666
            Top             =   2190
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   36
            Left            =   7455
            MultiLine       =   -1  'True
            TabIndex        =   659
            Top             =   240
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   36
            Left            =   6270
            TabIndex        =   658
            Top             =   210
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   36
            Left            =   7170
            TabIndex        =   657
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   37
            Left            =   7455
            MultiLine       =   -1  'True
            TabIndex        =   656
            Top             =   540
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   37
            Left            =   6270
            TabIndex        =   655
            Top             =   540
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   37
            Left            =   7170
            TabIndex        =   654
            Top             =   540
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   38
            Left            =   7455
            MultiLine       =   -1  'True
            TabIndex        =   653
            Top             =   855
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   38
            Left            =   6270
            TabIndex        =   652
            Top             =   855
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   38
            Left            =   7170
            TabIndex        =   651
            Top             =   855
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   39
            Left            =   7455
            MultiLine       =   -1  'True
            TabIndex        =   650
            Top             =   1185
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   39
            Left            =   6270
            TabIndex        =   649
            Top             =   1185
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   39
            Left            =   7170
            TabIndex        =   648
            Top             =   1185
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   40
            Left            =   7470
            MultiLine       =   -1  'True
            TabIndex        =   647
            Top             =   1530
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   40
            Left            =   6285
            TabIndex        =   646
            Top             =   1515
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   40
            Left            =   7185
            TabIndex        =   645
            Top             =   1530
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   41
            Left            =   7485
            MultiLine       =   -1  'True
            TabIndex        =   644
            Top             =   1860
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   41
            Left            =   6285
            TabIndex        =   643
            Top             =   1845
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   41
            Left            =   7185
            TabIndex        =   642
            Top             =   1860
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   35
            Left            =   5460
            MultiLine       =   -1  'True
            TabIndex        =   640
            Top             =   3840
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   35
            Left            =   4260
            TabIndex        =   639
            Top             =   3840
            Width           =   900
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   35
            Left            =   5175
            TabIndex        =   638
            Top             =   3840
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   34
            Left            =   5460
            MultiLine       =   -1  'True
            TabIndex        =   636
            Top             =   3510
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   34
            Left            =   4275
            TabIndex        =   635
            Top             =   3510
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   34
            Left            =   5175
            TabIndex        =   634
            Top             =   3510
            Width           =   255
         End
         Begin VB.CommandButton cmdRCfiles 
            Caption         =   "Clear All"
            Height          =   315
            Index           =   3
            Left            =   6015
            TabIndex        =   566
            Top             =   3195
            Width           =   930
         End
         Begin VB.CommandButton cmdRCfiles 
            Caption         =   "Pack"
            Height          =   330
            Index           =   2
            Left            =   6015
            TabIndex        =   517
            ToolTipText     =   "Get rid of RC sets that had less than the number of hits in the text box below"
            Top             =   3510
            Width           =   930
         End
         Begin VB.CommandButton cmdRCfiles 
            Caption         =   "Clear Use"
            Height          =   315
            Index           =   1
            Left            =   7080
            TabIndex        =   516
            Top             =   3195
            Width           =   930
         End
         Begin VB.CommandButton cmdRCfiles 
            Caption         =   "Load All"
            Height          =   330
            Index           =   0
            Left            =   7080
            TabIndex        =   515
            Top             =   3525
            Width           =   930
         End
         Begin VB.TextBox txtNRC 
            Enabled         =   0   'False
            Height          =   285
            Left            =   7680
            TabIndex        =   501
            ToolTipText     =   "Number of RC sets"
            Top             =   3885
            Width           =   315
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   33
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   499
            Top             =   3180
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   33
            Left            =   4275
            TabIndex        =   498
            Top             =   3180
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   33
            Left            =   5160
            TabIndex        =   497
            Top             =   3180
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   32
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   495
            Top             =   2850
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   32
            Left            =   4275
            TabIndex        =   494
            Top             =   2850
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   32
            Left            =   5160
            TabIndex        =   493
            Top             =   2850
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   31
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   491
            Top             =   2520
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   31
            Left            =   4275
            TabIndex        =   490
            Top             =   2520
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   31
            Left            =   5160
            TabIndex        =   489
            Top             =   2520
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   30
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   487
            Top             =   2190
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   30
            Left            =   4275
            TabIndex        =   486
            Top             =   2190
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   30
            Left            =   5160
            TabIndex        =   485
            Top             =   2190
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   29
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   477
            Top             =   1860
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   29
            Left            =   4275
            TabIndex        =   476
            Top             =   1860
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   29
            Left            =   5160
            TabIndex        =   475
            Top             =   1860
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   28
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   473
            Top             =   1530
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   28
            Left            =   4275
            TabIndex        =   472
            Top             =   1530
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   28
            Left            =   5160
            TabIndex        =   471
            Top             =   1530
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   27
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   469
            Top             =   1200
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   27
            Left            =   4275
            TabIndex        =   468
            Top             =   1200
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   27
            Left            =   5160
            TabIndex        =   467
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   26
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   465
            Top             =   870
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   26
            Left            =   4275
            TabIndex        =   464
            Top             =   870
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   26
            Left            =   5160
            TabIndex        =   463
            Top             =   870
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   25
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   461
            Top             =   540
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   25
            Left            =   4275
            TabIndex        =   460
            Top             =   540
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   25
            Left            =   5160
            TabIndex        =   459
            Top             =   540
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   24
            Left            =   5445
            MultiLine       =   -1  'True
            TabIndex        =   457
            Top             =   225
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   24
            Left            =   4275
            TabIndex        =   456
            Top             =   210
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   24
            Left            =   5160
            TabIndex        =   455
            Top             =   210
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   23
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   453
            Top             =   3840
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   23
            Left            =   2280
            TabIndex        =   452
            Top             =   3840
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   23
            Left            =   3165
            TabIndex        =   451
            Top             =   3840
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   22
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   449
            Top             =   3510
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   22
            Left            =   2280
            TabIndex        =   448
            Top             =   3510
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   22
            Left            =   3165
            TabIndex        =   447
            Top             =   3510
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   21
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   445
            Top             =   3180
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   21
            Left            =   2280
            TabIndex        =   444
            Top             =   3180
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   21
            Left            =   3165
            TabIndex        =   443
            Top             =   3180
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   20
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   441
            Top             =   2850
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   20
            Left            =   2280
            TabIndex        =   440
            Top             =   2850
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   20
            Left            =   3165
            TabIndex        =   439
            Top             =   2850
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   19
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   437
            Top             =   2520
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   19
            Left            =   2280
            TabIndex        =   436
            Top             =   2520
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   19
            Left            =   3165
            TabIndex        =   435
            Top             =   2520
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   18
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   433
            Top             =   2190
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   18
            Left            =   2280
            TabIndex        =   432
            Top             =   2190
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   18
            Left            =   3165
            TabIndex        =   431
            Top             =   2190
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   17
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   429
            Top             =   1860
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   17
            Left            =   2280
            TabIndex        =   428
            Top             =   1860
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   17
            Left            =   3165
            TabIndex        =   427
            Top             =   1860
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   16
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   425
            Top             =   1530
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   16
            Left            =   2280
            TabIndex        =   424
            Top             =   1530
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   16
            Left            =   3165
            TabIndex        =   423
            Top             =   1530
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   0
            ItemData        =   "FLTINFO.frx":0594
            Left            =   300
            List            =   "FLTINFO.frx":0596
            TabIndex        =   406
            Top             =   210
            Width           =   870
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   1
            Left            =   285
            TabIndex        =   405
            Top             =   540
            Width           =   885
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   2
            Left            =   285
            TabIndex        =   404
            Top             =   870
            Width           =   885
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   3
            Left            =   285
            TabIndex        =   403
            Top             =   1200
            Width           =   885
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   4
            Left            =   285
            TabIndex        =   402
            Top             =   1530
            Width           =   885
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   5
            Left            =   285
            TabIndex        =   401
            Top             =   1860
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   0
            Left            =   1170
            TabIndex        =   400
            Top             =   210
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   1
            Left            =   1170
            TabIndex        =   399
            Top             =   540
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   2
            Left            =   1170
            TabIndex        =   398
            Top             =   870
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   3
            Left            =   1170
            TabIndex        =   397
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   4
            Left            =   1170
            TabIndex        =   396
            Top             =   1530
            Width           =   255
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   5
            Left            =   1170
            TabIndex        =   395
            Top             =   1860
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   5
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   394
            Top             =   1860
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   4
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   393
            Top             =   1530
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   3
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   392
            Top             =   1200
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   2
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   391
            Top             =   870
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   1
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   390
            Top             =   540
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   0
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   389
            Top             =   210
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   6
            Left            =   1170
            TabIndex        =   388
            Top             =   2190
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   6
            Left            =   285
            TabIndex        =   387
            Top             =   2190
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   6
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   386
            Top             =   2190
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   7
            Left            =   1170
            TabIndex        =   385
            Top             =   2520
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   7
            Left            =   285
            TabIndex        =   384
            Top             =   2520
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   7
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   383
            Top             =   2520
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   8
            Left            =   1170
            TabIndex        =   382
            Top             =   2850
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   8
            Left            =   285
            TabIndex        =   381
            Top             =   2850
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   8
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   380
            Top             =   2850
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   9
            Left            =   1170
            TabIndex        =   379
            Top             =   3180
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   9
            Left            =   285
            TabIndex        =   378
            Top             =   3180
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   9
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   377
            Top             =   3180
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   10
            Left            =   1170
            TabIndex        =   376
            Top             =   3510
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   10
            Left            =   285
            TabIndex        =   375
            Top             =   3510
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   10
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   374
            Top             =   3510
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   11
            Left            =   1170
            TabIndex        =   373
            Top             =   3840
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   11
            Left            =   285
            TabIndex        =   372
            Top             =   3840
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   11
            Left            =   1455
            MultiLine       =   -1  'True
            TabIndex        =   371
            Top             =   3840
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   12
            Left            =   3165
            TabIndex        =   370
            Top             =   210
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   12
            Left            =   2280
            TabIndex        =   369
            Top             =   210
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   12
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   368
            Top             =   210
            Width           =   495
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   13
            Left            =   3165
            TabIndex        =   367
            Top             =   540
            Width           =   255
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   13
            Left            =   2280
            TabIndex        =   366
            Top             =   540
            Width           =   885
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   13
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   365
            Top             =   540
            Width           =   495
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   14
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   364
            Top             =   870
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   14
            Left            =   2280
            TabIndex        =   363
            Top             =   870
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   14
            Left            =   3165
            TabIndex        =   362
            Top             =   870
            Width           =   255
         End
         Begin VB.TextBox txtRCuse 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   15
            Left            =   3450
            MultiLine       =   -1  'True
            TabIndex        =   361
            Top             =   1200
            Width           =   495
         End
         Begin VB.ComboBox cboReg 
            Height          =   315
            Index           =   15
            Left            =   2280
            TabIndex        =   360
            Top             =   1200
            Width           =   885
         End
         Begin VB.TextBox txtRCformat 
            Height          =   285
            Index           =   15
            Left            =   3165
            TabIndex        =   359
            Top             =   1200
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "Below"
            Height          =   195
            Index           =   163
            Left            =   6015
            TabIndex        =   764
            ToolTipText     =   "Get rid of RC sets that had less than the number of hits in the text box below"
            Top             =   3915
            Width           =   465
         End
         Begin VB.Label lblRC 
            Caption         =   "44"
            Height          =   255
            Index           =   44
            Left            =   6045
            TabIndex        =   677
            Top             =   2895
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "43"
            Height          =   255
            Index           =   43
            Left            =   6030
            TabIndex        =   673
            Top             =   2565
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "42"
            Height          =   255
            Index           =   42
            Left            =   6045
            TabIndex        =   669
            Top             =   2235
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "36"
            Height          =   255
            Index           =   36
            Left            =   6015
            TabIndex        =   665
            Top             =   255
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "37"
            Height          =   255
            Index           =   37
            Left            =   6030
            TabIndex        =   664
            Top             =   585
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "38"
            Height          =   255
            Index           =   38
            Left            =   6030
            TabIndex        =   663
            Top             =   915
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "39"
            Height          =   255
            Index           =   39
            Left            =   6030
            TabIndex        =   662
            Top             =   1245
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "40"
            Height          =   255
            Index           =   40
            Left            =   6045
            TabIndex        =   661
            Top             =   1560
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "41"
            Height          =   255
            Index           =   41
            Left            =   6045
            TabIndex        =   660
            Top             =   1905
            Width           =   255
         End
         Begin VB.Line Line4 
            X1              =   5970
            X2              =   5970
            Y1              =   195
            Y2              =   4125
         End
         Begin VB.Label lblRC 
            Caption         =   "35"
            Height          =   255
            Index           =   35
            Left            =   4020
            TabIndex        =   641
            Top             =   3870
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "34"
            Height          =   255
            Index           =   34
            Left            =   4035
            TabIndex        =   637
            Top             =   3555
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "NRC"
            Height          =   195
            Index           =   120
            Left            =   7245
            TabIndex        =   502
            ToolTipText     =   "Number of RC sets"
            Top             =   3930
            Width           =   375
         End
         Begin VB.Label lblRC 
            Caption         =   "33"
            Height          =   255
            Index           =   33
            Left            =   4035
            TabIndex        =   500
            Top             =   3225
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "32"
            Height          =   255
            Index           =   32
            Left            =   4035
            TabIndex        =   496
            Top             =   2910
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "31"
            Height          =   255
            Index           =   31
            Left            =   4035
            TabIndex        =   492
            Top             =   2565
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "30"
            Height          =   255
            Index           =   30
            Left            =   4035
            TabIndex        =   488
            Top             =   2235
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "29"
            Height          =   255
            Index           =   29
            Left            =   4035
            TabIndex        =   478
            Top             =   1905
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "28"
            Height          =   255
            Index           =   28
            Left            =   4035
            TabIndex        =   474
            Top             =   1575
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "27"
            Height          =   255
            Index           =   27
            Left            =   4035
            TabIndex        =   470
            Top             =   1245
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "26"
            Height          =   255
            Index           =   26
            Left            =   4035
            TabIndex        =   466
            Top             =   915
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "25"
            Height          =   165
            Index           =   25
            Left            =   4035
            TabIndex        =   462
            Top             =   585
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "24"
            Height          =   255
            Index           =   24
            Left            =   4035
            TabIndex        =   458
            Top             =   255
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "23"
            Height          =   255
            Index           =   23
            Left            =   2040
            TabIndex        =   454
            Top             =   3840
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "22"
            Height          =   255
            Index           =   22
            Left            =   2040
            TabIndex        =   450
            Top             =   3510
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "21"
            Height          =   255
            Index           =   21
            Left            =   2040
            TabIndex        =   446
            Top             =   3180
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "20"
            Height          =   255
            Index           =   20
            Left            =   2040
            TabIndex        =   442
            Top             =   2850
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "19"
            Height          =   195
            Index           =   19
            Left            =   2040
            TabIndex        =   438
            Top             =   2520
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "18"
            Height          =   255
            Index           =   18
            Left            =   2040
            TabIndex        =   434
            Top             =   2190
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "17"
            Height          =   195
            Index           =   17
            Left            =   2040
            TabIndex        =   430
            Top             =   1890
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "16"
            Height          =   255
            Index           =   16
            Left            =   2040
            TabIndex        =   426
            Top             =   1530
            Width           =   255
         End
         Begin VB.Line Line3 
            X1              =   3975
            X2              =   3975
            Y1              =   180
            Y2              =   4110
         End
         Begin VB.Label lblRC 
            BackColor       =   &H80000014&
            Caption         =   "0"
            ForeColor       =   &H00000000&
            Height          =   255
            Index           =   0
            Left            =   75
            TabIndex        =   422
            Top             =   210
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "1"
            Height          =   255
            Index           =   1
            Left            =   75
            TabIndex        =   421
            Top             =   540
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "2"
            Height          =   255
            Index           =   2
            Left            =   60
            TabIndex        =   420
            Top             =   870
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "3"
            Height          =   255
            Index           =   3
            Left            =   75
            TabIndex        =   419
            Top             =   1200
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "4"
            Height          =   255
            Index           =   4
            Left            =   75
            TabIndex        =   418
            Top             =   1530
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "5"
            Height          =   255
            Index           =   5
            Left            =   75
            TabIndex        =   417
            Top             =   1860
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "6"
            Height          =   255
            Index           =   6
            Left            =   75
            TabIndex        =   416
            Top             =   2190
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "7"
            Height          =   255
            Index           =   7
            Left            =   75
            TabIndex        =   415
            Top             =   2520
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "8"
            Height          =   255
            Index           =   8
            Left            =   75
            TabIndex        =   414
            Top             =   2850
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "9"
            Height          =   255
            Index           =   9
            Left            =   75
            TabIndex        =   413
            Top             =   3180
            Width           =   135
         End
         Begin VB.Label lblRC 
            Caption         =   "10"
            Height          =   255
            Index           =   10
            Left            =   75
            TabIndex        =   412
            Top             =   3510
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "11"
            Height          =   255
            Index           =   11
            Left            =   75
            TabIndex        =   411
            Top             =   3840
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "12"
            Height          =   255
            Index           =   12
            Left            =   2040
            TabIndex        =   410
            Top             =   210
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "13"
            Height          =   255
            Index           =   13
            Left            =   2040
            TabIndex        =   409
            Top             =   540
            Width           =   255
         End
         Begin VB.Line Line1 
            X1              =   1980
            X2              =   1980
            Y1              =   225
            Y2              =   4125
         End
         Begin VB.Label lblRC 
            Caption         =   "14"
            Height          =   255
            Index           =   14
            Left            =   2040
            TabIndex        =   408
            Top             =   870
            Width           =   255
         End
         Begin VB.Label lblRC 
            Caption         =   "15"
            Height          =   255
            Index           =   15
            Left            =   2040
            TabIndex        =   407
            Top             =   1200
            Width           =   255
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Make Word"
         Height          =   4125
         Index           =   9
         Left            =   -70440
         TabIndex        =   327
         Top             =   420
         Width           =   3495
         Begin VB.CommandButton cmdPC 
            Caption         =   "% Edited"
            Height          =   375
            Index           =   1
            Left            =   2580
            TabIndex        =   356
            Top             =   3660
            Width           =   825
         End
         Begin VB.TextBox txtScans 
            Height          =   285
            Index           =   1
            Left            =   1050
            TabIndex        =   355
            Top             =   3720
            Width           =   735
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   12
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   353
            Top             =   1350
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   13
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   352
            Top             =   1680
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   14
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   351
            Top             =   1980
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Index           =   9
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   350
            Top             =   420
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   315
            Index           =   10
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   349
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtPC 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   348
            Top             =   1035
            Width           =   615
         End
         Begin VB.TextBox txtRAWbadMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   346
            Top             =   1650
            Width           =   615
         End
         Begin VB.TextBox txtTtgtMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   345
            Top             =   1965
            Width           =   615
         End
         Begin VB.TextBox txtNavMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   344
            Top             =   420
            Width           =   615
         End
         Begin VB.TextBox txtTAmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   343
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtCtsMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   342
            Top             =   1020
            Width           =   615
         End
         Begin VB.TextBox txtCycleMax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   341
            Top             =   1335
            Width           =   615
         End
         Begin VB.TextBox txtRAWbadMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   339
            Top             =   1650
            Width           =   615
         End
         Begin VB.TextBox txtTtgtMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   338
            Top             =   1965
            Width           =   615
         End
         Begin VB.TextBox txtNavMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   337
            Top             =   420
            Width           =   615
         End
         Begin VB.TextBox txtTAmin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   336
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtCtsMIn 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   335
            Top             =   1020
            Width           =   615
         End
         Begin VB.TextBox txtCycleMin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1230
            MultiLine       =   -1  'True
            TabIndex        =   334
            Top             =   1335
            Width           =   615
         End
         Begin VB.CheckBox chkEditRAWbad 
            Caption         =   "RAW trops"
            Height          =   195
            Left            =   90
            TabIndex        =   333
            Top             =   1680
            Width           =   1080
         End
         Begin VB.CheckBox chkEditTtgt 
            Caption         =   "Ttgt"
            Height          =   195
            Left            =   90
            TabIndex        =   332
            Top             =   1995
            Width           =   645
         End
         Begin VB.CheckBox chkEditNav 
            Caption         =   "Nav"
            Height          =   195
            Left            =   90
            TabIndex        =   331
            Top             =   450
            Width           =   615
         End
         Begin VB.CheckBox chkEditTA 
            Caption         =   "TAs"
            Height          =   195
            Left            =   90
            TabIndex        =   330
            Top             =   750
            Width           =   915
         End
         Begin VB.CheckBox chkEditCts 
            Caption         =   "Counts"
            Height          =   195
            Left            =   90
            TabIndex        =   329
            Top             =   1050
            Width           =   915
         End
         Begin VB.CheckBox chkEditCycle 
            Caption         =   "Cycle"
            Height          =   195
            Left            =   90
            TabIndex        =   328
            Top             =   1365
            Width           =   735
         End
         Begin VB.Label Label 
            Caption         =   "Total Scans"
            Height          =   255
            Index           =   130
            Left            =   120
            TabIndex        =   357
            Top             =   3780
            Width           =   855
         End
         Begin VB.Label lblPC 
            Caption         =   "Counts"
            Height          =   255
            Index           =   1
            Left            =   2685
            TabIndex        =   354
            Top             =   180
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "Maximum"
            Height          =   255
            Index           =   129
            Left            =   1905
            TabIndex        =   347
            Top             =   180
            Width           =   660
         End
         Begin VB.Label Label 
            Caption         =   "Minimum"
            Height          =   255
            Index           =   128
            Left            =   1200
            TabIndex        =   340
            Top             =   180
            Width           =   615
         End
      End
      Begin VB.TextBox txtScans 
         Alignment       =   1  'Right Justify
         Enabled         =   0   'False
         Height          =   285
         Index           =   0
         Left            =   -67560
         MultiLine       =   -1  'True
         TabIndex        =   325
         Top             =   3300
         Width           =   615
      End
      Begin VB.Frame Frame 
         Caption         =   "UT Range"
         Height          =   2865
         Index           =   6
         Left            =   -69000
         TabIndex        =   311
         Top             =   360
         Width           =   2055
         Begin VB.CheckBox chkUTminmax 
            Caption         =   "UT Takeoff/Landing"
            Height          =   255
            Left            =   120
            TabIndex        =   318
            Top             =   2520
            Width           =   1815
         End
         Begin VB.TextBox txtUTtakeoff 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   317
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtUTlanding 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   316
            Top             =   1440
            Width           =   615
         End
         Begin VB.TextBox txtUTmax 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   315
            Top             =   720
            Width           =   615
         End
         Begin VB.TextBox txtUTmin 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   314
            Top             =   360
            Width           =   615
         End
         Begin VB.TextBox txtUTksmax 
            Height          =   285
            Left            =   1200
            TabIndex        =   313
            Top             =   2160
            Width           =   615
         End
         Begin VB.TextBox txtUTksmin 
            Height          =   285
            Left            =   1200
            TabIndex        =   312
            Top             =   1800
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "Takeoff (s)"
            Height          =   255
            Index           =   20
            Left            =   120
            TabIndex        =   324
            Top             =   1080
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Landing (s)"
            Height          =   255
            Index           =   21
            Left            =   120
            TabIndex        =   323
            Top             =   1440
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Maximum (s)"
            Height          =   255
            Index           =   102
            Left            =   120
            TabIndex        =   322
            Top             =   720
            Width           =   1095
         End
         Begin VB.Label Label 
            Caption         =   "Minimum (s)"
            Height          =   255
            Index           =   103
            Left            =   120
            TabIndex        =   321
            Top             =   360
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Used Min"
            Height          =   255
            Index           =   118
            Left            =   120
            TabIndex        =   320
            Top             =   1800
            Width           =   855
         End
         Begin VB.Label Label 
            Caption         =   "Used Max"
            Height          =   255
            Index           =   117
            Left            =   120
            TabIndex        =   319
            Top             =   2160
            Width           =   855
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "Fit Region"
         Height          =   885
         Index           =   8
         Left            =   930
         TabIndex        =   300
         Top             =   3675
         Width           =   1815
         Begin VB.TextBox txtYfit 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   304
            Top             =   540
            Width           =   555
         End
         Begin VB.TextBox txtYfit 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1200
            MultiLine       =   -1  'True
            TabIndex        =   303
            Top             =   240
            Width           =   555
         End
         Begin VB.TextBox txtXfit 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   330
            MultiLine       =   -1  'True
            TabIndex        =   302
            Top             =   540
            Width           =   555
         End
         Begin VB.TextBox txtXfit 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   330
            MultiLine       =   -1  'True
            TabIndex        =   301
            Top             =   240
            Width           =   555
         End
         Begin VB.Label Label 
            Caption         =   "Y2"
            Height          =   225
            Index           =   149
            Left            =   960
            TabIndex        =   308
            Top             =   570
            Width           =   225
         End
         Begin VB.Label Label 
            Caption         =   "Y1"
            Height          =   225
            Index           =   148
            Left            =   960
            TabIndex        =   307
            Top             =   270
            Width           =   225
         End
         Begin VB.Label Label 
            Caption         =   "X2"
            Height          =   225
            Index           =   147
            Left            =   90
            TabIndex        =   306
            Top             =   570
            Width           =   225
         End
         Begin VB.Label Label 
            Caption         =   "X1"
            Height          =   225
            Index           =   146
            Left            =   90
            TabIndex        =   305
            Top             =   270
            Width           =   225
         End
      End
      Begin VB.CommandButton cmdAsciiCalfile 
         Caption         =   "Write ASCII Calfile"
         Height          =   375
         Index           =   1
         Left            =   -71040
         TabIndex        =   298
         Top             =   1635
         Width           =   1935
      End
      Begin VB.CommandButton cmdAsciiCalfile 
         Caption         =   "Read ASCII Calfile"
         Height          =   360
         Index           =   0
         Left            =   -69000
         TabIndex        =   296
         Top             =   1635
         Width           =   2055
      End
      Begin VB.Frame Frame 
         Caption         =   "Use MA for"
         Height          =   1155
         Index           =   7
         Left            =   930
         TabIndex        =   290
         Top             =   2520
         Width           =   1395
         Begin VB.CheckBox chkUseMAforCSgain 
            Caption         =   "CSs"
            Height          =   255
            Left            =   690
            TabIndex        =   310
            Top             =   810
            Width           =   645
         End
         Begin VB.CheckBox chkUseMAforTifa 
            Caption         =   "Tifa"
            Height          =   255
            Left            =   690
            TabIndex        =   295
            Top             =   210
            Width           =   585
         End
         Begin VB.CheckBox chkUseMAforTtgt 
            Caption         =   "Ttgt"
            Height          =   255
            Left            =   690
            TabIndex        =   294
            Top             =   510
            Width           =   585
         End
         Begin VB.CheckBox chkUseMAforCN 
            Caption         =   "ND"
            Height          =   195
            Left            =   90
            TabIndex        =   293
            ToolTipText     =   "Use Moving Average for Noise Diode Deflection"
            Top             =   840
            Width           =   555
         End
         Begin VB.CheckBox chkUseMAforCS 
            Caption         =   "CSf"
            Height          =   195
            Left            =   90
            TabIndex        =   292
            ToolTipText     =   "Use Moving Average for Sky Counts"
            Top             =   540
            Width           =   585
         End
         Begin VB.CheckBox chkUseMAforCB 
            Caption         =   "CB"
            Height          =   195
            Left            =   90
            TabIndex        =   291
            ToolTipText     =   "Use Moving Average for Base Counts"
            Top             =   240
            Value           =   1  'Checked
            Width           =   525
         End
      End
      Begin VB.CheckBox chkUseDefaultCalfile 
         Caption         =   "Use Default Calfile"
         Height          =   255
         Left            =   -68640
         TabIndex        =   289
         ToolTipText     =   "Default is MISSION.CAL"
         Top             =   2280
         Value           =   1  'Checked
         Width           =   1695
      End
      Begin VB.CommandButton cmdReadNewCAL 
         Caption         =   "Read New Calfile"
         Height          =   375
         Index           =   0
         Left            =   -68640
         TabIndex        =   288
         Top             =   3120
         Width           =   1695
      End
      Begin VB.CommandButton cmdReadNewCAL 
         Caption         =   "Read Only WCT"
         Height          =   375
         Index           =   1
         Left            =   -68640
         TabIndex        =   287
         Top             =   3630
         Width           =   1695
      End
      Begin VB.Frame Frame 
         Caption         =   "Ch Info"
         Height          =   2025
         Index           =   5
         Left            =   120
         TabIndex        =   280
         Top             =   2520
         Width           =   795
         Begin VB.OptionButton optChInfo 
            Caption         =   "Rms"
            Height          =   255
            Index           =   2
            Left            =   60
            TabIndex        =   760
            Top             =   1695
            Width           =   630
         End
         Begin VB.OptionButton optChInfo 
            Caption         =   "Tau"
            Height          =   255
            Index           =   1
            Left            =   60
            TabIndex        =   759
            Top             =   1410
            Width           =   630
         End
         Begin VB.OptionButton optChInfo 
            Caption         =   "Man"
            Height          =   255
            Index           =   0
            Left            =   60
            TabIndex        =   758
            Top             =   1155
            Value           =   -1  'True
            Width           =   630
         End
         Begin VB.TextBox txtChInfo 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   240
            MultiLine       =   -1  'True
            TabIndex        =   283
            Top             =   810
            Width           =   435
         End
         Begin VB.TextBox txtChInfo 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   240
            MultiLine       =   -1  'True
            TabIndex        =   282
            Top             =   510
            Width           =   435
         End
         Begin VB.TextBox txtChInfo 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   240
            MultiLine       =   -1  'True
            TabIndex        =   281
            Top             =   210
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "3"
            Height          =   255
            Index           =   97
            Left            =   90
            TabIndex        =   286
            Top             =   810
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "2"
            Height          =   255
            Index           =   96
            Left            =   90
            TabIndex        =   285
            Top             =   510
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "1"
            Height          =   255
            Index           =   89
            Left            =   90
            TabIndex        =   284
            Top             =   210
            Width           =   135
         End
      End
      Begin VB.ComboBox cboGE 
         Height          =   315
         Index           =   0
         Left            =   240
         TabIndex        =   275
         Text            =   "Offset"
         Top             =   870
         Width           =   795
      End
      Begin VB.Frame Frame 
         Caption         =   "Gain Limits"
         Height          =   1815
         Index           =   1
         Left            =   -70560
         TabIndex        =   218
         Top             =   360
         Width           =   3615
         Begin VB.TextBox txtND2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   236
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtND2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   235
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtND2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   234
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtND1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   233
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtND1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   232
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtND1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   2520
            MultiLine       =   -1  'True
            TabIndex        =   231
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtNV2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   230
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtNV2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   229
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtNV2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   228
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtNV1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   227
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtNV1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   226
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtNV1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   225
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtGE2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   224
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtGE2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   223
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGE2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   840
            MultiLine       =   -1  'True
            TabIndex        =   222
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtGE1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   221
            Top             =   1440
            Width           =   495
         End
         Begin VB.TextBox txtGE1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   220
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtGE1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   360
            MultiLine       =   -1  'True
            TabIndex        =   219
            Top             =   720
            Width           =   495
         End
         Begin VB.Label Label1 
            Caption         =   "     Gain Equation     Nav OAT        Noise Diode"
            Height          =   255
            Left            =   120
            TabIndex        =   241
            Top             =   240
            Width           =   3375
         End
         Begin VB.Label Label 
            Caption         =   "3"
            Height          =   255
            Index           =   81
            Left            =   120
            TabIndex        =   240
            Top             =   1440
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "2"
            Height          =   255
            Index           =   80
            Left            =   120
            TabIndex        =   239
            Top             =   1080
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "1"
            Height          =   255
            Index           =   78
            Left            =   120
            TabIndex        =   238
            Top             =   720
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "Ch   Min     Max        Min    Max       Min     Max"
            Height          =   255
            Index           =   79
            Left            =   120
            TabIndex        =   237
            Top             =   480
            Width           =   3375
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "ND Ts"
         Height          =   1905
         Index           =   4
         Left            =   5940
         TabIndex        =   214
         Top             =   360
         Width           =   2175
         Begin VB.TextBox txtGnd 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1530
            MultiLine       =   -1  'True
            TabIndex        =   783
            Top             =   1560
            Width           =   555
         End
         Begin VB.TextBox txtGnd 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   782
            Top             =   1560
            Width           =   555
         End
         Begin VB.TextBox txtGnd 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   270
            MultiLine       =   -1  'True
            TabIndex        =   780
            Top             =   1560
            Width           =   555
         End
         Begin VB.TextBox txtTrefND 
            Height          =   285
            Left            =   1620
            TabIndex        =   249
            Text            =   "39.5"
            Top             =   150
            Width           =   465
         End
         Begin VB.TextBox txtCnd2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1530
            MultiLine       =   -1  'True
            TabIndex        =   247
            Top             =   1260
            Width           =   555
         End
         Begin VB.TextBox txtCnd2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   246
            Top             =   1260
            Width           =   555
         End
         Begin VB.TextBox txtCnd2 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   270
            MultiLine       =   -1  'True
            TabIndex        =   245
            Top             =   1260
            Width           =   555
         End
         Begin VB.TextBox txtCnd1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1530
            MultiLine       =   -1  'True
            TabIndex        =   244
            Top             =   960
            Width           =   555
         End
         Begin VB.TextBox txtCnd1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   243
            Top             =   960
            Width           =   555
         End
         Begin VB.TextBox txtCnd1 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   270
            MultiLine       =   -1  'True
            TabIndex        =   242
            Top             =   960
            Width           =   555
         End
         Begin VB.TextBox txtCnd0 
            Alignment       =   1  'Right Justify
            Height          =   315
            Index           =   2
            Left            =   1530
            MultiLine       =   -1  'True
            TabIndex        =   217
            Text            =   "FLTINFO.frx":0598
            Top             =   630
            Width           =   555
         End
         Begin VB.TextBox txtCnd0 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   900
            MultiLine       =   -1  'True
            TabIndex        =   216
            Text            =   "FLTINFO.frx":059A
            Top             =   660
            Width           =   555
         End
         Begin VB.TextBox txtCnd0 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   270
            MultiLine       =   -1  'True
            TabIndex        =   215
            Text            =   "FLTINFO.frx":059C
            Top             =   660
            Width           =   555
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 3"
            ForeColor       =   &H00FF0000&
            Height          =   255
            Index           =   164
            Left            =   1530
            TabIndex        =   786
            Top             =   420
            Width           =   555
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 2"
            ForeColor       =   &H8000000E&
            Height          =   255
            Index           =   161
            Left            =   900
            TabIndex        =   785
            Top             =   420
            Width           =   525
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 1"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   95
            Left            =   300
            TabIndex        =   784
            Top             =   420
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "G"
            Height          =   225
            Index           =   94
            Left            =   30
            TabIndex        =   779
            Top             =   1620
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "C2"
            Height          =   225
            Index           =   101
            Left            =   30
            TabIndex        =   252
            Top             =   1260
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "C1"
            Height          =   255
            Index           =   100
            Left            =   30
            TabIndex        =   251
            Top             =   960
            Width           =   255
         End
         Begin VB.Label Label 
            Caption         =   "C0"
            Height          =   255
            Index           =   99
            Left            =   30
            TabIndex        =   250
            Top             =   690
            Width           =   255
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Tref"
            Height          =   255
            Index           =   98
            Left            =   1260
            TabIndex        =   248
            Top             =   150
            Width           =   375
         End
      End
      Begin VB.Frame Frame 
         Caption         =   "T Offsets"
         Height          =   1905
         Index           =   3
         Left            =   3390
         TabIndex        =   205
         Top             =   360
         Width           =   2505
         Begin VB.TextBox txtTTO 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   788
            Text            =   "FLTINFO.frx":059E
            Top             =   360
            Width           =   465
         End
         Begin VB.TextBox txtNDoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1950
            MultiLine       =   -1  'True
            TabIndex        =   542
            Text            =   "FLTINFO.frx":05A0
            Top             =   1560
            Width           =   465
         End
         Begin VB.TextBox txtNDoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   541
            Text            =   "FLTINFO.frx":05A4
            Top             =   1560
            Width           =   465
         End
         Begin VB.TextBox txtNDoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   930
            MultiLine       =   -1  'True
            TabIndex        =   540
            Text            =   "FLTINFO.frx":05A8
            Top             =   1560
            Width           =   465
         End
         Begin VB.TextBox txtNAVoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1950
            MultiLine       =   -1  'True
            TabIndex        =   539
            Text            =   "FLTINFO.frx":05AC
            Top             =   1260
            Width           =   465
         End
         Begin VB.TextBox txtNAVoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   538
            Text            =   "FLTINFO.frx":05B3
            Top             =   1260
            Width           =   465
         End
         Begin VB.TextBox txtNAVoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   930
            MultiLine       =   -1  'True
            TabIndex        =   537
            Text            =   "FLTINFO.frx":05BA
            Top             =   1260
            Width           =   465
         End
         Begin VB.TextBox txtTGToffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1950
            MultiLine       =   -1  'True
            TabIndex        =   536
            Text            =   "FLTINFO.frx":05C1
            Top             =   960
            Width           =   465
         End
         Begin VB.TextBox txtTGToffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   535
            Text            =   "FLTINFO.frx":05C3
            Top             =   960
            Width           =   465
         End
         Begin VB.TextBox txtTGToffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   930
            MultiLine       =   -1  'True
            TabIndex        =   534
            Text            =   "FLTINFO.frx":05C5
            Top             =   960
            Width           =   465
         End
         Begin VB.TextBox txtMXRoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1950
            MultiLine       =   -1  'True
            TabIndex        =   533
            Text            =   "FLTINFO.frx":05C7
            Top             =   660
            Width           =   465
         End
         Begin VB.TextBox txtMXRoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1440
            MultiLine       =   -1  'True
            TabIndex        =   532
            Text            =   "FLTINFO.frx":05C9
            Top             =   660
            Width           =   465
         End
         Begin VB.TextBox txtMXRoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   930
            MultiLine       =   -1  'True
            TabIndex        =   531
            Text            =   "FLTINFO.frx":05CB
            Top             =   660
            Width           =   465
         End
         Begin VB.TextBox txtNDoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   213
            Text            =   "FLTINFO.frx":05CD
            Top             =   1560
            Width           =   465
         End
         Begin VB.TextBox txtTGToffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   211
            Text            =   "FLTINFO.frx":05D1
            Top             =   960
            Width           =   465
         End
         Begin VB.TextBox txtMXRoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   210
            Text            =   "FLTINFO.frx":05D3
            Top             =   660
            Width           =   465
         End
         Begin VB.TextBox txtNAVoffset 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   450
            MultiLine       =   -1  'True
            TabIndex        =   209
            Text            =   "FLTINFO.frx":05D5
            Top             =   1260
            Width           =   465
         End
         Begin VB.Label Label 
            Caption         =   "TTO"
            Height          =   165
            Index           =   165
            Left            =   60
            TabIndex        =   787
            Top             =   390
            Width           =   315
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 3"
            ForeColor       =   &H00FF0000&
            Height          =   255
            Index           =   157
            Left            =   1950
            TabIndex        =   545
            Top             =   360
            Width           =   465
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 2"
            ForeColor       =   &H8000000E&
            Height          =   255
            Index           =   156
            Left            =   1440
            TabIndex        =   544
            Top             =   360
            Width           =   435
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 1"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   155
            Left            =   930
            TabIndex        =   543
            Top             =   360
            Width           =   435
         End
         Begin VB.Label Label 
            Caption         =   "ND"
            Height          =   165
            Index           =   92
            Left            =   60
            TabIndex        =   212
            Top             =   1590
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "TGT"
            Height          =   165
            Index           =   49
            Left            =   30
            TabIndex        =   208
            Top             =   990
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "MXR"
            Height          =   165
            Index           =   48
            Left            =   60
            TabIndex        =   207
            Top             =   690
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "NAV"
            Height          =   165
            Index           =   83
            Left            =   60
            TabIndex        =   206
            Top             =   1290
            Width           =   375
         End
      End
      Begin VB.ComboBox cboGscale 
         Height          =   315
         Left            =   7260
         TabIndex        =   201
         Top             =   2310
         Width           =   825
      End
      Begin VB.ComboBox cboOATsource 
         Height          =   315
         Left            =   7260
         TabIndex        =   200
         Top             =   2640
         Width           =   825
      End
      Begin VB.ComboBox cboREFsource 
         Height          =   315
         Left            =   7260
         TabIndex        =   199
         Top             =   2970
         Width           =   825
      End
      Begin VB.TextBox txtMTPcor 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -73665
         MultiLine       =   -1  'True
         TabIndex        =   195
         Top             =   3555
         Width           =   495
      End
      Begin VB.TextBox txtOATnavCOR 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   7260
         MultiLine       =   -1  'True
         TabIndex        =   191
         Top             =   3300
         Width           =   570
      End
      Begin VB.Frame Frame10 
         Caption         =   "Gain Equation"
         Height          =   2175
         Index           =   0
         Left            =   120
         TabIndex        =   164
         Top             =   360
         Width           =   3255
         Begin VB.ComboBox cboGE 
            Height          =   315
            Index           =   4
            Left            =   120
            TabIndex        =   279
            Top             =   1830
            Width           =   795
         End
         Begin VB.ComboBox cboGE 
            Height          =   315
            Index           =   3
            Left            =   120
            TabIndex        =   278
            Top             =   1500
            Width           =   795
         End
         Begin VB.ComboBox cboGE 
            Height          =   315
            Index           =   2
            Left            =   120
            TabIndex        =   277
            Top             =   1170
            Width           =   795
         End
         Begin VB.ComboBox cboGE 
            Height          =   315
            Index           =   1
            Left            =   120
            TabIndex        =   276
            Text            =   "Tifa"
            Top             =   840
            Width           =   795
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   1590
            MultiLine       =   -1  'True
            TabIndex        =   185
            Text            =   "FLTINFO.frx":05DC
            Top             =   510
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   1590
            MultiLine       =   -1  'True
            TabIndex        =   184
            Text            =   "FLTINFO.frx":05E1
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   1590
            MultiLine       =   -1  'True
            TabIndex        =   183
            Text            =   "FLTINFO.frx":05E6
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   1590
            MultiLine       =   -1  'True
            TabIndex        =   182
            Text            =   "FLTINFO.frx":05EB
            Top             =   1500
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   1590
            MultiLine       =   -1  'True
            TabIndex        =   181
            Text            =   "FLTINFO.frx":05F0
            Top             =   1830
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   2130
            MultiLine       =   -1  'True
            TabIndex        =   180
            Text            =   "FLTINFO.frx":05F5
            Top             =   510
            Width           =   495
         End
         Begin VB.TextBox txtGEr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   179
            Text            =   "FLTINFO.frx":05FA
            Top             =   510
            Width           =   495
         End
         Begin VB.TextBox txtGEr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   178
            Text            =   "FLTINFO.frx":05FF
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGEr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   177
            Text            =   "FLTINFO.frx":0604
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtGEr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   176
            Text            =   "FLTINFO.frx":0609
            Top             =   1500
            Width           =   495
         End
         Begin VB.TextBox txtGEr 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   175
            Text            =   "FLTINFO.frx":060E
            Top             =   1830
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   2130
            MultiLine       =   -1  'True
            TabIndex        =   174
            Text            =   "FLTINFO.frx":0613
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   2130
            MultiLine       =   -1  'True
            TabIndex        =   173
            Text            =   "FLTINFO.frx":0618
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   2130
            MultiLine       =   -1  'True
            TabIndex        =   172
            Text            =   "FLTINFO.frx":061D
            Top             =   1500
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   2130
            MultiLine       =   -1  'True
            TabIndex        =   171
            Text            =   "FLTINFO.frx":0622
            Top             =   1830
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   170
            Text            =   "FLTINFO.frx":0627
            Top             =   510
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   169
            Text            =   "FLTINFO.frx":062F
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   168
            Text            =   "FLTINFO.frx":0634
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   167
            Text            =   "FLTINFO.frx":0639
            Top             =   1500
            Width           =   495
         End
         Begin VB.TextBox txtGEval 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   2670
            MultiLine       =   -1  'True
            TabIndex        =   166
            Text            =   "FLTINFO.frx":063E
            Top             =   1830
            Width           =   495
         End
         Begin VB.TextBox txtNfit 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   165
            Text            =   "FLTINFO.frx":0643
            Top             =   210
            Width           =   375
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 1"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   67
            Left            =   1620
            TabIndex        =   190
            Top             =   210
            Width           =   435
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 2"
            ForeColor       =   &H8000000E&
            Height          =   255
            Index           =   68
            Left            =   2160
            TabIndex        =   189
            Top             =   210
            Width           =   465
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Ch 3"
            ForeColor       =   &H00FF0000&
            Height          =   255
            Index           =   69
            Left            =   2700
            TabIndex        =   188
            Top             =   210
            Width           =   465
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   "  Tref"
            ForeColor       =   &H00000000&
            Height          =   255
            Index           =   70
            Left            =   990
            TabIndex        =   187
            Top             =   210
            Width           =   495
         End
         Begin VB.Label Label 
            BackColor       =   &H00808080&
            Caption         =   " Nfit"
            Height          =   255
            Index           =   73
            Left            =   120
            TabIndex        =   186
            Top             =   210
            Width           =   375
         End
      End
      Begin VB.CommandButton cmdWCT 
         Caption         =   "&Sub"
         Height          =   375
         Index           =   1
         Left            =   -72480
         TabIndex        =   161
         Top             =   1620
         Width           =   615
      End
      Begin VB.CommandButton cmdWCT 
         Caption         =   "&Add"
         Height          =   375
         Index           =   0
         Left            =   -72480
         TabIndex        =   160
         Top             =   1260
         Width           =   615
      End
      Begin VB.TextBox txtWCTdates 
         Height          =   285
         Left            =   -71760
         TabIndex        =   159
         Top             =   900
         Width           =   1095
      End
      Begin VB.TextBox txtDeltaTmin 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -71130
         MultiLine       =   -1  'True
         TabIndex        =   155
         Text            =   "FLTINFO.frx":0645
         Top             =   2220
         Width           =   465
      End
      Begin VB.ListBox lstWCTdates 
         Height          =   840
         ItemData        =   "FLTINFO.frx":064A
         Left            =   -71760
         List            =   "FLTINFO.frx":0651
         TabIndex        =   153
         Top             =   1260
         Width           =   1095
      End
      Begin VB.TextBox txtGenDate 
         Height          =   285
         Left            =   -71760
         TabIndex        =   151
         Text            =   "20000120"
         Top             =   540
         Width           =   1095
      End
      Begin VB.TextBox txtReflectivity 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   7260
         MultiLine       =   -1  'True
         TabIndex        =   149
         Text            =   "FLTINFO.frx":065F
         Top             =   4230
         Width           =   555
      End
      Begin VB.TextBox txtEmissivity 
         Alignment       =   1  'Right Justify
         Height          =   315
         Left            =   7260
         MultiLine       =   -1  'True
         TabIndex        =   148
         Text            =   "FLTINFO.frx":0665
         Top             =   3900
         Width           =   555
      End
      Begin VB.Frame Frame9 
         Caption         =   "WCT"
         Height          =   4095
         Left            =   -74880
         TabIndex        =   101
         Top             =   390
         Width           =   2295
         Begin VB.CheckBox chkUseZpDependence 
            Caption         =   "Use Zp Dependence"
            Height          =   255
            Left            =   180
            TabIndex        =   522
            Top             =   3270
            Width           =   1905
         End
         Begin VB.CheckBox chkEnableWCT 
            Alignment       =   1  'Right Justify
            Caption         =   "Enable WCT"
            Height          =   255
            Left            =   930
            TabIndex        =   299
            Top             =   195
            Value           =   1  'Checked
            Width           =   1245
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   131
            Text            =   "FLTINFO.frx":066B
            Top             =   690
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   130
            Top             =   930
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   129
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   128
            Top             =   1410
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   127
            Top             =   1650
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   126
            Top             =   1890
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   125
            Top             =   2130
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   124
            Top             =   2370
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   123
            Top             =   2610
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   480
            MultiLine       =   -1  'True
            TabIndex        =   122
            Top             =   2850
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   121
            Top             =   690
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   120
            Top             =   930
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   119
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   118
            Top             =   1410
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   117
            Top             =   1650
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   116
            Top             =   1890
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   115
            Top             =   2130
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   17
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   114
            Top             =   2370
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   18
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   113
            Top             =   2610
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   19
            Left            =   1080
            MultiLine       =   -1  'True
            TabIndex        =   112
            Top             =   2850
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   20
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   111
            Top             =   690
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   21
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   110
            Top             =   930
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   22
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   109
            Top             =   1170
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   23
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   108
            Top             =   1410
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   24
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   107
            Top             =   1650
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   25
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   106
            Top             =   1890
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   26
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   105
            Top             =   2130
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   27
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   104
            Top             =   2370
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   28
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   103
            Top             =   2610
            Width           =   495
         End
         Begin VB.TextBox txtWCT 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   29
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   102
            Top             =   2850
            Width           =   495
         End
         Begin VB.Label Label 
            BackColor       =   &H00C0C0C0&
            Caption         =   " Ch 1"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   50
            Left            =   480
            TabIndex        =   145
            Top             =   450
            Width           =   375
         End
         Begin VB.Label Label 
            BackColor       =   &H00C0C0C0&
            Caption         =   " Ch 2"
            ForeColor       =   &H8000000E&
            Height          =   255
            Index           =   51
            Left            =   1080
            TabIndex        =   144
            Top             =   450
            Width           =   495
         End
         Begin VB.Label Label 
            BackColor       =   &H00C0C0C0&
            Caption         =   " Ch 3"
            ForeColor       =   &H00FF0000&
            Height          =   255
            Index           =   52
            Left            =   1680
            TabIndex        =   143
            Top             =   450
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "Loc"
            Height          =   255
            Index           =   53
            Left            =   120
            TabIndex        =   142
            Top             =   450
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "1"
            Height          =   255
            Index           =   54
            Left            =   240
            TabIndex        =   141
            Top             =   690
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "2"
            Height          =   255
            Index           =   55
            Left            =   240
            TabIndex        =   140
            Top             =   930
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "3"
            Height          =   255
            Index           =   56
            Left            =   240
            TabIndex        =   139
            Top             =   1170
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "4"
            Height          =   255
            Index           =   57
            Left            =   240
            TabIndex        =   138
            Top             =   1410
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "5"
            Height          =   255
            Index           =   58
            Left            =   240
            TabIndex        =   137
            Top             =   1650
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "6"
            Height          =   255
            Index           =   59
            Left            =   240
            TabIndex        =   136
            Top             =   1890
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "7"
            Height          =   255
            Index           =   60
            Left            =   240
            TabIndex        =   135
            Top             =   2130
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "8"
            Height          =   255
            Index           =   61
            Left            =   240
            TabIndex        =   134
            Top             =   2370
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "9"
            Height          =   255
            Index           =   62
            Left            =   240
            TabIndex        =   133
            Top             =   2610
            Width           =   135
         End
         Begin VB.Label Label 
            Caption         =   "10"
            Height          =   255
            Index           =   63
            Left            =   120
            TabIndex        =   132
            Top             =   2850
            Width           =   255
         End
      End
      Begin VB.CheckBox chkAutoLoad 
         Caption         =   "Auto Load IN File"
         Height          =   195
         Left            =   -74040
         TabIndex        =   100
         ToolTipText     =   "Use to Auto Load IN File after Flight Change"
         Top             =   1830
         Width           =   1815
      End
      Begin VB.Frame Frame3 
         Caption         =   "Stats"
         Height          =   1095
         Left            =   -74925
         TabIndex        =   88
         Top             =   2415
         Width           =   1815
         Begin VB.TextBox txtMRIavg 
            Enabled         =   0   'False
            Height          =   285
            Left            =   720
            TabIndex        =   92
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtMRIrms 
            Enabled         =   0   'False
            Height          =   285
            Left            =   1200
            TabIndex        =   91
            Top             =   720
            Width           =   495
         End
         Begin VB.TextBox txtDTrms 
            Enabled         =   0   'False
            Height          =   285
            Left            =   1200
            TabIndex        =   90
            Top             =   360
            Width           =   495
         End
         Begin VB.TextBox txtDTavg 
            Enabled         =   0   'False
            Height          =   285
            Left            =   720
            TabIndex        =   89
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "MRI"
            Height          =   255
            Index           =   18
            Left            =   120
            TabIndex        =   95
            Top             =   720
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "DT [s]"
            Height          =   255
            Index           =   17
            Left            =   120
            TabIndex        =   94
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "AVG     RMS"
            Height          =   255
            Index           =   19
            Left            =   720
            TabIndex        =   93
            Top             =   120
            Width           =   975
         End
      End
      Begin VB.ComboBox cboSU 
         Height          =   315
         Left            =   -74040
         TabIndex        =   87
         Top             =   900
         Width           =   1005
      End
      Begin VB.CheckBox chkSubstitute 
         Caption         =   "Replace missing trops with the following:"
         Height          =   195
         Left            =   -74880
         TabIndex        =   85
         Top             =   2160
         Width           =   5895
      End
      Begin VB.TextBox txtTzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -70800
         TabIndex        =   84
         Top             =   2460
         Width           =   615
      End
      Begin VB.TextBox txtTzt1 
         Height          =   285
         Index           =   4
         Left            =   -72360
         TabIndex        =   83
         Top             =   2460
         Width           =   615
      End
      Begin VB.TextBox txtOATzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -71400
         TabIndex        =   82
         Top             =   2460
         Width           =   495
      End
      Begin VB.TextBox txtOATzt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   4
         Left            =   -72960
         TabIndex        =   81
         Top             =   2460
         Width           =   495
      End
      Begin VB.CommandButton cmdFlight 
         Caption         =   "&Set  Default"
         Height          =   375
         Index           =   5
         Left            =   -67920
         TabIndex        =   80
         Top             =   420
         Width           =   1035
      End
      Begin VB.CommandButton cmdFlight 
         Caption         =   "&Add Flight"
         Height          =   375
         Index           =   0
         Left            =   -69000
         TabIndex        =   79
         Top             =   420
         Width           =   1035
      End
      Begin VB.ComboBox cboObjectives 
         Height          =   315
         Left            =   -74040
         TabIndex        =   78
         Text            =   "TBD"
         Top             =   1260
         Width           =   4935
      End
      Begin VB.ComboBox cboFltNos 
         Height          =   315
         Left            =   -70320
         TabIndex        =   73
         Text            =   "19999999"
         Top             =   900
         Width           =   1215
      End
      Begin VB.ComboBox cboFltDates 
         Height          =   315
         Left            =   -70320
         TabIndex        =   72
         Text            =   "19999999"
         Top             =   480
         Width           =   1215
      End
      Begin VB.ComboBox cboMissions 
         Height          =   315
         Left            =   -72360
         TabIndex        =   71
         Text            =   "SUCCESS"
         Top             =   480
         Width           =   1335
      End
      Begin VB.ComboBox cboPlatforms 
         Height          =   315
         Left            =   -74040
         TabIndex        =   70
         Text            =   "WB57"
         Top             =   480
         Width           =   1005
      End
      Begin VB.TextBox txtTotalCycles 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -67800
         MultiLine       =   -1  'True
         TabIndex        =   61
         Top             =   2850
         Width           =   855
      End
      Begin VB.TextBox txtSourceFile 
         Enabled         =   0   'False
         Height          =   285
         Left            =   -74040
         TabIndex        =   44
         Top             =   2400
         Width           =   4935
      End
      Begin VB.TextBox txtOATks2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -69120
         TabIndex        =   43
         Top             =   1800
         Width           =   495
      End
      Begin VB.TextBox txtOATks1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -69840
         TabIndex        =   42
         Top             =   1800
         Width           =   510
      End
      Begin VB.TextBox txtOATzt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -72960
         TabIndex        =   41
         Top             =   1800
         Width           =   495
      End
      Begin VB.TextBox txtOATks2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -69120
         TabIndex        =   40
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtOATks1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -69840
         TabIndex        =   39
         Top             =   1440
         Width           =   510
      End
      Begin VB.TextBox txtOATzt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -72960
         TabIndex        =   38
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtOATks1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69840
         TabIndex        =   37
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtOATks2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69120
         TabIndex        =   36
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtOATzt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -72960
         TabIndex        =   35
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtOATzt1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -72960
         TabIndex        =   34
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtOATzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -71400
         TabIndex        =   33
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtOATzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -71400
         TabIndex        =   32
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtOATzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -71400
         TabIndex        =   31
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtOATzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -71400
         TabIndex        =   30
         Top             =   1800
         Width           =   495
      End
      Begin VB.CheckBox chkOATtrop 
         Caption         =   "OAT Trops?"
         Height          =   255
         Left            =   -74880
         TabIndex        =   29
         Top             =   360
         Width           =   1215
      End
      Begin VB.TextBox txtOATks1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -69840
         TabIndex        =   28
         Top             =   720
         Width           =   495
      End
      Begin VB.TextBox txtOATks2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -69120
         TabIndex        =   27
         Top             =   1080
         Width           =   495
      End
      Begin VB.TextBox txtTzt1 
         Height          =   285
         Index           =   0
         Left            =   -72360
         TabIndex        =   26
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtTzt1 
         Height          =   285
         Index           =   1
         Left            =   -72360
         TabIndex        =   25
         Top             =   1080
         Width           =   615
      End
      Begin VB.TextBox txtTzt1 
         Height          =   285
         Index           =   2
         Left            =   -72360
         TabIndex        =   24
         Top             =   1440
         Width           =   615
      End
      Begin VB.TextBox txtTzt1 
         Height          =   285
         Index           =   3
         Left            =   -72360
         TabIndex        =   23
         Top             =   1800
         Width           =   615
      End
      Begin VB.TextBox txtTzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -70800
         TabIndex        =   22
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtTzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -70800
         TabIndex        =   21
         Top             =   1080
         Width           =   615
      End
      Begin VB.TextBox txtTzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -70800
         TabIndex        =   20
         Top             =   1440
         Width           =   615
      End
      Begin VB.TextBox txtTzt2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   3
         Left            =   -70800
         TabIndex        =   19
         Top             =   1800
         Width           =   615
      End
      Begin VB.Frame Frame6 
         Caption         =   "Program Time Stamps"
         Height          =   3255
         Left            =   -74880
         TabIndex        =   12
         Top             =   360
         Width           =   3105
         Begin VB.TextBox txtEXEver 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   789
            Top             =   1050
            Width           =   1725
         End
         Begin VB.TextBox txtFIver 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   67
            Top             =   1770
            Width           =   1725
         End
         Begin VB.TextBox txtEDITver 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   15
            Top             =   1410
            Width           =   1725
         End
         Begin VB.TextBox txtMAKEver 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   14
            Top             =   720
            Width           =   1725
         End
         Begin VB.TextBox txtCALver 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   1260
            MultiLine       =   -1  'True
            TabIndex        =   13
            Top             =   360
            Width           =   1725
         End
         Begin VB.Label Label 
            Caption         =   "MTPbin EXE"
            Height          =   255
            Index           =   166
            Left            =   120
            TabIndex        =   790
            Top             =   1050
            Width           =   1065
         End
         Begin VB.Label Label 
            Caption         =   "FLTINFO"
            Height          =   255
            Index           =   32
            Left            =   120
            TabIndex        =   66
            Top             =   1770
            Width           =   675
         End
         Begin VB.Label Label 
            Caption         =   "EDIT Version"
            Height          =   255
            Index           =   31
            Left            =   120
            TabIndex        =   18
            Top             =   1440
            Width           =   1035
         End
         Begin VB.Label Label 
            Caption         =   "MTPbin VBP"
            Height          =   255
            Index           =   30
            Left            =   120
            TabIndex        =   17
            Top             =   720
            Width           =   1065
         End
         Begin VB.Label Label 
            Caption         =   "Calfile Date"
            Height          =   255
            Index           =   29
            Left            =   120
            TabIndex        =   16
            Top             =   360
            Width           =   825
         End
      End
      Begin VB.Frame Frame7 
         Caption         =   "File Time Stamps"
         Height          =   3255
         Left            =   -71640
         TabIndex        =   3
         Top             =   360
         Width           =   2535
         Begin VB.TextBox txtMMSstamp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   69
            Top             =   720
            Width           =   1695
         End
         Begin VB.TextBox txtRAWstamp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   7
            Top             =   360
            Width           =   1695
         End
         Begin VB.TextBox txtREFstamp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   6
            Top             =   1080
            Width           =   1695
         End
         Begin VB.TextBox txtERFstamp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   5
            Top             =   1440
            Width           =   1695
         End
         Begin VB.TextBox txtCTCstamp 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   4
            Top             =   1800
            Width           =   1695
         End
         Begin VB.Label Label 
            Caption         =   "MMS"
            Height          =   255
            Index           =   34
            Left            =   120
            TabIndex        =   68
            Top             =   720
            Width           =   375
         End
         Begin VB.Label Label 
            Caption         =   "RAW"
            Height          =   255
            Index           =   33
            Left            =   120
            TabIndex        =   11
            Top             =   360
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "REF"
            Height          =   255
            Index           =   35
            Left            =   120
            TabIndex        =   10
            Top             =   1080
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "ERF"
            Height          =   255
            Index           =   36
            Left            =   120
            TabIndex        =   9
            Top             =   1440
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "CTC"
            Height          =   255
            Index           =   37
            Left            =   120
            TabIndex        =   8
            Top             =   1800
            Width           =   735
         End
      End
      Begin VB.TextBox txtPI 
         Height          =   285
         Left            =   -74040
         TabIndex        =   2
         Text            =   "MJ Mahoney (Michael.J.Mahoney@jpl.nasa.gov)"
         Top             =   2070
         Width           =   7095
      End
      Begin VB.TextBox txtDestinationFile 
         Enabled         =   0   'False
         Height          =   285
         Left            =   -74040
         TabIndex        =   1
         Top             =   2790
         Width           =   4935
      End
      Begin VB.Label Label 
         Caption         =   "Default Mission RCs are not being used"
         Height          =   195
         Index           =   167
         Left            =   -71220
         TabIndex        =   792
         Top             =   3300
         Width           =   4305
      End
      Begin VB.Label Label 
         Caption         =   "RAW File"
         Height          =   165
         Index           =   143
         Left            =   -68550
         TabIndex        =   778
         Top             =   2820
         Width           =   735
      End
      Begin VB.Label Label4 
         Caption         =   "Destination"
         Height          =   225
         Left            =   -74910
         TabIndex        =   776
         Top             =   2760
         Width           =   855
      End
      Begin VB.Label Label3 
         Caption         =   "Source"
         Height          =   165
         Left            =   -74880
         TabIndex        =   775
         Top             =   2430
         Width           =   765
      End
      Begin VB.Label Label 
         Caption         =   "Use MTPsetup to change these parameters"
         Height          =   195
         Index           =   159
         Left            =   -71220
         TabIndex        =   774
         Top             =   3030
         Width           =   4305
      End
      Begin VB.Label Label 
         Caption         =   "MTPdest2"
         Height          =   195
         Index           =   133
         Left            =   -72165
         TabIndex        =   772
         Top             =   4230
         Width           =   915
      End
      Begin VB.Label Label 
         Caption         =   "DataSource"
         Height          =   195
         Index           =   132
         Left            =   -72165
         TabIndex        =   770
         Top             =   3555
         Width           =   885
      End
      Begin VB.Label Label 
         Caption         =   "MTPdest1"
         Height          =   195
         Index           =   131
         Left            =   -72165
         TabIndex        =   769
         Top             =   3915
         Width           =   885
      End
      Begin VB.Label Label2 
         Caption         =   "Pcorr (hPa)"
         Height          =   210
         Left            =   6210
         TabIndex        =   762
         Top             =   3645
         Width           =   795
      End
      Begin VB.Label Label 
         Caption         =   "RC Path"
         Height          =   165
         Index           =   145
         Left            =   -74790
         TabIndex        =   577
         Top             =   4290
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "USE File"
         Height          =   255
         Index           =   121
         Left            =   -68520
         TabIndex        =   574
         Top             =   2460
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "ATP Range (km)"
         Height          =   195
         Index           =   162
         Left            =   -74880
         TabIndex        =   564
         Top             =   3915
         Width           =   1185
      End
      Begin VB.Label Label 
         Caption         =   "Counts Channels"
         Height          =   210
         Index           =   160
         Left            =   -74880
         TabIndex        =   560
         Top             =   4260
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "Reference Targets"
         Height          =   255
         Index           =   158
         Left            =   -72510
         TabIndex        =   558
         Top             =   2580
         Width           =   1365
      End
      Begin VB.Label Label 
         Caption         =   "Tmtp -Tnav"
         Height          =   195
         Index           =   151
         Left            =   2775
         TabIndex        =   519
         Top             =   4260
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Channels"
         Height          =   255
         Index           =   104
         Left            =   4620
         TabIndex        =   514
         Top             =   3990
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "El Angles"
         Height          =   195
         Index           =   105
         Left            =   4620
         TabIndex        =   513
         Top             =   4260
         Width           =   675
      End
      Begin VB.Label Label 
         Caption         =   "LocHor"
         Height          =   255
         Index           =   141
         Left            =   4650
         TabIndex        =   510
         ToolTipText     =   "Scan location of horizon view"
         Top             =   3720
         Width           =   555
      End
      Begin VB.Label Label 
         Caption         =   "Total Scans"
         Height          =   255
         Index           =   87
         Left            =   -69000
         TabIndex        =   326
         Top             =   3300
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "K"
         Height          =   255
         Index           =   150
         Left            =   7920
         TabIndex        =   309
         Top             =   3330
         Width           =   105
      End
      Begin VB.Label Label 
         Caption         =   "Gen Date"
         Height          =   255
         Index           =   144
         Left            =   -72480
         TabIndex        =   297
         Top             =   600
         Width           =   735
      End
      Begin VB.Line Line2 
         X1              =   6060
         X2              =   6060
         Y1              =   2370
         Y2              =   4440
      End
      Begin VB.Label Label 
         Caption         =   "Delta T Min (K)"
         Height          =   255
         Index           =   140
         Left            =   -72510
         TabIndex        =   274
         Top             =   2280
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "OATnavCor"
         Height          =   195
         Index           =   139
         Left            =   6210
         TabIndex        =   273
         Top             =   3330
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Ref Source"
         Height          =   195
         Index           =   138
         Left            =   6210
         TabIndex        =   272
         Top             =   3030
         Width           =   825
      End
      Begin VB.Label Label 
         Caption         =   "OAT Source"
         Height          =   195
         Index           =   137
         Left            =   6210
         TabIndex        =   271
         Top             =   2700
         Width           =   885
      End
      Begin VB.Label Label 
         Caption         =   "Gain Scale"
         Height          =   195
         Index           =   136
         Left            =   6210
         TabIndex        =   270
         Top             =   2370
         Width           =   795
      End
      Begin VB.Label Label 
         Caption         =   "Reflectivity"
         Height          =   195
         Index           =   135
         Left            =   6210
         TabIndex        =   269
         Top             =   4260
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Emissivity"
         Height          =   195
         Index           =   134
         Left            =   6210
         TabIndex        =   268
         Top             =   3930
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "ts(ks)"
         Height          =   255
         Index           =   127
         Left            =   -69120
         TabIndex        =   267
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "t1(ks)"
         Height          =   255
         Index           =   126
         Left            =   -69840
         TabIndex        =   266
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Tzt2(K)"
         Height          =   255
         Index           =   125
         Left            =   -70800
         TabIndex        =   265
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Zt2(km)"
         Height          =   255
         Index           =   124
         Left            =   -71400
         TabIndex        =   264
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Tzt1(K)"
         Height          =   255
         Index           =   123
         Left            =   -72360
         TabIndex        =   263
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Zt1(km)"
         Height          =   255
         Index           =   122
         Left            =   -72960
         TabIndex        =   262
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Records"
         Height          =   165
         Index           =   119
         Left            =   -68520
         TabIndex        =   261
         Top             =   2925
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "PI String"
         Height          =   255
         Index           =   116
         Left            =   -74880
         TabIndex        =   260
         Top             =   2070
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Objective"
         Height          =   255
         Index           =   115
         Left            =   -74880
         TabIndex        =   259
         Top             =   1320
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Flt No"
         Height          =   255
         Index           =   114
         Left            =   -70920
         TabIndex        =   258
         Top             =   960
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Sensor"
         Height          =   255
         Index           =   113
         Left            =   -74880
         TabIndex        =   257
         Top             =   960
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Flt Date"
         Height          =   255
         Index           =   112
         Left            =   -70920
         TabIndex        =   256
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Mission"
         Height          =   255
         Index           =   111
         Left            =   -72960
         TabIndex        =   255
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Platform"
         Height          =   255
         Index           =   110
         Left            =   -74880
         TabIndex        =   254
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Clock Corr (s)"
         Height          =   255
         Index           =   106
         Left            =   -74895
         TabIndex        =   253
         Top             =   3600
         Width           =   1050
      End
      Begin VB.Label Label 
         Caption         =   "OAT Source"
         Height          =   255
         Index           =   1
         Left            =   -69120
         TabIndex        =   204
         Top             =   2640
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Gain Scale"
         Height          =   255
         Index           =   41
         Left            =   -69120
         TabIndex        =   203
         Top             =   2280
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "REF Source"
         Height          =   255
         Index           =   42
         Left            =   -69120
         TabIndex        =   202
         Top             =   3000
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "BadCycles"
         Height          =   255
         Index           =   91
         Left            =   -68520
         TabIndex        =   198
         Top             =   2280
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "RFI Iterations"
         Height          =   255
         Index           =   90
         Left            =   -68520
         TabIndex        =   197
         Top             =   1920
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Clock Corr [s]"
         Height          =   255
         Index           =   3
         Left            =   -68520
         TabIndex        =   196
         Top             =   1200
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Channels"
         Height          =   255
         Index           =   16
         Left            =   -68520
         TabIndex        =   194
         Top             =   480
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Elev Angles"
         Height          =   255
         Index           =   64
         Left            =   -68520
         TabIndex        =   193
         Top             =   840
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "OATnavCOR [K]"
         Height          =   255
         Index           =   2
         Left            =   -69120
         TabIndex        =   192
         Top             =   3360
         Width           =   1215
      End
      Begin VB.Label Label 
         Caption         =   "CMA Cycles"
         Height          =   255
         Index           =   85
         Left            =   -68520
         TabIndex        =   163
         Top             =   1560
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Flt No"
         Height          =   255
         Index           =   0
         Left            =   -71040
         TabIndex        =   162
         Top             =   900
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "PitchMax (deg)"
         Height          =   255
         Index           =   77
         Left            =   -72480
         TabIndex        =   158
         Top             =   2940
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Nsamples"
         Height          =   255
         Index           =   76
         Left            =   -72480
         TabIndex        =   157
         Top             =   3300
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "PitchMin (deg)"
         Height          =   255
         Index           =   75
         Left            =   -72480
         TabIndex        =   156
         Top             =   2580
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "DeltaTmin (K)"
         Height          =   255
         Index           =   74
         Left            =   -72480
         TabIndex        =   154
         Top             =   2220
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "WCTdate"
         Height          =   255
         Index           =   72
         Left            =   -72480
         TabIndex        =   152
         Top             =   900
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "GenDate"
         Height          =   255
         Index           =   71
         Left            =   -72480
         TabIndex        =   150
         Top             =   540
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Reflectivity"
         Height          =   255
         Index           =   66
         Left            =   -74880
         TabIndex        =   147
         Top             =   3360
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Emissivity"
         Height          =   255
         Index           =   65
         Left            =   -74880
         TabIndex        =   146
         Top             =   3120
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "UT Used"
         Height          =   255
         Index           =   47
         Left            =   -74880
         TabIndex        =   99
         Top             =   2100
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "UT Range"
         Height          =   255
         Index           =   46
         Left            =   -74880
         TabIndex        =   98
         Top             =   1740
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "NRC"
         Height          =   255
         Index           =   12
         Left            =   -72840
         TabIndex        =   97
         Top             =   2640
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "RC Path"
         Height          =   255
         Index           =   8
         Left            =   -74880
         TabIndex        =   96
         Top             =   3300
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Sensor"
         Height          =   255
         Index           =   6
         Left            =   -74880
         TabIndex        =   86
         Top             =   900
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Program Drive"
         Height          =   255
         Index           =   43
         Left            =   -74880
         TabIndex        =   77
         Top             =   540
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Data Drive"
         Height          =   255
         Index           =   44
         Left            =   -74880
         TabIndex        =   76
         Top             =   900
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "User Initials"
         Height          =   255
         Index           =   45
         Left            =   -74880
         TabIndex        =   75
         Top             =   1260
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Purpose"
         Height          =   255
         Index           =   4
         Left            =   -74880
         TabIndex        =   74
         Top             =   1320
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Total Scans"
         Height          =   255
         Index           =   40
         Left            =   -69480
         TabIndex        =   65
         Top             =   480
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Maximum"
         Height          =   255
         Index           =   39
         Left            =   -71640
         TabIndex        =   64
         Top             =   480
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Minimum"
         Height          =   255
         Index           =   38
         Left            =   -72600
         TabIndex        =   63
         Top             =   480
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Records"
         Height          =   255
         Index           =   15
         Left            =   -67800
         TabIndex        =   62
         Top             =   3000
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Flt Date"
         Height          =   255
         Index           =   11
         Left            =   -71040
         TabIndex        =   60
         Top             =   480
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "Mission"
         Height          =   255
         Index           =   10
         Left            =   -73080
         TabIndex        =   59
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Platform"
         Height          =   255
         Index           =   9
         Left            =   -74880
         TabIndex        =   58
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "USE File"
         Height          =   255
         Index           =   5
         Left            =   -70200
         TabIndex        =   57
         Top             =   2880
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Zt1[km]"
         Height          =   255
         Index           =   23
         Left            =   -72960
         TabIndex        =   56
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label25 
         Caption         =   "Dip 2"
         Height          =   255
         Left            =   -73680
         TabIndex        =   55
         Top             =   1800
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "t2 [ks]"
         Height          =   255
         Index           =   28
         Left            =   -69120
         TabIndex        =   54
         Top             =   480
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "t1 [ks]"
         Height          =   255
         Index           =   27
         Left            =   -69840
         TabIndex        =   53
         Top             =   480
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Zt2 [km]"
         Height          =   255
         Index           =   25
         Left            =   -71400
         TabIndex        =   52
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label21 
         Caption         =   "Dip 1"
         Height          =   255
         Left            =   -73680
         TabIndex        =   51
         Top             =   1440
         Width           =   495
      End
      Begin VB.Label Label11 
         Caption         =   "Descent"
         Height          =   255
         Left            =   -73680
         TabIndex        =   50
         Top             =   1080
         Width           =   615
      End
      Begin VB.Label Label10 
         Caption         =   "Ascent"
         Height          =   255
         Left            =   -73680
         TabIndex        =   49
         Top             =   720
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Tzt1 [K]"
         Height          =   255
         Index           =   24
         Left            =   -72240
         TabIndex        =   48
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Tzt2 [K]"
         Height          =   255
         Index           =   26
         Left            =   -70680
         TabIndex        =   47
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "PI$"
         Height          =   255
         Index           =   14
         Left            =   -74880
         TabIndex        =   46
         Top             =   2400
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "OUT File"
         Height          =   255
         Index           =   13
         Left            =   -74880
         TabIndex        =   45
         Top             =   3360
         Width           =   735
      End
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   5640
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.OLE OLE1 
      Height          =   1140
      Left            =   2610
      TabIndex        =   793
      Top             =   1710
      Width           =   3030
   End
   Begin VB.Label Label 
      BackColor       =   &H00808080&
      Caption         =   " Ch 1"
      ForeColor       =   &H000000FF&
      Height          =   255
      Index           =   93
      Left            =   6420
      TabIndex        =   781
      Top             =   750
      Width           =   435
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileReadFLTINFO 
         Caption         =   "&ReadFLTINFO"
      End
      Begin VB.Menu mnuWriteFLTINFO 
         Caption         =   "&WriteFLTINFO"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmFLTINFO"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public ShowPC As Boolean
Function fLoadMissionRCs(RCpath$) As Integer
Dim i%, ii%, Filename$, RCnames$(0 To 100), NRC%

  NRC = 0
  Filename$ = Dir$(RCpath$ + "*.RCF")   'Check for RCF file first, then CFG
  If Len(Filename$) > 0 Then
    Do While Len(Filename$) > 0
      NRC = NRC + 1
      RCnames$(NRC - 1) = Filename$
      Filename$ = Dir$()
    Loop
    For i = 0 To NRC - 1
      'Reg(i) = Mid$(RCnames$(i), 4, 5)
      Call ReadInRC(fGetPathToDot(RCpath$ + RCnames$(i)), i)
      'cboReg(i).Text = Reg(i)
      'cboReg(i).ToolTipText = RCtemplate$(i)
    Next i
  Else
    Filename$ = Dir$(RCpath$ + "*.cfg")
    If Len(Filename$) > 0 Then
      Do While Len(Filename$) > 0
        RCnames$(i) = Filename$
        NRC = NRC + 1
        Filename$ = Dir$()
      Loop
      For i = 0 To NRC - 1
'        Reg(i) = Mid$(RCnames$(i), 4, 5)
        Call ReadInRC(fGetPathToDot(RCpath$ + RCnames$(i)), i)
'        cboReg(i).Text = Reg(i)
'        cboReg(i).ToolTipText = RCtemplate$(i)
      Next i
    End If
  End If
  fLoadMissionRCs = NRC
  cmdFLA_Click (4)  'Load Flight Levels from RCS (or FLA) file

End Function

Function fNRCavailable()
Dim i%, ii%, fn$(1 To 65), Filename$
' Loads RC combo boxes with all available RC sets
  i = 0
  Filename$ = Dir$(RCpath$ + "*.RCF")   'Check for RCF file first, then CFG
  If Len(Filename$) > 0 Then
    fn$(1) = Mid$(Filename$, 4, 5)
    Do While Len(Filename$) > 0
      i = i + 1
      Filename$ = Dir$()
      fn(i + 1) = Mid$(Filename$, 4, 5)
    Loop
    fNRCavailable = i
  
    For i = 0 To NRCmax - 1
      For ii = 1 To fNRCavailable
        cboReg(i).AddItem fn$(ii)
        If i = 0 And ii < NRCmax Then Call ReadInRC(RCpath$ + Left$(AC$, 1) + "RC" + fn(ii), ii - 1)
      Next ii
    Next i
  Else
    Filename$ = Dir$(RCpath$ + "*.cfg")
    If Len(Filename$) > 0 Then
      fn$(1) = Mid$(Filename$, 4, 5)
      Do While Len(Filename$) > 0
        i = i + 1
        Filename$ = Dir$()
        fn(i + 1) = Mid$(Filename$, 4, 5)
      Loop
      fNRCavailable = i
  
      For i = 0 To NRCmax - 1
        For ii = 1 To fNRCavailable
          cboReg(i).AddItem fn$(ii)
        Next ii
      Next i
    End If
  End If
End Function

Sub UpdateRCs(ByVal iRC%)
Dim i%

  RCs$ = Left$(AC$, 1) + "RC"
  Reg$(iRC) = cboReg(0).List(iRC)
  Call ReadInRC(RCpath$ + RCs$ + Reg$(iRC), iRC)
  'cboReg(iRC).ToolTipText = RCtemplate$(iRC)
  txtRCformat(iRC).Text = Str(RCformat(iRC))
  txtRCuse(iRC).Text = "0"
  lstFLA.Clear
  For i = 1 To NFL: lstFLA.AddItem Format$(FLA(i) / 100, "#0.000"): Next i
  txtNFL.Text = Str(NFL)
' UpdateForm  'Make sure Flight Levels are updated with RC change
' Also need code to verify that all RCs have same flight levels.
'  CallingForm.UpdateForm
End Sub

Sub UpdatePC(TotalCycle%)
Dim PC!(0 To 14), i%

  txtScans(0).Text = TotalCycles%
  txtScans(1).Text = TotalCycles%
  
  PC(0) = RetAltPC       'H0001 Retrieved Altitude
  PC(1) = TemperaturePC  'H0002 Temperature
  PC(2) = ZtOffPC        'H0004 (Trop-pALT) Offset
  PC(3) = TropAltPC      'H0008 Tropopause Altitude
  PC(4) = PitchPC        'H0010 Pitch Range
  PC(5) = RollPC         'H0020 Roll Range
  PC(6) = NlevPC         'H0040 Nlev
  PC(7) = RatePC         'H0080 DeltaP
  PC(8) = OATtropPC      'H0100 Number of scans which have substituted trops
  
  PC(9) = NavQualPC      'H0200 Nav
  PC(10) = TAqualPC      'H0400 TAs
  PC(11) = CtsQualPC     'H0800 Counts
  PC(12) = CycleQualPC   'H1000 Cycle
  PC(13) = RAWbadPC      'H2000 RAW trops
  PC(14) = TtgtPC        'H4000 Ttgt
  
  If ShowPC Then
    For i% = 0 To 14
      PC(i%) = Int(1000 * PC(i%) / TotalCycles%) / 10
      txtPC(i%).Text = Format$(PC(i%), "##0.0#")
    Next i%
  Else
    For i% = 0 To 14
      txtPC(i%).Text = Format$(PC(i%), "###0")
    Next i%
  End If

End Sub






Private Sub cboFileFormatIn_Click()

  FileFormatIn = Val(cboFileFormatIn.Text)

End Sub


Private Sub cboFileFormatOut_Click()

  FileFormatOut = Val(cboFileFormatOut.Text)

End Sub


Private Sub cboFltDates_Click()
Dim LI%

LI = cboFltDates.ListIndex
cboFltNos.Text = cboFltNos.List(LI)
cboObjectives.Text = cboObjectives.List(LI)
UpdateSourceFile
End Sub

Private Sub cboFltNos_Click()
Dim LI%

LI = cboFltNos.ListIndex
cboFltDates.Text = cboFltDates.List(LI)
cboObjectives.Text = cboObjectives.List(LI)
End Sub


Private Sub cboGE_Click(Index As Integer)
If cboGE(Index).Text <> "" Then txtNfit.Text = Str(Index + 1)
End Sub

Private Sub cboGscale_Click()
'UpdateFLTINFO
End Sub


Private Sub cboMissions_Click()

  UpdateFltDates frmFLTINFO
  Mission$ = Trim(cboMissions.Text)
  UpdateSourceFile
  cmdFLA_Click (4)  'Load Flight Levels from RCS (or FLA) file

End Sub


Private Sub cboOATsource_Click()
'UpdateFLTINFO
End Sub


Private Sub cboObjectives_Click()
Dim LI%
LI = cboObjectives.ListIndex
cboFltNos.Text = cboFltNos.List(LI)
cboFltDates.Text = cboFltDates.List(LI)
UpdateSourceFile

End Sub

Private Sub cboPlatforms_Click()
  
  LoadComboBoxM frmFLTINFO.cboMissions, Trim(cboPlatforms.Text)
  Platform$ = Trim(cboPlatforms.Text)
  Rdir$ = "\" + Platform$ + "\"
  Rdir2$ = Rdir1$ + "\MTP\Data" + Rdir$
  AC$ = Left$(Platform$, 2)
  'UpdateSourceFile

End Sub


Private Sub cboREFsource_Click()
'UpdateFLTINFO
End Sub


Private Sub cboReg_Click(Index As Integer)
Dim i%

  Reg$(Index) = Trim(cboReg(Index).Text)

'  USE5$ = cboUSE.Text

  UpdateRCs (Index)
  txtRCformat(Index).Text = RCformat(Index)

  For i = 0 To NRCmax - 1
    If Trim(cboReg(i).Text) <> "" Then NRC = i + 1
  Next i
  txtNRC.Text = NRC

End Sub





Private Sub chkEditCts_Click()
If chkEditCts.Value = 0 Then
  txtCtsMIn.Enabled = False: txtCtsMax.Enabled = False
  EditCts = False
Else
  txtCtsMIn.Enabled = True: txtCtsMax.Enabled = True
  EditCts = True
End If

End Sub

Private Sub chkEditCycle_Click()
If chkEditCycle.Value = 0 Then
  txtCycleMin.Enabled = False: txtCycleMax.Enabled = False
  EditCycle = False
Else
  txtCycleMin.Enabled = True: txtCycleMax.Enabled = True
  EditCycle = True
End If

End Sub


Private Sub chkEditNav_Click()
If chkEditNav.Value = 0 Then
  txtNavMin.Enabled = False: txtNavMax.Enabled = False
  EditNav = False
Else
  txtNavMin.Enabled = True: txtNavMax.Enabled = True
  EditNav = True
End If

End Sub


Private Sub chkEditRAWbad_Click()
If chkEditRAWbad.Value = 0 Then
  txtRAWbadMin.Enabled = False: txtRAWbadMax.Enabled = False
  EditRAWbad = False
Else
  txtRAWbadMin.Enabled = True: txtRAWbadMax.Enabled = True
  EditRAWbad = True
End If


End Sub

Private Sub chkEditTA_Click()
If chkEditTA.Value = 0 Then
  txtTAmin.Enabled = False: txtTAmax.Enabled = False
  EditTA = False
Else
  txtTAmin.Enabled = True: txtTAmax.Enabled = True
  EditTA = True
End If

End Sub

Private Sub chkEditTtgt_Click()
If chkEditTtgt.Value = 0 Then
  txtTtgtMin.Enabled = False: txtTtgtMax.Enabled = False
  EditTtgt = False
Else
  txtTtgtMin.Enabled = True: txtTtgtMax.Enabled = True
  EditTtgt = True
End If


End Sub


Private Sub chkEnableWCT_Click()
  If chkEnableWCT.Value = 1 Then EnableWCT = True Else EnableWCT = True
End Sub

Private Sub chkOATtrop_Click()
Dim Index%

If chkOATtrop.Value = 1 Then
  For Index% = 0 To 3
    txtOATzt1(Index%).Enabled = True
    txtTzt1(Index%).Enabled = True
    txtOATzt2(Index%).Enabled = True
    txtTzt2(Index%).Enabled = True
    txtOATks1(Index%).Enabled = True
    txtOATks2(Index%).Enabled = True
  Next Index%
Else
  For Index% = 0 To 3
    txtOATzt1(Index%).Enabled = False
    txtTzt2(Index%).Enabled = False
    txtOATzt2(Index%).Enabled = False
    txtTzt2(Index%).Enabled = False
    txtOATks1(Index%).Enabled = False
    txtOATks2(Index%).Enabled = False
  Next Index%

End If
End Sub

Private Sub chkPitch_Click()
If chkPitch.Value = 0 Then
  txtPitchMin.Enabled = False: txtPitchMax.Enabled = False
  EditPitch = False
Else
  txtPitchMin.Enabled = True: txtPitchMax.Enabled = True
  EditPitch = True
End If
End Sub


Private Sub chkRate_Click()
If chkRate.Value = 0 Then
  txtRateMin.Enabled = False: txtRateMax.Enabled = False
  EditRate = False
Else
  txtRateMin.Enabled = True: txtRateMax.Enabled = True
  EditRate = True
End If
End Sub

Private Sub chkRetAlt_Click()
If chkRetAlt.Value = 0 Then
  txtRetAltMin.Enabled = False: txtRetAltMax.Enabled = False
  EditRetAlt = False
Else
  txtRetAltMin.Enabled = True: txtRetAltMax.Enabled = True
  EditRetAlt = True
End If
End Sub


Private Sub chkRoll_Click()
If chkRoll.Value = 0 Then
  txtRollMin.Enabled = False: txtRollMax.Enabled = False
  EditRoll = False
Else
  txtRollMin.Enabled = True: txtRollMax.Enabled = True
  EditRoll = True
End If
End Sub




Private Sub chkTemperature_Click()
If chkTemperature.Value = 0 Then
  txtTemperatureMin.Enabled = False: txtTemperatureMax.Enabled = False
  EditTemperature = False
Else
  txtTemperatureMin.Enabled = True: txtTemperatureMax.Enabled = True
  EditTemperature = True
End If

End Sub

Private Sub chkTropAlt_Click()
If chkTropAlt.Value = 0 Then
  txtTropAltMin.Enabled = False: txtTropAltMax.Enabled = False
  EditTropAlt = False
Else
  txtTropAltMin.Enabled = True: txtTropAltMax.Enabled = True
  EditTropAlt = True
End If
End Sub

Private Sub chkZtOff_Click()
If chkZtOff.Value = 0 Then
  txtZtOffA.Enabled = False: txtZtOffB.Enabled = False
  EditZtOff = False
Else
  txtZtOffA.Enabled = True: txtZtOffB.Enabled = True
  EditZtOff = True
End If
End Sub





Private Sub cmdAsciiCalfile_Click(Index As Integer)

CALfile$ = Path$ + AC$ + YYYYMMDD$ + ".CAL"

If Index = 0 Then
  ReadCAL (CALfile$)
Else
  WriteCAL (CALfile$)
End If

End Sub

Private Sub cmdFileFormatRestore_Click()
' The C:\MTP\Setup\ contains default header record file named FLTINFO.REFxx
' where xx is the File Format, which can be used to restore a FileFormatIn
' Fake data from AVE January 21, 2004 flight is in folder for March 10, 2004
Dim FLTINFOfile$, i%
Static SAVEyyyymmdd$

  Select Case Val(cboFileFormatRestore.Text)
  Case 32:  FLTINFOfile$ = "C:\MTP\Setup\FLTINFO.REF32"
  Case 33:  FLTINFOfile$ = "C:\MTP\Setup\FLTINFO.REF33"
  Case Else
    MsgBox "You have selected an invalid File Format", vbOKOnly
    Exit Sub
  End Select
  
  If Len(Dir(FLTINFOfile$)) = 0 Then
    MsgBox FLTINFOfile$ + " does not exist!", vbOKOnly
    Exit Sub
  End If
  
  REFreadHEADER FLTINFOfile$                   'Read Saved Header Record

  SAVEyyyymmdd$ = YYYYMMDD$
  UpdateForm
  
  For i = 0 To cboFltDates.ListCount
    If Trim(cboFltDates.List(i)) = SAVEyyyymmdd$ Then Exit For
  Next i
  
  cboFltDates.Text = cboFltDates.List(i)
  cboFltNos.Text = cboFltNos.List(i)
  cboObjectives.Text = cboObjectives.List(i)
  
  UpdateSourceFile
  
  REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"                  'Write Header Record

End Sub

Private Sub cmdFLA_Click(Index As Integer)
Dim i%, FQFN$, FLAfile$

  Select Case Index
  Case 0  'Add
    lstFLA.AddItem txtFLA.Text
    txtFLA.Text = ""
  Case 1  'Remove
    lstFLA.RemoveItem (lstFLA.ListIndex)
  Case 2  'Clear
    lstFLA.Clear
  Case 3  'Default (HARDWARE.INI file)
    LoadListBoxH lstFLA, "ALT_" + Platform$, 2, 1#

  Case 4  'Setup
    lstFLA.Clear
    FQFN$ = SUpath$ + Mission$ + "_RCS.txt"
    If Len(Dir$(FQFN)) = 0 Then
      FQFN$ = MNpath$ + "Setup\" + Mission$ + "_FLA.txt"
      If Dir$(FQFN$) <> "" Then
        Call ReadFLAfile(Platform$, Mission$)  'Setup FLA file
        For i = 1 To NFL: lstFLA.AddItem Format$(FLA(i) / 100#, "#0.000"): Next i
      Else
        MsgBox FQFN$ + " does not exist!", vbOKOnly
      End If
    
'      Call ReadFLAfile(Platform$, Mission$)
'      For i = 1 To NFL: lstFLA.AddItem Format$(FLA(i) / 100, "#0.000"): Next i
    Else
      Call ReadMissionRCS(Mission$, "RC_ALTITUDES", False)
      For i = 1 To NFL
        lstFLA.AddItem Format$(FLA(i) / 100#, "#0.000")
      Next i
    End If
    
    
    
    
  End Select
  NFL = lstFLA.ListCount
  txtNFL.Text = Str$(NFL)

End Sub

Public Sub cmdFlight_Click(Index As Integer)
Dim OUTlu%, NUMfile$, out$, i%, CurYYYYMMDD$, LI%
Dim FLTINFOfile$, SAVEyyyymmdd$

  Select Case Index
  Case 0 'Add Flight
    cboFltDates.AddItem cboFltDates.Text
    cboFltNos.AddItem cboFltNos.Text
    cboObjectives.AddItem cboObjectives.Text
    UpdateSourceFile

    NUMfile$ = SUpath$ + Mission$ + "_NUM."
    If Len(Dir(NUMfile$ + "SAV")) > 0 Then
      Kill NUMfile$ + "SAV"
      Name NUMfile$ + "TXT" As NUMfile$ + "SAV"  'Save old file
    End If
    OUTlu = FreeFile
    Open NUMfile$ + "TXT" For Output As #OUTlu
    For i = 0 To cboFltDates.ListCount - 1
      If cboObjectives.List(i) = "" Then cboObjectives.List(i) = "TBD"
      out$ = Format(Trim(cboFltNos.List(i)), "000000") + " " + Trim(cboFltDates.List(i)) + " " + LTrim(cboObjectives.List(i))
      Print #OUTlu, out$
    Next i
    Close OUTlu%
  
  Case 1   'Use Mission NUM
  
  Case 4 'Clear
    cboFltDates.Clear
    cboFltNos.Clear
    cboObjectives.Clear
  
  Case 5 'Default
    FLTINFOfile$ = txtSourceFile.Text
    If Len(Dir(FLTINFOfile$)) = 0 Then
      MsgBox FLTINFOfile$ + " does not exist!", vbOKOnly
      frmFLTINFO.Caption = "FLTINFO Summary for " + AC$ + "/" + Mission$ + "/" + YYYYMMDD$
      Exit Sub
    End If

    REFreadHEADER FLTINFOfile$
    If Val(cboFileFormatIn) <> FileFormatIn Then cboFileFormatIn.Text = Str(FileFormatIn)
  
    SAVEyyyymmdd$ = YYYYMMDD$
    UpdateForm
    UpdateFLTINFO
    ArrayRead = False
    For i = 0 To cboFltDates.ListCount
      If Trim(cboFltDates.List(i)) = SAVEyyyymmdd$ Then Exit For
    Next i
  
    cboFltDates.Text = cboFltDates.List(i)
    cboFltNos.Text = cboFltNos.List(i)
    cboObjectives.Text = cboObjectives.List(i)
  
    cmdWriteDefault_Click  'Set as Default file

  Case 1 'Remove Flight
    CurYYYYMMDD$ = Trim(cboFltDates.Text)
    For i = 0 To cboFltDates.ListCount - 1
      If Trim(cboFltDates.List(i)) = CurYYYYMMDD$ Then
        LI = i
        Exit For
      End If
    Next i
    LI = cboFltDates.ListIndex
    If LI > -1 Then
      cboFltDates.RemoveItem (LI)
      cboFltNos.RemoveItem (LI)
      cboObjectives.RemoveItem (LI)
      cboFltDates.ListIndex = 0
    End If
  
  Case 2 'Save Change to Editted Flight Info
    LI = cboFltDates.ListIndex
    cboFltDates.List(LI) = cboFltDates.Text
    cboFltNos.List(LI) = cboFltNos.Text
    cboObjectives.List(LI) = cboObjectives.Text
    cboFltDates.ListIndex = LI
    cboFltNos.ListIndex = LI
    cboObjectives.ListIndex = LI
    
  Case 3 'Write to File
  
  End Select
  cmdFLA_Click (4)  'Load Flight Levels from RCS (or FLA) file

End Sub


Private Sub cmdFlight5_Click()

End Sub

Private Sub cmdManualUpdate_Click()

 UpdateFLTINFO

End Sub

Private Sub cmdNewMission_Click()
' Add a new Mission to the Missions.INI file
' And create all the directory structure to support it
' Using the selected platform and mission setup files, but renamed
Dim NewMission$, NewMNpath$, NewRCpath$, NewMPpath$, NewSUpath$, Ext$(1 To 7)
Dim i%, NewRAWpath$, NewRAOBpath$, NewPNGpath$, INPlu%, OUTlu%, A$
Dim NewYYYYMMDDpath$, INIfile$, RAWfile$, CALfile$, YYYYMMDDpath$
Dim NewINIfile$, NewRAWfile$, NewCALfile$
  
  NewMission$ = txtNewMission.Text
  NewMNpath$ = DataDrive$ + "\MTP\Data\" + Platform$ + "\" + NewMission$ + "\"
  NewRCpath$ = NewMNpath$ + "RC\"
  NewMPpath$ = NewMNpath$ + "MP\"
  NewSUpath$ = NewMNpath$ + "Setup\"
  NewRAWpath$ = NewMNpath$ + "Raw\"
  NewRAOBpath$ = NewMNpath$ + "RAOB\"
  NewPNGpath$ = NewMNpath$ + "PNG\"
  NewYYYYMMDDpath$ = NewMNpath$ + YYYYMMDD$ + "\"
  YYYYMMDDpath$ = MNpath$ + YYYYMMDD$ + "\"
  RAWfile$ = AC$ + YYYYMMDD$ + ".RAW"
  CALfile$ = AC$ + YYYYMMDD$ + ".CAL"
  
  If Left$(SU$, 3) = "CAN" Then
    INIfile$ = Left$(RAWfile$, Len(RAWfile$) - 3) + "MTPH"
  Else
    INIfile$ = Left$(RAWfile$, Len(RAWfile$) - 3) + "INI"
  End If
  
  CreateDirectoryTree NewMNpath$, False
  CreateDirectoryTree NewRCpath$, False
  CreateDirectoryTree NewMPpath$, False
  CreateDirectoryTree NewSUpath$, False
  CreateDirectoryTree NewRAWpath$, False
  CreateDirectoryTree NewRAOBpath$, False
  CreateDirectoryTree NewRAOBpath$ + "Templates\", False
  CreateDirectoryTree NewRAOBpath$ + "RMSerror\", False
  CreateDirectoryTree NewPNGpath$, False

' Add new mission to C:\MTP\Setup\Missions.INI file
  UpdateMissionsINI Platform$, NewMission$
  
' Copy and Rename Setup Files in Mission path
  Ext$(1) = "_HDR": Ext$(2) = "_CAL": Ext$(3) = "_WCT":  Ext$(4) = "_RCS"
  For i = 1 To 4
    FileCopy SUpath$ + Mission$ + Ext$(i) + ".txt", NewSUpath$ + NewMission$ + Ext$(i) + ".txt"
  Next i

' Now deal with NUM file and flight date folder files
'20100410 20100831 RF-07 St Croix local
  INPlu = FreeFile
  Open SUpath$ + Mission$ + "_NUM.txt" For Input As INPlu
  Do
    Line Input #INPlu, A$
  Loop Until Mid$(A$, 10, 8) = YYYYMMDD$
  Close INPlu
  OUTlu = FreeFile
  Open NewSUpath$ + NewMission$ + "_NUM.txt" For Output As OUTlu
  Print #OUTlu, A$
  Close OUTlu
  CreateDirectoryTree NewYYYYMMDDpath$, False
  FileCopy YYYYMMDDpath$ + INIfile$, NewYYYYMMDDpath$ + INIfile$
  FileCopy YYYYMMDDpath$ + CALfile$, NewYYYYMMDDpath$ + CALfile$
  FileCopy YYYYMMDDpath$ + RAWfile$, NewYYYYMMDDpath$ + RAWfile$
  
' Copy Retrieval Coefficient Folder
  CopyFolder RCpath$, NewRCpath$

End Sub

Private Sub cmdPC_Click(Index As Integer)

ShowPC = Not ShowPC
If ShowPC Then
  cmdPC(Index).Caption = "Show Counts"
  lblPC(Index).Caption = "% Edited"
Else
  cmdPC(Index).Caption = "Show %"
  lblPC(Index).Caption = " Counts"
End If
  UpdatePC (Cycle%)
End Sub


Public Sub cmdRCfiles_Click(Index As Integer)
Dim i%, ii%, iii%, Filename$, PackThreshold%, ans As Variant, SaveRC As Boolean, FltDayRC As Boolean
Dim yyyymmddP1$, RAOByyyymmdd$

  Select Case Index
  Case 0               'Load all RCs
    For i = 0 To NRC - 1: cboReg(i).Text = "": Next i
    NRCavailable = fLoadMissionRCs(RCpath$)
    NRC = NRCavailable  'fNRCavailable()   'This loads combo boxes with all available RCs

    If NRC > 0 Then
      If NRC > NRCmax Then NRC = NRCmax
      txtNRC.Text = Str(NRC)
    Else
      MsgBox "No Retrieval Coefficients were found in the path:" + vbCrLf + RCpath$, vbOKOnly
      Exit Sub
    End If

    For ii = 0 To NRC - 1
      Call UpdateRCs(ii)
      cboReg(ii).Text = Reg$(ii)
    Next ii
    cmdFLA_Click (4)  'Load Flight Levels from RCS (or FLA) file
  
  Case 1    'Clear RCs Use
    For i = 0 To NRCmax - 1
      RCuse(i) = 0
      txtRCuse(i).Text = Str(0)
    Next i
  
  Case 2    'Pack RCs
    ' Automatically remove RC sets with less hits than the PackThreshold
    ans = MsgBox("Save RCs for this day?", vbYesNo)
    If ans = vbYes Then SaveRC = True Else SaveRC = False
    'ABR___2008061900.RAOB2
    'XXXXX_YYYYMMDDHH
    '12345678901234567890
    PackThreshold = Val(cboPackThreshold.Text)
    For ii = 0 To NRCmax - 1
      Filename$ = cboReg(ii).ToolTipText
      If Len(Filename$) > 0 Then
        Filename$ = fGetFilename(Filename$)
        yyyymmddP1 = fYYYYMMDD(fDate(YYYYMMDD$) + 1)
        RAOByyyymmdd$ = Mid$(Filename$, 7, 8)
        If (RAOByyyymmdd$ = YYYYMMDD$ Or RAOByyyymmdd$ = yyyymmddP1$) And SaveRC Then FltDayRC = True Else FltDayRC = False
        If Not FltDayRC Then
          If Val(txtRCuse(ii).Text) < PackThreshold Then cboReg(ii).Text = ""
      End If
      End If
    Next ii
    ' Now do the packing
    For ii = 0 To NRCmax - 1
      If cboReg(ii).Text = "" Then
        i = ii
        txtRCuse(i).Text = ""
        Do
          i = i + 1
          If i > NRCmax - 1 Then i = NRCmax - 1: Exit Do
        Loop Until cboReg(i).Text <> "" Or i = NRCmax - 1
        If i <= NRCmax - 1 And cboReg(i).Text <> "" Then
          cboReg(ii).Text = cboReg(i).Text
          cboReg(ii).ToolTipText = RCtemplate$(i)
          cboReg(i).Text = ""
          cboReg(i).ToolTipText = ""
          txtRCuse(ii).Text = txtRCuse(i).Text
          txtRCuse(i).Text = ""
          Reg$(ii) = cboReg(ii).Text
          RCformat(ii) = RCformat(i)
          txtRCformat(ii).Text = txtRCformat(i).Text
          iii = ii
'          Call UpdateRCs(iii)

        Else
          Exit For
        End If
      End If
    Next ii
'   Get number of RCs and clear remaining boxes
    NRC = 0
    For ii = 0 To NRCmax - 1
      If cboReg(ii).Text = "" Then
        txtRCuse(ii).Text = ""
        Reg$(ii) = ""
        RCuse(ii) = 0
      Else
        NRC = NRC + 1
      End If
    Next ii
    txtNRC.Text = Str(NRC)
    CallingForm.UpdateForm
  
  Case 3 'Clear ALL RCs
    For i = 0 To NRCmax - 1
      cboReg(i).Text = ""
    Next i

  
  End Select
  REFwriteHEADER "C:/MTP/Setup/FLTinfo.REF2"
    For i = 0 To NRC - 1
      Filename$ = RCpath$ + RCs$ + Reg(i)
      Call ReadInRC(Filename$, i)
      cboReg(i).ToolTipText = RCtemplate$(i)
    Next i

End Sub

Private Sub cmdReadFLTINFO_Click(Index As Integer)
Dim FLTINFOfile$, i%
Static SAVEyyyymmdd$

  If Index = 1 Then
    FLTINFOfile$ = "C:\MTP\Setup\FLTINFO.REF"
  Else
    FLTINFOfile$ = txtSourceFile.Text
    If Len(Dir(FLTINFOfile$)) = 0 Then
      MsgBox FLTINFOfile$ + " does not exist!", vbOKOnly
      Exit Sub
    End If
  End If
  
  REFreadHEADER FLTINFOfile$

  If Val(cboFileFormatIn) <> FileFormatIn Then cboFileFormatIn.Text = Str(FileFormatIn)

  SAVEyyyymmdd$ = YYYYMMDD$
  UpdateForm          'Needs to be here!
  ArrayRead = False
  For i = 0 To cboFltDates.ListCount
    If Trim(cboFltDates.List(i)) = SAVEyyyymmdd$ Then Exit For
  Next i
  
  cboFltDates.Text = cboFltDates.List(i)
  cboFltNos.Text = cboFltNos.List(i)
  cboObjectives.Text = cboObjectives.List(i)
  
'  UpdateForm
  UpdateSourceFile
  
End Sub

Public Sub cmdReadNewCAL_Click(Index As Integer)
Dim Filename$, i%, j%

If chkUseDefaultCalfile.Value = 1 Then
  Filename$ = DataDrive$ + Rdir2$ + Mission$ + "\" + Mission$ + ".CAL"
Else
  Filename$ = DataDrive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".CAL"
End If

Select Case Index
Case 0
  ReadCAL (Filename$)
Case 1
  ReadWCT (Filename$)
Case 2
  ReadMissionWCT
  
Case Else
  MsgBox "This is not a valid Calfile command!", vbOKOnly
  Exit Sub
End Select

UpdateForm

End Sub



Private Sub cmdShowFltTrack_Click()
Dim Path$, Filename$, Status As Boolean, f As Form
  
  Set f = frmFltTrack
  Path$ = MNpath$ + "\FltTracks\"
  Filename = Dir(Path$ + "Track_" + YYYYMMDD$ + "*.*")
  If Len(Filename) > 0 Then
    With frmFltTrack
    .AutoRedraw = True
    If UCase$(Right$(Filename, 3)) = "PNG" Then
      'Status = LoadFormAsPNG(Path$ + Filename, f) 'Use MScapture
    Else
      .Picture = LoadPicture(Path$ + Filename)
    End If
    DoEvents
    .Top = (768 * Screen.TwipsPerPixelY - .Height) / 2
    .Left = (1024 * Screen.TwipsPerPixelX - .Width) / 2
    .Visible = True
    End With
  Else
    Path$ = "c:\www\missions\" + LCase(Mission) + "\" + LCase(Mission) + "_ctc\"
    Filename = Dir(Path$ + "Track_" + YYYYMMDD$ + "*.*")
    If Len(Filename) > 0 Then
      With frmFltTrack
      .AutoRedraw = True
      If UCase$(Right$(Filename, 3)) = "PNG" Then
        'Status = LoadFormAsPNG(Path$ + Filename, f) 'Use MScapture
      Else
        .Picture = LoadPicture(Path$ + Filename)
      End If
      DoEvents
      .Top = (768 * Screen.TwipsPerPixelY - .Height) / 2
      .Left = (1024 * Screen.TwipsPerPixelX - .Width) / 2
      .Visible = True
      End With
    Else
      MsgBox "The flight track for this flight is not available in the \Mission\FltTracks\ folder!", vbOKOnly
    End If
  End If
  
End Sub

Private Sub cmdTemplateMap_Click()
Dim i%, INPlu%, INPfile$, Path$, OUTlu%, Line$, FirstPass As Boolean
Dim X$, y$, Answer As Variant
Dim Filename$, FileCount%, Ext$, OUTfile$, RCfilenames$(1 To 65)

  cmdTemplateMap.Caption = "Processing ..."
  OUTfile$ = RTpath$ + "TemplateMap.txt"
  OUTlu = FreeFile
  Open OUTfile$ For Output As OUTlu
  RCfilenames$(1) = Dir(RCpath$ + "*.RCF", vbNormal)
  FileCount = 1
  Do While RCfilenames$(FileCount) <> ""
    FileCount = FileCount + 1
    RCfilenames$(FileCount) = Dir
  Loop
' Before converting, add ! to file name to save original
  For i = 1 To FileCount - 1
    Call RCread(0, RCpath$ + RCfilenames$(i))
    X$ = Format(i - 1, "00") + " " + Mid$(RCfilenames$(i), 4, 2) + " - "
    X$ = X$ + fGetFilename(RAOBfilename) + " " + Format(RAOBcount, "000") + " " + formats(LR1, "0.0")
    X$ = X$ + " " + formats(LR2, "0.0") + " " + Format(zLRb, "00.0")
    Print #OUTlu, X$
  Next i
  Close OUTlu
  cmdTemplateMap.Caption = "Template Map"
End Sub

Private Sub cmdViewRC_Click(Index As Integer)
Dim i%, j%, k%, L%, V!(1 To 33), pALTindex%, fmt$, A$

  For i = 0 To NRCmax - 1
    If i = NRCmax - 1 Then
      MsgBox "Select a RC set on the RC tab before using this command!", vbOKOnly
      Exit Sub
    End If
    If i = UserRCindex Then Exit For
  Next i
  
  If lstFLA.SelCount = 0 Then lstFLA.Selected(0) = True
  lstRCfile.Clear
  RCs$ = Left$(AC$, 1) + "RC"
  Reg$(UserRCindex) = cboReg(UserRCindex).Text
  RCfilename$ = txtRCpath.Text + RCs$ + Reg$(UserRCindex) + ".RCF"
  Call RCread(UserRCindex, RCfilename$)
  
  Select Case Index
  Case 0
  ' View configuration information
    With lstRCfile
    .Clear
    .AddItem "' RC Configuration file: Created- " + Date$ + " " + Time$
    .AddItem ""
    .AddItem "[RCformat]"
    .AddItem Str(RCformat(Index))
    .AddItem ""
  
    .AddItem "[RAOB]"
    .AddItem "RAOB File Used:            " + Trim(RAOBfilename)
    .AddItem "Number of available RAOBs:" + Str(RAOBcount)
    .AddItem "LR above top of RAOB:     " + Str(LR1)
    .AddItem "LR break altitude:        " + Str(zLRb)
    .AddItem "LR above break altitude:  " + Str(LR2)
    .AddItem "Record Step through RAOBs:" + Str(RecordStep)
    .AddItem "Minimum RAOB P altitude:  " + Str(MinimumRAOBz)
    .AddItem "Excess Ground T added (K): " + Str(ExcessTamplitude)
    .AddItem ""

    .AddItem "[Nobs] - Number of Observables"
    .AddItem Str(Nobs)
    A$ = ""
'  For i = 1 To Nobs  'Read observable errors
'    a$ = a$ + Format(OBrms(1, i), "0.00") + " "
'  Next i
'  .AddItem a$
    .AddItem ""
  
    .AddItem "[Nret] - Retrieval Altitude Offsets (km)"
    .AddItem Str(Nret)
    A$ = ""
    For i = 1 To Nret  'Write retrieval offset levels
      A$ = A$ + Format(dZ(i), "##0.0") + " "
    Next i
    .AddItem A$
    .AddItem ""
  
    .AddItem "[NFL] - Number of Flight Levels with RCs"
    .AddItem Str(NFL)
    A$ = ""
    For i = 1 To NFL  'Write flight levels
      A$ = A$ + Format(Zr(i), "#0.00") + " "
    Next i
    .AddItem A$
    .AddItem ""
  
    .AddItem "[Nlo] - Number of LO Frequencies (GHz)"
    .AddItem Str(Nlo)
    A$ = ""
    For i = 1 To Nlo      'Write LO Frequencies
      A$ = A$ + Format(LO(i), "#00.000") + " "
    Next i
    .AddItem A$
    .AddItem ""
  
    .AddItem "[Nel] - Number of Elevation Angeles (deg)"
    .AddItem Str(Nel)
    A$ = ""
    For i = 1 To Nel      'Write LO Frequencies
      A$ = A$ + Format(El(i), "#00.0") + " "
    Next i
    .AddItem A$
    .AddItem ""
  
    .AddItem "[Nif] - Number of IF Bandpass Segments and Their Weights"
    .AddItem Str(Nif)
    For j = 1 To 3
      A$ = "IF Offset (GHz): "
      For i = 1 To Nif      'Write IF offset frequencies (GHz)
        A$ = A$ + formats(IFoff(j, i), "#0.0000") + " "
      Next i
      .AddItem A$
    Next j
  
    For j = 1 To 3
      A$ = "IF Weight:   "
      For i = 1 To Nif      'Write IF weights
        A$ = A$ + formats(IFwt(j, i), "#0.0000") + " "
      Next i
      .AddItem A$
    Next j
    End With
       
  Case 1
  ' View selected flight level for select RC set on RC tab
    pALTindex = lstFLA.ListIndex + 1
    V(1) = BP(0, pALTindex, 16): V(2) = RAOBcount
    lstRCfile.AddItem fGetFilename(RCfilename) + fUsing(" ###0.00 #000", V()) + " Generated: " + Date$ + " " + Time$
    lstRCfile.AddItem ""
    For i = 1 To Nobs: V(i) = OBrms(pALTindex, i): Next i
    Call SameFormat2(lstRCfile, " ##0.00", 9, V(), Nobs)
    For i = 1 To Nobs: V(i) = OBav(UserRCindex, pALTindex, i): Next i
    Call SameFormat2(lstRCfile, " ##0.00", 9, V(), Nobs)
    fmt$ = " ###0.00 ###0.00 ###0.00 ###0.00"
    For k = 1 To Nret
      V(1) = BP(0, pALTindex, k)
      V(2) = RTav(UserRCindex, pALTindex, k)
      V(3) = RMSa(UserRCindex, pALTindex, k)
      V(4) = RMSe(UserRCindex, pALTindex, k)
      lstRCfile.AddItem fUsing(fmt$, V())
      For L = 1 To Nobs
        V(L) = rc(0, pALTindex, k, L)
      Next L
      Call SameFormat2(lstRCfile, " ##0.00000", 6, V(), Nobs)
    Next k
    
  End Select

End Sub

Private Sub cmdWCT_Click(Index As Integer)
Select Case Index
Case 0  'Add
  lstWCTdates.AddItem txtWCTdates.Text
  txtWCTdates.Text = ""
Case 1  'Remove
  lstWCTdates.RemoveItem (lstWCTdates.ListIndex)
End Select
'txtWCTdates.Text = Str$(lstWCTdates.ListCount)
End Sub

Public Sub cmdWriteDefault_Click()
  
  RawCycle = 0
  UpdateFLTINFO                               'Update FLTINFO form
  CallingForm.UpdateForm                      'Update MTPbin form
  REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"   'Write default header records
    
End Sub

Public Sub cmdWritePath_Click()

UpdateFLTINFO

On Error GoTo MakeDir
OpenREF:
  REFwriteHEADER PathDot$ + "REF"
  REFwriteHEADER PathDot$ + "BIN"

  Exit Sub

MakeDir:
  MkDir Left$(PathDot$ + "REF", Len(PathDot$ + "REF") - 14)
  GoTo OpenREF

End Sub





Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
Dim i%, j%, lu%

' Edit MTPsetup.INI if address, username or password change
  ReadSETUP ("cboDataSource")   'Only location where MTPsetup.INI is read
' Load data source combo box
  For i = 0 To cboN0
    With cboDataSource
    .AddItem cDataSource(i)
    End With
  Next i
  
  ReadSETUP ("cboDestination")   'Only location where MTPsetup.INI is read
' Load destination 1 combo box
  For i = 0 To cboN
    With cboDestination
    .AddItem cDestination(i)
    End With
  Next i
' Load destination 2 combo box
  ReadSETUP ("cboDestination2")   'Only location where MTPsetup.INI is read
  For i = 0 To cboN2
    With cboDestination2
    .AddItem cDestination2(i)
    End With
  Next i

' Get default drives
  ReadSETUP ("SYSTEM")   'Only location where MTPsetup.INI is read
  If UseDefaultRCs Or RealTime Then
    label(167).Caption = "Default Mission RCs are being used"
  Else
    label(167).Caption = "Default Mission RCs are NOT being used"
  End If

' Extract FTP IP address and directory path for data source
  For i = 0 To cboN0
    If UCase(Left$(cDataSource$(i), 3)) = "FTP" Then
      j = InStr(7, cDataSource$(i), "/")
      cFTP0(i) = Left$(cDataSource$(i), j - 1)
      cCD0$(i) = Right$(cDataSource$(i), Len(cDataSource$(i)) - j + 1)
    End If
  Next i

' Extract FTP IP address and directory path for destination 1 = server on GV
  For i = 0 To cboN
    If UCase(Left$(cDestination$(i), 3)) = "FTP" Then
      j = InStr(7, cDestination$(i), "/")
      cFTP(i) = Left$(cDestination$(i), j - 1)
      cCD$(i) = Right$(cDestination$(i), Len(cDestination$(i)) - j + 1)
    End If
  Next i
  
' Extract FTP IP address and directory path for destination 2 = ground on GV
  For i = 0 To cboN2
    If UCase(Left$(cDestination2$(i), 3)) = "FTP" Then
      j = InStr(7, cDestination2$(i), "/")
      cFTP2(i) = Left$(cDestination2$(i), j - 1)
      cCD2$(i) = Right$(cDestination2$(i), Len(cDestination2$(i)) - j + 1)
    End If
  Next i
  cboDataSource.ListIndex = DataSourceMode
  cboDestination.ListIndex = DataDestinationMode
  cboDestination2.ListIndex = DataDestinationMode2
  
  ShowPC = False
  cmdPC(0).Caption = "% Edited"
  lblPC(0).Caption = "Counts"
  cmdPC(1).Caption = "% Edited"
  lblPC(1).Caption = "Counts"
  
  With cboFileFormatIn
  .AddItem "32"
  .AddItem "33"
  .ListIndex = 0
  End With
  FileFormatIn = 32
  
  With cboFileFormatRestore
  .AddItem "32"
  .AddItem "33"
  .ListIndex = 0
  End With
  
  With cboFileFormatOut
  .AddItem "32"
  .AddItem "33"
  .ListIndex = 1
  End With
  FileFormatOut = 33
  
  For i = 0 To 4
    With cboGE(i)
    .AddItem "Offset"
    .AddItem "Tifa"
    .AddItem "Tlo1"
    .AddItem "Tlo2"
    .AddItem "Tnav"
    .AddItem "Time"
    End With
  Next i
  
  With frmFLTINFO.cboOATsource
  .AddItem "MTP"
  .AddItem "A/C"
  .AddItem "MMS"
  End With

  With frmFLTINFO.cboGscale
  .AddItem "EQN"
  .AddItem "ND"
  .AddItem "OAT"
  End With
  
  With frmFLTINFO.cboREFsource
  .AddItem "TGT"
  .AddItem "OAT"
  End With
  
  With cboPackThreshold
  .AddItem "0"
  .AddItem "10"
  .AddItem "20"
  .AddItem "30"
  .AddItem "40"
  .AddItem "50"
  .AddItem "60"
  .AddItem "70"
  .AddItem "80"
  .AddItem "90"
  .AddItem "100"
  .AddItem "200"
  .ListIndex = 0
  End With
  
'  LoadComboBoxH frmFLTINFO.cboOATsource, "OATSRC"
'  LoadComboBoxH frmFLTINFO.cboGscale, "GSCALE"
'  LoadComboBoxH frmFLTINFO.cboREFsource, "REFSRC"
  LoadComboBoxH frmFLTINFO.cboSU, "SU"
  LoadComboBoxH frmFLTINFO.cboPlatforms, "PLATFORM"

  cmdReadFLTINFO_Click (1) 'Load default FLTINFO
End Sub




Private Sub Frame_DblClick(Index As Integer)
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub lblRC_Click(Index As Integer)
Dim i%

UserRCindex = Index
For i = 0 To NRC - 1
  lblRC(i).BackColor = &H8000000F
Next i
lblRC(Index).BackColor = &H80000014

End Sub

Private Sub lstRCfile_Click()
Debug.Print lstRCfile.List(lstRCfile.ListIndex)
End Sub

Private Sub mnuFileReadFLTINFO_Click()
Dim lu%, Filename$

' Set Initial directory
  CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
' Set filters.
  CommonDialog1.Filter = "REF Files (*.REF)|*.REF|INI Files (*.INI)|*.INI|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  Filename$ = CommonDialog1.Filename

  If Filename$ = "" Then Exit Sub
' On Error GoTo errhandler
  Select Case UCase$(Right$(Filename$, 3))
  Case "INI"    'Only keep this just in case it's needed for initialization
    ReadFLTINFO (Filename$)
  Case "REF", "ERF"
    REFreadHEADER Filename$
  Case Else
    Call MsgBox("You must select INI, REF or ERF file!", vbOKOnly)
    Exit Sub
  End Select
  
  If Val(cboFileFormatIn) <> FileFormatIn Then cboFileFormatIn.Text = Str(FileFormatIn)

  UpdateForm

End Sub

Private Sub mnuFileExit_Click()
  Close
  Unload Me
End Sub


Private Sub mnuHelpAbout_Click()
'Set properties
frmAbout.Application = "MTP Flight Information Program"
frmAbout.Heading = "Release August 29, 2004"
frmAbout.Copyright = "2001 MJ Mahoney"
'Call a method
frmAbout.Display

End Sub


Private Sub mnuWriteFLTINFO_Click()
  UpdateFLTINFO
  WriteFLTINFO ("C:\MTP\Setup\FLTINFO.INI")
  WriteFLTINFO (Path$ + AC$ + YYYYMMDD$ + ".INI")
End Sub










Private Sub optRCindex_Click(Index As Integer)
UserRCindex = Index
End Sub

Private Sub optChInfo_Click(Index As Integer)
Dim i%
Select Case Index
Case 0  'Manual
  For i = 0 To 2
    txtChInfo(i).Enabled = True
  Next i
Case 1  '1/Tau
  For i = 0 To 2
    txtChInfo(i).Enabled = False
  Next i
  
Case 2  '1/RMS
  For i = 0 To 2
    txtChInfo(i).Enabled = False
  Next i
End Select

End Sub



Private Sub SSTab1_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub txtRCformat_Click(Index As Integer)
UserRCindex = Index
End Sub


Private Sub txtTemperatureMax_Change()
TemperatureMax = Val(txtTemperatureMax.Text)
End Sub

Private Sub txtTemperatureMin_Change()
TemperatureMin = Val(txtTemperatureMin.Text)
End Sub



Private Sub txtPitchMax_Change()
ePitchMax = Val(txtPitchMax)
End Sub

Private Sub txtPitchMin_Change()
ePitchMin = Val(txtPitchMin)
End Sub

Private Sub txtRetAltMax_Change()
RetAltMax = Val(txtRetAltMax)
End Sub

Private Sub txtRetAltMin_Change()
RetAltMin = Val(txtRetAltMin)
End Sub


Private Sub txtRollMax_Change()
eRollMax = Val(txtRollMax)
End Sub

Private Sub txtRollMin_Change()
eRollMin = Val(txtRollMin)
End Sub

Private Sub txtTropAltMax_Change()
TropAltMax = Val(txtTropAltMax)
End Sub


Private Sub txtTropAltMin_Change()
TropAltMin = Val(txtTropAltMin)
End Sub

Private Sub txtZtOffA_Change()
  ZtOffA = Val(txtZtOffA)
End Sub


Private Sub txtZtOffB_Change()
ZtOffB = Val(txtZtOffB)
End Sub


 


Sub UpdateSourceFile()

  YYYYMMDD$ = Trim(cboFltDates.Text)
  yymmdd$ = Right$(YYYYMMDD$, 6)
  YYYYMMDDI$ = YYYYMMDD$
  Starting_yyyymmdd$ = YYYYMMDD$
  Mission$ = cboMissions.Text
  Root$ = Drive$ + Rdir2$                     'eg C:\MTP\Data\DC8\
  MNpath$ = Root$ + Mission$ + "\"            'eg C:\MTP\Data\DC8\SOLVE\
  Call DirCheck(Root$, Mission$)
  RCpath$ = MNpath$ + "RC\"                   'eg C:\MTP\Data\DC8\SOLVE\RC\
  Call DirCheck(MNpath, "RC")
  MPpath$ = MNpath$ + "MP\"
  Call DirCheck(MNpath, "MP")
  Path$ = MNpath$ + YYYYMMDD$ + "\"           'eg C:\MTP\Data\DC8\SOLVE\20000120\
  Call DirCheck(MNpath, YYYYMMDD$)
  SUpath$ = MNpath$ + "Setup\"
  Call DirCheck(MNpath, "Setup")
  txtSourceFile.Text = DataDrive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
  txtDestinationFile.Text = DataDrive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\" + AC$ + YYYYMMDD$ + ".REF"
' Check if IN file should be automatically loaded
  If chkAutoLoad.Value = 1 Then cmdFlight_Click (5)   'cmdINfile_Click

End Sub
Sub UpdateForm()
Dim i%, j%, k%, n$, LI, RCcmd$, DandT$

  YYYYMMDDI$ = YYYYMMDD$
  yymmdd$ = Right$(YYYYMMDD$, 6)
  Drive$ = DataDrive$
  PgmDrive$ = ProgramDrive$
' Update form with information from FLTINFO file
  DandT$ = "yyyy mm dd hh:mm:ss"
' Derived parameters
  AC$ = Mid$(Rdir$, 2, 2)
  Rdir2$ = Rdir1$ + "\MTP\Data" + Rdir$
  Root$ = Drive$ + Rdir2$
  MNpath$ = Root$ + Mission$ + "\"
  RCpath$ = MNpath$ + "RC\"
  MPpath$ = MNpath$ + "MP\"
  Path$ = MNpath$ + YYYYMMDD$ + "\"
  PathDot$ = Path$ + AC$ + YYYYMMDD$ + "."
  RCs$ = Left$(AC$, 1) + "RC"
' Default Fully-Qualified File Names
  BINfile$ = PathDot$ + "BIN"
  CALfile$ = PathDot$ + "CAL"
  ERFfile$ = PathDot$ + "ERF"
  INPfile$ = PathDot$ + "INP"
  LOGfile$ = PathDot$ + "LOG"
  MMSfile$ = MNpath$ + "MMS\MM" + YYYYMMDD$ + "." + Platform$
  RAOBfile$ = RSpath$ + Mission$ + ".RAOB2"
  RAWfile$ = PathDot$ + "RAW"
  REFfile$ = PathDot$ + "REF"
  RTSfile$ = PathDot$ + "RTS"
  OUTfile$ = PathDot$ + "OUT"
  MPfile$ = MPpath$ + "MP" + YYYYMMDD$ + "." + Platform$
'  USEfile$ = RCpath$ + USE5$ + ".USE"

  txtRCpath.Text = RCpath$
  For i = 0 To cboPlatforms.ListCount - 1
    If cboPlatforms.List(i) = Mid$(Rdir$, 2, Len(Rdir$) - 2) Then cboPlatforms.ListIndex = i: Exit For
  Next i

' Load RCs
  For j = 0 To NRCmax - 1: cboReg(j).Clear: Next j
'  cboUSE.Clear

' Load all the default RCs into the combo box and get the RCtemplates
  NRCavailable = fNRCavailable()   'This loads combo boxes with all available RCs
  j = fLoadMissionRCs(txtRCpath.Text) 'Combo .text boxes filled with all RCs
  If j = 0 Then
    MsgBox "No Retrieval Coefficients were found in the path:" + vbCrLf + txtRCpath.Text, vbOKOnly
  End If
  DoEvents
  
'  j = fLoadMissionRCs(txtRCfile.Text) 'Combo .text boxes filled with all RCs
'  If j = 0 Then
'    MsgBox "No Retrieval Coefficients were found in the path:" + vbCrLf + txtRCfile.Text, vbOKOnly
'  End If
    
' If RCs have already been selected, then replace the default ones
'  cmdRCfiles_Click (1)   'Clear USE
'  cmdRCfiles_Click (3)   'Clear RCs
  If NRC > NRCmax Then NRC = NRCmax
  txtNRC.Text = Str(NRC)
  For i = 0 To NRC - 1
    If Reg$(i) <> "" Then
      cboReg(i).Text = Reg$(i)
      txtRCformat(i).Text = Str(RCformat(i))
      txtRCuse(i).Text = Str(RCuse(i))
      For j = 0 To NRCavailable - 1
        'Debug.Print Reg(i); "  "; cboReg(0).List(j)
        If Reg$(i) = cboReg(0).List(j) Then
          cboReg(i).ToolTipText = RCtemplate$(j)
          Exit For
        End If
      Next j
    Else
      cboReg(i).Text = ""
      txtRCformat(i).Text = ""
      txtRCuse(i).Text = ""
    End If
  Next i
  
  For i = NRC To NRCmax - 1   'Clear remaining boxes
    cboReg(i).Text = ""
    txtRCformat(i).Text = ""
    txtRCuse(i).Text = ""
  Next i
  txtNRC.Text = NRC
'  cboUSE.Text = USE5$
' This is where FltDates are loaded
  For i = 0 To cboMissions.ListCount - 1
    If Trim(cboMissions.List(i)) = Mission$ Then cboMissions.ListIndex = i: Exit For
  Next i
 
  For i = 0 To cboFltDates.ListCount - 1
    If Trim(cboFltDates.List(i)) = YYYYMMDD$ Then Exit For
    If i = cboFltDates.ListCount - 1 Then
      MsgBox "Flight Date: " + YYYYMMDD$ + " was not found!", vbOKOnly
    End If
  Next i
  
  cboFltDates.Text = cboFltDates.List(i)
  cboFltNos.Text = cboFltNos.List(i)
  cboObjectives.Text = cboObjectives.List(i)
  
  If Left$(Pi$, 3) = "ngs" Then
    txtPI.Text = DefaultPI$
  Else
    txtPI.Text = Pi$
  End If
  txtTotalCycles.Text = Str$(TotalCycles%)
  'txtFltNumber = FltNumber$   ' mjmmjm
  txtChannels.Text = Str$(Channels)
  ' Number of "Counts" channels
  If Ncts > 0 Then txtNcts.Text = Str(Ncts) Else txtNcts.Text = "12"
  
  lstFLA.Clear
  For i = 1 To NFL: lstFLA.AddItem Format$(FLA(i) / 100, "#0.000"): Next i

  txtNFL.Text = Str$(NFL)

  Select Case SU$
  Case "DC8": cboSU.ListIndex = 0            'DC8
  Case "ER2S": cboSU.ListIndex = 1           'SU#1
  Case "ER2T": cboSU.ListIndex = 2           'SU#2
  Case "ER2G": cboSU.ListIndex = 3           'Global Hawk
  Case "RKW": cboSU.ListIndex = 4            'Rockwell
  Case "NGV", "CAN1": cboSU.ListIndex = 5    'MTPH1 on HIAPER
  Case "CAN2": cboSU.ListIndex = 6           'MTPH2 on HALO
  Case "CAN3": cboSU.ListIndex = 7           'MTPH3 at JPL
  End Select


  txtOATnavCOR.Text = Format(OATnavCOR, "#0.00")

  If GainScale = 0 Then
    Select Case CalSource$
    Case "Gain_Eqn"
    Case "OATnav"
    Case "MMS"
    Case ""
    End Select
    
  Else
    cboOATsource.ListIndex = OATsource - 1
    cboGscale.ListIndex = GainScale - 1
    cboREFsource.ListIndex = REFsource - 1
  End If

  txtMTPcor.Text = Str$(utMTPcor)
  txtLAT(0) = Str$(LAT1): txtLAT(1) = Str$(LAT2)
  txtLAT(2) = Str$(LAT3): txtLAT(3) = Str$(LAT4)

  'frmFLTINFO.cboAlgorithm.ListIndex = Algorithm

  If UseMMSpALT Then chkMMSpALT.Value = 1 Else chkMMSpALT.Value = 0
  txtALTfujCONST.Text = Str$(ALTfujCONST)
  txtALTfujSLOPE.Text = Str$(ALTfujSLOPE)

  txtDTavg.Text = DTavg
  txtDTrms.Text = DTrms
  txtMRIavg.Text = MRIavg
  txtMRIrms.Text = MRIrms

' LIMITS information
  txtUTmin.Text = Str(UTmin): txtUTmax.Text = Str(UTmax)
  txtUTksmin.Text = Str(UTksmin): txtUTksmax.Text = Str(UTksmax)
  
  txtTropAltMin = TropAltMin: txtTropAltMax = TropAltMax
  If EditTropAlt Then chkTropAlt.Value = 1 Else chkTropAlt.Value = 0
  chkTropAlt_Click

  txtRetAltMin = RetAltMin: txtRetAltMax = RetAltMax
  If EditRetAlt Then chkRetAlt.Value = 1 Else chkRetAlt.Value = 0
  chkRetAlt_Click

  txtZtOffA = ZtOffA: txtZtOffB = ZtOffB
  If EditZtOff Then chkZtOff.Value = 1 Else chkZtOff.Value = 0
  chkZtOff_Click

  txtTemperatureMin = TemperatureMin: txtTemperatureMax = TemperatureMax '
  If EditTemperature Then chkTemperature.Value = 1 Else chkTemperature.Value = 0
  chkTemperature_Click

  txtPitchMin = ePitchMin: txtPitchMax = ePitchMax
  If EditPitch Then chkPitch.Value = 1 Else chkPitch.Value = 0
  chkPitch_Click

  txtRollMin = eRollMin: txtRollMax = eRollMax
  If EditRoll Then chkRoll.Value = 1 Else chkRoll.Value = 0
  chkRoll_Click

  txtNlevMin.Text = Str$(NlevMin)
  txtNlevMax.Text = Str$(NlevMax)
'  If EditNlev Then chkEditNlev.value = 1 Else chkEditNlev.value = 0
'  chkEditNlev_Click

  txtRateMin.Text = Str$(RateMin)
  txtRateMax.Text = Str$(RateMax)
  If EditRate Then chkRate.Value = 1 Else chkRate.Value = 0
  chkRate_Click

  If OATtrop Then
    chkOATtrop.Value = 1
'    chkEditOATtrop.value = 1
  Else
    chkOATtrop.Value = 0
'    chkEditOATtrop.value = 0
  End If
  'chkEditOATtrop_Click  xxxmjm

  If EditNav Then chkEditNav.Value = 1 Else chkEditNav.Value = 0

  If EditTA Then chkEditTA.Value = 1 Else chkEditTA.Value = 0
  txtTAmin.Text = Str$(TAmin)
  txtTAmax.Text = Str$(TAmax)
  chkEditTA_Click

  If EditCts Then chkEditCts.Value = 1 Else chkEditCts.Value = 0
  txtCtsMIn.Text = Str$(CtsMin)
  txtCtsMax.Text = Str$(CtsMax)
  chkEditCts_Click

  If EditCycle Then chkEditCycle.Value = 1 Else chkEditCycle.Value = 0
  chkEditCycle_Click

  If EditRAWbad Then chkEditRAWbad.Value = 1 Else chkEditRAWbad.Value = 0
  chkEditRAWbad_Click

  If EditTtgt Then chkEditTtgt.Value = 1 Else chkEditTtgt.Value = 0
  txtTtgtMin.Text = Str$(TtgtMin)
  txtTtgtMax.Text = Str$(TtgtMax)
  chkEditTtgt_Click

  UpdatePC (Cycle%)

  txtOATzt1(0).Text = Str$(OATzt10)
  txtTzt1(0).Text = Str$(Tzt10)
  txtOATzt2(0).Text = Str$(OATzt20)
  txtTzt2(0).Text = Str$(Tzt20)
  txtOATks1(0).Text = Str$(OATks10)
  txtOATks2(0).Text = Str$(OATks20)

  txtOATzt1(1).Text = Str$(OATzt11)
  txtTzt1(1).Text = Str$(Tzt11)
  txtOATzt2(1).Text = Str$(OATzt21)
  txtTzt2(1).Text = Str$(Tzt21)
  txtOATks1(1).Text = Str$(OATks11)
  txtOATks2(1).Text = Str$(OATks21)

  txtOATzt1(2).Text = Str$(OATzt12)
  txtTzt1(2).Text = Str$(Tzt12)
  txtOATzt2(2).Text = Str$(OATzt22)
  txtTzt2(2).Text = Str$(Tzt22)
  txtOATks1(2).Text = Str$(OATks12)
  txtOATks2(2).Text = Str$(OATks22)

  txtOATzt1(3).Text = Str$(OATzt13)
  txtTzt1(3).Text = Str$(Tzt13)
  txtOATzt2(3).Text = Str$(OATzt23)
  txtTzt2(3).Text = Str$(Tzt23)
  txtOATks1(3).Text = Str$(OATks13)
  txtOATks2(3).Text = Str$(OATks23)
  chkOATtrop_Click

  If SubTrop Then chkSubstitute.Value = 1 Else chkSubstitute.Value = 0
  txtOATzt1(4).Text = Str$(OATzt14)
  txtTzt1(4).Text = Str$(Tzt14)
  txtOATzt2(4).Text = Str$(OATzt24)
  txtTzt2(4).Text = Str$(Tzt24)

  txtRAWstamp.Text = Format$(RAWstamp, DandT$)
  txtMMSstamp.Text = Format$(MMSstamp, DandT$)
  txtREFstamp.Text = Format$(REFstamp, DandT$)
  txtERFstamp.Text = Format$(ERFstamp, DandT$)
  txtCTCstamp.Text = Format$(CTCstamp, DandT$)

  txtFIver.Text = Format$(FLTINFOversion, DandT$)
  txtMAKEver.Text = Format$(MAKEversion, DandT$)
  EXEversion = FileDateTime(PgmDrive$ + "\MTP\VB6\EXE\MTPbin.exe")
  txtEXEver.Text = Format$(EXEversion, DandT$)
  txtEDITver.Text = Format$(EDITversion, DandT$)
  txtCALver.Text = Format$(CALversion, DandT$)

  txtChannels.Text = Channels
  txtNel.Text = Nel
  For i = 0 To 3
    txtMXRoffset(i).Text = aMXRoffset(i)
    txtTGToffset(i).Text = aTGToffset(i)
    txtNAVoffset(i).Text = aNAVoffset(i)
    txtNDoffset(i).Text = aNDoffset(i)
  Next i
 
  txtEmissivity.Text = Format(Emissivity, "0.000")
  txtReflectivity.Text = Format(Reflectivity, "0.000")
  txtNfit.Text = Nfit
  txtGenDate.Text = Gendate
  For i = 1 To 10
    If WCTdates(i) = "" Then Exit For
    lstWCTdates.List(i - 1) = WCTdates(i)
  Next i

' Window Correction Table (WCT)
  If EnableWCT Then chkEnableWCT.Value = 1 Else chkEnableWCT.Value = 0
  For i = 1 To Channels
    For j = 1 To Nel
      txtWCT(j - 1 + (i - 1) * Nel).Text = Format(WINcor(i, j), "#0.00")
    Next j
  Next i

  For i = 1 To Nfit
    cboGE(i - 1).Text = NP$(i)
    txtGEr(i - 1).Text = Format(GOF(i), "00.0")
    For j = 1 To Channels
      If i = 1 Then
        txtGEval((j - 1) * 5 + (i - 1)).Text = Format(GEC(j, i), "00.00")
      Else
        txtGEval((j - 1) * 5 + (i - 1)).Text = Format(GEC(j, i), "0.000")
      End If
    Next j
  Next i

' Gain limits
  For i = 1 To Channels
    txtGE1(i - 1).Text = GeqnMin(i): txtGE2(i - 1).Text = GeqnMax(i)
    txtNV1(i - 1).Text = GnavMin(i): txtNV2(i - 1).Text = GnavMax(i)
    txtND1(i - 1).Text = GndMin(i): txtND2(i - 1).Text = GndMax(i)
  Next i

' Counts Thresholds
  txtCMAcycles.Text = CMAcycles
  txtCMAcycles2.Text = CMAcycles2
  txtRFIthreshold.Text = RFIthreshold
  txtMUXthreshold.Text = MUXthreshold
  txtRFIiterations.Text = RFIiterations
  txtRFIiterations2.Text = RFIiterations2
  txtBadCycles.Text = BadCycles
  txtBadCycles2.Text = Badcycles2
  txtRAWextension.Text = RAWextension
  
  If CallingForm.Name = "frmMTPbin" Then
    If Right$(RAWextension, 2) <> "AW" Then
      CallingForm.cboPTfile.Text = Right$(RAWextension, 2)
      CallingForm.chkUseSubstitutedRAWfile.Value = 1
      CallingForm.cmdREF(0).Caption = "&Open R" + Trim(CallingForm.cboPTfile.Text)
    Else
      CallingForm.cboPTfile.ListIndex = 0
      CallingForm.chkUseSubstitutedRAWfile.Value = 0
      CallingForm.cmdREF(0).Caption = "&Open RAW"
    End If
  End If
  
  If UseMAforCB Then chkUseMAforCB.Value = 1 Else chkUseMAforCB.Value = 0
  If UseMAforCS Then chkUseMAforCS.Value = 1 Else chkUseMAforCS.Value = 0
  If UseMAforCSgain Then chkUseMAforCSgain.Value = 1 Else chkUseMAforCSgain.Value = 0
  If UseMAforCN Then chkUseMAforCN.Value = 1 Else chkUseMAforCN.Value = 0
  If UseMAforTtgt Then chkUseMAforTtgt.Value = 1 Else chkUseMAforTtgt.Value = 0
  If UseMAforTifa Then chkUseMAforTifa.Value = 1 Else chkUseMAforTifa.Value = 0

' Channel Information
  ChInfoSum = 0#
  For i = 1 To Channels
    txtChInfo(i - 1).Text = Format(ChInfo(i), "#0.00")
    ChInfoSum = ChInfoSum + ChInfo(i)
  Next i
  
' Fit Region
  txtXfit(0).Text = Str(TBfitX1)
  txtXfit(1).Text = Str(TBfitX2)
  txtYfit(0).Text = Str(TBfitY1)
  txtYfit(1).Text = Str(TBfitY2)

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    txtCnd0(i - 1).Text = Format(Cnd0(i), "##0.000")
    txtCnd1(i - 1).Text = Format(Cnd1(i), "#0.0000")
    txtCnd2(i - 1).Text = Format(Cnd2(i), "#0.0000")
  Next i
  txtTrefND.Text = TrefND
  txtLocHor.Text = Str(LocHor)
  If RHS Then chkLHS.Value = 0 Else chkLHS.Value = 1

  UpdateSourceFile
  Caption = "FLTINFO Summary for " + AC$ + "/" + Mission$ + "/" + YYYYMMDD$
  If Val(cboFileFormatIn) <> FileFormatIn Then cboFileFormatIn.Text = Str(FileFormatIn)
  
  If ATPrange = 0 Then ATPrange = 20
  txtATPrange.Text = ATPrange
  Read_HPA
  txtPcorr.Text = Format(Pcorr, "#0.00")
  DoEvents
  
End Sub

Sub UpdateFLTINFO()
Dim D$, i%, j%

'DoAll = False            'Always make DoAll FALSE except in MTPman

  Drive$ = DataDrive$
  Rdir$ = "\" + Trim(cboPlatforms.Text) + "\"
  Rdir2$ = Rdir1$ + "\MTP\Data\" + Trim(cboPlatforms.Text) + "\"
  PgmDrive$ = ProgramDrive$
  Mission$ = Trim(cboMissions.Text)
  YYYYMMDD$ = Trim(cboFltDates.Text)
  Yeer = Val(Left$(YYYYMMDD$, 4))
  Pi$ = txtPI.Text
  FltNumber$ = Trim(cboFltNos.Text)
  Path$ = Drive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\"
  frmFLTINFO.Caption = "FLTINFO Summary for " + AC$ + "/" + Mission$ + "/" + YYYYMMDD$

' CALIBRATION information
  For i = 0 To NRCmax - 1
    If Trim(cboReg(i).Text) <> "" Then
      RCformat(i) = Val(txtRCformat(i).Text)
      Reg(i) = cboReg(i).Text
      RCuse(i) = Val(txtRCuse(i).Text)
      NRC = i + 1
      aRC(i) = i    'fGetiRC(i, Me)
    Else
      RCformat(i) = -1
      Reg(i) = ""
    End If
    Reg0$ = Reg(0)
    Reg1$ = Reg(1)
    Reg2$ = Reg(2)
  Next i

  GainScale = cboGscale.ListIndex + 1
  REFsource = cboREFsource.ListIndex + 1
  OATsource = cboOATsource.ListIndex + 1

  SU$ = Trim(cboSU.Text)
'  USE5$ = cboUSE.Text
  NFL = Val(txtNFL.Text)
  For i = 1 To NFL
    FLA(i) = Val(100 * lstFLA.List(i - 1))
  Next i
  If txtNcts.Text <> "" Then Ncts = Val(txtNcts.Text) Else Ncts = 12
  
  utMTPcor = Val(txtMTPcor.Text)
  OATnavCOR = Val(txtOATnavCOR.Text)
' CalSource$ = cboCalSource.Text

  If chkMMSpALT.Value = 1 Then UseMMSpALT = True Else UseMMSpALT = False
  ALTfujCONST = Val(txtALTfujCONST.Text)
  ALTfujSLOPE = Val(txtALTfujSLOPE.Text)

  'Algorithm = cboAlgorithm.ListIndex

  If Algorithm = 2 Then UserLATs = True Else UserLATs = False
  LAT1 = Val(txtLAT(0).Text): LAT2 = Val(txtLAT(1).Text)
  LAT3 = Val(txtLAT(2).Text): LAT4 = Val(txtLAT(3).Text)

' Time and MRI Statistics
  DTavg = Val(txtDTavg.Text)
  DTrms = Val(txtDTrms.Text)
  MRIavg = Val(txtMRIavg.Text)
  MRIrms = Val(txtMRIrms.Text)

' LIMITS information
  UTksmin = Val(txtUTksmin.Text): UTksmax = Val(txtUTksmax.Text)

  TropAltMin = Val(txtTropAltMin.Text): TropAltMax = Val(txtTropAltMax.Text)
  If chkTropAlt.Value = 1 Then EditTropAlt = True Else EditTropAlt = False

  RetAltMin = Val(txtRetAltMin.Text): RetAltMax = Val(txtRetAltMax.Text)
  If chkRetAlt.Value = 1 Then EditRetAlt = True Else EditRetAlt = False

  ZtOffA = Val(txtZtOffA.Text): ZtOffB = Val(txtZtOffB.Text)
  If chkZtOff.Value = 1 Then EditZtOff = True Else EditZtOff = False

  TemperatureMin = Val(txtTemperatureMin.Text): TemperatureMax = Val(txtTemperatureMax.Text)
  If chkTemperature.Value = 1 Then EditTemperature = True Else EditTemperature = False

  ePitchMin = Val(txtPitchMin.Text): ePitchMax = Val(txtPitchMax.Text)
  If chkPitch.Value = 1 Then EditPitch = True Else EditPitch = False

  eRollMin = Val(txtRollMin.Text): eRollMax = Val(txtRollMax.Text)
  If chkRoll.Value = 1 Then EditRoll = True Else EditRoll = False


'If chkEditNlev.value = 1 Then EditNlev = True Else EditNlev = False
  NlevMin = Val(txtNlevMin.Text)
  NlevMax = Val(txtNlevMax.Text)

  If chkRate.Value = 1 Then EditRate = True Else EditRate = False
  RateMin = Val(txtRateMin.Text)
  RateMax = Val(txtRateMax.Text)

  If chkEditNav.Value = 1 Then EditNav = True Else EditNav = False

  If chkEditTA.Value = 1 Then EditTA = True Else EditTA = False
  TAmin = Val(txtTAmin.Text)
  TAmax = Val(txtTAmax.Text)

  If chkEditCts.Value = 1 Then EditCts = True Else EditCts = False
  CtsMin = Val(txtCtsMIn.Text)
  CtsMax = Val(txtCtsMax.Text)

  If chkEditCycle.Value = 1 Then EditCycle = True Else EditCycle = False

  If chkEditRAWbad.Value = 1 Then EditRAWbad = True Else EditRAWbad = False

  If chkEditTtgt.Value = 1 Then EditTtgt = True Else EditTtgt = False
  TtgtMin = Val(txtTtgtMin.Text)
  TtgtMax = Val(txtTtgtMax.Text)

  TotalCycles% = Val(txtScans(0).Text)
  Channels = Val(txtChannels.Text)
  'UTsec = Val(txtLimitsUT(0).Text)

' OAT Trop Information
  If chkOATtrop.Value = 1 Then OATtrop = True Else OATtrop = False

  OATzt10 = Val(txtOATzt1(0).Text)
  Tzt10 = Val(txtTzt1(0).Text)
  OATzt20 = Val(txtOATzt2(0).Text)
  Tzt20 = Val(txtTzt2(0).Text)
  OATks10 = Val(txtOATks1(0).Text)
  OATks20 = Val(txtOATks2(0).Text)

  OATzt11 = Val(txtOATzt1(1).Text)
  Tzt11 = Val(txtTzt1(1).Text)
  OATzt21 = Val(txtOATzt2(1).Text)
  Tzt21 = Val(txtTzt2(1).Text)
  OATks11 = Val(txtOATks1(1).Text)
  OATks21 = Val(txtOATks2(1).Text)

  OATzt12 = Val(txtOATzt1(2).Text)
  Tzt12 = Val(txtTzt1(2).Text)
  OATzt22 = Val(txtOATzt2(2).Text)
  Tzt22 = Val(txtTzt2(2).Text)
  OATks12 = Val(txtOATks1(2).Text)
  OATks22 = Val(txtOATks2(2).Text)

  OATzt13 = Val(txtOATzt1(3).Text)
  Tzt13 = Val(txtTzt1(3).Text)
  OATzt23 = Val(txtOATzt2(3).Text)
  Tzt23 = Val(txtTzt2(3).Text)
  OATks13 = Val(txtOATks1(3).Text)
  OATks23 = Val(txtOATks2(3).Text)

  If chkSubstitute.Value = 1 Then SubTrop = True Else SubTrop = False
  OATzt14 = Val(txtOATzt1(4).Text)
  Tzt14 = Val(txtTzt1(4).Text)
  OATzt24 = Val(txtOATzt2(4).Text)
  Tzt24 = Val(txtTzt2(4).Text)

' HISTORY Information
  D$ = txtRAWstamp.Text
  If D$ <> "" Then RAWstamp = CDate(D$) Else RAWstamp = Date$ + " " + Time$
  D$ = txtMMSstamp.Text
  If D$ <> "" Then MMSstamp = CDate(D$) Else MMSstamp = Date$ + " " + Time$
  D$ = txtREFstamp.Text
  If D$ <> "" Then REFstamp = CDate(D$) Else REFstamp = Date$ + " " + Time$
  D$ = txtERFstamp.Text
  If D$ <> "" Then ERFstamp = CDate(D$) Else ERFstamp = Date$ + " " + Time$
  D$ = txtCTCstamp.Text
  If D$ <> "" Then CTCstamp = CDate(D$) Else CTCstamp = Date$ + " " + Time$

  Channels = txtChannels.Text
  Nel = txtNel.Text
  For i = 0 To 3
    aMXRoffset(i) = txtMXRoffset(i).Text
    aTGToffset(i) = txtTGToffset(i).Text
    aNAVoffset(i) = txtNAVoffset(i).Text
    aNDoffset(i) = txtNDoffset(i).Text
  Next i
  MXRoffset = aMXRoffset(0)
  TGToffset = aTGToffset(0)
  NAVoffset = aNAVoffset(0)
  NDoffset = aNDoffset(0)
  
  Emissivity = txtEmissivity.Text
  Reflectivity = txtReflectivity.Text
  Nfit = txtNfit.Text
  Gendate$ = txtGenDate.Text
  For i = 1 To 10
'    If lstWCTdates.List(i - 1) = "" Then Exit For
    WCTdates(i) = lstWCTdates.List(i - 1)
  Next i
  
  If chkEnableWCT.Value = 1 Then EnableWCT = True Else EnableWCT = False
  For i = 1 To Channels
    For j = 1 To Nel
      WINcor(i, j) = Val(txtWCT(j - 1 + (i - 1) * Nel).Text)
    Next j
  Next i

  For i = 1 To Nfit
    NP$(i) = Trim(cboGE(i - 1).Text)
    GOF(i) = txtGEr(i - 1).Text
    For j = 1 To Channels
      GEC(j, i) = txtGEval((j - 1) * 5 + (i - 1)).Text
    Next j
  Next i

' Gain limits
  For i = 1 To Channels
    GeqnMin(i) = Val(txtGE1(i - 1).Text): GeqnMax(i) = Val(txtGE2(i - 1).Text)
    GnavMin(i) = Val(txtNV1(i - 1).Text): GnavMax(i) = Val(txtNV2(i - 1).Text)
    GndMin(i) = Val(txtND1(i - 1).Text): GndMax(i) = Val(txtND2(i - 1).Text)
  Next i

' Counts Thresholds
  CMAcycles = Val(txtCMAcycles.Text)
  CMAcycles2 = Val(txtCMAcycles2.Text)
  RFIthreshold = Val(txtRFIthreshold.Text)
  MUXthreshold = Val(txtMUXthreshold.Text)
  RFIiterations = Val(txtRFIiterations.Text)
  RFIiterations2 = Val(txtRFIiterations2.Text)
  BadCycles = Val(txtBadCycles.Text)
  Badcycles2 = Val(txtBadCycles2.Text)
  RAWextension = txtRAWextension.Text

  If chkUseMAforCB.Value = 1 Then UseMAforCB = True Else UseMAforCB = False
  If chkUseMAforCS.Value = 1 Then UseMAforCS = True Else UseMAforCS = False
  If chkUseMAforCSgain.Value = 1 Then UseMAforCSgain = True Else UseMAforCSgain = False
  If chkUseMAforCN.Value = 1 Then UseMAforCN = True Else UseMAforCN = False
  If chkUseMAforTtgt.Value = 1 Then UseMAforTtgt = True Else UseMAforTtgt = False
  If chkUseMAforTifa.Value = 1 Then UseMAforTifa = True Else UseMAforTifa = False

' Channel Information
  ChInfoSum = 0#
  For i = 1 To Channels
    ChInfo(i) = Val(txtChInfo(i - 1).Text)
    ChInfoSum = ChInfoSum + ChInfo(i)
  Next i

' Fit Region
  TBfitX1 = Val(txtXfit(0).Text)
  TBfitX2 = Val(txtXfit(1).Text)
  TBfitY1 = Val(txtYfit(0).Text)
  TBfitY2 = Val(txtYfit(1).Text)

  LocHor = Val(txtLocHor.Text)
  If chkLHS.Value = 0 Then RHS = True Else RHS = False

' Noise Diode Noise Temperatures
  For i = 1 To Channels
    Cnd0(i) = Val(txtCnd0(i - 1).Text)
    Cnd1(i) = Val(txtCnd1(i - 1).Text)
    Cnd2(i) = Val(txtCnd2(i - 1).Text)
  Next i
  TrefND = txtTrefND.Text
  
  ATPrange = Val(txtATPrange.Text)
  DoEvents
  
End Sub
