VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmCurtain 
   AutoRedraw      =   -1  'True
   BorderStyle     =   0  'None
   Caption         =   "CTC Options"
   ClientHeight    =   5175
   ClientLeft      =   1260
   ClientTop       =   2385
   ClientWidth     =   3270
   Icon            =   "CTCoptions.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   5175
   ScaleWidth      =   3270
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdPlotMP 
      Caption         =   "&Plot MP"
      Height          =   375
      Left            =   750
      TabIndex        =   74
      ToolTipText     =   "Start CTC plotting"
      Top             =   4770
      Width           =   915
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   2670
      TabIndex        =   72
      ToolTipText     =   "Start CTC plotting"
      Top             =   4770
      Width           =   585
   End
   Begin VB.TextBox txtFile 
      Height          =   285
      Left            =   30
      TabIndex        =   70
      Top             =   4410
      Width           =   3195
   End
   Begin VB.CommandButton cmdDoAll 
      Caption         =   "Do All"
      Height          =   375
      Left            =   30
      TabIndex        =   53
      Top             =   4770
      Width           =   675
   End
   Begin VB.CommandButton cmdPlotCTC 
      Caption         =   "&Plot ERF"
      Height          =   375
      Left            =   1710
      TabIndex        =   52
      ToolTipText     =   "Start CTC plotting"
      Top             =   4770
      Width           =   915
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   60
      Top             =   4740
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4335
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3255
      _ExtentX        =   5741
      _ExtentY        =   7646
      _Version        =   393216
      TabHeight       =   520
      TabCaption(0)   =   "&Axis Limits"
      TabPicture(0)   =   "CTCoptions.frx":0442
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Frame5"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Frame1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Frame4"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).ControlCount=   3
      TabCaption(1)   =   "&Flags"
      TabPicture(1)   =   "CTCoptions.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "chkPlotGoodTrop"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "chkPlotGoodScan"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "chkTropopause"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "chkAircraft"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "chkLatitude"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "chkDocument"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "chkIncreasingLatitude"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "chkIncreasingLongitude"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).Control(8)=   "chkMRI"
      Tab(1).Control(8).Enabled=   0   'False
      Tab(1).Control(9)=   "chkEnableAveraging"
      Tab(1).Control(9).Enabled=   0   'False
      Tab(1).Control(10)=   "chkSubstituteLastATP"
      Tab(1).Control(10).Enabled=   0   'False
      Tab(1).Control(11)=   "chkPlotPalt"
      Tab(1).Control(11).Enabled=   0   'False
      Tab(1).Control(12)=   "chkPreliminary"
      Tab(1).Control(12).Enabled=   0   'False
      Tab(1).Control(13)=   "chkColdPoint"
      Tab(1).Control(13).Enabled=   0   'False
      Tab(1).ControlCount=   14
      TabCaption(2)   =   "Format"
      TabPicture(2)   =   "CTCoptions.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Frame7"
      Tab(2).Control(1)=   "Frame6"
      Tab(2).Control(2)=   "Frame3"
      Tab(2).Control(3)=   "Frame2"
      Tab(2).Control(4)=   "txtMission"
      Tab(2).Control(5)=   "txtFltDate"
      Tab(2).Control(6)=   "txtFltNumber"
      Tab(2).Control(7)=   "Label(0)"
      Tab(2).Control(8)=   "Label(1)"
      Tab(2).Control(9)=   "Label(2)"
      Tab(2).ControlCount=   10
      Begin VB.CheckBox chkColdPoint 
         Caption         =   "Show Cold Point"
         Height          =   195
         Left            =   -74865
         TabIndex        =   73
         Top             =   3645
         Width           =   1770
      End
      Begin VB.Frame Frame7 
         Caption         =   "Plot"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1095
         Left            =   -73050
         TabIndex        =   61
         Top             =   390
         Width           =   1215
         Begin VB.OptionButton optPlot 
            Caption         =   "IAC + CTC"
            Height          =   195
            Index           =   2
            Left            =   120
            TabIndex        =   64
            Top             =   780
            Width           =   1065
         End
         Begin VB.OptionButton optPlot 
            Caption         =   "IAC Only"
            Height          =   195
            Index           =   1
            Left            =   120
            TabIndex        =   63
            Top             =   510
            Width           =   975
         End
         Begin VB.OptionButton optPlot 
            Caption         =   "CTC Only"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   62
            Top             =   240
            Width           =   975
         End
      End
      Begin VB.Frame Frame6 
         Caption         =   "IAC Levels"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   855
         Left            =   -74880
         TabIndex        =   57
         Top             =   1800
         Width           =   3075
         Begin VB.ComboBox cboPTstart 
            Height          =   315
            Left            =   120
            TabIndex        =   69
            Text            =   "300"
            Top             =   480
            Width           =   735
         End
         Begin VB.ComboBox cboPTend 
            Height          =   315
            Left            =   930
            TabIndex        =   68
            Text            =   "500"
            Top             =   480
            Width           =   735
         End
         Begin VB.ComboBox cboPTstep 
            Height          =   315
            Left            =   1740
            TabIndex        =   67
            Text            =   "40"
            Top             =   480
            Width           =   735
         End
         Begin VB.ComboBox cboPTpen 
            Height          =   315
            Left            =   2520
            TabIndex        =   65
            Text            =   "2"
            Top             =   480
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "Pen"
            Height          =   255
            Index           =   6
            Left            =   2520
            TabIndex        =   66
            Top             =   240
            Width           =   495
         End
         Begin VB.Label Label 
            Caption         =   "PTstep"
            Height          =   255
            Index           =   5
            Left            =   1740
            TabIndex        =   60
            Top             =   240
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "PTend"
            Height          =   255
            Index           =   4
            Left            =   930
            TabIndex        =   59
            Top             =   240
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "PTstart"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   58
            Top             =   240
            Width           =   615
         End
      End
      Begin VB.CheckBox chkPreliminary 
         Caption         =   "Use PRELIMINARY"
         Height          =   255
         Left            =   -74880
         TabIndex        =   56
         Top             =   2160
         Width           =   1815
      End
      Begin VB.CheckBox chkPlotPalt 
         Caption         =   "Plot Pressure Altitude"
         Height          =   255
         Left            =   -74880
         TabIndex        =   55
         Top             =   3360
         Value           =   1  'Checked
         Width           =   1815
      End
      Begin VB.CheckBox chkSubstituteLastATP 
         Caption         =   "Substitute Last ATP"
         Height          =   255
         Left            =   -74880
         TabIndex        =   54
         Top             =   1920
         Width           =   1815
      End
      Begin VB.Frame Frame3 
         Caption         =   "Cycles"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1335
         Left            =   -72930
         TabIndex        =   45
         Top             =   2880
         Width           =   1125
         Begin VB.OptionButton optColorCycles 
            Caption         =   "&One"
            Height          =   252
            Index           =   0
            Left            =   60
            TabIndex        =   48
            Top             =   240
            Value           =   -1  'True
            Width           =   732
         End
         Begin VB.OptionButton optColorCycles 
            Caption         =   "&Two"
            Height          =   252
            Index           =   1
            Left            =   60
            TabIndex        =   47
            Top             =   480
            Width           =   732
         End
         Begin VB.OptionButton optColorCycles 
            Caption         =   "Thr&ee"
            Height          =   252
            Index           =   2
            Left            =   60
            TabIndex        =   46
            Top             =   720
            Width           =   735
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "Color Style"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1335
         Left            =   -74880
         TabIndex        =   41
         Top             =   2880
         Width           =   1875
         Begin VB.OptionButton optColorStyle 
            Caption         =   "&Continuous Colors"
            Height          =   255
            Index           =   3
            Left            =   60
            TabIndex        =   71
            Top             =   960
            Width           =   1575
         End
         Begin VB.OptionButton optColorStyle 
            Caption         =   "&Custom for MTP"
            Height          =   255
            Index           =   0
            Left            =   60
            TabIndex        =   44
            Top             =   240
            Value           =   -1  'True
            Width           =   1695
         End
         Begin VB.OptionButton optColorStyle 
            Caption         =   "Cycle &Hue 360 deg"
            Height          =   255
            Index           =   1
            Left            =   60
            TabIndex        =   43
            Top             =   480
            Width           =   1695
         End
         Begin VB.OptionButton optColorStyle 
            Caption         =   "&DIAL Colors"
            Height          =   255
            Index           =   2
            Left            =   60
            TabIndex        =   42
            Top             =   720
            Width           =   1575
         End
      End
      Begin VB.TextBox txtMission 
         Height          =   285
         Left            =   -74070
         TabIndex        =   40
         Top             =   480
         Width           =   975
      End
      Begin VB.TextBox txtFltDate 
         Height          =   285
         Left            =   -74070
         TabIndex        =   39
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtFltNumber 
         Height          =   285
         Left            =   -74070
         TabIndex        =   38
         Top             =   1170
         Width           =   975
      End
      Begin VB.Frame Frame4 
         Caption         =   "Z"
         Height          =   1335
         Left            =   120
         TabIndex        =   29
         Top             =   2880
         Width           =   3075
         Begin VB.OptionButton optCurtain 
            Caption         =   "Theta"
            Height          =   195
            Index           =   1
            Left            =   1620
            TabIndex        =   35
            Top             =   240
            Width           =   1125
         End
         Begin VB.OptionButton optCurtain 
            Caption         =   "Temperature"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   34
            Top             =   240
            Width           =   1215
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   1020
            MultiLine       =   -1  'True
            TabIndex        =   33
            Top             =   480
            Width           =   495
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   1020
            MultiLine       =   -1  'True
            TabIndex        =   32
            Top             =   840
            Width           =   495
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   4
            LargeChange     =   100
            Left            =   1590
            Max             =   3200
            Min             =   1600
            SmallChange     =   10
            TabIndex        =   31
            Top             =   480
            Value           =   1600
            Width           =   1395
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   5
            LargeChange     =   100
            Left            =   1590
            Max             =   3200
            Min             =   1600
            SmallChange     =   10
            TabIndex        =   30
            Top             =   840
            Value           =   1600
            Width           =   1395
         End
         Begin VB.Label Label4 
            Caption         =   "&Min T [K]"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   37
            Top             =   480
            Width           =   735
         End
         Begin VB.Label Label5 
            Caption         =   "M&ax T [K]"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   36
            Top             =   840
            Width           =   735
         End
      End
      Begin VB.Frame Frame1 
         Caption         =   "X"
         Height          =   1575
         Left            =   120
         TabIndex        =   18
         Top             =   360
         Width           =   3075
         Begin VB.OptionButton optAbscissa 
            Caption         =   "UT [k-sec]"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   26
            Top             =   480
            Width           =   1095
         End
         Begin VB.OptionButton optAbscissa 
            Caption         =   "Longitude [deg]"
            Height          =   255
            Index           =   2
            Left            =   1590
            TabIndex        =   25
            Top             =   480
            Width           =   1455
         End
         Begin VB.OptionButton optAbscissa 
            Caption         =   "Latitude [deg]"
            Height          =   255
            Index           =   1
            Left            =   1590
            TabIndex        =   24
            Top             =   240
            Width           =   1335
         End
         Begin VB.OptionButton optAbscissa 
            Caption         =   "UT [hr]"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   23
            Top             =   240
            Width           =   855
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   2
            LargeChange     =   10
            Left            =   1590
            Max             =   240
            TabIndex        =   22
            Top             =   870
            Width           =   1395
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   3
            LargeChange     =   10
            Left            =   1590
            Max             =   240
            TabIndex        =   21
            Top             =   1200
            Value           =   24
            Width           =   1395
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   20
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   19
            Top             =   1200
            Width           =   495
         End
         Begin VB.Label Label2 
            Caption         =   "&Left"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   28
            Top             =   840
            Width           =   375
         End
         Begin VB.Label Label3 
            Caption         =   "&Right"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   27
            Top             =   1200
            Width           =   495
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "Y"
         Height          =   975
         Left            =   120
         TabIndex        =   11
         Top             =   1920
         Width           =   3075
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   1
            LargeChange     =   10
            Left            =   1590
            Max             =   400
            TabIndex        =   15
            Top             =   600
            Value           =   28
            Width           =   1395
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   0
            LargeChange     =   10
            Left            =   1590
            Max             =   400
            TabIndex        =   14
            Top             =   240
            Width           =   1395
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   288
            Index           =   1
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   13
            Top             =   600
            Width           =   492
         End
         Begin VB.TextBox txtAxis 
            Alignment       =   1  'Right Justify
            Height          =   288
            Index           =   0
            Left            =   990
            MultiLine       =   -1  'True
            TabIndex        =   12
            Top             =   240
            Width           =   492
         End
         Begin VB.Label Label0 
            Caption         =   "&Bottom [km]"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   17
            Top             =   240
            Width           =   975
         End
         Begin VB.Label Label1 
            Caption         =   "&Top [km]"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   16
            Top             =   600
            Width           =   735
         End
      End
      Begin VB.CheckBox chkEnableAveraging 
         Caption         =   "Enable Averaging"
         Height          =   255
         Left            =   -74880
         TabIndex        =   10
         Top             =   2880
         Width           =   1695
      End
      Begin VB.CheckBox chkMRI 
         Caption         =   "Show MRI"
         Height          =   255
         Left            =   -74880
         TabIndex        =   9
         Top             =   2640
         Value           =   1  'Checked
         Width           =   1215
      End
      Begin VB.CheckBox chkIncreasingLongitude 
         Caption         =   "Increasing Longitude"
         Height          =   255
         Left            =   -74880
         TabIndex        =   8
         Top             =   720
         Width           =   1815
      End
      Begin VB.CheckBox chkIncreasingLatitude 
         Caption         =   "Increasing Latitude"
         Height          =   255
         Left            =   -74880
         TabIndex        =   7
         Top             =   480
         Width           =   1695
      End
      Begin VB.CheckBox chkDocument 
         Caption         =   "Show &Documentation"
         Height          =   255
         Left            =   -74880
         TabIndex        =   6
         Top             =   1440
         Width           =   1935
      End
      Begin VB.CheckBox chkLatitude 
         Caption         =   "Show &Latitude at Top"
         Enabled         =   0   'False
         Height          =   255
         Left            =   -74880
         TabIndex        =   5
         Top             =   1200
         Width           =   1935
      End
      Begin VB.CheckBox chkAircraft 
         Caption         =   "Show &A/C Altitude"
         Height          =   255
         Left            =   -74880
         TabIndex        =   4
         Top             =   2400
         Width           =   1815
      End
      Begin VB.CheckBox chkTropopause 
         Caption         =   "Show &Tropopause"
         Height          =   255
         Left            =   -74880
         TabIndex        =   3
         Top             =   960
         Width           =   1695
      End
      Begin VB.CheckBox chkPlotGoodScan 
         Caption         =   "Plot Only Good Scans"
         Height          =   255
         Left            =   -74880
         TabIndex        =   2
         Top             =   1680
         Width           =   1935
      End
      Begin VB.CheckBox chkPlotGoodTrop 
         Caption         =   "Plot Only Good Trops"
         Height          =   255
         Left            =   -74880
         TabIndex        =   1
         Top             =   3120
         Width           =   1815
      End
      Begin VB.Label Label 
         Caption         =   "Mission"
         Height          =   255
         Index           =   0
         Left            =   -74880
         TabIndex        =   51
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Flt Date"
         Height          =   255
         Index           =   1
         Left            =   -74880
         TabIndex        =   50
         Top             =   870
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Flt Number"
         Height          =   255
         Index           =   2
         Left            =   -74880
         TabIndex        =   49
         Top             =   1230
         Width           =   855
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileSavePNG 
         Caption         =   "Save PNG"
      End
      Begin VB.Menu mnuFileLoadPNG 
         Caption         =   "Load PNG"
      End
      Begin VB.Menu mnuFileRealtime 
         Caption         =   "&Realtime REF File"
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open REF/ERF File"
      End
      Begin VB.Menu mnuFileOpenMP 
         Caption         =   "Open &MP File"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close Input File"
      End
      Begin VB.Menu mnuFileSaveBMP 
         Caption         =   "&Save BMP"
      End
      Begin VB.Menu mnuFileLoadBMP 
         Caption         =   "&Load BMP"
      End
      Begin VB.Menu mnufileUnload 
         Caption         =   "&Unload BMP"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsAltitude 
         Caption         =   "&Show A/C Altitude"
      End
      Begin VB.Menu mnuOptionsTropopause 
         Caption         =   "Show &Tropopause"
      End
      Begin VB.Menu mnuOptionsColdPoint 
         Caption         =   "Show &Cold Point"
      End
      Begin VB.Menu mnuOptionsAveraging 
         Caption         =   "&Pixel Averaging"
      End
      Begin VB.Menu mnuOptionsAutoATP 
         Caption         =   "&Auto ATP"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewATP 
         Caption         =   "&ATP"
      End
      Begin VB.Menu mnuViewHeader 
         Caption         =   "&Header Info"
      End
      Begin VB.Menu mnuViewLimits 
         Caption         =   "&Limits Summary"
      End
      Begin VB.Menu mnuViewClear 
         Caption         =   "&Clear Display"
      End
   End
