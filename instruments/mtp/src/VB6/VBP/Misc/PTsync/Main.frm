VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Karen's Time Sync"
   ClientHeight    =   6960
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   9210
   Icon            =   "Main.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6960
   ScaleWidth      =   9210
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picAccess 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      HasDC           =   0   'False
      Height          =   240
      Left            =   8940
      Picture         =   "Main.frx":212A
      ScaleHeight     =   240
      ScaleWidth      =   240
      TabIndex        =   80
      Tag             =   "ACCESS"
      Top             =   60
      Width           =   240
   End
   Begin VB.CommandButton btnEdit 
      Caption         =   "&Edit Servers"
      Height          =   375
      Left            =   120
      TabIndex        =   51
      Top             =   6240
      Width           =   1575
   End
   Begin VB.Timer timAuto 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   2760
      Top             =   6240
   End
   Begin VB.Timer timOutServer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   4320
      Top             =   6240
   End
   Begin MSWinsockLib.Winsock sckDaytimeTcp 
      Index           =   0
      Left            =   5400
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      LocalPort       =   13
   End
   Begin MSWinsockLib.Winsock sckTimeTcp 
      Index           =   0
      Left            =   4800
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      LocalPort       =   37
   End
   Begin MSWinsockLib.Winsock sckSntp 
      Left            =   4080
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      LocalPort       =   123
   End
   Begin VB.Timer timOutClient 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   3720
      Top             =   6240
   End
   Begin VB.Timer timZone 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   3120
      Top             =   6240
   End
   Begin VB.CommandButton btnHelp 
      Caption         =   "&Help ..."
      Height          =   375
      Left            =   120
      TabIndex        =   55
      Top             =   6240
      Width           =   1335
   End
   Begin VB.CommandButton btnAbout 
      Caption         =   "&About ..."
      Height          =   375
      Left            =   6240
      TabIndex        =   52
      ToolTipText     =   "Tell me about this program"
      Top             =   6240
      Width           =   1335
   End
   Begin MSComctlLib.StatusBar sbMain 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   54
      Top             =   6705
      Width           =   9210
      _ExtentX        =   16245
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   4
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   12444
            MinWidth        =   1940
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Alignment       =   1
            AutoSize        =   2
            Object.Width           =   159
            MinWidth        =   18
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   6
            Alignment       =   1
            AutoSize        =   2
            Object.Width           =   1931
            MinWidth        =   1941
            TextSave        =   "9/22/2004"
         EndProperty
         BeginProperty Panel4 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            Alignment       =   1
            AutoSize        =   2
            Object.Width           =   1588
            MinWidth        =   1587
            TextSave        =   "11:35 AM"
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton btnExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   7680
      TabIndex        =   53
      ToolTipText     =   "Exit program, Saving All Changes to Current and Stored Time Zones"
      Top             =   6240
      Width           =   1335
   End
   Begin MSWinsockLib.Winsock sckClient 
      Index           =   0
      Left            =   3480
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin MSWinsockLib.Winsock sckTimeUdp 
      Left            =   4920
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      LocalPort       =   37
   End
   Begin MSWinsockLib.Winsock sckDaytimeUdp 
      Left            =   5640
      Top             =   6240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      LocalPort       =   13
   End
   Begin TabDlg.SSTab tabSync 
      Height          =   6015
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   8925
      _ExtentX        =   15743
      _ExtentY        =   10610
      _Version        =   393216
      Tabs            =   5
      Tab             =   4
      TabsPerRow      =   6
      TabHeight       =   556
      TabCaption(0)   =   "  &Welcome  "
      TabPicture(0)   =   "Main.frx":26B4
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "picWelcome"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "  &Auto Sync  "
      TabPicture(1)   =   "Main.frx":26D0
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "picAuto"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).ControlCount=   1
      TabCaption(2)   =   "  &Manual Sync  "
      TabPicture(2)   =   "Main.frx":26EC
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "picClient"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).ControlCount=   1
      TabCaption(3)   =   "  &Time Server  "
      TabPicture(3)   =   "Main.frx":2708
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "picServer"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).ControlCount=   1
      TabCaption(4)   =   "  &Settings "
      TabPicture(4)   =   "Main.frx":2724
      Tab(4).ControlEnabled=   -1  'True
      Tab(4).Control(0)=   "Picture3"
      Tab(4).Control(0).Enabled=   0   'False
      Tab(4).ControlCount=   1
      Begin VB.PictureBox Picture3 
         BorderStyle     =   0  'None
         HasDC           =   0   'False
         Height          =   5415
         Left            =   120
         ScaleHeight     =   5415
         ScaleWidth      =   8655
         TabIndex        =   63
         TabStop         =   0   'False
         Top             =   480
         Width           =   8655
         Begin VB.Frame Frame3 
            Caption         =   "&Other Settings: "
            Height          =   1455
            Left            =   120
            TabIndex        =   50
            Top             =   2280
            Width           =   8415
            Begin VB.PictureBox Picture5 
               BorderStyle     =   0  'None
               HasDC           =   0   'False
               Height          =   1095
               Left            =   120
               ScaleHeight     =   1095
               ScaleWidth      =   8175
               TabIndex        =   71
               TabStop         =   0   'False
               Top             =   240
               Width           =   8175
               Begin VB.CheckBox chkSysTray 
                  Caption         =   "Display an Icon in the System &Tray when Minimized"
                  Height          =   255
                  Left            =   120
                  TabIndex        =   73
                  Top             =   600
                  Width           =   6015
               End
               Begin VB.CheckBox chkAutoSync 
                  Caption         =   "A&utomatically Sync My Clock when This Program Starts"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   120
                  TabIndex        =   72
                  Top             =   120
                  Width           =   6015
               End
            End
         End
         Begin VB.Frame fraStartup 
            Caption         =   "Auto&matic Launch Settings: "
            Height          =   2055
            Left            =   120
            TabIndex        =   49
            Top             =   0
            Width           =   8415
            Begin VB.PictureBox Picture4 
               BorderStyle     =   0  'None
               HasDC           =   0   'False
               Height          =   1695
               Left            =   120
               ScaleHeight     =   1695
               ScaleWidth      =   8175
               TabIndex        =   64
               TabStop         =   0   'False
               Top             =   240
               Width           =   8175
               Begin VB.CommandButton btnStartupApply 
                  Caption         =   "&Apply"
                  Enabled         =   0   'False
                  Height          =   375
                  Left            =   7080
                  TabIndex        =   67
                  Top             =   0
                  Width           =   1095
               End
               Begin VB.CheckBox chkStartup 
                  Caption         =   "&Launch This Program When Windows Starts"
                  Height          =   255
                  Left            =   120
                  TabIndex        =   70
                  Top             =   120
                  Width           =   5775
               End
               Begin VB.OptionButton optStartupUser 
                  Caption         =   "Only When the &Current User (%) Logs On"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   360
                  TabIndex        =   69
                  Top             =   420
                  Width           =   7815
               End
               Begin VB.OptionButton optStartupCommon 
                  Caption         =   "When An&y User Logs On"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   360
                  TabIndex        =   68
                  Top             =   660
                  Width           =   6255
               End
               Begin VB.CheckBox chkStartupSync 
                  Caption         =   "Sync &My Clock When This Program is Automatically Launched"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   360
                  TabIndex        =   66
                  Top             =   1080
                  Width           =   7815
               End
               Begin VB.CheckBox chkStartupExit 
                  Caption         =   "Then Exit This &Program"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   720
                  TabIndex        =   65
                  Top             =   1380
                  Width           =   4935
               End
            End
         End
      End
      Begin VB.PictureBox picServer 
         BorderStyle     =   0  'None
         HasDC           =   0   'False
         Height          =   5415
         Left            =   -74880
         ScaleHeight     =   5415
         ScaleWidth      =   8655
         TabIndex        =   61
         TabStop         =   0   'False
         Top             =   480
         Width           =   8655
         Begin VB.Frame fraServer 
            Caption         =   "Server Protocols: "
            Height          =   1575
            Left            =   120
            TabIndex        =   38
            Top             =   0
            Width           =   8415
            Begin VB.PictureBox Picture1 
               BorderStyle     =   0  'None
               HasDC           =   0   'False
               Height          =   1215
               Left            =   5280
               ScaleHeight     =   1215
               ScaleWidth      =   3015
               TabIndex        =   62
               TabStop         =   0   'False
               Top             =   240
               Width           =   3015
               Begin VB.OptionButton optDaytimeSMTP 
                  Caption         =   "Send SNTP &Format"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   240
                  TabIndex        =   46
                  Top             =   900
                  Width           =   2535
               End
               Begin VB.OptionButton optDaytimeKaren 
                  Caption         =   "Send ""&Karen's"" NIST Format"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   240
                  TabIndex        =   45
                  Top             =   660
                  Width           =   2775
               End
               Begin VB.OptionButton optDaytimeNIST 
                  Caption         =   "Send &NIST Format"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   240
                  TabIndex        =   44
                  Top             =   420
                  Value           =   -1  'True
                  Width           =   2535
               End
               Begin VB.CheckBox chkServerDaytime 
                  Caption         =   "Enable &Daytime Protocol"
                  Height          =   255
                  Left            =   0
                  TabIndex        =   43
                  Top             =   120
                  Width           =   2535
               End
            End
            Begin VB.CheckBox chkServerSntp 
               Caption         =   "Enable &SNTP Protocol"
               Height          =   255
               Left            =   240
               TabIndex        =   39
               Top             =   360
               Width           =   2175
            End
            Begin VB.CheckBox chkServerTime 
               Caption         =   "Enable T&ime Protocol"
               Height          =   255
               Left            =   2760
               TabIndex        =   41
               Top             =   360
               Width           =   2175
            End
            Begin VB.Label Label5 
               BackStyle       =   0  'Transparent
               BorderStyle     =   1  'Fixed Single
               Height          =   975
               Left            =   2520
               TabIndex        =   40
               Top             =   360
               Width           =   45
            End
            Begin VB.Label Label6 
               BackStyle       =   0  'Transparent
               BorderStyle     =   1  'Fixed Single
               Height          =   975
               Left            =   5040
               TabIndex        =   42
               Top             =   360
               Width           =   45
            End
         End
         Begin MSComctlLib.ListView lvServerLog 
            Height          =   3255
            Left            =   120
            TabIndex        =   48
            Top             =   2040
            Width           =   8415
            _ExtentX        =   14843
            _ExtentY        =   5741
            View            =   3
            LabelEdit       =   1
            LabelWrap       =   -1  'True
            HideSelection   =   0   'False
            FullRowSelect   =   -1  'True
            _Version        =   393217
            ForeColor       =   -2147483640
            BackColor       =   -2147483643
            Appearance      =   1
            NumItems        =   4
            BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Object.Width           =   0
            EndProperty
            BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   1
               Text            =   "Event Time"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   2
               Text            =   "Protocol"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               SubItemIndex    =   3
               Text            =   "Details"
               Object.Width           =   18
            EndProperty
         End
         Begin VB.Label Label2 
            BackStyle       =   0  'Transparent
            Caption         =   "&Activity: "
            Height          =   255
            Left            =   120
            TabIndex        =   47
            Top             =   1800
            Width           =   2655
         End
      End
      Begin VB.PictureBox picClient 
         Appearance      =   0  'Flat
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         HasDC           =   0   'False
         Height          =   5415
         Left            =   -74880
         MousePointer    =   1  'Arrow
         ScaleHeight     =   5415
         ScaleWidth      =   8655
         TabIndex        =   60
         TabStop         =   0   'False
         Top             =   480
         Width           =   8655
         Begin VB.Frame Frame2 
            Caption         =   "Manually Sync Now "
            Height          =   1215
            Left            =   120
            TabIndex        =   35
            Top             =   2400
            Width           =   8415
            Begin VB.PictureBox Picture6 
               BorderStyle     =   0  'None
               HasDC           =   0   'False
               Height          =   855
               Left            =   120
               ScaleHeight     =   855
               ScaleWidth      =   8175
               TabIndex        =   74
               TabStop         =   0   'False
               Top             =   240
               Width           =   8175
               Begin VB.CheckBox chkClientTest 
                  Caption         =   "Test Only (do not adjust my clock)"
                  Height          =   255
                  Left            =   0
                  TabIndex        =   79
                  Top             =   600
                  Value           =   1  'Checked
                  Width           =   3135
               End
               Begin VB.CommandButton btnClientSntp 
                  Caption         =   "Using &SNTP Protocol"
                  Default         =   -1  'True
                  Enabled         =   0   'False
                  BeginProperty Font 
                     Name            =   "MS Sans Serif"
                     Size            =   8.25
                     Charset         =   0
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  Height          =   360
                  Left            =   0
                  TabIndex        =   78
                  ToolTipText     =   "Make the currently selected zone the active time zone"
                  Top             =   120
                  Width           =   2175
               End
               Begin VB.CommandButton btnClientDaytime 
                  Caption         =   "Using &Daytime Protocol"
                  Enabled         =   0   'False
                  Height          =   375
                  Left            =   4560
                  TabIndex        =   77
                  Top             =   120
                  Width           =   2175
               End
               Begin VB.CommandButton btnClientTime 
                  Caption         =   "Using &Time Protocol"
                  Enabled         =   0   'False
                  Height          =   375
                  Left            =   2280
                  TabIndex        =   76
                  Top             =   120
                  Width           =   2175
               End
               Begin VB.CommandButton btnClientCancel 
                  Cancel          =   -1  'True
                  Caption         =   "&Cancel"
                  Enabled         =   0   'False
                  Height          =   375
                  Left            =   6960
                  TabIndex        =   75
                  Top             =   120
                  Width           =   1215
               End
            End
         End
         Begin VB.PictureBox picSplitClient 
            BorderStyle     =   0  'None
            HasDC           =   0   'False
            Height          =   1215
            Left            =   4200
            MousePointer    =   9  'Size W E
            ScaleHeight     =   1215
            ScaleWidth      =   135
            TabIndex        =   33
            ToolTipText     =   "Click and drag here to resize the two panes on either side."
            Top             =   1080
            Width           =   135
         End
         Begin VB.Frame fraClient 
            Caption         =   "Only Show Servers that Support: "
            Height          =   735
            Left            =   120
            TabIndex        =   25
            Top             =   0
            Width           =   8415
            Begin VB.CheckBox chkSntp 
               Caption         =   "SNTP Protocol"
               Height          =   255
               Left            =   360
               TabIndex        =   26
               Top             =   360
               Value           =   1  'Checked
               Width           =   1815
            End
            Begin VB.CheckBox chkTime 
               Caption         =   "Time Protocol"
               Height          =   255
               Left            =   2400
               TabIndex        =   27
               Top             =   360
               Value           =   1  'Checked
               Width           =   1815
            End
            Begin VB.CheckBox chkDaytime 
               Caption         =   "Daytime Protocol"
               Height          =   255
               Left            =   4320
               TabIndex        =   28
               Top             =   360
               Value           =   1  'Checked
               Width           =   2055
            End
            Begin VB.Label lblServers 
               Alignment       =   2  'Center
               BackStyle       =   0  'Transparent
               BeginProperty Font 
                  Name            =   "MS Sans Serif"
                  Size            =   9.75
                  Charset         =   0
                  Weight          =   400
                  Underline       =   0   'False
                  Italic          =   0   'False
                  Strikethrough   =   0   'False
               EndProperty
               Height          =   255
               Left            =   6720
               TabIndex        =   30
               Top             =   360
               Width           =   1455
            End
            Begin VB.Label Label1 
               BackStyle       =   0  'Transparent
               BorderStyle     =   1  'Fixed Single
               Height          =   375
               Left            =   6480
               TabIndex        =   29
               Top             =   240
               Width           =   45
            End
         End
         Begin MSComctlLib.ListView lvClientLog 
            Height          =   1335
            Left            =   120
            TabIndex        =   37
            Top             =   3960
            Width           =   8415
            _ExtentX        =   14843
            _ExtentY        =   2355
            View            =   3
            LabelEdit       =   1
            LabelWrap       =   -1  'True
            HideSelection   =   0   'False
            FullRowSelect   =   -1  'True
            _Version        =   393217
            ForeColor       =   -2147483640
            BackColor       =   -2147483643
            Appearance      =   1
            NumItems        =   4
            BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Object.Width           =   0
            EndProperty
            BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   1
               Text            =   "Event Time"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   2
               Text            =   "Proto"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               SubItemIndex    =   3
               Text            =   "Details"
               Object.Width           =   18
            EndProperty
         End
         Begin MSComctlLib.TreeView tvClient 
            Height          =   1215
            Left            =   120
            TabIndex        =   32
            Top             =   1080
            Width           =   4155
            _ExtentX        =   7329
            _ExtentY        =   2143
            _Version        =   393217
            HideSelection   =   0   'False
            Indentation     =   529
            LabelEdit       =   1
            LineStyle       =   1
            Sorted          =   -1  'True
            Style           =   6
            Appearance      =   1
            MousePointer    =   1
         End
         Begin MSComctlLib.ListView lvClient 
            Height          =   1215
            Left            =   4320
            TabIndex        =   34
            Top             =   1080
            Width           =   4215
            _ExtentX        =   7435
            _ExtentY        =   2143
            View            =   3
            LabelEdit       =   1
            LabelWrap       =   -1  'True
            HideSelection   =   0   'False
            HideColumnHeaders=   -1  'True
            FullRowSelect   =   -1  'True
            _Version        =   393217
            ForeColor       =   -2147483640
            BackColor       =   -2147483643
            Appearance      =   1
            MousePointer    =   1
            NumItems        =   3
            BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Object.Width           =   0
            EndProperty
            BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   1
               SubItemIndex    =   1
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               SubItemIndex    =   2
               Object.Width           =   18
            EndProperty
         End
         Begin VB.Label Label3 
            BackStyle       =   0  'Transparent
            Caption         =   "&Select a Time Server Near Your Current Location:"
            Height          =   210
            Left            =   120
            TabIndex        =   31
            Top             =   840
            Width           =   4395
         End
         Begin VB.Label Label4 
            BackStyle       =   0  'Transparent
            Caption         =   "&Activity: "
            Height          =   255
            Left            =   120
            TabIndex        =   36
            Top             =   3720
            Width           =   1455
         End
      End
      Begin VB.PictureBox picAuto 
         BorderStyle     =   0  'None
         HasDC           =   0   'False
         Height          =   5415
         Left            =   -74880
         MousePointer    =   1  'Arrow
         ScaleHeight     =   5415
         ScaleWidth      =   8655
         TabIndex        =   58
         TabStop         =   0   'False
         Top             =   480
         Width           =   8655
         Begin VB.PictureBox picSplitAuto 
            BorderStyle     =   0  'None
            HasDC           =   0   'False
            Height          =   1575
            Left            =   4200
            MousePointer    =   9  'Size W E
            ScaleHeight     =   1575
            ScaleWidth      =   135
            TabIndex        =   16
            ToolTipText     =   "Click and drag here to resize the two panes on either side."
            Top             =   240
            Width           =   135
         End
         Begin VB.Frame Frame1 
            Caption         =   "Step &2: Select a Schedule: "
            Height          =   1215
            Left            =   120
            TabIndex        =   18
            Top             =   2040
            Width           =   8415
            Begin VB.PictureBox Picture2 
               BorderStyle     =   0  'None
               HasDC           =   0   'False
               Height          =   855
               Left            =   120
               ScaleHeight     =   855
               ScaleWidth      =   8175
               TabIndex        =   59
               TabStop         =   0   'False
               Top             =   240
               Width           =   8175
               Begin VB.CommandButton btnAutoSync 
                  Caption         =   "Sync &Now"
                  Enabled         =   0   'False
                  Height          =   375
                  Left            =   6960
                  TabIndex        =   22
                  Top             =   0
                  Width           =   1215
               End
               Begin VB.CheckBox chkAuto 
                  Caption         =   "Automatically Sync My &Clock Every:"
                  Enabled         =   0   'False
                  Height          =   255
                  Left            =   120
                  TabIndex        =   19
                  Top             =   120
                  Width           =   6735
               End
               Begin MSComCtl2.DTPicker dtAuto 
                  Height          =   315
                  Left            =   480
                  TabIndex        =   21
                  Top             =   360
                  Width           =   2655
                  _ExtentX        =   4683
                  _ExtentY        =   556
                  _Version        =   393216
                  Enabled         =   0   'False
                  BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                     Name            =   "MS Sans Serif"
                     Size            =   8.25
                     Charset         =   0
                     Weight          =   400
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  CustomFormat    =   "y 'days,' H 'hours,' m 'mins'"
                  Format          =   19660803
                  UpDown          =   -1  'True
                  CurrentDate     =   38107
               End
               Begin VB.Label lblAuto 
                  AutoSize        =   -1  'True
                  BackStyle       =   0  'Transparent
                  Caption         =   "(Next Auto Sync in %)"
                  Height          =   195
                  Left            =   3240
                  TabIndex        =   20
                  Top             =   360
                  Width           =   1530
               End
            End
         End
         Begin MSComctlLib.ListView lvAutoLog 
            Height          =   1575
            Left            =   120
            TabIndex        =   24
            Top             =   3720
            Width           =   8415
            _ExtentX        =   14843
            _ExtentY        =   2778
            View            =   3
            LabelEdit       =   1
            LabelWrap       =   -1  'True
            HideSelection   =   0   'False
            FullRowSelect   =   -1  'True
            _Version        =   393217
            ForeColor       =   -2147483640
            BackColor       =   -2147483643
            Appearance      =   1
            NumItems        =   4
            BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Object.Width           =   0
            EndProperty
            BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   1
               Text            =   "Event Time"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   2
               SubItemIndex    =   2
               Text            =   "Protocol"
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               SubItemIndex    =   3
               Text            =   "Details"
               Object.Width           =   18
            EndProperty
         End
         Begin MSComctlLib.TreeView tvAuto 
            Height          =   1575
            Left            =   120
            TabIndex        =   15
            Top             =   240
            Width           =   4155
            _ExtentX        =   7329
            _ExtentY        =   2778
            _Version        =   393217
            HideSelection   =   0   'False
            Indentation     =   529
            LabelEdit       =   1
            LineStyle       =   1
            Sorted          =   -1  'True
            Style           =   6
            Appearance      =   1
            MousePointer    =   1
         End
         Begin MSComctlLib.ListView lvAuto 
            Height          =   1575
            Left            =   4320
            TabIndex        =   17
            Top             =   240
            Width           =   4215
            _ExtentX        =   7435
            _ExtentY        =   2778
            View            =   3
            LabelEdit       =   1
            LabelWrap       =   -1  'True
            HideSelection   =   0   'False
            HideColumnHeaders=   -1  'True
            FullRowSelect   =   -1  'True
            _Version        =   393217
            ForeColor       =   -2147483640
            BackColor       =   -2147483643
            Appearance      =   1
            MousePointer    =   1
            NumItems        =   3
            BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Object.Width           =   0
            EndProperty
            BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               Alignment       =   1
               SubItemIndex    =   1
               Object.Width           =   18
            EndProperty
            BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
               SubItemIndex    =   2
               Object.Width           =   18
            EndProperty
         End
         Begin VB.Label Label8 
            BackStyle       =   0  'Transparent
            Caption         =   "Step &1: Select a Time Server Near Your Current Location:"
            Height          =   210
            Left            =   120
            TabIndex        =   14
            Top             =   0
            Width           =   4395
         End
         Begin VB.Label Label7 
            BackStyle       =   0  'Transparent
            Caption         =   "Activit&y: "
            Height          =   255
            Left            =   120
            TabIndex        =   23
            Top             =   3480
            Width           =   1455
         End
      End
      Begin VB.PictureBox picWelcome 
         BorderStyle     =   0  'None
         HasDC           =   0   'False
         Height          =   5415
         Left            =   -74880
         ScaleHeight     =   5415
         ScaleWidth      =   8655
         TabIndex        =   56
         TabStop         =   0   'False
         Top             =   480
         Width           =   8655
         Begin VB.Label Label20 
            BackStyle       =   0  'Transparent
            Caption         =   "Here's a short list of what the program can do. Click on a tab (above), or the words below, to view the corresponding settings:"
            Height          =   495
            Left            =   480
            TabIndex        =   3
            Top             =   1680
            Width           =   7695
         End
         Begin VB.Label Label19 
            BackStyle       =   0  'Transparent
            Caption         =   "Settings that control how and when this program is launched, etc."
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1800
            MouseIcon       =   "Main.frx":2740
            MousePointer    =   99  'Custom
            TabIndex        =   11
            Top             =   3240
            Width           =   6495
         End
         Begin VB.Label Label18 
            Alignment       =   1  'Right Justify
            BackStyle       =   0  'Transparent
            Caption         =   "&Settings:"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   255
            Left            =   120
            MouseIcon       =   "Main.frx":2892
            MousePointer    =   99  'Custom
            TabIndex        =   10
            Top             =   3240
            Width           =   1575
         End
         Begin VB.Label Label17 
            BackStyle       =   0  'Transparent
            Caption         =   $"Main.frx":29E4
            Height          =   855
            Left            =   480
            TabIndex        =   2
            Top             =   600
            Width           =   7695
         End
         Begin VB.Label Label16 
            BackStyle       =   0  'Transparent
            Caption         =   "Add, Change or Delete Time Servers available to this program"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1800
            MouseIcon       =   "Main.frx":2B26
            MousePointer    =   99  'Custom
            TabIndex        =   13
            Top             =   3840
            Width           =   6375
         End
         Begin VB.Label Label15 
            Alignment       =   1  'Right Justify
            BackStyle       =   0  'Transparent
            Caption         =   "&Edit Servers:"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   255
            Left            =   120
            MouseIcon       =   "Main.frx":2C78
            MousePointer    =   99  'Custom
            TabIndex        =   12
            Top             =   3840
            Width           =   1575
         End
         Begin VB.Label Label14 
            Alignment       =   1  'Right Justify
            BackStyle       =   0  'Transparent
            Caption         =   "&Time Server:"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   255
            Left            =   120
            MouseIcon       =   "Main.frx":2DCA
            MousePointer    =   99  'Custom
            TabIndex        =   8
            Top             =   3600
            Visible         =   0   'False
            Width           =   1575
         End
         Begin VB.Label Label13 
            Alignment       =   1  'Right Justify
            BackStyle       =   0  'Transparent
            Caption         =   "&Manual Sync:"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   255
            Left            =   120
            MouseIcon       =   "Main.frx":2F1C
            MousePointer    =   99  'Custom
            TabIndex        =   6
            Top             =   2760
            Width           =   1575
         End
         Begin VB.Label Label12 
            Alignment       =   1  'Right Justify
            BackStyle       =   0  'Transparent
            Caption         =   "&Auto Sync:"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   255
            Left            =   120
            MouseIcon       =   "Main.frx":306E
            MousePointer    =   99  'Custom
            TabIndex        =   4
            Top             =   2280
            Width           =   1575
         End
         Begin VB.Label Label11 
            BackStyle       =   0  'Transparent
            Caption         =   "Allow other computers to sync their clocks to this computer's clock"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1800
            MouseIcon       =   "Main.frx":31C0
            MousePointer    =   99  'Custom
            TabIndex        =   9
            Top             =   3600
            Visible         =   0   'False
            Width           =   6375
         End
         Begin VB.Label Label10 
            BackStyle       =   0  'Transparent
            Caption         =   "Manually sync this computer's clock to another computer's clock"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1800
            MouseIcon       =   "Main.frx":3312
            MousePointer    =   99  'Custom
            TabIndex        =   7
            Top             =   2760
            Width           =   6255
         End
         Begin VB.Label Label9 
            BackStyle       =   0  'Transparent
            Caption         =   "Automatically sync this computer's clock to another computer's clock"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   1800
            MouseIcon       =   "Main.frx":3464
            MousePointer    =   99  'Custom
            TabIndex        =   5
            Top             =   2280
            Width           =   6495
         End
         Begin VB.Label lblWelcome 
            BackStyle       =   0  'Transparent
            Caption         =   "Welcome to "
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   -1  'True
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000D&
            Height          =   495
            Left            =   120
            TabIndex        =   1
            Top             =   120
            Width           =   8415
         End
      End
   End
   Begin VB.Label lblAccess 
      BackStyle       =   0  'Transparent
      Caption         =   "&Z"
      ForeColor       =   &H8000000F&
      Height          =   255
      Left            =   8640
      TabIndex        =   57
      ToolTipText     =   "12"
      Top             =   0
      Width           =   255
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Copyright © 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/

