VERSION 5.00
Begin VB.Form frmStatus 
   Caption         =   "ATP Status"
   ClientHeight    =   3870
   ClientLeft      =   7785
   ClientTop       =   870
   ClientWidth     =   4320
   Icon            =   "frmStatus.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   2  'Custom
   ScaleHeight     =   3870
   ScaleWidth      =   4320
   Begin VB.ComboBox cboFooter 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   360
      Left            =   1050
      TabIndex        =   35
      Text            =   "Combo1"
      Top             =   3450
      Width           =   2190
   End
   Begin VB.TextBox txtStatus 
      BackColor       =   &H0000C000&
      Height          =   510
      Left            =   120
      TabIndex        =   32
      Text            =   "No NAV"
      Top             =   60
      Width           =   885
   End
   Begin VB.Timer Timer1 
      Left            =   3225
      Top             =   3435
   End
   Begin VB.TextBox txtFraction 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3645
      MultiLine       =   -1  'True
      TabIndex        =   31
      Text            =   "frmStatus.frx":0442
      Top             =   3090
      Width           =   630
   End
   Begin VB.TextBox txtRC2 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2160
      TabIndex        =   30
      Text            =   "VORC3"
      Top             =   3045
      Width           =   1065
   End
   Begin VB.TextBox txtRC1 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1050
      TabIndex        =   29
      Text            =   "WUSC3"
      Top             =   3045
      Width           =   1095
   End
   Begin VB.Frame Frame1 
      Caption         =   "Display"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   2490
      TabIndex        =   25
      Top             =   2415
      Width           =   1800
      Begin VB.OptionButton optZx 
         Caption         =   "Zg"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Index           =   1
         Left            =   1035
         TabIndex        =   27
         Top             =   225
         Width           =   660
      End
      Begin VB.OptionButton optZx 
         Caption         =   "Zp"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Index           =   0
         Left            =   90
         TabIndex        =   26
         Top             =   255
         Width           =   660
      End
   End
   Begin VB.TextBox txtALTkm 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   4
      Left            =   1755
      MultiLine       =   -1  'True
      TabIndex        =   23
      Text            =   "frmStatus.frx":0449
      Top             =   2580
      Width           =   660
   End
   Begin VB.TextBox txtALTkft 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   4
      Left            =   1065
      MultiLine       =   -1  'True
      TabIndex        =   22
      Text            =   "frmStatus.frx":0450
      Top             =   2580
      Width           =   660
   End
   Begin VB.TextBox txtTheta 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   3
      Left            =   3420
      MultiLine       =   -1  'True
      TabIndex        =   21
      Text            =   "frmStatus.frx":0457
      Top             =   1965
      Width           =   855
   End
   Begin VB.TextBox txtTheta 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   2
      Left            =   3405
      MultiLine       =   -1  'True
      TabIndex        =   20
      Text            =   "frmStatus.frx":045F
      Top             =   1530
      Width           =   855
   End
   Begin VB.TextBox txtTheta 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   1
      Left            =   3405
      MultiLine       =   -1  'True
      TabIndex        =   19
      Text            =   "frmStatus.frx":0467
      Top             =   1110
      Width           =   855
   End
   Begin VB.TextBox txtTheta 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   0
      Left            =   3390
      MultiLine       =   -1  'True
      TabIndex        =   18
      Text            =   "frmStatus.frx":046F
      Top             =   660
      Width           =   855
   End
   Begin VB.TextBox txtTemp 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   3
      Left            =   2490
      MultiLine       =   -1  'True
      TabIndex        =   17
      Text            =   "frmStatus.frx":0477
      Top             =   1980
      Width           =   855
   End
   Begin VB.TextBox txtTemp 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   2
      Left            =   2490
      MultiLine       =   -1  'True
      TabIndex        =   16
      Text            =   "frmStatus.frx":047F
      Top             =   1545
      Width           =   855
   End
   Begin VB.TextBox txtTemp 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   1
      Left            =   2490
      MultiLine       =   -1  'True
      TabIndex        =   15
      Text            =   "frmStatus.frx":0487
      Top             =   1110
      Width           =   855
   End
   Begin VB.TextBox txtTemp 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   0
      Left            =   2475
      MultiLine       =   -1  'True
      TabIndex        =   14
      Text            =   "frmStatus.frx":048F
      Top             =   690
      Width           =   855
   End
   Begin VB.TextBox txtALTkm 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   3
      Left            =   1755
      MultiLine       =   -1  'True
      TabIndex        =   13
      Text            =   "frmStatus.frx":0497
      Top             =   1995
      Width           =   660
   End
   Begin VB.TextBox txtALTkm 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   2
      Left            =   1755
      MultiLine       =   -1  'True
      TabIndex        =   12
      Text            =   "frmStatus.frx":049E
      Top             =   1545
      Width           =   660
   End
   Begin VB.TextBox txtALTkm 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   1
      Left            =   1770
      MultiLine       =   -1  'True
      TabIndex        =   11
      Text            =   "frmStatus.frx":04A5
      Top             =   1110
      Width           =   660
   End
   Begin VB.TextBox txtALTkm 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   0
      Left            =   1785
      MultiLine       =   -1  'True
      TabIndex        =   10
      Text            =   "frmStatus.frx":04AC
      Top             =   675
      Width           =   645
   End
   Begin VB.TextBox txtALTkft 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   3
      Left            =   1065
      MultiLine       =   -1  'True
      TabIndex        =   5
      Text            =   "frmStatus.frx":04B3
      Top             =   1995
      Width           =   660
   End
   Begin VB.TextBox txtALTkft 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   2
      Left            =   1065
      MultiLine       =   -1  'True
      TabIndex        =   4
      Text            =   "frmStatus.frx":04BA
      Top             =   1545
      Width           =   660
   End
   Begin VB.TextBox txtALTkft 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   1
      Left            =   1065
      MultiLine       =   -1  'True
      TabIndex        =   3
      Text            =   "frmStatus.frx":04C1
      Top             =   1110
      Width           =   660
   End
   Begin VB.TextBox txtALTkft 
      Alignment       =   1  'Right Justify
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   420
      Index           =   0
      Left            =   1065
      MultiLine       =   -1  'True
      TabIndex        =   2
      Text            =   "frmStatus.frx":04C8
      Top             =   675
      Width           =   645
   End
   Begin VB.Label Label 
      Caption         =   "Footer"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Index           =   8
      Left            =   90
      TabIndex        =   34
      Top             =   3450
      Width           =   1005
   End
   Begin VB.Label Label1 
      Caption         =   "Wt"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   3255
      TabIndex        =   33
      Top             =   3105
      Width           =   375
   End
   Begin VB.Label Label 
      Caption         =   "RCs"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   1
      Left            =   90
      TabIndex        =   28
      Top             =   3075
      Width           =   615
   End
   Begin VB.Label Label 
      Caption         =   "Zg-Zp"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   405
      Index           =   7
      Left            =   105
      TabIndex        =   24
      Top             =   2610
      Width           =   855
   End
   Begin VB.Label Label 
      Caption         =   "Coldest"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Index           =   6
      Left            =   75
      TabIndex        =   9
      Top             =   2040
      Width           =   975
   End
   Begin VB.Label Label 
      Caption         =   "Trop2"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Index           =   5
      Left            =   75
      TabIndex        =   8
      Top             =   1575
      Width           =   855
   End
   Begin VB.Label Label 
      Caption         =   "Trop1"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Index           =   4
      Left            =   75
      TabIndex        =   7
      Top             =   1140
      Width           =   795
   End
   Begin VB.Label Label 
      Caption         =   "A/C"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   330
      Index           =   3
      Left            =   90
      TabIndex        =   6
      Top             =   690
      Width           =   645
   End
   Begin VB.Label Label 
      Caption         =   "  [kft]   [km]     [K]        [K]"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Index           =   2
      Left            =   990
      TabIndex        =   1
      Top             =   330
      Width           =   3195
   End
   Begin VB.Label Label 
      Caption         =   "Zp    Zp        T       Theta"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Index           =   0
      Left            =   1230
      TabIndex        =   0
      Top             =   15
      Width           =   3000
   End