End
Attribute VB_Name = "frmCurtain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public Filename$, FileRecords%, yy_mm_dd$, ymdd$, ZYX$, yymmdd$
Private LastY1!, LastY2!, LastZ1!, LastZ2!

Sub Initialize()
Dim C%, M%, X$, i%, lu%

  If ReadREF Then
    M = Len(Filename$)
    Do
      M = M - 1
      X$ = UCase$(Mid$(Filename$, M, 3))
   '  Debug.Print m; x$
    Loop Until X$ = "\NG" Or X$ = "\WB" Or X$ = "\ER" Or X$ = "\DC" Or X$ = "\RT" Or X$ = "\M5" Or X$ = "\GH"
    If Len(Filename$) - M + 1 > 12 Then
      YYYYMMDD$ = Mid$(Filename$, M + 3, 8)
      yymmdd$ = Right$(YYYYMMDD$, 6)
    Else
      yymmdd$ = Mid$(Filename$, M + 3, 6)
      YYYYMMDD$ = "19" + yymmdd$
    End If
    yymmdd$ = yymmdd$
    yy_mm_dd$ = Left$(yymmdd$, 2) & "/" & Mid$(yymmdd$, 3, 2) & "/" & Right$(yymmdd$, 2)
    ymdd$ = Mid$(yymmdd$, 2, 1) + Hex(Mid$(yymmdd$, 3, 2)) + Mid$(yymmdd$, 5, 2)

  Else 'Read MP file header
    Call MPread(CTClu, 0)  'Read header lines
    With frmCTC
    .FileRecords = 10000
    .LATstart = 65
    .LATend = 20
    End With
  End If

  If RealTime Or Not ReadREF Then
    UTmin = 0:    UTmax = 150000
    LATmin = -90:    LATmax = 90
    LONmin = -180:    LONmax = 180
    Zmax = 24
    Tmin = 180: Tmax = 300
    With frmCTC  'Set default values for limits in REALTIME mode
      .UTstart = 0