Private Const FRA_TOP = 720
Private Const FRA_LEFT = 360

Private Const ARG_EXIT = "/Exit"
Private Const ARG_SYNC = "/Sync"

Private Const ALREADY_IN_USE = " - Already in Use by Another Program"
Private Const AUTO_DISABLED = "Disabled"
Private Const AUTO_NEXT = "(Next Auto Sync in %)"
Private Const AUTO_SCHED = "Automatically Sync my clock to ""%"" every:"

'Private Const AUTO_STARTUP_SCHED = "Automatically Sync my clock to ""%"" when this program starts"
Private Const AUTO_STARTUP_SCHED = "Automatically Sync my clock when this program starts"
Private Const AUTO_STARTUP_ENABLED = "Automatic Clock Adjustment at Startup ENABLED"
Private Const AUTO_STARTUP_DISABLED = "Automatic Clock Adjustment at Startup DISABLED"

'Private Const IDC_APPSTARTING = 32650&
'Private Const IDC_ARROW = 32512&
'Private Const IDC_CROSS = 32515&
'Private Const IDC_IBEAM = 32513&
'Private Const IDC_NO = 32648&
'Private Const IDC_SIZEALL = 32646&
'Private Const IDC_SIZENESW = 32643&
'Private Const IDC_SIZENS = 32645&
'Private Const IDC_SIZENWSE = 32642&
'Private Const IDC_SIZEWE = 32644&
'Private Const IDC_UPARROW = 32516&
'Private Const IDC_WAIT = 32514&

'Private Const OCR_CROSS = 32515
'Private Const OCR_IBEAM = 32513
'Private Const OCR_ICOCUR = 32647
'Private Const OCR_ICON = 32641
'Private Const OCR_NO = 32648
'Private Const OCR_NORMAL = 32512
'Private Const OCR_SIZE = 32640
'Private Const OCR_SIZEALL = 32646
'Private Const OCR_SIZENESW = 32643
'Private Const OCR_SIZENS = 32645
'Private Const OCR_SIZENWSE = 32642
'Private Const OCR_SIZEWE = 32644
'Private Const OCR_UP = 32516
'Private Const OCR_WAIT = 32514

'Private Const OBM_CHECK = 32760
'Private Const OBM_COMBO = 32738
'Private Const OBM_DNARROW = 32752
'Private Const OBM_DNARROWD = 32742
'Private Const OBM_DNARROWI = 32736
'Private Const OBM_ZOOM = 32748
'Private Const OBM_ZOOMD = 32745

