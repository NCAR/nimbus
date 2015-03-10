VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form MTPcalc 
   Caption         =   "SoftMJ Conversion Utility"
   ClientHeight    =   2025
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   5955
   Icon            =   "MTPcalc.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2025
   ScaleWidth      =   5955
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdClose 
      Caption         =   "E&xit"
      Height          =   495
      Left            =   4680
      TabIndex        =   48
      Top             =   1200
      Width           =   1215
   End
   Begin VB.CommandButton cmdRAOBsites 
      Caption         =   "&RAOB Sites"
      Height          =   495
      Left            =   4680
      TabIndex        =   47
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton cmdCalculate 
      Caption         =   "&Calculate"
      Height          =   495
      Left            =   4680
      TabIndex        =   4
      Top             =   0
      Width           =   1215
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   1965
      Left            =   30
      TabIndex        =   0
      Top             =   30
      Width           =   4575
      _ExtentX        =   8070
      _ExtentY        =   3466
      _Version        =   393216
      Tabs            =   10
      Tab             =   1
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   "&Std Alt"
      TabPicture(0)   =   "MTPcalc.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "txtInValue(0)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "txtOutValue(0)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "cmbOutUnit(0)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "cmbInUnit(0)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "lblLtoR(0)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).ControlCount=   5
      TabCaption(1)   =   "&Length"
      TabPicture(1)   =   "MTPcalc.frx":045E
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "lblLtoR(1)"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "cmbOutUnit(1)"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "cmbInUnit(1)"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "txtOutValue(1)"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "txtInValue(1)"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).ControlCount=   5
      TabCaption(2)   =   "&Mass"
      TabPicture(2)   =   "MTPcalc.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "txtInValue(2)"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "txtOutValue(2)"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "cmbInUnit(2)"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "cmbOutUnit(2)"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "lblLtoR(2)"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).ControlCount=   5
      TabCaption(3)   =   "&Time"
      TabPicture(3)   =   "MTPcalc.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "txtInValue(3)"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "txtOutValue(3)"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "cmbInUnit(3)"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "cmbOutUnit(3)"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).Control(4)=   "lblLtoR(3)"
      Tab(3).Control(4).Enabled=   0   'False
      Tab(3).ControlCount=   5
      TabCaption(4)   =   "&Velocity"
      TabPicture(4)   =   "MTPcalc.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "txtInValue(4)"
      Tab(4).Control(0).Enabled=   0   'False
      Tab(4).Control(1)=   "txtOutValue(4)"
      Tab(4).Control(1).Enabled=   0   'False
      Tab(4).Control(2)=   "cmbInUnit(4)"
      Tab(4).Control(2).Enabled=   0   'False
      Tab(4).Control(3)=   "cmbOutUnit(4)"
      Tab(4).Control(3).Enabled=   0   'False
      Tab(4).Control(4)=   "lblLtoR(4)"
      Tab(4).Control(4).Enabled=   0   'False
      Tab(4).ControlCount=   5
      TabCaption(5)   =   "&Pressure"
      TabPicture(5)   =   "MTPcalc.frx":04CE
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "txtInValue(5)"
      Tab(5).Control(0).Enabled=   0   'False
      Tab(5).Control(1)=   "txtOutValue(5)"
      Tab(5).Control(1).Enabled=   0   'False
      Tab(5).Control(2)=   "cmbInUnit(5)"
      Tab(5).Control(2).Enabled=   0   'False
      Tab(5).Control(3)=   "cmbOutUnit(5)"
      Tab(5).Control(3).Enabled=   0   'False
      Tab(5).Control(4)=   "lblLtoR(5)"
      Tab(5).Control(4).Enabled=   0   'False
      Tab(5).ControlCount=   5
      TabCaption(6)   =   "&DOY"
      TabPicture(6)   =   "MTPcalc.frx":04EA
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "txtOutValue(6)"
      Tab(6).Control(0).Enabled=   0   'False
      Tab(6).Control(1)=   "txtInValue(6)"
      Tab(6).Control(1).Enabled=   0   'False
      Tab(6).Control(2)=   "Label(1)"
      Tab(6).Control(2).Enabled=   0   'False
      Tab(6).Control(3)=   "Label(0)"
      Tab(6).Control(3).Enabled=   0   'False
      Tab(6).Control(4)=   "lblLtoR(6)"
      Tab(6).Control(4).Enabled=   0   'False
      Tab(6).ControlCount=   5
      TabCaption(7)   =   "Std Den"
      TabPicture(7)   =   "MTPcalc.frx":0506
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "txtLatitude"
      Tab(7).Control(0).Enabled=   0   'False
      Tab(7).Control(1)=   "cmbInUnit(7)"
      Tab(7).Control(1).Enabled=   0   'False
      Tab(7).Control(2)=   "cmbOutUnit(7)"
      Tab(7).Control(2).Enabled=   0   'False
      Tab(7).Control(3)=   "txtOutValue(7)"
      Tab(7).Control(3).Enabled=   0   'False
      Tab(7).Control(4)=   "txtInValue(7)"
      Tab(7).Control(4).Enabled=   0   'False
      Tab(7).Control(5)=   "cmbMonth"
      Tab(7).Control(5).Enabled=   0   'False
      Tab(7).Control(6)=   "Label(4)"
      Tab(7).Control(6).Enabled=   0   'False
      Tab(7).Control(7)=   "Label(3)"
      Tab(7).Control(7).Enabled=   0   'False
      Tab(7).Control(8)=   "Label(2)"
      Tab(7).Control(8).Enabled=   0   'False
      Tab(7).Control(9)=   "lblLtoR(7)"
      Tab(7).Control(9).Enabled=   0   'False
      Tab(7).ControlCount=   10
      TabCaption(8)   =   "PT"
      TabPicture(8)   =   "MTPcalc.frx":0522
      Tab(8).ControlEnabled=   0   'False
      Tab(8).Control(0)=   "frame1"
      Tab(8).Control(0).Enabled=   0   'False
      Tab(8).ControlCount=   1
      TabCaption(9)   =   "T"
      TabPicture(9)   =   "MTPcalc.frx":053E
      Tab(9).ControlEnabled=   0   'False
      Tab(9).Control(0)=   "cmbOutUnit(9)"
      Tab(9).Control(0).Enabled=   0   'False
      Tab(9).Control(1)=   "cmbInUnit(9)"
      Tab(9).Control(1).Enabled=   0   'False
      Tab(9).Control(2)=   "txtOutValue(9)"
      Tab(9).Control(2).Enabled=   0   'False
      Tab(9).Control(3)=   "txtInValue(9)"
      Tab(9).Control(3).Enabled=   0   'False
      Tab(9).Control(4)=   "lblLtoR(9)"
      Tab(9).Control(4).Enabled=   0   'False
      Tab(9).ControlCount=   5
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   9
         Left            =   -71520
         TabIndex        =   56
         Text            =   "C"
         Top             =   840
         Width           =   855
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   9
         ItemData        =   "MTPcalc.frx":055A
         Left            =   -73920
         List            =   "MTPcalc.frx":055C
         TabIndex        =   55
         Text            =   "K"
         Top             =   840
         Width           =   870
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   9
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   54
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   9
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   53
         Top             =   840
         Width           =   975
      End
      Begin VB.Frame frame1 
         Caption         =   "Solve For"
         Height          =   855
         Left            =   -74880
         TabIndex        =   49
         Top             =   720
         Width           =   4215
         Begin VB.TextBox txtInValue 
            Height          =   315
            Index           =   8
            Left            =   120
            TabIndex        =   63
            Top             =   480
            Width           =   735
         End
         Begin VB.TextBox txtOutValue 
            Height          =   315
            Index           =   8
            Left            =   2880
            TabIndex        =   62
            Top             =   480
            Width           =   735
         End
         Begin VB.ComboBox cmbInUnit 
            Height          =   315
            Index           =   8
            ItemData        =   "MTPcalc.frx":055E
            Left            =   840
            List            =   "MTPcalc.frx":0560
            TabIndex        =   61
            Text            =   "K"
            Top             =   480
            Width           =   510
         End
         Begin VB.ComboBox cmbOutUnit 
            Height          =   315
            Index           =   8
            Left            =   3600
            TabIndex        =   60
            Text            =   "K"
            Top             =   480
            Width           =   495
         End
         Begin VB.TextBox txtPTpressure 
            Height          =   285
            Left            =   1440
            TabIndex        =   59
            Top             =   480
            Width           =   735
         End
         Begin VB.ComboBox cmbPTpressure 
            Height          =   315
            Left            =   2160
            TabIndex        =   58
            Text            =   "mb"
            Top             =   480
            Width           =   615
         End
         Begin VB.OptionButton optPT 
            Caption         =   "Pressure"
            Height          =   195
            Index           =   2
            Left            =   1440
            TabIndex        =   52
            Top             =   240
            Width           =   975
         End
         Begin VB.OptionButton optPT 
            Caption         =   "Temperature"
            Height          =   195
            Index           =   1
            Left            =   120
            TabIndex        =   51
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optPT 
            Caption         =   "Potential T"
            Height          =   195
            Index           =   0
            Left            =   2880
            TabIndex        =   50
            Top             =   240
            Width           =   1095
         End
      End
      Begin VB.TextBox txtLatitude 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   -74280
         MultiLine       =   -1  'True
         TabIndex        =   43
         Top             =   720
         Width           =   615
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   7
         ItemData        =   "MTPcalc.frx":0562
         Left            =   -73680
         List            =   "MTPcalc.frx":0564
         TabIndex        =   42
         Text            =   "mb"
         Top             =   1080
         Width           =   750
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   7
         Left            =   -71520
         TabIndex        =   40
         Text            =   "#/cm3"
         Top             =   1080
         Width           =   855
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   7
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   39
         Top             =   1080
         Width           =   975
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   7
         Left            =   -74280
         MultiLine       =   -1  'True
         TabIndex        =   38
         Top             =   1080
         Width           =   615
      End
      Begin VB.ComboBox cmbMonth 
         Height          =   315
         Left            =   -71520
         TabIndex        =   37
         Text            =   "Jan"
         Top             =   720
         Width           =   855
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   6
         Left            =   -72720
         MultiLine       =   -1  'True
         TabIndex        =   33
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   6
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   32
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   5
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   31
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   5
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   30
         Top             =   840
         Width           =   975
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   5
         ItemData        =   "MTPcalc.frx":0566
         Left            =   -73920
         List            =   "MTPcalc.frx":0568
         TabIndex        =   28
         Text            =   "mb"
         Top             =   840
         Width           =   870
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   5
         Left            =   -71520
         TabIndex        =   27
         Text            =   "mm Hg"
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   4
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   26
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   4
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   25
         Top             =   840
         Width           =   975
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   4
         ItemData        =   "MTPcalc.frx":056A
         Left            =   -73920
         List            =   "MTPcalc.frx":056C
         TabIndex        =   23
         Text            =   "mb"
         Top             =   840
         Width           =   870
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   4
         Left            =   -71520
         TabIndex        =   22
         Text            =   "mm Hg"
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   3
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   21
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   3
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   20
         Top             =   840
         Width           =   975
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   3
         ItemData        =   "MTPcalc.frx":056E
         Left            =   -73920
         List            =   "MTPcalc.frx":0570
         TabIndex        =   18
         Text            =   "mb"
         Top             =   840
         Width           =   870
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   3
         Left            =   -71520
         TabIndex        =   17
         Text            =   "mm Hg"
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   2
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   16
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   2
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   15
         Top             =   840
         Width           =   975
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   2
         ItemData        =   "MTPcalc.frx":0572
         Left            =   -73920
         List            =   "MTPcalc.frx":0574
         TabIndex        =   13
         Text            =   "mb"
         Top             =   840
         Width           =   870
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   2
         Left            =   -71520
         TabIndex        =   12
         Text            =   "mm Hg"
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   1
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   11
         Top             =   840
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   1
         Left            =   2520
         MultiLine       =   -1  'True
         TabIndex        =   10
         Top             =   840
         Width           =   975
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   1
         ItemData        =   "MTPcalc.frx":0576
         Left            =   1080
         List            =   "MTPcalc.frx":0578
         TabIndex        =   8
         Text            =   "mb"
         Top             =   840
         Width           =   870
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   1
         Left            =   3480
         TabIndex        =   7
         Text            =   "mm Hg"
         Top             =   840
         Width           =   855
      End
      Begin VB.TextBox txtInValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   0
         Left            =   -74880
         MultiLine       =   -1  'True
         TabIndex        =   6
         Top             =   780
         Width           =   975
      End
      Begin VB.TextBox txtOutValue 
         Alignment       =   1  'Right Justify
         Height          =   315
         Index           =   0
         Left            =   -72480
         MultiLine       =   -1  'True
         TabIndex        =   5
         Top             =   780
         Width           =   975
      End
      Begin VB.ComboBox cmbOutUnit 
         Height          =   315
         Index           =   0
         Left            =   -71520
         TabIndex        =   2
         Text            =   "mm Hg"
         Top             =   780
         Width           =   855
      End
      Begin VB.ComboBox cmbInUnit 
         Height          =   315
         Index           =   0
         ItemData        =   "MTPcalc.frx":057A
         Left            =   -73920
         List            =   "MTPcalc.frx":057C
         TabIndex        =   1
         Text            =   "mb"
         Top             =   780
         Width           =   870
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   9
         Left            =   -72960
         TabIndex        =   57
         Top             =   840
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "Month"
         Height          =   255
         Index           =   4
         Left            =   -72480
         TabIndex        =   46
         Top             =   720
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   " Deg"
         Height          =   255
         Index           =   3
         Left            =   -73440
         TabIndex        =   45
         Top             =   720
         Width           =   735
      End
      Begin VB.Label Label 
         Caption         =   "Latitude"
         Height          =   255
         Index           =   2
         Left            =   -74880
         TabIndex        =   44
         Top             =   720
         Width           =   615
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   7
         Left            =   -72840
         TabIndex        =   41
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "YYYYMMDD"
         Height          =   255
         Index           =   1
         Left            =   -71640
         TabIndex        =   36
         Top             =   840
         Width           =   975
      End
      Begin VB.Label Label 
         Caption         =   "YYYYDOY"
         Height          =   255
         Index           =   0
         Left            =   -73920
         TabIndex        =   35
         Top             =   840
         Width           =   855
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   6
         Left            =   -73080
         TabIndex        =   34
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   5
         Left            =   -72960
         TabIndex        =   29
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   4
         Left            =   -72960
         TabIndex        =   24
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   3
         Left            =   -72960
         TabIndex        =   19
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   2
         Left            =   -72960
         TabIndex        =   14
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   1
         Left            =   2040
         TabIndex        =   9
         Top             =   840
         Width           =   255
      End
      Begin VB.Label lblLtoR 
         BackColor       =   &H00C0C0C0&
         Caption         =   "=>"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   0
         Left            =   -72960
         TabIndex        =   3
         Top             =   780
         Width           =   255
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsSD 
         Caption         =   "&SigDigits"
         Begin VB.Menu mnuSD 
            Caption         =   "&0"
            Index           =   0
         End
         Begin VB.Menu mnuSD 
            Caption         =   "&1"
            Index           =   1
         End
         Begin VB.Menu mnuSD 
            Caption         =   "&2"
            Index           =   2
         End
         Begin VB.Menu mnuSD 
            Caption         =   "&3"
            Checked         =   -1  'True
            Index           =   3
         End
         Begin VB.Menu mnuSD 
            Caption         =   "&4"
            Index           =   4
         End
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "MTPcalc"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim LeftToRight(0 To 9) As Boolean
Dim fmt$