'      .LATstart = 0
      .LONstart = 0
      .UTend = 180000
'      .LATend = 50
      .LONend = 50
    End With
  
  Else
    REFreadHEADER (Filename$)
    If UTmax < UTmin Then UTmax = UTmax + 86400
    With frmCTC
      .Record = 0
      .GetREFcycle (False)
      .UTstart = UTsec
      .LATstart = Latitude
      .LONstart = Longitude
      .Record = TotalCycles - 1  'NB Dec since reading will Inc
      .FileRecords = TotalCycles
      .GetREFcycle (False)      'Get time of last record
      .UTend = UTsec
      .LATend = Latitude
      .LONend = Longitude
    End With
  
  With frmCTC
  ' Change default abscissa range from 0 to 24 hours
  ' Note that INTing converts x.y to x and -x.y to -(x+1), just what we want
    .UTH1 = Int(UTmin / 3600)
    .UTH2 = Int(UTmax / 3600 + 1)
    .UTS1 = Int(UTmin / 1000)
    .UTS2 = Int(UTmax / 1000 + 1)
    .LAT1 = Int(LATmin)
    .LAT2 = Int(LATmax + 1)
    .LON1 = Int(LONmin)
    .LON2 = Int(LONmax + 1)
    .Zt = Int(Zmax + 0.99)
    .T1 = Int(Tmin / 10) * 10
    .T2 = Int(Tmax / 10 + 1) * 10
    .PT1 = 300
    .PT2 = 420
  End With
  End If

  frmCTC.SetDefaultScales
  frmCTC.CurtainFileOpen = True
  frmCurtain.SetFocus
  Show

  If frmCTC.CurtainFileOpen Then frmCTC.SetDefaultScales
  C% = frmCTC.Xcoord
  optAbscissa(frmCTC.Xcoord).Value = True
  optCurtain(frmCTC.Zcoord).Value = True
  optColorStyle(frmCTC.ColorStyle).Value = True
  If frmCTC.ColorStyle = 0 Then frmCTC.ColorCycles = 1
  optColorCycles(frmCTC.ColorCycles - 1).Value = True

  Select Case frmCTC.Xcoord
  Case 0
    HScroll(2).Min = 0
    HScroll(2).Max = 480
    HScroll(3).Min = 0
    HScroll(3).Max = 480
  Case 1
    HScroll(2).Min = -900
    HScroll(2).Max = 900
    HScroll(3).Min = -900
    HScroll(3).Max = 900
  Case 2
    HScroll(2).Min = -1800
    HScroll(2).Max = 1800
    HScroll(3).Min = -1800
    HScroll(3).Max = 1800
  Case 3
    HScroll(2).Min = 0
    HScroll(2).Max = 1600
    HScroll(3).Min = 0
    HScroll(3).Max = 1600
  End Select

  Select Case frmCTC.Zcoord
  Case 0
    HScroll(4).Min = 700
    HScroll(5).Max = 3700
    HScroll(4).Min = 700
    HScroll(5).Max = 3700
  Case 1
    HScroll(4).Min = 2000
    HScroll(4).Max = 6000
    HScroll(5).Min = 2000
    HScroll(5).Max = 6000
  End Select

  HScroll(2).Value = frmCTC.XVl * 10
  HScroll(3).Value = frmCTC.XVr * 10

  
    lu = FreeFile
    Open "C:\MTP\Setup\CTC.INI" For Input As lu
    Input #lu, LastY1
    Input #lu, LastY2
    Input #lu, LastZ1
    Input #lu, LastZ2
    Close lu
    HScroll(0).Value = LastY1 * 10
    HScroll(1).Value = LastY2 * 10
    
    If frmCTC.ZVl < 170 Or frmCTC.ZVl > 320 Then frmCTC.ZVl = 170
    HScroll(4).Value = LastZ1 * 10
    If frmCTC.ZVr < 170 Or frmCTC.ZVr > 320 Then frmCTC.ZVr = 320
    HScroll(5).Value = LastZ2 * 10

  For i% = 0 To 5: txtAxis(i%).Text = Str$(HScroll(i%).Value / 10): Next

  If frmCTC.IncreasingLatitude Then chkIncreasingLatitude.Value = 1 Else chkIncreasingLatitude.Value = False
  If frmCTC.IncreasingLongitude Then chkIncreasingLongitude.Value = 1 Else chkIncreasingLongitude.Value = False
  If frmCTC.ShowAircraft Then chkAircraft.Value = 1 Else chkAircraft.Value = 0
  If frmCTC.ShowTropopause Then chkTropopause.Value = 1 Else chkTropopause.Value = 0
  If frmCTC.ShowLatitude Then chkLatitude.Value = 1 Else chkLatitude.Value = 0
  If frmCTC.ShowDocument Then chkDocument.Value = 1 Else chkDocument.Value = 0
  If frmCTC.ShowMRI Then chkMRI.Value = 1 Else chkMRI.Value = 0
  If frmCTC.PlotGoodScan Then chkPlotGoodScan.Value = 1 Else chkPlotGoodScan.Value = 0
  If frmCTC.PlotGoodTrop Then chkPlotGoodTrop.Value = 1 Else chkPlotGoodTrop.Value = 0

