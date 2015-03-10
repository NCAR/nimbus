VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmBandpass 
   BackColor       =   &H80000004&
   Caption         =   "Bandpass Weight"
   ClientHeight    =   4695
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7710
   LinkTopic       =   "Form1"
   ScaleHeight     =   4695
   ScaleWidth      =   7710
   StartUpPosition =   3  'Windows Default
   Begin TabDlg.SSTab SSTab1 
      Height          =   4665
      Left            =   30
      TabIndex        =   0
      Top             =   -15
      Width           =   7680
      _ExtentX        =   13547
      _ExtentY        =   8229
      _Version        =   393216
      Tab             =   2
      TabHeight       =   520
      TabCaption(0)   =   "IF Bandpass"
      TabPicture(0)   =   "frmBandpass.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Label(6)"
      Tab(0).Control(1)=   "Label(2)"
      Tab(0).Control(2)=   "Label(7)"
      Tab(0).Control(3)=   "Label(10)"
      Tab(0).Control(4)=   "Label(8)"
      Tab(0).Control(5)=   "Label(9)"
      Tab(0).Control(6)=   "Label(11)"
      Tab(0).Control(7)=   "Label(12)"
      Tab(0).Control(8)=   "picBandpass"
      Tab(0).Control(9)=   "cmdStart(2)"
      Tab(0).Control(10)=   "txtYY2"
      Tab(0).Control(11)=   "txtYY1"
      Tab(0).Control(12)=   "txtZp"
      Tab(0).Control(13)=   "chkShowUSBLSB"
      Tab(0).Control(14)=   "chkChannel(0)"
      Tab(0).Control(15)=   "chkChannel(2)"
      Tab(0).Control(16)=   "chkChannel(1)"
      Tab(0).Control(17)=   "txtXX1"
      Tab(0).Control(18)=   "txtXX2"
      Tab(0).Control(19)=   "cboLO(0)"
      Tab(0).Control(20)=   "cboLO(1)"
      Tab(0).Control(21)=   "cboLO(2)"
      Tab(0).Control(22)=   "cmdBP"
      Tab(0).Control(23)=   "txtYY3"
      Tab(0).Control(24)=   "txtXX3"
      Tab(0).Control(25)=   "cmdExit(1)"
      Tab(0).Control(26)=   "txtBPavgD(0)"
      Tab(0).Control(27)=   "txtBPrmsD(0)"
      Tab(0).Control(28)=   "txtBPrms(0)"
      Tab(0).Control(29)=   "txtBPavg(0)"
      Tab(0).Control(30)=   "txtBPavg(1)"
      Tab(0).Control(31)=   "txtBPrms(1)"
      Tab(0).Control(32)=   "txtBPrmsD(1)"
      Tab(0).Control(33)=   "txtBPavgD(1)"
      Tab(0).Control(34)=   "txtBPavg(2)"
      Tab(0).Control(35)=   "txtBPrms(2)"
      Tab(0).Control(36)=   "txtBPrmsD(2)"
      Tab(0).Control(37)=   "txtBPavgD(2)"
      Tab(0).ControlCount=   38
      TabCaption(1)   =   "Setup"
      TabPicture(1)   =   "frmBandpass.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label(1)"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "Frame1"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "Frame2"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "txtOutFilename"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "cmdStart(1)"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "Frame3"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "cmdExit(2)"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "Frame6"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).ControlCount=   8
      TabCaption(2)   =   "Absorption Coefficient"
      TabPicture(2)   =   "frmBandpass.frx":0038
      Tab(2).ControlEnabled=   -1  'True
      Tab(2).Control(0)=   "Label(0)"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "Label(3)"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "Label(4)"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "picAbsorption"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "txtX1"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).Control(5)=   "txtX2"
      Tab(2).Control(5).Enabled=   0   'False
      Tab(2).Control(6)=   "txtY2"
      Tab(2).Control(6).Enabled=   0   'False
      Tab(2).Control(7)=   "txtY1"
      Tab(2).Control(7).Enabled=   0   'False
      Tab(2).Control(8)=   "cmdAbsorption"
      Tab(2).Control(8).Enabled=   0   'False
      Tab(2).Control(9)=   "lstZ"
      Tab(2).Control(9).Enabled=   0   'False
      Tab(2).Control(10)=   "txtZ"
      Tab(2).Control(10).Enabled=   0   'False
      Tab(2).Control(11)=   "cmdZ(0)"
      Tab(2).Control(11).Enabled=   0   'False
      Tab(2).Control(12)=   "cmdZ(1)"
      Tab(2).Control(12).Enabled=   0   'False
      Tab(2).Control(13)=   "cmdZ(2)"
      Tab(2).Control(13).Enabled=   0   'False
      Tab(2).Control(14)=   "cmdZ(3)"
      Tab(2).Control(14).Enabled=   0   'False
      Tab(2).Control(15)=   "cmdZ(4)"
      Tab(2).Control(15).Enabled=   0   'False
      Tab(2).Control(16)=   "txtX3"
      Tab(2).Control(16).Enabled=   0   'False
      Tab(2).Control(17)=   "chkShowLiebe93"
      Tab(2).Control(17).Enabled=   0   'False
      Tab(2).Control(18)=   "cboShowLine"
      Tab(2).Control(18).Enabled=   0   'False
      Tab(2).Control(19)=   "txtX4"
      Tab(2).Control(19).Enabled=   0   'False
      Tab(2).Control(20)=   "txtY3"
      Tab(2).Control(20).Enabled=   0   'False
      Tab(2).Control(21)=   "chkMultiplePlots"
      Tab(2).Control(21).Enabled=   0   'False
      Tab(2).Control(22)=   "cmdExit(0)"
      Tab(2).Control(22).Enabled=   0   'False
      Tab(2).Control(23)=   "Frame4"
      Tab(2).Control(23).Enabled=   0   'False
      Tab(2).ControlCount=   24
      Begin VB.Frame Frame4 
         Caption         =   "Components"
         Height          =   1050
         Left            =   6225
         TabIndex        =   94
         Top             =   2805
         Width           =   1365
         Begin VB.CheckBox chkAbsComponents 
            Caption         =   "O2"
            Height          =   210
            Index           =   1
            Left            =   75
            TabIndex        =   100
            Top             =   210
            Width           =   570
         End
         Begin VB.CheckBox chkAbsComponents 
            Caption         =   "N2"
            Height          =   210
            Index           =   2
            Left            =   825
            TabIndex        =   99
            Top             =   210
            Width           =   510
         End
         Begin VB.CheckBox chkAbsComponents 
            Caption         =   "Vapor"
            Height          =   210
            Index           =   3
            Left            =   75
            TabIndex        =   98
            Top             =   495
            Width           =   750
         End
         Begin VB.CheckBox chkAbsComponents 
            Caption         =   "Liquid"
            Height          =   210
            Index           =   4
            Left            =   75
            TabIndex        =   97
            Top             =   780
            Width           =   720
         End
         Begin VB.TextBox txtWaterVapor 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   825
            MultiLine       =   -1  'True
            TabIndex        =   96
            Text            =   "frmBandpass.frx":0054
            ToolTipText     =   "Water Vapor (g/m3)"
            Top             =   435
            Width           =   390
         End
         Begin VB.TextBox txtWaterLiquid 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   825
            MultiLine       =   -1  'True
            TabIndex        =   95
            Text            =   "frmBandpass.frx":0056
            ToolTipText     =   "Liquid Water (g/m3)"
            Top             =   720
            Width           =   390
         End
      End
      Begin VB.TextBox txtBPavgD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   91
         Text            =   "frmBandpass.frx":0058
         Top             =   3180
         Width           =   495
      End
      Begin VB.TextBox txtBPrmsD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   90
         Text            =   "frmBandpass.frx":005C
         Top             =   3180
         Width           =   495
      End
      Begin VB.TextBox txtBPrms 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   89
         Text            =   "frmBandpass.frx":0060
         Top             =   2895
         Width           =   495
      End
      Begin VB.TextBox txtBPavg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   2
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   88
         Text            =   "frmBandpass.frx":0064
         Top             =   2895
         Width           =   495
      End
      Begin VB.TextBox txtBPavgD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   85
         Text            =   "frmBandpass.frx":0068
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtBPrmsD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   84
         Text            =   "frmBandpass.frx":006C
         Top             =   2220
         Width           =   495
      End
      Begin VB.TextBox txtBPrms 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   83
         Text            =   "frmBandpass.frx":0070
         Top             =   1935
         Width           =   495
      End
      Begin VB.TextBox txtBPavg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   1
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   82
         Text            =   "frmBandpass.frx":0074
         Top             =   1935
         Width           =   495
      End
      Begin VB.TextBox txtBPavg 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   79
         Text            =   "frmBandpass.frx":0078
         Top             =   990
         Width           =   495
      End
      Begin VB.TextBox txtBPrms 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   78
         Text            =   "frmBandpass.frx":007C
         Top             =   990
         Width           =   495
      End
      Begin VB.TextBox txtBPrmsD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -67905
         MultiLine       =   -1  'True
         TabIndex        =   77
         Text            =   "frmBandpass.frx":0080
         Top             =   1275
         Width           =   495
      End
      Begin VB.TextBox txtBPavgD 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   -68430
         MultiLine       =   -1  'True
         TabIndex        =   76
         Text            =   "frmBandpass.frx":0084
         Top             =   1275
         Width           =   495
      End
      Begin VB.Frame Frame6 
         Caption         =   "Optimize LO "
         Height          =   1320
         Left            =   -72870
         TabIndex        =   70
         Top             =   3285
         Width           =   1500
         Begin VB.CommandButton cmdStart 
            Caption         =   "Run LO Shift"
            Height          =   375
            Index           =   0
            Left            =   90
            TabIndex        =   74
            Top             =   855
            Width           =   1275
         End
         Begin VB.TextBox txtShift 
            Alignment       =   1  'Right Justify
            Height          =   315
            Index           =   0
            Left            =   105
            MultiLine       =   -1  'True
            TabIndex        =   73
            Text            =   "frmBandpass.frx":0088
            Top             =   240
            Width           =   435
         End
         Begin VB.TextBox txtShift 
            Alignment       =   1  'Right Justify
            Height          =   315
            Index           =   1
            Left            =   600
            MultiLine       =   -1  'True
            TabIndex        =   72
            Text            =   "frmBandpass.frx":008D
            Top             =   240
            Width           =   300
         End
         Begin VB.TextBox txtShift 
            Alignment       =   1  'Right Justify
            Height          =   315
            Index           =   2
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   71
            Text            =   "frmBandpass.frx":0092
            Top             =   240
            Width           =   420
         End
         Begin VB.Label Label 
            Caption         =   "Start   Step  End"
            Height          =   225
            Index           =   5
            Left            =   120
            TabIndex        =   75
            Top             =   585
            Width           =   1290
         End
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "E&xit"
         Height          =   360
         Index           =   2
         Left            =   -68025
         TabIndex        =   68
         Top             =   4245
         Width           =   645
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "E&xit"
         Height          =   360
         Index           =   1
         Left            =   -68010
         TabIndex        =   67
         Top             =   4245
         Width           =   645
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "E&xit"
         Height          =   360
         Index           =   0
         Left            =   6990
         TabIndex        =   66
         Top             =   4260
         Width           =   645
      End
      Begin VB.TextBox txtXX3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72090
         MultiLine       =   -1  'True
         TabIndex        =   65
         Text            =   "frmBandpass.frx":0098
         Top             =   4320
         Width           =   540
      End
      Begin VB.TextBox txtYY3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74955
         MultiLine       =   -1  'True
         TabIndex        =   64
         Text            =   "frmBandpass.frx":009D
         Top             =   2115
         Width           =   360
      End
      Begin VB.CheckBox chkMultiplePlots 
         Caption         =   "Multiple Plots"
         Height          =   210
         Left            =   6255
         TabIndex        =   63
         Top             =   2610
         Width           =   1260
      End
      Begin VB.TextBox txtY3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   45
         MultiLine       =   -1  'True
         TabIndex        =   61
         Text            =   "frmBandpass.frx":00A3
         Top             =   2070
         Width           =   405
      End
      Begin VB.TextBox txtX4 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2280
         MultiLine       =   -1  'True
         TabIndex        =   59
         Text            =   "frmBandpass.frx":00A9
         Top             =   4290
         Width           =   525
      End
      Begin VB.ComboBox cboShowLine 
         Height          =   315
         Left            =   6255
         TabIndex        =   57
         Top             =   2040
         Width           =   1305
      End
      Begin VB.CommandButton cmdBP 
         Caption         =   "BB"
         Height          =   315
         Left            =   -67845
         TabIndex        =   53
         ToolTipText     =   "Baseband (BB) or Bandpass (BP)"
         Top             =   345
         Width           =   420
      End
      Begin VB.ComboBox cboLO 
         Height          =   315
         Index           =   2
         Left            =   -68415
         TabIndex        =   52
         Text            =   "58.800"
         Top             =   2580
         Width           =   1005
      End
      Begin VB.ComboBox cboLO 
         Height          =   315
         Index           =   1
         Left            =   -68415
         TabIndex        =   51
         Text            =   "56.650"
         Top             =   1620
         Width           =   1005
      End
      Begin VB.ComboBox cboLO 
         Height          =   315
         Index           =   0
         Left            =   -68430
         TabIndex        =   50
         Text            =   "55.510"
         Top             =   660
         Width           =   1035
      End
      Begin VB.Frame Frame3 
         Caption         =   "Filter Properties"
         Height          =   750
         Left            =   -72855
         TabIndex        =   45
         Top             =   1365
         Width           =   5430
         Begin VB.CommandButton cmdDo 
            Caption         =   "Do"
            Height          =   300
            Left            =   4890
            TabIndex        =   56
            Top             =   240
            Width           =   420
         End
         Begin VB.TextBox txtKL3dB 
            Alignment       =   1  'Right Justify
            Enabled         =   0   'False
            Height          =   285
            Left            =   4305
            MultiLine       =   -1  'True
            TabIndex        =   55
            Text            =   "frmBandpass.frx":00AF
            Top             =   255
            Width           =   525
         End
         Begin VB.TextBox txtFWHM 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   2550
            MultiLine       =   -1  'True
            TabIndex        =   49
            Text            =   "frmBandpass.frx":00B4
            Top             =   285
            Width           =   525
         End
         Begin VB.TextBox txtFcenter 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   870
            MultiLine       =   -1  'True
            TabIndex        =   48
            Text            =   "frmBandpass.frx":00BA
            Top             =   300
            Width           =   525
         End
         Begin VB.Label Label3 
            Caption         =   "KL 3 dB (MHz)"
            Height          =   300
            Left            =   3180
            TabIndex        =   54
            Top             =   300
            Width           =   1170
         End
         Begin VB.Label Label2 
            Caption         =   "FWHM (MHz)"
            Height          =   255
            Left            =   1485
            TabIndex        =   47
            Top             =   285
            Width           =   1050
         End
         Begin VB.Label Label1 
            Caption         =   "Fc (MHz)"
            Height          =   255
            Left            =   120
            TabIndex        =   46
            Top             =   300
            Width           =   720
         End
      End
      Begin VB.CommandButton cmdStart 
         Caption         =   "Write Wt Table"
         Height          =   375
         Index           =   1
         Left            =   -70995
         TabIndex        =   42
         Top             =   4230
         Width           =   1530
      End
      Begin VB.TextBox txtOutFilename 
         Height          =   285
         Left            =   -70980
         TabIndex        =   41
         Text            =   "C:\MTP\Setup\Bandpass.txt"
         Top             =   3915
         Width           =   3555
      End
      Begin VB.TextBox txtXX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -69690
         MultiLine       =   -1  'True
         TabIndex        =   39
         Text            =   "frmBandpass.frx":00BE
         Top             =   4305
         Width           =   540
      End
      Begin VB.TextBox txtXX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74595
         MultiLine       =   -1  'True
         TabIndex        =   38
         Text            =   "frmBandpass.frx":00C4
         Top             =   4305
         Width           =   540
      End
      Begin VB.CheckBox chkChannel 
         Caption         =   "Ch2"
         Height          =   240
         Index           =   1
         Left            =   -69075
         TabIndex        =   36
         Top             =   1695
         Value           =   1  'Checked
         Width           =   600
      End
      Begin VB.CheckBox chkChannel 
         Caption         =   "Ch3"
         Height          =   240
         Index           =   2
         Left            =   -69090
         TabIndex        =   35
         Top             =   2640
         Value           =   1  'Checked
         Width           =   600
      End
      Begin VB.CheckBox chkChannel 
         Caption         =   "Ch1"
         Height          =   240
         Index           =   0
         Left            =   -69075
         TabIndex        =   34
         Top             =   735
         Value           =   1  'Checked
         Width           =   600
      End
      Begin VB.CheckBox chkShowUSBLSB 
         Caption         =   "Show SBs"
         Height          =   195
         Left            =   -69105
         TabIndex        =   33
         Top             =   4350
         Value           =   1  'Checked
         Width           =   1035
      End
      Begin VB.TextBox txtZp 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68520
         MultiLine       =   -1  'True
         TabIndex        =   31
         Text            =   "frmBandpass.frx":00CA
         ToolTipText     =   "Pressure Altitude of calculation"
         Top             =   4005
         Width           =   435
      End
      Begin VB.TextBox txtYY1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74940
         MultiLine       =   -1  'True
         TabIndex        =   30
         Text            =   "frmBandpass.frx":00CD
         Top             =   4005
         Width           =   360
      End
      Begin VB.TextBox txtYY2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74955
         MultiLine       =   -1  'True
         TabIndex        =   29
         Text            =   "frmBandpass.frx":00D1
         Top             =   405
         Width           =   360
      End
      Begin VB.CommandButton cmdStart 
         Caption         =   "Draw"
         Height          =   375
         Index           =   2
         Left            =   -69135
         TabIndex        =   28
         Top             =   3525
         Width           =   1740
      End
      Begin VB.CheckBox chkShowLiebe93 
         Caption         =   "Show Liebe 93"
         Height          =   195
         Left            =   6255
         TabIndex        =   27
         Top             =   2385
         Width           =   1365
      End
      Begin VB.TextBox txtX3 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   4095
         MultiLine       =   -1  'True
         TabIndex        =   26
         Text            =   "frmBandpass.frx":00D3
         Top             =   4305
         Width           =   525
      End
      Begin VB.CommandButton cmdZ 
         Caption         =   "Replace"
         Height          =   255
         Index           =   4
         Left            =   6780
         TabIndex        =   25
         Top             =   1545
         Width           =   795
      End
      Begin VB.CommandButton cmdZ 
         Caption         =   "Default"
         Height          =   255
         Index           =   3
         Left            =   6780
         TabIndex        =   24
         Top             =   1275
         Width           =   795
      End
      Begin VB.CommandButton cmdZ 
         Caption         =   "Clear"
         Height          =   255
         Index           =   2
         Left            =   6780
         TabIndex        =   23
         Top             =   990
         Width           =   795
      End
      Begin VB.CommandButton cmdZ 
         Caption         =   "Sub"
         Height          =   255
         Index           =   1
         Left            =   6780
         TabIndex        =   22
         Top             =   705
         Width           =   795
      End
      Begin VB.CommandButton cmdZ 
         Caption         =   "Add"
         Height          =   255
         Index           =   0
         Left            =   6780
         TabIndex        =   21
         Top             =   420
         Width           =   795
      End
      Begin VB.TextBox txtZ 
         Height          =   285
         Left            =   6255
         TabIndex        =   20
         Top             =   405
         Width           =   495
      End
      Begin VB.ListBox lstZ 
         Height          =   1035
         ItemData        =   "frmBandpass.frx":00D7
         Left            =   6255
         List            =   "frmBandpass.frx":00DE
         TabIndex        =   19
         Top             =   705
         Width           =   480
      End
      Begin VB.CommandButton cmdAbsorption 
         Caption         =   "Draw"
         Height          =   375
         Left            =   6240
         TabIndex        =   18
         Top             =   3870
         Width           =   1395
      End
      Begin VB.TextBox txtY1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   60
         MultiLine       =   -1  'True
         TabIndex        =   17
         Text            =   "frmBandpass.frx":00E6
         Top             =   3960
         Width           =   405
      End
      Begin VB.TextBox txtY2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   60
         MultiLine       =   -1  'True
         TabIndex        =   16
         Text            =   "frmBandpass.frx":00EA
         Top             =   405
         Width           =   405
      End
      Begin VB.TextBox txtX2 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   5790
         MultiLine       =   -1  'True
         TabIndex        =   15
         Text            =   "frmBandpass.frx":00F0
         Top             =   4275
         Width           =   405
      End
      Begin VB.TextBox txtX1 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   465
         MultiLine       =   -1  'True
         TabIndex        =   14
         Text            =   "frmBandpass.frx":00F5
         Top             =   4275
         Width           =   405
      End
      Begin VB.PictureBox picAbsorption 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00E0E0E0&
         Height          =   3855
         Left            =   465
         ScaleHeight     =   3795
         ScaleWidth      =   5670
         TabIndex        =   13
         Top             =   405
         Width           =   5730
      End
      Begin VB.Frame Frame2 
         Caption         =   "IF Model"
         Height          =   3240
         Left            =   -74910
         TabIndex        =   5
         Top             =   1365
         Width           =   1995
         Begin VB.TextBox txtIFmodel 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   75
            MultiLine       =   -1  'True
            TabIndex        =   12
            Top             =   210
            Width           =   885
         End
         Begin VB.CommandButton cmdIFmodel 
            Caption         =   "Replace"
            Height          =   360
            Index           =   4
            Left            =   1020
            TabIndex        =   11
            Top             =   1875
            Width           =   780
         End
         Begin VB.ListBox lstIFmodel 
            Height          =   2595
            Left            =   75
            TabIndex        =   10
            Top             =   510
            Width           =   915
         End
         Begin VB.CommandButton cmdIFmodel 
            Caption         =   "Add"
            Height          =   360
            Index           =   0
            Left            =   1020
            TabIndex        =   9
            Top             =   210
            Width           =   780
         End
         Begin VB.CommandButton cmdIFmodel 
            Caption         =   "Remove"
            Height          =   360
            Index           =   1
            Left            =   1020
            TabIndex        =   8
            Top             =   615
            Width           =   780
         End
         Begin VB.CommandButton cmdIFmodel 
            Caption         =   "Clear"
            Height          =   360
            Index           =   2
            Left            =   1020
            TabIndex        =   7
            Top             =   1035
            Width           =   780
         End
         Begin VB.CommandButton cmdIFmodel 
            Caption         =   "Default"
            Height          =   360
            Index           =   3
            Left            =   1020
            TabIndex        =   6
            Top             =   1455
            Width           =   780
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "Filter Type"
         Height          =   1005
         Left            =   -74880
         TabIndex        =   2
         Top             =   345
         Width           =   7455
         Begin VB.CommandButton cmdReadBandpass 
            Caption         =   "Read Bandpass"
            Height          =   345
            Left            =   5820
            TabIndex        =   69
            Top             =   540
            Width           =   1560
         End
         Begin VB.OptionButton optIFmodel 
            Caption         =   "Gaussian"
            Height          =   195
            Index           =   2
            Left            =   120
            TabIndex        =   44
            Top             =   615
            Width           =   1035
         End
         Begin VB.TextBox txtINPfile 
            Height          =   285
            Left            =   2835
            TabIndex        =   43
            Text            =   "C:\MTP\Setup\DC8_PAVE_IF_Bandpass20050425.txt"
            Top             =   210
            Width           =   4530
         End
         Begin VB.OptionButton optIFmodel 
            Caption         =   "Measured"
            Height          =   195
            Index           =   1
            Left            =   1770
            TabIndex        =   4
            Top             =   270
            Value           =   -1  'True
            Width           =   1035
         End
         Begin VB.OptionButton optIFmodel 
            Caption         =   "K&L Model"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   3
            Top             =   270
            Width           =   1035
         End
      End
      Begin VB.PictureBox picBandpass 
         AutoRedraw      =   -1  'True
         BackColor       =   &H00E0E0E0&
         Height          =   3885
         Left            =   -74595
         ScaleHeight     =   3825
         ScaleWidth      =   5370
         TabIndex        =   1
         Top             =   405
         Width           =   5430
      End
      Begin VB.Label Label 
         Caption         =   "U+ L"
         Height          =   180
         Index           =   12
         Left            =   -68790
         TabIndex        =   93
         Top             =   2940
         Width           =   420
      End
      Begin VB.Label Label 
         Caption         =   "U- L"
         Height          =   180
         Index           =   11
         Left            =   -68775
         TabIndex        =   92
         Top             =   3225
         Width           =   330
      End
      Begin VB.Label Label 
         Caption         =   "U+ L"
         Height          =   180
         Index           =   9
         Left            =   -68790
         TabIndex        =   87
         Top             =   1980
         Width           =   420
      End
      Begin VB.Label Label 
         Caption         =   "U- L"
         Height          =   180
         Index           =   8
         Left            =   -68775
         TabIndex        =   86
         Top             =   2265
         Width           =   330
      End
      Begin VB.Label Label 
         Caption         =   "U- L"
         Height          =   180
         Index           =   10
         Left            =   -68775
         TabIndex        =   81
         Top             =   1320
         Width           =   330
      End
      Begin VB.Label Label 
         Caption         =   "U+ L"
         Height          =   180
         Index           =   7
         Left            =   -68790
         TabIndex        =   80
         Top             =   1035
         Width           =   420
      End
      Begin VB.Label Label 
         Caption         =   "Mark Frequency"
         Height          =   210
         Index           =   4
         Left            =   6255
         TabIndex        =   62
         Top             =   1815
         Width           =   1290
      End
      Begin VB.Label Label 
         Caption         =   "Tic Mark (GHz)"
         Height          =   210
         Index           =   3
         Left            =   2970
         TabIndex        =   60
         Top             =   4335
         Width           =   1125
      End
      Begin VB.Label Label 
         Caption         =   "Resolution (GHz)"
         Height          =   210
         Index           =   0
         Left            =   1020
         TabIndex        =   58
         Top             =   4320
         Width           =   1320
      End
      Begin VB.Label Label 
         Caption         =   "Output Filename"
         Height          =   225
         Index           =   1
         Left            =   -70980
         TabIndex        =   40
         Top             =   3645
         Width           =   1245
      End
      Begin VB.Label Label 
         Caption         =   "LO Frequencies"
         Height          =   225
         Index           =   2
         Left            =   -69075
         TabIndex        =   37
         Top             =   435
         Width           =   1170
      End
      Begin VB.Label Label 
         Caption         =   "Zp (km)"
         Height          =   225
         Index           =   6
         Left            =   -69105
         TabIndex        =   32
         Top             =   4065
         Width           =   615
      End
   End
