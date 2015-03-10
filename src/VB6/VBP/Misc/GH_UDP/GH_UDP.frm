VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form frmGHUDP 
   Caption         =   "Global Hawk UDP Packets"
   ClientHeight    =   4365
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9075
   Icon            =   "GH_UDP.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4365
   ScaleWidth      =   9075
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrUpdate 
      Interval        =   2000
      Left            =   5580
      Top             =   3900
   End
   Begin VB.Timer tmrWatchdog 
      Interval        =   3000
      Left            =   6090
      Top             =   3900
   End
   Begin InetCtlsObjects.Inet Inet1 
      Left            =   6570
      Top             =   3870
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "Exit"
      Height          =   435
      Left            =   8100
      TabIndex        =   15
      Top             =   3930
      Width           =   855
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   3855
      Left            =   60
      TabIndex        =   0
      Top             =   30
      Width           =   8985
      _ExtentX        =   15849
      _ExtentY        =   6800
      _Version        =   393216
      TabHeight       =   520
      TabCaption(0)   =   "Status"
      TabPicture(0)   =   "GH_UDP.frx":0442
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label2"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label5"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "StatusBar1"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "chkLOGstatus"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "chkAllpackets"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "cboRemotePort1"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "cboRemoteIP1"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "chkClear"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "cmdConnect"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "txtUDPin1"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "chkWriteMTPstatus"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "txtDeltaUT"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).ControlCount=   13
      TabCaption(1)   =   "MTP Lowrate"
      TabPicture(1)   =   "GH_UDP.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Label3"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "Label4"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "StatusBar2"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "chkWriteMTPcurrent"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "txtUDPin2"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "chkLOGlowrate"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "cmdLowrate"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "cboRemoteIP2"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).Control(8)=   "cboRemotePort2"
      Tab(1).Control(8).Enabled=   0   'False
      Tab(1).Control(9)=   "chkFTPlowrate"
      Tab(1).Control(9).Enabled=   0   'False
      Tab(1).Control(10)=   "cmdPut"
      Tab(1).Control(10).Enabled=   0   'False
      Tab(1).Control(11)=   "cmdGet"
      Tab(1).Control(11).Enabled=   0   'False
      Tab(1).ControlCount=   12
      TabCaption(2)   =   "Eng Data"
      TabPicture(2)   =   "GH_UDP.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Frame5"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "Frame13"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "txtLOchanging"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "txtMTPcurrentRAW"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "chkLOchanging"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).Control(5)=   "chkMTPcurrentRAW"
      Tab(2).Control(5).Enabled=   0   'False
      Tab(2).ControlCount=   6
      Begin VB.CheckBox chkMTPcurrentRAW 
         Caption         =   "Is MTPcurrent.RAW updating?"
         Height          =   285
         Left            =   -74910
         TabIndex        =   82
         Top             =   3120
         Value           =   1  'Checked
         Width           =   2565
      End
      Begin VB.CheckBox chkLOchanging 
         Caption         =   "Is LO synthesizer changing frequency?"
         Height          =   315
         Left            =   -74910
         TabIndex        =   81
         Top             =   3450
         Value           =   1  'Checked
         Width           =   3105
      End
      Begin VB.TextBox txtMTPcurrentRAW 
         BackColor       =   &H0000FF00&
         Height          =   315
         Left            =   -71790
         TabIndex        =   80
         Text            =   "Last Update Time"
         Top             =   3120
         Width           =   5715
      End
      Begin VB.TextBox txtLOchanging 
         BackColor       =   &H0000FF00&
         Height          =   315
         Left            =   -71820
         TabIndex        =   79
         Top             =   3480
         Width           =   5745
      End
      Begin VB.CommandButton cmdGet 
         Caption         =   "Get"
         Height          =   405
         Left            =   -69030
         TabIndex        =   78
         Top             =   3120
         Width           =   915
      End
      Begin VB.CommandButton cmdPut 
         Caption         =   "Put"
         Height          =   405
         Left            =   -69930
         TabIndex        =   77
         Top             =   3120
         Width           =   855
      End
      Begin VB.CheckBox chkFTPlowrate 
         Caption         =   "FTP Lowrate Packet to Web Page"
         Height          =   195
         Left            =   -69900
         TabIndex        =   76
         Top             =   2850
         Value           =   1  'Checked
         Width           =   3165
      End
      Begin VB.TextBox txtDeltaUT 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   8040
         MultiLine       =   -1  'True
         TabIndex        =   74
         Text            =   "GH_UDP.frx":0496
         Top             =   3030
         Width           =   825
      End
      Begin VB.CheckBox chkWriteMTPstatus 
         Caption         =   "Write MTPstatus.txt"
         Height          =   195
         Left            =   4920
         TabIndex        =   73
         Top             =   2820
         Value           =   1  'Checked
         Width           =   1875
      End
      Begin VB.Frame Frame13 
         Caption         =   "Raw Mux"
         Height          =   2655
         Left            =   -74880
         TabIndex        =   56
         Top             =   390
         Width           =   1695
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   72
            Top             =   2280
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   71
            Top             =   2040
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   70
            Top             =   1800
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   69
            Top             =   1560
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   68
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   67
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   960
            MultiLine       =   -1  'True
            TabIndex        =   66
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   345
            Index           =   8
            Left            =   975
            MultiLine       =   -1  'True
            TabIndex        =   65
            Top             =   600
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   64
            Top             =   2280
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   63
            Top             =   2040
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   62
            Top             =   1800
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   61
            Top             =   1560
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   60
            Top             =   1320
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   59
            Top             =   1080
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   58
            Top             =   840
            Width           =   615
         End
         Begin VB.TextBox txtRux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   120
            MultiLine       =   -1  'True
            TabIndex        =   57
            Top             =   600
            Width           =   615
         End
      End
      Begin VB.Frame Frame5 
         Caption         =   "Temperatures"
         Height          =   2655
         Left            =   -73080
         TabIndex        =   21
         Top             =   360
         Width           =   2535
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   16
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   38
            Top             =   2280
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   15
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   37
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   14
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   36
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   13
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   35
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   12
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   34
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   11
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   33
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   10
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   32
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   9
            Left            =   1920
            MultiLine       =   -1  'True
            TabIndex        =   31
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   8
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   30
            Top             =   2280
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   7
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   29
            Top             =   2040
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   6
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   28
            Top             =   1800
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   5
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   27
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   4
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   26
            Top             =   1320
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   3
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   25
            Top             =   1080
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   2
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   24
            Top             =   840
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   1
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   23
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtMux 
            Alignment       =   1  'Right Justify
            Height          =   285
            Index           =   0
            Left            =   720
            MultiLine       =   -1  'True
            TabIndex        =   22
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label37 
            Caption         =   "Acc (-)"
            Height          =   255
            Left            =   1320
            TabIndex        =   55
            Top             =   2280
            Width           =   615
         End
         Begin VB.Label Label36 
            Caption         =   "PS 12 v"
            Height          =   255
            Left            =   1320
            TabIndex        =   54
            Top             =   2040
            Width           =   615
         End
         Begin VB.Label Label35 
            Caption         =   "PS 5 v"
            Height          =   255
            Left            =   1320
            TabIndex        =   53
            Top             =   1800
            Width           =   495
         End
         Begin VB.Label Label34 
            Caption         =   "DC 2"
            Height          =   255
            Left            =   1320
            TabIndex        =   52
            Top             =   1560
            Width           =   615
         End
         Begin VB.Label Label33 
            Caption         =   "DC 1"
            Height          =   255
            Left            =   1320
            TabIndex        =   51
            Top             =   1320
            Width           =   615
         End
         Begin VB.Label Label32 
            Caption         =   "Vref"
            Height          =   255
            Left            =   1320
            TabIndex        =   50
            Top             =   1080
            Width           =   615
         End
         Begin VB.Label Label31 
            BackColor       =   &H80000000&
            Caption         =   "Spare"
            Height          =   255
            Left            =   1320
            TabIndex        =   49
            Top             =   840
            Width           =   615
         End
         Begin VB.Label Label30 
            Caption         =   "Motor"
            Height          =   255
            Left            =   1320
            TabIndex        =   48
            Top             =   600
            Width           =   615
         End
         Begin VB.Label Label29 
            Caption         =   "Window"
            Height          =   255
            Left            =   120
            TabIndex        =   47
            Top             =   2280
            Width           =   615
         End
         Begin VB.Label Label28 
            Caption         =   "Acc(+)"
            Height          =   255
            Left            =   120
            TabIndex        =   46
            Top             =   2040
            Width           =   615
         End
         Begin VB.Label Label27 
            Caption         =   "Tgt Hi"
            Height          =   255
            Left            =   120
            TabIndex        =   45
            Top             =   1800
            Width           =   615
         End
         Begin VB.Label Label26 
            Caption         =   "Diode"
            Height          =   255
            Left            =   120
            TabIndex        =   44
            Top             =   600
            Width           =   615
         End
         Begin VB.Label Label25 
            Caption         =   "Tgt Lo"
            Height          =   255
            Left            =   120
            TabIndex        =   43
            Top             =   1560
            Width           =   495
         End
         Begin VB.Label Label24 
            Caption         =   "IF Amp"
            Height          =   255
            Left            =   120
            TabIndex        =   42
            Top             =   1320
            Width           =   615
         End
         Begin VB.Label Label23 
            Caption         =   "LO2"
            Height          =   255
            Left            =   120
            TabIndex        =   41
            Top             =   1080
            Width           =   495
         End
         Begin VB.Label Label22 
            Caption         =   "LO1"
            Height          =   255
            Left            =   120
            TabIndex        =   40
            Top             =   840
            Width           =   495
         End
         Begin VB.Label Label21 
            Caption         =   "Target"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   39
            Top             =   360
            Width           =   495
         End
      End
      Begin VB.ComboBox cboRemotePort2 
         Height          =   315
         Left            =   -73950
         TabIndex        =   19
         Text            =   "10200"
         ToolTipText     =   "Local Host will use the same port number"
         Top             =   3180
         Width           =   1695
      End
      Begin VB.ComboBox cboRemoteIP2 
         Height          =   315
         Left            =   -73950
         TabIndex        =   18
         Text            =   "10.3.1.200"
         Top             =   2850
         Width           =   1695
      End
      Begin VB.CommandButton cmdLowrate 
         Caption         =   "Connect"
         Height          =   405
         Left            =   -66990
         TabIndex        =   14
         Top             =   3360
         Width           =   855
      End
      Begin VB.CheckBox chkLOGlowrate 
         Caption         =   "Log Lowrate Packets"
         Height          =   195
         Left            =   -72150
         TabIndex        =   13
         Top             =   2850
         Width           =   1845
      End
      Begin VB.TextBox txtUDPin2 
         Height          =   2355
         Left            =   -74940
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   12
         Top             =   420
         Width           =   8805
      End
      Begin VB.CheckBox chkWriteMTPcurrent 
         Caption         =   "Write MTPcurrent"
         Height          =   195
         Left            =   -72150
         TabIndex        =   11
         Top             =   3120
         Value           =   1  'Checked
         Width           =   1635
      End
      Begin VB.TextBox txtUDPin1 
         Height          =   2355
         Left            =   90
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   7
         Top             =   360
         Width           =   8775
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "Connect"
         Height          =   375
         Left            =   8040
         TabIndex        =   6
         Top             =   3390
         Width           =   855
      End
      Begin VB.CheckBox chkClear 
         Caption         =   "Clear Each Packet"
         Height          =   195
         Left            =   2940
         TabIndex        =   5
         Top             =   2820
         Width           =   1665
      End
      Begin VB.ComboBox cboRemoteIP1 
         Height          =   315
         Left            =   1020
         TabIndex        =   4
         Text            =   "130.134.183.222"
         Top             =   2790
         Width           =   1695
      End
      Begin VB.ComboBox cboRemotePort1 
         Height          =   315
         Left            =   1020
         TabIndex        =   3
         Text            =   "5100"
         ToolTipText     =   "Local Host will use the same port number"
         Top             =   3120
         Width           =   1695
      End
      Begin VB.CheckBox chkAllpackets 
         Caption         =   "View All packets"
         Height          =   195
         Left            =   2940
         TabIndex        =   2
         Top             =   3300
         Width           =   1485
      End
      Begin VB.CheckBox chkLOGstatus 
         Caption         =   "Log Status Packets"
         Height          =   195
         Left            =   2940
         TabIndex        =   1
         Top             =   3060
         Width           =   2025
      End
      Begin MSComctlLib.StatusBar StatusBar1 
         Height          =   255
         Left            =   60
         TabIndex        =   10
         Top             =   3510
         Width           =   6795
         _ExtentX        =   11986
         _ExtentY        =   450
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   3
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   4762
               MinWidth        =   4762
            EndProperty
            BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3175
               MinWidth        =   3175
            EndProperty
            BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3704
               MinWidth        =   3704
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.StatusBar StatusBar2 
         Height          =   255
         Left            =   -74880
         TabIndex        =   20
         Top             =   3510
         Width           =   5985
         _ExtentX        =   10557
         _ExtentY        =   450
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   3
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   4410
               MinWidth        =   4410
               Object.ToolTipText     =   "4"
            EndProperty
            BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3175
               MinWidth        =   3175
            EndProperty
            BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3175
               MinWidth        =   3175
            EndProperty
         EndProperty
      End
      Begin VB.Label Label5 
         Caption         =   "Last Update(s)"
         Height          =   255
         Left            =   7800
         TabIndex        =   75
         Top             =   2760
         Width           =   1125
      End
      Begin VB.Label Label4 
         Caption         =   "Remote Port"
         Height          =   285
         Left            =   -74880
         TabIndex        =   17
         Top             =   3210
         Width           =   915
      End
      Begin VB.Label Label3 
         Caption         =   "Remote IP"
         Height          =   225
         Left            =   -74880
         TabIndex        =   16
         Top             =   2910
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "Remote IP"
         Height          =   225
         Left            =   90
         TabIndex        =   9
         Top             =   2820
         Width           =   855
      End
      Begin VB.Label Label2 
         Caption         =   "Remote Port"
         Height          =   285
         Left            =   90
         TabIndex        =   8
         Top             =   3120
         Width           =   915
      End
   End
   Begin MSWinsockLib.Winsock Winsock2 
      Left            =   7620
      Top             =   3900
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      RemotePort      =   10200
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   7170
      Top             =   3900
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      RemotePort      =   5100
   End