Const mb_mmHg! = 1000 / 760
Const mb_torr! = 1000 / 760
Const mb_kPa! = 10#

Const m_km! = 1000
Const m_mi! = 1609#
Const m_nm! = 1852
Const m_kft! = 304.69

Const atm_mmHg = 1 / 760     '0 Celsius
Const atm_mb = 1 / 1013.25
Const atm_ftH2O = 1 / 33.9   '4 Celcius
Const atm_inHg = 1 / 29.92   '0 Celcius
Const atm_kPa = 1 / 101.325
Const atm_psi = 1 / 14.7
Const atm_torr = atm_mmHg



Const m_ft! = 0.3048


Const kg_gm! = 0.001
Const kg_lb! = 0.4536
Const kg_oz! = kg_lb / 16

Const sec_min = 60
Const sec_hr! = sec_min * 60
Const sec_day! = sec_hr * 24
Const sec_mon! = sec_day * 30
Const sec_yr! = sec_day * 365


Const kph_kts = 1 / 0.54
Const kph_mph = 1 / 0.6214
Const kph_fps = 1 / 0.91139
Const kph_mps = 1 / 0.27779

Private Sub cmdCalculate_Click()
Dim x!, y!, TabIndex%, k1!, k2!, XD#, YD#, i%
Dim Iyear&, Idoy&, Imonth&, Iday&, Idate As Date, ii%
Dim lu%, Filename$, Latitude!, iLAT%, Line1$, Line2$, Tt$
Dim Rho!, Rho1!, Rho2!, x1!, x2!, Pmb!, P!, T!, Tk!, pt!, PTK!

