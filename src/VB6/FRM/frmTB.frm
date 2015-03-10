VERSION 5.00
Begin VB.Form frmTB 
   Caption         =   "Brightness Temperatures"
   ClientHeight    =   5550
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4380
   Icon            =   "frmTB.frx":0000
   LinkTopic       =   "Form2"
   ScaleHeight     =   5550
   ScaleWidth      =   4380
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdOptions 
      Caption         =   "Options"
      Height          =   375
      Left            =   2700
      TabIndex        =   18
      Top             =   4455
      Width           =   780
   End
   Begin VB.CheckBox chkShowFL 
      Caption         =   "Show Flight Level"
      Height          =   195
      Left            =   315
      TabIndex        =   17
      Top             =   5355
      Value           =   1  'Checked
      Width           =   1755
   End
   Begin VB.CheckBox chkShowArcAvg 
      Caption         =   "Show AA Observables"
      Height          =   195
      Left            =   2025
      TabIndex        =   16
      Top             =   5085
      Width           =   2130
   End
   Begin VB.CommandButton cmdAutoScaleTA 
      Caption         =   "M"
      Height          =   315
      Left            =   2205
      TabIndex        =   5
      ToolTipText     =   "Auto (A) or Manual (M) Scale"
      Top             =   4515
      Width           =   315
   End
   Begin VB.TextBox txtT2 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3660
      MultiLine       =   -1  'True
      TabIndex        =   4
      Top             =   4515
      Width           =   375
   End
   Begin VB.TextBox txtT1 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   330
      MultiLine       =   -1  'True
      TabIndex        =   3
      Top             =   4515
      Width           =   375
   End
   Begin VB.TextBox txtTC 
      Alignment       =   2  'Center
      Height          =   285
      Left            =   1815
      MultiLine       =   -1  'True
      TabIndex        =   2
      Top             =   4515
      Width           =   375
   End
   Begin VB.PictureBox picTA 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00808080&
      Height          =   4365
      Left            =   330
      ScaleHeight     =   4305
      ScaleWidth      =   3645
      TabIndex        =   1
      Top             =   90
      Width           =   3705
   End
   Begin VB.CheckBox chkRemoveAAbias 
      Caption         =   "Remove AA Bias"
      Height          =   225
      Left            =   315
      TabIndex        =   0
      Top             =   5085
      Width           =   1530
   End
   Begin VB.Label label 
      Caption         =   "FL"
      Height          =   255
      Index           =   68
      Left            =   90
      TabIndex        =   15
      Top             =   4185
      Width           =   225
   End
   Begin VB.Label label 
      Caption         =   "Loc"
      Height          =   255
      Index           =   150
      Left            =   4095
      TabIndex        =   14
      Top             =   4185
      Width           =   255
   End
   Begin VB.Label label 
      Caption         =   "4"
      Height          =   255
      Index           =   152
      Left            =   120
      TabIndex        =   13
      Top             =   3510
      Width           =   165
   End
   Begin VB.Label label 
      Caption         =   "8"
      Height          =   255
      Index           =   188
      Left            =   120
      TabIndex        =   12
      Top             =   2745
      Width           =   165
   End
   Begin VB.Label label 
      Caption         =   "12"
      Height          =   255
      Index           =   189
      Left            =   90
      TabIndex        =   11
      Top             =   1905
      Width           =   195
   End
   Begin VB.Label label 
      Caption         =   "16"
      Height          =   255
      Index           =   190
      Left            =   90
      TabIndex        =   10
      Top             =   1095
      Width           =   195
   End
   Begin VB.Label label 
      Caption         =   "20"
      Height          =   255
      Index           =   191
      Left            =   90
      TabIndex        =   9
      Top             =   300
      Width           =   195
   End
   Begin VB.Label label 
      Caption         =   "10"
      Height          =   255
      Index           =   192
      Left            =   4080
      TabIndex        =   8
      Top             =   3960
      Width           =   195
   End
   Begin VB.Label label 
      Caption         =   "1"
      Height          =   255
      Index           =   193
      Left            =   4110
      TabIndex        =   7
      Top             =   375
      Width           =   165
   End
   Begin VB.Label label 
      Caption         =   "6"
      Height          =   255
      Index           =   194
      Left            =   4095
      TabIndex        =   6
      Top             =   2340
      Width           =   195
   End
End
Attribute VB_Name = "frmTB"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public AutoScaleTA As Boolean
Sub TBupdate()
' Update TB display window
Dim i%, j%, LineC&, T!, TAmin!, TAmax!, FL!, iRC1%, iRC2%, TAAp!(1 To 20), AAbias!
  
' Plot Antenna Tempertures in picture window
  With picTA
  .Cls
  .ScaleMode = 0
  .ScaleTop = 0
  .ScaleHeight = 11
  End With