End
Attribute VB_Name = "frmGHUDP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public nLowratePackets%, nStatusPackets%, Tlast&
Public Aline$, Bline$, Cline$, Dline$, Eline$
Function fTstringToSec(ByVal T$, ByVal ColonFlag As Boolean) As Long
Dim i%, hr%, Min%, sec%
'Convert a time string (T$) to seconds
'If Colon_Flag is TRUE, T$ format is assumed to be hh:mm:ss; otherwise, hhmmss
  
  If ColonFlag Then i% = 3 Else i% = 2
  hr = Val(Mid$(T$, 1, 2))
  Min = Val(Mid$(T$, 1 + i%, 2))
  sec = Val(Mid$(T$, 1 + 2 * i%, 2))
  fTstringToSec& = 3600# * hr + 60# * Min + sec

End Function
Sub EngData(A$)
'  Case "D"                           '  MUX()
Dim SU$, Mission$, Muxs!(0 To 32), Mux%(1 To 32), i%, j%
Dim Tifa!, Tlo!, Thi!, Ttgt!, Tnd!, Tlo1!, Tlo2!, Tmxr!, Twin!



  SU$ = "ER2T"
  Mission$ = "GloPac"
  
    Select Case SU$
    Case "DC8"
      If Mission$ = "SOLVE2" Or (Mission$ = "PAVE" And yyyymmdd$ = "20050109") Then
