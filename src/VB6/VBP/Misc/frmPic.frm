VERSION 5.00
Begin VB.Form frmPic 
   BackColor       =   &H00000000&
   BorderStyle     =   0  'None
   Caption         =   "Form2"
   ClientHeight    =   4470
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6765
   LinkTopic       =   "Form2"
   ScaleHeight     =   4470
   ScaleWidth      =   6765
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Label lblMJ 
      BackColor       =   &H00000000&
      Caption         =   " Copyright MJ Mahoney "
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   5040
      TabIndex        =   1
      Top             =   4200
      Width           =   1695
   End
   Begin VB.Label lblDescription 
      BackColor       =   &H00000000&
      Caption         =   "Description"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   4200
      Width           =   1215
   End
   Begin VB.Image Image2 
      Height          =   4215
      Left            =   120
      Top             =   120
      Width           =   6375
   End
End
Attribute VB_Name = "frmPic"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub Picture1_Click()
  
If frmMJMviewer.Visible Then
  frmMJMviewer.Hide
Else
  frmMJMviewer.Show
End If
DoEvents
End Sub


Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Select Case KeyCode
Case 38  'Up Arrow
  frmMJMviewer.filList.ListIndex = frmMJMviewer.filList.ListIndex - 1
Case 40  'Down Arrow
  frmMJMviewer.filList.ListIndex = frmMJMviewer.filList.ListIndex + 1
End Select

End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
   If KeyAscii = 27 Then   'Space or Escape
     If frmMJMviewer.Visible Then
       frmMJMviewer.Visible = False
     Else
       frmMJMviewer.Visible = True
     End If
   End If
   DoEvents
End Sub

Private Sub Form_Load()
frmPic.KeyPreview = True
End Sub

Private Sub Image2_Click()
If frmMJMviewer.Visible Then
  frmMJMviewer.Hide
Else
  frmMJMviewer.Show
End If
End Sub