End
Attribute VB_Name = "frmBandpass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim iMax%, IFfreq!(1 To 1000), BP!(1 To 3, 1 To 1000), BPwt!(1 To 3)
Dim fu!(1 To 3, 1 To 1000), FL!(1 To 3, 1 To 1000)
Dim Au!(1 To 3, 1 To 1000), Al!(1 To 3, 1 To 1000)
Dim AuL!(1 To 3, 1 To 1000), AlL!(1 To 3, 1 To 1000)
Dim flag(1 To 4) As Boolean

Sub AbsAvgRms(T!, P!, V!, L!, fu!, FL!, flag() As Boolean, BP!(), BPwt!, n%, Avg!, RMS!, Diff!)
Dim x!, i%, AbsUSB!(1 To 1000), AbsLSB!(1 To 1000)
  
  For i = 1 To n
    AbsUSB(i) = ABS_TOT(T!, P!, V!, L, fu, flag())
    AbsLSB(i) = ABS_TOT(T!, P!, V!, L, FL, flag())
    x = (AbsUSB(i) + AbsLSB(i)) * BP(i) / 2#
    Avg = Avg + x
    RMS = RMS + x ^ 2
    Diff = Diff + (AbsUSB(i) - AbsLSB(i)) * BP(i)
  Next i
