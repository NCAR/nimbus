VERSION 5.00
Begin VB.Form MTPsetup 
   Caption         =   "Form1"
   ClientHeight    =   2130
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3405
   LinkTopic       =   "Form1"
   ScaleHeight     =   2130
   ScaleWidth      =   3405
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame 
      Caption         =   "MTPsetup.INI"
      Height          =   2085
      Index           =   13
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3375
      Begin VB.ComboBox cboRTdefault 
         Height          =   315
         Left            =   1170
         TabIndex        =   6
         Text            =   "FALSE"
         Top             =   1320
         Width           =   975
      End
      Begin VB.DriveListBox drvProgram 
         Height          =   315
         Left            =   1170
         TabIndex        =   5
         Top             =   270
         Width           =   2115
      End
      Begin VB.DriveListBox drvData 
         Height          =   315
         Left            =   1170
         TabIndex        =   4
         Top             =   630
         Width           =   2115
      End
      Begin VB.TextBox txtUID 
         Height          =   285
         Left            =   1170
         TabIndex        =   3
         Text            =   "mjm"
         Top             =   990
         Width           =   735
      End
      Begin VB.CommandButton cmdSetup 
         Caption         =   "&Update"
         Height          =   375
         Left            =   2520
         TabIndex        =   2
         Top             =   1650
         Width           =   765
      End
      Begin VB.TextBox txtAlternateRoot 
         Height          =   285
         Left            =   1170
         TabIndex        =   1
         Top             =   1680
         Width           =   1335
      End
      Begin VB.Label Label 
         Caption         =   "Realtime (def)"
         Height          =   255
         Index           =   159
         Left            =   90
         TabIndex        =   11
         Top             =   1350
         Width           =   1035
      End
      Begin VB.Label Label 
         Caption         =   "User Initials"
         Height          =   255
         Index           =   133
         Left            =   120
         TabIndex        =   10
         Top             =   1050
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Data Drive"
         Height          =   255
         Index           =   132
         Left            =   90
         TabIndex        =   9
         Top             =   690
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Program Drive"
         Height          =   255
         Index           =   131
         Left            =   90
         TabIndex        =   8
         Top             =   330
         Width           =   1095
      End
      Begin VB.Label Label 
         Caption         =   "Alternate Root"
         Height          =   255
         Index           =   161
         Left            =   120
         TabIndex        =   7
         Top             =   1710
         Width           =   1035
      End
   End
End
Attribute VB_Name = "MTPsetup"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Sub ReadSETUP(Program$)
  Dim i0%, i1%, i2%, lu%, Cmd$, V$, FixPath As Boolean

  If Len(Dir("C:\MTP\Setup\MTPsetup.INI")) = 0 Then
    MsgBox "Unable to open C:\MTP\Setup\MTPsetup.INI", vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Input As lu%
  FIsize% = 0
  FixPath = False
Do
NextCategory:
  If EOF(lu%) Then GoTo Exit_Sub

  FIsize% = FIsize% + 1
  Input #lu%, Cmd$                       'Read a line
  
  If Left$(Cmd$, 1) = "[" Then           'Ignore everything until category found
    i0% = InStr(2, Cmd$, "]")
    If i0% = 0 Then
      MsgBox "Right Bracket not found. Fix line number " + Str(FIsize%), vbOKOnly
      Exit Sub
    End If
    If Mid$(Cmd$, 2, i0% - 2) = Program$ Then    'Read setup info for only the requested program
      Select Case Mid$(Cmd$, 2, i0% - 2)
      Case "SYSTEM"
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          Input #lu%, Cmd$                       'Read a line
          'PRINT cmd$
          If Len(Cmd$) = 0 Then GoTo NextCategory

          i1% = InStr(1, Cmd$, "=")
          If i1% > 0 Then                     'Look for a command line
            i2% = InStr(i1%, Cmd$, " ")   'and end of its value
            If i2% = 0 Then i2% = Len(Cmd$) + 1
            V$ = Mid$(Cmd$, i1% + 1, i2% - i1% - 1)
           'Debug.Print cmd$
            Select Case Left$(Cmd$, i1% - 1)
            Case "ProgramDrive$":  programdrive$ = V$
            Case "DataDrive$":  DataDrive$ = V$
            Case "UID$": UID$ = V$
            Case "DefaultRTmode"
              DefaultRTmode = Val(V$)
              If DefaultRTmode = 0 Then RealTime = False Else RealTime = True
            Case "AlternateRoot"
              If V$ = """""" Then Rdir1$ = "" Else Rdir1$ = Val(V$)
            Case Else
            End Select
          End If
        Loop
      Case Else
      End Select
    End If
  End If
