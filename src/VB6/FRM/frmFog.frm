VERSION 5.00
Begin VB.Form frmFog 
   Caption         =   "Form1"
   ClientHeight    =   4020
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6990
   LinkTopic       =   "Form1"
   ScaleHeight     =   4020
   ScaleWidth      =   6990
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkConvertLiquid 
      Caption         =   "Convert Liquid Water Loss to Vapor"
      Height          =   255
      Left            =   30
      TabIndex        =   32
      Top             =   3240
      Width           =   3015
   End
   Begin VB.TextBox txtElev 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   21
      Text            =   "frmFog.frx":0000
      Top             =   210
      Width           =   615
   End
   Begin VB.TextBox txtRhoL 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   20
      Top             =   570
      Width           =   615
   End
   Begin VB.TextBox txtBI 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   19
      Text            =   "frmFog.frx":0003
      Top             =   1290
      Width           =   615
   End
   Begin VB.TextBox txtIntRhoL 
      Alignment       =   1  'Right Justify
      Enabled         =   0   'False
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   18
      Top             =   1650
      Width           =   615
   End
   Begin VB.CommandButton cmdCalculateTB 
      Caption         =   "Calculate TB"
      Height          =   375
      Left            =   5460
      TabIndex        =   17
      Top             =   3570
      Width           =   1215
   End
   Begin VB.TextBox txtFrequency1 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   16
      Text            =   "frmFog.frx":0009
      Top             =   2010
      Width           =   615
   End
   Begin VB.TextBox txtTB1 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   15
      Top             =   2490
      Width           =   615
   End
   Begin VB.TextBox txtFrequency2 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2460
      MultiLine       =   -1  'True
      TabIndex        =   14
      Top             =   2010
      Width           =   615
   End
   Begin VB.TextBox txtTB2 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2460
      MultiLine       =   -1  'True
      TabIndex        =   13
      Top             =   2490
      Width           =   615
   End
   Begin VB.TextBox txtTB3 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3180
      MultiLine       =   -1  'True
      TabIndex        =   12
      Top             =   2490
      Width           =   615
   End
   Begin VB.Frame Frame 
      Caption         =   "Enable Absorption"
      Height          =   1215
      Index           =   0
      Left            =   5340
      TabIndex        =   7
      Top             =   90
      Width           =   1575
      Begin VB.CheckBox chkEnableAbs 
         Caption         =   "H2O Vapor "
         Height          =   195
         Index           =   2
         Left            =   120
         TabIndex        =   11
         Top             =   720
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.CheckBox chkEnableAbs 
         Caption         =   "Oxygen"
         Height          =   195
         Index           =   0
         Left            =   120
         TabIndex        =   10
         Top             =   240
         Value           =   1  'Checked
         Width           =   975
      End
      Begin VB.CheckBox chkEnableAbs 
         Caption         =   "Nitrogen"
         Height          =   195
         Index           =   1
         Left            =   120
         TabIndex        =   9
         Top             =   480
         Value           =   1  'Checked
         Width           =   975
      End
      Begin VB.CheckBox chkEnableAbs 
         Caption         =   "H2O Liquid"
         Height          =   195
         Index           =   3
         Left            =   120
         TabIndex        =   8
         Top             =   960
         Value           =   1  'Checked
         Width           =   1095
      End
   End
   Begin VB.CheckBox chkEnableF2 
      Caption         =   "Automatically Do Image Frequency"
      Height          =   375
      Left            =   60
      TabIndex        =   6
      Top             =   2850
      Value           =   1  'Checked
      Width           =   3015
   End
   Begin VB.TextBox txtIntRhoV 
      Alignment       =   1  'Right Justify
      Enabled         =   0   'False
      Height          =   285
      Left            =   1500
      MultiLine       =   -1  'True
      TabIndex        =   5
      Top             =   930
      Width           =   615
   End
   Begin VB.TextBox txtRHthreshold 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3900
      MultiLine       =   -1  'True
      TabIndex        =   4
      Text            =   "frmFog.frx":000E
      Top             =   210
      Width           =   495
   End
   Begin VB.TextBox txtLiquid12 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3300
      MultiLine       =   -1  'True
      TabIndex        =   3
      Top             =   570
      Width           =   495
   End
   Begin VB.TextBox txtLiquid22 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   3900
      MultiLine       =   -1  'True
      TabIndex        =   2
      Top             =   570
      Width           =   495
   End
   Begin VB.TextBox txtZo 
      Height          =   285
      Left            =   1500
      TabIndex        =   1
      Top             =   3570
      Width           =   615
   End
   Begin VB.CommandButton cmdDefault 
      Caption         =   "Default"
      Height          =   375
      Left            =   2340
      TabIndex        =   0
      Top             =   3570
      Width           =   1215
   End
   Begin VB.Label Label 
      Caption         =   "Elevation (deg)"
      Height          =   255
      Index           =   24
      Left            =   60
      TabIndex        =   31
      Top             =   210
      Width           =   1095
   End
   Begin VB.Label Label 
      Caption         =   "Rho Liquid (g/m3)"
      Height          =   255
      Index           =   25
      Left            =   60
      TabIndex        =   30
      Top             =   570
      Width           =   1335
   End
   Begin VB.Label Label 
      Caption         =   "Inversion Base (m)"
      Height          =   255
      Index           =   27
      Left            =   60
      TabIndex        =   29
      Top             =   1290
      Width           =   1335
   End
   Begin VB.Label Label 
      Caption         =   "LWC (microns)"
      Height          =   255
      Index           =   35
      Left            =   60
      TabIndex        =   28
      Top             =   1650
      Width           =   1095
   End
   Begin VB.Label Label 
      Caption         =   "TB (K)"
      Height          =   255
      Index           =   36
      Left            =   60
      TabIndex        =   27
      Top             =   2490
      Width           =   495
   End
   Begin VB.Label Label 
      Caption         =   "Frequency (GHz)"
      Height          =   255
      Index           =   37
      Left            =   60
      TabIndex        =   26
      Top             =   2010
      Width           =   1335
   End
   Begin VB.Label Label 
      Caption         =   "Int.Rho Vapor (cm)"
      Height          =   255
      Index           =   26
      Left            =   60
      TabIndex        =   25
      Top             =   930
      Width           =   1335
   End
   Begin VB.Label Label 
      Caption         =   "Liquid RH Threshold"
      Height          =   255
      Index           =   38
      Left            =   2220
      TabIndex        =   24
      Top             =   210
      Width           =   1575
   End
   Begin VB.Label Label 
      Caption         =   "Liquid from (m)"
      Height          =   255
      Index           =   43
      Left            =   2220
      TabIndex        =   23
      Top             =   570
      Width           =   1095
   End
   Begin VB.Label Label 
      Caption         =   "Start Altitude (km)"
      Height          =   255
      Index           =   44
      Left            =   60
      TabIndex        =   22
      Top             =   3570
      Width           =   1335
   End