'Private Const HTERROR = (-2)
'Private Const HTTRANSPARENT = (-1)
'Private Const HTNOWHERE = 0
'Private Const HTCLIENT = 1
'Private Const HTCAPTION = 2
'Private Const HTSYSMENU = 3
'Private Const HTGROWBOX = 4
'Private Const HTSIZE = HTGROWBOX
'Private Const HTMENU = 5
'Private Const HTHSCROLL = 6
'Private Const HTMINBUTTON = 8
'Private Const HTREDUCE = HTMINBUTTON
'Private Const HTVSCROLL = 7
'Private Const HTMAXBUTTON = 9
'Private Const HTZOOM = HTMAXBUTTON
'Private Const HTLEFT = 10
'Private Const HTRIGHT = 11
'Private Const HTTOP = 12
'Private Const HTTOPLEFT = 13
'Private Const HTTOPRIGHT = 14
'Private Const HTBOTTOM = 15
'Private Const HTBOTTOMLEFT = 16
'Private Const HTBOTTOMRIGHT = 17
'Private Const HTSIZEFIRST = HTLEFT
'Private Const HTSIZELAST = HTBOTTOMRIGHT
'Private Const HTBORDER = 18

Private Enum SHORTCUT_FIELD
    SHORTCUT_LNK_FILE
    SHORTCUT_EXE_FILE
    SHORTCUT_WORK_DIR
    SHORTCUT_DESC
    SHORTCUT_PATH
End Enum

Private Declare Function SendMessageA Lib "user32" ( _
    ByVal hWnd As Long, _
    ByVal wMsg As Long, _
    ByVal wParam As Long, _
    lParam As Any) As Long

Private WithEvents PTMe As PTForm
Attribute PTMe.VB_VarHelpID = -1

Private TimeOutCnt As Long
Private BegTrip As Variant
Private RoundTrip As Variant

Private SntpIn As SNTP_MESSAGE
Private ServerCnt As Long
Private ServerDaytimeFormat As DAYTIME_FORMAT

Private Status As Panel
Private Zoom As Panel
Private MaxHeight As Long
Private MinHeight As Long
Private LocalListItem As ListItem
Private PrevNow As Date
Private AutoStatus As Boolean
Private AutoStartupStatus As Boolean
Private AutoSecs As Long
Private AutoCnt As Long
Private SyncOnStartup As Boolean
Private SyncThenExit As Boolean
Private Function DispSomeServers(SelectedServer As TimeServer, tv As TreeView, lv As ListView, ShowSntp As CheckBox, ShowDaytime As CheckBox, ShowTime As CheckBox) As String
    Dim bSntp As Boolean
    Dim bTime As Boolean
    Dim bDaytime As Boolean

    If ShowSntp.Value = vbChecked Then
        bSntp = True
    Else
        bSntp = False
    End If

    If ShowTime.Value = vbChecked Then
        bTime = True
    Else
        bTime = False
    End If

    If ShowDaytime.Value = vbChecked Then
        bDaytime = True
    Else
        bDaytime = False
    End If

    DispServers SelectedServer, tv, lv, bSntp, bDaytime, bTime
    DispSomeServers = FormatNumber(lv.ListItems.Count, 0) & " Servers found"
End Function
Private Sub DispServers(SelectedServer As TimeServer, tv As TreeView, lv As ListView, Optional ShowSntp As Boolean = True, Optional ShowDaytime As Boolean = False, Optional ShowTime As Boolean = False)
    Dim i As Long
    Dim Root As MSComctlLib.Node
    Dim cn As MSComctlLib.Node
    Dim sn As MSComctlLib.Node
    Dim n As MSComctlLib.Node
    Dim ts As TimeServer
    Dim AddIt As Boolean
    Dim sel As MSComctlLib.Node
    Dim HostName As String
    Dim SelHost As String

    tv.Nodes.Clear
    lv.ListItems.Clear
    Set Root = tv.Nodes.Add(, tvwFirst, "ROOT", ROOT_DESC)
    Root.Sorted = True

    If SelectedServer Is Nothing Then
        SelHost = ""
    Else
        SelHost = SelectedServer.HostName
    End If

    On Error Resume Next
    For i = 0 To TsCnt - 1
        Set ts = Tss(i)
        AddIt = False
        If ts.Enabled Then
            If ShowSntp And ts.SntpSupported Then AddIt = True
            If ShowTime And ts.TimeSupported Then AddIt = True
            If ShowDaytime And ts.DaytimeSupported Then AddIt = True
            If AddIt Then
                Set cn = Nothing
                Set cn = tv.Nodes(KEY_COUNTRY & Tss(i).Country)
                If cn Is Nothing Then
                    Set cn = tv.Nodes.Add(Root, tvwChild, KEY_COUNTRY & Tss(i).Country, Tss(i).Country)
                    cn.Sorted = True
                End If
                If Len(Trim(Tss(i).State)) > 0 Then
                    Set sn = Nothing
                    Set sn = tv.Nodes(KEY_STATE & Tss(i).State)
                    If sn Is Nothing Then
                        Set sn = tv.Nodes.Add(cn, tvwChild, KEY_STATE & Tss(i).State, Tss(i).State)
                        sn.Sorted = True
                    End If
                Else
                    Set sn = cn
                End If
                HostName = Tss(i).HostName
                Set n = tv.Nodes.Add(sn, tvwChild, KEY_HOST & HostName, HostName)
                n.Tag = CStr(i)
                n.Sorted = True
                If StrComp(HostName, SelHost, vbTextCompare) = 0 Then
                    Set sel = n
                End If
            End If
        End If
    Next i

    Root.Expanded = True
    If sel Is Nothing Then
        Set tv.SelectedItem = Root
        Root.EnsureVisible
        Set SelectedServer = Nothing
    Else
        Set tv.SelectedItem = sel
        sel.EnsureVisible
    End If
    tv.MousePointer = ccArrow
End Sub
'Private Sub DispServer(LstItm As ListItem, ts As TimeServer)
'    LstItm.Checked = ts.Enabled
'    LstItm.Text = ts.CountryCode
'    LstItm.SubItems(COL_COUNTRY) = ts.Country
'    LstItm.SubItems(COL_STATE) = ts.State
'    LstItm.SubItems(COL_HOSTNAME) = ts.HostName
'    LstItm.SubItems(COL_LOCATION) = ts.Location
'    LstItm.SubItems(COL_COORDS) = ts.Coords
'    LstItm.SubItems(COL_SERVICEAREA) = ts.ServiceArea
'    LstItm.SubItems(COL_POLICY) = ts.Policy
'    LstItm.SubItems(COL_SYNC) = ts.SyncMethod
'    LstItm.SubItems(COL_CONFIG) = ts.Config
'    LstItm.SubItems(COL_CONTACT) = ts.Contact
'    LstItm.SubItems(COL_SNTP) = ts.SntpSupported
'    LstItm.SubItems(COL_STRATUM) = ts.Stratum
'    LstItm.SubItems(COL_TIME) = ts.TimeSupported
'    LstItm.SubItems(COL_DAYTIME) = ts.DaytimeSupported
'    LstItm.SubItems(COL_DAYTIME_FORMAT) = ts.DaytimeFormat
'    If ts.RoundTrip > 0 Then
'        LstItm.SubItems(COL_ROUNDTRIP) = ts.RoundTrip & " msecs"
'    Else
'        LstItm.SubItems(COL_ROUNDTRIP) = " "
'    End If
'    LstItm.SubItems(COL_ROUNDTRIP_SORT) = Format(ts.RoundTrip, "000000")
''    LstItm.SubItems(COL_ROUNDTRIP) = " "
''    LstItm.SubItems(COL_ROUNDTRIP_SORT) = Format(0, "000000")
'End Sub
'Private Sub DispServerDetails(ActivityCode As ACTIVITY_CODE, LstItm As ListItem)
'    ActivityLog ActivityCode, "Current Time Server: " & LstItm.SubItems(COL_HOSTNAME)
'    ActivityLog ActivityCode, "Location: " & LstItm.SubItems(COL_LOCATION)
'    ActivityLog ActivityCode, "Service Area: " & LstItm.SubItems(COL_SERVICEAREA)
'    ActivityLog ActivityCode, "Policy: " & LstItm.SubItems(COL_POLICY)
'End Sub
Private Sub btnAbout_Click()
    frmAbout.Show vbModal
End Sub
Private Function Picker2Secs(dt As DTPicker) As Long
    Dim secs As Long
    Dim d As Date
    Dim yy As Long

    d = dt.Value
    secs = 0
'    secs = Second(d)
    secs = secs + (Minute(d) * 60)
    secs = secs + (Hour(d) * SECS_PER_HOUR)
    yy = Year(d) Mod 100
    secs = secs + (yy * SECS_PER_DAY) ' yy is really days
    Picker2Secs = secs
End Function
Private Function Secs2Picker(TotalSecs As Long) As Date
    Dim d As Date
    Dim days As Variant
    Dim hrs As Long
    Dim mins As Long
    Dim secs As Long
    Dim temp As Variant

    If TotalSecs = 0 Then
        Secs2Picker = 0
    Else
        days = BigIntDiv(TotalSecs, SECS_PER_DAY)
        temp = BigMod(TotalSecs, SECS_PER_DAY)
        hrs = temp \ 3600
        temp = temp Mod 3600
        mins = temp \ 60
        secs = temp Mod 60
        Secs2Picker = DateSerial(2000 + days, 1, 1) + TimeSerial(hrs, mins, 0)
    End If
End Function
Private Sub AutoApply()
    timAuto.Enabled = False
    If AutoServer Is Nothing Then
        chkAuto.Enabled = False
        lblAuto.Caption = AUTO_DISABLED
        lblAuto.Enabled = False
        dtAuto.Enabled = False
        SetAutoStatus False
        chkAutoSync.Enabled = False
        SetAutoStartupStatus False
    Else
        btnAutoSync.Enabled = True
        chkAuto.Enabled = True
        chkAutoSync.Enabled = True
        If chkAutoSync.Value = vbChecked Then
            If AutoServer.HostName > " " Then
                SetAutoStartupStatus True
            Else
                SetAutoStartupStatus False
            End If
        Else
            SetAutoStartupStatus False
        End If
        If chkAuto.Value = vbChecked Then
            dtAuto.Enabled = True
            AutoSecs = Picker2Secs(dtAuto)
            AutoCnt = AutoSecs
            If (AutoSecs > 0) And (AutoServer.HostName > " ") Then
                lblAuto.Enabled = True
                DispNextAuto AutoSecs
                timAuto.Enabled = True
                SetAutoStatus True
            Else
                lblAuto.Caption = AUTO_DISABLED
                lblAuto.Enabled = False
                If AutoServer.HostName > " " Then
                    dtAuto.Enabled = True
                Else
                    dtAuto.Enabled = False
                End If
                SetAutoStatus False
            End If
        Else
            dtAuto.Enabled = False
            lblAuto.Caption = AUTO_DISABLED
            lblAuto.Enabled = False
'            AutoSecs = 0
            SetAutoStatus False
        End If
    End If
End Sub
Private Sub DispNextAuto(secs As Long)
    If secs > 0 Then
        lblAuto.Caption = Replace(AUTO_NEXT, "%", ApiDispDHMS(CDec(secs * CDec(1000)), SHORT_NORMAL))
        lblAuto.Enabled = True
    Else
        lblAuto.Caption = AUTO_DISABLED
        lblAuto.Enabled = False
    End If
End Sub
Private Sub btnClientDaytime_Click()
    Dim i As Long
    Dim sck As Winsock
    Dim Code As ACTIVITY_CODE

    If chkClientTest.Value = vbChecked Then
        Code = ACTIVITY_CLIENT_DAYTIME_TEST
    Else
        Code = ACTIVITY_CLIENT_DAYTIME_SYNC
    End If

    EnableSync Code
    ActivityClear Code
    i = CLng(tvClient.SelectedItem.Tag)
    Set sck = InitClientDaytime(Tss(i).HostName, Code)
    btnClientCancel.Tag = CLng(sck.Index)
End Sub
Private Function InitClientDaytime(HostName As String, ActivityCode As ACTIVITY_CODE) As Winsock
    Dim DQ As String
    Dim inx As Long

    DQ = SockHostDQ(HostName)
    If Len(DQ) <= 0 Then
        ActivityLog ActivityCode, "Could Not Resolve Host Name: " & HostName
        EnableSync ActivityCode
        ActivityAdjust ActivityCode
        Exit Function
    End If

    inx = sckClient.UBound + 1
    Load sckClient(inx)

    With sckClient(inx)
        .Close
        .Tag = JoinSckTag(TIMEOUT_VAL, ActivityCode, -1)
        .Protocol = sckTCPProtocol
        .RemotePort = PORT_DAYTIME ' Daytime port
        .RemoteHost = DQ ' HostName
        .Connect
    End With
    BegTrip = ApiTickStart()

    ActivityLog ActivityCode, "Connecting To " & HostName & " (" & DQ & ")"
    Set InitClientDaytime = sckClient(inx)
    timOutClient.Enabled = True
    ActivityAdjust ActivityCode
End Function
Private Sub btnEdit_Click()
    btnEdit.Enabled = False
    Status.Text = "Loading Server information ..."
    Me.Enabled = False
    Load frmEdit
    Status.Text = "Editing Server information ..."

    frmEdit.Show vbModal, Me

    If Not EditCancelled Then
        Status.Text = "Loading Updated Server information ..."
        DispServers AutoServer, tvAuto, lvAuto, True, False, False
        DispAutoServer AutoServer
        lblServers.Caption = DispSomeServers(ClientServer, tvClient, lvClient, chkSNTP, chkDaytime, chkTime)
        DispClientServer ClientServer
    End If

    btnEdit.Enabled = True
    Me.Enabled = True
    On Error Resume Next
    Me.SetFocus
    Status.Text = "Ready"
End Sub
Private Sub btnExit_Click()
    Unload Me
End Sub
Private Sub btnHelp_Click()
    ApiHelpTopic 1000
End Sub
Private Sub DispZone(tz As TimeZone, lv As ListView)

    Set LocalListItem = lv.ListItems.Add()
    LocalListItem.SubItems(1) = "Current Local Time:"
    PrevNow = 0
    timZone_Timer
    LvAdjust lv
    PrevNow = 0
    timZone.Enabled = True
End Sub
Private Sub btnClientSntp_Click()
    Dim sck As Winsock
    Dim Code As ACTIVITY_CODE

    If chkClientTest.Value = vbChecked Then
        Code = ACTIVITY_CLIENT_SNTP_TEST
    Else
        Code = ACTIVITY_CLIENT_SNTP_SYNC
    End If

    If ClientServer Is Nothing Then
        DisableSync Code
        Exit Sub
    End If

    EnableSync Code
    ActivityClear Code
    Set sck = InitClientSntp(ClientServer, Code)
    btnClientCancel.Tag = CLng(sck.Index)
End Sub
Private Function MonthNum(Name As String) As Long
    Dim mm As Long

    If IsNumeric(Name) Then
        MonthNum = CLng(Name)
        Exit Function
    End If

    Select Case LCase(Name)
        Case "jan", "january":      mm = 1
        Case "feb", "february":     mm = 2
        Case "mar", "march":        mm = 3
        Case "apr", "april":        mm = 4
        Case "may":                 mm = 5
        Case "jun", "june":         mm = 6
        Case "jul", "july":         mm = 7
        Case "aug", "august":       mm = 8
        Case "sep", "september":    mm = 9
        Case "oct", "october":      mm = 10
        Case "nov", "november":     mm = 11
        Case "dec", "december":     mm = 12
        Case Else:                  mm = -1
    End Select

    MonthNum = mm
End Function
Private Sub btnClientCancel_Click()
    Dim inx As Long
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    inx = CLng(btnClientCancel.Tag)
    SplitSckTag sckClient(inx).Tag, TimeOut, ActivityCode, ServerInx

    sckClient(inx).Close
    Unload sckClient(inx)
    DisableSync ActivityCode