TabIndex = SSTab1.Tab

If LeftToRight(TabIndex) Then
  x = Val(txtInValue(TabIndex).Text)
Else
  y = Val(txtOutValue(TabIndex).Text)
End If

Select Case TabIndex
Case 0  'Standard Atmosphere
  Select Case cmbInUnit(0).Text
  Case "atm": k1 = 1
  Case "mb", "hPa": k1 = atm_mb
  Case "mm Hg": k1 = atm_mmHg
  Case "in Hg": k1 = atm_inHg
  Case "ft H2O": k1 = atm_ftH2O
  Case "psi": k1 = atm_psi
  Case "torr:k1= atm_torr"
  Case "kPa": k1 = atm_kPa
  End Select
  
  Select Case cmbOutUnit(0).Text
  Case "m": k2 = 1
  Case "km": k2 = m_km
  Case "mi": k2 = m_mi
  Case "nm": k2 = m_nm
  Case "kft": k2 = m_kft
  End Select
  
  If LeftToRight(TabIndex) Then
    x = x * k1 / atm_mb   'Convert [atm] to [mb]
    y = fPtoZ(x) * 1000   'Convert P[mb] to Z[m]
    y = y / k2
    txtOutValue(TabIndex).Text = Format(y, fmt$)
  Else
    y = y * k2 / 1000   'Convert m to km
    x = fZtoP(y)        'Convert Z[km] to P[mb]
    x = x * atm_mb      'Convert P[mb] to P[atm]
    x = x / k1
    txtInValue(TabIndex).Text = Format(x, fmt$)
  End If
  Exit Sub
  
