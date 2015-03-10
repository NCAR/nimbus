VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form 1"
   ClientHeight    =   3045
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6930
   LinkTopic       =   "Form1"
   ScaleHeight     =   3045
   ScaleWidth      =   6930
   StartUpPosition =   3  'Windows Default
   Begin TabDlg.SSTab SSTab1 
      Height          =   2985
      Left            =   30
      TabIndex        =   0
      Top             =   30
      Width           =   6855
      _ExtentX        =   12091
      _ExtentY        =   5265
      _Version        =   393216
      TabHeight       =   520
      TabCaption(0)   =   "Tab 0"
      TabPicture(0)   =   "Controls.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label1"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Frame1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Command1"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Command2"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Combo1"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Text1"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "List1"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "Check1"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).ControlCount=   8
      TabCaption(1)   =   "Tab 1"
      TabPicture(1)   =   "Controls.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).ControlCount=   0
      TabCaption(2)   =   "Tab 2"
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
      Begin VB.CheckBox Check1 
         Caption         =   "Check Box"
         Height          =   255
         Left            =   2970
         TabIndex        =   9
         Top             =   2490
         Width           =   1395
      End
      Begin VB.ListBox List1 
         Height          =   1425
         ItemData        =   "Controls.frx":0038
         Left            =   2940
         List            =   "Controls.frx":003F
         TabIndex        =   7
         Top             =   930
         Width           =   3855
      End
      Begin VB.TextBox Text1 
         Height          =   345
         Left            =   4770
         TabIndex        =   6
         Text            =   "Text Box"
         Top             =   540
         Width           =   1935
      End
      Begin VB.ComboBox Combo1 
         Height          =   315
         Left            =   2910
         TabIndex        =   5
         Text            =   "Combo Box"
         Top             =   540
         Width           =   1785
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Button 2"
         Height          =   405
         Left            =   5760
         TabIndex        =   4
         Top             =   2460
         Width           =   1035
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Button 1"
         Height          =   405
         Left            =   4650
         TabIndex        =   3
         Top             =   2460
         Width           =   1035
      End
      Begin VB.Frame Frame1 
         Caption         =   "Frame1"
         Height          =   1935
         Left            =   150
         TabIndex        =   1
         Top             =   420
         Width           =   2715
         Begin VB.OptionButton Option2 
            Caption         =   "Option2"
            Height          =   255
            Left            =   150
            TabIndex        =   8
            Top             =   630
            Width           =   885
         End
         Begin VB.OptionButton Option1 
            Caption         =   "Option1"
            Height          =   315
            Left            =   150
            TabIndex        =   2
            Top             =   330
            Width           =   2475
         End
      End
      Begin VB.Label Label1 
         Caption         =   "Label1"
         Height          =   225
         Left            =   180
         TabIndex        =   10
         Top             =   2460
         Width           =   1455
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