Private Sub cmdSetup_Click()
  
  programdrive$ = Left$(drvProgram.Drive, 2)
  DataDrive$ = Left$(drvData.Drive, 2)
  UID$ = txtUID.Text
  DefaultRTmode = Trim(Str(cboRTdefault.ListIndex))
  Rdir1$ = Trim(txtAlternateRoot.Text)
  
  WriteSETUP ("SYSTEM")

End Sub

Sub WriteSETUP(Program$)
Dim i0%, i1%, i2%, INPlu%, OUTlu%, Cmd$, X$

  If Len(Dir("C:\MTP\Setup\MTPsetup.SAV")) > 0 Then Kill "C:\MTP\Setup\MTPsetup.SAV"
  Name "C:\MTP\Setup\MTPsetup.INI" As "C:\MTP\Setup\MTPsetup.SAV"

  OUTlu% = FreeFile
  Open "C:\MTP\Setup\MTPsetup.INI" For Output As OUTlu%

' Write header info
  Call PrintStr(OUTlu%, "' C:\MTP\Setup\MTPsetup.INI" + " was last written on " + Date$ + " at " + Time$, "", "")
  Call PrintStr(OUTlu%, "' This INI-file follows a few simple but mandatory format requirements.", "", "")
  Call PrintStr(OUTlu%, "' Settings are entered into categories which occupy a single line and", "", "")
  Call PrintStr(OUTlu%, "' must be bracketed by square brackets (e.g. [PATH]). Variables for which", "", "")
  Call PrintStr(OUTlu%, "' settings are assigned must have exactly the same name in the program in", "", "")
  Call PrintStr(OUTlu%, "' which they are used and be followed by an equal sign (=). Spaces are", "", "")
  Call PrintStr(OUTlu%, "' not allowed. Anything beyond the first space in a line is ignored.", "", "")
  Call PrintStr(OUTlu%, "' Blank lines are also ignored, but are required betweeen categories.", "", "")
  Call PrintStr(OUTlu%, "", "", "")

' Now read old file until requested Program is found
  Call PrintStr(OUTlu%, "[SYSTEM]", "", "")
  X$ = Left$(drvProgram, 2)
  Call PrintStr(OUTlu%, "ProgramDrive$", X$, "Default Program hard drive")
  X$ = Left$(drvData, 2)
  Call PrintStr(OUTlu%, "DataDrive$", X$, "Default Data hard drive")
  X$ = txtUID.Text
  Call PrintStr(OUTlu%, "UID$", X$, "Default user ID (initials)")
  X$ = cboRTdefault.Text
  Call PrintStr(OUTlu%, "DefaultRTmode", Trim(X$), "Default Real Time Mode (0=FALSE, 1=TRUE)")
  X$ = txtAlternateRoot.Text
  If Len(X$) = 0 Then X$ = """"""
  Call PrintStr(OUTlu%, "AlternateRoot", X$, "")
  Call PrintStr(OUTlu%, "", "", "")
    
  Print #OUTlu, "[EOF]"
  Close OUTlu%

End Sub
Sub PrintStr(lu%, x1$, X$, x2$)
' Print to lu%, x1$=x$         'x2$
' eg            PgmDrive$=c:   'Program Drive
Dim fmt$, L%

  If X$ = "" Then
    If lu% = 0 Then Debug.Print x1$ Else Print #lu%, x1$
  Else
    fmt$ = x1$ + "=" + X$
    L% = 30 - Len(fmt$)
    If L% < 0 Then L% = 0
    fmt$ = fmt$ + Space$(L%) + "'" + x2$
    If lu% = 0 Then Debug.Print fmt$ Else Print #lu%, fmt$
  End If

End Sub

