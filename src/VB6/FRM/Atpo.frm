VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmFormatATP 
   Caption         =   "ATP Display Options"
   ClientHeight    =   3060
   ClientLeft      =   5985
   ClientTop       =   7425
   ClientWidth     =   4335
   Icon            =   "Atpo.frx":0000
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
      Tab             =   3
      TabsPerRow      =   5
      TabHeight       =   520
      TabCaption(0)   =   "&X-Axis"
      TabPicture(0)   =   "Atpo.frx":0442
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Label3"
      Tab(0).Control(1)=   "Label4"
      Tab(0).Control(2)=   "HScroll(0)"
      Tab(0).Control(3)=   "HScroll(1)"
      Tab(0).Control(4)=   "txtAxis(0)"
      Tab(0).Control(5)=   "txtAxis(1)"
      Tab(0).Control(6)=   "Frame4"
      Tab(0).Control(7)=   "cmbXunit"
      Tab(0).ControlCount=   8
      TabCaption(1)   =   "&Y-Axis"
      TabPicture(1)   =   "Atpo.frx":045E
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame3"
      Tab(1).Control(1)=   "Frame2"
      Tab(1).ControlCount=   2
      TabCaption(2)   =   "&Z-Axis"
      TabPicture(2)   =   "Atpo.frx":047A
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
      TabCaption(3)   =   "&Flags"
      TabPicture(3)   =   "Atpo.frx":0496
      Tab(3).ControlEnabled=   -1  'True
      Tab(3).Control(0)=   "chkDocument"
      Tab(3).Control(0).Enabled=   0   'False
      Tab(3).Control(1)=   "chkAircraft"
      Tab(3).Control(1).Enabled=   0   'False
      Tab(3).Control(2)=   "chkTropopause"
      Tab(3).Control(2).Enabled=   0   'False
      Tab(3).Control(3)=   "cmbFooter"
      Tab(3).Control(3).Enabled=   0   'False
      Tab(3).Control(4)=   "chkTexAQS"
      Tab(3).Control(4).Enabled=   0   'False
      Tab(3).Control(5)=   "chkShowPSCthresholds"
      Tab(3).Control(5).Enabled=   0   'False
      Tab(3).Control(6)=   "cmdDefaultBG"
      Tab(3).Control(6).Enabled=   0   'False
      Tab(3).Control(7)=   "cmdBorder"
      Tab(3).Control(7).Enabled=   0   'False
      Tab(3).Control(8)=   "cmdSaveProfile"
      Tab(3).Control(8).Enabled=   0   'False
      Tab(3).Control(9)=   "txtSaveProfile"
      Tab(3).Control(9).Enabled=   0   'False
      Tab(3).Control(10)=   "chkAAprofile"
      Tab(3).Control(10).Enabled=   0   'False
      Tab(3).ControlCount=   11
      TabCaption(4)   =   "Format"
      TabPicture(4)   =   "Atpo.frx":04B2
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "Frame1"
      Tab(4).ControlCount=   1
      Begin VB.CheckBox chkAAprofile 
         Caption         =   "Show AA Profile"
         Height          =   225
         Left            =   120
         TabIndex        =   43
         Top             =   390
         Width           =   1815
      End
      Begin VB.TextBox txtSaveProfile 
         Height          =   345
         Left            =   1290
         TabIndex        =   42
         Text            =   "C:\MTP\PNG\ATP"
         Top             =   1980
         Width           =   2955
      End
      Begin VB.CommandButton cmdSaveProfile 
         Caption         =   "Save Profile"
         Height          =   375
         Left            =   90
         TabIndex        =   41
         Top             =   1950
         Width           =   1155
      End
      Begin VB.CommandButton cmdBorder 
         Caption         =   "Border Style"
         Height          =   375
         Left            =   2010
         TabIndex        =   40
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdDefaultBG 
         Caption         =   "Default BG"
         Height          =   375
         Left            =   3120
         TabIndex        =   39
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CheckBox chkShowPSCthresholds 
         Caption         =   "Show PSC thresholds"
         Height          =   255
         Left            =   120
         TabIndex        =   38
         Top             =   1710
         Width           =   1935
      End
      Begin VB.CheckBox chkTexAQS 
         Caption         =   "Use TexAQS Scales"
         Height          =   255
         Left            =   120
         TabIndex        =   37
         Top             =   1440
         Width           =   1815
      End
      Begin VB.ComboBox cmbFooter 
         Height          =   315
         Left            =   2010
         TabIndex        =   36
         Top             =   930
         Width           =   2205
      End
      Begin VB.ComboBox cmbXunit 
         Height          =   315
         Left            =   -71400
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
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   660
         Width           =   1695
      End
      Begin VB.CheckBox chkAircraft 
         Caption         =   "Show A/C Altitude"
         Height          =   375
         Left            =   120
         TabIndex        =   14
         Top             =   1140
         Width           =   1695
      End
      Begin VB.CheckBox chkDocument 
         Caption         =   "Show Documentation"
         Height          =   255
         Left            =   120
         TabIndex        =   13
         Top             =   930
         Width           =   1935
      End
      Begin VB.Frame Frame4 
         Height          =   615
         Left            =   -74880
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
         Left            =   -73560
         TabIndex        =   7
         Top             =   1440
         Width           =   495
      End
      Begin VB.TextBox txtAxis 
         Height          =   285
         Index           =   0
         Left            =   -73560
         TabIndex        =   6
         Top             =   1080
         Width           =   495
      End
      Begin VB.HScrollBar HScroll 
         Height          =   255
         Index           =   1
         Left            =   -72960
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
         Left            =   -72960
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
         Left            =   -74760
         TabIndex        =   9
         Top             =   1440
         Width           =   975
      End
      Begin VB.Label Label3 
         Caption         =   "Left  [K["
         Height          =   255
         Left            =   -74760
         TabIndex        =   8
         Top             =   1080
         Width           =   855
      End
   End
