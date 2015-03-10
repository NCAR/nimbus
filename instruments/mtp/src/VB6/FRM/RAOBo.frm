VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmFormatRAOB 
   Caption         =   "RAOB Display Options"
   ClientHeight    =   3060
   ClientLeft      =   5985
   ClientTop       =   7425
   ClientWidth     =   4335
   Icon            =   "RAOBo.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3060
   ScaleWidth      =   4335
   Begin VB.CommandButton cmdExit 
      Caption         =   "&Exit"
      Height          =   375
      Left            =   120
      TabIndex        =   24
      Top             =   2640
      Width           =   855
   End
   Begin VB.CommandButton cmdInitialize 
      Caption         =   "&Initialize"
      Height          =   375
      Left            =   1080
      TabIndex        =   2
      Top             =   2640
      Width           =   975
   End
   Begin VB.CommandButton cmdRefresh 
      Caption         =   "&Refresh"
      Height          =   375
      Left            =   2160
      TabIndex        =   1
      Top             =   2640
      Width           =   975
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "&Close"
      Height          =   375
      Left            =   3240
      TabIndex        =   0
      Top             =   2640
      Width           =   975
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   2415
      Left            =   0
      TabIndex        =   3
      Top             =   120
      Width           =   4335
      _ExtentX        =   7646
      _ExtentY        =   4260
      _Version        =   393216
      Tabs            =   5
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   "&X-Axis"
      TabPicture(0)   =   "RAOBo.frx":0442
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label3"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label4"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "HScroll(0)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "HScroll(1)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "txtAxis(0)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "txtAxis(1)"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Frame4"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "cmbXunit"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).ControlCount=   8
      TabCaption(1)   =   "&Y-Axis"
      TabPicture(1)   =   "RAOBo.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame3"
      Tab(1).Control(1)=   "Frame2"
      Tab(1).ControlCount=   2
      TabCaption(2)   =   "&Z-Axis"
      TabPicture(2)   =   "RAOBo.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
      TabCaption(3)   =   "&Flags"
      TabPicture(3)   =   "RAOBo.frx":0496
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "cmdDisableAll"
      Tab(3).Control(1)=   "chkShowPA"
      Tab(3).Control(2)=   "chkSelectLevel"
      Tab(3).Control(3)=   "chkTropopause"
      Tab(3).Control(4)=   "chkAircraft"
      Tab(3).Control(5)=   "chkDocument"
      Tab(3).ControlCount=   6
      TabCaption(4)   =   "Format"
      TabPicture(4)   =   "RAOBo.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame1"
      Tab(4).ControlCount=   1
      Begin VB.CommandButton cmdDisableAll 
         Caption         =   "Disable All"
         Height          =   375
         Left            =   -71760
         TabIndex        =   38
         Top             =   1920
         Width           =   975
      End
      Begin VB.CheckBox chkShowPA 
         Caption         =   "Show Pressure Altitude"
         Height          =   255
         Left            =   -74880
         TabIndex        =   37
         Top             =   1200
         Width           =   2055
      End
      Begin VB.CheckBox chkSelectLevel 
         Caption         =   "Show Selected Level"
         Height          =   255
         Left            =   -72720
         TabIndex        =   36
         Top             =   840
         Width           =   1815
      End
      Begin VB.ComboBox cmbXunit 
         Height          =   315
         Left            =   3600
         TabIndex        =   35
         Top             =   480
         Width           =   615
      End
      Begin VB.Frame Frame1 
         Caption         =   "Font Size"
         Height          =   855
         Left            =   -74880
         TabIndex        =   31
         Top             =   360
         Width           =   975
         Begin VB.VScrollBar vscFontSize 
            Height          =   495
            Left            =   600
            Max             =   0
            Min             =   30
            TabIndex        =   34
            Top             =   240
            Width           =   255
         End
         Begin VB.TextBox txtFontSize 
            Height          =   285
            Left            =   120
            TabIndex        =   33
            Top             =   240
            Width           =   375
         End
         Begin VB.CheckBox chkFontSize 
            Caption         =   "Check1"
            Height          =   195
            Left            =   120
            TabIndex        =   32
            Top             =   600
            Width           =   255
         End
      End
      Begin VB.Frame Frame2 
         Caption         =   "Primary"
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
         TabIndex        =   17
         Top             =   360
         Width           =   3975
         Begin VB.ComboBox cmbYunitl 
            Height          =   315
            Left            =   3000
            TabIndex        =   27
            Top             =   240
            Width           =   855
         End
         Begin VB.OptionButton optYcoordl 
            Caption         =   "gALT"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   26
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton optYcoordl 
            Caption         =   "pALT"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   25
            Top             =   240
            Width           =   855
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   2
            Left            =   1920
            Max             =   40
            TabIndex        =   21
            Top             =   600
            Width           =   1935
         End
         Begin VB.HScrollBar HScroll 
            Height          =   255
            Index           =   3
            Left            =   1920
            Max             =   40
            TabIndex        =   20
            Top             =   960
            Width           =   1935
         End
         Begin VB.TextBox txtAxis 
            Height          =   285
            Index           =   2
            Left            =   1320
            TabIndex        =   19
            Top             =   600
            Width           =   495
         End
         Begin VB.TextBox txtAxis 
            Height          =   285
            Index           =   3
            Left            =   1320
            TabIndex        =   18
            Top             =   960
            Width           =   495
         End
         Begin VB.Label Label2 
            Caption         =   "Bottom [km]"
            Height          =   255
            Left            =   120
            TabIndex        =   23
            Top             =   600
            Width           =   975
         End
         Begin VB.Label Label1 
            Caption         =   "Top [km]"
            Height          =   255
            Left            =   120
            TabIndex        =   22
            Top             =   960
            Width           =   735
         End
      End
      Begin VB.Frame Frame3 
         Caption         =   "Secondary"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   -74880
         TabIndex        =   16
         Top             =   1680
         Width           =   3975
         Begin VB.OptionButton optYcoordr 
            Caption         =   "gALT"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   30
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton optYcoordr 
            Caption         =   "pALT"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   29
            Top             =   240
            Width           =   735
         End
         Begin VB.ComboBox cmbYunitr 
            Height          =   315
            Left            =   3000
            TabIndex        =   28
            Top             =   240
            Width           =   855
         End
      End
      Begin VB.CheckBox chkTropopause 
         Caption         =   "Show Tropopause"
         Height          =   375
         Left            =   -74880
         TabIndex        =   15
         Top             =   480
         Width           =   1695
      End
      Begin VB.CheckBox chkAircraft 
         Caption         =   "Show A/C Altitude"
         Height          =   375
         Left            =   -72720
         TabIndex        =   14
         Top             =   480
         Width           =   1695
      End
      Begin VB.CheckBox chkDocument 
         Caption         =   "Show Documentation"
         Height          =   255
         Left            =   -74880
         TabIndex        =   13
         Top             =   840
         Width           =   1935
      End
      Begin VB.Frame Frame4 
         Height          =   615
         Left            =   120
         TabIndex        =   10
         Top             =   360
         Width           =   3375
         Begin VB.OptionButton optAbscissa 
            Caption         =   "Temperature"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   12
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optAbscissa 
            Caption         =   "Potential Temperature"
            Height          =   255
            Index           =   1
            Left            =   1320
            TabIndex        =   11
            Top             =   240
            Width           =   1935
         End
      End
      Begin VB.TextBox txtAxis 
         Height          =   285
         Index           =   1
         Left            =   1440
         TabIndex        =   7
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtAxis 
         Height          =   285
         Index           =   0
         Left            =   1440
         TabIndex        =   6
         Top             =   1080
         Width           =   495
      End
      Begin VB.HScrollBar HScroll 
         Height          =   255
         Index           =   1
         Left            =   2040
         Max             =   340
         Min             =   180
         SmallChange     =   5
         TabIndex        =   5
         Top             =   1440
         Value           =   180
         Width           =   2175
      End
      Begin VB.HScrollBar HScroll 
         Height          =   255
         Index           =   0
         Left            =   2040
         Max             =   340
         Min             =   180
         SmallChange     =   5
         TabIndex        =   4
         Top             =   1080
         Value           =   180
         Width           =   2175
      End
      Begin VB.Label Label4 
         Caption         =   "Right  [K]"
         Height          =   255
         Left            =   240
         TabIndex        =   9
         Top             =   1440
         Width           =   975
      End
      Begin VB.Label Label3 
         Caption         =   "Left  [K["
         Height          =   255
         Left            =   240
         TabIndex        =   8
         Top             =   1080
         Width           =   855
      End
   End