' Before 20041120 was 'If yyyymmdd$ > 20021100 Then'
' D line now contains raw counts again, and
' d line contains converted numbers which are ignored as they are converted
'
' 0        1         2         3         4         5         6         7         8         9         10        11
' 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
'     Tnd    Tamp   Tsyn   Tmix   Tt1    Tt2    ACC+   Twin   Tmot   XXX    Vref   Tdat1  Tdat2  VCC/2  15V/n  Trad  ACCm"
' D +39.30 +44.49 -89.57 +39.55 +03.40 +04.10 +01.05 +02.84 +35.01 +01.98 +02.05 -104.42 -104.42 +02.49 +01.91 +42.00  ACCm not added until after SOLVE2
' Channel Numbers are:
' 1 tND, 2 tLO1, 3 tLO2, 4 tIFA,5 TGTl,6 TGTh,7 ACCp 8 tWIN,9 tMTR,10 xxx,11 ref,12 tDC1,13 tDC2,14 tPS5,15 tPS12,16 Trad, 17 ACCm
        For i = 1 To 16: Muxs(i) = Mid$(A$, 2 + (i - 1) * 7, 7): Next i
      Else
        For j = 1 To 17: Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3)): Next j
      ' First 4 channels (different assignments in Old vs New format)
        For j = 1 To 17: Muxs(j) = ThermistorDC(j, Mux(j)): Next j
      End If
        
      Muxs(13) = Muxs(12)  'DC2 Make DC8 DC1 and DC2 same as ER2
      Muxs(12) = Muxs(11)  'DC1
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If yyyymmdd$ > 20021100 Then
        Ttgt = Tlo
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi   'DC8 used -33C xxx
      End If
    Case "ER2G"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = Thermistor(j, Mux(j)): Next j
      
    Case "ER2S"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = ThermistorER_1(j, Mux(j)): Next j
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If yyyymmdd$ > "20051100" Then
        If Tlo < -26 Then Ttgt = Tlo Else Ttgt = Thi   'New RTD circuit has new overlap region
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi   'DC8 used -33C xxx
      End If
      Twin = ThermistorER_1(8, Mux(8))
     'Debug.Print UTks; Tifa; Tlo; Thi; Ttgt
    Case "ER2T"
      For j = 1 To 16
        Mux(j) = fHexToDec(Mid$(A$, 3 + 4 * (j - 1), 3))
      Next j
      For j = 1 To 16: Muxs(j) = ThermistorER_2(j, Mux(j)): Next j
      Tifa = Muxs(4)
      Tlo = Muxs(5)
      Thi = Muxs(6)
      If yyyymmdd$ > "20070701" Then
        If Thi < 0# Then Ttgt = Tlo Else Ttgt = Thi    'range changed to +/-80 C for hot target
      Else
        If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi
      End If
      Twin = ThermistorER_2(8, Mux(8))
     
    Case Else
      Stop
    End Select
    Muxs(0) = Ttgt
  If Muxs(9) < -99 Then Muxs(9) = -99.9
  If Muxs(12) < -99 Then Muxs(12) = -99.9
  If Muxs(13) < -99 Then Muxs(13) = -99.9
  ' Muxs(i) Parameter
  ' 00 Ttgt  ' 01 Tnd  ' 02 Tlo1  ' 03 Tlo2
  ' 04 Tifa  ' 05 Tlo  ' 06 Thi   ' 07 ACC+
  ' 08 Twin  ' 09 Tmtr ' 10       ' 11 Vref
  ' 12 Tdc1  ' 13 Tdc2 ' 14 PS5   ' 15 PS12  ' 16 ACC-
  Tnd = Muxs(1)
  Tlo1 = Muxs(2)   'VCO=LO equivalences
  Tlo2 = Muxs(3)
  Tmxr = Muxs(4)  'IFA=MXR
  Tifa = Muxs(4)
  Tlo = Muxs(5) 'target low
  Thi = Muxs(6) 'target hi
  'If SU$ <> "DC8" Then
  '  If Thi < -37.5 Then Ttgt = Tlo Else Ttgt = Thi 'DC8 used -33C xxx
  'End If
  Muxs(0) = Ttgt
  
  Twin = Muxs(8) 'Window
  If Twin < -80 Or Twin > 45 Then Muxs(8) = Twin   '99
  Tmtr = Muxs(9)
  Vref = Muxs(11)    'Voltage reference for a/d to correct thermistors (=2.51 V)
  Tdc1 = Muxs(12)
  Tdc2 = Muxs(13)
  Vps5 = Muxs(14)
  Vps12 = Muxs(15)
