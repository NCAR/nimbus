VERSION 5.00
Begin VB.Form frmTcalNGV 
   Caption         =   "Form1"
   ClientHeight    =   4710
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5940
   LinkTopic       =   "Form1"
   ScaleHeight     =   4710
   ScaleWidth      =   5940
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtFltDate 
      BackColor       =   &H0080FF80&
      Height          =   375
      Left            =   3420
      TabIndex        =   19
      Text            =   "20060325"
      Top             =   3570
      Width           =   870
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "Exit"
      Height          =   585
      Left            =   4635
      TabIndex        =   17
      Top             =   3990
      Width           =   1200
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "Extract"
      Height          =   585
      Left            =   3390
      TabIndex        =   16
      Top             =   3990
      Width           =   1200
   End
   Begin VB.Frame Frame 
      Caption         =   "Flight Dates to Process"
      Height          =   3075
      Index           =   5
      Left            =   600
      TabIndex        =   7
      Top             =   0
      Width           =   2265
      Begin VB.TextBox txtFltDates 
         Height          =   285
         Left            =   90
         TabIndex        =   15
         Top             =   240
         Width           =   1125
      End
      Begin VB.ListBox lstFltDates 
         Height          =   2400
         ItemData        =   "frmTcalNGV.frx":0000
         Left            =   90
         List            =   "frmTcalNGV.frx":0007
         TabIndex        =   14
         ToolTipText     =   "Flight Dates to be Batch Processed"
         Top             =   570
         Width           =   1125
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Add"
         Height          =   315
         Index           =   0
         Left            =   1290
         TabIndex        =   13
         ToolTipText     =   "Add this Flight Date to List"
         Top             =   240
         Width           =   915
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Remove"
         Height          =   315
         Index           =   1
         Left            =   1290
         TabIndex        =   12
         ToolTipText     =   "Remove Selected Flighht Date from List"
         Top             =   600
         Width           =   915
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Clear"
         Height          =   315
         Index           =   2
         Left            =   1290
         TabIndex        =   11
         ToolTipText     =   "Clear Flight Date List"
         Top             =   960
         Width           =   915
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Default All"
         Height          =   315
         Index           =   3
         Left            =   1290
         TabIndex        =   10
         ToolTipText     =   "Load List with FLTinfo Flight Dates"
         Top             =   1320
         Width           =   915
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Default"
         Height          =   315
         Index           =   4
         Left            =   1290
         TabIndex        =   9
         ToolTipText     =   "Load List with MISSIONbatch.NUM"
         Top             =   1680
         Width           =   915
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Save"
         Height          =   315
         Index           =   5
         Left            =   1290
         TabIndex        =   8
         ToolTipText     =   "Save Flight Date List to MISSIONbatch.NUM"
         Top             =   2040
         Width           =   915
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "RAOB Range"
      Height          =   975
      Index           =   3
      Left            =   0
      TabIndex        =   0
      Top             =   3090
      Width           =   2865
      Begin VB.TextBox txtRmax 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2070
         MultiLine       =   -1  'True
         TabIndex        =   3
         Text            =   "frmTcalNGV.frx":0015
         Top             =   300
         Width           =   405
      End
      Begin VB.CommandButton cmdBatchProcessor 
         Caption         =   "Get RAOBs within"
         Height          =   405
         Index           =   1
         Left            =   90
         TabIndex        =   2
         Top             =   240
         Width           =   1905
      End
      Begin VB.TextBox txtALTkmMin 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2070
         MultiLine       =   -1  'True
         TabIndex        =   1
         Text            =   "frmTcalNGV.frx":001B
         Top             =   600
         Width           =   405
      End
      Begin VB.Label Label 
         Caption         =   "km"
         Height          =   225
         Index           =   124
         Left            =   2520
         TabIndex        =   6
         Top             =   330
         Width           =   255
      End
      Begin VB.Label Label 
         Caption         =   "Minimum Pressure Altitude"
         Height          =   165
         Index           =   24
         Left            =   120
         TabIndex        =   5
         Top             =   690
         Width           =   1905
      End
      Begin VB.Label Label 
         Caption         =   "km"
         Height          =   225
         Index           =   25
         Left            =   2520
         TabIndex        =   4
         Top             =   630
         Width           =   225
      End
   End
   Begin VB.Label Label1 
      Caption         =   "Flight Date (YYYYMMDD)"
      Height          =   285
      Left            =   3390
      TabIndex        =   18
      Top             =   3345
      Width           =   1935
   End
