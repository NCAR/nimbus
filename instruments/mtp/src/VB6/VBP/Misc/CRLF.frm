VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form CRLF 
   Caption         =   "Convert Line Terminating Characters"
   ClientHeight    =   2025
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   4680
   Icon            =   "CRLF.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2025
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkReplaceInFile 
      Caption         =   "Replace Input File"
      Height          =   255
      Left            =   2880
      TabIndex        =   13
      Top             =   960
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton cmdDoDir 
      Caption         =   "Do Dir"
      Height          =   375
      Left            =   3240
      TabIndex        =   12
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtFileMask 
      Height          =   285
      Left            =   840
      TabIndex        =   11
      Text            =   "*.asc"
      Top             =   480
      Width           =   3735
   End
   Begin VB.TextBox txtOUText 
      Height          =   285
      Left            =   2640
      TabIndex        =   9
      Text            =   "CRLF"
      Top             =   1560
      Width           =   495
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   3960
      TabIndex        =   7
      Top             =   1560
      Width           =   615
   End
   Begin VB.Frame Frame1 
      Caption         =   "Convert"
      Height          =   615
      Left            =   120
      TabIndex        =   4
      Top             =   840
      Width           =   2655
      Begin VB.OptionButton optConvert 
         Caption         =   "CRLF to LF"
         Height          =   255
         Index           =   1
         Left            =   1320
         TabIndex        =   6
         Top             =   240
         Width           =   1215
      End
      Begin VB.OptionButton optConvert 
         Caption         =   "LF to CRLF"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.TextBox txtProgress 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   840
      MultiLine       =   -1  'True
      TabIndex        =   3
      Top             =   1560
      Width           =   495
   End
   Begin VB.TextBox txtPath 
      Height          =   285
      Left            =   840
      TabIndex        =   1
      Text            =   "C:\MTP\Data\NGV\START08\NG\"
      Top             =   75
      Width           =   3735
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   3360
      Top             =   1560
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Label Label10 
      Caption         =   "File Mask"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   480
      Width           =   735
   End
   Begin VB.Label Label3 
      Caption         =   "OUT extension"
      Height          =   255
      Left            =   1440
      TabIndex        =   8
      Top             =   1560
      Width           =   1095
   End
   Begin VB.Label Label2 
      Caption         =   "Progress"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   1560
      Width           =   615
   End
   Begin VB.Label Label1 
      Caption         =   "Path"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   495
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
End
Attribute VB_Name = "CRLF"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim ConversionType%

Private Sub Command1_Click()
Unload Me
End Sub

Sub FixLineTerminator(INPfile$, OUTfile$, ConversionType%, ShowProgress As Boolean)
' Routine to fix line termination character string
' ConversionType = 0 for LF to CRLF
'                  1 for CRLF to LF
' ShowProgress shows % of way through input file if a text box called
' txtProgress.text has been defined on form

Dim INPlu%, OUTlu%, out$, ByteCount&, c$, Bytes&, last$

  INPlu% = FreeFile
  Open INPfile$ For Input As INPlu%
  ByteCount = LOF(INPlu)
  OUTlu = FreeFile
  Open OUTfile$ For Output As OUTlu
  out$ = ""