End
Attribute VB_Name = "frmFormatRAOB"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim C%

Dim T1s%, T2s%, PT1s%, PT2s%
Dim zbkms%, ztkms%, zbkfts%, ztkfts%, zbmbs%, ztmbs%, zbhPas%, zthPas%

Sub Initialize()
'If frmRAOB.RAOBFileOpen Then frmRAOB.SetDefaultScales
C% = frmRAOB.Xcoord
optAbscissa(frmRAOB.Xcoord).Value = True
optYcoordl(frmRAOB.Ycoordl).Value = True
optYcoordr(frmRAOB.Ycoordr).Value = True

cmbXunit.Clear
cmbXunit.AddItem "K"
cmbXunit.AddItem "C"
cmbXunit.AddItem "F"
cmbXunit.ListIndex = frmRAOB.Xunit

cmbYunitl.Clear
cmbYunitl.AddItem "km"
'cmbYunitl.AddItem "kft"
cmbYunitl.ListIndex = frmRAOB.Yunitl

cmbYunitr.Clear
cmbYunitr.AddItem "km"
cmbYunitr.AddItem "kft"
cmbYunitr.AddItem "mb"
cmbYunitr.AddItem "hPa"
cmbYunitr.ListIndex = frmRAOB.Yunitr

Select Case frmRAOB.Xcoord
Case 0
  HScroll(0).Min = 150
  HScroll(0).Max = 350
  HScroll(0).Value = frmRAOB.T1K
  HScroll(1).Min = 150
  HScroll(1).Max = 350
  HScroll(1).Value = frmRAOB.T2K