'  dACC = (Muxs(7) - Muxs(16)) / 5.2  'dACC is in units of 0.01 g
  dACC = (Muxs(7) - Muxs(16)) / 52  'dACC is in units of 0.1 g
'  Muxs(17) = OATn
'  Muxs(18) = ALTkm * 3    Dline = True
  
  ' Write Engineering Temperatures
' If EnableEng Then
    txtMux(0).Text = Format$(Muxs(0), "##0.0")
  For i% = 1 To 16
    txtMux(i%).Text = Format$(Muxs(i%), "##0.0")
    txtRux(i% - 1).Text = Format$(Mux(i%), "####0")
  Next i%
' End If

End Sub
Function fHexToDec(HexNo$) As Integer
Dim V%
' Convert HexNo$ to Decimal Number
  V = Val("&H" + HexNo$)
  If V < 0 Then V = 65536 + V  'MSB is not sign, add 2^16
  fHexToDec = V
End Function



Sub WriteMTPcurrent(A$)
Dim OUTlu%

  OUTlu = FreeFile
  Open "C:\MTP\RTA\RAW\MTPcurrent.RAW" For Output As OUTlu
  Print #OUTlu, A$
  Close OUTlu
  
End Sub


Function fSubstituteCRLF(A$, s$) As String
Dim i%, j%, b$, C$
  
  C$ = ""
  j = 1
  Do
    i = InStr(j, A$, s$)
    If i = 0 Then
      C$ = C$ + Right$(A$, Len(A$) - j + 1)
      Exit Do
    End If
    b$ = Mid$(A$, j, i - j)
    C$ = C$ + b$ + vbCrLf
    j = i + 1
  Loop
  fSubstituteCRLF = C$
    