End
Attribute VB_Name = "frmFormatATP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim C%

Dim T1s%, T2s%, PT1s%, PT2s%
Dim zbkms%, ztkms%, zbkfts%, ztkfts%, zbmbs%, ztmbs%, zbhPas%, zthPas%

Public DefaultBG As Boolean
Sub Initialize()
Dim i%

'If frmATP.ATPFileOpen Then frmATP.SetDefaultScales
C% = frmATP.Xcoord
optAbscissa(frmATP.Xcoord).Value = True
optYcoordl(frmATP.Ycoordl).Value = True
optYcoordr(frmATP.Ycoordr).Value = True

cmbXunit.Clear
cmbXunit.AddItem "K"
cmbXunit.AddItem "C"
cmbXunit.AddItem "F"
cmbXunit.ListIndex = frmATP.Xunit

cmbYunitl.Clear
cmbYunitl.AddItem "km"
'cmbYunitl.AddItem "kft"
cmbYunitl.ListIndex = frmATP.Yunitl

cmbYunitr.Clear
cmbYunitr.AddItem "km"
cmbYunitr.AddItem "kft"
cmbYunitr.AddItem "mb"
cmbYunitr.AddItem "hPa"
cmbYunitr.ListIndex = frmATP.Yunitr

Select Case frmATP.Xcoord
Case 0
  HScroll(0).Min = 150
  HScroll(0).Max = 350
  HScroll(0).Value = frmATP.T1K
  HScroll(1).Min = 150
  HScroll(1).Max = 350
  HScroll(1).Value = frmATP.T2K
Case 1
  HScroll(0).Min = 200
  HScroll(0).Max = 600
  HScroll(0).Value = frmATP.PT1K
  HScroll(1).Min = 200
  HScroll(1).Max = 600
  HScroll(1).Value = frmATP.PT2K
End Select

Select Case frmATP.Yunitl
Case 0 'km
  HScroll(2).Min = 0
  HScroll(2).Max = 50
  HScroll(2).Value = frmATP.zbkm
  HScroll(3).Min = 0
  HScroll(3).Max = 50
  HScroll(3).Value = frmATP.ztkm
Case 1  'kft
  HScroll(2).Min = 0
  HScroll(2).Max = 100
  HScroll(2).Value = frmATP.zbkft
  HScroll(3).Min = 0
  HScroll(3).Max = 100
  HScroll(3).Value = frmATP.ztkft

Case 2  'mb
  HScroll(2).Min = 0
  HScroll(2).Max = 1050
  HScroll(2).Value = frmATP.zbmb
  HScroll(3).Min = 0
  HScroll(3).Max = 1050
  HScroll(2).Value = frmATP.ztmb
Case 3  'kPa
  HScroll(2).Min = 0
  HScroll(2).Max = 105
  HScroll(2).Value = frmATP.zbhPa
  HScroll(3).Min = 0
  HScroll(3).Max = 105
  HScroll(2).Value = frmATP.zthPa
