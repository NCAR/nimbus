VERSION 5.00
Begin VB.Form frmMap 
   AutoRedraw      =   -1  'True
   Caption         =   "Map of"
   ClientHeight    =   3510
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4815
   Icon            =   "frmMap.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3510
   ScaleWidth      =   4815
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picMap 
      AutoSize        =   -1  'True
      Height          =   3255
      Left            =   0
      ScaleHeight     =   3195
      ScaleWidth      =   4635
      TabIndex        =   0
      Top             =   0
      Width           =   4695
   End
End
Attribute VB_Name = "frmMap"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub picMap_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub


