VERSION 5.00
Begin VB.Form frmPic1 
   AutoRedraw      =   -1  'True
   Caption         =   "BMP"
   ClientHeight    =   6810
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10860
   LinkTopic       =   "Form1"
   ScaleHeight     =   454
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   724
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picTrack1 
      AutoSize        =   -1  'True
      Height          =   2415
      Left            =   480
      ScaleHeight     =   2355
      ScaleWidth      =   3675
      TabIndex        =   0
      Top             =   120
      Width           =   3735
   End
   Begin VB.Label lblPic2 
      Alignment       =   1  'Right Justify
      Caption         =   "mjm 01-25-03"
      Height          =   255
      Left            =   7800
      TabIndex        =   1
      Top             =   6480
      Width           =   2895
   End
   Begin VB.Image Image1 
      Height          =   2775
      Left            =   480
      Top             =   2880
      Width           =   3735
   End
End
Attribute VB_Name = "frmPic1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
With frmPic1
.picTrack1.ScaleMode = vbPixels
.picTrack1.AutoRedraw = True
.Hide
End With
End Sub


