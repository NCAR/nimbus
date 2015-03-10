VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form frmRAOBget 
   Caption         =   "Download RAOBs"
   ClientHeight    =   5370
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   6750
   Icon            =   "frmRAOBget.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5370
   ScaleWidth      =   6750
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdButton 
      Caption         =   "E&xit"
      Height          =   375
      Index           =   2
      Left            =   5640
      TabIndex        =   120
      Top             =   5010
      Width           =   975
   End
   Begin VB.TextBox txtStatus 
      Height          =   285
      Left            =   720
      TabIndex        =   118
      ToolTipText     =   "Status of current download"
      Top             =   5010
      Width           =   2055
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4965
      Left            =   30
      TabIndex        =   0
      Top             =   30
      Width           =   6735
      _ExtentX        =   11880
      _ExtentY        =   8758
      _Version        =   393216
      TabHeight       =   520
      TabCaption(0)   =   "&Download"
      TabPicture(0)   =   "frmRAOBget.frx":0442
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label(12)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label(5)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label(4)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label(3)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Label(2)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Label(1)"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Label(9)"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "Label(19)"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "Label(23)"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "rtfSounding"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "chkEnableInet"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "cmbHour(1)"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "cmbDay(1)"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "cmbMonth(1)"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "cmbYear(1)"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "cmdButton(1)"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "cmbYear(0)"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "cmbMonth(0)"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "cmbDay(0)"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).Control(19)=   "cmbHour(0)"
      Tab(0).Control(19).Enabled=   0   'False
      Tab(0).Control(20)=   "cmdButton(0)"
      Tab(0).Control(20).Enabled=   0   'False
      Tab(0).Control(21)=   "cmbStation"
      Tab(0).Control(21).Enabled=   0   'False
      Tab(0).Control(22)=   "cmdButton(3)"
      Tab(0).Control(22).Enabled=   0   'False
      Tab(0).Control(23)=   "Frame1"
      Tab(0).Control(23).Enabled=   0   'False
      Tab(0).Control(24)=   "cmbHour(2)"
      Tab(0).Control(24).Enabled=   0   'False
      Tab(0).Control(25)=   "Inet1"
      Tab(0).Control(25).Enabled=   0   'False
      Tab(0).Control(26)=   "cboDelay"
      Tab(0).Control(26).Enabled=   0   'False
      Tab(0).Control(27)=   "tmrWait"
      Tab(0).Control(27).Enabled=   0   'False
      Tab(0).Control(28)=   "chkPlotRAOB"
      Tab(0).Control(28).Enabled=   0   'False
      Tab(0).Control(29)=   "chkShowTropopause"
      Tab(0).Control(29).Enabled=   0   'False
      Tab(0).Control(30)=   "chkShowPressureAltitude"
      Tab(0).Control(30).Enabled=   0   'False
      Tab(0).Control(31)=   "chkSingleSoundings"
      Tab(0).Control(31).Enabled=   0   'False
      Tab(0).Control(32)=   "chkDeleteBatchFile"
      Tab(0).Control(32).Enabled=   0   'False
      Tab(0).ControlCount=   33
      TabCaption(1)   =   "&Station List Selection"
      TabPicture(1)   =   "frmRAOBget.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "cboMap"
      Tab(1).Control(1)=   "chkLocation"
      Tab(1).Control(2)=   "chkDetails"
      Tab(1).Control(3)=   "txtListItems"
      Tab(1).Control(4)=   "Frame"
      Tab(1).Control(5)=   "cmdSelect(7)"
      Tab(1).Control(6)=   "Frame10"
      Tab(1).Control(7)=   "lbList(1)"
      Tab(1).Control(8)=   "txtList(1)"
      Tab(1).Control(9)=   "lbList(0)"
      Tab(1).Control(10)=   "cmdSelect(0)"
      Tab(1).Control(11)=   "cmdSelect(2)"
      Tab(1).Control(12)=   "cmdSelect(3)"
      Tab(1).Control(13)=   "cmdSelect(4)"
      Tab(1).Control(14)=   "txtList(0)"
      Tab(1).Control(15)=   "cmdSelect(6)"
      Tab(1).Control(16)=   "cmdSelect(1)"
      Tab(1).Control(17)=   "txtSaveFile"
      Tab(1).Control(18)=   "Label(13)"
      Tab(1).Control(19)=   "Label(16)"
      Tab(1).Control(20)=   "Label(15)"
      Tab(1).ControlCount=   21
      TabCaption(2)   =   "&Convert ASCII"
      TabPicture(2)   =   "frmRAOBget.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "chkGeoPotHeight(0)"
      Tab(2).Control(1)=   "Frame3"
      Tab(2).Control(2)=   "txtT17"
      Tab(2).Control(3)=   "txtT10"
      Tab(2).Control(4)=   "cdConvertIn"
      Tab(2).Control(5)=   "cmdStartConversion"
      Tab(2).Control(6)=   "Frame2"
      Tab(2).Control(7)=   "Frame4"
      Tab(2).Control(8)=   "Drive1"
      Tab(2).Control(9)=   "chkFindTrop"
      Tab(2).Control(10)=   "Label(22)"
      Tab(2).Control(11)=   "Label(10)"
      Tab(2).ControlCount=   12
      Begin VB.CheckBox chkDeleteBatchFile 
         Caption         =   "Delete Batch File"
         Height          =   195
         Left            =   4830
         TabIndex        =   117
         Top             =   3420
         Value           =   1  'Checked
         Width           =   1695
      End
      Begin VB.CheckBox chkSingleSoundings 
         Caption         =   "Get Single Soundings"
         Height          =   195
         Left            =   4830
         TabIndex        =   116
         Top             =   3150
         Width           =   1845
      End
      Begin VB.ComboBox cboMap 
         Height          =   315
         Left            =   -71070
         TabIndex        =   114
         Text            =   "North America"
         Top             =   2700
         Width           =   2685
      End
      Begin VB.CheckBox chkGeoPotHeight 
         Caption         =   "Use Geopotential Height"
         Height          =   255
         Index           =   0
         Left            =   -70800
         TabIndex        =   110
         Top             =   3840
         Width           =   2415
      End
      Begin VB.CheckBox chkShowPressureAltitude 
         Caption         =   "Show pALT"
         Height          =   195
         Left            =   4830
         TabIndex        =   109
         Top             =   2640
         Width           =   1215
      End
      Begin VB.CheckBox chkShowTropopause 
         Caption         =   "Show Trop"
         Height          =   195
         Left            =   4830
         TabIndex        =   108
         Top             =   2400
         Width           =   1095
      End
      Begin VB.CheckBox chkPlotRAOB 
         Caption         =   "Plot RAOB"
         Height          =   195
         Left            =   4830
         TabIndex        =   107
         ToolTipText     =   "Plot downloaded RAOB"
         Top             =   2160
         Width           =   1095
      End
      Begin VB.CheckBox chkLocation 
         Caption         =   "Include Location"
         Height          =   375
         Left            =   -72030
         TabIndex        =   104
         ToolTipText     =   "If selected, Save Site Location and Trops"
         Top             =   1950
         Width           =   915
      End
      Begin VB.Timer tmrWait 
         Left            =   4680
         Top             =   1320
      End
      Begin VB.ComboBox cboDelay 
         Height          =   315
         Left            =   150
         TabIndex        =   103
         ToolTipText     =   "Time between RAOB requests"
         Top             =   3270
         Width           =   1065
      End
      Begin VB.Frame Frame3 
         Caption         =   "Generate Upper Air Binary File"
         Height          =   1095
         Left            =   -74880
         TabIndex        =   98
         Top             =   3240
         Width           =   2415
         Begin VB.CommandButton cmdExtractUA 
            Caption         =   "Start"
            Height          =   375
            Left            =   120
            TabIndex        =   101
            Top             =   240
            Width           =   2175
         End
         Begin VB.TextBox txtRecord 
            Alignment       =   1  'Right Justify
            Height          =   285
            Left            =   1680
            MultiLine       =   -1  'True
            TabIndex        =   100
            Top             =   720
            Width           =   615
         End
         Begin VB.Label lblRecord 
            Caption         =   "Current Record"
            Height          =   255
            Left            =   120
            TabIndex        =   99
            Top             =   720
            Width           =   1575
         End
      End
      Begin VB.TextBox txtT17 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -68880
         MultiLine       =   -1  'True
         TabIndex        =   96
         Text            =   "frmRAOBget.frx":0496
         Top             =   3240
         Width           =   495
      End
      Begin VB.TextBox txtT10 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -69480
         MultiLine       =   -1  'True
         TabIndex        =   95
         Text            =   "frmRAOBget.frx":049B
         Top             =   3240
         Width           =   495
      End
      Begin InetCtlsObjects.Inet Inet1 
         Left            =   5280
         Top             =   1200
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
      End
      Begin VB.CheckBox chkDetails 
         Caption         =   "  Auto Details"
         Height          =   495
         Left            =   -72030
         TabIndex        =   94
         ToolTipText     =   "Automatically Show Details of Selection"
         Top             =   1500
         Width           =   855
      End
      Begin VB.ComboBox cmbHour 
         Height          =   315
         Index           =   2
         Left            =   1800
         TabIndex        =   91
         Text            =   "12"
         ToolTipText     =   "Frequency of RAOBs"
         Top             =   2880
         Width           =   615
      End
      Begin VB.TextBox txtListItems 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -72030
         MultiLine       =   -1  'True
         TabIndex        =   77
         Top             =   2700
         Width           =   375
      End
      Begin VB.Frame Frame 
         Caption         =   "RAOB Site List (RSL)"
         Height          =   1455
         Left            =   -71070
         TabIndex        =   65
         Top             =   390
         Width           =   2730
         Begin VB.CommandButton cmdSelect 
            Caption         =   "&Default"
            Height          =   375
            Index           =   8
            Left            =   1845
            TabIndex        =   121
            ToolTipText     =   "Open Dialog Box to Find RSL File"
            Top             =   960
            Width           =   795
         End
         Begin VB.CommandButton cmdSelect 
            Caption         =   "&Open Other RSL File"
            Height          =   375
            Index           =   5
            Left            =   120
            TabIndex        =   111
            ToolTipText     =   "Open Dialog Box to Find RSL File"
            Top             =   960
            Width           =   1695
         End
         Begin VB.CommandButton cmdList 
            Caption         =   "Open"
            Height          =   255
            Index           =   1
            Left            =   1800
            TabIndex        =   69
            Top             =   600
            Width           =   615
         End
         Begin VB.CommandButton cmdList 
            Caption         =   "Open"
            Height          =   255
            Index           =   0
            Left            =   1800
            TabIndex        =   68
            ToolTipText     =   "Open Selected RSL File"
            Top             =   240
            Width           =   615
         End
         Begin VB.ComboBox cmbMissions 
            Height          =   315
            Left            =   120
            TabIndex        =   67
            Text            =   "Southern California"
            Top             =   240
            Width           =   1695
         End
         Begin VB.ComboBox cmbWyoming 
            Height          =   315
            Left            =   120
            TabIndex        =   66
            Text            =   "North America"
            Top             =   600
            Width           =   1695
         End
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Save As"
         Height          =   375
         Index           =   7
         Left            =   -73140
         TabIndex        =   63
         ToolTipText     =   "Save Site Locations or Site Names"
         Top             =   1920
         Width           =   1095
      End
      Begin VB.Frame Frame10 
         Caption         =   "Special Site Searches"
         Height          =   1815
         Left            =   -74880
         TabIndex        =   61
         Top             =   3060
         Width           =   6495
         Begin VB.TextBox txtCurrentSite 
            Height          =   285
            Left            =   5760
            TabIndex        =   113
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtGEMPAK 
            Height          =   285
            Left            =   5760
            TabIndex        =   93
            Top             =   720
            Width           =   615
         End
         Begin VB.CommandButton cmdSearch 
            Caption         =   "Search GEMPAK for =>"
            Height          =   735
            Index           =   2
            Left            =   4560
            TabIndex        =   92
            ToolTipText     =   "Search C:\MTP\RAOB\station-query.html"
            Top             =   240
            Width           =   1095
         End
         Begin VB.TextBox txtSaveSearch 
            Height          =   285
            Left            =   120
            TabIndex        =   89
            Text            =   "C:\MTP\RAOB\Search.txt"
            Top             =   1440
            Width           =   2775
         End
         Begin VB.CommandButton cmdSearch 
            Caption         =   "Save"
            Height          =   375
            Index           =   1
            Left            =   3000
            TabIndex        =   88
            ToolTipText     =   "Save Search in Filename to Left"
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtSearch 
            Height          =   285
            Index           =   4
            Left            =   1560
            TabIndex        =   87
            Top             =   960
            Width           =   375
         End
         Begin VB.TextBox txtSearch 
            Height          =   285
            Index           =   3
            Left            =   1560
            TabIndex        =   86
            Top             =   720
            Width           =   375
         End
         Begin VB.TextBox txtSearch 
            Height          =   285
            Index           =   2
            Left            =   1560
            TabIndex        =   85
            Top             =   480
            Width           =   375
         End
         Begin VB.TextBox txtSearch 
            Height          =   285
            Index           =   1
            Left            =   1560
            TabIndex        =   84
            Top             =   240
            Width           =   375
         End
         Begin VB.CheckBox chkSearch 
            Caption         =   "WMO Block"
            Height          =   255
            Index           =   4
            Left            =   120
            TabIndex        =   83
            ToolTipText     =   "Select Sites in WMO Block"
            Top             =   960
            Width           =   1335
         End
         Begin VB.CheckBox chkSearch 
            Caption         =   "State"
            Height          =   255
            Index           =   3
            Left            =   120
            TabIndex        =   82
            ToolTipText     =   "Select Sites in State"
            Top             =   720
            Width           =   735
         End
         Begin VB.CheckBox chkSearch 
            Caption         =   "Country"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   81
            ToolTipText     =   "Select Sites in Country"
            Top             =   480
            Width           =   1335
         End
         Begin VB.CheckBox chkSearch 
            Caption         =   "WMO Region"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   80
            ToolTipText     =   "Select sites in WMO region"
            Top             =   240
            Width           =   1335
         End
         Begin VB.CheckBox chkSearch 
            Caption         =   "Area"
            Height          =   255
            Index           =   0
            Left            =   2160
            TabIndex        =   79
            ToolTipText     =   "Select Sites in Lat/Lon Region"
            Top             =   240
            Width           =   735
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   3720
            MultiLine       =   -1  'True
            TabIndex        =   73
            Top             =   960
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   72
            Top             =   960
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   3720
            MultiLine       =   -1  'True
            TabIndex        =   71
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRange 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   3000
            MultiLine       =   -1  'True
            TabIndex        =   70
            Top             =   600
            Width           =   615
         End
         Begin VB.CommandButton cmdSearch 
            Caption         =   "Start"
            Height          =   375
            Index           =   0
            Left            =   3720
            TabIndex        =   62
            ToolTipText     =   "Do Selected Search & Put in Site List"
            Top             =   1320
            Width           =   615
         End
         Begin VB.Label Label 
            Caption         =   "Looking at Line"
            Height          =   255
            Index           =   11
            Left            =   4560
            TabIndex        =   112
            Top             =   1080
            Width           =   1095
         End
         Begin VB.Line Line2 
            X1              =   2040
            X2              =   2040
            Y1              =   240
            Y2              =   1320
         End
         Begin VB.Line Line1 
            X1              =   4440
            X2              =   4440
            Y1              =   240
            Y2              =   1680
         End
         Begin VB.Label Label13 
            Caption         =   "Longitude"
            Height          =   255
            Left            =   2160
            TabIndex        =   76
            Top             =   960
            Width           =   735
         End
         Begin VB.Label Label1 
            Caption         =   "Latitude"
            Height          =   255
            Left            =   2160
            TabIndex        =   75
            Top             =   600
            Width           =   615
         End
         Begin VB.Label Label20 
            Caption         =   "   From         To"
            Height          =   255
            Left            =   3000
            TabIndex        =   74
            Top             =   360
            Width           =   1335
         End
      End
      Begin MSComDlg.CommonDialog cdConvertIn 
         Left            =   -74880
         Top             =   4800
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.CommandButton cmdStartConversion 
         Caption         =   "Start Conversion"
         Height          =   705
         Left            =   -69390
         TabIndex        =   59
         Top             =   4170
         Width           =   1005
      End
      Begin VB.Frame Frame2 
         Caption         =   "ASCII RAOB Input"
         Height          =   1335
         Left            =   -74880
         TabIndex        =   47
         Top             =   360
         Width           =   6495
         Begin VB.ComboBox cmbSrcFormat 
            Height          =   315
            ItemData        =   "frmRAOBget.frx":04A0
            Left            =   720
            List            =   "frmRAOBget.frx":04A2
            TabIndex        =   53
            Text            =   "Wyoming"
            Top             =   240
            Width           =   1095
         End
         Begin VB.TextBox txtFileMask 
            Height          =   285
            Left            =   4080
            TabIndex        =   52
            Text            =   "R*.*"
            Top             =   600
            Width           =   2295
         End
         Begin VB.TextBox txtPath 
            Height          =   285
            Left            =   720
            TabIndex        =   51
            Text            =   "C:\er2\cirex\raob\"
            Top             =   600
            Width           =   2655
         End
         Begin VB.CommandButton cmdConvertIn 
            Caption         =   "Input File"
            Height          =   255
            Left            =   120
            TabIndex        =   50
            ToolTipText     =   "Use to convert single file"
            Top             =   960
            Width           =   975
         End
         Begin VB.TextBox txtConvertIn 
            Height          =   285
            Left            =   1200
            TabIndex        =   49
            Top             =   960
            Width           =   5175
         End
         Begin VB.ComboBox cmbInputMode 
            Height          =   315
            Left            =   3000
            TabIndex        =   48
            Text            =   "Single File with many RAOBs"
            Top             =   240
            Width           =   3375
         End
         Begin VB.Label lblLabel 
            Caption         =   "Format"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   58
            Top             =   240
            Width           =   615
         End
         Begin VB.Label lblSource 
            Caption         =   "File Mask"
            Height          =   255
            Index           =   1
            Left            =   3480
            TabIndex        =   57
            Top             =   600
            Width           =   975
         End
         Begin VB.Label lblSource 
            Caption         =   "Path"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   56
            Top             =   600
            Width           =   495
         End
         Begin VB.Label lblLabel 
            Caption         =   "Mode"
            Height          =   255
            Index           =   1
            Left            =   2400
            TabIndex        =   55
            Top             =   240
            Width           =   495
         End
         Begin VB.Label Label2 
            Caption         =   "Label2"
            Height          =   135
            Left            =   480
            TabIndex        =   54
            Top             =   360
            Width           =   15
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "Random RAOB Output"
         Height          =   855
         Left            =   -74880
         TabIndex        =   41
         Top             =   2280
         Width           =   6495
         Begin VB.CommandButton cmdConvertOut 
            Caption         =   "Output File"
            Height          =   255
            Left            =   120
            TabIndex        =   45
            ToolTipText     =   "Select random output file"
            Top             =   480
            Width           =   975
         End
         Begin VB.TextBox txtConvertOut 
            Height          =   285
            Left            =   1200
            TabIndex        =   44
            Text            =   "D:\RAOB\FSL.RAOB"
            Top             =   480
            Width           =   5175
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "New File"
            Height          =   255
            Index           =   0
            Left            =   1200
            TabIndex        =   43
            Top             =   240
            Width           =   975
         End
         Begin VB.OptionButton optOutput 
            Caption         =   "Append"
            Height          =   255
            Index           =   1
            Left            =   2160
            TabIndex        =   42
            Top             =   240
            Width           =   975
         End
         Begin VB.Label lblLabel 
            Caption         =   "Mode"
            Height          =   255
            Index           =   2
            Left            =   120
            TabIndex        =   46
            Top             =   240
            Width           =   495
         End
      End
      Begin VB.DriveListBox Drive1 
         Height          =   315
         Left            =   -73800
         TabIndex        =   40
         Top             =   1800
         Width           =   5415
      End
      Begin VB.CheckBox chkFindTrop 
         Caption         =   "Find Tropopause"
         Height          =   255
         Left            =   -70800
         TabIndex        =   39
         Top             =   3600
         Value           =   1  'Checked
         Width           =   1575
      End
      Begin VB.Frame Frame1 
         Caption         =   "Save Options"
         Height          =   1215
         Left            =   120
         TabIndex        =   33
         Top             =   3630
         Width           =   5415
         Begin VB.TextBox txtRAOBfile 
            Height          =   285
            Left            =   3480
            TabIndex        =   106
            Text            =   "Default.RAOB2"
            ToolTipText     =   "Name of RAOB file"
            Top             =   840
            Width           =   1815
         End
         Begin VB.CheckBox chkRTsave 
            Caption         =   "Save in .RAOB file                       Filename"
            Height          =   255
            Left            =   120
            TabIndex        =   105
            ToolTipText     =   "Convert TXT file to RAOB file"
            Top             =   840
            Width           =   3285
         End
         Begin VB.CheckBox chkAutoSave 
            Alignment       =   1  'Right Justify
            Caption         =   "Auto Save"
            Height          =   255
            Left            =   2265
            TabIndex        =   38
            ToolTipText     =   "Automatically Save Retrieved Files"
            Top             =   210
            Width           =   1095
         End
         Begin VB.TextBox txtSaveName 
            Enabled         =   0   'False
            Height          =   285
            Left            =   3480
            TabIndex        =   37
            ToolTipText     =   "Name of current TXT file"
            Top             =   480
            Width           =   1815
         End
         Begin VB.TextBox txtSavePath 
            Height          =   285
            Left            =   120
            TabIndex        =   36
            Text            =   "G:\DC8\PEMTB\RAOB\"
            ToolTipText     =   "Path to TXT and RAOB files"
            Top             =   480
            Width           =   3285
         End
         Begin VB.Label Label 
            Caption         =   "Current .txt File"
            Height          =   255
            Index           =   7
            Left            =   3510
            TabIndex        =   35
            Top             =   240
            Width           =   1215
         End
         Begin VB.Label Label 
            Caption         =   "Path"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   34
            Top             =   240
            Width           =   375
         End
      End
      Begin VB.CommandButton cmdButton 
         Caption         =   "&Clear List"
         Height          =   375
         Index           =   3
         Left            =   120
         TabIndex        =   32
         Top             =   2520
         Width           =   1095
      End
      Begin VB.ListBox lbList 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1950
         Index           =   1
         ItemData        =   "frmRAOBget.frx":04A4
         Left            =   -74040
         List            =   "frmRAOBget.frx":04A6
         MultiSelect     =   1  'Simple
         TabIndex        =   31
         Top             =   960
         Width           =   855
      End
      Begin VB.TextBox txtList 
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
         Index           =   1
         Left            =   -74040
         TabIndex        =   30
         Top             =   600
         Width           =   855
      End
      Begin VB.ListBox lbList 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1950
         Index           =   0
         ItemData        =   "frmRAOBget.frx":04A8
         Left            =   -74880
         List            =   "frmRAOBget.frx":04AA
         MultiSelect     =   1  'Simple
         TabIndex        =   28
         Top             =   960
         Width           =   795
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Clear List"
         Height          =   375
         Index           =   0
         Left            =   -73140
         TabIndex        =   27
         ToolTipText     =   "Clear RAOB Site List"
         Top             =   1200
         Width           =   1095
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Remove"
         Height          =   375
         Index           =   2
         Left            =   -73140
         TabIndex        =   26
         ToolTipText     =   "REmove This Site from List"
         Top             =   840
         Width           =   1095
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Add Site"
         Height          =   375
         Index           =   3
         Left            =   -73140
         TabIndex        =   25
         ToolTipText     =   "Add This Site to List"
         Top             =   480
         Width           =   1095
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Transfer &All"
         Height          =   375
         Index           =   4
         Left            =   -73140
         TabIndex        =   24
         ToolTipText     =   "Transfer All Sites to Download List"
         Top             =   2640
         Width           =   1095
      End
      Begin VB.TextBox txtList 
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
         Index           =   0
         Left            =   -74880
         TabIndex        =   23
         Top             =   600
         Width           =   795
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "Transfer &One"
         Height          =   375
         Index           =   6
         Left            =   -73140
         TabIndex        =   22
         ToolTipText     =   "Transfer Selected Site to Download List"
         Top             =   2280
         Width           =   1095
      End
      Begin VB.CommandButton cmdSelect 
         Caption         =   "&Details"
         Height          =   375
         Index           =   1
         Left            =   -73140
         TabIndex        =   21
         ToolTipText     =   "Show Details for Selected Site"
         Top             =   1560
         Width           =   1095
      End
      Begin VB.TextBox txtSaveFile 
         Height          =   285
         Left            =   -71100
         TabIndex        =   20
         Text            =   "C:\MTP\RAOB\RSL\Save.RSL"
         Top             =   1980
         Width           =   2715
      End
      Begin VB.ComboBox cmbStation 
         Height          =   315
         Left            =   120
         TabIndex        =   13
         ToolTipText     =   "Sites to be download from Station List"
         Top             =   2160
         Width           =   1095
      End
      Begin VB.CommandButton cmdButton 
         Caption         =   "&Retrieve"
         Height          =   375
         Index           =   0
         Left            =   5640
         TabIndex        =   12
         Top             =   3960
         Width           =   975
      End
      Begin VB.ComboBox cmbHour 
         Height          =   315
         Index           =   0
         Left            =   1800
         TabIndex        =   11
         Text            =   "00"
         Top             =   2160
         Width           =   615
      End
      Begin VB.ComboBox cmbDay 
         Height          =   315
         Index           =   0
         Left            =   2520
         TabIndex        =   10
         Top             =   2160
         Width           =   615
      End
      Begin VB.ComboBox cmbMonth 
         Height          =   315
         Index           =   0
         Left            =   3240
         TabIndex        =   9
         Text            =   "JAN"
         Top             =   2160
         Width           =   735
      End
      Begin VB.ComboBox cmbYear 
         Height          =   315
         Index           =   0
         Left            =   4020
         TabIndex        =   8
         Text            =   "1999"
         Top             =   2160
         Width           =   735
      End
      Begin VB.CommandButton cmdButton 
         Caption         =   "&Save"
         Height          =   375
         Index           =   1
         Left            =   5640
         TabIndex        =   6
         Top             =   4440
         Width           =   975
      End
      Begin VB.ComboBox cmbYear 
         Height          =   315
         Index           =   1
         Left            =   4020
         TabIndex        =   5
         Text            =   "1999"
         Top             =   2520
         Width           =   735
      End
      Begin VB.ComboBox cmbMonth 
         Height          =   315
         Index           =   1
         Left            =   3240
         TabIndex        =   4
         Text            =   "JAN"
         Top             =   2520
         Width           =   735
      End
      Begin VB.ComboBox cmbDay 
         Height          =   315
         Index           =   1
         Left            =   2520
         TabIndex        =   3
         Top             =   2520
         Width           =   615
      End
      Begin VB.ComboBox cmbHour 
         Height          =   315
         Index           =   1
         Left            =   1800
         TabIndex        =   2
         Text            =   "00"
         Top             =   2520
         Width           =   615
      End
      Begin VB.CheckBox chkEnableInet 
         Caption         =   "Enable Inet"
         Height          =   225
         Left            =   4830
         TabIndex        =   1
         Top             =   2880
         Width           =   1215
      End
      Begin RichTextLib.RichTextBox rtfSounding 
         Height          =   1455
         Left            =   120
         TabIndex        =   7
         Top             =   360
         Width           =   6495
         _ExtentX        =   11456
         _ExtentY        =   2566
         _Version        =   393217
         Enabled         =   -1  'True
         TextRTF         =   $"frmRAOBget.frx":04AC
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Location Maps"
         Height          =   255
         Index           =   13
         Left            =   -71070
         TabIndex        =   115
         Top             =   2460
         Width           =   1815
      End
      Begin VB.Label Label 
         Caption         =   "Delay [s]"
         Height          =   255
         Index           =   23
         Left            =   1440
         TabIndex        =   102
         Top             =   3330
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "MRI Levels [km]"
         Height          =   255
         Index           =   22
         Left            =   -70800
         TabIndex        =   97
         Top             =   3240
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Step"
         Height          =   255
         Index           =   19
         Left            =   1440
         TabIndex        =   90
         Top             =   2880
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Sites"
         Height          =   255
         Index           =   16
         Left            =   -71610
         TabIndex        =   78
         Top             =   2730
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "   ICAO         WMO no."
         Height          =   255
         Index           =   15
         Left            =   -74880
         TabIndex        =   64
         Top             =   360
         Width           =   1755
      End
      Begin VB.Label Label 
         Caption         =   "Default Drive"
         Height          =   255
         Index           =   10
         Left            =   -74880
         TabIndex        =   60
         Top             =   1800
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "Hour"
         Height          =   255
         Index           =   9
         Left            =   1800
         TabIndex        =   29
         Top             =   1920
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "RAOB Site List"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   19
         Top             =   1920
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Start"
         Height          =   255
         Index           =   2
         Left            =   1440
         TabIndex        =   18
         Top             =   2160
         Width           =   375
      End
      Begin VB.Label Label 
         Caption         =   "Day"
         Height          =   255
         Index           =   3
         Left            =   2520
         TabIndex        =   17
         Top             =   1920
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Month"
         Height          =   255
         Index           =   4
         Left            =   3240
         TabIndex        =   16
         Top             =   1920
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "Year"
         Height          =   255
         Index           =   5
         Left            =   4020
         TabIndex        =   15
         Top             =   1920
         Width           =   495
      End
      Begin VB.Label Label 
         Caption         =   "End"
         Height          =   255
         Index           =   12
         Left            =   1440
         TabIndex        =   14
         Top             =   2520
         Width           =   375
      End
   End
   Begin VB.Label Label 
      Caption         =   "Status"
      Height          =   255
      Index           =   8
      Left            =   120
      TabIndex        =   119
      Top             =   5040
      Width           =   495
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsFormat 
         Caption         =   "&Format RAOB PLot"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "frmRAOBget"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim ConvertInFile$, GOTOcommand As Boolean, SourceFormat%
