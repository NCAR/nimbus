VERSION 5.00
Begin VB.Form frmHTMLwriter 
   Caption         =   "HTML Writer"
   ClientHeight    =   1050
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   2190
   Icon            =   "frmHTMLwriter.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   1050
   ScaleWidth      =   2190
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtTotal 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   1335
      MultiLine       =   -1  'True
      TabIndex        =   4
      Top             =   45
      Width           =   435
   End
   Begin VB.TextBox txtCurrent 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   615
      MultiLine       =   -1  'True
      TabIndex        =   2
      Top             =   45
      Width           =   405
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "Start"
      Height          =   465
      Left            =   1305
      TabIndex        =   0
      Top             =   570
      Width           =   870
   End
   Begin VB.Label Label 
      Caption         =   "of"
      Height          =   180
      Index           =   1
      Left            =   1095
      TabIndex        =   3
      Top             =   90
      Width           =   180
   End
   Begin VB.Label Label 
      Caption         =   "Module"
      Height          =   255
      Index           =   0
      Left            =   45
      TabIndex        =   1
      Top             =   60
      Width           =   600
   End
End
Attribute VB_Name = "frmHTMLwriter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub cmdStart_Click()
Dim i%, X!, INPlu%, OUTlu%, Filenames$(30), FileCount%, Path$, a$, b$, c$
Dim FilenameOut$, PathOut$, j%, id$, idCount%, k1%, k2%
  
  idCount% = 0
  id$ = "xc"
  Path$ = "C:\MTP\VB6\BAS\"
  PathOut$ = "C:\mtp\www\notes\software\modules\"
  Filenames$(1) = Dir(Path$ + "*.bas", vbNormal)
  FileCount = 1
' Before converting, add ! to file name to save original
  Do While Filenames$(FileCount) <> ""
    FileCount = FileCount + 1
    Filenames$(FileCount) = Dir
  Loop
  FileCount = FileCount - 1
  txtTotal.Text = FileCount
    
' Open php output file
' Note that php file instead of html file is written so that password protection
' can be implemented in a php script.
' See: http://www.zubrag.com/scripts/password-protect.php
' <?php include("/afs/jpl.nasa.gov/group/mtp/www/notes/software/password_protect.php"); ?>
' The above line must be included in the header to cause the hypertext preprocessor to run
'
  OUTlu = FreeFile
  FilenameOut$ = "C:\mtp\www\notes\software\MTP_Module_Documentation.php"
  Open FilenameOut$ For Output As OUTlu
' Open module_header.txt file
  INPlu = FreeFile
  Open "C:\mtp\www\notes\software\modules\module_header.txt" For Input As INPlu
  Do
    Line Input #INPlu, a$
    If Left$(a$, 15) = "Last Revision: " Then
      a$ = "Last Revision: " + fTimestamp2(False)
    End If
    Print #OUTlu, a$
  Loop Until EOF(INPlu)
  Close INPlu
  
  For i = 1 To FileCount
    txtCurrent.Text = Str(i)
    DoEvents
'01  <li>Calibration.bas
'02    <ul id="Calibration" title="Calibration">
    Print #OUTlu, "<li>" + Filenames$(i)
    b$ = Left$(Filenames$(i), Len(Filenames$(i)) - 4)
    Print #OUTlu, "<ul id=""" & b$ & """ " & "title=""" & b$ & """>"
    
    INPlu = FreeFile
    Open Path$ + Filenames$(i) For Input As INPlu
    Do
'03      <li><a>Function 1</a>
'04        <ul id="xc" title="Calibration">
'05          <li>Line 1</li>
'06          <li>Line 2</li>      Line Input #INPlu, a$
      Line Input #INPlu, a$
      If Left$(a$, 4) = "Sub " Or Left$(a$, 9) = "Function " Then
'      If InStr(a$, " Sub ") Or InStr(a$, " Function ") Then
        k1 = InStr(a$, "Sub ")
        k2 = InStr(a$, "Function ")
      '  If k1 > 0 And k2 > 0 And k1 < 25 And k2 < 25 Then
        j = InStr(a$, "(")
        Print #OUTlu, "<li><a>" & Left(a$, j - 1) & "</a>"
        If idCount = 0 Then
          id$ = "xc"
          idCount = 1
        Else
          id$ = "xc" + Format(idCount, "0000")
          idCount = idCount + 1
        End If
        Print #OUTlu, "<ul id=""" & id$ & """ " & "title=""" & b$ & """>"
'        Print #OUTlu, "<li>" & a$ & "</li>"
        Print #OUTlu, "<pre>"
        Print #OUTlu, a$
        Do
          Line Input #INPlu, a$
'          Print #OUTlu, "<li>" & a$ & "</li>"
          Print #OUTlu, a$
'        Loop Until Left$(a$, 12) = "End Function" Or Left$(a$, 7) = "End Sub"
        Loop Until InStr(a$, "End Function") Or InStr(a$, "End Sub")
        Print #OUTlu, "</pre>"
'08        </ul>
'09      </li>
      Print #OUTlu, "</ul>"
      Print #OUTlu, "</li>"
      End If
    Loop Until EOF(INPlu)
    Close INPlu
    Print #OUTlu, "</ul>"
    Print #OUTlu, "</li>"
    
  Next i
  
  Print #OUTlu, "</ul>"
  Print #OUTlu, "<hr style=""width: 100%; height: 2px;"">| "
  Print #OUTlu, "<a href=""../../index.html"">MTP Home Page</a> |<br>"
  Print #OUTlu, "</body>"
  Print #OUTlu, "</html>"
  Close OUTlu

'01  <li>Calibration.bas
'02    <ul id="Calibration" title="Calibration">
'03      <li><a>Function 1</a>
'04        <ul id="xc" title="Calibration">
'05          <li>Line 1</li>
'06          <li>Line 2</li>
'07          <li>Line 3</li>
'08        </ul>
'09      </li>
'10      <li><a>Function 2</a></li>
'11        <ul id="xc" title="Calibration">
'12          <li>Line 1</li>
'13          <li>Line 2</li>
'14          <li>Line 3</li>
'15        </ul>
'16    </ul>
'17  </li>
  Unload Me
  End
End Sub
Function fTimestamp2(Abbreviate As Boolean) As String
Dim i%, j%, a$, m%, d$, b$
' Write timestamp as either:
' January 1, 2008 12:12:12 AM or
' Jan 1, 2008 12:12:12 AM depending on whether Abbreviate is FALSE or TRUE
  d$ = Date
  i = InStr(d$, "/")
  j = InStr(i, d$, "/")
  a$ = MonthName(Val(Left$(d$, i - 1)), Abbreviate)
  b$ = Mid$(d$, i + 1, j)
  a$ = a$ + " " + b$ + ", " + Right$(d$, 4) + " "
  b$ = Time
  fTimestamp2 = a$ + b$
End Function
Function fReplaceExtension(ByVal File$, ByVal Ext$) As String
' Replace extension on File$ with Ext$, do not include "."
Dim i%, j%

  fReplaceExtension = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "." Then
      fReplaceExtension = Left$(File$, i) + Ext$
      Exit Function
    End If
  Loop Until i = 1

End Function