End Function


Private Sub cmdConnect_Click()
On Error GoTo ErrHandler
Dim LOGlu%

  With Winsock1
   'Set the remotehost property
   .RemoteHost = Trim(cboRemoteIP1.Text)
   'Set the remoteport property.
   'This should be equal to the localhost property of the remote machine.
   .RemotePort = Trim(cboRemotePort1.Text)
   'The localport property cannot be changed,so check if it has already been set.
   If .LocalPort = Empty Then
      .LocalPort = Trim(cboRemotePort1.Text)
      .Bind .LocalPort
   End If
  End With


' Make sure that the user can't change the local port
' txtLocalPort.Locked = True

' Show the current status of the connection in the status bar
  StatusBar1.Panels(1).Text = "  Connected to " & Winsock1.RemoteHost & "  "

  nStatusPackets = 0
  tmrUpdate.Enabled = True
  Tlast = Timer

Exit Sub

ErrHandler:
MsgBox "Winsock failed to establish connection with remote server", vbCritical

End Sub

Private Sub cmdExit_Click()
  
  Unload Me
  End

End Sub

Private Sub cmdGet_Click()
Dim INPlu%, A$, b$

        Inet1.URL = "ftp://ghoc.dfrc.nasa.gov"
        GoSub InetStatus
          
        Inet1.UserName = "mtp"
        GoSub InetStatus
          
        Inet1.Password = "emptypea"
        GoSub InetStatus
          
        Inet1.Execute , "CD " + "/home/mtp/public_html/"
        GoSub InetStatus
        
        If Len(Dir("C:\MTP\RTA\RAW\MTPcurrentGET.RAW")) > 0 Then Kill "C:\MTP\RTA\RAW\MTPcurrentGET.RAW"
        Inet1.Execute , "GET " & "MTPcurrent.RAW" & " " & "C:\MTP\RTA\RAW\MTPcurrentGET.RAW"   'HTML code
        GoSub InetStatus
        
        INPlu = FreeFile
        Open "C:\MTP\RTA\RAW\MTPcurrentGET.RAW" For Input As INPlu
        Do
          Line Input #INPlu, A$
          b$ = b$ + A$ + vbCrLf
        Loop Until EOF(INPlu)
        Close INPlu
        txtUDPin2.Text = b$
        