End Sub
Private Sub btnAutoSync_Click()
    btnAutoSync.Enabled = False
    timAuto.Enabled = False
    InitClientSntp AutoServer, ACTIVITY_AUTO_SYNC
    AutoApply
End Sub
Private Function InitClientSntp(Server As TimeServer, ActivityCode As ACTIVITY_CODE) As Winsock
    Dim DQ As String
    Dim SntpOut(47) As Byte
    Dim inx As Long

    DQ = SockHostDQ(Server.HostName)
    If Len(DQ) <= 0 Then
        ActivityLog ActivityCode, "Could Not Resolve Host Name: " & Server.HostName
        EnableSync ActivityCode
        ActivityAdjust ActivityCode
        Exit Function
    End If
    If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
        ActiveTz.SntpServer = Server.HostName
    End If

    SntpOut(0) = &H1B
    inx = sckClient.UBound + 1
    Load sckClient(inx)

    With sckClient(inx)
        .Close
        .Tag = JoinSckTag(TIMEOUT_VAL, ActivityCode, Server.Index)
        .Protocol = sckUDPProtocol
        .RemotePort = PORT_SNTP  'S/NTP service port
        .RemoteHost = DQ ' HostName
        .Connect
        .SendData SntpOut
    End With
    BegTrip = ApiTickStart()

    ActivityLog ActivityCode, "Connecting To " & Server.HostName & " (" & DQ & ")"
    Set InitClientSntp = sckClient(inx)
    timOutClient.Enabled = True
    ActivityAdjust ActivityCode
End Function
Private Sub FiniClientSntp(ActivityCode As ACTIVITY_CODE, SntpIn As SNTP_MESSAGE, ServerInx As Long)
    Dim s As String
    Dim i As Long
    Dim CurSecs As Variant
    Dim TotalSecs As Variant
    Dim ts As SNTP_TIMESTAMP
    Dim WholeSecs As Variant
    Dim FracSecs As Variant
    Dim Stratum As Long
    Dim ost As SYSTEMTIME
    Dim nst As SYSTEMTIME
    Dim CurDate As Date
    Dim NewDate As Date
    Dim lv As ListView

    GetSystemTime ost
    ActivityLog ActivityCode, "Response Received in: " & FormatNumber(RoundTrip, 0) & " msecs (" & FormatNumber(RoundTrip / 1000, 3) & " secs)"

    ts = SntpIn.TransmitTimeStamp
    TotalSecs = Round(TimeStampToSecs(ts), 3)

    If TotalSecs = 0 Then
        ActivityLog ActivityCode, "Server Returned Invalid Time (0)"
        ActivityAdjust ActivityCode
        If SyncThenExit Then
            SyncThenExit = False
            Unload Me
        End If
        Exit Sub
    End If

#If DEBUG_ON Then
    ActivityLog ACTIVITY_CLIENT_SNTP Or ACTIVITY_DEBUG, "TotalSecs: " & CStr(TotalSecs)
#End If
    TotalSecs = TotalSecs + (RoundTrip / 2000)
    TotalSecs = Round(TotalSecs, 3)
    WholeSecs = Int(TotalSecs)
    FracSecs = TotalSecs - WholeSecs
#If DEBUG_ON Then
    ActivityLog ACTIVITY_CLIENT_SNTP Or ACTIVITY_DEBUG, "RoundTrip/2: " & CStr((RoundTrip / 2000))
    ActivityLog ACTIVITY_CLIENT_SNTP Or ACTIVITY_DEBUG, "Adj TotalSecs: " & CStr(TotalSecs)
    ActivityLog ACTIVITY_CLIENT_SNTP Or ACTIVITY_DEBUG, "WholeSecs: " & CStr(WholeSecs)
    ActivityLog ACTIVITY_CLIENT_SNTP Or ACTIVITY_DEBUG, "FracSecs: " & CStr(FracSecs)