Case 1  'Unit Conversion

  Select Case cmbInUnit(1).Text
  Case "m": k1 = 1
  Case "km": k1 = m_km
  Case "mi": k1 = m_mi
  Case "nm": k1 = m_nm
  Case "ft": k1 = m_ft
  Case "kft": k1 = m_kft
  End Select

' Convert length to displayed unit
  Select Case cmbOutUnit(1).Text
  Case "m": k2 = 1
  Case "km": k2 = m_km
  Case "mi": k2 = m_mi
  Case "nm": k2 = m_nm
  Case "ft": k2 = m_ft
  Case "kft": k2 = m_kft
  End Select
  
Case 2 'Mass
  Select Case cmbInUnit(2).Text
  Case "kg": k1 = 1
  Case "g": k1 = kg_g
  Case "lb": k1 = kg_lb
  Case "oz": k1 = kg_oz
  End Select
  
  Select Case cmbOutUnit(2).Text
  Case "kg": k2 = 1
  Case "g": k2 = kg_g
  Case "lb": k2 = kg_lb
  Case "oz": k2 = kg_oz
  End Select
  
Case 3 ' Convert Time
  Select Case cmbInUnit(3).Text
  Case "s": k1 = 1
  Case "m": k1 = sec_min
  Case "h": k1 = sec_hr
  Case "dy": k1 = sec_day
  Case "mo": k1 = sec_mon
  Case "yr": k1 = sec_yr
  Case "h:m:s": k1 = 1
    y1 = InStr(1, txtInValue(3).Text, ":")
    y2 = InStr(y1 + 1, txtInValue(3).Text, ":")
    H = 0: M = 0: s = 0: Tt$ = txtInValue(3).Text
    If y1 = 0 Then
      s = Val(Tt$)
    Else
      If y2 = 0 Then
        M = Val(Left$(Tt$, y1 - 1))
        s = Val(Right$(Tt$, Len(Tt$) - y1))
      Else
        H = Val(Left$(Tt$, y1 - 1))
        M = Val(Mid$(Tt$, y1 + 1, y2 - 1))
        s = Val(Right$(Tt$, Len(Tt$) - y2))
      End If
    End If
    x = 3600 * H + 60 * M + s
  End Select

