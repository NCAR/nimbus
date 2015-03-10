VERSION 5.00
Begin VB.Form frmBatch 
   Caption         =   "Batch Processor"
   ClientHeight    =   3780
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6915
   Icon            =   "frmBatch.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3780
   ScaleWidth      =   6915
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame 
      Caption         =   "Change Retrieval Parameters"
      Height          =   3075
      Index           =   9
      Left            =   0
      TabIndex        =   18
      Top             =   30
      Width           =   2385
      Begin VB.TextBox txtNewParameter 
         Height          =   285
         Index           =   2
         Left            =   1680
         TabIndex        =   24
         Text            =   "TRUE"
         Top             =   930
         Width           =   615
      End
      Begin VB.CheckBox chkChangeParameter 
         Caption         =   "Enable WCT"
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   23
         Top             =   960
         Width           =   1485
      End
      Begin VB.TextBox txtNewParameter 
         Height          =   285
         Index           =   1
         Left            =   1680
         TabIndex        =   22
         Text            =   "TRUE"
         Top             =   600
         Width           =   615
      End
      Begin VB.CheckBox chkChangeParameter 
         Caption         =   "Read New WCT"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   21
         Top             =   630
         Width           =   1515
      End
      Begin VB.TextBox txtNewParameter 
         Alignment       =   1  'Right Justify
         Height          =   285
         Index           =   0
         Left            =   1680
         MultiLine       =   -1  'True
         TabIndex        =   20
         Text            =   "frmBatch.frx":0442
         Top             =   270
         Width           =   615
      End
      Begin VB.CheckBox chkChangeParameter 
         Caption         =   "OATnavCOR"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   19
         Top             =   300
         Width           =   1335
      End
   End
   Begin VB.Frame Frame5 
      Caption         =   "Process Selection"
      Height          =   1875
      Left            =   4770
      TabIndex        =   13
      Top             =   60
      Width           =   1995
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Export MP-File"
         Height          =   255
         Index           =   3
         Left            =   120
         TabIndex        =   17
         Top             =   720
         Width           =   1455
      End
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Plot CTC"
         Height          =   255
         Index           =   4
         Left            =   120
         TabIndex        =   16
         Top             =   960
         Width           =   1335
      End
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Editting"
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   15
         Top             =   480
         Width           =   1335
      End
      Begin VB.CheckBox chkProcesses 
         Caption         =   "Retrieval"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   14
         Top             =   240
         Width           =   1335
      End
   End
   Begin VB.CommandButton cmdBatchProcessor 
      Caption         =   "Start Batch Process"
      Height          =   465
      Index           =   0
      Left            =   4800
      TabIndex        =   12
      Top             =   2700
      Width           =   1995
   End
   Begin VB.CommandButton cmdBatchProcessor 
      Caption         =   "E&xit"
      Height          =   465
      Index           =   2
      Left            =   4800
      TabIndex        =   11
      Top             =   3240
      Width           =   1995
   End
   Begin VB.Timer tmrBatch1 
      Left            =   570
      Top             =   3270
   End
   Begin VB.Timer tmrBatch0 
      Left            =   60
      Top             =   3240
   End
   Begin VB.CheckBox chkProcesses 
      Caption         =   "Open Raw"
      Height          =   255
      Index           =   0
      Left            =   4740
      TabIndex        =   10
      Top             =   2340
      Visible         =   0   'False
      Width           =   1335
   End
   Begin VB.TextBox txtCurrentProcess 
      Height          =   285
      Left            =   4740
      TabIndex        =   9
      Top             =   1980
      Width           =   2025
   End
   Begin VB.Frame Frame 
      Caption         =   "Flight Dates to Process"
      Height          =   3645
      Index           =   5
      Left            =   2520
      TabIndex        =   0
      Top             =   60
      Width           =   2175
      Begin VB.TextBox txtFltDates 
         Height          =   285
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Width           =   885
      End
      Begin VB.ListBox lstFltDates 
         Height          =   2985
         ItemData        =   "frmBatch.frx":0446
         Left            =   120
         List            =   "frmBatch.frx":044D
         TabIndex        =   7
         ToolTipText     =   "Flight Dates to be Batch Processed"
         Top             =   570
         Width           =   885
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Add"
         Height          =   315
         Index           =   0
         Left            =   1080
         TabIndex        =   6
         ToolTipText     =   "Add this Flight Date to List"
         Top             =   240
         Width           =   975
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Remove"
         Height          =   315
         Index           =   1
         Left            =   1080
         TabIndex        =   5
         ToolTipText     =   "Remove Selected Flighht Date from List"
         Top             =   600
         Width           =   975
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Clear"
         Height          =   315
         Index           =   2
         Left            =   1080
         TabIndex        =   4
         ToolTipText     =   "Clear Flight Date List"
         Top             =   960
         Width           =   975
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Default All"
         Height          =   315
         Index           =   3
         Left            =   1080
         TabIndex        =   3
         ToolTipText     =   "Load List with FLTinfo Flight Dates"
         Top             =   1320
         Width           =   975
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Default"
         Height          =   315
         Index           =   4
         Left            =   1080
         TabIndex        =   2
         ToolTipText     =   "Load List with MISSIONbatch.NUM"
         Top             =   1680
         Width           =   975
      End
      Begin VB.CommandButton cmdFltDates 
         Caption         =   "&Save"
         Height          =   315
         Index           =   5
         Left            =   1080
         TabIndex        =   1
         ToolTipText     =   "Save Flight Date List to MISSIONbatch.NUM"
         Top             =   2040
         Width           =   975
      End
   End