Exit Sub

  
End Sub

Sub OpenFile()
Dim C%

  If RealTime Then  'look in RAW directory
' Set Initial directory
    CommonDialog1.InitDir = "C:\MTP\RTA\REF\"  'Drive$ + Rdir2$ + "RAW"
' Set Size of FileName buffer
    CommonDialog1.MaxFileSize = 1024
' Set filters.
    CommonDialog1.Filter = "REF Files (*.REF)|*.REF|All Files (*.*)|*.*"
' Specify default filter.
    CommonDialog1.FilterIndex = 1
  Else
' Set Initial directory
    CommonDialog1.InitDir = MNpath$ + "MP\"
' Set Size of FileName buffer
    CommonDialog1.MaxFileSize = 1024
' Set filters.
    If ReadREF Then
      CommonDialog1.Filter = "REF/ERF Files |*.REF;*.ERF|All Files (*.*)|*.*"
    Else
      CommonDialog1.Filter = "All Files (*.*)|*.*"
    End If
' Specify default filter.
    CommonDialog1.FilterIndex = 1
  End If
' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  Filename$ = CommonDialog1.Filename

  On Error GoTo errhandler
  CTClu% = FreeFile
  If ReadREF Then
    FileFormatIn = fREFreadFileFormat(Filename$)
    Select Case FileFormatIn
    Case 32: Open Filename$ For Random Access Read As CTClu% Len = Len(REF)
    Case 33: Open Filename$ For Random Access Read As CTClu% Len = Len(REF2)
    End Select
  Else
    Open Filename$ For Input As CTClu%
  End If
  Initialize
  txtFile.Text = Filename$

Exit Sub

errhandler:
  MsgBox "Error encountered while trying to open file, please retry.", 48, "Text Editor"
  Close CTClu%
  Exit Sub

End Sub

Sub OpenREFfile(Filename$)

  CTClu% = FreeFile
  FileFormatIn = fREFreadFileFormat(Filename$)
  Select Case FileFormatIn
  Case 32: Open Filename$ For Random Access Read As CTClu% Len = Len(REF)
  Case 33: Open Filename$ For Random Access Read As CTClu% Len = Len(REF2)
  End Select
  
  If Not DoAll Then REFreadHEADER Filename$
  Select Case FileFormatIn
  Case 32:  frmCTC.FileRecords = LOF(CTClu%) \ Len(REF) - HiddenRecords
  Case 33: frmCTC.FileRecords = LOF(CTClu%) \ Len(REF2) - HiddenRecords
  End Select

End Sub

Private Sub cboPTpen_Click()
frmCTC.ThetaPen = Val(Trim(cboPTpen.Text))
End Sub


Private Sub chkAircraft_Click()
If chkAircraft.Value = 1 Then
  frmCTC.ShowAircraft = True
Else
  frmCTC.ShowAircraft = False
End If
End Sub