' Calculate Weighted Average
  Avg = Avg / BPwt
  RMS = RMS / BPwt ^ 2
' Calculate Weighted RMS
  If RMS - Avg ^ 2 * n > 0 Then  'Calculate RMS
    RMS = Sqr((RMS - Avg ^ 2 * n) / (n - 1))
  Else
    RMS(j) = 0#
  End If
  Diff = Diff / BPwt

End Sub


Sub DrawBandpass()
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, A!
Dim Xmin!, Xmax!, Ymin!, Ymax!, r%, iStep%, color&, Ystep!
  
  Xmin = Val(txtXX1.Text)
  Xmax = Val(txtXX2.Text)
  Xstep = Val(txtXX3.Text)
  Ymin = Val(txtYY1.Text)
  Ymax = Val(txtYY2.Text)
  Ystep = Val(txtYY3.Text)

' Plot Flight Track in picture window
  With picBandpass
    .Cls
    .ScaleMode = 0
    .ScaleLeft = Xmin
    .ScaleWidth = Xmax - Xmin
    .ScaleTop = Ymax
    .ScaleHeight = Ymin - Ymax
  End With
  
' Draw y marks
  For A = 0 To Ymax Step Ystep
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
  For i = Xmin To Xmax Step Xstep
    picBandpass.PSet (i, Ymin)
    picBandpass.Line -(i, Ymax), QBColor(0)
  Next i