End
Attribute VB_Name = "frmStatus"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal y As Long, ByVal cx As Long, ByVal cY As Long, ByVal wFlags As Long) As Long

Private Const HWND_TOPMOST = -1 'bring to top and stay there
Private Const HWND_NOTOPMOST = -2 'put the window into a normal position

Private Const SWP_NOMOVE = &H2 'don't move window
Private Const SWP_NOSIZE = &H1 'don't size window

Private Declare Function GetForegroundWindow Lib "user32" () As Long

Private Sub cboFooter_Click()
frmATP.FooterType = cboFooter.ListIndex
End Sub


Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True

End Sub

Private Sub Form_Load()

  With Timer1
  .Interval = 30000
  .Enabled = True
  End With
  
'  Timer2.Interval = 1000
  
  With cboFooter
  .AddItem "Default"                 '0
  .AddItem "2 Channel Gains"         '1
  .AddItem "3 Channel Gains"         '2
  .AddItem "Some Mux Data, CB, dND"  '3
  .AddItem "All Mux Data"            '4
'  .AddItem "B, C Line Differences"   '5
'  .AddItem "D, E Line Differences"   '6
  .ListIndex = frmATP.FooterType
  End With

End Sub


Private Sub optZx_Click(Index As Integer)
' Determine whether Zp or Zg should be displayed on ATP
'  optZx(Index).value = True
  If optZx(0).Value = True Then
    CallingForm.ShowZp = True
  Else
    CallingForm.ShowZp = False
  End If

End Sub


Private Sub Timer1_Timer()
Static NotFirst As Boolean, UTsecLast&

If NotFirst Then
  If UTsec = UTsecLast Then
    txtStatus.BackColor = &HFF&
  Else
    txtStatus.BackColor = &HC000&
  End If
  UTsecLast = UTsec
  If NavData Then txtStatus.Text = "" Else txtStatus.Text = "No NAV"

Else
  NotFirst = True
  UTsecLast = UTsec
End If

End Sub


Private Sub Timer2_Timer()
' If the window on top is not this window...
  If Me.hwnd <> GetForegroundWindow Then
'   Make this form be on top
    Call SetWindowPos(GetForegroundWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE)
'   Make the window on top below this form
    Call SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE)
  End If

End Sub