Dim InputMode%


'Dim Site1 As Boolean, TimeSec&, Ltn!, Lnn!, EventNumber&, EventTime&
 Dim IyearE%, IdoyE%, ImonthE%, IdayE%
'Dim DefaultBG As Boolean
'Dim EventList$(1 To 20), OAT11!, OAT21!, OAT12!, OAT22!
'Dim CurrentRecord&

Public InFile$
Public SitePrefix$
Public AddIndex%, SelectIndex%, DetailsIndex%

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
.Left = (Screen.Width - .Width) / 2
.Top = (Screen.Height - .Height) / 2
.Show
End With
End Sub


Private Sub chkPlotRAOB_Click()
  
  With frmRAOB
    If chkPlotRAOB.Value = 1 Then
      .PlotSingleScan = True
      .LineColor = Yellow
      .LineThickness = 2
      .Visible = True
      .Left = frmRAOBget.Width
      .Top = frmRAOBget.Top
      .Height = frmRAOBget.Height
      .Width = 1024 * Screen.TwipsPerPixelX - frmRAOBget.Width
    Else
      .Visible = False
    End If
  End With
End Sub

Private Sub chkSingleSoundings_Click()

  If chkSingleSoundings.Value = 1 Then
    cmbMonth(1).Enabled = True
  Else
    cmbMonth(1).Enabled = False
  End If
  
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

