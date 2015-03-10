VERSION 5.00
Begin VB.Form frmFltTrack 
   BorderStyle     =   0  'None
   Caption         =   "Form2"
   ClientHeight    =   3195
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4755
   LinkTopic       =   "Form2"
   ScaleHeight     =   3195
   ScaleWidth      =   4755
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "frmFltTrack"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Sub Form_Click()
  Me.Visible = False
  Unload Me
End Sub

Private Sub picFltTrack_Click()

End Sub


Private Sub Form_DblClick()

  CaptureFormMJ Me, "", 3, True
  
End Sub