' Convert time to displayed unit
  Select Case cmbOutUnit(3).Text
  Case "s": k2 = 1
  Case "m": k2 = sec_min
  Case "h": k2 = sec_hr
  Case "dy": k2 = sec_day
  Case "mo": k2 = sec_mon
  Case "yr": k2 = sec_yr
  Case "h:m:s": k2 = 1
    y = x / 86400   'Convert to serial number
    txtOutValue(TabIndex).Text = Format(y, "hh:mm:ss")
    Exit Sub  'Only works left to right, NOT right to left
  End Select
  
Case 4    'Convert velocity  Select Case cmbInUnit.Text
  Select Case cmbInUnit(4).Text
  Case "km/hr": k1 = 1
  Case "knots": k1 = kph_kts
  Case "mph": k1 = kph_mph
  Case "ft/s": k1 = kph_fps
  Case "m/s": k1 = kph_mps
  End Select

  Select Case cmbOutUnit(4).Text
  Case "km/hr": k2 = 1
  Case "knots": k2 = kph_kts
  Case "mph": k2 = kph_mph
  Case "ft/s": k2 = kph_fps
  Case "m/s": k2 = kph_mps
  End Select

Case 5  'Convert Pressure
  Select Case cmbInUnit(5).Text
  Case "mb", "hPa": k1 = atm_mb
  Case "atm": k1 = 1
  Case "mm Hg": k1 = atm_mmHg
  Case "in Hg": k1 = atm_inHg
  Case "ft H2O": k1 = atm_ftH2O
  Case "psi": k1 = atm_psi
  Case "torr:k1= atm_torr"
  Case "kPa": k1 = atm_kPa
  End Select

  Select Case cmbOutUnit(5).Text
  Case "mb", "hPa": k2 = atm_mb
  Case "atm": k2 = 1
  Case "mm Hg": k2 = atm_mmHg
  Case "in Hg": k2 = atm_inHg
  Case "ft H2O": k2 = atm_ftH2O
  Case "psi": k2 = atm_psi
  Case "torr": k2 = atm_torr
  Case "kPa": k2 = atm_kPa
  End Select