End
Attribute VB_Name = "frmTcalNGV"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Sub GetLevelFlightLegs(FltDate$, dUTmin!, PitchMax!, dZpMax!, dPitchMax!, Hist%(), HistStep!, OUTlu%)
Dim X!, i%, j%, ZpSave!(1 To 2000), ZpMax!, ZpMin, ZpAvg!, ZpRMS!, Sum1!, Sum2!
Dim dd$, mm$, yy$, Record%, Rcurrent!, Path$, Filename$, dpALTok As Boolean, nMax%
Dim PitchLast!, pALTlast!(1 To 10), dPitch!, dpALT!(1 To 10), UTks1!, UTks2!, UTks!, count%
Dim iMax%, zHistStep!
Static ZpFltMax!

  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  yy$ = Left$(FltDate$, 4)
  mm$ = Mid$(FltDate$, 5, 2)
  dd$ = Right$(FltDate$, 2)
  nMax = 10
  'ZpFltMax = 0#
  Filename$ = Path$ + FltDate$ + "\" + AC$ + FltDate$ + ".REF"
  'xxx mjm REFopen Filename$
  
  Record = 1: j = 0
  'Call REFread(REFlu, Record)
  For i = 1 To nMax:  pALTlast(i) = pALT: Next i
  Do
    UTks1 = 0#: UTks2 = 0#: count = 0
    Do
      Record = Record + 1
      'Call REFread(REFlu, Record)
    ' Accumulate data for Zp Histogram
      iMax = 22 / HistStep + 1
      Do
        zHistStep = -HistStep
        For i = 1 To iMax
          zHistStep = zHistStep + HistStep
          If pALT >= zHistStep And pALT < zHistStep + HistStep Then
            Hist(i) = Hist(i) + 1
            Exit Do
          End If
        Next i
      Loop
    ' Now do remaining statistics
      If pALT > ZpFltMax Then
      ZpFltMax = pALT
      End If
      For i = 1 To nMax: dpALT(i) = pALT - pALTlast(i):  Next i
      dpALTok = True
      For i = 1 To nMax
        If Abs(dpALT(i)) > dZpMax Then dpALTok = False: Exit For
      Next i
      For i = nMax To 2 Step -1: pALTlast(i) = pALTlast(i - 1): Next i
      pALTlast(1) = pALT
      dPitch = Pitch - PitchLast
      PitchLast = Pitch
      UTks = UTsec / 1000#