Case 1
  HScroll(0).Min = 180
  HScroll(0).Max = 600
  HScroll(0).Value = frmRAOB.PT1K
  HScroll(1).Min = 180
  HScroll(1).Max = 600
  HScroll(1).Value = frmRAOB.PT2K
End Select

Select Case frmRAOB.Yunitl
Case 0 'km
  HScroll(2).Min = 0
  HScroll(2).Max = 50
  HScroll(2).Value = frmRAOB.zbkm
  HScroll(3).Min = 0
  HScroll(3).Max = 50
  HScroll(3).Value = frmRAOB.ztkm
Case 1  'kft
  HScroll(2).Min = 0
  HScroll(2).Max = 100
  HScroll(2).Value = frmRAOB.zbkft
  HScroll(3).Min = 0
  HScroll(3).Max = 100
  HScroll(3).Value = frmRAOB.ztkft

Case 2  'mb
  HScroll(2).Min = 0
  HScroll(2).Max = 1050
  HScroll(2).Value = frmRAOB.zbmb
  HScroll(3).Min = 0
  HScroll(3).Max = 1050
  HScroll(2).Value = frmRAOB.ztmb
Case 3  'kPa
  HScroll(2).Min = 0
  HScroll(2).Max = 105
  HScroll(2).Value = frmRAOB.zbhPa
  HScroll(3).Min = 0
  HScroll(3).Max = 105
  HScroll(2).Value = frmRAOB.zthPa
End Select

For i% = 0 To 3: txtAxis(i%).Text = Str$(HScroll(i%).Value): Next