SourceFormat = cmbSrcFormat.ListIndex
Select Case SourceFormat
Case 0 'Forecast Systems Laboratory (FSL)
  txtFileMask.Text = "*.fsl"
Case 1 'Wyoming
  txtFileMask.Text = "*.txt"
Case 2 'BLG
  txtFileMask.Text = "r*.*"
Case 3 'SJK
  txtFileMask.Text = "*.sjk"
Case 4 'Pfister
  txtFileMask.Text = "*.txt"
Case 5 'EAFB
  txtFileMask.Text = "edw*.txt"
Case 6 'UCSD
  txtFileMask.Text = "*.txt"
Case 7 'Makky
  txtFileMask.Text = "RS*.*"
End Select

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


Private Sub cmdExtractUA_Click()
  UAfromFlatfile
End Sub


Private Sub cmdSearch_Click(Index As Integer)
Dim Record&, lu%, INPlu%, OUTlu%, X$, V!(1 To 3)
Dim UseSite As Boolean, f(0 To 4) As Boolean, FindWMO As Boolean
Dim LAT1!, LAT2!, LON1!, LON2!, GPnum&
Dim ICAOstn$, WMOnum&, Found As Boolean, A$

  LAT1 = Val(txtRange(0).Text): LAT2 = Val(txtRange(1).Text)
  LON1 = Val(txtRange(2).Text): LON2 = Val(txtRange(3).Text)
  
