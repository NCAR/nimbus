VERSION 5.00
Begin VB.Form frmEditBits 
   Caption         =   "Edit Bits"
   ClientHeight    =   2895
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4530
   Icon            =   "frmEditBits.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2895
   ScaleWidth      =   4530
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame9 
      Caption         =   "MakeWord"
      Height          =   2745
      Left            =   120
      TabIndex        =   26
      Top             =   120
      Width           =   2055
      Begin VB.CheckBox chkWords 
         Caption         =   "Spare 2"
         Height          =   255
         Index           =   7
         Left            =   960
         TabIndex        =   50
         Top             =   2160
         Width           =   855
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Spare 2"
         Height          =   255
         Index           =   7
         Left            =   240
         TabIndex        =   49
         Top             =   2160
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Spare 2"
         Height          =   255
         Index           =   7
         Left            =   600
         TabIndex        =   48
         Top             =   2160
         Width           =   255
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Spare 1"
         Height          =   255
         Index           =   6
         Left            =   960
         TabIndex        =   47
         Top             =   1920
         Width           =   855
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   6
         Left            =   240
         TabIndex        =   46
         Top             =   1920
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   6
         Left            =   600
         TabIndex        =   45
         Top             =   1920
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   5
         Left            =   600
         TabIndex        =   44
         Top             =   1680
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   4
         Left            =   600
         TabIndex        =   43
         Top             =   1440
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   3
         Left            =   600
         TabIndex        =   42
         Top             =   1200
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   2
         Left            =   600
         TabIndex        =   41
         Top             =   960
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   1
         Left            =   600
         TabIndex        =   40
         Top             =   720
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   0
         Left            =   600
         TabIndex        =   39
         Top             =   480
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   5
         Left            =   240
         TabIndex        =   38
         Top             =   1680
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   4
         Left            =   240
         TabIndex        =   37
         Top             =   1440
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   3
         Left            =   240
         TabIndex        =   36
         Top             =   1200
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   2
         Left            =   240
         TabIndex        =   35
         Top             =   960
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   1
         Left            =   240
         TabIndex        =   34
         Top             =   720
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   33
         Top             =   480
         Width           =   255
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Cycle"
         Height          =   255
         Index           =   3
         Left            =   960
         TabIndex        =   32
         Top             =   1200
         Width           =   855
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "RAWbad"
         Height          =   255
         Index           =   5
         Left            =   960
         TabIndex        =   31
         Top             =   1680
         Width           =   975
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Ttgt"
         Height          =   255
         Index           =   4
         Left            =   960
         TabIndex        =   30
         Top             =   1440
         Width           =   735
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Nav"
         Height          =   255
         Index           =   0
         Left            =   960
         TabIndex        =   29
         Top             =   480
         Width           =   735
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "TAs"
         Height          =   255
         Index           =   1
         Left            =   960
         TabIndex        =   28
         Top             =   720
         Width           =   735
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Cts"
         Height          =   255
         Index           =   2
         Left            =   960
         TabIndex        =   27
         Top             =   960
         Width           =   855
      End
      Begin VB.Label Label56 
         Caption         =   "Scan Trop  Actual"
         Height          =   255
         Left            =   120
         TabIndex        =   51
         Top             =   240
         Width           =   1575
      End
   End
   Begin VB.Frame Frame6 
      Caption         =   "EditWord"
      Height          =   2745
      Left            =   2280
      TabIndex        =   0
      Top             =   120
      Width           =   2145
      Begin VB.CheckBox chkWords 
         Caption         =   "Rate (m/s)"
         Height          =   255
         Index           =   16
         Left            =   960
         TabIndex        =   54
         Top             =   2430
         Width           =   1065
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   16
         Left            =   240
         TabIndex        =   53
         Top             =   2430
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   16
         Left            =   600
         TabIndex        =   52
         Top             =   2430
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   15
         Left            =   600
         TabIndex        =   24
         Top             =   2160
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   14
         Left            =   600
         TabIndex        =   23
         Top             =   1920
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   13
         Left            =   600
         TabIndex        =   22
         Top             =   1680
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   12
         Left            =   600
         TabIndex        =   21
         Top             =   1440
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   11
         Left            =   600
         TabIndex        =   20
         Top             =   1200
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   10
         Left            =   600
         TabIndex        =   19
         Top             =   960
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   9
         Left            =   600
         TabIndex        =   18
         Top             =   720
         Width           =   255
      End
      Begin VB.CheckBox chkBadTrop 
         Caption         =   "Check8"
         Height          =   255
         Index           =   8
         Left            =   600
         TabIndex        =   17
         Top             =   480
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   15
         Left            =   240
         TabIndex        =   16
         Top             =   2160
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   14
         Left            =   240
         TabIndex        =   15
         Top             =   1920
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   13
         Left            =   240
         TabIndex        =   14
         Top             =   1680
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   12
         Left            =   240
         TabIndex        =   13
         Top             =   1440
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   11
         Left            =   240
         TabIndex        =   12
         Top             =   1200
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   10
         Left            =   240
         TabIndex        =   11
         Top             =   960
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   9
         Left            =   240
         TabIndex        =   10
         Top             =   720
         Width           =   255
      End
      Begin VB.CheckBox chkBadScan 
         Caption         =   "Check8"
         Height          =   255
         Index           =   8
         Left            =   240
         TabIndex        =   9
         Top             =   480
         Width           =   255
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "RetAlt"
         Height          =   255
         Index           =   8
         Left            =   960
         TabIndex        =   8
         Top             =   480
         Width           =   855
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Temps"
         Height          =   255
         Index           =   9
         Left            =   960
         TabIndex        =   7
         Top             =   720
         Width           =   855
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "OATtrop"
         Height          =   255
         Index           =   10
         Left            =   960
         TabIndex        =   6
         Top             =   960
         Width           =   975
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "ZtOff"
         Height          =   255
         Index           =   11
         Left            =   960
         TabIndex        =   5
         Top             =   1200
         Width           =   735
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Trop Alt"
         Height          =   255
         Index           =   12
         Left            =   960
         TabIndex        =   4
         Top             =   1440
         Width           =   855
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Pitch"
         Height          =   255
         Index           =   13
         Left            =   960
         TabIndex        =   3
         Top             =   1680
         Width           =   735
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Roll"
         Height          =   255
         Index           =   14
         Left            =   960
         TabIndex        =   2
         Top             =   1920
         Width           =   615
      End
      Begin VB.CheckBox chkWords 
         Caption         =   "Nlev "
         Height          =   255
         Index           =   15
         Left            =   960
         TabIndex        =   1
         Top             =   2160
         Width           =   735
      End
      Begin VB.Label Label57 
         Caption         =   "Scan Trop   Actual"
         Height          =   255
         Left            =   120
         TabIndex        =   25
         Top             =   240
         Width           =   1335
      End
   End
End
Attribute VB_Name = "frmEditBits"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