Private Sub chkColdPoint_Click()
If chkColdPoint.Value = 1 Then
  frmCTC.ShowColdPoint = True
Else
  frmCTC.ShowColdPoint = False
End If
End Sub

Private Sub chkDocument_Click()
If chkDocument.Value = 1 Then
  frmCTC.ShowDocument = True
Else
  frmCTC.ShowDocument = False
End If
End Sub


Private Sub chkEnableAveraging_Click()
If chkEnableAveraging.Value = 1 Then
  frmCTC.EnableAveraging = True
Else
  frmCTC.EnableAveraging = False
End If
End Sub


Private Sub chkIncreasingLatitude_Click()
  
  If chkIncreasingLatitude.Value = 1 Then
    frmCTC.IncreasingLatitude = True
  Else
    frmCTC.IncreasingLatitude = False
  End If
  
  optAbscissa_Click (frmCTC.Xcoord)      'Update latitude display if necessary

End Sub


Private Sub chkIncreasingLongitude_Click()
  If chkIncreasingLongitude.Value = 1 Then
    frmCTC.IncreasingLongitude = True
  Else
    frmCTC.IncreasingLongitude = False
  End If

End Sub


Private Sub chkLatitude_Click()

If chkLatitude.Value = 1 Then
  frmCTC.ShowLatitude = True
Else
  frmCTC.ShowLatitude = False
End If

End Sub


Private Sub chkMRI_Click()
If chkMRI.Value = 1 Then
  frmCTC.ShowMRI = True
Else
  frmCTC.ShowMRI = False
End If

End Sub


Private Sub chkPlotGoodScan_Click()
If chkPlotGoodScan.Value = 1 Then
  frmCTC.PlotGoodScan = True
Else
  frmCTC.PlotGoodScan = False
End If
End Sub


Private Sub chkPlotGoodTrop_Click()
If chkPlotGoodTrop.Value = 1 Then
  frmCTC.PlotGoodTrop = True
Else
  frmCTC.PlotGoodTrop = False
End If
End Sub


Private Sub chkPlotPalt_Click()
If chkPlotPalt.Value = 1 Then
  frmCTC.PlotPalt = True
Else
  frmCTC.PlotPalt = False
End If
End Sub


Private Sub chkPreliminary_Click()
If chkPreliminary.Value = 1 Then
  frmCTC.Preliminary = True
Else
  frmCTC.Preliminary = False
End If
End Sub



Private Sub chkSubstituteLastATP_Click()
If chkSubstituteLastATP.Value = 1 Then
  frmCTC.SubstituteLastATP = True
Else
  frmCTC.SubstituteLastATP = False
End If
End Sub


Private Sub chkTropopause_Click()
If chkTropopause.Value = 1 Then
  frmCTC.ShowTropopause = True
Else
  frmCTC.ShowTropopause = False
End If
End Sub



Private Sub cmdDoAll_Click()
DoAll = True
REFwriteFLTINFO "C:\MTP\Setup\FLTINFO.REF"

Form_Load
End Sub


Private Sub cmdExit_Click()
Dim lu%

  lu = FreeFile
  Open "C:\MTP\Setup\CTC.INI" For Output As lu
  Print #lu, LastY1
  Print #lu, LastY2
  Print #lu, LastZ1
  Print #lu, LastZ2
  Close lu

  mnuFileExit_Click
End Sub

Private Sub cmdPlotCTC_Click()
' First save current format settings on CTC form
Dim X$, y$, z$, lu%

 
With frmCTC
  .YVl = Val(txtAxis(0).Text)
  .YVr = Val(txtAxis(1).Text)
  .YVs = frmCTC.YVr - frmCTC.YVl
  .YVc = frmCTC.YVl + frmCTC.YVs / 2
   LastY1 = .YVl  'Save for use on next plot
   LastY2 = .YVr
  
  .XVl = Val(txtAxis(2).Text)
  .XVr = Val(txtAxis(3).Text)
  .XVs = frmCTC.XVr - frmCTC.XVl
  .XVc = frmCTC.XVl + frmCTC.XVs / 2

  .ZVl = Val(txtAxis(4).Text)
  .ZVr = Val(txtAxis(5).Text)
  .ZVs = frmCTC.ZVr - frmCTC.ZVl
  .ZVc = frmCTC.ZVl + frmCTC.ZVs / 2
   LastZ1 = .ZVl  'Save for use on next plot
   LastZ2 = .ZVr
  
  .ThetaStart = Val(cboPTstart.Text)
  .ThetaEnd = Val(cboPTend.Text)
  .ThetaStep = Val(cboPTstep.Text)
  .ThetaPen = Val(cboPTpen.Text)

End With

If txtMission.Text <> "" Then Mission$ = txtMission.Text
'
' Now do plot
'
If frmCTC.CurtainFileOpen Then
  Record% = 0              'Reset record back to first
  frmCTC.GetREFcycle (False)    'Go back to first record
  
' Generate BMP filename prefix based on plot coordinates
  Select Case frmCTC.Zcoord
  Case 0:  z$ = "T"     'Temperature
  Case 1:  z$ = "P"     'Potential Temperature
  End Select
  y$ = "Z"              'Pressure Altitude
  Select Case frmCTC.Xcoord
  Case 0:  X$ = "H"     'UT hours
  Case 1:  X$ = "A"     'Latitude
  Case 2:  X$ = "O"     'Longitude
  Case 3:  X$ = "S"     'UT seconds
  End Select
  ZYX$ = z$ + y$ + X$
  
  With frmCTC
    .XVlast = frmCTC.XVvalue
    .Curtain_Plot_Set
    .Curtain_Plot_Axes_Labels
    .Curtain_Plot_Tic_Marks
    .Curtain_Plot_Color_Bar
    .ReLabel = True
    .SamplesAveraged = 0
    .zt1last = 99.9
    .zt2last = 99.9
    .Zt1AVG = 0#:  .Zt2Avg = 0#:  .AltAvg = 0#:  .MRIavg1 = 0#: .ZcpAVG = 0#
    .Zt1Cnt = 0:  .Zt2Cnt = 0:   .AltCnt = 0:  .MRIcnt = 0: .ZcpCnt = 0:
    If .XVl < .XVr Then
      .XVend = .XVvalue + .XunitsPerPixel
    Else
      .XVend = .XVvalue - .XunitsPerPixel
    End If
    .tmrCurtainAutoStep.Interval = .CurtainAutoStepInterval
    .tmrCurtainAutoStep.Enabled = True
    .CurtainPlotUpdate = False   'True if Curtain plot just done
  End With
  Record% = 0
Else
  MsgBox "You have not opened a data file, please do so (F4).", 48, "File|Start Command"
End If


End Sub







Private Sub cmdPlotMP_Click()
Dim Filename$

  Filename$ = MNpath$ + "MP\MP" + YYYYMMDD$ + fACext(AC$)
  RealTime = False
  ReadREF = False
  Close CTClu
  CTClu = FreeFile
  Open Filename$ For Input As CTClu%
  Initialize
  txtFile.Text = Filename$