End Select

For i% = 0 To 3: txtAxis(i%).Text = Str$(HScroll(i%).Value): Next

If frmATP.ShowAircraft Then chkAircraft.Value = 1 Else chkAircraft.Value = 0
If frmATP.ShowAAprofile Then chkAAprofile.Value = 1 Else chkAAprofile.Value = 0
If frmATP.ShowTropopause Then chkTropopause.Value = 1 Else chkTropopause.Value = 0
If frmATP.ShowDocument Then chkDocument.Value = 1 Else chkDocument.Value = 0

cmbFooter.Clear
cmbFooter.AddItem "Default"
cmbFooter.AddItem "2 Channel Gains"
cmbFooter.AddItem "3 Channel Gains"
cmbFooter.AddItem "Some Mux Data, CB, dND"
cmbFooter.AddItem "All Mux Data"
cmbFooter.AddItem "B,C Line Differences"
cmbFooter.AddItem "d,E Line Differences"
cmbFooter.ListIndex = frmATP.FooterType


End Sub

Sub SaveInState()

T1s = frmATP.T1K
T2s = frmATP.T2K
PT1s = frmATP.PT1K
PT2s = frmATP.PT2K

zbkms = frmATP.zbkm
ztkms = frmATP.ztkm
zbkfts = frmATP.zbkft
ztkfts = frmATP.ztkft
zbmbs = frmATP.zbmb
ztmbs = frmATP.ztmb
zbhPas = frmATP.zbhPa
zthPas = frmATP.zthPa

End Sub

Private Sub chkAAprofile_Click()
If chkAAprofile.Value = 1 Then
  frmATP.ShowAAprofile = True
Else
  frmATP.ShowAAprofile = False
End If
End Sub

Private Sub chkAircraft_Click()

If chkAircraft.Value = 1 Then
  frmATP.ShowAircraft = True
Else
  frmATP.ShowAircraft = False
End If
End Sub

Private Sub chkDocument_Click()

If chkDocument.Value = 1 Then
  frmATP.ShowDocument = True
Else
  frmATP.ShowDocument = False
End If

End Sub


Private Sub chkShowPSCthresholds_Click()
If chkShowPSCthresholds.Value = 1 Then
  frmATP.ShowPSCthresholds = True
Else
  frmATP.ShowPSCthresholds = False
End If

End Sub


Private Sub chkTexAQS_Click()
If chkTexAQS.Value = 1 Then
  frmATP.UseTexAQS = True
Else
  frmATP.UseTexAQS = False
End If
frmATP.ATP_Initialize
With frmATP
  .ShowAircraft = True
  .ShowTropopause = True
  .ShowDocument = False
'  .ShowSelectLevel = False
'  .ShowPressureAltitude = False
End With

frmATP.ATPupdate
End Sub

Private Sub chkTropopause_Click()

If chkTropopause.Value = 1 Then
  frmATP.ShowTropopause = True
Else
  frmATP.ShowTropopause = False
End If
End Sub




Private Sub cmbFooter_Click()
frmATP.FooterType = cmbFooter.ListIndex
End Sub


Private Sub cmbYunitl_Click()
frmATP.Yunitl = cmbYunitl.ListIndex
Select Case frmATP.Yunitl
Case 0 'km
  HScroll(2).Min = 0
  HScroll(2).Max = 50
  HScroll(2).Value = frmATP.zbkm
  HScroll(3).Min = 0
  HScroll(3).Max = 50
  HScroll(3).Value = frmATP.ztkm
Case 1  'kft
  HScroll(2).Min = 0
  HScroll(2).Max = 100
  HScroll(2).Value = frmATP.zbkft
  HScroll(3).Min = 0
  HScroll(3).Max = 100
  HScroll(3).Value = frmATP.ztkft
Case 2  'mb
  HScroll(2).Min = 0
  HScroll(2).Max = 1050
  HScroll(2).Value = frmATP.zbmb
  HScroll(3).Min = 0
  HScroll(3).Max = 1050
  HScroll(2).Value = frmATP.ztmb
Case 3  'kPa
  HScroll(2).Min = 0
  HScroll(2).Max = 105
  HScroll(2).Value = frmATP.zbhPa
  HScroll(3).Min = 0
  HScroll(3).Max = 105
  HScroll(2).Value = frmATP.zthPa
End Select
End Sub