Select Case Index
Case 0
  INPlu = FreeFile
  Open "C:\MTP\RAOB\BIN\MTP_UA.bin" For Random Access Read Write As INPlu Len = Len(GP)
  
  Record = 0
  Do
    Record = Record + 1
    For i = 0 To 4: f(i) = False: Next i
    'txtProgress.Text = Str$(Record)
    DoEvents
    GPread INPlu, Record
    
   
    If chkSearch(0).Value = 1 Then  'Region window
      If GPlatitude > LAT1 And GPlatitude < LAT2 Then
        If GPlongitude > LON1 And GPlongitude < LON2 Then
          f(0) = True
        End If
      End If
    Else
      f(0) = True
    End If
      
    If chkSearch(1).Value = 1 Then 'WMO Region
      If GPregion = txtSearch(1).Text Then f(1) = True
      Debug.Print GPregion; ICAOsite; WMOnumber
    Else
      f(1) = True
    End If
    If chkSearch(2).Value = 1 Then 'Country
      If GPcountry = txtSearch(2).Text Then f(2) = True
    Else
      f(2) = True
    End If
    
    If chkSearch(3).Value = 1 Then 'State
      If GPstate = txtSearch(3).Text Then f(3) = True
    Else
      f(3) = True
    End If
     
    If chkSearch(4).Value = 1 Then 'WMO Block number
      If Int(WMOnumber / 1000) = Val(txtSearch(4).Text) Then f(4) = True
    Else
      f(4) = True
    End If
      
    UseSite = f(0) And f(1) And f(2) And f(3) And f(4)
    If UseSite Then
      lbList(0).AddItem ICAOsite
      lbList(1).AddItem WMOnumber