Call cmdPlotCTC_Click

End Sub

Private Sub Form_Click()
Dim X!


End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub


Private Sub Form_Load()
Dim C%, i%, lu%

  frmCTC.Curtaininitialize = True
  Picture = LoadPicture("c:\mtp\vb6\DIB\rainbow.dib")  ' Load bitmap.
  Show

' frmCurtain.ScaleMode = 1
  With frmCurtain
    .Top = 0
    .Left = 12000
    .Width = 3360
    .Height = 5835
  End With

  With cboPTpen
    .AddItem "1"
    .AddItem "2"
    .AddItem "3"
    .ListIndex = 1
  End With

  With cboPTstart
    .AddItem "280"
    .AddItem "285"
    .AddItem "290"
    .AddItem "295"
    .AddItem "300"
    .AddItem "305"
    For i = 310 To 380 Step 10
      .AddItem Str(i)
    Next i
    .ListIndex = 4
  End With
  
  With cboPTend
    For i = 350 To 600 Step 10
      .AddItem Str(i)
    Next i
    .ListIndex = 10
  End With
  
  With cboPTstep
    .AddItem "5"
    .AddItem "10"
    .AddItem "20"
    .AddItem "40"
    .ListIndex = 3
  End With

  REFreadHEADER "C:\MTP\Setup\FLTINFO.REF"

  frmCTC.Curtain_Initialize
  chkPlotPalt.Value = 1
  chkColdPoint.Value = 1
  chkPlotPalt_Click
  optPlot(0).Value = True
  optAbscissa(frmCTC.Xcoord).Value = True
  DoAll = False

  If DoAll Then
    Path$ = Drive$ + Rdir2$ + Mission$ + "\" + YYYYMMDD$ + "\"
    Filename$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
    If Dir(Filename$) = "" Then
      If Not DoAll Then Call MsgBox("Could not find ERF file: " + Filename$ + vbCrLf + "Will look for REF file!", vbOKCancel)
      Filename$ = Path$ + AC$ + YYYYMMDD$ + ".REF"
      If Dir(Filename$) = "" Then
        Call MsgBox("Could not find file: " + Filename$, vbOK)
        Exit Sub
      End If
    End If
    
    OpenREFfile Filename$
    
    REFreadHEADER (Filename$)
    If UTmax < UTmin Then UTmax = UTmax + 86400
    With frmCTC
      .Record = 0
      .GetREFcycle (False)
      .UTstart = UTsec
      .LATstart = Latitude
      .LONstart = Longitude
      .Record = frmCTC.FileRecords - 1  'NB Dec since reading will Inc
      .GetREFcycle (False)      'Get time of last record
      .UTend = UTsec
      .LATend = Latitude
      .LONend = Longitude
    End With
  
    With frmCTC
  ' Change default abscissa range from 0 to 24 hours
  ' Note that INTing converts x.y to x and -x.y to -(x+1), just what we want
      .UTH1 = Int(UTmin / 3600)
      .UTH2 = Int(UTmax / 3600 + 1)
      .UTS1 = Int(UTmin / 1000)
      .UTS2 = Int(UTmax / 1000 + 1)
      .LAT1 = Int(LATmin)
      .LAT2 = Int(LATmax + 1)
      .LON1 = Int(LONmin)
      .LON2 = Int(LONmax + 1)
      .Zt = 24
      .T1 = 180
      .T2 = 300
      .PT1 = 300
      .PT2 = 420
    End With
    frmCTC.SetDefaultScales
    frmCTC.CurtainFileOpen = True
    frmCurtain.SetFocus
    Show

    If frmCTC.CurtainFileOpen Then frmCTC.SetDefaultScales
    C% = frmCTC.Xcoord
    optAbscissa(frmCTC.Xcoord).Value = True
    optCurtain(frmCTC.Zcoord).Value = True
    optColorStyle(frmCTC.ColorStyle).Value = True
    If frmCTC.ColorStyle = 0 Then frmCTC.ColorCycles = 1
    optColorCycles(frmCTC.ColorCycles - 1).Value = True

    Select Case frmCTC.Xcoord
    Case 0
      HScroll(2).Min = 0
      HScroll(2).Max = 480
      HScroll(3).Min = 0
      HScroll(3).Max = 480
    Case 1
      HScroll(2).Min = -900
      HScroll(2).Max = 900
      HScroll(3).Min = -900
      HScroll(3).Max = 900
    Case 2
      HScroll(2).Min = -1800
      HScroll(2).Max = 1800
      HScroll(3).Min = -1800
      HScroll(3).Max = 1800
    Case 3
      HScroll(2).Min = 0
      HScroll(2).Max = 1600
      HScroll(3).Min = 0
      HScroll(3).Max = 1600
    End Select
  
    Select Case frmCTC.Zcoord
    Case 0
      HScroll(4).Min = 700
      HScroll(5).Max = 3700
      HScroll(4).Min = 700
      HScroll(5).Max = 3700
    
      HScroll(4).Min = 2000
      HScroll(4).Max = 6000
      HScroll(5).Min = 2000
      HScroll(5).Max = 6000
    End Select
    
    lu = FreeFile
    Open "C:\MTP\Setup\CTC.INI" For Input As lu
    Input #lu, LastY1
    Input #lu, LastY2
    Input #lu, LastZ1
    Input #lu, LastZ2
    Close lu
    
    HScroll(0).Value = LastY1 * 10
    HScroll(1).Value = LastY2 * 10
    
    HScroll(2).Value = frmCTC.XVl * 10
    HScroll(3).Value = frmCTC.XVr * 10

    If frmCTC.ZVl < 170 Or frmCTC.ZVl > 320 Then frmCTC.ZVl = 170
    HScroll(4).Value = LastZ1 * 10
    If frmCTC.ZVr < 170 Or frmCTC.ZVr > 320 Then frmCTC.ZVr = 320
    HScroll(5).Value = LastZ2 * 10

    For i% = 0 To 5: txtAxis(i%).Text = Str$(HScroll(i%).Value / 10): Next
  

    If frmCTC.IncreasingLatitude Then chkIncreasingLatitude.Value = 1 Else chkIncreasingLatitude.Value = False
    If frmCTC.IncreasingLongitude Then chkIncreasingLongitude.Value = 1 Else chkIncreasingLongitude.Value = False
    If frmCTC.ShowAircraft Then chkAircraft.Value = 1 Else chkAircraft.Value = 0
    If frmCTC.ShowTropopause Then chkTropopause.Value = 1 Else chkTropopause.Value = 0
    If frmCTC.ShowColdPoint Then chkColdPoint.Value = 1 Else chkColdPoint = 0
    If frmCTC.ShowLatitude Then chkLatitude.Value = 1 Else chkLatitude.Value = 0
    If frmCTC.ShowDocument Then chkDocument.Value = 1 Else chkDocument.Value = 0
    If frmCTC.ShowMRI Then chkMRI.Value = 1 Else chkMRI.Value = 0
    If frmCTC.PlotGoodScan Then chkPlotGoodScan.Value = 1 Else chkPlotGoodScan.Value = 0
    If frmCTC.PlotGoodTrop Then chkPlotGoodTrop.Value = 1 Else chkPlotGoodTrop.Value = 0

    frmCTC.Preliminary = False
    frmCTC.PlotPalt = True
  
    cmdPlotCTC_Click

  End If