If frmRAOB.ShowAircraft Then chkAircraft.Value = 1 Else chkAircraft.Value = 0
If frmRAOB.ShowTropopause Then chkTropopause.Value = 1 Else chkTropopause.Value = 0
If frmRAOB.ShowDocument Then chkDocument.Value = 1 Else chkDocument.Value = 0
If frmRAOB.ShowSelectLevel Then chkSelectLevel.Value = 1 Else chkSelectLevel.Value = 0
frmFormatRAOB.cmdDisableAll.Caption = "Disable All"
End Sub

Sub SaveInState()

T1s = frmRAOB.T1K
T2s = frmRAOB.T2K
PT1s = frmRAOB.PT1K
PT2s = frmRAOB.PT2K

zbkms = frmRAOB.zbkm
ztkms = frmRAOB.ztkm
zbkfts = frmRAOB.zbkft
ztkfts = frmRAOB.ztkft
zbmbs = frmRAOB.zbmb
ztmbs = frmRAOB.ztmb
zbhPas = frmRAOB.zbhPa
zthPas = frmRAOB.zthPa

End Sub

Private Sub chkAircraft_Click()

If chkAircraft.Value = 1 Then
  frmRAOB.ShowAircraft = True
Else
  frmRAOB.ShowAircraft = False
End If
End Sub

Private Sub chkDocument_Click()

If chkDocument.Value = 1 Then
  frmRAOB.ShowDocument = True
Else
  frmRAOB.ShowDocument = False
End If

End Sub


Private Sub chkSelectLevel_Click()
If chkSelectLevel.Value = 1 Then
  frmRAOB.ShowSelectLevel = True
Else
  frmRAOB.ShowSelectLevel = False
End If

End Sub

Private Sub chkShowPA_Click()

If chkShowPA.Value = 1 Then
  frmRAOB.ShowPressureAltitude = True
Else
  frmRAOB.ShowPressureAltitude = False
End If

End Sub

Private Sub chkTropopause_Click()

If chkTropopause.Value = 1 Then
  frmRAOB.ShowTropopause = True
Else
  frmRAOB.ShowTropopause = False
End If
End Sub




Private Sub cmbYunitl_Click()
frmRAOB.Yunitl = cmbYunitl.ListIndex
Select Case frmRAOB.Yunitl
Case 0 'km
  HScroll(2).Min = 0
  HScroll(2).Max = 50
  HScroll(2).Value = frmRAOB.zbkm
  HScroll(3).Min = 0
  HScroll(3).Max = 50
  HScroll(3).Value = frmRAOB.ztkm
Case 1  'kft
  HScroll(2).Min = 0
  HScroll(2).Max = 100
  HScroll(2).Value = frmRAOB.zbkft
  HScroll(3).Min = 0
  HScroll(3).Max = 100
  HScroll(3).Value = frmRAOB.ztkft
Case 2  'mb
  HScroll(2).Min = 0
  HScroll(2).Max = 1050
  HScroll(2).Value = frmRAOB.zbmb
  HScroll(3).Min = 0
  HScroll(3).Max = 1050
  HScroll(2).Value = frmRAOB.ztmb
Case 3  'kPa
  HScroll(2).Min = 0
  HScroll(2).Max = 105
  HScroll(2).Value = frmRAOB.zbhPa
  HScroll(3).Min = 0
  HScroll(3).Max = 105
  HScroll(2).Value = frmRAOB.zthPa
End Select
End Sub


Private Sub cmbYunitr_Click()
frmRAOB.Yunitr = cmbYunitr.ListIndex
End Sub


Private Sub cmdClose_Click()

With frmRAOB
  Select Case .Xcoord
  Case 0
    .T1 = Val(txtAxis(0).Text)
    .T2 = Val(txtAxis(1).Text)
  Case 1
    .PT1 = Val(txtAxis(0).Text)
    .PT2 = Val(txtAxis(1).Text)
  End Select

  Select Case .Ycoordl
  Case 0
    .Zb = Val(txtAxis(2).Text)
    .Zt = Val(txtAxis(3).Text)
  Case 1
    .zbkft = Val(txtAxis(2).Text)
    .ztkft = Val(txtAxis(3).Text)
  End Select