'      txtList(1).Text = WMOnumber
'      cmdSelect_Click (3)
    End If
  Loop Until EOF(INPlu)
  Close INPlu
  
Case 1    'Write two header lines for Search Save
  OUTlu = FreeFile
  Open txtSaveSearch.Text For Output As OUTlu
  Print #OUTlu, Mission$ + " Region RAOB stations"
  X$ = Format(LAT1, "###0.0") + " " + Format(LAT2, "###0.0") + " " + Format(LON1, "####0.0") + " " + Format(LON2, "####0.0")
  Print #OUTlu, "LatMin, LatMax, LonMin, LonMax: " + X$
  Print #OUTlu, "WMO No  WMO  LATITUDE LONGITUDE AltM  STATION LOCATION" ' SONEX15 Region RaOb stations
  lu = FreeFile
  Open "C:\MTP\RAOB\MTP_UA.bin" For Random Access Read Write As INPlu Len = Len(WMO)
  Record = 0
  Do
    Record = Record + 1
  ' txtProgress.Text = Str$(Record)
    DoEvents
    Call GPread(INPlu, Record)
    X$ = " " + Format(WMOnumber, "00000") + "  " + WMO4l + "  "
    V(1) = WMOlatitude: V(2) = WMOlongitude: V(3) = WMOaltitude
    Print #OUTlu, X$ + fUsing$("#000.00   #000.00 #000", V()) + "  " + WMOstation
  Loop Until EOF(INPlu)
  Close INPlu, OUTlu
    
Case 2  ' Get record from GEMPAK file (need if WMO misses a site, eg NKX)
  INPlu = FreeFile
  Open "C:\MTP\RAOB\station-query.html" For Input As INPlu
  A$ = fReadLF(INPlu)
  A$ = fReadLF(INPlu)
  txtGEMPAK.Text = UCase$(txtGEMPAK.Text)
  If fNumeric(txtGEMPAK.Text) Then
    FindWMO = True
    WMOnum = Val(txtGEMPAK.Text)
  Else
    FindWMO = False
    ICAOstn = txtGEMPAK.Text
  End If
  GPnum = 2
  Do
    A$ = fReadLF(INPlu)
    GPnum = GPnum + 1
    txtCurrentSite.Text = Val(GPnum)
    If FindWMO Then
      If WMOnum = Val(Mid$(A$, 10, 6)) Then Found = True
    Else
      If ICAOstn = Trim(Left$(A$, 8)) Then Found = True
    End If
    DoEvents
  Loop Until EOF(INPlu) Or Found
  Close INPlu
  If Found Then
  '00000000011111111112222222222333333333344444444445555555555666666666677777777778
  '12345678901234567890123456789012345678901234567890123456789012345678901234567890
  'EDW       72381 EDWARDS AFB                      CA US  3490 -11788   702
    With frmRAOBsites
      .Visible = True
      .txtShow(1).Text = Val(Mid$(A$, 10, 6))         'WMO
      .txtShow(0).Text = Trim(Left$(A$, 8))           'ICAO
      .txtLocation(2).Text = Mid$(A$, 50, 2)              'State
      .txtLocation(1).Text = Mid$(A$, 53, 2)              'Country
      .txtLocation(3).Text = Trim(Mid$(A$, 17, 32))       'Location
      .txtLocation(4).Text = Val(Mid$(A$, 56, 5)) / 100#  'Latitude
      .txtLocation(5).Text = Val(Mid$(A$, 62, 6)) / 100#  'Longitude
      .txtLocation(6).Text = Val(Mid$(A$, 69, 5))         'Geopotential Height [m]
    End With
  End If

End Select
  
End Sub

Private Sub cmdSelect_Click(Index As Integer)
Dim i%, A$, INPlu%, UArecord&, SelectIndexSave%

Select Case Index
Case 0  'Clear List
  lbList(0).Clear
  lbList(1).Clear

Case 1  'Details
  With frmRAOBsites
    .Visible = True
    .optShow(SelectIndex).Value = True
    .txtShow(SelectIndex).Text = lbList(SelectIndex).List(lbList(SelectIndex).ListIndex)
    .ShowDetails
  End With

  
Case 2  'Remove item from list
  SelectIndexSave = SelectIndex
  For i = 0 To lbList(SelectIndexSave).ListCount - 1 'Match the two lists
    If lbList(SelectIndexSave).Selected(i) Then
      If SelectIndexSave = 0 Then
        lbList(1).Selected(i) = True
      Else
        lbList(0).Selected(i) = True
      End If
    Else
      If SelectIndexSave = 0 Then
        lbList(1).Selected(i) = False
      Else
        lbList(0).Selected(i) = False
      End If
    End If
  Next i
  DoEvents
  Call ListBoxRemoveItem(lbList(0), True)
  Call ListBoxRemoveItem(lbList(1), True)
  
'  If i >= 0 Then
'    If i >= 0 And i < lbList(0).ListCount - 1 Then
'      lbList(0).RemoveItem (i)
'      lbList(0).Selected(i) = True
'    Else
'      lbList(0).RemoveItem (i)
'      If i > 0 Then lbList(0).Selected(i - 1) = True
'    End If
'    If i >= 0 And i < lbList(1).ListCount - 1 Then
'      lbList(1).RemoveItem (i)
'      lbList(1).Selected(i) = True
'    Else
'      lbList(1).RemoveItem (i)
'      If i > 0 Then lbList(1).Selected(i - 1) = True
'    End If
'  End If
  
Case 3  'Add item to list
  Status = fGetUAsite(UCase$(txtList(AddIndex).Text), UArecord)
  If Status Then
    If AddIndex = 0 Then lbList(0).AddItem ICAOsite Else lbList(0).AddItem "XXXX" 'Don't use ICAO if WMO given (Wyoming defaults to WMO!)
    lbList(1).AddItem WMOnumber
    lbList(AddIndex).ListIndex = lbList(AddIndex).ListCount - 1
    If chkDetails.Value = 1 Then
      With frmRAOBsites
        .Visible = True
        .optShow(AddIndex).Value = True
        .txtShow(AddIndex).Text = lbList(AddIndex).List(lbList(AddIndex).ListIndex)
        .ShowDetails
      End With
    End If
    txtList(0).Text = ""
    txtList(1).Text = ""
End If

Case 4  'Copy Entire list to GetRAOB Stations
  For i = 0 To lbList(1).ListCount - 1
    Debug.Print i; lbList(0).List(i)
    If lbList(0).List(i) = "XXXX" Then
      A$ = lbList(1).List(i)
      Select Case Len(A$)
      Case 1: A$ = "0000" + A$
      Case 2: A$ = "000" + A$
      Case 3: A$ = "00" + A$
      Case 4: A$ = "0" + A$
      End Select
      cmbStation.AddItem A$
    Else
      cmbStation.AddItem lbList(0).List(i)
    End If
  Next i
  
Case 5  'Open RSL file
' Set Initial directory
  cdConvertIn.InitDir = RSpath$   '"C:\MTP\RAOB\RSL\"
' Set Size of FileName buffer
  cdConvertIn.MaxFileSize = 1024