End
Attribute VB_Name = "frmFog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdCalculateTB_Click()
Dim TB1!, TB2!, TB3!, F1!, f2!, Elev!, BI!, EnableAbs(1 To 4) As Boolean, i%
Dim RhoL!(1 To 300), VMR!, zo!, ii%, RHthreshold!, RhoTot!, IntRhoL!

If RAOBlu = 0 Then
  MsgBox "You must open a RAOB file before using this control!", vbOKOnly
  Exit Sub
End If

For i = 0 To 3
  If chkEnableAbs(i).Value = 1 Then EnableAbs(i + 1) = True Else EnableAbs(i + 1) = False
Next i
Elev = Val(txtElev.Text)
F1 = Val(txtFrequency1.Text)
RHthreshold = Val(txtRHthreshold.Text)  'Never used!
If txtZo.Text = "" Then zo = ZGraob(1) Else zo = Val(txtZo.Text)
txtZo.Text = Format(zo, "#0.000")

VMR = 0#
For i = 1 To Nlevr
  RhoL(i) = 0#
  If chkConvertLiquid.Value = 1 Then
    If i >= iLiquid1 And i <= iLiquid2 Then  'Levels where RH > RH threshold
    ' Use point below first point to determine VMR
      If i = iLiquid1 Then
        If i > 1 Then ii = i - 1 Else ii = i
        VMR = fEv(RZraob(ii), TZraob(ii)) / PZraob(ii)
      End If
      RhoTot = fRhoVvmr(PZraob(i), TZraob(i), VMR)
      If RhoTot > RVraob(i) Then RhoL(i) = RhoTot - RVraob(i)
    End If
  End If
Next i



IntRhoV = fRhoInt(ZGraob(), RVraob(), Nlevr, 0, False)
txtIntRhoV.Text = Format(IntRhoV, "0.00")
IntRhoL = fRhoInt(ZGraob(), RhoL(), Nlevr, 0, False)
txtIntRhoL.Text = Format(IntRhoL * 1000, "0.00")


If F1 < 60 And chkEnableF2.Value = 1 Then
  f2 = fFusb(TZraob(1), PZraob(1), RVraob(1), F1)
  txtFrequency2.Text = Format(f2, "00.000")
  TB1 = fTBraob(F1, Elev, zo, ZGraob(), TZraob(), RVraob(), RhoL(), BI, Nlevr, EnableAbs())
  txtTB1.Text = Format(TB1, "000.0")
  TB2 = fTBraob(f2, Elev, zo, ZGraob(), TZraob(), RVraob(), RhoL(), BI, Nlevr, EnableAbs())
  txtTB2.Text = Format(TB2, "000.0")
  txtTB3.Text = Format(TB2 - TB1, "000.0")
Else
  TB1 = fTBraob(F1, Elev, zo, ZGraob(), TZraob(), RVraob(), RhoL(), BI, Nlevr, EnableAbs())
  txtTB1.Text = Format(TB1, "#0.0")
  txtTB2.Text = ""
  txtTB3.Text = ""
  txtFrequency2.Text = ""
End If
Debug.Print TB1
DoEvents

End Sub


Private Sub cmdDefault_Click()
txtZo.Text = ZGraob(1)
End Sub


Private Sub txtRhoL_Change()
Dim LWC

LWC = Val(txtRhoL.Text) * Val(txtBI.Text)

txtIntRhoL.Text = Str(LWC)

End Sub


