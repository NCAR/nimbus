VERSION 5.00
Begin VB.Form frmFIreset 
   Caption         =   "FLTINFO Reset"
   ClientHeight    =   1050
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   2430
   LinkTopic       =   "Form1"
   ScaleHeight     =   1050
   ScaleWidth      =   2430
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   420
      Left            =   1665
      TabIndex        =   2
      Top             =   585
      Width           =   720
   End
   Begin VB.CommandButton cmdReset 
      Caption         =   "Reset Level 2"
      Height          =   450
      Index           =   1
      Left            =   45
      TabIndex        =   1
      ToolTipText     =   "Copies FLTINFO.REF32SAV and FLTINFO.REF33SAV to default FLINFO.REF and REF2"
      Top             =   555
      Width           =   1455
   End
   Begin VB.CommandButton cmdReset 
      Caption         =   "Reset Level 1"
      Height          =   450
      Index           =   0
      Left            =   45
      TabIndex        =   0
      ToolTipText     =   "Copies FLTINFO.REF32 and FLTINFO.REF33 to default FLINFO.REF and REF2"
      Top             =   60
      Width           =   1455
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileExit 
         Caption         =   "&Exit"
      End
   End
End
Attribute VB_Name = "frmFIreset"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub cmdExit_Click()

  Unload Me
  End
  
End Sub

Private Sub cmdReset_Click(Index As Integer)

  Kill "C:\MTP\Setup\FLTINFO.REF"
  Kill "C:\MTP\Setup\FLTINFO.REF2"
  
  Select Case Index
  Case 0  'Use FLTINFO.REF32
    FileCopy "C:\MTP\Setup\FLTINFO.REF32", "C:\MTP\Setup\FLTINFO.REF"
    FileCopy "C:\MTP\Setup\FLTINFO.REF33", "C:\MTP\Setup\FLTINFO.REF2"
  Case 1  'Use FLTINFO.REF32SAV
    FileCopy "C:\MTP\Setup\FLTINFO.REF32SAV", "C:\MTP\Setup\FLTINFO.REF"
    FileCopy "C:\MTP\Setup\FLTINFO.REF33SAV", "C:\MTP\Setup\FLTINFO.REF2"
  End Select

End Sub




Private Sub Form_Load()

With frmFIreset
.Top = (Screen.Height - .Height) \ 2
.Left = (Screen.Width - .Width) \ 2
End With

End Sub

Private Sub mnuFileExit_Click()
  cmdExit_Click
End Sub