Case 6
  If LeftToRight(TabIndex) Then
    XD = Val(txtInValue(6).Text)
    Iyear = Int(XD / 1000)
    Idoy = Int(XD - Iyear * 1000)
    Imonth = DatePart("M", DateSerial(Iyear, 1, Idoy))
    Iday = DatePart("D", DateSerial(Iyear, 1, Idoy))
    XD = 10000 * Iyear + 100 * Imonth + Iday
    txtOutValue(TabIndex).Text = Format(XD, "00000000")
  Else
    YD = Val(txtOutValue(6).Text)
    Iyear = Int(YD / 10000)
    Imonth = Int((YD - Iyear * 10000) / 100)
    Iday = Int(YD - Iyear * 10000 - Imonth * 100)
    Idate = DateSerial(Iyear, Imonth, Iday)
    Idoy = DatePart("Y", Idate)
    YD = 1000 * Iyear + Idoy
    txtInValue(TabIndex).Text = Format(YD, "0000000")
  End If
  
  Exit Sub
  
Case 7
  Latitude = Val(txtLatitude.Text)
  'Next convert altitude to km if necessary
  Select Case cmbInUnit(7).Text
  Case "mb": x = fPtoZ(x)
  Case "km": x = x
  Case "kft": x = x / 3.2808
  End Select
  'Now determine month to use
  i = cmbMonth.ListIndex
  i = i + 1
  Rho = fStandardDensity(i, x, Latitude)
  
  Select Case cmbOutUnit(7).Text
  Case "#/cm3": y = Rho * 2.08E+19
  Case "g/cm3": y = Rho / 1000
  Case "kg/m3": y = Rho
  Case Else
    y = 0#
  End Select
  txtOutValue(7).Text = Format(y, "0.000E+00")
  Close lu
  Exit Sub
  