' Set filters.
  cdConvertIn.Filter = "RSL Files|*.RSL|All Files (*.*)|*.*"
' Specify default filter.
  cdConvertIn.FilterIndex = 1
' Display the File Open dialog box.
  cdConvertIn.ShowOpen

ConvertInFile$ = cdConvertIn.Filename
txtSaveFile.Text = cdConvertIn.Filename
If ConvertInFile$ = "" Then
  Call MsgBox("Select Convert In File!", vbOKOnly)
  Exit Sub
Else
'  On Error GoTo errhandler
  INPlu% = FreeFile
  Open ConvertInFile$ For Input As INPlu%
  Do
    Input #INPlu, A$
    If fNumeric(A$) Then
      txtList(1).Text = A$
    Else
      txtList(0).Text = A$
    End If
    cmdSelect_Click (3)
  Loop Until EOF(INPlu)
  Close INPlu
End If

Case 6  'Copy select item to GetRAOB Stations
  X$ = lbList(SelectIndex).List(lbList(SelectIndex).ListIndex)
  cmbStation.AddItem X$
  cmbStation.Text = X$
  
Case 7  'Save list of stations
  If Dir(txtSaveFile.Text) <> "" Then Kill txtSaveFile.Text
  lu = FreeFile
  Open txtSaveFile.Text For Output As #lu
  
  If chkLocation.Value = 1 Then
    For i = 0 To lbList(1).ListCount - 1
      Status = fGetUAsite(Str(lbList(1).List(i)), UArecord)
      If Status Then
        If Len(ICAOsite) = 0 Then ICAOsite = " "
        Print #lu, ICAOsite, vbTab, WMOnumber, vbTab, GPlatitude, vbTab, GPlongitude, vbTab, Ztrop, vbTab, Ttrop
      End If
    Next i
  Else
    For i = 0 To lbList(0).ListCount - 1
      If lbList(0).List(i) = "" Then
        Print #lu, lbList(1).List(i)  'Write WMO if no ICAO
      Else
        Print #lu, lbList(0).List(i)
      End If
    Next i
  End If
  Close lu

Case 8 'Open Mission Default RSL file (eg TC4.RSL)
  ConvertInFile$ = "C:\MTP\Data\" + Platform$ + "\" + Mission$ + "\RAOB\" + Mission$ + ".RSL"
  txtSaveFile.Text = ConvertInFile$
  If ConvertInFile$ = "" Then
    Call MsgBox("Select Convert In File!", vbOKOnly)
    Exit Sub
  Else
'   On Error GoTo errhandler
    INPlu% = FreeFile
    Open ConvertInFile$ For Input As INPlu%
    Do
      Input #INPlu, A$
      If fNumeric(A$) Then
        txtList(1).Text = A$
      Else
        txtList(0).Text = A$
      End If
      cmdSelect_Click (3)
    Loop Until EOF(INPlu)
    Close INPlu
  End If

End Select
txtListItems.Text = lbList(0).ListCount
End Sub

Private Sub cmbDay_Click(Index As Integer)
If Index = 0 Then cmbDay(1).ListIndex = cmbDay(0).ListIndex
End Sub


Private Sub cmbHour_Click(Index As Integer)
If Index = 0 Then cmbHour(1).ListIndex = cmbHour(0).ListIndex
End Sub


Private Sub cmbMonth_Click(Index As Integer)
If Index = 0 Then cmbMonth(1).ListIndex = cmbMonth(0).ListIndex
End Sub


Private Sub cmbYear_Click(Index As Integer)
If Index = 0 Then cmbYear(1).ListIndex = cmbYear(0).ListIndex
End Sub



Private Sub cmdButton_Click(Index As Integer)
Dim StartTime As Date, EndTime As Date, StartDate As Date, EndDate As Date, GetDate As Date
Dim URL$, Station$, Hour$, Day$, Month$, Year$, lu%, From$, Too$, GetTime As Date, StepTime As Date
Dim OutFilename$, i%, H As Date, Delay!, ErrNumber, TimeOutFlag As Boolean, TempFilename$
Dim First$, A$, YYYYMMDD$
Static OUTlu%, OUTlu2%, INPlu%, INPlu2%, X!

  On Error GoTo ErrorHandler
  Select Case Index
  Case 0  'Read file over Internet
  ' If realtime conversion, then assign OUTPUT file
    If chkRTsave.Value = 1 Then
      If Len(Dir(txtSavePath.Text)) = 0 Then
        Call CreateDirectoryTree(txtSavePath.Text, True)
      End If
      OUTlu = FreeFile
      Open txtSavePath.Text + txtRAOBfile.Text For Random Access Read Write As OUTlu% Len = Len(RAOB2)
      OutFileRecord = 0
      RAOBtype = 0
    End If
'http://weather.uwyo.edu/cgi-bin/sounding?region=naconf&TYPE=TEXT%3ALIST&YEAR=2001&MONTH=09&FROM=2512&TO=2512&STNM=72387
'http://weather.uwyo.edu/cgi-bin/sounding?region=np&TYPE=TEXT%3ALIST&YEAR=2001&MONTH=09&FROM=2512&TO=2512&STNM=71917
'http://weather.uwyo.edu/cgi-bin/sounding?region=naconf&TYPE=TEXT%3ALIST&YEAR=2004&MONTH=12&FROM=1200&TO=1512&STNM=72393
    SitePrefix$ = "http://weather.uwyo.edu/cgi-bin/sounding?&TYPE=TEXT%3ALIST"
    ' First check if beginning and end times are the same
    StartDate = cmbYear(0).Text + "/" + Format$(cmbMonth(0).ListIndex + 1, "00") + "/" + Format(Val(cmbDay(0).Text), "00")
    EndDate = cmbYear(1).Text + "/" + Format$(cmbMonth(1).ListIndex + 1, "00") + "/" + Format(Val(cmbDay(1).Text), "00")
    StartTime = Format(Val(cmbHour(0).Text), "00") + ":00"
    EndTime = Format(Val(cmbHour(1).Text), "00") + ":00"
    StepTime = Format(Val(cmbHour(2).Text), "00") + ":00"
    StartDate = StartDate + StartTime
    EndDate = EndDate + EndTime
    ' Check to see if there is a site list or something in Combo text box
    If cmbStation.ListCount = 0 Then
      If cmbStation.Text <> "" Then
        cmbStation.AddItem cmbStation.Text
      Else
        MsgBox "You have not select a Site!" + vbCr + "There is nothing to do.", vbOKOnly
        Exit Sub
      End If
    End If
    Delay = Val(cboDelay.Text)
  
    For i = 0 To cmbStation.ListCount - 1
      cmbStation.Text = UCase$(cmbStation.List(i))
    
      If chkSingleSoundings.Value = 1 Then  'Get soundings one at a time
        For GetDate = StartDate To EndDate Step StepTime
          If Delay > 0 Then GoSleep Delay
          GetTime = GetDate
        'Hour$ = "&HOUR=" + Format(DatePart("h", GetTime), "00")
        'Day$ = "&DAY=" + Format$(DatePart("d", GetTime), "00")
          Month$ = "&MONTH=" + Format$(DatePart("m", GetTime), "00")
          Year$ = "&YEAR=" + Format(DatePart("yyyy", GetTime), "0000")
          Station$ = "&STNM=" + cmbStation.Text
          From$ = "&FROM=" + Format$(DatePart("d", GetTime), "00") + Format(DatePart("h", GetTime), "00")
          Too$ = "&TO=" + Format$(DatePart("d", GetTime), "00") + Format(DatePart("h", GetTime), "00")
'&YEAR=2001&MONTH=09&FROM=2512&TO=2512&STNM=71917
          URL$ = SitePrefix$ + Year$ + Month$ + From$ + Too$ + Station$

          OutFilename$ = cmbStation.Text + Format(DatePart("yyyy", GetTime), "0000") + Format$(DatePart("m", GetTime), "00")
          OutFilename$ = OutFilename$ + Format$(DatePart("d", GetTime), "00") + Format(DatePart("h", GetTime), "00") + ".txt"
          txtSaveName.Text = OutFilename$
          txtStatus.Text = "Waiting ..."