End
Attribute VB_Name = "frmBatch"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub cmdBatchProcessor_Click(Index As Integer)
Dim FltDate$, Pathprefix$, FQFN$, INPlu%, OUTlu%, i%, j%, k%, LI%, Rmax!, nSites%
  
  
  Select Case Index
  Case 0
    BatchStart = True
    tmrBatch0.Enabled = True
    
  Case 1
    
  Case 2                 'Exit
    Unload Me
  
  Case Else
  
End Select

End Sub


Private Sub cmdFltDates_Click(Index As Integer)
Dim lu%, Path$, Filename$, FltNumber&, FltDate&, i%, A$

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

Case 4  'Default MTPman.NUM
  lstFltDates.Clear
  lu = FreeFile
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  Filename$ = Path$ + Mission$ + "batch.NUM"
  On Error GoTo ErrorHandler1
  Open Filename$ For Input As lu
  ' Read pairs of numbers, FltNumber and FltDate
  Do
    Input #lu, FltNumber, FltDate
    lstFltDates.AddItem Str(FltDate)
  Loop Until EOF(lu)
  Close lu

Case 5  'Save to MISSIONbatch.NUM
  lu = FreeFile
  Path$ = Drive$ + Rdir2$ + Mission$ + "\"
  Filename$ = Path$ + Mission$ + "batch.NUM"
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

Private Sub Form_Load()

With tmrBatch0
  .Interval = 500
  .Enabled = False
End With
With tmrBatch1
  .Interval = 500
  .Enabled = False
End With

End Sub



Private Sub tmrBatch0_Timer()
Dim i%, j%, FltDate$, LI%
Static CurrentListIndex%, Pathprefix$
  
  tmrBatch0.Enabled = False
  If BatchStart Then
    BatchStart = False
    DoAll = True
    tmrBatch0.Interval = 500
    CurrentListIndex = 0
    ProcessCount = 0