End Sub


Private Sub Form_Resize()
If Not frmCTC.CurtainFileOpen Then Exit Sub
If Not frmCTC.Curtaininitialize Then
  'Plot_Update
Else
  frmCTC.Curtaininitialize = False
End If

End Sub



Private Sub HScroll_Change(Index As Integer)
  txtAxis(Index).Text = Str$(HScroll(Index).Value / 10)

End Sub


Public Sub lblMTP_Click()
' By default use ERF file
  Filename$ = Path$ + AC$ + YYYYMMDD$ + ".ERF"
' If ERF file doesn't exist check for REF file
  If Dir(Filename$) = "" Then
    If Not DoAll Then Call MsgBox("Could not find ERF file: " + Filename$ + vbCrLf + "Will look for REF file!", vbOKCancel)
    Filename$ = Path$ + AC$ + YYYYMMDD$ + ".REF"
    If Dir(Filename$) = "" Then
      Call MsgBox("Could not find file: " + Filename$, vbOK)
      Exit Sub
    End If
  End If
  
  CTClu% = FreeFile
  FileFormatIn = fREFreadFileFormat(Filename$)
  Select Case FileFormatIn
  Case 32
    Open Filename$ For Random Access Read As CTClu% Len = Len(REF)
    frmCTC.FileRecords = LOF(CTClu%) \ Len(REF) - HiddenRecords
  Case 33
    Open Filename$ For Random Access Read As CTClu% Len = Len(REF2)
    frmCTC.FileRecords = LOF(CTClu%) \ Len(REF2) - HiddenRecords
  End Select
  
  ReadREF = True
  Initialize
  txtFile.Text = Filename$
  
End Sub

Private Sub mnuFileClose_Click()
Close
Unload Me

End Sub

Private Sub mnuFileLoadPNG_Click()
Dim Status As Boolean
' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
' Set filters.
CommonDialog1.Filter = "PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

Filename$ = CommonDialog1.Filename

frmCTC.Show
'Status = LoadFormAsPNG(Filename$, frmCTC)
'frmCTC.Picture = LoadPicture(filename$)
frmCTC.tmrCurtainAutoStep.Enabled = False
End Sub

Private Sub mnuFileOpen_Click()
Dim C%

  RealTime = False
  ReadREF = True
  OpenFile
  
End Sub


Private Sub mnuFileOpenMP_Click()
  RealTime = False
  ReadREF = False
  OpenFile

End Sub


Private Sub mnuFileRealtime_Click()
  RealTime = True
  ReadREF = True
  OpenFile
End Sub

Private Sub mnuFileSavePNG_Click()
Dim X$, y$, z$, Status As Boolean, Filename2$, Filename$

' Set Initial directory
  Call DirCheck(MNpath$ + "PNG\", "TZS")
  CommonDialog1.InitDir = MNpath$
  Filename$ = CommonDialog1.InitDir + "PNG\TZS\" + ZYX$ + "_" + YYYYMMDD$ + ".PNG"
  Call DirCheck(MNpath$, "PNG")
  If Not DoAll Then
'   Set Default filename
    CommonDialog1.Filename = Filename$
'   Set filters.
    CommonDialog1.Filter = "Bit Map Files (*.PNG)|*.PNG|All Files (*.*)|*.*"
'   Specify default filter.
    CommonDialog1.FilterIndex = 1
'   Display the File Open dialog box.
    CommonDialog1.ShowSave

    Filename$ = CommonDialog1.Filename

    frmCTC.Show   'Show image or the size will not be correct
    frmCTC.ScaleMode = 1
    CaptureFormMJ frmCTC, Filename$, 1, False
'    If Not Status Then MsgBox "PNG file of CTC was not saved!", vbOKOnly
'    If Mission$ = "TCSP" Then
'      Filename2$ = CommonDialog1.InitDir + "PNG\Quicklook\" + ZYX$ + "_" + yyyymmdd$ + ".PNG"
'      Status = SaveFormAsPNG(frmCTC, Filename2$)
'    End If
    Unload frmCTC
  
  Else
    frmCTC.Show   'Show image or the size will not be correct
    frmCTC.ScaleMode = 1
    CaptureFormMJ frmCTC, Filename$, 1, False
    'Status = SaveFormAsPNG(frmCTC, Filename$)
    If Not Status Then MsgBox "PNG file of CTC was not saved!", vbOKOnly
'  Unload frmCTC
    mnuFileExit_Click
End If
cmdExit_Click

End Sub

Private Sub mnufileUnload_Click()
Unload frmCTC
End Sub

Private Sub mnuFileExit_Click()

  Close
  Unload frmCTC
  Unload Me
  
End Sub

Private Sub mnuFileLoadBMP_Click()
' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
' Set filters.
CommonDialog1.Filter = "Bit Map Files (*.BMP)|*.BMP|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

Filename$ = CommonDialog1.Filename

frmCTC.Show
frmCTC.Picture = LoadPicture(Filename$)
frmCTC.tmrCurtainAutoStep.Enabled = False
End Sub

Public Sub mnuFileSaveBMP_Click()
Dim X$, y$, z$, Status As Boolean

' Set Initial directory
CommonDialog1.InitDir = Drive$ + Rdir2$ + Mission$
Filename$ = CommonDialog1.InitDir + "\BMP\" + ZYX$ + "_" + YYYYMMDD$ + ".BMP"

If Not DoAll Then
' Set Default filename
CommonDialog1.Filename = Filename$
' Set filters.
CommonDialog1.Filter = "Bit Map Files (*.BMP)|*.BMP|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowSave

Filename$ = CommonDialog1.Filename

'If filename$ = frmMTPdata.txtFile.Text Then   ' Avoid opening a file if it is already loaded.
'  Exit Sub'
'Else
  SavePicture frmCTC.Image, Filename$
'  filename$ = Left$(filename$, Len(filename$) - 3) + "PNG"
'  Status = SaveFormAsPNG(frmCTC, filename$)
'End If
Unload frmCTC
Else
  SavePicture frmCTC.Image, Filename$
  If Not Status Then MsgBox "PNG file of CTC was not saved!", vbOKOnly
'  Unload frmCTC
  mnuFileExit_Click
End If
End Sub

Private Sub mnuOptionsAltitude_Click()
mnuOptionsAltitude.Checked = Not mnuOptionsAltitude.Checked
If mnuOptionsAltitude.Checked Then
  frmCTC.ShowAircraft = True
Else
  frmCTC.ShowAircraft = False
End If
End Sub

Private Sub mnuOptionsAutoATP_Click()

mnuOptionsAutoATP.Checked = Not mnuOptionsAutoATP.Checked
If mnuOptionsAutoATP.Checked Then
  frmCTC.AutoATPupdate = True
  'mnuViewATP_Click
Else
  frmCTC.AutoATPupdate = False
End If

End Sub

Private Sub mnuOptionsAveraging_Click()
mnuOptionsAveraging.Checked = Not mnuOptionsAveraging.Checked
If mnuOptionsAveraging.Checked Then
  frmCTC.EnableAveraging = True
Else
  frmCTC.EnableAveraging = False
End If

End Sub


Private Sub mnuOptionsShowColdPoint_Click()
mnuOptionsColdPoint.Checked = Not mnuOptionsColdPoint.Checked
If mnuOptionsColdPoint.Checked Then
  frmCTC.ShowColdPoint = True
Else
  frmCTC.ShowColdPoint = False
End If
End Sub

Private Sub mnuOptionsTropopause_Click()

mnuOptionsTropopause.Checked = Not mnuOptionsTropopause.Checked
If mnuOptionsTropopause.Checked Then
  frmCTC.ShowTropopause = True
Else
  frmCTC.ShowTropopause = False
End If

End Sub




Private Sub mnuViewClear_Click()
frmCurtain.Cls
End Sub

Private Sub mnuViewHeader_Click()
mnuViewHeader.Checked = Not mnuViewHeader.Checked
If mnuViewHeader.Checked Then
'  frmMTPdata.Visible = True
Else
'  frmMTPdata.Visible = False
End If

End Sub

Private Sub mnuViewLimits_Click()

frmCTC.LoadLimitsSummary (Filename$)

End Sub

Private Sub mnuViewATP_Click()
mnuViewATP.Checked = Not mnuViewATP.Checked
If mnuViewATP.Checked Then
'  mnuOptionsAveraging.Checked = False
'  EnableAveraging = False   'Turn off pixel averaging
  RequestATPupdate = True
Else
  RequestATPupdate = False
  mnuOptionsAutoATP.Checked = False
  frmCTC.AutoATPupdate = False
  Unload frmATP
End If

End Sub

Private Sub optAbscissa_Click(Index As Integer)
frmCTC.Xcoord = Index
Select Case Index
Case 0
  HScroll(2).Min = 0
  HScroll(2).Max = 240
  HScroll(2).Value = frmCTC.UTH1 * 10
  HScroll(3).Min = 0
  HScroll(3).Max = 480
  HScroll(3).Value = frmCTC.UTH2 * 10
  frmCTC.XVl = frmCTC.UTH1: frmCTC.XVr = frmCTC.UTH2
Case 1
' If IncreasingLatitude And LAT1 > LAT2 Then temp = LAT1: LAT1 = LAT2: LAT2 = temp
  HScroll(2).Min = -900
  HScroll(2).Max = 900
  HScroll(3).Min = -900
  HScroll(3).Max = 900
  If frmCTC.IncreasingLatitude Then
    If frmCTC.LAT1 < frmCTC.LAT2 Then
      HScroll(2).Value = frmCTC.LAT1 * 10
      HScroll(3).Value = frmCTC.LAT2 * 10
    Else
      HScroll(2).Value = frmCTC.LAT2 * 10
      HScroll(3).Value = frmCTC.LAT1 * 10
    End If
  Else                         'Leave alone
    HScroll(2).Value = frmCTC.LAT1 * 10
    HScroll(3).Value = frmCTC.LAT2 * 10
  End If
  frmCTC.XVl = HScroll(2).Value * 10
  frmCTC.XVr = HScroll(3).Value * 10
Case 2
  'If IncreasingLongitude And LON1 > LON2 Then temp = LON1: LON1 = LON2: LON2 = temp
  HScroll(2).Min = -1800
  HScroll(2).Max = 1800
  HScroll(2).Value = frmCTC.LON1 * 10
  HScroll(3).Min = -1800
  HScroll(3).Max = 1800
  HScroll(3).Value = frmCTC.LON2 * 10
  frmCTC.XVl = frmCTC.LON1
  frmCTC.XVr = frmCTC.LON2
Case 3
  HScroll(2).Min = 0
  HScroll(2).Max = 1600
  HScroll(2).Value = frmCTC.UTS1 * 10
  HScroll(3).Min = 0
  HScroll(3).Max = 1600
  HScroll(3).Value = frmCTC.UTS2 * 10
  frmCTC.XVl = frmCTC.UTS1
  frmCTC.XVr = frmCTC.UTS2
End Select
End Sub

Private Sub optColorCycles_Click(Index As Integer)
  frmCTC.ColorCycles = Index + 1
End Sub

Private Sub optColorStyle_Click(Index As Integer)
  frmCTC.ColorStyle = Index
End Sub


Private Sub optCurtain_Click(Index As Integer)
frmCTC.Zcoord = Index
Select Case Index
Case 0                      'Temperature
  HScroll(4).Min = 1700
  HScroll(4).Max = 3200
  If frmCTC.T1 < 170 Then frmCTC.T1 = 170
  HScroll(4).Value = frmCTC.T1 * 10
  HScroll(5).Min = 1700
  HScroll(5).Max = 3200
  If frmCTC.T2 > 320 Then frmCTC.T2 = 320
  HScroll(5).Value = frmCTC.T2 * 10
  frmCTC.ZVl = frmCTC.T1: frmCTC.ZVr = frmCTC.T2
Case 1                      'Potential Temperature
  HScroll(4).Min = 2000
  HScroll(4).Max = 6000
  If frmCTC.PT1 < 200 Then frmCTC.PT1 = 200
  HScroll(4).Value = frmCTC.PT1 * 10
  HScroll(5).Min = 2000
  HScroll(5).Max = 6000
  If frmCTC.PT2 > 600 Then frmCTC.PT2 = 200
  HScroll(5).Value = frmCTC.PT2 * 10
  frmCTC.ZVl = frmCTC.PT1: frmCTC.ZVr = frmCTC.PT2
End Select

End Sub


Private Sub optPlot_Click(Index As Integer)
Select Case Index
Case 0
  frmCTC.PlotCTC = True
  frmCTC.PlotIAC = False
Case 1
  frmCTC.PlotCTC = False
  frmCTC.PlotIAC = True
Case 2
  frmCTC.PlotCTC = True
  frmCTC.PlotIAC = True
End Select
End Sub

Private Sub SSTab1_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub txtAxis_Change(Index As Integer)
  HScroll(Index).Value = Val(txtAxis(Index).Text) * 10

End Sub