TryAgain:
          TimeOutFlag = False
          If chkEnableInet Then InFile$ = Inet1.OpenURL(URL$)
          If TimeOutFlag Then GoTo TryAgain
          InFile$ = fLFtoCRLF$(InFile$)  'Make sure there is a CRLF on each line
          rtfSounding.Text = InFile$
          If chkAutoSave.Value = 1 And Left$(InFile$, 3) <> "UPA" Then cmdButton_Click (1)  'Save file
        Next GetDate
    
      Else  'Get a range of soundings (but only for 1 month)
        If Delay > 0 Then GoSleep Delay
        StartDate = cmbYear(0).Text + "/" + Format$(cmbMonth(0).ListIndex + 1, "00") + "/" + Format(Val(cmbDay(0).Text), "00")
        ' Force EndDate to be for the same month
        EndDate = cmbYear(1).Text + "/" + Format$(cmbMonth(0).ListIndex + 1, "00") + "/" + Format(Val(cmbDay(1).Text), "00")
        StartDate = StartDate + StartTime
        EndDate = EndDate + EndTime
        Month$ = "&MONTH=" + Format$(DatePart("m", StartDate), "00")
        Year$ = "&YEAR=" + Format(DatePart("yyyy", StartDate), "0000")
        
        Station$ = "&STNM=" + Format(fGetWMOnumber(cmbStation.Text), "00000")
        From$ = "&FROM=" + Format$(DatePart("d", StartDate), "00") + Format(DatePart("h", StartDate), "00")
        Too$ = "&TO=" + Format$(DatePart("d", EndDate), "00") + Format(DatePart("h", EndDate), "00")
'&YEAR=2001&MONTH=09&FROM=2512&TO=3012&STNM=71917
        URL$ = SitePrefix$ + Year$ + Month$ + From$ + Too$ + Station$
    
        TempFilename$ = "_" + cmbStation.Text + Format(DatePart("yyyy", StartDate), "0000") + Format$(DatePart("m", StartDate), "00")
        TempFilename$ = TempFilename$ + Format$(DatePart("d", StartDate), "00") + Format$(DatePart("h", StartDate), "00") + "-" + Format(DatePart("d", EndDate), "00") + Format(DatePart("h", EndDate), "00") + ".txt"
        txtSaveName.Text = TempFilename$
        txtStatus.Text = "Waiting ..."
TryAgain2:
        TimeOutFlag = False
        If chkEnableInet Then InFile$ = Inet1.OpenURL(URL$)
        'If Mid$(InFile$, 22, 9) = "Can't get" Then
        If Len(InFile$) < 1500 Then
          txtStatus.Text = "No soundings were available!"
          GoTo NextSite
        End If
        If TimeOutFlag Then GoTo TryAgain2
'        InFile$ = fLFtoCRLF$(InFile$)  'Make sure there is a CRLF on each line
        ' The above call not needed as files are read to next LF
        ' Also this code is EXTREMELY slow
        OUTlu2 = FreeFile()
        Open txtSavePath.Text + TempFilename$ For Output As #OUTlu2
        Write #OUTlu2, InFile$
        Close #OUTlu2
        txtStatus.Text = "File Saved!"
        
        INPlu2 = FreeFile
        Open txtSavePath.Text + TempFilename$ For Input As #INPlu2
        Do        'Go to first "<H2>" to get UT hour and day
          First$ = fReadLF(INPlu2)
        Loop Until Mid$(First$, 1, 4) = "<H2>"
        Do                                   '12345678901234567890
'       <H2>72518 ALB Albany Observations at 00Z 01 Jan 2002</H2>
          A$ = Right$(First$, 20)
          OutFilename$ = cmbStation.Text + Format(DatePart("yyyy", StartDate), "0000") + Format$(DatePart("m", StartDate), "00")
          OutFilename$ = OutFilename$ + Mid$(A$, 5, 2) + Mid$(A$, 1, 2) + ".txt"
          txtSaveName.Text = OutFilename$
          OUTlu2 = FreeFile
          Open txtSavePath.Text + OutFilename$ For Output As OUTlu2
          Print #OUTlu2, """<HTML>"
          Print #OUTlu2, First$
          Do
            A$ = fReadLF(INPlu2)
'          Debug.Print a$
            If Mid$(A$, 1, 4) = "<H2>" Then Exit Do
            Print #OUTlu2, A$
          Loop Until EOF(INPlu2)
          Close OUTlu2
          First$ = A$
          If chkAutoSave.Value = 1 Then cmdButton_Click (1)  'Save file
        Loop Until EOF(INPlu2)
        Close INPlu2
        If chkDeleteBatchFile.Value = 1 Then Kill txtSavePath.Text + TempFilename$
        
      End If
NextSite:
    Next i
    txtStatus.Text = "Done!"
    If chkRTsave.Value = 1 Then Close OUTlu
  
  Case 1  'Save file
    If chkSingleSoundings.Value = 1 Then  'If soundings are saved one at a time, save file
      lu = FreeFile()
      Open txtSavePath.Text + txtSaveName.Text For Output As #lu
      Write #lu, InFile$
      Close #lu
      txtStatus.Text = "File Saved!"
    End If
    INPlu = FreeFile
    Open txtSavePath.Text + txtSaveName.Text For Input As #INPlu
    If LOF(INPlu) > 3000 Then
      SourceFormat = 1 'Wyoming Sondes only for now
      Call ConvertRAOB(INPlu, SourceFormat, frmRAOBget)
      If Nlevr > 0 And chkRTsave.Value = 1 Then
        OutFileRecord = OutFileRecord + 1
        Call RAOBwrite(OUTlu, OutFileRecord)
      End If
    End If
    Close INPlu
    
    If chkPlotRAOB.Value = 1 Then
      DetermineROABtoShow
      frmRAOB.Zt1 = Ztrop
      frmRAOB.PlotT0 = True
      If chkShowTropopause.Value = 1 Then frmRAOB.ShowTropopause = True Else frmRAOB.ShowTropopause = False
      If chkShowPressureAltitude.Value = 1 Then frmRAOB.ShowPressureAltitude = True Else frmRAOB.ShowPressureAltitude = False
      frmRAOB.RAOBupdate
      DoEvents
    End If

  Case 2
    mnuFileExit_Click
  
  Case 3  'Clear Station List
    cmbStation.Clear

  End Select

  Exit Sub

ErrorHandler:
  Select Case Err
  Case 35761
    ErrNumber = Err
    TimeOutFlag = True
    Resume Next
  End Select
  
End Sub
Sub DetermineROABtoShow()
' Always start with original RAOB, then add requested editting
  If chkShowPressureAltitude.Value = 1 Then
    For i% = 1 To Nlevr
      Zplot(i%) = ZPraob(i%)
      Tplot(i%) = TZraob(i%)
    Next i
    Zt1 = Ztrop
    TT1 = Ttrop
    If Ltrop > 0 Then LT1 = Ltrop Else LT1 = 1
    Zt2 = Ztrop2
    TT2 = Ttrop2
    Lt2 = 1
  
  Else  'Use Geopotential Height
    For i% = 1 To Nlevr
      Zplot(i%) = ZGraob(i%)
      Tplot(i%) = TZraob(i%)
    Next i
    TT1 = Ttrop
    If Ltrop > 0 Then LT1 = Ltrop Else LT1 = 1
    Zt1 = ZGraob(LT1)
    TT2 = Ttrop2
    If Ltrop > 0 Then LT1 = Ltrop Else LT1 = 1
    Zt2 = ZGraob(LT1)
  End If
'  If UseEditLevel And EditLevel Then   'Edit levels
'    For i% = 1 To Nedit
'      Tplot(LZraob(i%)) = TZraob(Nlevr + i)
'    Next i%
'  End If

End Sub


Private Sub cmdStartConversion_Click()
Dim i%, ans, A$
If InputMode <> 3 Then
' Make sure a source format has been selected
  If SourceFormat < 0 Then
    Call MsgBox("You must select a Source Format option!", vbOKOnly, "Warning")
    Exit Sub
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
  Open txtConvertOut.Text For Random Access Read Write As OUTlu% Len = Len(RAOB)
  OutFileRecords = LOF(OUTlu%) \ Len(RAOB)

  If optOutput(1).Value Then    ' Append to existing file
    OutFileRecord = OutFileRecords
  Else                          ' Overwrite file if it exists
    OutFileRecord = 0
  End If
End If