Private Sub cmbYunitr_Click()
frmATP.Yunitr = cmbYunitr.ListIndex
End Sub


Private Sub cmdBorder_Click()
'Static BorderStyle%
If BorderStyle = 2 Then BorderStyle = 0 Else BorderStyle = 2
frmATP.BorderStyle = BorderStyle
frmATP.ATP_Initialize
End Sub

Private Sub cmdClose_Click()

With frmATP
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

DoEvents
frmFormatATP.Visible = False

End Sub

Private Sub cmdDefaultBG_Click()
  DefaultBG = Not DefaultBG
  If DefaultBG Then
' And assign to some attributes to facilitate changing
  With frmATP
    .LRL = White               'Lapse Rate lines
    .LRP = LGray               'Lapse Rate pointers
    .ACP = White               'Aircraft pointer
    .ACS = White               'Aircraft symbol
    .PBG = Blue                'Plot Background color
    .PBC = Black               'Plot border color
    .PFG = Yellow              'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Yellow              'Plot tic marks and border
    .Pgm = Yellow              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
  End With
  Else
  With frmATP
    .LRL = White               'Lapse Rate lines
    .LRP = Black 'BGray               'Lapse Rate pointers
    .ACP = bGray               'Aircraft pointer
    .ACS = bGray               'Aircraft symbol
    .PBG = White                'Plot Background color
    .PBC = White               'Plot border color
    .PFG = Black              'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Black              'Plot tic marks and border
    .Pgm = Black 'LGray              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
  End With
  End If

End Sub

Private Sub cmdExit_Click()
' Restore original values

With frmATP
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

Unload Me

End Sub

Private Sub cmdInitialize_Click()

With frmATP
  .T1 = .T1def
  .T2 = .T2def
  .PT1 = .PT1Def
  .PT2 = .PT2Def
  .Zb = .zbDef
  .Zt = .ztDef
  .zbkft = .zbkftDef
  .ztkft = .ztkftDef
  Initialize
  .ATPupdate
End With

End Sub

Public Sub cmdRefresh_Click()

With frmATP
  Select Case .Xcoord
  Case 0
  .T1K = Val(txtAxis(0).Text)
  .T2K = Val(txtAxis(1).Text)
  Case 1
  .PT1K = Val(txtAxis(0).Text)
  .PT2K = Val(txtAxis(1).Text)
  End Select

  Select Case .Ycoordl
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
  .ATPupdate
End With

End Sub


Private Sub cmdSaveProfile_Click()
Dim i%, j%, Filename$, Status As Boolean
  
  Filename$ = txtSaveProfile.Text
  i% = InStr(Filename$, ".")
  If i > 0 Then Filename$ = Left$(Filename$, i - 1)
  frmATP.ScaleMode = 1
  'Status = SaveFormAsPNG(frmATP, Filename$ + ".PNG")
  CaptureFormMJ frmATP, Filename$ + ".PNG", 1, True
  'If Not Status Then MsgBox "PNG file of CTC was not saved!", vbOKOnly
  txtSaveProfile.Text = Filename$ + ".PNG"

End Sub


Private Sub Form_DblClick()
  CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
  Top = frmATP.Top '+ (frmATP.Height - Height) / 2
  Left = frmATP.Left + (frmATP.Width) '- Width) / 2
'  Show
  SaveInState
  Initialize
End Sub



Private Sub HScroll_Change(Index As Integer)
  txtAxis(Index).Text = Str$(HScroll(Index).Value)
End Sub


Private Sub optAbscissa_Click(Index As Integer)

frmATP.Xcoord = Index
Select Case Index
Case 0                      'Temperature
  HScroll(0).Min = 150
  HScroll(0).Max = 350
  HScroll(0).Value = frmATP.T1K
  HScroll(1).Min = 150
  HScroll(1).Max = 350
  HScroll(1).Value = frmATP.T2K
Case 1                      'Potential Temperature
  HScroll(0).Min = 200
  HScroll(0).Max = 600
  HScroll(0).Value = frmATP.PT1K
  HScroll(1).Min = 200
  HScroll(1).Max = 600
  HScroll(1).Value = frmATP.PT2K
End Select

End Sub

Private Sub txtAxis_Change(Index As Integer)
  
  HScroll(Index).Value = Val(txtAxis(Index).Text)

End Sub



Private Sub vscFontSize_Change()
txtFontSize = Str$(vscFontSize.Value)
End Sub


