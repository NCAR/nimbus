VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4980
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6975
   LinkTopic       =   "Form1"
   ScaleHeight     =   4980
   ScaleWidth      =   6975
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Start 
      Caption         =   "Command1"
      Height          =   885
      Left            =   5550
      TabIndex        =   0
      Top             =   3390
      Width           =   1305
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Start_Click()
Dim lu%, P$, OUTlu%, i&
  
  lu = FreeFile
  Open "C:\MTP\Data\M55\Scout-O3\ucse0035.dat" For Binary As lu
  OUTlu = FreeFile
  Open "C:\MTP\Data\M55\Scout-o3\ucse0035.txt" For Output As OUTlu
  i = 0
  Do
    i = i + 1
    P$ = P$ + Input(1, lu)
    'Debug.Print lhex(P$)
  Loop Until EOF(lu)
  Print #OUTlu, lhex$(P$)
  Print #OUTlu, P$
  Close lu, OUTlu
  Stop
End Sub

Function lhex$(lh$)

lx$ = ""
For i = 1 To Len(lh$)
   lx$ = lx$ + hx$(Mid$(lh$, i, 1))
Next i
lhex$ = lx$
End Function
Function hx$(x$)
hxt$ = ""
For hxi = 1 To Len(x$)
   hxt$ = hxt$ + Right$("00" + Hex$(Asc(Mid$(x$, hxi, 1))), 2)
Next hxi
hx$ = hxt$
End Function