Select Case InputMode
Case 0   ' Do entire directory
  RAOBtype = 1
  ConvertInFile$ = Dir(txtPath.Text + txtFileMask.Text, vbNormal)
  Do
    txtConvertIn.Text = txtPath.Text + ConvertInFile$
    DoEvents
    INPlu = FreeFile
    Open txtPath.Text + ConvertInFile$ For Input As INPlu
    If LOF(INPlu) > 3000 Then
      Call ConvertRAOB(INPlu, SourceFormat, frmRAOBget)
      If Nlevr > 0 Then
        OutFileRecord = OutFileRecord + 1
        Call RAOBwrite(OUTlu, OutFileRecord)
      End If
    End If
    Close INPlu
    ConvertInFile$ = Dir
    'Debug.Print ConvertInFile$
  Loop Until ConvertInFile$ = ""
  
Case 1                       ' Do single file
  OutFileRecord = OutFileRecord + 1
  INPlu = FreeFile
  Open txtConvertIn.Text For Input As INPlu
  Call ConvertRAOB(INPlu, SourceFormat, frmRAOBget)
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
  RAOBtype = 1
  INPlu = FreeFile
  Open txtConvertIn.Text For Input As INPlu
  Do
    Call ConvertRAOB(INPlu, SourceFormat, frmRAOBget)
    If Not EOF(INPlu) Then
      OutFileRecord = OutFileRecord + 1
      Call RAOBwrite(OUTlu, OutFileRecord)
      txtProgress.Text = Str$(OutFileRecord)
      DoEvents
    End If
  Loop Until EOF(INPlu)
  Close INPlu
Case 3  ' Sequential Makky Conversion
    Call ConvertRAOB(INPlu, SourceFormat, frmRAOBget)
End Select
Close OUTlu

End Sub

Private Sub Combo1_Change()

End Sub

Private Sub Form_Load()
Dim i%, j%, FQFN$

  ReadSETUP ("SYSTEM")
  FQFN$ = "C:\MTP\Setup\FLTINFO.REF"
  REFreadHEADER FQFN$
  PgmDrive$ = ProgramDrive$
  Drive$ = DataDrive$
  nRAOBlevels = 150

  With frmRAOBget
  .Top = 0
  .Left = 0
  .Height = 6150
  End With

  Load frmRAOBsites
  With frmRAOBsites
  .Top = frmRAOBget.Height
  .Left = frmRAOBget.Left
  .Visible = False
  End With

  Set frmRAOB.ExtForm = frmRAOBget

  txtSavePath.Text = Drive$ + "\MTP\Data\" + Platform$ + "\" + Mission$ + "\RAOB\"

  chkPlotRAOB.Value = 1
  chkShowTropopause.Value = 1
  chkRTsave.Value = 1
  chkAutoSave.Value = 1
  chkEnableInet.Value = 1
  chkShowPressureAltitude.Value = 1
  chkGeoPotHeight(0).Value = 1

'  ReadSETUP "SYSTEM"
'  REFreadFLTINFO "C:\MTP\FLTINFO.REF"

' Synch Drive List Box to default drive
  DriveList$ = Left$(Drive1.Drive, 2)
  If UCase$(DriveList$) <> UCase$(DataDrive$) Then
    For i = 0 To Drive1.ListCount - 1
      If UCase$(Left$(Drive1.List(i), 1)) = UCase$(Left$(DataDrive$, 1)) Then
'      Drive1.Drive = Drive1.List(i)
        Exit For
      End If
    Next i
  End If
  txtPath.Text = Left$(Drive1.Drive, 2) + Rdir$ + Mission$ + "\RAOB\"
  txtConvertOut.Text = Left$(Drive1.Drive, 2) + Rdir$ + Mission$ + "\RAOB\" + Mission$ + ".RAOB"

  optOutput(1).Value = True

  With cmbSrcFormat
  .AddItem "FSL"
  .AddItem "Wyoming"
  .AddItem "BLG"
  .AddItem "SJK"
  .AddItem "Pfister"
  .AddItem "EDW"
  .AddItem "TTY"
  .AddItem "Makky"
  .ListIndex = 1
  End With

  With cmbInputMode
  .AddItem "All files satisfying mask"
  .AddItem "Single file with one RAOB"
  .AddItem "Single file with many RAOBs"
  .AddItem "Makky Files Sequential"
  .ListIndex = 0
  End With
  
  With cmbWyoming
  .AddItem "United States"
  .AddItem "South America"
  .AddItem "Antarctica"
  .AddItem "Arctic"
  .AddItem "Central Pacific"
  .AddItem "Australia & NewZealand"
  .AddItem "Europe"
  .AddItem "Canada"
  .AddItem "Southeast Asia"
  .AddItem "Mexico & Carribean"
  .ListIndex = 0
  End With
  
  With cmbMissions
  .AddItem "PEMTB"
  .AddItem "SONEX"
  .AddItem "ACCENT"
  .AddItem "Southern CA"
  .ListIndex = 3
  End With

' Hour Step
  With cmbHour(2)
  .AddItem "3"
  .AddItem "6"
  .AddItem "12"
  .AddItem "24"
  .ListIndex = 2
  End With
  
' Hour
  For j = 0 To 1
  cmbHour(j).AddItem "0"
  cmbHour(j).AddItem "3"
  cmbHour(j).AddItem "6"
  cmbHour(j).AddItem "9"
  cmbHour(j).AddItem "12"
  cmbHour(j).AddItem "15"
  cmbHour(j).AddItem "18"
  cmbHour(j).AddItem "21"
  Next j
  cmbHour(0).ListIndex = 0

'Day
  For i = 1 To 31
    For j = 0 To 1
      cmbDay(j).AddItem Str$(i)
    Next j
  Next i
  cmbDay(0).ListIndex = 0

'Month
  For j = 0 To 1
  cmbMonth(j).AddItem "JAN"
  cmbMonth(j).AddItem "FEB"
  cmbMonth(j).AddItem "MAR"
  cmbMonth(j).AddItem "APR"
  cmbMonth(j).AddItem "MAY"
  cmbMonth(j).AddItem "JUN"
  cmbMonth(j).AddItem "JUL"
  cmbMonth(j).AddItem "AUG"
  cmbMonth(j).AddItem "SEP"
  cmbMonth(j).AddItem "OCT"
  cmbMonth(j).AddItem "NOV"
  cmbMonth(j).AddItem "DEC"
  Next j
  cmbMonth(0).ListIndex = 0

' Year
  For j = 0 To 1
    For i = Val(Right$(Date, 4)) To 1996 Step -1
      cmbYear(j).AddItem Format(i, "0000")
    Next i
  Next j
  cmbYear(0).ListIndex = 0

  With cboDelay
  .AddItem "0"
  .AddItem "2"
  .AddItem "5"
  .AddItem "10"
  .AddItem "15"
  .AddItem "20"
  .AddItem "30"
  .AddItem "60"
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

End Sub



Private Sub lbList_Click(Index As Integer)
Dim X$, Record&

SelectIndex = Index
X$ = lbList(Index).List(lbList(Index).ListIndex)
txtList(Index).Text = X$
If Index = 0 Then txtList(1).Text = "" Else txtList(0).Text = ""
If chkDetails.Value = 1 Then
  With frmRAOBsites
    .Visible = True
    .optShow(AddIndex).Value = True
    .txtShow(AddIndex).Text = lbList(AddIndex).List(lbList(AddIndex).ListIndex)
    .ShowDetails
  End With
End If
End Sub

Private Sub mnuFileExit_Click()
  
  Unload Me
  Unload frmFormatRAOB
  Unload frmRAOB
  Unload frmRAOBsites
  Unload frmAbout
  
End Sub


Private Sub mnuHelpAbout_Click()
  Load frmAbout
  'Set defaults
  frmAbout.lblApplication.Caption = "Internet RAOBget"
  frmAbout.lblHeading.Caption = "MJM Software"
  frmAbout.lblCopyright.Caption = "Copyright © 1999"
  frmAbout.Show
End Sub


Private Sub mnuOptionsFormat_Click()

CenterChildForm frmRAOB, frmFormatRAOB
frmFormatRAOB.Visible = True

End Sub

Private Sub tmrWait_Timer()
tmrWait.Enabled = False
End Sub

Private Sub txtList_Change(Index As Integer)
AddIndex = Index
End Sub

Private Sub txtStation_Change(Index As Integer)
DetailsIndex = Index
End Sub


Private Sub txtStation_Click(Index As Integer)
DetailsIndex = Index
End Sub


Private Sub txtList_KeyPress(Index As Integer, KeyAscii As Integer)
  
  If KeyAscii = 13 Then cmdSelect_Click (3) 'Add to list if CR detected

End Sub


