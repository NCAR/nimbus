VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Form1"
   ClientHeight    =   3930
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5325
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   262
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   355
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      Height          =   1245
      Left            =   90
      ScaleHeight     =   1185
      ScaleWidth      =   1125
      TabIndex        =   2
      Top             =   60
      Width           =   1185
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   375
      Left            =   4320
      TabIndex        =   0
      Top             =   3360
      Width           =   945
   End
   Begin VB.Label Label1 
      Caption         =   "Pressure Altitude"
      Height          =   225
      Left            =   2190
      TabIndex        =   1
      Top             =   1770
      Width           =   1845
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Type LOGFONT
  lfHeight As Long
  lfWidth As Long
  lfEscapement As Long
  lfOrientation As Long
  lfWeight As Long
  lfItalic As Byte
  lfUnderline As Byte
  lfStrikeOut As Byte
  lfCharSet As Byte
  lfOutPrecision As Byte
  lfClipPrecision As Byte
  lfQuality As Byte
  lfPitchAndFamily As Byte
' lfFaceName(LF_FACESIZE) As Byte 'THIS WAS DEFINED IN API-CHANGES MY OWN
  lfFacename As String * 33
End Type

Private Declare Function CreateFontIndirect Lib "GDI32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long
Private Declare Function SelectObject Lib "GDI32" (ByVal hDC As Long, ByVal hObject As Long) As Long
Private Declare Function DeleteObject Lib "GDI32" (ByVal hObject As Long) As Long



Private Function fRotateText(ByVal theText As String, ByVal theFontname As String, ByVal theSize As Integer, ByVal theAngle As Single, ByVal theX As Long, ByVal theY As Long) As Boolean

'Name:      fText90D
'Author:    Dalin Nie
'Date:      2/18/98
'Purpose:   Print a 90 degree text in the form
'Pass In:   TheText-- the text you want to print
'           theSize:   Size of the font
'           theAngle:  How many degree's you want to rotate
'           theX, theY, Where you wnat your text
'Return:    Boolean


  On Error GoTo Trap
  Dim f As LOGFONT, hPrevFont As Long, hFont As Long
  Dim FONTSIZE As Integer
  FONTSIZE = theSize

  f.lfEscapement = 10 * theAngle 'rotation angle, in tenths
  'FontName = "Arial" + Chr$(0) 'null terminated
  f.lfFacename = theFontname
  f.lfHeight = (theSize * -20) / Screen.TwipsPerPixelY
  hFont = CreateFontIndirect(f)
  hPrevFont = SelectObject(Me.hDC, hFont)
  CurrentX = theX
  CurrentY = theY
  Print theText
  
'  Clean up, restore original font
  hFont = SelectObject(Me.hDC, hPrevFont)
  DeleteObject hFont
  fRotateText = True
  
  Exit Function

Trap:

End Function


Private Sub Command1_Click()
Dim i%, rotation!, FontName As String
  
  FontName = "Arial" + Chr$(0)  'must be null terminated
  For i = 0 To 360 Step 10
    rotation = i * 1#
    Call fRotateText(Label1.Caption, FontName, 10, rotation, Label1.Left, Label1.Top + Label1.Height)
  Next i

End Sub

Private Sub Form_Load()
    Form1.Show
End Sub