Exit Sub

InetStatus:
  Do While Inet1.StillExecuting
  DoEvents
  Loop
  'Debug.Print icError; Inet1.ResponseInfo
  Return

End Sub

Private Sub cmdLowrate_Click()
On Error GoTo ErrHandler
Dim LOGlu%

With Winsock2
   'Set the remotehost property
   .RemoteHost = Trim(cboRemoteIP2.Text)
   'Set the remoteport property.
   'This should be equal to the localhost property of the remote machine.
   .RemotePort = Trim(cboRemotePort2.Text)
   'The localport property cannot be changed,so check if it has already been set.
   If .LocalPort = Empty Then
      .LocalPort = Trim(cboRemotePort2.Text)
      .Bind .LocalPort
   End If
End With


'Make sure that the user can't change the local port
'txtLocalPort.Locked = True

'Show the current status of the connection in the status bar
StatusBar2.Panels(1).Text = "  Connected to " & Winsock2.RemoteHost & "  "

nLowratePackets = 0
Exit Sub

ErrHandler:
MsgBox "Winsock failed to establish connection with remote server", vbCritical

End Sub



Private Sub cmdPut_Click()
        Inet1.URL = "ftp://ghoc.dfrc.nasa.gov"
        GoSub InetStatus
          
        Inet1.UserName = "mtp"
        GoSub InetStatus
          
        Inet1.Password = "emptypea"
        GoSub InetStatus
          
        Inet1.Execute , "CD " + "/home/mtp/public_html/"
        GoSub InetStatus
        
        Inet1.Execute , "PUT " & "C:\MTP\RTA\RAW\MTPcurrent.RAW" & " " & "MTPcurrent.RAW"     'HTML code
        GoSub InetStatus
        
Exit Sub

InetStatus:
  Do While Inet1.StillExecuting
  DoEvents
  Loop
  'Debug.Print icError; Inet1.ResponseInfo
  Return
End Sub


Private Sub Form_Load()
  
  yyyymmdd$ = Right$(Date, 4) + Format(Month(Date), "00") + Format(Day(Date), "00")
  
  With Me
'  .Width = 4980
'  .Height = 3225
  .Left = (Screen.Width - .Width) / 2
  .Top = (Screen.Height - .Height) / 2
  End With
  
  With cboRemoteIP1
  .AddItem "130.134.183.222"
  'MTP IP
  .AddItem "10.3.1.200"
  .AddItem "10.3.1.210"
  .AddItem "10.3.1.255"
  ' IWG IP
  .AddItem "130.134.183.235"
  .ListIndex = 0
  End With
  
  With cboRemotePort1
  .AddItem "5100"    'Port for GH Status packets
  .AddItem "10200"
  .AddItem "10210"
  .AddItem "10300"
  .AddItem "10400"
  ' IWG Port
  .AddItem "7071"
  .ListIndex = 0
  End With

  tmrUpdate.Enabled = False
  tmrWatchdog.Enabled = True