' Apply various limit requirements
      If dpALTok Then  'And Abs(dPitch) < dPitchMax And Abs(Pitch) < PitchMax Then
        count = count + 1
        ZpSave(count) = pALT
        If UTks1 = 0# Then  'Start a flight segment
          UTks1 = UTks
        Else                'End a flight segment
          UTks2 = UTks      'Keep updating level flight end UT
        End If
      Else
        If (UTks2 > 0#) And (UTks2 - UTks1 > dUTmin) Then Exit Do    'Limit to >10 min legs
        UTks1 = 0
        UTks2 = 0
        count = 0
      End If
      DoEvents
    Loop Until EOF(REFlu)
    If EOF(REFlu) Then
    Exit Do
    End If
      ZpMin = 999#: ZpMax = -999#: Sum1 = 0#: Sum2 = 0#
    For i = 1 To count
      If ZpSave(i) < ZpMin Then ZpMin = ZpSave(i)
      If ZpSave(i) > ZpMax Then ZpMax = ZpSave(i)
      Sum1 = Sum1 + ZpSave(i)         'Sum time difference
      Sum2 = Sum2 + ZpSave(i) ^ 2    'Sum square of time differences
    Next i
    ZpAvg = Sum1 / count        'Calculate average
    If Sum2 - ZpAvg ^ 2 * count > 0 Then ZpRMS = Sqr((Sum2 - ZpAvg ^ 2 * count) / (count - 1)) 'Calculate RMS
    Debug.Print " "; Format(count, "0000"); " "; Format(UTks1, "#00.000"); " "; Format(UTks2, "#00.000"); " "; Format(ZpAvg, "#0.00"); " "; Format(ZpRMS, "0.00"); " "; Format(ZpMin, "#0.00"); " "; Format(ZpMax, "#0.00"); " "; Format(ZpFltMax, "#0.00")
    Print #OUTlu, " "; Format(count, "0000"); " "; Format(UTks1, "#00.000"); " "; Format(UTks2, "#00.000"); " "; Format(ZpAvg, "#0.00"); " "; Format(ZpRMS, "0.00"); " "; Format(ZpMin, "#0.00"); " "; Format(ZpMax, "#0.00"); " "; Format(ZpFltMax, "#0.00")

  Loop Until EOF(REFlu)
  Close REFlu

' Print out closest approach list
  For i = 1 To j
'    If OUTlu <> 0 Then Print #OUTlu, yy$ + vbTab + mm$ + vbTab + dd$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + R$(i) + vbTab + R$(i) + vbTab + Format(Rsave(i), "000.0")
'    Debug.Print yy$ + vbTab + mm$ + vbTab + dd$ + vbTab + Format(UTks(i), "00.00") + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + "0" + vbTab + R$(i) + vbTab + R$(i) + vbTab + Format(Rsave(i), "000.0")
  Next i
      Debug.Print ZpFltMax

End Sub

Private Sub cmdBatchProcessor_Click(Index As Integer)
Dim Rmax!, OUTlu%, nSites%, i%, ZpMax!, dZpMax!, dPitchMax!, dUTmin!, PitchMax!
Dim Hist%(1 To 500), HistStep!, iMax%

  Select Case Index
  Case 1   'ID RAOB Range
    Rmax = Val(txtRmax.Text)     'Maximum Range for a RAOB selection
    OUTlu = FreeFile
    Open Drive$ + Rdir2$ + Mission$ + "\" + Mission$ + "_RAOBrange.txt" For Output As OUTlu
    nSites = 0 'Re-initialize so
    For i = 0 To lstFltDates.ListCount - 1
      FltDate$ = Trim(lstFltDates.List(i))
      lstFltDates.ListIndex = i  'Select current flight date
      txtFltDates.Text = FltDate$
      Call GetRAOBrange(FltDate$, Rmax, OUTlu, nSites%, Val(txtALTkmMin.Text))
    Next i
    Close OUTlu
    
  End Select
End Sub

Private Sub cmdExit_Click()
Unload Me
End Sub

Private Sub cmdFltDates_Click(Index As Integer)
Dim lu%, Path$, Filename$, FltNumber&, FltDate&, i%, a$

On Error GoTo ErrorHandler1

Select Case Index
Case 0  'Add
  lstFltDates.AddItem txtFltDates.Text

Case 1  'Remove
  i = lstFltDates.ListIndex
  lstFltDates.RemoveItem (lstFltDates.ListIndex)
  If i < lstFltDates.ListCount Then lstFltDates.ListIndex = i Else lstFltDates.ListIndex = i - 1

Case 2  'Clear
  lstFltDates.Clear

Case 3  'Default All MISSION.NUM
  lstFltDates.Clear
  For i = 0 To frmFLTINFO.cboFltDates.ListCount - 1
    lstFltDates.AddItem Trim(frmFLTINFO.cboFltDates.List(i))
  Next i
  Close lu

Case 4  'Default MISSIONbatch.NUM
  lstFltDates.Clear
  lu = FreeFile
  Path$ = "c:\MTP\Data\NGV\TREX\NG\"
  
  Filename$ = Dir$(Path$ + "*.txt")   'Check for RCF file first, then CFG
  If Len(Filename$) > 0 Then
    Do While Len(Filename$) > 0
     lstFltDates.AddItem Filename$
     ' NRC = NRC + 1
      Filename$ = Dir$()
    Loop
  End If

Case 5  'Save to MISSIONbatch.NUM
  lu = FreeFile
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  Filename$ = SUpath$ + Mission$ + "_NUM_batch.txt"
  Open Filename$ For Output As lu
  ' Write pairs of numbers, FltNumber and FltDate
  For i = 0 To lstFltDates.ListCount - 1
    Print #lu, i + 1, lstFltDates.List(i)
  Next i
  Close lu

End Select
Exit Sub
'
ErrorHandler1:
  MsgBox "Unable to open: " + Filename$, vbOKOnly
  Exit Sub
End Sub


Private Sub cmdTest_Click()
Dim ngvlu%, i%, a$, OUTlu%, FD$, color&, red&, green&

red = &HFF&   'red
green = &H80FF80  'green
txtFltDate.BackColor = red
FD$ = Trim(txtFltDate.Text)
ngvlu = FreeFile
Open "c:\mtp\data\ngv\trex\ng\ND" + FD$ + ".txt" For Input As ngvlu
Line Input #ngvlu, a$

OUTlu = FreeFile
Open "C:\MTP\Data\NGV\TREX\NG\FT" + FD$ + ".txt" For Output As OUTlu

Do
  Call ReadNGV(ngvlu)
  'Debug.Print nUTsec, nLatitude, nLongitude, nTr, nTa
  Print #OUTlu, nUTsec; nLatitude; nLongitude; nZp; nTr; nTa
Loop Until EOF(ngvlu)
Close ngvlu, OUTlu

txtFltDate.BackColor = green


End Sub