' Draw bandpass
  For i = 1 To 3
    If chkChannel(i - 1).value = 1 Then
      Select Case i
      Case 1: color = QBColor(13)
      Case 2: color = QBColor(8)
      Case 3: color = QBColor(11)
      End Select
      picBandpass.DrawWidth = 2
      picBandpass.PSet (IFfreq(1), BP(i, 1) * Ymax * 0.95)
      For j = 1 To iMax
        picBandpass.Line -(IFfreq(j), BP(i, j) * Ymax * 0.95), color 'And continue from there
      Next j
      picBandpass.DrawWidth = 1

    ' Plot USB and LSB
      Select Case i
      Case 1: color = QBColor(12)
      Case 2: color = QBColor(15)
      Case 3: color = QBColor(9)
      End Select
      If chkShowUSBLSB.value = 1 Then
        picBandpass.PSet (IFfreq(1), Au(i, 1))
        For j = 1 To iMax
          picBandpass.Line -(IFfreq(j), Au(i, j)), color  'And continue from there
        Next j
        
        picBandpass.PSet (IFfreq(1), Al(i, 1))
        For j = 1 To iMax
          picBandpass.Line -(IFfreq(j), Al(i, j)), color  'And continue from there
        Next j
      End If
    
    End If
  Next i
End Sub