End Sub


Private Sub tmrUpdate_Timer()
Dim Tcurrent&, DeltaT&
'Static Tlast&

  tmrUpdate.Enabled = False
  DeltaT = Timer - Tlast
  txtDeltaUT.Text = Str(DeltaT)
  tmrUpdate.Enabled = True
  
End Sub

Private Sub tmrWatchdog_Timer()
' Watchdog timer to make sure that MTP is working properly
Dim INPlu%, CurrentSec&, PathRAW$, FilenameRAW$, PathWWW$, A$, b$
Dim CurrentStamp As Date
Dim CountDifference%, i%, j%, k%, n1%, n2%, n3%, dn%, LOok As Boolean, dSec&
Static LastSec&, LastStamp As Date

  tmrWatchdog.Enabled = False
  dn = 30 'count threshold
  PathRAW = "C:/MTP/RTA/RAW/"
  FilenameRAW = "MTPcurrent.RAW"
  PathWWW = "C:\MTP\WWW\missions\realtime\"
  
  If chkMTPcurrentRAW.Value = 1 Then
    CurrentStamp = FileDateTime(PathRAW + FilenameRAW)
    If LastStamp = Empty Then LastStamp = CurrentStamp
    If CurrentStamp > LastStamp Then dSec = 0: LastSec = 0
    CurrentSec = Timer
    If LastSec = 0 Then LastSec = CurrentSec
    If CurrentSec > LastSec Then
      dSec = CurrentSec - LastSec
    End If
    If dSec > 16 And LastStamp = CurrentStamp Then
      txtMTPcurrentRAW.BackColor = &HFF&   'red
    Else
      txtMTPcurrentRAW.BackColor = &HFF00& 'green
      
    End If
    txtMTPcurrentRAW.Text = Str(CurrentStamp) + " -- Elapsed Time: " + Str(dSec) + " sec"
    DoEvents
    LastStamp = CurrentStamp
  End If
'Non-GV
'AG103:13:59:11 +35.126 -117.508 341.2  -7.5   9.3  5574  5483   0.0  +0.0 +0.00 -175.1 1 224 100414 135828
'B  8013  9599 11819  8071  9621 11823  8093  9628 11823  8150  9665 11840  8176  9689 11856
'C  8200  9698 11845  8238  9729 11874  8257  9744 11877  8284  9766 11912  8299  9780 11929
'D 474 599 60F 46B D5E 000 821 911 690 825 655 695 34E 7D3 7D3 7DA
'E  9613 11527 13516  8320  9827 11977
'
'GV
'A 20100316 20:32:19 -01.04 00.00 +00.31 00.00 +01.60 0.00 290.79 00.28 +39.913 +0.000 -105.118 +0.000 +072574 +072560
'IWG1,20100316T203218,39.9128,-105.118,1718.66,,5249.65,,1.00288,0.447474,0.784894,0.00130787,-0.0011557,36.4897,nan,nan,-1.04252,0.30899,0,0,17.9387,-14.856,18.1453,835.156,0.001,,0,0,0,45.6079,44.3921,,
'B 018492 020137 019264 018510 020150 019284 018529 020165 019297 018551 020183 019315 018603 020237 019383 018669 020325 019464 018909 020580 019766 018957 020634 019818 018945 020622 019807 018906 020579 019764
'M01: 2928 2337 2928 3087 1922 2922 2434 2948
'M02: 2011 1224 2131 2194 2317 1179 4095 1367
'Pt: 2157 13799 13808 10995 13404 13383 13254 14443
'E 020928 022670 022208 019199 020912 020145

  If chkLOchanging.Value = 1 Then
    INPlu = FreeFile
    Open PathRAW + FilenameRAW For Input As INPlu
    If EOF(INPlu) Then
      MsgBox PathRAW + FilenameRAW + " has reached an EOF!", vbOKOnly
      tmrWatchdog.Enabled = True
      Close INPlu
      Exit Sub
    End If
    Do
      Line Input #INPlu, A$
    Loop Until Left$(A$, 1) = "B" Or EOF(INPlu)
    Close INPlu
    If Len(A$) < 30 Then
      tmrWatchdog.Enabled = True
      Exit Sub
    End If
    txtLOchanging.Text = A$
    i% = InStr(4, A$, " ")
    n1 = Val(Mid$(A$, 3, i - 3))
    j% = InStr(i + 2, A$, " ")
    n2 = Val(Mid$(A$, i + 1, j - i - 1))
    k% = InStr(j + 2, A$, " ")
    n3 = Val(Mid$(A$, j + 1, k - j - 1))
    LOok = True
    If Abs(n1 - n2) < dn And Abs(n2 - n3) < dn Then LOok = False
    If Not LOok Then
      txtLOchanging.BackColor = &HFF&   'red
    Else
      txtLOchanging.BackColor = &HFF00& 'green
    End If
  End If
  
  tmrWatchdog.Enabled = True