' Set temperature display scale
  If AutoScaleTA Then
    Tmin = 500
    Tmax = 100
    For i = 1 To 10
      T = TA(1, i)
      If T > Tmax Then Tmax = T
      If T < Tmin Then Tmin = T
    Next i
    
    If chkShowArcAvg.Value = 1 Then
      For j = 1 To 10
        T = TAA(RCindex1, 1, j)
        If T > Tmax Then Tmax = T
        If T < Tmin Then Tmin = T
      Next j
      If RCindex2 >= 0 Then 'Need to plot second set of AA observables.
        For j = 1 To 10
          T = TAA(RCindex2, 1, j)
          If T > Tmax Then Tmax = T
          If T < Tmin Then Tmin = T
        Next j
      End If
    End If
    
    Tmin = ((Tmin - 5) \ 5) * 5
    Tmax = ((Tmax + 5) \ 5) * 5
    picTA.ScaleWidth = Tmax - Tmin
    If picTA.ScaleWidth < 20 Then
      Tmax = Tmin + 20
      picTA.ScaleWidth = 20
    End If
    picTA.ScaleLeft = Tmin
    txtT1.Text = Format(Tmin, "000")
    txtT2.Text = Format(Tmax, "000")
    txtTC.Text = Format(picTA.ScaleWidth, "000")
  Else
    Tmin = Val(txtT1.Text)
    picTA.ScaleLeft = Tmin
    picTA.ScaleWidth = Val(txtTC.Text)
    Tmax = picTA.ScaleLeft + picTA.ScaleWidth
    txtT2.Text = Format(Tmax, "000")
  End If
  
  For i = 1 To Channels
    Select Case i
    Case 1: LineC = QBColor(12)  'LT RED
    Case 2: LineC = QBColor(15)  'Bright WHITE
    Case 3: LineC = QBColor(9)   'LT BLUE
    End Select
    picTA.PSet (TA(i, 1), 1)
    For j = 2 To 10
      picTA.Line -(TA(i, j), j), LineC    'And continue from there
    Next j
  Next i
  
' Draw archive average TAs
  If chkShowArcAvg.Value = 1 Then
    Select Case Algorithm
    Case 1, 2, 3, 4, 5, 6
      iRC1 = aRC(IlnPmin1)
      iRC2 = aRC(IlnPmin2)
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = 1 To 10: TAAp(j) = TAA(iRC1, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC1, 2)
          For j = 1 To 10: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(1), 1)
        For j = 2 To 10
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
      If Sgn(TBavg(iRC2)) <> Sgn(TBavg(iRC1)) Then
        For i = 1 To Channels
          Select Case i
          Case 1: LineC = QBColor(4)  'RED
          Case 2: LineC = QBColor(7)  'WHITE
          Case 3: LineC = QBColor(1)   'BLUE
          End Select
          For j = 1 To 10: TAAp(j) = TAA(iRC2, i, j): Next j
          If chkRemoveAAbias.Value = 1 Then
            AAbias = fCalcBiasWrtAA(ob(), iRC2, 2)
            For j = 1 To 10: TAAp(j) = TAAp(j) + AAbias: Next j
          End If
          picTA.PSet (TAAp(1), 1)
          For j = 2 To 10
            picTA.Line -(TAAp(j), j), LineC    'And continue from there
          Next j
        Next i
      End If
      
    Case 7  'Separate Up and Dn retrievals
      iRC1 = aRC(IlnPmin1)
      iRC2 = aRC(IlnPmin2)
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = 1 To LocHor: TAAp(j) = TAA(iRC1, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC1, 2)
          For j = 1 To LocHor: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(1), 1)
        For j = 2 To LocHor
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
      For i = 1 To Channels
        Select Case i
        Case 1: LineC = QBColor(4)  'RED
        Case 2: LineC = QBColor(7)  'WHITE
        Case 3: LineC = QBColor(1)   'BLUE
        End Select
        For j = LocHor To 10: TAAp(j) = TAA(iRC2, i, j): Next j
        If chkRemoveAAbias.Value = 1 Then
          AAbias = fCalcBiasWrtAA(ob(), iRC2, 2)
          For j = LocHor To 10: TAAp(j) = TAAp(j) + AAbias: Next j
        End If
        picTA.PSet (TAAp(LocHor), LocHor)
        For j = LocHor + 1 To 10
          picTA.Line -(TAAp(j), j), LineC    'And continue from there
        Next j
      Next i
    
    End Select
  End If

' Draw tic marks
  For i = 1 To 10
    If i = LocHor Then 'A/C Level
      picTA.PSet (Tmin, i)
      picTA.Line -(Tmin + picTA.ScaleWidth, i), QBColor(15)
    Else
      picTA.PSet (Tmin, i)
      picTA.Line -(Tmin + 0.04 * picTA.ScaleWidth, i), QBColor(15)
      picTA.PSet (Tmin + picTA.ScaleWidth, i)
      picTA.Line -(Tmin + 0.96 * picTA.ScaleWidth, i), QBColor(15)
    End If
  Next i
' Draw a/c altitude
  If chkShowFL.Value = 1 Then
    FL = 11 - pALT / 2
    picTA.PSet (Tmin, FL)
    picTA.Line -(Tmin + picTA.ScaleWidth, FL), QBColor(13)  'Pink
'    picTA.Line -(Tmin + 0.04 * picTA.ScaleWidth, FL), QBColor(13) 'Pink
'    picTA.PSet (Tmin + picTA.ScaleWidth, FL)
'    picTA.Line -(Tmin + 0.96 * picTA.ScaleWidth, FL), QBColor(13)
  End If

End Sub


Private Sub cmdAutoScaleTA_Click()

AutoScaleTA = Not AutoScaleTA
If AutoScaleTA Then
  cmdAutoScaleTA.Caption = "Manual   Scale"
  txtT1.Enabled = False
  txtTC.Enabled = False
Else
  cmdAutoScaleTA.Caption = "  Auto     Scale"
  txtT1.Enabled = True
  txtTC.Enabled = True
End If

End Sub

Private Sub cmdOptions_Click()
  
  If frmTB.cmdOptions.Caption = "Options" Then
    frmTB.Height = 5955
    frmTB.cmdOptions.Caption = "Hide"
  Else
    frmTB.Height = 5250
    frmTB.cmdOptions.Caption = "Options"
  End If
End Sub

Private Sub Form_DblClick()
  
  CaptureFormMJ Me, "", 3, True

End Sub

Private Sub Form_Load()

  chkShowFL.Value = 1
  chkShowArcAvg.Value = 1
  frmTB.Height = 5250
  
End Sub


