VERSION 5.00
Begin VB.Form frmEfold 
   Caption         =   "Form1"
   ClientHeight    =   1830
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3900
   LinkTopic       =   "Form1"
   ScaleHeight     =   1830
   ScaleWidth      =   3900
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   2790
      TabIndex        =   0
      Top             =   1320
      Width           =   1065
   End
End
Attribute VB_Name = "frmEfold"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Command1_Click()
Dim lu%, i%, j%, x!(0 To 200), xTime!, xSum!, a$, OUTlu%
Dim T!(0 To 37000), P!(0 To 37000), iMax%, Tavg!, Tmtp!

  lu = FreeFile  '1 second data
  Open "C:\DC8\TexAQS\TPnav0828.txt" For Input As lu
  OUTlu = FreeFile
  Open "C:\DC8\TexAQS\TPnavOUT30.txt" For Output As OUTlu
  For i = 0 To 3600
    Input #lu, s!, T!(i), P!(i)
'
  Next i
  eTime = 30#  'E-folding time
  i = -1:  xSum = 0#
  Do
    i = i + 1
    x(i) = Exp(-i / eTime)
    xSum = xSum + x(i)
  Loop Until x(i) < 0.01
  iMax = i
  
  For i = 0 To iMax: x(i) = x(i) / xSum: Next i
  
  j = 0
  Do
    Tavg = 0#
    For i = 0 To iMax
      Tavg = Tavg + T(i + j) * x(i)
      If i = 15 Then Tmtp = T(i + j)
    Next i
    If iMax < 15 Then Tmtp = T(15 + j)
    'Debug.Print Format(j / 1000, "0.000"); vbTab; Format(Tavg, "000.00"); vbTab; Format(Tmtp, "000.0"); vbTab; Format(Tmtp - Tavg, "#0.00")
    Print #OUTlu, Format(j / 1000, "0.000"); vbTab; Format(P(j), "0.000"); vbTab; Format(Tmtp - Tavg, "#0.00")
    j = j + 15
  Loop Until j + iMax > 3600
  Debug.Print eTime; iMax
  Close lu, OUTlu
End Sub

Sub AvgRMS(a!(), N%, Avg!, RMS!)
Dim Sum1!, Sum2!, x!, i%
  
' Scan to Scan RMS
  For i = 1 To N
    x = a(i)
    Sum1 = Sum1 + x   'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  Avg = Sum1 / N     'Calculate average
  If Sum2 - Avg ^ 2 / N > 0 Then RMS = Sqr((Sum2 - Avg ^ 2 * N) / (N - 1)) 'Calculate RMS
  
End Sub

Private Sub Form_Load()

End Sub