Case 8  'Potential Temperature
  If optPT(0).Value = True Then 'T,P to PT
    T = Val(txtInValue(8).Text)
    Select Case cmbInUnit(8).Text  'convert to K
    Case "K"
      Tk = T
    Case "C"
      Tk = T + cTo
    Case "F"
      Tk = (T - 32#) * 5 / 9 + cTo
    End Select
    P = Val(txtPTpressure.Text)
    Select Case cmbPTpressure.Text 'convert to mb
    Case "mb"
      Pmb = P
    Case "km"
      Pmb = fZtoP(P)
    Case "kft"
      Pmb = fZtoP(P / ckft_km)
    End Select
    PTK = fTheta(Tk, Pmb)
    Select Case cmbOutUnit(8).Text
    Case "K": txtOutValue(8).Text = Str(PTK)
    Case "C": txtOutValue(8).Text = Str(PTK - cTo)
    Case "F": txtOutValue(8).Text = Str(9# / 5# * (PTK - cTo) + 32#)
    End Select
    
  ElseIf optPT(2).Value = True Then 'T,PT to P
    i = 1
    T = Val(txtInValue(8).Text)
    Select Case cmbInUnit(8).Text  'convert to K
    Case "K"
      Tk = T
    Case "C"
      Tk = T + cTo
    Case "F"
      Tk = (T - 32#) * 5 / 9 + cTo
    End Select
    pt = Val(txtOutValue(8).Text)
    Select Case cmbOutUnit(8).Text 'convert to mb
    Case "K"
      PTK = pt
    Case "C"
      PTK = pt + cTo
    Case "F"
      PTK = (pt - 32#) * 5 / 9 + cTo
    End Select
    Pmb = fPressure(PTK, Tk)
    Select Case cmbPTpressure.Text
    Case "mb": txtPTpressure.Text = Str(Pmb)
    Case "km": txtPTpressure.Text = Str(fPtoZ(Pmb))
    Case "kft": txtPTpressure.Text = Str(fPtoZ(Pmb) * ckft_km)
    End Select
    
  ElseIf optPT(1).Value = True Then 'P, PT to T
    P = Val(txtPTpressure.Text)
    Select Case cmbPTpressure.Text 'convert to mb
    Case "mb"
      Pmb = P
    Case "km"
      Pmb = fZtoP(P)
    Case "kft"
      Pmb = fZtoP(P / ckft_km)
    End Select
    pt = Val(txtOutValue(8).Text)
    Select Case cmbOutUnit(8).Text 'convert to mb
    Case "K"
      PTK = pt
    Case "C"
      PTK = pt + cTo
    Case "F"
      PTK = (pt - 32#) * 5 / 9 + cTo
    End Select
    Tk = fKelvin(PTK, Pmb)
    Select Case cmbInUnit(8).Text
    Case "K": txtInValue(8).Text = Str(Tk)
    Case "C": txtInValue(8).Text = Str(Tk - cTo)
    Case "F": txtInValue(8).Text = Str((Tk - cTo) * 9# / 5# + 32#)
    End Select
  End If
  DoEvents
  Exit Sub

Case 9 'Temperature conversion (x => Kelvin => y)

  If LeftToRight(TabIndex) Then
    Select Case cmbInUnit(9).Text  'Convert to K
    Case "K": x = x
    Case "C": x = x + cTo
    Case "F": x = (x - 32#) * 5# / 9# + cTo
    Case "R": x = x * 5# / 9#
    End Select
  
    Select Case cmbOutUnit(9).Text
    Case "K": y = x
    Case "C": y = x - cTo
    Case "F": y = (x - cTo) * 9# / 5# + 32#
    Case "R": y = x * 9# / 5#
    End Select
    txtOutValue(TabIndex).Text = Format(y, "###0.000")

  Else
  
    Select Case cmbOutUnit(9).Text  'Convert to K
    Case "K": y = y
    Case "C": y = y + cTo
    Case "F": y = (y - 32#) * 5# / 9# + cTo
    Case "R": y = y * 5# / 9#
    End Select
  
    Select Case cmbInUnit(9).Text
    Case "K": x = y
    Case "C": x = y - cTo
    Case "F": x = (y - cTo) * 9# / 5# + 32#
    Case "R": x = y * 9# / 5#
    End Select

    txtInValue(TabIndex).Text = Format(x, "###0.000")
  End If
  Exit Sub
  
End Select

If LeftToRight(TabIndex) Then
  y = x * k1 / k2
  txtOutValue(TabIndex).Text = Format(y, fmt$)
Else
  x = y * k2 / k1
  txtInValue(TabIndex).Text = Format(x, fmt$)
End If


End Sub






Private Sub cmdClose_Click()
Unload frmAbout
Unload frmRAOBsites
Unload Me
End Sub


Private Sub cmdRAOBsites_Click()
Load frmRAOBsites
frmRAOBsites.Top = MTPcalc.Top + MTPcalc.Height
frmRAOBsites.Left = MTPcalc.Left + (MTPcalc.Width - frmRAOBsites.Width) / 2
frmRAOBsites.Visible = True
End Sub


Private Sub Form_Load()
Dim i%, j%

'Center Form in Window
Left = (Screen.Width - Width) \ 2
Top = Screen.Height \ 6
Show

mnuSD_Click (3)

cmbInUnit(0).AddItem "mb"
cmbInUnit(0).AddItem "atm"
cmbInUnit(0).AddItem "mm Hg"
cmbInUnit(0).AddItem "in Hg"
cmbInUnit(0).AddItem "ft H2O"
cmbInUnit(0).AddItem "psi"
cmbInUnit(0).AddItem "torr"
cmbInUnit(0).AddItem "kPa"
cmbInUnit(0).AddItem "hPa"
  
cmbOutUnit(0).AddItem "m"
cmbOutUnit(0).AddItem "km"
cmbOutUnit(0).AddItem "mi"
cmbOutUnit(0).AddItem "nm"
cmbOutUnit(0).AddItem "kft"
  
'Set default units
cmbInUnit(0).Text = cmbInUnit(0).List(0)
cmbOutUnit(0).Text = cmbOutUnit(0).List(1)


cmbInUnit(1).AddItem "m"
cmbInUnit(1).AddItem "km"
cmbInUnit(1).AddItem "ft"
cmbInUnit(1).AddItem "kft"
cmbInUnit(1).AddItem "mi"
cmbInUnit(1).AddItem "nm"
'Set default units
cmbInUnit(1).Text = cmbInUnit(1).List(1)
  
cmbInUnit(2).AddItem "kg"
cmbInUnit(2).AddItem "g"
cmbInUnit(2).AddItem "lb"
cmbInUnit(2).AddItem "oz"
'Set default units
cmbInUnit(2).Text = cmbInUnit(2).List(0)

cmbInUnit(3).AddItem "s"
cmbInUnit(3).AddItem "m"
cmbInUnit(3).AddItem "h"
cmbInUnit(3).AddItem "h:m:s"
cmbInUnit(3).AddItem "dy"
cmbInUnit(3).AddItem "mo"
cmbInUnit(3).AddItem "yr"
'Set default units
cmbInUnit(3).Text = cmbInUnit(3).List(0)

cmbInUnit(4).AddItem "km/hr"
cmbInUnit(4).AddItem "knots"
cmbInUnit(4).AddItem "mph"
cmbInUnit(4).AddItem "ft/s"
cmbInUnit(4).AddItem "m/s"
  
cmbInUnit(4).Text = cmbInUnit(4).List(0)

cmbInUnit(5).AddItem "atm"
cmbInUnit(5).AddItem "mb"
cmbInUnit(5).AddItem "mm Hg"
cmbInUnit(5).AddItem "in Hg"
cmbInUnit(5).AddItem "ft H2O"
cmbInUnit(5).AddItem "psi"
cmbInUnit(5).AddItem "torr"
cmbInUnit(5).AddItem "kPa"
cmbInUnit(5).AddItem "hPa"
cmbInUnit(5).Text = cmbInUnit(5).List(0)

cmbInUnit(7).AddItem "mb"
cmbInUnit(7).AddItem "km"
cmbInUnit(7).AddItem "kft"
cmbInUnit(7).ListIndex = 1

cmbOutUnit(7).AddItem "#/cm3"
cmbOutUnit(7).AddItem "kg/m3"
cmbOutUnit(7).AddItem "g/cm3"
cmbOutUnit(7).ListIndex = 0

cmbInUnit(8).AddItem "K"
cmbInUnit(8).AddItem "C"
cmbInUnit(8).AddItem "F"
cmbInUnit(8).ListIndex = 0

cmbOutUnit(8).AddItem "K"
cmbOutUnit(8).AddItem "C"
cmbOutUnit(8).AddItem "F"
cmbOutUnit(8).ListIndex = 0

cmbPTpressure.AddItem "mb"
cmbPTpressure.AddItem "km"
cmbPTpressure.AddItem "kft"
cmbPTpressure.ListIndex = 0
optPT(0).Value = True

cmbInUnit(9).AddItem "K"
cmbInUnit(9).AddItem "C"
cmbInUnit(9).AddItem "F"
cmbInUnit(9).AddItem "R"
cmbInUnit(9).ListIndex = 1

cmbOutUnit(9).AddItem "K"
cmbOutUnit(9).AddItem "C"
cmbOutUnit(9).AddItem "F"
cmbOutUnit(9).AddItem "R"
cmbOutUnit(9).ListIndex = 0

cmbMonth.AddItem "Jan"
cmbMonth.AddItem "Feb"
cmbMonth.AddItem "Mar"
cmbMonth.AddItem "Apr"
cmbMonth.AddItem "May"
cmbMonth.AddItem "Jun"
cmbMonth.AddItem "Jul"
cmbMonth.AddItem "Aug"
cmbMonth.AddItem "Sep"
cmbMonth.AddItem "Oct"
cmbMonth.AddItem "Nov"
cmbMonth.AddItem "Dec"
cmbMonth.ListIndex = 0

For j = 0 To 5
  For i% = 0 To cmbInUnit(j).ListCount - 1
    cmbOutUnit(j).AddItem cmbInUnit(j).List(i%)
  Next i
  cmbOutUnit(j).Text = cmbOutUnit(j).List(defIndex)
  LeftToRight(j) = True
Next j
LeftToRight(6) = True
LeftToRight(7) = True
LeftToRight(9) = True

End Sub


Private Sub lblLtoR_Click(Index As Integer)
LeftToRight(Index) = Not LeftToRight(Index)
If LeftToRight(Index) Then
  lblLtoR(Index).Caption = "=>"
Else
  lblLtoR(Index).Caption = "<="
End If

End Sub

Private Sub mnuHelpAbout_Click()
'Set properties
frmAbout.Application = "Unit Conversion Utility"
frmAbout.Heading = "Release February 19, 2001"
frmAbout.Copyright = "2001 MJ Mahoney"
'Call a method
frmAbout.Display
End Sub

Private Sub mnuSD_Click(Index As Integer)

For i = 0 To mnuSD.Count - 1
  mnuSD(i).Checked = False
Next i
mnuSD(Index).Checked = True

Select Case Index
Case 0: fmt$ = "#####0"
Case 1: fmt$ = "#####0.0"
Case 2: fmt$ = "#####0.00"
Case 3: fmt$ = "#####0.000"
Case 4: fmt$ = "#####0.0000"
Case Else: fmt$ = "#####0.000"
End Select

End Sub