Do
  c$ = Input(1, INPlu)
  Bytes = Bytes + 1
  Select Case ConversionType
  Case 0
    If c$ = vbLf Then
      Print #OUTlu, out$                  'Print adds CRLF!!
      out$ = ""
    Else
      If c$ <> vbCr Then out$ = out$ + c$ 'avoid double Cr
    End If
  Case 1
    If c$ = vbCr Then
      c$ = Input(1, INPlu)
      Bytes = Bytes + 1
      If c$ = vbLf Then
        out$ = out$ + vbLf
        Put #OUTlu, , out$
        out$ = ""
      Else
        out$ = out$ + vbCr
      End If
    Else
      out$ = out$ + c$
    End If
  
  End Select
  If ShowProgress And Bytes Mod 100 = 0 Then
    txtProgress.Text = Str$(Int(Bytes * 100# / ByteCount + 0.5))
    DoEvents
  End If
Loop Until EOF(INPlu)

Close INPlu, OUTlu

End Sub


Private Sub cmdDoDir_Click()
Dim INPlu%, INPfile$, Path$, OUTlu%, Line$, FirstPass As Boolean
Dim X$, y$, Answer As Variant
Dim Filenames$(1 To 500), FileCount%, Ext$

Ext$ = "Old"
' Combine all Makky*.CONV files in txtPath.txt into a single file (Makky.txt)
Path$ = txtPath.Text
If Right$(Path$, 1) <> "\" Then
  txtPath.Text = Path$ + "\"
  Path$ = txtPath.Text
End If
Filenames$(1) = Dir(Path$ + txtFileMask.Text, vbNormal)
FileCount = 1
' Before converting, add ! to file name to save original
Do While Filenames$(FileCount) <> ""
  FileCount = FileCount + 1
  Filenames$(FileCount) = Dir
Loop

For i = 1 To FileCount - 1
  Name Path$ + Filenames$(i) As Path$ + Ext$ + Filenames$(i)
  Call FixLineTerminator(Path$ + Ext$ + Filenames$(i), Path$ + Filenames$(i), ConversionType, True)
Next i

Answer = MsgBox("Do you wish to delete original files" + vbCrLf + vbLf + "Which are Prefixed 'Old' ?", vbYesNo + vbDefaultButton1, "Warning!")

If Answer = vbYes Then Kill Path$ + Ext$ + txtFileMask.Text

End Sub

Private Sub cmdExit_Click()
Unload Me
End Sub

Private Sub cmdJulie_Click()
Dim INPlu%, INPfile$, Path$, OUTlu%, Line$, FirstPass As Boolean
Dim X$, y$, Answer As Variant
Dim Filenames$(1 To 500), FileCount%, Ext$

' Combine all Makky*.CONV files in txtPath.txt into a single file (Makky.txt)
Path$ = "c:\MTP\Data\NGV\START08\RAOB\RC\"
Filenames$(1) = Dir(Path$ + "NRCE     *.*", vbNormal)
FileCount = 1
' Before converting, add ! to file name to save original
Do While Filenames$(FileCount) <> ""
  FileCount = FileCount + 1
  Filenames$(FileCount) = Dir
Loop
'NRCE     FA056.0100
'12345678901234567890
For i = 1 To FileCount - 1
  Ext$ = "NRC" + Right$(Filenames$(i), 10)
  Name Path$ + Filenames$(i) As Path$ + Ext$
Next i


End Sub

Private Sub Form_Load()
optConvert(0).Value = True
End Sub


Private Sub mnuFileExit_Click()
cmdExit_Click
End Sub

Private Sub mnuFileOpen_Click()
Dim INPfilename$, OUTfilename$, i%, OUTlu%, INPlu%
Dim ByteCount&, c$, Bytes&, out$, Path$

Path$ = txtPath.Text
If Right$(Path$, 1) <> "\" Then
  txtPath.Text = Path$ + "\"
  Path$ = txtPath.Text
End If

' Set Initial directory
CommonDialog1.InitDir = Path$
' Set filters.
CommonDialog1.Filter = "All Files (*.*)|*.*|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

INPfilename$ = CommonDialog1.FileName

If INPfilename$ = "" Then Exit Sub

  m = Len(INPfilename$)
  For i = m To 1 Step -1
    If Mid$(INPfilename$, i, 1) = "." Then Exit For
  Next i
  ' Make sure input file had an extension
  If i = 0 Then
    OUTfilename$ = INPfilename$ + "." + txtOUText.Text
  Else
    OUTfilename$ = Left$(INPfilename$, i) + txtOUText.Text
  End If
  
  Call FixLineTerminator(INPfilename$, OUTfilename$, ConversionType, True)

  If chkReplaceInFile.Value = 1 Then
    Kill INPfilename$
    Name OUTfilename$ As INPfilename$
  End If
End Sub


Private Sub optConvert_Click(Index As Integer)
  ConversionType = Index
End Sub