Function fGaussianFilter(f!, StdDev!, Fc!)
' Linear response to Gaussian Filter (normalized to 1)
' FWHM/(2*SQRT(2*LN(2)))
' B$6*EXP(-0.5*((B11-B$2)/B$8)^2)
fGaussianFilter = Exp(-0.5 * ((f - Fc) / StdDev) ^ 2)

End Function

Function fKLfilter(f!, FWHM!, Fc!)
Dim F1!, f2!, f3!, f4!, x!
' Linear response to K and L baseband filter
F1 = Fc - FWHM / 2#
f2 = F1 + 3
f4 = Fc + FWHM / 2#
f3 = f4 - 3

Select Case f
Case Is < F1: fKLfilter = 10 ^ (-3 / 10 + (f - F1) * 6.6 / 10#)
Case Is = F1: fKLfilter = 0.5
Case Is = F1 + 1: fKLfilter = 10 ^ (-2 / 10)
Case Is = F1 + 2: fKLfilter = 10 ^ (-1 / 10)
Case Is <= f3: fKLfilter = 1
Case Is = f4 - 2: fKLfilter = 10 ^ (-1 / 10)
Case Is = f4 - 1: fKLfilter = 10 ^ (-2 / 10)
Case Is = f4: fKLfilter = 0.5
Case Is > f4: fKLfilter = 10 ^ (-3 / 10 + (f4 - f) * 0.525 / 10)
End Select

End Function


Sub main()
End Sub




Private Sub cboShowLine_Click()
Dim Ymin!, Ymax!, f!

  Ymin = Val(txtY1.Text)
  Ymax = Val(txtY2.Text)
  f = Val(cboShowLine.Text)
  picAbsorption.PSet (f, Ymin)
  picAbsorption.Line -(f, Ymax), QBColor(13)

End Sub


Private Sub chkAbsComponents_Click(Index As Integer)
Dim i%


  If chkAbsComponents(Index).value = 1 Then
    flag(Index) = True
  Else
    flag(Index) = False
  End If
  

End Sub


Private Sub cmdAbsorption_Click()
Dim i%, j%, LineC&, TAmin!, TAmax!, A!, Zp!, T!, f!, P!, V!, L!
Dim r%, color&
Static Xmin!, Xmax!, Xstep!, Xres!, Ymin!, Ymax!, Ystep!

  
If chkMultiplePlots.value = 0 Then
  Xmin = Val(txtX1.Text)
  Xmax = Val(txtX2.Text)
  Xstep = Val(txtX3.Text)
  Xres = Val(txtX4.Text)
  
  Ymin = Val(txtY1.Text)
  Ymax = Val(txtY2.Text)
  Ystep = Val(txtY3.Text)
' Set scales
  With picAbsorption
    .Cls
    .ScaleMode = 0
    .ScaleLeft = Xmin
    .ScaleWidth = Xmax - Xmin
    .ScaleTop = Ymax
    .ScaleHeight = Ymin - Ymax
  End With
  
' Draw y marks
  For A = Ymin To Ymax Step Ystep
    picAbsorption.PSet (Xmin, A)
    picAbsorption.Line -(Xmax, A), QBColor(0)
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
  For i = Xmin To Xmax Step Xstep
    picAbsorption.PSet (i, Ymin)
    picAbsorption.Line -(i, Ymax), QBColor(0)
  Next i
End If
' Draw absorption
  V = Val(txtWaterVapor.Text)
  L = Val(txtWaterLiquid.Text)
  For i = 0 To lstZ.ListCount - 1
    Zp = lstZ.List(i)
    T = fTstd(Zp)
    P = fZtoP(Zp)
    Select Case i
    Case 0: color = QBColor(12)
    Case 1: color = QBColor(15)
    Case 2: color = QBColor(9)
    Case 3: color = QBColor(14)
    Case 4: color = QBColor(13)
    Case Else: color = QBColor(5)
    End Select
    picAbsorption.PSet (Xmin, ABS_TOT(T, P, V, L, Xmin, flag))
    For f = Xmin To Xmax Step Xres
      picAbsorption.Line -(f, ABS_TOT(T, P, V, L, f, flag)), color  'And continue from there
    Next f
    If chkShowLiebe93.value = 1 Then
      picAbsorption.PSet (Xmin, O2ABS(T, P, V, Xmin))
      For f = Xmin To Xmax Step Xres
        picAbsorption.Line -(f, O2ABS(T, P, V, f)), QBColor(13)  'And continue from there
      Next f
    End If
  Next i
End Sub


Private Sub cmdBP_Click()

If cmdBP.Caption = "BP" Then
  cboLO(0).ListIndex = 0
  cboLO(1).ListIndex = 0
  cboLO(2).ListIndex = 0
  cmdBP.Caption = "BB"
Else
  cboLO(0).ListIndex = 9
  cboLO(1).ListIndex = 12
  cboLO(2).ListIndex = 16
  cmdBP.Caption = "BP"
End If

End Sub

Private Sub cmdDo_Click()
cmdReadBandpass_Click
End Sub

Private Sub cmdExit_Click(Index As Integer)

  Unload Me
  End
End Sub

Private Sub cmdIFmodel_Click(Index As Integer)
Dim i%, A$

Select Case Index
Case 0  'Add
  lstIFmodel.AddItem txtPalt.Text
Case 1  'Remove
  lstIFmodel.RemoveItem (lstPalt.ListIndex)
Case 2  'Clear
  lstIFmodel.Clear
Case 3  'Default
  lstIFmodel.Clear
  If optIFmodel(0).value Then
    With lstIFmodel
    .AddItem "000"
    .AddItem "010"
    .AddItem "020"
    .AddItem "050"
    .AddItem "080"
    .AddItem "100"
    .AddItem "120"
    .AddItem "140"
    .AddItem "160"
    .AddItem "180"
    .AddItem "200"
    .AddItem "220"
    End With
  Else
    With lstIFmodel
    .AddItem "170"
    .AddItem "250"
    .AddItem "265"
    .AddItem "275"
    .AddItem "290"
    .AddItem "320"
    .AddItem "350"
    .AddItem "365"
    .AddItem "375"
    .AddItem "390"
    .AddItem "415"
    .AddItem "470"
    End With
  End If
  
Case 4  'Replace
  i = lstIFmodel.ListIndex
  A$ = txtIFmodel.Text
  lstIFmodel.List(i) = A$
  
End Select
End Sub



Private Sub cmdReadBandpass_Click()
Dim i%, INPlu%, A$, Fc!, FWHM!, StdDev!, F1!, f2!, iIF%, f%

' Manually enter frequencies defining the edges of each filter bin
' Program will use Bandpass measurements to calculated weighted average
' frequencies in each of these bins, and their weights.
' Note that bin edge frequencies must be at spectrum analyzer measurements points
' Make bins smaller when their is structure in the bandpass.
  
  BPwt(1) = 0#: BPwt(2) = 0#: BPwt(3) = 0#
  iIF = -1
  For i = 0 To 2
    If optIFmodel(i).value = True Then iIF = i
  Next i
  
  Select Case iIF
  Case 0    'K&L Baseband filter model
    txtFcenter.Enabled = False
    txtKL3dB.Enabled = True
    FWHM = Val(txtFWHM.Text)
    Fc = Val(txtKL3dB.Text) + FWHM / 2
    txtFcenter.Text = Str(Fc)
    For i = 1 To 300
      IFfreq(i) = i
      BP(1, i) = fKLfilter(IFfreq(i), FWHM, Fc)
      BP(2, i) = BP(1, i)
      BP(3, i) = BP(1, i)
      BPwt(1) = BPwt(1) + BP(1, i)
      BPwt(2) = BPwt(2) + BP(2, i)
      BPwt(3) = BPwt(3) + BP(3, i)
    Next i
    iMax = 300
    txtXX1.Text = Str(1)
    txtXX2.Text = Str(300)
    
  Case 1    'Measured Bandpass
    txtFcenter.Enabled = True
    txtKL3dB.Enabled = False
    BandpassFilename = txtINPfile.Text
    INPlu = FreeFile
    Open BandpassFilename For Input As INPlu
    'Frequency (MHz) CH1 CH2 CH3
    '170 0.018388544 0.019126418 0.019226293
    Input #INPlu, A$
    i = 0
    Do
      i = i + 1
      Input #INPlu, IFfreq(i), BP(1, i), BP(2, i), BP(3, i)
      BPwt(1) = BPwt(1) + BP(1, i)
      BPwt(2) = BPwt(2) + BP(2, i)
      BPwt(3) = BPwt(3) + BP(3, i)
    Loop Until EOF(INPlu)
    Close INPlu
    iMax = i
    txtXX1.Text = Str(170)
    txtXX2.Text = Str(470)
    txtXX3.Text = Str(50)
    
  Case 2   'Gaussian Filter Model
    txtFcenter.Enabled = True
    txtKL3dB.Enabled = False
    Fc = Val(txtFcenter.Text)
    FWHM = Val(txtFWHM.Text)
    StdDev = FWHM / (2 * Sqr(2 * Log(2)))
    F1 = Fc - 2.5 * FWHM / 2
    If F1 < 0 Then F1 = 1
    f2 = Fc + 2.5 * FWHM / 2
    i = 0
    For f = F1 To f2 Step 1
      i = i + 1
      IFfreq(i) = f
      BP(1, i) = fGaussianFilter(IFfreq(i), StdDev, Fc)
      BP(2, i) = BP(1, i)
      BP(3, i) = BP(1, i)
      BPwt(1) = BPwt(1) + BP(1, i)
      BPwt(2) = BPwt(2) + BP(2, i)
      BPwt(3) = BPwt(3) + BP(3, i)
    Next f
    iMax = i
    txtXX1.Text = Str(F1)
    txtXX2.Text = Str(f2)
    
  End Select

End Sub

Private Sub cmdStart_Click(Index As Integer)
Dim i%, j%, INPlu%, OUTlu%, iBin%, iLast%, iCount%
Dim A$, T!, P!, V!, L!, f!, Zp!, FloMax!(1 To 3), x!, y!, AbsRmsD!(1 To 3)
Dim Fusb!, Flsb!, AbsLSB!(1 To 500), AbsUSB!(1 To 500), AbsAvgD!(1 To 3)
Dim AbsAvg!(1 To 3), LO!(1 To 3), z!, AbsDiff!(1 To 3), AbsRms!(1 To 3)
Dim OutputFilename$, FB!(1 To 15), Fbin!(1 To 3, 1 To 15), Rbin!(1 To 3, 1 To 15)
Dim Metric!, IF1!, IF2!, ImaxP%
  
  IF1 = Val(txtXX1.Text)
  IF2 = Val(txtXX2.Text)
  
  For i = 0 To lstIFmodel.ListCount - 1
    FB(i + 1) = lstIFmodel.List(i)
  Next i
  iBin = lstIFmodel.ListCount
  LO(1) = cboLO(0).Text
  LO(2) = cboLO(1).Text
  LO(3) = cboLO(2).Text
        
' Calculate USB and LSB frequencies and absorption coefficients
' Note that the LSB spectrum is inverted in the IF
  Zp = Val(txtZp.Text)
  T = fTstd(Zp)
  P = fZtoP(Zp)
  V = Val(txtWaterVapor.Text)
  L = Val(txtWaterLiquid.Text)
  For j = 1 To 3
    AbsAvg(j) = 0#
    AbsRms(j) = 0#
    AbsAvgD(j) = 0#
    AbsRmsD(j) = 0#
    ImaxP = 0
    For i = 1 To iMax
      fu(j, i) = LO(j) + IFfreq(i) / 1000#
      Au(j, i) = ABS_TOT(T, P, V, L, fu(j, i), flag)  'Ros98
      AuL(j, i) = O2ABS!(T!, P!, V!, fu(j, i))        'Lie93
      FL(j, i) = LO(j) - IFfreq(i) / 1000#
      Al(j, i) = ABS_TOT(T, P, V, L, FL(j, i), flag)
      AlL(j, i) = O2ABS!(T!, P!, V!, FL(j, i))
      x = (Au(j, i) + Al(j, i)) / 2#
      y = (Au(j, i) - Al(j, i))
      If BP(j, i) > 0 Then
        ImaxP = ImaxP + 1
        AbsAvg(j) = AbsAvg(j) + BP(j, i) * x
        AbsRms(j) = AbsRms(j) + BP(j, i) * x ^ 2
        AbsAvgD(j) = AbsAvgD(j) + BP(j, i) * y
        AbsRmsD(j) = AbsRmsD(j) + BP(j, i) * y ^ 2
      End If
    Next i
  ' Calculate Weighted Average
    AbsAvg(j) = AbsAvg(j) / BPwt(j)
    txtBPavg(j - 1).Text = Format(AbsAvg(j), "0.00")
    
    If AbsRms(j) - BPwt(j) * AbsAvg(j) ^ 2 > 0 Then   'Calculate RMS
      AbsRms(j) = Sqr((AbsRms(j) - BPwt(j) * AbsAvg(j) ^ 2) / ((ImaxP - 1) * BPwt(j) / ImaxP))
    Else
      AbsRms(j) = 0#
    End If
    txtBPrms(j - 1).Text = Format(AbsRms(j), "0.00")
  
  ' The RMS on the average can be very small therefore look at difference
  ' Calculate Weighted RMS
  ' Sqr((Sum2 - 2 * Nobs * Xw ^ 2 + SWi * Xw ^ 2) / ((Nobs - 1) * SWi / Nobs))
    AbsAvgD(j) = AbsAvgD(j) / BPwt(j)
    txtBPavgD(j - 1).Text = Format(AbsAvgD(j), "0.00")
    
    If AbsRmsD(j) - BPwt(j) * AbsAvgD(j) ^ 2 > 0 Then   'Calculate RMS
      AbsRmsD(j) = Sqr((AbsRmsD(j) - BPwt(j) * AbsAvgD(j) ^ 2) / ((ImaxP - 1) * BPwt(j) / ImaxP))
    Else
      AbsRms(j) = 0#
    End If
    txtBPrmsD(j - 1).Text = Format(AbsRmsD(j), "0.00")
    
  Next j
  
  If Index = 2 Then   'Draw Bandpass
    DrawBandpass
    Exit Sub
  End If
  
  OutputFilename = txtOutFilename.Text
  OUTlu = FreeFile
  Open OutputFilename For Output As OUTlu
' Calculate bin response and weighted bin frequency
  iLast = 0
  For j = 1 To iBin - 1  'Bins are 1 to 2, 2 to 3, ...,iBin-1 to iBin
    iCount = 0
    i = iLast
    Do
      i = i + 1
      If IFfreq(i) >= FB(j) Then  'Each bin defined by FB(j) to FB(j+1)
        iCount = iCount + 1
        For k = 1 To 3
          Rbin(k, j) = Rbin(k, j) + BP(k, i)
          Fbin(k, j) = Fbin(k, j) + IFfreq(i) * BP(k, i)
        Next k
      End If
      If IFfreq(i) = FB(iBin) Then Exit Do
    Loop Until IFfreq(i + 1) > FB(j + 1)
    iLast = i
    For k = 1 To 3
      Rbin(k, j) = Rbin(k, j) / iCount
      Fbin(k, j) = Fbin(k, j) / iCount / Rbin(k, j)
    Next k
  Next j
  For i = 1 To 3
    For j = 1 To iBin - 1
      Debug.Print j; Fbin(i, j); Rbin(i, j)
    Next j
  Next i
  If Index = 1 Then   'Write Weight Table Only
    Close OUTlu
    Exit Sub
  End If
' If Index=0, Shift LO
  z = Val(txtZp.Text)
  P = fZtoP(z)
  T = fTstd(z)
  V = Val(txtWaterVapor.Text)
  L = Val(txtWaterLiquid.Text)
  Debug.Print "Zp"; z; "T"; T; Date; Time
  Print #OUTlu, Date; Time
  Print #OUTlu, "Zp"; z; "T"; T
  For j = 1 To 3
    If chkChannel(j - 1).value = 1 Then
      Debug.Print "Channel"; j
      Print #OUTlu, "Channel"; j
      AbsAvg(j) = 0#
      AbsRms(j) = 0#
      AbsDiff(j) = 0#
      Metric = 0#
      Print #OUTlu, " "; "LO MHz AbsAvg AbsDiff Metric"
      For k = Val(txtShift(0).Text) To Val(txtShift(2).Text) Step Val(txtShift(1).Text)
        For i = 1 To iMax
          Fusb = LO(j) + IFfreq(i) / 1000# + k / 1000#
          Flsb = LO(j) - IFfreq(i) / 1000# + k / 1000#
          AbsUSB(i) = ABS_TOT(T!, P!, V!, L, Fusb, flag())
          AbsLSB(i) = ABS_TOT(T!, P!, V!, L, Flsb, flag())
          x = (AbsUSB(i) + AbsLSB(i)) * BP(j, i) / 2#
          AbsAvg(j) = AbsAvg(j) + x
          AbsRms(j) = AbsRms(j) + x ^ 2
          AbsDiff(j) = AbsDiff(j) + (AbsUSB(i) - AbsLSB(i)) * BP(j, i)
        Next i
        ' Calculate Weighted Average
        AbsAvg(j) = AbsAvg(j) / BPwt(j)
        txtBPavg(j - 1).Text = Format(AbsAvg(j), "0.00")
        ' Calculate Weighted RMS
        If AbsRms(j) - AbsAvg(j) ^ 2 * iMax > 0 Then  'Calculate RMS
          AbsRms(j) = Sqr((AbsRms(j) - AbsAvg(j) ^ 2 * iMax) / (iMax - 1))
        Else
          AbsRms(j) = 0#
        End If
        txtBPrms(j - 1).Text = Format(AbsRms(j), "0.00")
        
        AbsDiff(j) = AbsDiff(j) / BPwt(j)
        x = AbsAvg(j) / AbsDiff(j)
        If x > Metric Then
          Metric = x
          FloMax(j) = LO(j) + k / 1000
        End If
        
' Figure of merit is largest average absorption divided by difference in USB/LSB response
' Should be highest when absorption is largest and difference between USB/LSB is smallest
        'Debug.Print LO(j) + k / 1000#; k; AbsAvg(j); AbsDiff(j); AbsAvg(j) / AbsDiff(j) / 10#
        Print #OUTlu, " "; LO(j) + k / 1000#; k; AbsAvg(j); AbsDiff(j); AbsAvg(j) / AbsDiff(j) / 10#
      Next k
      cboLO(j - 1).Text = FloMax(j)
    End If
  Next j
  Close OUTlu

End Sub



Private Sub cmdZ_Click(Index As Integer)
Dim i%, A$

Select Case Index
Case 0  'Add
  lstZ.AddItem txtZ.Text
Case 1  'Remove
  lstZ.RemoveItem (lstZ.ListIndex)
Case 2  'Clear
  lstZ.Clear
Case 3  'Default
  lstZ.Clear
    With lstZ
    .AddItem "0"
    .AddItem "10"
    .AddItem "15"
    .AddItem "20"
    End With
    
Case 4  'Replace
  i = lstZ.ListIndex
  A$ = txtZ.Text
  lstZ.List(i) = A$
  
End Select
End Sub

Private Sub Form_Load()
Dim f(1 To 40), i%

' Rosenkranz'98 33 60 GHz band lines
  f(1) = 51.5034: f(2) = 52.0214: f(3) = 52.5424: f(4) = 53.0669: f(5) = 53.5957
  f(6) = 54.13: f(7) = 54.6712: f(8) = 55.2214: f(9) = 55.7838: f(10) = 56.2648
  f(11) = 56.3634: f(12) = 56.9682: f(13) = 57.6125: f(14) = 58.3239: f(15) = 58.4466
  f(16) = 59.1642: f(17) = 59.591: f(18) = 60.3061: f(19) = 60.4348: f(20) = 61.1506
  f(21) = 61.8002: f(22) = 62.4112: f(23) = 62.4863: f(24) = 62.998: f(25) = 63.5685
  f(26) = 64.1278: f(27) = 64.6789: f(28) = 65.2241: f(29) = 65.7648: f(30) = 66.3021
  f(31) = 66.8368: f(32) = 67.3696: f(33) = 67.9009: f(34) = 118.7503: f(35) = 368.4984
  f(36) = 424.7632:  f(37) = 487.2494:  f(38) = 715.3931:  f(39) = 773.8397: f(40) = 834.1458

  cboLO(0).Text = 55.51
  cboLO(1).Text = 56.65
  cboLO(2).Text = 58.8
  cboLO(0).AddItem Format(55.51, "00.0000")
  cboLO(1).AddItem Format(56.65, "00.0000")
  cboLO(2).AddItem Format(58.8, "00.0000")

' Load frequency combo boxes
  For i = 1 To 40
    cboLO(0).AddItem Format(f(i), "00.0000")
    cboLO(1).AddItem Format(f(i), "00.0000")
    cboLO(2).AddItem Format(f(i), "00.0000")
    cboShowLine.AddItem Format(f(i), "00.0000")
  Next i

' Enable all absorption components
  For i = 1 To 4
    flag(i) = True
    chkAbsComponents(i).value = 1
  Next i

  cmdIFmodel_Click (3)  'load model
  cmdReadBandpass_Click

End Sub


Private Sub optIFmodel_Click(Index As Integer)

cmdReadBandpass_Click

End Sub

Private Sub picAbsorption_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
  
  picAbsorption.ToolTipText = "X=" + Format(x, "##0.0") + "," + "Y=" + Format(y, "##0.0")

End Sub


Private Sub picBandpass_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
  
  picBandpass.ToolTipText = "X=" + Format(x, "##0.0") + "," + "Y=" + Format(y, "##0.0")

End Sub