'    Pathprefix$ = DataDrive$ + "\" + frmFLTINFO.cboPlatforms.Text + "\" + frmFLTINFO.cboMissions.Text + "\"
    Pathprefix$ = MNpath$
    For i = 1 To 4
      If chkProcesses(i).Value = 1 Then
        If i = 1 Then                             'Do Retrieval
          ProcessCount = 2
          ProcessList(0) = 0
          If chkChangeParameter(0).Value = 1 Then  'Change OATnavCOR
            ProcessList(1) = 10  'Do TB fit
            ProcessList(2) = 1
            ProcessCount = 3
          Else
            ProcessList(1) = 1
          End If
        Else
          ProcessCount = ProcessCount + 1
          ProcessList(ProcessCount - 1) = i
        End If
      End If
    Next i
    CurrentProcess = -1
    REFwriteHEADER "C:\MTP\Setup\FLTINFO.REF"
  
  Else
    CurrentListIndex = CurrentListIndex + 1
    If CurrentListIndex > lstFltDates.ListCount - 1 Then
      DoAll = False               'Clear flag to allow automatic startup
      BatchStart = False
      ProcessCount = 0
      Exit Sub
    End If
  End If

' Batch Process assumes that FLTINFO YYYYMMDD files already exist
    i = CurrentListIndex
    FltDate$ = Trim(lstFltDates.List(i))
    lstFltDates.ListIndex = i  'Select current flight date
    txtFltDates.Text = FltDate$
    j = 0
    Do
      If Trim(Str$(frmFLTINFO.cboFltDates.List(j))) = FltDate$ Then LI = j: Exit Do
      j = j + 1
    Loop
    
    With frmFLTINFO
      .cboFltDates.ListIndex = LI
      .cboFltNos.ListIndex = LI
      .cboObjectives.ListIndex = LI
    End With
    
    BatchFQFN$ = Pathprefix$ + FltDate$ + "\" + Left$(frmFLTINFO.cboPlatforms.Text, 2) + FltDate$ + ".REF"
    With frmFLTINFO
      .txtSourceFile.Text = BatchFQFN$
      .cmdFlight_Click (5)          'Sets DOALL false
      .cmdWriteDefault_Click
    End With
    DoAll = True
    tmrBatch1.Enabled = True

End Sub


Private Sub tmrBatch1_Timer()
Dim k%
Static ProcessNumber%
      
  tmrBatch1.Enabled = False
  CurrentProcess = CurrentProcess + 1
  If CurrentProcess > ProcessCount - 1 Then
    CurrentProcess = -1
    tmrBatch0.Enabled = True
    Exit Sub
  End If
  
  Select Case ProcessList(CurrentProcess)
  Case 0  'Open Raw
    For k = 0 To 2
      If chkChangeParameter(k).Value = 1 Then
        Select Case k
        Case 0 'OATnavCOR
          OATnavCOR = Val(txtNewParameter(k).Text)
          frmFLTINFO.txtOATnavCOR.Text = txtNewParameter(k)
        Case 1 'Read New WCT
          Call frmFLTINFO.cmdReadNewCAL_Click(1)
        Case 2 'Enable WCT
          If UCase(txtNewParameter(k).Text) = "TRUE" Then
            frmFLTINFO.chkEnableWCT.Value = 1
          Else
            frmFLTINFO.chkEnableWCT.Value = 0
          End If
        End Select
      End If
    Next k
    frmFLTINFO.cmdWritePath_Click
    DoEvents
    txtCurrentProcess.Text = "Reading RAW file"
    frmMTPbin.cmdREF_Click (0)  'Open RAW File
  
  Case 1    'Do Retrieval
    txtCurrentProcess.Text = "Performing Retrieval"
    DoEvents
    frmMTPbin.cmdREF_Click (11) 'And perform retrieval

  Case 2     'Edit
    txtCurrentProcess.Text = "Editting REF File"
    DoEvents
    frmMTPbin.cmdREF_Click (7)
    
  Case 3     'Write MP File
    txtCurrentProcess.Text = "Writing MP File"
    DoEvents
    frmMTPbin.cmdREF_Click (9)
  
   Case 4     'CTC Pot
    txtCurrentProcess.Text = "Plotting Temperature Curtain"
    DoEvents
    frmMTPbin.cmdREF_Click (0)
    
  Case 10
    txtCurrentProcess.Text = "Fitting TBs"
    DoEvents
    frmMTPbin.cmdTB_Click (0)           'Adjust gain to reflect new OATnavCOR

  Case Else
  
  End Select

End Sub


Private Sub txtRmax_Change()

End Sub