' Note that secondary axis is slaved to primary
' Need only select units
End With

frmFormatRAOB.Visible = False
DoEvents

End Sub

Private Sub cmdDisableAll_Click()
If cmdDisableAll.Caption = "Disable All" Then
  cmdDisableAll.Caption = "Enable All"
  chkAircraft.Value = 0
  chkTropopause.Value = 0
  chkDocument.Value = 0
  chkSelectLevel.Value = 0
  chkShowPA.Value = 0
Else
  cmdDisableAll.Caption = "Disable All"
  chkAircraft.Value = 1
  chkTropopause.Value = 1
  chkDocument.Value = 1
  chkSelectLevel.Value = 1
  chkShowPA.Value = 1
End If
  chkAircraft_Click
  chkTropopause_Click
  chkDocument_Click
  chkSelectLevel_Click
  chkShowPA_Click
End Sub

Private Sub cmdExit_Click()
' Restore original values

With frmRAOB
.T1K = T1s
.T2K = T2s
.PT1K = PT1s
.PT2K = PT2s

.zbkm = zbkms
.ztkm = ztkms
.zbkft = zbkfts
.ztkft = ztkfts
.zbmb = zbmbs
.ztmb = ztmbs
.zbhPa = zbhPas
.zthPa = zthPas
End With
frmFormatRAOB.Visible = False

End Sub

Private Sub cmdInitialize_Click()

With frmRAOB
  .T1 = .T1def
  .T2 = f.T2def
  .PT1 = .PT1Def
  .PT2 = .PT2Def
  .Zb = .zbDef
  .Zt = .ztDef
  .zbkft = .zbkftDef
  .ztkft = .ztkftDef
  Initialize
  .RAOBupdate
End With

End Sub

Private Sub cmdRefresh_Click()

With frmRAOB
  Select Case frmRAOB.Xcoord
  Case 0
  .T1K = Val(txtAxis(0).Text)
  .T2K = Val(txtAxis(1).Text)
  Case 1
  .PT1K = Val(txtAxis(0).Text)
  .PT2K = Val(txtAxis(1).Text)
  End Select

  Select Case Ycoordl
  Case 0
  .zbkm = Val(txtAxis(2).Text)
  .ztkm = Val(txtAxis(3).Text)
  Case 1
  .zbkft = Val(txtAxis(2).Text)
  .ztkft = Val(txtAxis(3).Text)
  Case 2
  .zbmb = Val(txtAxis(2).Text)
  .ztmb = Val(txtAxis(3).Text)
  Case 3
  .zbhPa = Val(txtAxis(2).Text)
  .zthPa = Val(txtAxis(3).Text)
  End Select
' Note that secondary axis is slaved to primary
' Need only select units
  DoEvents
  .RAOBupdate
End With

End Sub


Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()

'Show

SaveInState
Initialize

End Sub



Private Sub HScroll_Change(Index As Integer)
  txtAxis(Index).Text = Str$(HScroll(Index).Value)
End Sub


Private Sub optAbscissa_Click(Index As Integer)

frmRAOB.Xcoord = Index
Select Case Index
Case 0                      'Temperature
  HScroll(0).Min = 150
  HScroll(0).Max = 350
  HScroll(0).Value = frmRAOB.T1K
  HScroll(1).Min = 150
  HScroll(1).Max = 350
  HScroll(1).Value = frmRAOB.T2K
Case 1                      'Potential Temperature
  HScroll(0).Min = 180
  HScroll(0).Max = 600
  HScroll(0).Value = frmRAOB.PT1K
  HScroll(1).Min = 180
  HScroll(1).Max = 600
  HScroll(1).Value = frmRAOB.PT2K
End Select
txtAxis(3).Text = 20#

End Sub

Private Sub txtAxis_Change(Index As Integer)
  
  HScroll(Index).Value = Val(txtAxis(Index).Text)

End Sub



Private Sub vscFontSize_Change()
txtFontSize = Str$(vscFontSize.Value)
End Sub