End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'A$ is the text that has just arrived from across the socket
Dim A$, STAlu%
Static LOGlu%

  
' Get the new text
  Winsock1.GetData A$
' MTP,20100413T164025.000,259,9663,11363,13389,8390,9690,11900,7463,8594,10988,-0.8,-4.632350e+01,4.557010e+01,1.400000e+01,424
' 1234567890123456789012345
  If Left$(A$, 4) = "MTP," Then
    If chkLOGstatus.Value = 1 And LOGlu = 0 Then
      LOGlu = FreeFile
      Open "C:\MTP\RTA\RAW\MTPStatusData_Log.txt" For Output As LOGlu
    End If

    If chkLOGstatus.Value = 1 Then Print #LOGlu, Time & " " & Mid$(A$, 14, 6) & " " & A$
  
' Show the new text
'    A$ = Right$(A$, Len(A$) - InStr(1, A$, ","))  'strip "MTP,"
    Tlast = Timer
    txtUDPin1.Text = A$
    Debug.Print Time & " " & Mid$(A$, 14, 6)
    If chkWriteMTPstatus.Value = 1 Then
      STAlu = FreeFile
      Open "C:\MTP\RTA\RAW\MTPstatus.txt" For Output As STAlu
      Print #STAlu, A$
      Close STAlu
    End If
'   Show the byte size of this transmission and packet count in the statusbar
    StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
    nStatusPackets = nStatusPackets + 1
    StatusBar1.Panels(3).Text = " UDP Packets = " & Str(nStatusPackets)

  Else 'Check if non-MTP packets should be shown
    If chkAllpackets.Value = 1 Then txtUDPin1.Text = A$
  End If
  
'  DoEvents
  A$ = ""
  bytesTotal = 0
End Sub

Private Sub Winsock2_DataArrival(ByVal bytesTotal As Long)
'A$ is the text that has just arrived from across the socket
Dim A$
Static LOGlu%

  If chkLOGlowrate.Value = 1 And nLowratePackets = 0 Then
    LOGlu = FreeFile
    Open "C:\MTP\RTA\RAW\MTP_LowrateData_Log.txt" For Output As LOGlu
  End If

' Get the new text
  Winsock2.GetData A$
  If chkLOGlowrate.Value = 1 Then Print #LOGlu, A$
  
' Show the new text
  If Left$(A$, 5) = "MTP,A" Then
    A$ = Right$(A$, Len(A$) - InStr(1, A$, ","))  'strip "MTP,"
'   Extract E line
    i = InStr(1, A$, ",D ")
    j = InStr(i + 1, A$, ",")
    Eline$ = Mid$(A$, i + 1, j - i - 1)
    Call EngData(Eline$)
    
    A$ = fSubstituteCRLF(A$, ",")
    txtUDPin2.Text = A$
    Debug.Print A$
    If chkWriteMTPcurrent.Value = 1 Then
      Call WriteMTPcurrent(A$)
      If chkFTPlowrate.Value = 1 Then
        Inet1.URL = "ftp://ghoc.dfrc.nasa.gov"
        GoSub InetStatus
          
        Inet1.UserName = "mtp"
        GoSub InetStatus
          
        Inet1.Password = "emptypea"
        GoSub InetStatus
          
        Inet1.Execute , "CD " + "/home/mtp/public_html/"
        GoSub InetStatus
        
        Inet1.Execute , "PUT " & "C:\MTP\RTA\RAW\MTPcurrent.RAW" & " " & "MTPcurrent.RAW"     'HTML code
        GoSub InetStatus
        
      End If
    End If
  Else 'Check if non-MTP packets should be shown
    txtUDPin2.Text = A$
  End If
' Show the byte size of this transmission and packet count in the statusbar
  StatusBar2.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
  nLowratePackets = nLowratePackets + 1
  StatusBar2.Panels(3).Text = " UDP Packets = " & Str(nLowratePackets)
  Exit Sub
  
InetStatus:
  Do While Inet1.StillExecuting
  DoEvents
  Loop
  'Debug.Print icError; Inet1.ResponseInfo
  Return

End Sub