#End If

    ActivityLog ActivityCode, "Our Current Date/Time: " & DispSystemTime(ost)

    ' reduce TotalSecs to prevent overflow
    TotalSecs = TotalSecs - DateDiff("s", #1/1/1900#, #1/1/1970#)
    NewDate = DateAdd("s", TotalSecs, #1/1/1970#)
    nst.wYear = Year(NewDate)
    nst.wMonth = Month(NewDate)
    nst.wDay = Day(NewDate)
    nst.wHour = Hour(NewDate)
    nst.wMinute = Minute(NewDate)
    nst.wSecond = Second(NewDate)
    nst.wMilliseconds = Round(FracSecs * 1000, 0)

    If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
        SetSystemTime nst
    End If

    ActivityLog ActivityCode, "Server's Current Date/Time: " & DispSystemTime(nst)

    CurSecs = DateDiff("s", #1/1/1970#, DateSerial(ost.wYear, ost.wMonth, ost.wDay) + TimeSerial(ost.wHour, ost.wMinute, ost.wSecond))
    CurSecs = CurSecs + (ost.wMilliseconds / 1000)
    s = FormatNumber(TotalSecs - CurSecs, 3, vbFalse) & " secs"
    If Left(s, 1) <> "-" Then s = "+" & s
    ActivityLog ActivityCode, "Difference/Adjustment to Our Clock: " & s

'    If SntpIn.Precision And &H80 Then
'        ActivityLog activitycode, "Precision: +/- 2^-" & CStr(SntpIn.Precision Xor &H80)
'    Else
'        ActivityLog activitycode, "Precision: +/- 2^+" & SntpIn.Precision
'    End If
'    ActivityLog activitycode, "Poll:" & SntpIn.Poll
'    s = ""
'    For i = 0 To UBound(SntpIn.ReferenceID)
'        If SntpIn.ReferenceID(i) > 0 Then
'            s = s & Chr(SntpIn.ReferenceID(i))
'        Else
'            Exit For
'        End If
'    Next i
'    Select Case UCase(s)
'        Case "LOCL":  s = s & " (uncalibrated local clock used as a primary reference for a subnet without external means of synchronization)"
'        Case "PPS":  s = s & " (atomic clock or other pulse-per-second source individually calibrated to national standards)"
'        Case "ACTS":  s = s & " (US National Institute of Standards and Technology Internet/Dialup Modem Service)"
'        Case "USNO":  s = s & " (US Naval Observatory Internet/Modem Service)"
'        Case "PTB":  s = s & " (PTB (Germany) Modem Service)"
'        Case "TDF":  s = s & " (Allouis (France) Radio 164 kHz)"
'        Case "DCF":  s = s & " (Mainflingen (Germany) Radio 77.5 kHz)"
'        Case "MSF":  s = s & " (Rugby (UK) Radio 60 kHz)"
'        Case "WWV":  s = s & " (Ft. Collins (US) Radio 2.5, 5, 10, 15, 20 MHz)"
'        Case "WWVB":  s = s & " (Boulder (US) Radio 60 kHz)"
'        Case "WWVH":  s = s & " (Kaui Hawaii (US) Radio 2.5, 5, 10, 15 MHz)"
'        Case "CHU":  s = s & " (Ottawa (Canada) Radio 3330, 7335, 14670 kHz)"
'        Case "LORC":  s = s & " (LORAN-C radionavigation system)"
'        Case "OMEG":  s = s & " (OMEGA radionavigation system)"
'        Case "GPS":  s = s & " (Global Positioning Service)"
'        Case "GOES":  s = s & " (Geostationary Orbit Environment Satellite)"
'    End Select
'    ActivityLog ActivityCode, "Reference:" & s

    Stratum = SntpIn.Stratum
'    Select Case Stratum
'        Case 0: s = "Unknown/Root?"
'        Case 1: s = "1 (Primary)"
'        Case 2 To 15: s = CStr(i) & " (Secondary)"
'        Case Else:  s = "Reserved"
'    End Select
'    ActivityLog ActivityCode, "Stratum:" & s

'    If (SntpIn.RootDelay.WholeSecs <> 0) Or (SntpIn.RootDelay.FracSecs <> 0) Then
'        WholeSecs = ApiConvUInt16Dec(SntpIn.RootDelay.WholeSecs)
'        FracSecs = ApiConvUInt16Dec(SntpIn.RootDelay.FracSecs) / (2 ^ 32)
'        ActivityLog activitycode, "Root Delay:" & FormatNumber(WholeSecs + FracSecs, 0)
'
'        WholeSecs = ApiConvUInt16Dec(SntpIn.RootDispersion.WholeSecs)
'        FracSecs = ApiConvUInt16Dec(SntpIn.RootDispersion.FracSecs) / (2 ^ 32)
'        ActivityLog activitycode, "Root Dispersion:" & FormatNumber(WholeSecs + FracSecs, 0)
'    End If

'    i = (SntpIn.LiVnMode And SNTP_LI_MASK) / 64
'    Select Case i
'        Case 0: s = "None Today"
'        Case 1: s = "Last Minute of Today has 61 Seconds"
'        Case 2: s = "Last Minute of Today has 59 seconds"
'        Case 3: s = "Invalid"
'    End Select

'    ActivityLog ActivityCode, "Leap Seconds:" & s
'    i = SntpIn.LiVnMode And SNTP_VN_MASK
'    ActivityLog ActivityCode, "Version:" & CStr(i / 8)
'    i = SntpIn.LiVnMode And SNTP_MODE_MASK
'    Select Case i
'        Case 0: s = "Reserved"
'        Case 1: s = "Symetric Active"
'        Case 2: s = "Symetric Passive"
'        Case 3: s = "Client"
'        Case 4: s = "Server"
'        Case 5: s = "Broadcast"
'        Case 6: s = "Reserved (NTP Control Message)"
'        Case 7: s = "Reserved (Private)"
'        Case Else: s = "Unknown (" & CStr(i) & ")"
'    End Select
'    ActivityLog ActivityCode, "Mode:" & s

    ActivityAdjust ActivityCode

    Set lv = ActivityDest(ActivityCode)
    If Not (lv Is Nothing) Then
        If ServerInx >= 0 Then
            Tss(ServerInx).SntpSupported = True
            Tss(ServerInx).Stratum = Stratum
            If Tss(ServerInx).RoundTrip <= 0 Then
                Tss(ServerInx).RoundTrip = RoundTrip
            End If
            Tss(ServerInx).Enabled = True
        End If
    End If

    If ServerInx >= 0 Then
        If (ActivityCode And ACTIVITY_CLIENT) <> 0 Then
            DispClientServer Tss(ServerInx)
        ElseIf (ActivityCode And ACTIVITY_SERVER) <> 0 Then
            DispAutoServer Tss(ServerInx)
        End If
    End If

    If SyncThenExit Then
        SyncThenExit = False
        Unload Me
    End If
End Sub
Private Sub btnClientTime_Click()
    Dim i As Long
    Dim sck As Winsock
    Dim Code As ACTIVITY_CODE

    If chkClientTest.Value = vbChecked Then
        Code = ACTIVITY_CLIENT_TIME_TEST
    Else
        Code = ACTIVITY_CLIENT_TIME_SYNC
    End If

    EnableSync Code
    ActivityClear Code
    i = CLng(tvClient.SelectedItem.Tag)
    Set sck = InitClientTime(Tss(i).HostName, Code)
    btnClientCancel.Tag = CLng(sck.Index)
End Sub
Private Function InitClientTime(HostName As String, ActivityCode As ACTIVITY_CODE) As Winsock
    Dim DQ As String
    Dim inx As Long

    DQ = SockHostDQ(HostName)
    If Len(DQ) <= 0 Then
        ActivityLog ActivityCode, "Could Not Resolve Host Name: " & HostName
        EnableSync ActivityCode
        ActivityAdjust ActivityCode
        Exit Function
    End If

    inx = sckClient.UBound + 1
    Load sckClient(inx)

    With sckClient(inx)
        .Close
        .Tag = JoinSckTag(TIMEOUT_VAL, ActivityCode, -1)
        .Protocol = sckTCPProtocol
        .RemotePort = PORT_TIME  'Time service port
        .RemoteHost = DQ ' HostName
        .Connect
    End With
    BegTrip = ApiTickStart()

    ActivityLog ActivityCode, "Connecting To " & HostName & " (" & DQ & ")"
    Set InitClientTime = sckClient(inx)
    timOutClient.Enabled = True
    ActivityAdjust ActivityCode
End Function
Private Function DispDaytimeDetails(Prefix As String) As String
    Dim s As String

    s = s & Prefix
    Select Case ServerDaytimeFormat
        Case DAYTIME_KAREN:
            s = s & """Karen's"" NIST Format"

        Case DAYTIME_SMTP:
            s = s & "SMTP Format"

        Case Else:
            s = s & "NIST Format"
            ServerDaytimeFormat = DAYTIME_NIST

    End Select

    DispDaytimeDetails = s
End Function
Private Sub btnStartupApply_Click()
    If chkStartup.Value = vbChecked Then
        If optStartupCommon.Value Then
            StartupShortcutCreate True
        Else
            StartupShortcutCreate False
        End If
    Else
        StartupShortcutKill
    End If

    btnStartupApply.Enabled = False
End Sub
Private Sub chkAuto_Click()
    AutoApply
End Sub
Private Sub SetAutoStatus(Status As Boolean)
    If AutoStatus = Status Then Exit Sub
    AutoStatus = Status
    If AutoStatus Then
        ActivityLog ACTIVITY_AUTO_SYNC, "Automatic Clock Adjustments ENABLED"
    Else
        ActivityLog ACTIVITY_AUTO_SYNC, "Automatic Clock Adjustments DISABLED"
    End If
End Sub
Private Sub chkStartup_Click()
    If chkStartup.Value = vbChecked Then
        optStartupUser.Enabled = True
        optStartupCommon.Enabled = True
        chkStartupSync.Enabled = True
        If chkStartupSync.Value = vbChecked Then
            chkStartupExit.Enabled = True
        Else
            chkStartupExit.Enabled = False
        End If
    Else
        optStartupUser.Enabled = False
        optStartupCommon.Enabled = False
        chkStartupSync.Enabled = False
        chkStartupExit.Enabled = False
    End If

    If Not chkStartup.Visible Then Exit Sub
    btnStartupApply.Enabled = True
End Sub
Private Sub StartupShortcutCreate(Common As Boolean)
    Dim ExeFile As String
    Dim LnkFile As String
    Dim WorkDir As String
    Dim SyncExit As Boolean
    Dim Args As String

    Me.MousePointer = vbHourglass
    DoEvents

    If chkStartupSync.Value = vbChecked Then
        SyncExit = True
    Else
        SyncExit = False
    End If

    StartupShortcutKill
    Status.Text = "Adding Shortcut to Startup Folder ..."
    
    Args = ""
    If chkStartupSync.Value = vbChecked Then
        Args = Args & ARG_SYNC
    End If
    If chkStartupExit.Value = vbChecked Then
        Args = Args & ARG_EXIT
    End If

    On Error Resume Next
    ApiShortcutCreate _
        ShortcutField(SHORTCUT_LNK_FILE, Common), _
        ShortcutField(SHORTCUT_EXE_FILE), _
        ShortcutField(SHORTCUT_WORK_DIR), _
        Args, _
        ShortcutField(SHORTCUT_DESC), _
        ShortcutField(SHORTCUT_EXE_FILE), _
        0, SW_MINIMIZE
    Status.Text = "Shortcut in Startup Folder Added"

    Me.MousePointer = vbDefault
    DoEvents
End Sub
Private Sub StartupShortcutKill()
    Dim Fid As String

    Status.Text = "Removing Shortcut from Startup Folder ..."
    Fid = ShortcutField(SHORTCUT_LNK_FILE, True, True)
    If ApiFileExists(Fid) Then Kill Fid

    Fid = ShortcutField(SHORTCUT_LNK_FILE, True, False)
    If ApiFileExists(Fid) Then Kill Fid

    Fid = ShortcutField(SHORTCUT_LNK_FILE, False, True)
    If ApiFileExists(Fid) Then Kill Fid

    Fid = ShortcutField(SHORTCUT_LNK_FILE, False, False)
    If ApiFileExists(Fid) Then Kill Fid
    Status.Text = "Shortcut in Startup Folder Removed"
End Sub
Private Function StartupShortcutFind() As String
    Dim StartupFolder As String
    Dim fd As PT_FILE_INFO
    Dim ExeFile As String
    Dim TargetFile As String
    Dim s As String
    Dim i As Long

    ExeFile = ShortcutField(SHORTCUT_EXE_FILE)

    StartupFolder = ShortcutField(SHORTCUT_PATH, True)
    ApiFindFirstFile StartupFolder & "*" & LINKEXT, fd
    Do While fd.Continue
        ApiShortcutInfo fd.FullName, s, TargetFile, s, s, i
        If StrComp(ExeFile, TargetFile, vbTextCompare) = 0 Then
            StartupShortcutFind = fd.FullName
            ApiFindClose fd
            Exit Function
        End If
        ApiFindNextFile fd
    Loop
    ApiFindClose fd

    StartupFolder = ShortcutField(SHORTCUT_PATH, False)
    ApiFindFirstFile StartupFolder & "*" & LINKEXT, fd
    Do While fd.Continue
        ApiShortcutInfo fd.FullName, s, TargetFile, s, s, i
        If StrComp(ExeFile, TargetFile, vbTextCompare) = 0 Then
            StartupShortcutFind = fd.FullName
            ApiFindClose fd
            Exit Function
        End If
        ApiFindNextFile fd
    Loop
    ApiFindClose fd

    StartupShortcutFind = ""
End Function
Private Function ShortcutField(Field As SHORTCUT_FIELD, Optional Common As Boolean = False, Optional SyncExit As Boolean = False) As String
    Dim ExeFile As String
    Dim LnkFile As String
    Dim WorkDir As String
    Dim Desc As String
    Dim LnkPath As String

    If ApiUnderIde() Then
        ExeFile = "PTSync"
    Else
        ExeFile = App.EXEName
    End If
    WorkDir = ApiFolderFix(App.Path)

    Select Case Field
        Case SHORTCUT_DESC
            Desc = "Launch " & App.FileDescription
            If SyncExit Then
                Desc = Desc & " (Sync Clock then Exit)"
            End If
            ShortcutField = Desc

        Case SHORTCUT_EXE_FILE
            ShortcutField = WorkDir & ExeFile & EXE_EXT

        Case SHORTCUT_LNK_FILE
            If Common Then
                LnkPath = ApiSpecialFolder(CSIDL_COMMON_STARTUP)
            Else
                LnkPath = ApiSpecialFolder(CSIDL_STARTUP)
            End If
            LnkFile = LnkPath & App.FileDescription
            If SyncExit Then
                LnkFile = LnkFile & " (Sync Clock then Exit)"
            End If
            ShortcutField = LnkFile & LINKEXT

        Case SHORTCUT_WORK_DIR
            ShortcutField = WorkDir

        Case SHORTCUT_PATH
            If Common Then
                LnkPath = ApiSpecialFolder(CSIDL_COMMON_STARTUP)
            Else
                LnkPath = ApiSpecialFolder(CSIDL_STARTUP)
            End If
            ShortcutField = LnkPath
    
    End Select
End Function
Private Sub chkStartupAutoSync_Click()
    AutoApply
End Sub
Private Sub SetAutoStartupStatus(Status As Boolean)
    If AutoStartupStatus = Status Then Exit Sub
    AutoStartupStatus = Status
    If AutoStartupStatus Then
        ActivityLog ACTIVITY_AUTO_SYNC, AUTO_STARTUP_ENABLED
    Else
        ActivityLog ACTIVITY_AUTO_SYNC, AUTO_STARTUP_DISABLED
    End If
End Sub
Private Sub chkServerDaytime_Click()
    If chkServerDaytime.Value = vbChecked Then
        ServerDaytimeEnable True
    Else
        ServerDaytimeEnable False
    End If
End Sub
Private Sub ServerDaytimeEnable(b As Boolean)
    Dim i As Long
    Dim Port As String
    Dim ServiceEnabled As Boolean


    Port = CStr(sckDaytimeUdp.LocalPort)
    ServiceEnabled = False
    On Error Resume Next

    If b Then
        If sckDaytimeTcp(0).State <> sckClosed Then sckDaytimeTcp(0).Close
        sckDaytimeTcp(i).Bind
        sckDaytimeTcp(i).Listen
        If Err.Number <> 0 Then
            ActivityLog ACTIVITY_SERVER_DAYTIME, "Could Not Enable TCP " & Port & ALREADY_IN_USE
            Err.Clear
        Else
            ActivityLog ACTIVITY_SERVER_DAYTIME, "Listening on TCP Port " & Port
            ServiceEnabled = True
        End If

        If sckDaytimeUdp.State <> sckClosed Then sckDaytimeUdp.Close
        sckDaytimeUdp.Bind
        If Err.Number <> 0 Then
            ActivityLog ACTIVITY_SERVER_DAYTIME, "Could Not Enable UDP " & Port & ALREADY_IN_USE
            ApiBeep
            Err.Clear
            chkServerDaytime.Value = vbUnchecked
        Else
            ActivityLog ACTIVITY_SERVER_DAYTIME, "Listening on UDP Port " & Port
            ServiceEnabled = True
            optDaytimeNIST.Enabled = True
            optDaytimeKaren.Enabled = True
            optDaytimeSMTP.Enabled = True
        End If

        If ServiceEnabled Then
            ActivityLog ACTIVITY_SERVER_DAYTIME, DispDaytimeDetails("Protocol Enabled using ")
            ServerCnt = ServerCnt + 1
            If Not timOutServer.Enabled Then timOutServer.Enabled = True
            Status.Text = "DAYTIME Server Enabled"
        End If
    Else
        For i = sckDaytimeTcp.LBound To sckDaytimeTcp.UBound
            If sckDaytimeTcp(i).State <> sckClosed Then sckDaytimeTcp(i).Close
            If i > 1 Then Unload sckDaytimeTcp(i)
        Next i
        If sckDaytimeUdp.State <> sckClosed Then sckDaytimeUdp.Close
        Status.Text = "DAYTIME Server Disabled"

        ServerCnt = ServerCnt - 1
        optDaytimeNIST.Enabled = False
        optDaytimeKaren.Enabled = False
        optDaytimeSMTP.Enabled = False
        ActivityLog ACTIVITY_SERVER_DAYTIME, "Protocol Disabled"
        If ServerCnt <= 0 Then
            timOutServer.Enabled = False
            ServerCnt = 0
        End If
    End If

    ActivityAdjust ACTIVITY_SERVER
End Sub
Private Sub chkServerSntp_Click()
    If chkServerSntp.Value = vbChecked Then
        ServerSntpEnable True
    Else
        ServerSntpEnable False
    End If
End Sub
Private Sub ServerSntpEnable(b As Boolean)
    Dim i As Long
    Dim Port As String

    Port = CStr(sckSntp.LocalPort)
    On Error Resume Next
    If chkServerSntp.Value = vbChecked Then
        If sckSntp.State <> sckClosed Then sckSntp.Close
        sckSntp.Bind
        If Err.Number <> 0 Then
            ActivityLog ACTIVITY_SERVER_SNTP, "Could Not Enable UDP " & Port & ALREADY_IN_USE
            ApiBeep
            Err.Clear
            chkServerSntp.Value = vbUnchecked
        Else
            ActivityLog ACTIVITY_SERVER_SNTP, "Listening on UDP Port " & Port
            ServerCnt = ServerCnt + 1
            ActivityLog ACTIVITY_SERVER_SNTP, "Protocol Enabled"
            If Not timOutServer.Enabled Then timOutServer.Enabled = True
            Status.Text = "SNTP Server Enabled"
        End If
    Else
        If sckSntp.State <> sckClosed Then sckSntp.Close
        Status.Text = "SNTP Server Disabled"
        ActivityLog ACTIVITY_SERVER_SNTP, "Protocol Disabled"
        ServerCnt = ServerCnt - 1
        If ServerCnt <= 0 Then
            timOutServer.Enabled = False
            ServerCnt = 0
        End If
    End If

    ActivityAdjust ACTIVITY_SERVER
End Sub
Private Sub chkServerTime_Click()
    If chkServerTime.Value = vbChecked Then
        ServerTimeEnable True
    Else
        ServerTimeEnable False
    End If
End Sub
Private Sub ServerTimeEnable(b As Boolean)
    Dim i As Long
    Dim Port As String
    Dim ServiceEnabled As Boolean

    Port = CStr(sckTimeUdp.LocalPort)
    ServiceEnabled = False
    On Error Resume Next
    If chkServerTime.Value = vbChecked Then
        If sckTimeTcp(0).State <> sckClosed Then sckTimeTcp(0).Close
        sckTimeTcp(i).Bind
        sckTimeTcp(i).Listen
        If Err.Number = 0 Then
            ActivityLog ACTIVITY_SERVER_TIME, "Listening on TCP Port " & Port
            ServiceEnabled = True
        Else
            ActivityLog ACTIVITY_SERVER_TIME, "Could Not Enable TCP " & Port & ALREADY_IN_USE
            Err.Clear
        End If

        If sckTimeUdp.State <> sckClosed Then sckTimeUdp.Close
        sckTimeUdp.Bind
        If Err.Number <> 0 Then
            ActivityLog ACTIVITY_SERVER_TIME, "Could Not Enable UDP " & Port & ALREADY_IN_USE
            ApiBeep
            Err.Clear
            chkServerTime.Value = vbUnchecked
        Else
            ActivityLog ACTIVITY_SERVER_TIME, "Listening on UDP Port " & Port
            ServiceEnabled = True
        End If
        If ServiceEnabled Then
            ActivityLog ACTIVITY_SERVER_TIME, "Protocol Enabled"
            Status.Text = "TIME Server Enabled"
            ServerCnt = ServerCnt + 1
            If Not timOutServer.Enabled Then timOutServer.Enabled = True
        End If
    Else
        For i = sckTimeTcp.LBound To sckTimeTcp.UBound
            If sckTimeTcp(i).State <> sckClosed Then sckTimeTcp(i).Close
            If i > 1 Then Unload sckTimeTcp(i)
        Next i
        If sckTimeUdp.State <> sckClosed Then sckTimeUdp.Close
        Status.Text = "TIME Server Disabled"

        ActivityLog ACTIVITY_SERVER_TIME, "Protocol Disabled"
        ServerCnt = ServerCnt - 1
        If ServerCnt <= 0 Then
            timOutServer.Enabled = False
            ServerCnt = 0
        End If
    End If

    ActivityAdjust ACTIVITY_SERVER
End Sub
Private Sub chkSntp_Click()
    lblServers.Caption = DispSomeServers(ClientServer, tvClient, lvClient, chkSNTP, chkDaytime, chkTime)
    DispClientServer ClientServer
End Sub
Private Sub chkStartupExit_Click()
    If Not chkStartupExit.Visible Then Exit Sub
    btnStartupApply.Enabled = True
End Sub
Private Sub chkStartupSync_Click()
    If chkStartupSync.Value = vbChecked Then
        chkStartupExit.Enabled = True
    Else
        chkStartupExit.Enabled = False
    End If

    If Not chkStartupSync.Visible Then Exit Sub
    btnStartupApply.Enabled = True
End Sub
Private Sub chkSysTray_Click()
    If chkSysTray.Value = vbChecked Then
        PTMe.MinToSysTray = True
    Else
        PTMe.MinToSysTray = False
    End If
End Sub
Private Sub chkTime_Click()
    lblServers.Caption = DispSomeServers(ClientServer, tvClient, lvClient, chkSNTP, chkDaytime, chkTime)
    DispClientServer ClientServer
End Sub
Private Sub chkDaytime_Click()
    lblServers.Caption = DispSomeServers(ClientServer, tvClient, lvClient, chkSNTP, chkDaytime, chkTime)
    DispClientServer ClientServer
End Sub
Private Sub dtAuto_Change()
    AutoApply
End Sub
Private Sub Label12_Click()
    tabSync.Tab = 1
End Sub
Private Sub Label15_Click()
    btnEdit_Click
End Sub
Private Sub Label16_Click()
    btnEdit_Click
End Sub
Private Sub Label18_Click()
    tabSync.Tab = 4
End Sub
Private Sub Label19_Click()
    tabSync.Tab = 4
End Sub
Private Sub Label9_Click()
    tabSync.Tab = 1
End Sub
Private Sub Label13_Click()
    tabSync.Tab = 2
End Sub
Private Sub Label10_Click()
    tabSync.Tab = 2
End Sub
Private Sub Label14_Click()
    tabSync.Tab = 3
End Sub
Private Sub Label11_Click()
    tabSync.Tab = 3
End Sub
Private Sub lvClientLog_ItemClick(ByVal Item As MSComctlLib.ListItem)
    lvClientLog.ToolTipText = Item.Text
End Sub
Private Sub optDaytimeKaren_Click()
    If Not optDaytimeKaren.Enabled Then Exit Sub
    If optDaytimeKaren.Value Then
        ServerDaytimeFormat = DAYTIME_KAREN
        ActivityLog ACTIVITY_SERVER_DAYTIME, DispDaytimeDetails("Daytime Format changed to ")
        ActivityAdjust ACTIVITY_SERVER
    End If
End Sub
Private Sub optDaytimeNIST_Click()
    If Not optDaytimeNIST.Enabled Then Exit Sub
    If optDaytimeNIST.Value Then
        ServerDaytimeFormat = DAYTIME_NIST
        ActivityLog ACTIVITY_SERVER_DAYTIME, DispDaytimeDetails("Daytime Format changed to ")
        ActivityAdjust ACTIVITY_SERVER
    End If
End Sub
Private Sub optDaytimeSMTP_Click()
    If Not optDaytimeSMTP.Enabled Then Exit Sub
    If optDaytimeSMTP.Value Then
        ServerDaytimeFormat = DAYTIME_SMTP
        ActivityLog ACTIVITY_SERVER_DAYTIME, DispDaytimeDetails("Daytime Format changed to ")
        ActivityAdjust ACTIVITY_SERVER
    End If
End Sub
Private Sub optStartupCommon_Click()
    If Not optStartupCommon.Visible Then Exit Sub
    btnStartupApply.Enabled = True
End Sub
Private Sub optStartupUser_Click()
    If Not optStartupUser.Visible Then Exit Sub
    btnStartupApply.Enabled = True
End Sub
Private Sub sckClient_DataArrival(Index As Integer, ByVal bytesTotal As Long)
    Dim buf() As Byte
    Dim DataSntp As SNTP_MESSAGE
    Dim DataTime() As Byte
    Dim DataDaytime As String
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    RoundTrip = ApiTickElapsed(BegTrip)
    SplitSckTag sckClient(Index).Tag, TimeOut, ActivityCode, ServerInx
    
    On Error Resume Next
    Select Case ActivityCode And ACTIVITY_PROTO_MASK

        Case ACTIVITY_SNTP:
            If bytesTotal <= 0 Then
                ReDim buf(0)
            Else
                ReDim buf(bytesTotal - 1)
            End If
            sckClient(Index).GetData buf, vbByte And vbArray
            If Err.Number <> 0 Then
                ActivityLog ActivityCode, "Error: " & Err.Description & " (" & Err.Number & ")"
                Err.Clear
                ActivityAdjust ActivityCode
            ElseIf bytesTotal <> Len(DataSntp) Then
                ActivityLog ActivityCode, "Invalid Reply - Expected " & CStr(Len(DataSntp)) & " bytes, Received " & CStr(bytesTotal) & " bytes"
                ActivityAdjust ActivityCode
            Else
                ApiMemoryCopy VarPtr(DataSntp), VarPtr(buf(0)), bytesTotal
                FiniClientSntp ActivityCode, DataSntp, ServerInx
            End If

        Case ACTIVITY_TIME:
            If bytesTotal <= 0 Then
                ReDim buf(0)
            Else
                ReDim buf(bytesTotal - 1)
            End If
            sckClient(Index).GetData buf, vbByte And vbArray
            If Err.Number <> 0 Then
                ActivityLog ActivityCode, "Error: " & Err.Description & " (" & Err.Number & ")"
                Err.Clear
                ActivityAdjust ActivityCode
            ElseIf bytesTotal <> 4 Then
                ActivityLog ActivityCode, "Invalid Reply - Expected " & CStr(Len(DataSntp)) & " bytes, Received " & CStr(4) & " bytes"
                ActivityAdjust ActivityCode
            Else
                FiniClientTime ActivityCode, buf
            End If

        Case ACTIVITY_DAYTIME:
            sckClient(Index).GetData DataDaytime, vbString
            If Err.Number <> 0 Then
                ActivityLog ActivityCode, "Error:", Err.Description & " (" & Err.Number & ")"
                Err.Clear
                ActivityAdjust ActivityCode
            Else
                FiniClientDaytime ActivityCode, DataDaytime
            End If

        Case Else:
            ActivityLog ActivityCode, "Unknown Activity in sckClient_DataArrival: " & DispProtocol(ActivityCode)
            ActivityAdjust ActivityCode
    End Select

    sckClient(Index).Close
    Unload sckClient(Index)
    DisableSync ActivityCode
End Sub
Private Sub EnableWelcome(b As Boolean)

    lblWelcome.Enabled = b

    btnExit.Enabled = b
    picWelcome.Enabled = b
    Me.Enabled = b
    DoEvents
End Sub
Private Sub EnableServer(b As Boolean)
    btnExit.Enabled = b
    picServer.Enabled = b
    Me.Enabled = b
    DoEvents
End Sub
Private Sub EnableClient(b As Boolean)
    Dim i As Long

    If b Then
        lvClient.ForeColor = &H80000008  ' Window Text
    Else
        lvClient.ForeColor = &H80000011  ' Disabled text
    End If

    If tvClient.SelectedItem Is Nothing Then
        btnClientSntp.Enabled = False
        btnClientTime.Enabled = False
        btnClientDaytime.Enabled = False
    ElseIf IsNumeric(tvClient.SelectedItem.Tag) Then
        i = CLng(tvClient.SelectedItem.Tag)
        If Tss(i).SntpSupported Then
            btnClientSntp.Enabled = b
        Else
            btnClientSntp.Enabled = False
        End If
        If Tss(i).TimeSupported Then
            btnClientTime.Enabled = b
        Else
            btnClientTime.Enabled = False
        End If
        If Tss(i).DaytimeSupported Then
            btnClientDaytime.Enabled = b
        Else
            btnClientDaytime.Enabled = False
        End If
    Else
        btnClientSntp.Enabled = False
        btnClientTime.Enabled = False
        btnClientDaytime.Enabled = False
    End If

    btnClientCancel.Enabled = False
    DoEvents
End Sub
Private Sub EnableSync(ActivityCode As ACTIVITY_CODE)
    AbleSync True
End Sub
Private Sub DisableSync(ActivityCode As ACTIVITY_CODE)
    AbleSync False
End Sub
Private Sub AbleSync(b As Boolean)
    Dim Run As Boolean

    If b Then
        TimeOutCnt = TIMEOUT_VAL
    End If

    lvClient.Enabled = Not b
    If ClientServer Is Nothing Then
        btnClientSntp.Enabled = False
        btnClientTime.Enabled = False
        btnClientDaytime.Enabled = False
        Run = False
        btnClientCancel.Enabled = False
    Else
        Run = False
        If ClientServer.SntpSupported Then
            btnClientSntp.Enabled = Not b
            Run = b
        Else
            btnClientSntp.Enabled = False
        End If
        If ClientServer.TimeSupported Then
            btnClientTime.Enabled = Not b
            Run = b
        Else
            btnClientTime.Enabled = False
        End If
        If ClientServer.DaytimeSupported Then
            btnClientDaytime.Enabled = Not b
            Run = b
        Else
            btnClientDaytime.Enabled = False
        End If
    End If

    btnClientCancel.Enabled = Run
    If lvClient.Enabled And lvClient.Visible Then lvClient.SetFocus
    DoEvents
End Sub
Private Sub sckDaytimeTcp_Connect(Index As Integer)
    SendDaytime sckDaytimeTcp(Index)
End Sub
Private Sub SendTime(sck As Winsock)
    Dim DataTime(3) As Byte
    Dim st As SYSTEMTIME
    Dim CurDate As Date
    Dim secs As Variant
    Dim epoch As Variant
    Dim pow As Variant
    Dim lim As Long
    Dim i As Long
    Dim s As String

    epoch = CDec(DateDiff("s", #1/1/1900#, #1/1/1970#))
    GetSystemTime st
    CurDate = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)
    secs = CDec(DateDiff("s", #1/1/1970#, CurDate)) + epoch
    If st.wMilliseconds >= 500 Then secs = secs + 1

    lim = UBound(DataTime)
    For i = lim To 0 Step -1
        pow = CDec(256 ^ (lim - 1))
        DataTime(i) = secs / pow
        secs = secs - (DataTime(i) * pow)
    Next i

    sck.SendData DataTime

    If sck.Protocol = sckTCPProtocol Then
        s = "TCP"
    Else
        s = "UDP"
    End If
    ActivityLog ACTIVITY_SERVER_TIME, "TIME Sent via " & s & " to " & sck.RemoteHost & " (" & sck.RemoteHost & ")"
    ActivityAdjust ACTIVITY_SERVER_TIME
End Sub
Private Sub SendDaytime(sck As Winsock)
    Dim s As String
    Dim st As SYSTEMTIME
    Dim tt As Long
    Dim d As Date

' JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(NIST) OTM ' NIST

    d = Now()
    If Not ActiveTz.DstEnabled Then
        tt = 0
    ElseIf ActiveTz.DstActive Then
        If Month(d) = ActiveTz.StdBeg.Month Then
            tt = ActiveTz.StdBeg.Day - Day(d) + 1
        Else
            tt = 50
        End If
    Else
        If Month(d) = ActiveTz.DstBeg.Month Then
            tt = 51 + (ActiveTz.DstBeg.Day - Day(d))
        Else
            tt = 0
        End If
    End If

    GetSystemTime st
    d = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)
    s = Format(d, "yy-mm-dd hh:nn:ss " & Format(tt, "00") & " 0 0 00 UTC(KAREN) *")
    sck.SendData s
    
    If sck.Protocol = sckTCPProtocol Then
        s = "TCP"
    Else
        s = "UDP"
    End If
    ActivityLog ACTIVITY_SERVER_DAYTIME, "DATTIME Sent via " & s & " to " & sck.RemoteHost & " (" & sck.RemoteHost & ")"
    ActivityAdjust ACTIVITY_SERVER_DAYTIME
End Sub

Private Sub sckDaytimeTcp_SendComplete(Index As Integer)
    sckDaytimeTcp(Index).Close
    Unload sckDaytimeTcp(Index)
End Sub
Private Sub sckDaytimeUdp_DataArrival(ByVal bytesTotal As Long)
    SendDaytime sckDaytimeUdp
End Sub
Private Sub sckSntp_DataArrival(ByVal bytesTotal As Long)
    Dim SntpArray() As Byte
    Dim SntpMsg As SNTP_MESSAGE
    Dim Client As String
    Dim CurSecs As Variant
    Dim ts As SNTP_TIMESTAMP
    Dim TotalSecs As Variant
    Dim WholeSecs As Variant
    Dim FracSecs As Variant
    Dim st As SYSTEMTIME
    Dim i As Long

    Client = """" & sckSntp.RemoteHost & """ (" & sckSntp.RemoteHostIP & ")"
    ActivityLog ACTIVITY_SERVER_SNTP, "Request from " & Client & " Recieved"
#If DEBUG_ON Then
    ActivityLog ACTIVITY_SERVER, "bytesTotal: " & CStr(bytesTotal)
#End If
    If bytesTotal <> Len(SntpMsg) Then
        ActivityLog ACTIVITY_SERVER_SNTP, "Request Was Invalid: Expected " & CStr(Len(SntpMsg)) & " bytes, Recieved " & bytesTotal & " bytes"
        Exit Sub
    End If

    ReDim SntpArray(LenB(SntpMsg))
    sckSntp.GetData SntpArray, vbByte Or vbArray, LenB(SntpMsg)
    ApiMemoryCopy VarPtr(SntpMsg), VarPtr(SntpArray(0)), LenB(SntpMsg)

    GetSystemTime st
    CurSecs = DateDiff("s", #1/1/1970#, DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond))
    WholeSecs = CurSecs + DateDiff("s", #1/1/1900#, #1/1/1970#)
    FracSecs = CDec(st.wMilliseconds) / CDec(1000)
    TotalSecs = WholeSecs + FracSecs
#If DEBUG_ON Then
    ActivityLog ACTIVITY_SERVER_SNTP Or ACTIVITY_DEBUG, "TotalSecs: " & CStr(TotalSecs)
    ActivityLog ACTIVITY_SERVER_SNTP Or ACTIVITY_DEBUG, "WholeSecs: " & CStr(WholeSecs)
    ActivityLog ACTIVITY_SERVER_SNTP Or ACTIVITY_DEBUG, "FracSecs: " & CStr(FracSecs)
'    ActivityLog ACTIVITY_SERVER_SNTP Or ACTIVITY_DEBUG, "st.wMilliseconds: " & CStr(st.wMilliseconds)
#End If

    ts = SecsToTimeStamp(TotalSecs)

'    ts.Data(0) = Fix(WholeSecs / 16777216)
'    WholeSecs = WholeSecs - (CDec(ts.Data(0)) * 16777216)
'    ts.Data(1) = Fix(WholeSecs / CDec(65536))
'    WholeSecs = WholeSecs - (CDec(ts.Data(1)) * 65536)
'    ts.Data(2) = Fix(WholeSecs / 256)
'    WholeSecs = WholeSecs - (CDec(ts.Data(2)) * 256)
'    ts.Data(3) = WholeSecs

'    ts.Data(4) = Fix(FracSecs / 16777216)
'    FracSecs = FracSecs - (CDec(ts.Data(4)) * 16777216)
'    ts.Data(5) = Fix(FracSecs / CDec(65536))
'    FracSecs = FracSecs - (CDec(ts.Data(5)) * 65536)
'    ts.Data(6) = Fix(FracSecs / 256)
'    FracSecs = FracSecs - (CDec(ts.Data(6)) * 256)
'    ts.Data(7) = FracSecs

    SntpMsg.TransmitTimeStamp = ts

    ApiMemoryCopy VarPtr(SntpArray(0)), VarPtr(SntpMsg), LenB(SntpMsg)
    sckSntp.SendData SntpArray
    ActivityLog ACTIVITY_SERVER_SNTP, "Request from " & Client & " Processed"
End Sub
Private Sub sckTimeTcp_Connect(Index As Integer)
    SendTime sckTimeTcp(Index)
End Sub
Private Sub sckTimeTcp_ConnectionRequest(Index As Integer, ByVal requestID As Long)
    Dim inx As Long

    inx = sckTimeTcp.UBound + 1
    Load sckTimeTcp(inx)
    sckTimeTcp(inx).Tag = JoinSckTag(TIMEOUT_VAL, ACTIVITY_SERVER_TIME, -1)
    sckTimeTcp(inx).Accept requestID
End Sub
Private Sub sckDaytimeTcp_ConnectionRequest(Index As Integer, ByVal requestID As Long)
    Dim inx As Long

    inx = sckDaytimeTcp.UBound + 1
    Load sckDaytimeTcp(inx)
    sckDaytimeTcp(inx).Tag = JoinSckTag(TIMEOUT_VAL, ACTIVITY_SERVER_DAYTIME, -1)
    sckDaytimeTcp(inx).Accept requestID
End Sub
Private Sub sckTimeTcp_SendComplete(Index As Integer)
    sckTimeTcp(Index).Close
    Unload sckTimeTcp(Index)
End Sub
Private Sub sckTimeUdp_DataArrival(ByVal bytesTotal As Long)
    SendTime sckTimeUdp
End Sub
Private Sub tabSync_Click(PreviousTab As Integer)
    If btnStartupApply.Enabled Then btnStartupApply_Click
End Sub
Private Sub timAuto_Timer()
    If AutoSecs <= 0 Then
        timAuto.Enabled = False
        chkAuto.Value = vbUnchecked
        AutoApply
        Exit Sub
    End If

    AutoCnt = AutoCnt - 1
    If AutoCnt <= 0 Then
        InitClientSntp AutoServer, ACTIVITY_AUTO_SYNC
        AutoCnt = AutoSecs
    Else
        If lblAuto.Visible Then DispNextAuto AutoCnt
    End If
End Sub
Private Sub timOutClient_Timer()
    Dim sck As Winsock
    Dim inx As Long
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    For Each sck In sckClient
        If Len(sck.Tag) > 0 Then
            inx = sck.Index
            SplitSckTag sck.Tag, TimeOut, ActivityCode, ServerInx
            TimeOut = TimeOut - 1
            sck.Tag = JoinSckTag(TimeOut, ActivityCode, ServerInx)
            If TimeOut <= 0 Then
                DispSyncError sckClient(inx), ActivityCode, "Connection Has Timed Out"
                sckClient(inx).Close
                Unload sckClient(inx)
                DisableSync ActivityCode
                ActivityAdjust ActivityCode
                If SyncThenExit Then
                    SyncThenExit = False
                    tabSync.Tab = 1
                End If
            End If
        End If
    Next sck
End Sub
Private Sub DispSyncError(sck As Winsock, ActivityCode As ACTIVITY_CODE, msg As String)
    Dim s As String

    ActivityLog ActivityCode, "Error: " & msg
    ActivityLog ActivityCode, "This may indicate a temporary or permanent problem with the time server.", " "
    ActivityLog ActivityCode, "A different time server may succeed.", " "
    ActivityLog ActivityCode, " ", " "
    ActivityLog ActivityCode, "Also, if you are using a firewall or other type of Internet filter, make sure", " "
    s = """ can access "
    If sck.Protocol = sckUDPProtocol Then
        s = s & "UDP"
    Else
        s = s & "TCP"
    End If
    s = s & " port " & CStr(sck.RemotePort)
    ActivityLog ActivityCode, "the program """ & ShortcutField(SHORTCUT_EXE_FILE) & s, " "
End Sub
Private Sub Form_Initialize()
    ApiInitCommonControls
End Sub
'Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
'    Select Case KeyCode
'        Case vbKeyF1:
'            If ApiHelpEnabled Then
'                btnHelp_Click
'            Else
'                frmAbout.Show vbModal
'            End If
'
''        Case vbKeyF24:
''            WakeUpWindow Me
''            Exit Sub
'
'    End Select
'End Sub
Private Sub Form_Load()
    Dim i As Long
    Dim s As String
    Dim b As Boolean
    Dim ColHdr As ColumnHeader
    Dim SubKey As String
    Dim result As Long
    Dim inx As Long
    Dim cmd As String
    Dim AutoHostName As String
    Dim ClientHostName As String
    Dim Args As String
    Dim LnkFile As String

    tabSync.TabVisible(3) = False

    Set Reg = New Registry

    Set PTMe = New PTForm
    PTMe.ConstructorEx Me

    PTMe.AddSplitter tvAuto, picSplitAuto, lvAuto
    PTMe.AddSplitter tvClient, picSplitClient, lvClient

    Set Status = sbMain.Panels(1)
    Set Zoom = sbMain.Panels(2)

    SockInit

    If Not ApiHelpEnabled Then btnHelp.Visible = False

    chkTime.Top = chkSNTP.Top
    chkDaytime.Top = chkSNTP.Top
    lblServers.Top = chkSNTP.Top
    lblServers.Height = chkSNTP.Height

    Set ActiveTz = New TimeZone

    SubKey = Reg.DefaultKey & "\Auto"
    AutoHostName = Reg.ReadValue("HostName", "", SubKey)
    ActiveTz.SntpServer = AutoHostName
    b = Reg.ReadValue("Startup", False, SubKey)
    If b Then
        chkAutoSync.Value = vbChecked
    Else
        chkAutoSync.Value = vbUnchecked
    End If

    AutoSecs = Reg.ReadValue("Secs", SECS_PER_DAY, SubKey)
    dtAuto.Value = Secs2Picker(AutoSecs)

    b = Reg.ReadValue("Enabled", False, SubKey)
    If b Then
        chkAuto.Value = vbChecked
    Else
        chkAuto.Value = vbUnchecked
    End If
    lvAuto.Sorted = False

    LnkFile = StartupShortcutFind
    If LnkFile > " " Then
        chkStartup.Value = vbChecked
        If StrComp(ShortcutField(SHORTCUT_LNK_FILE, True), LnkFile, vbTextCompare) = 0 Then
            optStartupCommon.Value = True
        Else
            optStartupUser.Value = True
        End If
        ApiShortcutInfo LnkFile, s, s, s, Args, i
        If Len(Args) > 0 Then
            If InStr(1, Args, ARG_SYNC, vbTextCompare) > 0 Then
                chkStartupSync.Value = vbChecked
                If InStr(1, Args, ARG_EXIT, vbTextCompare) > 0 Then
                    chkStartupExit.Value = vbChecked
                Else
                    chkStartupExit.Value = vbUnchecked
                End If
            Else
                chkStartupSync.Value = vbUnchecked
                chkStartupExit.Value = vbUnchecked
            End If
        Else
            chkStartupSync.Value = vbUnchecked
            chkStartupExit.Value = vbUnchecked
        End If
    Else
        chkStartup.Value = vbUnchecked
        optStartupUser.Value = True
        chkStartupSync.Value = vbUnchecked
        chkStartupExit.Value = vbUnchecked
    End If
    optStartupUser.Caption = Replace(optStartupUser.Caption, "%", ApiUserName)

    If PTMe.MinToSysTray Then
        chkSysTray.Value = vbChecked
    Else
        chkSysTray.Value = vbUnchecked
    End If

'    b = Reg.ReadValue("StartupExit", False)
'    If b Then
'        chkStartupSync.Value = vbChecked
'    Else
'        chkStartupSync.Value = vbUnchecked
'    End If

    SubKey = Reg.DefaultKey & "\Client"
    ClientHostName = Reg.ReadValue("HostName", "", SubKey)
    lvClient.Sorted = False

    LoadServers AutoHostName, ClientHostName

    DispServers AutoServer, tvAuto, lvAuto, True, False, False
    DispAutoServer AutoServer

    lblServers.Caption = DispSomeServers(ClientServer, tvClient, lvClient, chkSNTP, chkDaytime, chkTime)
    DispClientServer ClientServer

    AutoApply

    optDaytimeSMTP.Tag = DAYTIME_SMTP
    optDaytimeNIST.Tag = DAYTIME_NIST
    optDaytimeKaren.Tag = DAYTIME_KAREN

    SubKey = Reg.DefaultKey & "\Server"
    ServerDaytimeFormat = Reg.ReadValue("DaytimeFormat", DAYTIME_NIST, SubKey)
    Select Case ServerDaytimeFormat
        Case DAYTIME_KAREN:
            optDaytimeKaren.Value = True

        Case DAYTIME_SMTP:
            optDaytimeSMTP.Value = True

        Case Else
            optDaytimeNIST.Value = True
            ServerDaytimeFormat = DAYTIME_NIST

    End Select
    
    chkServerSntp.Value = Reg.ReadValue("SNTP", vbUnchecked, SubKey)
    chkServerTime.Value = Reg.ReadValue("Time", vbUnchecked, SubKey)
    chkServerDaytime.Value = Reg.ReadValue("Daytime", vbUnchecked, SubKey)

    lblWelcome.Caption = "Welcome to " & App.FileDescription & "!"

    LvAdjust lvAutoLog
    LvAdjust lvClientLog
    LvAdjust lvServerLog

    LabelAlign lblAuto, dtAuto
    PTMe.ReInit

    cmd = Trim(Command)
    If Left(cmd, 1) = vbQuote Then cmd = Mid(cmd, 2)
    If Right(cmd, 1) = vbQuote Then cmd = Left(cmd, Len(cmd) - 1)
    cmd = Trim(cmd)

    If Len(cmd) > 0 Then
        If InStr(1, cmd, ARG_EXIT, vbTextCompare) > 0 Then
            SyncOnStartup = True
            SyncThenExit = True
        End If
        If InStr(1, cmd, ARG_SYNC, vbTextCompare) > 0 Then
            SyncOnStartup = True
        End If
    End If
    If chkAutoSync.Value = vbChecked Then SyncOnStartup = True

    If SyncOnStartup Then
        tabSync.Tab = 1
        If AutoServer Is Nothing Then
            SyncThenExit = False
            MsgBox "Cannot Sync -- No Time Server Specified", vbOKOnly, App.FileDescription
        Else
            InitClientSntp AutoServer, ACTIVITY_AUTO_SYNC
            If SyncThenExit Then Me.Visible = False
        End If
    Else
        tabSync.Tab = 0
    End If
End Sub
Private Sub Form_Unload(Cancel As Integer)
    Dim i As Long
    Dim ColHdr As ColumnHeader
    Dim SubKey As String
    Dim sck As Winsock
    Dim b As Boolean

    On Error Resume Next
    timOutClient.Enabled = False
    btnExit.Enabled = False
    Me.Enabled = False
    DoEvents

    For Each sck In sckClient
        sck.Close
        Unload sck
    Next sck

    sckSntp.Close

    sckTimeUdp.Close
    For Each sck In sckTimeTcp
        sck.Close
        Unload sck
    Next sck

    sckDaytimeUdp.Close
    For Each sck In sckDaytimeTcp
        sck.Close
        Unload sck
    Next sck

    SockFini

'    SaveServers

    If btnStartupApply.Enabled Then btnStartupApply_Click
    AutoApply

'    If chkStartupSync.Value = vbChecked Then
'        b = True
'    Else
'        b = False
'    End If
'    Reg.WriteValue "StartupExit", b

    SubKey = Reg.DefaultKey & "\Auto"
    If chkAutoSync.Value = vbChecked Then
        b = True
    Else
        b = False
    End If
    Reg.WriteValue "Startup", b, SubKey

    Reg.WriteValue "Secs", AutoSecs, SubKey
    If (chkAuto.Value = vbChecked) And (AutoSecs > 0) Then
        b = True
    Else
        b = False
    End If
    Reg.WriteValue "Enabled", b, SubKey
    If AutoServer Is Nothing Then
        Reg.WriteValue "HostName", "", SubKey
    Else
        Reg.WriteValue "HostName", AutoServer.HostName, SubKey
    End If

    SubKey = Reg.DefaultKey & "\Client"
    If ClientServer Is Nothing Then
        Reg.WriteValue "HostName", "", SubKey
    Else
        Reg.WriteValue "HostName", ClientServer.HostName, SubKey
    End If

    SubKey = Reg.DefaultKey & "\Server"
    Reg.WriteValue "SNTP", chkServerSntp.Value, SubKey
    Reg.WriteValue "Time", chkServerTime.Value, SubKey
    Reg.WriteValue "Daytime", chkServerDaytime.Value, SubKey
    Reg.WriteValue "DaytimeFormat", ServerDaytimeFormat, SubKey

    PTMe.Unload
    Set PTMe = Nothing

    End
End Sub
Private Sub timOutServer_Timer()
    Dim i As Long
    Dim RunCnt As Long
    Dim sck As Winsock
    Dim ActivityCode As ACTIVITY_CODE
    Dim TimeOut As Long
    Dim ServerInx As Long

    On Error Resume Next
    RunCnt = 0
'    For Each sck In sckSntp
'        If sck.Index > 0 Then
'            SplitSckTag sck.Tag, TimeOut, ActivityCode
'            TimeOut = TimeOut - 1
'            If TimeOut > 0 Then
'                sck.Tag = JoinSckTag(TimeOut, ActivityCode)
'                RunCnt = RunCnt + 1
'            Else
'                ActivityLog ActivityCode, "Request Timeout From " & sck.RemoteHost & " (" & sck.RemoteHostIP & ")"
'            End If
'        End If
'    Next sck

    For Each sck In sckTimeTcp
        If sck.Index > 0 Then
            SplitSckTag sck.Tag, TimeOut, ActivityCode, ServerInx
            TimeOut = TimeOut - 1
            If TimeOut > 0 Then
                sck.Tag = JoinSckTag(TimeOut, ActivityCode, ServerInx)
                RunCnt = RunCnt + 1
            ElseIf sck.Index > 0 Then
                sck.Close
                ActivityLog ActivityCode, "Request Timeout From " & sck.RemoteHost & " (" & sck.RemoteHostIP & ")"
                Unload sck
            End If
        End If
    Next sck

    For Each sck In sckTimeTcp
        If sck.Index > 0 Then
            SplitSckTag sck.Tag, TimeOut, ActivityCode, ServerInx
            TimeOut = TimeOut - 1
            If TimeOut > 0 Then
                sck.Tag = JoinSckTag(TimeOut, ActivityCode, ServerInx)
                RunCnt = RunCnt + 1
            ElseIf sck.Index > 0 Then
                sck.Close
                ActivityLog ActivityCode, "Request Timeout From " & sck.RemoteHost & " (" & sck.RemoteHostIP & ")"
                Unload sck
            End If
        End If
    Next sck
End Sub
Private Sub timZone_Timer()
    Dim d As Date

    If LocalListItem Is Nothing Then Exit Sub

    d = Now()
    If Minute(d) = Minute(PrevNow) Then
        If PrevNow <> 0 Then Exit Sub
    End If

    PrevNow = d
    d = ActiveTz.DateTimeLocal
    LocalListItem.SubItems(2) = FormatDateTime(d, vbLongDate) & " " & Format(d, "h:nn ampm")
End Sub
Private Sub FiniClientTime(ActivityCode As ACTIVITY_CODE, DataTime() As Byte)
    Dim st As SYSTEMTIME
    Dim CurDate As Date
    Dim NewDate As Date
    Dim secs As Variant
    Dim lim As Long
    Dim i As Long
    Dim lv As ListView

    GetSystemTime st
    CurDate = DateSerial(st.wYear, st.wMonth, st.wDay) + TimeSerial(st.wHour, st.wMinute, st.wSecond)

    lim = UBound(DataTime)
    For i = 0 To lim
        secs = secs + (DataTime(i) * (256 ^ (lim - i)))
    Next i

    secs = secs - DateDiff("s", #1/1/1900#, #1/1/1970#)
    NewDate = DateAdd("s", secs, #1/1/1970#)

    With st
        .wYear = Year(NewDate)
        .wMonth = Month(NewDate)
        .wDay = Day(NewDate)
        .wHour = Hour(NewDate)
        .wMinute = Minute(NewDate)
        .wSecond = Second(NewDate)
    End With

    ActivityLog ActivityCode, "Response Received in: " & FormatNumber(RoundTrip, 0) & " msecs (" & FormatNumber(RoundTrip / 1000, 3) & " secs)"
    ActivityLog ActivityCode, "Our Current Date/Time: " & CStr(CurDate) & " UTC"
    ActivityLog ActivityCode, "Server's Current Date/Time: " & CStr(NewDate) & " UTC"
    ActivityLog ActivityCode, "Difference/Adjustment to Our Clock: " & DateDiff("s", NewDate, CurDate) & " secs"

    If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
        SetSystemTime st
    End If

    ActivityAdjust ActivityCode

    Set lv = ActivityDest(ActivityCode)
    i = CLng(lv.SelectedItem.Tag)
    Tss(i).TimeSupported = True
    If Tss(i).RoundTrip <= 0 Then
        Tss(i).RoundTrip = RoundTrip
    End If
    Tss(i).Enabled = True

    If (ActivityCode And ACTIVITY_CLIENT) <> 0 Then
        DispClientServer Tss(i)
    ElseIf (ActivityCode And ACTIVITY_SERVER) <> 0 Then
        DispAutoServer Tss(i)
    End If
End Sub
Private Sub FiniClientDaytime(ActivityCode As ACTIVITY_CODE, DataDaytime As String)
    Dim s As String
    Dim i As Long
    Dim sa() As String
    Dim sx() As String
    Dim DaytimeFormat As DAYTIME_FORMAT
    Dim Prev As String
    Dim ost As SYSTEMTIME
    Dim nst As SYSTEMTIME
    Dim tt As Long
    Dim lv As ListView
    Dim DataReturned As String
    Dim OldTime As Date
    Dim NewTime As Date

    GetSystemTime ost
    OldTime = DateSerial(ost.wYear, ost.wMonth, ost.wDay) + TimeSerial(ost.wHour, ost.wMinute, ost.wSecond)

' Sun Apr 25 hh:mm:ss yyyy ' local time
' Weekday, Month Day, Year Time-Zone ' DOW
' dd mmm yy hh:mm:ss zzz ' SMTP
' JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(NIST) OTM ' NIST
' JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(KAREN) OTM ' KAREN
    s = Replace(DataDaytime, vbCrLf, "")
    s = Replace(s, vbLf, "")
    s = Trim(Replace(s, vbCr, ""))
    Do While Prev <> s
        Prev = s
        s = Replace(s, "  ", " ")
    Loop
    DataReturned = s

    ActivityLog ActivityCode, "Response Received in: " & FormatNumber(RoundTrip, 0) & " msecs (" & FormatNumber(RoundTrip / 1000, 3) & " secs)"
    ActivityLog ActivityCode, "Data Returned: " & DataReturned

    sa = Split(s, " ")
    DaytimeFormat = DAYTIME_UNKNOWN
    If UBound(sa) = 8 Then ' could be NIST or KAREN
        If InStr(1, sa(7), "NIST", vbTextCompare) > 0 Then
            DaytimeFormat = DAYTIME_NIST
        ElseIf InStr(1, sa(7), "KAREN", vbTextCompare) > 0 Then
            DaytimeFormat = DAYTIME_KAREN
        End If
    ElseIf UBound(sa) = 4 Then
        If IsNumeric(sa(0)) Then
            If Val(sa(0)) > 0 And (Val(sa(0)) < 32) Then
                If InStr(1, sa(3), ":") > 0 Then ' SMTP
                    DaytimeFormat = DAYTIME_SMTP
                End If
            End If
        Else
            If IsNumeric(sa(2)) Then
                If (Val(sa(2)) > 0) And (Val(sa(2)) < 32) Then
                    If (InStr(1, sa(4), "-") > 0) And (InStr(1, sa(4), ":") > 0) Then
                        DaytimeFormat = DAYTIME_DOW_MMM_DD_YYY_TZ
                    ElseIf (InStr(1, sa(3), ":") > 0) And IsNumeric(sa(4)) Then ' Local
                        DaytimeFormat = DAYTIME_LOCAL
                    End If
                End If
            End If
        End If
    End If

    Select Case DaytimeFormat
        Case DAYTIME_NIST, DAYTIME_KAREN:
            If DaytimeFormat = DAYTIME_NIST Then
                ActivityLog ActivityCode, "Data Format: " & "NIST"
            Else
                ActivityLog ActivityCode, "Data Format: " & "Karen's NIST"
            End If
            ActivityLog ActivityCode, "Modified Julian: " & sa(0) ' number of days since January 1, 4713 B.C.
            sx = Split(sa(1), "-")
            nst.wYear = sx(0)
            If nst.wYear < 100 Then
                nst.wYear = nst.wYear + 2000
            End If
            nst.wMonth = sx(1)
            nst.wDay = sx(2)
            ActivityLog ActivityCode, "YY-MO-DA: " & sa(1)
            sx = Split(sa(2), ":")
            nst.wHour = sx(0)
            nst.wMinute = sx(1)
            nst.wSecond = sx(2)
'            st.wMilliseconds = Round(FracSecs * 1000, 0)
            ActivityLog ActivityCode, "HH:MM:SS: " & sa(2)
            tt = CLng(sa(3))
            Select Case tt
                Case 0:
                    ActivityLog ActivityCode, "Daylight Saving Time Info: " & "Standard Time In Effect"
                Case 1 To 49:
                    ActivityLog ActivityCode, "Daylight Saving Time Info: " & "Standard Time Ends in " & tt & " days"
                Case 50:
                    ActivityLog ActivityCode, "Daylight Saving Time Info: " & "Daylight Saving Time in Effect"
                Case 51 To 99:
                    ActivityLog ActivityCode, "Daylight Saving Time Info: " & "Daylight Saving Time Ends in " & CStr(tt - 50) & " days"
            End Select
'            ActivityLog ActivityCode, "TT: " & sa(3)
            Select Case sa(4)
                Case "0":
                    s = "None"
                Case "1":
                    s = "+1"
                Case "2":
                    s = "-1"
                Case Else
                    s = sa(4)
            End Select
            ActivityLog ActivityCode, "Leap Secs at End of This Month: " & s
            Select Case sa(5)
                Case "0":
                    s = "Accurate"
                Case "1"
                    s = "Error <= 5 secs"
                Case "2"
                    s = "Error > 5 secs"
                Case "4"
                    s = "H/W or S/W Failure"
                Case Else
                    s = sa(5)
            End Select

            ActivityLog ActivityCode, "Server Health: " & s
            ActivityLog ActivityCode, "msAdv: " & sa(6)
            ActivityLog ActivityCode, "Our Current Date/Time: " & DispSystemTime(ost)
            ActivityLog ActivityCode, "Server's Current Date/Time: " & DispSystemTime(nst)
            If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
                SetSystemTime nst
            End If
            NewTime = DateSerial(nst.wYear, nst.wMonth, nst.wDay) + TimeSerial(nst.wHour, nst.wMinute, nst.wSecond)
            s = FormatNumber(DateDiff("s", OldTime, NewTime) - (ost.wMilliseconds / 1000), 3) & " secs"
            If Left(s, 1) <> "-" Then s = "+" & s
            ActivityLog ActivityCode, "Difference/Adjustment to Our Clock: " & s

        Case DAYTIME_SMTP:
            ActivityLog ActivityCode, "Data Format: " & "SMTP"
            nst.wYear = sa(1)
            If nst.wYear < 100 Then
                nst.wYear = nst.wYear + 2000
            End If
            ActivityLog ActivityCode, "Year: " & CStr(nst.wYear)
            nst.wMonth = MonthNum(sa(1))
            ActivityLog ActivityCode, "Month: " & nst.wMonth & " (" & sa(1) & ")"
            nst.wDay = sa(0)
            ActivityLog ActivityCode, "Day: " & CStr(nst.wDay)
            sx = Split(sa(3), ":")
            nst.wHour = sx(0)
            ActivityLog ActivityCode, "Hour: " & CStr(nst.wHour)
            nst.wMinute = sx(1)
            ActivityLog ActivityCode, "Minute: " & CStr(nst.wMinute)
            nst.wSecond = sx(2)
            ActivityLog ActivityCode, "Second: " & CStr(nst.wSecond)
            ActivityLog ActivityCode, "Time Zone: " & sa(3)
            ActivityLog ActivityCode, "Our Current Date/Time: " & DispSystemTime(ost)
            ActivityLog ActivityCode, "Server's Current Date/Time: " & DispSystemTime(nst)
            NewTime = DateSerial(nst.wYear, nst.wMonth, nst.wDay) + TimeSerial(nst.wHour, nst.wMinute, nst.wSecond)
            s = FormatNumber(DateDiff("s", OldTime, NewTime) - (ost.wMilliseconds / 1000), 3) & " secs"
            If Left(s, 1) <> "-" Then s = "+" & s
            ActivityLog ActivityCode, "Difference/Adjustment to Our Clock: " & s
            If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
                If StrComp(sa(3), "UTC", vbTextCompare) Then
                    SetSystemTime nst
                Else
                    ActivityLog ActivityCode, "NOTE: Cannot Sync Our Computer Clock because time zone", ""
                    ActivityLog ActivityCode, "report reported by Time Server is not recognized.", ""
                End If
            End If

        Case DAYTIME_DOW_MMM_DD_YYY_TZ:
            ActivityLog ActivityCode, "Data Format: " & "Weekday, Month Day, Year Time-Zone"
            ActivityLog ActivityCode, "Day of Week:" & sa(0)
            ActivityLog ActivityCode, "Year: " & sa(3)
            ActivityLog ActivityCode, "Month: " & sa(2) & " (" & MonthNum(sa(2)) & ")"
            ActivityLog ActivityCode, "Day: " & sa(1)
            s = Replace(sa(4), "-", ":")
            sx = Split(s, ":")
            ActivityLog ActivityCode, "Hour: " & sx(0)
            ActivityLog ActivityCode, "Minute: " & sx(1)
            ActivityLog ActivityCode, "Second: " & sx(2)
            ActivityLog ActivityCode, "Time Zone: " & sx(3)
            nst.wYear = sa(3)
            If nst.wYear < 100 Then
                nst.wYear = nst.wYear + 2000
            End If
            nst.wMonth = sa(2)
            nst.wDay = sa(1)
            nst.wHour = sx(0)
            nst.wMinute = sx(1)
            nst.wSecond = sx(2)
'           nst.wMilliseconds = Round(FracSecs * 1000, 0)
            ActivityLog ActivityCode, "Our Current Date/Time: " & DispSystemTime(ost)
            ActivityLog ActivityCode, "Server's Current Date/Time: " & DispSystemTime(nst)
            NewTime = DateSerial(nst.wYear, nst.wMonth, nst.wDay) + TimeSerial(nst.wHour, nst.wMinute, nst.wSecond)
            s = FormatNumber(DateDiff("s", OldTime, NewTime), 0) & " secs"
            s = FormatNumber(DateDiff("s", OldTime, NewTime) - (ost.wMilliseconds / 1000), 3) & " secs"
            If Left(s, 1) <> "-" Then s = "+" & s
            If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
                If StrComp(sx(3), "UTC", vbTextCompare) Then
                    SetSystemTime nst
                Else
                    ActivityLog ActivityCode, "NOTE: Cannot Sync Our Computer Clock because time zone", ""
                    ActivityLog ActivityCode, "report reported by Time Server is not recognized.", ""
                End If
            End If

        Case DAYTIME_LOCAL:
            ActivityLog ActivityCode, "Data Format: " & "Local"
            ActivityLog ActivityCode, "Day of Week: " & sa(0)
            ActivityLog ActivityCode, "Year: " & sa(4)
            ActivityLog ActivityCode, "Month: " & sa(1) & " (" & MonthNum(sa(1)) & ")"
            ActivityLog ActivityCode, "Day: " & sa(2)
            sx = Split(sa(3), ":")
            ActivityLog ActivityCode, "Hour: " & sx(0)
            ActivityLog ActivityCode, "Minute: " & sx(1)
            ActivityLog ActivityCode, "Second: " & sx(2)
            nst.wYear = sa(4)
            If nst.wYear < 100 Then
                nst.wYear = nst.wYear + 2000
            End If
            nst.wMonth = sa(1)
            nst.wDay = sa(2)
            nst.wHour = sx(0)
            nst.wMinute = sx(1)
            nst.wSecond = sx(2)
            ActivityLog ActivityCode, "Our Current Date/Time: " & DispSystemTime(ost)
            ActivityLog ActivityCode, "Server's Current Date/Time: " & DispSystemTime(nst)
            NewTime = DateSerial(nst.wYear, nst.wMonth, nst.wDay) + TimeSerial(nst.wHour, nst.wMinute, nst.wSecond)
            s = FormatNumber(DateDiff("s", OldTime, NewTime) - (ost.wMilliseconds / 1000), 3) & " secs"
            If Left(s, 1) <> "-" Then s = "+" & s
            ActivityLog ActivityCode, "Difference/Adjustment to Our Clock: " & s

            If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
                ActivityLog ActivityCode, "NOTE: Cannot Sync Our Computer Clock because Time Server", ""
                ActivityLog ActivityCode, "reports its local time, without an indication of their Time Zone.", ""
            End If

        Case Else:
            ActivityLog ActivityCode, "Data Format: " & "Unknown"
            ActivityLog ActivityCode, "Server's Current Date/Time: " & DataReturned
            If (ActivityCode And ACTIVITY_SYNC) <> 0 Then
                ActivityLog ActivityCode, "NOTE: Cannot Sync Our Computer Clock because Daytime", ""
                ActivityLog ActivityCode, "format sent by this Time Server is not recognized.", ""
            End If

    End Select

    ActivityAdjust ActivityCode

    Set lv = ActivityDest(ActivityCode)
    i = CLng(lv.SelectedItem.Tag)
    Tss(i).DaytimeSupported = True
    Tss(i).DaytimeFormat = DaytimeFormat
    If Tss(i).RoundTrip <= 0 Then
        Tss(i).RoundTrip = RoundTrip
    End If
    Tss(i).Enabled = True

    If (ActivityCode And ACTIVITY_CLIENT) <> 0 Then
        DispClientServer Tss(i)
    ElseIf (ActivityCode And ACTIVITY_SERVER) <> 0 Then
        DispAutoServer Tss(i)
    End If
End Sub
Private Sub tvAuto_NodeClick(ByVal Node As MSComctlLib.Node)
    Dim inx As Long

    If Not NodeIsHost(Node) Then
        lvAuto.ListItems.Clear
        Set AutoServer = Nothing
        AutoApply
        Exit Sub
    End If

    inx = CLng(Node.Tag)
    Set AutoServer = Tss(inx)
    DispAutoServer AutoServer
End Sub
Private Sub DispAutoServer(ts As TimeServer)
    Dim LstItm As ListItem

    lvAuto.ListItems.Clear
    If ts Is Nothing Then Exit Sub
'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "Country:"
'    LstItm.SubItems(2) = ts.Country
'
'    If Len(Trim(ts.State)) > 0 Then
'        Set LstItm = lvAuto.ListItems.Add
'        LstItm.SubItems(1) = "State:"
'        LstItm.SubItems(2) = ts.State
'    End If

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Service Area:"
    LstItm.SubItems(2) = ts.ServiceArea

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Policy:"
    LstItm.SubItems(2) = ts.Policy

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Contact:"
    LstItm.SubItems(2) = ts.Contact

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Stratum:"
    LstItm.SubItems(2) = ts.Stratum

'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "Configuration:"
'    LstItm.SubItems(2) = ts.Config

'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "SNTP:"
'    If ts.SntpSupported Then
'        LstItm.SubItems(2) = "Supported"
'    Else
'        LstItm.SubItems(2) = "Not Supported"
'    End If
'
'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "DAYTIME:"
'    If ts.DaytimeSupported Then
'        LstItm.SubItems(2) = "Supported"
'        Set LstItm = lvAuto.ListItems.Add
'        LstItm.SubItems(1) = "Daytime Format:"
'        LstItm.SubItems(2) = ts.DispDaytimeFormat
'    Else
'        LstItm.SubItems(2) = "Not Supported"
'    End If
'
'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "TIME:"
'    If ts.TimeSupported Then
'        LstItm.SubItems(2) = "Supported"
'    Else
'        LstItm.SubItems(2) = "Not Supported"
'    End If

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Location:"
    LstItm.SubItems(2) = ts.Location

    Set LstItm = lvAuto.ListItems.Add
    LstItm.SubItems(1) = "Coordinates:"
    LstItm.SubItems(2) = ts.Coords
'
'    Set LstItm = lvAuto.ListItems.Add
'    LstItm.SubItems(1) = "Synchronization:"
'    LstItm.SubItems(2) = ts.SyncMethod

    LvAdjust lvAuto

    chkAuto.Caption = Replace(AUTO_SCHED, "%", AutoServer.HostName)
    chkAutoSync.Caption = Replace(AUTO_STARTUP_SCHED, "%", AutoServer.HostName)
    AutoApply
End Sub
Private Sub tvClient_NodeClick(ByVal Node As MSComctlLib.Node)
    Dim inx As Long

    If Not NodeIsHost(Node) Then
        lvClient.ListItems.Clear
        EnableClient False
        Set ClientServer = Nothing
        Exit Sub
    End If

    inx = CLng(Node.Tag)
    Set ClientServer = Tss(inx)
    DispClientServer Tss(inx)
End Sub
Private Sub DispClientServer(ts As TimeServer)
    Dim LstItm As ListItem

    lvClient.ListItems.Clear
    If ts Is Nothing Then Exit Sub
'    Set LstItm = lvClient.ListItems.Add
'    LstItm.SubItems(1) = "Country:"
'    LstItm.SubItems(2) = ts.Country

'    If Len(Trim(ts.State)) > 0 Then
'        Set LstItm = lvClient.ListItems.Add
'        LstItm.SubItems(1) = "State:"
'        LstItm.SubItems(2) = ts.State
'    End If

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Service Area:"
    LstItm.SubItems(2) = ts.ServiceArea

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Policy:"
    LstItm.SubItems(2) = ts.Policy

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Contact:"
    LstItm.SubItems(2) = ts.Contact

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Stratum:"
    LstItm.SubItems(2) = ts.Stratum

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "SNTP:"
    If ts.SntpSupported Then
        LstItm.SubItems(2) = "Supported"
    Else
        LstItm.SubItems(2) = "Not Supported"
    End If

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "DAYTIME:"
    If ts.DaytimeSupported Then
        LstItm.SubItems(2) = "Supported"
        Set LstItm = lvClient.ListItems.Add
        LstItm.SubItems(1) = "Daytime Format:"
        LstItm.SubItems(2) = ts.DispDaytimeFormat
    Else
        LstItm.SubItems(2) = "Not Supported"
    End If

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "TIME:"
    If ts.TimeSupported Then
        LstItm.SubItems(2) = "Supported"
    Else
        LstItm.SubItems(2) = "Not Supported"
    End If

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Location:"
    LstItm.SubItems(2) = ts.Location

    Set LstItm = lvClient.ListItems.Add
    LstItm.SubItems(1) = "Coordinates:"
    LstItm.SubItems(2) = ts.Coords

'    Set LstItm = lvClient.ListItems.Add
'    LstItm.SubItems(1) = "Configuration:"
'    LstItm.SubItems(2) = ts.ServerHardware

'    Set LstItm = lvClient.ListItems.Add
'    LstItm.SubItems(1) = "Synchronization:"
'    LstItm.SubItems(2) = ts.SyncProto

    LvAdjust lvClient
    EnableClient True
End Sub

